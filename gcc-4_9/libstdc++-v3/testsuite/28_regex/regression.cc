// { dg-options "-std=gnu++11" }

//
// Copyright (C) 2015 Free Software Foundation, Inc.
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

#include <testsuite_hooks.h>
#include <testsuite_regex.h>

using namespace __gnu_test;
using namespace std;

// PR libstdc++/67362
void
test01()
{
  bool test __attribute__((unused)) = true;

  regex re("((.)", regex_constants::basic);
}

// PR libstdc++/77356
void
test02()
{
  bool test __attribute__((unused)) = true;

  static const char* kNumericAnchor ="(\\$|usd)(usd|\\$|to|and|up to|[0-9,\\.\\-\\sk])+";
  const std::regex re(kNumericAnchor);
  (void)re;
}

void
test03()
{
  bool test __attribute__((unused)) = true;

  VERIFY(regex_match_debug("!", std::regex("[![:alnum:]]")));
  VERIFY(regex_match_debug("-", std::regex("[a-]", regex_constants::basic)));
  VERIFY(regex_match_debug("-", std::regex("[a-]")));
}

int
main()
{
  test01();
  test02();
  test03();
  return 0;
}

