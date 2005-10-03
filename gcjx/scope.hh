// Scopes used during resolution.

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

#ifndef GCJX_SCOPE_HH
#define GCJX_SCOPE_HH

class model_class;
class model_method;
class model_unit;
class model_variable_decl;
class ICatcher;
class IScope;
class IAnnotatable;

class resolution_scope : public warning_scope, public IScope
{
  // Stack of scope objects.
  std::deque<IScope *> scope_stack;

  // Cache the current class.
  model_class *current_class;

  // Update the class cache before popping a scope.
  void update_cache ();

public:

  resolution_scope ();

  ~resolution_scope ();

  void look_up_name (const std::string &name,
		     std::set<model_class *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_variable_decl *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_method *> &result,
		     IContext *request,
		     model_class *qualifier);

  void add_binding (model_variable_decl *var);

  void add_binding (model_class *klass);

  void search_for_type_name (const std::string &, std::set<model_class *> &,
			     IContext *, model_class *);

  void search_for_field_name (const std::string &,
			      std::set<model_variable_decl *> &,
			      IContext *, model_class *);

  /// Return the current compilation unit.  This can be NULL in
  /// special situations, for instance when resolving the imports of a
  /// compilation unit.
  model_unit *get_compilation_unit () const;

  // Return the class we are currently resolving.  This will return
  // NULL outside of a class context, for instance while resolving
  // import declarations.
  model_class *get_current_class () const
  {
    return current_class;
  }

  model_method *get_current_method () const;

  ICatcher *get_current_catcher () const;

  IContext *get_current_context () const;

  /// Return true if we're in a static context, false otherwise.
  bool static_p () const;

  /// Note the asymmetric API: we can push a scope on the stack with
  /// this method, but there is no way to pop a scope.
  void push_scope (IScope *);


  /// This class is used, via the RAII idiom, to push a new IScope on
  /// the current resolution_scope, and then later remove it when this
  /// object is destroyed.
  class push_iscope : public stack_temporary<IScope *>
  {
    resolution_scope *save;

  public:

    push_iscope (resolution_scope *, IScope *);

    ~push_iscope ()
    {
      save->update_cache ();
    }
  };
};

#endif // GCJX_SCOPE_HH
