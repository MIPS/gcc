// Copyright (C) 2006 Free Software Foundation, Inc.
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

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#include <string>
#include <bits/exception_string.h>

_GLIBCXX_BEGIN_NAMESPACE(std)

  void
  __exception_string::_Rep::
  _M_destroy() throw()
  {
    std::allocator<char>().deallocate(reinterpret_cast<char*>(this),
				      sizeof(_Rep) + _M_size);
  }

  __exception_string::
  __exception_string(const string& __str)
  {
    const size_t __size = __str.size() + 1;

    void* __place = std::allocator<char>().allocate(sizeof(_Rep) + __size);
    _Rep* __r = new (__place) _Rep;

    std::memcpy(__r->_M_refdata(), __str.c_str(), __size);
    __r->_M_size = __size;
    __r->_M_refcount = 0;

    _M_data = __r->_M_refdata();
  }

  __exception_string&
  __exception_string::
  operator=(const __exception_string& __ex_str)
  {
    if (_M_rep() != __ex_str._M_rep())
      {
	char* __tmp = __ex_str._M_rep()->_M_refcopy();
	_M_rep()->_M_dispose();
	_M_data = __tmp;
      }
    return *this;
  }

_GLIBCXX_END_NAMESPACE
