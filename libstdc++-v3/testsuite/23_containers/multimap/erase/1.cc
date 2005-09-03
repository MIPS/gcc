// 2005-09-02  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.
//
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#include <map>
#include <testsuite_hooks.h>

// libstdc++/20787 (DR 130)
void test01()
{
  bool test __attribute__((unused)) = true;
  using namespace std;
  
  typedef multimap<int, int> Multimap;
  Multimap mm0;
  Multimap::iterator iter0, iter1, iter2, iter3;
  typedef Multimap::value_type mmp;

  iter1 = mm0.insert(mmp(0, 1));
  iter0 = mm0.erase(iter1);
  VERIFY( mm0.empty() );
  VERIFY( iter0 == mm0.end() );

  iter1 = mm0.insert(mmp(1, 2));
  iter2 = mm0.insert(mmp(2, 3));
  iter0 = mm0.erase(iter1);
  VERIFY( iter0 == iter2 );
  VERIFY( iter0 == mm0.find(2) );
  VERIFY( (*iter0).second == 3 );
  iter0 = mm0.erase(iter2);
  VERIFY( mm0.empty() );
  VERIFY( iter0 == mm0.end() );

  iter1 = mm0.insert(mmp(3, 4));
  iter2 = mm0.insert(mmp(4, 5));
  iter0 = mm0.erase(iter1, mm0.end());
  VERIFY( mm0.empty() );
  VERIFY( iter0 == mm0.end() );

  iter1 = mm0.insert(mmp(1, 2));
  iter2 = mm0.insert(mmp(2, 3));
  iter3 = mm0.insert(mmp(3, 4));
  iter0 = mm0.erase(iter1, iter3);
  VERIFY( iter0 == iter3 );
  VERIFY( iter0 == mm0.find(3) );
  VERIFY( (*iter0).second == 4 );
  VERIFY( mm0.size() == 1 );
  
  iter1 = mm0.insert(mmp(4, 5));
  iter2 = mm0.insert(mmp(5, 6));
  iter3 = mm0.insert(mmp(6, 7));
  iter0 = mm0.erase(iter2, iter3);
  VERIFY( iter0 == iter3 );
  VERIFY( iter0 == mm0.find(6) );
  VERIFY( (*iter0).second == 7 );
  VERIFY( mm0.size() == 3 );
}

int main ()
{
  test01();
  return 0;
}
