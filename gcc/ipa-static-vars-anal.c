/* Callgraph based analysis of static variables.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* This file gathers information about how statics that are local to
   the compilation unit are used.  It is to be run after the call
   graph is built and after inlining decisions have been made.

   First each function is analyzed to determine which local static
   variables are either read or written or have their address taken.
   Any local static that has its address taken is removed from
   consideration.  Once the local read and writes are determined, a
   transitive closure of this information is performed over the call
   graph to determine the worst case set of side effects of each call.
   In later parts of the compiler, these local and global sets are
   examined to make the call clobbering less traumatic, promote some
   statics to registers, and improve aliasing information.

   This must be run after inlining decisions have been made since
   otherwise, the local sets will not contain information that is
   consistent with post inlined state.  The global sets are less prone
   to problems since they are by definition transitive.  */

/* The code in this module is called by the ipa pass manager. It
   should be one of the later passes since it's information is used by
   the rest of the compilation. */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "pointer-set.h"
#include "ggc.h"
#include "ipa-static.h"
#include "c-common.h"
#include "tree-gimple.h"
#include "cgraph.h"
#include "output.h"
#include "flags.h"

/* FIXME -- PROFILE-RESTRUCTURE: change comment from DECL_UID to var-ann. */    
/* This splay tree contains all of the static variables that are
   being considered by the compilation level alias analysis.  For
   module_at_a_time compilation, this is the set of static but not
   public variables.  Any variables that either have their address
   taken or participate in otherwise unsavory operations are deleted
   from this list.  */
static GTY((param1_is(int), param2_is(tree)))
     splay_tree static_vars_to_consider_by_uid;

/* This bitmap is used to knock out the module static variables whose
   addresses have been taken and passed around.  This is indexed by
   uid.  */
static bitmap module_statics_escape;

/* FIXME -- PROFILE-RESTRUCTURE: change comment from DECL_UID to var-ann. */    
/* A bit is set for every module static we are considering and is
   indexed by DECL_UID.  This is ored into the local info when asm
   code is found that clobbers all memory. */
static bitmap all_module_statics;

/* Records tree nodes seen in cgraph_create_edges.  Simply using
   walk_tree_without_duplicates doesn't guarantee each node is visited
   once because it gets a new htab upon each recursive call from
   scan_for_static_refs.  */
static struct pointer_set_t *visited_nodes;

tree memory_identifier_string;

/* Debugging function for postorder and inorder code. NOTE is a string
   that is printed before the nodes are printed.  ORDER is an array of
   cgraph_nodes that has COUNT useful nodes in it.  */

static void 
print_order (FILE* out, 
	     const char * note, 
	     struct cgraph_node** order, 
	     int count) 
{
  int i;
  fprintf (out, "\n\n ordered call graph: %s\n", note);
  
  for (i = count - 1; i >= 0; i--)
    dump_cgraph_node(dump_file, order[i]);
  fprintf (out, "\n");
  fflush(out);
}

/* FIXME -- PROFILE-RESTRUCTURE: Remove this function, it becomes a nop. */    
/* Convert IN_DECL bitmap which is indexed by DECL_UID to IN_ANN, a
   bitmap indexed by var_ann (VAR_DECL)->uid.  */

static void 
convert_UIDs_in_bitmap (bitmap in_ann, bitmap in_decl) 
{
  int index;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP(in_decl, 0, index, bi)
    {
      splay_tree_node n = 
	      splay_tree_lookup (static_vars_to_consider_by_uid, index);
      if (n != NULL) 
	{
	  tree t = (tree)n->value;
	  var_ann_t va = var_ann (t);
	  if (va) 
	    bitmap_set_bit(in_ann, va->uid);
	}
    }
}


/* FIXME -- PROFILE-RESTRUCTURE: Remove this function, it becomes a
   nop. */    
/* The bitmaps used to represent the static global variables are
   indexed by DECL_UID however, this is not used inside of functions
   to index the ssa variables.  The denser var_ann (VAR_DECL)->uid is
   used there.  This function is called from
   tree_dfa:find_referenced_vars after the denser representation is
   built.  This function invalidates any cached indexes.  */ 

void
ipa_static_reset_maps (void) 
{
  struct cgraph_node *node;
  
  for (node = cgraph_nodes; node; node = node->next)
    {
      ipa_static_vars_info_t info = node->static_vars_info;

      if (info) 
	{
	  ipa_local_static_vars_info_t l = info->local;
	  ipa_global_static_vars_info_t g = info->global;

	  if (l->var_anns_valid) 
	    {
	      bitmap_clear (l->statics_read_by_ann_uid);
	      bitmap_clear (l->statics_written_by_ann_uid);
	      l->var_anns_valid = false;
	    }

	  /* There may be orphans in the cgraph so must check that
	     there is global info.  */
	  if (g && g->var_anns_valid) 
	    {
	      bitmap_clear (g->statics_read_by_ann_uid);
	      bitmap_clear (g->statics_written_by_ann_uid);
	      bitmap_clear (g->statics_not_read_by_ann_uid);
	      bitmap_clear (g->statics_not_written_by_ann_uid);
	      g->var_anns_valid = false;
	    }
	}
    }
}

/* Get the cgraph_node for the local function and make sure the
   var_ann bitmaps are properly converted.  */
 
static ipa_local_static_vars_info_t
get_local_static_vars_info (tree fn) 
{
  ipa_local_static_vars_info_t l;

  /* Was not compiled -O2 or higher.  */ 
  ipa_static_vars_info_t info = get_var_ann (fn)->static_vars_info;

  if (!info)
    return NULL;

  l = info->local;
  if (!l->var_anns_valid) 
    {
      convert_UIDs_in_bitmap (l->statics_read_by_ann_uid, 
			      l->statics_read_by_decl_uid);
      convert_UIDs_in_bitmap (l->statics_written_by_ann_uid, 
			      l->statics_written_by_decl_uid);
      l->var_anns_valid = true;
    }
  return l;
}

/* Get the global static_vars_info structure for the function FN and
   make sure the ann_uid's bitmaps are properly converted.  */
 
static ipa_global_static_vars_info_t
get_global_static_vars_info (tree fn) 
{
  ipa_global_static_vars_info_t g;

  /* Was not compiled -O2 or higher.  */ 
  ipa_static_vars_info_t info = get_var_ann (fn)->static_vars_info;

  if (!info)
    return NULL;

  g = info->global;
  if (!g) 
    return NULL;

  if (!g->var_anns_valid) 
    {
      convert_UIDs_in_bitmap (g->statics_read_by_ann_uid, 
			      g->statics_read_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_written_by_ann_uid, 
			      g->statics_written_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_not_read_by_ann_uid, 
			      g->statics_not_read_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_not_written_by_ann_uid, 
			      g->statics_not_written_by_decl_uid);
      g->var_anns_valid = true;
    }
  return g;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that may be read locally by the execution of the function
   fn.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_read_local (tree fn)
{
  ipa_local_static_vars_info_t l = get_local_static_vars_info (fn);
  if (l) 
    return l->statics_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that may be written locally by the execution of the function
   fn.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_written_local (tree fn)
{
  ipa_local_static_vars_info_t l = get_local_static_vars_info (fn);
  if (l) 
    return l->statics_written_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are read during the execution of the function
   FN.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_read_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are written during the execution of the function
   FN.  Note that variables written may or may not be read during the
   function call.  Returns NULL if no data is available, such as it
   was not compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_written_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_written_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are not read during the execution of the function
   FN.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_not_read_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_not_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are not written during the execution of the function
   FN.  Note that variables written may or may not be read during the
   function call.  Returns NULL if no data is available, such as it
   was not compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_not_written_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_not_written_by_ann_uid;
  else
    return NULL;
}

struct searchc_env {
  struct cgraph_node **stack;
  int stack_size;
  struct cgraph_node **result;
  int order_pos;
  splay_tree nodes_marked_new;
  bool reduce;
  int count;
};

struct dfs_info {
  int dfn_number;
  int low_link;
  bool new;
  bool on_stack;
};

/* This is an implementation of Tarjan's strongly connected region
   finder as reprinted in Aho Hopcraft and Ullman's The Design and
   Analysis of Computer Programs (1975) pages 192-193.  This version
   has been customized for cgraph_nodes.  The env parameter is because
   it is recursive and there are no nested functions here.  This
   function should only be called from itself or
   cgraph_reduced_inorder.  ENV is a stack env and would be
   unnecessary if C had nested functions.  V is the node to start
   searching from.  */

static void
searchc (struct searchc_env* env, struct cgraph_node *v) 
{
  struct cgraph_edge *edge;
  struct dfs_info *v_info = v->aux;
  
  /* mark node as old */
  v_info->new = false;
  splay_tree_remove (env->nodes_marked_new, v->uid);
  
  v_info->dfn_number = env->count;
  v_info->low_link = env->count;
  env->count++;
  env->stack[(env->stack_size)++] = v;
  v_info->on_stack = true;
  
  for (edge = v->callees; edge; edge = edge->next_callee)
    {
      struct dfs_info * w_info;
      struct cgraph_node *w = edge->callee;
      /* Bypass the clones and only look at the master node.  Skip
	 external and other bogus nodes.  */
      w = cgraph_master_clone (w);
      if (w && w->aux) 
	{
	  w_info = w->aux;
	  if (w_info->new) 
	    {
	      searchc (env, w);
	      v_info->low_link =
		(v_info->low_link < w_info->low_link) ?
		v_info->low_link : w_info->low_link;
	    } 
	  else 
	    if ((w_info->dfn_number < v_info->dfn_number) 
		&& (w_info->on_stack)) 
	      v_info->low_link =
		(w_info->dfn_number < v_info->low_link) ?
		w_info->dfn_number : v_info->low_link;
	}
    }


  if (v_info->low_link == v_info->dfn_number) 
    {
      struct cgraph_node *last = NULL;
      struct cgraph_node *x;
      struct dfs_info *x_info;
      do {
	x = env->stack[--(env->stack_size)];
	x_info = x->aux;
	x_info->on_stack = false;
	
	if (env->reduce) 
	  {
	    x->next_cycle = last;
	    last = x;
	  } 
	else 
	  env->result[env->order_pos++] = x;
      } 
      while (v != x);
      if (env->reduce) 
	env->result[env->order_pos++] = v;
    }
}

/* Topsort the call graph by caller relation.  Put the result in ORDER.

   The REDUCE flag is true if you want the cycles reduced to single
   nodes.  Only consider nodes that have the output bit set. */

static int
reduced_inorder (struct cgraph_node **order, bool reduce)
{
  struct cgraph_node *node;
  struct searchc_env env;
  splay_tree_node result;
  env.stack = xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  env.stack_size = 0;
  env.result = order;
  env.order_pos = 0;
  env.nodes_marked_new = splay_tree_new (splay_tree_compare_ints, 0, 0);
  env.count = 1;
  env.reduce = reduce;
  
  for (node = cgraph_nodes; node; node = node->next) 
    if (cgraph_is_master_clone (node)
	&& (node->local.finalized))
      {
	struct dfs_info *info = xcalloc (1, sizeof (struct dfs_info));
	info->new = true;
	info->on_stack = false;
	node->aux = info;
	node->next_cycle = NULL;
	
	splay_tree_insert (env.nodes_marked_new,
			   node->uid, (splay_tree_value)node);
      } 
    else 
      node->aux = NULL;
  result = splay_tree_min (env.nodes_marked_new);
  while (result)
    {
      node = (struct cgraph_node *)result->value;
      searchc (&env, node);
      result = splay_tree_min (env.nodes_marked_new);
    }
  splay_tree_delete (env.nodes_marked_new);
  free (env.stack);

  for (node = cgraph_nodes; node; node = node->next)
    if (node->aux)
      {
	free (node->aux);
	node->aux = NULL;
      }
  return env.order_pos;
}

/* Add VAR to all_module_statics and the two static_vars_to_consider*
   sets.  */

static inline
void add_static_var (tree var) 
{
  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
     DECL_UID to get the uid from the var_ann field. */    
  splay_tree_insert (static_vars_to_consider_by_uid,
		     DECL_UID (var), (splay_tree_value)var);
  
  if (dump_file)
    fprintf (dump_file, "\nConsidering var:%s",
	     lang_hooks.decl_printable_name (var, 2));
  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
     DECL_UID to get the uid from the var_ann field. */    
  bitmap_set_bit (all_module_statics, DECL_UID (var));
}

/* FIXME this needs to be enhanced.  If we are compiling a single
   module this returns true if the variable is a module level static,
   but if we are doing whole program compilation, this could return
   true if TREE_PUBLIC is true. */
/* Return true if the variable T is the right kind of static variable to
   perform compilation unit scope escape analysis.  */

static inline
bool has_proper_scope_for_analysis (tree t)
{
  bool result = (TREE_STATIC (t)) && !(TREE_PUBLIC (t)) && !(TREE_THIS_VOLATILE (t));
  if ((result) && !bitmap_bit_p (all_module_statics, DECL_UID (t)))
    add_static_var (t);
  return result;
}

/* If T is a VAR_DECL for a static that we are interrested in, add the
   uid to the bitmap.  */

static void
check_operand (tree t, bitmap bm)
{
  if (!t) return;

  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from DECL_UID to
     get the uid from the var_ann field. */    
  if ((TREE_CODE (t) == VAR_DECL) && has_proper_scope_for_analysis (t))
    bitmap_set_bit (bm, DECL_UID (t));
}

/* Examine tree T for references to static variables. All internal
   references like array references or indirect references are added
   to the INTERNAL_BM. Direct references are added to BASE_BM.  When
   this is called from the rhs or recursively, both bitmap operands
   are for the read bitmap.  When called from the lhs, the BASE_BM is
   the write bitmap.  */

static void
check_tree (tree t, bitmap base_bm, bitmap internal_bm)
{
  if ((TREE_CODE (t) == EXC_PTR_EXPR) || (TREE_CODE (t) == FILTER_EXPR))
    return;

  while (TREE_CODE (t) == REALPART_EXPR 
	 || TREE_CODE (t) == IMAGPART_EXPR
	 || handled_component_p (t))
    {
      if (TREE_CODE (t) == ARRAY_REF)
	check_operand (TREE_OPERAND (t, 1), internal_bm);
      t = TREE_OPERAND (t, 0);
    }

  /* The bottom of an indirect reference can only be read, not
     written.  So just recurse and but what ever we find, check it
     against the read bitmaps.  */
  if (INDIRECT_REF_P (t))
    check_tree (TREE_OPERAND (t, 0), internal_bm, internal_bm);

  if (SSA_VAR_P (t))
    check_operand (t, base_bm);
}

/* Scan tree T to see if there are any addresses taken in within T.  */

static void 
look_for_address_of (tree t)
{
  if (TREE_CODE (t) == ADDR_EXPR)
    {
      tree x = get_base_var (t);
      if ((TREE_CODE (x) == VAR_DECL) && has_proper_scope_for_analysis (x))
	{
	  if (dump_file)
	    fprintf (dump_file, "\nadding address:%s",
		     lang_hooks.decl_printable_name (x, 2));
	  
	  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
	     DECL_UID to get the uid from the var_ann field. */    
	  bitmap_set_bit (module_statics_escape, DECL_UID (x));
	}
    }
}


/* Check to see if T is a read or address of operation on a static var
   we are interested in analyzing.  FN is passed in to get access to
   its bit vectors.  */

static void
check_rhs_var (struct cgraph_node *fn, tree t)
{
  look_for_address_of (t);

  if (fn == NULL) 
    return;

  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from DECL_UID to
     get the uid from the var_ann field. */    
  check_tree(t,
	     fn->static_vars_info->local->statics_read_by_decl_uid,
	     fn->static_vars_info->local->statics_read_by_decl_uid);
}

/* Check to see if T is an assignment to a static var we are
   interrested in analyzing.  FN is passed in to get access to its bit
   vectors.
*/

static void
check_lhs_var (struct cgraph_node *fn, tree t)
{
  if (fn == NULL) 
    return;

  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from DECL_UID to
     get the uid from the var_ann field. */    
  check_tree(t, 
	     fn->static_vars_info->local->statics_written_by_decl_uid,
	     fn->static_vars_info->local->statics_read_by_decl_uid);
}

/* This is a scaled down version of get_asm_expr_operands from
   tree_ssa_operands.c.  The version there runs much later and assumes
   that aliasing information is already available. Here we are just
   trying to find if the set of inputs and outputs contain references
   or address of operations to local static variables.  FN is the
   function being analyzed and STMT is the actual asm statement.  */

static void
get_asm_expr_operands (struct cgraph_node * fn, tree stmt)
{
  int noutputs = list_length (ASM_OUTPUTS (stmt));
  const char **oconstraints
    = (const char **) alloca ((noutputs) * sizeof (const char *));
  int i;
  tree link;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout;
  
  for (i=0, link = ASM_OUTPUTS (stmt); link; ++i, link = TREE_CHAIN (link))
    {
      oconstraints[i] = constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_output_constraint (&constraint, i, 0, 0,
			       &allows_mem, &allows_reg, &is_inout);
      
      /* Memory operands are addressable.  Note that STMT needs the
	 address of this operand.  */
      if (!allows_reg && allows_mem) 
	check_lhs_var (fn, TREE_VALUE (link));
    }

  for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
    {
      constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0,
			      oconstraints, &allows_mem, &allows_reg);
      
      /* Memory operands are addressable.  Note that STMT needs the
	 address of this operand.  */
      if (!allows_reg && allows_mem) 
	check_rhs_var (fn, TREE_VALUE (link));
    }
  
  for (link = ASM_CLOBBERS (stmt); link; link = TREE_CHAIN (link))
    if (simple_cst_equal(TREE_VALUE (link), memory_identifier_string) == 1) 
      {
	/* Abandon all hope, ye who enter here. */
	fn->local.calls_read_all = true;
	fn->local.calls_write_all = true;
      }      
}

/* Check the parameters of a function call from CALLER to CALL_EXPR to
   see if any of them are static vars.  Also check to see if this is
   either an indirect call, a call outside the compilation unit, or
   has special attributes that effect the clobbers.  The caller
   parameter is the tree node for the caller and the second operand is
   the tree node for the entire call expression.  */

static void
process_call_for_static_vars(struct cgraph_node * caller, tree call_expr) 
{
  int flags = call_expr_flags(call_expr);
  tree operandList = TREE_OPERAND (call_expr, 1);
  tree operand;
  tree callee_t = get_callee_fndecl (call_expr);

  for (operand = operandList;
       operand != NULL_TREE;
       operand = TREE_CHAIN (operand))
    {
      tree argument = TREE_VALUE (operand);
      check_rhs_var (caller, argument);
    }
  
  /* Const and pure functions have less clobber effects than other
     functions so we process these first.  Otherwise if it is a call
     outside the compilation unit or an indirect call we punt.  This
     leaves local calls which will be processed by following the call
     graph.  */  

  if (flags & ECF_CONST) 
    return;
  else if (flags & ECF_PURE) 
    caller->local.calls_write_all = true;
  else 
    {
      enum availability avail = AVAIL_NOT_AVAILABLE;
      if (callee_t)
	{
	  struct cgraph_node* callee = cgraph_node(callee_t);
	  avail = cgraph_function_body_availability (callee);
	}

      /* Indirect call or external call. */
      if (avail == AVAIL_NOT_AVAILABLE || avail == AVAIL_OVERWRITTABLE) 
	{
	  caller->local.calls_read_all = true;
	  caller->local.calls_write_all = true;
	}
    }
}

/* FIXME -- PROFILE-RESTRUCTURE: Change to walk by explicitly walking
   the basic blocks rather than calling walktree.  */    

/* Walk tree and record all calls.  Called via walk_tree.  FIXME When
   this is moved into the tree-profiling-branch, and is dealing with
   low GIMPLE, this routine should be changed to use tree iterators
   rather than being a walk_tree callback.  The data is the function
   that is being scanned.  */
/* TP is the part of the tree currently under the
   microscope. WALK_SUBTREES is part of the walk_tree api but is
   unused here.  DATA is cgraph_node of the function being walked.  */

static tree
scan_for_static_refs (tree *tp, 
		      int *walk_subtrees ATTRIBUTE_UNUSED, 
		      void *data)
{
  struct cgraph_node *fn = data;
  tree t = *tp;
  
  switch (TREE_CODE (t))  
    {
    case VAR_DECL:
      if (DECL_INITIAL (t))
	walk_tree (&DECL_INITIAL (t), scan_for_static_refs, fn, visited_nodes);
      break;

    case MODIFY_EXPR:
      {
	/* First look on the lhs and see what variable is stored to */
	tree rhs = TREE_OPERAND (t, 1);
	check_lhs_var (fn, TREE_OPERAND (t, 0));
	/* Next check the operands on the rhs to see if they are ok. */
	switch (TREE_CODE_CLASS (TREE_CODE (rhs))) 
	  {
	  case tcc_binary:
	    check_rhs_var (fn, TREE_OPERAND (rhs, 0));
	    check_rhs_var (fn, TREE_OPERAND (rhs, 1));
	    break;
	  case tcc_unary:
	  case tcc_reference:
	    check_rhs_var (fn, TREE_OPERAND (rhs, 0));
	    break;
	  case tcc_declaration:
	    check_rhs_var (fn, rhs);
	    break;
	  case tcc_expression:
	    switch (TREE_CODE (rhs)) 
	      {
	      case ADDR_EXPR:
		check_rhs_var (fn, rhs);
		break;
	      case CALL_EXPR: 
		process_call_for_static_vars (fn, rhs);
		break;
	      default:
		break;
	      }
	    break;
	  default:
	    break;
	  }
      }
      break;
      
      
    case CALL_EXPR: 
      process_call_for_static_vars (fn, t);
      break;
      
    case ASM_EXPR:
      get_asm_expr_operands (fn, t);
      break;
      
    default:
      break;
    }
  return NULL;
}

/* Lookup the tree node for the static variable that has UID and
   conver the name to a string for debugging.  */

static const char *
get_static_name_by_uid (int index)
{
  splay_tree_node stn = splay_tree_lookup (static_vars_to_consider_by_uid, index);
  if (stn)
    return lang_hooks.decl_printable_name ((tree)(stn->value), 2);
  return NULL;
}

/* FIXME -- PROFILE-RESTRUCTURE: Change all *_decl_uid to *_ann_uid.  */

/* Or in all of the bits from every callee into X, the caller's, bit
   vector.  There are several cases to check to avoid the sparse
   bitmap oring.  */

static void
propagate_bits (struct cgraph_node *x)
{
  ipa_static_vars_info_t x_info = x->static_vars_info;
  ipa_global_static_vars_info_t x_global = x_info->global;

  struct cgraph_edge *e;
  for (e = x->callees; e; e = e->next_callee) 
    {
      struct cgraph_node *y = e->callee;

      /* Only look at the master nodes and skip external nodes.  */
      y = cgraph_master_clone (y);
      if (y)
	{
	  if (y->static_vars_info)
	    {
	      ipa_static_vars_info_t y_info; 
	      ipa_global_static_vars_info_t y_global;
	      y_info = y->static_vars_info;
	      y_global = y_info->global;
	      
	      if (x_global->statics_read_by_decl_uid != all_module_statics)
		{
		  if (y_global->statics_read_by_decl_uid 
		      == all_module_statics)
		    {
		      BITMAP_XFREE(x_global->statics_read_by_decl_uid);
		      x_global->statics_read_by_decl_uid 
			= all_module_statics;
		    }
		  /* Skip bitmaps that are pointer equal to node's bitmap
		     (no reason to spin within the cycle).  */
		  else if (x_global->statics_read_by_decl_uid 
			   != y_global->statics_read_by_decl_uid)
		    bitmap_a_or_b (x_global->statics_read_by_decl_uid,
				   x_global->statics_read_by_decl_uid,
				   y_global->statics_read_by_decl_uid);
		}
	      
	      if (x_global->statics_written_by_decl_uid != all_module_statics)
		{
		  if (y_global->statics_written_by_decl_uid 
		      == all_module_statics)
		    {
		      BITMAP_XFREE(x_global->statics_written_by_decl_uid);
		      x_global->statics_written_by_decl_uid 
			= all_module_statics;
		    }
		  /* Skip bitmaps that are pointer equal to node's bitmap
		     (no reason to spin within the cycle).  */
		  else if (x_global->statics_written_by_decl_uid 
			   != y_global->statics_written_by_decl_uid)
		    bitmap_a_or_b (x_global->statics_written_by_decl_uid,
				   x_global->statics_written_by_decl_uid,
				   y_global->statics_written_by_decl_uid);
		}
	    }
	  else 
	    {

	    }
	}
    }
}

/* FIXME -- PROFILE-RESTRUCTURE: Change all *_decl_uid to *_ann_uid.  */

/* Look at all of the callees of X to see which ones represent inlined
   calls.  For each of these callees, merge their local info into TARGET
   and check their children recursively.  */

static void 
merge_callee_local_info (struct cgraph_node *target, 
			 struct cgraph_node *x)
{
  struct cgraph_edge *e;
  ipa_local_static_vars_info_t x_l = target->static_vars_info->local;

  /* Make the world safe for tail recursion.  */
  if (x->aux) 
    return;

  x->aux = x;

  for (e = x->callees; e; e = e->next_callee) 
    {
      struct cgraph_node *y = e->callee;
      if (y->global.inlined_to) 
	{
	  ipa_static_vars_info_t y_info;
	  ipa_local_static_vars_info_t y_l;
	  struct cgraph_node* orig_y = y;
	 
	  y = cgraph_master_clone (y);
	  if (y)
	    {
	      y_info = y->static_vars_info;
	      y_l = y_info->local;
	      bitmap_a_or_b (x_l->statics_read_by_decl_uid,
			     x_l->statics_read_by_decl_uid,
			     y_l->statics_read_by_decl_uid);
	      bitmap_a_or_b (x_l->statics_written_by_decl_uid,
			     x_l->statics_written_by_decl_uid,
			     y_l->statics_written_by_decl_uid);
	      target->local.calls_read_all |= y->local.calls_read_all;
	      target->local.calls_write_all |= y->local.calls_write_all;
	      merge_callee_local_info (target, y);
	    }
	  else 
	    {
	      fprintf(stderr, "suspect inlining of ");
	      dump_cgraph_node (stderr, orig_y);
	      fprintf(stderr, "\ninto ");
	      dump_cgraph_node (stderr, target);
	      dump_cgraph (stderr);
	      gcc_assert(false);
	    }
	}
    }

  x->aux = NULL;
}

/* The init routine for analyzing global static variable usage. See
   comments at top for description.  */

static void 
ipa_init (void) 
{
  memory_identifier_string = build_string(7, "memory");

  static_vars_to_consider_by_uid =
    splay_tree_new_ggc (splay_tree_compare_ints);

  module_statics_escape = BITMAP_XMALLOC ();
  all_module_statics = BITMAP_XMALLOC ();

  /* There are some shared nodes, in particular the initializers on
     static declarations.  We do not need to scan them more than once
     since all we would be interrested in are the addressof
     operations.  */
  visited_nodes = pointer_set_create ();
}

/* Check out the rhs of a static or global initialization VNODE to see
   if any of them contain addressof operations.  Note that some of
   these variables may not even be referenced in the code in this
   compilation unit but their right hand sides may contain references
   to variables defined within this unit.  */

static void 
analyze_variable (struct cgraph_varpool_node *vnode)
{
  tree global = vnode->decl;
  if (!memory_identifier_string) ipa_init();

  if (TREE_CODE (global) == VAR_DECL)
    if (DECL_INITIAL (global)) 
      walk_tree (&DECL_INITIAL (global), scan_for_static_refs, 
		 NULL, visited_nodes);
}

/* This is the main routine for finding the reference patterns for
   global variables within a function FN.  */

static void
analyze_function (struct cgraph_node *fn)
{
  tree decl = fn->decl;
  ipa_static_vars_info_t info 
    = xcalloc (1, sizeof (struct ipa_static_vars_info_d));
  ipa_local_static_vars_info_t l
    = xcalloc (1, sizeof (struct ipa_local_static_vars_info_d));
  var_ann_t var_ann = get_var_ann (fn->decl);

  if (!memory_identifier_string) ipa_init();

  /* Add the info to the tree's annotation.  */
  fn->static_vars_info = info;
  var_ann->static_vars_info = info;

  info->local = l;
  l->statics_read_by_decl_uid = BITMAP_XMALLOC ();
  l->statics_written_by_decl_uid = BITMAP_XMALLOC ();
  l->statics_read_by_ann_uid = BITMAP_XMALLOC ();
  l->statics_written_by_ann_uid = BITMAP_XMALLOC ();

  if (dump_file)
    fprintf (dump_file, "\n local analysis of %s", cgraph_node_name (fn));
  
  walk_tree (&DECL_SAVED_TREE (decl), scan_for_static_refs, fn, visited_nodes);
}

/* Produce the global information by preforming a transitive closure
   on the local information that was produced by ipa_analyze_function
   and ipa_analyze_variable.  */


static void
static_execute (void)
{
  struct cgraph_node *node;
  struct cgraph_node *w;
  struct cgraph_node **order =
    xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  int order_pos = order_pos = reduced_inorder (order, false);
  int i;

  if (!memory_identifier_string) ipa_init();
  pointer_set_destroy (visited_nodes);
  visited_nodes = NULL;
  if (dump_file) 
    dump_cgraph (dump_file);

  /* Prune out the variables that were found to behave badly
     (i.e. have there address taken).  */
  {
    int index;
    bitmap_iterator bi;

    EXECUTE_IF_SET_IN_BITMAP (module_statics_escape, 0, index, bi)
      {
	splay_tree_remove (static_vars_to_consider_by_uid, index);
      }
    bitmap_operation (all_module_statics, all_module_statics,
		      module_statics_escape, BITMAP_AND_COMPL);

    BITMAP_XFREE(module_statics_escape);

    if (0) {
      FILE *ok_statics_file = fopen("/home/zadeck/ok_statics", "r");
      char line[100];
      int value;
      fgets(line, sizeof(line), ok_statics_file);
      sscanf(line, "%d", &value);
      for (i = 0; i < value; i++ )
   	{
   	  int j = bitmap_first_set_bit(all_module_statics);
 	  if (j == -1) break;
   	  fprintf(stderr, "  not considering %s\n",
   		  get_static_name_by_uid (j));
   	  splay_tree_remove (static_vars_to_consider_by_uid, j);
   	  bitmap_clear_bit(all_module_statics, j);
   	}
    }

    if (dump_file)
      EXECUTE_IF_SET_IN_BITMAP (all_module_statics, 0, index, bi)
	{
	  fprintf (dump_file, "\nPromotable global:%s",
		   get_static_name_by_uid (index));
	}

    /* Remove any variables from the local maps that are not in
       all_module_statics.  This will include all of the variables
       that were found to escape in the function scanning.  */
    for (i = 0; i < order_pos; i++ )
      {
	ipa_local_static_vars_info_t l;
	node = order[i];
	l = node->static_vars_info->local;

	bitmap_a_and_b (l->statics_read_by_decl_uid, 
			l->statics_read_by_decl_uid,
			all_module_statics);
	bitmap_a_and_b (l->statics_written_by_decl_uid, 
			l->statics_written_by_decl_uid,
			all_module_statics);
      }
  }

  if (dump_file)
    {
      for (i = 0; i < order_pos; i++ )
	{
	  int index;
	  ipa_local_static_vars_info_t l;
	  bitmap_iterator bi;

	  node = order[i];
	  l = node->static_vars_info->local;
	  fprintf (dump_file, 
		   "\nFunction name:%s/%i:", 
		   cgraph_node_name (node), node->uid);
	  fprintf (dump_file, "\n  locals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (l->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  locals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (l->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf(dump_file, "%s ",
		      get_static_name_by_uid (index));
	    }
	}
    }

  /* Propagate the local information thru the call graph to produce
     the global information.  All the nodes within a cycle will have
     the same info so we collapse cycles first.  Then we can do the
     propagation in one pass from the leaves to the roots.  */
  order_pos = reduced_inorder (order, true);
  if (dump_file)
    print_order(dump_file, "reduced", order, order_pos);

  for (i = 0; i < order_pos; i++ )
    {
      ipa_static_vars_info_t node_info;
      ipa_global_static_vars_info_t node_g = 
	xcalloc (1, sizeof (struct ipa_global_static_vars_info_d));
      ipa_local_static_vars_info_t node_l;
      
      bool read_all;
      bool write_all;

      node = order[i];
      node_info = node->static_vars_info;
      if (!node_info) 
	{
	  dump_cgraph_node (stderr, node);
	  dump_cgraph (stderr);
	  abort ();
	}

      node_info->global = node_g;
      node_l = node_info->local;

      read_all = node->local.calls_read_all;
      write_all = node->local.calls_write_all;

      /* If any node in a cycle is calls_read_all or calls_write_all
	 they all are. */
      w = node->next_cycle;
      while (w)
	{
	  read_all |= w->local.calls_read_all;
	  write_all |= w->local.calls_write_all;
	  w = w->next_cycle;
	}

      /* Initialized the bitmaps for the reduced nodes */
      if (read_all) 
	node_g->statics_read_by_decl_uid = all_module_statics;
      else 
	{
	  node_g->statics_read_by_decl_uid = BITMAP_XMALLOC ();
	  bitmap_copy (node_g->statics_read_by_decl_uid, 
		       node_l->statics_read_by_decl_uid);
	}

      if (write_all) 
	node_g->statics_written_by_decl_uid = all_module_statics;
      else
	{
	  node_g->statics_written_by_decl_uid = BITMAP_XMALLOC ();
	  bitmap_copy (node_g->statics_written_by_decl_uid, 
		       node_l->statics_written_by_decl_uid);
	}

      w = node->next_cycle;
      while (w)
	{
	  ipa_static_vars_info_t w_info = w->static_vars_info;
	  ipa_local_static_vars_info_t w_l = w_info->local;

	  /* All nodes within a cycle share the same global info bitmaps.  */
	  w_info->global = node_g;
	  
	  /* These global bitmaps are initialized from the local info
	     of all of the nodes in the region.  However there is no
	     need to do any work if the bitmaps were set to
	     all_module_statics.  */
	  if (!read_all)
	    bitmap_a_or_b (node_g->statics_read_by_decl_uid,
			   node_g->statics_read_by_decl_uid,
			   w_l->statics_read_by_decl_uid);
	  if (!write_all)
	    bitmap_a_or_b (node_g->statics_written_by_decl_uid,
			   node_g->statics_written_by_decl_uid,
			   w_l->statics_written_by_decl_uid);
	  w = w->next_cycle;
	}

      propagate_bits (node);

      w = node->next_cycle;
      while (w)
	{
	  propagate_bits (w);
	  w = w->next_cycle;
	}
    }

  /* Need to fix up the local information sets.  The information that
     has been gathered so far is preinlining.  However, the
     compilation will progress post inlining so the local sets for the
     inlined calls need to be merged into the callers.  */
  for (i = 0; i < order_pos; i++ )
    {
      node = order[i];
      if (cgraph_is_immortal_master_clone (node))
	merge_callee_local_info (node, node);
    }

  if (dump_file)
    {
      for (i = 0; i < order_pos; i++ )
	{
	  ipa_static_vars_info_t node_info;
	  ipa_global_static_vars_info_t node_g;
	  ipa_local_static_vars_info_t node_l;
	  int index;
	  bitmap_iterator bi;

	  node = order[i];
	  node_info = node->static_vars_info;
	  node_g = node_info->global;
	  node_l = node_info->local;
	  fprintf (dump_file, 
		   "\nFunction name:%s/%i:", 
		   cgraph_node_name (node), node->uid);
	  w = node->next_cycle;
	  while (w) 
	    {
	      fprintf (dump_file, "\n  next cycle: %s/%i ",
		       cgraph_node_name (w), w->uid);
	      w = w->next_cycle;
	    }
	  fprintf (dump_file, "\n  locals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_l->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  locals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_l->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf(dump_file, "%s ",
		      get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  globals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_g->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  globals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_g->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	}
    }

  /* Cleanup. */
  for (i = 0; i < order_pos; i++ )
    {
      ipa_static_vars_info_t node_info;
      ipa_global_static_vars_info_t node_g;
      node = order[i];
      node_info = node->static_vars_info;
      node_g = node_info->global;
      
      node_g->var_anns_valid = false;

      /* Create the complimentary sets.  These are more useful for
	 certain apis.  */
      node_g->statics_not_read_by_decl_uid = BITMAP_XMALLOC ();
      node_g->statics_not_written_by_decl_uid = BITMAP_XMALLOC ();

      /* FIXME -- PROFILE-RESTRUCTURE: Delete next 4 assignments.  */
      node_g->statics_read_by_ann_uid = BITMAP_XMALLOC ();
      node_g->statics_written_by_ann_uid = BITMAP_XMALLOC ();
      node_g->statics_not_read_by_ann_uid = BITMAP_XMALLOC ();
      node_g->statics_not_written_by_ann_uid = BITMAP_XMALLOC ();

      if (node_g->statics_read_by_decl_uid != all_module_statics) 
	{
	  bitmap_operation (node_g->statics_not_read_by_decl_uid, 
			    all_module_statics,
			    node_g->statics_read_by_decl_uid,
			    BITMAP_AND_COMPL);
	}

      if (node_g->statics_written_by_decl_uid != all_module_statics) 
	bitmap_operation (node_g->statics_not_written_by_decl_uid, 
			  all_module_statics,
			  node_g->statics_written_by_decl_uid,
			  BITMAP_AND_COMPL);
      w = node;

      while (w)
	{
	  struct cgraph_node * last = w;
	  ipa_static_vars_info_t w_info = w->static_vars_info;
	  ipa_local_static_vars_info_t w_l = w_info->local;
	  w_l->var_anns_valid = false;

	  w = w->next_cycle;
	  last->next_cycle = NULL;
	}
    }

  free (order);
}


static bool
gate_static_vars (void)
{
  return flag_unit_at_a_time != 0;
}

struct tree_opt_pass pass_ipa_static =
{
  "static-var",				/* name */
  gate_static_vars,			/* gate */
  analyze_function,                     /* IPA function */
  analyze_variable,		        /* IPA variable */
  static_execute,			/* execute */
  NULL, NULL,				/* IPA modification */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,				        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};


#include "gt-ipa-static-vars-anal.h"
