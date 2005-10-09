// Variable and field references.

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

#ifndef GCJX_MODEL_VARREF_HH
#define GCJX_MODEL_VARREF_HH

/// This represents a reference to a local variable, either a read or
/// an assignment.  Note that these aren't constructed by the parser,
/// but are constructed by forwarding member references as needed.
class model_simple_variable_ref : public model_expression
{
  // The simple name.
  std::string name;

  // The variable we're referencing.
  model_variable_decl *decl;

  // True if this is a use of a variable (not just setting it).
  bool is_use;

  bool compute_constant_p ()
  {
    return decl->constant_p ();
  }

public:

  model_simple_variable_ref (const location &w)
    : model_expression (w),
      decl (NULL),
      is_use (true)
  {
  }

  model_simple_variable_ref (const location &w, model_variable_decl *v)
    : model_expression (w),
      decl (v),
      is_use (true)
  {
    name = v->get_name ();
  }

  void set_name (const std::string &n)
  {
    name = n;
  }

  void set_variable (model_variable_decl *v)
  {
    assert (! decl);
    decl = v;
  }

  model_variable_decl *get_variable () const
  {
    return decl;
  }

  void resolve (resolution_scope *scope)
  {
    // A variable reference might be resolved multiple times, since it
    // can also be resolved during name lookup.
    if (! type_cache.set_p ())
      {
	set_type (decl->type ());
	decl->set_used ();
      }
  }

  void visit (visitor *);

  jvalue value ();

  std::string string_value ();

  void set_left_hand_side (bool compound)
  {
    // A compound assignment represents a use, a simple assignment
    // does not.
    is_use = compound;
  }
};

#endif // GCJX_MODEL_VARREF_HH
