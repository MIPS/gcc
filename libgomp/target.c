/* Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek <jakub@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This file handles the maintainence of threads in response to team
   creation and termination.  */

#include "libgomp.h"
#include <stdlib.h>
#include <string.h>

static int
resolve_device (int device)
{
  return -1;
}

/* Called when encountering a target directive.  If DEVICE
   is -1, it means use device-var ICV.  If it is -2 (or any other value
   larger than last available hw device, use host fallback.
   FN is address of host code, FNNAME corresponding name to lookup
   in the target code.  HOSTADDRS, SIZES and KINDS are arrays
   with MAPNUM entries, with addresses of the host objects,
   sizes of the host objects (resp. for pointer kind pointer bias
   and assumed sizeof (void *) size) and kinds.  */

void
GOMP_target (int device, void (*fn) (void *), const char *fnname,
	     size_t mapnum, void **hostaddrs, size_t *sizes,
	     unsigned char *kinds)
{
  if (resolve_device (device) == -1)
    {
      fn (hostaddrs);
      return;
    }
}

void
GOMP_target_data (int device, size_t mapnum, void **hostaddrs, size_t *sizes,
		  unsigned char *kinds)
{
  if (resolve_device (device) == -1)
    return;
}

void
GOMP_target_end_data (void)
{
}

void
GOMP_target_update (int device, size_t mapnum, void **hostaddrs, size_t *sizes,
		    unsigned char *kinds)
{
  if (resolve_device (device) == -1)
    return;
}

void
GOMP_teams (unsigned int num_teams, unsigned int thread_limit)
{
}
