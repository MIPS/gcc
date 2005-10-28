// Conversions as specified by JLS.

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

#ifndef GCJX_CONVERSIONS_HH
#define GCJX_CONVERSIONS_HH

class model_expression;
class model_type;

model_type *binary_numeric_promotion (model_type *lhs_type,
				      model_type *rhs_type);

/// This handles binary numeric promotion.  It returns the promoted
/// type, and it replaces the expressions with casts if required.
model_type *binary_numeric_promotion (ref_expression &lhs,
				      ref_expression &rhs);

model_type *unary_numeric_promotion (model_type *op_type);

/// This handles unary numeric promotion.  It returns the promoted
/// type, and it replaces the expression with a cast if required.
model_type *unary_numeric_promotion (ref_expression &);

model_type *narrowing_primitive_conversion (model_type *to, model_type *from);

model_type *widening_primitive_conversion (model_type *to, model_type *from);

bool widening_reference_conversion (model_type *to, model_type *from);

model_type *widening_conversion (model_type *to, model_type *from);

model_type *assignment_conversion (model_type *lhs_type,
				   model_expression *rhs);

model_type *assignment_conversion (model_type *lhs, model_type *rhs);

model_type *assignment_conversion (model_type *lhs, ref_expression &rhs);

model_type *method_invocation_conversion (model_type *formal,
					  model_type *actual,
					  method_phase phase);

/// Note in this case we do not need a phase argument.
/// FIXME: document why...
void method_invocation_conversion (model_type *formal,
				   ref_expression &actual);

/// This implements capture conversion as defined in the JLS.
/// Essentially it replaces wildcards in a parameterization with new
/// type variables.
model_class_instance *capture_conversion (model_class_instance *);

/// Return the wrapper type for a given primitive type.  This also
/// works for `void'.  Only primitive types and void can be passed as
/// arguments.
model_class *box_primitive_type (model_type *in);

/// Apply boxing conversion to the argument type.  Returns null if
/// boxing conversion is not applicable.
model_type *boxing_conversion (model_type *in);

model_type *unboxing_conversion (model_type *in);

/// Return true if the type is convertible to 'boolean'.
bool boolean_conversion (model_type *in);

/// Return true if the type of the expression is convertible to
/// 'boolean'.  If the expression is of type java.lang.Boolean, it is
/// wrapped in an explicit cast.
bool boolean_conversion (ref_expression &expr);

bool casting_conversion (model_type *to, model_type *from);

bool constant_type_p (model_type *t);

/// Return true if the value of the expression is representable in the
/// given type.  This only works when both arguments are integral
/// types excluding 'long'.
bool representable_p (model_type *, model_expression *);

/// If the expression's type is not identical to the destination type,
/// wrap the expression in a cast to the destination type.
void maybe_cast_wrap (model_type *dest, ref_expression &expr);

/// Return true if the type is convertible to a numeric type.
bool convertible_to_numeric_p (model_type *);

/// Return true if the expression is commensurate with the type.  This
/// is used for checking whether annotation values are valid.
bool annotation_commensurate_p (model_type *, model_expression *);

#endif // GCJX_CONVERSIONS_HH
