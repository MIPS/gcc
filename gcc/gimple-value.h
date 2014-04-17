#ifndef GIMPLE_VALUE_H
#define GIMPLE_VALUE_H

#include "gimple-value-core.h"
#include "gimple-wrapper.h"

namespace Gimple {

/* Class tree_desc methods.  */

inline enum tree_code 
tree_desc::code () const
{
  return (enum tree_code) node.base.code;
}

inline void
tree_desc::set_code (enum tree_code c) 
{
  node.base.code = c;
}

inline enum tree_code_class
tree_desc::code_class () const
{
  return tree_code_type [node.base.code];
}

inline bool
tree_desc::addressable () const
{
  return node.base.addressable_flag;
}
inline void
tree_desc::set_addressable (const bool f)
{
  node.base.addressable_flag = f;
}

inline bool 
tree_desc::this_volatile () const
{
  return node.base.volatile_flag;
}

inline void
tree_desc::set_this_volatile (const bool f)
{
  node.base.volatile_flag = f;
}

inline bool 
tree_desc::no_warning () const
{
  return node.base.nowarning_flag;
}

inline void
tree_desc::set_no_warning (const bool f)
{
  node.base.nowarning_flag = f;
}

inline bool 
tree_desc::used () const
{
  return node.base.used_flag;
}

inline void
tree_desc::set_used(const bool f)
{
  node.base.used_flag = f;
}

inline bool
tree_desc::static_p () const
{
  return node.base.static_flag;
}

inline void
tree_desc::set_static_p (const bool f)
{
  node.base.static_flag = f;
}

inline bool
tree_desc::side_effects () const
{
  return node.base.side_effects_flag;
}

inline void
tree_desc::set_side_effects (const bool f)
{
  node.base.side_effects_flag = f;
}

inline bool
tree_desc::constant () const
{
  return node.base.constant_flag;
}

inline void
tree_desc::set_constant (const bool f)
{
  node.base.constant_flag = f;
}
inline bool
tree_desc::test_node (void) const
{
  return true;
}

inline bool
tree_desc::test_node (enum tree_code_class cl) const
{
  if (tree_code_type[code ()] == cl)
    return true;
  return false;
}

inline bool
tree_desc::test_node (enum tree_code_class c1,
			   enum tree_code_class c2) const
{
  enum tree_code_class c = code_class ();
  if (c < c1 || c > c2)
    return false;
  return true;
}

inline bool
tree_desc::test_node (enum tree_code tc) const
{
  if (code () == tc)
    return true;
  return false;
}

inline bool 
tree_desc::test_node (enum tree_node_structure_enum s) const
{
  if (tree_contains_struct[code ()][s] == 1)
    return true;
  return false;
}

inline bool
tree_desc::test_node (enum tree_code t1, enum tree_code t2) const
{
  enum tree_code tc = code ();
  if (tc == t1 || tc == t2)
    return true;
  return false;
}

inline bool
tree_desc::test_node (enum tree_code t1, enum tree_code t2, enum tree_code t3,
	       enum tree_code t4, enum tree_code t5) const
{
  enum tree_code tc = code ();
  if (tc == t1 || tc == t2 || tc == t3 || tc == t4 || tc == t5)
    return true;
  return false;
}

inline void
tree_desc::check_node (void) const
{
}

inline void
tree_desc::check_node (enum tree_code_class cl) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (cl) && &node != global_trees[TI_ERROR_MARK])
    tree_class_check_failed (reinterpret_cast<const_tree>(this), cl , __FILE__,
			     __LINE__, __FUNCTION__);
#endif
}

inline void
tree_desc::check_node (enum tree_code_class c1,
			      enum tree_code_class c2) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (c1, c2) && &node != global_trees[TI_ERROR_MARK])
    tree_class_check_failed (reinterpret_cast<const_tree>(this), c2, __FILE__,
			     __LINE__, __FUNCTION__);
#endif
}

inline void
tree_desc::check_node (enum tree_code tc) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (tc))
    tree_check_failed (reinterpret_cast<const_tree>(this), __FILE__, __LINE__,
		       __FUNCTION__, tc, 0);
#endif
}

inline void 
tree_desc::check_node (enum tree_node_structure_enum s) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (s))
    tree_contains_struct_check_failed (reinterpret_cast<const_tree>(this), s,
				       __FILE__, __LINE__, __FUNCTION__);
#endif
}

inline void
tree_desc::check_node (enum tree_code t1, enum tree_code t2) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (t1, t2))
    tree_check_failed (reinterpret_cast<const_tree>(this), __FILE__, __LINE__,
		       __FUNCTION__, t1, t2, 0);
#endif
}

inline void
tree_desc::check_node (enum tree_code t1, enum tree_code t2, enum tree_code t3,
	        enum tree_code t4, enum tree_code t5) const
{
#if GIMPLE_CHECKING_ON
  if (!test_node (t1, t2, t3, t4, t5))
    tree_check_failed (reinterpret_cast<const_tree>(this), __FILE__, __LINE__,
		       __FUNCTION__, t1, t2, t3, t4, t5, 0);
#endif
}

inline Gimple::value
block_desc::supercontext () const
{
  return node.block.supercontext;
}


inline Gimple::type
type_desc::type() const
{ 
  return Gimple::type (node.typed.type);
}
inline bool 
type_desc::type_unsigned () const
{ return node.base.u.bits.unsigned_flag; }

inline enum machine_mode
type_desc::mode () const
{ 
  if (code () == VECTOR_TYPE)
    return vector_mode ();
  else
    return node.type_common.mode;
}
  
inline bool
type_desc::complete_type_p () const
{
  return (size () != NULL_GIMPLE);
}

inline bool 
type_desc::pointer_type_p () const
{ 
  return code () == POINTER_TYPE || code () == REFERENCE_TYPE;
}

inline bool
type_desc::aggregate_type_p () const
{
  enum tree_code c = code();
  return (c == ARRAY_TYPE || c == RECORD_TYPE || c == UNION_TYPE
	  || c == QUAL_UNION_TYPE);
}

inline bool
type_desc::integral_type_p () const
{
  enum tree_code c = code();
  return (c == ENUMERAL_TYPE || c == BOOLEAN_TYPE || c == INTEGER_TYPE);
}

inline bool
type_desc::scalar_float_type_p () const
{
  return code () == REAL_TYPE;
}

inline bool
type_desc::fixed_point_type_p () const
{
  return code () == FIXED_POINT_TYPE;
}

inline bool
type_desc::boolean_type_p () const
{
  return code () == BOOLEAN_TYPE;
}

inline bool
type_desc::method_or_function_type_p () const
{
  return code () == FUNCTION_TYPE || code() == METHOD_TYPE;
}

inline bool
type_desc::complex_type_p () const
{
  return code () == COMPLEX_TYPE;
}

inline bool
type_desc::vector_type_p () const
{
  return code () == VECTOR_TYPE;
}

inline bool
type_desc::array_type_p () const
{
  return code () == ARRAY_TYPE;
}


inline addr_space_t 
type_desc::addr_space () const
{
  return node.base.u.bits.address_space;
}

inline Gimple::type
type_desc::main_variant () const
{
  return node.type_common.main_variant;
}

inline type_list
type_desc::chain () const
{
  return node.common.chain;
}

inline Gimple::type
type_desc::canonical () const
{
  return node.type_common.canonical;
}

inline unsigned int
type_desc::precision () const
{
  return node.type_common.precision;
}

inline value
type_desc::size () const
{
  return node.type_common.size;
}


inline bool
array_type_desc::string_flag () const
{
  return node.type_common.string_flag;
}

inline numerical_type
array_type_desc::domain () const
{
  return node.type_non_common.values;
}

inline Gimple::type_list
function_or_method_type_desc::arg_types () const
{
  return node.type_non_common.values;
}

inline Gimple::type
method_type_desc::basetype () const
{
  return node.type_non_common.maxval;
}

inline value
numerical_type_desc::min_value() const
{
  return node.type_non_common.minval;
}

inline value
numerical_type_desc::max_value() const
{
  return node.type_non_common.maxval;
}

inline identifier_list
type_desc::attributes () const
{
  return node.type_common.attributes;
}

// value methods
//
//

inline Gimple::type
value_desc::type() const
{ 
  return Gimple::type (node.typed.type);
}

inline void 
value_desc::set_type (Gimple::type t)
{ 
  node.typed.type = t;
}

inline bool 
value_desc::clobber_p () const
{
  return (code () == CONSTRUCTOR && this_volatile ());
}

inline int
value_desc::op_len () const
{
  int index;
  if (code_class () == tcc_vl_exp)
    {
      integer_cst i(node.exp.operands[0]);
      index = i->low ();
    }
  else
    index = tree_code_length[(int) code ()];
  return index;
}

inline value
value_desc::op(const int i) const
{
  if (i < 0 || i > op_len ())
    tree_operand_check_failed (i, &node, __FILE__, __LINE__, __FUNCTION__);
  return value(node.exp.operands[i]);
}

inline location_t
value_desc::expr_location() const
{
  enum tree_code_class cl = code_class ();
  if (cl >= tcc_reference && cl <= tcc_expression)
    return node.exp.locus;
  else
    return UNKNOWN_LOCATION;
}

inline int
value_ops_desc::op_len () const
{
  return value_desc::op_len ();
}

inline value
value_ops_desc::op(const int i) const
{
  return value_desc::op(i);
}

inline Gimple::value
unary_desc::op1 () const
{
  return op(0);
}  

inline Gimple::value
binary_desc::op1 () const
{
  return op(0);
}  

inline Gimple::value
binary_desc::op2 () const
{
  return op(1);
}  

inline const char *
identifier_desc::pointer () const
{
  return (const char *)(node.identifier.id.str);
}

inline bool
constant_desc::overflow_p () const
{
  return node.base.public_flag;
}

inline void
constant_desc::set_overflow_p (bool f)
{
  node.base.public_flag = f;
}

inline double_int 
integer_cst_desc::int_cst () const
{ return node.int_cst.int_cst; }

inline unsigned HOST_WIDE_INT 
integer_cst_desc::low () const
{ return node.int_cst.int_cst.low; }

inline HOST_WIDE_INT 
integer_cst_desc::high () const
{ return node.int_cst.int_cst.high; }

inline REAL_VALUE_TYPE
real_cst_desc::real_cst () const
{
  return *(node.real_cst.real_cst_ptr);
}

inline value
case_label_expr_desc::case_low () const
{ 
  return op (0); 
}

inline value
case_label_expr_desc::case_high () const
{ 
  return op (1); 
}


inline value
case_label_expr_desc::case_label () const
{ 
  return op (2); 
}


inline value
case_label_expr_desc::case_chain() const
{ 
  return op (3); 
}


inline value
mem_ref_desc::base () const
{
  return value_desc::op(0);
}

inline value
mem_ref_desc::offset () const
{
  return value_desc::op(1);
}



inline value
addr_expr_desc::expr () const
{
  return value_desc::op(0);
}

inline ::vec<constructor_elt, va_gc> *
constructor_desc::elts () const
{
  return node.constructor.elts;
}

inline void
constructor_desc::set_elts (::vec<constructor_elt, va_gc> *e)
{
  node.constructor.elts = e;
}


//
// decl methods
//

inline Gimple::identifier
decl_desc::name() const
{ 
  return Gimple::identifier (node.decl_minimal.name);
}

inline bool
decl_desc::decl_ignored_p () const
{ 
  return node.decl_common.ignored_flag;
}


inline decl
decl_desc::decl_abstract_origin () const
{
  decl orig(node.decl_common.abstract_origin);
  return orig;
}

inline unsigned int
decl_desc::align () const
{
  return node.decl_common.align;
}

inline void
decl_desc::set_align (unsigned int a)
{
  node.decl_common.align = a;
}

inline bool
decl_desc::pt_uid_set_p () const
{
  return node.decl_common.pt_uid != -1u;
}

inline unsigned int
decl_desc::pt_uid () const
{
  return node.decl_common.pt_uid != -1u ? node.decl_minimal.uid 
					: node.decl_common.pt_uid;
}

inline void
decl_desc::set_pt_uid (unsigned int uid)
{
  node.decl_common.pt_uid = uid;
}

inline bool 
decl_desc::decl_from_inline () const
{
  decl origin = decl_abstract_origin ();
  return origin && origin != this;
}

inline bool
decl_desc::readonly () const
{
  return node.base.readonly_flag;
}

inline void
decl_desc::set_readonly (const bool f) 
{
  node.base.readonly_flag = f;
}

inline bool
decl_desc::external () const
{
  return node.decl_common.decl_flag_1;
}

inline void
decl_desc::set_external(const bool f) 
{
  node.decl_common.decl_flag_1 = f;
}


inline location_t
decl_desc::source_location () const
{
  return node.decl_minimal.locus;
}

inline void
decl_desc::set_source_location (const location_t loc)
{
  node.decl_minimal.locus = loc;
}

inline bool 
decl_desc::gimple_reg_p () const
{
  return node.decl_common.gimple_reg_flag;
}

inline void
decl_desc::set_gimple_reg_p (const bool f)
{
  node.decl_common.gimple_reg_flag = f;
}

inline bool 
decl_desc::artificial() const
{
  return node.decl_common.artificial_flag;
}

inline void
decl_desc::set_artificial (const bool f)
{
  node.decl_common.artificial_flag = f;
}

inline bool 
decl_desc::ignored_p () const
{
  return node.decl_common.ignored_flag;
}

inline void
decl_desc::set_ignored_p (const bool f)
{
  node.decl_common.ignored_flag = f;
}

inline value_list
decl_desc::attributes () const
{
  return node.decl_common.attributes;
}

inline void
decl_desc::set_attributes (const value_list list)
{
  node.decl_common.attributes = list;
}
/*
inline bool
decl_desc::is_type_context () const
{
  Gimple::value v = node.decl_minimal.context;
  return (!v || v->code() != FUNCTION_DECL);
}

inline Gimple::function_decl
decl_desc::function_context () const
{
  return node.decl_minimal.context;
}

inline void
decl_desc::set_function_context (Gimple::function_decl decl)
{
  node.decl_minimal.context = decl;
}

inline Gimple::type
decl_desc::type_context () const
{
  return node.decl_minimal.context;
}

inline void
decl_desc::set_type_context (Gimple::type t)
{
  node.decl_minimal.context = t;
}

*/

inline Gimple::value
decl_desc::context () const
{
  return node.decl_minimal.context;
}

inline void
decl_desc::set_context (Gimple::value t)
{
  node.decl_minimal.context = t;
}

inline unsigned int
decl_desc::uid () const
{
  return node.decl_minimal.uid;
}

inline enum machine_mode
decl_desc::mode () const
{
  return node.decl_common.mode;
}

inline void 
decl_desc::set_mode (enum machine_mode m)
{
  node.decl_common.mode = m;
}

inline bool 
decl_desc::decl_has_assembler_p() const
{
  return tree_contains_struct[code ()][TS_DECL_WITH_VIS] == 1;
}

inline bool 
decl_desc::decl_has_rtl_p() const
{
  return tree_contains_struct[code ()][TS_DECL_WRTL] == 1;
}

inline bool 
decl_desc::decl_has_noncommon_p() const
{
  return tree_contains_struct[code ()][TS_DECL_NON_COMMON] == 1;
}

inline bool 
decl_desc::assembler_name_set_p() const
{
  return (decl_has_assembler_p ()
	  && node.decl_with_vis.assembler_name != (tree)0);
}

inline bool
decl_desc::decl_public() const
{ return node.base.public_flag; }

inline bool
decl_desc::decl_asm_written() const
{ return node.base.asm_written_flag; }

inline bool 
decl_with_viz_desc::hard_register () const
{
  return node.decl_with_vis.hard_register;
}

inline bool 
decl_with_viz_desc::seen_in_bind_expr_p () const
{
  return node.decl_with_vis.seen_in_bind_expr;
}

inline void
decl_with_viz_desc::set_seen_in_bind_expr_p (const bool f)
{
  node.decl_with_vis.seen_in_bind_expr = f;
}

inline bool 
decl_with_viz_desc::assembler_name_set_p() const
{
  return node.decl_with_vis.assembler_name != (tree)0;
}


/*
inline bool
decl_desc::decl_visibility_specified() const
{ return DECL_WITH_VIS_CHECK (Tree)->decl_with_vis.visibility_specified; }

inline bool
decl_desc::decl_comdat() const
{ return DECL_WITH_VIS_CHECK (Tree)->decl_with_vis.comdat_flag; }

*/
inline bool 
var_decl_desc::is_virtual_operand () const
{ 
  return node.base.u.bits.saturating_flag;
}

inline struct function *
function_decl_desc::function () const
{ return node.function_decl.f; }


inline enum built_in_class  
function_decl_desc::builtin_class () const
{
  return node.function_decl.built_in_class;
}

inline enum built_in_function
function_decl_desc::function_code () const
{
  return node.function_decl.function_code;
}

inline bool
function_decl_desc::builtin () const
{
  return builtin_class () != NOT_BUILT_IN;
}

inline bool
label_decl_desc::forced_label () const
{
  return node.base.side_effects_flag;
}

inline Gimple::value
value_list_desc::value () const
{ 
  return Gimple::value (node.list.value);
}

inline Gimple::value_ptr
value_list_desc::ptrto_value ()
{ 
  return Gimple::value_ptr (&(node.list.value));
}


inline value_list
value_list_desc::chain () const
{ 
  return value_list (node.common.chain);
}

inline Gimple::identifier
identifier_list_desc::value () const
{ 
  return Gimple::identifier (node.list.value);
}

inline identifier_list
identifier_list_desc::chain () const
{ 
  return identifier_list (node.common.chain);
}



inline Gimple::type
type_list_desc::value () const
{ 
  return Gimple::type (node.list.value);
}

inline type_list
type_list_desc::chain () const
{ 
  return type_list (node.common.chain);
}



//
// ssa_name
//

inline unsigned int 
ssa_name_desc::version () const
{ return node.base.u.version; }

inline void
ssa_name_desc::set_version (unsigned int v) 
{ node.base.u.version = v; }


inline bool
ssa_name_desc::occurs_in_abnormal_phi () const
{
  return node.base.asm_written_flag;
}

inline void
ssa_name_desc::set_occurs_in_abnormal_phi (bool f)
{
 node.base.asm_written_flag = f;
}

inline decl
ssa_name_desc::var () const
{ 
  /* node.ssa_name.var can be either a DECL or an IDENTIFIER. */
  if (node.ssa_name.var)
    return dyn_cast<decl> (value(node.ssa_name.var));
  else
    return decl();
}

inline Gimple::identifier
ssa_name_desc::identifier () const
{ 
  Gimple::identifier i;
  if (node.ssa_name.var)
    {
      i = dyn_cast<Gimple::identifier> (value(node.ssa_name.var));

      /* If this is not an identifier, it must be a Decl.  */
      if (!i)
	{
	  decl d = node.ssa_name.var;
	  i = d->name ();
	}
    }
  return i;
}

inline void
ssa_name_desc::set_var (decl decl)
{ node.ssa_name.var = decl; }

inline void
ssa_name_desc::set_identifier (Gimple::identifier ident)
{ node.ssa_name.var = ident; }

inline gimple 
ssa_name_desc::def_stmt () const
{ return node.ssa_name.def_stmt; }

inline void 
ssa_name_desc::set_def_stmt (gimple stmt)
{ node.ssa_name.def_stmt = stmt; }

inline struct ptr_info_def *
ssa_name_desc::ptr_info () const
{ return node.ssa_name.info.ptr_info; }

inline void 
ssa_name_desc::set_ptr_info (struct ptr_info_def *p)
{ node.ssa_name.info.ptr_info = p; }

inline bool 
ssa_name_desc::in_free_list () const
{ return node.base.nothrow_flag; }

inline void 
ssa_name_desc::set_in_free_list (bool f)
{ node.base.nothrow_flag = f; }

inline bool 
ssa_name_desc::is_default_def () const
{ return node.base.default_def_flag; }

inline void 
ssa_name_desc::set_is_default_def (bool f)
{ node.base.default_def_flag = f; }

inline struct ssa_use_operand_t * 
ssa_name_desc::imm_use_node_ptr () 
{ return &(node.ssa_name.imm_uses); }

inline bool
ssa_name_desc::same_base (const Gimple::ssa_name n) const
{ return (node.ssa_name.var == n->node.ssa_name.var); }

} // namespace Gimple

#endif /* GIMPLE_VALUE_H  */
