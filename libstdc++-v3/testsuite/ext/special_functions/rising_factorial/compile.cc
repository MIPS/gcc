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

// pochhammer

#include <cmath>

void
test01()
{
  float af = 0.5F;
  double ad = 0.5;
  long double al = 0.5L;

  float nf = 1.5F;
  double nd = 1.5;
  long double nl = 1.5L;

  __gnu_cxx::pochhammer(af, nf);
  __gnu_cxx::pochhammerf(af, nf);
  __gnu_cxx::pochhammer(ad, nd);
  __gnu_cxx::pochhammer(al, nl);
  __gnu_cxx::pochhammerl(al, nl);

  __gnu_cxx::pochhammer(ad, nf);
  __gnu_cxx::pochhammerf(af, nf);
  __gnu_cxx::pochhammer(al, nd);
  __gnu_cxx::pochhammer(af, nl);
  __gnu_cxx::pochhammerl(al, nl);

  return;
}

