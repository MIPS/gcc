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

// ibetac

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float af = std::numeric_limits<float>::quiet_NaN();
  double ad = std::numeric_limits<double>::quiet_NaN();
  long double al = std::numeric_limits<long double>::quiet_NaN();

  float bf = 0.75F;
  double bd = 0.75;
  long double bl = 0.75L;

  float xf = 3.5F;
  double xd = 3.5;
  long double xl = 3.5L;

  float a = __gnu_cxx::ibetac(bf, af, xf);
  float b = __gnu_cxx::ibetacf(bf, af, xf);
  double c = __gnu_cxx::ibetac(bd, ad, xd);
  long double d = __gnu_cxx::ibetac(bl, al, xl);
  long double e = __gnu_cxx::ibetacl(bl, al, xl);

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
  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  float bf = std::numeric_limits<float>::quiet_NaN();
  double bd = std::numeric_limits<double>::quiet_NaN();
  long double bl = std::numeric_limits<long double>::quiet_NaN();

  float xf = 3.5F;
  double xd = 3.5;
  long double xl = 3.5L;

  float a = __gnu_cxx::ibetac(af, bf, xf);
  float b = __gnu_cxx::ibetacf(af, bf, xf);
  double c = __gnu_cxx::ibetac(ad, bd, xd);
  long double d = __gnu_cxx::ibetac(al, bl, xl);
  long double e = __gnu_cxx::ibetacl(al, bl, xl);

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
  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  float bf = 0.75F;
  double bd = 0.75;
  long double bl = 0.75L;

  float xf = std::numeric_limits<float>::quiet_NaN();
  double xd = std::numeric_limits<double>::quiet_NaN();
  long double xl = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::ibetac(af, bf, xf);
  float b = __gnu_cxx::ibetacf(af, bf, xf);
  double c = __gnu_cxx::ibetac(ad, bd, xd);
  long double d = __gnu_cxx::ibetac(al, bl, xl);
  long double e = __gnu_cxx::ibetacl(al, bl, xl);

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
  return 0;
}

