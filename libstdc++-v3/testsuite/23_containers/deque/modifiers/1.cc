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
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#include <deque>
#include <testsuite_hooks.h>
#include <testsuite_rvalref.h>

using namespace __gnu_test;

bool test __attribute__((unused)) = true;

// Check modifiers work when inserting members of the same deque.
// The principle is to check that we don't start moving elements around
// and making space for new values when the value we are inserting
// is already in the deque, unless we copy it somewhere else first.

void
test01()
{
  std::deque<copycounter> deq(2);
  copycounter::copycount = 0;
  deq[0] = 0;
  deq[1] = 1;
  deq.insert(deq.begin() + 1, deq[1]);
  VERIFY(deq[0] == 0 && deq[1] == 1 && deq[2] == 1);
}

void
test02()
{
  std::deque<copycounter> deq(3);
  copycounter::copycount = 0;
  deq[0] = 0;
  deq[1] = 0;
  deq[2] = 1; 
  deq.insert(deq.begin() + 1, 2, deq[2]);
  VERIFY(deq[0] == 0 && deq[1] == 1 && deq[2] == 1 && deq[3] == 0 &&
	 deq[4] == 1);
}

void
test03()
{
  std::deque<copycounter> deq(2);
  copycounter::copycount = 0;
  deq[0] = 0;
  deq[1] = 1;
  deq.push_back(deq[0]);
  VERIFY(deq[0] == 0 && deq[1] == 1 && deq[2] == 0);
}

int main()
{
  test01();
  test02();
  test03();
  return 0;
}
