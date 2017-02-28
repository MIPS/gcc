/* Copyright (C) 2017 Free Software Foundation, Inc.

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

/* OpenACC Profiling Interface.  */

#include "libgomp.h"
#include "oacc-int.h"
#include "acc_prof.h"
#include <assert.h>
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef PLUGIN_SUPPORT
# include <dlfcn.h>
#endif

#define STATIC_ASSERT(expr) _Static_assert (expr, "!(" #expr ")")

/* Statically assert that the layout of the common fields in the
   "acc_event_info" variants matches.  */
/* event_type */
STATIC_ASSERT (offsetof (acc_event_info, event_type)
	       == offsetof (acc_event_info, data_event.event_type));
STATIC_ASSERT (offsetof (acc_event_info, data_event.event_type)
	       == offsetof (acc_event_info, launch_event.event_type));
STATIC_ASSERT (offsetof (acc_event_info, data_event.event_type)
	       == offsetof (acc_event_info, other_event.event_type));
/* valid_bytes */
STATIC_ASSERT (offsetof (acc_event_info, data_event.valid_bytes)
	       == offsetof (acc_event_info, launch_event.valid_bytes));
STATIC_ASSERT (offsetof (acc_event_info, data_event.valid_bytes)
	       == offsetof (acc_event_info, other_event.valid_bytes));
/* parent_construct */
STATIC_ASSERT (offsetof (acc_event_info, data_event.parent_construct)
	       == offsetof (acc_event_info, launch_event.parent_construct));
STATIC_ASSERT (offsetof (acc_event_info, data_event.parent_construct)
	       == offsetof (acc_event_info, other_event.parent_construct));
/* implicit */
STATIC_ASSERT (offsetof (acc_event_info, data_event.implicit)
	       == offsetof (acc_event_info, launch_event.implicit));
STATIC_ASSERT (offsetof (acc_event_info, data_event.implicit)
	       == offsetof (acc_event_info, other_event.implicit));
/* tool_info */
STATIC_ASSERT (offsetof (acc_event_info, data_event.tool_info)
	       == offsetof (acc_event_info, launch_event.tool_info));
STATIC_ASSERT (offsetof (acc_event_info, data_event.tool_info)
	       == offsetof (acc_event_info, other_event.tool_info));

struct goacc_prof_callback_entry
{
  acc_prof_callback cb;
  int ref;
  bool enabled;
  struct goacc_prof_callback_entry *next;
};

/* goacc_prof_callbacks_enabled[acc_ev_none] acts as a global toggle.  */
static bool goacc_prof_callbacks_enabled[acc_ev_last];
static struct goacc_prof_callback_entry *goacc_prof_callback_entries[acc_ev_last];

/* This lock is used to protect access to goacc_prof_callbacks_enabled, and
   goacc_prof_callback_entries.  */
static gomp_mutex_t goacc_prof_lock;

void
goacc_profiling_initialize (void)
{
  gomp_mutex_init (&goacc_prof_lock);

  /* Initially, all callbacks for all events are enabled.  */
  for (int i = 0; i < acc_ev_last; ++i)
    goacc_prof_callbacks_enabled[i] = true;

  /* We are to invoke an external acc_register_library routine, defaulting to
     our stub oacc-profiling-acc_register_library.c:acc_register_library
     implementation.  */
  gomp_debug (0, "%s: calling acc_register_library\n", __FUNCTION__);
  //TODO.
  acc_register_library (acc_prof_register, acc_prof_unregister, NULL);
#ifdef PLUGIN_SUPPORT
  char *acc_proflibs = getenv ("ACC_PROFLIB");
  while (acc_proflibs != NULL && acc_proflibs[0] != '\0')
    {
      char *acc_proflibs_sep = strchr (acc_proflibs, ';');
      char *acc_proflib;
      if (acc_proflibs_sep == acc_proflibs)
	{
	  /* Stray ";" separator: make sure we don't dlopen the main
	     program.  */
	  acc_proflib = NULL;
	}
      else
	{
	  if (acc_proflibs_sep != NULL)
	    {
	      /* Single out the first library.  */
	      acc_proflib = gomp_malloc (acc_proflibs_sep - acc_proflibs + 1);
	      memcpy (acc_proflib, acc_proflibs,
		      acc_proflibs_sep - acc_proflibs);
	      acc_proflib[acc_proflibs_sep - acc_proflibs] = '\0';
	    }
	  else
	    {
	      /* No ";" separator, so only one library.  */
	      acc_proflib = acc_proflibs;
	    }

	  gomp_debug (0, "%s: dlopen(%s)\n", __FUNCTION__, acc_proflib);
	  void *dl_handle = dlopen (acc_proflib, RTLD_LAZY);
	  if (dl_handle != NULL)
	    {
	      typeof (&acc_register_library) a_r_l
		= dlsym (dl_handle, "acc_register_library");
	      if (a_r_l == NULL)
		goto dl_fail;
	      /* Avoid duplicate registration, for example if the same shared
		 library is specified in LD_PRELOAD and ACC_PROFLIB -- which
		 TAU 2.26 does when using "tau_exec -openacc".  */
	      if (a_r_l != acc_register_library)
		{
		  gomp_debug (0, "  %s: calling %s:acc_register_library\n",
			      __FUNCTION__, acc_proflib);
		  //TODO.
		  a_r_l (acc_prof_register, acc_prof_unregister, NULL);
		}
	      else
		gomp_debug (0, "  %s: skipping duplicate"
			    " %s:acc_register_library\n",
			    __FUNCTION__, acc_proflib);
	    }
	  else
	    {
	    dl_fail:
	      gomp_error ("while loading ACC_PROFLIB %s: %s",
			  acc_proflib, dlerror ());
	      if (dl_handle != NULL)
		{
		  int err = dlclose (dl_handle);
		  dl_handle = NULL;
		  if (err != 0)
		    goto dl_fail;
		}
	    }
	}

      if (acc_proflib != acc_proflibs)
	{
	  free (acc_proflib);

	  acc_proflibs = acc_proflibs_sep + 1;
	}
      else
	acc_proflibs = NULL;
    }
#endif /* PLUGIN_SUPPORT */
}

void
acc_prof_register (acc_event_t ev, acc_prof_callback cb, acc_register_t reg)
{
  //TODO
  gomp_debug (0, "%s: ev=%d, cb=%p, reg=%d\n",
	      __FUNCTION__, (int) ev, (void *) cb, (int) reg);

  enum
  {
    EVENT_KIND_BOGUS,
    EVENT_KIND_NORMAL,
    /* As end events invoke callbacks in the reverse order, we register these
       in the reverse order here.  */
    EVENT_KIND_END,
  } event_kind = EVENT_KIND_BOGUS;
  switch (ev)
    {
    case acc_ev_none:
    case acc_ev_device_init_start:
    case acc_ev_device_shutdown_start:
    case acc_ev_runtime_shutdown:
    case acc_ev_create:
    case acc_ev_delete:
    case acc_ev_alloc:
    case acc_ev_free:
    case acc_ev_enter_data_start:
    case acc_ev_exit_data_start:
    case acc_ev_update_start:
    case acc_ev_compute_construct_start:
    case acc_ev_enqueue_launch_start:
    case acc_ev_enqueue_upload_start:
    case acc_ev_enqueue_download_start:
    case acc_ev_wait_start:
      event_kind = EVENT_KIND_NORMAL;
      break;
    case acc_ev_device_init_end:
    case acc_ev_device_shutdown_end:
    case acc_ev_enter_data_end:
    case acc_ev_exit_data_end:
    case acc_ev_update_end:
    case acc_ev_compute_construct_end:
    case acc_ev_enqueue_launch_end:
    case acc_ev_enqueue_upload_end:
    case acc_ev_enqueue_download_end:
    case acc_ev_wait_end:
      event_kind = EVENT_KIND_END;
      break;
    case acc_ev_last:
      break;
    }
  if (event_kind == EVENT_KIND_BOGUS)
    {
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these, for forward compatibility?
      gomp_error ("ignoring %s request for TODOinvalid acc_event_t %d",
		  __FUNCTION__, /* TODO */ (int) ev);
      return;
    }

  bool bogus = true;
  switch (reg)
    {
    case acc_reg:
    case acc_toggle:
    case acc_toggle_per_thread:
      bogus = false;
      break;
    }
  if (bogus)
    {
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these, for forward compatibility?
      gomp_error ("ignoring %s request with TODOinvalid acc_register_t %d",
		  __FUNCTION__, /* TODO */ (int) reg);
      return;
    }

  /* Special cases.  */
  if (reg == acc_toggle)
    {
      if (cb == NULL)
	{
	  gomp_debug (0, "  globally enabling callbacks\n");
	  gomp_mutex_lock (&goacc_prof_lock);
	  /* goacc_prof_callbacks_enabled[acc_ev_none] acts as a global
	     toggle.  */
	  goacc_prof_callbacks_enabled[ev] = true;
	  gomp_mutex_unlock (&goacc_prof_lock);
	  return;
	}
      else if (ev == acc_ev_none && cb != NULL)
	{
	  gomp_debug (0, "  ignoring request\n");
	  /* Silently ignore request.  */
	  return;
	}
    }
  else if (reg == acc_toggle_per_thread)
    {
      if (ev == acc_ev_none && cb == NULL)
	{
	  gomp_debug (0, "  thread: enabling callbacks\n");
	  goacc_lazy_initialize ();
	  struct goacc_thread *thr = goacc_thread ();
	  thr->prof_callbacks_enabled = true;
	  return;
	}
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these?
      gomp_error ("ignoring %s request for acc_toggle_per_thread with TODO",
		  __FUNCTION__);
      return;
    }

  gomp_mutex_lock (&goacc_prof_lock);

  struct goacc_prof_callback_entry *it, *it_p;
  it = goacc_prof_callback_entries[ev];
  it_p = NULL;
  while (it)
    {
      if (it->cb == cb)
	break;
      it_p = it;
      it = it->next;
    }

  switch (reg)
    {
    case acc_reg:
      /* If we already have this callback registered, just increment its ref
	 count.  */
      if (it != NULL)
	{
	  it->ref++;
	  gomp_debug (0, "  already registered;"
		      " incrementing ref count to: %d\n", it->ref);
	}
      else
	{
	  struct goacc_prof_callback_entry *e
	    = gomp_malloc (sizeof (struct goacc_prof_callback_entry));
	  e->cb = cb;
	  e->ref = 1;
	  e->enabled = true;
	  bool prepend = (event_kind == EVENT_KIND_END);
	  /* If we don't have any callback registered yet, also use the
	     "prepend" code path.  */
	  if (it_p == NULL)
	    prepend = true;
	  if (prepend)
	    {
	      gomp_debug (0, "  prepending\n");
	      e->next = goacc_prof_callback_entries[ev];
	      goacc_prof_callback_entries[ev] = e;
	    }
	  else
	    {
	      gomp_debug (0, "  appending\n");
	      e->next = NULL;
	      it_p->next = e;
	    }
	}
      break;

    case acc_toggle:
      if (it == NULL)
	{
	  /* Silently ignore acc_toggle request if not registered.  */
	  gomp_debug (0, "  not enabling; not registered\n");
	}
      else
	{
	  gomp_debug (0, "  enabling\n");
	  it->enabled = true;
	}
      break;

    case acc_toggle_per_thread:
      __builtin_unreachable ();
    }

  gomp_mutex_unlock (&goacc_prof_lock);
}

void
acc_prof_unregister (acc_event_t ev, acc_prof_callback cb, acc_register_t reg)
{
  //TODO
  gomp_debug (0, "%s: ev=%d, cb=%p, reg=%d\n",
	      __FUNCTION__, (int) ev, (void *) cb, (int) reg);

  if (ev < acc_ev_none
      || ev >= acc_ev_last)
    {
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these, for forward compatibility?
      gomp_error ("ignoring %s request for TODOinvalid acc_event_t %d",
		  __FUNCTION__, /* TODO */ (int) ev);
      return;
    }

  bool bogus = true;
  switch (reg)
    {
    case acc_reg:
    case acc_toggle:
    case acc_toggle_per_thread:
      bogus = false;
      break;
    }
  if (bogus)
    {
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these, for forward compatibility?
      gomp_error ("ignoring %s request with TODOinvalid acc_register_t %d",
		  __FUNCTION__, /* TODO */ (int) reg);
      return;
    }

  /* Special cases.  */
  if (reg == acc_toggle)
    {
      if (cb == NULL)
	{
	  gomp_debug (0, "  globally disabling callbacks\n");
	  gomp_mutex_lock (&goacc_prof_lock);
	  /* goacc_prof_callbacks_enabled[acc_ev_none] acts as a global
	     toggle.  */
	  goacc_prof_callbacks_enabled[ev] = false;
	  gomp_mutex_unlock (&goacc_prof_lock);
	  return;
	}
      else if (ev == acc_ev_none && cb != NULL)
	{
	  gomp_debug (0, "  ignoring request\n");
	  /* Silently ignore request.  */
	  return;
	}
    }
  else if (reg == acc_toggle_per_thread)
    {
      if (ev == acc_ev_none && cb == NULL)
	{
	  gomp_debug (0, "  thread: disabling callbacks\n");
	  goacc_lazy_initialize ();
	  struct goacc_thread *thr = goacc_thread ();
	  thr->prof_callbacks_enabled = false;
	  return;
	}
      //TODO: should this be a fatal error?  Or, should we (silently?) ignore these?
      gomp_error ("ignoring %s request for acc_toggle_per_thread with TODO",
		  __FUNCTION__);
      return;
    }

  gomp_mutex_lock (&goacc_prof_lock);

  struct goacc_prof_callback_entry *it, *it_p;
  it = goacc_prof_callback_entries[ev];
  it_p = NULL;
  while (it)
    {
      if (it->cb == cb)
	break;
      it_p = it;
      it = it->next;
    }

  switch (reg)
    {
    case acc_reg:
      if (it == NULL)
	{
	  //TODO: should this be a fatal error?  Or, should we (silently?) ignore these?
	  gomp_error ("ignoring %s request for acc_event_t %d: not registered",
		      __FUNCTION__, /* TODO */ (int) ev);
	  gomp_mutex_unlock (&goacc_prof_lock);
	  return;
	}
      it->ref--;
      gomp_debug (0, "  decrementing ref count to: %d\n", it->ref);
      if (it->ref == 0)
	{
	  if (it_p == NULL)
	    goacc_prof_callback_entries[ev] = it->next;
	  else
	    it_p->next = it->next;
	  free (it);
	}
      break;

    case acc_toggle:
      if (it == NULL)
	{
	  /* Silently ignore acc_toggle request if not registered.  */
	  gomp_debug (0, "  not disabling; not registered\n");
	}
      else
	{
	  gomp_debug (0, "  disabling\n");
	  it->enabled = false;
	}
      break;

    case acc_toggle_per_thread:
      __builtin_unreachable ();
    }

  gomp_mutex_unlock (&goacc_prof_lock);
}

/* Set up to dispatch events?  */

bool
goacc_profiling_setup_p (struct goacc_thread *thr,
			 acc_prof_info *prof_info, acc_api_info *api_info)
{
  //TODO
  gomp_debug (0, "%s (%p)\n", __FUNCTION__, thr);

  /* If we don't have any per-thread state yet, we can't register prof_info and
     api_info.  */
  /* TODO: In this case, should we actually call goacc_lazy_initialize here,
     and return the "thr" from goacc_profiling_setup_p?  */
  if (__builtin_expect (thr == NULL, false))
    {
      //TODO
      gomp_debug (0, "Can't generate OpenACC Profiling Interface events for"
		  " the current call, construct, or directive\n");
      return false;
    }

  bool profiling_dispatch_p
    = __builtin_expect (goacc_profiling_dispatch_p (false), false);
  if (thr->prof_info != NULL)
    {
      assert (profiling_dispatch_p); //TODO
      /* Profiling has already been set up for an outer construct.  In this
	 case, we continue to use the existing information, and thus return
	 "false" here.

	 This can happen, for example, for an enter data directive, which sets
	 up profiling, then calls into acc_copyin, which should not again set
	 up profiling, should not overwrite the existing information.  */
      //TODO: Is this all kosher?
      return false;
    }

  if (profiling_dispatch_p)
    {
      thr->prof_info = prof_info;

      prof_info->event_type = -1; /* Must be set later.  */
      prof_info->valid_bytes = _ACC_PROF_INFO_VALID_BYTES;
      prof_info->version = _ACC_PROF_INFO_VERSION;
      //TODO
      if (thr->dev)
	{
	  prof_info->device_type = acc_device_type (thr->dev->type);
	  prof_info->device_number = thr->dev->target_id;
	}
      else
	{
	  prof_info->device_type = -1;
	  prof_info->device_number = -1;
	}
      prof_info->thread_id = -1; //TODO
      prof_info->async = acc_async_sync; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info->async_queue = prof_info->async;
      prof_info->src_file = NULL; //TODO
      prof_info->func_name = NULL; //TODO
      prof_info->line_no = -1; //TODO
      prof_info->end_line_no = -1; //TODO
      prof_info->func_line_no = -1; //TODO
      prof_info->func_end_line_no = -1; //TODO

      thr->api_info = api_info;

      api_info->device_api = acc_device_api_none; //TODO
      api_info->valid_bytes = _ACC_API_INFO_VALID_BYTES;
      api_info->device_type = prof_info->device_type;
      api_info->vendor = -1; //TODO
      api_info->device_handle = NULL; //TODO
      api_info->context_handle = NULL; //TODO
      api_info->async_handle = NULL; //TODO
    }

  return profiling_dispatch_p;
}

/* Prepare to dispatch events?  */

bool
goacc_profiling_dispatch_p (bool check_not_nested_p)
{
  //TODO
  gomp_debug (0, "%s\n", __FUNCTION__);

  struct goacc_thread *thr = goacc_thread ();
  if (__builtin_expect (thr == NULL, false))
    {
      /* If we don't have any per-thread state yet, that means that per-thread
	 callback dispatch has not been explicitly disabled (which only a call
	 to acc_prof_unregister with acc_toggle_per_thread will do, and that
	 would have allocated per-thread state via goacc_lazy_initialize);
	 initially, all callbacks for all events are enabled.  */
      //TODO
      gomp_debug (0, "  %s: don't have any per-thread state yet\n", __FUNCTION__);
    }
  else
    {
      if (check_not_nested_p)
	{
	  /* No nesting.  */
	  assert (thr->prof_info == NULL);
	  assert (thr->api_info == NULL);
	}

      if (__builtin_expect (!thr->prof_callbacks_enabled, true))
	{
	  //TODO
	  gomp_debug (0, "  %s: disabled for this thread\n", __FUNCTION__);
	  return false;
	}
    }

  gomp_mutex_lock (&goacc_prof_lock);

  /* goacc_prof_callbacks_enabled[acc_ev_none] acts as a global toggle.  */
  if (__builtin_expect (!goacc_prof_callbacks_enabled[acc_ev_none], true))
    {
      //TODO
      gomp_debug (0, "  %s: disabled globally\n", __FUNCTION__);
      gomp_mutex_unlock (&goacc_prof_lock);
      return false;
    }

  gomp_mutex_unlock (&goacc_prof_lock);

  return true;
}

/* Dispatch events.

   TODO.
   This must only be called if goacc_profiling_dispatch_p returned a true
   result.  */

void
goacc_profiling_dispatch (acc_prof_info *prof_info, acc_event_info *event_info,
			  acc_api_info *apt_info)
{
  acc_event_t event_type = event_info->event_type;
  //TODO
  gomp_debug (0, "%s: event_type=%d\n", __FUNCTION__, (int) event_type);
  //TODO
  assert (event_type > acc_ev_none
	  && event_type < acc_ev_last);

  gomp_mutex_lock (&goacc_prof_lock);

  if (!goacc_prof_callbacks_enabled[event_type])
    {
      //TODO
      gomp_debug (0, "  %s: disabled for this event type\n", __FUNCTION__);
      gomp_mutex_unlock (&goacc_prof_lock);
      return;
    }

  for (struct goacc_prof_callback_entry *e
	 = goacc_prof_callback_entries[event_type];
       e != NULL;
       e = e->next)
    {
      if (!e->enabled)
	{
	  //TODO
	  gomp_debug (0, "  %s: disabled for callback %p\n",
		      __FUNCTION__, e->cb);
	  continue;
	}

      //TODO
      gomp_debug (0, "  %s: calling callback %p\n", __FUNCTION__, e->cb);
      e->cb (prof_info, event_info, apt_info);
    }

  gomp_mutex_unlock (&goacc_prof_lock);
}
