/* High-level event callback mechanism for GCC plugins.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "hashtab.h"
#include "toplev.h"
#include "gcc-plugin.h"

#include "highlev-plugin-internal.h"

/* Event structure.  */
struct hl_plugin_event
{
  int event;			/* Number of the event */
  event_callback_t run;		/* Callback function */
};


/* Parameter structure.  */
struct event_parameter
{
  const char *name;		/* Name for the parameter */
  void *value;			/* Pointer to data */
  event_parameter_type type;	/* Type enumeration of value */
};


static htab_t parameters_hash = NULL; 


/* Hash dependent functions.  */
static hashval_t
string_htab_hash (const void *x)
{
  const char *p = *((const char * const *) x);

  if (p != NULL)
    return htab_hash_string (p);

  return 0;
}


static int
string_htab_eq (const void *x, const void *y)
{
  const char *s1 = *((const char * const *) x);
  const char *s2 = *((const char * const *) y);

  return !strcmp (s1, s2);
}


static void
parameter_htab_del (void *p)
{
  struct parameter *param = (struct parameter *) p;
  free (param);
}


static void
hash_param_callback (void *gcc_data, void *user_data)
{
  struct hl_plugin_event *ev = (struct hl_plugin_event *) user_data;
  va_list va;
  const char *name;
  int type;
  void *value;

  /* Possible extension:
     Might interpret gcc_data differently for specific values of ev->event,
     e.g. we could get a struct loop and put all the struct members in
     named parameters.  */
  va_copy (va, * (va_list *) gcc_data);

  while ((name = va_arg (va, const char *)) != NULL)
    {
      type = va_arg (va, int);
      value = va_arg (va, void *);
      register_event_parameter (name, value, type);
    }
  va_end (va);
  ev->run ();
  va_copy (va, * (va_list *) gcc_data);
  while ((name = va_arg (va, const char *)) != NULL)
    {
      unregister_event_parameter (name);
      va_arg (va, void *);
    }
}

/* Register a new event into hash table.  */
void 
register_plugin_event (const char *event_name, event_callback_t func)
{
  struct hl_plugin_event *ev = XCNEW (struct hl_plugin_event);

  if (event_name == NULL)
    internal_error ("Event cannot be registered with NULL name string\n");

  ev->event = get_named_event_id (event_name, INSERT);
  ev->run = func;
  register_callback ("ICI", ev->event, hash_param_callback, ev);
  ici_refresh_internal_callbacks (ev->event);
}


/* Unregister an event.  */
void 
unregister_plugin_event (const char *event_name)
{
  int event = get_named_event_id (event_name, NO_INSERT);

  if (event < 0)
    return;

  unregister_callback ("ICI", event);
}


/* Return the array of all event names, terminated by NULL.  */
const char **
list_plugin_events (void)
{
  int i;

  /* allocate space for all names + the terminating NULL */
  const char **all_events = XNEWVEC (const char *, (event_last + 1));

  /* collect all event names */
  for (i = 0; i < event_last; i++)
    all_events[i] = plugin_event_name[i];

  /* mark end-of-array */
  all_events[i] = NULL;

  return all_events;
}


/* Register parameter.
 * Insert parameter into hash table. Allocates hash table
 * if needed.  */
void 
register_event_parameter (const char *name, void* value, 
                          event_parameter_type type)
{
  void **slot;
  struct event_parameter *param = XCNEW (struct event_parameter);

  param->name = name;
  param->value = value;
  param->type = type;

  if (param->name == NULL || !strcmp ("", param->name))
    internal_error ("Parameter cannot be registered with NULL name string\n");
  
  if (parameters_hash == NULL)
    parameters_hash =
      htab_create_alloc (150, string_htab_hash, 
			 string_htab_eq, parameter_htab_del, xcalloc, free);
  
  if (parameters_hash != NULL)
    {
      slot = htab_find_slot (parameters_hash, param, INSERT);
      *slot = (void *) param;
    }
}


/* Unregister parameter. Remove it from hash table.  */
void 
unregister_event_parameter (const char *name)
{
  struct event_parameter param;
  param.name = name;

  if (parameters_hash == NULL)
    internal_error ("Parameter hash not initialized.\n");

  if (parameters_hash != NULL)
    htab_remove_elt (parameters_hash, &param);
}


/* Get parameter pointer to data.
   Used by plugin to get pointer to data of the parameter <name>.  */
void *
get_event_parameter (const char *name)
{
  struct event_parameter tmp_param, *param;
  tmp_param.name = name;

  if (parameters_hash == NULL)
    internal_error ("Parameter hash not initialized.\n");
  
  if (parameters_hash != NULL)
    {
      param = (struct event_parameter *)
	htab_find (parameters_hash, &tmp_param);
      
      if (param != NULL && param->value != NULL)
	return param->value;
    }
  return NULL;
}

/* Get parameter type.
   Used by plugin to get type of data pointer of the parameter <name>.  */
event_parameter_type
get_event_parameter_type (const char *name)
{
  struct event_parameter tmp_param, *param;
  tmp_param.name = name;

  if (parameters_hash == NULL)
    internal_error ("Parameter hash not initialized.\n");
  
  if (parameters_hash != NULL)
    {
      param = (struct event_parameter *)
	htab_find (parameters_hash, &tmp_param);
      
      if (param != NULL && param->value != NULL)
	return param->type;
    }
  return EP_VOID;
}

/* Get list of names of all registered ICI parameters.
 * Traverse the hash table collecting the names of entries.
 */
static int
add_parameter_name (void **slot, void *data)
{
  const char ***crnt_name_ptr = (const char ***) data;
  struct event_parameter *parameter = *(struct event_parameter **) slot;

  /* store the name of the current parameter at the corresponding
     location */
  **crnt_name_ptr = parameter->name;

  /* advance the current location */
  (*crnt_name_ptr)++;

  /* return "OK" */
  return 1;
}


/* return the array of all parameter names, terminated by NULL */
const char **
list_event_parameters (void)
{
  size_t num_parameters = htab_elements (parameters_hash);

  /* allocate space for all names + the terminating NULL */
  const char **all_parameters = (const char **)
    xmalloc (sizeof (const char *) * (num_parameters+1));

  const char ***data = &all_parameters; /* data ptr for mapped function */
  const char **first_parameter = all_parameters; /* keep track of actual start */

  /* mark end-of-array */
  all_parameters[num_parameters] = NULL;

  /* collect all parameter names */
  htab_traverse_noresize (parameters_hash, add_parameter_name, data);

  /* use the stored start-of-array - all_parameters has changed during
     htab_traverse_noresize */
  return first_parameter;  
}
