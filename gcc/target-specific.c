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

/* Stack of options that are saved when processing an function with
   attribute(option).  */

struct cl_option_stors_stack
{
  struct cl_option_stors_stack *prev;
  union cl_option_stor copy[1];			/* must be last */
};

static struct cl_option_stors_stack *cl_option_top = NULL;

/* Structure to hold target options that are accumulated for passing on to
   the target backend hook.  */

typedef struct
{
  char *string_pool;		/* pointer to strings */
  int argc;			/* current number of arguments */
  int max_argc;			/* current number of arguments */
  size_t string_size;		/* # bytes needed to hold all arguments */
  size_t string_cur;		/* current string position */
  const char *argv[1];		/* argument vector (must be last) */
} cl_option_args;

static int target_specific_qsort (const void *ptr_a, const void *ptr_b);
static int target_specific_count_args (tree args, size_t *p_str_size);
static void target_specific_build_args_internal (cl_option_args *ap,
						 tree args,
						 bool compare_p);
static cl_option_args *target_specific_build_args (tree args,
						   bool compare_p);
static void target_specific_free_args (cl_option_args *ap);
static bool target_specific_push (int argc, const char **argv);
static void target_specific_pop (void);
static unsigned int driver_push_set_options (void);
static unsigned int driver_pop_options (void);


/* Count how many target specific options there are in ARGS, and record how
   much space is needed to build the strings in P_STR_SIZE.

   The ARGS argument is either a TREE_LIST that points to STRING_CST nodes
   (when we are called from handle_option_attribute to valid the attributes),
   or a TREE_LIST that points to a TREE_LIST which in turn points to STRING_CST
   nodes (when we use the result of lookup_attribute ("option")).  */

static int target_specific_count_args (tree args, size_t *p_str_size)
{
  int ret = 0;

  if (TREE_CODE (args) == TREE_LIST)
    {
      for (; args; args = TREE_CHAIN (args))
	if (TREE_VALUE (args))
	  ret += target_specific_count_args (TREE_VALUE (args), p_str_size);
    }

  else if (TREE_CODE (args) == STRING_CST)
    {
      ret++;
      *p_str_size += TREE_STRING_LENGTH (args) + 2; /* room for '-' and '\0' */
    }

  else
    gcc_unreachable ();

  return ret;
}

/* Build the argument vector in AP, using the target specific options in ARGS.
   If COMPARE_P is true, we don't need the initial '-' in front of the
   argument. */

static void
target_specific_build_args_internal (cl_option_args *ap, tree args, bool compare_p)
{
  if (TREE_CODE (args) == TREE_LIST)
    {
      for (; args; args = TREE_CHAIN (args))
	if (TREE_VALUE (args))
	  target_specific_build_args_internal (ap, TREE_VALUE (args), compare_p);
    }

  else if (TREE_CODE (args) == STRING_CST)
    {
      gcc_assert (ap->argc < ap->max_argc);
      if (compare_p)
	ap->argv[ap->argc++] = TREE_STRING_POINTER (args);

      else
	{
	  char *p = ap->string_pool + ap->string_cur;
	  size_t len = TREE_STRING_LENGTH (args);

	  gcc_assert (ap->string_cur + len + 2 <= ap->string_size);
	  p[0] = '-';
	  memcpy (p+1, TREE_STRING_POINTER (args), len);
	  p[len+1] = '\0';
	  ap->argv[ap->argc++] = p;
	}
    }

  else
    gcc_unreachable ();
}

/* Sort an array of character pointers.  */

static int
target_specific_qsort (const void *ptr_a, const void *ptr_b)
{
  return strcmp (*(const char **)ptr_a, *(const char **)ptr_b);
}

/* Build argument vectors from the target specific options ARGS.  If COMPARE_P
   is true, we don't need to add a '-' in front of each argument, and we should
   sort the arguments to be able to compare to see if two functions have the
   same target specific options.  */

static cl_option_args *
target_specific_build_args (tree args, bool compare_p)
{
  size_t string_size = 0;
  int max_argc = target_specific_count_args (args, &string_size);
  cl_option_args *ap;

  if (! max_argc)
    ap = NULL;

  else
    {
      ap = xmalloc (sizeof (cl_option_args) + (sizeof (char *) * max_argc));
      ap->argc = 0;
      ap->max_argc = max_argc;
      ap->string_size = string_size;
      ap->string_cur = 0;
      ap->string_pool = (!compare_p ? xmalloc (string_size) : NULL);

      target_specific_build_args_internal (ap, args, compare_p);

      gcc_assert (ap->argc == ap->max_argc);
      ap->argv[ap->argc] = NULL;

      if (compare_p && ap->argc > 1)
	qsort (ap->argv, ap->argc, sizeof (char *), target_specific_qsort);
    }

  return ap;
}

/* Release memory allocated for argument vectors */

static void
target_specific_free_args (cl_option_args *ap)
{
  if (ap)
    {
      if (ap->string_pool)
	free (ap->string_pool);
      free (ap);
    }
}


/* Save attribute settable options to stack, pass new options to backend hook,
   and return true/false if the new options are valid.  */

static bool
target_specific_push (int argc, const char **argv)
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
static void
target_specific_pop (void)
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
	  cl_option_args *ap = target_specific_build_args (opt_attrs, false);

	  if (ap)
	    {
	      target_specific_push (ap->argc, ap->argv);
	      target_specific_free_args (ap);
	    }
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
	target_specific_pop ();
    }

  return true;
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
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning (OPT_Wattributes, "option attribute ignored");
      *no_add_attrs = true;
    }
  else if (! targetm.target_specific.push_options
	   || ! targetm.target_specific.pop_options)
    {
      error ("option attribute is not supported on this machine");
      *no_add_attrs = true;
    }
  else
    {
      cl_option_args *ap = target_specific_build_args (args, false);

      if (ap)
	{
	  if (! target_specific_push (ap->argc, ap->argv))
	    *no_add_attrs = true;

	  target_specific_pop ();
	  target_specific_free_args (ap);
	}
    }

  return NULL_TREE;
}


/* Determine whether a function FN can be inlined.  Be conservative, and assume
   any function with target specific options cannot be inlined.  */

bool
default_target_specific_can_inline_p (const_tree fn)
{
  if (! DECL_ATTRIBUTES (fn))
    return true;

  else if (lookup_attribute ("option", DECL_ATTRIBUTES (fn)))
    return false;

  return true;
}

/* RTL pass to push the current options if the function used
   attribute(option).  */
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

/* RTL pass to pop the current options if the function used
   attribute(option).  */
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
