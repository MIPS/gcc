/*
 * Search for references that a functions loads or stores.
 */

#include <stdarg.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "gimple-iterator.h"
#include "tree-dfa.h"
#include "alias.h"
#include "cgraph.h"
#include "ipa-utils.h"
#include "symbol-summary.h"
#include "ipa-tbaa-modref.h"
#include "gimple-pretty-print.h"
#include "gimple-walk.h"
#include "print-tree.h"

//#include "rtl.h"
//#include "ssa.h"
//#include "fold-const.h"
//#include "tree-cfg.h"
//#include "domwalk.h"
//#include "tree-cfgcleanup.h"
//#include "params.h"
//#include "tree-ssa-loop.h"
//#include "config.h"
//#include "system.h"
//#include "coretypes.h"
//#include "backend.h"
//#include "tree.h"
//#include "gimple.h"
//#include "tree-pass.h"
//#include "data-streamer.h"
//#include "splay-tree.h"
//#include "ipa-reference.h"
//#include "calls.h"

/*
 * Summary of load/store compound alias classes for a function.
 */
class ipa_tbaa_summary_t : public fast_function_summary<ipa_tbaa_funcref *, va_heap>
{
public:
  ipa_tbaa_summary_t(symbol_table *symtab)
      : fast_function_summary<ipa_tbaa_funcref *, va_heap>(symtab) {}
  virtual void insert (cgraph_node *, ipa_tbaa_funcref *state);
  virtual void duplicate (cgraph_node *src_node, cgraph_node *dst_node,
			  ipa_tbaa_funcref *src_data,
			  ipa_tbaa_funcref *dst_data);
};

/*
 * Global variable holding the function summaries for all analyzed functions.
 */
static ipa_tbaa_summary_t *ipa_tbaa_summary;

static ipa_tbaa_summary_t *get(void)
{
  if (!ipa_tbaa_summary)
	  ipa_tbaa_summary = new ipa_tbaa_summary_t(symtab);
  return ipa_tbaa_summary;
}

#if 0
/* FIXME: This must match logic in get_alias_set so we get same types in
   tree-ssa-alias as here.  */
static tree get_base_type(tree exp)
{
  tree base_ref = exp;
  while (handled_component_p (base_ref))
    base_ref = TREE_OPERAND (base_ref, 0);
  return reference_alias_ptr_type (base_ref);
#if 0
  poly_int64_pod poffset;
  poly_int64_pod psize;
  poly_int64_pod pmax_size;
  bool preverse;

  tree base = get_ref_base_and_extent(stmt, &poffset, &psize, &pmax_size,
				      &preverse);
  return TREE_TYPE(base);
#endif
}

static tree get_ref_type(tree expr)
{
  return reference_alias_ptr_type (expr);
}
#endif

ATTRIBUTE_PRINTF_1
void dbg(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  if (dump_file)
    vfprintf(dump_file, msg, args);
  va_end(args);
}
static void store_access(std::vector<ipa_tbaa_caset> *v,
			 alias_set_type base_set, alias_set_type ref_set)
{
  ipa_tbaa_caset c;

  /* If we know everything is aliased there is no need to track further.  */
  if (v->size () == 1 && (*v)[0].base == 0 && (*v)[0].ref == 0)
    return;
  c.base = base_set;
  c.ref = ref_set;

  /* If we know everything is aliased just give up.  */
  if (!base_set && !ref_set)
    v->clear ();
  /* Otherwise look for duplicates.
     TODO: This can be smarter and more generic accesses can be ruled
     out first.  */
  else 
    for (ipa_tbaa_caset a: *v)
      if (a.base == base_set && a.ref == ref_set)
        return;
  /* TODO: Add cap on size of the vector and if there are too many entries
     simplify them.  */
  v->push_back(c);
}

#if 0
static void store_access(std::vector<ipa_tbaa_caset> *v,
			 tree base_type, tree ref_type)
{
  alias_set_type base_set = get_alias_set(base_type);
  alias_set_type ref_set = get_alias_set(ref_type);
  /* Variably modified types can not be tracked inter-procedurally.  */
  if (variably_modified_type_p (ref_type, current_function_decl))
    ref_set = 0;
  if (variably_modified_type_p (base_type, current_function_decl))
    base_set = ref_set;
  store_access (v,base_set,ref_set);
  if (dump_file)
    {
      dbg(" - Base alias set %d for: ", base_set);
      print_generic_expr (dump_file, base_type);
      dbg("\n");
      dbg(" - Ref alias set %d for: ", ref_set);
      print_generic_expr (dump_file, ref_type);
      dbg("\n");
    }
}
#endif

static void store_access2(std::vector<ipa_tbaa_caset> *v, tree expr)
{
  if (dump_file)
    {
      dbg(" Analyzing expression: ");
      print_generic_expr (dump_file, expr);
      dbg("\n");
    }
  /* Non-escaping memory is fine  */
  tree t = get_base_address (expr);
  if (t && (INDIRECT_REF_P (t) || TREE_CODE (t) == MEM_REF
	    || TREE_CODE (t) == TARGET_MEM_REF)
      && TREE_CODE (TREE_OPERAND (t, 0)) == SSA_NAME
      && !ptr_deref_may_alias_global_p (TREE_OPERAND (t, 0)))
    {
      dbg("   Not escaping\n");
      return;
    }

  /* Automatic variables are fine.  */
  if (DECL_P (t)
      /* TODO: It is possible to track what part of parameters are used.  */
      && TREE_CODE (t) != PARM_DECL
      && auto_var_in_fn_p (t, current_function_decl))
   {
     dbg("   Automatic variable\n");
     return;
   }
  if (DECL_P (t) && TREE_READONLY (t) && TREE_CODE (t) != PARM_DECL)
   {
     dbg("   Readonly variable\n");
     return;
   }

  //std::vector<tree> comp_types;

  //tree expr2 = expr;
  //while (handled_component_p (expr2))
  //  {
  //    comp_types.push_back (TREE_TYPE (expr2));
  //    expr2 = TREE_OPERAND (expr2, 0);
  //  }

  //tree base_type = get_base_type(expr);
  //comp_types.push_back (base_type);

  //for(size_t i = 0, j = 1; j < comp_types.size(); i++, j++)
  //  store_access(v, comp_types[j], comp_types[i]);

  // Tohle se bude muset vyresit az budeme delat IPA pass.
#if 1
  ao_ref r;
  ao_ref_init (&r, expr);
  dbg("  access %i->%i", ao_ref_base_alias_set (&r), ao_ref_alias_set (&r));
  store_access (v, ao_ref_base_alias_set (&r), ao_ref_alias_set (&r));
#else
  gcc_assert (get_alias_set (get_base_type (expr)) == ao_ref_base_alias_set (&r)
	      && get_alias_set (get_ref_type (expr)) == ao_ref_alias_set (&r));
  store_access(v, get_base_type(expr), get_ref_type(expr));
#endif
}

ipa_tbaa_funcref *get_ipa_tbaa_summary(cgraph_node *func)
{
  /* Avoid creation of the summary too early.  */
  if (!ipa_tbaa_summary)
    return NULL;

  enum availability avail;
  func = func->function_or_virtual_thunk_symbol (&avail);
  if (avail <= AVAIL_INTERPOSABLE)
    return NULL;

  ipa_tbaa_funcref *r = get()->get(func);
  if (r && r->finished)
    return r;
  return NULL;
}

namespace
{

/*
 * Definition of the pass.
 */
const pass_data pass_data_tbaa_modref = {
  GIMPLE_PASS,
  "tbaa_modref",
  OPTGROUP_IPA,
  TV_IPA_TBAA_MODREF,
  (PROP_cfg | PROP_ssa),
  0,
  0,
  0,
  0,
};

class pass_tbaa_modref : public gimple_opt_pass
{
      public:
	pass_tbaa_modref(gcc::context *ctxt)
	    : gimple_opt_pass(pass_data_tbaa_modref, ctxt) {}
  
	~pass_tbaa_modref ()
	  {
	    /*ipa_tbaa_summary = NULL;*/
	    // TODO free
	  }

	/* opt_pass methods: */
	opt_pass *clone()
	{
		return new pass_tbaa_modref(m_ctxt);
	}
	virtual bool gate (function *) { return flag_strict_aliasing; }
	virtual unsigned int execute(function *);
}; // class pass_tbaa_modref

 /* TODO contains horrible hacks */
ipa_tbaa_funcref *get_cur(void)
{
  cgraph_node *fnode = cgraph_node::get(current_function_decl);
  bool had = (get()->get(fnode) != NULL);
  ipa_tbaa_funcref *r = get()->get_create(fnode);
  if (!had)
    {
      r->finished = false;
    }
  return r;
}

void remove_cur(void)
{
  cgraph_node *fnode = cgraph_node::get(current_function_decl);
  get()->remove(fnode);
}

enum result
{
  RSLT_OK = 0,
  RSLT_NOTRACK = 1
};

enum result analyze_call(function *f, gimple *stmt)
{
  (void) f;
  /* Check flags on the function call. In certain cases, analysis can be
     greatly simplified.  */
  int flags = gimple_call_flags(stmt);
  if (flags & ECF_CONST)
    {
      dbg(" - ECF_CONST, ignoring all stores and all loads except for args.\n");
      return RSLT_OK;
    }

  /* TODO noreturn && no except region && no setjmp => discard all stores.  */

  /* Next, we try to get the callee's function declaration, so that we can get
     to their TBAA summary.  */
  tree callee = gimple_call_fndecl(stmt);
  bool ignore_stores = flags & ECF_PURE;

  /* Calls to noreturn functions do not affect global memory.  */
  if ((flags & (ECF_NORETURN | ECF_NOTHROW)) == (ECF_NORETURN | ECF_NOTHROW)
      || (!flag_exceptions && (flags & ECF_NORETURN)))
    ignore_stores = true;


  if (!callee)
    {
      if (ignore_stores)
        {
          store_access(&get_cur()->loads,0,0);
          return RSLT_OK;
        }
      dbg (" - Cannot find callee's function declaration.\n");
      return RSLT_NOTRACK;
    }
  if (recursive_call_p (current_function_decl, callee))
    {
      dbg (" - Skipping recursive call.\n");
      return RSLT_OK;
    }
  struct cgraph_node *callee_node = cgraph_node::get (callee);
  gcc_assert (callee_node != NULL);
  enum availability avail;
  /* Check availability of the function symbol.  */
  callee_node = callee_node->function_symbol(&avail);
  if (avail <= AVAIL_INTERPOSABLE)
    {
      if (ignore_stores)
        {
          store_access(&get_cur()->loads,0,0);
          return RSLT_OK;
        }
      dbg (" - Function availability <= AVAIL_INTERPOSABLE.\n");
      return RSLT_NOTRACK;
    }
  ipa_tbaa_funcref *callee_funcref = get ()->get (callee_node);
  if (!callee_funcref)
    {
      if (ignore_stores)
        {
          store_access(&get_cur()->loads,0,0);
          return RSLT_OK;
        }
      dbg (" - No TBAA summary available for callee.\n");
      return RSLT_NOTRACK;
    }
  if (!ignore_stores)
    for (ipa_tbaa_caset store: callee_funcref->stores)
      store_access (&get_cur()->stores, store.base, store.ref);
  for (ipa_tbaa_caset load: callee_funcref->loads)
    store_access (&get_cur()->loads, load.base, load.ref);
  return RSLT_OK;
}

bool analyze_load (gimple *, tree, tree op, void *)
{
  store_access2(&get_cur()->loads, op);
  return false;
}

bool analyze_store (gimple *, tree, tree op, void *)
{
  store_access2(&get_cur()->stores, op);
  return false;
}

enum result analyze_stmt(function *f, gimple *stmt)
{
  walk_stmt_load_store_ops (stmt, NULL, analyze_load, analyze_store);

  switch (gimple_code (stmt))
   {
   case GIMPLE_CALL:
     return analyze_call(f, stmt);
   case GIMPLE_ASM:
     if (gimple_asm_clobbers_memory_p (as_a <gasm *> (stmt)))
       {
         dbg(" - Function contains GIMPLE_ASM statement.\n");
         return RSLT_NOTRACK;
       }
     break;
   default:
     break;
   }

  return RSLT_OK;
}

const char *result_to_str(enum result r)
{
  switch (r)
    {
    case RSLT_OK:
      return "OK";
    case RSLT_NOTRACK:
      return "not tracked";
    }
    gcc_unreachable ();
}

static void dbg_dump_caset_vector(std::vector<ipa_tbaa_caset> &v)
{
  for (ipa_tbaa_caset &a: v)
    dbg("   - %d->%d\n", a.base, a.ref);
}
} // namespace

static void analyze_function(function *f)
{
  dbg("[ipa-tbaa-modref] Analyzing function '%s'...\n", function_name(f));
  basic_block bb;
  enum result r;
  get_cur()->fname = xstrdup(function_name(f)); /* TODO */
  get_cur()->loads.clear();
  get_cur()->stores.clear();
  get_cur()->finished = false;
  FOR_EACH_BB_FN(bb, f)
    {
      gimple_stmt_iterator si;
      for (si = gsi_after_labels(bb); !gsi_end_p(si); gsi_next(&si))
	{
	  r = analyze_stmt(f, gsi_stmt(si));
	  if (r != RSLT_OK)
	    goto skip;
	}
    }
skip:
  if (r == RSLT_NOTRACK)
    {
      remove_cur();
    }
  else
    {
      if (dump_file)
	{
	  dbg(" - Loads:\n");
	  dbg_dump_caset_vector(get_cur()->loads);
	  dbg(" - Stores:\n");
	  dbg_dump_caset_vector(get_cur()->stores);
	}
      get_cur()->finished = true;
    }
  dbg(" - Done with result '%s'.\n", result_to_str(r));
  dbg("\n");
}

unsigned int pass_tbaa_modref::execute(function *f)
{
  analyze_function(f);
  return 0;
}

/* Called when new function is inserted to callgraph late.  */

void
ipa_tbaa_summary_t::insert (struct cgraph_node *node, ipa_tbaa_funcref *)
{
  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
  analyze_function (DECL_STRUCT_FUNCTION (node->decl));
  pop_cfun ();
}

void
analyze_tbaa_modref (struct cgraph_node *node)
{
  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
  analyze_function (DECL_STRUCT_FUNCTION (node->decl));
  pop_cfun ();
}

/* Called when new clone is inserted to callgraph late.  */

void
ipa_tbaa_summary_t::duplicate (cgraph_node *, cgraph_node *,
			       ipa_tbaa_funcref *src_data,
			       ipa_tbaa_funcref *dst_data)
{
  dst_data->finished = src_data->finished;
  dst_data->stores = src_data->stores;
  dst_data->loads = src_data->loads;
}


gimple_opt_pass *make_pass_tbaa_modref(gcc::context *ctxt)
{
  return new pass_tbaa_modref(ctxt);
}
