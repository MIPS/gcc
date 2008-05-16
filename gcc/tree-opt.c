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

/* Stack of options that are saved when processing an function with
   attribute(option).  */

struct cl_option_stors_stack
{
  struct cl_option_stors_stack *prev;
  union cl_option_stor copy[1];
};

static struct cl_option_stors_stack *cl_option_top = NULL;

/* Save attribute settable options to stack, pass new options to backend hook,
   and return true/false if the new options are valid.  */
bool
push_attribute_options (int argc, const char **argv)
{
  bool ret = true;
  struct cl_option_stors_stack *ptr
    = xmalloc (sizeof(struct cl_option_stors_stack)
	       + (sizeof (union cl_option_stor) * (cl_option_stors_count - 1)));

  memcpy (ptr->copy,
	  cl_option_stors,
	  sizeof(union cl_option_stor) * cl_option_stors_count);

  ptr->prev = cl_option_top;
  cl_option_top = ptr;

  if (targetm.target_specific.push_options)
    ret = targetm.target_specific.push_options (argc, argv);

  return ret;
}

/* Restore attribute options from stack */
void
pop_attribute_options (void)
{
  struct cl_option_stors_stack *ptr = cl_option_top;

  if (ptr)
    {
      memcpy (cl_option_stors,
	      ptr->copy,
	      sizeof(union cl_option_stor) * cl_option_stors_count);

      cl_option_top = ptr->prev;
      free (ptr);

      if (targetm.target_specific.pop_options)
	targetm.target_specific.pop_options ();
    }
}

/* Apply options to the function. */
static unsigned int
driver_push_set_options (void)
{
  if (targetm.target_specific.push_options)
    {
      tree attrs = DECL_ATTRIBUTES (current_function_decl);
      tree opt_attrs = (attrs ? lookup_attribute ("option", attrs) : NULL_TREE);

      if (opt_attrs)
	{
	  tree orig_opt_attrs = opt_attrs;
	  int argc = 0;
	  int max_argc = 0;
	  const char **argv;

	  /* Count the number of arguments */
	  for (; opt_attrs; opt_attrs = TREE_CHAIN (opt_attrs))
	    {
	      tree args = TREE_VALUE (opt_attrs);

	      for (; args; args = TREE_CHAIN (args))
		{
		  if (TREE_VALUE (args))
		    max_argc++;
		}
	    }

	  argv = (const char **) alloca (sizeof (char *) * (max_argc + 1));

	  /* Fill in the arguments */
	  for (opt_attrs = orig_opt_attrs;
	       opt_attrs;
	       opt_attrs = TREE_CHAIN (opt_attrs))
	    {
	      tree args = TREE_VALUE (opt_attrs);

	      for (; args; args = TREE_CHAIN (args))
		{
		  if (TREE_VALUE (args))
		    argv[argc++] = TREE_STRING_POINTER (TREE_VALUE (args));
		}
	    }

	  argv[argc] = NULL;
	  push_attribute_options (argc, argv);
	}
    }

  return true;
}

/* Pop options.  */
static unsigned int
driver_pop_options (void)
{
  if (targetm.target_specific.pop_options)
    {
      tree attrs = DECL_ATTRIBUTES (current_function_decl);

      if (attrs && lookup_attribute ("option", attrs))
	pop_attribute_options ();
    }

  return true;
}

struct rtl_opt_pass pass_push_set_options =
{
  {
    RTL_PASS,			/* type */
    "push_set_options",		/* name */
    NULL,			/* gate */
    driver_push_set_options,	/* execute */
    NULL,			/* sub */
    NULL,			/* next */
    0,				/* static_pass_number */
    0,				/* tv_id */
    0,				/* properties_required */
    0,				/* properties_provided */
    0,				/* properties_destroyed */
    0,				/* todo_flags_start */
    0,				/* todo_flags_finish */
  }
};

struct rtl_opt_pass pass_pop_options =
{
  {
    RTL_PASS,			/* type */
    "pop_options",		/* name */
    NULL,			/* gate */
    driver_pop_options,		/* execute */
    NULL,			/* sub */
    NULL,			/* next */
    0,				/* static_pass_number */
    0,				/* tv_id */
    0,				/* properties_required */
    0,				/* properties_provided */
    0,				/* properties_destroyed */
    0,				/* todo_flags_start */
    0,				/* todo_flags_finish */
  }
};
