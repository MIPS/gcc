#ifndef GIMPLE_TREE
#define GIMPLE_TREE

#include "langhooks.h"

#define MAY_HAVE_DEBUG_STMTS    (flag_var_tracking_assignments)

const char * gimple_tree_printable_name (tree, int);

extern void clean_symbol_name (char *p);

inline bool
integer_zero_p (Gimple::value v)
{
  extern int integer_zero_p (const_tree);
  return integer_zero_p ((const_tree) v);
}

inline bool
void_type_p (Gimple::type t)
{
  return t == void_type_node;
}

inline bool
is_empty_stmt (Gimple::value v)
{
  return (is_a<Gimple::nop_expr> (v) && void_type_p (v->type ())
	  && integer_zerop (as_a<Gimple::unary> (v)->op1 ()));
}

inline Gimple::constant 
build_zero_cst (Gimple::type type)
{
  extern tree build_zero_cst (tree);
  return build_zero_cst ((tree) type);
}

inline Gimple::value
strip_invariant_refs (const Gimple::value op)
{
  extern const_tree strip_invariant_refs (const_tree);
  return strip_invariant_refs ((const_tree)op);
}

inline bool
decl_address_invariant_p (const Gimple::value op)
{
  extern bool decl_address_invariant_p (const_tree);
  return decl_address_invariant_p ((const_tree) op);
}

inline bool
decl_address_ip_invariant_p (const Gimple::value op)
{
  extern bool decl_address_ip_invariant_p (const_tree);
  return decl_address_ip_invariant_p ((const_tree) op);
}

inline bool
needs_to_live_in_memory (Gimple::value t)
{
  extern bool needs_to_live_in_memory (const_tree);
  return needs_to_live_in_memory ((const_tree) t);
}

static inline bool
handled_component_p (Gimple::value t)
{
  switch (t->code())
    {
    case COMPONENT_REF:
    case BIT_FIELD_REF:
    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case VIEW_CONVERT_EXPR:
      return true;

    default:
      return false;
    }
}


inline Gimple::var_decl 
build_var_decl (location_t loc, Gimple::identifier name, Gimple::type t)
{
  extern tree build_decl_stat (location_t, enum tree_code, tree,
			       tree MEM_STAT_DECL);
  return build_decl_stat (loc, VAR_DECL, name, t);
}


// -------------------------------------------

// from gimplify.h

inline void gimple_add_tmp_var (Gimple::var_decl var)
{
  extern void gimple_add_tmp_var (tree);
  gimple_add_tmp_var ((tree)var);
}

// from tree.h


inline Gimple::identifier
decl_assembler_name (Gimple::decl_with_viz d)
{
  return d->assembler_name ();
}

inline bool
prototype_p (Gimple::function_or_method_type f)
{
  gcc_assert (f);
  return f->arg_types();
}

inline bool
comp_type_attributes (Gimple::type t1, Gimple::type t2)
{
  extern int comp_type_attributes (const_tree, const_tree);
  return comp_type_attributes((const_tree)t1, (const_tree)t2);
}

inline bool
tree_int_cst_equal (Gimple::integer_cst const1, Gimple::integer_cst const2)
{
  if (const1 == const2)
    return true;

  if (!const1 || !const2)
    return false;

  return const1->low () == const2->low () && const1->high () == const2->high ();
}


inline Gimple::value
chainon (Gimple::value x, Gimple::value y)
{
  extern tree chainon (tree, tree);
  return chainon ((tree)x, (tree)y);
}



inline Gimple::value
get_base_address (Gimple::value t)
{ 
  extern tree get_base_address (tree);
  return get_base_address ((tree)t);
}


inline bool
convert_expr_code_p (const enum tree_code tc)
{
  return (tc == NOP_EXPR || tc == CONVERT_EXPR);
}


inline bool
convert_expr_code_p (const Gimple::value p)
{
  enum tree_code tc = p->code ();
  return convert_expr_code_p (tc);
}


inline bool
ssa_var_p (Gimple::value p)
{
  if (is_a<Gimple::var_decl>(p) || is_a<Gimple::parm_decl>(p)
      || is_a<Gimple::result_decl>(p) || is_a<Gimple::ssa_name>(p))
    return true;
  return false;
}

inline bool 
is_comparison_p (enum tree_code code)
{
  return tree_code_type[(int) (code)] == tcc_comparison;
}

#endif
