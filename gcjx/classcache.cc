// Class cache implementation.

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

#include "typedefs.hh"

void
class_cache::find_class ()
{
  concurrence::sync::lock_sentinel sync (mutex);
  // We could require the compiler as an argument to the constructor.
  cache = global->get_compiler ()->find_class (name);
  if (! cache)
    throw type_not_found_error (LOCATION_UNKNOWN,
				"required type %1 not found; "
				"check your class path")
      % name;
}
