// Walking the model.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_VISITOR_HH
#define GCJX_VISITOR_HH

class visitor
{
public:

  virtual ~visitor ()
  {
  }

  virtual void visit_method (model_method *,
			     const std::list<ref_variable_decl> &,
			     const ref_block &) = 0;

  virtual void visit_assert (model_assert *, const ref_expression &,
			     const ref_expression &) = 0;

  virtual void visit_block (model_block *,
			    const std::list<ref_stmt> &) = 0;

  virtual void visit_bytecode_block (model_bytecode_block *,
				     int, int, int, const uint8 *) = 0;

  virtual void visit_break (model_break *, const ref_stmt &) = 0;

  virtual void visit_catch (model_catch *, const ref_variable_decl &,
			    const ref_block &) = 0;

  virtual void visit_continue (model_continue *, const ref_stmt &) = 0;

  virtual void visit_class_decl_stmt (model_class_decl_stmt *,
				      const ref_class &) = 0;

  virtual void visit_do (model_do *, const ref_expression &,
			 const ref_stmt &) = 0;

  virtual void visit_empty (model_empty *) = 0;

  virtual void visit_expression_stmt (model_expression_stmt *,
				      const ref_expression &) = 0;

  virtual void visit_for_enhanced (model_for_enhanced *,
				   const ref_stmt &, const ref_expression &,
				   const ref_variable_decl &) = 0;

  virtual void visit_for (model_for *, const ref_stmt &,
			  const ref_expression &, const ref_stmt &,
			  const ref_stmt &) = 0;

  virtual void visit_if (model_if *, const ref_expression &,
			 const ref_stmt &, const ref_stmt &) = 0;

  virtual void visit_label (model_label *, const ref_stmt &) = 0;

  virtual void visit_return (model_return *, const ref_expression &) = 0;

  virtual void visit_switch (model_switch *,
			     const ref_expression &,
			     const std::list<ref_switch_block> &) = 0;

  virtual void visit_switch_block (model_switch_block *,
				   const std::list<ref_stmt> &) = 0;

  virtual void visit_synchronized (model_synchronized *,
				   const ref_expression &,
				   const ref_stmt &) = 0;

  virtual void visit_throw (model_throw *, const ref_expression &) = 0;

  virtual void visit_try (model_try *, const ref_block &,
			  const std::list<ref_catch> &, const ref_block &) = 0;

  virtual void visit_variable_stmt (model_variable_stmt *,
				    const std::list<ref_variable_decl> &) = 0;

  virtual void visit_while (model_while *, const ref_expression &,
			    const ref_stmt &) = 0;

  virtual void visit_array_initializer (model_array_initializer *,
					const ref_forwarding_type &,
					const std::list<ref_expression> &) = 0;

  virtual void visit_array_ref (model_array_ref *,
				const ref_expression &,
				const ref_expression &) = 0;

  virtual void visit_assignment (model_assignment *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_op_assignment (model_minus_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_mult_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_div_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_and_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_or_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_plus_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_xor_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_mod_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_ls_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_rs_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_op_assignment (model_urs_equal *,
				    const ref_expression &,
				    const ref_expression &) = 0;

  virtual void visit_arith_binary (model_minus *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_mult *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_div *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_mod *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_and *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_or *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_xor *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_binary (model_plus *,
				   const ref_expression &,
				   const ref_expression &) = 0;

  virtual void visit_arith_shift (model_left_shift *,
				  const ref_expression &,
				  const ref_expression &) = 0;

  virtual void visit_arith_shift (model_right_shift *,
				  const ref_expression &,
				  const ref_expression &) = 0;

  virtual void visit_arith_shift (model_unsigned_right_shift *,
				  const ref_expression &,
				  const ref_expression &) = 0;

  virtual void visit_cast (model_cast *, const ref_forwarding_type &,
			   const ref_expression &) = 0;

  virtual void visit_class_ref (model_class_ref *,
				const ref_forwarding_type &) = 0;

  virtual void visit_comparison (model_equal *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_comparison (model_notequal *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_comparison (model_lessthan *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_comparison (model_greaterthan *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_comparison (model_lessthanequal *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_comparison (model_greaterthanequal *,
				 const ref_expression &,
				 const ref_expression &) = 0;

  virtual void visit_conditional (model_conditional *,
				  const ref_expression &,
				  const ref_expression &,
				  const ref_expression &) = 0;

  virtual void visit_field_ref (model_field_ref *,
				const ref_expression &,
				model_field *) = 0;

  virtual void visit_field_initializer (model_field_initializer *,
					model_field *) = 0;

  virtual void visit_field (model_field *) = 0;

  virtual void visit_instanceof (model_instanceof *,
				 const ref_expression &,
				 const ref_forwarding_type &) = 0;

  virtual void visit_logical_binary (model_lor *,
				     const ref_expression &,
				     const ref_expression &) = 0;

  virtual void visit_logical_binary (model_land *,
				     const ref_expression &,
				     const ref_expression &) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jboolean &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jbyte &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jchar &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jshort &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jint &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jlong &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jfloat &val) = 0;

  virtual void visit_simple_literal (model_literal_base *,
				     const jdouble &val) = 0;

  virtual void visit_string_literal (model_string_literal *,
				     const std::string &val) = 0;

  virtual void visit_method_invocation (model_method_invocation *,
					model_method *,
					const ref_expression &,
					const std::list<ref_expression> &) = 0;

  virtual void
  visit_type_qualified_invocation (model_type_qualified_invocation *,
				   const model_method *,
				   const std::list<ref_expression> &,
				   bool) = 0;

  virtual void visit_super_invocation (model_super_invocation *,
				       const model_method *,
				       const std::list<ref_expression> &,
				       const ref_expression &) = 0;

  virtual void visit_this_invocation (model_this_invocation *,
				      const model_method *,
				      const std::list<ref_expression> &) = 0;

  virtual void visit_new (model_new *, const model_method *,
			  const ref_forwarding_type &,
			  const std::list<ref_expression> &) = 0;

  virtual void visit_new_array (model_new_array *,
				const ref_forwarding_type &,
				const std::list<ref_expression> &,
				const ref_expression &) = 0;

  virtual void visit_null_literal (model_null_literal *) = 0;

  virtual void visit_prefix_simple (model_prefix_plus *,
				    const ref_expression &) = 0;

  virtual void visit_prefix_simple (model_prefix_minus *,
				    const ref_expression &) = 0;

  virtual void visit_prefix_simple (model_bitwise_not *,
				    const ref_expression &) = 0;

  virtual void visit_prefix_simple (model_logical_not *,
				    const ref_expression &) = 0;

  virtual void visit_prefix_side_effect (model_prefix_plusplus *,
					 const ref_expression &) = 0;

  virtual void visit_prefix_side_effect (model_prefix_minusminus *,
					 const ref_expression &) = 0;

  virtual void visit_postfix_side_effect (model_postfix_plusplus *,
					  const ref_expression &) = 0;

  virtual void visit_postfix_side_effect (model_postfix_minusminus *,
					  const ref_expression &) = 0;

  virtual void visit_this (model_this *) = 0;

  virtual void visit_simple_variable_ref (model_simple_variable_ref *,
					  const model_variable_decl *) = 0;

  virtual void visit_forwarding_type (model_forwarding_type *,
                                      model_type *) = 0;

  virtual void visit_forwarding_resolved (model_forwarding_resolved *,
                                          model_type *) = 0;

  virtual void visit_forwarding_owned (model_forwarding_owned *,
                                       const ref_type &) = 0;

  virtual void visit_forwarding_simple (model_forwarding_simple *,
                                        const std::list<std::string> &) = 0;

  virtual void
  visit_forwarding_array (model_forwarding_array *,
                          const owner<model_forwarding_type> &) = 0;

  virtual void
  visit_forwarding_element (model_forwarding_element *,
                            const owner<model_forwarding_type> &) = 0;

  virtual void visit_forwarding_full (model_forwarding_full *,
                                      const std::string &) = 0;

  virtual void
  visit_forwarding_inner (model_forwarding_inner *,
                          const std::list<std::string> &,
                          const owner<model_forwarding_type> &) = 0;

  virtual void
  visit_forwarding_parameterized (model_forwarding_parameterized *,
                                  const owner<model_forwarding_type> &,
                                  const std::list< owner<model_forwarding_type> > &) = 0;

  virtual void visit_variable_decl (model_variable_decl *,
                                    const std::string &,
                                    const ref_forwarding_type &,
                                    const ref_expression &,
                                    bool,
                                    bool) = 0;

  virtual void visit_parameter_decl (model_variable_decl *,
                                     const std::string &,
                                     const ref_forwarding_type &,
                                     const ref_expression &,
                                     bool,
                                     bool) = 0;

  virtual void visit_catch_decl (model_variable_decl *,
                                 const std::string &,
                                 const ref_forwarding_type &,
                                 const ref_expression &,
                                 bool,
                                 bool) = 0;

  virtual void visit_package (model_package *,
                              const std::list<std::string> &) = 0;

  virtual void visit_type (model_type *, const std::string &) = 0;

  virtual void visit_identifier (model_identifier *, const std::string &) = 0;

  virtual void visit_annotation_member (model_annotation_member *,
                                        const ref_forwarding_type &) = 0;

  virtual void visit_annotation_value (model_annotation_value *,
                                       const std::string &,
                                       const ref_expression &) = 0;

  virtual void visit_import_single (model_import_single *,
                                    const std::list<std::string> &,
                                    model_class *) = 0;

  virtual void visit_import_on_demand (model_import_on_demand *,
                                       const std::list<std::string> &,
                                       Iname *, bool) = 0;

  virtual void visit_static_import_single (model_static_import_single *,
                                           const std::list<std::string> &,
                                           model_class *,
                                           const std::string &) = 0;

  virtual void visit_static_import_on_demand (model_static_import_on_demand *,
                                              const std::list<std::string> &,
                                              model_class *) = 0;
  
  virtual void visit_unit_source (model_unit_source *, model_package *,
                                  const std::list<ref_class> &,
                                  const std::string &,
                                  bool, const std::list<ref_import> &) = 0;
  
  virtual void visit_unit_class (model_unit_class *, model_package *,
                                 const std::list<ref_class> &,
                                 const std::string &, bool) = 0;
  
  virtual void visit_unit_fake (model_unit_fake *, model_package *,
                                const std::list<ref_class> &,
                                const std::string &, bool) = 0;

  virtual void visit_abstract_method (model_abstract_method *,
                                      const std::list<ref_variable_decl> &,
                                      const ref_block &,
                                      model_method *) = 0;

  virtual void visit_annotation_type (model_annotation_type *,
                                      const std::string &,
                                      const std::string &) = 0;
  
  virtual void visit_array_type (model_array_type *,
                                 const std::string &,
                                 const std::string &, model_type *) = 0;

  virtual void visit_class (model_class *, const std::string &,
                            const std::string &) = 0;

  virtual void visit_class_instance (model_class_instance *,
                                     const std::string &,
                                     const std::string &, model_class *) = 0;
  
  virtual void visit_constructor (model_constructor *,
                                  const std::list<ref_variable_decl> &,
                                  const ref_block &) = 0;

  virtual void visit_enum (model_enum *, const std::string &,
                           const std::string &,
                           const std::list<ref_enum_constant> &) = 0;

  virtual void visit_enum_constant (model_enum_constant *,
                                    const std::string &,
                                    const std::string &,
                                    const std::list<ref_expression> &) = 0;

  virtual void visit_fp_primitive (model_primitive_base *, char, jfloat) = 0;

  virtual void visit_fp_primitive (model_primitive_base *, char, jdouble) = 0;

  virtual void visit_int_primitive (model_primitive_base *, char,
                                    long long, long long, jbyte) = 0;

  virtual void visit_int_primitive (model_primitive_base *, char,
                                    long long, long long, jchar) = 0;

  virtual void visit_int_primitive (model_primitive_base *, char,
                                    long long, long long, jshort) = 0;

  virtual void visit_int_primitive (model_primitive_base *, char,
                                    long long, long long, jint) = 0;

  virtual void visit_int_primitive (model_primitive_base *, char,
                                    long long, long long, jlong) = 0;

  virtual void visit_primitive_boolean (model_primitive_boolean *) = 0;

  virtual void visit_initializer_block (model_initializer_block *,
                                        const std::list<ref_stmt> &,
                                        bool) = 0;

  virtual void visit_new_primary (model_new_primary *, const model_method *,
                                  const ref_forwarding_type &,
                                  const std::list<ref_expression> &,
                                  const std::string &,
                                  const std::list<ref_forwarding_type> &) = 0;

  virtual void visit_null_type (model_null_type *, const std::string &) = 0;

  virtual void visit_phony_block (model_phony_block *,
                                  const std::list<ref_stmt> &) = 0;

  virtual void visit_primordial_package (model_primordial_package *,
                                         const std::list<std::string> &) = 0;

  virtual void visit_unnamed_package (model_unnamed_package *,
                                      const std::list<std::string> &) = 0;

  virtual void visit_synthetic_this (model_synthetic_this *) = 0;

  virtual void visit_this_outer (model_this_outer *) = 0;

  virtual void
  visit_type_variable (model_type_variable *,
                       const std::string &, const std::string &,
                       const std::list<ref_forwarding_type> &) = 0;

  virtual void visit_void_type (model_void_type *, const std::string &) = 0;

  virtual void visit_wildcard (model_wildcard *, const std::string &,
                               const std::string &, bool,
                               const ref_forwarding_type &) = 0;

  virtual void visit_javadoc (model_javadoc *, bool) = 0;

  virtual void
  visit_generic_invocation (model_method_invocation *,
			    model_method *,
			    const ref_expression &,
                            const std::list<ref_expression> &,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_generic_invocation (model_type_qualified_invocation *,
                            const model_method *,
                            const std::list<ref_expression> &, bool,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_generic_invocation (model_super_invocation *,
                            const model_method *,
                            const std::list<ref_expression> &,
                            const ref_expression &,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_generic_invocation (model_this_invocation *, const model_method *,
                            const std::list<ref_expression> &,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_generic_invocation (model_new *, const model_method *,
                            const ref_forwarding_type &,
                            const std::list<ref_expression> &,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_generic_invocation (model_new_primary *, const model_method *,
                            const ref_forwarding_type &,
                            const std::list<ref_expression> &,
                            const std::string &,
                            const std::list<ref_forwarding_type> &,
                            const std::list<ref_forwarding_type> &) = 0;

  virtual void
  visit_annotation (model_annotation *, const ref_forwarding_type &,
                    const std::list<ref_annotation_value> &) = 0;
  virtual void
  visit_annotation_initializer (model_annotation_initializer *,
                                const ref_forwarding_type &,
                                const std::list<ref_expression> &) = 0;

  virtual void visit_memberref_enum (model_memberref_enum *,
                                     const ref_forwarding_type &,
                                     const std::string &) = 0;
};


// Handy function to map over a list.
template<typename T>
void
visit (visitor *v, const std::list<T> &l)
{
  for (typename std::list<T>::const_iterator i = l.begin ();
       i != l.end ();
       ++i)
    (*i)->visit (v);
}

#endif // GCJX_VISITOR_HH
