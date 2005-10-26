// Type unification.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_UNIFY_HH
#define GCJX_UNIFY_HH

/// Perform type inference according to the algorithm in the JLS.
/// @param actual the actual argument types
/// @param method the method being invoked
/// @param assignment_type if not null, the type to which the result
/// of this method is converted by assignment conversion.  If no
/// assignment conversion is performed, should be NULL.
/// @param result the resulting actual argument types
/// FIXME: actual result type ... ?
void
unify (const std::list<model_type *> &actual,
       model_method *method,
       model_class *assignment_type,
       model_type_map &result);

/// Compute the LUB of two classes.
model_class *compute_lub (model_element *request, model_class *,
			  model_class *);

#endif // GCJX_UNIFY_HH
