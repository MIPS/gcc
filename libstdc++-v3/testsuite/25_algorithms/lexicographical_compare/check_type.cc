// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// { dg-do compile }

#include <algorithm>

struct Lhs1 { };

struct Rhs1 { };

bool operator<(const Lhs1&, const Rhs1&) {return true;}

bool operator<(const Rhs1&, const Lhs1&) {return false;}

struct X { };

bool predicate(const X&, const X&) {return true;}

void test1()
{
  Lhs1 lhs1[1];
  Rhs1 rhs1[1];

  std::lexicographical_compare(lhs1, lhs1 + 1, rhs1, rhs1 + 1);

  X x1[1];
  X x2[1];

  std::lexicographical_compare(x1, x1 + 1, x2, x2 + 1, predicate);
}
