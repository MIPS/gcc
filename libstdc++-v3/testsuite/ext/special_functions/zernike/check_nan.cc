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

// zernike

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  unsigned int n = 2;
  int m = 1;

  float rhof = std::numeric_limits<float>::quiet_NaN();
  double rhod = std::numeric_limits<double>::quiet_NaN();
  long double rhol = std::numeric_limits<long double>::quiet_NaN();

  float phif = 0.5F;
  double phid = 0.5;
  long double phil = 0.5L;

  float a = __gnu_cxx::zernike(n, m, rhof, phif);
  float b = __gnu_cxx::zernikef(n, m, rhof, phif);
  double c = __gnu_cxx::zernike(n, m, rhod, phid);
  long double d = __gnu_cxx::zernike(n, m, rhol, phil);
  long double e = __gnu_cxx::zernikel(n, m, rhol, phil);

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
  unsigned int n = 2;
  int m = 1;

  float rhof = 3.25F;
  double rhod = 3.25;
  long double rhol = 3.25L;

  float phif = std::numeric_limits<float>::quiet_NaN();
  double phid = std::numeric_limits<double>::quiet_NaN();
  long double phil = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::zernike(n, m, rhof, phif);
  float b = __gnu_cxx::zernikef(n, m, rhof, phif);
  double c = __gnu_cxx::zernike(n, m, rhod, phid);
  long double d = __gnu_cxx::zernike(n, m, rhol, phil);
  long double e = __gnu_cxx::zernikel(n, m, rhol, phil);

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

