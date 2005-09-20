/* OpenMP directive matching and resolving.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */


#include "config.h"
#include "system.h"
#include "flags.h"
#include "gfortran.h"
#include "match.h"
#include "parse.h"

/* Match an end of OpenMP directive.  End of OpenMP directive is optional
   whitespace, followed by '\n' or comment '!'.  */

match
gfc_match_omp_eos (void)
{
  locus old_loc;
  int c;

  old_loc = gfc_current_locus;
  gfc_gobble_whitespace ();

  c = gfc_next_char ();
  switch (c)
    {
    case '!':
      do
	c = gfc_next_char ();
      while (c != '\n');
      /* Fall through */

    case '\n':
      return MATCH_YES;
    }

  gfc_current_locus = old_loc;
  return MATCH_NO;
}

/* Free an omp_clauses structure.  */

void
gfc_free_omp_clauses (gfc_omp_clauses *c)
{
  int i;
  if (c == NULL)
    return;

  gfc_free_expr (c->if_expr);
  gfc_free_expr (c->num_threads);
  gfc_free_expr (c->chunk_size);
  for (i = 0; i < OMP_LIST_NUM; i++)
    gfc_free_namelist (c->lists[i]);
  gfc_free (c);
}

/* Match a variable/common block list and construct a namelist from it.  */

static match
gfc_match_omp_variable_list (const char *str, gfc_namelist **list)
{
  gfc_namelist *head, *tail, *p;
  locus old_loc;
  char n[GFC_MAX_SYMBOL_LEN+1];
  gfc_common_head *c;
  gfc_symbol *sym;
  match m;
  gfc_namespace *ns;

  head = tail = NULL;

  old_loc = gfc_current_locus;

  m = gfc_match (str);
  if (m != MATCH_YES)
    return m;

  for (;;)
    {
      m = gfc_match_symbol (&sym, 1);
      switch (m)
	{
	case MATCH_YES:
	  p = gfc_get_namelist ();
	  if (head == NULL)
	    head = tail = p;
	  else
	    {
	      tail->next = p;
	      tail = tail->next;
	    }
	  tail->sym = sym;
	  goto next_item;
	case MATCH_NO:
	  break;
	case MATCH_ERROR:
	  goto cleanup;
	}

      m = gfc_match (" / %n /", n);
      if (m == MATCH_ERROR)
	goto cleanup;
      if (m == MATCH_NO)
	goto syntax;

      c = NULL;
      for (ns = gfc_current_ns; ns != NULL; ns = ns->parent)
	{
	  gfc_symtree *st = gfc_find_symtree (ns->common_root, n);
	  if (st != NULL)
	    {
	      c = st->n.common;
	      break;
	    }
	}
      if (c == NULL)
	{
	  gfc_error ("COMMON block /%s/ not found at %C", n);
	  goto cleanup;
	}
      for (sym = c->head; sym; sym = sym->common_next)
	{
	  p = gfc_get_namelist ();
	  if (head == NULL)
	    head = tail = p;
	  else
	    {
	      tail->next = p;
	      tail = tail->next;
	    }
	  tail->sym = sym;
	}

    next_item:
      if (gfc_match_char (')') == MATCH_YES)
	break;
      if (gfc_match_char (',') != MATCH_YES)
	goto syntax;
    }

  while (*list)
    list = &(*list)->next;

  *list = head;
  return MATCH_YES;

syntax:
  gfc_error ("Syntax error in OpenMP variable list at %C");

cleanup:
  gfc_free_namelist (head);
  gfc_current_locus = old_loc;
  return MATCH_ERROR;
}

#define OMP_CLAUSE_PRIVATE	(1 << 0)
#define OMP_CLAUSE_FIRSTPRIVATE	(1 << 1)
#define OMP_CLAUSE_LASTPRIVATE	(1 << 2)
#define OMP_CLAUSE_COPYPRIVATE	(1 << 3)
#define OMP_CLAUSE_SHARED	(1 << 4)
#define OMP_CLAUSE_COPYIN	(1 << 5)
#define OMP_CLAUSE_REDUCTION	(1 << 6)
#define OMP_CLAUSE_IF		(1 << 7)
#define OMP_CLAUSE_NUM_THREADS	(1 << 8)
#define OMP_CLAUSE_SCHEDULE	(1 << 9)
#define OMP_CLAUSE_DEFAULT	(1 << 10)
#define OMP_CLAUSE_ORDERED	(1 << 11)

/* Match OpenMP directive clauses. MASK is a bitmask of
   clauses that are allowed for a particular directive.  */

static match
gfc_match_omp_clauses (gfc_omp_clauses **cp, int mask)
{
  gfc_omp_clauses *c = gfc_get_omp_clauses ();
  locus old_loc;
  bool needs_space = true, first = true;

  *cp = NULL;
  while (1)
    {
      if ((first || gfc_match_char (',') != MATCH_YES)
	  && (needs_space && gfc_match_space () != MATCH_YES))
	break;
      needs_space = false;
      first = false;
      gfc_gobble_whitespace ();
      if ((mask & OMP_CLAUSE_IF) && c->if_expr == NULL
	  && gfc_match ("if ( %e )", &c->if_expr) == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_NUM_THREADS) && c->num_threads == NULL
	  && gfc_match ("num_threads ( %e )", &c->num_threads) == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_PRIVATE)
	  && gfc_match_omp_variable_list ("private (",
					  &c->lists[OMP_LIST_PRIVATE])
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_FIRSTPRIVATE)
	  && gfc_match_omp_variable_list ("firstprivate (",
					  &c->lists[OMP_LIST_FIRSTPRIVATE])
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_LASTPRIVATE)
	  && gfc_match_omp_variable_list ("lastprivate (",
					  &c->lists[OMP_LIST_LASTPRIVATE])
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_COPYPRIVATE)
	  && gfc_match_omp_variable_list ("copyprivate (",
					  &c->lists[OMP_LIST_COPYPRIVATE])
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_SHARED)
	  && gfc_match_omp_variable_list ("shared (",
					  &c->lists[OMP_LIST_SHARED])
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_COPYIN)
	  && gfc_match_omp_variable_list ("copyin (",
					  &c->lists[OMP_LIST_COPYIN])
	     == MATCH_YES)
	continue;
      old_loc = gfc_current_locus;
      if ((mask & OMP_CLAUSE_REDUCTION)
	  && gfc_match ("reduction ( ") == MATCH_YES)
	{
	  int reduction = OMP_LIST_NUM;
	  if (gfc_match_char ('+') == MATCH_YES)
	    reduction = OMP_LIST_PLUS;
	  else if (gfc_match_char ('*') == MATCH_YES)
	    reduction = OMP_LIST_MULT;
	  else if (gfc_match_char ('-') == MATCH_YES)
	    reduction = OMP_LIST_SUB;
	  else if (gfc_match (".and.") == MATCH_YES)
	    reduction = OMP_LIST_AND;
	  else if (gfc_match (".or.") == MATCH_YES)
	    reduction = OMP_LIST_OR;
	  else if (gfc_match (".eqv.") == MATCH_YES)
	    reduction = OMP_LIST_EQV;
	  else if (gfc_match (".neqv.") == MATCH_YES)
	    reduction = OMP_LIST_NEQV;
	  else if (gfc_match ("max") == MATCH_YES)
	    reduction = OMP_LIST_MAX;
	  else if (gfc_match (".min.") == MATCH_YES)
	    reduction = OMP_LIST_MIN;
	  else if (gfc_match (".iand.") == MATCH_YES)
	    reduction = OMP_LIST_IAND;
	  else if (gfc_match (".ior.") == MATCH_YES)
	    reduction = OMP_LIST_IOR;
	  else if (gfc_match (".ieor.") == MATCH_YES)
	    reduction = OMP_LIST_IEOR;
	  if (reduction != OMP_LIST_NUM
	      && gfc_match_omp_variable_list (" :", &c->lists[reduction])
		 == MATCH_YES)
	    continue;
	  else
	    gfc_current_locus = old_loc;
	}
      if ((mask & OMP_CLAUSE_DEFAULT)
	  && c->default_sharing == OMP_DEFAULT_UNKNOWN)
	{
	  if (gfc_match ("default ( shared )") == MATCH_YES)
	    c->default_sharing = OMP_DEFAULT_SHARED;
	  else if (gfc_match ("default ( private )") == MATCH_YES)
	    c->default_sharing = OMP_DEFAULT_PRIVATE;
	  else if (gfc_match ("default ( none )") == MATCH_YES)
	    c->default_sharing = OMP_DEFAULT_NONE;
	  if (c->default_sharing != OMP_DEFAULT_UNKNOWN)
	    continue;
	}
      old_loc = gfc_current_locus;
      if ((mask & OMP_CLAUSE_SCHEDULE)
	  && c->sched_kind == OMP_SCHED_NONE
	  && gfc_match ("schedule ( ") == MATCH_YES)
	{
	  if (gfc_match ("static") == MATCH_YES)
	    c->sched_kind = OMP_SCHED_STATIC;
	  else if (gfc_match ("dynamic") == MATCH_YES)
	    c->sched_kind = OMP_SCHED_DYNAMIC;
	  else if (gfc_match ("guided") == MATCH_YES)
	    c->sched_kind = OMP_SCHED_GUIDED;
	  else if (gfc_match ("runtime") == MATCH_YES)
	    c->sched_kind = OMP_SCHED_RUNTIME;
	  if (c->sched_kind != OMP_SCHED_NONE)
	    {
	      match m = MATCH_NO;
	      if (c->sched_kind != OMP_SCHED_RUNTIME)
		m = gfc_match (" , %e )", &c->chunk_size);
	      if (m != MATCH_YES)
		m = gfc_match_char (')');
	      if (m != MATCH_YES)
		c->sched_kind = OMP_SCHED_NONE;
	    }
	  if (c->sched_kind != OMP_SCHED_NONE)
	    continue;
	  else
	    gfc_current_locus = old_loc;
	}
      if ((mask & OMP_CLAUSE_ORDERED) && !c->ordered
	  && gfc_match ("ordered") == MATCH_YES)
	{
	  c->ordered = needs_space = true;
	  continue;
	}

      break;
    }

  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_free_omp_clauses (c);
      return MATCH_ERROR;
    }

  *cp = c;
  return MATCH_YES;
}

#define OMP_PARALLEL_CLAUSES \
  (OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE | OMP_CLAUSE_SHARED	\
   | OMP_CLAUSE_COPYIN | OMP_CLAUSE_REDUCTION | OMP_CLAUSE_IF		\
   | OMP_CLAUSE_NUM_THREADS | OMP_CLAUSE_DEFAULT)
#define OMP_DO_CLAUSES \
  (OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE				\
   | OMP_CLAUSE_LASTPRIVATE | OMP_CLAUSE_REDUCTION			\
   | OMP_CLAUSE_SCHEDULE | OMP_CLAUSE_ORDERED)
#define OMP_SECTIONS_CLAUSES \
  (OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE				\
   | OMP_CLAUSE_LASTPRIVATE | OMP_CLAUSE_REDUCTION)

match
gfc_match_omp_parallel (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_critical (void)
{
  char n[GFC_MAX_SYMBOL_LEN+1];

  if (gfc_match (" ( %n )", n) != MATCH_YES)
    n[0] = '\0';
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_CRITICAL;
  new_st.ext.omp_name = n[0] ? xstrdup (n) : NULL;
  return MATCH_YES;
}

match
gfc_match_omp_do (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_DO_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_DO;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_flush (void)
{
  gfc_namelist *list = NULL;
  gfc_match_omp_variable_list (" (", &list);
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_free_namelist (list);
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_FLUSH;
  new_st.ext.omp_namelist = list;
  return MATCH_YES;
}

match
gfc_match_omp_threadprivate (void)
{
  gfc_namelist *list = NULL;
  gfc_match_omp_variable_list (" (", &list);
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_free_namelist (list);
      return MATCH_ERROR;
    }
  /* XXX Mark vars thread private now.  */
  gfc_free_namelist (list);
  return MATCH_YES;
}

match
gfc_match_omp_parallel_do (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES | OMP_DO_CLAUSES)
      != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL_DO;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_parallel_sections (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES | OMP_SECTIONS_CLAUSES)
      != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL_SECTIONS;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_parallel_workshare (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL_WORKSHARE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_sections (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_SECTIONS_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_SECTIONS;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_single (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE)
      != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_SINGLE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

match
gfc_match_omp_workshare (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_WORKSHARE;
  new_st.ext.omp_clauses = gfc_get_omp_clauses ();
  return MATCH_YES;
}

match
gfc_match_omp_master (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_MASTER;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}

match
gfc_match_omp_ordered (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_ORDERED;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}

match
gfc_match_omp_atomic (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_ATOMIC;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}

match
gfc_match_omp_barrier (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_BARRIER;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}

match
gfc_match_omp_end_nowait (void)
{
  bool nowait = false;
  if (gfc_match ("% nowait") == MATCH_YES)
    nowait = true;
  if (gfc_match_omp_eos () != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_END_NOWAIT;
  new_st.ext.omp_bool = nowait;
  return MATCH_YES;
}

match
gfc_match_omp_end_single (void)
{
  gfc_omp_clauses *c;
  if (gfc_match ("% nowait") == MATCH_YES)
    {
      new_st.op = EXEC_OMP_END_NOWAIT;
      new_st.ext.omp_bool = true;
      return MATCH_YES;
    }
  if (gfc_match_omp_clauses (&c, OMP_CLAUSE_COPYPRIVATE) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_END_SINGLE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}

/* OpenMP directive resolving routines.  */

static void
resolve_omp_clauses (gfc_code *code)
{
  gfc_omp_clauses *omp_clauses = code->ext.omp_clauses;

  if (omp_clauses == NULL)
    return;

  if (omp_clauses->if_expr)
    {
      gfc_expr *expr = omp_clauses->if_expr;
      if (gfc_resolve_expr (expr) == FAILURE
	  || expr->ts.type != BT_LOGICAL || expr->rank != 0)
	gfc_error ("IF clause at %L requires a scalar LOGICAL expression",
		   &expr->where);
    }
  if (omp_clauses->num_threads)
    {
      gfc_expr *expr = omp_clauses->num_threads;
      if (gfc_resolve_expr (expr) == FAILURE
	  || expr->ts.type != BT_INTEGER || expr->rank != 0)
	gfc_error ("NUM_THREADS clause at %L requires a scalar"
		   " INTEGER expression", &expr->where);
    }
  if (omp_clauses->chunk_size)
    {
      gfc_expr *expr = omp_clauses->chunk_size;
      if (gfc_resolve_expr (expr) == FAILURE
	  || expr->ts.type != BT_INTEGER || expr->rank != 0)
	gfc_error ("SCHEDULE clause's chunk_size at %L requires"
		   " a scalar INTEGER expression", &expr->where);
    }
}

/* Resolve OpenMP directive clauses and check various requirements
   of each directive.  */

void
gfc_resolve_omp_directive (gfc_code *code, gfc_namespace *ns ATTRIBUTE_UNUSED)
{
  switch (code->op)
    {
    case EXEC_OMP_DO:
    case EXEC_OMP_END_SINGLE:
    case EXEC_OMP_PARALLEL:
    case EXEC_OMP_PARALLEL_DO:
    case EXEC_OMP_PARALLEL_SECTIONS:
    case EXEC_OMP_SECTIONS:
    case EXEC_OMP_SINGLE:
    case EXEC_OMP_WORKSHARE:
    case EXEC_OMP_PARALLEL_WORKSHARE:
      if (code->ext.omp_clauses)
	resolve_omp_clauses (code);
      break;
    default:
      break;
    }
}
