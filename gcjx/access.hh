// Access control.

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

#ifndef GCJX_ACCESS_HH
#define GCJX_ACCESS_HH

class IContext;
class IMember;
class model_class;
class model_type;

bool accessible_p (model_type *t, IContext *request);

/// Return true if the member is accessible from the request context.
/// The third argument is used to indicate a qualifying class; this is
/// used for handling 'protected' access checking on instance members.
/// It can be NULL.  The fourth argument is used only during
/// inheritance and is used to override the declaring class of the
/// member.
bool accessible_p (IMember *t, IContext *request,
		   model_class *qualifier,
		   model_class *declarer = NULL);

/// The runtime access model differs from the language's access model
/// in that the runtime treats all classes as top level.  So,
/// sometimes the language allows access to a member of an outer class
/// when the runtime would not.  In these cases, we must add
/// trampolines to allow the required access.  This function returns
/// true when class REQUEST must access member TARGET in this way.
/// This should only be called in situations where the language is
/// already known to allow access.  If the return value is true,
/// *RESULT will be set to the class to which the accessor should be
/// added; in the case of inherited protected members, this will be
/// some outer class of REQUEST.
bool trampoline_required_p (const IMember *target, model_class *request,
			    model_class **result);

#endif // GCJX_ACCESS_HH
