// { dg-do compile }
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

void
test01()
{
  float thetaf = 0.5F, phif = 1.235F;
  double thetad = 0.5, phid = 1.235;
  long double thetal = 0.5L, phil = 1.235L;
  unsigned int l = 2;
  int m = 1;

  __gnu_cxx::sph_harmonic(l, m, thetaf, phif);
  __gnu_cxx::sph_harmonicf(l, m, thetaf, phif);
  __gnu_cxx::sph_harmonic(l, m, thetad, phid);
  __gnu_cxx::sph_harmonic(l, m, thetal, phil);
  __gnu_cxx::sph_harmonicl(l, m, thetal, phil);

  return;
}

