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


#define DISABLE_ITERATOR_DEBUG

#include<vector>
#include<algorithm>
#include<stdio.h>
#include<string>
#include<sstream>

#include<testsuite_performance.h>
#include<testsuite_iterators.h>

using namespace std;
using namespace __gnu_test;

// Used to stop move symantics
struct vec_wrap
{
  vector<int> v; 
  
  explicit vec_wrap(vector<int> in_vec) : v(in_vec)
  { }
  
  vec_wrap()
  { }
};

bool inline
operator<(const vec_wrap& lhs, const vec_wrap& rhs)
{ return lhs.v == rhs.v; }


// Used to perform the move symantics algorithm on an int
struct int_wrap
{
  int i;
  
  explicit int_wrap(int in_val) : i(in_val)
  {  }
  
  int_wrap(__gnu_cxx::__rvalref<int_wrap> in) : i(in.__ref.i)
  { }
  
  void
  operator=(int in)
  { i = in; }
  
  void
  operator=(__gnu_cxx::__rvalref<int_wrap> in)
  { i = in.__ref.i; }
  
  int_wrap()
  { }
};

struct ptr_wrap
{
  vector<int>* ptr;
  ptr_wrap(vector<int>* in_ptr) : ptr(in_ptr)
  { }
};

bool inline
operator<(ptr_wrap lhs, ptr_wrap rhs)
{ return *(lhs.ptr) < *(rhs.ptr); }

bool inline
operator<(const int_wrap& lhs, const int_wrap& rhs)
{ return lhs.i < rhs.i; }

namespace __gnu_cxx
{
  template<>
    struct __is_moveable<int_wrap>
    { static const bool __value = true; };
}

struct
dereference_compare
{
  template<typename T>
    bool operator()(T lhs, T rhs)
    { return *lhs < *rhs; }
};

template<typename T>
void sort_and_output(T& vec, std::string description)
{
  time_counter time;
  resource_counter resource;
  start_counters(time, resource);
  std::sort(vec.begin(), vec.end());
  stop_counters(time, resource);
  report_performance(__FILE__, description.c_str(), time, resource);
  clear_counters(time, resource);
  
}

void
test1(int vec_length, vector<int> sort_vec, string vec_description)
{
  vector<vector<int> > moveable_vecs;
  for(int i = 0; i < sort_vec.size(); i++)
  {
    vector<int> vec(vec_length, sort_vec[i]);
    moveable_vecs.push_back(vec);
  } 
  ostringstream outputline;
  outputline << "moveable vector - length " << sort_vec.size() << ":" <<
    vec_description;
  sort_and_output(moveable_vecs, outputline.str());  
}


void
test2(int vec_length, vector<int> sort_vec, string vec_description )
{
  vector<vec_wrap> unmoveable_vecs;
  for(int i = 0; i < sort_vec.size(); i++)
  {
    vector<int> vec(vec_length, sort_vec[i]);
    vec_wrap wrap(vec);
    unmoveable_vecs.push_back(wrap);
  }  
  ostringstream outputline;
  outputline << "unmoveable vector - length " << sort_vec.size() << ":" <<
    vec_description;
  sort_and_output(unmoveable_vecs, outputline.str());
}

  
void
test3(int vec_length, vector<int> sort_vec, string vec_description)
{
  vector<vector<int> > primary_vec;
  vector<ptr_wrap> ptr_vec;
  for(int i = 0; i < sort_vec.size(); i++)
  {
    vector<int> vec(vec_length, sort_vec[i]);
    primary_vec.push_back(vec);
  }
  for(int i = 0; i < sort_vec.size(); i++)
    ptr_vec.push_back(&primary_vec[i]);
  ostringstream outputline;
  outputline << "pointers to vector - length " << sort_vec.size() << ":" <<
    vec_description;
  sort_and_output(ptr_vec, outputline.str());
}
 
 
void
test4(vector<int> sort_vec, string vec_description)
{
  ostringstream outputline;
  outputline << "ints - length " << sort_vec.size() << ":" << vec_description;
  sort_and_output(sort_vec, outputline.str());
}

void
test5(vector<int> sort_vec, string vec_description)
{
  vector<int_wrap> moveable_int_vec(sort_vec.size());
  copy(sort_vec.begin(), sort_vec.end(), moveable_int_vec.begin());
  ostringstream outputline;
  outputline << "moveable ints - length " << sort_vec.size() << ":" <<
    vec_description;
  sort_and_output(moveable_int_vec, outputline.str());
}
  
int
main(void)
{
  for(int length = 100000; length < 1000001; length*=10)
  {
    vector<int> sort_vec;
    int val = 1000;
    for(int i = 0; i < length; ++i)
    {
      val = (val * 88888) % 1000003;
      sort_vec.push_back(val);
    }
    test1(1000000/length, sort_vec, "random vector");
    test2(1000000/length, sort_vec, "random vector");
    test3(1000000/length, sort_vec, "random vector");
    
  }
  
  for(int length = 1000000; length < 10000001; length*=10)
  {
    vector<int> sort_vec;
    int val = 1000;
    for(int i = 0; i < length; ++i)
    {
      val = (val * 88888) % 1000003;
      sort_vec.push_back(val);
    }
    test4(sort_vec, "random vector");
    test5(sort_vec, "random vector");
  }
}
