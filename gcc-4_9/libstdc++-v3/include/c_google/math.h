// -*- C++ -*- C forwarding header.

// Copyright (C) 1997-2014 Free Software Foundation, Inc.
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

/** @file math.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, rather than any of the @a *.h implementation files.
 *
 *  This is the C++ wrapper for the Standard C Library header @c math.h,
 *  and it merely replaces names defined as macros in C.
 */

//
// ISO C++ 14882: 26.5  C library
//

#pragma GCC system_header

#ifndef _GLIBCXX_MATH_H
#define _GLIBCXX_MATH_H 1

#include_next <math.h>

// Only do any customizations if we're compiling C++ code. We handle this case
// in the event this header is found by header search while compiling C code.
#if __cplusplus

// Define a macro to signal to Google3 that we have the math.h header fixes
// necessary for C++ and making this header modular w.r.t. <cmath>.
// TODO(chandlerc): Remove this once Google3 always has this header.
#define _GLIBCXX_GOOGLE_MATH_H_FIXED 1

// Since this is a "C" header, it may have been included in an extern "C"
// context. We explicitly put the internal libstdc++ headers back into an extern
// "C++" context in that event.
extern "C++" {
#include <bits/c++config.h>
#include <bits/cpp_type_traits.h>
#include <ext/type_traits.h>
}

// Get rid of those macros defined in C <math.h> in lieu of real functions.
#undef abs
#undef div
#undef acos
#undef asin
#undef atan
#undef atan2
#undef ceil
#undef cos
#undef cosh
#undef exp
#undef fabs
#undef floor
#undef fmod
#undef frexp
#undef ldexp
#undef log
#undef log10
#undef modf
#undef pow
#undef sin
#undef sinh
#undef sqrt
#undef tan
#undef tanh

#if _GLIBCXX_USE_C99_MATH
#if !_GLIBCXX_USE_C99_FP_MACROS_DYNAMIC

// These are always macros imported from C99-land.
#undef fpclassify
#undef isfinite
#undef isinf
#undef isnan
#undef isnormal
#undef signbit
#undef isgreater
#undef isgreaterequal
#undef isless
#undef islessequal
#undef islessgreater
#undef isunordered

// Provide global functions to replace these macros. Where possible these use
// 'bool' return types and even constexpr as the macros they replace don't have
// observable return types anyways and the expressions they lower to are
// potentially viable constant expressions.
extern "C++" {
#if __cplusplus >= 201103L
constexpr int
fpclassify(float __x)
{ return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL,
                              FP_SUBNORMAL, FP_ZERO, __x); }

constexpr int
fpclassify(double __x)
{ return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL,
                              FP_SUBNORMAL, FP_ZERO, __x); }

constexpr int
fpclassify(long double __x)
{ return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL,
                              FP_SUBNORMAL, FP_ZERO, __x); }

template<typename _Tp>
  constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value,
                                            int>::__type
  fpclassify(_Tp __x)
  { return __x != 0 ? FP_NORMAL : FP_ZERO; }

constexpr bool
isfinite(float __x)
{ return __builtin_isfinite(__x); }

constexpr bool
isfinite(double __x)
{ return __builtin_isfinite(__x); }

constexpr bool
isfinite(long double __x)
{ return __builtin_isfinite(__x); }

template<typename _Tp>
  constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value,
                                            bool>::__type
  isfinite(_Tp __x)
  { return true; }

constexpr bool
isnormal(float __x)
{ return __builtin_isnormal(__x); }

constexpr bool
isnormal(double __x)
{ return __builtin_isnormal(__x); }

constexpr bool
isnormal(long double __x)
{ return __builtin_isnormal(__x); }

template<typename _Tp>
  constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value,
                                            bool>::__type
  isnormal(_Tp __x)
  { return __x != 0 ? true : false; }

// The front-end doesn't provide a type generic builtin (libstdc++/58625).
constexpr bool
signbit(float __x)
{ return __builtin_signbitf(__x); }

constexpr bool
signbit(double __x)
{ return __builtin_signbit(__x); }

constexpr bool
signbit(long double __x)
{ return __builtin_signbitl(__x); }

template<typename _Tp>
  constexpr typename __gnu_cxx::__enable_if<std::__is_integer<_Tp>::__value,
                                            bool>::__type
  signbit(_Tp __x)
  { return __x < 0 ? true : false; }

constexpr bool
isgreater(float __x, float __y)
{ return __builtin_isgreater(__x, __y); }

constexpr bool
isgreater(double __x, double __y)
{ return __builtin_isgreater(__x, __y); }

constexpr bool
isgreater(long double __x, long double __y)
{ return __builtin_isgreater(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  isgreater(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_isgreater(__type(__x), __type(__y));
  }

constexpr bool
isgreaterequal(float __x, float __y)
{ return __builtin_isgreaterequal(__x, __y); }

constexpr bool
isgreaterequal(double __x, double __y)
{ return __builtin_isgreaterequal(__x, __y); }

constexpr bool
isgreaterequal(long double __x, long double __y)
{ return __builtin_isgreaterequal(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  isgreaterequal(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_isgreaterequal(__type(__x), __type(__y));
  }

constexpr bool
isless(float __x, float __y)
{ return __builtin_isless(__x, __y); }

constexpr bool
isless(double __x, double __y)
{ return __builtin_isless(__x, __y); }

constexpr bool
isless(long double __x, long double __y)
{ return __builtin_isless(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  isless(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_isless(__type(__x), __type(__y));
  }

constexpr bool
islessequal(float __x, float __y)
{ return __builtin_islessequal(__x, __y); }

constexpr bool
islessequal(double __x, double __y)
{ return __builtin_islessequal(__x, __y); }

constexpr bool
islessequal(long double __x, long double __y)
{ return __builtin_islessequal(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  islessequal(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_islessequal(__type(__x), __type(__y));
  }

constexpr bool
islessgreater(float __x, float __y)
{ return __builtin_islessgreater(__x, __y); }

constexpr bool
islessgreater(double __x, double __y)
{ return __builtin_islessgreater(__x, __y); }

constexpr bool
islessgreater(long double __x, long double __y)
{ return __builtin_islessgreater(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  islessgreater(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_islessgreater(__type(__x), __type(__y));
  }

constexpr bool
isunordered(float __x, float __y)
{ return __builtin_isunordered(__x, __y); }

constexpr bool
isunordered(double __x, double __y)
{ return __builtin_isunordered(__x, __y); }

constexpr bool
isunordered(long double __x, long double __y)
{ return __builtin_isunordered(__x, __y); }

template<typename _Tp, typename _Up>
  constexpr typename
  __gnu_cxx::__enable_if<(std::__is_arithmetic<_Tp>::__value
                          && std::__is_arithmetic<_Up>::__value), bool>::__type
  isunordered(_Tp __x, _Up __y)
  {
    typedef typename __gnu_cxx::__promote_2<_Tp, _Up>::__type __type;
    return __builtin_isunordered(__type(__x), __type(__y));
  }

#else

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  fpclassify(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL,
                                FP_SUBNORMAL, FP_ZERO, __type(__f));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isfinite(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isfinite(__type(__f));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isnormal(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isnormal(__type(__f));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  signbit(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
      return sizeof(__type) == sizeof(float)
	? __builtin_signbitf(__type(__f))
	: sizeof(__type) == sizeof(double)
	? __builtin_signbit(__type(__f))
	: __builtin_signbitl(__type(__f));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isgreater(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isgreater(__type(__f1), __type(__f2));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isgreaterequal(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isgreaterequal(__type(__f1), __type(__f2));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isless(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isless(__type(__f1), __type(__f2));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  islessequal(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_islessequal(__type(__f1), __type(__f2));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  islessgreater(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_islessgreater(__type(__f1), __type(__f2));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isunordered(_Tp __f1, _Tp __f2)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isunordered(__type(__f1), __type(__f2));
  }

#endif

// The C library provides the double variant of isinf and isnan without
// constexpr, so we just use the C++98 templates to add the desired overloads.
template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isinf(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isinf_sign(__type(__f));
  }

template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<std::__is_arithmetic<_Tp>::__value,
                                         int>::__type
  isnan(_Tp __f)
  {
    typedef typename __gnu_cxx::__promote<_Tp>::__type __type;
    return __builtin_isnan(__type(__f));
  }
} // extern "C++"

#endif /* _GLIBCXX_USE_C99_FP_MACROS_DYNAMIC */
#endif

#if __cplusplus >= 201103L

#ifdef _GLIBCXX_USE_C99_MATH_TR1

#undef acosh
#undef acoshf
#undef acoshl
#undef asinh
#undef asinhf
#undef asinhl
#undef atanh
#undef atanhf
#undef atanhl
#undef cbrt
#undef cbrtf
#undef cbrtl
#undef copysign
#undef copysignf
#undef copysignl
#undef erf
#undef erff
#undef erfl
#undef erfc
#undef erfcf
#undef erfcl
#undef exp2
#undef exp2f
#undef exp2l
#undef expm1
#undef expm1f
#undef expm1l
#undef fdim
#undef fdimf
#undef fdiml
#undef fma
#undef fmaf
#undef fmal
#undef fmax
#undef fmaxf
#undef fmaxl
#undef fmin
#undef fminf
#undef fminl
#undef hypot
#undef hypotf
#undef hypotl
#undef ilogb
#undef ilogbf
#undef ilogbl
#undef lgamma
#undef lgammaf
#undef lgammal
#undef llrint
#undef llrintf
#undef llrintl
#undef llround
#undef llroundf
#undef llroundl
#undef log1p
#undef log1pf
#undef log1pl
#undef log2
#undef log2f
#undef log2l
#undef logb
#undef logbf
#undef logbl
#undef lrint
#undef lrintf
#undef lrintl
#undef lround
#undef lroundf
#undef lroundl
#undef nan
#undef nanf
#undef nanl
#undef nearbyint
#undef nearbyintf
#undef nearbyintl
#undef nextafter
#undef nextafterf
#undef nextafterl
#undef nexttoward
#undef nexttowardf
#undef nexttowardl
#undef remainder
#undef remainderf
#undef remainderl
#undef remquo
#undef remquof
#undef remquol
#undef rint
#undef rintf
#undef rintl
#undef round
#undef roundf
#undef roundl
#undef scalbln
#undef scalblnf
#undef scalblnl
#undef scalbn
#undef scalbnf
#undef scalbnl
#undef tgamma
#undef tgammaf
#undef tgammal
#undef trunc
#undef truncf
#undef truncl

#endif // _GLIBCXX_USE_C99_MATH_TR1

#endif // C++11

#endif // C++

#endif
