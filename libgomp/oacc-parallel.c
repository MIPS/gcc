/* Copyright (C) 2013-2014 Free Software Foundation, Inc.

   Contributed by Thomas Schwinge <thomas@codesourcery.com>.

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

/* This file handles OpenACC constructs.  */

#include "libgomp.h"
#include "libgomp_g.h"

void
GOACC_parallel (int device, void (*fn) (void *), const void *openmp_target,
		size_t mapnum, void **hostaddrs, size_t *sizes,
		unsigned short *kinds,
		int num_gangs, int num_workers, int vector_length)
{
  unsigned char kinds_[mapnum];
  size_t i;

  /* TODO.  Eventually, we'll be interpreting all mapping kinds according to
     the OpenACC semantics; for now we're re-using what is implemented for
     OpenMP.  */
  for (i = 0; i < mapnum; ++i)
    {
      unsigned char kind = kinds[i];
      unsigned char align = kinds[i] >> 8;
      if (kind > 4)
	gomp_fatal ("memory mapping kind %x for %zd is not yet supported",
		    kind, i);

      kinds_[i] = kind | align << 3;
    }
  if (num_gangs != 1)
    gomp_fatal ("num_gangs (%d) different from one is not yet supported",
		num_gangs);
  if (num_workers != 1)
    gomp_fatal ("num_workers (%d) different from one is not yet supported",
		num_workers);
  if (vector_length != 1)
    gomp_fatal ("vector_length (%d) different from one is not yet supported",
		vector_length);

  GOMP_target (device, fn, openmp_target, mapnum, hostaddrs, sizes, kinds_);
}


void
GOACC_data_start (int device, const void *openmp_target, size_t mapnum,
		  void **hostaddrs, size_t *sizes, unsigned short *kinds)
{
  unsigned char kinds_[mapnum];
  size_t i;

  /* TODO.  Eventually, we'll be interpreting all mapping kinds according to
     the OpenACC semantics; for now we're re-using what is implemented for
     OpenMP.  */
  for (i = 0; i < mapnum; ++i)
    {
      unsigned char kind = kinds[i];
      unsigned char align = kinds[i] >> 8;
      if (kind > 4)
	gomp_fatal ("memory mapping kind %x for %zd is not yet supported",
		    kind, i);

      kinds_[i] = kind | align << 3;
    }
  GOMP_target_data (device, openmp_target, mapnum, hostaddrs, sizes, kinds_);
}

void
GOACC_data_end (void)
{
  GOMP_target_end_data ();
}


void
GOACC_kernels (int device, void (*fn) (void *), const void *openmp_target,
	       size_t mapnum, void **hostaddrs, size_t *sizes,
	       unsigned short *kinds,
	       int num_gangs, int num_workers, int vector_length)
{
  /* TODO.  */
  GOACC_parallel (device, fn, openmp_target, mapnum, hostaddrs, sizes, kinds,
		  num_gangs, num_workers, vector_length);
}


void
GOACC_update (int device, const void *openmp_target, size_t mapnum,
	      void **hostaddrs, size_t *sizes, unsigned short *kinds)
{
  unsigned char kinds_[mapnum];
  size_t i;

  /* TODO.  Eventually, we'll be interpreting all mapping kinds according to
     the OpenACC semantics; for now we're re-using what is implemented for
     OpenMP.  */
  for (i = 0; i < mapnum; ++i)
    {
      unsigned char kind = kinds[i];
      unsigned char align = kinds[i] >> 8;
      if (kind > 4)
	gomp_fatal ("memory mapping kind %x for %zd is not yet supported",
		    kind, i);

      kinds_[i] = kind | align << 3;
    }
  GOMP_target_update (device, openmp_target, mapnum, hostaddrs, sizes, kinds_);
}
