// 2005-12-09  Paolo Carlini  <pcarlini@suse.de>
//
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

#include <string>
#include <vector>
#include <testsuite_performance.h>

using namespace std;
using namespace __gnu_test;

const int max_length = 1000;
const int max_loop_count = 1000000000;

void test01()
{
  time_counter time;
  resource_counter resource;

  vector<string> vs;
  for (int i = 0; i < max_length; ++i)
    vs.push_back(string(i, 'a'));

  vector<int> vi(max_length);
  
  for (int length = 1; length <= max_length; length *= 10)
    {
      const int loop_count = max_loop_count / length;
      start_counters(time, resource);
      for (int j = 0; j < loop_count; ++j)
	{
	  for (int k = 0; k < length; ++k)
	    vi[k] &= vs[k].compare(vs[k]);
	}
      stop_counters(time, resource);
      report_performance(__FILE__, "", time, resource);
      clear_counters(time, resource);
    }
}

int main()
{
  test01();
  return 0;
}
