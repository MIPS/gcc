// Represent 'new'.

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

#ifndef GCJX_MODEL_NEW_HH
#define GCJX_MODEL_NEW_HH

class model_new : public model_invocation_base
{
protected:

  // The class we're constructing.  This might be NULL initially, but
  // in all cases it will be set during resolution.
  ref_forwarding_type klass;

  // If we're creating a new instance of an anonymous class, then the
  // 'new' expression owns the class, and we keep ownership here.
  ref_class anonymous;

  void determine_enclosing_instance (resolution_scope *, model_class *,
				     ref_expression &, ref_expression &);

  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);
  void finish_search_class (resolution_scope *, model_class **,
			    model_class **);

public:

  model_new (const location &w)
    : model_invocation_base (w)
  {
    set_method ("<init>");
  }

  model_new (const location &w, model_type *t)
    : model_invocation_base (w),
      klass (new model_forwarding_resolved (w, t))
  {
    set_method ("<init>");
  }

  model_new (const location &w, const ref_forwarding_type &t)
    : model_invocation_base (w),
      klass (t)
  {
    set_method ("<init>");
  }

  model_new (location w, const std::list<std::string> &id)
    : model_invocation_base (w),
      klass (new model_forwarding_simple (w, id))
  {
    set_method ("<init>");
  }

  void set_class (const ref_forwarding_type &t)
  {
    assert (! klass);
    klass = t;
  }

  void set_anonymous (const ref_class &k)
  {
    anonymous = k;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

/// This is used in the 'primary . new Type' case.
class model_new_primary : public model_new
{
  // The simple name of the class.
  std::string simple_name;

  // Type parameters, if any.
  std::list<ref_forwarding_type> type_params;

protected:

  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);

public:

  model_new_primary (const location &w)
    : model_new (w)
  {
  }

  void set_simple_name (const std::string &sn)
  {
    simple_name = sn;
  }

  void set_simple_type_parameters (const std::list<ref_forwarding_type> &p)
  {
    type_params = p;
  }

  void visit (visitor *);
};

/// These typedefs are used to represent 'new' expressions with
/// explicit actual type parameters for their generic constructors.
typedef class model_generic_invocation<model_new> model_generic_new;
typedef class model_generic_invocation<model_new_primary> model_generic_new_primary;

#endif // GCJX_MODEL_NEW_HH
