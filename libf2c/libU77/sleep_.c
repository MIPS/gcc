/* Copyright (C) 1995 Free Software Foundation, Inc.
This file is part of GNU Fortran libU77 library.

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Library General Public License as published
by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GNU Fortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with GNU Fortran; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#endif
#include "f2c.h"

/* Subroutine */
int
G77_sleep_0 (const integer * seconds)
{
#ifndef _WIN32
  (void) sleep ((unsigned int) *seconds);
#else
  /* w32api Sleep takes duration in millisecs  */
  (void) Sleep (((unsigned int) *seconds) * 1000 );
#endif
  return 0;
}
