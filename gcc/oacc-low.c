/* Lowering and expanding passes of OpenACC.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "gimple.h"
#include "gimple-oacc.h"
#include "tree-inline.h"
#include "diagnostic-core.h"
#include "tree-cfg.h"
#include "tree-ssa.h"
#include "expr.h"
#include "tree-pass.h"
#include "splay-tree.h"
#include "cfgloop.h"
#include "gimple-pretty-print.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "plugin.h"
#include "function.h"
#include "stringpool.h"
#include "tree-ssa-alias.h"   
#include "tree-ssanames.h"
#include "gimple.h"
#include "gimple-ssa.h"
#include "bitmap.h"
#include "tree-ssa-live.h"
#include "tree-outof-ssa.h"
#include "tree-into-ssa.h"
#include "gimple-opencl.h"
#include "diagnose-gotos.h"
#include "input.h"
#include "gtype-desc.h"
#include "options.h"
#include "pointer-set.h"
#include "gimple-iterator.h"
#include "cgraph.h"
#include "gimple-builder.h"
#include "gimple-walk.h"
#include "attribs.h"
#include "gimplify.h"
#include "tree-ssa-loop.h"
#include "tree-ssa-loop-ivopts.h"   
#include "tree-ssa-loop-niter.h"
#include "tree-ssa-loop-manip.h"
#include "tree-cfgcleanup.h"
#include "tree-phinodes.h"

#define OACC_PF_DATAIN  1
#define OACC_PF_EXEC    2
#define OACC_PF_DATAOUT 3

#define OACC_CF_NONE    0
#define OACC_CF_COPY    1
#define OACC_CF_CREATE  2

/* A transformation matrix, which is a self-contained ROWSIZE x COLSIZE
   matrix.  Rather than use floats, we simply keep a single DENOMINATOR that
   represents the denominator for every element in the matrix.  */
typedef struct lambda_trans_matrix_s
{
    lambda_matrix matrix;
    int rowsize;
    int colsize;
    int denominator;
} *lambda_trans_matrix;

#define LTM_MATRIX(T) ((T)->matrix)
#define LTM_ROWSIZE(T) ((T)->rowsize)
#define LTM_COLSIZE(T) ((T)->colsize)
#define LTM_DENOMINATOR(T) ((T)->denominator)

/* Allocate a new transformation matrix.  */

static lambda_trans_matrix
lambda_trans_matrix_new (int colsize, int rowsize,
                         struct obstack *lambda_obstack)
{
    lambda_trans_matrix ret;

    ret = (lambda_trans_matrix)
          obstack_alloc (lambda_obstack,
                         sizeof (struct lambda_trans_matrix_s));
    LTM_MATRIX (ret) = lambda_matrix_new (rowsize,
                                          colsize,
                                          lambda_obstack);
    LTM_ROWSIZE (ret) = rowsize;
    LTM_COLSIZE (ret) = colsize;
    LTM_DENOMINATOR (ret) = 1;
    return ret;
}
/* Multiply a vector VEC by a matrix MAT.
   MAT is an M*N matrix, and VEC is a vector with length N.  The result
   is stored in DEST which must be a vector of length M.  */

static void
lambda_matrix_vector_mult (lambda_matrix matrix, int m, int n,
                           lambda_vector vec, lambda_vector dest)
{
    int i, j;

    lambda_vector_clear (dest, m);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            dest[i] += matrix[i][j] * vec[j];
}

/* Return true if TRANS is a legal transformation matrix that respects
   the dependence vectors in DISTS and DIRS.  The conservative answer
   is false.

   "Wolfe proves that a unimodular transformation represented by the
   matrix T is legal when applied to a loop nest with a set of
   lexicographically non-negative distance vectors RDG if and only if
   for each vector d in RDG, (T.d >= 0) is lexicographically positive.
   i.e.: if and only if it transforms the lexicographically positive
   distance vectors to lexicographically positive vectors.  Note that
   a unimodular matrix must transform the zero vector (and only it) to
   the zero vector." S.Muchnick.  */

static bool
lambda_transform_legal_p (lambda_trans_matrix trans,
                          int nb_loops,
                          vec<ddr_p> dependence_relations)
{
    unsigned int i, j;
    lambda_vector distres;
    struct data_dependence_relation *ddr;

    gcc_checking_assert (LTM_COLSIZE (trans) == nb_loops
                && LTM_ROWSIZE (trans) == nb_loops);

    /* When there are no dependences, the transformation is correct.  */
    if (dependence_relations.length () == 0)
        return true;

    ddr = dependence_relations[0];
    if (ddr == NULL)
        return true;

    /* When there is an unknown relation in the dependence_relations, we
       know that it is no worth looking at this loop nest: give up.  */
    if (DDR_ARE_DEPENDENT (ddr) == chrec_dont_know)
        return false;

    distres = lambda_vector_new (nb_loops);

    /* For each distance vector in the dependence graph.  */
    FOR_EACH_VEC_ELT (dependence_relations, i, ddr)
    {
        /* Don't care about relations for which we know that there is no
           dependence, nor about read-read (aka. output-dependences):
           these data accesses can happen in any order.  */
        if (DDR_ARE_DEPENDENT (ddr) == chrec_known
                || (DR_IS_READ (DDR_A (ddr)) && DR_IS_READ (DDR_B (ddr))))
            continue;

        /* Conservatively answer: "this transformation is not valid".  */
        if (DDR_ARE_DEPENDENT (ddr) == chrec_dont_know)
            return false;

        /* If the dependence could not be captured by a distance vector,
           conservatively answer that the transform is not valid.  */
        if (DDR_NUM_DIST_VECTS (ddr) == 0)
            return false;

        /* Compute trans.dist_vect */
        for (j = 0; j < DDR_NUM_DIST_VECTS (ddr); j++)
            {
                lambda_matrix_vector_mult (LTM_MATRIX (trans), nb_loops, nb_loops,
                                           DDR_DIST_VECT (ddr, j), distres);

                if (!lambda_vector_lexico_pos (distres, nb_loops))
                    return false;
            }
    }
    return true;
}

/* name of .cl file to generate, based on source file name*/
static GTY(()) char *ocl_module;

/* OpenACC region tree node */
typedef struct oacc_region_t* oacc_region;

typedef struct oacc_region_t
{
    gimple stmt, end_stmt;
    oacc_region parent;
    vec <oacc_region>* children;
} oacc_region_t;

/* kernel schedule parameters */
typedef struct oacc_schedule_t* oacc_schedule;

typedef struct oacc_schedule_t
{
  tree items;       /* total work items */
  tree tiling;      /* how to tile loop */
  tree group_size;  /* workgroup size */
} oacc_schedule_t;

/* kernel function generation helper */
typedef struct oacc_kernel_t *oacc_kernel;
typedef struct oacc_kernel_t
{
  tree func;
  vec<tree> niters;
  basic_block bb_entry;
  basic_block bb_exit;
  splay_tree params_map;
  tree kernel_handle;
  unsigned collapse;
  vec<tree> clauses;
  bool no_check, no_parallel;
} oacc_kernel_t;

/* loop collapsing data */
typedef struct collapse_loop_data_t *collapse_loop_data;
typedef struct collapse_loop_data_t
{
  tree niter;
  gimple_stmt_iterator gsi;
} collapse_loop_data_t;

/* kernel collapsing data */
typedef struct collapse_data_t *collapse_data;
typedef struct collapse_data_t
{
  vec<collapse_loop_data> loops;
  tree workitem_id;
  gimple wi_def_stmt;
} collapse_data_t;

/* data item to copy */
typedef struct copy_data_item_t
{
  tree datum;
  unsigned to_do;
  bool check_presence, is_arg;
  copy_data_item_t() : datum(NULL_TREE),
      to_do(OACC_CF_NONE), check_presence(false), is_arg(false)
  {}
} copy_data_item_t;

typedef struct copyin_data_t *copyin_data;
typedef struct copyin_data_t
{
  vec<copy_data_item_t> data;
  unsigned event_count;
  copyin_data_t() : event_count(0)
  {}
} copyin_data_t;

typedef struct copyout_data_t *copyout_data;
typedef struct copyout_data_t
{
  vec<copy_data_item_t> data;
} copyout_data_t;

/* lowering context */
typedef struct oacc_context
{
    /* This field must be at the beginning, as we do "inheritance": Some
       callback functions for tree-inline.c (e.g., omp_copy_decl)
       receive a copy_body_data pointer that is up-casted to an
       omp_context pointer.  */
    copy_body_data cb;

    /* The tree of contexts corresponding to the encountered constructs.  */
    struct oacc_context *outer;
    gimple stmt;

    splay_tree param_map;
    splay_tree local_map;
    /* Nesting depth of this context.  Used to beautify error messages re
       invalid gotos.  The outermost ctx is depth 1, with depth 0 being
       reserved for the main body of the function.  */
    int depth;

    /* True if this parallel directive is nested within another.  */
    bool is_nested;
} oacc_context;

static splay_tree all_contexts;
static int nesting_level;
static oacc_region root_region;
static int save_opt_level = -1;
static GTY((param1_is(unsigned), param2_is(unsigned))) splay_tree kernels;

static void analyze_gimple(gimple_seq* pseq, oacc_context* ctx);
static void lower_oacc(gimple_seq* pseq, oacc_context* ctx);

static collapse_loop_data
new_collapse_loop_data(tree niter)
{
  collapse_loop_data data;

  data = XCNEW(collapse_loop_data_t);
  data->niter = niter;
  return data;
}

static void
delete_collapse_loop_data(collapse_loop_data data)
{
  XDELETE(data);
}

static oacc_region
new_oacc_region(gimple stmt, oacc_region parent)
{
    oacc_region region;

    region = XCNEW(oacc_region_t);
    region->stmt = stmt;
    region->end_stmt = NULL;
    region->parent = parent;
    vec_alloc(region->children, 3);
    if(parent != NULL)
    {
      parent->children->safe_push(region);
    }

    return region;
}

static void
delete_oacc_region(oacc_region region)
{
  unsigned i;

  for(i = 0; i < region->children->length(); ++i)
    {
      delete_oacc_region((*region->children)[i]);
    }
  vec_free(region->children);
  XDELETE(region);
}

DEBUG_FUNCTION static void
dump_oacc_region (FILE* file, oacc_region region, size_t spc)
{
    size_t i;

    for(i = 0; i < spc; ++i)
      {
        fprintf(file, " ");
      }

    if(region->stmt != NULL)
      {
        print_gimple_stmt(file, region->stmt, 0, TDF_SLIM);
      }
    else
      {
        fprintf(file, "(NULL)\n");
      }
    for(i = 0; i < region->children->length(); ++i)
      {
        dump_oacc_region(file, (*region->children)[i], spc+4);
      }

}

static oacc_kernel
new_oacc_kernel(tree func)
{
  oacc_kernel kernel;
  
  kernel = XCNEW(oacc_kernel_t);
  kernel->func = func;
  kernel->niters.create(1);
  kernel->collapse = 0;
  kernel->bb_entry = kernel->bb_exit = NULL;
  kernel->params_map = splay_tree_new(splay_tree_compare_pointers, 0, 0);
  kernel->kernel_handle = NULL_TREE;
  kernel->clauses.create(1);
  kernel->no_check = false;
  kernel->no_parallel = false;
  return kernel;
}

static void
delete_oacc_kernel(oacc_kernel kernel)
{
  splay_tree_delete(kernel->params_map);
  XDELETE(kernel);
}


static oacc_context*
new_oacc_context(gimple stmt, oacc_context* outer)
{
    oacc_context *ctx = XCNEW(oacc_context);
    splay_tree_insert(all_contexts, (splay_tree_key)stmt, (splay_tree_value)ctx);
    ctx->stmt = stmt;

    if(outer)
        {
            ctx->outer = outer;
            ctx->cb = outer->cb;
            ctx->cb.block = NULL;
            ctx->depth = outer->depth+1;
        }
    else
        {
            ctx->cb.src_fn = current_function_decl;
            ctx->cb.dst_fn = current_function_decl;
            ctx->cb.src_node = cgraph_get_node (current_function_decl);
            gcc_checking_assert (ctx->cb.src_node);
            ctx->cb.dst_node = ctx->cb.src_node;
            ctx->cb.src_cfun = cfun;
            ctx->cb.eh_lp_nr = 0;
            ctx->cb.transform_call_graph_edges = CB_CGE_MOVE;
            ctx->depth = 1;
        }
    ctx->cb.decl_map = pointer_map_create();
    return ctx;
}

static void
delete_oacc_context(splay_tree_value value)
{
    oacc_context *ctx = (oacc_context *) value;

    if(ctx->param_map)
        splay_tree_delete(ctx->param_map);
    if(ctx->local_map)
        splay_tree_delete(ctx->local_map);
    XDELETE(ctx);
}

static int
add_local_to_bind(splay_tree_node n, void* data)
{
    gimple stmt = (gimple)data;
    gimple_bind_append_vars(stmt, (tree)n->value);
    return 0;
}

static void
add_locals_to_bind(gimple bind, oacc_context* ctx)
{
    splay_tree_foreach(ctx->local_map, add_local_to_bind, (void*)bind);
}

/* add kernel's locals to new function scope*/
static void
add_locals(gimple_seq* pbody, oacc_context* ctx)
{
    if(gimple_code(gimple_seq_first(*pbody)) == GIMPLE_BIND)
        {
            add_locals_to_bind(gimple_seq_first(*pbody), ctx);
        }
}

static int
create_call_params(splay_tree_node node, void* data)
{
    vec<tree>* pargs = (vec<tree>*)data;
    if((tree)node->value != NULL_TREE)
        {
            pargs->quick_push((tree)node->key);
        }
    return 0;
}

static tree
get_if_clause_expr (gimple_stmt_iterator *gsi)
{
  gimple gs = gsi_stmt (*gsi);
  tree clause;

  if (gimple_code (gs) == GIMPLE_OACC_KERNELS)
    clause = gimple_oacc_kernels_clauses (gs);
  else if (gimple_code (gs) == GIMPLE_OACC_PARALLEL)
    clause = gimple_oacc_parallel_clauses (gs);
  else
    gcc_unreachable ();

  while (1)
    {
      if (clause == NULL)
        return NULL_TREE;
      if (OACC_CLAUSE_CODE (clause) == OACC_CLAUSE_IF)
        return OACC_CLAUSE_IF_EXPR (clause);
      clause = OACC_CLAUSE_CHAIN (clause);
    }
}
/* insert host version of kernel's body after statement pointed by GSI */
static void
add_host_version(gimple_stmt_iterator *gsi, gimple_seq orig)
{
    if(gimple_code(gimple_seq_first(orig)) == GIMPLE_BIND)
      {
        orig = gimple_bind_body(orig);
      }
    gsi_insert_seq_after(gsi, orig, GSI_CONTINUE_LINKING);
}

/* When there is assignment like D.1988 = a.1 + D.1987 where a.1 id pointer
 * we have to add one assignment before the statement:
 * D.1988 = a.1;
 */
static void
add_assingments_to_ptrs (gimple_seq *seq, oacc_context* ctx)
{
  gimple_stmt_iterator gsi;

  for (gsi = gsi_start (*seq); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      gimple stmt = gsi_stmt(gsi);
      gimple_seq inner_seq;
      gimple_stmt_iterator inner_gsi;

      gcc_checking_assert (gimple_code(stmt) == GIMPLE_BIND);
      inner_seq = gimple_bind_body (stmt);
      for (inner_gsi = gsi_start (inner_seq); !gsi_end_p (inner_gsi);
           gsi_next (&inner_gsi))
        {
          gimple inner_stmt = gsi_stmt(inner_gsi);
          if (is_gimple_assign (inner_stmt))
            {
              tree rhs = gimple_assign_rhs1 (inner_stmt);
              if (POINTER_TYPE_P(TREE_TYPE(rhs)))
                {
                  tree lhs = gimple_assign_lhs (inner_stmt);
                  tree op = gimple_assign_rhs2 (inner_stmt);

                  /* _oacc_ptr_tmp = a */
                  tree convert_var =
                      (tree) splay_tree_lookup(ctx->local_map,
                                               (splay_tree_key) rhs)->value;
                  gcc_checking_assert (convert_var);
                  gimple convert_stmt = gimple_build_assign (convert_var, rhs);
                  gsi_insert_before (&inner_gsi, convert_stmt, GSI_SAME_STMT);

                  /* D.1988 = _oacc_ptr_tmp + offset */
                  gimple assign_stmt = build_assign (
                      (enum tree_code)inner_stmt->subcode,
                      convert_var, op, lhs);
                  gsi_replace (&inner_gsi, assign_stmt, true);
                }
            }
        }
    }
}

/* lower kernel or parallel body */
static void
lower_oacc_kernels(gimple_stmt_iterator *gsi, oacc_context* ctx)
{
    struct function *child_cfun;
    tree child_fn, if_clause_expr;
    gimple stmt = gsi_stmt (*gsi);
    gimple_seq body, orig;
    unsigned i;
    
    orig = gimple_oacc_body(stmt);
    if(dump_file)
        {
            fprintf(dump_file, "Original KERNELS body:\n");
            print_gimple_seq(dump_file, orig, 0, 0);
            fprintf(dump_file, "\n");
        }
    /* first recursively lower body */
    lower_oacc(&orig, ctx);
    if(dump_file)
        {
            fprintf(dump_file, "After lowering in kernels:\n");
            print_gimple_seq(dump_file, orig, 0, 0);
            fprintf(dump_file, "\n");
        }
    body = gimple_seq_copy(orig);
    add_assingments_to_ptrs(&body, ctx);
    child_fn = GIMPLE_OACC_CHILD_FN(stmt);

    /* add locals to function body*/
    add_locals(&body, ctx);
    gimple_set_body(child_fn, body);
    if(dump_file)
        {
            fprintf(dump_file, "In child fn:\n");
            print_gimple_seq(dump_file, gimple_body(child_fn), 0, 0);
            fprintf(dump_file, "\n");
        }

    /* add new child function to call graph manager */
    child_cfun = DECL_STRUCT_FUNCTION (child_fn);
    child_cfun->curr_properties = cfun->curr_properties;
    cgraph_add_new_function (child_fn, false);

    /* resize gimple statement to make room for kernel parameters */
    vec<tree> args;
    args.create(16);
    splay_tree_foreach(ctx->param_map, create_call_params, (void*)&args);
    size_t size;
    if (args.length() != 0)
        size = sizeof(struct gimple_statement_oacc_kernels)
               + (args.length() - 1) * sizeof(tree);
    else
        size = sizeof(struct gimple_statement_oacc_kernels);
    gimple new_stmt = ggc_alloc_cleared_gimple_statement_stat (size);
    memcpy(new_stmt, stmt, sizeof(struct gimple_statement_oacc_kernels));

    /* set statement parameters */
    gimple_set_num_ops(new_stmt, args.length());
    for(i = 0; i < args.length(); ++i)
        {
            tree arg = args[i];
            if(dump_file)
                {
                    fprintf(dump_file, "argument ");
                    print_generic_expr(dump_file, arg, 0);
                    fprintf(dump_file, " type ");
                    print_generic_expr(dump_file, TREE_TYPE(arg), 0);
                    fprintf(dump_file, "\n");
                }
            gimple_set_op(new_stmt, i, arg);
        }

    gsi_replace (gsi, new_stmt, true);
    /* add conditional host version */
    if_clause_expr = get_if_clause_expr (gsi);
    if (if_clause_expr)
      {
        location_t locus = gimple_location (stmt);
        tree true_label = create_artificial_label (locus);
        tree false_label = create_artificial_label (locus);
        tree exit_label = create_artificial_label (locus);

        /* if (D.1859) goto <D.1900>; else goto <D.1901>; */
        gimple cond = gimple_build_cond_from_tree (if_clause_expr,
                                                   true_label, false_label);
        gsi_insert_before (gsi, cond, GSI_SAME_STMT);
        /* <D.1900>: */
        gsi_insert_before (gsi, gimple_build_label (true_label), GSI_SAME_STMT);
        /* oacc_compute_region_end */
        gsi_insert_after (gsi, gimple_alloc(GIMPLE_OACC_COMPUTE_REGION_END, 0),
                          GSI_CONTINUE_LINKING);
        /* goto <D.1902>; */
        gsi_insert_after (gsi, gimple_build_goto (exit_label),
                          GSI_CONTINUE_LINKING);
        /* <D.1901>; */
        gsi_insert_after (gsi, gimple_build_label (false_label),
                          GSI_CONTINUE_LINKING);

        add_host_version (gsi, orig);

        /* <D.1902>; */
        gsi_insert_after (gsi, gimple_build_label (exit_label),
                          GSI_CONTINUE_LINKING);
      }
    else
      gsi_insert_after(gsi, gimple_alloc(GIMPLE_OACC_COMPUTE_REGION_END, 0),
                       GSI_CONTINUE_LINKING);

    gimple_oacc_set_body(new_stmt, NULL);
}

/* lower data directive body */
static void
lower_oacc_data(gimple_stmt_iterator *gsi, oacc_context* ctx)
{
  gimple_seq body;
  gimple stmt = gsi_stmt (*gsi);
   
  body = gimple_oacc_body(stmt);
  lower_oacc(&body, ctx);
  add_host_version(gsi, body);
  gsi_insert_after(gsi, gimple_alloc(GIMPLE_OACC_DATA_REGION_END, 0),
                 GSI_CONTINUE_LINKING);
  gimple_oacc_set_body(stmt, NULL);
}

static void
lower_oacc_host_data(gimple_stmt_iterator *gsi, oacc_context *ctx)
{
  gimple_seq body;
  gimple stmt = gsi_stmt (*gsi);
   
  body = gimple_oacc_body(stmt);
  lower_oacc(&body, ctx);
  add_host_version(gsi, body);
  gsi_insert_after(gsi, gimple_alloc(GIMPLE_OACC_DATA_REGION_END, 0),
                 GSI_CONTINUE_LINKING);
  gimple_oacc_set_body(stmt, NULL);
}

/* lower loop directive */
static void
lower_oacc_loop(gimple_stmt_iterator *gsi, oacc_context* ctx)
{
  gimple_seq body;
  gimple stmt = gsi_stmt (*gsi);
  gimple_seq outer_body;
  gimple outer_bind;
  
  gcc_checking_assert(ctx != 0 && ctx->stmt != 0 && is_gimple_oacc(ctx->stmt));
  outer_body = gimple_oacc_body(ctx->stmt);
  outer_bind = gimple_seq_first(outer_body);
  
  body = gimple_oacc_body(stmt);
    if(dump_file)
        {
            fprintf(dump_file, "Before lowering in loop:\n");
            print_gimple_seq(dump_file, body, 0, 0);
            fprintf(dump_file, "\n");
        }
  lower_oacc(&body, ctx);
    if(dump_file)
        {
            fprintf(dump_file, "After lowering in loop:\n");
            print_gimple_seq(dump_file, body, 0, 0);
            fprintf(dump_file, "\n");
        }
  gimple_bind_append_vars(outer_bind, gimple_bind_vars(gimple_seq_first(body)));
  add_host_version(gsi, body);
  gimple_oacc_set_body(stmt, NULL);
}

/* lowering pass statement callback 
   dispatch call to appropriate handler */
static tree
lower_stmt_cb(gimple_stmt_iterator *gsi, bool *handled_ops_p,
              struct walk_stmt_info *wi)
{
    gimple stmt = gsi_stmt (*gsi);
    oacc_context *ctx = (oacc_context *) wi->info;

    *handled_ops_p = true;

    switch (gimple_code (stmt))
        {
        case GIMPLE_OACC_KERNELS:
        case GIMPLE_OACC_PARALLEL:
            ctx = (oacc_context*)splay_tree_lookup(all_contexts,
                                                   (splay_tree_key)stmt)->value;
            lower_oacc_kernels (gsi, ctx);
            break;
        case GIMPLE_OACC_DATA:
            ctx = (oacc_context*)splay_tree_lookup(all_contexts,
                                                   (splay_tree_key)stmt)->value;
            lower_oacc_data (gsi, ctx);
            break;
        case GIMPLE_OACC_HOST_DATA:
            ctx = (oacc_context*)splay_tree_lookup(all_contexts,
                                                   (splay_tree_key)stmt)->value;
            lower_oacc_host_data (gsi, ctx);
            break;
        case GIMPLE_OACC_LOOP:
            lower_oacc_loop (gsi, ctx);
            break;
        case GIMPLE_OACC_CACHE:
        case GIMPLE_OACC_WAIT:
        case GIMPLE_OACC_DECLARE:
        case GIMPLE_OACC_UPDATE:
            /* nothing to do in lowering */
            break;

        default:
            *handled_ops_p = false;
            break;
        }
    return NULL_TREE;
}

/* lowering pass operand callback
   gather kernel's locals */
static tree
lower_op_cb(tree *tp, int *walk_subtrees, void *data)
{
    struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
    oacc_context* ctx = (oacc_context*)wi->info;
    tree t = *tp;

    if(ctx != NULL && ctx->param_map != NULL && is_gimple_variable(t))
        {
            tree t1 = NULL_TREE;
            splay_tree_node v = splay_tree_lookup(ctx->param_map,
                                                (splay_tree_key)t);
            if(v)
                {
                    t1 = (tree)v->value;
                    if(t1 == NULL_TREE)
                        {
                            v = splay_tree_lookup(ctx->local_map,
                                                (splay_tree_key)t);
                            t1 = (tree)v->value;
                        }
                    *tp = t1;
                    if(dump_file)
                        {
                            fprintf(dump_file, "Subst ");
                            print_generic_expr(dump_file, t, 0);
                            fprintf(dump_file, " with ");
                            print_generic_expr(dump_file, t1, 0);
                            fprintf(dump_file, "\n");
                        }
                }
        }
    else if(ctx != 0 && TREE_CODE(t) == LABEL_DECL)
        {
            DECL_CONTEXT(t) = ctx->cb.dst_fn;
        }
    return NULL_TREE;
}

/* lowering subpass */
static void
lower_oacc(gimple_seq* pseq, oacc_context* ctx)
{
    struct walk_stmt_info wi;

    memset (&wi, 0, sizeof (wi));
    wi.info = ctx;

    walk_gimple_seq_mod (pseq, lower_stmt_cb, lower_op_cb, &wi);
}

/* gather kernel's child function arguments */
static int
gather_oacc_fn_args(splay_tree_node node, void* data)
{
    vec<tree>* pargs = (vec<tree>*)data;
    tree arg = (tree)node->key;
    if(!is_gimple_reg(arg) || POINTER_TYPE_P(TREE_TYPE(arg)))
      pargs->quick_push(arg);
    else
      node->value = (splay_tree_value)NULL_TREE;
    return 0;
}

/* make artifical name valid C identifier */
static tree
normalize_name(tree name)
{
    char *normal, *pn;
    const char *ptr;

    normal = XALLOCAVEC(char, IDENTIFIER_LENGTH(name) + 1);
    pn = normal;
    for(ptr = IDENTIFIER_POINTER(name); *ptr != '\0'; ++ptr)
        {
            if(*ptr == '.' || *ptr == '$')
                {
                    *pn++ = '_';
                }
            else
                {
                    *pn++ = *ptr;
                }
        }
    *pn = '\0';
    return get_identifier(normal);
}

static void
generate_local_reg (splay_tree *local_map, tree arg)
{
    const char *id = "_oacc_param";
    if(DECL_NAME(arg))
      id = IDENTIFIER_POINTER(DECL_NAME(arg));
    tree t1 = create_tmp_reg(TREE_TYPE(arg), id);
    splay_tree_insert(*local_map, (splay_tree_key)arg,
                                  (splay_tree_value)t1);
    if(dump_file)
        {
            fprintf(dump_file, "Create local reg for: ");
            print_generic_expr(dump_file, arg, 0);
            fprintf(dump_file, " reg:");
            print_generic_expr(dump_file, t1, 0);
            fprintf(dump_file, "\n");
        }
}

/* gather child function locals */
static int
gather_oacc_fn_locals(splay_tree_node node, void* data)
{
    splay_tree* local_map = (splay_tree*)data;
    tree arg = (tree)node->key;
    if(is_gimple_reg(arg) && !POINTER_TYPE_P(TREE_TYPE(arg)))
        generate_local_reg (local_map, arg);
    /* We need to add value to the map, since then we use it in assignment. */
    if(POINTER_TYPE_P(TREE_TYPE(arg)))
        generate_local_reg (local_map, (tree)node->value);
    return 0;
}

static int
fix_decl_context(splay_tree_node node, void* data)
{
    tree decl = (tree)data;
    tree loc = (tree)node->value;
    DECL_CONTEXT(loc) = decl;
    return 0;
}

/* creates child function for compute region */
static void
create_oacc_child_function(oacc_context* ctx)
{
    tree name, type, decl, t;
    vec<tree> args, types;
    unsigned i;
    tree kernel_attr = NULL_TREE, global_attr = NULL_TREE;
    
    kernel_attr = get_opencl_attribute("*kernel");
    global_attr = get_opencl_attribute("*global");
    gcc_checking_assert(kernel_attr != NULL_TREE
                         && global_attr != NULL_TREE);
    
    /* make child function name clone of current */
    name = clone_function_name(current_function_decl, "_oacc_fn");
    name = normalize_name(name);
    if(dump_file)
        {
            fprintf(dump_file, "new fn name='%s'\n", IDENTIFIER_POINTER(name));
        }

    /* create kernel's child function */
    /* gather parameters type */
    args.create(16);
    splay_tree_foreach(ctx->param_map, gather_oacc_fn_args, (void*)&args);
    types.create(16);
    for(i = 0; i < args.length(); ++i)
        {
            tree type = TREE_TYPE(args[i]);
            if(dump_file)
                {
                    fprintf(dump_file, "Create parameter type: ");
                    print_generic_expr(dump_file, args[i], 0);
                    fprintf(dump_file, " type=");
                    print_generic_expr(dump_file, type, 0);
                    fprintf(dump_file, "\n");
                }
            types.quick_push(type);
        }
    /* create child function type signature */
    type = build_function_type_array(void_type_node, types.length(),
                                     types.address());
    /* build function declaration*/
    decl = build_decl(gimple_location(ctx->stmt), FUNCTION_DECL, name, type);
    ctx->cb.dst_fn = decl;
    
    decl_attributes(&decl, kernel_attr, 0);
    TREE_STATIC (decl) = 1;
    TREE_USED (decl) = 1;
    DECL_ARTIFICIAL (decl) = 1;
    DECL_NAMELESS (decl) = 1;
    DECL_IGNORED_P (decl) = 0;
    TREE_PUBLIC (decl) = 0;
    DECL_UNINLINABLE (decl) = 1;
    DECL_EXTERNAL (decl) = 0;
    DECL_CONTEXT (decl) = NULL_TREE;
    DECL_INITIAL (decl) = make_node (BLOCK);

    t = build_decl (DECL_SOURCE_LOCATION (decl),
                    RESULT_DECL, NULL_TREE, void_type_node);
    DECL_ARTIFICIAL (t) = 1;
    DECL_IGNORED_P (t) = 1;
    DECL_CONTEXT (t) = decl;
    DECL_RESULT (decl) = t;

    /* add function parameters (to local scope) */
    /* to properly connect DECL_CHAIN process them in reverse order */
    for(i = args.length(); i > 0; --i)
        {
            const char *id = "_oacc_param";
            
            if(DECL_NAME(args[i-1]))
                {
                    id = IDENTIFIER_POINTER(DECL_NAME(args[i-1]));
                }
            t = build_decl(DECL_SOURCE_LOCATION (decl), PARM_DECL,
                           create_tmp_var_name(id), types[i-1]);
          if(dump_file)
                {
                    fprintf(dump_file, "Create formal param: ");
                    print_generic_expr(dump_file, t, 0);
                    fprintf(dump_file, " type=");
                    print_generic_expr(dump_file, types[i-1], 0);
                    fprintf(dump_file, "\n");
                }

            DECL_ARTIFICIAL (t) = 1;
            DECL_NAMELESS (t) = 1;
            DECL_ARG_TYPE (t) = types[i-1];
            DECL_CONTEXT(t) = decl;
            TREE_USED (t) = 1;
            TREE_ADDRESSABLE (t) = 1;
            decl_attributes(&t, global_attr, 0);
            if(i > 0)
                DECL_CHAIN (t) = DECL_ARGUMENTS (decl);
            DECL_ARGUMENTS (decl) = t;
            splay_tree_lookup(ctx->param_map, (splay_tree_key)args[i-1])->value
                = (splay_tree_value)t;
        }

    splay_tree_foreach(ctx->param_map, gather_oacc_fn_locals,
                       (void*)&ctx->local_map);
    splay_tree_foreach(ctx->local_map, fix_decl_context, (void*)decl);

    /* Allocate memory for the function structure.  The call to
       allocate_struct_function clobbers CFUN, so we need to restore
       it afterward.  */
    push_struct_function (decl);
    cfun->function_end_locus = gimple_location (ctx->stmt);
    pop_cfun ();
}

/* scan kernels/parallel directive */
static void
scan_oacc_kernels (gimple_stmt_iterator *gsi, oacc_context *outer_ctx)
{
    tree child_fn;
    oacc_context* ctx;
    gimple stmt = gsi_stmt (*gsi);

    /* Ignore directives with empty bodies.  */
    if (empty_body_p (gimple_oacc_body (stmt)))
        {
            gsi_replace (gsi, gimple_build_nop (), false);
            return;
        }

    ctx = new_oacc_context (stmt, outer_ctx);
    if (nesting_level > 1)
        ctx->is_nested = true;
    ctx->param_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);
    ctx->local_map = splay_tree_new (splay_tree_compare_pointers, 0, 0);

    analyze_gimple(gimple_oacc_body_ptr(stmt), ctx);
    create_oacc_child_function(ctx);

    child_fn = ctx->cb.dst_fn;
    GIMPLE_OACC_SET_CHILD_FN(stmt, child_fn);
}

/* scan data directive */
static void
scan_oacc_data (gimple_stmt_iterator *gsi, oacc_context *outer_ctx)
{
    oacc_context* ctx;
    gimple stmt = gsi_stmt (*gsi);

    /* Ignore directives with empty bodies.  */
    if (empty_body_p (gimple_oacc_body (stmt)))
        {
            gsi_replace (gsi, gimple_build_nop (), false);
            return;
        }

    ctx = new_oacc_context (stmt, outer_ctx);
    if (nesting_level > 1)
        ctx->is_nested = true;

    analyze_gimple(gimple_oacc_body_ptr(stmt), ctx);
}

static void
scan_oacc_host_data (gimple_stmt_iterator *gsi, oacc_context *outer_ctx)
{
    oacc_context* ctx;
    gimple stmt = gsi_stmt (*gsi);

    /* Ignore directives with empty bodies.  */
    if (empty_body_p (gimple_oacc_body (stmt)))
        {
            gsi_replace (gsi, gimple_build_nop (), false);
            return;
        }

    ctx = new_oacc_context (stmt, outer_ctx);
    if (nesting_level > 1)
        ctx->is_nested = true;

    analyze_gimple(gimple_oacc_body_ptr(stmt), ctx);
}

/* scan loop directive */
static void
scan_oacc_loop (gimple_stmt_iterator *gsi, oacc_context *outer_ctx)
{
    oacc_context* ctx;
    gimple stmt = gsi_stmt (*gsi);

    /* Ignore directives with empty bodies.  */
    if (empty_body_p (gimple_oacc_body (stmt)))
        {
            gsi_replace (gsi, gimple_build_nop (), false);
            return;
        }

    analyze_gimple(gimple_oacc_body_ptr(stmt), outer_ctx);
}

/* check openACC nesting restrictions */
static bool
check_oacc_nesting_restrictions (gimple stmt, oacc_context *ctx)
{
    gimple outer_ctx = ctx->stmt;

    switch(gimple_code(stmt))
      {
      case GIMPLE_OACC_KERNELS:
      case GIMPLE_OACC_PARALLEL:
        if(gimple_code(outer_ctx) == GIMPLE_OACC_KERNELS ||
                gimple_code(outer_ctx) == GIMPLE_OACC_PARALLEL)
            {
              error("invalid nesting of OpenACC parallel/kernels regions");
              return false;
            }
        break;
      case GIMPLE_OACC_LOOP:
        if(gimple_code(outer_ctx) != GIMPLE_OACC_KERNELS &&
                gimple_code(outer_ctx) != GIMPLE_OACC_PARALLEL &&
                gimple_code(outer_ctx) != GIMPLE_OACC_LOOP)
            {
              error("OpenACC 'loop' directive is not nested "
                      "inside compute region");
              return false;
            }
        break;
      case GIMPLE_OACC_CACHE:
        if(gimple_code(outer_ctx) != GIMPLE_OACC_LOOP)
            {
              error("OpenACC 'cache'  directive is not nested "
                      "inside 'loop' directive");
              return false;
            }
      default:
          break;
      }
    return true;
}

/* analyze subpass statement callback */
static tree
analyze_stmt_cb(gimple_stmt_iterator *gsi, bool *handled_ops_p,
                struct walk_stmt_info *wi)
{
    gimple stmt = gsi_stmt (*gsi);
    oacc_context *ctx = (oacc_context *) wi->info;

    if (gimple_has_location (stmt))
        input_location = gimple_location (stmt);

    /* Check the OpenACC nesting restrictions.  */
    if (ctx != NULL)
        {
            bool remove = false;
            if (is_gimple_oacc (stmt))
                remove = !check_oacc_nesting_restrictions (stmt, ctx);
            if (remove)
                {
                    stmt = gimple_build_nop ();
                    gsi_replace (gsi, stmt, false);
                }
        }

    *handled_ops_p = true;

    switch (gimple_code (stmt))
        {
        case GIMPLE_OACC_KERNELS:
        case GIMPLE_OACC_PARALLEL:
            nesting_level++;
            scan_oacc_kernels (gsi, ctx);
            nesting_level--;
            break;
        case GIMPLE_OACC_LOOP:
            scan_oacc_loop (gsi, ctx);
            break;
        case GIMPLE_OACC_DATA:
            nesting_level++;
            scan_oacc_data (gsi, ctx);
            nesting_level--;
            break;
        case GIMPLE_OACC_HOST_DATA:
            nesting_level++;
            scan_oacc_host_data (gsi, ctx);
            nesting_level--;
            break;
        case GIMPLE_OACC_DECLARE:
        case GIMPLE_OACC_CACHE:
        case GIMPLE_OACC_WAIT:
        case GIMPLE_OACC_UPDATE:
          /* nothing to do in lowering pass */
          break;

        case GIMPLE_BIND:
        {
            tree var;

            *handled_ops_p = false;
            if (ctx)
              for (var = gimple_bind_vars (stmt); var ; var = DECL_CHAIN (var))
                  insert_decl_map (&ctx->cb, var, var);
        }
        break;
        default:
            *handled_ops_p = false;
            break;
        }
    return NULL_TREE;
}

/* analyze subpass operand callback */
static tree
analyze_op_cb(tree *tp, int *walk_subtrees, void *data)
{
    struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
    oacc_context* ctx = (oacc_context*)wi->info;
    tree t = *tp;

    if(ctx != NULL && ctx->param_map != NULL && is_gimple_variable(t))
        {
            tree* n = (tree *) pointer_map_contains (ctx->cb.decl_map, t);
            if(!n)
                {
                    splay_tree_node v;
                    v = splay_tree_lookup(ctx->param_map, (splay_tree_key)t);
                    if(!v)
                        {
                            splay_tree_insert(ctx->param_map, (splay_tree_key)t,
                                              (splay_tree_value)t);
                        }
                }
        }

    return NULL_TREE;
}

/* analyze subpass */
static void
analyze_gimple(gimple_seq* pseq, oacc_context* ctx)
{
    location_t saved_location;
    struct walk_stmt_info wi;

    memset (&wi, 0, sizeof (wi));
    wi.info = ctx;
    wi.want_locations = true;

    saved_location = input_location;
    walk_gimple_seq_mod (pseq, analyze_stmt_cb, analyze_op_cb, &wi);
    input_location = saved_location;
}

/* generate file name for OpenCL source file */
static char*
generate_ocl_module_name(void)
{
    size_t filename_len = strlen(main_input_filename);
    char* cur_module = XNEWVEC(char, filename_len + 4);
    const char* slash_ptr = strrchr(main_input_filename, '/');
    const char* dot_ptr = strrchr(main_input_filename, '.');

    if(dot_ptr && (!slash_ptr || dot_ptr > slash_ptr))
        {
            int cpy_len = filename_len - strlen(dot_ptr) + 1;
            memcpy(cur_module, main_input_filename, cpy_len);
            cur_module[cpy_len] = '\0';
        }
    else
        {
            strcpy(cur_module, main_input_filename);
            strcpy(cur_module, ".");
        }
    strcat(cur_module, "cl");
    return cur_module;
}

/* set generated OpenCL source file with respect to current source file */
static bool
switch_ocl_module(char* cur_module)
{
    if(ocl_module == NULL
            || strcmp((const char*)ocl_module, (const char*)cur_module) != 0)
        {
            if(access(cur_module, 0) == 0)
                {
                    unlink(cur_module);
                }

            if(ocl_module) ggc_free(ocl_module);
            ocl_module = (char*)ggc_strdup(cur_module);

            return true;
        }

    return false;
}

/* Main entry point for lowering OpenACC.  */
static unsigned int
execute_lower_oacc (void)
{
    gimple_seq func_body = 0;
    
    char* cur_module = generate_ocl_module_name ();
    if(switch_ocl_module (cur_module))
        {
            if(dump_file)
                {
                  fprintf (dump_file,
                           "main module: '%s', opencl module: '%s'\n",
                           main_input_filename, ocl_module);
                }
        }
    XDELETEVEC(cur_module);

    if(lookup_attribute("*kernel", DECL_ATTRIBUTES(current_function_decl))
                        != NULL_TREE)
      {
        return 0;
      }
    
    /* analyze gimple to find OpenACC directives */
    all_contexts = splay_tree_new (splay_tree_compare_pointers, 0,
                                   delete_oacc_context);
    func_body = gimple_body (current_function_decl);
    nesting_level = 0;
    analyze_gimple (&func_body, NULL);
    gcc_checking_assert (nesting_level == 0);

    /* if found, lower them */
    if(all_contexts->root)
        {
            push_gimplify_context();
            lower_oacc(&func_body, NULL);
            pop_gimplify_context(NULL);
        }

    if(all_contexts)
        {
            splay_tree_delete(all_contexts);
            all_contexts = NULL;
        }


    return 0;
}

/* OpenACC lowering gate */
static bool
gate_lower_oacc (void)
{
    return (flag_openacc != 0);
}

namespace
{
const pass_data pass_data_lower_oacc =
{
    GIMPLE_PASS,
    "oacclow",				/* name */
    OPTGROUP_NONE,                        /* optinfo_flags */
    gate_lower_oacc,			/* gate */
    execute_lower_oacc,			/* execute */
    TV_NONE,				/* tv_id */
    PROP_gimple_any,			/* properties_required */
    0,					/* properties_provided */
    0,					/* properties_destroyed */
    0,					/* todo_flags_start */
    0                      		/* todo_flags_finish */
};

class pass_lower_oacc : public gimple_opt_pass
{
public:
    pass_lower_oacc(gcc::context *ctxt)
        :
        gimple_opt_pass(pass_data_lower_oacc,
                        ctxt)
    {}

    /* opt_pass methods: */
    bool gate ()
    {
        return gate_lower_oacc();
    }
    unsigned int execute ()
    {
        return execute_lower_oacc ();
    }
};

}

gimple_opt_pass *
make_pass_lower_oacc (gcc::context *ctxt)
{
    return new pass_lower_oacc (ctxt);
}

/* update SSA for current function */
static void
do_update_ssa()
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB(bb)
  {
    for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
        {
            gimple stmt = gsi_stmt(gsi);
            gimple_set_modified (stmt, true);
            update_stmt_operands(stmt);
        }
  }
  update_ssa(TODO_update_ssa);
}

/* generate function call */
static gimple
build_call(location_t locus, tree funcdecl, int n, ...)
{
    va_list ap;
    int i;
    tree* args = XALLOCAVEC(tree, n);

    va_start(ap, n);
    for(i = 0; i < n; ++i)
        {
            args[i] = va_arg(ap, tree);
        }
    va_end(ap);

    return gimple_build_call_from_tree(
              build_call_expr_loc_array(locus, funcdecl,
                                       n, args));
}

/* replace current statement */
inline static void
gen_replace(gimple_stmt_iterator* gsi, gimple stmt)
{
    gsi_replace(gsi, stmt, true);
}

/* add new statement */
inline static void
gen_add(gimple_stmt_iterator* gsi, gimple stmt)
{
    gsi_insert_after(gsi, stmt, GSI_NEW_STMT);
}

/* mark child function as can be parallelized 
 and specify loop collapsing */
static void
mark_kernels_parallel(tree kernel_fn, unsigned collapse)
{
    if(!kernels)
    {
      kernels = splay_tree_new_ggc (splay_tree_compare_pointers,
        ggc_alloc_splay_tree_scalar_scalar_splay_tree_s,
        ggc_alloc_splay_tree_scalar_scalar_splay_tree_node_s);
    }
    splay_tree_insert(kernels, (splay_tree_key)DECL_UID(kernel_fn),
                      (splay_tree_value)collapse);
}

/* check for loop reducibility */
static bool
loop_irreducible_p(struct loop* l)
{
    unsigned i;
    basic_block *bbs;
    bool res = false;

    if(loop_preheader_edge (l)->src->flags & BB_IRREDUCIBLE_LOOP)
        {
            return true;
        }

    bbs = get_loop_body_in_dom_order (l);
    for (i = 0; i < l->num_nodes; i++)
        {
            if (bbs[i]->flags & BB_IRREDUCIBLE_LOOP)
                {
                    res = true;
                    break;
                }
        }
    free (bbs);

    return res;
}

/* preliminary checks */
static bool
loop_precheck(struct loop* l)
{
    if(!single_dom_exit(l))
        {
            if(dump_file)
                {
                    fprintf(dump_file, " !single_dom_exit\n");
                }
            return false;
        }
    else if(!can_duplicate_loop_p(l) || loop_irreducible_p(l))
        {
            return false;
        }
    return true;
}

/* can loop be parallelized? */
static bool
loop_parallelizable(struct loop* l, struct obstack* pobstack)
{
    vec<loop_p> loop_nest;
    vec<ddr_p> dependence_relations;
    vec<data_reference_p> datarefs;
    lambda_trans_matrix trans;
    bool ret = false;

    /* Check for problems with dependences.  If the loop can be reversed,
       the iterations are independent.  */
    datarefs.create (10);
    dependence_relations.create (10 * 10);
    loop_nest.create (3);
    if(dump_file) fprintf(dump_file, "compute data dependences\n");

    if (! compute_data_dependences_for_loop (l, true, &loop_nest, &datarefs,
            &dependence_relations))
      {
        if (dump_file /**/&& (dump_flags & TDF_DETAILS)/**/)
            fprintf (dump_file, "  FAILED: cannot analyze data dependencies\n");
        ret = false;
        goto end;
      }


    if (dump_file /**/&& (dump_flags & TDF_DETAILS)/**/)
      {
        fprintf(dump_file, "loop_nest %d, ddr %d, datarefs %d\n",
                loop_nest.length(), dependence_relations.length(),
                datarefs.length());
        dump_data_dependence_relations (dump_file, dependence_relations);
      }

    trans = lambda_trans_matrix_new (1, 1, pobstack);
    LTM_MATRIX (trans)[0][0] = -1;

    if(dump_file) fprintf(dump_file, "lambda transform legal\n");
    if (lambda_transform_legal_p (trans, 1, dependence_relations))
        {
            ret = true;
            if (dump_file /*&& (dump_flags & TDF_DETAILS)*/)
                fprintf (dump_file, "  SUCCESS: may be parallelized\n");
        }
    else if (dump_file /*&& (dump_flags & TDF_DETAILS)*/)
        fprintf (dump_file,
                 "  FAILED: data dependencies exist across iterations\n");

end:
    loop_nest.release ();
    free_dependence_relations (dependence_relations);
    free_data_refs (datarefs);
    return ret;
}

/* find iterations count */
static bool
get_loop_iteration_count(struct loop *loop, struct tree_niter_desc *pniter_desc)
{
  HOST_WIDE_INT estimated;

  estimated = estimated_stmt_executions_int (loop);
  if (estimated == -1)
      estimated = max_stmt_executions_int (loop);
  if (dump_file)
      fprintf (dump_file, "Estimated # of iterations %lld\n",
                    estimated);

  edge exit = single_dom_exit (loop);

  gcc_checking_assert (exit);

  if (!number_of_iterations_exit (loop, exit, pniter_desc,
        false))
      {
          if(estimated != -1)
              {
                  pniter_desc->niter = 
                    build_int_cst(uint32_type_node,
                                      estimated);
              }
          else
              {
                  if (dump_file)
                      fprintf (dump_file,
                               "  FAILED: number of "
                               "iterations not known\n");
                  return false;
              }
      }
  else
      {
          if(dump_file)
              {
                  fprintf(dump_file, "niter: ");
                  print_generic_expr(dump_file,
                                pniter_desc->niter, 0);
                  fprintf(dump_file, "\n");
                  fprintf(dump_file, "max: %lld\n",
                              pniter_desc->max.to_shwi());
              }
      }
  return true;
}

/* check for parallelization */
static bool
loop_parallelizable_p(struct loop *loop, struct tree_niter_desc *pniter_desc)
{
  struct obstack oacc_obstack;

  gcc_obstack_init (&oacc_obstack);
  if(!loop_precheck(loop))
    {
      return false;
    }

  if(!loop_parallelizable(loop, &oacc_obstack))
    {
      return false;
    }
  obstack_free (&oacc_obstack, NULL);
  if(!get_loop_iteration_count(loop, pniter_desc))
  {
    return false;
  }
  return true;
}

/* clone child function */
static tree
clone_function(tree fn, gimple kernel_stmt)
{
  tree name, new_fn = NULL, t;

  name = clone_function_name(fn, "_oacc_fn");
  name = normalize_name(name);
  if(dump_file)
      {
          fprintf(dump_file, "new fn name='%s'\n", IDENTIFIER_POINTER(name));
      }
  new_fn = build_decl(gimple_location(kernel_stmt), FUNCTION_DECL, name,
                    TREE_TYPE(fn));
  TREE_STATIC (new_fn) = 1;
  TREE_USED (new_fn) = 1;
  DECL_ARTIFICIAL (new_fn) = 1;
  DECL_NAMELESS (new_fn) = 1;
  DECL_IGNORED_P (new_fn) = 0;
  TREE_PUBLIC (new_fn) = 0;
  DECL_UNINLINABLE (new_fn) = 1;
  DECL_EXTERNAL (new_fn) = 0;
  DECL_CONTEXT (new_fn) = NULL_TREE;
  DECL_INITIAL (new_fn) = make_node (BLOCK);
  DECL_ATTRIBUTES(new_fn) = DECL_ATTRIBUTES(fn);
  
  t = build_decl (DECL_SOURCE_LOCATION (new_fn),
                  RESULT_DECL, NULL_TREE, void_type_node);
  DECL_ARTIFICIAL (t) = 1;
  DECL_IGNORED_P (t) = 1;
  DECL_CONTEXT (t) = new_fn;
  DECL_RESULT (new_fn) = t;

  return new_fn;
}

static void
map_parameters(tree orig_fn, oacc_kernel kernel, gimple kernel_stmt)
{
  unsigned i;
  tree t, param;
  vec<tree> params;

  params.create(3);
  for(param = DECL_ARGUMENTS(orig_fn); param;
    param = DECL_CHAIN(param))
  {
    params.safe_push(param);
  }

  for(i = params.length(); i > 0; --i)
    {
      param = params[i-1];
      const char *id = "_oacc_param";

      if(DECL_NAME(param))
        {
          id = IDENTIFIER_POINTER(DECL_NAME(param));
        }
      t = build_decl(DECL_SOURCE_LOCATION(kernel->func), PARM_DECL,
                      create_tmp_var_name(id), TREE_TYPE(param));
      if(dump_file)
          {
              fprintf(dump_file, "Create formal param: ");
              print_generic_expr(dump_file, t, 0);
              fprintf(dump_file, " type=");
              print_generic_expr(dump_file, TREE_TYPE(param), 0);
              fprintf(dump_file, "\n");
          }
      DECL_ARTIFICIAL (t) = 1;
      DECL_NAMELESS (t) = 1;
      DECL_ARG_TYPE (t) = TREE_TYPE(param);
      DECL_CONTEXT(t) = kernel->func;
      TREE_USED (t) = 1;
      TREE_ADDRESSABLE (t) = 1;
      DECL_ATTRIBUTES(t) = DECL_ATTRIBUTES(param);
      if(i > 0)
          DECL_CHAIN (t) = DECL_ARGUMENTS (kernel->func);
      DECL_ARGUMENTS (kernel->func) = t;
      splay_tree_insert(kernel->params_map, (splay_tree_key)param,
                                (splay_tree_value)t);
      if(dump_file)
          {
              fprintf(dump_file, "Map ");
              print_generic_expr(dump_file, param, 0);
              fprintf(dump_file, " to ");
              print_generic_expr(dump_file, t, 0);
              fprintf(dump_file, "\n");
          }
    }
  push_struct_function (kernel->func);
  cfun->function_end_locus = gimple_location(kernel_stmt);
  pop_cfun ();
}

static tree
map_params_cb(tree *tp, int *walk_subtrees, void *data)
{
  struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
  splay_tree param_map = (splay_tree)wi->info;
  tree t = *tp;

  if(is_gimple_variable(t))
  {
    splay_tree_node v;
    v = splay_tree_lookup(param_map, (splay_tree_key)t);
    if(v)
    {
      *tp = (tree)(v->value);
      if(dump_file)
      {
        fprintf(dump_file, "Map param ");
        print_generic_expr(dump_file, t, 0);
        fprintf(dump_file, " to param ");
        print_generic_expr(dump_file, *tp, 0);
        fprintf(dump_file, "\n");
      }
    }
  }
  return NULL_TREE;
}

DEBUG_FUNCTION static void
dump_dominators(FILE* fp, basic_block node, int indent)
{
  int i;
  basic_block son;

  for(i = 0; i < indent; ++i)
    fprintf(fp, " ");
  fprintf(fp, "%d\n", node->index);
  for(son = first_dom_son (CDI_DOMINATORS, node);
      son;
      son = next_dom_son(CDI_DOMINATORS, son))
    dump_dominators(fp, son, indent+2);
}

DEBUG_FUNCTION static void
dump_ssa(FILE *fp)
{
  unsigned i;

  fprintf(fp, "#SSA names: %d\n", num_ssa_names);
  for(i = 0; i < num_ssa_names; ++i)
  {
    tree var = ssa_name(i);
    fprintf(fp, "%d ", i);
    if(var == NULL_TREE)
    {
      fprintf(fp, "(NULL)");
    }
    else
    {
      print_generic_expr(fp, var, 0);
      if(SSA_NAME_IN_FREE_LIST (var))
      {
        fprintf(fp, " IN_FREE_LIST");
      }
    }
    fprintf(fp, "\n");
  }
}

DEBUG_FUNCTION static void
dump_fn_body(FILE* dump_file, const char* title)
{
    tree var;
    unsigned i;
    basic_block bb;

    fprintf(dump_file, "==============================\n%s"
            "\n========================\n", title);

    fprintf(dump_file, "#SSA names: %d\n", num_ssa_names);
    print_generic_decl(dump_file, current_function_decl, 0);
    fprintf(dump_file, "\n{\n");

    FOR_EACH_LOCAL_DECL(DECL_STRUCT_FUNCTION(current_function_decl), i, var)
    {
        print_generic_decl(dump_file, var, 0);
        fprintf(dump_file, "\n");
    }
    for(i = 1; i < num_ssa_names; ++i)
        {
            var = ssa_name(i);
            if(var && !SSA_NAME_VAR(var))
                {
                    fprintf(dump_file, "\t");
                    print_generic_expr(dump_file, var, 0);
                    fprintf(dump_file, "\n");
                }
        }

    FOR_EACH_BB(bb)
    {
        fprintf(dump_file, "L%d:\n", bb->index);
        gimple_stmt_iterator gsi;
        for(gsi = gsi_start_phis(bb); !gsi_end_p(gsi); gsi_next(&gsi))
            {
                gimple stmt = gsi_stmt(gsi);
                fprintf(dump_file, "\t#");
                print_gimple_stmt(dump_file, stmt, 0, 0);
            }
        for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
            {
                gimple stmt = gsi_stmt(gsi);
                fprintf(dump_file, "\t");
                print_gimple_stmt(dump_file, stmt, 0, 0);
            }
        edge e;
        gimple stmt;
        edge true_edge, false_edge;
        stmt = last_stmt (bb);

        if (stmt && gimple_code (stmt) == GIMPLE_COND)
            {

                extract_true_false_edges_from_block (bb, &true_edge,
                                            &false_edge);

                fprintf(dump_file, "\tgoto L%d", true_edge->dest->index);
                fprintf(dump_file, "\n");
                fprintf(dump_file, "else\n");
                fprintf(dump_file, "\tgoto L%d", false_edge->dest->index);
                fprintf(dump_file, "\n");
                continue;
            }
        e = find_fallthru_edge (bb->succs);
        if (e && e->dest != bb->next_bb)
            {
                fprintf(dump_file, "\tgoto L%d", e->dest->index);
                fprintf(dump_file, "\n");
            }
    }

    fprintf(dump_file, "}\n");
    fflush(dump_file);
}

/* get loop in source file location */
static location_t
guess_loop_location(struct loop* l)
{
  location_t locus = UNKNOWN_LOCATION;
  basic_block header = l->header;
  gimple_stmt_iterator gsi = gsi_start_bb(header);
  if(!gsi_end_p(gsi))
  {
    gimple stmt = gsi_stmt(gsi);
    locus = gimple_location(stmt);
  }
  return locus;
}

/* get parallelization clauses */
static void
get_loop_parallelization(oacc_kernel kernel)
{
  unsigned i;
  
  for(i = 0; i < kernel->clauses.length(); ++i)
    {
      tree clause = kernel->clauses[i];
      while(clause != NULL_TREE)
        {
          switch(OACC_CLAUSE_CODE(clause))
            {
            case OACC_CLAUSE_INDEPENDENT:
              kernel->no_check = true;
              break;
            case OACC_CLAUSE_SEQ:
              kernel->no_parallel = true;
              break;
            default:
              break;
            }
          clause = OACC_CLAUSE_CHAIN(clause);
        }
    }
}

/* get loop collapsing count */
static void
check_loop_parallelizable(struct loop* loop, oacc_kernel kernel)
{
  struct tree_niter_desc niter_desc;
  struct loop* l;
  unsigned collapse = kernel->collapse;
  
  niter_desc.niter = NULL_TREE;
  bool is_parallelizable = true;
  
  get_loop_parallelization(kernel);
  if(!kernel->no_check)
    is_parallelizable = loop_parallelizable_p(loop, &niter_desc);
  else if(kernel->no_parallel)
    is_parallelizable = false;
  
  if(!is_parallelizable)
    {
      if(!kernel->no_parallel)
        warning_at(guess_loop_location(loop), 
            OPT_fopenacc, "loop cannot be parallelized");
      kernel->niters.safe_push(integer_one_node);
      kernel->collapse = 0;
      return;
    }
  for(l = loop; collapse != 0 && l != NULL; collapse--, l = l->inner)
    {
      niter_desc.niter = NULL_TREE;
      if(!get_loop_iteration_count(l, &niter_desc))
        {
          if(kernel->niters.length() < 1)
            {
                kernel->niters.safe_push(integer_one_node);
                kernel->collapse = 0;
            }
          break;
        }
      kernel->niters.safe_push(niter_desc.niter);
    }
}

/* gather kernel's schedule clauses */
static void
gather_kernel_schedule(struct loop* root, splay_tree schedule, oacc_kernel kernel)
{
  splay_tree_node node = splay_tree_lookup(schedule, (splay_tree_key)root);
  struct loop* loop = NULL;
  
  if(node != NULL)
    {
      tree clauses = (tree)node->value;
      kernel->clauses.safe_push(clauses);
    }
  for(loop = root->next; loop != NULL; loop = loop->next)
    {
      gather_kernel_schedule(loop, schedule, kernel);
    }
  if(root->inner != NULL)
    gather_kernel_schedule(root->inner, schedule, kernel);
}

/* get collapse clause if any */
static void
get_collapse_count(oacc_kernel kernel)
{
  unsigned i;
  
  kernel->collapse = 1;
  
  for(i = 0; i < kernel->clauses.length(); ++i)
    {
      tree clause = kernel->clauses[i];
      while(clause != NULL_TREE)
        {
          if(OACC_CLAUSE_CODE(clause) == OACC_CLAUSE_COLLAPSE)
            break;
          clause = OACC_CLAUSE_CHAIN(clause);
        }
      if(clause != NULL_TREE)
        {
          tree collapse = OACC_CLAUSE_COLLAPSE_EXPR(clause);
          HOST_WIDE_INT n = tree_to_shwi(collapse);
          kernel->collapse = (unsigned)n;
          return;
        }
    }
}

/* create child functions for kernels */
static void
create_fns_for_kernels(tree child_fn, vec<oacc_kernel>* kernels,
                       gimple kernel_stmt, unsigned nloops,
                       struct loop **vloops, splay_tree schedule)
{
  unsigned i;
  oacc_kernel kernel;

  for(i = 0; i < nloops; ++i)
    {
      tree fn = child_fn;

      if(i > 0)
      {
        fn = clone_function(child_fn, kernel_stmt);
      }
      kernel = new_oacc_kernel(fn);
      if(i > 0)
      {
        map_parameters(child_fn, kernel, kernel_stmt);
      }
      gather_kernel_schedule(vloops[i], schedule, kernel);
      get_collapse_count(kernel);
      check_loop_parallelizable(vloops[i], kernel);
      kernels->safe_push(kernel);
    }
}

/* create basic blocks to ensure that region is SingleEntrySingleExit*/
static void
create_padding_blocks(struct loop **vloops, unsigned nloops, struct loops* loops)
{
  unsigned i;

  for(i = 0; i < nloops - 1; ++i)
  {
    edge exit_loop = single_exit(vloops[i]);
    basic_block new_bb, before, after, imm_dom;
    gimple_stmt_iterator gsi;

    before = exit_loop->src;
    if(dump_file)
    {
      fprintf(dump_file, "before %d has %d successors\n",
        before->index, EDGE_COUNT(before->succs));
    }
    if(!single_succ_p(before))
    {
      edge p;
      edge_iterator ei;

      after = exit_loop->dest;
      if(dump_file)
      {
        fprintf(dump_file, "after %d has %d predecessors\n",
          after->index, EDGE_COUNT(after->preds));
      }
      imm_dom = get_immediate_dominator(CDI_DOMINATORS, after);

      new_bb = create_empty_bb(before);
      add_bb_to_loop(new_bb, loops->tree_root);
      redirect_edge_and_branch(exit_loop, new_bb);
      make_single_succ_edge(new_bb, after, EDGE_FALLTHRU);

      FOR_EACH_EDGE(p, ei, after->preds)
      {
        if(dump_file)
        {
          fprintf(dump_file, "edge %d -> %d\n", p->src->index, p->dest->index);
        }
        if(p->src->index != new_bb->index)
          redirect_edge_and_branch(p, new_bb);
      }

      set_immediate_dominator (CDI_DOMINATORS, new_bb, imm_dom);
      set_immediate_dominator (CDI_DOMINATORS, after, new_bb);
      if(i > 0)
      {
          gsi = gsi_last_bb(new_bb);
          gsi_insert_before(&gsi, gimple_build_return(NULL_TREE), GSI_SAME_STMT);
      }
    }
  }
}

/* find SESE regions to split kernels */
static void
collect_sese_regions(struct loop **vloops, unsigned nloops,
                     vec<oacc_kernel>* kernels)
{
  unsigned i;
  basic_block bb_entry, bb_exit;

  bb_entry = single_succ_edge(ENTRY_BLOCK_PTR_FOR_FN(cfun))->dest;
  for(i = 0; i < nloops; ++i)
  {
    if(i == nloops - 1)
    {
      bb_exit = single_pred_edge(EXIT_BLOCK_PTR_FOR_FN(cfun))->src;
    }
    else
    {
      bb_exit = single_exit(vloops[i])->dest;
    }
    if(dump_file)
    {
      fprintf(dump_file, "LOOP #%d, ENTRY %d, EXIT %d\n",
        vloops[i]->num, bb_entry->index, bb_exit->index);
    }
    
    (*kernels)[i]->bb_entry = bb_entry;
    (*kernels)[i]->bb_exit = bb_exit;


    if(i < nloops - 1)
    {
      bb_entry = single_succ(bb_exit);
    }

  }
}

/* release variables moved to child function */
static void
release_moved_vars(vec<basic_block>* bbs)
{
  unsigned i, j;

  for(i = 1; i < num_ssa_names; ++i)
  {
    tree var = ssa_name(i);
    if(var)
    {
      basic_block def_block = gimple_bb(SSA_NAME_DEF_STMT(var));

      for(j = 0; j < bbs->length(); ++j)
      {
        if((*bbs)[j] == def_block)
        {
          if(dump_file)
          {
            fprintf(dump_file, "Release ssa name %d ", i);
            print_generic_expr(dump_file, var, 0);
            fprintf(dump_file, " defined in block %d", def_block->index);
            fprintf(dump_file, "\n");
          }
          release_ssa_name(var);
          break;
        }
      }
    }
  }
}

static void
map_params_in_new_fn(oacc_kernel kernel)
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB(bb)
  {
    for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
      {
        struct walk_stmt_info wi;
        gimple stmt = gsi_stmt(gsi);

        memset(&wi, 0, sizeof(struct walk_stmt_info));
        wi.info = (void*)kernel->params_map;
        walk_gimple_op(stmt, map_params_cb, &wi);
      }
  }
}

/* find loop dominated by basic block */
static struct loop*
find_dominated_loop(basic_block bb, struct loop* root)
{
  basic_block son;
  
  for(son = first_dom_son(CDI_DOMINATORS, bb); son != NULL;
      son = next_dom_son(CDI_DOMINATORS, son))
    {
      if(son == root->header)
        return root;
    }
  
  for(struct loop* l = root->inner; l != NULL;
      l = l->next)
    {
      struct loop* found = find_dominated_loop(bb, l);
      if(found != NULL)
        return found;
    }
  return NULL;
}

DEBUG_FUNCTION static void
dump_clauses(tree clauses)
{
  while(clauses != NULL_TREE)
    {
      print_generic_expr(dump_file, clauses, 0);
      fprintf(dump_file, "\n");
      clauses = OACC_CLAUSE_CHAIN(clauses);
    }
}

/* get loop schedule clauses */
static void
gather_loop_schedule(tree child_fn, struct loops* loops, splay_tree schedule)
{
    struct function* child_cfun;
    basic_block bb;
    gimple_stmt_iterator gsi;
    
    child_cfun = DECL_STRUCT_FUNCTION(child_fn);
    push_cfun(child_cfun);
    FOR_EACH_BB(bb)
    {
      for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
        {
          gimple stmt = gsi_stmt(gsi);

          if(gimple_code(stmt) == GIMPLE_OACC_LOOP)
            {
              tree clauses = gimple_oacc_loop_clauses(stmt);
              if(clauses != NULL_TREE)
                {
                  struct loop* loop = find_dominated_loop(bb, loops->tree_root);
                  
                  if(dump_file)
                    {
                      fprintf(dump_file, "LOOP statement clauses\n");
                      dump_clauses(clauses);
                    }
                  
                  if(loop != NULL)
                    {
                      splay_tree_insert(schedule, (splay_tree_key)loop,
                                        (splay_tree_value)clauses);
                    }
                }
            }
        }
    }

    pop_cfun();
}

/* split executable region into kernels */
static void
extract_kernels(struct loops* loops, tree child_fn,
                vec<oacc_kernel>* kernels, gimple kernel_stmt)
{
  struct loop *ploop;
  oacc_kernel kernel;
  unsigned nloops, i;
  struct loop **vloops;
  struct function* child_cfun;
  splay_tree loop_schedule;
  
  loop_schedule = splay_tree_new(splay_tree_compare_pointers, 0, 0);
  
  gather_loop_schedule (child_fn, loops, loop_schedule);
  if(gimple_code(kernel_stmt) == GIMPLE_OACC_PARALLEL 
    || loops->tree_root->inner == NULL 
    || loops->tree_root->inner->next == NULL)
    {
      kernel = new_oacc_kernel(child_fn);

      if(loops->tree_root->inner != NULL)
        {
          gather_kernel_schedule(loops->tree_root->inner,
                                 loop_schedule, kernel);
          get_collapse_count(kernel);
          check_loop_parallelizable(loops->tree_root->inner, kernel);
        }

      kernels->safe_push(kernel);
      mark_kernels_parallel (child_fn, kernel->collapse);
      splay_tree_delete(loop_schedule);
      return;
    }

  if(dump_file)
  {
    basic_block *body;

    body = get_loop_body(loops->tree_root);
    fprintf(dump_file, "FAKE 0 LOOP: ");
    for(i = 0; i < loops->tree_root->num_nodes; ++i)
    {
      fprintf(dump_file, "%d%c", body[i]->index,
        (i < loops->tree_root->num_nodes-1) ? ' ' : '\n');
    }
    XDELETEVEC(body);
    for(ploop = loops->tree_root->inner; ploop != NULL; ploop = ploop->next)
    {
      fprintf(dump_file, "LOOP %d: ", ploop->num);
      body = get_loop_body(ploop);
      for(i = 0; i < ploop->num_nodes; ++i)
        fprintf(dump_file, "%d%c", body[i]->index,
        (i < ploop->num_nodes-1) ? ' ' : '\n');
      XDELETEVEC(body);
    }

    fflush(dump_file);
  }

  /* Loops connected to siblings in reverse order of 
    appearance so we reorder them
  */
  for(nloops = 0, ploop = loops->tree_root->inner;
      ploop != NULL;
      ++nloops, ploop = ploop->next)
      ;
  
  vloops = XALLOCAVEC(struct loop*, nloops);

  for(i = nloops, ploop = loops->tree_root->inner;
      i > 0;
      --i, ploop = ploop->next)
      vloops[i-1] = ploop;

  create_fns_for_kernels(child_fn, kernels, kernel_stmt,
                         nloops, vloops, loop_schedule);

  if(dump_file)
  {
    child_cfun = DECL_STRUCT_FUNCTION(child_fn);
    push_cfun(child_cfun);
    dump_fn_body(dump_file, "ORIGINAL CHILD FUNCTION");
    fflush(dump_file);
    pop_cfun();
  }

  create_padding_blocks(vloops, nloops, loops);

  if(dump_file)
  {
    child_cfun = DECL_STRUCT_FUNCTION(child_fn);
    push_cfun(child_cfun);
    dump_fn_body(dump_file, "AFTER PADDING");
    fflush(dump_file);
    pop_cfun();
  }

  collect_sese_regions(vloops, nloops, kernels);

  if(dump_file)
  {
    dump_dominators(dump_file, ENTRY_BLOCK_PTR_FOR_FN(cfun), 0);
  }

  for(i = 0; i < nloops; ++i)
  {
    tree fn = (*kernels)[i]->func;

    if(i > 0)
    {
      int opt_level;
      gimple_stmt_iterator gsi;
      basic_block bb;
      vec<basic_block> bbs;
      
      bbs.create(5);
      bbs.safe_push((*kernels)[i]->bb_entry);
      gather_blocks_in_sese_region((*kernels)[i]->bb_entry,
                    (*kernels)[i]->bb_exit, &bbs);

      if(dump_file)
      {
        unsigned j;
        fprintf(dump_file, "LOOP #%d ", i);
        for(j = 0; j < bbs.length(); ++j)
          fprintf(dump_file, "%d ", bbs[j]->index);
        fprintf(dump_file, "\n");
      }

      child_cfun = DECL_STRUCT_FUNCTION (fn);
      init_tree_ssa (child_cfun);
	    init_ssa_operands (child_cfun);
	    child_cfun->gimple_df->in_ssa_p = true;
      bb = move_sese_region_to_fn (child_cfun, (*kernels)[i]->bb_entry,
        (*kernels)[i]->bb_exit, NULL_TREE);
      if(i == nloops - 1)
      {
        gsi = gsi_last_bb(bb);
        gsi_insert_before(&gsi, gimple_build_return(NULL_TREE), GSI_SAME_STMT);
      }

      release_moved_vars(&bbs);

      if(dump_file)
      {
        dump_fn_body(dump_file, "AFTER MOVE SESE");
        dump_ssa(dump_file);
        dump_dominators(dump_file, ENTRY_BLOCK_PTR_FOR_FN(cfun), 0);
        fflush(dump_file);
      }

      child_cfun->curr_properties = cfun->curr_properties;
      cgraph_add_new_function (fn, true);
      push_cfun(child_cfun);

      if(dump_file)
      {
        dump_fn_body(dump_file, "NEW FUNCTION");
        fflush(dump_file);
      }

      map_params_in_new_fn((*kernels)[i]);

      if(dump_file)
      {
        fprintf(dump_file, "optimize %d, save_opt_level %d\n",
          optimize, save_opt_level);
      }

      if(save_opt_level >= 0)
      {
        opt_level = optimize;
        optimize = save_opt_level;
      }
      calculate_dominance_info (CDI_DOMINATORS);
      do_update_ssa();
      loop_optimizer_init (LOOPS_NORMAL | LOOPS_HAVE_RECORDED_EXITS);
      rewrite_into_loop_closed_ssa (NULL, TODO_update_ssa);
      pop_cfun();
      if(save_opt_level >= 0)
        optimize = opt_level;
    }
    mark_kernels_parallel (fn, (*kernels)[i]->collapse);
  }

  splay_tree_delete(loop_schedule);
}

/* get variable defined by statement */
static tree
get_gimple_def_var(gimple stmt)
{
    tree var = NULL_TREE;
    gcc_checking_assert(gimple_code(stmt) == GIMPLE_ASSIGN
               || gimple_code(stmt) == GIMPLE_CALL 
               || gimple_code(stmt) == GIMPLE_PHI);

    if(gimple_code(stmt) == GIMPLE_PHI)
        {
            var = gimple_phi_result(stmt);
        }
    else if(is_gimple_assign(stmt))
        {
            var = gimple_assign_lhs(stmt);
        }
    else
        {
            var = gimple_call_lhs(stmt);
        }
    return var;
}

/* set variable to be defined by statement */
static void
set_gimple_def_var(gimple stmt, tree var)
{
    gcc_checking_assert(gimple_code(stmt) == GIMPLE_ASSIGN
               || gimple_code(stmt) == GIMPLE_CALL);

    if(is_gimple_assign(stmt))
        {
            gimple_assign_set_lhs(stmt, var);
        }
    else
        {
            gimple_call_set_lhs(stmt, var);
        }
}

struct loop_ctrl_var
{
  tree var;
  struct loop* loop;
};
static bool check_for_ctrl_var(tree, struct loop_ctrl_var* ctrl_var);

typedef bool (*walk_use_def_chains_fn) (tree, gimple, void *);

static bool
walk_use_def_chains_1 (tree var, walk_use_def_chains_fn fn, void *data,
                      struct pointer_set_t *visited, bool is_dfs)
{
  gimple def_stmt;

  if (pointer_set_insert (visited, var))
    return false;

  def_stmt = SSA_NAME_DEF_STMT (var);

  if (gimple_code (def_stmt) != GIMPLE_PHI)
    {
      /* If we reached the end of the use-def chain, call FN.  */
      return fn (var, def_stmt, data);
    }
  else
    {
      size_t i;

      /* When doing a breadth-first search, call FN before following the
        use-def links for each argument.  */
      if (!is_dfs)
       for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
         if (fn (gimple_phi_arg_def (def_stmt, i), def_stmt, data))
           return true;

      /* Follow use-def links out of each PHI argument.  */
      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
       {
         tree arg = gimple_phi_arg_def (def_stmt, i);

         /* ARG may be NULL for newly introduced PHI nodes.  */
         if (arg
             && TREE_CODE (arg) == SSA_NAME
             && walk_use_def_chains_1 (arg, fn, data, visited, is_dfs))
           return true;
       }

      /* When doing a depth-first search, call FN after following the
        use-def links for each argument.  */
      if (is_dfs)
       for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
         if (fn (gimple_phi_arg_def (def_stmt, i), def_stmt, data))
           return true;
    }

  return false;
}

static void
walk_use_def_chains (tree var, walk_use_def_chains_fn fn, void *data,
                     bool is_dfs)
{
  gimple def_stmt;

  gcc_assert (TREE_CODE (var) == SSA_NAME);

  def_stmt = SSA_NAME_DEF_STMT (var);

  /* We only need to recurse if the reaching definition comes from a PHI
     node.  */
  if (gimple_code (def_stmt) != GIMPLE_PHI)
    (*fn) (var, def_stmt, data);
  else
    {
      struct pointer_set_t *visited = pointer_set_create ();
      walk_use_def_chains_1 (var, fn, data, visited, is_dfs);
      pointer_set_destroy (visited);
    }
}

/* walk use-def chain callback */
static bool
check_for_ctrl_var_cb(tree var, gimple stmt, void* data)
{
    struct loop_ctrl_var *ctrl_var = (struct loop_ctrl_var*)data;
    unsigned i;

    for(i = 0; i < gimple_num_ops(stmt); ++i)
      {
          tree op_var = gimple_op(stmt, i);
          if(op_var == var)
            {
              continue;
            }
          if(check_for_ctrl_var(op_var, ctrl_var))
            {
              return true;
            }
      }
    return false;
}

/* check for VAR is control variable for loop */
static bool
check_for_ctrl_var(tree var, struct loop_ctrl_var* ctrl_var)
{
  tree chrec = analyze_scalar_evolution(ctrl_var->loop, var);
  if(dump_file)
    {
      fprintf(dump_file, "check var ");
      print_generic_expr(dump_file, var, 0);
      fprintf(dump_file, " %s", ((tree_is_chrec(chrec)) ? "CHREC" : ""));
      fprintf(dump_file, "\n");
    }
  if(tree_is_chrec(chrec))
    {
      ctrl_var->var = var;
      return true;
    }

  if(is_gimple_constant(var))
    {
      return false;;
    }

  if(is_gimple_variable(var))
    {
      if(SSA_NAME_VAR(var))
        {
          ctrl_var->var = var;
          return true;
        }
      else
        {
          walk_use_def_chains(var, check_for_ctrl_var_cb,
                              (void*)ctrl_var, false);

          if(ctrl_var->var != NULL_TREE)
            {
              return true;
            }
        }
    }

  return false;;
}

/* find a control variable in LOOP with HEADER */
static tree
find_ctrl_var(basic_block header, struct loop* loop)
{
    gimple cond_stmt;
    loop_ctrl_var ctrl_var;

    cond_stmt = gsi_stmt(gsi_last_bb(header));

    gcc_checking_assert(gimple_code(cond_stmt) == GIMPLE_COND);

    ctrl_var.var = NULL_TREE;
    ctrl_var.loop = loop;

    if(check_for_ctrl_var(gimple_cond_lhs(cond_stmt), &ctrl_var))
      {
        return ctrl_var.var;
      }

    check_for_ctrl_var(gimple_cond_rhs(cond_stmt), &ctrl_var);
    gcc_checking_assert(ctrl_var.var != NULL_TREE);
    return ctrl_var.var;
}

/* walk use-def chain callback */
static bool
ctrl_var_cb(tree var, gimple stmt, void* data)
{
    vec<gimple_stmt_iterator>* piter = (vec<gimple_stmt_iterator>*)data;
    gimple_stmt_iterator gsi = gsi_for_stmt(stmt);
    while(!gsi_end_p(gsi))
        {
            if(gsi_stmt(gsi) == stmt)
                {
                    if(gimple_code (stmt) == GIMPLE_PHI)
                        {
                            unsigned i;
                            for(i = 0; i < piter->length(); ++i)
                                {
                                    if(gsi_stmt((*piter)[i]) == stmt)
                                        {
                                            break;
                                        }
                                }
                            if(i < piter->length())
                                {
                                    break;
                                }
                        }
                    piter->safe_push(gsi);
                    break;
                }
            gsi_next(&gsi);
        }

    return false;
}

/* gather definitions of control variable */
static void
gather_control_var_defs(tree var, vec<gimple_stmt_iterator>* piter)
{
    piter->create(16);
    walk_use_def_chains(var, ctrl_var_cb, (void*)piter, false);
}

/* collect phi arguments in block BB that comes from basic block SRC*/
static void
gather_latch_phi_args(vec<struct phi_arg_d*>* phi_args, basic_block bb,
                      basic_block src)
{
  gimple_stmt_iterator phi_iter;
  for(phi_iter = gsi_start_phis(bb); !gsi_end_p(phi_iter);
          gsi_next(&phi_iter))
      {
          gimple phi_stmt = gsi_stmt(phi_iter);
          unsigned i;

          for(i = 0; i < gimple_phi_num_args(phi_stmt); ++i)
              {
                  edge e = gimple_phi_arg_edge(phi_stmt, i);
                  if(e->src->index == src->index)
                      {
                          phi_args->safe_push(gimple_phi_arg(phi_stmt, i));
                          break;
                      }
              }
      }
}

/* rewrite arguments of PHI function */
static void
fix_phi_args(vec<struct phi_arg_d*>* phi_args, basic_block bb,
             basic_block src)
{
  gimple_stmt_iterator phi_iter;
  for(phi_iter = gsi_start_phis(bb); !gsi_end_p(phi_iter);
          gsi_next(&phi_iter))
      {
          gimple phi_stmt = gsi_stmt(phi_iter);
          unsigned i, j;

          for(i = 0; i < gimple_phi_num_args(phi_stmt); ++i)
            {
              edge e = gimple_phi_arg_edge(phi_stmt, i);
              if(e->src->index == src->index
                      && gimple_phi_arg_def(phi_stmt, i) == NULL_TREE)
                {
                  tree def_var = gimple_phi_result(phi_stmt);
                  for(j = 0; j < phi_args->length(); ++j)
                    {
                      struct phi_arg_d* pd = (*phi_args)[j];
                      tree arg_var = get_use_from_ptr (&pd->imm_use);
                      if(SSA_NAME_VAR(def_var) != NULL_TREE
                           && SSA_NAME_VAR(def_var) == SSA_NAME_VAR(arg_var))
                        {
                            gimple_phi_set_arg(phi_stmt, i, pd);
                        }
                    }
                }
            }
      }
}

/* fix dominators of basic block */
static void
fix_dominators(basic_block bb)
{
  vec<basic_block> blocks;

  blocks.create(1);
  blocks.safe_push(bb);
  iterate_fix_dominators(CDI_DOMINATORS, blocks, true);
}

/* find last dominated definition */
static tree
find_last_dom_def(vec<gimple_stmt_iterator>* defs, basic_block dom_bb)
{
  unsigned i, def_idx = defs->length();
  tree def_var = NULL_TREE;
  gimple_stmt_iterator gsi;
  gimple stmt;

  for(i = 0; i < defs->length(); ++i)
    {
      if((*defs)[i].bb == dom_bb)
        {
          def_idx = i;
          break;
        }
    }
  gcc_checking_assert(def_idx < defs->length());

  if(dump_file)
    {
      fprintf(dump_file, "last ctrl var def in block %d\n",
              dom_bb->index);
    }
  gsi = (*defs)[def_idx];
  stmt = gsi_stmt(gsi);
  def_var = get_gimple_def_var(stmt);
  return def_var;
}

/* make sure VAR has the same unsigned property as TYPE */
static tree
generate_unsigned_typecast(gimple_stmt_iterator *gsi, tree type, tree var,
                           gimple* def_stmt)
{
  gimple stmt = NULL;
  tree convert_var = NULL_TREE;
  
  if(TYPE_UNSIGNED(type) == TYPE_UNSIGNED(TREE_TYPE(var)))
    return var;
  
  stmt = build_type_cast(type, var);
  convert_var = create_tmp_reg(type, "_oacc_tmp");
  
  convert_var = make_ssa_name(convert_var, stmt);
  set_gimple_def_var(stmt, convert_var);
  gen_add (gsi, stmt);
  if(def_stmt != NULL)
    *def_stmt = stmt;
  return convert_var;
}

/* generate initialization of index variable in kernel after collapsing 
 must be done from outer loop to inners */
static void
generate_ctrl_var_init(gimple_stmt_iterator* gsi, gimple stmt,
                       collapse_data data, unsigned idx)
{
  location_t location = gimple_location(stmt);
  tree lhs = get_gimple_def_var(stmt);
  tree workitem_id = NULL_TREE, init = NULL_TREE;
  tree builtin_decl = NULL_TREE;
  tree builtin_return_type = NULL_TREE;
  tree new_var = NULL_TREE;
  gimple add_stmt = NULL, copy_stmt = NULL;

  /* get workitem global id
   for outer loop calls OpenCL global_id()
   for inner loops get saved variable */
  builtin_decl = builtin_decl_explicit(BUILT_IN_GOACC_GET_GLOBAL_ID);
  builtin_return_type = TREE_TYPE (TREE_TYPE (builtin_decl));

  if(idx > 0)
  {
    workitem_id = data->workitem_id;
  }
  else
  {
    //FIXME: add more conversion magic???
    gcc_checking_assert(TREE_CODE(builtin_return_type) == TREE_CODE(TREE_TYPE(lhs)));

    /* _oacc_tmp = __builtin_get_global_id (0); */
    gimple call_stmt = build_call(location,
                builtin_decl, 1,
                build_int_cst(builtin_return_type, 0));
    workitem_id = create_tmp_reg(builtin_return_type, "_oacc_tmp");
    workitem_id = make_ssa_name(workitem_id, call_stmt);
    set_gimple_def_var(call_stmt, workitem_id);
    gen_add(gsi, call_stmt);
    data->workitem_id = workitem_id;
    data->wi_def_stmt = call_stmt;
  }
  
  /* generate typecast if unsigned properties of loop control variable
   and builtin function are different */
  if (TYPE_UNSIGNED(builtin_return_type) != TYPE_UNSIGNED(TREE_TYPE(lhs)))
    {
      /* _ = (int) _oacc_tmp; */
      gimple convert_stmt = NULL;
      
      workitem_id = generate_unsigned_typecast(gsi, TREE_TYPE(lhs), 
                               workitem_id, &convert_stmt);
      data->workitem_id = workitem_id;
      data->wi_def_stmt = convert_stmt;
    }

  copy_stmt = build_type_cast(TREE_TYPE(workitem_id), workitem_id);
  init = create_tmp_reg(TREE_TYPE(workitem_id), "_oacc_tmp");
  init = make_ssa_name(init, copy_stmt);
  set_gimple_def_var(copy_stmt, init);
  gen_add(gsi, copy_stmt);
  
  /* delete global id by loop counts of inner loops */
  if(idx + 1 < data->loops.length())
    {
      unsigned i;
      tree quotient = NULL_TREE, divisor = NULL_TREE;
      gimple div_stmt = NULL;

      for(i = idx + 1; i < data->loops.length(); ++i)
      {
        divisor = generate_unsigned_typecast(gsi, TREE_TYPE(init),
                            data->loops[i]->niter, NULL);
        quotient = make_ssa_name(SSA_NAME_VAR(init), copy_stmt);
        div_stmt = build_assign(TRUNC_DIV_EXPR, init, divisor);
        set_gimple_def_var(div_stmt, quotient);
        gen_add(gsi, div_stmt);
        init = quotient;
      }
      
    }

  /* if loop is inner get remainder by loop iteration count */
  if(idx > 0)
    {
      tree remain = NULL_TREE, divisor = NULL_TREE;
      gimple mod_stmt = NULL;
      divisor = generate_unsigned_typecast(gsi, TREE_TYPE(init),
                    data->loops[idx]->niter, NULL);
      remain = make_ssa_name(SSA_NAME_VAR(init), copy_stmt);
      mod_stmt = build_assign(TRUNC_MOD_EXPR, init, divisor);
      set_gimple_def_var(mod_stmt, remain);
      gen_add(gsi, mod_stmt);
      init = remain;
    }

    /* add initialization value to loop index */
    /* i = _oacc_tmp + i; */
    add_stmt = build_assign(PLUS_EXPR, lhs, init);
    new_var = copy_ssa_name(lhs, add_stmt);
    set_gimple_def_var(add_stmt, new_var);
    gen_add(gsi, add_stmt);
}

/* rewrite definitions of control variable */
static void
fix_ctrl_var_defs(vec<gimple_stmt_iterator>* defs,
                  vec<basic_block>* loop_blocks,
                  tree def_var, basic_block header_bb,
                  collapse_loop_data loop_data)
{
  unsigned i, j;

  for(i = 0; i < defs->length(); ++i)
    {
      gimple_stmt_iterator gsi = (*defs)[i];
      for(j = 0; j < loop_blocks->length(); ++j)
        {
          if(gsi.bb == (*loop_blocks)[j])
            {
              break;
            }
        }
      if(j < loop_blocks->length())
        {
          gimple stmt = gsi_stmt(gsi);
          tree lhs = get_gimple_def_var(stmt);
          replace_uses_by(lhs, def_var);
          gsi_remove(&gsi, true);
        }
      else if(dominated_by_p(CDI_DOMINATORS, header_bb, gsi.bb))
        {
          if(dump_file)
            {
              fprintf(dump_file,
                    "use in block %d dominates header %d\n",
                    gsi.bb->index, header_bb->index);
            }
          gimple stmt = gsi_stmt(gsi);
          if(gimple_code(stmt) != GIMPLE_PHI)
            loop_data->gsi = gsi;
        }
    }
}

/* perform loop collapsing */
static void
collapse_loop(struct loop* l, collapse_loop_data data)
{
    unsigned i;
    basic_block* bb = get_loop_body(l);
    vec<basic_block> loop_blocks;
    edge latch, exit;
    basic_block hdr_blk, imm_bb;
    tree ctrl_var, def_var;
    vec<gimple_stmt_iterator> ctrl_var_defs;
    vec<struct phi_arg_d*> phi_args;
    basic_block exit_bb;

    if(dump_file)
        {
            fprintf(dump_file, "parallelize loop %d\n", l->num);
        }


    loop_blocks.create(l->num_nodes);
    for(i = 0; i < l->num_nodes; ++i)
    {
      loop_blocks.safe_push(bb[i]);
    }
    XDELETEVEC(bb);

    if(dump_file)
        {
            fprintf(dump_file, "header: %d\n", l->header->index);
            fprintf(dump_file, "latch: %d\n", l->latch->index);
            fprintf(dump_file, "exit edge %p\n", (void*)single_exit(l));
            fflush(dump_file);
        }

    gcc_checking_assert(single_dom_exit(l));
    if(dump_file)
        {
            dump_fn_body(dump_file, "BEFORE");
        }

    /* find header block and loop exit edge*/
    latch = loop_latch_edge(l);
    exit = single_dom_exit(l);
    hdr_blk = exit->src;

    /* find loop control variable (index)*/
    ctrl_var = find_ctrl_var(hdr_blk, l);

    if(dump_file)
      {
          fprintf(dump_file, "loop control var ");
          print_generic_expr(dump_file, ctrl_var, 0);
          fprintf(dump_file, "\n");
      }

    /* collect all definitions of control variable */
    gather_control_var_defs(ctrl_var, &ctrl_var_defs);
    if(dump_file)
      {
        unsigned i;
        for(i = 0; i < ctrl_var_defs.length(); ++i)
          {
            print_gimple_stmt(dump_file, gsi_stmt(ctrl_var_defs[i]),
                                0, 0);
          }
      }


    if(dump_file)
      {
          fprintf(dump_file, "latch edge %d -> %d\n",
                  latch->src->index, latch->dest->index);
          fprintf(dump_file, "exit edge %d -> %d\n",
                  exit->src->index, exit->dest->index);
          fprintf(dump_file, "redirecting %d -> %d\n",
                  latch->src->index, exit->dest->index);
      }


    /* collect arguments of PHI nodes to be fixed*/
    phi_args.create(4);
    gather_latch_phi_args(&phi_args, latch->dest, latch->src);

    /* redirect latch edge to exit */
    if(!redirect_edge_and_branch(latch, exit->dest))
      {
          redirect_edge_and_branch_force(latch, exit->dest);
      }

    /* rewrite arguments of PHI functions which invalidated by redirection */
    fix_phi_args(&phi_args, exit->dest, latch->src);




    if(dump_file && 0)
      {
          dump_fn_body(dump_file, "AFTER REDIRECT");
      }

    /* now we can remove exit edge */
    gcc_checking_assert(can_remove_branch_p(exit));
    exit_bb = exit->dest;
    remove_branch(exit);
    fix_dominators(exit_bb);

    if(dump_file && 0)
      {
          dump_fn_body(dump_file, "AFTER REMOVE EXIT");
      }

    /* rewrite definitions of control variable */
    imm_bb = get_immediate_dominator(CDI_DOMINATORS, hdr_blk);
    gcc_checking_assert(imm_bb);
    def_var = find_last_dom_def(&ctrl_var_defs, imm_bb);
    gcc_checking_assert(def_var);

    fix_ctrl_var_defs(&ctrl_var_defs, &loop_blocks, def_var, hdr_blk, data);


    if(dump_file && 0)
      {
          dump_fn_body(dump_file, "AFTER CTRL VAR");
      }

    /* header may be remains in block after loop, 
     so CFG contains unneeded jumps */
    if(single_pred_p(hdr_blk) && single_succ_p(hdr_blk))
    {
      edge e_pred, e_succ;

      e_pred = single_pred_edge(hdr_blk);
      e_succ = single_succ_edge(hdr_blk);

      if(e_pred->src != hdr_blk->prev_bb && e_succ->dest != hdr_blk->next_bb)
      {
        if(dump_file)
        {
          fprintf(dump_file, "header block %d\n", hdr_blk->index);
          fprintf(dump_file, "\tpred %d, prev %d\n", e_pred->src->index,
            hdr_blk->prev_bb->index);
          fprintf(dump_file, "\tsucc %d, next %d\n", e_succ->dest->index,
            hdr_blk->next_bb->index);
        }
        move_block_after(hdr_blk, e_pred->src);

      }
    }
    if(dump_file)
      {
          dump_fn_body(dump_file, "AFTER");
      }
}

static void
generate_index_inits(collapse_data data)
{
  unsigned i;

  for(i = 0; i < data->loops.length(); ++i)
  {
    gimple_stmt_iterator  gsi = data->loops[i]->gsi;

    generate_ctrl_var_init(&gsi, gsi_stmt(gsi), data, i);
  }
}

/* convert nesting loops to straight line code*/
static void
parallelize_loop(struct loop* loop, unsigned collapse)
{
  unsigned i;
  struct tree_niter_desc niter_desc;
  struct loop* l = loop;
  struct collapse_data_t data;

  gcc_checking_assert(l);
  data.loops.create(5);
  data.workitem_id = NULL_TREE;

  for(i = 0; i < collapse; ++i)
  {
    bool have_niter;
    collapse_loop_data loop_data;

    niter_desc.niter = NULL_TREE;
    have_niter = get_loop_iteration_count(l, &niter_desc);
    if(i > 0 && !have_niter)
    {
      warning_at(guess_loop_location(l), OPT_fopenacc,
        "cannot collapse loop: number of iterations is unknown");
      break;
    }
    loop_data = new_collapse_loop_data(niter_desc.niter);
    collapse_loop(l, loop_data);
    data.loops.safe_push(loop_data);
    l = l->inner;
    if(l == NULL)
    {
      if(i < collapse - 1)
      {
        warning_at(guess_loop_location(loop), OPT_fopenacc,
          "no more loops to collapse");
      }
      break;
    }
  }

  generate_index_inits(&data);

  for(i = 0; i < data.loops.length(); ++i)
  {
    delete_collapse_loop_data(data.loops[i]);
  }
}

/* parallelize loops */
static void
parallelize_loops(struct loop* root, unsigned collapse)
{
    if(root->inner == NULL)
        {
            return;
        }

    calculate_dominance_info (CDI_DOMINATORS);
    fix_loop_structure(NULL);
    scev_initialize();
    struct loop* ploop;
    for(ploop = root->inner; ploop != NULL; ploop = ploop->next)
        {
            parallelize_loop(ploop, collapse);
        }
    scev_finalize();
}

/* set child function as current */
static void
switch_to_child_func(struct function *child_cfun)
{
  push_cfun(child_cfun);
  do_update_ssa();
  loop_optimizer_init (LOOPS_NORMAL | LOOPS_HAVE_RECORDED_EXITS);
  rewrite_into_loop_closed_ssa (NULL, TODO_update_ssa);
  scev_initialize ();
}

/* switch back current function */
static void
switch_func_back(int save_opt_level)
{
  scev_finalize();
  if(save_opt_level >= 0)
    {
      optimize = save_opt_level;
    }
  cleanup_tree_cfg();
  do_update_ssa();
  pop_cfun();
}

/* calculate kernel's schedule according to clauses */
static void
schedule_kernel(oacc_schedule sched, tree niter, tree clause, oacc_kernel kernel)
{
  tree num_gangs = NULL_TREE,
    num_workers = NULL_TREE,
    vector_length = NULL_TREE;
  unsigned i;

  while(clause != NULL_TREE)
  {
    switch(OACC_CLAUSE_CODE(clause))
    {
    case OACC_CLAUSE_NUM_GANGS:
      num_gangs = OACC_CLAUSE_NUM_GANGS_EXPR(clause);
      break;
    case OACC_CLAUSE_NUM_WORKERS:
      num_workers = OACC_CLAUSE_NUM_WORKERS_EXPR(clause);
      break;
    case OACC_CLAUSE_VECTOR_LENGTH:
      vector_length = OACC_CLAUSE_VECTOR_LENGTH_EXPR(clause);
      break;
    default:
      break;
    }
    clause = OACC_CLAUSE_CHAIN(clause);
  }

  for(i = 0; i < kernel->clauses.length(); ++i)
    {
      clause = kernel->clauses[i];
      while(clause != NULL_TREE)
        {
          switch(OACC_CLAUSE_CODE(clause))
            {
            case OACC_CLAUSE_GANG:
              if(OACC_CLAUSE_GANG_EXPR(clause) != NULL_TREE)
                num_gangs = OACC_CLAUSE_GANG_EXPR(clause);
              break;
            case OACC_CLAUSE_WORKER:
              if(OACC_CLAUSE_WORKER_EXPR(clause) != NULL_TREE)
                num_workers = OACC_CLAUSE_WORKER_EXPR(clause);
              break;
            case OACC_CLAUSE_VECTOR:
              if(OACC_CLAUSE_VECTOR_EXPR(clause) != NULL_TREE)
                vector_length = OACC_CLAUSE_VECTOR_EXPR(clause);
              break;
            default:
              break;
            }
          clause = OACC_CLAUSE_CHAIN(clause);
        }
    }
  
  sched->items = integer_one_node;
  sched->tiling = NULL;
  sched->group_size = integer_minus_one_node;

  /*
    We currently do not use 'num_workers' clause hint.
    This clause has its origin from NVIDIA warps,
    OpenCL on the other hand doesn't support this concept.
  */

  if(num_gangs != NULL_TREE)
  {
    tree workitems = num_gangs;

    if(niter == NULL_TREE)
    {
      sched->items = workitems;
    }
    else
    {
      sched->items = niter;
      sched->tiling = workitems;
    }
  }
  else if(niter != NULL_TREE)
  {
    sched->items = niter;
  }

  if(vector_length != NULL_TREE)
  {
    sched->group_size = vector_length;
  }
}

/* find clause for given VAR */
static tree
find_clause(tree var, gimple stmt)
{
  tree clause = NULL_TREE;

  for(clause = GIMPLE_OACC_CLAUSES(stmt); clause != NULL_TREE;
      clause = OACC_CLAUSE_CHAIN(clause))
      {
         tree v = OACC_CLAUSE_DECL(clause);
         enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
         
         if(v != var)
           continue;
         
         switch(code)
           {
           case OACC_CLAUSE_COPY:
           case OACC_CLAUSE_CREATE:
           case OACC_CLAUSE_PRESENT:
           case OACC_CLAUSE_PRESENT_OR_COPY:
           case OACC_CLAUSE_PRESENT_OR_CREATE:
           case OACC_CLAUSE_COPYIN:
           case OACC_CLAUSE_PRESENT_OR_COPYIN:
           case OACC_CLAUSE_PRESENT_OR_COPYOUT:
           case OACC_CLAUSE_COPYOUT:
               return clause;
           default:
             break;
           }
      }

  return NULL_TREE;
}

/* apply clause to data item */
static void
apply_clause(tree clause, copy_data_item_t *item)
{
  enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
  
  switch(code)
    {
    case OACC_CLAUSE_PRESENT:
      item->to_do = OACC_CF_NONE;
      break;
    case OACC_CLAUSE_COPY:
    case OACC_CLAUSE_COPYIN:
    case OACC_CLAUSE_COPYOUT:
    case OACC_CLAUSE_PRESENT_OR_COPY:
    case OACC_CLAUSE_PRESENT_OR_COPYIN:
    case OACC_CLAUSE_PRESENT_OR_COPYOUT:
      item->to_do = OACC_CF_COPY;
      break;
    case OACC_CLAUSE_CREATE:
    case OACC_CLAUSE_PRESENT_OR_CREATE:
      item->to_do = OACC_CF_CREATE;
    default:
      break;
    }

  switch(code)
    {
    case OACC_CLAUSE_PRESENT:
    case OACC_CLAUSE_PRESENT_OR_COPY:
    case OACC_CLAUSE_PRESENT_OR_COPYIN:
    case OACC_CLAUSE_PRESENT_OR_COPYOUT:
    case OACC_CLAUSE_PRESENT_OR_CREATE:
      item->check_presence = true;
      break;
    default:
      break;
    }
}

/* generate copyin list */
static void
create_copyin_list(gimple stmt, copyin_data data)
{
  struct pointer_set_t *vars;
  tree clause = NULL_TREE;
  
  vars = pointer_set_create();
  data->data.create(3);
  
  for(unsigned i = 0; i < gimple_oacc_nparams(stmt); ++i)
    {
      copy_data_item_t item;
      tree arg = GIMPLE_OACC_PARAMS_PTR(stmt)[i];
      
      clause = NULL_TREE;
      if(pointer_set_contains(vars, (void*)arg))
        {
          continue;
        }
      pointer_set_insert(vars, (void*)arg);
      item.datum = arg;
      item.is_arg = true;
      clause = find_clause(arg, stmt);
      if(clause != NULL_TREE)
        {
           enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
            if(code == OACC_CLAUSE_COPYOUT
               || code == OACC_CLAUSE_PRESENT_OR_COPYOUT)
              {
                item.to_do = OACC_CF_CREATE;
                if(code == OACC_CLAUSE_PRESENT_OR_COPYOUT)
                  item.check_presence = true;
              }
            else
                apply_clause(clause, &item);
        }
      else
        {
          item.to_do = OACC_CF_COPY;
          item.check_presence = true;
        }
      data->data.safe_push(item);
      if(item.to_do != OACC_CF_NONE)
        data->event_count++;
    }
  
  for(clause = GIMPLE_OACC_CLAUSES(stmt); clause != NULL_TREE;
      clause = OACC_CLAUSE_CHAIN(clause))
      {
        copy_data_item_t item;
        tree var = OACC_CLAUSE_DECL(clause);
        enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
        if(pointer_set_contains(vars, (void*)var))
          {
            continue;
          }
        item.datum = var;
        switch(code)
          {
          case OACC_CLAUSE_COPY:
          case OACC_CLAUSE_COPYIN:
          case OACC_CLAUSE_CREATE:
          case OACC_CLAUSE_PRESENT:
          case OACC_CLAUSE_PRESENT_OR_COPY:
          case OACC_CLAUSE_PRESENT_OR_COPYIN:
          case OACC_CLAUSE_PRESENT_OR_CREATE:
            apply_clause(clause, &item);
            break;
          case OACC_CLAUSE_COPYOUT:
          case OACC_CLAUSE_PRESENT_OR_COPYOUT:
            item.to_do = OACC_CF_CREATE;
            if(code == OACC_CLAUSE_PRESENT_OR_COPYOUT)
              item.check_presence = true;
            break;
          default:
            continue;
          }
        pointer_set_insert(vars, (void*)var);
        data->data.safe_push(item);
        if(item.to_do != OACC_CF_NONE)
          data->event_count++;
      }
  
  pointer_set_destroy(vars);
}

/* generate copyout list */
static void
create_copyout_list(gimple stmt, copyout_data data)
{
  struct pointer_set_t *vars;
  tree clause = NULL_TREE;
  
  vars = pointer_set_create();
  data->data.create(3);
  
  for(unsigned i = 0; i < gimple_oacc_nparams(stmt); ++i)
    {
      copy_data_item_t item;
      tree arg = GIMPLE_OACC_PARAMS_PTR(stmt)[i];
      
      clause = NULL_TREE;
      if(pointer_set_contains(vars, (void*)arg))
        {
          continue;
        }
      pointer_set_insert(vars, (void*)arg);
      item.datum = arg;
      item.is_arg = true;
      clause = find_clause(arg, stmt);
      if(clause != NULL_TREE)
        {
            enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
            if(code == OACC_CLAUSE_COPY
               || code == OACC_CLAUSE_PRESENT_OR_COPY
               || code == OACC_CLAUSE_COPYOUT
               || code ==  OACC_CLAUSE_PRESENT_OR_COPYOUT)
              {
                apply_clause(clause, &item);
                data->data.safe_push(item);
              }
        }
      else
        {
          item.to_do = OACC_CF_COPY;
          data->data.safe_push(item);
        }
    }
  
  for(clause = GIMPLE_OACC_CLAUSES(stmt); clause != NULL_TREE;
      clause = OACC_CLAUSE_CHAIN(clause))
      {
        copy_data_item_t item;
        tree var = OACC_CLAUSE_DECL(clause);
        enum oacc_clause_code code = OACC_CLAUSE_CODE(clause);
        if(pointer_set_contains(vars, (void*)var))
          {
            continue;
          }
        item.datum = var;
        switch(code)
          {
          case OACC_CLAUSE_COPY:
          case OACC_CLAUSE_COPYOUT:
          case OACC_CLAUSE_PRESENT_OR_COPY:
          case OACC_CLAUSE_PRESENT_OR_COPYOUT:
            apply_clause(clause, &item);
            pointer_set_insert(vars, (void*)var);
            data->data.safe_push(item);
            break;
          default:
            continue;
          }
      }
  
  pointer_set_destroy(vars);
}

/* generate code for region start */
static void
generate_region_start(gimple_stmt_iterator* gsi, location_t locus)
{
  gsi_replace( gsi, gimple_build_nop (), false);  

  if(flag_enable_openacc_profiling)
    {
      /* OACC_start_profiling */
      gen_add(gsi, build_call(locus,
        builtin_decl_explicit(BUILT_IN_GOACC_START_PROFILING), 0));
    }

  /* OACC_check_cur_dev */
  gen_add(gsi, build_call(locus,
    builtin_decl_explicit(BUILT_IN_GOACC_CHECK_CUR_DEV), 0));
}

/* generate code for kernels' handles creation */
static void
generate_get_kernel_handles(gimple_stmt_iterator* gsi, location_t locus,
                            vec<oacc_kernel>* kernels)
{
  unsigned i;
  gimple call_stmt;

  /* OACC_get_kernel */
  for(i = 0; i < kernels->length(); ++i) 
    {
      call_stmt = build_call(locus,
               builtin_decl_explicit(BUILT_IN_GOACC_GET_KERNEL), 2,
               build_string_literal(strlen(ocl_module)+1, ocl_module),
               build_string_literal(
                   strlen(IDENTIFIER_POINTER(DECL_NAME((*kernels)[i]->func)))+1,
                   IDENTIFIER_POINTER(DECL_NAME((*kernels)[i]->func))));
      gimple_call_set_lhs(call_stmt, (*kernels)[i]->kernel_handle);
      gen_add(gsi, call_stmt);
    }
}

/* generate code for event queue */
static void
generate_create_event_queue(gimple_stmt_iterator* gsi, location_t locus,
                            tree queue_handle, const char* src_file,
                            int src_line)
{
  gimple call_stmt;

  call_stmt = build_call(locus,
           builtin_decl_explicit(BUILT_IN_GOACC_CREATE_EVENTS), 2,
           build_string_literal(strlen(src_file)+1, src_file),
           build_int_cst(uint32_type_node, src_line));
  gimple_call_set_lhs(call_stmt, queue_handle);
  gen_add(gsi, call_stmt);
}

/* generate code for enqueue events  */
static void
generate_enqueue_events(gimple_stmt_iterator* gsi, location_t locus,
                        tree queue_handle, unsigned nevents, int event_type)
{
  if(nevents == 0)
    return;
  gen_add(gsi, build_call(locus,
        builtin_decl_explicit(BUILT_IN_GOACC_ENQUEUE_EVENTS), 3,
        queue_handle, ((nevents == 1) ? integer_one_node :
                        build_int_cst(uint32_type_node, nevents)),
        build_int_cst(uint32_type_node, event_type)));
}

/* generate code for advance event queue */
static void
generate_advance_events(gimple_stmt_iterator* gsi, location_t locus,
                        tree queue_handle)
{
  gen_add(gsi, build_call(locus,
              builtin_decl_explicit(BUILT_IN_GOACC_ADVANCE_EVENTS), 1,
              queue_handle));
}

/* generate copyin call */
static void
generate_copyin(gimple_stmt_iterator* gsi, location_t locus, tree arg,
                tree size, tree check_presence, tree index,
                tree queue_handle, tree buffer_handle)
{
  gimple call_stmt;

  call_stmt = build_call(locus,
     builtin_decl_explicit(BUILT_IN_GOACC_COPYIN), 5,
     arg, size, check_presence, queue_handle, index);
  gimple_call_set_lhs(call_stmt, buffer_handle);
  gen_add(gsi, call_stmt);
}

/* generate check present call */
static void
generate_check_present(gimple_stmt_iterator* gsi, location_t locus, tree arg,
                       tree buffer_handle)
{
  gimple call_stmt;

  call_stmt = build_call(locus,
     builtin_decl_explicit(BUILT_IN_GOACC_CHECK_PRESENT), 1, arg);
  gimple_call_set_lhs(call_stmt, buffer_handle);
  gen_add(gsi, call_stmt);
}

/* generate create on device call */
static void
generate_create(gimple_stmt_iterator* gsi, location_t locus, tree arg,
                tree size, tree check_presence, tree index,
                tree queue_handle, tree buffer_handle)
{
  gimple call_stmt;

  call_stmt = build_call(locus,
     builtin_decl_explicit(BUILT_IN_GOACC_CREATE_ON_DEVICE), 5,
     arg, size, check_presence, queue_handle, index);
  gimple_call_set_lhs(call_stmt, buffer_handle);
  gen_add(gsi, call_stmt);
}

/* generate set kernel's args call */
static void
generate_set_arg(gimple_stmt_iterator* gsi, location_t locus, tree kernel_handle,
                 tree index, tree buffer_handle)
{
  /* OACC_set_kernel_arg */
  gen_add(gsi, build_call(locus,
      builtin_decl_explicit(BUILT_IN_GOACC_SET_KERNEL_ARG), 3,
      kernel_handle, index, buffer_handle));
}

/* generate copyout call */
static void
generate_copyout(gimple_stmt_iterator* gsi, location_t locus, tree arg,
                tree size, tree check_presence, tree index,
                tree queue_handle)
{
  /* OACC_copyout */
  gen_add(gsi, build_call(locus,
      builtin_decl_explicit(BUILT_IN_GOACC_COPYOUT), 5,
      arg, size, check_presence, queue_handle, index));
}

/* generate wait events call */
static void
generate_wait_events(gimple_stmt_iterator* gsi, location_t locus,
                     tree queue_handle)
{
  gen_add(gsi, build_call(locus,
              builtin_decl_explicit(BUILT_IN_GOACC_WAIT_EVENTS), 1,
              queue_handle));
}

/* generate start kernel call */
static void
generate_start_kernel(gimple_stmt_iterator* gsi, location_t locus,
                      tree kernel_handle, tree workitems,
                      tree offset, tree groupsize, tree queue_handle)
{
  /* OACC_start_kernel */
  gen_add(gsi, build_call(locus,
                builtin_decl_explicit(BUILT_IN_GOACC_START_KERNEL), 6,
                kernel_handle, workitems, offset, groupsize,
                queue_handle, integer_zero_node));
}

/* tile loop: generate outer loop for calling kernel */
static void
tile_the_loop(gimple_stmt_iterator* gsi, location_t locus, tree kernel_handle,
              oacc_schedule_t* sched, tree queue_handle)
{
  gimple stmt, init_stmt, init_t_stmt;
  basic_block bb;
  edge new_edge, e, e_counter, e_wi_1, e_wi_0;
  basic_block new_bb, header_bb, body_0_bb, body_1_bb, body_2_bb;
  struct loop* l;
  tree counter, counter_1, counter_2, counter_3;
  tree conv_var;
  tree wi, wi_1, wi_2, wi_3;
  tree tmp_var;

  /* split block at call position*/
  stmt = gsi_stmt(*gsi);
  bb = gimple_bb(stmt);
  new_edge = split_block(bb, stmt);

  /* create new empty basic blocks for body of loop and header */
  new_bb = new_edge->dest;
  body_0_bb = create_empty_bb(bb);
  body_1_bb = create_empty_bb(body_0_bb);
  body_2_bb = create_empty_bb(body_1_bb);
  header_bb = create_empty_bb(body_2_bb);

  /* link blocks by edges */
  new_edge = redirect_edge_and_branch(new_edge, header_bb);
  e = make_edge(header_bb, body_0_bb, EDGE_DFS_BACK | EDGE_TRUE_VALUE);
  e = make_edge(header_bb, new_bb, EDGE_FALSE_VALUE);
  e = make_edge(body_0_bb, body_1_bb, EDGE_TRUE_VALUE);
  e_wi_0 = make_edge(body_0_bb, body_2_bb, EDGE_FALSE_VALUE);
  e_wi_1 = make_edge(body_1_bb, body_2_bb, EDGE_FALLTHRU);
  e_counter = make_single_succ_edge(body_2_bb, header_bb, EDGE_FALLTHRU);

  /* set dominators */
  set_immediate_dominator(CDI_DOMINATORS, header_bb, bb);
  set_immediate_dominator(CDI_DOMINATORS, new_bb, header_bb);
  set_immediate_dominator(CDI_DOMINATORS, body_0_bb, header_bb);
  set_immediate_dominator(CDI_DOMINATORS, body_1_bb, body_0_bb);
  set_immediate_dominator(CDI_DOMINATORS, body_2_bb, body_0_bb);

  /* we're creating new loop */
  l = alloc_loop();
  l->header = header_bb;
  header_bb->loop_father = l;
  l->latch = body_2_bb;
  body_2_bb->loop_father = l;
  body_0_bb->loop_father = l;
  body_1_bb->loop_father = l;


  place_new_loop(cfun, l);
  flow_loop_tree_node_add(bb->loop_father, l);
  
  /* create loop control variable and SSA versions for it*/
  counter = create_tmp_reg(intSI_type_node, "_oacc_counter");
  counter_1 = make_ssa_name(counter, NULL);
  init_stmt = gimple_build_assign(counter_1, integer_zero_node);
  gen_add(gsi, init_stmt);

  conv_var = generate_unsigned_typecast(gsi, TREE_TYPE(counter),
                sched->items, NULL);

  counter_2 = make_ssa_name(counter, init_stmt);
  counter_3 = make_ssa_name(counter, init_stmt);

  /* workitems variable */
  *gsi = gsi_start_bb(body_0_bb);
  wi = create_tmp_reg(TREE_TYPE(sched->tiling), "_oacc_tiling");
  wi_1 = make_ssa_name(wi, NULL);
  init_t_stmt = gimple_build_assign(wi_1, sched->tiling);
  wi_2 = make_ssa_name(wi, init_t_stmt);
  wi_3 = make_ssa_name(wi, init_t_stmt);
  gen_add(gsi, init_t_stmt);

  tmp_var = create_tmp_reg(intSI_type_node, "_oacc_tmp");
  stmt = build_assign(MINUS_EXPR, conv_var, counter_2);
  set_gimple_def_var(stmt, tmp_var);
  gen_add(gsi, stmt);

  gen_add(gsi, gimple_build_cond(LT_EXPR, tmp_var, sched->tiling,
                  NULL, NULL));

  *gsi = gsi_start_bb(body_1_bb);
  stmt = gimple_build_assign(wi_2, tmp_var);
  gen_add(gsi, stmt);
  

  *gsi = gsi_start_bb(body_2_bb);
  stmt = create_phi_node(wi_3, body_2_bb);
  add_phi_arg(stmt, wi_1, e_wi_0, locus);
  add_phi_arg(stmt, wi_2, e_wi_1, locus);
  generate_start_kernel(gsi, locus, kernel_handle,
                wi_3,               /* WORKITEMS */
                counter_2,          /* OFFSET */
                sched->group_size,  /* GROUPSIZE */
                queue_handle);
  stmt = build_assign(PLUS_EXPR, counter_2, sched->tiling);
  set_gimple_def_var(stmt, counter_3);
  gen_add(gsi, stmt);

  *gsi = gsi_start_bb(header_bb);
  stmt = create_phi_node(counter_2, header_bb);
  add_phi_arg(stmt, counter_1, new_edge, locus);
  add_phi_arg(stmt, counter_3, e_counter, locus);

  gen_add(gsi, gimple_build_cond(LT_EXPR, counter_2, conv_var,
                    NULL, NULL));
  *gsi = gsi_start_bb(new_bb);
}

/* in the case of collapsing generate multiplication of collapsed loop bounds */
static tree
get_workitem_count(gimple_stmt_iterator *gsi, vec<tree>* nitems)
{
    gimple stmt0;
    tree count = NULL_TREE, count0;
    unsigned i;

    gcc_checking_assert(nitems->length() > 0);

    if(nitems->length() == 1)
      return (*nitems)[0];
  
    count = create_tmp_reg(TREE_TYPE((*nitems)[0]), "_oacc_tmp");
    stmt0 = build_assign(MULT_EXPR, (*nitems)[0], (*nitems)[1]);
    set_gimple_def_var(stmt0, count);
    gen_add(gsi, stmt0);
  
    count0 = count;
    for(i = 2; i < nitems->length(); ++i)
      {
        gimple stmt;
        tree count_n = make_ssa_name(count0, stmt0);
        
        stmt = build_assign(MULT_EXPR, count, (*nitems)[i]);
        set_gimple_def_var(stmt, count_n);
        gen_add(gsi, stmt);
        count = count_n;
      }
  
  return count;
}

/* generate kernels and calls to run-time library */
static void
expand_oacc_kernels(gimple_stmt_iterator* gsi)
{
    unsigned i, j, idx;
    gimple stmt = gsi_stmt(*gsi);
    tree sched_clauses = ((gimple_code(stmt) == GIMPLE_OACC_PARALLEL)
      ? gimple_oacc_parallel_clauses(stmt) : NULL_TREE);

    tree child_fn = GIMPLE_OACC_CHILD_FN(stmt);
    struct function* child_cfun = DECL_STRUCT_FUNCTION(child_fn);
    struct loops* loops = NULL;
    vec<oacc_kernel> kernels;
    tree type, buffer_handle, queue_handle, bits_per_byte;
    location_t locus;
    const char* src_file = NULL;
    int src_line = 0;
    copyin_data_t cpin;
    copyout_data_t cpout;

    
    /* if directive was kernels split region into separate child functions */
    if(optimize < 1)
      {
          save_opt_level = optimize;
          ++optimize;
      }
    switch_to_child_func(child_cfun);

    loops = current_loops;
    gcc_checking_assert(loops != NULL && loops->tree_root != NULL);

    kernels.create(3);
    extract_kernels(loops, child_fn, &kernels, stmt);
    switch_func_back(save_opt_level);

    /* generate handles for run time*/
    type = build_pointer_type(void_type_node);
    for(i = 0; i < kernels.length(); ++i)
    {
      kernels[i]->kernel_handle = create_tmp_reg(type, "_oacc_tmp");
    }
    buffer_handle = create_tmp_reg(type, "_oacc_tmp");
    queue_handle = create_tmp_reg(type, "_oacc_tmp");

    locus = gimple_location(stmt);
    src_file = LOCATION_FILE(locus);
    src_line = LOCATION_LINE(locus);
    if(dump_file)
      {
          fprintf(dump_file, "Exec region at file: '%s', line %d\n",
                        src_file, src_line);
      }

    /* create copyin/copyout lists */
    create_copyin_list(stmt, &cpin);
    create_copyout_list(stmt, &cpout);

    if(dump_file)
      {
        unsigned i;
        fprintf(dump_file, "copyin list:\n");
        for(i = 0; i < cpin.data.length(); ++i)
          {
            print_generic_expr(dump_file, cpin.data[i].datum, 0);
            fprintf(dump_file, " %u, %d, %d\n", cpin.data[i].to_do,
                    cpin.data[i].check_presence, cpin.data[i].is_arg);
          }
        fprintf(dump_file, "copyout list:\n");
        for(i = 0; i < cpout.data.length(); ++i)
          {
            print_generic_expr(dump_file, cpout.data[i].datum, 0);
            fprintf(dump_file, " %u, %d, %d\n", cpout.data[i].to_do,
                    cpout.data[i].check_presence, cpout.data[i].is_arg);
          }
      }

    /*start compute region */
    generate_region_start(gsi, locus);
    generate_get_kernel_handles(gsi, locus, &kernels);
    generate_create_event_queue(gsi, locus, queue_handle, src_file, src_line);

    generate_enqueue_events(gsi, locus, queue_handle,
        cpin.event_count, OACC_PF_DATAIN);
    for(i = 0; i < kernels.length(); ++i)
      {
        generate_enqueue_events(gsi, locus, queue_handle, 1, OACC_PF_EXEC);
      }

    generate_enqueue_events(gsi, locus, queue_handle,
        cpout.data.length(), OACC_PF_DATAOUT);

    bits_per_byte = build_int_cst(uint32_type_node, 8);

    /* copy in data */
    idx = 0;
    for(i = 0; i < cpin.data.length(); ++i)
      {
          tree arg = cpin.data[i].datum;
          tree type, size;
          tree check_presence = (cpin.data[i].check_presence) ? 
              integer_zero_node : integer_one_node;
          
          if(is_gimple_reg(arg))
            continue;
          type = TREE_TYPE(arg);
          size = TYPE_SIZE(type);

          switch(cpin.data[i].to_do)
            {
            case OACC_CF_NONE:
              if(cpin.data[i].check_presence)
                {
                  generate_check_present(gsi, locus, build_fold_addr_expr(arg),
                                         buffer_handle);
                }
              break;
            case OACC_CF_COPY:
                generate_copyin(gsi, locus, build_fold_addr_expr(arg),
                      fold_binary(TRUNC_DIV_EXPR, uint32_type_node,
                              size, bits_per_byte),
                      check_presence, build_int_cst(uint32_type_node, idx++),
                queue_handle, buffer_handle);
                break;
            case OACC_CF_CREATE:
                generate_create(gsi, locus, build_fold_addr_expr(arg),
                      fold_binary(TRUNC_DIV_EXPR, uint32_type_node,
                              size, bits_per_byte),
                      check_presence, build_int_cst(uint32_type_node, idx++),
                queue_handle, buffer_handle);
              break;
            default:
              gcc_unreachable();
            }
          if(cpin.data[i].is_arg) {
            for(j = 0; j < kernels.length(); ++j)
              {
                generate_set_arg(gsi, locus, kernels[j]->kernel_handle,
                  build_int_cst(uint32_type_node, i), buffer_handle);
              }
            }
      }

    /* synchronize */
    if(cpin.event_count > 0)
        generate_advance_events(gsi, locus, queue_handle);

    /* start kernels */
    for(i = 0; i < kernels.length(); ++i)
    {
      oacc_schedule_t sched;
      tree nworkitems = get_workitem_count(gsi, &(kernels[i]->niters));
      
      memset(&sched, sizeof(oacc_schedule_t), 0);
      schedule_kernel(&sched, nworkitems, sched_clauses, kernels[i]);

      if(sched.tiling != NULL_TREE)
      {
        tile_the_loop(gsi, locus, kernels[i]->kernel_handle,
          &sched, queue_handle);
      }
      else 
      {
        generate_start_kernel(gsi, locus, kernels[i]->kernel_handle,
                      sched.items,       /* WORKITEMS */
                      integer_zero_node, /* OFFSET */
                      sched.group_size,  /* GROUPSIZE */
                      queue_handle);
      }
      generate_advance_events(gsi, locus, queue_handle);
    }

    for(i = 0; i < kernels.length(); ++i)
      delete_oacc_kernel(kernels[i]);

    /* copy out data */
    for(i = 0; i < cpout.data.length(); ++i)
        {
            tree arg = cpout.data[i].datum;
            tree type, size;

            if(is_gimple_reg(arg))
              continue;
            type = TREE_TYPE(arg);
            size = TYPE_SIZE(type);

            switch(cpout.data[i].to_do)
              {
              case OACC_CF_COPY:
                generate_copyout(gsi, locus, build_fold_addr_expr(arg),
                  fold_binary(TRUNC_DIV_EXPR, uint32_type_node, size,
                      bits_per_byte),
                  integer_one_node, build_int_cst(uint32_type_node, i),
                  queue_handle);
                break;
              default:
                gcc_unreachable();
              }
        }

    if(cpout.data.length() > 0)
        generate_advance_events(gsi, locus, queue_handle);
    generate_wait_events(gsi, locus, queue_handle);

}

/* generate data region */
static void
expand_oacc_data(gimple_stmt_iterator* gsi)
{
    unsigned i;
    gimple stmt = gsi_stmt(*gsi);
    tree type, buffer_handle, queue_handle, bits_per_byte;
    location_t locus;
    const char* src_file = NULL;
    int src_line = 0;
    copyin_data_t cpin;

    type = build_pointer_type(void_type_node);
    buffer_handle = create_tmp_reg(type, "_oacc_tmp");
    queue_handle = create_tmp_reg(type, "_oacc_tmp");

    locus = gimple_location(stmt);
    src_file = LOCATION_FILE(locus);
    src_line = LOCATION_LINE(locus);
    if(dump_file)
      {
          fprintf(dump_file, "Data region start at file: '%s', line %d\n",
                        src_file, src_line);
      }
    create_copyin_list(stmt, &cpin);

    if(dump_file)
      {
        fprintf(dump_file, "copyin list:\n");
        for(i = 0; i < cpin.data.length(); ++i)
          {
            print_generic_expr(dump_file, cpin.data[i].datum, 0);
            fprintf(dump_file, " %u, %d, %d\n", cpin.data[i].to_do,
                    cpin.data[i].check_presence, cpin.data[i].is_arg);
          }
      }
    
    generate_region_start(gsi, locus);
    generate_create_event_queue(gsi, locus, queue_handle, src_file, src_line);

    generate_enqueue_events(gsi, locus, queue_handle,
        cpin.data.length(), OACC_PF_DATAIN);

   bits_per_byte = build_int_cst(uint32_type_node, 8);

    for(i = 0; i < cpin.data.length(); ++i)
      {
          tree arg = cpin.data[i].datum;
          tree type, size;
          tree check_presence = (cpin.data[i].check_presence) ? 
              integer_zero_node : integer_one_node;
          if(is_gimple_reg(arg))
            continue;
          type = TREE_TYPE(arg);
          size = TYPE_SIZE(type);

          switch(cpin.data[i].to_do)
            {
            case OACC_CF_NONE:
              if(cpin.data[i].check_presence)
                {
                  generate_check_present(gsi, locus, build_fold_addr_expr(arg),
                                         buffer_handle);
                }
              break;
            case OACC_CF_COPY:
                generate_copyin(gsi, locus, build_fold_addr_expr(arg),
                      fold_binary(TRUNC_DIV_EXPR, uint32_type_node,
                              size, bits_per_byte),
                      check_presence, build_int_cst(uint32_type_node, i),
                queue_handle, buffer_handle);
                break;
            case OACC_CF_CREATE:
                generate_create(gsi, locus, build_fold_addr_expr(arg),
                      fold_binary(TRUNC_DIV_EXPR, uint32_type_node,
                              size, bits_per_byte),
                      check_presence, build_int_cst(uint32_type_node, i),
                queue_handle, buffer_handle);
              break;
            default:
              gcc_unreachable();
            }
      }

    generate_advance_events(gsi, locus, queue_handle);
    generate_wait_events(gsi, locus, queue_handle);

 }

/* generate end data region */
static void
expand_oacc_end_data_region(gimple_stmt_iterator* gsi, gimple start_stmt)
{
    unsigned i;
    gimple stmt = gsi_stmt(*gsi);
    tree type, buffer_handle, queue_handle, bits_per_byte;
    location_t locus;
    const char* src_file = NULL;
    int src_line = 0;
    copyout_data_t cpout;

    type = build_pointer_type(void_type_node);
    buffer_handle = create_tmp_reg(type, "_oacc_tmp");
    queue_handle = create_tmp_reg(type, "_oacc_tmp");

    locus = gimple_location(start_stmt);
    src_file = LOCATION_FILE(locus);
    src_line = LOCATION_LINE(locus);
    if(dump_file)
      {
          fprintf(dump_file, "Data region end at file: '%s', line %d\n",
                        src_file, src_line);
      }
    create_copyout_list(start_stmt, &cpout);

    if(dump_file)
      {
        fprintf(dump_file, "copyout list:\n");
        for(i = 0; i < cpout.data.length(); ++i)
          {
            print_generic_expr(dump_file, cpout.data[i].datum, 0);
            fprintf(dump_file, " %u, %d, %d\n", cpout.data[i].to_do,
                    cpout.data[i].check_presence, cpout.data[i].is_arg);
          }
      }
    
    generate_region_start(gsi, locus);
    generate_create_event_queue(gsi, locus, queue_handle, src_file, src_line);
    generate_enqueue_events(gsi, locus, queue_handle,
        cpout.data.length(), OACC_PF_DATAOUT);

    bits_per_byte = build_int_cst(uint32_type_node, 8);
    for(i = 0; i < cpout.data.length(); ++i)
        {
            tree arg = cpout.data[i].datum;
            tree type, size;

            if(is_gimple_reg(arg))
              continue;
            type = TREE_TYPE(arg);
            size = TYPE_SIZE(type);

            switch(cpout.data[i].to_do)
              {
              case OACC_CF_NONE:
                break;
              case OACC_CF_COPY:
                generate_copyout(gsi, locus, build_fold_addr_expr(arg),
                  fold_binary(TRUNC_DIV_EXPR, uint32_type_node, size,
                      bits_per_byte),
                  integer_one_node, build_int_cst(uint32_type_node, i),
                  queue_handle);
                break;
              default:
                gcc_unreachable();
              }
        }

    generate_advance_events(gsi, locus, queue_handle);
    generate_wait_events(gsi, locus, queue_handle);
}

static void
expand_oacc_end_compute_region(gimple_stmt_iterator* gsi)
{
  gsi_replace (gsi, gimple_build_nop (), false);
}

/* generate OpenCL */
static void
generate_opencl(void)
{
    rewrite_out_of_ssa(&SA);
    cleanup_tree_cfg();
    {
      basic_block bb;
      gimple_stmt_iterator gsi;
      
      FOR_EACH_BB(bb)
      {
        for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
          {
            gimple stmt = gsi_stmt(gsi);
            if(is_gimple_oacc(stmt))
              {
                gsi_replace(&gsi, gimple_build_nop(), true);
              }
          }
      }
    }
    generate_opencl_kernel(ocl_module, current_function_decl, &SA);
    finish_out_of_ssa(&SA);
}

/* if we generated OenCL code, function body isn't needed */
static void
finish_current_fn(void)
{
    if(dump_file)
        {
            fprintf(dump_file, "gimple_body=%p\n", (void*)cfun->gimple_body);
            fprintf(dump_file, "gimple_df=%p\n", (void*)cfun->gimple_df);
            fprintf(dump_file, "cfg=%p\n", (void*)cfun->cfg);
        }
    basic_block bb;
    unsigned i;

    for(i = 2; i < basic_block_info->length() - 2; ++i)
    {
      bb = (*basic_block_info)[i];
      unlink_block(bb);
    }

}

/* dispatch expansion based on region type */
static void
expand_region (oacc_region region)
{
  gimple stmt;
  gimple_stmt_iterator gsi;

  stmt = region->stmt;
  switch(gimple_code(stmt))
    {
    case GIMPLE_OACC_PARALLEL:
    case GIMPLE_OACC_KERNELS:
      gsi = gsi_for_stmt(stmt);
      expand_oacc_kernels(&gsi);
      gcc_checking_assert(region->end_stmt);
      gsi = gsi_for_stmt(region->end_stmt);
      expand_oacc_end_compute_region(&gsi);
      break;
    case GIMPLE_OACC_DATA:
      gsi = gsi_for_stmt(stmt);
      expand_oacc_data(&gsi);
      gcc_checking_assert(region->end_stmt);
      gsi = gsi_for_stmt(region->end_stmt);
      expand_oacc_end_data_region(&gsi, stmt);
      break;
    default:
      gcc_unreachable();
    }
}

/* depth-first region tree traversing */
static void
traverse_regions (oacc_region region)
{
  unsigned i;

  for(i = 0; i < region->children->length(); ++i)
  {
    traverse_regions ((*region->children)[i]);
  }
  if(region->stmt != NULL)
    expand_region(region);
}

/* build region tree */
static void
build_oacc_region(basic_block bb, oacc_region outer)
{
  gimple_stmt_iterator gsi;
  basic_block child;

  gsi = gsi_last_bb(bb);
  if(!gsi_end_p(gsi) && is_gimple_oacc(gsi_stmt(gsi)))
    {
      gimple stmt;
      oacc_region region = NULL;

      stmt = gsi_stmt(gsi);
      switch(gimple_code(stmt))
        {
        case GIMPLE_OACC_KERNELS:
        case GIMPLE_OACC_PARALLEL:
        case GIMPLE_OACC_DATA:
        case GIMPLE_OACC_HOST_DATA:
          region = new_oacc_region(stmt, outer);
          outer = region;
          break;
        case GIMPLE_OACC_COMPUTE_REGION_END:
        case GIMPLE_OACC_DATA_REGION_END:
          gcc_checking_assert(outer);
          region = outer;
          outer = outer->parent;
          GIMPLE_OACC_SET_STATEMENT(stmt, region->stmt);
          region->end_stmt = stmt;
          break;
        default:
          break;
        }
    }
  for(child = first_dom_son(CDI_DOMINATORS, bb); child != NULL;
      child = next_dom_son(CDI_DOMINATORS, child))
    build_oacc_region(child, outer);
}

/* Main entry point for expanding OpenACC.  */

static unsigned int
execute_expand_oacc (void)
{
    splay_tree_node v;

    if(kernels &&
        (v = splay_tree_lookup(kernels, DECL_UID(current_function_decl)))
        != NULL)
    {
      if((unsigned)v->value > 0)
      {
        parallelize_loops(current_loops->tree_root, (unsigned)v->value);
      }
      generate_opencl();
      return 0;
    }

    root_region = new_oacc_region(NULL, NULL);
    calculate_dominance_info (CDI_DOMINATORS);
    build_oacc_region (ENTRY_BLOCK_PTR_FOR_FN(cfun), root_region);

    if(root_region->children->length() > 0)
      {
        if(dump_file)
          {
            fprintf(dump_file, "ACC REGION TREE\n===============\n");
            dump_oacc_region(dump_file, root_region, 0);
          }
        traverse_regions(root_region);
      }
    delete_oacc_region(root_region);
    root_region = NULL;

    return 0;
}

/* OpenACC expansion */

static bool
gate_expand_oacc (void)
{
    return (flag_openacc != 0);
}


namespace
{
const pass_data pass_data_expand_oacc =
{
    GIMPLE_PASS,
    "oaccexp",				/* name */
    OPTGROUP_NONE,                        /* optinfo_flags */
    gate_expand_oacc,			/* gate */
    execute_expand_oacc,			/* execute */
    TV_NONE,				/* tv_id */
    PROP_cfg,			/* properties_required */
    0,					/* properties_provided */
    0,					/* properties_destroyed */
    0,					/* todo_flags_start */
    TODO_update_ssa                 		/* todo_flags_finish */
};

class pass_expand_oacc : public gimple_opt_pass
{
public:
    pass_expand_oacc(gcc::context *ctxt)
        :
        gimple_opt_pass(pass_data_expand_oacc,
                        ctxt)
    {}

    /* opt_pass methods: */
    bool gate ()
    {
        return gate_expand_oacc ();
    }
    unsigned int execute ()
    {
        return
            execute_expand_oacc ();
    }

}; // class pass_expand_oacc
}

gimple_opt_pass *
make_pass_expand_oacc (gcc::context *ctxt)
{
    return new pass_expand_oacc (ctxt);
}

/* Diagnose OpenACC structured block violations  */

/* Check for mismatched contexts and generate an error if needed.  Return
   true if an error is detected.  */

static bool
diagnose_cb(gimple_stmt_iterator *gsi_p, gimple branch_ctx, gimple label_ctx)
{
    if (label_ctx == branch_ctx)
        return false;
    /* If it's obvious we have an invalid entry, be specific about the error.*/
    if (branch_ctx == NULL)
        error ("invalid entry to OpenACC structured block");
    else
        /* Otherwise, be vague and lazy, but efficient.  */
        error ("invalid branch to/from an OpenACC structured block");

    gsi_replace (gsi_p, gimple_build_nop (), false);
    return true;
}

static void
get_recursive_seq (enum gimple_code code, gimple stmt, vec<gimple_seq*>* seq)
{
  switch (code)
  {
  case GIMPLE_OACC_KERNELS:
  case GIMPLE_OACC_PARALLEL:
  case GIMPLE_OACC_LOOP:
    seq->safe_push (gimple_oacc_body_ptr (stmt));
    break;
  default:
    break;
  }
}

static unsigned int
diagnose_oacc_structured_block_errors (void)
{

    vec<enum gimple_code> oacc_codes;

    oacc_codes.create(3);
    oacc_codes.quick_push(GIMPLE_OACC_KERNELS);
    oacc_codes.quick_push(GIMPLE_OACC_PARALLEL);
    oacc_codes.quick_push(GIMPLE_OACC_LOOP);

    diagnose_gotos(&oacc_codes, get_recursive_seq, diagnose_cb);
    return 0;
}

static bool
gate_diagnose_oacc_blocks (void)
{
    return flag_openacc != 0;
}


namespace
{
const pass_data pass_data_diagnose_oacc_blocks =
{
    GIMPLE_PASS,
    "*diagnose_oacc_blocks",		/* name */
    OPTGROUP_NONE,                      /* optinfo_flags */
    gate_diagnose_oacc_blocks,		/* gate */
    diagnose_oacc_structured_block_errors,	/* execute */
    TV_NONE,				/* tv_id */
    PROP_gimple_any,			/* properties_required */
    0,					/* properties_provided */
    0,					/* properties_destroyed */
    0,					/* todo_flags_start */
    0,					/* todo_flags_finish */
};

class pass_diagnose_oacc_blocks : public gimple_opt_pass
{
public:
    pass_diagnose_oacc_blocks(gcc::context *ctxt)
        : gimple_opt_pass(pass_data_diagnose_oacc_blocks, ctxt)
    {}

    /* opt_pass methods: */
    bool gate ()
    {
        return gate_diagnose_oacc_blocks ();
    }
    unsigned int execute ()
    {
        return diagnose_oacc_structured_block_errors ();
    }

}; // class pass_diagnose_oacc_blocks
}

gimple_opt_pass *
make_pass_diagnose_oacc_blocks (gcc::context *ctxt)
{
    return new pass_diagnose_oacc_blocks (ctxt);
}

#include "gt-oacc-low.h"
