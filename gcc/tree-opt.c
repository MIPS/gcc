/* Tree Options.
   Copyright (C) 2003, 2004, 2007 Free Software Foundation, Inc.
   Contributed by Karthik Kumar

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
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "function.h"
#include "flags.h"
#include "toplev.h"
#include "ggc.h"
#include "output.h"
#include "tm_p.h"
#include "target.h"
#include "tree-pass.h"
#include "diagnostic.h"
#include "opts.h"

/* Apply options to the function. */
static unsigned int
driver_push_set_options (void)
{
  tree attrs = DECL_ATTRIBUTES (current_function_decl);
  push_attribute_options ();
  if (attrs)
    {
      tree opt_attrs = lookup_attribute ("option", attrs);
      for (; opt_attrs; opt_attrs = TREE_CHAIN (opt_attrs))
	{
	  tree args = TREE_VALUE (opt_attrs);
	  tree value = NULL_TREE;
	  unsigned int result;
	  const char *argv[2] = { NULL, NULL };
	  char *modswitch = NULL;
	  unsigned long len = 0;
	  if (args && (value = TREE_VALUE (args)))
	    {
	      argv[0] = TREE_STRING_POINTER (value);
	      len = strlen (argv[0]);
	      modswitch = ggc_alloc (len + 2);
	      modswitch[0] = '-';
	      modswitch[1] = 0;
	      strcat (modswitch, argv[0]);
	      argv[0] = modswitch;
	      modswitch = NULL;
	      args = TREE_CHAIN (args);
	      if (args && (value = TREE_VALUE (args)))
		argv[1] = TREE_STRING_POINTER (value);
	      result = handle_option (argv, 0, 1);
	      if (!result)
		{
		  error ("unable to set option");
		}
	      else
		{
#ifdef OVERRIDE_OPTIONS
		  /* Some machines may reject certain combinations of options.  */
		  OVERRIDE_OPTIONS;
#endif
		}
	    }
	}
    }
  return true;
}

/* Pop options.  */
static unsigned int
driver_pop_options (void)
{
  pop_attribute_options ();
#ifdef OVERRIDE_OPTIONS
  /* Some machines may reject certain combinations of options.  */
  OVERRIDE_OPTIONS;
#endif
  return true;
}

struct tree_opt_pass pass_push_set_options = {
  "push_set_options",           /* name */
  NULL,                         /* gate */
  driver_push_set_options,      /* execute */
  NULL,                         /* sub */
  NULL,                         /* next */
  0,                            /* static_pass_number */
  0,                            /* tv_id */
  0,                            /* properties_required */
  0,                            /* properties_provided */
  0,                            /* properties_destroyed */
  0,                            /* todo_flags_start */
  0,                            /* todo_flags_finish */
  0                             /* letter */
};

struct tree_opt_pass pass_pop_options = {
  "pop_options",                /* name */
  NULL,                         /* gate */
  driver_pop_options,           /* execute */
  NULL,                         /* sub */
  NULL,                         /* next */
  0,                            /* static_pass_number */
  0,                            /* tv_id */
  0,                            /* properties_required */
  0,                            /* properties_provided */
  0,                            /* properties_destroyed */
  0,                            /* todo_flags_start */
  0,                            /* todo_flags_finish */
  0                             /* letter */
};
