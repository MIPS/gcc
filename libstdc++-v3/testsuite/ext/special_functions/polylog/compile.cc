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

// polylog

#include <cmath>

void
test01()
{
  float sf = 1.5F;
  double sd = 1.5;
  long double sl = 1.5L;

  float wf = 0.5F;
  double wd = 0.5;
  long double wl = 0.5L;

  __gnu_cxx::polylog(sf, wf);
  __gnu_cxx::polylogf(sf, wf);
  __gnu_cxx::polylog(sd, wd);
  __gnu_cxx::polylog(sl, wl);
  __gnu_cxx::polylogl(sl, wl);

  __gnu_cxx::polylog(sf, wd);
  __gnu_cxx::polylogf(sf, wf);
  __gnu_cxx::polylog(sd, wl);
  __gnu_cxx::polylog(sl, wf);
  __gnu_cxx::polylogl(sl, wl);

  return;
}

