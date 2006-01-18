// Code generator for bytecode.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#ifndef GCJX_LLVM_GEN_HH
#define GCJX_LLVM_GEN_HH

#include "codegen.hh"
#include "llvm/Module.h"
#include "llvm/llvmclass.hh"
#include "llvm/llvmfunction.hh"

using namespace llvm;

/// This is the primary interface to bytecode generation.
class llvm_code_generator : public visitor, public code_generator
{
public:

  /// Construct a new bytecode code generator.  Argument is the
  /// compiler being used to generate code.
  llvm_code_generator (compiler *, directory_cache &);
  virtual ~llvm_code_generator();

  /// This is called to generate code for a class and write it.
  void generate (model_class *);
  void generate (model_method *);

  void visit_method (model_method *,
		     const std::list<ref_variable_decl> &,
		     const ref_block &);

  void visit_assert (model_assert *, const ref_expression &,
		     const ref_expression &);

  void visit_block (model_block *,
		    const std::list<ref_stmt> &);

  void visit_bytecode_block (model_bytecode_block *, int, int, int,
			     const uint8 *);

  void visit_break (model_break *, const ref_stmt &);

  void visit_catch (model_catch *, const ref_variable_decl &,
		    const ref_block &);

  void visit_continue (model_continue *, const ref_stmt &);

  void visit_class_decl_stmt (model_class_decl_stmt *,
			      const ref_class &);

  void visit_do (model_do *, const ref_expression &,
		 const ref_stmt &);

  void visit_empty (model_empty *);

  void visit_expression_stmt (model_expression_stmt *,
			      const ref_expression &);

  void visit_for_enhanced (model_for_enhanced *,
			   const ref_stmt &, const ref_expression &,
			   const ref_variable_decl &);

  void visit_for (model_for *, const ref_stmt &,
		  const ref_expression &, const ref_stmt &,
		  const ref_stmt &);

  void visit_if (model_if *, const ref_expression &,
		 const ref_stmt &, const ref_stmt &);

  void visit_label (model_label *, const ref_stmt &);

  void visit_return (model_return *, const ref_expression &);

  void visit_switch (model_switch *,
		     const ref_expression &,
		     const std::list<ref_switch_block> &);

  void visit_switch_block (model_switch_block *,
			   const std::list<ref_stmt> &);

  void visit_synchronized (model_synchronized *,
			   const ref_expression &,
			   const ref_stmt &);

  void visit_throw (model_throw *, const ref_expression &);

  void visit_try (model_try *, const ref_block &,
		  const std::list<ref_catch> &, const ref_block &);

  void visit_variable_stmt (model_variable_stmt *,
			    const std::list<ref_variable_decl> &);

  void visit_while (model_while *, const ref_expression &,
		    const ref_stmt &);


  void visit_array_initializer (model_array_initializer *,
				const ref_forwarding_type &,
				const std::list<ref_expression> &);

  void visit_array_ref (model_array_ref *,
			const ref_expression &,
			const ref_expression &);

  void visit_assignment (model_assignment *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_method_invocation (model_method_invocation *,
				model_method *,
				const ref_expression &,
				const std::list<ref_expression> &);

  void visit_type_qualified_invocation (model_type_qualified_invocation *,
					const model_method *,
					const std::list<ref_expression> &,
					bool);

  void visit_super_invocation (model_super_invocation *,
			       const model_method *,
			       const std::list<ref_expression> &,
			       const ref_expression &);

  void visit_this_invocation (model_this_invocation *,
			      const model_method *,
			      const std::list<ref_expression> &);

  void visit_op_assignment (model_minus_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_mult_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_div_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_and_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_or_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_xor_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_mod_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_ls_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_rs_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_urs_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_plus_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_arith_binary (model_minus *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_mult *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_div *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_mod *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_and *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_or *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_xor *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_plus *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_shift (model_left_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_arith_shift (model_right_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_arith_shift (model_unsigned_right_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_cast (model_cast *, const ref_forwarding_type &,
		   const ref_expression &);

  void visit_class_ref (model_class_ref *,
			const ref_forwarding_type &);

  void visit_comparison (model_equal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_notequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_lessthan *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_greaterthan *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_lessthanequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_greaterthanequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_conditional (model_conditional *,
			  const ref_expression &,
			  const ref_expression &,
			  const ref_expression &);

  void visit_field_ref (model_field_ref *,
			const ref_expression &,
			model_field *);

  void visit_field_initializer (model_field_initializer *,
				model_field *);

  void visit_field (model_field *);

  void visit_instanceof (model_instanceof *,
			 const ref_expression &,
			 const ref_forwarding_type &);

  void visit_logical_binary (model_lor *,
			     const ref_expression &,
			     const ref_expression &);

  void visit_logical_binary (model_land *,
			     const ref_expression &,
			     const ref_expression &);

  void visit_simple_literal (model_literal_base *,
			     const jboolean &val);

  void visit_simple_literal (model_literal_base *,
			     const jbyte &val);

  void visit_simple_literal (model_literal_base *,
			     const jchar &val);

  void visit_simple_literal (model_literal_base *,
			     const jshort &val);

  void visit_simple_literal (model_literal_base *,
			     const jint &val);

  void visit_simple_literal (model_literal_base *,
			     const jlong &val);

  void visit_simple_literal (model_literal_base *,
			     const jfloat &val);

  void visit_simple_literal (model_literal_base *,
			     const jdouble &val);

  void visit_string_literal (model_string_literal *,
			     const std::string &val);

  void visit_new (model_new *, const model_method *,
		  const ref_forwarding_type &,
		  const std::list<ref_expression> &);

  void visit_new_array (model_new_array *,
			const ref_forwarding_type &,
			const std::list<ref_expression> &,
			const ref_expression &);

  void visit_null_literal (model_null_literal *);

  void visit_prefix_simple (model_prefix_plus *,
			    const ref_expression &);

  void visit_prefix_simple (model_prefix_minus *,
			    const ref_expression &);

  void visit_prefix_simple (model_bitwise_not *,
			    const ref_expression &);

  void visit_prefix_simple (model_logical_not *,
			    const ref_expression &);

  void visit_prefix_side_effect (model_prefix_plusplus *,
				 const ref_expression &);

  void visit_prefix_side_effect (model_prefix_minusminus *,
				 const ref_expression &);

  void visit_postfix_side_effect (model_postfix_plusplus *,
				  const ref_expression &);

  void visit_postfix_side_effect (model_postfix_minusminus *,
				  const ref_expression &);

  void visit_this (model_this *);

  void visit_simple_variable_ref (model_simple_variable_ref *,
				  const model_variable_decl *);

  void visit_forwarding_type (model_forwarding_type *,
                              model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_resolved (model_forwarding_resolved *,
                                  model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_owned (model_forwarding_owned *,
                               const ref_type &)
  {
    // Nothing.
  }

  void visit_forwarding_simple (model_forwarding_simple *,
                                const std::list<std::string> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_array (model_forwarding_array *,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_element (model_forwarding_element *,
                            const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void visit_forwarding_full (model_forwarding_full *,
                              const std::string &)
  {
    // Nothing.
  }

  void
  visit_forwarding_inner (model_forwarding_inner *,
                          const std::list<std::string> &,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_parameterized (model_forwarding_parameterized *,
                                  const owner<model_forwarding_type> &,
                                  const std::list< owner<model_forwarding_type> > &)
  {
    // Nothing.
  }
  void visit_variable_decl (model_variable_decl *,
                             const std::string &,
                             const ref_forwarding_type &,
                             const ref_expression &,
                             bool,
                             bool);


  void visit_parameter_decl (model_variable_decl *,
                             const std::string &,
                             const ref_forwarding_type &,
                             const ref_expression &,
                             bool,
                             bool)
  {
    // Nothing.
  }

  void visit_catch_decl (model_variable_decl *,
                         const std::string &,
                         const ref_forwarding_type &,
                         const ref_expression &,
                         bool,
                         bool)
  {
    // Nothing.
  }

  void visit_package (model_package *, const std::list<std::string> &)
  {
    // Nothing.
  }

  void visit_type (model_type *, const std::string &)
  {
    // Nothing.
  }

  void visit_identifier (model_identifier *, const std::string &)
  {
    // Nothing.
  }
  
  void visit_annotation_member (model_annotation_member *,
                                const ref_forwarding_type &)
  {
    // Nothing.
  }

  void visit_annotation_value (model_annotation_value *,
                               const std::string &, const ref_expression &)
  {
    // Nothing.
  }

  void visit_import_single (model_import_single *,
                            const std::list<std::string> &, model_class *)
  {
    // Nothing.
  }

  void visit_import_on_demand (model_import_on_demand *,
                               const std::list<std::string> &, Iname *, bool)
  {
    // Nothing.
  }

  void visit_static_import_single (model_static_import_single *,
				   const std::list<std::string> &,
				   model_class *, const std::string &)
  {
    // Nothing.
  }

  void visit_static_import_on_demand (model_static_import_on_demand *,
				      const std::list<std::string> &,
				      model_class *)
  {
    // Nothing.
  }
  
  void visit_unit_source (model_unit_source *, model_package *,
			  const std::list<ref_class> &,
			  const std::string &,
			  bool, const std::list<ref_import> &)
  {
    // Nothing.
  }
  
  void visit_unit_class (model_unit_class *, model_package *,
			 const std::list<ref_class> &,
			 const std::string &, bool)
  {
    // Nothing.
  }
  
  void visit_unit_fake (model_unit_fake *, model_package *,
			const std::list<ref_class> &,
			const std::string &, bool)
  {
    // Nothing.
  }

  void visit_abstract_method (model_abstract_method *am,
                              const std::list<ref_variable_decl> &params,
			      const ref_block &body, model_method *)
  {
    visit_method (am, params, body);
  }

  void visit_annotation_type (model_annotation_type *at,
                              const std::string &descr,
                              const std::string &name)
  {
    visit_class (at, descr, name);
  }
  
  void visit_array_type (model_array_type *at, const std::string &descr,
                         const std::string &name, model_type *)
  {
    visit_class (at, descr, name);
  }


  void visit_class_instance (model_class_instance *ci,
                             const std::string &descr,
                             const std::string &name, model_class *)
  {
    visit_class (ci, descr, name);
  }
  
  void visit_constructor (model_constructor *c,
                          const std::list<ref_variable_decl> &params,
                          const ref_block &body)
  {
    visit_method (c, params, body);
  }

  void visit_enum (model_enum *e, const std::string &descr,
                   const std::string &name,
                   const std::list<ref_enum_constant> &)
  {
    visit_class (e, descr, name);
  }

void visit_class (model_class *c, const std::string &descr,
                    const std::string &name);

  void visit_enum_constant (model_enum_constant *ec,
                            const std::string &descr,
                            const std::string &name,
                            const std::list<ref_expression> &)
  {
    visit_class (ec, descr, name);
  }

  void visit_fp_primitive (model_primitive_base *, char, jfloat)
  {
    // Nothing.
  }

  void visit_fp_primitive (model_primitive_base *, char, jdouble)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jbyte)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jchar)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jshort)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jint)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jlong)
  {
    // Nothing.
  }

  void visit_primitive_boolean (model_primitive_boolean *)
  {
    // Nothing.
  }

  void visit_initializer_block (model_initializer_block *ib,
                                const std::list<ref_stmt> &stmts, bool)
  {
    visit_block (ib, stmts);
  }

  void visit_new_primary (model_new_primary *np, const model_method *meth,
			  const ref_forwarding_type &klass,
			  const std::list<ref_expression> &args,
			  const std::string &,
			  const std::list<ref_forwarding_type> &)
  {
    visit_new (np, meth, klass, args);
  }

  void visit_null_type (model_null_type *nt, const std::string &descr)
  {
    visit_type (nt, descr);
  }

  void visit_phony_block (model_phony_block *pb,
                          const std::list<ref_stmt> &stmts)
  {
    visit_block (pb, stmts);
  }

  void visit_primordial_package (model_primordial_package *pp,
                                 const std::list<std::string> &name)
  {
    visit_package (pp, name);
  }

  void visit_unnamed_package (model_unnamed_package *up,
                              const std::list<std::string> &name)
  {
    visit_package (up, name);
  }

  void visit_synthetic_this (model_synthetic_this *st)
  {
    visit_this (st);
  }

  void visit_this_outer (model_this_outer *to)
  {
    visit_this (to);
  }

  void
  visit_type_variable (model_type_variable *tv,
                       const std::string &descr, const std::string &name,
                       const std::list<ref_forwarding_type> &)
  {
    visit_class (tv, descr, name);
  }

  void visit_void_type (model_void_type *vt, const std::string &descr)
  {
    visit_type (vt, descr);
  }

  void visit_wildcard (model_wildcard *w, const std::string &descr,
                       const std::string &name, bool,
                       const ref_forwarding_type &)
  {
    visit_class (w, descr, name);
  }

  void visit_javadoc (model_javadoc *, bool)
  {
    // Nothing.
  }

  void
  visit_generic_invocation (model_method_invocation *mi,
			    model_method *meth,
			    const ref_expression &qual,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    visit_method_invocation (mi, meth, qual, args);
  }

  void
  visit_generic_invocation (model_type_qualified_invocation *tqi,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
			    bool is_super,
                            const std::list<ref_forwarding_type> &)
  {
    visit_type_qualified_invocation (tqi, meth, args, is_super);
  }

  void
  visit_generic_invocation (model_super_invocation *si,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const ref_expression &finit,
                            const std::list<ref_forwarding_type> &)
  {
    visit_super_invocation (si, meth, args, finit);
  }

  void
  visit_generic_invocation (model_this_invocation *ti,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    visit_this_invocation (ti, meth, args);
  }

  void
  visit_generic_invocation (model_new *n, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    visit_new (n, meth, klass, args);
  }

  void
  visit_generic_invocation (model_new_primary *np, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::string &simple_name,
                            const std::list<ref_forwarding_type> &type_params,
                            const std::list<ref_forwarding_type> &)
  {
    visit_new_primary (np, meth, klass, args, simple_name, type_params);
  }

  void
  visit_annotation (model_annotation *, const ref_forwarding_type &,
                    const std::list<ref_annotation_value> &)
  {
    abort ();
  }

  void
  visit_annotation_initializer (model_annotation_initializer *,
                                const ref_forwarding_type &,
                                const std::list<ref_expression> &)
  {
    abort ();
  }

  void visit_memberref_enum (model_memberref_enum *,
                             const ref_forwarding_type &,
                             const std::string &)
  {
    abort ();
  }

private:
  	// This maps our types to llvm types.
  	std::map<model_type *,Type *> typemap;
  	std::map<const model_variable_decl *,Value *> localmap;
  	std::map<const model_field *,Value *> fieldMap;
  	std::map<const model_method *,llvm_function *> methodMap;
	Module* module;
	llvm_function *currentMethod;
	llvm_class *currentClass;
	const Type * convertType( model_type *type );
	Value * castToType(Value *V, const Type *Ty);
	llvm_function * get_method (model_method * method );

};

#endif // GCJX_LLVM_GEN_HH
