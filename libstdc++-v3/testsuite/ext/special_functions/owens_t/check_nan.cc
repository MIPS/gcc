// { dg-require-c-std "" }
// { dg-add-options ieee }
// { dg-options "-D__STDCPP_WANT_MATH_SPEC_FUNCS__" }

// Copyright (C) 2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// owens_t

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float hf = std::numeric_limits<float>::quiet_NaN();
  double hd = std::numeric_limits<double>::quiet_NaN();
  long double hl = std::numeric_limits<long double>::quiet_NaN();

  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  float a = __gnu_cxx::owens_t(hf, af);
  float b = __gnu_cxx::owens_tf(hf, af);
  double c = __gnu_cxx::owens_t(hd, ad);
  long double d = __gnu_cxx::owens_t(hl, al);
  long double e = __gnu_cxx::owens_tl(hl, al);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

void
test02()
{
  float hf = 0.5F;
  double hd = 0.5;
  long double hl = 0.5L;

  float af = std::numeric_limits<float>::quiet_NaN();
  double ad = std::numeric_limits<double>::quiet_NaN();
  long double al = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::owens_t(hf, af);
  float b = __gnu_cxx::owens_tf(hf, af);
  double c = __gnu_cxx::owens_t(hd, ad);
  long double d = __gnu_cxx::owens_t(hl, al);
  long double e = __gnu_cxx::owens_tl(hl, al);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

int
main()
{
  test01();
  test02();
  return 0;
}

