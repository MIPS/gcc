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
  template<typename _Lhs, typename _Rhs = _Lhs>
    struct less
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs < __rhs; }
    
      bool
      operator()(const _Lhs& __lhs, const _Rhs& __rhs) const
      { return __lhs < __rhs; }
    
      bool
      operator()(const _Rhs& __lhs, const _Lhs& __rhs) const
      { return __lhs < __rhs; }
      
      bool
      operator()(const _Rhs& __lhs, const _Rhs& __rhs) const 
      { return __lhs < __rhs; }
    };

  template<typename _Lhs>
    struct less<_Lhs, _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs < __rhs; }
    };

  template<typename _Lhs>
    struct less<_Lhs, const _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs < __rhs; }
    };

  template<typename _Lhs>
    struct less<const _Lhs, _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs < __rhs; }
    };
 
  template<typename _Lhs, typename _Rhs = _Lhs>
    struct equal_to
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs == __rhs; }
    
      bool
      operator()(const _Lhs& __lhs, const _Rhs& __rhs) const
      { return __lhs == __rhs; }
    
      bool
      operator()(const _Rhs& __lhs, const _Lhs& __rhs) const
      { return __lhs == __rhs; }
      
      bool
      operator()(const _Rhs& __lhs, const _Rhs& __rhs) const 
      { return __lhs == __rhs; }
    };

  template<typename _Lhs>
    struct equal_to<_Lhs, _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs == __rhs; }
    };

  template<typename _Lhs>
    struct equal_to<_Lhs, const _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
      { return __lhs == __rhs; }
    };
  
  template<typename _Lhs>
    struct equal_to<const _Lhs, _Lhs>
    {
      bool
      operator()(const _Lhs& __lhs, const _Lhs& __rhs) const
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
  template<typename _Lhs, typename _Value, typename _Comp>  
    class __bind2nd
    {
      const _Value& _M_value;
      _Comp _M_comp;

    public:
      explicit
      __bind2nd(const _Value& __inval, _Comp __comp) 
      : _M_value(__inval), _M_comp(__comp) {}

      bool
      operator()(const _Lhs& __lhs)
      { return _M_comp(__lhs, _M_value); }
    };

  /**
   * @if maint
   * Specialisation of __bind2nd for equality.
   * @endif
  */
  template<typename _Lhs, typename _Value>
    class __bind2nd<_Lhs, _Value, __gnu_cxx::__ops::equal_to<_Lhs, _Value> >
    {
      const _Value& _M_value;

    public:
      explicit
      __bind2nd(const _Value& __inval,
		__gnu_cxx::__ops::equal_to<_Lhs, _Value>)
      : _M_value(__inval) {}

      bool
      operator()(const _Lhs& __lhs)
      { return __lhs == _M_value; }
    };

  template<typename _Lhs, typename _Value, typename _Comp>
    inline __bind2nd<_Lhs, _Value, _Comp>
    bind2nd(const _Value& __value, _Comp __comp)
    { return __bind2nd<_Lhs, _Value, _Comp>(__value, __comp); }

} // namespace __ops
} // namespace __gnu_cxx

#endif
