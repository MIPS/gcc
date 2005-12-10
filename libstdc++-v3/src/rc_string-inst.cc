// Components for manipulating sequences of characters -*- C++ -*-

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

//
// ISO C++ 14882:
//

#include <bits/c++config.h>
#include <memory> 	// For allocator.
#include <bits/char_traits.h>
#include <ext/rc_string.h>

// Instantiation configuration.
#ifndef C
# define C char
#endif

namespace __gnu_cxx
{
  using std::allocator;

  template class __rc_string<C, std::char_traits<C>, allocator<C> >;

  typedef __rc_string<C, std::char_traits<C>, allocator<C> > RS;

  template
    RS::__rc_string(C*, C*, const allocator<C>&);

  template
    RS::__rc_string(const C*, const C*, const allocator<C>&);

  template 
    RS::__rc_string(RS::iterator, RS::iterator, const allocator<C>&);

  template 
    C* 
    RS::_S_construct(RS::iterator, RS::iterator, 
		     const allocator<C>&, std::forward_iterator_tag);

  template
    C*
    RS::_S_construct(C*, C*, const allocator<C>&,
		     std::forward_iterator_tag);

  template
    C*
    RS::_S_construct(const C*, const C*, const allocator<C>&,
		     std::forward_iterator_tag);
} // namespace __gnu_cxx
