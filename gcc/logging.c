/* Definitions for various logging mechanisms in GCC.
   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "function.h"
#include "langhooks.h"
#include "logging.h"

#ifdef ENABLE_LOGGING

/* This will form a stack of Pass Activity Logs.  */
typedef struct pass_activity_log_stack {
  struct pass_activity_log log;
  struct pass_activity_log_stack *next;
  int depth;
} pal_stack;



pal_stack root_log = { EMPTY_PASS_ACTIVITY_LOG, NULL, 0 };
static pal_stack *current_pal_stack = &root_log;
struct pass_activity_log *current_log = &(root_log.log);

static void finalize_pal (FILE *, pal_stack *);
static void push_pass_action (const char *);
static pal_stack * pop_pass_action (void);
static int stack_depth = 0;

static FILE *logfile = NULL;



#define print_field (f,name,value) 	if (value) 	\
					  fprintf (f, " %s:%ld", name, value)
    

/* Add PTR's data to the permanent log.  */
static void 
finalize_pal (FILE *f, pal_stack *stack_ptr)
{
  pal *ptr = &(stack_ptr->log);

  if (f == NULL)
    return;

  fprintf (f, "Pass '%s '  SA:%ld  SC:%ld  SD:%ld  SM:%ld NC:%ld  NR:%ld  NL:%ld NDL:%ld\n", 
	   ptr->name, ptr->stmts_added, ptr->stmts_changed, 
	   ptr->stmts_deleted, ptr->stmts_moved, ptr->names_created,
	   ptr->names_removed, ptr->imm_links, ptr->imm_delinks);

#if 0
  fprintf (f, "Pass '%s'", ptr->name);
  print_field (f, "Stmts-added", ptr->stmts_added);
  print_field (f, "Stmts-changed", ptr->stmts_changed);
  print_field (f, "Stmts-deleted", ptr->stmts_deleted);
  print_field (f, "Stmts-moved", ptr->stmts_moved);
  print_field (f, "SSA-names-created", ptr->names_created);
  print_field (f, "SSA-names-removed", ptr->names_removed);
  print_field (f, "Imm-links-added", ptr->imm_links);
  print_field (f, "Imm-links-removed", ptr->imm_delinks);
  fprintf (f, "\n");
#endif
}


/* Create a new current pass action log called NAME on the stack.  */

void
push_pass_action (const char *pass_name)
{
  pal_stack *tmp = xmalloc (sizeof (pal_stack));

  memset (tmp, 0, sizeof (pal_stack));
  if (pass_name != NULL)
    {
      strncpy (tmp->log.name, pass_name, PAL_NAME_SIZE - 1);
      tmp->log.name[PAL_NAME_SIZE - 1] = '\0';
    }

  tmp->next = current_pal_stack;
  tmp->depth = ++stack_depth;
  current_pal_stack = tmp;
  current_log = &(current_pal_stack->log);
}


/* Remove the current pass action log from the stack and return it.  
   Return root_log if the stack is emp.  */

pal_stack *
pop_pass_action (void)
{
  pal_stack *ret = current_pal_stack;

  if (ret != &root_log) 
    {
      current_pal_stack = current_pal_stack->next;
      current_log = &(current_pal_stack->log);
    }
  --stack_depth;
  return ret;
}


static void
clear_root_log (void)
{
  strcpy (root_log.log.name, "Root");
  root_log.log.stmts_added = root_log.log.stmts_changed = 0;
  root_log.log.stmts_deleted = 0;
  root_log.log.stmts_moved = root_log.log.names_created = 0;
  root_log.log.names_removed= 0;
  root_log.log.imm_links = root_log.log.imm_delinks = 0;
  root_log.depth = 0;
  root_log.next = NULL;

  stack_depth = 0;

}
/* Start all the logging routines.  */

void 
initialize_logging (tree fndecl)
{
  clear_root_log ();
  current_pal_stack = &root_log;
  current_log = &(current_pal_stack->log);

  logfile = fopen ("/tmp/gcc-logging", "a");
  fprintf (logfile, "### Start logging : ");

  if (fndecl)
    {
      if (cfun)
	switch (cfun->function_frequency)
	{
	  case FUNCTION_FREQUENCY_HOT:
	    fprintf (logfile, "hot ");
	    break;

	  case FUNCTION_FREQUENCY_UNLIKELY_EXECUTED:
	    fprintf (logfile, "cold ");
	    break;

	  default:
	    fprintf (logfile, "normal ");
	}

      fprintf (logfile, "%s (%s)", 
	       lang_hooks.decl_printable_name (fndecl, 2),
	       (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fndecl))));
    }
  fprintf (logfile, "\n");
}


/* Terminate all the logging routines and fush any remaining logs.  */

void
finalize_logging (void)
{
  pal_stack *tmp;

  while ((tmp = pop_pass_action ()) != &root_log)
    {
      finalize_pal (logfile, tmp);
      free (tmp);
    }
  finalize_pal (logfile, &root_log);

  gcc_assert (stack_depth == -1);

  fprintf (logfile, "### End logging\n");
  /* Clear root log.  */
  clear_root_log ();
  fclose (logfile);
  logfile = NULL;
}


void start_pass_logging (const char *name)
{
  push_pass_action (name);
}


void finish_pass_logging (const char *name)
{
  pal_stack *tmp = pop_pass_action ();

  /* Make sure its the right pass being removed.  */
  if (name != NULL)
    gcc_assert (strncmp (tmp->log.name, name, PAL_NAME_SIZE - 1) == 0);
  else
    gcc_assert (tmp->log.name[0] == '\0');

  finalize_pal (logfile, tmp);
  free (tmp);

}

#endif
