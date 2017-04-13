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

// ellint_rj

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float xf = std::numeric_limits<float>::quiet_NaN();
  double xd = std::numeric_limits<double>::quiet_NaN();
  long double xl = std::numeric_limits<long double>::quiet_NaN();

  float yf = 10.0F;
  double yd = 10.0;
  long double bl = 10.0L;

  float zf = 3.0F;
  double zd = 3.0;
  long double cl = 3.0L;

  float pf = 0.5F;
  double pd = 0.5;
  long double pl = 0.5L;

  float a = __gnu_cxx::ellint_rj(xf, yf, zf, pf);
  float b = __gnu_cxx::ellint_rjf(xf, yf, zf, pf);
  double c = __gnu_cxx::ellint_rj(xd, yd, zd, pd);
  long double d = __gnu_cxx::ellint_rj(xl, bl, cl, pl);
  long double e = __gnu_cxx::ellint_rjl(xl, bl, cl, pl);

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
  float xf = 2.0F;
  double xd = 2.0;
  long double xl = 2.0L;

  float yf = std::numeric_limits<float>::quiet_NaN();
  double yd = std::numeric_limits<double>::quiet_NaN();
  long double bl = std::numeric_limits<long double>::quiet_NaN();

  float zf = 3.0F;
  double zd = 3.0;
  long double cl = 3.0L;

  float pf = 0.5F;
  double pd = 0.5;
  long double pl = 0.5L;

  float a = __gnu_cxx::ellint_rj(xf, yf, zf, pf);
  float b = __gnu_cxx::ellint_rjf(xf, yf, zf, pf);
  double c = __gnu_cxx::ellint_rj(xd, yd, zd, pd);
  long double d = __gnu_cxx::ellint_rj(xl, bl, cl, pl);
  long double e = __gnu_cxx::ellint_rjl(xl, bl, cl, pl);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

void
test03()
{
  float xf = 2.0F;
  double xd = 2.0;
  long double xl = 2.0L;

  float yf = 10.0F;
  double yd = 10.0;
  long double bl = 10.0L;

  float zf = std::numeric_limits<float>::quiet_NaN();
  double zd = std::numeric_limits<double>::quiet_NaN();
  long double cl = std::numeric_limits<long double>::quiet_NaN();

  float pf = 0.5F;
  double pd = 0.5;
  long double pl = 0.5L;

  float a = __gnu_cxx::ellint_rj(xf, yf, zf, pf);
  float b = __gnu_cxx::ellint_rjf(xf, yf, zf, pf);
  double c = __gnu_cxx::ellint_rj(xd, yd, zd, pd);
  long double d = __gnu_cxx::ellint_rj(xl, bl, cl, pl);
  long double e = __gnu_cxx::ellint_rjl(xl, bl, cl, pl);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

void
test04()
{
  float xf = 2.0F;
  double xd = 2.0;
  long double xl = 2.0L;

  float yf = 10.0F;
  double yd = 10.0;
  long double bl = 10.0L;

  float zf = 3.0F;
  double zd = 3.0;
  long double cl = 3.0L;

  float pf = std::numeric_limits<float>::quiet_NaN();
  double pd = std::numeric_limits<double>::quiet_NaN();
  long double pl = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::ellint_rj(xf, yf, zf, pf);
  float b = __gnu_cxx::ellint_rjf(xf, yf, zf, pf);
  double c = __gnu_cxx::ellint_rj(xd, yd, zd, pd);
  long double d = __gnu_cxx::ellint_rj(xl, bl, cl, pl);
  long double e = __gnu_cxx::ellint_rjl(xl, bl, cl, pl);

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
  test03();
  test04();
  return 0;
}

