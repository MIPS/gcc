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

// jacobi_dn

#include <cmath>

void
test01()
{
  float kf = 1.5F;
  double kd = 1.5;
  long double kl = 1.5L;

  float uf = 0.5F;
  double ud = 0.5;
  long double ul = 0.5L;

  __gnu_cxx::jacobi_dn(kf, uf);
  __gnu_cxx::jacobi_dnf(kf, uf);
  __gnu_cxx::jacobi_dn(kd, ud);
  __gnu_cxx::jacobi_dn(kl, ul);
  __gnu_cxx::jacobi_dnl(kl, ul);

  __gnu_cxx::jacobi_dn(kf, ud);
  __gnu_cxx::jacobi_dnf(kf, uf);
  __gnu_cxx::jacobi_dn(kd, ul);
  __gnu_cxx::jacobi_dn(kl, uf);
  __gnu_cxx::jacobi_dnl(kl, ul);

  return;
}

