// Interface to a scope.

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

#ifndef GCJX_MODEL_ISCOPE_HH
#define GCJX_MODEL_ISCOPE_HH

class model_class;
class model_method;
class model_variable_decl;
class IContext;

/// This is used when determining whether we're in a static context.
typedef enum
  {
    /// This scope has no idea.
    DONT_KNOW,

    /// Yes, static context.
    STATIC_CONTEXT,

    /// No, not static context.
    NOT_STATIC_CONTEXT,
  } static_result;

/// This represents a scope and is used to implement name lookup.  It
/// is implemented by various elements that provide name mappings.
class IScope
{
public:

  virtual ~IScope ()
  {
  }

  virtual void look_up_name (const std::string &name,
			     std::set<model_class *> &result,
			     IContext *request,
			     model_class *qualifier)
  {
  }

  virtual void look_up_name (const std::string &name,
			     std::set<model_variable_decl *> &result,
			     IContext *request,
			     model_class *qualifier)
  {
  }

  virtual void look_up_name (const std::string &name,
			     std::set<model_method *> &result,
			     IContext *request,
			     model_class *qualifier)
  {
  }

  /// Add a new variable binding to the current scope.  For most types
  /// of scope, this is an invalid operation, so the default
  /// implementation aborts.
  virtual void add_binding (model_variable_decl *)
  {
    abort ();
  }

  /// Add a new class binding to the current scope.  For most types of
  /// scope, this is an invalid operation, so the default
  /// implementation aborts.
  virtual void add_binding (model_class *)
  {
    abort ();
  }

  /// Return an answer about whether we're in a static context.
  /// The default implementation always returns DONT_KNOW.
  virtual static_result is_static_scope () const
  {
    return DONT_KNOW;
  }
};

#endif // GCJX_MODEL_ISCOPE_HH
