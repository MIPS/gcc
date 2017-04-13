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

// hurwitz_zeta

#include <cmath>

void
test01()
{
  float sf = 1.5F;
  double sd = 1.5;
  long double sl = 1.5L;

  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  __gnu_cxx::hurwitz_zeta(sf, af);
  __gnu_cxx::hurwitz_zetaf(sf, af);
  __gnu_cxx::hurwitz_zeta(sd, ad);
  __gnu_cxx::hurwitz_zeta(sl, al);
  __gnu_cxx::hurwitz_zetal(sl, al);

  __gnu_cxx::hurwitz_zeta(sf, ad);
  __gnu_cxx::hurwitz_zetaf(sf, af);
  __gnu_cxx::hurwitz_zeta(sd, al);
  __gnu_cxx::hurwitz_zeta(sl, af);
  __gnu_cxx::hurwitz_zetal(sl, al);

  return;
}

