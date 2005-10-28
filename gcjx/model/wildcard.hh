// Type parameter wildcards.

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

#ifndef GCJX_MODEL_WILDCARD_HH
#define GCJX_MODEL_WILDCARD_HH

/// This class represents a wildcard in a generic type specification;
/// that is, the '?' in 'Type<?>'.  This handles both <? extends T>
/// and <? super T>.
class model_wildcard : public model_class
{
  // True if this is a 'super' wildcard, false for 'extends'.  A plain
  // '?' has this set to 'false'.
  bool is_super;

  // The type bound.  If there is no bound, this is NULL.
  ref_forwarding_type bound;

  void resolve_hook (resolution_scope *);

public:

  model_wildcard (const location &w)
    : model_class (w),
      is_super (false)
  {
  }

  model_wildcard (const location &w, model_class *b, bool spr = false)
    : model_class (w),
      is_super (spr),
      bound (new model_forwarding_resolved (w, b))
  {
  }

  void set_bound (const ref_forwarding_type &b)
  {
    bound = b;
  }

  void set_super ()
  {
    is_super = true;
  }

  bool super_p () const
  {
    return is_super;
  }

  bool wildcard_p () const
  {
    return true;
  }

  model_class *get_bound () const
  {
    if (! bound)
      return NULL;
    return assert_cast<model_class *> (bound->type ());
  }

  bool has_bound_p () const
  {
    return bool (bound);
  }

  model_type *erasure ();

  bool assignable_from_p (model_type *);

  bool contains_p (model_class *);

  std::string get_pretty_name () const
  {
    return name;
  }

  std::string get_signature ();

  void visit (visitor *);
};

#endif // GCJX_MODEL_WILDCARD_HH
