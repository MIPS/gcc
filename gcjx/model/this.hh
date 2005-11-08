// "this" in various flavors.

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

#ifndef GCJX_MODEL_THIS_HH
#define GCJX_MODEL_THIS_HH

class model_this : public model_expression
{
  // This is the outer class to which we refer.  This is never NULL.
  ref_forwarding_type outer;

  bool compute_constant_p ()
  {
    return false;
  }

protected:

  // This is used to check some constraints on an ordinary
  // (user-supplied) "this".
  virtual void check_outer (model_class *);

public:

  model_this (const location &w, const ref_forwarding_type &t)
    : model_expression (w),
      outer (t)
  {
  }

  model_this (const location &w, model_type *t)
    : model_expression (w),
      outer (new model_forwarding_resolved (w, t))
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  /// This is used to check whether a class "matches".  By default
  /// this is just an equality relationship, but for synthetic outer
  /// "this" access we allow a subclass as well.
  virtual bool check_match (model_class *one, model_class *two)
  {
    return one->erasure () == two->erasure ();
  }

  /// This will create a new model_this object given a referring class
  /// and the declaring class of some member.  If the member is not
  /// declared in the referring class or one of its outer classes,
  /// this will return NULL.  IS_PRIVATE should be true if the
  /// referred-to member is "private".
  static model_this *get_synthetic_this (const location &w,
					 model_class *referrer,
					 model_class *declarer,
					 bool is_private = false);
};

/// Like model_this, but used in the case of "Outer.this".
class model_this_outer : public model_this
{
public:

  model_this_outer (const location &w, const ref_forwarding_type &t)
    : model_this (w, t)
  {
  }

  void visit (visitor *);
};

class model_synthetic_this : public model_this
{
protected:

  void check_outer (model_class *)
  {
    // Irrelevant for a synthetic this.
  }

public:

  model_synthetic_this (const location &w, model_type *t)
    : model_this (w, t)
  {
  }

  bool check_match (model_class *, model_class *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_THIS_HH
