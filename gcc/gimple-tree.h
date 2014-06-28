#ifndef GIMPLE_TREE
#define GIMPLE_TREE

#include "gimple-value.h"

extern G::value_ptr boolean_true_node_ptr;
extern G::value_ptr boolean_false_node_ptr;
extern G::type_ptr void_type_node_ptr;
extern G::type_ptr char_type_node_ptr;
extern G::type_ptr boolean_type_node_ptr;
extern G::type_ptr integer_type_node_ptr;
extern G::integer_cst_ptr integer_zero_node_ptr;
extern G::type_ptr pointer_type_node_ptr;

#define gimple_boolean_true (*boolean_true_node_ptr)
#define gimple_boolean_false (*boolean_false_node_ptr)
#define gimple_void_type (*void_type_node_ptr)
#define gimple_char_type (*char_type_node_ptr)
#define gimple_ptr_type (*pointer_type_node_ptr)
#define gimple_boolean_type (*boolean_type_node_ptr)
#define gimple_integer_type (*integer_type_node_ptr)
#define gimple_integer_zero (*integer_zero_node_ptr)


#define MAY_HAVE_DEBUG_STMTS    (flag_var_tracking_assignments)

/* From gimple-tree.c.  */
const char * gimple_tree_printable_name (tree, int);

/* From tree.h, but no 'tree' in the signature.  */
extern void clean_symbol_name (char *p);
extern tree build_call_expr_loc(location_t, tree, int, ...);  /*builtins.c */


/* The gimple wide-int overload templates.   */
namespace wi
{
  template <>
  struct int_traits <G::integer_cst>
  {
    static const enum precision_type precision_type = VAR_PRECISION;
    static const bool host_dependent_precision = false;
    static const bool is_sign_extended = false;
    static unsigned int get_precision (G::integer_cst);
    static wi::storage_ref decompose (HOST_WIDE_INT *, unsigned int,
				      G::integer_cst);
  };

  template <int N>
  class extended_gimple
  {
  private:
    G::integer_cst m_t;

  public:
    extended_gimple (G::integer_cst);

    unsigned int get_precision () const;
    const HOST_WIDE_INT *get_val () const;
    unsigned int get_len () const;
  };

  template <int N>
  struct int_traits <extended_gimple <N> >
  {
    static const enum precision_type precision_type = CONST_PRECISION;
    static const bool host_dependent_precision = false;
    static const bool is_sign_extended = true;
    static const unsigned int precision = N;
  };

  generic_wide_int <extended_gimple <WIDE_INT_MAX_PRECISION> >
  to_widest (G::integer_cst);

  generic_wide_int <extended_gimple <ADDR_MAX_PRECISION> > to_offset (G::integer_cst);
}

inline unsigned int
wi::int_traits <G::integer_cst>::get_precision (G::integer_cst tcst)
{
  return tcst->type()->precision ();
}

/* Convert the tree_cst X into a wide_int of PRECISION.  */
inline wi::storage_ref
wi::int_traits <G::integer_cst>::decompose (HOST_WIDE_INT *,
					unsigned int precision, G::integer_cst x)
{
  return wi::storage_ref (x->addr(0), x->nunits (), precision);
}

inline generic_wide_int <wi::extended_gimple <WIDE_INT_MAX_PRECISION> >
wi::to_widest (G::integer_cst t)
{
  return t;
}

inline generic_wide_int <wi::extended_gimple <ADDR_MAX_PRECISION> >
wi::to_offset (G::integer_cst t)
{
  return t;
}

template <int N>
inline wi::extended_gimple <N>::extended_gimple (G::integer_cst t)
  : m_t (t)
{
  gcc_checking_assert (t->type()->precision () <= N);
}

template <int N>
inline unsigned int
wi::extended_gimple <N>::get_precision () const
{
  return N;
}

template <int N>
inline const HOST_WIDE_INT *
wi::extended_gimple <N>::get_val () const
{
  return m_t->addr (0);
}

template <int N>
inline unsigned int
wi::extended_gimple <N>::get_len () const
{
  if (N == ADDR_MAX_PRECISION)
    return m_t->offset_nunits ();
  else if (N >= WIDE_INT_MAX_PRECISION)
    return m_t->ext_nunits ();
  else
    /* This class is designed to be used for specific output precisions
       and needs to be as fast as possible, so there is no fallback for
       other casees.  */
    gcc_unreachable ();
}

namespace wi
{
  template <typename T>
  bool fits_to_tree_p (const T &x, G::integer_cst);

  wide_int min_value (G::type);
  wide_int max_value (G::type);
  wide_int from_mpz (G::type, mpz_t, bool);
}

template <typename T>
bool
wi::fits_to_tree_p (const T &x, G::type type)
{
  if (type->type_sign () == UNSIGNED)
    return eq_p (x, zext (x, type->precision ()));
  else
    return eq_p (x, sext (x, type->precision ()));
}

/* Produce the smallest number that is represented in TYPE.  The precision
   and sign are taken from TYPE.  */
inline wide_int
wi::min_value (G::type type)
{
  return min_value (type->precision (), type->type_sign());
}

/* Produce the largest number that is represented in TYPE.  The precision
   and sign are taken from TYPE.  */
inline wide_int
wi::max_value (G::type type)
{
  return max_value (type->precision (), type->type_sign ());
}




inline int
simple_cst_equal (G::value v1, G::value v2)
{
  extern int simple_cst_equal (const_tree, const_tree);
  return simple_cst_equal ((const_tree)v1, (const_tree)v2);
}

inline G::value 
unshare_expr (G::value v)
{
  extern tree unshare_expr (tree);
  return unshare_expr ((tree)v);
}
inline bool
integer_zerop (G::value v)
{
  extern int integer_zerop (const_tree);
  return integer_zerop ((const_tree) v);
}

inline bool
void_type_p (G::type t)
{
  return t == gimple_void_type;
}

inline bool
is_empty_stmt (G::value v)
{
  return (is_a<G::nop_expr> (v) && void_type_p (v->type ())
	  && integer_zerop (v->op (0)));
}

inline G::constant 
build_zero_cst (G::type type)
{
  extern tree build_zero_cst (tree);
  return build_zero_cst ((tree) type);
}

inline G::value
strip_invariant_refs (const G::value op)
{
  extern const_tree strip_invariant_refs (const_tree);
  return strip_invariant_refs ((const_tree)op);
}

inline bool
decl_address_invariant_p (const G::value op)
{
  extern bool decl_address_invariant_p (const_tree);
  return decl_address_invariant_p ((const_tree) op);
}

inline bool
decl_address_ip_invariant_p (const G::value op)
{
  extern bool decl_address_ip_invariant_p (const_tree);
  return decl_address_ip_invariant_p ((const_tree) op);
}

inline bool
needs_to_live_in_memory (G::value t)
{
  extern bool needs_to_live_in_memory (const_tree);
  return needs_to_live_in_memory ((const_tree) t);
}

static inline bool
handled_component_p (G::value t)
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


inline G::var_decl 
build_var_decl (location_t loc, G::identifier name, G::type t)
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

inline G::function_decl
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

inline void gimple_add_tmp_var (G::var_decl var)
{
  extern void gimple_add_tmp_var (tree);
  gimple_add_tmp_var ((tree)var);
}

// from tree.h

inline bool
prototype_p (G::function_or_method_type f)
{
  gcc_assert (f);
  return f->arg_types();
}

inline bool
comp_type_attributes (G::type t1, G::type t2)
{
  extern int comp_type_attributes (const_tree, const_tree);
  return comp_type_attributes((const_tree)t1, (const_tree)t2);
}

inline bool
gimple_int_cst_equal (G::integer_cst const1, G::integer_cst const2)
{
  if (const1 == const2)
    return true;

  if (!const1 || !const2)
    return false;

  return wi::to_widest (const1) == wi::to_widest (const2);
}

inline bool
gimple_int_cst_equal (G::value const1, G::value const2)
{
  if (const1 == const2)
    return true;

  if (!const1 || !const2)
    return false;

  if (is_a <G::integer_cst> (const1) && is_a <G::integer_cst> (const2))
    return wi::to_widest (as_a<G::integer_cst> (const1))
	   == wi::to_widest (as_a<G::integer_cst> (const2));
  return false;

}

inline G::value
chainon (G::value x, G::value y)
{
  extern tree chainon (tree, tree);
  return chainon ((tree)x, (tree)y);
}



inline G::value
get_base_address (G::value t)
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
convert_expr_code_p (const G::value p)
{
  enum tree_code tc = p->code ();
  return convert_expr_code_p (tc);
}


inline bool
ssa_var_p (G::value p)
{
  if (is_a<G::var_decl>(p) || is_a<G::parm_decl>(p)
      || is_a<G::result_decl>(p) || is_a<G::ssa_name>(p))
    return true;
  return false;
}

inline bool 
is_comparison_p (enum tree_code code)
{
  return tree_code_type[(int) (code)] == tcc_comparison;
}

inline bool
get_pointer_alignment_1 (G::value v, unsigned int *alignp,
			 unsigned HOST_WIDE_INT *bitpos)
{
  /* From builtins.c */
  extern bool get_pointer_alignment_1 (tree, unsigned int *,
				       unsigned HOST_WIDE_INT *);
  return get_pointer_alignment_1 ((tree)v, alignp, bitpos);
}


static inline G::identifier_list
lookup_attribute (const char *attr_name, G::identifier_list list)
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


static inline G::value_list
lookup_attribute (const char *attr_name, G::value_list list)
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
template <typename T> T create (G::type, G::value, G::value);

template <>
inline G::modify_expr
create (G::type type , G::value lhs, G::value rhs)
{
  extern tree build2_stat (enum tree_code, tree, tree, tree MEM_STAT_DECL);
  return build2_stat (MODIFY_EXPR, type, lhs, rhs);
}

template <typename T> T create (G::type);
template <>
inline G::constructor
create<G::constructor> (G::type type)
{
  G::constructor c = G::create<G::constructor> ();
  c->set_type (type);
  c->set_elts (NULL);
  c->set_side_effects (false);
  c->set_constant (true);
  return c;
}

inline G::integer_cst
wide_int_to_int_cst (G::type type, const wide_int_ref &pcst)
{
  extern tree wide_int_to_tree (tree type, const wide_int_ref &cst);
  return G::integer_cst (wide_int_to_tree (type, pcst));

}
#if 0
template <typename T> T create (G::type type, const wide_int &cst);

template<>
inline G::integer_cst
create<G::integer_cst>  (G::type type, const wide_int &cst)
{
  extern tree build_int_cst_wide (tree, const wide_int &);
  return G::integer_cst (build_int_cst_wide ((tree)type, cst));
}


/* double_int_to_tree */
template <typename T> T create (G::type type, double_int cst);

template<>
inline G::integer_cst
create<G::integer_cst>  (G::type type, double_int d)
{
  extern tree double_int_to_tree (tree, double_int);
  return double_int_to_tree (type, d);
}
#endif

/* build_int_cst */
inline G::integer_cst
create_int_cst (G::type type, HOST_WIDE_INT cst)
{
  extern tree build_int_cst (tree, HOST_WIDE_INT);
  return build_int_cst ((tree)type, cst);
}


/* build_int_cstu */
inline G::integer_cst
create_int_cstu (G::type type, unsigned HOST_WIDE_INT cst)
{
  extern tree build_int_cstu (tree, unsigned HOST_WIDE_INT);
  return build_int_cstu ((tree)type, cst);
}

/* build_int_cst_type */
inline G::integer_cst
create_int_cst_type (G::type type, HOST_WIDE_INT low)
{
  extern tree build_int_cst_type (tree, HOST_WIDE_INT);
  return build_int_cst_type ((tree)type, low);
}

inline bool 
gimple_fits_uhwi_p (G::value v)
{
  extern bool tree_fits_uhwi_p (const_tree t);
  return tree_fits_uhwi_p (v);
}

inline unsigned HOST_WIDE_INT
gimple_to_uhwi (G::integer_cst i)
{
  gcc_assert (gimple_fits_uhwi_p (i));
  return i->low ();
}

inline unsigned HOST_WIDE_INT
gimple_to_uhwi (G::value v)
{
  gcc_assert (gimple_fits_uhwi_p (v));
  return as_a<G::integer_cst> (v)->low ();
}



inline G::constant
drop_tree_overflow (G::constant t)
{
  gcc_checking_assert (t->overflow_p ());

  /* For tree codes with a sharing machinery re-build the result.  */
  G::integer_cst i = dyn_cast<G::integer_cst> (t);
  if (i)
    return wide_int_to_int_cst (i->type (), i);
  /* Otherwise, as all tcc_constants are possibly shared, copy the node
     and drop the flag.  */
  t = G::copy (t);
  t->set_overflow_p (false);
  return t;
}

#ifndef CASE_CONVERT
#define CASE_CONVERT				\
  case NOP_EXPR:				\
  case CONVERT_EXPR
#endif


inline G::integer_type
build_gimple_nonstandard_integer_type (unsigned HOST_WIDE_INT precision,
				       int unsignedp) 
{
  extern tree build_nonstandard_integer_type (unsigned HOST_WIDE_INT, int);
  return build_nonstandard_integer_type(precision, unsignedp);
}


inline G::array_type
build_gimple_array_type (G::type elt_type, G::type index_type)
{
  extern tree build_array_type (tree, tree);
  return build_array_type ((tree)elt_type, (tree)index_type);
}


#define gimple_size_int (X) create_int_cst (sizetype_tab[(int) stk_sizetype], X);

inline G::type
build_gimple_index_type (G::integer_cst i)
{
  extern tree build_index_type (tree);
  return build_index_type ((tree)i);
}

inline G::array_type
build_gimple_array_type_nelts (G::type elt_type,
			       unsigned HOST_WIDE_INT nelts)
{
  G::integer_cst i;
  i = create_int_cst (G::type(sizetype_tab[(int) stk_sizetype]),
		      nelts - 1);
  return build_gimple_array_type (elt_type, build_gimple_index_type (i));
}

inline bool
contains_placeholder_p (const G::value value)
{
  extern bool contains_placeholder_p (const_tree exp);
  return contains_placeholder_p ((const_tree)value);
}

#endif  /* GIMPLE_TREE */
