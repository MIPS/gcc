/* Copyright (C) 2014 Free Software Foundation, Inc.
   Contributed by Mentor Embedded.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

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

#ifndef GOMP_CONSTANTS_H
#define GOMP_CONSTANTS_H 1

/* Enumerated variable mapping types used to communicate between GCC and
   libgomp.  These values are used for both OpenMP and OpenACC.  */

#define _GOMP_MAP_FLAG_SPECIAL		(1 << 2)
#define _GOMP_MAP_FLAG_FORCE		(1 << 3)

#define GOMP_MAP_ALLOC			0x00
#define GOMP_MAP_ALLOC_TO		0x01
#define GOMP_MAP_ALLOC_FROM		0x02
#define GOMP_MAP_ALLOC_TOFROM		0x03
#define GOMP_MAP_POINTER		0x04
#define GOMP_MAP_TO_PSET		0x05
#define GOMP_MAP_FORCE_ALLOC		0x08
#define GOMP_MAP_FORCE_TO		0x09
#define GOMP_MAP_FORCE_FROM		0x0a
#define GOMP_MAP_FORCE_TOFROM		0x0b
#define GOMP_MAP_FORCE_PRESENT		0x0c
#define GOMP_MAP_FORCE_DEALLOC		0x0d
#define GOMP_MAP_FORCE_DEVICEPTR	0x0e

#define GOMP_MAP_COPYTO_P(X) \
  ((X) == GOMP_MAP_ALLOC_TO || (X) == GOMP_MAP_FORCE_TO)

#define GOMP_MAP_COPYFROM_P(X) \
  ((X) == GOMP_MAP_ALLOC_FROM || (X) == GOMP_MAP_FORCE_FROM)

#define GOMP_MAP_TOFROM_P(X) \
  ((X) == GOMP_MAP_ALLOC_TOFROM || (X) == GOMP_MAP_FORCE_TOFROM)

#define GOMP_MAP_POINTER_P(X) \
  ((X) == GOMP_MAP_POINTER)

#define GOMP_IF_CLAUSE_FALSE		-2

/* Canonical list of target type codes for OpenMP/OpenACC.  */
#define GOMP_TARGET_NONE		0
#define GOMP_TARGET_HOST		2
#define GOMP_TARGET_HOST_NONSHM		3
#define GOMP_TARGET_NOT_HOST		4
#define GOMP_TARGET_NVIDIA_PTX		5
#define GOMP_TARGET_INTEL_MIC		6

#endif
