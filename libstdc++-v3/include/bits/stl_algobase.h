// Bits and pieces used in algorithms -*- C++ -*-

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
 *
 * Copyright (c) 1996-1998
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

/** @file stl_algobase.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _ALGOBASE_H
#define _ALGOBASE_H 1

#include <bits/c++config.h>
#include <cstring>
#include <cwchar>
#include <cstddef>
#include <bits/functexcept.h>
#include <bits/stl_pair.h>
#include <bits/cpp_type_traits.h>
#include <ext/type_traits.h>
#include <limits>
#include <bits/stl_iterator_base_types.h>
#include <bits/stl_iterator_base_funcs.h>
#include <bits/stl_iterator.h>
#include <debug/debug.h>
#include <bits/concepts.h>

_GLIBCXX_BEGIN_NAMESPACE(std)

  /**
   *  @brief Swaps two values.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @return   Nothing.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  any type which has a copy constructor and an assignment operator.
  */
  template<typename _Tp>
   _GLIBCXX_WHERE(CopyAssignable<_Tp>, CopyConstructible<_Tp>)
   inline void
    swap(_Tp& __a, _Tp& __b)
    {
      _Tp __tmp = __a;
      __a = __b;
      __b = __tmp;
    }

  /**
   *  @brief Swaps the contents of two iterators.
   *  @param  a  An iterator.
   *  @param  b  Another iterator.
   *  @return   Nothing.
   *
   *  This function swaps the values pointed to by two iterators, not the
   *  iterators themselves.
  */
  template<ForwardIterator _Iter1, ForwardIterator _Iter2>
    requires HasSwap<_Iter1::reference, _Iter2::reference>
    inline void
    iter_swap(_Iter1 __a, _Iter2 __b)
    {
      swap(*__a, *__b);
    }

  #undef min
  #undef max

 /**
   *  @brief Swap the elements of two sequences.
   *  @param  first1  A forward iterator.
   *  @param  last1   A forward iterator.
   *  @param  first2  A forward iterator.
   *  @return   An iterator equal to @p first2+(last1-first1).
   *
   *  Swaps each element in the range @p [first1,last1) with the
   *  corresponding element in the range @p [first2,(last1-first1)).
   *  The ranges must not overlap.
  */
  template<ForwardIterator _Iter1, ForwardIterator _Iter2>
    requires HasSwap<_Iter1::reference, _Iter2::reference>
    _Iter2
    swap_ranges(_Iter1 __first1, _Iter1 __last1,
                _Iter2 __first2)
    {
      __glibcxx_requires_valid_range(__first1, __last1);

      for ( ; __first1 != __last1; ++__first1, ++__first2)
        swap(*__first1, *__first2);
      return __first2;
    }

  /**
   *  @brief This does what you think it does.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @return   The lesser of the parameters.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  temporary expressions, since they are only evaluated once, unlike a
   *  preprocessor macro.
  */
   template<_GLIBCXX_REQ_PARM(LessThanComparable, _Tp)>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b)
    {
      //return __b < __a ? __b : __a;
      if (__b < __a)
        return __b;
      return __a;
    }

  /**
   *  @brief This does what you think it does.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @return   The greater of the parameters.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  temporary expressions, since they are only evaluated once, unlike a
   *  preprocessor macro.
  */
  template<_GLIBCXX_REQ_PARM(LessThanComparable, _Tp)>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
    {
      //return  __a < __b ? __b : __a;
      if (__a < __b)
        return __b;
      return __a;
    }

  /**
   *  @brief This does what you think it does.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @param  comp  A @link s20_3_3_comparisons comparison functor@endlink.
   *  @return   The lesser of the parameters.
   *
   *  This will work on temporary expressions, since they are only evaluated
   *  once, unlike a preprocessor macro.
  */
  template<typename _Tp, _GLIBCXX_PARM_REQ(_Compare, BinaryPredicate<_Tp, _Tp>)>
   inline const _Tp&
    min(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {
      //return __comp(__b, __a) ? __b : __a;
      if (__comp(__b, __a))
        return __b;
      return __a;
    }

  /**
   *  @brief This does what you think it does.
   *  @param  a  A thing of arbitrary type.
   *  @param  b  Another thing of arbitrary type.
   *  @param  comp  A @link s20_3_3_comparisons comparison functor@endlink.
   *  @return   The greater of the parameters.
   *
   *  This will work on temporary expressions, since they are only evaluated
   *  once, unlike a preprocessor macro.
  */
  template<typename _Tp, _GLIBCXX_PARM_REQ(_Compare, BinaryPredicate<_Tp, _Tp>)>
   inline const _Tp&
    max(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {
      //return __comp(__a, __b) ? __b : __a;
      if (__comp(__a, __b))
        return __b;
      return __a;
    }

  /**
   *  @brief Copies the range [first,last) into result.
   *  @param  first  An input iterator.
   *  @param  last   An input iterator.
   *  @param  result An output iterator.
   *  @return   result + (first - last)
   *
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).  Result may not be contained within
   *  [first,last); the copy_backward function should be used instead.
   *
   *  Note that the end of the output range is permitted to be contained
   *  within [first,last).
  */
  template<InputIterator _InIter, 
           OutputIterator<auto, _InIter::reference> _OutIter>
    inline _OutIter
    copy(_InIter __first, _InIter __last, _OutIter __result)
    {
      __glibcxx_requires_valid_range(__first, __last);
      for (; __first != __last; ++__result, ++__first)
        *__result = *__first;
      return __result;
    }

  // FIXME: Re-enable the random-access and memmove optimizations for
  // copy and copy_backward, plus the streambuf_iterator optimizations
  // for copy.

  /**
   *  @brief Copies the range [first,last) into result.
   *  @param  first  A bidirectional iterator.
   *  @param  last   A bidirectional iterator.
   *  @param  result A bidirectional iterator.
   *  @return   result - (first - last)
   *
   *  The function has the same effect as copy, but starts at the end of the
   *  range and works its way to the start, returning the start of the result.
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).
   *
   *  Result may not be in the range [first,last).  Use copy instead.  Note
   *  that the start of the output range may overlap [first,last).
  */
  template <BidirectionalIterator _BI1, BidirectionalIterator _BI2>
    requires OutputIterator<_BI2, _BI1::reference>
    inline _BI2
    copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
    {
      __glibcxx_requires_valid_range(__first, __last);
      while (__first != __last)
        *--__result = *--__last;
      return __result;
    }


  // FIXME: bring back the memset optimizations for fill

  /**
   *  @brief Fills the range [first,last) with copies of value.
   *  @param  first  A forward iterator.
   *  @param  last   A forward iterator.
   *  @param  value  A reference-to-const of arbitrary type.
   *  @return   Nothing.
   *
   *  This function fills a range with copies of the same value.  For char
   *  types filling contiguous areas of memory, this becomes an inline call
   *  to @c memset or @c wmemset.
  */
  template<ForwardIterator _Iter, typename _Tp>
    requires OutputIterator<_Iter, const _Tp&>
    inline void
    fill(_Iter __first, _Iter __last, const _Tp& __value)
    {
      __glibcxx_requires_valid_range(__first, __last);
      for (; __first != __last; ++__first)
        *__first = __value;
    }



  /**
   *  @brief Fills the range [first,first+n) with copies of value.
   *  @param  first  An output iterator.
   *  @param  n      The count of copies to perform.
   *  @param  value  A reference-to-const of arbitrary type.
   *  @return   The iterator at first+n.
   *
   *  This function fills a range with copies of the same value.  For char
   *  types filling contiguous areas of memory, this becomes an inline call
   *  to @c memset or @ wmemset.
  */
  template<typename _Iter, IntegralLike _Size, typename _Tp>
    requires OutputIterator<_Iter, const _Tp&>
    inline _Iter
    fill_n(_Iter __first, _Size __n, const _Tp& __value)
    {
      for (; __n > 0; --__n, ++__first)
        *__first = __value;
      return __first;
    }

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using @c == and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */
  template<InputIterator _Iter1, InputIterator _Iter2>
    requires HasEqualTo<_Iter1::value_type, _Iter2::value_type>
    pair<_Iter1, _Iter2>
    mismatch(_Iter1 __first1, _Iter1 __last1,
             _Iter2 __first2)
    {
      __glibcxx_requires_valid_range(__first1, __last1);

      while (__first1 != __last1 && *__first1 == *__first2)
        {
          ++__first1;
          ++__first2;
        }
      return pair<_Iter1, _Iter2>(__first1, __first2);
    }

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @param  binary_pred  A binary predicate @link s20_3_1_base functor@endlink.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */
  template<InputIterator _Iter1, InputIterator _Iter2,
           BinaryPredicate<auto, _Iter1::value_type, _Iter2::value_type> _Pred>
    requires CopyConstructible<_Pred>
    pair<_Iter1, _Iter2>
    mismatch(_Iter1 __first1, _Iter1 __last1,
             _Iter2 __first2, _Pred __binary_pred)
    {
      __glibcxx_requires_valid_range(__first1, __last1);

      while (__first1 != __last1 && __binary_pred(*__first1, *__first2))
        {
          ++__first1;
          ++__first2;
        }
      return pair<_Iter1, _Iter2>(__first1, __first2);
    }

  /**
   *  @brief Tests a range for element-wise equality.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @return   A boolean true or false.
   *
   *  This compares the elements of two ranges using @c == and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */
  template<InputIterator _Iter1, InputIterator _Iter2>
    requires HasEqualTo<_Iter1::value_type, _Iter2::value_type>
    inline bool
    equal(_Iter1 __first1, _Iter1 __last1,
          _Iter2 __first2)
    {
      __glibcxx_requires_valid_range(__first1, __last1);

      for (; __first1 != __last1; ++__first1, ++__first2)
        if (!(*__first1 == *__first2))
          return false;
      return true;
    }

  /**
   *  @brief Tests a range for element-wise equality.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @param  binary_pred  A binary predicate @link s20_3_1_base functor@endlink.
   *  @return   A boolean true or false.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */
  template<InputIterator _Iter1, InputIterator _Iter2,
           BinaryPredicate<auto, _Iter1::value_type, _Iter2::value_type> _Pred>
    requires CopyConstructible<_Pred>
    inline bool
    equal(_Iter1 __first1, _Iter1 __last1,
          _Iter2 __first2,
          _Pred __pred)
    {
      __glibcxx_requires_valid_range(__first1, __last1);

      for (; __first1 != __last1; ++__first1, ++__first2)
        if (!__pred(*__first1, *__first2))
          return false;
      return true;
    }

  /**
   *  @brief Performs "dictionary" comparison on ranges.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @param  last2   An input iterator.
   *  @return   A boolean true or false.
   *
   *  "Returns true if the sequence of elements defined by the range
   *  [first1,last1) is lexicographically less than the sequence of elements
   *  defined by the range [first2,last2).  Returns false otherwise."
   *  (Quoted from [25.3.8]/1.)  If the iterators are all character pointers,
   *  then this is an inline call to @c memcmp.
  */
  template<InputIterator _Iter1, InputIterator _Iter2>
    requires HasLess<_Iter1::value_type, _Iter2::value_type>
          && HasLess<_Iter2::value_type, _Iter1::value_type>
    bool
    lexicographical_compare(_Iter1 __first1, _Iter1 __last1,
                            _Iter2 __first2, _Iter2 __last2)
    {
      __glibcxx_requires_valid_range(__first1, __last1);
      __glibcxx_requires_valid_range(__first2, __last2);

      for (; __first1 != __last1 && __first2 != __last2;
           ++__first1, ++__first2)
        {
          if (*__first1 < *__first2)
            return true;
          if (*__first2 < *__first1)
            return false;
        }
      return __first1 == __last1 && __first2 != __last2;
    }

  /**
   *  @brief Performs "dictionary" comparison on ranges.
   *  @param  first1  An input iterator.
   *  @param  last1   An input iterator.
   *  @param  first2  An input iterator.
   *  @param  last2   An input iterator.
   *  @param  comp  A @link s20_3_3_comparisons comparison functor@endlink.
   *  @return   A boolean true or false.
   *
   *  The same as the four-parameter @c lexigraphical_compare, but uses the
   *  comp parameter instead of @c <.
  */
  template<InputIterator _Iter1, InputIterator _Iter2,
           CopyConstructible _Compare>
    requires BinaryPredicate<_Compare, _Iter1::value_type, _Iter2::value_type>
          && BinaryPredicate<_Compare, _Iter2::value_type, _Iter1::value_type>
    bool
    lexicographical_compare(_Iter1 __first1, _Iter1 __last1,
                            _Iter2 __first2, _Iter2 __last2,
                            _Compare __comp)
    {
      __glibcxx_requires_valid_range(__first1, __last1);
      __glibcxx_requires_valid_range(__first2, __last2);

      for (; __first1 != __last1 && __first2 != __last2;
           ++__first1, ++__first2)
        {
          if (__comp(*__first1, *__first2))
            return true;
          if (__comp(*__first2, *__first1))
            return false;
        }
      return __first1 == __last1 && __first2 != __last2;
    }

  inline bool
  lexicographical_compare(const unsigned char* __first1,
                          const unsigned char* __last1,
                          const unsigned char* __first2,
                          const unsigned char* __last2)
  {
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;
    const int __result = std::memcmp(__first1, __first2,
                                     std::min(__len1, __len2));
    return __result != 0 ? __result < 0 : __len1 < __len2;
  }

  inline bool
  lexicographical_compare(const char* __first1, const char* __last1,
                          const char* __first2, const char* __last2)
  {
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    if (std::numeric_limits<char>::is_signed)
      return std::lexicographical_compare((const signed char*) __first1,
					  (const signed char*) __last1,
					  (const signed char*) __first2,
					  (const signed char*) __last2);
    else
      return std::lexicographical_compare((const unsigned char*) __first1,
					  (const unsigned char*) __last1,
					  (const unsigned char*) __first2,
					  (const unsigned char*) __last2);
  }

_GLIBCXX_END_NAMESPACE

#endif
