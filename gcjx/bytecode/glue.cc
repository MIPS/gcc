// Glue between verifier and gcjx.

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

#include <string>

static int
skip_one_type (const char *descriptor, int index)
{
  int ch = descriptor[index++];
  while (ch == '[')
    ch = descriptor[index++];

  if (ch == 'L')
    {
      do
	{
	  ch = descriptor[index++];
	}
      while (ch != ';');
    }

  return index;
}

int
vfy_count_arguments (char *descriptor)
{
  int count = 0;
  // Start after the '('.
  int i = 1;
  while (descriptor[i] != ')')
    {
      i = skip_one_type (descriptor, i);
      ++count;
    }
  return count;
}
