/* This replaces the use of stat and struct stat.st_ino to determine if
   files are different in gcc.c (do_spec_1) handling of --save-temps
   switch.
   Contributed by Danny Smith (dannysmith@users.sourceforge.net)
   Copyright 2002 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include <string.h>
#include <stdlib.h>

/* Return non-zero if src and dst filenames do not refer to same files. */

int
w32_file_id_cmp (src, dst)
    const char * src;
    const char * dst;
{
  char fullpath_src[_MAX_PATH];
  char fullpath_dst[_MAX_PATH];
 
 /* Just compare full pathnames.  */
  _fullpath (fullpath_src, src, _MAX_PATH);
  _fullpath (fullpath_dst, dst, _MAX_PATH);    
  return  (_stricmp (fullpath_src, fullpath_dst));
}
