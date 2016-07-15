// <numeric> parallel algorithms -*- C++ -*-

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

/** @file bits/par_numeric.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numeric}
 */

#ifndef _GLIBCXX_PAR_NUMERIC_H
#define _GLIBCXX_PAR_NUMERIC_H 1

#pragma GCC system_header

#include <execution>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // TODO inclusive_scan etc. and other new non-parallel algos.

  /**
   * @defgroup parallel_numerics Parallel Numeric Algorithms
   * @ingroup numerics
   *
   * For C++17 the Standard Library adds parallel versions of most algorithms,
   * in the form of overloads taking an additional argument specifying an
   * Execution Policy.
   */

  template<typename _ExecutionPolicy, typename _InputIterator, typename _Tp,
	   typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _Tp>
    reduce(_ExecutionPolicy&& __exec,
	   _InputIterator __first, _InputIterator __last, _Tp __init,
	   _BinaryOperation __binary_op)
    {
      // sequential
      return std::reduce(__first, __last, std::move(__init),
			 std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _Tp>
    reduce(_ExecutionPolicy&& __exec,
	   _InputIterator __first, _InputIterator __last, _Tp __init)
    {
      return std::reduce(std::forward<_ExecutionPolicy>(__exec),
			 __first, __last, std::move(__init),
			 std::plus<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator>
    inline
    __if_execpol<_ExecutionPolicy,
		 typename iterator_traits<_InputIterator>::value_type>
    reduce(_ExecutionPolicy&& __exec,
	   _InputIterator __first, _InputIterator __last)
    {
      return std::reduce(std::forward<_ExecutionPolicy>(__exec),
			 __first, __last, typename
			 iterator_traits<_InputIterator>::value_type{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _UnaryFunction, typename _Tp, typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _Tp>
    transform_reduce(_ExecutionPolicy&& __exec,
		     _InputIterator __first, _InputIterator __last,
		     _UnaryOperation __unary_op, _Tp __init,
		     _BinaryOperation __binary_op)
    {
      // sequential
      return std::transform_reduce(__first, __last, std::move(__unary_op),
				   std::move(__init), std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _Tp,
	   typename _BinaryOperation1, typename _BinaryOperation2>
    inline __if_execpol<_ExecutionPolicy, _Tp>
    inner_product(_ExecutionPolicy&& __exec,
		  _InputIterator1 __first1, _InputIterator1 __last1,
		  _InputIterator2 __first2, _Tp __init,
		  _BinaryOperation1 __binary_op1,
		  _BinaryOperation2 __binary_op2)
    {
      // sequential
      return std::inner_product(__first1, __last1, __first2,
				std::move(__init),
				std::move(__binary_op1),
				std::move(__binary_op2));
    }

  template<typename _ExecutionPolicy, typename _InputIterator1,
	   typename _InputIterator2, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _Tp>
    inner_product(_ExecutionPolicy&& __exec,
		  _InputIterator1 __first1, _InputIterator1 __last1,
		  _InputIterator2 __first2, _Tp __init)
    {
      return std::inner_product(std::forward<_ExecutionPolicy>(__exec),
			        __first1, __last1, __first2,
				std::move(__init),
				std::plus<>{},
				std::multiplies<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _Tp, typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    exclusive_scan(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result, _Tp __init,
		   _BinaryOperation __binary_op);

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    exclusive_scan(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result, _Tp __init)
    {
      return std::exclusive_scan(std::forward<_ExecutionPolicy>(__exec),
				 __first, __last, __result, __init,
				 std::plus<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _BinaryOperation, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    inclusive_scan(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result,
		   _BinaryOperation __binary_op, _Tp __init)
    {
      return std::inclusive_scan(__first, __last, __result,
				 std::move(__binary_op), std::move(__init));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    inclusive_scan(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result, _BinaryOperation __binary_op)
    {
      return std::inclusive_scan(__first, __last, __result,
				 std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    inclusive_scan(_ExecutionPolicy&& __exec,
		   _InputIterator __first, _InputIterator __last,
		   _OutputIterator __result)
    {
      return std::inclusive_scan(std::forward<_ExecutionPolicy>(__exec),
				 __first, __last, __result,
				 std::plus<>{});
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _UnaryOperation,
	   typename _Tp, typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    transform_exclusive_scan(_ExecutionPolicy&& __exec,
			     _InputIterator __first, _InputIterator __last,
			     _OutputIterator __result,
			     _UnaryOperation __unary_op,
			     _Tp __init, _BinaryOperation __binary_op)
    {
      return std::transform_exclusive_scan(__first, __last, __result,
					   std::move(__unary_op),
					   std::move(__init),
					   std::move(__binary_op));
    }


  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _UnaryOperation,
	   typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    transform_inclusive_scan(_ExecutionPolicy&& __exec,
			     _InputIterator __first, _InputIterator __last,
			     _OutputIterator __result,
			     _UnaryOperation __unary_op,
			     _BinaryOperation __binary_op)
    {
      return std::transform_inclusive_scan(__first, __last, __result,
					   std::move(__unary_op),
					   std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _UnaryOperation,
	   typename _BinaryOperation, typename _Tp>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    transform_inclusive_scan(_ExecutionPolicy&& __exec,
			     _InputIterator __first, _InputIterator __last,
			     _OutputIterator __result,
			     _UnaryOperation __unary_op,
			     _BinaryOperation __binary_op, _Tp __init)
    {
      return std::transform_inclusive_scan(__first, __last, __result,
					   std::move(__unary_op),
					   std::move(__init),
					   std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator, typename _BinaryOperation>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    adjacent_difference(_ExecutionPolicy&& __exec,
			_InputIterator __first, _InputIterator __last,
			_OutputIterator __result, _BinaryOperation __binary_op)
    {
      return std::adjacent_difference(__first, __last, __result,
				      std::move(__binary_op));
    }

  template<typename _ExecutionPolicy, typename _InputIterator,
	   typename _OutputIterator>
    inline __if_execpol<_ExecutionPolicy, _OutputIterator>
    adjacent_difference(_ExecutionPolicy&& __exec,
			_InputIterator __first, _InputIterator __last,
			_OutputIterator __result)
    {
      return std::adjacent_difference(std::forward<_ExecutionPolicy>(__exec),
				      __first, __last, __result,
				      std::minus<>{});
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif
