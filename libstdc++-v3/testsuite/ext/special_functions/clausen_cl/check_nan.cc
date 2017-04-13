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

// clausen

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float wf = std::numeric_limits<float>::quiet_NaN();
  double wd = std::numeric_limits<double>::quiet_NaN();
  long double wl = std::numeric_limits<long double>::quiet_NaN();

  unsigned int m = 0;

  std::complex<float> a = __gnu_cxx::clausen(m, wf);
  std::complex<float> b = __gnu_cxx::clausenf(m, wf);
  std::complex<double> c = __gnu_cxx::clausen(m, wd);
  std::complex<long double> d = __gnu_cxx::clausen(m, wl);
  std::complex<long double> e = __gnu_cxx::clausenl(m, wl);

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
  return 0;
}

