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

// owens_t

#include <cmath>

void
test01()
{
  float hf = 0.5F, af = 0.5F;
  double hd = 0.5, ad = 0.5;
  long double hl = 0.5L, al = 0.5L;

  __gnu_cxx::owens_t(hf, af);
  __gnu_cxx::owens_tf(hf, af);
  __gnu_cxx::owens_t(hd, ad);
  __gnu_cxx::owens_t(hl, al);
  __gnu_cxx::owens_tl(hl, al);

  return;
}

