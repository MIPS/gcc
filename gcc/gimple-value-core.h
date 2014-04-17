#ifndef GIMPLE_VALUE_CORE_H
#define GIMPLE_VALUE_CORE_H

#include "tree-core.h"

namespace Gimple {

template<typename T> class _ptr;
template<typename pT, typename dT> class _dptr;
template<typename T> class _addr;

#define FWD_DECL_BASE(NAME)	typedef _ptr<class NAME ## _desc> NAME
#define FWD_DECL(NAME, BASE)	typedef _dptr<class NAME ## _desc, BASE> NAME
#define FWD_DECL_PTR(NAME)	typedef _addr<NAME> NAME ## _ptr

typedef _ptr<class value_ops_desc> value;
FWD_DECL_BASE (type);
FWD_DECL_BASE (block);

FWD_DECL_PTR (value);

FWD_DECL (numerical_type, type);

FWD_DECL (identifier, value);
FWD_DECL (decl, value);
FWD_DECL (value_list, value);
FWD_DECL (type_list, value);
FWD_DECL (identifier_list, value);
FWD_DECL (ssa_name, value);
FWD_DECL (function_decl, value);
FWD_DECL (var_decl, value);


// This class accesses parts of trees that are common to all tree types.
// All methods other than comparisons are protected forcing inheriting
// classes to provide their own iterface.  This will ensure that any future  
// implementations of those classes will have any required methods exposed
// directly in that class.

class tree_desc {

// The new interface keeps the tree node in protected space.
// Original interface files can be identified by having included tree.h. 
// That require making access to the tree structures public and transparent
// so macros like TREE_CODE() which do not perform TREE_CHECKS can access the 
// base field directly.  Furthermore, when tree checking is turned off, most
// macros directly access nodes, so tree_desc must export the same fields as
// a tree node.
//
#ifdef GCC_TREE_H
  public:
    union {
      tree_node node;
      struct tree_base GTY ((tag ("TS_BASE"))) base;
      struct tree_typed GTY ((tag ("TS_TYPED"))) typed;
      struct tree_common GTY ((tag ("TS_COMMON"))) common;
      struct tree_int_cst GTY ((tag ("TS_INT_CST"))) int_cst;
      struct tree_real_cst GTY ((tag ("TS_REAL_CST"))) real_cst;
      struct tree_fixed_cst GTY ((tag ("TS_FIXED_CST"))) fixed_cst;
      struct tree_vector GTY ((tag ("TS_VECTOR"))) vector;
      struct tree_string GTY ((tag ("TS_STRING"))) string;
      struct tree_complex GTY ((tag ("TS_COMPLEX"))) complex;
      struct tree_identifier GTY ((tag ("TS_IDENTIFIER"))) identifier;
      struct tree_decl_minimal GTY((tag ("TS_DECL_MINIMAL"))) decl_minimal;
      struct tree_decl_common GTY ((tag ("TS_DECL_COMMON"))) decl_common;
      struct tree_decl_with_rtl GTY ((tag ("TS_DECL_WRTL"))) decl_with_rtl;
      struct tree_decl_non_common  GTY ((tag ("TS_DECL_NON_COMMON")))
	decl_non_common;
      struct tree_parm_decl  GTY  ((tag ("TS_PARM_DECL"))) parm_decl;
      struct tree_decl_with_vis GTY ((tag ("TS_DECL_WITH_VIS"))) decl_with_vis;
      struct tree_var_decl GTY ((tag ("TS_VAR_DECL"))) var_decl;
      struct tree_field_decl GTY ((tag ("TS_FIELD_DECL"))) field_decl;
      struct tree_label_decl GTY ((tag ("TS_LABEL_DECL"))) label_decl;
      struct tree_result_decl GTY ((tag ("TS_RESULT_DECL"))) result_decl;
      struct tree_const_decl GTY ((tag ("TS_CONST_DECL"))) const_decl;
      struct tree_type_decl GTY ((tag ("TS_TYPE_DECL"))) type_decl;
      struct tree_function_decl GTY ((tag ("TS_FUNCTION_DECL"))) function_decl;
      struct tree_translation_unit_decl GTY ((tag ("TS_TRANSLATION_UNIT_DECL")))
	translation_unit_decl;
      struct tree_type_common GTY ((tag ("TS_TYPE_COMMON"))) type_common;
      struct tree_type_with_lang_specific GTY ((tag ("TS_TYPE_WITH_LANG_SPECIFIC")))
	type_with_lang_specific;
      struct tree_type_non_common GTY ((tag ("TS_TYPE_NON_COMMON")))
	type_non_common;
      struct tree_list GTY ((tag ("TS_LIST"))) list;
      struct tree_vec GTY ((tag ("TS_VEC"))) vec;
      struct tree_exp GTY ((tag ("TS_EXP"))) exp;
      struct tree_ssa_name GTY ((tag ("TS_SSA_NAME"))) ssa_name;
      struct tree_block GTY ((tag ("TS_BLOCK"))) block;
      struct tree_binfo GTY ((tag ("TS_BINFO"))) binfo;
      struct tree_statement_list GTY ((tag ("TS_STATEMENT_LIST"))) stmt_list;
      struct tree_constructor GTY ((tag ("TS_CONSTRUCTOR"))) constructor;
      struct tree_omp_clause GTY ((tag ("TS_OMP_CLAUSE"))) omp_clause;
      struct tree_optimization_option GTY ((tag ("TS_OPTIMIZATION"))) optimization;
      struct tree_target_option GTY ((tag ("TS_TARGET_OPTION"))) target_option;
    };
#else
  protected:
    tree_node node;
#endif

  protected:
    inline void set_code (enum tree_code);
    inline enum tree_code_class code_class () const;

    bool test_node (void) const;
    bool test_node (enum tree_code_class) const;
    bool test_node (enum tree_code) const;
    bool test_node (enum tree_node_structure_enum) const;
    bool test_node (enum tree_code_class, enum tree_code_class) const;
    bool test_node (enum tree_code, enum tree_code) const;
    bool test_node (enum tree_code, enum tree_code,
               enum tree_code, enum tree_code,
               enum tree_code) const;

    void check_node (void) const;
    void check_node (enum tree_code_class) const;
    void check_node (enum tree_code) const;
    void check_node (enum tree_node_structure_enum) const;
    void check_node (enum tree_code_class, enum tree_code_class) const;
    void check_node (enum tree_code, enum tree_code) const;
    void check_node (enum tree_code, enum tree_code,
                enum tree_code, enum tree_code,
                enum tree_code) const;
  public:
    enum tree_code code () const;
    bool addressable () const;
    void set_addressable (const bool);
    bool this_volatile () const;
    void set_this_volatile (const bool);
    bool no_warning () const;
    void set_no_warning (const bool);
    bool used () const;
    void set_used (const bool);
    bool static_p () const;
    void set_static_p (const bool);
    bool side_effects () const;
    void set_side_effects (bool);
    bool constant () const;
    void set_constant (bool);

    template<typename T> friend class _ptr;
    template<typename T, typename D> friend class _dptr;
};


class type_desc : public tree_desc
{
  protected:
    enum machine_mode vector_mode() const;
  public:
    Gimple::type type() const;
    type_list chain () const;
    bool type_unsigned () const;
    enum machine_mode mode () const;
    addr_space_t addr_space () const;
    Gimple::type main_variant () const;
    Gimple::type canonical () const;
    unsigned int precision () const;
    value size () const;
    identifier_list attributes () const;

    bool complete_type_p () const;
    bool pointer_type_p () const;
    bool aggregate_type_p () const;
    bool integral_type_p () const;
    bool scalar_float_type_p () const;
    bool fixed_point_type_p () const;
    bool boolean_type_p () const;
    bool method_or_function_type_p () const;
    bool complex_type_p () const;
    bool vector_type_p () const;
    bool array_type_p () const;
};

class boolean_type_desc : public type_desc
{
};

class integer_type_desc : public type_desc
{
};

class real_type_desc : public type_desc
{
};

class fixed_point_type_desc : public type_desc
{
};

class function_or_method_type_desc : public type_desc
{
  public:
    Gimple::type_list arg_types() const;
};

class function_type_desc : public function_or_method_type_desc
{
};

class method_type_desc : public function_or_method_type_desc
{
  public:
    Gimple::type basetype () const;
};

class numerical_type_desc : public type_desc
{
  public:
    Gimple::value min_value () const;
    Gimple::value max_value () const;
};

class array_type_desc : public type_desc
{
  public:
    bool string_flag () const;
    numerical_type domain() const;
};

class complex_type_desc : public type_desc
{
};

class vector_type_desc : public type_desc
{
};

class block_desc : public tree_desc
{
  public:
    Gimple::value supercontext () const;
};

class value_desc : public tree_desc
{
  protected:
    inline value op (const int i) const;
    int op_len () const;
  public:
    Gimple::type type() const;
    void set_type (Gimple::type);
    location_t expr_location () const;
    bool clobber_p () const;
};

class value_ops_desc : public value_desc
{
  public:
    inline value op (const int i) const;
    int op_len () const;
};


class unary_desc : public value_desc
{
  public:
    value op1 () const;
};

class binary_desc : public value_desc
{
  public:
    value op1 () const;
    value op2 () const;
};

class modify_expr_desc : public binary_desc
{
};

class comparison_desc : public binary_desc
{
};

class array_ref_desc : public value_desc
{
};

class array_range_ref_desc : public value_desc
{
};

class identifier_desc : public value_desc
{
  public:
    const char *pointer() const;
};

class truth_not_expr_desc : public value_desc
{
};

class with_size_expr_desc : public value_desc
{
};

class bit_field_ref_desc : public value_desc
{
};

class mem_ref_desc : public value_desc
{
  public:
    value base() const;
    value offset() const;
};

class target_mem_ref_desc : public value_desc
{
  public:
    value base() const;
    value offset() const;
};

class obj_type_ref_desc : public value_desc
{
};

class addr_expr_desc : public value_desc
{
  public:
    value expr() const;
};

class call_expr_desc : public value_desc
{
};

class nop_expr_desc : public value_desc
{
};

class constructor_desc : public value_desc
{
  public:
  ::vec<constructor_elt, va_gc> *elts () const;
  void set_elts (::vec<constructor_elt, va_gc> *);
};

class case_label_expr_desc : public value_desc
{
  public:
    value case_low () const;
    value case_high () const;
    value case_label () const;
    value case_chain () const;
};

class constant_desc : public value_desc
{
  public:
    bool overflow_p () const;
    void set_overflow_p (bool);
};

class integer_cst_desc : public constant_desc
{
  public:
    double_int int_cst () const;
    unsigned HOST_WIDE_INT low () const;
    HOST_WIDE_INT high () const;
};

class real_cst_desc : public constant_desc
{
  public:
    REAL_VALUE_TYPE real_cst() const;
};

class decl_desc : public value_desc
{
  public:
    Gimple::identifier name() const;
    decl decl_abstract_origin () const;
    unsigned int align () const;
    void set_align (unsigned int a);
    bool pt_uid_set_p () const;
    unsigned int pt_uid () const;
    void set_pt_uid (unsigned int uid);

    bool decl_from_inline () const;
    bool readonly () const;
    void set_readonly (const bool);
    bool decl_ignored_p () const;
    bool external () const;
    void set_external (const bool);
    bool decl_visibility_specified() const;
    bool decl_comdat() const;

    bool decl_public() const;
    bool decl_asm_written() const;

    bool assembler_name_set_p () const;
    Gimple::identifier assembler_name () const;

    location_t source_location () const;
    void set_source_location (const location_t);
    bool gimple_reg_p () const;
    void set_gimple_reg_p (const bool);
    bool artificial () const;
    void set_artificial (const bool);
    bool ignored_p () const;
    void set_ignored_p (const bool);
    Gimple::value context() const;
    void set_context(Gimple::value);
    unsigned int uid() const;

    enum machine_mode mode () const;
    void set_mode (enum machine_mode);
/*
    bool is_type_context () const;
    Gimple::function_decl function_context () const;
    void set_function_context (Gimple::function_decl);
    Gimple::type type_context () const;
    void set_type_context (Gimple::type);
*/
    value_list attributes () const;
    void set_attributes (const value_list);


protected:
    bool decl_has_assembler_p () const;
    bool decl_has_rtl_p () const;
    bool decl_has_noncommon_p () const;
};

class debug_expr_decl_desc : public decl_desc
{
};

class decl_with_rtl_desc : public decl_desc
{
};

class decl_with_viz_desc : public decl_with_rtl_desc
{
  public:
    bool assembler_name_set_p () const;
    Gimple::identifier assembler_name ();
    bool seen_in_bind_expr_p () const;
    void set_seen_in_bind_expr_p (const bool);
    bool hard_register() const;

};

class decl_noncommon_desc : public decl_with_viz_desc
{
};


class function_decl_desc : public decl_noncommon_desc
{
  public:
    struct function *function () const;
    bool builtin () const;
    enum built_in_class builtin_class () const;
    enum built_in_function function_code () const;

};

class var_decl_desc : public decl_with_viz_desc
{
  public:
    bool is_virtual_operand () const;
};

class parm_decl_desc : public decl_with_rtl_desc
{
};

class result_decl_desc : public decl_with_rtl_desc
{
};


class label_decl_desc : public decl_with_rtl_desc
{
  public:
    bool forced_label () const;
};

class value_list_desc : public value_desc
{
  public:
    Gimple::value value() const;
    Gimple::value_ptr ptrto_value();
    value_list chain () const;
};

class type_list_desc : public value_list_desc
{
  public:
    Gimple::type value() const;
    type_list chain () const;
};

class identifier_list_desc : public value_list_desc
{
  public:
    Gimple::identifier value() const;
    identifier_list chain () const;
};

class ssa_name_desc : public value_desc
{
  public:
    unsigned int version () const;
    void set_version(unsigned int);
    decl var () const ;
    void set_var (decl);
    Gimple::identifier identifier () const;
    void set_identifier (Gimple::identifier);
    gimple def_stmt () const;
    void set_def_stmt (gimple);
    struct ptr_info_def *ptr_info () const;
    void set_ptr_info (struct ptr_info_def *);
    bool in_free_list () const;
    void set_in_free_list (bool);
    bool is_default_def () const;
    void set_is_default_def (bool);
    struct ssa_use_operand_t * imm_use_node_ptr ();
    bool occurs_in_abnormal_phi () const;
    void set_occurs_in_abnormal_phi (bool);
    bool same_base (const Gimple::ssa_name n) const;
};

} // namespace Gimple


#endif /* GIMPLE_VALUE_CORE_H */
