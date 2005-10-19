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

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#include <vector>
#include <string>

#include<testsuite_performance.h>

using namespace std;
using namespace __gnu_test;

const int length = 500;
const int loop_count = 200;

struct string_wrap
{
  string s; 
  
  explicit string_wrap(string in_str) : s(in_str)
  {  }
  
  string_wrap() { }
};

bool inline
operator==(const string_wrap& lhs, const string_wrap& rhs)
{ return lhs.s == rhs.s; }

const string s(100, 'z');

void test01()
{
  time_counter time;
  resource_counter resource;

  start_counters(time, resource);
  for (int i = 0; i < loop_count; ++i)
    {
      vector<string> vs;
      for (int j = 0; j < length; ++j)
	vs.insert(vs.begin(), s);
    }
  stop_counters(time, resource);

  report_performance(__FILE__, "moveable string", time, resource);
}

void test02()
{
  time_counter time;
  resource_counter resource;

  const string_wrap ws(s);

  start_counters(time, resource);
  for (int i = 0; i < loop_count; ++i)
    {
      vector<string_wrap> vws;
      for (int j = 0; j < length; ++j)
	vws.insert(vws.begin(), ws);
    }
  stop_counters(time, resource);

  report_performance(__FILE__, "unmoveable string", time, resource);
}

int main()
{
  test01();
  test02();
  return 0;
}
