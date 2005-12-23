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

// Would include <iterator>, but we only need iterator_traits and need to
// avoid cyclic dependancies.
#include <bits/c++config.h>
#include <cstddef>
#include <bits/stl_iterator_base_types.h>

_GLIBCXX_BEGIN_NAMESPACE(__gnu_cxx)

  template<typename _Tp>
    struct __is_moveable
    { static const bool __value = false; };

  template<typename _Tp>
    struct __rvalref
    {
      _Tp& __ref;

      explicit
      __rvalref(_Tp& __inref) : __ref(__inref) { }
    };

  template<typename _Tp, bool = __is_moveable<_Tp>::__value>
    struct __move_helper
    {
      typedef _Tp& __type;
      static __type
      __move(_Tp& __in)
      { return __in; }
    };

  template<typename _Tp>
    struct __move_helper<_Tp, true>
    {
      typedef __rvalref<_Tp> __type;
      static __type 
      __move(_Tp& __in)
      { return __rvalref<_Tp>(__in); }
    };
       
  template<typename _Tp>
    inline typename __move_helper<_Tp>::__type 
    __move(_Tp& __in)
    { return __move_helper<_Tp>::__move(__in); }

  // We shouldn't need this overload of __move.  It is needed in particular
  // for forward (and better) iterators whose operator*() returns by value.
  // The standard say there shouldn't be any such iterators.  Then someone
  // wrote vector<bool>, which breaks that rule.
  template<typename _Tp>
    inline const _Tp&
    __move(const _Tp& __in)
    { return __in; }

  /**
   *  @if maint
   *  This class takes a range of iterators and wraps them so when they
   *  are dereferenced they return any rvalue reference instead of a plain
   *  reference. While you can wrap forward and better classes
   *  of iterators, the resulting iterators aren't really iterators of that
   *  type, as operator* returns by value not reference.
   *  @endif
   */
  template<typename _Iterator>
    struct __move_iterator
    {
      _Iterator __iterator;

      typedef _Iterator __iterator_type;
      typedef typename std::iterator_traits<_Iterator>::difference_type
        difference_type;
      typedef typename std::iterator_traits<_Iterator>::pointer
        pointer;
      typedef typename std::iterator_traits<_Iterator>::value_type
        value_type;
      typedef typename std::iterator_traits<_Iterator>::iterator_category
        iterator_category;
      typedef typename __move_helper<value_type>::__type reference;

      __move_iterator() 
      { }
    
      explicit __move_iterator(__iterator_type __in) 
      : __iterator(__in)
      { }
    
      template<typename _Tp> 
	__move_iterator(const __move_iterator<_Tp>& __in)
	: __iterator(__in)
	{ }

      __iterator_type 
      base() const
      { return __iterator; }
    
      reference 
      operator*() const
      { return __gnu_cxx::__move(*__iterator); }
    
      pointer   
      operator->() const
      { return &**this; }

      __move_iterator& 
      operator++()
      { ++__iterator; }
    
      __move_iterator  
      operator++(int)
      { 
	__move_iterator __tmp(*this);
	++__iterator;
	return __tmp;
      }
      
      __move_iterator& 
      operator--()
      { --__iterator; }
    
      __move_iterator  
      operator--(int)
      {
	__move_iterator __tmp(*this);
	--__iterator;
	return __tmp;
      }

      __move_iterator  
      operator+(difference_type __distance) const
      { return __move_iterator(__iterator + __distance); }

      __move_iterator& 
      operator+=(difference_type __distance)
      {
	__iterator += __distance;
	return *this;
      }
    
      __move_iterator  
      operator-(difference_type __distance) const
      { return __move_iterator(__iterator - __distance); }
    
      __move_iterator& 
      operator-=(difference_type __distance)
      { 
	__iterator -= __distance;
	return *this;
      }
    
      reference 
      operator[](difference_type __distance) const
      { return __iterator[__distance]; }
    };

  template<typename _Iterator>
    bool
    operator==(const __move_iterator<_Iterator>& __lhs,
	       const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator == __rhs.__iterator; }

  template<typename _Iterator>
    bool
    operator!=(const __move_iterator<_Iterator>& __lhs,
	       const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator != __rhs.__iterator; }

  template<typename _Iterator>
    bool
    operator<(const __move_iterator<_Iterator>& __lhs,
	      const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator < __rhs.__iterator; }

  template<typename _Iterator>
    bool
    operator<=(const __move_iterator<_Iterator>& __lhs,
	       const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator <= __rhs.__iterator; }

  template<typename _Iterator>
    bool
    operator>(const __move_iterator<_Iterator>& __lhs,
	      const __move_iterator<_Iterator>& __rhs)
    { return __rhs.__iterator > __lhs.__iterator; }

  template<typename _Iterator>
    bool
    operator>=(const __move_iterator<_Iterator>& __lhs,
	       const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator >= __rhs.__iterator; }

  template<typename _Iterator>
    typename __move_iterator<_Iterator>::difference_type
    operator-(const __move_iterator<_Iterator>& __lhs,
	      const __move_iterator<_Iterator>& __rhs)
    { return __lhs.__iterator - __rhs.__iterator; }

  template<typename _Iterator>
    __move_iterator<_Iterator>
    operator+(typename __move_iterator<_Iterator>::difference_type __distance,
	      const __move_iterator<_Iterator>& __iterator)
    { return __distance + __iterator; }

  template<typename _Iterator>
    __move_iterator<_Iterator>
    __make_move_iterator(const _Iterator& __iterator)
    { return __move_iterator<_Iterator>(__iterator); }

_GLIBCXX_END_NAMESPACE

#endif
