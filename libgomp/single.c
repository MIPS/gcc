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

/* This file handles the SINGLE construct.  */

#include "libgomp.h"


/* This routine is called when first encountering a SINGLE construct that
   doesn't have a COPYPRIVATE clause.  Returns true if this is the thread
   that should execute the clause.  */

bool
GOMP_single_start (void)
{
  bool ret = gomp_work_share_start (false);
  gomp_mutex_unlock (&gomp_thread ()->ts.work_share->lock);
  gomp_work_share_end_nowait ();
  return ret;
}

/* This routine is called when first encountering a SINGLE construct that
   does have a COPYPRIVATE clause.  Returns NULL if this is the thread
   that should execute the clause; otherwise the return value is pointer
   given to GOMP_single_copy_end by the thread that did execute the clause.  */

void *
GOMP_single_copy_start (void)
{
  struct gomp_thread *thr = gomp_thread ();

  bool first;
  void *ret;

  first = gomp_work_share_start (false);
  gomp_mutex_unlock (&thr->ts.work_share->lock);
  
  if (first)
    ret = NULL;
  else
    {
      gomp_sem_wait (&thr->ts.team->threads[thr->ts.team_id]->release);

      ret = thr->ts.work_share->copyprivate;
      gomp_work_share_end_nowait ();
    }

  return ret;
}

/* This routine is called when the thread that entered a SINGLE construct
   with a COPYPRIVATE clause gets to the end of the construct.  */

void
GOMP_single_copy_end (void *data)
{
  struct gomp_thread *thr = gomp_thread ();
  struct gomp_team *team = thr->ts.team;
  unsigned int i, n, me;

  if (team != NULL)
    {
      thr->ts.work_share->copyprivate = data;

      n = team->nthreads;
      me = thr->ts.team_id;
      for (i = 0; i < n; ++i)
	if (i != me)
	  gomp_sem_post (&team->threads[i]->release);
    }

  gomp_work_share_end_nowait ();
}
