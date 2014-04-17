#ifndef GIMPLE_TREE
#define GIMPLE_TREE

#include "gimple-value.h"

extern Gimple::value_ptr boolean_true_node_ptr;
extern Gimple::value_ptr boolean_false_node_ptr;
extern Gimple::type_ptr void_type_node_ptr;
extern Gimple::type_ptr char_type_node_ptr;
extern Gimple::type_ptr boolean_type_node_ptr;
extern Gimple::type_ptr integer_type_node_ptr;
extern Gimple::integer_cst_ptr integer_zero_node_ptr;

#define gimple_boolean_true (*boolean_true_node_ptr)
#define gimple_boolean_false (*boolean_false_node_ptr)
#define gimple_void_type (*void_type_node_ptr)
#define gimple_char_type (*char_type_node_ptr)
#define gimple_boolean_type (*boolean_type_node_ptr)
#define gimple_integer_type (*integer_type_node_ptr)
#define gimple_integer_zero (*integer_zero_node_ptr)


#define MAY_HAVE_DEBUG_STMTS    (flag_var_tracking_assignments)

/* From gimple-tree.c.  */
const char * gimple_tree_printable_name (tree, int);

/* From tree.h, but no 'tree' in the signature.  */
extern void clean_symbol_name (char *p);
extern tree build_call_expr_loc(location_t, tree, int, ...);  /*builtins.c */

inline int
simple_cst_equal (Gimple::value v1, Gimple::value v2)
{
  extern int simple_cst_equal (const_tree, const_tree);
  return simple_cst_equal ((const_tree)v1, (const_tree)v2);
}

inline Gimple::value 
unshare_expr (Gimple::value v)
{
  extern tree unshare_expr (tree);
  return unshare_expr ((tree)v);
}
inline bool
integer_zerop (Gimple::value v)
{
  extern int integer_zerop (const_tree);
  return integer_zerop ((const_tree) v);
}

inline bool
void_type_p (Gimple::type t)
{
  return t == gimple_void_type;
}

inline bool
is_empty_stmt (Gimple::value v)
{
  return (is_a<Gimple::nop_expr> (v) && void_type_p (v->type ())
	  && integer_zerop (v->op (0)));
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


inline bool
builtin_valid_p (enum built_in_function fncode)
{
  return (IN_RANGE ((int)fncode, ((int)BUILT_IN_NONE) + 1,
	  ((int) END_BUILTINS) - 1));
}

inline Gimple::function_decl
gimple_builtin_decl_explicit (enum built_in_function fncode)
{
  gcc_checking_assert (builtin_valid_p (fncode));
  return builtin_info.decl[(size_t)fncode];
}


inline bool
gimple_builtin_decl_explicit_p (enum built_in_function fncode)
{
  gcc_checking_assert (builtin_valid_p (fncode));
  return builtin_info.decl[(size_t)fncode] != NULL_GIMPLE;
}


// -------------------------------------------

// from gimplify.h

inline void gimple_add_tmp_var (Gimple::var_decl var)
{
  extern void gimple_add_tmp_var (tree);
  gimple_add_tmp_var ((tree)var);
}

// from tree.h

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

inline bool
get_pointer_alignment_1 (Gimple::value v, unsigned int *alignp,
			 unsigned HOST_WIDE_INT *bitpos)
{
  /* From builtins.c */
  extern bool get_pointer_alignment_1 (tree, unsigned int *,
				       unsigned HOST_WIDE_INT *);
  return get_pointer_alignment_1 ((tree)v, alignp, bitpos);
}


static inline Gimple::identifier_list
lookup_attribute (const char *attr_name, Gimple::identifier_list list)
{
  extern tree private_lookup_attribute (const char *, size_t, tree);
  gcc_checking_assert (attr_name[0] != '_');
  /* In most cases, list is NULL_TREE.  */
  if (list == NULL_GIMPLE)
    return NULL_GIMPLE;
  else
    /* Do the strlen() before calling the out-of-line implementation.
       In most cases attr_name is a string constant, and the compiler
       will optimize the strlen() away.  */
    return private_lookup_attribute (attr_name, strlen (attr_name), (tree)list);
}


static inline Gimple::value_list
lookup_attribute (const char *attr_name, Gimple::value_list list)
{
  extern tree private_lookup_attribute (const char *, size_t, tree);
  gcc_checking_assert (attr_name[0] != '_');
  /* In most cases, list is NULL_TREE.  */
  if (list == NULL_GIMPLE)
    return NULL_GIMPLE;
  else
    /* Do the strlen() before calling the out-of-line implementation.
       In most cases attr_name is a string constant, and the compiler
       will optimize the strlen() away.  */
    return private_lookup_attribute (attr_name, strlen (attr_name), (tree)list);
}

/* build2 */
template <typename T> T create (Gimple::type, Gimple::value, Gimple::value);

template <>
inline Gimple::modify_expr
create (Gimple::type type , Gimple::value lhs, Gimple::value rhs)
{
  extern tree build2_stat (enum tree_code, tree, tree, tree MEM_STAT_DECL);
  return build2_stat (MODIFY_EXPR, type, lhs, rhs);
}

template <typename T> T create (Gimple::type);
template <>
inline Gimple::constructor
create<Gimple::constructor> (Gimple::type type)
{
  Gimple::constructor c = Gimple::create<Gimple::constructor> ();
  c->set_type (type);
  c->set_elts (NULL);
  c->set_side_effects (false);
  c->set_constant (true);
  return c;
}

template <typename T> T create (Gimple::type type, unsigned HOST_WIDE_INT low, HOST_WIDE_INT high);

template<>
inline Gimple::integer_cst
create<Gimple::integer_cst>  (Gimple::type type, unsigned HOST_WIDE_INT low, HOST_WIDE_INT high)
{
  extern tree build_int_cst_wide (tree, unsigned HOST_WIDE_INT, HOST_WIDE_INT);
  return Gimple::integer_cst (build_int_cst_wide ((tree)type, low, high));
}


/* double_int_to_tree */
template <typename T> T create (Gimple::type type, double_int cst);

template<>
inline Gimple::integer_cst
create<Gimple::integer_cst>  (Gimple::type type, double_int d)
{
  extern tree build_int_cst_wide (tree, unsigned HOST_WIDE_INT, HOST_WIDE_INT);
  d = d.ext (type->precision (), type->type_unsigned ());
  return Gimple::integer_cst (build_int_cst_wide ((tree)type, d.low, d.high));
}

/* build_int_cst_type */
template <typename T> T create (Gimple::type type, HOST_WIDE_INT low);

template<>
inline Gimple::integer_cst
create<Gimple::integer_cst> (Gimple::type type, HOST_WIDE_INT low)
{
  return create<Gimple::integer_cst> (type, double_int::from_shwi (low));
}

inline bool 
gimple_fits_uhwi_p (Gimple::value v)
{
  Gimple::integer_cst c = v;
  return c && (c->high () == 0);
}

inline unsigned HOST_WIDE_INT
gimple_to_uhwi (Gimple::integer_cst value)
{
  gcc_assert (value->high () == 0);
  return value->low ();
}


inline Gimple::constant
drop_tree_overflow (Gimple::constant t)
{
  Gimple::integer_cst i = t;
  gcc_checking_assert (t->overflow_p ());

  /* For tree codes with a sharing machinery re-build the result.  */
  if (i)
    return create<Gimple::integer_cst> (i->type (), i->low (), i->high ());
  /* Otherwise, as all tcc_constants are possibly shared, copy the node
     and drop the flag.  */
  t = Gimple::copy (t);
  t->set_overflow_p (false);
  return t;
}

#ifndef CASE_CONVERT
#define CASE_CONVERT				\
  case NOP_EXPR:				\
  case CONVERT_EXPR
#endif


inline Gimple::integer_type
build_gimple_nonstandard_integer_type (unsigned HOST_WIDE_INT precision,
				       int unsignedp) 
{
  extern tree build_nonstandard_integer_type (unsigned HOST_WIDE_INT, int);
  return build_nonstandard_integer_type(precision, unsignedp);
}


inline Gimple::array_type
build_gimple_array_type (Gimple::type elt_type, Gimple::type index_type)
{
  extern tree build_array_type (tree, tree);
  return build_array_type ((tree)elt_type, (tree)index_type);
}


#define gimple_size_int (X) create<Gimple::integer_cst> (sizetype_tab[(int) stk_sizetype], X);

inline Gimple::type
build_gimple_index_type (Gimple::integer_cst i)
{
  extern tree build_index_type (tree);
  return build_index_type ((tree)i);
}

inline Gimple::array_type
build_gimple_array_type_nelts (Gimple::type elt_type,
			       unsigned HOST_WIDE_INT nelts)
{
  Gimple::integer_cst i;
  i = create<Gimple::integer_cst> (Gimple::type(sizetype_tab[(int) stk_sizetype]), nelts - 1);
  return build_gimple_array_type (elt_type, build_gimple_index_type (i));
}


#endif  /* GIMPLE_TREE */
