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

// sph_harmonic

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  unsigned int l = 2;
  int m = 1;

  float thetaf = std::numeric_limits<float>::quiet_NaN();
  double thetad = std::numeric_limits<double>::quiet_NaN();
  long double thetal = std::numeric_limits<long double>::quiet_NaN();

  float phif = 1.235F;
  double phid = 1.235;
  long double phil = 1.235L;

  std::complex<float> a = __gnu_cxx::sph_harmonic(l, m, thetaf, phif);
  std::complex<float> b = __gnu_cxx::sph_harmonicf(l, m, thetaf, phif);
  std::complex<double> c = __gnu_cxx::sph_harmonic(l, m, thetad, phid);
  std::complex<long double> d = __gnu_cxx::sph_harmonic(l, m, thetal, phil);
  std::complex<long double> e = __gnu_cxx::sph_harmonicl(l, m, thetal, phil);

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
  unsigned int l = 2, m = 1;

  float thetaf = 0.5F;
  double thetad = 0.5;
  long double thetal = 0.5L;

  float phif = std::numeric_limits<float>::quiet_NaN();
  double phid = std::numeric_limits<double>::quiet_NaN();
  long double phil = std::numeric_limits<long double>::quiet_NaN();

  std::complex<float> a = __gnu_cxx::sph_harmonic(l, m, thetaf, phif);
  std::complex<float> b = __gnu_cxx::sph_harmonicf(l, m, thetaf, phif);
  std::complex<double> c = __gnu_cxx::sph_harmonic(l, m, thetad, phid);
  std::complex<long double> d = __gnu_cxx::sph_harmonic(l, m, thetal, phil);
  std::complex<long double> e = __gnu_cxx::sph_harmonicl(l, m, thetal, phil);

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

