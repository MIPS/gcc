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

#ifndef _GOMP_H
#define _GOMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* FIXME.  Is this really needed?  Shouldn't we assume that this is
   always compiled with C99 compilers?  Specifically, is this compiled
   with anything other than GCC?  */
#ifndef HAVE_BOOLEAN
typedef int boolean;
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE    1
#endif

/* Upper bound on the length of the environment variable buffer.  */
#define	ENV_BUFFER_SIZE 25


/* Error codes.  Error strings are defined in gomp.c:gomp_err_strings.  */
enum GOMP_ERR
{
  GOMP_ERR_MIN = 100,

  GOMP_ERR_ENV_SCHEDULE,
  GOMP_ERR_ENV_DYNAMIC,
  GOMP_ERR_ENV_NESTED,
  GOMP_ERR_NUM_THREAD_POSITIVE,

  GOMP_ERR_MAX
};

/* FIXME.  DOCUMENT.  */
enum schedule_kind
{
  GOMP_DYNAMIC,
  GOMP_GUIDED,
  GOMP_RUNTIME,
  GOMP_STATIC
};


/* Structure containing the current state.  */
struct t_gomp_state 
{
/* FIXME.  DOCUMENT FIELDS.  */
  enum schedule_kind schedule;
  unsigned int schedule_chunksize;
  unsigned int default_number_of_threads;
  boolean dynamic_adjustment;
  boolean nested_parallism;
};

/* DEFAULT STATE */
#define GOMP_DEFAULT_SCHEDULE GOMP_DYNAMIC
/* FIXME.  DOCUMENT.  */
#define GOMP_DEFAULT_SCHEDULE_CHUNKSIZE 1
/* FIXME.  DOCUMENT.  */
#define GOMP_DEFAULT_NUMBER_THREADS 1
/* FIXME.  DOCUMENT.  */
#define GOMP_DEFAULT_DYNAMIC_ADJUSTMENT TRUE
/* FIXME.  DOCUMENT.  */
#define GOMP_DEFAULT_NESTED_PARALLISM FALSE


extern struct t_gomp_state gomp_state;

extern void gomp_strip (char *s);
extern int gomp_init (void);
extern void gomp_print_state (FILE * out, char *indent);

extern int gomp_status;
extern int gomp_is_debug (void);
extern void gomp_set_debug (int debug);
extern char *gomp_get_errstr (int err);

#ifdef __cplusplus
}
#endif

#endif /* _GOMP_H */
