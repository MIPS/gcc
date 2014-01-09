/* OpenMP directive matching and resolving.
   Copyright (C) 2005-2013 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek

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
#include "flags.h"
#include "gfortran.h"
#include "match.h"
#include "parse.h"
#include "pointer-set.h"

/* Match an end of OpenMP directive.  End of OpenMP directive is optional
   whitespace, followed by '\n' or comment '!'.  */

match
gfc_match_omp_eos (void)
{
  locus old_loc;
  char c;

  old_loc = gfc_current_locus;
  gfc_gobble_whitespace ();

  c = gfc_next_ascii_char ();
  switch (c)
    {
    case '!':
      do
	c = gfc_next_ascii_char ();
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
  gfc_free_expr (c->final_expr);
  gfc_free_expr (c->num_threads);
  gfc_free_expr (c->chunk_size);
  gfc_free_expr (c->async_expr);
  gfc_free_expr (c->gang_expr);
  gfc_free_expr (c->worker_expr);
  gfc_free_expr (c->vector_expr);
  gfc_free_expr (c->num_gangs_expr);
  gfc_free_expr (c->num_workers_expr);
  gfc_free_expr (c->vector_length_expr);
  gfc_free_expr (c->non_clause_wait_expr);

  for (i = 0; i < OACC_LIST_NUM; i++)
    gfc_free_namelist (c->lists[i]);

  gfc_free_exprlist (c->waitlist);

  free (c);
}

/* Free expression list. */
void
gfc_free_exprlist (gfc_exprlist *list)
{
  gfc_exprlist *n;

  for (; list; list = n)
    {
      n = list->next;
      free (list);
    }
}

static void
set_subarray (struct gfc_oacc_subarray **oacc_subarray,
              gfc_expr *left, gfc_expr *right, int i)
{
  if (*oacc_subarray == NULL)
    *oacc_subarray = gfc_get_oacc_subarray();
  (*oacc_subarray)->left[i] = left;
  (*oacc_subarray)->right[i] = right;
}

static match
match_subarray (struct gfc_oacc_subarray **sa)
{
  match m;

  m = gfc_match (" ( ");
  if (m == MATCH_YES)
    {
      gfc_expr *left_expr, *right_expr;
      int i;

      for (i = 0;; i++)
        {
          if (i == GFC_MAX_DIMENSIONS)
            return MATCH_ERROR;

          /* a special case for single array element */
          if (gfc_match (" %e ) ", &left_expr) == MATCH_YES)
            {
              set_subarray (sa, left_expr, left_expr, i);
              break;
            }
          if (gfc_match (" %e : %e ", &left_expr, &right_expr) == MATCH_YES)
            set_subarray (sa, left_expr, right_expr, i);
          if (gfc_match (" : %e ", &right_expr) == MATCH_YES)
            set_subarray (sa, NULL, right_expr, i);
          if (gfc_match (" %e : ", &left_expr) == MATCH_YES)
            set_subarray (sa, left_expr, NULL, i);

          if (gfc_match (" , ") == MATCH_YES)
            continue;
          else if (gfc_match (" ) ") == MATCH_YES)
            break;
          else
            return MATCH_ERROR;
        }
      (*sa)->dimensions = i + 1;
      return MATCH_YES;
    }
  else
    return m;
}

/* Match a variable/common block list and construct a namelist from it.  */

static match
gfc_match_omp_variable_list (const char *str, gfc_namelist **list,
			     bool allow_common, bool allow_subarrays)
{
  gfc_namelist *head, *tail, *p;
  locus old_loc;
  char n[GFC_MAX_SYMBOL_LEN+1];
  gfc_symbol *sym;
  match m;
  gfc_symtree *st;
  struct gfc_oacc_subarray *sa = NULL;

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
          if (allow_subarrays)
            {
              m = match_subarray (&sa);
              if (m == MATCH_ERROR)
                goto syntax;
            }
	  gfc_set_sym_referenced (sym);
	  p = gfc_get_namelist ();
	  if (head == NULL)
	    head = tail = p;
	  else
	    {
	      tail->next = p;
	      tail = tail->next;
	    }
	  tail->sym = sym;
          if (sa != NULL)
            {
              tail->oacc_subarray = sa;
              sa = NULL;
            }
	  goto next_item;
	case MATCH_NO:
	  break;
	case MATCH_ERROR:
	  goto cleanup;
	}

      if (!allow_common)
	goto syntax;

      m = gfc_match (" / %n /", n);
      if (m == MATCH_ERROR)
	goto cleanup;
      if (m == MATCH_NO)
	goto syntax;

      st = gfc_find_symtree (gfc_current_ns->common_root, n);
      if (st == NULL)
	{
	  gfc_error ("COMMON block /%s/ not found at %C", n);
	  goto cleanup;
	}
      for (sym = st->n.common->head; sym; sym = sym->common_next)
	{
	  gfc_set_sym_referenced (sym);
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
  gfc_error ("Syntax error in variable list at %C");

cleanup:
  gfc_free_namelist (head);
  gfc_current_locus = old_loc;
  return MATCH_ERROR;
}

static match
match_oacc_exprlist (const char *str, gfc_exprlist **list, bool allow_asterisk)
{
  gfc_exprlist *head, *tail, *p;
  locus old_loc;
  gfc_expr *expr;
  match m;

  head = tail = NULL;

  old_loc = gfc_current_locus;

  m = gfc_match (str);
  if (m != MATCH_YES)
    return m;

  for (;;)
    {
      m = gfc_match_expr (&expr);
      if (m == MATCH_YES || allow_asterisk)
        {
          p = gfc_get_exprlist ();
          if (head == NULL)
            head = tail = p;
          else
            {
              tail->next = p;
              tail = tail->next;
            }
          if (m == MATCH_YES)
            tail->expr = expr;
          else if (gfc_match (" *") != MATCH_YES)
            goto syntax;
          goto next_item;
        }
      if (m == MATCH_ERROR)
        goto cleanup;
      goto syntax;

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
  gfc_error ("Syntax error in OpenACC expression list at %C");

cleanup:
  gfc_free_exprlist (head);
  gfc_current_locus = old_loc;
  return MATCH_ERROR;
}

static match
match_oacc_clause_gang (gfc_oacc_clauses *cp)
{
  if (gfc_match_char ('(') != MATCH_YES)
    return MATCH_NO;
  if (gfc_match (" num :") == MATCH_YES)
    {
      cp->gang_static = false;
      return gfc_match (" %e )", &cp->gang_expr);
    }
  if (gfc_match (" static :") == MATCH_YES)
    {
      cp->gang_static = true;
      if (gfc_match (" * )") != MATCH_YES)
        return gfc_match (" %e )", &cp->gang_expr);
      return MATCH_YES;
    }
  return gfc_match (" %e )", &cp->gang_expr);
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
#define OMP_CLAUSE_COLLAPSE	(1 << 12)
#define OMP_CLAUSE_UNTIED	(1 << 13)
#define OMP_CLAUSE_FINAL	(1 << 14)
#define OMP_CLAUSE_MERGEABLE	(1 << 15)

/* OpenACC 2.0 clauses. */
#define OACC_CLAUSE_IF                   OMP_CLAUSE_IF
#define OACC_CLAUSE_ASYNC                (1 << 16)
#define OACC_CLAUSE_NUM_GANGS            (1 << 17)
#define OACC_CLAUSE_NUM_WORKERS          (1 << 18)
#define OACC_CLAUSE_VECTOR_LENGTH        (1 << 19)
#define OACC_CLAUSE_REDUCTION            OMP_CLAUSE_REDUCTION
#define OACC_CLAUSE_COPY                 (1 << 20)
#define OACC_CLAUSE_COPYIN               OMP_CLAUSE_COPYIN
#define OACC_CLAUSE_COPYOUT              (1 << 21)
#define OACC_CLAUSE_CREATE               (1 << 22)
#define OACC_CLAUSE_PRESENT              (1 << 23)
#define OACC_CLAUSE_PRESENT_OR_COPY      (1 << 24)
#define OACC_CLAUSE_PRESENT_OR_COPYIN    (1 << 25)
#define OACC_CLAUSE_PRESENT_OR_COPYOUT   (1 << 26)
#define OACC_CLAUSE_PRESENT_OR_CREATE    (1 << 27)
#define OACC_CLAUSE_DEVICEPTR            (1 << 28)
#define OACC_CLAUSE_PRIVATE              OMP_CLAUSE_PRIVATE
#define OACC_CLAUSE_FIRSTPRIVATE         OMP_CLAUSE_FIRSTPRIVATE
#define OACC_CLAUSE_COLLAPSE             OMP_CLAUSE_COLLAPSE
#define OACC_CLAUSE_GANG                 (1 << 29)
#define OACC_CLAUSE_WORKER               (1 << 30)
#define OACC_CLAUSE_VECTOR               (1 << 31)
#define OACC_CLAUSE_SEQ                  (1ll << 32)
#define OACC_CLAUSE_INDEPENDENT          (1ll << 33)
#define OACC_CLAUSE_USE_DEVICE           (1ll << 34)
#define OACC_CLAUSE_DEVICE_RESIDENT      (1ll << 35)
#define OACC_CLAUSE_HOST                 (1ll << 36)
#define OACC_CLAUSE_DEVICE               (1ll << 37)
#define OACC_CLAUSE_DEFAULT              (1ll << 38)
#define OACC_CLAUSE_WAIT                 (1ll << 39)
#define OACC_CLAUSE_DELETE               (1ll << 40)
#define OACC_CLAUSE_AUTO                 (1ll << 41)
#define OACC_CLAUSE_TILE                 (1ll << 42)


/* Match OpenMP directive clauses. MASK is a bitmask of
   clauses that are allowed for a particular directive.  */

static match
gfc_match_omp_clauses (gfc_omp_clauses **cp, long long mask, bool is_acc)
{
  gfc_omp_clauses *c = gfc_get_omp_clauses ();
  locus old_loc;
  bool needs_space = true, first = true;
  c->is_acc = is_acc;

  *cp = NULL;
  while (1)
    {
      if ((first || gfc_match_char (',') != MATCH_YES)
	  && (needs_space && gfc_match_space () != MATCH_YES))
	break;
      needs_space = false;
      first = false;
      gfc_gobble_whitespace ();
      if ((mask & OACC_CLAUSE_ASYNC) && !c->async)
        if (gfc_match ("async") == MATCH_YES)
          {
            c->async = true;
            if (gfc_match (" ( %e )", &c->async_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & OACC_CLAUSE_GANG) && !c->gang)
        if (gfc_match ("gang") == MATCH_YES)
          {
            c->gang = true;
            if (match_oacc_clause_gang(c) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & OACC_CLAUSE_WORKER) && !c->worker)
        if (gfc_match ("worker") == MATCH_YES)
          {
            c->worker = true;
            if (gfc_match (" ( num : %e )", &c->worker_expr) == MATCH_YES
                || gfc_match (" ( %e )", &c->worker_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & OACC_CLAUSE_VECTOR_LENGTH) && c->vector_length_expr == NULL
          && gfc_match ("vector_length ( %e )", &c->vector_length_expr)
          == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_VECTOR) && !c->vector)
        if (gfc_match ("vector") == MATCH_YES)
          {
            c->vector = true;
            if (gfc_match (" ( length : %e )", &c->vector_expr) == MATCH_YES
                || gfc_match (" ( %e )", &c->vector_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & OMP_CLAUSE_IF) && c->if_expr == NULL
	  && gfc_match ("if ( %e )", &c->if_expr) == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_FINAL) && c->final_expr == NULL
	  && gfc_match ("final ( %e )", &c->final_expr) == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_NUM_THREADS) && c->num_threads == NULL
	  && gfc_match ("num_threads ( %e )", &c->num_threads) == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_PRIVATE)
	  && gfc_match_omp_variable_list ("private (",
					  &c->lists[OMP_LIST_PRIVATE], true,
					  false)
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_FIRSTPRIVATE)
	  && gfc_match_omp_variable_list ("firstprivate (",
					  &c->lists[OMP_LIST_FIRSTPRIVATE],
					  true, false)
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_LASTPRIVATE)
	  && gfc_match_omp_variable_list ("lastprivate (",
					  &c->lists[OMP_LIST_LASTPRIVATE],
					  true, false)
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_COPYPRIVATE)
	  && gfc_match_omp_variable_list ("copyprivate (",
					  &c->lists[OMP_LIST_COPYPRIVATE],
					  true, false)
	     == MATCH_YES)
	continue;
      if ((mask & OMP_CLAUSE_SHARED)
	  && gfc_match_omp_variable_list ("shared (",
					  &c->lists[OMP_LIST_SHARED], true,
					  false)
	     == MATCH_YES)
	continue;
      if (!c->is_acc && (mask & OMP_CLAUSE_COPYIN)
	  && gfc_match_omp_variable_list ("copyin (",
					  &c->lists[OMP_LIST_COPYIN], true,
					  false)
	     == MATCH_YES)
	continue;
      if (c->is_acc && (mask & OACC_CLAUSE_COPYIN)
          && gfc_match_omp_variable_list ("copyin (",
                                          &c->lists[OACC_LIST_COPYIN], true,
                                          true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_NUM_GANGS) && c->num_gangs_expr == NULL
          && gfc_match ("num_gangs ( %e )", &c->num_gangs_expr) == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_NUM_WORKERS) && c->num_workers_expr == NULL
          && gfc_match ("num_workers ( %e )", &c->num_workers_expr)
          == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_COPY)
          && gfc_match_omp_variable_list ("copy (",
                                      &c->lists[OACC_LIST_COPY], true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_COPYOUT)
          && gfc_match_omp_variable_list ("copyout (",
                                      &c->lists[OACC_LIST_COPYOUT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_CREATE)
          && gfc_match_omp_variable_list ("create (",
                                      &c->lists[OACC_LIST_CREATE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_DELETE)
          && gfc_match_omp_variable_list ("delete (",
                                      &c->lists[OACC_LIST_DELETE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT)
          && gfc_match_omp_variable_list ("present (",
                                      &c->lists[OACC_LIST_PRESENT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPY)
          && gfc_match_omp_variable_list ("present_or_copy (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPY], true,
                                      true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPY)
          && gfc_match_omp_variable_list ("pcopy (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPY], true,
                                      true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPYIN)
          && gfc_match_omp_variable_list ("present_or_copyin (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPYIN],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPYIN)
          && gfc_match_omp_variable_list ("pcopyin (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPYIN],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPYOUT)
          && gfc_match_omp_variable_list ("present_or_copyout (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPYOUT],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_COPYOUT)
          && gfc_match_omp_variable_list ("pcopyout (",
                                      &c->lists[OACC_LIST_PRESENT_OR_COPYOUT],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_CREATE)
          && gfc_match_omp_variable_list ("present_or_create (",
                                      &c->lists[OACC_LIST_PRESENT_OR_CREATE],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_PRESENT_OR_CREATE)
          && gfc_match_omp_variable_list ("pcreate (",
                                      &c->lists[OACC_LIST_PRESENT_OR_CREATE],
                                      true, true)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_DEVICEPTR)
          && gfc_match_omp_variable_list ("deviceptr (",
                                      &c->lists[OACC_LIST_DEVICEPTR], true,
                                      false)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_USE_DEVICE)
          && gfc_match_omp_variable_list ("use_device (",
                                      &c->lists[OACC_LIST_USE_DEVICE], true,
                                      false)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_DEVICE_RESIDENT)
          && gfc_match_omp_variable_list ("device_resident (",
                                      &c->lists[OACC_LIST_DEVICE_RESIDENT], true,
                                      false)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_HOST)
          && gfc_match_omp_variable_list ("host (",
                                      &c->lists[OACC_LIST_HOST], true, false)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_DEVICE)
          && gfc_match_omp_variable_list ("device (",
                                      &c->lists[OACC_LIST_DEVICE], true, false)
             == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_TILE)
          && match_oacc_exprlist ("tile (", &c->tilelist, true) == MATCH_YES)
        continue;
      if ((mask & OACC_CLAUSE_SEQ) && !c->seq
          && gfc_match ("seq") == MATCH_YES)
        {
          c->seq = true;
          needs_space = true;
          continue;
        }
      if ((mask & OACC_CLAUSE_INDEPENDENT) && !c->independent
          && gfc_match ("independent") == MATCH_YES)
        {
          c->independent = true;
          needs_space = true;
          continue;
        }
      if ((mask & OACC_CLAUSE_DEFAULT) && !c->default_none
                && gfc_match ("default ( none )") == MATCH_YES)
        {
          c->default_none = true;
          needs_space = true;
          continue;
        }
      if ((mask & OACC_CLAUSE_AUTO) && !c->par_auto
                && gfc_match ("auto") == MATCH_YES)
        {
          c->par_auto = true;
          needs_space = true;
          continue;
        }
      if ((mask & OACC_CLAUSE_WAIT) && !c->wait
                && gfc_match ("wait") == MATCH_YES)
        {
          c->wait = true;
          match_oacc_exprlist (" (", &c->waitlist, false);
          continue;
        }
      old_loc = gfc_current_locus;
      if ((mask & OMP_CLAUSE_REDUCTION)
	  && gfc_match ("reduction ( ") == MATCH_YES)
	{
	  int reduction = OMP_LIST_NUM;
	  char buffer[GFC_MAX_SYMBOL_LEN + 1];
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
	  else if (gfc_match_name (buffer) == MATCH_YES)
	    {
	      gfc_symbol *sym;
	      const char *n = buffer;

	      gfc_find_symbol (buffer, NULL, 1, &sym);
	      if (sym != NULL)
		{
		  if (sym->attr.intrinsic)
		    n = sym->name;
		  else if ((sym->attr.flavor != FL_UNKNOWN
			    && sym->attr.flavor != FL_PROCEDURE)
			   || sym->attr.external
			   || sym->attr.generic
			   || sym->attr.entry
			   || sym->attr.result
			   || sym->attr.dummy
			   || sym->attr.subroutine
			   || sym->attr.pointer
			   || sym->attr.target
			   || sym->attr.cray_pointer
			   || sym->attr.cray_pointee
			   || (sym->attr.proc != PROC_UNKNOWN
			       && sym->attr.proc != PROC_INTRINSIC)
			   || sym->attr.if_source != IFSRC_UNKNOWN
			   || sym == sym->ns->proc_name)
		    {
		      gfc_error_now ("%s is not INTRINSIC procedure name "
				     "at %C", buffer);
		      sym = NULL;
		    }
		  else
		    n = sym->name;
		}
	      if (strcmp (n, "max") == 0)
		reduction = OMP_LIST_MAX;
	      else if (strcmp (n, "min") == 0)
		reduction = OMP_LIST_MIN;
	      else if (strcmp (n, "iand") == 0)
		reduction = OMP_LIST_IAND;
	      else if (strcmp (n, "ior") == 0)
		reduction = OMP_LIST_IOR;
	      else if (strcmp (n, "ieor") == 0)
		reduction = OMP_LIST_IEOR;
	      if (reduction != OMP_LIST_NUM
		  && sym != NULL
		  && ! sym->attr.intrinsic
		  && ! sym->attr.use_assoc
		  && ((sym->attr.flavor == FL_UNKNOWN
		       && !gfc_add_flavor (&sym->attr, FL_PROCEDURE, sym->name,
		                           NULL))
		      || !gfc_add_intrinsic (&sym->attr, NULL)))
		{
		  gfc_free_omp_clauses (c);
		  return MATCH_ERROR;
		}
	    }
	  if (reduction != OMP_LIST_NUM
	      && gfc_match_omp_variable_list (" :", &c->lists[reduction],
					      false, false)
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
	  else if (gfc_match ("default ( firstprivate )") == MATCH_YES)
	    c->default_sharing = OMP_DEFAULT_FIRSTPRIVATE;
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
	  else if (gfc_match ("auto") == MATCH_YES)
	    c->sched_kind = OMP_SCHED_AUTO;
	  if (c->sched_kind != OMP_SCHED_NONE)
	    {
	      match m = MATCH_NO;
	      if (c->sched_kind != OMP_SCHED_RUNTIME
		  && c->sched_kind != OMP_SCHED_AUTO)
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
      if ((mask & OMP_CLAUSE_UNTIED) && !c->untied
	  && gfc_match ("untied") == MATCH_YES)
	{
	  c->untied = needs_space = true;
	  continue;
	}
      if ((mask & OMP_CLAUSE_MERGEABLE) && !c->mergeable
	  && gfc_match ("mergeable") == MATCH_YES)
	{
	  c->mergeable = needs_space = true;
	  continue;
	}
      if ((mask & OMP_CLAUSE_COLLAPSE) && !c->collapse)
	{
	  gfc_expr *cexpr = NULL;
	  match m = gfc_match ("collapse ( %e )", &cexpr);

	  if (m == MATCH_YES)
	    {
	      int collapse;
	      const char *p = gfc_extract_int (cexpr, &collapse);
	      if (p)
		{
		  gfc_error_now (p);
		  collapse = 1;
		}
	      else if (collapse <= 0)
		{
		  gfc_error_now ("COLLAPSE clause argument not"
				 " constant positive integer at %C");
		  collapse = 1;
		}
	      c->collapse = collapse;
	      gfc_free_expr (cexpr);
	      continue;
	    }
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
   | OMP_CLAUSE_SCHEDULE | OMP_CLAUSE_ORDERED | OMP_CLAUSE_COLLAPSE)
#define OMP_SECTIONS_CLAUSES \
  (OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE				\
   | OMP_CLAUSE_LASTPRIVATE | OMP_CLAUSE_REDUCTION)
#define OMP_TASK_CLAUSES \
  (OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE | OMP_CLAUSE_SHARED	\
   | OMP_CLAUSE_IF | OMP_CLAUSE_DEFAULT | OMP_CLAUSE_UNTIED		\
   | OMP_CLAUSE_FINAL | OMP_CLAUSE_MERGEABLE)

#define OACC_PARALLEL_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_ASYNC | OACC_CLAUSE_NUM_GANGS                    \
   | OACC_CLAUSE_NUM_WORKERS | OACC_CLAUSE_VECTOR_LENGTH | OACC_CLAUSE_REDUCTION \
   | OACC_CLAUSE_COPY | OACC_CLAUSE_COPYIN | OACC_CLAUSE_COPYOUT                 \
   | OACC_CLAUSE_CREATE | OACC_CLAUSE_PRESENT | OACC_CLAUSE_PRESENT_OR_COPY      \
   | OACC_CLAUSE_PRESENT_OR_COPYIN | OACC_CLAUSE_PRESENT_OR_COPYOUT             \
   | OACC_CLAUSE_PRESENT_OR_CREATE | OACC_CLAUSE_DEVICEPTR | OACC_CLAUSE_PRIVATE \
   | OACC_CLAUSE_FIRSTPRIVATE | OACC_CLAUSE_DEFAULT | OACC_CLAUSE_WAIT)
#define OACC_KERNELS_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_ASYNC | OACC_CLAUSE_DEVICEPTR                    \
   | OACC_CLAUSE_COPY | OACC_CLAUSE_COPYIN | OACC_CLAUSE_COPYOUT                 \
   | OACC_CLAUSE_CREATE | OACC_CLAUSE_PRESENT | OACC_CLAUSE_PRESENT_OR_COPY      \
   | OACC_CLAUSE_PRESENT_OR_COPYIN | OACC_CLAUSE_PRESENT_OR_COPYOUT             \
   | OACC_CLAUSE_PRESENT_OR_CREATE | OACC_CLAUSE_DEFAULT | OACC_CLAUSE_WAIT)
#define OACC_DATA_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_DEVICEPTR  | OACC_CLAUSE_COPY | OACC_CLAUSE_COPYIN \
   | OACC_CLAUSE_COPYOUT | OACC_CLAUSE_CREATE | OACC_CLAUSE_PRESENT               \
   | OACC_CLAUSE_PRESENT_OR_COPY | OACC_CLAUSE_PRESENT_OR_COPYIN                 \
   | OACC_CLAUSE_PRESENT_OR_COPYOUT | OACC_CLAUSE_PRESENT_OR_CREATE)
#define OACC_LOOP_CLAUSES \
  (OACC_CLAUSE_COLLAPSE | OACC_CLAUSE_GANG | OACC_CLAUSE_WORKER     \
   | OACC_CLAUSE_VECTOR | OACC_CLAUSE_SEQ | OACC_CLAUSE_INDEPENDENT \
   | OACC_CLAUSE_PRIVATE | OACC_CLAUSE_REDUCTION | OACC_CLAUSE_AUTO \
   | OACC_CLAUSE_TILE)
#define OACC_PARALLEL_LOOP_CLAUSES \
  (OACC_LOOP_CLAUSES | OACC_PARALLEL_CLAUSES)
#define OACC_KERNELS_LOOP_CLAUSES \
  (OACC_LOOP_CLAUSES | OACC_KERNELS_CLAUSES)
#define OACC_HOST_DATA_CLAUSES OACC_CLAUSE_USE_DEVICE
#define OACC_DECLARE_CLAUSES \
  (OACC_CLAUSE_COPY | OACC_CLAUSE_COPYIN | OACC_CLAUSE_COPYOUT                \
   | OACC_CLAUSE_CREATE | OACC_CLAUSE_DEVICEPTR | OACC_CLAUSE_DEVICE_RESIDENT \
   | OACC_CLAUSE_PRESENT | OACC_CLAUSE_PRESENT_OR_COPY                       \
   | OACC_CLAUSE_PRESENT_OR_COPYIN | OACC_CLAUSE_PRESENT_OR_COPYOUT          \
   | OACC_CLAUSE_PRESENT_OR_CREATE)
#define OACC_UPDATE_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_ASYNC | OACC_CLAUSE_HOST | OACC_CLAUSE_DEVICE)
#define OACC_ENTER_DATA_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_ASYNC | OACC_CLAUSE_WAIT | OACC_CLAUSE_COPYIN \
   | OACC_CLAUSE_CREATE | OACC_CLAUSE_PRESENT_OR_COPYIN                     \
   | OACC_CLAUSE_PRESENT_OR_CREATE)
#define OACC_EXIT_DATA_CLAUSES \
  (OACC_CLAUSE_IF | OACC_CLAUSE_ASYNC | OACC_CLAUSE_WAIT | OACC_CLAUSE_COPYOUT \
   | OACC_CLAUSE_DELETE)


match
gfc_match_omp_parallel (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_omp_task (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_TASK_CLAUSES, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_TASK;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_omp_taskwait (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after TASKWAIT clause at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_TASKWAIT;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}


match
gfc_match_omp_taskyield (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after TASKYIELD clause at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_TASKYIELD;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}


match
gfc_match_omp_critical (void)
{
  char n[GFC_MAX_SYMBOL_LEN+1];

  if (gfc_match (" ( %n )", n) != MATCH_YES)
    n[0] = '\0';
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP CRITICAL statement at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_CRITICAL;
  new_st.ext.omp_name = n[0] ? xstrdup (n) : NULL;
  return MATCH_YES;
}


match
gfc_match_omp_do (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_DO_CLAUSES, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_DO;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_omp_flush (void)
{
  gfc_namelist *list = NULL;
  gfc_match_omp_variable_list (" (", &list, true, false);
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP FLUSH statement at %C");
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
  locus old_loc;
  char n[GFC_MAX_SYMBOL_LEN+1];
  gfc_symbol *sym;
  match m;
  gfc_symtree *st;

  old_loc = gfc_current_locus;

  m = gfc_match (" (");
  if (m != MATCH_YES)
    return m;

  for (;;)
    {
      m = gfc_match_symbol (&sym, 0);
      switch (m)
	{
	case MATCH_YES:
	  if (sym->attr.in_common)
	    gfc_error_now ("Threadprivate variable at %C is an element of "
			   "a COMMON block");
	  else if (!gfc_add_threadprivate (&sym->attr, sym->name,
	           &sym->declared_at))
	    goto cleanup;
	  goto next_item;
	case MATCH_NO:
	  break;
	case MATCH_ERROR:
	  goto cleanup;
	}

      m = gfc_match (" / %n /", n);
      if (m == MATCH_ERROR)
	goto cleanup;
      if (m == MATCH_NO || n[0] == '\0')
	goto syntax;

      st = gfc_find_symtree (gfc_current_ns->common_root, n);
      if (st == NULL)
	{
	  gfc_error ("COMMON block /%s/ not found at %C", n);
	  goto cleanup;
	}
      st->n.common->threadprivate = 1;
      for (sym = st->n.common->head; sym; sym = sym->common_next)
	if (!gfc_add_threadprivate (&sym->attr, sym->name, &sym->declared_at))
	  goto cleanup;

    next_item:
      if (gfc_match_char (')') == MATCH_YES)
	break;
      if (gfc_match_char (',') != MATCH_YES)
	goto syntax;
    }

  return MATCH_YES;

syntax:
  gfc_error ("Syntax error in !$OMP THREADPRIVATE list at %C");

cleanup:
  gfc_current_locus = old_loc;
  return MATCH_ERROR;
}


match
gfc_match_omp_parallel_do (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES | OMP_DO_CLAUSES, false)
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
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES | OMP_SECTIONS_CLAUSES,
                             false)
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
  if (gfc_match_omp_clauses (&c, OMP_PARALLEL_CLAUSES, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_PARALLEL_WORKSHARE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_omp_sections (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_SECTIONS_CLAUSES, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_SECTIONS;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_omp_single (void)
{
  gfc_omp_clauses *c;
  if (gfc_match_omp_clauses (&c, OMP_CLAUSE_PRIVATE | OMP_CLAUSE_FIRSTPRIVATE,
                             false)
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
    {
      gfc_error ("Unexpected junk after $OMP WORKSHARE statement at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_WORKSHARE;
  new_st.ext.omp_clauses = gfc_get_omp_clauses ();
  return MATCH_YES;
}


match
gfc_match_omp_master (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP MASTER statement at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_MASTER;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}


match
gfc_match_omp_ordered (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP ORDERED statement at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_ORDERED;
  new_st.ext.omp_clauses = NULL;
  return MATCH_YES;
}


match
gfc_match_omp_atomic (void)
{
  gfc_omp_atomic_op op = GFC_OMP_ATOMIC_UPDATE;
  if (gfc_match ("% update") == MATCH_YES)
    op = GFC_OMP_ATOMIC_UPDATE;
  else if (gfc_match ("% read") == MATCH_YES)
    op = GFC_OMP_ATOMIC_READ;
  else if (gfc_match ("% write") == MATCH_YES)
    op = GFC_OMP_ATOMIC_WRITE;
  else if (gfc_match ("% capture") == MATCH_YES)
    op = GFC_OMP_ATOMIC_CAPTURE;
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP ATOMIC statement at %C");
      return MATCH_ERROR;
    }
  new_st.op = EXEC_OMP_ATOMIC;
  new_st.ext.omp_atomic = op;
  return MATCH_YES;
}


match
gfc_match_omp_barrier (void)
{
  if (gfc_match_omp_eos () != MATCH_YES)
    {
      gfc_error ("Unexpected junk after $OMP BARRIER statement at %C");
      return MATCH_ERROR;
    }
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
    {
      gfc_error ("Unexpected junk after NOWAIT clause at %C");
      return MATCH_ERROR;
    }
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
  if (gfc_match_omp_clauses (&c, OMP_CLAUSE_COPYPRIVATE, false) != MATCH_YES)
    return MATCH_ERROR;
  new_st.op = EXEC_OMP_END_SINGLE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_parallel_loop (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_PARALLEL_LOOP_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_PARALLEL_LOOP;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_parallel (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_PARALLEL_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_PARALLEL;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_kernels_loop (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_KERNELS_LOOP_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_KERNELS_LOOP;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_kernels (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_KERNELS_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_KERNELS;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_data (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_DATA_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_DATA;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_host_data (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_HOST_DATA_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_HOST_DATA;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_loop (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_LOOP_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_LOOP;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_declare (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_DECLARE_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_update (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_UPDATE_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_UPDATE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_enter_data (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_ENTER_DATA_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_ENTER_DATA;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_exit_data (void)
{
  gfc_oacc_clauses *c;
  if (gfc_match_omp_clauses (&c, OACC_EXIT_DATA_CLAUSES, true) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_OACC_EXIT_DATA;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_wait (void)
{
  gfc_oacc_clauses *c = gfc_get_oacc_clauses ();
  gfc_match (" ( %e )", &c->non_clause_wait_expr);

  new_st.op = EXEC_OACC_WAIT;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}


match
gfc_match_oacc_cache (void)
{
  gfc_oacc_clauses *c = gfc_get_oacc_clauses ();
  match m = gfc_match_omp_variable_list (" (",&c->lists[OACC_LIST_CACHE], true,
                                     true);
  if (m != MATCH_YES)
    {
      gfc_free_omp_clauses(c);
      return m;
    }

  if (gfc_current_state() != COMP_DO)
    {
      gfc_error ("ACC CACHE directive must be inside of loop %C");
      gfc_free_omp_clauses(c);
      return MATCH_ERROR;
    }

  new_st.op = EXEC_OACC_CACHE;
  new_st.ext.omp_clauses = c;
  return MATCH_YES;
}



static void
resolve_oacc_scalar_int_expr (gfc_expr *expr, const char *clause)
{
  if (!gfc_resolve_expr (expr)
      || expr->ts.type != BT_INTEGER || expr->rank != 0)
    gfc_error ("%s clause at %L requires a scalar INTEGER expression",
                     clause, &expr->where);
}


static void
resolve_oacc_positive_int_expr (gfc_expr *expr, const char *clause)
{
  resolve_oacc_scalar_int_expr (expr, clause);
  if (expr->expr_type == EXPR_CONSTANT && expr->ts.type == BT_INTEGER
      && expr->value.integer->_mp_size <= 0)
    gfc_warning ("INTEGER expression of %s clause at %L must be positive",
                     clause, &expr->where);
}


static void
resolve_subarray (gfc_namelist *n, locus *where)
{
  struct gfc_oacc_subarray *sa = n->oacc_subarray;
  gfc_symbol *sym = n->sym;
  int i;

  if (sym->as->rank <= 0)
    gfc_error ("subarray of variable %L", where);
  if (sym->as->rank != sa->dimensions)
    gfc_error ("Number of dimensions of array not equal to rank %L", where);
  for (i = 0; i < sa->dimensions; i++)
    {
      if (i < sa->dimensions - 1)
        if (sa->left[i] == NULL || sa->right[i] == NULL)
          gfc_warning ("All dimensions of subarray except last must specify "
                       "whole dimension %L", where);

      if (sa->left[i] == NULL)
        sa->left[i] = gfc_copy_expr (sym->as->lower[i]);
      if (sa->right[i] == NULL)
        sa->right[i] = gfc_copy_expr (sym->as->upper[i]);

      if (!gfc_resolve_expr(sa->left[i])
          || sa->left[i]->ts.type != BT_INTEGER || sa->left[i]->rank != 0
          || !gfc_resolve_expr(sa->right[i])
          || sa->right[i]->ts.type != BT_INTEGER || sa->right[i]->rank != 0)
        gfc_error ("Bound must be a scalar INTEGER expression %L", where);
    }
}


/* OpenMP directive resolving routines.  */

static void
resolve_omp_clauses (gfc_code *code)
{
  gfc_omp_clauses *omp_clauses = code->ext.omp_clauses;
  gfc_namelist *n;
  gfc_oacc_clauses *oacc_clauses = omp_clauses;
  gfc_exprlist *el;
  int list;
  static const char *clause_names[]
    = { "PRIVATE", "FIRSTPRIVATE", "LASTPRIVATE", "COPYPRIVATE", "SHARED",
	"COPYIN", "REDUCTION"};
  static const char *oacc_clause_names[]
    = { "COPY", "COPYIN", "COPYOUT", "CREATE", "DELETE",
        "PRESENT", "PRESENT_OR_COPY", "PRESENT_OR_COPYIN", "PRESENT_OR_COPYOUT",
        "PRESENT_OR_CREATE", "DEVICEPTR", "USE_DEVICE", "DEVICE_RESIDENT",
        "HOST", "DEVICE", "CACHE"};

  if (omp_clauses == NULL)
    return;

  if (omp_clauses->if_expr)
    {
      gfc_expr *expr = omp_clauses->if_expr;
      if (!gfc_resolve_expr (expr)
	  || expr->ts.type != BT_LOGICAL || expr->rank != 0)
	gfc_error ("IF clause at %L requires a scalar LOGICAL expression",
		   &expr->where);
    }
  if (omp_clauses->final_expr)
    {
      gfc_expr *expr = omp_clauses->final_expr;
      if (!gfc_resolve_expr (expr)
	  || expr->ts.type != BT_LOGICAL || expr->rank != 0)
	gfc_error ("FINAL clause at %L requires a scalar LOGICAL expression",
		   &expr->where);
    }
  if (omp_clauses->num_threads)
    {
      gfc_expr *expr = omp_clauses->num_threads;
      if (!gfc_resolve_expr (expr)
	  || expr->ts.type != BT_INTEGER || expr->rank != 0)
	gfc_error ("NUM_THREADS clause at %L requires a scalar "
		   "INTEGER expression", &expr->where);
    }
  if (omp_clauses->chunk_size)
    {
      gfc_expr *expr = omp_clauses->chunk_size;
      if (!gfc_resolve_expr (expr)
	  || expr->ts.type != BT_INTEGER || expr->rank != 0)
	gfc_error ("SCHEDULE clause's chunk_size at %L requires "
		   "a scalar INTEGER expression", &expr->where);
    }

  /* Check that no symbol appears on multiple clauses, except that
     a symbol can appear on both firstprivate and lastprivate.  */
  for (list = 0; list < OACC_LIST_NUM; list++)
    for (n = omp_clauses->lists[list]; n; n = n->next)
      {
	n->sym->mark = 0;
	if (n->sym->attr.flavor == FL_VARIABLE || n->sym->attr.proc_pointer)
	  continue;
	if (n->sym->attr.flavor == FL_PROCEDURE
	    && n->sym->result == n->sym
	    && n->sym->attr.function)
	  {
	    if (gfc_current_ns->proc_name == n->sym
		|| (gfc_current_ns->parent
		    && gfc_current_ns->parent->proc_name == n->sym))
	      continue;
	    if (gfc_current_ns->proc_name->attr.entry_master)
	      {
		gfc_entry_list *el = gfc_current_ns->entries;
		for (; el; el = el->next)
		  if (el->sym == n->sym)
		    break;
		if (el)
		  continue;
	      }
	    if (gfc_current_ns->parent
		&& gfc_current_ns->parent->proc_name->attr.entry_master)
	      {
		gfc_entry_list *el = gfc_current_ns->parent->entries;
		for (; el; el = el->next)
		  if (el->sym == n->sym)
		    break;
		if (el)
		  continue;
	      }
	  }
	gfc_error ("Object '%s' is not a variable at %L", n->sym->name,
		   &code->loc);
      }

  for (list = 0; list < OMP_LIST_NUM; list++)
    if (list != OMP_LIST_FIRSTPRIVATE && list != OMP_LIST_LASTPRIVATE)
      for (n = omp_clauses->lists[list]; n; n = n->next)
	{
	  if (n->sym->mark)
	    gfc_error ("Symbol '%s' present on multiple clauses at %L",
		       n->sym->name, &code->loc);
	  else
	    n->sym->mark = 1;
	}

  gcc_assert (OMP_LIST_LASTPRIVATE == OMP_LIST_FIRSTPRIVATE + 1);
  for (list = OMP_LIST_FIRSTPRIVATE; list <= OMP_LIST_LASTPRIVATE; list++)
    for (n = omp_clauses->lists[list]; n; n = n->next)
      if (n->sym->mark)
	{
	  gfc_error ("Symbol '%s' present on multiple clauses at %L",
		     n->sym->name, &code->loc);
	  n->sym->mark = 0;
	}

  for (n = omp_clauses->lists[OMP_LIST_FIRSTPRIVATE]; n; n = n->next)
    {
      if (n->sym->mark)
	gfc_error ("Symbol '%s' present on multiple clauses at %L",
		   n->sym->name, &code->loc);
      else
	n->sym->mark = 1;
    }
  for (n = omp_clauses->lists[OMP_LIST_LASTPRIVATE]; n; n = n->next)
    n->sym->mark = 0;

  for (n = omp_clauses->lists[OMP_LIST_LASTPRIVATE]; n; n = n->next)
    {
      if (n->sym->mark)
	gfc_error ("Symbol '%s' present on multiple clauses at %L",
		   n->sym->name, &code->loc);
      else
	n->sym->mark = 1;
    }

  for (list = 0; list < OMP_LIST_NUM; list++)
    if ((n = omp_clauses->lists[list]) != NULL)
      {
	const char *name;

	if (list < OMP_LIST_REDUCTION_FIRST)
	  name = clause_names[list];
	else if (list <= OMP_LIST_REDUCTION_LAST)
	  name = clause_names[OMP_LIST_REDUCTION_FIRST];
	else
	  gcc_unreachable ();

	switch (list)
	  {
	  case OMP_LIST_COPYIN:
	    for (; n != NULL; n = n->next)
	      {
		if (!n->sym->attr.threadprivate)
		  gfc_error ("Non-THREADPRIVATE object '%s' in COPYIN clause"
			     " at %L", n->sym->name, &code->loc);
		if (n->sym->ts.type == BT_DERIVED && n->sym->ts.u.derived->attr.alloc_comp)
		  gfc_error ("COPYIN clause object '%s' at %L has ALLOCATABLE components",
			     n->sym->name, &code->loc);
	      }
	    break;
	  case OMP_LIST_COPYPRIVATE:
	    for (; n != NULL; n = n->next)
	      {
		if (n->sym->as && n->sym->as->type == AS_ASSUMED_SIZE)
		  gfc_error ("Assumed size array '%s' in COPYPRIVATE clause "
			     "at %L", n->sym->name, &code->loc);
		if (n->sym->ts.type == BT_DERIVED && n->sym->ts.u.derived->attr.alloc_comp)
		  gfc_error ("COPYPRIVATE clause object '%s' at %L has ALLOCATABLE components",
			     n->sym->name, &code->loc);
	      }
	    break;
	  case OMP_LIST_SHARED:
	    for (; n != NULL; n = n->next)
	      {
		if (n->sym->attr.threadprivate)
		  gfc_error ("THREADPRIVATE object '%s' in SHARED clause at "
			     "%L", n->sym->name, &code->loc);
		if (n->sym->attr.cray_pointee)
		  gfc_error ("Cray pointee '%s' in SHARED clause at %L",
			    n->sym->name, &code->loc);
	      }
	    break;
	  default:
	    for (; n != NULL; n = n->next)
	      {
		if (n->sym->attr.threadprivate)
		  gfc_error ("THREADPRIVATE object '%s' in %s clause at %L",
			     n->sym->name, name, &code->loc);
		if (n->sym->attr.cray_pointee)
		  gfc_error ("Cray pointee '%s' in %s clause at %L",
			    n->sym->name, name, &code->loc);
		if (list != OMP_LIST_PRIVATE)
		  {
		    if (n->sym->attr.pointer
			&& list >= OMP_LIST_REDUCTION_FIRST
			&& list <= OMP_LIST_REDUCTION_LAST)
		      gfc_error ("POINTER object '%s' in %s clause at %L",
				 n->sym->name, name, &code->loc);
		    /* Variables in REDUCTION-clauses must be of intrinsic type (flagged below).  */
		    if ((list < OMP_LIST_REDUCTION_FIRST || list > OMP_LIST_REDUCTION_LAST)
			 && n->sym->ts.type == BT_DERIVED
			 && n->sym->ts.u.derived->attr.alloc_comp)
		      gfc_error ("%s clause object '%s' has ALLOCATABLE components at %L",
				 name, n->sym->name, &code->loc);
		    if (n->sym->attr.cray_pointer
			&& list >= OMP_LIST_REDUCTION_FIRST
			&& list <= OMP_LIST_REDUCTION_LAST)
		      gfc_error ("Cray pointer '%s' in %s clause at %L",
				 n->sym->name, name, &code->loc);
		  }
		if (n->sym->as && n->sym->as->type == AS_ASSUMED_SIZE)
		  gfc_error ("Assumed size array '%s' in %s clause at %L",
			     n->sym->name, name, &code->loc);
		if (n->sym->attr.in_namelist
		    && (list < OMP_LIST_REDUCTION_FIRST
			|| list > OMP_LIST_REDUCTION_LAST))
		  gfc_error ("Variable '%s' in %s clause is used in "
			     "NAMELIST statement at %L",
			     n->sym->name, name, &code->loc);
	        if (list >= OACC_LIST_REDUCTION_FIRST
	            && list <= OACC_LIST_REDUCTION_LAST
	            && n->sym->as && n->sym->as->rank != 0)
	          gfc_error ("OpenACC reduction requires SCALAR variable "
	                     "at %L", &code->loc);

		switch (list)
		  {
		  case OMP_LIST_PLUS:
		  case OMP_LIST_MULT:
		  case OMP_LIST_SUB:
		    if (!gfc_numeric_ts (&n->sym->ts))
		      gfc_error ("%c REDUCTION variable '%s' at %L must be of numeric type, got %s",
				 list == OMP_LIST_PLUS ? '+'
				 : list == OMP_LIST_MULT ? '*' : '-',
				 n->sym->name, &code->loc,
				 gfc_typename (&n->sym->ts));
		    break;
		  case OMP_LIST_AND:
		  case OMP_LIST_OR:
		  case OMP_LIST_EQV:
		  case OMP_LIST_NEQV:
		    if (n->sym->ts.type != BT_LOGICAL)
		      gfc_error ("%s REDUCTION variable '%s' must be LOGICAL "
				 "at %L",
				 list == OMP_LIST_AND ? ".AND."
				 : list == OMP_LIST_OR ? ".OR."
				 : list == OMP_LIST_EQV ? ".EQV." : ".NEQV.",
				 n->sym->name, &code->loc);
		    break;
		  case OMP_LIST_MAX:
		  case OMP_LIST_MIN:
		    if (n->sym->ts.type != BT_INTEGER
			&& n->sym->ts.type != BT_REAL)
		      gfc_error ("%s REDUCTION variable '%s' must be "
				 "INTEGER or REAL at %L",
				 list == OMP_LIST_MAX ? "MAX" : "MIN",
				 n->sym->name, &code->loc);
		    break;
		  case OMP_LIST_IAND:
		  case OMP_LIST_IOR:
		  case OMP_LIST_IEOR:
		    if (n->sym->ts.type != BT_INTEGER)
		      gfc_error ("%s REDUCTION variable '%s' must be INTEGER "
				 "at %L",
				 list == OMP_LIST_IAND ? "IAND"
				 : list == OMP_LIST_MULT ? "IOR" : "IEOR",
				 n->sym->name, &code->loc);
		    break;
		  /* Workaround for PR middle-end/26316, nothing really needs
		     to be done here for OMP_LIST_PRIVATE.  */
		  case OMP_LIST_PRIVATE:
		    gcc_assert (code->op != EXEC_NOP);
		  default:
		    break;
		  }
	      }
	    break;
	  }
      }

  if (!oacc_clauses->is_acc)
    return;

  /* In OpenACC, single variable may appear in several clauses,
     such as COPYIN and COPYOUT. Hence, we use warning instead of error. */
  /*TODO: Support these cases. */
  for (list = OACC_LIST_FIRST; list < OACC_LIST_NUM; list++)
    for (n = oacc_clauses->lists[list]; n; n = n->next)
      {
        if (n->sym->mark)
          gfc_warning ("Symbol '%s' present on multiple clauses at %L",
              n->sym->name, &code->loc);
        else
          n->sym->mark = 1;
      }
  if (oacc_clauses->async)
    if (oacc_clauses->async_expr)
      resolve_oacc_scalar_int_expr (oacc_clauses->async_expr, "ASYNC");
  if (oacc_clauses->num_gangs_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->num_gangs_expr, "NUM_GANGS");
  if (oacc_clauses->num_workers_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->num_workers_expr, "NUM_WORKERS");
  if (oacc_clauses->vector_length_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->vector_length_expr, "VECTOR_LENGTH");
  if (oacc_clauses->gang_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->gang_expr, "GANG");
  if (oacc_clauses->worker_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->worker_expr, "WORKER");
  if (oacc_clauses->vector_expr)
    resolve_oacc_positive_int_expr (oacc_clauses->vector_expr, "VECTOR");
  if (oacc_clauses->wait)
    if (oacc_clauses->waitlist)
      for (el = oacc_clauses->waitlist; el; el = el->next)
        resolve_oacc_positive_int_expr (el->expr, "WAIT");

  for (list = OACC_LIST_FIRST; list < OACC_LIST_NUM; list++)
    if ((n = oacc_clauses->lists[list]) != NULL)
      {
        const char *name = oacc_clause_names[list - OACC_LIST_FIRST];

        if (n->oacc_subarray)
          resolve_subarray (n, &code->loc);

        if (list > OACC_LIST_DATA_CLAUSE_LAST)
          {
            if (n->sym->attr.pointer)
              gfc_error ("POINTER object '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
            if (n->sym->as && n->sym->as->type == AS_ASSUMED_SIZE)
              gfc_error ("Assumed size array '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
          }

        if (list == OACC_LIST_DEVICEPTR)
          {
            if (n->sym->attr.pointer)
              gfc_error ("POINTER object '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
            if (n->sym->attr.allocatable)
              gfc_error ("ALLOCATABLE object '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
            if (n->sym->attr.value)
              gfc_error ("VALUE object '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
          }

        if (list >= OACC_LIST_DATA_CLAUSE_FIRST
            && list <= OACC_LIST_DATA_CLAUSE_LAST)
          {
            if (n->sym->ts.type == BT_DERIVED
                && n->sym->attr.allocatable)
              gfc_error ("ALLOCATABLE object '%s' of DERIVED type in %s clause at %L",
                         n->sym->name, name, &code->loc);
            if (n->sym->ts.type == BT_DERIVED
                && n->sym->attr.pointer)
              gfc_error ("POINTER object '%s' of DERIVED type in %s clause at %L",
                         n->sym->name, name, &code->loc);
          }
      }
}


/* Return true if SYM is ever referenced in EXPR except in the SE node.  */

static bool
expr_references_sym (gfc_expr *e, gfc_symbol *s, gfc_expr *se)
{
  gfc_actual_arglist *arg;
  if (e == NULL || e == se)
    return false;
  switch (e->expr_type)
    {
    case EXPR_CONSTANT:
    case EXPR_NULL:
    case EXPR_VARIABLE:
    case EXPR_STRUCTURE:
    case EXPR_ARRAY:
      if (e->symtree != NULL
	  && e->symtree->n.sym == s)
	return true;
      return false;
    case EXPR_SUBSTRING:
      if (e->ref != NULL
	  && (expr_references_sym (e->ref->u.ss.start, s, se)
	      || expr_references_sym (e->ref->u.ss.end, s, se)))
	return true;
      return false;
    case EXPR_OP:
      if (expr_references_sym (e->value.op.op2, s, se))
	return true;
      return expr_references_sym (e->value.op.op1, s, se);
    case EXPR_FUNCTION:
      for (arg = e->value.function.actual; arg; arg = arg->next)
	if (expr_references_sym (arg->expr, s, se))
	  return true;
      return false;
    default:
      gcc_unreachable ();
    }
}


/* If EXPR is a conversion function that widens the type
   if WIDENING is true or narrows the type if WIDENING is false,
   return the inner expression, otherwise return NULL.  */

static gfc_expr *
is_conversion (gfc_expr *expr, bool widening)
{
  gfc_typespec *ts1, *ts2;

  if (expr->expr_type != EXPR_FUNCTION
      || expr->value.function.isym == NULL
      || expr->value.function.esym != NULL
      || expr->value.function.isym->id != GFC_ISYM_CONVERSION)
    return NULL;

  if (widening)
    {
      ts1 = &expr->ts;
      ts2 = &expr->value.function.actual->expr->ts;
    }
  else
    {
      ts1 = &expr->value.function.actual->expr->ts;
      ts2 = &expr->ts;
    }

  if (ts1->type > ts2->type
      || (ts1->type == ts2->type && ts1->kind > ts2->kind))
    return expr->value.function.actual->expr;

  return NULL;
}


static void
resolve_omp_atomic (gfc_code *code)
{
  gfc_code *atomic_code = code;
  gfc_symbol *var;
  gfc_expr *expr2, *expr2_tmp;

  code = code->block->next;
  gcc_assert (code->op == EXEC_ASSIGN);
  gcc_assert ((atomic_code->ext.omp_atomic != GFC_OMP_ATOMIC_CAPTURE
	       && code->next == NULL)
	      || (atomic_code->ext.omp_atomic == GFC_OMP_ATOMIC_CAPTURE
		  && code->next != NULL
		  && code->next->op == EXEC_ASSIGN
		  && code->next->next == NULL));

  if (code->expr1->expr_type != EXPR_VARIABLE
      || code->expr1->symtree == NULL
      || code->expr1->rank != 0
      || (code->expr1->ts.type != BT_INTEGER
	  && code->expr1->ts.type != BT_REAL
	  && code->expr1->ts.type != BT_COMPLEX
	  && code->expr1->ts.type != BT_LOGICAL))
    {
      gfc_error ("!$OMP ATOMIC statement must set a scalar variable of "
		 "intrinsic type at %L", &code->loc);
      return;
    }

  var = code->expr1->symtree->n.sym;
  expr2 = is_conversion (code->expr2, false);
  if (expr2 == NULL)
    {
      if (atomic_code->ext.omp_atomic == GFC_OMP_ATOMIC_READ
	  || atomic_code->ext.omp_atomic == GFC_OMP_ATOMIC_WRITE)
	expr2 = is_conversion (code->expr2, true);
      if (expr2 == NULL)
	expr2 = code->expr2;
    }

  switch (atomic_code->ext.omp_atomic)
    {
    case GFC_OMP_ATOMIC_READ:
      if (expr2->expr_type != EXPR_VARIABLE
	  || expr2->symtree == NULL
	  || expr2->rank != 0
	  || (expr2->ts.type != BT_INTEGER
	      && expr2->ts.type != BT_REAL
	      && expr2->ts.type != BT_COMPLEX
	      && expr2->ts.type != BT_LOGICAL))
	gfc_error ("!$OMP ATOMIC READ statement must read from a scalar "
		   "variable of intrinsic type at %L", &expr2->where);
      return;
    case GFC_OMP_ATOMIC_WRITE:
      if (expr2->rank != 0 || expr_references_sym (code->expr2, var, NULL))
	gfc_error ("expr in !$OMP ATOMIC WRITE assignment var = expr "
		   "must be scalar and cannot reference var at %L",
		   &expr2->where);
      return;
    case GFC_OMP_ATOMIC_CAPTURE:
      expr2_tmp = expr2;
      if (expr2 == code->expr2)
	{
	  expr2_tmp = is_conversion (code->expr2, true);
	  if (expr2_tmp == NULL)
	    expr2_tmp = expr2;
	}
      if (expr2_tmp->expr_type == EXPR_VARIABLE)
	{
	  if (expr2_tmp->symtree == NULL
	      || expr2_tmp->rank != 0
	      || (expr2_tmp->ts.type != BT_INTEGER
		  && expr2_tmp->ts.type != BT_REAL
		  && expr2_tmp->ts.type != BT_COMPLEX
		  && expr2_tmp->ts.type != BT_LOGICAL)
	      || expr2_tmp->symtree->n.sym == var)
	    {
	      gfc_error ("!$OMP ATOMIC CAPTURE capture statement must read from "
			 "a scalar variable of intrinsic type at %L",
			 &expr2_tmp->where);
	      return;
	    }
	  var = expr2_tmp->symtree->n.sym;
	  code = code->next;
	  if (code->expr1->expr_type != EXPR_VARIABLE
	      || code->expr1->symtree == NULL
	      || code->expr1->rank != 0
	      || (code->expr1->ts.type != BT_INTEGER
		  && code->expr1->ts.type != BT_REAL
		  && code->expr1->ts.type != BT_COMPLEX
		  && code->expr1->ts.type != BT_LOGICAL))
	    {
	      gfc_error ("!$OMP ATOMIC CAPTURE update statement must set "
			 "a scalar variable of intrinsic type at %L",
			 &code->expr1->where);
	      return;
	    }
	  if (code->expr1->symtree->n.sym != var)
	    {
	      gfc_error ("!$OMP ATOMIC CAPTURE capture statement reads from "
			 "different variable than update statement writes "
			 "into at %L", &code->expr1->where);
	      return;
	    }
	  expr2 = is_conversion (code->expr2, false);
	  if (expr2 == NULL)
	    expr2 = code->expr2;
	}
      break;
    default:
      break;
    }

  if (expr2->expr_type == EXPR_OP)
    {
      gfc_expr *v = NULL, *e, *c;
      gfc_intrinsic_op op = expr2->value.op.op;
      gfc_intrinsic_op alt_op = INTRINSIC_NONE;

      switch (op)
	{
	case INTRINSIC_PLUS:
	  alt_op = INTRINSIC_MINUS;
	  break;
	case INTRINSIC_TIMES:
	  alt_op = INTRINSIC_DIVIDE;
	  break;
	case INTRINSIC_MINUS:
	  alt_op = INTRINSIC_PLUS;
	  break;
	case INTRINSIC_DIVIDE:
	  alt_op = INTRINSIC_TIMES;
	  break;
	case INTRINSIC_AND:
	case INTRINSIC_OR:
	  break;
	case INTRINSIC_EQV:
	  alt_op = INTRINSIC_NEQV;
	  break;
	case INTRINSIC_NEQV:
	  alt_op = INTRINSIC_EQV;
	  break;
	default:
	  gfc_error ("!$OMP ATOMIC assignment operator must be "
		     "+, *, -, /, .AND., .OR., .EQV. or .NEQV. at %L",
		     &expr2->where);
	  return;
	}

      /* Check for var = var op expr resp. var = expr op var where
	 expr doesn't reference var and var op expr is mathematically
	 equivalent to var op (expr) resp. expr op var equivalent to
	 (expr) op var.  We rely here on the fact that the matcher
	 for x op1 y op2 z where op1 and op2 have equal precedence
	 returns (x op1 y) op2 z.  */
      e = expr2->value.op.op2;
      if (e->expr_type == EXPR_VARIABLE
	  && e->symtree != NULL
	  && e->symtree->n.sym == var)
	v = e;
      else if ((c = is_conversion (e, true)) != NULL
	       && c->expr_type == EXPR_VARIABLE
	       && c->symtree != NULL
	       && c->symtree->n.sym == var)
	v = c;
      else
	{
	  gfc_expr **p = NULL, **q;
	  for (q = &expr2->value.op.op1; (e = *q) != NULL; )
	    if (e->expr_type == EXPR_VARIABLE
		&& e->symtree != NULL
		&& e->symtree->n.sym == var)
	      {
		v = e;
		break;
	      }
	    else if ((c = is_conversion (e, true)) != NULL)
	      q = &e->value.function.actual->expr;
	    else if (e->expr_type != EXPR_OP
		     || (e->value.op.op != op
			 && e->value.op.op != alt_op)
		     || e->rank != 0)
	      break;
	    else
	      {
		p = q;
		q = &e->value.op.op1;
	      }

	  if (v == NULL)
	    {
	      gfc_error ("!$OMP ATOMIC assignment must be var = var op expr "
			 "or var = expr op var at %L", &expr2->where);
	      return;
	    }

	  if (p != NULL)
	    {
	      e = *p;
	      switch (e->value.op.op)
		{
		case INTRINSIC_MINUS:
		case INTRINSIC_DIVIDE:
		case INTRINSIC_EQV:
		case INTRINSIC_NEQV:
		  gfc_error ("!$OMP ATOMIC var = var op expr not "
			     "mathematically equivalent to var = var op "
			     "(expr) at %L", &expr2->where);
		  break;
		default:
		  break;
		}

	      /* Canonicalize into var = var op (expr).  */
	      *p = e->value.op.op2;
	      e->value.op.op2 = expr2;
	      e->ts = expr2->ts;
	      if (code->expr2 == expr2)
		code->expr2 = expr2 = e;
	      else
		code->expr2->value.function.actual->expr = expr2 = e;

	      if (!gfc_compare_types (&expr2->value.op.op1->ts, &expr2->ts))
		{
		  for (p = &expr2->value.op.op1; *p != v;
		       p = &(*p)->value.function.actual->expr)
		    ;
		  *p = NULL;
		  gfc_free_expr (expr2->value.op.op1);
		  expr2->value.op.op1 = v;
		  gfc_convert_type (v, &expr2->ts, 2);
		}
	    }
	}

      if (e->rank != 0 || expr_references_sym (code->expr2, var, v))
	{
	  gfc_error ("expr in !$OMP ATOMIC assignment var = var op expr "
		     "must be scalar and cannot reference var at %L",
		     &expr2->where);
	  return;
	}
    }
  else if (expr2->expr_type == EXPR_FUNCTION
	   && expr2->value.function.isym != NULL
	   && expr2->value.function.esym == NULL
	   && expr2->value.function.actual != NULL
	   && expr2->value.function.actual->next != NULL)
    {
      gfc_actual_arglist *arg, *var_arg;

      switch (expr2->value.function.isym->id)
	{
	case GFC_ISYM_MIN:
	case GFC_ISYM_MAX:
	  break;
	case GFC_ISYM_IAND:
	case GFC_ISYM_IOR:
	case GFC_ISYM_IEOR:
	  if (expr2->value.function.actual->next->next != NULL)
	    {
	      gfc_error ("!$OMP ATOMIC assignment intrinsic IAND, IOR "
			 "or IEOR must have two arguments at %L",
			 &expr2->where);
	      return;
	    }
	  break;
	default:
	  gfc_error ("!$OMP ATOMIC assignment intrinsic must be "
		     "MIN, MAX, IAND, IOR or IEOR at %L",
		     &expr2->where);
	  return;
	}

      var_arg = NULL;
      for (arg = expr2->value.function.actual; arg; arg = arg->next)
	{
	  if ((arg == expr2->value.function.actual
	       || (var_arg == NULL && arg->next == NULL))
	      && arg->expr->expr_type == EXPR_VARIABLE
	      && arg->expr->symtree != NULL
	      && arg->expr->symtree->n.sym == var)
	    var_arg = arg;
	  else if (expr_references_sym (arg->expr, var, NULL))
	    gfc_error ("!$OMP ATOMIC intrinsic arguments except one must not "
		       "reference '%s' at %L", var->name, &arg->expr->where);
	  if (arg->expr->rank != 0)
	    gfc_error ("!$OMP ATOMIC intrinsic arguments must be scalar "
		       "at %L", &arg->expr->where);
	}

      if (var_arg == NULL)
	{
	  gfc_error ("First or last !$OMP ATOMIC intrinsic argument must "
		     "be '%s' at %L", var->name, &expr2->where);
	  return;
	}

      if (var_arg != expr2->value.function.actual)
	{
	  /* Canonicalize, so that var comes first.  */
	  gcc_assert (var_arg->next == NULL);
	  for (arg = expr2->value.function.actual;
	       arg->next != var_arg; arg = arg->next)
	    ;
	  var_arg->next = expr2->value.function.actual;
	  expr2->value.function.actual = var_arg;
	  arg->next = NULL;
	}
    }
  else
    gfc_error ("!$OMP ATOMIC assignment must have an operator or intrinsic "
	       "on right hand side at %L", &expr2->where);

  if (atomic_code->ext.omp_atomic == GFC_OMP_ATOMIC_CAPTURE && code->next)
    {
      code = code->next;
      if (code->expr1->expr_type != EXPR_VARIABLE
	  || code->expr1->symtree == NULL
	  || code->expr1->rank != 0
	  || (code->expr1->ts.type != BT_INTEGER
	      && code->expr1->ts.type != BT_REAL
	      && code->expr1->ts.type != BT_COMPLEX
	      && code->expr1->ts.type != BT_LOGICAL))
	{
	  gfc_error ("!$OMP ATOMIC CAPTURE capture statement must set "
		     "a scalar variable of intrinsic type at %L",
		     &code->expr1->where);
	  return;
	}

      expr2 = is_conversion (code->expr2, false);
      if (expr2 == NULL)
	{
	  expr2 = is_conversion (code->expr2, true);
	  if (expr2 == NULL)
	    expr2 = code->expr2;
	}

      if (expr2->expr_type != EXPR_VARIABLE
	  || expr2->symtree == NULL
	  || expr2->rank != 0
	  || (expr2->ts.type != BT_INTEGER
	      && expr2->ts.type != BT_REAL
	      && expr2->ts.type != BT_COMPLEX
	      && expr2->ts.type != BT_LOGICAL))
	{
	  gfc_error ("!$OMP ATOMIC CAPTURE capture statement must read "
		     "from a scalar variable of intrinsic type at %L",
		     &expr2->where);
	  return;
	}
      if (expr2->symtree->n.sym != var)
	{
	  gfc_error ("!$OMP ATOMIC CAPTURE capture statement reads from "
		     "different variable than update statement writes "
		     "into at %L", &expr2->where);
	  return;
	}
    }
}


struct omp_context
{
  gfc_code *code;
  struct pointer_set_t *sharing_clauses;
  struct pointer_set_t *private_iterators;
  struct omp_context *previous;
} *omp_current_ctx;
static gfc_code *omp_current_do_code;
static int omp_current_do_collapse;

void
gfc_resolve_omp_do_blocks (gfc_code *code, gfc_namespace *ns)
{
  if (code->block->next && code->block->next->op == EXEC_DO)
    {
      int i;
      gfc_code *c;

      omp_current_do_code = code->block->next;
      omp_current_do_collapse = code->ext.omp_clauses->collapse;
      for (i = 1, c = omp_current_do_code; i < omp_current_do_collapse; i++)
	{
	  c = c->block;
	  if (c->op != EXEC_DO || c->next == NULL)
	    break;
	  c = c->next;
	  if (c->op != EXEC_DO)
	    break;
	}
      if (i < omp_current_do_collapse || omp_current_do_collapse <= 0)
	omp_current_do_collapse = 1;
    }
  gfc_resolve_blocks (code->block, ns);
  omp_current_do_collapse = 0;
  omp_current_do_code = NULL;
}


void
gfc_resolve_omp_parallel_blocks (gfc_code *code, gfc_namespace *ns)
{
  struct omp_context ctx;
  gfc_omp_clauses *omp_clauses = code->ext.omp_clauses;
  gfc_namelist *n;
  int list;

  ctx.code = code;
  ctx.sharing_clauses = pointer_set_create ();
  ctx.private_iterators = pointer_set_create ();
  ctx.previous = omp_current_ctx;
  omp_current_ctx = &ctx;

  for (list = 0; list < OMP_LIST_NUM; list++)
    for (n = omp_clauses->lists[list]; n; n = n->next)
      pointer_set_insert (ctx.sharing_clauses, n->sym);

  if (code->op == EXEC_OMP_PARALLEL_DO)
    gfc_resolve_omp_do_blocks (code, ns);
  else
    gfc_resolve_blocks (code->block, ns);

  omp_current_ctx = ctx.previous;
  pointer_set_destroy (ctx.sharing_clauses);
  pointer_set_destroy (ctx.private_iterators);
}


/* Save and clear openmp.c private state.  */

void
gfc_omp_save_and_clear_state (struct gfc_omp_saved_state *state)
{
  state->ptrs[0] = omp_current_ctx;
  state->ptrs[1] = omp_current_do_code;
  state->ints[0] = omp_current_do_collapse;
  omp_current_ctx = NULL;
  omp_current_do_code = NULL;
  omp_current_do_collapse = 0;
}


/* Restore openmp.c private state from the saved state.  */

void
gfc_omp_restore_state (struct gfc_omp_saved_state *state)
{
  omp_current_ctx = (struct omp_context *) state->ptrs[0];
  omp_current_do_code = (gfc_code *) state->ptrs[1];
  omp_current_do_collapse = state->ints[0];
}


/* Note a DO iterator variable.  This is special in !$omp parallel
   construct, where they are predetermined private.  */

void
gfc_resolve_do_iterator (gfc_code *code, gfc_symbol *sym)
{
  int i = omp_current_do_collapse;
  gfc_code *c = omp_current_do_code;

  if (sym->attr.threadprivate)
    return;

  /* !$omp do and !$omp parallel do iteration variable is predetermined
     private just in the !$omp do resp. !$omp parallel do construct,
     with no implications for the outer parallel constructs.  */

  while (i-- >= 1)
    {
      if (code == c)
	return;

      c = c->block->next;
    }

  if (omp_current_ctx == NULL)
    return;

  if (pointer_set_contains (omp_current_ctx->sharing_clauses, sym))
    return;

  if (! pointer_set_insert (omp_current_ctx->private_iterators, sym))
    {
      gfc_omp_clauses *omp_clauses = omp_current_ctx->code->ext.omp_clauses;
      gfc_namelist *p;

      p = gfc_get_namelist ();
      p->sym = sym;
      p->next = omp_clauses->lists[OMP_LIST_PRIVATE];
      omp_clauses->lists[OMP_LIST_PRIVATE] = p;
    }
}


static void
resolve_omp_do (gfc_code *code)
{
  gfc_code *do_code, *c;
  int list, i, collapse;
  gfc_namelist *n;
  gfc_symbol *dovar;

  if (code->ext.omp_clauses)
    resolve_omp_clauses (code);

  do_code = code->block->next;
  collapse = code->ext.omp_clauses->collapse;
  if (collapse <= 0)
    collapse = 1;
  for (i = 1; i <= collapse; i++)
    {
      if (do_code->op == EXEC_DO_WHILE)
	{
	  gfc_error ("!$OMP DO cannot be a DO WHILE or DO without loop control "
		     "at %L", &do_code->loc);
	  break;
	}
      gcc_assert (do_code->op == EXEC_DO);
      if (do_code->ext.iterator->var->ts.type != BT_INTEGER)
	gfc_error ("!$OMP DO iteration variable must be of type integer at %L",
		   &do_code->loc);
      dovar = do_code->ext.iterator->var->symtree->n.sym;
      if (dovar->attr.threadprivate)
	gfc_error ("!$OMP DO iteration variable must not be THREADPRIVATE "
		   "at %L", &do_code->loc);
      if (code->ext.omp_clauses)
	for (list = 0; list < OMP_LIST_NUM; list++)
	  if (list != OMP_LIST_PRIVATE && list != OMP_LIST_LASTPRIVATE)
	    for (n = code->ext.omp_clauses->lists[list]; n; n = n->next)
	      if (dovar == n->sym)
		{
		  gfc_error ("!$OMP DO iteration variable present on clause "
			     "other than PRIVATE or LASTPRIVATE at %L",
			     &do_code->loc);
		  break;
		}
      if (i > 1)
	{
	  gfc_code *do_code2 = code->block->next;
	  int j;

	  for (j = 1; j < i; j++)
	    {
	      gfc_symbol *ivar = do_code2->ext.iterator->var->symtree->n.sym;
	      if (dovar == ivar
		  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->start)
		  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->end)
		  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->step))
		{
		  gfc_error ("!$OMP DO collapsed loops don't form rectangular iteration space at %L",
			     &do_code->loc);
		  break;
		}
	      if (j < i)
		break;
	      do_code2 = do_code2->block->next;
	    }
	}
      if (i == collapse)
	break;
      for (c = do_code->next; c; c = c->next)
	if (c->op != EXEC_NOP && c->op != EXEC_CONTINUE)
	  {
	    gfc_error ("collapsed !$OMP DO loops not perfectly nested at %L",
		       &c->loc);
	    break;
	  }
      if (c)
	break;
      do_code = do_code->block;
      if (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE)
	{
	  gfc_error ("not enough DO loops for collapsed !$OMP DO at %L",
		     &code->loc);
	  break;
	}
      do_code = do_code->next;
      if (do_code == NULL
	  || (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE))
	{
	  gfc_error ("not enough DO loops for collapsed !$OMP DO at %L",
		     &code->loc);
	  break;
	}
    }
}

typedef struct omp_context oacc_context;
oacc_context *oacc_current_ctx;

static bool
oacc_is_parallel (gfc_code *code)
{
  return code->op == EXEC_OACC_PARALLEL || code->op == EXEC_OACC_PARALLEL_LOOP;
}

static bool
oacc_is_kernels (gfc_code *code)
{
  return code->op == EXEC_OACC_KERNELS || code->op == EXEC_OACC_KERNELS_LOOP;
}

static bool
oacc_is_loop (gfc_code *code)
{
  return code->op == EXEC_OACC_PARALLEL_LOOP
         || code->op == EXEC_OACC_KERNELS_LOOP
         || code->op == EXEC_OACC_LOOP;
}


static void
resolve_oacc_nested_loops (gfc_code *code, gfc_code* do_code, int collapse,
                          const char *clause)
{
  gfc_symbol *dovar;
  gfc_code *c;
  int i;

  for (i = 1; i <= collapse; i++)
    {
      if (do_code->op == EXEC_DO_WHILE)
        {
          gfc_error ("!$ACC LOOP cannot be a DO WHILE or DO without loop control "
                     "at %L", &do_code->loc);
          break;
        }
      gcc_assert (do_code->op == EXEC_DO);
      if (do_code->ext.iterator->var->ts.type != BT_INTEGER)
        gfc_error ("!$ACC LOOP iteration variable must be of type integer at %L",
                   &do_code->loc);
      dovar = do_code->ext.iterator->var->symtree->n.sym;
      if (i > 1)
        {
          gfc_code *do_code2 = code->block->next;
          int j;

          for (j = 1; j < i; j++)
            {
              gfc_symbol *ivar = do_code2->ext.iterator->var->symtree->n.sym;
              if (dovar == ivar
                  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->start)
                  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->end)
                  || gfc_find_sym_in_expr (ivar, do_code->ext.iterator->step))
                {
                  gfc_error ("!$ACC LOOP %s loops don't form rectangular iteration space at %L",
                             clause, &do_code->loc);
                  break;
                }
              if (j < i)
                break;
              do_code2 = do_code2->block->next;
            }
        }
      if (i == collapse)
        break;
      for (c = do_code->next; c; c = c->next)
        if (c->op != EXEC_NOP && c->op != EXEC_CONTINUE)
          {
            gfc_error ("%s !$ACC LOOP loops not perfectly nested at %L",
                       clause, &c->loc);
            break;
          }
      if (c)
        break;
      do_code = do_code->block;
      if (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE)
        {
          gfc_error ("not enough DO loops for %s !$ACC LOOP at %L",
                     clause, &code->loc);
          break;
        }
      do_code = do_code->next;
      if (do_code == NULL
          || (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE))
        {
          gfc_error ("not enough DO loops for %s !$ACC LOOP at %L",
                     clause, &code->loc);
          break;
        }
    }
}

static void
resolve_oacc_params_in_parallel (gfc_code *code, const char *clause)
{
  oacc_context *c;

  if (oacc_is_parallel (code))
    gfc_error ("LOOP %s in PARALLEL section allows no argument or static at %L",
               clause, &code->loc);
  for (c = oacc_current_ctx; c; c = c->previous)
    {
      if (oacc_is_loop (c->code))
        break;
      if (oacc_is_parallel (c->code))
        gfc_error ("LOOP %s in PARALLEL section allows no argument or static at %L",
                   clause, &code->loc);
    }
}


static void
resolve_oacc_loop_blocks (gfc_code *code)
{
  oacc_context *c;

  if (!oacc_is_loop (code))
    return;

  if (code->op == EXEC_OACC_LOOP)
    for (c = oacc_current_ctx; c; c = c->previous)
      {
        if (oacc_is_loop (c->code))
          {
            if (code->ext.omp_clauses->gang)
              {
                if (c->code->ext.omp_clauses->gang)
                  gfc_error ("Loop parallelized across gangs is not allowed "
                             "inside another loop parallelized across gangs at %L",
                             &code->loc);
                if (c->code->ext.omp_clauses->worker)
                  gfc_error ("Loop parallelized across gangs is not allowed "
                             "inside loop parallelized across workers at %L",
                             &code->loc);
                if (c->code->ext.omp_clauses->vector)
                  gfc_error ("Loop parallelized across gangs is not allowed "
                             "inside loop parallelized across workers at %L",
                             &code->loc);
              }
            if (code->ext.omp_clauses->worker)
              {
                if (c->code->ext.omp_clauses->worker)
                  gfc_error ("Loop parallelized across workers is not allowed "
                             "inside another loop parallelized across workers at %L",
                             &code->loc);
                if (c->code->ext.omp_clauses->vector)
                  gfc_error ("Loop parallelized across workers is not allowed "
                             "inside another loop parallelized across vectors at %L",
                             &code->loc);
              }
            if (code->ext.omp_clauses->vector)
              if (c->code->ext.omp_clauses->vector)
                gfc_error ("Loop parallelized across vectors is not allowed "
                           "inside another loop parallelized across vectors at %L",
                           &code->loc);
          }

        if (oacc_is_parallel (c->code) || oacc_is_kernels (c->code))
          break;
      }

  if (code->ext.omp_clauses->seq)
    {
      if (code->ext.omp_clauses->independent)
        gfc_error ("Both SEQ and INDEPENDENT are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->gang)
        gfc_error ("Both SEQ and GANG are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->worker)
        gfc_error ("Both SEQ and WORKER are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->vector)
        gfc_error ("Both SEQ and VECTOR are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->par_auto)
        gfc_error ("Both SEQ and AUTO are not allowed in %L", &code->loc);
    }
  if (code->ext.omp_clauses->par_auto)
    {
      if (code->ext.omp_clauses->gang)
        gfc_error ("Both AUTO and GANG are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->worker)
        gfc_error ("Both AUTO and WORKER are not allowed in %L", &code->loc);
      if (code->ext.omp_clauses->vector)
        gfc_error ("Both AUTO and VECTOR are not allowed in %L", &code->loc);
    }
  if (!code->ext.omp_clauses->tilelist)
    {
      if (code->ext.omp_clauses->gang)
        {
          if (code->ext.omp_clauses->worker)
            gfc_error ("Both GANG and WORKER are not allowed in %L", &code->loc);
          if (code->ext.omp_clauses->vector)
            gfc_error ("Both GANG and VECTOR are not allowed in %L", &code->loc);
        }
      if (code->ext.omp_clauses->worker)
        if (code->ext.omp_clauses->vector)
          gfc_error ("Both WORKER and VECTOR are not allowed in %L", &code->loc);
    }
  else if (code->ext.omp_clauses->gang
           && code->ext.omp_clauses->worker
           && code->ext.omp_clauses->vector)
    gfc_error ("All GANG, WORKER and VECTOR are not allowed in %L", &code->loc);

  if (code->ext.omp_clauses->gang
      && code->ext.omp_clauses->gang_expr
      && !code->ext.omp_clauses->gang_static)
    resolve_oacc_params_in_parallel (code, "GANG");

  if (code->ext.omp_clauses->worker
      && code->ext.omp_clauses->worker_expr)
    resolve_oacc_params_in_parallel (code, "WORKER");

  if (code->ext.omp_clauses->tilelist)
    {
      gfc_exprlist *el;
      int num = 0;
      for (el = code->ext.omp_clauses->tilelist; el; el = el->next)
        {
          num++;
          if (el->expr == NULL)
            continue;
          resolve_oacc_positive_int_expr (el->expr, "TILE");
          if (el->expr->expr_type != EXPR_CONSTANT)
            gfc_error ("TILE requires constant expression at %L", &code->loc);
        }
      resolve_oacc_nested_loops (code, code->block->next, num, "tiled");
    }
}


void
gfc_resolve_oacc_blocks (gfc_code *code, gfc_namespace *ns)
{
  oacc_context ctx;

  resolve_oacc_loop_blocks (code);

  ctx.code = code;
  ctx.previous = oacc_current_ctx;
  oacc_current_ctx = &ctx;

  gfc_resolve_blocks (code->block, ns);

  oacc_current_ctx = ctx.previous;
}


static void
resolve_oacc_loop(gfc_code *code)
{
  gfc_code *do_code;
  int collapse;
  int list;

  if (code->ext.omp_clauses)
    resolve_omp_clauses (code);

  for (list = OACC_LIST_REDUCTION_FIRST; list <= OACC_LIST_REDUCTION_LAST; list++)
    if (code->ext.omp_clauses->lists[list])
      {
        bool kernels_region = false;
        oacc_context *c;

        if (code->op == EXEC_OACC_KERNELS_LOOP)
          kernels_region = true;
        if (!kernels_region)
          for (c = oacc_current_ctx; c != NULL && !kernels_region; c = c->previous)
            if (c->code->op == EXEC_OACC_KERNELS)
              kernels_region = true;

        if (kernels_region)
          {
            gfc_error ("REDUCTION clause is not allowed in KERNELS region at %L",
                &code->loc);
            break;
          }
      }

  do_code = code->block->next;
  collapse = code->ext.omp_clauses->collapse;

  if (collapse <= 0)
    collapse = 1;
  resolve_oacc_nested_loops (code, do_code, collapse, "collapsed");
}


static void
resolve_oacc_cache (gfc_code *code)
{
  gfc_namelist* n;

  for (n = code->ext.omp_clauses->lists[OACC_LIST_CACHE]; n; n = n->next)
    if (!n->oacc_subarray)
      gfc_error ("Only single array elements and subarrays are allowed in "
                 "ACC CACHE directive %L",
                 &code->loc);
}


static void
resolve_oacc_wait (gfc_code *code)
{
  gfc_exprlist* el;

  for (el = code->ext.omp_clauses->waitlist; el; el = el->next)
    resolve_oacc_positive_int_expr (el->expr, "WAIT");
}

/* Resolve OpenMP directive clauses and check various requirements
   of each directive.  */

void
gfc_resolve_omp_directive (gfc_code *code, gfc_namespace *ns ATTRIBUTE_UNUSED)
{
  if (code->op != EXEC_OMP_ATOMIC)
    gfc_maybe_initialize_eh ();

  switch (code->op)
    {
    case EXEC_OMP_DO:
    case EXEC_OMP_PARALLEL_DO:
      resolve_omp_do (code);
      break;
    case EXEC_OMP_WORKSHARE:
    case EXEC_OMP_PARALLEL_WORKSHARE:
    case EXEC_OMP_PARALLEL:
    case EXEC_OMP_PARALLEL_SECTIONS:
    case EXEC_OMP_SECTIONS:
    case EXEC_OMP_SINGLE:
    case EXEC_OMP_TASK:
      if (code->ext.omp_clauses)
	resolve_omp_clauses (code);
      break;
    case EXEC_OMP_ATOMIC:
      resolve_omp_atomic (code);
      break;
    default:
      break;
    }
}


void
gfc_resolve_oacc_directive (gfc_code *code, gfc_namespace *ns ATTRIBUTE_UNUSED)
{
  switch (code->op)
    {
    case EXEC_OACC_CACHE:
      resolve_oacc_cache (code);
      break;
    case EXEC_OACC_WAIT:
      resolve_oacc_wait (code);
      break;
    case EXEC_OACC_UPDATE:
    case EXEC_OACC_HOST_DATA:
    case EXEC_OACC_DATA:
    case EXEC_OACC_KERNELS:
    case EXEC_OACC_PARALLEL:
    case EXEC_OACC_ENTER_DATA:
    case EXEC_OACC_EXIT_DATA:
      resolve_omp_clauses (code);
      break;
    case EXEC_OACC_LOOP:
    case EXEC_OACC_KERNELS_LOOP:
    case EXEC_OACC_PARALLEL_LOOP:
      resolve_oacc_loop (code);
      break;
    default:
      break;
    }
}
