/* Copyright (C) 2002-2003 Free Software Foundation, Inc.
   Contributed by Andy Vaught

This file is part of GNU G95.

GNU G95 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU G95 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU G95; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "libgfortran.h"
#include "io.h"

/* endfile.c--  Implement the ENDFILE statement */

void
st_endfile (void)
{
  unit_t *u;

  library_start ();

  u = get_unit (0);
  if (u != NULL)
    {
      current_unit = u;		/* next_record() needs this set */
      if (u->current_record)
	next_record (1);

      struncate (u->s);
      u->endfile = AFTER_ENDFILE;
    }

  library_end ();
}
