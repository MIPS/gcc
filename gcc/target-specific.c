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
  union cl_option_stor copy[1];
};

static struct cl_option_stors_stack *cl_option_top = NULL;

/* Structure to hold target options that are accumulated for passing on to
   the target backend hook.  */

typedef struct
{
  int argc;			/* current number of arguments */
  int max_argc;			/* max arguments allocated */
  char *string_pool;		/* pointer to the string pool */
  size_t string_alloc;		/* # bytes allocated to the string pool */
  size_t string_num;		/* # bytes used in the string pool */
  char *argv[1];		/* arguments */
} cl_option_args;

#define MIN_ALLOC_ARGS		10	/* number of arguments to allocate at a time */
#define STRING_POOL_SIZE	250	/* size of the string pool to allocate */

static int target_specific_qsort (const void *ptr_a, const void *ptr_b);
static cl_option_args *target_specific_build_arguments (cl_option_args *ap,
							tree args,
							bool compare_p);
static void target_specific_free_arguments (cl_option_args *ap);
static bool target_specific_push (int argc, char **argv);
static void target_specific_pop (void);
static unsigned int driver_push_set_options (void);
static unsigned int driver_pop_options (void);


/* Internal function to sort an array of character pointers.  */
static int
target_specific_qsort (const void *ptr_a, const void *ptr_b)
{
  return strcmp (*(char **)ptr_a, *(char **)ptr_b);
}

/* Internal function to build argument vectors.  */
static cl_option_args *
target_specific_build_arguments (cl_option_args *ap, tree args, bool compare_p)
{
  for (; args; args = TREE_CHAIN (args))
    {
      tree args2 = TREE_VALUE (args);
      if (args)
	{
	  size_t len = TREE_STRING_LENGTH (args2);
	  char *p;

	  /* Allocate or grow arguments */
	  if (! ap)
	    {
	      ap = xmalloc (sizeof (cl_option_args)
			    + MIN_ALLOC_ARGS * (sizeof (char *)));

	      ap->argc = 0;
	      ap->max_argc = MIN_ALLOC_ARGS;
	      ap->string_alloc = STRING_POOL_SIZE;
	      ap->string_num = 0;
	      ap->string_pool = (compare_p
				 ? NULL
				 : xmalloc (STRING_POOL_SIZE));
	    }
	  else if (ap->argc == ap->max_argc)
	    {
	      ap->max_argc += MIN_ALLOC_ARGS;
	      ap = xrealloc (ap, (sizeof (cl_option_args)
				  + (ap->max_argc * sizeof (char *))));
	    }

	  if (compare_p)
	    {
	      ap->argv[ap->argc++] = (char *)TREE_STRING_POINTER (args2);
	      ap->argv[ap->argc] = NULL;
	    }
	  else
	    {
	      /* Grow string pool if needed */
	      if (ap->string_num + len + 2 >= ap->string_alloc)
		{
		  size_t slen = ap->string_alloc + len + STRING_POOL_SIZE;
		  char *old_str = ap->string_pool;
		  char *new_str = xrealloc (old_str, slen);
		  int i;

		  ap->string_alloc = slen;
		  for (i = 0; i < ap->argc; i++)
		    ap->argv[i] = new_str + (ap->argv[i] - old_str);
		}

	      /* Add '-' in front of the argument.  */
	      p = ap->string_pool + ap->string_num;
	      ap->string_num += len + 2;
	      p[0] = '-';
	      memcpy (p+1, TREE_STRING_POINTER (args2), len);
	      p[len+1] = '\0';
	      ap->argv[ap->argc++] = p;
	      ap->argv[ap->argc] = NULL;
	    }
	}
    }

  if (ap && compare_p && ap->argc > 1)
    qsort (ap->argv, ap->argc, sizeof (char *), target_specific_qsort);

  return ap;
}

/* Internal function to release memory allocated for argument vectors */
static void
target_specific_free_arguments (cl_option_args *ap)
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
target_specific_push (int argc, char **argv)
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
    ret = targetm.target_specific.push_options (argc, (const char **)argv);

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
	  cl_option_args *ap = NULL;

	  /* Fill in the arguments */
	  for (; opt_attrs; opt_attrs = TREE_CHAIN (opt_attrs))
	    ap = target_specific_build_arguments (ap,
						  TREE_VALUE (opt_attrs),
						  false);

	  if (ap)
	    {
	      target_specific_push (ap->argc, ap->argv);
	      target_specific_free_arguments (ap);
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
      cl_option_args *ap = target_specific_build_arguments (NULL, args, false);

      if (ap)
	{
	  if (! target_specific_push (ap->argc, ap->argv))
	    *no_add_attrs = true;

	  target_specific_pop ();
	  target_specific_free_arguments (ap);
	}
    }

  return NULL_TREE;
}


/* Determine whether one function can inline another based on the target
   specific options.  */
bool
default_target_specific_can_inline_p (const_tree caller, const_tree callee)
{
  tree callee_attrs = DECL_ATTRIBUTES (callee);
  tree callee_target_specific = (callee_attrs
				 ? lookup_attribute ("option", callee_attrs)
				 : NULL_TREE);

  if (!callee_target_specific)
    return true;		/* anything calling generic is fine */

  else
    {
      tree caller_attrs = DECL_ATTRIBUTES (caller);
      tree caller_target_specific = (caller_attrs
				     ? lookup_attribute ("option", caller_attrs)
				     : NULL_TREE);


      if (caller_target_specific)
	{
	  /* XXX should add code to check if we have exactly the same options */
	  return false;		/* target specific calling target specific */
	}

      else
	return false;		/* generic calling target specific */
    }
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
