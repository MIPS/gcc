/* Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License 
   along with libgomp; see the file COPYING.LIB.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files, some
   of which are compiled with GCC, to produce an executable, this library
   does not by itself cause the resulting executable to be covered by the
   GNU General Public License.  This exception does not however invalidate
   any other reasons why the executable file might be covered by the GNU
   General Public License.  */

/* This file handles the CRITICAL construct.  */

#include "libgomp.h"


static gomp_mutex_t default_lock;

void
GOMP_critical_start (void)
{
  gomp_mutex_lock (&default_lock);
}

void
GOMP_critical_end (void)
{
  gomp_mutex_unlock (&default_lock);
}

#ifndef HAVE_SYNC_BUILTINS
static gomp_mutex_t create_lock_lock;
#endif

void
GOMP_critical_name_start (gomp_mutex_t **pplock)
{
  gomp_mutex_t *plock = *pplock;

  if (plock == NULL)
    {
#ifdef HAVE_SYNC_BUILTINS
      gomp_mutex_t *nlock = gomp_malloc (sizeof (gomp_mutex_t));
      gomp_mutex_init (nlock);

      plock = __sync_val_compare_and_swap (pplock, plock, nlock);
      if (plock != nlock)
	{
	  gomp_mutex_destroy (nlock);
	  free (nlock);
	}
#else
      gomp_mutex_lock (&create_lock_lock);
      plock = *pplock;
      if (plock == NULL)
	{
	  plock = gomp_malloc (sizeof (gomp_mutex_t));
	  gomp_mutex_init (plock);
	  __sync_synchronize ();
	  *pplock = plock;
	}
      gomp_mutex_unlock (&create_lock_lock);
#endif
    }

  gomp_mutex_lock (plock);
}

void
GOMP_critical_name_end (gomp_mutex_t **pplock)
{
  gomp_mutex_unlock (*pplock);
}

static void __attribute__((constructor))
initialize_critical (void)
{
  gomp_mutex_init (&default_lock);
#ifndef HAVE_SYNC_BUILTINS
  gomp_mutex_init (&create_lock_lock);
#endif
}
