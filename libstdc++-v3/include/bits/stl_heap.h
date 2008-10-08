// Heap implementation -*- C++ -*-

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
// Free Software Foundation, Inc.
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
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file stl_heap.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _STL_HEAP_H
#define _STL_HEAP_H 1

#include <debug/debug.h>
#include <bits/concepts.h>

_GLIBCXX_BEGIN_NAMESPACE(std)

  // is_heap, a predicate testing whether or not a range is
  // a heap.  This function is an extension, not part of the C++
  // standard.
  template<RandomAccessIterator _Iter, IntegralLike _Distance>
    requires Convertible<_Distance, _Iter::difference_type>
          && LessThanComparable<_Iter::value_type>
    bool
    __is_heap(_Iter __first, _Distance __n)
    {
      _Distance __parent = 0;
      for (_Distance __child = 1; __child < __n; ++__child)
        {
          if (__first[__parent] < __first[__child])
            return false;
          if ((__child & 1) == 0)
            ++__parent;
        }
      return true;
    }

  template<RandomAccessIterator _Iter, IntegralLike _Distance,
           BinaryPredicate<auto, _Iter::value_type, _Iter::value_type> _Compare>
    requires Convertible<_Distance, _Iter::difference_type>
    bool
    __is_heap(_Iter __first, _Compare __comp,
              _Distance __n)
    {
      _Distance __parent = 0;
      for (_Distance __child = 1; __child < __n; ++__child)
        {
          if (__comp(__first[__parent], __first[__child]))
            return false;
          if ((__child & 1) == 0)
            ++__parent;
        }
      return true;
    }

  template<RandomAccessIterator _Iter>
    requires LessThanComparable<_Iter::value_type>
    inline bool
    __is_heap(_Iter __first, _Iter __last)
    { return std::__is_heap(__first, std::distance(__first, __last)); }

  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    inline bool
    __is_heap(_Iter __first, _Iter __last, _Compare __comp)
    { return std::__is_heap(__first, __comp, std::distance(__first, __last)); }

  template<RandomAccessIterator _Iter>
    requires LessThanComparable<_Iter::value_type>
    inline bool
    is_heap(_Iter __first, _Iter __last)
    { return std::__is_heap(__first, std::distance(__first, __last)); }

  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    inline bool
    is_heap(_Iter __first, _Iter __last, _Compare __comp)
    { return std::__is_heap(__first, __comp, std::distance(__first, __last)); }

  // Heap-manipulation functions: push_heap, pop_heap, make_heap, sort_heap.

  template<RandomAccessIterator _Iter, IntegralLike _Distance>
    requires Convertible<_Distance, _Iter::difference_type>
          && ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    void
    __push_heap(_Iter __first,
                _Distance __holeIndex, _Distance __topIndex, 
                _Iter::value_type __value)
    {
      _Distance __parent = (__holeIndex - 1) / 2;
      while (__holeIndex > __topIndex && *(__first + __parent) < __value)
        {
          *(__first + __holeIndex) = std_move(*(__first + __parent));
          __holeIndex = __parent;
          __parent = (__holeIndex - 1) / 2;
        }
      *(__first + __holeIndex) = std_move(__value);
    }

  /**
   *  @brief  Push an element onto a heap.
   *  @param  first  Start of heap.
   *  @param  last   End of heap + element.
   *  @ingroup heap
   *
   *  This operation pushes the element at last-1 onto the valid heap over the
   *  range [first,last-1).  After completion, [first,last) is a valid heap.
  */
  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    inline void
    push_heap(_Iter __first, _Iter __last)
    {
      typedef _Iter::value_type _ValueType;
      typedef _Iter::difference_type _DistanceType;

      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap(__first, __last - 1);

      std::__push_heap(__first, _DistanceType((__last - __first) - 1),
                       _DistanceType(0), _ValueType(*(__last - 1)));
    }

  template<RandomAccessIterator _Iter, IntegralLike _Distance,
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter>
          && Convertible<_Distance, _Iter::difference_type>
    void
    __push_heap(_Iter __first, _Distance __holeIndex,
                _Distance __topIndex, 
                _Iter::value_type __value, 
                _Compare __comp)
    {
      _Distance __parent = (__holeIndex - 1) / 2;
      while (__holeIndex > __topIndex
             && __comp(*(__first + __parent), __value))
        {
          *(__first + __holeIndex) = std_move(*(__first + __parent));
          __holeIndex = __parent;
          __parent = (__holeIndex - 1) / 2;
        }
      *(__first + __holeIndex) = std_move(__value);
    }

  /**
   *  @brief  Push an element onto a heap using comparison functor.
   *  @param  first  Start of heap.
   *  @param  last   End of heap + element.
   *  @param  comp   Comparison functor.
   *  @ingroup heap
   *
   *  This operation pushes the element at last-1 onto the valid heap over the
   *  range [first,last-1).  After completion, [first,last) is a valid heap.
   *  Compare operations are performed using comp.
  */
  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter> && CopyConstructible<_Compare>
    inline void
    push_heap(_Iter __first, _Iter __last,
              _Compare __comp)
    {
      typedef _Iter::value_type _ValueType;
      typedef _Iter::difference_type _DistanceType;

      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap_pred(__first, __last - 1, __comp);

      _ValueType __value(std_move(*(__last - 1)));
      std::__push_heap(__first, _DistanceType((__last - __first) - 1),
                       _DistanceType(0), std_move(__value), __comp);
    }

  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    void
    __adjust_heap(_Iter __first, _Iter::difference_type __holeIndex,
                  _Iter::difference_type __len, 
                  RandomAccessIterator<_Iter>::value_type __value)
    {
      const _Iter::difference_type __topIndex = __holeIndex;
      _Iter::difference_type __secondChild = __holeIndex;
      while (__secondChild < (__len - 1) / 2)
	{
	  __secondChild = 2 * (__secondChild + 1);
	  if (*(__first + __secondChild) < *(__first + _Iter::difference_type(__secondChild - 1)))
	    __secondChild--;
	  *(__first + __holeIndex) = std_move(*(__first + __secondChild));
	  __holeIndex = __secondChild;
	}
      if ((__len & 1) == 0 && __secondChild == (__len - 2) / 2)
	{
	  __secondChild = 2 * (__secondChild + 1);
	  *(__first + __holeIndex) = std_move(*(__first + _Iter::difference_type(__secondChild - 1)));
	  __holeIndex = __secondChild - 1;
	}
      std::__push_heap(__first, __holeIndex, __topIndex, std_move(__value));
    }

  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    inline void
    __pop_heap(_Iter __first, _Iter __last,
               _Iter __result)
    {
      typedef _Iter::difference_type _Distance;
      _Iter::value_type __value(std_move(*__result));
      *__result = std_move(*__first);
      std::__adjust_heap(__first, _Distance(0), _Distance(__last - __first),
                         std_move(__value));
    }

  /**
   *  @brief  Pop an element off a heap.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @ingroup heap
   *
   *  This operation pops the top of the heap.  The elements first and last-1
   *  are swapped and [first,last-1) is made into a heap.
  */
  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    inline void
    pop_heap(_Iter __first, _Iter __last)
    {
      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap(__first, __last);

      --__last;
      std::__pop_heap(__first, __last, __last);
    }

  template<RandomAccessIterator _Iter,
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter>
    void
    __adjust_heap(_Iter __first, _Iter::difference_type __holeIndex,
                  _Iter::difference_type __len, 
                  _Iter::value_type __value, 
                  _Compare __comp)
    {
      const _Iter::difference_type __topIndex = __holeIndex;
      _Iter::difference_type __secondChild = __holeIndex;
      while (__secondChild < (__len - 1) / 2)
	{
	  __secondChild = 2 * (__secondChild + 1);
	  if (__comp(*(__first + __secondChild),
		     *(__first + _Iter::difference_type(__secondChild - 1))))
	    __secondChild--;
	  *(__first + __holeIndex) = std_move(*(__first + __secondChild));
	  __holeIndex = __secondChild;
	}
      if ((__len & 1) == 0 && __secondChild == (__len - 2) / 2)
	{
	  __secondChild = 2 * (__secondChild + 1);
	  *(__first + __holeIndex) = std_move(*(__first + _Iter::difference_type(__secondChild - 1)));
	  __holeIndex = __secondChild - 1;
	}
      std::__push_heap(__first, __holeIndex, __topIndex, __value, __comp);
    }

  template<RandomAccessIterator _Iter,
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter> && CopyConstructible<_Compare>
    inline void
    __pop_heap(_Iter __first, _Iter __last,
               _Iter __result, 
               _Compare __comp)
    {
      typedef _Iter::difference_type _Distance;
      _Iter::value_type __value(std_move(*__result));
      *__result = std_move(*__first);
      std::__adjust_heap(__first, _Distance(0), _Distance(__last - __first),
                         std_move(__value), __comp);
    }

  /**
   *  @brief  Pop an element off a heap using comparison functor.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @param  comp   Comparison functor to use.
   *  @ingroup heap
   *
   *  This operation pops the top of the heap.  The elements first and last-1
   *  are swapped and [first,last-1) is made into a heap.  Comparisons are
   *  made using comp.
  */
  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter> && CopyConstructible<_Compare>
    inline void
    pop_heap(_Iter __first,
             _Iter __last, _Compare __comp)
    {
      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap_pred(__first, __last, __comp);

      --__last;
      std::__pop_heap(__first, __last, __last, __comp);
    }

  /**
   *  @brief  Construct a heap over a range.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @ingroup heap
   *
   *  This operation makes the elements in [first,last) into a heap.
  */
  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    void
    make_heap(_Iter __first, _Iter __last)
    {
      typedef _Iter::value_type _ValueType;
      typedef _Iter::difference_type _DistanceType;

      __glibcxx_requires_valid_range(__first, __last);

      if (__last - __first < 2)
        return;

      const _DistanceType __len = __last - __first;
      _DistanceType __parent = (__len - 2) / 2;
      while (true)
        {
          _ValueType __value(std_move(*(__first + __parent)));
          std::__adjust_heap(__first, __parent, __len, std_move(__value));
          if (__parent == 0)
            return;
          __parent--;
        }
    }

  /**
   *  @brief  Construct a heap over a range using comparison functor.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @param  comp   Comparison functor to use.
   *  @ingroup heap
   *
   *  This operation makes the elements in [first,last) into a heap.
   *  Comparisons are made using comp.
  */
  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter> && CopyConstructible<_Compare>
    void
    make_heap(_Iter __first, _Iter __last,
              _Compare __comp)
    {
      typedef _GLIBCXX_ITERATOR_TRAITS_NEST(_Iter, value_type)
          _ValueType;
      typedef _GLIBCXX_ITERATOR_TRAITS_NEST(_Iter, difference_type)
          _DistanceType;

      __glibcxx_requires_valid_range(__first, __last);

      if (__last - __first < 2)
        return;

      const _DistanceType __len = __last - __first;
      _DistanceType __parent = (__len - 2) / 2;
      while (true)
        {
          std::__adjust_heap(__first, __parent, __len,
                             _ValueType(*(__first + __parent)), __comp);
          if (__parent == 0)
            return;
          __parent--;
        }
    }

  /**
   *  @brief  Sort a heap.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @ingroup heap
   *
   *  This operation sorts the valid heap in the range [first,last).
  */
  template<RandomAccessIterator _Iter>
    requires ShuffleIterator<_Iter>
          && LessThanComparable<_Iter::value_type>
    void
    sort_heap(_Iter __first, _Iter __last)
    {
      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap(__first, __last);

      while (__last - __first > 1)
	std::pop_heap(__first, _Iter(__last--));
    }

  /**
   *  @brief  Sort a heap using comparison functor.
   *  @param  first  Start of heap.
   *  @param  last   End of heap.
   *  @param  comp   Comparison functor to use.
   *  @ingroup heap
   *
   *  This operation sorts the valid heap in the range [first,last).
   *  Comparisons are made using comp.
  */
  template<RandomAccessIterator _Iter, 
           StrictWeakOrder<auto, _Iter::value_type> _Compare>
    requires ShuffleIterator<_Iter> && CopyConstructible<_Compare>
    void
    sort_heap(_Iter __first, _Iter __last,
              _Compare __comp)
    {
      __glibcxx_requires_valid_range(__first, __last);
      __glibcxx_requires_heap_pred(__first, __last, __comp);

      while (__last - __first > 1)
        std::pop_heap(__first, _Iter(__last--), __comp);
    }

_GLIBCXX_END_NAMESPACE

#endif /* _STL_HEAP_H */
