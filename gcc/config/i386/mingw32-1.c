/* This replaces the use of stat and struct stat.st_ino to determine if
   files are different in gcc.c (do_spec_1) handling of --save-temps
   switch.
   Contributed by Danny Smith (dannysmith@users.sourceforge.net)
   Copyright 2003 Free Software Foundation, Inc.

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* Return non-zero if src and dst filenames do not refer to same files. */

int w32_file_id_cmp (const char *, const char *);

int
w32_file_id_cmp (src, dst)
    const char * src;
    const char * dst;
{
  char fullpath_src[MAX_PATH];
  char fullpath_dst[MAX_PATH];
  char* pfilename;

 /* Just compare full pathnames, without regard to case. */
  GetFullPathName (src,MAX_PATH,fullpath_src,&pfilename);
  GetFullPathName (dst,MAX_PATH,fullpath_dst,&pfilename);
  return  (lstrcmpi (fullpath_src, fullpath_dst));
}
