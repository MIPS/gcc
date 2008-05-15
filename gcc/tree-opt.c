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
#ifdef TARGET_SPECIFIC_OPTION
  tree attrs = DECL_ATTRIBUTES (current_function_decl);

#ifdef TARGET_SPECIFIC_INIT
  TARGET_SPECIFIC_INIT ();
#endif

  push_attribute_options ();

  if (attrs)
    {
      tree opt_attrs = lookup_attribute ("option", attrs);

      for (; opt_attrs; opt_attrs = TREE_CHAIN (opt_attrs))
	{
	  tree args = TREE_VALUE (opt_attrs);

	  for (; args; args = TREE_CHAIN (args))
	    {
	      tree value = TREE_VALUE (args);
	      if (value)
		{
		  if (! TARGET_SPECIFIC_OPTION (TREE_STRING_POINTER (value)))
		    error ("Invalid target specific option %s",
			   TREE_STRING_POINTER (value));
		}
	    }
	}

#ifdef TARGET_SPECIFIC_PUSH
      TARGET_SPECIFIC_PUSH ();
#endif
    }
#endif
  return true;
}

/* Pop options.  */
static unsigned int
driver_pop_options (void)
{
#ifdef TARGET_SPECIFIC_OPTION
  pop_attribute_options ();

#ifdef TARGET_SPECIFIC_POP
  TARGET_SPECIFIC_POP ();
#endif
#endif
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
