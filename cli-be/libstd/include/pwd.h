/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef __PWD_H
#define __PWD_H

#include <__cdefs.h>


#include <sys/types.h>
#include <stddef.h>

__BEGIN_EXTERN_C

/* The passwd structure.  */
struct passwd
{
  char *pw_name;      /* Username.  */
  char *pw_passwd;    /* Password.  */
  uid_t pw_uid;	      /* User ID.  */
  gid_t pw_gid;       /* Group ID.  */
  char *pw_gecos;     /* Real name.  */
  char *pw_dir;       /* Home directory.  */
  char *pw_shell;     /* Shell program.  */
};

/* Implemented in src/_users.c */
LIBSTD_LPROTO(struct passwd *, getpwuid, uid_t);
LIBSTD_LPROTO(struct passwd *, getpwnam, char *);

__END_EXTERN_C

#endif /*__PWD_H*/

