// Represent a location.

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

#include <iostream>
#include <stdlib.h>

#include "location.hh"

location LOCATION_UNKNOWN;

std::ostream &
operator<< (std::ostream &out, const location &loc)
{
  if (loc.file != NULL)
    {
      out << loc.file << ":";
      if (loc.line != -1)
	out << loc.line << ":" << loc.column << ":";
      out << " ";
    }
  return out;
}
