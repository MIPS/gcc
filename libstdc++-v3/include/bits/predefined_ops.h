// Default predicates for internal use -*- C++ -*-

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

/** @file predefined_ops.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _GLIBCXX_PREDEFINED_OPS_H
#define _GLIBCXX_PREDEFINED_OPS_H	1

namespace __gnu_cxx
{
namespace __ops
{
  struct less
  {
    template<typename _Lhs, typename _Rhs>
      bool
      operator()(const _Lhs& __lhs, const _Rhs& __rhs) const
      { return __lhs < __rhs; }
  };

  struct equal_to
  {
    template<typename _Lhs, typename _Rhs>
      bool
      operator()(const _Lhs& __lhs, const _Rhs& __rhs) const
      { return __lhs == __rhs; }
  };
  
  /**
   * @if maint
   * A class inspired by bind2nd and company, which wraps a function or
   * class and a reference, and produces a unary function object. It has less
   * requirements than the standard version and is designed to be use to wrap
   * binary predicates given to algorithms in the standard library.
   * @endif
  */
  template<typename _Comp, typename _Value>  
    class __bind2nd
    {
      _Comp _M_comp;
      const _Value& _M_value;

    public:
      explicit
      __bind2nd(_Comp __comp, const _Value& __inval) 
      : _M_comp(__comp), _M_value(__inval) {}
      
      template<typename _Lhs>
        bool
        operator()(const _Lhs& __lhs)
        { return _M_comp(__lhs, _M_value); }
    };

  /**
   * @if maint
   * Specialisation of __bind2nd for equality.
   * @endif
  */
  template<typename _Value>
    class __bind2nd<__gnu_cxx::__ops::equal_to, _Value>
    {
      const _Value& _M_value;

    public:
      explicit
      __bind2nd(__gnu_cxx::__ops::equal_to, const _Value& __inval)
      : _M_value(__inval) {}
      
      template<typename _Lhs>
      bool
      operator()(const _Lhs& __lhs)
      { return __lhs == _M_value; }
    };

  template<typename _Comp, typename _Value>
    inline __bind2nd<_Comp, _Value>
    bind2nd(_Comp __comp, const _Value& __value)
    { return __bind2nd<_Comp, _Value>(__comp, __value); }

} // namespace __ops
} // namespace __gnu_cxx

#endif
