// Copyright (C) 2019-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-do compile { target c++20 } }

#include <span>

void
test01()
{
  int a[4];
  std::span<int, 4> s(a);
  (void) s.subspan<5, 0>(); // { dg-error "here" }
}

void
test02()
{
  int a[4];
  std::span<int, 4> s(a);
  (void) s.subspan<3, 5>(); // { dg-error "here" }
}

void
test03()
{
  int a[4];
  std::span<int, 4> s(a);
  (void) s.subspan<3, 2>(); // { dg-error "here" }
}

// { dg-error "static assertion failed" "" { target *-*-* } 0 }
