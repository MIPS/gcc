// Represent a location.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_LOCATION_HH
#define GCJX_LOCATION_HH

class location
{
  const char *file;
  short line;
  short column;

  friend std::ostream &operator<< (std::ostream &out, const location &loc);

public:

  location ()
    : file (NULL),
      line (-1),
      column (-1)
  {
  }

  location (const char *f)
    : file (f),
      line (-1),
      column (-1)
  {
  }

  location (const char *f, short c, short l)
    : file (f),
      line (l),
      column (c)
  {
  }

  const char *get_file () const
  {
    return file;
  }

  short get_line () const
  {
    return line;
  }

  short get_column () const
  {
    return column;
  }
};

extern location LOCATION_UNKNOWN;

std::ostream &operator<< (std::ostream &out, const location &loc);

#endif // GCJX_LOCATION_HH
