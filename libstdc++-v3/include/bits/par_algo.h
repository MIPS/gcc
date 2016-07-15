// <algorithm> parallel algorithms -*- C++ -*-

// Copyright (C) 2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/par_algo.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{algorithm}
 */

#ifndef _GLIBCXX_PAR_ALGO_H
#define _GLIBCXX_PAR_ALGO_H 1

#pragma GCC system_header

#include <execution>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @defgroup parallel_algorithms Parallel Algorithms
   * @ingroup algorithms
   *
   * For C++17 the Standard Library adds parallel versions of most algorithms,
   * in the form of overloads taking an additional argument specifying an
   * Execution Policy.
   */


  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    find_if(_ExecutionPolicy&& __exec,
	    _InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
      // sequential
      return std::find_if(__first, __last, __pred);
    }


  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    all_of(_ExecutionPolicy&& __exec,
	   _InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
      return std::find_if(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last, [&](auto&& __arg) {
	  return !__pred(__arg);
      }) == __last;
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    any_of(_ExecutionPolicy&& __exec,
	   _InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
      return std::find_if(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last, std::move(__pred)) != __last;
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    none_of(_ExecutionPolicy&& __exec,
	    _InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
      return std::find_if(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last, std::move(__pred)) == __last;
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Function>
    inline __if_execpol<_ExecutionPolicy, void>
    for_each(_ExecutionPolicy&& __exec,
	     _InputIterator __first, _InputIterator __last, _Function __fn)
    {
      std::for_each(__first, __last, std::move(__fn));
    }

  template<typename _ExecutionPolicy, typename _InputIterator, typename _Size,
	   typename _Function>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    for_each_n(_ExecutionPolicy&& __exec, _InputIterator __first, _Size __num,
	       _Function __fn)
    {
      // sequential
      return std::for_each_n(__first, __num, std::move(__fn));
    }

  template<typename _ExecutionPolicy, typename _InputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    find(_ExecutionPolicy&& __exec,
	 _InputIterator __first, _InputIterator __last, const _Tp& __value)
    {
      return std::find_if(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last,
			  [&](auto&& __arg) { return __arg == __value; });
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    find_if_not(_ExecutionPolicy&& __exec,
		_InputIterator __first, _InputIterator __last,
		_Predicate __pred)
    {
      return std::find_if(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last,
			  [&](auto&& __arg) { return !__pred(__arg); });
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator1,
	   typename _ForwardIterator2, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator1>
    find_end(_ExecutionPolicy&& __exec,
	     _ForwardIterator1 __first1, _ForwardIterator1 __last1,
	     _ForwardIterator2 __first2, _ForwardIterator2 __last2,
	     _BinaryPredicate __pred)
    {
      // sequential
      return std::find_end(__first1, __last1, __first2, __last2,
			   std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator1,
	   typename _ForwardIterator2>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator1>
    find_end(_ExecutionPolicy&& __exec,
	     _ForwardIterator1 __first1, _ForwardIterator1 __last1,
	     _ForwardIterator2 __first2, _ForwardIterator2 __last2)
    {
      return std::find_end(std::forward<_ExecutionPolicy>(__exec),
			   __first1, __last1, __first2, __last2,
			   std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _ForwardIterator, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    find_first_of(_ExecutionPolicy&& __exec,
		  _InputIterator __first1, _InputIterator __last1,
		  _ForwardIterator __first2, _ForwardIterator __last2,
		  _BinaryPredicate __pred)
    {
      // sequential
      return std::find_first_of(__first1, __last1, __first2, __last2,
				std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _InputIterator>
    find_first_of(_ExecutionPolicy&& __exec,
		  _InputIterator __first1, _InputIterator __last1,
		  _ForwardIterator __first2, _ForwardIterator __last2)
    {
      return std::find_first_of(std::forward<_ExecutionPolicy>(__exec),
				__first1, __last1, __first2, __last2,
				std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    adjacent_find(_ExecutionPolicy&& __exec,
		  _ForwardIterator __first, _ForwardIterator __last,
		  _BinaryPredicate __pred)
    {
      // sequential
      return std::adjacent_find(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    adjacent_find(_ExecutionPolicy&& __exec,
		  _ForwardIterator __first, _ForwardIterator __last)
    {
      return std::adjacent_find(std::forward<_ExecutionPolicy>(__exec),
				__first, __last, std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy,
		 typename iterator_traits<_InputIterator>::difference_type>
    count_if(_ExecutionPolicy&& __exec,
	     _InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
      // sequential
      return std::count_if(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy,
		 typename iterator_traits<_InputIterator>::difference_type>
    count(_ExecutionPolicy&& __exec,
    _InputIterator __first, _InputIterator __last, const _Tp& __value)
    {
      return std::count_if(std::forward<_ExecutionPolicy>(__exec),
			   __first, __last,
			   [&](auto&& __arg) { return __arg == __value; });
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _BinaryPredicate>
    inline
    __if_execpol<_ExecutionPolicy, pair<_InputIterator1, _InputIterator2>>
    mismatch(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _BinaryPredicate __pred)
    {
      // sequential
      return std::mismatch(__first1, __last1, __first2,
			   std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline
    __if_execpol<_ExecutionPolicy, pair<_InputIterator1, _InputIterator2>>
    mismatch(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2)
    {
      return std::mismatch(std::forward<_ExecutionPolicy>(__exec),
			   __first1, __last1, __first2,
			   std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _BinaryPredicate>
    inline
    __if_execpol<_ExecutionPolicy, pair<_InputIterator1, _InputIterator2>>
    mismatch(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2,
	     _BinaryPredicate __pred)
    {
      // sequential
      return std::mismatch(__first1, __last1, __first2, __last2,
			   std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline
    __if_execpol<_ExecutionPolicy, pair<_InputIterator1, _InputIterator2>>
    mismatch(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2)
    {
      return std::mismatch(std::forward<_ExecutionPolicy>(__exec),
			   __first1, __last1, __first2, __last2,
			   std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    equal(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _BinaryPredicate __pred)
    {
      // sequential
      return std::equal(__first1, __last1, __first2, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline __if_execpol<_ExecutionPolicy, bool>
    equal(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2)
    {
      return std::equal(std::forward<_ExecutionPolicy>(__exec),
			__first1, __last1, __first2, __last2,
			std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    equal(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2,
	  _BinaryPredicate __pred)
    {
      // sequential
      return std::equal(__first1, __last1, __first2, __last2,
			std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline __if_execpol<_ExecutionPolicy, bool>
    equal(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2)
    {
      return std::equal(std::forward<_ExecutionPolicy>(__exec),
			__first1, __last1, __first2, __last2,
			std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator1,
	   typename _ForwardIterator2, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator1>
    search( _ExecutionPolicy&& __exec,
	  _ForwardIterator1 __first1, _ForwardIterator1 __last1,
	  _ForwardIterator2 __first2, _ForwardIterator2 __last2,
	  _BinaryPredicate __pred)
    {
      // sequential
      return std::search(__first1, __last1, __first2, __last2,
			 std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator1,
	   typename _ForwardIterator2>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator1>
    search( _ExecutionPolicy&& __exec,
	   _ForwardIterator1 __first1, _ForwardIterator1 __last1,
	   _ForwardIterator2 __first2, _ForwardIterator2 __last2)
    {
      return std::search(std::forward<_ExecutionPolicy>(__exec),
			 __first1, __last1, __first2, __last2,
			 std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Size, typename _Tp, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    search_n(_ExecutionPolicy&& __exec,
	  _ForwardIterator __first, _ForwardIterator __last,
	  _Size __count, const _Tp& __value,
	  _BinaryPredicate __pred)
    {
      // sequential
      return std::search_n(__first, __last, __value, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Size, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    search_n(_ExecutionPolicy&& __exec,
	  _ForwardIterator __first, _ForwardIterator __last,
	  _Size __count, const _Tp& __value)
    {
      return std::search_n(std::forward<_ExecutionPolicy>(__exec),
			   __first1, __last1, __value, std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    copy(_ExecutionPolicy&& __exec,
	  _InputIterator __first, _InputIterator __last,
	  _OutputIterator __result)
    {
      // sequential
      return std::copy(__first, __last, __result);
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Size,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    copy_n(_ExecutionPolicy&& __exec,
	  _InputIterator __first, _Size __num,
	  _OutputIterator __result)
    {
      // sequential
      return std::copy_n(__first, __num, __result);
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    copy_if(_ExecutionPolicy&& __exec,
	  _InputIterator __first, _InputIterator __last,
	  _OutputIterator __result, _Predicate __pred)
    {
      // sequential
      return std::copy_if(__first, __last, __result, std::move(__pred));
    }


  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    move(_ExecutionPolicy&& __exec,
	  _InputIterator __first, _InputIterator __last,
	  _OutputIterator __result)
    {
      // sequential
      return std::move(__first, __last, __result);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator1,
	   typename _ForwardIterator2>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator2>
    swap_ranges(_ExecutionPolicy&& __exec,
		_ForwardIterator1 __first1, _ForwardIterator1 __last1,
		_ForwardIterator2 __first2)
    {
      // sequential
      return std::swap_ranges(__first1, __last1, __first2);
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator,
	   typename UnaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    transform(_ExecutionPolicy&& __exec,
	      _InputIterator __first, _InputIterator __last,
	      _OutputIterator __result, UnaryOperation __op)
    {
      // sequential
      return std::transform(__first, __last, __result, std::move(__op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2,
	   typename _OutputIterator, typename BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    transform(_ExecutionPolicy&& __exec,
	      _InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _OutputIterator __result,
	      BinaryOperation __binary_op)
    {
      // sequential
      return std::transform(__first1, __last1, __first2, __last2, __result,
			    std::move(__binary_op));
    }


  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Predicate, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, void>
    replace_if(_ExecutionPolicy&& __exec,
	  _ForwardIterator __first, _ForwardIterator __last,
	  _Predicate __pred, const _Tp& __new_value)
    {
      std::replace_if(__first, __last, std::move(__pred), __new_value);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, void>
    replace(_ExecutionPolicy&& __exec,
	    _ForwardIterator __first, _ForwardIterator __last,
	    const _Tp& __old_value, const _Tp& __new_value)
    {
      std::replace(std::forward<_ExecutionPolicy>(__exec),
		   __first, __last,
		   [&](auto&& __val) { return __val == __old_value; },
		   __new_value);
    }



  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator,
	   typename _Predicate, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    replace_copy_if(_ExecutionPolicy&& __exec,
		    _InputIterator __first, _InputIterator __last,
		    _OutputIterator __result,
		    _Predicate __pred, const _Tp& __new_value)
    {
      // sequential
      return std::replace_copy_if(__first, __last, __result,
				  std::move(__pred), __new_value);
    }


  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    replace_copy(_ExecutionPolicy&& __exec,
		 _InputIterator __first, _InputIterator __last,
		 _OutputIterator __result,
		 const _Tp& __old_value, const _Tp& __new_value)
    {
      return std::replace_copy(std::forward<_ExecutionPolicy>(__exec),
			       __first, __last, __result,
			       [&](auto&& __val) {
				   return __val == __old_value;
			       }, __new_value);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Tp>
    inline __if_execpol<_ExecutionPolicy, void>
    fill(_ExecutionPolicy&& __exec,
	 _ForwardIterator __first, _ForwardIterator __last,
	 const _Tp& __value)
    {
      std::fill(__first, __last, __value);
    }

  template<typename _ExecutionPolicy, typename _OutputIterator,
	   typename _Size, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    fill_n(_ExecutionPolicy&& __exec,
	   _OutputIterator __first, _Size __num, const _Tp& __value)
    {
      // sequential
      return std::fill(__first, __num, __value);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename Generator>
    inline __if_execpol<_ExecutionPolicy, void>
    generate(_ExecutionPolicy&& __exec,
	     _ForwardIterator __first, _ForwardIterator __last,
	     Generator __gen)
    {
      std::generate(__first, __last, std::move(__gen));
    }

  template<typename _ExecutionPolicy, typename _OutputIterator,
	   typename _Size, typename Generator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    generate_n(_ExecutionPolicy&& __exec,
	       _OutputIterator __first, _Size num, Generator __gen)
    {
      // sequential
      return std::generate_n(__first, __num, std::move(__gen));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    remove_if(_ExecutionPolicy&& __exec,
	      _ForwardIterator __first, _ForwardIterator __last,
	      _Predicate __pred)
    {
      // sequential
      return std::remove_if(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    remove(_ExecutionPolicy&& __exec,
	   _ForwardIterator __first, _ForwardIterator __last,
	   const _Tp& __value)
    {
      return std::remove_if(std::forward<_ExecutionPolicy>(__exec),
			    __first, __last,
			    [&](auto&& __val) { return __val == __value; });
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    remove_copy_if(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result, _Predicate __pred)
    {
      // sequential
      return std::remove_copy_if(__first, __last, __result, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    remove_copy(_ExecutionPolicy&& __exec,
		_InputIterator __first, _InputIterator __last,
		_OutputIterator __result, const _Tp& __value)
    {
      return std::remove_copy_if(std::forward<_ExecutionPolicy>(__exec),
				 __first, __last, __result,
				 [&](auto&& __val) {
				     return __val == __value;
				 });
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    unique(_ExecutionPolicy&& __exec,
	   _ForwardIterator __first, _ForwardIterator __last,
	   _BinaryPredicate __pred)
    {
      // sequential
      return std::unique(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    unique(_ExecutionPolicy&& __exec,
	   _ForwardIterator __first, _ForwardIterator __last)
    {
      return std::unique(std::forward<_ExecutionPolicy>(__exec),
			 __first, __last, std::equal_to<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _BinaryPredicate>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    unique_copy(_ExecutionPolicy&& __exec,
		_InputIterator __first, _InputIterator __last,
		_OutputIterator __result, _BinaryPredicate __pred)
    {
      // sequential
      return std::unique_copy(__first, __last, __result, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    unique_copy(_ExecutionPolicy&& __exec,
		_InputIterator __first, _InputIterator __last,
		_OutputIterator __result)
    {
      return std::unique_copy(std::forward<_ExecutionPolicy>(__exec),
			 __first, __last, __result, std::equal_to<>{});
    }


  template<typename _ExecutionPolicy, typename _BidirectionalIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    reverse(_ExecutionPolicy&& __exec,
	    _BidirectionalIterator __first, _BidirectionalIterator __last)
    {
      std::reverse(__first, __last);
    }

  template<typename _ExecutionPolicy, typename _BidirectionalIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    reverse_copy(_ExecutionPolicy&& __exec,
		 _BidirectionalIterator __first,
		 _BidirectionalIterator __last,
		 _OutputIterator __result)
    {
      // sequential
      return std::reverse_copy(__first, __last, __result);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    rotate(_ExecutionPolicy&& __exec,
	   _ForwardIterator __first,
	   _ForwardIterator __middle,
	   _ForwardIterator __last)
    {
      // sequential
      return std::rotate(__first, __middle, __last);
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    rotate_copy(_ExecutionPolicy&& __exec,
		_ForwardIterator __first, _ForwardIterator __middle,
		_ForwardIterator __last, _OutputIterator __result)
    {
      // sequential
      return std::rotate_copy(__first, __middle, __last, __result);
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, bool>
    is_partitioned(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _Predicate __pred)
    {
      // sequential
      return std::is_partitioned(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Predicate>
    _ForwardIterator
    partition(_ExecutionPolicy&& __exec,
	      _ForwardIterator __first, _ForwardIterator __last,
	      _Predicate __pred)
    {
      // sequential
      return std::partition(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _BidirectionalIterator,
	   typename _Predicate>
    inline __if_execpol<_ExecutionPolicy, _BidirectionalIterator>
    stable_partition(_ExecutionPolicy&& __exec,
	_BidirectionalIterator __first,
	_BidirectionalIterator __last,
	_Predicate __pred)
    {
      // sequential
      return std::stable_partition(__first, __last, std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator1, typename _OutputIterator2,
	   typename _Predicate>
    inline
    __if_execpol<_ExecutionPolicy, pair<_OutputIterator1, _OutputIterator2>>
    partition_copy(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator1 __out_true, _OutputIterator2 __out_false,
		   _Predicate __pred)
    {
      // sequential
      return std::partition_copy(__first, __last, __out_true, __out_false,
				 std::move(__pred));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, void>
    sort(_ExecutionPolicy&& __exec,
	 _RandomAccessIterator __first, _RandomAccessIterator __last,
	 _Compare __comp)
    {
      // sequential
      return std::sort(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    sort(_ExecutionPolicy&& __exec,
	 _RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::sort(std::forward<_ExecutionPolicy>(__exec),
		       __first, __last, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, void>
    stable_sort(_ExecutionPolicy&& __exec,
		_RandomAccessIterator __first, _RandomAccessIterator __last,
		_Compare __comp)
    {
      // sequential
      return std::stable_sort(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    stable_sort(_ExecutionPolicy&& __exec,
		_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::stable_sort(std::forward<_ExecutionPolicy>(__exec),
			      __first, __last, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, void>
    partial_sort(_ExecutionPolicy&& __exec,
		 _RandomAccessIterator __first, _RandomAccessIterator __middle,
		 _RandomAccessIterator __last, _Compare __comp)
    {
      // sequential
      return std::partial_sort(__first, __middle, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    partial_sort(_ExecutionPolicy&& __exec,
		 _RandomAccessIterator __first, _RandomAccessIterator __middle,
		 _RandomAccessIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::partial_sort(std::forward<_ExecutionPolicy>(__exec),
			       __first, __middle, __last,
			       std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _RandomAccessIterator, typename _Compare>
    inline __if_execpol<_ExecutionPolicy, RandomAccessIterator>
    partial_sort_copy(_ExecutionPolicy&& __exec,
		      _InputIterator __first, _InputIterator __last,
		      _RandomAccessIterator __result_first,
		      _RandomAccessIterator __result_last,
		      _Compare __comp)
    {
      // sequential
      return std::partial_sort_copy(__first, __last,
				    _result_first, __result_last,
				    std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, _RandomAccessIterator>
    partial_sort_copy(_ExecutionPolicy&& __exec,
		      _InputIterator __first, _InputIterator __last,
		      _RandomAccessIterator __result_first,
		      _RandomAccessIterator __result_last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::partial_sort_copy(std::forward<_ExecutionPolicy>(__exec),
				    __first, , __last,
				    __result_first, __result_last,
				    std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, bool>
    is_sorted(_ExecutionPolicy&& __exec,
	      _ForwardIterator __first, _ForwardIterator __last,
	      _Compare __comp)
    {
      // sequential
      return std::is_sorted(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, bool>
    is_sorted(_ExecutionPolicy&& __exec,
	      _ForwardIterator __first, _ForwardIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::is_sorted(std::forward<_ExecutionPolicy>(__exec),
			    __first, __last, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    is_sorted_until(_ExecutionPolicy&& __exec,
		    _ForwardIterator __first, _ForwardIterator __last,
		    _Compare __comp)
    {
      // sequential
      return std::is_sorted_until(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    is_sorted_until(_ExecutionPolicy&& __exec,
		    _ForwardIterator __first, _ForwardIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::is_sorted_until(std::forward<_ExecutionPolicy>(__exec),
				  __first, __last, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, void>
    nth_element(_ExecutionPolicy&& __exec,
		_RandomAccessIterator __first, _RandomAccessIterator __nth,
		_RandomAccessIterator __last, _Compare __comp)
    {
      // sequential
      return std::nth_element(__first, __nth, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    nth_element(_ExecutionPolicy&& __exec,
		_RandomAccessIterator __first, _RandomAccessIterator __nth,
		_RandomAccessIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::nth_element(std::forward<_ExecutionPolicy>(__exec),
			      __first, __nth, __last, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    merge(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2,
	  _OutputIterator __result, _Compare __comp)
    {
      // sequential
      return std::merge(__first1, __last1, __first2, __last2, __result,
			std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    merge(_ExecutionPolicy&& __exec,
	  _InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2,
	  _OutputIterator __result)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::merge(std::forward<_ExecutionPolicy>(__exec),
			__first1, __last1, __first2, __last2,
			__result, std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _BidirectionalIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, void>
    inplace_merge(_ExecutionPolicy&& __exec,
		  _BidirectionalIterator __first,
		  _BidirectionalIterator __middle,
		  _BidirectionalIterator __last, _Compare __comp)
    {
      // sequential
      return std::inplace_merge(__first, __middle, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _BidirectionalIterator>
    inline __if_execpol<_ExecutionPolicy, void>
    inplace_merge(_ExecutionPolicy&& __exec,
		  _BidirectionalIterator __first,
		  _BidirectionalIterator __middle,
		  _BidirectionalIterator __last)
    {
      using _ValueType
	= typename iterator_traits<_RandomAccessIterator>::value_type;
      return std::inplace_merge(std::forward<_ExecutionPolicy>(__exec),
				__first, __middle, __last,
				std::less<_ValueType>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _Compare>
    inline __if_execpol<_ExecutionPolicy, bool>
    includes(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
    {
      // sequential
      return std::includes(__first1, __last1, __first2, __last2,
			   std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline __if_execpol<_ExecutionPolicy, bool>
    includes(_ExecutionPolicy&& __exec,
	     _InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2)
    {
      return std::includes(std::forward<_ExecutionPolicy>(__exec),
			   __first1, __last1, __first2, __last2,
			   std::less<>{});
    }


  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_union(_ExecutionPolicy&& __exec,
	      _InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _InputIterator2 __last2,
	      _OutputIterator __result, _Compare __comp)
    {
      // sequential
      return std::set_union(__first1, __last1, __first2, __last2,
			    __result, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_union(_ExecutionPolicy&& __exec,
	      _InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _InputIterator2 __last2,
	      _OutputIterator __result)
    {
      return std::set_union(std::forward<_ExecutionPolicy>(__exec),
			    __first1, __last1, __first2, __last2,
			    __result, std::less<>{});
    }


  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_intersection(_ExecutionPolicy&& __exec,
		     _InputIterator1 __first1, _InputIterator1 __last1,
		     _InputIterator2 __first2, _InputIterator2 __last2,
		     _OutputIterator __result, _Compare __comp)
    {
      // sequential
      return std::set_intersection(__first1, __last1, __first2, __last2,
				   __result, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_intersection(_ExecutionPolicy&& __exec,
		     _InputIterator1 __first1, _InputIterator1 __last1,
		     _InputIterator2 __first2, _InputIterator2 __last2,
		     _OutputIterator __result)
    {
      return std::set_intersection(std::forward<_ExecutionPolicy>(__exec),
				   __first1, __last1, __first2, __last2,
				   __result, std::less<>{});
    }


  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_difference(_ExecutionPolicy&& __exec,
		   _InputIterator1 __first1, _InputIterator1 __last1,
		   _InputIterator2 __first2, _InputIterator2 __last2,
		   _OutputIterator __result, _Compare __comp)
    {
      // sequential
      return std::set_difference(__first1, __last1, __first2, __last2,
				 __result, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_difference(_ExecutionPolicy&& __exec,
		   _InputIterator1 __first1, _InputIterator1 __last1,
		   _InputIterator2 __first2, _InputIterator2 __last2,
		   _OutputIterator __result)
    {
      return std::set_difference(std::forward<_ExecutionPolicy>(__exec),
				 __first1, __last1, __first2, __last2,
				 __result, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_symmetric_difference(_ExecutionPolicy&& __exec,
			     _InputIterator1 __first1, _InputIterator1 __last1,
			     _InputIterator2 __first2, _InputIterator2 __last2,
			     _OutputIterator __result, _Compare __comp)
    {
      // sequential
      return std::set_symmetric_difference(__first1, __last1,
					   __first2, __last2,
					   __result, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    set_symmetric_difference(_ExecutionPolicy&& __exec,
			     _InputIterator1 __first1, _InputIterator1 __last1,
			     _InputIterator2 __first2, _InputIterator2 __last2,
			     _OutputIterator __result)
    {
      return std::set_symmetric_difference(
	  std::forward<_ExecutionPolicy>(__exec),
	  __first1, __last1, __first2, __last2,
	  __result, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, bool>
    is_heap(_ExecutionPolicy&& __exec,
	    _RandomAccessIterator __first, _RandomAccessIterator __last,
	    _Compare __comp)
    {
      // sequential
      return std::is_heap(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, bool>
    is_heap(_ExecutionPolicy&& __exec,
	_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      return std::is_heap(std::forward<_ExecutionPolicy>(__exec),
			  __first, __last, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _RandomAccessIterator>
    is_heap_until(_ExecutionPolicy&& __exec,
		  _RandomAccessIterator __first, _RandomAccessIterator __last,
		  _Compare __comp)
    {
      // sequential
      return std::is_heap_until(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _RandomAccessIterator>
    inline __if_execpol<_ExecutionPolicy, _RandomAccessIterator>
    is_heap_until(_ExecutionPolicy&& __exec,
		  _RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      return std::is_heap_until(std::forward<_ExecutionPolicy>(__exec),
				__first, __last, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    min_element(_ExecutionPolicy&& __exec,
		_ForwardIterator __first, _ForwardIterator __last,
		_Compare __comp)
    {
      // sequential
      return std::min_element(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    min_element(_ExecutionPolicy&& __exec,
		_ForwardIterator __first, _ForwardIterator __last)
    {
      return std::min_element(std::forward<_ExecutionPolicy>(__exec),
			      __first, __last, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Compare>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    max_element(_ExecutionPolicy&& __exec,
		_ForwardIterator __first, _ForwardIterator __last,
		_Compare __comp)
    {
      // sequential
      return std::max_element(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline __if_execpol<_ExecutionPolicy, _ForwardIterator>
    max_element(_ExecutionPolicy&& __exec,
		_ForwardIterator __first, _ForwardIterator __last)
    {
      return std::max_element(std::forward<_ExecutionPolicy>(__exec),
			      __first, __last, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator,
	   typename _Compare>
    inline
    __if_execpol<_ExecutionPolicy, pair<_ForwardIterator, _ForwardIterator>>
    minmax_element(_ExecutionPolicy&& __exec,
		   _ForwardIterator __first, _ForwardIterator __last,
		   _Compare __comp)
    {
      // sequential
      return std::minmax_element(__first, __last, std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _ForwardIterator>
    inline
    __if_execpol<_ExecutionPolicy, pair<_ForwardIterator, _ForwardIterator>>
    minmax_element(_ExecutionPolicy&& __exec,
		   _ForwardIterator __first, _ForwardIterator __last)
    {
      return std::minmax_element(std::forward<_ExecutionPolicy>(__exec),
				 __first, __last, std::less<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _Compare>
    inline __if_execpol<_ExecutionPolicy, bool>
    lexicographical_compare(_ExecutionPolicy&& __exec,
			    _InputIterator1 __first1, _InputIterator1 __last1,
			    _InputIterator2 __first2, _InputIterator2 __last2,
			    _Compare __comp)
    {
      // sequential
      return std::lexicographical_compare(__first1, __last1,
					  __first2, __last2,
					  std::move(__comp));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2>
    inline __if_execpol<_ExecutionPolicy, bool>
    lexicographical_compare(_ExecutionPolicy&& __exec,
			    _InputIterator1 __first1, _InputIterator1 __last1,
			    _InputIterator2 __first2, _InputIterator2 __last2)
    {
      return std::lexicographical_compare(
	  std::forward<_ExecutionPolicy>(__exec),
	  __first1, __last1, __first2, __last2, std::less<>{});
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif
