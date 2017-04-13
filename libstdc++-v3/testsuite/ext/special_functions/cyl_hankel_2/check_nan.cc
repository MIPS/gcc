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

// cyl_hankel_2

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float xf = std::numeric_limits<float>::quiet_NaN();
  double xd = std::numeric_limits<double>::quiet_NaN();
  long double xl = std::numeric_limits<long double>::quiet_NaN();

  float nuf = 0.0F;
  double nud = 0.0;
  long double nul = 0.0L;

  std::complex<float> a = __gnu_cxx::cyl_hankel_2(nuf, xf);
  std::complex<float> b = __gnu_cxx::cyl_hankel_2f(nuf, xf);
  std::complex<double> c = __gnu_cxx::cyl_hankel_2(nud, xd);
  std::complex<long double> d = __gnu_cxx::cyl_hankel_2(nul, xl);
  std::complex<long double> e = __gnu_cxx::cyl_hankel_2l(nul, xl);

  bool test [[gnu::unused]] = true;
  VERIFY(__gnu_cxx::isnan(a));
  VERIFY(__gnu_cxx::isnan(b));
  VERIFY(__gnu_cxx::isnan(c));
  VERIFY(__gnu_cxx::isnan(d));
  VERIFY(__gnu_cxx::isnan(e));

  return;
}

void
test02()
{
  float xf = 1.0F;
  double xd = 1.0;
  long double xl = 1.0L;

  float nuf = std::numeric_limits<float>::quiet_NaN();
  double nud = std::numeric_limits<double>::quiet_NaN();
  long double nul = std::numeric_limits<long double>::quiet_NaN();

  std::complex<float> a = __gnu_cxx::cyl_hankel_2(nuf, xf);
  std::complex<float> b = __gnu_cxx::cyl_hankel_2f(nuf, xf);
  std::complex<double> c = __gnu_cxx::cyl_hankel_2(nud, xd);
  std::complex<long double> d = __gnu_cxx::cyl_hankel_2(nul, xl);
  std::complex<long double> e = __gnu_cxx::cyl_hankel_2l(nul, xl);

  bool test [[gnu::unused]] = true;
  VERIFY(__gnu_cxx::isnan(a));
  VERIFY(__gnu_cxx::isnan(b));
  VERIFY(__gnu_cxx::isnan(c));
  VERIFY(__gnu_cxx::isnan(d));
  VERIFY(__gnu_cxx::isnan(e));

  return;
}

int
main()
{
  test01();
  test02();
  return 0;
}

