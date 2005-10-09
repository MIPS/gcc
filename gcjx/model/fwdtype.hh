// Type forwarding.

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

#ifndef GCJX_MODEL_FWDTYPE_HH
#define GCJX_MODEL_FWDTYPE_HH

// Most types aren't known at parse time, but instead must be looked
// up during resolution.  This class represents types of that sort.
// Another reason it is useful is to sort out ownership of types: an
// expression can't own a type, but it can own a type forwarder.
class model_forwarding_type : public model_element
{
protected:

  // The type to which we've resolved.
  model_type *resolved_type;

public:

  model_forwarding_type (const location &w)
    : model_element (w),
      resolved_type (NULL)
  {
  }

  model_forwarding_type (const location &w, const ref_type &t)
    : model_element (w),
      resolved_type (t.get ())
  {
  }

  model_forwarding_type (const location &w, model_type *t)
    : model_element (w),
      resolved_type (t)
  {
  }

  model_type *type () const
  {
    return resolved_type;
  }

  owner<model_forwarding_type> array ();

  void resolve_classes (resolution_scope *);

  virtual void resolve (resolution_scope *) = 0;

  // Return true if this represents an array, false otherwise.
  virtual bool array_p () const
  {
    return false;
  }

  void visit (visitor *);
};

/// This is a forwarding type whose target is already known when the
/// forwarder is built.
class model_forwarding_resolved : public model_forwarding_type
{
public:

  model_forwarding_resolved (const location &w, model_type *t)
    : model_forwarding_type (w, t)
  {
    assert (resolved_type);
  }

  model_forwarding_resolved (const location &w, const ref_type &t)
    : model_forwarding_type (w, t.get ())
  {
    assert (resolved_type);
  }

  void resolve (resolution_scope *)
  {
    // Nothing to do.
  }

  void visit (visitor *);
};

/// This is like model_forwarding_resolved, but it owns the resulting
/// type.
class model_forwarding_owned : public model_forwarding_resolved
{
  ref_type ref;

public:

  model_forwarding_owned (const location &w, const ref_type &t)
    : model_forwarding_resolved (w, t),
      ref (t)
  {
  }

  void visit (visitor *);
};  

class model_forwarding_simple : public model_forwarding_type
{
  // Name of the type.
  std::list<std::string> name;

public:

  model_forwarding_simple (const location &w, const std::list<std::string> &t)
    : model_forwarding_type (w),
      name (t)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

// This represents an array where the element type is also a
// forwarding type.
class model_forwarding_array : public model_forwarding_type
{
  // Element type.
  owner<model_forwarding_type> element;

public:

  model_forwarding_array (const owner<model_forwarding_type> &elt)
    : model_forwarding_type (LOCATION_UNKNOWN),
      element (elt)
  {
  }

  void resolve (resolution_scope *);

  bool array_p () const
  {
    return true;
  }

  void visit (visitor *);
};

// This represents the element type of an array where the array's type
// is also a forwarding type.
class model_forwarding_element : public model_forwarding_type
{
  // Array type.
  owner<model_forwarding_type> array_type;

public:

  model_forwarding_element (const owner<model_forwarding_type> &ary)
    : model_forwarding_type (LOCATION_UNKNOWN),
      array_type (ary)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

/// This is a forwarding type whose argument is always fully-qualified
/// and comes from a .class file.  In particular this means that
/// nested classes are represented using "$" notation, not as separate
/// list elements.
class model_forwarding_full : public model_forwarding_type
{
  // Name of the type.
  std::string name;

public:

  model_forwarding_full (const std::string &t)
    : model_forwarding_type (LOCATION_UNKNOWN),
      name (t)
  {
  }

  model_forwarding_full (const location &w, const std::string &t)
    : model_forwarding_type (w),
      name (t)
  {
  }

  void resolve (resolution_scope *);

  std::string get_name () const
  {
    return name;
  }

  void visit (visitor *);
};

/// This is a forwarding type which finds its argument as a member of
/// another type.  It can accept a multi-word argument.
class model_forwarding_inner : public model_forwarding_type
{
  // Name of the member type to find.
  std::list<std::string> name;

  // The enclosing class.
  owner<model_forwarding_type> parent;

public:

  model_forwarding_inner (const location &w,
			  const std::list<std::string> &n,
			  const owner<model_forwarding_type> &p)
    : model_forwarding_type (w),
      name (n),
      parent (p)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

/// This is a forwarding type whose argument is an instance of a
/// parameterized class.
class model_forwarding_parameterized : public model_forwarding_type
{
  // The base class.
  owner<model_forwarding_type> base;

  // List of the parameters to the base class.
  std::list< owner<model_forwarding_type> > parameters;

public:

  model_forwarding_parameterized (const location &w,
				  const owner<model_forwarding_type> &b)
    : model_forwarding_type (w),
      base (b)
  {
    assert (base);
  }

  model_forwarding_parameterized (const location &w,
				  const owner<model_forwarding_type> &b,
				  const std::list< owner<model_forwarding_type> > &ps)
    : model_forwarding_type (w),
      base (b),
      parameters (ps)
  {
    assert (base);
    assert (! parameters.empty ());
  }

  void add (const owner<model_forwarding_type> &param)
  {
    parameters.push_back (param);
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

const format &operator% (const format &, const owner<model_forwarding_type> &);

#endif // GCJX_MODEL_FWDTYPE_HH
