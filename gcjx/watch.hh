// A wrapper that ensures that a variable is set before use.

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

#ifndef GCJX_WATCH_HH
#define GCJX_WATCH_HH

/// This class is used to catch a certain type of programming error.
/// This template wraps a value and verifies that the value is never
/// used before it is set.
template<typename T>
class watch
{
  // True if the value has been set.
  bool flag;

  // The wrapped value.
  T val;

public:

  watch ()
    : flag (false),
      val (T (0))
  {
  }

  watch (const watch<T> &v)
  {
    flag = v.flag;
    val = v.val;
  }

  operator T () const
  {
    assert (flag);
    return val;
  }

  watch<T> &operator= (const T &v)
  {
    flag = true;
    val = v;
    return *this;
  }

  watch<T> &operator= (const watch<T> &v)
  {
    assert (v.flag);
    flag = v.flag;
    val = v.val;
    return *this;
  }

  T operator-> () const
  {
    assert (flag);
    return val;
  }

  bool set_p () const
  {
    return flag;
  }
};

#endif // GCJX_WATCH_HH
