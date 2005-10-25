// An intersection type.

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

#ifndef GCJX_MODEL_INTERSECT_HH
#define GCJX_MODEL_INTERSECT_HH

/// This represents an intersection type.  An intersection type cannot
/// be created in user source code, but can be created by capture
/// conversion or type inference.  Intersection types are
/// characterized by a list of bounds.
class model_intersection_type : public model_class
{
  void compute_descriptor ()
  {
    // We override this method to avoid crashes if our superclass'
    // method is called.  There's no sensible value here however.    
  }

public:

  model_intersection_type (const location &, const std::list<model_class *> &);

  model_type *erasure ();

  bool assignable_from_p (model_type *);
};

#endif // GCJX_MODEL_INTERSECT_HH
