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

// ellint_rf

#include <cmath>
#include <complex>
#include <testsuite_hooks.h>

void
test01()
{
  auto rf1 = __gnu_cxx::ellint_rf<int>(1, 2, 0);

  bool test [[gnu::unused]] = true;
  double eps = 1.0e-12;
  VERIFY(std::abs(rf1 - 1.3110287771461) < eps);
}

int
main()
{
  test01();
  return 0;
}
