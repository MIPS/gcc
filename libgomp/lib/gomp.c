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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libgomp.h"
#include "gomp.h"

#ifdef __cplusplus
extern	"C"	{ 
#endif

/* Structure containing the current state.  */
struct t_gomp_state gomp_state; 

/* FIXME.  DOCUMENT.  */
int gomp_status = 0;

/* FIXME.  DOCUMENT.  */
static int gomp_debug = 0;

/* Error messages, keep in sync with GOMP_ERR in gomp.h.  */
static char *gomp_err_strings[] = {
	"unknown schedule-kind",
	"unknown dynamic",
	"unknown nested",
	"num_thread must be positive",
	NULL
};


/* FIXME DOCUMENT.  */

void
gomp_strip (char *s)
{
  char tmp[ENV_BUFFER_SIZE]; 
  int i, start, stop;

  if (s == NULL)
    return;

  strncpy (tmp, s, ENV_BUFFER_SIZE - 1);
  tmp[ENV_BUFFER_SIZE - 1] = '\0';

  for (start = 0; start < strlen (tmp); start++)
    if (tmp[start] != ' ' && tmp[start] != '\t')
      break;

  for (stop = strlen(tmp) - 1; stop > start; stop--)
    if (tmp[stop] != ' ' && tmp[stop] != '\t')
      break;

  for (i = start; i <= stop; i++)
    s[i - start] = tmp[i];

  s[i - start] = '\0';
}


/* FIXME DOCUMENT.  */

int
gomp_init (void)
{
  int error = 0;
  char *schedule = NULL;

  gomp_state.schedule = GOMP_DEFAULT_SCHEDULE;
  gomp_state.schedule_chunksize = GOMP_DEFAULT_SCHEDULE_CHUNKSIZE;
  gomp_state.default_number_of_threads = GOMP_DEFAULT_NUMBER_THREADS;
  gomp_state.dynamic_adjustment = GOMP_DEFAULT_DYNAMIC_ADJUSTMENT;
  gomp_state.nested_parallism = GOMP_DEFAULT_NESTED_PARALLISM;

  if ((schedule = getenv ("OMP_SCHEDULE")) != NULL)
    {
      char tmp[ENV_BUFFER_SIZE];
      char *pchunk_size = NULL;
      char *schedule_kind = NULL;
      int chunk_size = 0;

      strncpy (tmp, schedule, ENV_BUFFER_SIZE - 1);
      tmp[ENV_BUFFER_SIZE - 1] = '\0';
      schedule_kind = strtok (tmp, ",");

      if ((pchunk_size = strtok (NULL, ",")) != NULL)
	gomp_strip (pchunk_size);

      gomp_strip (schedule_kind);

      if (pchunk_size)
	chunk_size = atoi (pchunk_size);

      if (!strcmp (schedule_kind, "static"))
	gomp_state.schedule = GOMP_STATIC;
      else if (!strcmp (schedule_kind, "dynamic"))
	gomp_state.schedule = GOMP_DYNAMIC;
      else if (!strcmp (schedule_kind, "guided"))
	gomp_state.schedule = GOMP_GUIDED;
      else if (!strcmp (schedule_kind, "runtime"))
	gomp_state.schedule = GOMP_RUNTIME;
      else
	{
	  error = GOMP_ERR_ENV_SCHEDULE;
	  if (gomp_is_debug ())
	    fprintf (stderr, "gomp - %s\n", gomp_get_errstr (error));
	}

      if (chunk_size > 0)
	gomp_state.schedule_chunksize = (unsigned int) chunk_size;
    }

  {
    char *pnum_threads = NULL;

    if ((pnum_threads = getenv ("OMP_NUM_THREADS")) != NULL)
      {
	int num_threads = 0;
	num_threads = atoi (pnum_threads);
	if (num_threads > 0)
	  gomp_state.default_number_of_threads = (unsigned int) num_threads;
      }
  }

  {
    char *omp_dynamic = NULL;

    if ((omp_dynamic = getenv ("OMP_DYNAMIC")) != NULL)
      {
	gomp_strip (omp_dynamic);
	if (!strcmp (omp_dynamic, "TRUE"))
	  gomp_state.dynamic_adjustment = TRUE;

	else if (!strcmp (omp_dynamic, "FALSE"))
	  gomp_state.dynamic_adjustment = FALSE;

	else
	  {
	    error = GOMP_ERR_ENV_DYNAMIC;
	    if (gomp_is_debug ())
	      fprintf (stderr, "gomp - %s\n", gomp_get_errstr (error));
	  }
      }
  }

  {
    char *omp_nested = NULL;

    if ((omp_nested = getenv ("OMP_NESTED")) != NULL)
      {
	gomp_strip (omp_nested);

	if (!strcmp (omp_nested, "TRUE"))
	  gomp_state.nested_parallism = TRUE;
	else if (!strcmp (omp_nested, "FALSE"))
	  gomp_state.nested_parallism = FALSE;
	else
	  {
	    error = GOMP_ERR_ENV_NESTED;
	    if (gomp_is_debug ())
	      fprintf (stderr, "gomp - %s\n", gomp_get_errstr (error));
	  }
      }
  }

  return error;
}


/* FIXME DOCUMENT.  */

void
gomp_print_state (FILE *out, char *indent)
{
  /* Prints each element of the GOMP state structure to the specified
     stream.  Each element will be indented by the 'indent' string.  */
  char *ind;
  ind = indent ? indent : "";
  char *tmp = NULL;

  switch (gomp_state.schedule)
    {
    case GOMP_DYNAMIC:
      tmp = "DYNAMIC";
      break;
    case GOMP_STATIC:
      tmp = "STATIC";
      break;
    case GOMP_RUNTIME:
      tmp = "RUNTIME";
      break;
    case GOMP_GUIDED:
      tmp = "GUIDED";
      break;
    }

  fprintf (out, "%sschedule: %s\n", ind, tmp);
  fprintf (out, "%sschedule_chunksize: %d\n", ind,
	   gomp_state.schedule_chunksize);
  fprintf (out, "%sdefault_number_of_threads: %d\n", ind,
	   gomp_state.default_number_of_threads);
  fprintf (out, "%sdynamic_adjustment: %d\n", ind,
	   gomp_state.dynamic_adjustment);
  fprintf (out, "%snested_parallism: %d\n", ind, gomp_state.nested_parallism);
}


/* FIXME.  DOCUMENT.  */

void
gomp_set_debug (int flag) 
{
  gomp_debug = flag;
}


/* FIXME.  DOCUMENT.  */

int
gomp_is_debug (void) 
{
  return gomp_debug;
}


/* FIXME.  DOCUMENT.  */

char *
gomp_get_errstr (int err) 
{
  if ( err == 0 )
    return "success";
  else if ( err <= GOMP_ERR_MIN || err >= GOMP_ERR_MAX )
    return "unkown error";
  else
    return gomp_err_strings[err - GOMP_ERR_MIN - 1];  
}

#ifdef __cplusplus
}
#endif
