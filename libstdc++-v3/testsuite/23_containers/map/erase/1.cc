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

  typedef map<int, int> Map;
  Map m0;
  Map::iterator iter0;
  pair<Map::iterator, bool> p1, p2, p3;
  typedef Map::value_type mp;

  p1 = m0.insert(mp(0, 1));
  VERIFY( p1.second == true );
  iter0 = m0.erase(p1.first);
  VERIFY( m0.empty() );
  VERIFY( iter0 == m0.end() );

  p1 = m0.insert(mp(1, 2));
  VERIFY( p1.second == true );
  p2 = m0.insert(mp(2, 3));
  VERIFY( p2.second == true );
  iter0 = m0.erase(p1.first);
  VERIFY( iter0 == p2.first );
  VERIFY( iter0 == m0.find(2) );
  VERIFY( (*iter0).second == 3 );
  iter0 = m0.erase(p2.first);
  VERIFY( m0.empty() );
  VERIFY( iter0 == m0.end() );

  p1 = m0.insert(mp(3, 4));
  VERIFY( p1.second == true );
  p2 = m0.insert(mp(4, 5));
  VERIFY( p2.second == true );
  iter0 = m0.erase(p1.first, m0.end());
  VERIFY( m0.empty() );
  VERIFY( iter0 == m0.end() );

  p1 = m0.insert(mp(1, 2));
  VERIFY( p1.second == true );
  p2 = m0.insert(mp(2, 3));
  VERIFY( p2.second == true );
  p3 = m0.insert(mp(3, 4));
  VERIFY( p3.second == true );
  iter0 = m0.erase(p1.first, p3.first);
  VERIFY( iter0 == p3.first );
  VERIFY( iter0 == m0.find(3) );
  VERIFY( (*iter0).second == 4 );
  VERIFY( m0.size() == 1 );
  
  p1 = m0.insert(mp(4, 5));
  VERIFY( p1.second == true );
  p2 = m0.insert(mp(5, 6));
  VERIFY( p2.second == true );
  p3 = m0.insert(mp(6, 7));
  VERIFY( p3.second == true );
  iter0 = m0.erase(p2.first, p3.first);
  VERIFY( iter0 == p3.first );
  VERIFY( iter0 == m0.find(6) );
  VERIFY( (*iter0).second == 7 );
  VERIFY( m0.size() == 3 );
}

int main ()
{
  test01();
  return 0;
}
