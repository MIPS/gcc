/* GOMP - GNU OpenMP runtime library.
   Copyright (C) 2004
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */
/* Function declarations for libgomp (GNU OpenMP).

   Copyright 2003 Free Software Foundation, Inc.
   
   Note - certain prototypes declared in this header file are for
   functions whoes implementation copyright does not belong to the
   FSF.  Those prototypes are present in this file for reference
   purposes only and their presence in this file should not construed
   as an indication of ownership by the FSF of the implementation of
   those functions in any way or form whatsoever.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef LIBGOMP_H
#define LIBGOMP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OMP_NEST_LOCK_T
/* FIXME.  DOCUMENT.  */
typedef struct
{
  int owner;
  int count;
} omp_nest_lock_t;
#endif

#ifndef OMP_LOCK_T
typedef int omp_lock_t;
#endif

extern void omp_set_num_threads (int);
extern int omp_get_num_threads (void);
extern int omp_get_max_threads (void);
extern int omp_get_thread_num (void);
extern int omp_get_num_procs (void);
extern void omp_set_dynamic (int);
extern int omp_get_dynamic (void);
extern int omp_in_parallel (void);
extern void omp_set_nested (int);
extern int omp_get_nested (void);
extern void omp_init_lock (omp_lock_t *);
extern void omp_destroy_lock (omp_lock_t *);
extern void omp_set_lock (omp_lock_t *);
extern void omp_unset_lock (omp_lock_t *);
extern int omp_test_lock (omp_lock_t *);
extern void omp_init_nest_lock (omp_nest_lock_t *);
extern void omp_destroy_nest_lock (omp_nest_lock_t *);
extern void omp_set_nest_lock (omp_nest_lock_t *);
extern void omp_unset_nest_lock (omp_nest_lock_t *);
extern int omp_test_nest_lock (omp_nest_lock_t *);
extern double omp_get_wtime (void);
extern double omp_get_wtick (void);

#ifdef __cplusplus
}
#endif

#endif /* LIBGOMP_H */ 
