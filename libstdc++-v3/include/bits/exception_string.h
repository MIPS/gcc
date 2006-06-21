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

/** @file exception_string.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _GLIBCXX_EXCEPTION_STRING_H
#define _GLIBCXX_EXCEPTION_STRING_H 1

#include <bits/atomicity.h>

_GLIBCXX_BEGIN_NAMESPACE(std)

  /** Reference-counted "mini-string" for internal use in the std exception
   *  classes, needed due to the no-throw requirements for copy-constructor
   *  and assignment operator, not fulfilled by sso_string.  */
  class __exception_string
  {
    struct _Rep
    {
      size_t	    _M_size;
      _Atomic_word  _M_refcount;

      void
      _M_destroy() throw();

      void
      _M_dispose() throw()
      {
	if (__gnu_cxx::__exchange_and_add(&_M_refcount, -1) <= 0)
	  _M_destroy();
      }

      char*
      _M_refdata() throw()
      { return reinterpret_cast<char*>(this + 1); }

      char*
      _M_refcopy() throw()
      {
	__gnu_cxx::__atomic_add(&_M_refcount, 1);
	return _M_refdata();
      }
    };

    char* _M_data;

    _Rep*
    _M_rep() const throw()
    { return &((reinterpret_cast<_Rep*>(_M_data))[-1]); }

  public:
    __exception_string(const __exception_string& __ex_str)
    : _M_data(__ex_str._M_rep()->_M_refcopy()) { }

    __exception_string(const string& __str);

    ~__exception_string()
    { _M_rep()->_M_dispose(); }

    __exception_string&
    operator=(const __exception_string& __ex_str);

    const char*
    c_str() const
    { return _M_data; }
  };

_GLIBCXX_END_NAMESPACE

#endif /* _GLIBCXX_EXCEPTION_STRING_H */
