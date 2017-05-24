/* OpenACC Runtime Library Definitions.

   Copyright (C) 2013-2017 Free Software Foundation, Inc.

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

#include <assert.h>
#include "openacc.h"
#include "libgomp.h"
#include "oacc-int.h"

int
acc_async_test (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  int res = thr->dev->openacc.async_test_func (async);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }

  return res;
}

int
acc_async_test_all (void)
{
  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  int res = thr->dev->openacc.async_test_all_func ();

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }

  return res;
}

void
acc_wait (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  thr->dev->openacc.async_wait_func (async);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

/* acc_async_wait is an OpenACC 1.0 compatibility name for acc_wait.  */
#ifdef HAVE_ATTRIBUTE_ALIAS
strong_alias (acc_wait, acc_async_wait)
#else
void
acc_async_wait (int async)
{
  acc_wait (async);
}
#endif

void
acc_wait_async (int async1, int async2)
{
  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async2; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  thr->dev->openacc.async_wait_async_func (async1, async2);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

void
acc_wait_all (void)
{
  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  thr->dev->openacc.async_wait_all_func ();

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

/* acc_async_wait_all is an OpenACC 1.0 compatibility name for acc_wait_all.  */
#ifdef HAVE_ATTRIBUTE_ALIAS
strong_alias (acc_wait_all, acc_async_wait_all)
#else
void
acc_async_wait_all (void)
{
  acc_wait_all ();
}
#endif

void
acc_wait_all_async (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  thr->dev->openacc.async_wait_all_async_func (async);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

int
acc_get_default_async (void)
{
  /* In the following, no OpenACC Profiling Interface events can possibly be
     generated.  */

  struct goacc_thread *thr = goacc_thread ();

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  return thr->default_async;
}

void
acc_set_default_async (int async)
{
  /* In the following, no OpenACC Profiling Interface events can possibly be
     generated.  */

  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  thr->default_async = async;
}
