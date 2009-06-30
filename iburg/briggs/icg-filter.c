/* Decide whether to use IRA or ICG at function granularity.
   Copyright (C) 2008  Free Software Foundation, Inc.

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
#include "flags.h"
#include "tm_p.h"
#include "errors.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "df.h"
#include "langhooks.h"
#include "icg.h"

#define MAX_NAME_SIZE 256

static int original_flag_icg = -1;
static int original_flag_late_codegen = -1;

/* Strip leading and trailing white space and comments from line.
   Modifies STR.  Stupid C, I hate you.  */

static
char *strip_string(char *str)
{
  char *start, *end;

  for (start = str;
       *start && (*start == ' ' || *start == '\t' || *start == '\n');
       start++) { }
  if (*start)
    {
      for (end = start; *end; end++) {
	if (*end == '#')
	  {
	    *end = '\0';
	    break;
	  }
      }
      for (; end >= start && (*end == '\0' || *end == ' '
			      || *end == '\t' || *end == '\n'); end--)
	*end = '\0';
    }
  return start;
}


/* Reads in a file specified by FLAG_VALUE into NAMES.  Memory is
   allocated for NAMES.  Returns sense of filter based on
   existence of leading '-' in FLAG_VALUE.  */

static bool
read_allocate_names (const char *flag_value, char ***names)
{
  const char *filename;
  bool sense;
  int n, nlines;
  char buf[MAX_NAME_SIZE] ;
  FILE *fp;

  /* FLAG_VALUE always begins with '='.  */
  gcc_assert (flag_value[0] == '=');
  if (flag_value[1] == '-') {
    sense = true;
    filename = &(flag_value[2]);
  } else {
    sense = false;
    filename = &(flag_value[1]);
  }
  fp = fopen (filename, "r");
  if (fp == NULL)
    internal_error ("Can't open ICG filter file \"%s\".", filename);

  /* First run through, count number of lines.  */
  nlines = 0;
  while (fgets (buf, MAX_NAME_SIZE, fp) == buf)
    if (*(strip_string (buf)))
      nlines++;
  *names = XNEWVEC (char *, nlines + 1);
  (*names)[nlines] = NULL;

  /* Second run, save lines.  */
  rewind(fp);
  n = 0;
  while (fgets (buf, MAX_NAME_SIZE, fp) == buf)
    {
      char *str = strip_string (buf);
      if (*str)
	{
	  gcc_assert (n < nlines);
	  (*names)[n] = XNEWVEC (char, MAX_NAME_SIZE);
	  strncpy((*names)[n], str, MAX_NAME_SIZE);
	  n++;
	}
    }
  fclose(fp);
  return sense;
}

/* Returns TRUE if NAME is in NAMES, where NAMES is created with
   read_allocate_names.  */

static bool
name_in_array_p (const char *name, char **names)
{
  int i;
  for (i = 0; names[i]; i++)
    if (strcmp (name, names[i]) == 0)
      return true;
  return false;
}

#define REJECT(msg) { \
  fflush(stdout); \
  fprintf(stderr, "ICG rejects \"%s\" (%s): %s\n", dname, aname, msg); \
  return true; \
}

/*
 * Return TRUE if this node is to be rejected,
 * eg it appears to contain things that the rest of ICG can not handle.
 */
static
bool icg_filter_walk_rtx_tree_test(rtx insn, const char *dname, const char *aname)
{
  enum rtx_code this_code;
  enum machine_mode this_mode;

  this_code = GET_CODE(insn);
  this_mode = GET_MODE(insn);

  switch (this_mode) {

  case XFmode:
    REJECT("XFmode");
    /*NOTREACHED*/

  case TFmode:
    REJECT("TFmode");
    /*NOTREACHED*/

  default:
    break;
  }

  switch (this_code) {

  case UNSPEC:
    {
      const int unspec_code = XINT(insn, 1);
      if (unspec_code == UNSPEC_SSE_PROLOGUE_SAVE) {
	REJECT("UNSPEC_SSE_PROLOGUE_SAVE in function");
	/*NOTREACHED*/
      }
      if (unspec_code == UNSPEC_ADD_CARRY) {
	REJECT("UNSPEC_ADD_CARRY in function");
	/*NOTREACHED*/
      }
    }
    break;

  case UNSPEC_VOLATILE:
    {
      const int unspec_code = XINT(insn, 1);
      char buf[BUFSIZ];
      /*
       * some candidates:
       * UNSPEC_SET_GOT (done as a pattern sync_old_addsi), code: lock xaddl %eax
       * (tarpit to support UNSPEC_SET_GOT in gcc/config/i386)
       */
      snprintf(buf, sizeof(buf),
	"unspec_volatile code in function: %d", unspec_code);
      REJECT(buf);
      /*NOTREACHED*/
    }
    break;

  case ASM_OPERANDS:
    {
      const char *template = ASM_OPERANDS_TEMPLATE(insn);
      char buf[BUFSIZ];
      snprintf(buf, sizeof(buf), "asm in function: \"%s\"",
	template ? template : "?");
      if (icg_analyze_asm(insn, ICG_ASM_PHASE_FILTER)) {
	fflush(stdout);
	fprintf(stderr, "ICG consumes asm \"%s\" (%s): %s\n", dname, aname, buf); \
        /* We will attempt to accept this */
      } else {
	REJECT(buf);
      }
    }
    break;

  default:
    break;
  }
  return false;	/* this node is not rejected, eg it is accepted */
}

typedef struct icg_filter_closure_ {
  const char *dname;
  const char *aname;
} icg_filter_closure;

/*
 * This is a worker function for for_each_rtx.
 * Return -1 to stop traversing sub expressions.
 * Return 0 to continue traversing.
 * Otherwise stop traversal immediately; we'll do this when we reject the tree.
 */
static
int icg_filter_walk_worker(rtx *x, void *data)
{
  const icg_filter_closure *cl = (const icg_filter_closure *)data;
  if (x && *x) {
    rtx insn = *x;
    const bool node_rejected = icg_filter_walk_rtx_tree_test(insn, cl->dname, cl->aname);
    if (node_rejected) {
      return 1;	/* return 1 immediately */
    }
  }
  return 0;	/* continue traversing */
}

/*
 * Return TRUE iff ICG rejects this tree as a candidate,
 * eg return FALSE if ICG should accept this tree
 * (Note logical inversion with skip_icg_p.)
 */
static
bool icg_filter_walk_rtx_tree(rtx insn, const char *dname, const char *aname)
{
  icg_filter_closure cld;
  icg_filter_closure *cl = &cld;
  bool is_rejected = false;
  cl->dname = dname;
  cl->aname = aname;
  is_rejected = for_each_rtx(&insn, icg_filter_walk_worker, cl);
  return is_rejected;
}

/* Programmatically determine whether the function should be compiled
   with ICG.  (Note logical inversion with
   icg_filter_walk_rtx_tree).  */

static bool
skip_icg_programmatic_p (const char *dname, const char *aname)
{
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb) {
    FOR_BB_INSNS (bb, insn) {
      if (INSN_P (insn)) {
	if (icg_filter_walk_rtx_tree(insn, dname, aname) == true) {
	  /*
	   * There's a rejected node somewhere in the tree.  We return
	   * true indicating that we do not accept the tree.
	   */
	  return true;
	}
      }
    }
  }
  return false;
}

/* Return TRUE iff ICG should not be applied to the current function based
   on flag_icg_filter_file option.  */

static bool
skip_icg_file_p  (const char *dname, const char *aname)
{
  static char **names = NULL;
  static bool skip_sense;
  bool match;

  gcc_assert (flag_icg_filter_file);
  if (!names)
    skip_sense = read_allocate_names (flag_icg_filter_file, &names);

  match = name_in_array_p (main_input_filename, names);
  if (skip_sense && match)
    REJECT ("filename name matches in icg-filter reject file");
  if (!skip_sense && !match)
    REJECT ("filename name not in icg-filter accept file");
  return false;
}


/* Return TRUE iff ICG should not be applied to the current function
   based on flag_icg_filter_function option.  */

static bool
skip_icg_function_p  (const char *dname, const char *aname)
{
  static char **names = NULL;
  static bool skip_sense;
  bool match;

  gcc_assert (flag_icg_filter_function);
  if (!names)
    skip_sense = read_allocate_names (flag_icg_filter_function, &names);

  match = name_in_array_p (dname, names) || name_in_array_p (aname, names);
  if (skip_sense && match)
    REJECT ("function name matches in icg-filter reject file");
  if (!skip_sense && !match)
    REJECT ("function name not in icg-filter accept file");
  return false;
}

#undef REJECT

/* Return TRUE iff ICG should not be applied to the current function
   based on -ficg-filter-* flags. */

static bool
skip_icg_p (void)
{
  const char *dname = current_function_name();
  const char *aname = current_function_assembler_name();

  if (flag_icg_filter_program &&
      skip_icg_programmatic_p (dname, aname)) {
    return true;
  }

  if (flag_icg_filter_file && skip_icg_file_p (dname, aname)) {
    return true;
  }

  if (flag_icg_filter_function && skip_icg_function_p (dname, aname)) {
    return true;
  }

  fflush(stdout);
  fprintf(stderr, "ICG accepts \"%s\" (%s)\n", dname, aname);
  return false;
}

/* Decide whether to use ICG or IRA for this function and set the
   flags accordingly. */

static unsigned
rest_of_handle_icg_filter (void)
{
  if (skip_icg_p ())
    {
      flag_icg = 0;
      flag_late_codegen = 1;
      flag_ira = 1;
    }
  else
    {
      flag_icg = 1;
      flag_late_codegen = original_flag_late_codegen;
      flag_ira = 0;
    }
  return 0;
}

static bool
gate_icg_filter (void)
{
  if (original_flag_icg == -1)
    {
      original_flag_icg = flag_icg;
      original_flag_late_codegen = flag_late_codegen;
    }

  return (original_flag_icg && (flag_icg_filter_program
				|| flag_icg_filter_file
				|| flag_icg_filter_function));
}


struct rtl_opt_pass pass_icg_filter =
{
  {
    RTL_PASS,
    "icg",                                /* name */
    gate_icg_filter,                      /* gate */
    rest_of_handle_icg_filter,            /* execute */
    NULL,                                 /* sub */
    NULL,                                 /* next */
    0,                                    /* static_pass_number */
    0,                                    /* tv_id */
    0,                                    /* properties_required */
    0,                                    /* properties_provided */
    0,                                    /* properties_destroyed */
    0,                                    /* todo_flags_start */
    TODO_dump_func,                       /* todo_flags_finish */
  }
};
