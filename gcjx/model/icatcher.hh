// Something that catches an exception.

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

#ifndef GCJX_MODEL_ICATCHER_HH
#define GCJX_MODEL_ICATCHER_HH

/// This interface represents a block-like object which surrounds
/// other statements which can throw unchecked exceptions.  When
/// resolving classes, an unchecked exception is propagated to the
/// innermost such catcher.
class ICatcher
{
public:

  virtual ~ICatcher ()
  {
  }

  /// This is called by something that can throw (e.g., a 'throw'
  /// statement).  The receiver is responsible for propagating this
  /// upward if it can't be handled locally.  The argument must be a
  /// checked exception type.
  virtual void note_throw_type (model_type *) = 0;
};

#endif // GCJX_MODEL_ICATCHER_HH
