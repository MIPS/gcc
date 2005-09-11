// Implementing moveable types -*- C++ -*-

// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/** @file moveable.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _GLIBCXX_MOVEABLE_H
#define _GLIBCXX_MOVEABLE_H	1

namespace __gnu_cxx
{

  template<class _Tp>
    struct __is_moveable
    { static const bool __value = false; };

  template<class _Tp>
    struct __rvalref
    {
      _Tp& __ref;

      explicit
      __rvalref(_Tp& __inref) : __ref(__inref) { }
    };

  template<class _Tp, bool = __is_moveable<_Tp>::__value>
    struct __move_helper
    {
      typedef _Tp& __type;
      static __type
      __move(_Tp& __in)
      { return __in; }
    };

  template<class _Tp>
    struct __move_helper<_Tp, true>
    {
      typedef __rvalref<_Tp> __type;
      static __type 
      __move(_Tp& __in)
      { return __rvalref<_Tp>(__in); }
    };
       
  template<class _Tp>
    inline typename __move_helper<_Tp>::__type 
    __move(_Tp& __in)
    { return __move_helper<_Tp>::__move(__in); }

  // We shouldn't need this overload of __move.  It is needed in particular
  // for forward (and better) iterators whose operator*() returns by value.
  // The standard say there shouldn't be any such iterators.  Then someone
  // wrote vector<bool>, which breaks that rule.
  template<class _Tp>
    inline const _Tp&
    __move(const _Tp& __in)
    { return __in; }

} // namespace __gnu_cxx

#endif
