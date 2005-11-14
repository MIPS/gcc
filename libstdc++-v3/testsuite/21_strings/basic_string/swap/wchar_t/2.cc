// 2005-11-13  Paolo Carlini  <pcarlini@suse.de>

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

// 21.3.5.8 basic_string::swap

#include <string>
#include <testsuite_hooks.h>
#include <testsuite_allocator.h>

// ... some more using uneq_allocator as a non-empty allocator.
void
test01()
{
  bool test __attribute__((unused)) = true;
  using namespace std;

  typedef __gnu_test::uneq_allocator<wchar_t> my_alloc;
  typedef basic_string<wchar_t, char_traits<wchar_t>, my_alloc > my_wstring;

  const wchar_t* title00 = L"";
  const wchar_t* title01 = L"Rivers of sand";
  const wchar_t* title02 = L"Concret PH";
  const wchar_t* title03 = L"Sonatas and Interludes for Prepared Piano";
  const wchar_t* title04 = L"never as tired as when i'm waking up";

  my_wstring::size_type size01, size02;

  my_alloc alloc01(1);

  my_wstring str01(title00, alloc01);
  size01 = str01.size();
  my_wstring str02(title00, alloc01);
  size02 = str02.size();
  
  str01.swap(str02);
  VERIFY( str01.size() == size02 );
  VERIFY( str01 == title00 );
  VERIFY( str02.size() == size01 );
  VERIFY( str02 == title00 );

  my_wstring str03(title00, alloc01);
  size01 = str03.size();
  my_wstring str04(title02, alloc01);
  size02 = str04.size();
  
  str03.swap(str04);
  VERIFY( str03.size() == size02 );
  VERIFY( str03 == title02 );
  VERIFY( str04.size() == size01 );
  VERIFY( str04 == title00 );
  
  my_wstring str05(title01, alloc01);
  size01 = str05.size();
  my_wstring str06(title02, alloc01);
  size02 = str06.size();
  
  str05.swap(str06);
  VERIFY( str05.size() == size02 );
  VERIFY( str05 == title02 );
  VERIFY( str06.size() == size01 );
  VERIFY( str06 == title01 );

  my_wstring str07(title01, alloc01);
  size01 = str07.size();
  my_wstring str08(title03, alloc01);
  size02 = str08.size();

  str07.swap(str08);
  VERIFY( str07.size() == size02 );
  VERIFY( str07 == title03 );
  VERIFY( str08.size() == size01 );
  VERIFY( str08 == title01 );

  my_wstring str09(title03, alloc01);
  size01 = str09.size();
  my_wstring str10(title04, alloc01);
  size02 = str10.size();

  str09.swap(str10);
  VERIFY( str09.size() == size02 );
  VERIFY( str09 == title04 );
  VERIFY( str10.size() == size01 );
  VERIFY( str10 == title03 );

  my_wstring str11(title04, alloc01);
  size01 = str11.size();
  my_wstring str12(title01, alloc01);
  size02 = str12.size();

  str11.swap(str12);
  VERIFY( str11.size() == size02 );
  VERIFY( str11 == title01 );
  VERIFY( str12.size() == size01 );
  VERIFY( str12 == title04 );

  my_wstring str13(title03, alloc01);
  size01 = str13.size();
  my_wstring str14(title03, alloc01);
  size02 = str14.size();

  str13.swap(str14);
  VERIFY( str13.size() == size02 );
  VERIFY( str13 == title03 );
  VERIFY( str14.size() == size01 );
  VERIFY( str14 == title03 );
}

int main()
{ 
  test01();
  return 0;
}
