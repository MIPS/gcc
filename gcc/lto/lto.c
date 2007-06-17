/* Top-level LTO routines.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "opts.h"
#include "lto.h"

void
lto_file_init (lto_file *file, const char *filename)
{
  file->debug_info = NULL;
  file->debug_info_length = 0;
  file->debug_abbrev = NULL;
  file->debug_abbrev_length = 0;
  file->filename = filename;
}

bool
lto_file_read (lto_file *file)
{
  return true;
}

void
lto_main (int debug_p ATTRIBUTE_UNUSED)
{
  unsigned i;

  /* Read all of the object files specified on the command line.  */
  for (i = 0; i < num_in_fnames; ++i)
    {
      lto_file *file;

      file = lto_elf_file_open (in_fnames[i]);
      if (!file)
	break;
      gcc_assert (file->debug_info);
      gcc_assert (file->debug_abbrev);
      if (!lto_file_read (file))
	return;
      lto_elf_file_close (file);
    }
}
