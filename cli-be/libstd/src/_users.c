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

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include <__cdefs.h>

#include "__host.h"

#include <unistd.h>
#include <pwd.h>

/* according to man getpwd, it is safe to reuse this structure */
static struct passwd pwdbf;


LIBSTD_LPROTO_IMPL(uid_t, getuid, void)
{
  return LIBSTD_HNAME(getuid)();
}

LIBSTD_LPROTO_IMPL(uid_t, geteuid, void)
{
  return LIBSTD_HNAME(geteuid)();
}

LIBSTD_LPROTO_IMPL(struct passwd *, getpwuid, uid_t uid)
{
  if (LIBSTD_HNAME(getpwuid)(uid, 
    &(pwdbf.pw_name),
    &(pwdbf.pw_passwd),
    &(pwdbf.pw_gid),
    &(pwdbf.pw_gecos),
    &(pwdbf.pw_dir),
    &(pwdbf.pw_shell)) == -1)
    return NULL;

  pwdbf.pw_uid = uid;

  return &pwdbf; 
}

LIBSTD_LPROTO_IMPL(struct passwd *, getpwnam, char * uname)
{
  if (LIBSTD_HNAME(getpwnam)(uname, 
    &(pwdbf.pw_passwd),
    &(pwdbf.pw_uid),
    &(pwdbf.pw_gid),
    &(pwdbf.pw_gecos),
    &(pwdbf.pw_dir),
    &(pwdbf.pw_shell)) == -1)
    return NULL;

  pwdbf.pw_name = uname;

  return &pwdbf;
}




