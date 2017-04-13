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

// pochhammer

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  float nf = std::numeric_limits<float>::quiet_NaN();
  double nd = std::numeric_limits<double>::quiet_NaN();
  long double nl = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::pochhammer(af, nf);
  float b = __gnu_cxx::pochhammerf(af, nf);
  double c = __gnu_cxx::pochhammer(ad, nd);
  long double d = __gnu_cxx::pochhammer(al, nl);
  long double e = __gnu_cxx::pochhammerl(al, nl);

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
  float af = std::numeric_limits<float>::quiet_NaN();
  double ad = std::numeric_limits<double>::quiet_NaN();
  long double al = std::numeric_limits<long double>::quiet_NaN();

  float nf = 1.5F;
  double nd = 1.5;
  long double nl = 1.5L;

  float a = __gnu_cxx::pochhammer(af, nf);
  float b = __gnu_cxx::pochhammerf(af, nf);
  double c = __gnu_cxx::pochhammer(ad, nd);
  long double d = __gnu_cxx::pochhammer(al, nl);
  long double e = __gnu_cxx::pochhammerl(al, nl);

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

