// Wrap a pointer and provide ownership.

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

#ifndef GCJX_OWNER_HH
#define GCJX_OWNER_HH

template<typename T>
class owner
{
  T *value;

  void ref () const;

  void deref ();

  template<typename S> friend class owner;

public:

  owner ()
  {
    value = NULL;
  }

  owner (T *v)
    : value (v)
  {
    ref ();
  }

  // Constructor that accepts an owner of any subtype of the contained
  // type.
  template<typename S>
  owner (const owner<S> &other)
    : value (assert_cast<T *> (other.value))
  {
    ref ();
  }

  owner (const owner<T> &other)
    : value (other.value)
  {
    ref ();
  }

  ~owner ()
  {
    deref ();
  }

  // Accept an owner of any subtype of the contained type.
  template<typename S>
  owner<T> &operator= (const owner<S> &other)
  {
    other.ref ();
    deref ();
    value = assert_cast<T *> (other.value);
    return *this;
  }

  // We need this operator because g++ won't use the template above as
  // the default assignment operator.  I haven't looked but I assume
  // this is part of the C++ spec.
  owner<T> &operator= (const owner<T> &other)
  {
    other.ref ();
    deref ();
    value = other.value;
    return *this;
  }

  owner<T> &operator= (T *nv);

  operator bool () const
  {
    return value != NULL;
  }

  bool operator== (const owner<T> &other) const
  {
    return value == other.value;
  }

  bool operator!= (const owner<T> &other) const
  {
    return value != other.value;
  }

  // Explicit, not implicit, conversion to T*.  We don't want to
  // encourage this.
  T *get () const
  {
    return value;
  }

  T *operator-> () const
  {
    return value;
  }
};

#endif // GCJX_OWNER_HH
