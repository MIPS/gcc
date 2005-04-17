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


#include<vector>
#include<algorithm>

#include<sstream>
#include<testsuite_performance.h>
#include<testsuite_iterators.h>

using namespace std;
using namespace __gnu_test;

const int length = 100000;
const int loop_count = 500;

// Used to stop move symantics
struct vec_wrap
{
  vector<int> v; 
  
  explicit vec_wrap(vector<int> in_vec) : v(in_vec)
  {  }
  
  vec_wrap() { }
};

bool inline
operator==(const vec_wrap& lhs, const vec_wrap& rhs)
{ return lhs.v == rhs.v; }

void
test1()
{
  time_counter time;
  resource_counter resource;
  vector<vector<int> > moveable_vecs;
  for(int i = 0; i < length; i++)
  {
    vector<int> vec(1, i);
    moveable_vecs.push_back(vec);
  }
  moveable_vecs[0] = vector<int>();
  start_counters(time, resource);
  for(int loop = 0 ; loop < loop_count; loop++)
  {
    moveable_vecs[1]=moveable_vecs[0];
    unique(moveable_vecs.begin(), moveable_vecs.end() - loop);
  }
  stop_counters(time, resource);
  report_performance(__FILE__, "moveable vector", time, resource);
  clear_counters(time, resource);
}

void
test2()
{
  time_counter time;
  resource_counter resource;
  vector<vec_wrap> unmoveable_vecs;
  for(int i = 0; i < length; i++)
  {
    vector<int> vec(20, i);
    vec_wrap wrap(vec);
    unmoveable_vecs.push_back(wrap);
  }
  unmoveable_vecs[0] = vec_wrap();
  start_counters(time, resource);
  for(int loop = 0 ; loop < loop_count; loop++)
  {
    unmoveable_vecs[1] = unmoveable_vecs[0];
    unique(unmoveable_vecs.begin(), unmoveable_vecs.end());
  }
  stop_counters(time, resource);
  report_performance(__FILE__, "20 element unmoveable vector", time, resource);
  clear_counters(time, resource);
}
  
  
void
test3()
{
  time_counter time;
  resource_counter resource;
  vector<vec_wrap> unmoveable_vecs;
  for(int i = 0; i < length; i++)
  {
    vector<int> vec(1, i);
    vec_wrap wrap(vec);
    unmoveable_vecs.push_back(wrap);
  }
  unmoveable_vecs[0] = vec_wrap();
  start_counters(time, resource);
  for(int loop = 0 ; loop < loop_count; loop++)
  {
    unmoveable_vecs[1] = unmoveable_vecs[0];
    unique(unmoveable_vecs.begin(), unmoveable_vecs.end());
  }
  stop_counters(time, resource);
  report_performance(__FILE__, "1 element unmoveable vector", time, resource);
  clear_counters(time, resource);
}
  
  
int
main(void)
{
  test1();
  test2();
  test3();
}


