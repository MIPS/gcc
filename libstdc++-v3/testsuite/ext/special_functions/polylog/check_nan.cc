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

// polylog

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float sf = std::numeric_limits<float>::quiet_NaN();
  double sd = std::numeric_limits<double>::quiet_NaN();
  long double sl = std::numeric_limits<long double>::quiet_NaN();

  float wf = 0.5F;
  double wd = 0.5;
  long double wl = 0.5L;

  float a = __gnu_cxx::polylog(sf, wf);
  float b = __gnu_cxx::polylogf(sf, wf);
  double c = __gnu_cxx::polylog(sd, wd);
  long double d = __gnu_cxx::polylog(sl, wl);
  long double e = __gnu_cxx::polylogl(sl, wl);

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
  float sf = 1.5F;
  double sd = 1.5;
  long double sl = 1.5L;

  float af = std::numeric_limits<float>::quiet_NaN();
  double ad = std::numeric_limits<double>::quiet_NaN();
  long double al = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::polylog(sf, af);
  float b = __gnu_cxx::polylogf(sf, af);
  double c = __gnu_cxx::polylog(sd, ad);
  long double d = __gnu_cxx::polylog(sl, al);
  long double e = __gnu_cxx::polylogl(sl, al);

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

