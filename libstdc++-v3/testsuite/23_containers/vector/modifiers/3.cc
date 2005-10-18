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

// 23.2.4.3 vector modifiers

#include <vector>
#include <testsuite_hooks.h>
#include <testsuite_rvalref.h>

using namespace __gnu_test;

bool test __attribute__((unused)) = true;

// Check modifiers work when inserting members of the same vector.
// The principle is to check that we don't start moving elements around
// and making space for new values when the value we are inserting
// is already in the vector, unless we copy it somewhere else first.

void
test01()
{
  std::vector<copycounter> vec(2);
  copycounter::copycount = 0;
  // The standard doesn't state this line has to pass, but this test
  // requires it.
  VERIFY(vec.capacity() == 2);
  vec[0] = 0;
  vec[1] = 1;
  vec.insert(vec.begin(), vec[1]);
  VERIFY(vec[0] == 1 && vec[1] == 0 && vec[2] == 1);
}

void
test02()
{
  std::vector<copycounter> vec(2);
  vec.reserve(10);
  vec[0] = 0;
  vec[1] = 1;
  vec.insert(vec.begin(), vec[1]);
  VERIFY(vec[0] == 1 && vec[1] == 0 && vec[2] == 1);
}

void
test03()
{
  std::vector<copycounter> vec(3);
  copycounter::copycount = 0;
  // The standard doesn't state this line has to pass, but this test
  // requires it.
  VERIFY(vec.capacity() == 3);
  vec[0] = 0;
  vec[1] = 0;
  vec[2] = 1; 
  vec.insert(vec.begin(), 2, vec[2]);
  VERIFY(vec[0] == 1 && vec[1] == 1 && vec[2] == 0 && vec[3] == 0 &&
	 vec[4] == 1);
}

void
test04()
{
  std::vector<copycounter> vec(3);
  vec.reserve(10);
  vec[0] = 0;
  vec[1] = 0;
  vec[2] = 1;
  vec.insert(vec.begin(), 2, vec[2]);
  VERIFY(vec[0] == 1 && vec[1] == 1 && vec[2] == 0 && vec[3] == 0 &&
	 vec[4] == 1);
}

void
test05()
{
  std::vector<copycounter> vec(2);
  copycounter::copycount = 0;
  // The standard doesn't state this line has to pass, but this test
  // requires it.
  VERIFY(vec.capacity() == 2);
  vec[0] = 0;
  vec[1] = 1;
  vec.push_back(vec[0]);
  VERIFY(vec[0] == 0 && vec[1] == 1 && vec[2] == 0);
}

int main()
{
  test01();
  test02();
  test03();
  test04();
  test05();
  return 0;
}
