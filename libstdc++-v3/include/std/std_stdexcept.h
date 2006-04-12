// Standard exception classes  -*- C++ -*-

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
// Free Software Foundation, Inc.
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

//
// ISO C++ 19.1  Exception classes
//

/** @file stdexcept
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_STDEXCEPT
#define _GLIBCXX_STDEXCEPT 1

#pragma GCC system_header

#include <exception>
#include <string>
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


  /** Logic errors represent problems in the internal logic of a program;
   *  in theory, these are preventable, and even detectable before the
   *  program runs (e.g., violations of class invariants).
   *  @brief One of two subclasses of exception.
   */
  class logic_error : public exception 
  {
    __exception_string _M_msg;

  public:
    /** Takes a character string describing the error.  */
    explicit 
    logic_error(const string&  __arg);

    virtual 
    ~logic_error() throw();

    /** Returns a C-style character string describing the general cause of
     *  the current error (the same string passed to the ctor).  */
    virtual const char* 
    what() const throw();
  };

  /** Thrown by the library, or by you, to report domain errors (domain in
   *  the mathmatical sense).  */
  class domain_error : public logic_error 
  {
  public:
    explicit domain_error(const string&  __arg);
  };

  /** Thrown to report invalid arguments to functions.  */
  class invalid_argument : public logic_error 
  {
  public:
    explicit invalid_argument(const string&  __arg);
  };

  /** Thrown when an object is constructed that would exceed its maximum
   *  permitted size (e.g., a basic_string instance).  */
  class length_error : public logic_error 
  {
  public:
    explicit length_error(const string&  __arg);
  };

  /** This represents an argument whose value is not within the expected
   *  range (e.g., boundary checks in basic_string).  */
  class out_of_range : public logic_error 
  {
  public:
    explicit out_of_range(const string&  __arg);
  };

  /** Runtime errors represent problems outside the scope of a program;
   *  they cannot be easily predicted and can generally only be caught as
   *  the program executes.
   *  @brief One of two subclasses of exception.
   */
  class runtime_error : public exception 
  {
    __exception_string _M_msg;

  public:
    /** Takes a character string describing the error.  */
    explicit 
    runtime_error(const string&  __arg);

    virtual 
    ~runtime_error() throw();

    /** Returns a C-style character string describing the general cause of
     *  the current error (the same string passed to the ctor).  */
    virtual const char* 
    what() const throw();
  };

  /** Thrown to indicate range errors in internal computations.  */
  class range_error : public runtime_error 
  {
  public:
    explicit range_error(const string&  __arg);
  };

  /** Thrown to indicate arithmetic overflow.  */
  class overflow_error : public runtime_error 
  {
  public:
    explicit overflow_error(const string&  __arg);
  };

  /** Thrown to indicate arithmetic underflow.  */
  class underflow_error : public runtime_error 
  {
  public:
    explicit underflow_error(const string&  __arg);
  };

_GLIBCXX_END_NAMESPACE

#endif /* _GLIBCXX_STDEXCEPT */
