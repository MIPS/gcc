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
#include "targhooks.h"
#include "tree-pass.h"
#include "diagnostic.h"
#include "opts.h"

static int target_specific_count_args (tree args);
static void target_specific_build_args (tree args,
					int *p_argc,
					int max_argc,
					const char *argv[]);

/* Count how many target specific options there are in ARGS.

   The ARGS argument is either a TREE_LIST that points to STRING_CST nodes
   (when we are called from handle_option_attribute to valid the attributes),
   or a TREE_LIST that points to a TREE_LIST which in turn points to STRING_CST
   nodes (when we use the result of lookup_attribute ("option")).  */

static int
target_specific_count_args (tree args)
{
  int ret = 0;

  if (TREE_CODE (args) == TREE_LIST)
    {
      for (; args; args = TREE_CHAIN (args))
	if (TREE_VALUE (args))
	  ret += target_specific_count_args (TREE_VALUE (args));
    }

  else if (TREE_CODE (args) == STRING_CST)
    ret++;

  else
    gcc_unreachable ();

  return ret;
}

/* Build the argument vector from the attribute list of ARGS, updating the
   current argument count in the int pointed to by P_ARGC, with a maximum count
   of MAX_ARGC, and the vector in ARGV.  */

static void
target_specific_build_args (tree args, int *p_argc, int max_argc,
			    const char *argv[])
{
  if (TREE_CODE (args) == TREE_LIST)
    {
      for (; args; args = TREE_CHAIN (args))
	if (TREE_VALUE (args))
	  target_specific_build_args (TREE_VALUE (args), p_argc, max_argc,
				      argv);
    }

  else if (TREE_CODE (args) == STRING_CST)
    {
      gcc_assert (*p_argc < max_argc);
      argv[*p_argc] = TREE_STRING_POINTER (args);
      (*p_argc)++;
    }

  else
    gcc_unreachable ();
}


/* For handling "option" attribute. arguments as in
   struct attribute_spec.handler.  */

tree
handle_option_attribute (tree *node,
			 tree ARG_UNUSED (name),
			 tree args,
			 int ARG_UNUSED (flags),
			 bool *no_add_attrs)
{
#ifndef HAVE_TARGET_SPECIFIC
  error ("option attribute is not supported on this machine");
  *no_add_attrs = true;

#else
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning (OPT_Wattributes, "option attribute ignored");
      *no_add_attrs = true;
    }
  else if (! targetm.target_specific.validate)
    {
      error ("option attribute is not supported on this machine");
      *no_add_attrs = true;
    }
  else
    {
      int max_argc = target_specific_count_args (args);

      if (max_argc > 0)
	{
	  int argc = 0;
	  const char **argv = alloca ((max_argc + 1) * sizeof (char *));

	  target_specific_build_args (args, &argc, max_argc, argv);
	  argv[max_argc] = NULL;
	  if (targetm.target_specific.validate (argc, argv, *node))
	    *no_add_attrs = true;
	}
    }

#endif

  return NULL_TREE;
}


/* Determine whether a function FN can be inlined.  Be conservative, and assume
   any function with different target specific options cannot be inlined.

   Backends should really provide their own hook, as this function will be slow
   since it has to do a lot of string comparison and copies to determine if two
   functions have the same target specific options.
*/

bool
default_target_specific_can_inline_p (tree caller, tree callee)
{
  bool ret = false;
  struct target_specific_data *callee_opts = DECL_TARGET_SPECIFIC (callee);
  struct target_specific_data *caller_opts = DECL_TARGET_SPECIFIC (caller);

  /* If callee has no option attributes, then it is ok to inline */
  if (!callee_opts)
    ret = true;

  /* If caller has no option attributes, but callee does then it is not ok to
     inline */
  else if (!caller_opts)
    ret = false;

  /* Both caller and callee have attributes, see if they are the same */
  else
    ret = (callee_opts == caller_opts);

  return ret;
}
