// 2005-11-02  Paolo Carlini  <pcarlini@suse.de>

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

const int  A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const int A1[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const int A2[] = {0, 2, 3, 4, 10, 11, 12, 13, 14, 15};
const int A3[] = {0, 2, 3, 4, 10, 11};
const int  N = sizeof(A)  / sizeof(int);
const int N1 = sizeof(A1) / sizeof(int);
const int N2 = sizeof(A2) / sizeof(int);
const int N3 = sizeof(A3) / sizeof(int);

void
test01()
{
  bool test __attribute__((unused)) = true;

  typedef std::vector<int>   vec_type;
  typedef vec_type::iterator iterator_type;

  vec_type v(A, A + N);

  iterator_type it1 = v.erase(v.begin() + 1);
  VERIFY( it1 == v.begin() + 1 );
  VERIFY( std::equal(v.begin(), v.end(), A1) );
  
  iterator_type it2 = v.erase(v.begin() + 4, v.begin() + 9);
  VERIFY( it2 == v.begin() + 4 );
  VERIFY( std::equal(v.begin(), v.end(), A2) );
  
  iterator_type it3 = v.erase(v.begin() + 6, v.end());
  VERIFY( it3 == v.begin() + 6 );
  VERIFY( std::equal(v.begin(), v.end(), A3) );

  iterator_type it4 = v.erase(v.begin(), v.end());
  VERIFY( it4 == v.begin() );
  VERIFY( v.empty() );
}

void
test02()
{
  bool test __attribute__((unused)) = true;

  typedef std::vector<std::vector<int> >   vec_type;
  typedef vec_type::iterator          iterator_type;

  vec_type v, v1, v2, v3;
  for (int i = 0; i < N; ++i)
    v.push_back(std::vector<int>(1, A[i]));
  for (int i = 0; i < N1; ++i)
    v1.push_back(std::vector<int>(1, A1[i]));
  for (int i = 0; i < N2; ++i)
    v2.push_back(std::vector<int>(1, A2[i]));
  for (int i = 0; i < N3; ++i)
    v3.push_back(std::vector<int>(1, A3[i]));
  
  iterator_type it1 = v.erase(v.begin() + 1);
  VERIFY( it1 == v.begin() + 1 );
  VERIFY( std::equal(v.begin(), v.end(), v1.begin()) );
  
  iterator_type it2 = v.erase(v.begin() + 4, v.begin() + 9);
  VERIFY( it2 == v.begin() + 4 );
  VERIFY( std::equal(v.begin(), v.end(), v2.begin()) );
  
  iterator_type it3 = v.erase(v.begin() + 6, v.end());
  VERIFY( it3 == v.begin() + 6 );
  VERIFY( std::equal(v.begin(), v.end(), v3.begin()) );

  iterator_type it4 = v.erase(v.begin(), v.end());
  VERIFY( it4 == v.begin() );
  VERIFY( v.empty() );
}

int main()
{
  test01();
  test02();
  return 0;
}
