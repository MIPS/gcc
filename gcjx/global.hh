// All global state.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_GLOBAL_HH
#define GCJX_GLOBAL_HH

class compiler;

class global_state : protected concurrence::thread_local<compiler *>
{
public:

  global_state ()
  {
    set (NULL);
  }

  void set_compiler (compiler *tc)
  {
    set (tc);
  }

  // Return the compiler currently in use.  For the time being there
  // can be only one.
  compiler *get_compiler ()
  {
    return get ();
  }
};

extern global_state *global;

#endif // GCJX_GLOBAL_HH
