// Meaning of a name.

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

#ifndef GCJX_NAME_HH
#define GCJX_NAME_HH

class Iname;
class model_class;
class model_element;
class model_package;

// Classify a fully-qualified name as a package name.  This will
// return the package object, or will throw an exception on error.
// The model_element is used for error reporting and is assumed to be
// the location of the fully-qualified identifier.
model_package *classify_package_name (model_element *,
				      const std::list<std::string> &);

// Classify a simple name as a type.  This will return the class, or
// will throw an exception on error.  The model_element is used for
// error reporting and is assumed to be the location of the
// fully-qualified identifier.
model_class *classify_type_name (resolution_scope *, model_element *,
				 const std::string &);

// Classify a qualified name as a type.  This will return the class,
// or will throw an exception on error.  The model_element is used for
// error reporting and is assumed to be the location of the
// fully-qualified identifier.  The final argument, when not NULL,
// will be used as the base context for this lookup.
model_class *classify_type_name (resolution_scope *, model_element *,
				 const std::list<std::string> &,
				 model_class * = NULL);

Iname *classify_package_or_type_name (resolution_scope *scope,
				      model_element *request,
				      const std::list<std::string> &name,
				      model_class * = NULL);

/// Classify a qualified name as an expression name.  This form is
/// used only for expression names which resolve to fields.  FIXME
/// fill in other info here, like do we throw or return NULL on error.
ref_expression classify_expression_name (resolution_scope *,
					 model_element *,
					 const std::list<std::string> &);

/// Classify a qualified name as an expression name.  This form is
/// used only for expression names which resolve to method calls.
/// FIXME fill in other info here, like do we throw or return NULL on
/// error.
ref_invocation_base classify_expression_name (resolution_scope *,
					      model_element *,
					      const std::list<std::string> &,
					      const std::list<ref_expression> &,
					      const std::list<ref_forwarding_type> &);

/// Look up a primitive type by its one-character descriptor.  Returns
/// NULL if it doesn't represent the descriptor of a primitive type.
model_type *classify_primitive_type_descriptor (const std::string &);

/// Look up the type represented by a descriptor as it would appear in
/// a class file.
model_type *classify_type_descriptor (resolution_scope *,
				      model_element *,
				      const std::string &);

/// Find a field in a class.  If the name is ambiguous, throw an
/// exception.  If no such field is found, return NULL.  Otherwise,
/// return the field.  The final argument is the qualifying class, if
/// required.  If NULL, the search class will be used.
model_field *classify_field_name (model_element *, model_class *,
				  model_class *, const std::string &,
				  model_class * = NULL);

/// Return an expression that refers to a particular local variable.
/// If needed, this will return a field reference to a captured local
/// variable.
ref_expression create_variable_reference (resolution_scope *scope,
					  model_element *request,
					  model_variable_decl *vdecl);

#endif // GCJX_NAME_HH
