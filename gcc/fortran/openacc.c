/* OpenACC directive matching and resolving.
 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "flags.h"
#include "gfortran.h"
#include "match.h"
#include "parse.h"
#include "pointer-set.h"
#include "arith.h"              /* for gfc_convert_integer */

#define ACC_CLAUSE_IF                   (1l << 0)
#define ACC_CLAUSE_ASYNC                (1l << 1)
#define ACC_CLAUSE_NUM_GANGS            (1l << 2)
#define ACC_CLAUSE_NUM_WORKERS          (1l << 3)
#define ACC_CLAUSE_VECTOR_LENGTH        (1l << 4)
#define ACC_CLAUSE_REDUCTION            (1l << 5)
#define ACC_CLAUSE_COPY                 (1l << 6)
#define ACC_CLAUSE_COPYIN               (1l << 7)
#define ACC_CLAUSE_COPYOUT              (1l << 8)
#define ACC_CLAUSE_CREATE               (1l << 9)
#define ACC_CLAUSE_PRESENT              (1l << 10)
#define ACC_CLAUSE_PRESENT_OR_COPY      (1l << 11)
#define ACC_CLAUSE_PRESENT_OR_COPYIN    (1l << 12)
#define ACC_CLAUSE_PRESENT_OR_COPYOUT   (1l << 13)
#define ACC_CLAUSE_PRESENT_OR_CREATE    (1l << 14)
#define ACC_CLAUSE_DEVICEPTR            (1l << 15)
#define ACC_CLAUSE_PRIVATE              (1l << 16)
#define ACC_CLAUSE_FIRSTPRIVATE         (1l << 17)
#define ACC_CLAUSE_COLLAPSE             (1l << 18)
#define ACC_CLAUSE_GANG                 (1l << 19)
#define ACC_CLAUSE_WORKER               (1l << 20)
#define ACC_CLAUSE_VECTOR               (1l << 21)
#define ACC_CLAUSE_SEQ                  (1l << 22)
#define ACC_CLAUSE_INDEPENDENT          (1l << 23)
#define ACC_CLAUSE_USE_DEVICE           (1l << 24)
#define ACC_CLAUSE_DEVICE_RESIDENT      (1l << 25)
#define ACC_CLAUSE_HOST                 (1l << 26)
#define ACC_CLAUSE_DEVICE               (1l << 27)
#define ACC_CLAUSE_DEFAULT              (1l << 28)
#define ACC_CLAUSE_WAIT                 (1l << 29)
#define ACC_CLAUSE_DELETE               (1l << 30)

/* Match an end of OpenACC directive.  End of OpenACC directive is optional
   whitespace, followed by '\n' or comment '!'.  */

match
gfc_match_acc_eos (void)
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

/* Free an acc_clauses structure.  */
void
gfc_free_acc_clauses (gfc_acc_clauses *c)
{
  int i;

  if (c == NULL)
    return;
  gfc_free_expr (c->if_expr);
  gfc_free_expr (c->async_expr);
  gfc_free_expr (c->gang_expr);
  gfc_free_expr (c->worker_expr);
  gfc_free_expr (c->vector_expr);
  gfc_free_expr (c->num_gangs_expr);
  gfc_free_expr (c->num_workers_expr);
  gfc_free_expr (c->vector_length_expr);
  gfc_free_expr (c->non_clause_wait_expr);

  for (i = 0; i < ACC_LIST_NUM; i++)
    gfc_free_namelist (c->lists[i]);

  gfc_free_exprlist (c->waitlist);

  free (c);
}

static void
set_subarray (struct gfc_acc_subarray **acc_subarray,
              gfc_expr *left, gfc_expr *right, int i)
{
  if (*acc_subarray == NULL)
    *acc_subarray = gfc_get_acc_subarray();
  (*acc_subarray)->left[i] = left;
  (*acc_subarray)->right[i] = right;
}

static match
match_subarray (struct gfc_acc_subarray **sa)
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
match_acc_variable_list (const char *str, gfc_namelist **list,
                             bool allow_common, bool allow_subarrays)
{
  gfc_namelist *head, *tail, *p;
  locus old_loc;
  char n[GFC_MAX_SYMBOL_LEN+1];
  gfc_symbol *sym;
  match m;
  gfc_symtree *st;
  struct gfc_acc_subarray *sa = NULL;

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
              tail->acc_subarray = sa;
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
  gfc_error ("Syntax error in OpenACC variable list at %C");

cleanup:
  gfc_free_namelist (head);
  gfc_current_locus = old_loc;
  return MATCH_ERROR;
}

static match
match_acc_exprlist (const char *str, gfc_exprlist **list)
{
  gfc_exprlist *head, *tail, *p;
  locus old_loc;
  char n[GFC_MAX_SYMBOL_LEN+1];
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
      switch (m)
        {
        case MATCH_YES:
          p = gfc_get_exprlist ();
          if (head == NULL)
            head = tail = p;
          else
            {
              tail->next = p;
              tail = tail->next;
            }
          tail->expr = expr;
          goto next_item;
        case MATCH_NO:
          break;
        case MATCH_ERROR:
          goto cleanup;
        }
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

/* Match OpenACC directive clauses. MASK is a bitmask of
   clauses that are allowed for a particular directive.  */
static match
match_acc_clauses (gfc_acc_clauses **cp, long mask)
{
  gfc_acc_clauses *c = gfc_get_acc_clauses ();
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

      /* Optional expressions */
      if ((mask & ACC_CLAUSE_ASYNC) && !c->async)
        if (gfc_match ("async") == MATCH_YES)
          {
            c->async = true;
            if (gfc_match (" ( %e )", &c->async_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & ACC_CLAUSE_GANG) && !c->gang)
        if (gfc_match ("gang") == MATCH_YES)
          {
            c->gang = true;
            if (gfc_match (" ( %e )", &c->gang_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & ACC_CLAUSE_WORKER) && !c->worker)
        if (gfc_match ("worker") == MATCH_YES)
          {
            c->worker = true;
            if (gfc_match (" ( %e )", &c->worker_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }
      if ((mask & ACC_CLAUSE_VECTOR_LENGTH) && c->vector_length_expr == NULL
          && gfc_match ("vector_length ( %e )", &c->vector_length_expr) == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_VECTOR) && !c->vector)
        if (gfc_match ("vector") == MATCH_YES)
          {
            c->vector = true;
            if (gfc_match (" ( %e )", &c->vector_expr) == MATCH_YES)
              needs_space = false;
            else
              needs_space = true;
            continue;
          }

      /* Expresions */
      if ((mask & ACC_CLAUSE_IF) && c->if_expr == NULL
          && gfc_match ("if ( %e )", &c->if_expr) == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_NUM_GANGS) && c->num_gangs_expr == NULL
          && gfc_match ("num_gangs ( %e )", &c->num_gangs_expr) == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_NUM_WORKERS) && c->num_workers_expr == NULL
          && gfc_match ("num_workers ( %e )", &c->num_workers_expr) == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_COPY)
          && match_acc_variable_list ("copy (",
                                      &c->lists[ACC_LIST_COPY], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_COPYIN)
          && match_acc_variable_list ("copyin (",
                                      &c->lists[ACC_LIST_COPYIN], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_COPYOUT)
          && match_acc_variable_list ("copyout (",
                                      &c->lists[ACC_LIST_COPYOUT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_CREATE)
          && match_acc_variable_list ("create (",
                                      &c->lists[ACC_LIST_CREATE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_DELETE)
          && match_acc_variable_list ("delete (",
                                      &c->lists[ACC_LIST_DELETE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT)
          && match_acc_variable_list ("present (",
                                      &c->lists[ACC_LIST_PRESENT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPY)
          && match_acc_variable_list ("present_or_copy (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPY], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPY)
          && match_acc_variable_list ("pcopy (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPY], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPYIN)
          && match_acc_variable_list ("present_or_copyin (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPYIN],true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPYIN)
          && match_acc_variable_list ("pcopyin (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPYIN], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPYOUT)
          && match_acc_variable_list ("present_or_copyout (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPYOUT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_COPYOUT)
          && match_acc_variable_list ("pcopyout (",
                                      &c->lists[ACC_LIST_PRESENT_OR_COPYOUT], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_CREATE)
          && match_acc_variable_list ("present_or_create (",
                                      &c->lists[ACC_LIST_PRESENT_OR_CREATE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRESENT_OR_CREATE)
          && match_acc_variable_list ("pcreate (",
                                      &c->lists[ACC_LIST_PRESENT_OR_CREATE], true, true)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_DEVICEPTR)
          && match_acc_variable_list ("deviceptr (",
                                      &c->lists[ACC_LIST_DEVICEPTR], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_PRIVATE)
          && match_acc_variable_list ("private (",
                                      &c->lists[ACC_LIST_PRIVATE], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_FIRSTPRIVATE)
          && match_acc_variable_list ("firstprivate (",
                                      &c->lists[ACC_LIST_FIRSTPRIVATE], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_USE_DEVICE)
          && match_acc_variable_list ("use_device (",
                                      &c->lists[ACC_LIST_USE_DEVICE], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_DEVICE_RESIDENT)
          && match_acc_variable_list ("device_resident (",
                                      &c->lists[ACC_LIST_DEVICE_RESIDENT], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_HOST)
          && match_acc_variable_list ("host (",
                                      &c->lists[ACC_LIST_HOST], true, false)
             == MATCH_YES)
        continue;
      if ((mask & ACC_CLAUSE_DEVICE)
          && match_acc_variable_list ("device (",
                                      &c->lists[ACC_LIST_DEVICE], true, false)
             == MATCH_YES)
        continue;


      /* Integer */
      if ((mask & ACC_CLAUSE_COLLAPSE) && !c->collapse)
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

      /* Without params */
      if ((mask & ACC_CLAUSE_SEQ) && !c->seq
          && gfc_match ("seq") == MATCH_YES)
        {
          c->seq = true;
          needs_space = true;
          continue;
        }
      if ((mask & ACC_CLAUSE_INDEPENDENT) && !c->independent
          && gfc_match ("independent") == MATCH_YES)
        {
          c->independent = true;
          needs_space = true;
          continue;
        }
      if ((mask & ACC_CLAUSE_DEFAULT) && !c->default_none
                && gfc_match ("default ( none )") == MATCH_YES)
        {
          c->default_none = true;
          needs_space = true;
          continue;
        }

      /* optional list */
      if ((mask & ACC_CLAUSE_WAIT) && !c->wait
                && gfc_match ("wait") == MATCH_YES)
        {
          c->wait = true;
          match_acc_exprlist (" (", &c->waitlist);
          continue;
        }

      /* Reduction */
      old_loc = gfc_current_locus;
      if ((mask & ACC_CLAUSE_REDUCTION)
          && gfc_match ("reduction (") == MATCH_YES)
        {
          int reduction = ACC_LIST_NUM;
          char buffer[GFC_MAX_SYMBOL_LEN + 1];
          if (gfc_match_char ('+') == MATCH_YES)
            reduction = ACC_LIST_PLUS;
          else if (gfc_match_char ('*') == MATCH_YES)
            reduction = ACC_LIST_MULT;
          else if (gfc_match_char ('-') == MATCH_YES)
            reduction = ACC_LIST_SUB;
          else if (gfc_match (".and.") == MATCH_YES)
            reduction = ACC_LIST_AND;
          else if (gfc_match (".or.") == MATCH_YES)
            reduction = ACC_LIST_OR;
          else if (gfc_match (".eqv.") == MATCH_YES)
            reduction = ACC_LIST_EQV;
          else if (gfc_match (".neqv.") == MATCH_YES)
            reduction = ACC_LIST_NEQV;
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
                reduction = ACC_LIST_MAX;
              else if (strcmp (n, "min") == 0)
                reduction = ACC_LIST_MIN;
              else if (strcmp (n, "iand") == 0)
                reduction = ACC_LIST_IAND;
              else if (strcmp (n, "ior") == 0)
                reduction = ACC_LIST_IOR;
              else if (strcmp (n, "ieor") == 0)
                reduction = ACC_LIST_IEOR;
              if (reduction != ACC_LIST_NUM
                  && sym != NULL
                  && ! sym->attr.intrinsic
                  && ! sym->attr.use_assoc
                  && ((sym->attr.flavor == FL_UNKNOWN
                       && !gfc_add_flavor (&sym->attr, FL_PROCEDURE, sym->name, NULL))
                      || !gfc_add_intrinsic (&sym->attr, NULL)))
                {
                  gfc_free_acc_clauses (c);
                  return MATCH_ERROR;
                }
            }
          if (reduction != ACC_LIST_NUM
              && match_acc_variable_list (" :", &c->lists[reduction],
                                              false, false)
                 == MATCH_YES)
            continue;
          else
            gfc_current_locus = old_loc;
        }

      break;
    }

  if (gfc_match_acc_eos () != MATCH_YES)
    {
      gfc_free_acc_clauses (c);
      return MATCH_ERROR;
    }

  *cp = c;
  return MATCH_YES;
}

#define ACC_PARALLEL_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_ASYNC | ACC_CLAUSE_NUM_GANGS                                       \
   | ACC_CLAUSE_NUM_WORKERS | ACC_CLAUSE_VECTOR_LENGTH | ACC_CLAUSE_REDUCTION                    \
   | ACC_CLAUSE_COPY | ACC_CLAUSE_COPYIN | ACC_CLAUSE_COPYOUT                                    \
   | ACC_CLAUSE_CREATE | ACC_CLAUSE_PRESENT | ACC_CLAUSE_PRESENT_OR_COPY                         \
   | ACC_CLAUSE_PRESENT_OR_COPYIN | ACC_CLAUSE_PRESENT_OR_COPYOUT | ACC_CLAUSE_PRESENT_OR_CREATE \
   | ACC_CLAUSE_DEVICEPTR | ACC_CLAUSE_PRIVATE | ACC_CLAUSE_FIRSTPRIVATE | ACC_CLAUSE_DEFAULT    \
   | ACC_CLAUSE_WAIT)

#define ACC_KERNELS_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_ASYNC | ACC_CLAUSE_DEVICEPTR                                       \
   | ACC_CLAUSE_COPY | ACC_CLAUSE_COPYIN | ACC_CLAUSE_COPYOUT                                    \
   | ACC_CLAUSE_CREATE | ACC_CLAUSE_PRESENT | ACC_CLAUSE_PRESENT_OR_COPY                         \
   | ACC_CLAUSE_PRESENT_OR_COPYIN | ACC_CLAUSE_PRESENT_OR_COPYOUT | ACC_CLAUSE_PRESENT_OR_CREATE \
   | ACC_CLAUSE_DEFAULT | ACC_CLAUSE_WAIT)

#define ACC_DATA_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_DEVICEPTR                                                          \
   | ACC_CLAUSE_COPY | ACC_CLAUSE_COPYIN | ACC_CLAUSE_COPYOUT                                    \
   | ACC_CLAUSE_CREATE | ACC_CLAUSE_PRESENT | ACC_CLAUSE_PRESENT_OR_COPY                         \
   | ACC_CLAUSE_PRESENT_OR_COPYIN | ACC_CLAUSE_PRESENT_OR_COPYOUT | ACC_CLAUSE_PRESENT_OR_CREATE)

#define ACC_LOOP_CLAUSES \
  (ACC_CLAUSE_COLLAPSE | ACC_CLAUSE_GANG | ACC_CLAUSE_WORKER | ACC_CLAUSE_VECTOR \
   | ACC_CLAUSE_SEQ | ACC_CLAUSE_INDEPENDENT | ACC_CLAUSE_PRIVATE | ACC_CLAUSE_REDUCTION)

#define ACC_PARALLEL_LOOP_CLAUSES \
  (ACC_LOOP_CLAUSES | ACC_PARALLEL_CLAUSES)

#define ACC_KERNELS_LOOP_CLAUSES \
  (ACC_LOOP_CLAUSES | ACC_KERNELS_CLAUSES)

#define ACC_HOST_DATA_CLAUSES ACC_CLAUSE_USE_DEVICE

#define ACC_DECLARE_CLAUSES \
  (ACC_CLAUSE_COPY | ACC_CLAUSE_COPYIN | ACC_CLAUSE_COPYOUT | ACC_CLAUSE_CREATE \
   | ACC_CLAUSE_DEVICEPTR | ACC_CLAUSE_DEVICE_RESIDENT | ACC_CLAUSE_PRESENT | ACC_CLAUSE_PRESENT_OR_COPY \
   | ACC_CLAUSE_PRESENT_OR_COPYIN | ACC_CLAUSE_PRESENT_OR_COPYOUT | ACC_CLAUSE_PRESENT_OR_CREATE)

#define ACC_UPDATE_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_ASYNC | ACC_CLAUSE_HOST | ACC_CLAUSE_DEVICE)

#define ACC_ENTER_DATA_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_ASYNC | ACC_CLAUSE_WAIT | ACC_CLAUSE_COPYIN | ACC_CLAUSE_CREATE \
   | ACC_CLAUSE_PRESENT_OR_COPYIN | ACC_CLAUSE_PRESENT_OR_CREATE)

#define ACC_EXIT_DATA_CLAUSES \
  (ACC_CLAUSE_IF | ACC_CLAUSE_ASYNC | ACC_CLAUSE_WAIT | ACC_CLAUSE_COPYOUT | ACC_CLAUSE_DELETE)

match
gfc_match_acc_parallel_loop (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_PARALLEL_LOOP_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_PARALLEL_LOOP;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_parallel (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_PARALLEL_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_PARALLEL;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_kernels_loop (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_KERNELS_LOOP_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_KERNELS_LOOP;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_kernels (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_KERNELS_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_KERNELS;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_data (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_DATA_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_DATA;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_host_data (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_HOST_DATA_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_HOST_DATA;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_loop (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_LOOP_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_LOOP;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_declare (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_DECLARE_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_update (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_UPDATE_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_UPDATE;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_enter_data (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_ENTER_DATA_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_ENTER_DATA;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_exit_data (void)
{
  gfc_acc_clauses *c;
  if (match_acc_clauses (&c, ACC_EXIT_DATA_CLAUSES) != MATCH_YES)
    return MATCH_ERROR;

  new_st.op = EXEC_ACC_EXIT_DATA;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_wait (void)
{
  gfc_acc_clauses *c = gfc_get_acc_clauses ();
  gfc_match (" ( %e )", &c->non_clause_wait_expr);

  new_st.op = EXEC_ACC_WAIT;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

match
gfc_match_acc_cache (void)
{
  gfc_acc_clauses *c = gfc_get_acc_clauses ();
  match m = match_acc_variable_list (" (",&c->lists[ACC_LIST_CACHE], true, true);
  if (m != MATCH_YES)
    {
      gfc_free_acc_clauses(c);
      return m;
    }

  if (gfc_current_state() != COMP_DO)
    {
      gfc_error ("ACC CACHE directive must be inside of loop %C");
      gfc_free_acc_clauses(c);
      return MATCH_ERROR;
    }

  new_st.op = EXEC_ACC_CACHE;
  new_st.ext.acc_clauses = c;
  return MATCH_YES;
}

struct acc_context
{
  gfc_code *code;
  struct acc_context *previous;
} *acc_current_ctx;

void
gfc_resolve_acc_blocks (gfc_code *code, gfc_namespace *ns)
{
  struct acc_context ctx;

  /* kernels or parallel regions may not contain other parallel or kernels regions */
  if (code->op == EXEC_ACC_PARALLEL || code->op == EXEC_ACC_PARALLEL_LOOP ||
      code->op == EXEC_ACC_KERNELS || code->op == EXEC_ACC_KERNELS_LOOP)
    {
      struct acc_context* c;
      for (c = acc_current_ctx; c; c = c->previous)
        if (c->code->op == EXEC_ACC_PARALLEL || c->code->op == EXEC_ACC_PARALLEL_LOOP
            || c->code->op == EXEC_ACC_KERNELS || c->code->op == EXEC_ACC_KERNELS_LOOP)
          {
            const char *name;
            if (c->code->op == EXEC_ACC_PARALLEL || c->code->op == EXEC_ACC_PARALLEL_LOOP)
              name = "PARALLEL";
            else
              name = "KERNELS";
            gfc_error ("%s construct may not contain parallel or kernels regions %L",
                     name, &code->loc);
          }
    }

  ctx.code = code;
  ctx.previous = acc_current_ctx;
  acc_current_ctx = &ctx;

  gfc_resolve_blocks (code->block, ns);

  acc_current_ctx = ctx.previous;
}

static void
resolve_acc_scalar_int_expr (gfc_expr *expr, const char *clause)
{
  if (!gfc_resolve_expr (expr)
      || expr->ts.type != BT_INTEGER || expr->rank != 0)
    gfc_error ("%s clause at %L requires a scalar INTEGER expression",
                     clause, &expr->where);
}

static void
resolve_acc_positive_int_expr (gfc_expr *expr, const char *clause)
{
  resolve_acc_scalar_int_expr (expr, clause);
  if (expr->expr_type == EXPR_CONSTANT && expr->ts.type == BT_INTEGER
      && expr->value.integer->_mp_size <= 0)
    gfc_warning ("INTEGER expression of %s clause at %L must be positive",
                     clause, &expr->where);
}

static void
resolve_subarray (gfc_namelist *n, locus *where)
{
  struct gfc_acc_subarray *sa = n->acc_subarray;
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
          gfc_warning ("All dimensions of subarray except last must specify whole dimension %L", where);

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

static void
resolve_acc_clauses (gfc_code *code)
{
  gfc_acc_clauses *acc_clauses = code->ext.acc_clauses;
  gfc_namelist *n;
  gfc_exprlist *el;
  int list;
  static const char *clause_names[]
    = { "COPY", "COPYIN", "COPYOUT", "CREATE", "DELETE",
        "PRESENT", "PRESENT_OR_COPY", "PRESENT_OR_COPYIN", "PRESENT_OR_COPYOUT", "PRESENT_OR_CREATE",
        "DEVICEPTR", "PRIVATE", "FIRSTPRIVATE", "USE_DEVICE", "DEVICE_RESIDENT",
        "HOST", "DEVICE", "CACHE", "REDUCTION"};

  if (acc_clauses == NULL)
    return;

  /* Expressions */
  if (acc_clauses->if_expr)
    {
      gfc_expr *expr = acc_clauses->if_expr;
      if (!gfc_resolve_expr (expr)
          || expr->ts.type != BT_LOGICAL || expr->rank != 0)
        gfc_error ("IF clause at %L requires a scalar LOGICAL expression",
                           &expr->where);
    }
  if (acc_clauses->async)
    if (acc_clauses->async_expr)
      resolve_acc_scalar_int_expr (acc_clauses->async_expr, "ASYNC");
  if (acc_clauses->num_gangs_expr)
    resolve_acc_positive_int_expr (acc_clauses->num_gangs_expr, "NUM_GANGS");
  if (acc_clauses->num_workers_expr)
    resolve_acc_positive_int_expr (acc_clauses->num_workers_expr, "NUM_WORKERS");
  if (acc_clauses->vector_length_expr)
    resolve_acc_positive_int_expr (acc_clauses->vector_length_expr, "VECTOR_LENGTH");
  if (acc_clauses->gang_expr)
    resolve_acc_positive_int_expr (acc_clauses->gang_expr, "GANG");
  if (acc_clauses->worker_expr)
    resolve_acc_positive_int_expr (acc_clauses->worker_expr, "WORKER");
  if (acc_clauses->vector_expr)
    resolve_acc_positive_int_expr (acc_clauses->vector_expr, "VECTOR");
  if (acc_clauses->wait)
    if (acc_clauses->waitlist)
      for (el = acc_clauses->waitlist; el; el = el->next)
        resolve_acc_positive_int_expr (el->expr, "WAIT");

  if (acc_clauses->seq && acc_clauses->independent)
    gfc_error ("Both SEQ and INDEPENDENT clauses are not allowed at %L",
               &code->loc);

  for (list = 0; list < ACC_LIST_NUM; list++)
    for (n = acc_clauses->lists[list]; n; n = n->next)
      {
        n->sym->mark = 0;
        if (n->sym->attr.flavor == FL_VARIABLE)
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
            if (n->sym->attr.proc_pointer)
              continue;
          }
        gfc_error ("Object '%s' is not a variable at %L", n->sym->name,
                   &code->loc);
      }

  // todo:
  /* Check the variable does not present in multiple clauses.
     However, following combinations of clauses are allowed
     1) copyin & copyout
     2) present_or_copyin & copyout
     Consequently, we need to drop these clauses to check them later
   */
  for (list = 0; list < ACC_LIST_NUM; list++)
    for (n = acc_clauses->lists[list]; n; n = n->next)
      {
        if (n->sym->mark)
          gfc_warning ("Symbol '%s' present on multiple clauses at %L",
              n->sym->name, &code->loc);
        else
          n->sym->mark = 1;
      }
  /* Lists */
  for (list = 0; list < ACC_LIST_NUM; list++)
    if ((n = acc_clauses->lists[list]) != NULL)
      {
        const char *name;

        if (list < ACC_LIST_REDUCTION_FIRST)
          name = clause_names[list];
        else if (list <= ACC_LIST_REDUCTION_LAST)
          name = clause_names[ACC_LIST_REDUCTION_FIRST];
        else
          gcc_unreachable ();

        if (n->acc_subarray)
          resolve_subarray (n, &code->loc);

        if (list > ACC_LIST_DATA_CLAUSE_LAST)
          {
            if (n->sym->attr.pointer)
              gfc_error ("POINTER object '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
            if (n->sym->as && n->sym->as->type == AS_ASSUMED_SIZE)
              gfc_error ("Assumed size array '%s' in %s clause at %L",
                         n->sym->name, name, &code->loc);
          }

        if (list == ACC_LIST_DEVICEPTR)
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

        if (list >= ACC_LIST_DATA_CLAUSE_FIRST
            && list <= ACC_LIST_DATA_CLAUSE_LAST)
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

        switch (list)
          {
          case ACC_LIST_PLUS:
          case ACC_LIST_MULT:
          case ACC_LIST_SUB:
            if (!gfc_numeric_ts (&n->sym->ts))
              gfc_error ("%c REDUCTION variable '%s' at %L must be of numeric type, got %s",
                         list == ACC_LIST_PLUS ? '+'
                         : list == ACC_LIST_MULT ? '*' : '-',
                         n->sym->name, &code->loc,
                         gfc_typename (&n->sym->ts));
            break;
          case ACC_LIST_AND:
          case ACC_LIST_OR:
          case ACC_LIST_EQV:
          case ACC_LIST_NEQV:
            if (n->sym->ts.type != BT_LOGICAL)
              gfc_error ("%s REDUCTION variable '%s' must be LOGICAL "
                         "at %L",
                         list == ACC_LIST_AND ? ".AND."
                         : list == ACC_LIST_OR ? ".OR."
                         : list == ACC_LIST_EQV ? ".EQV." : ".NEQV.",
                         n->sym->name, &code->loc);
            break;
          case ACC_LIST_MAX:
          case ACC_LIST_MIN:
            if (n->sym->ts.type != BT_INTEGER
                && n->sym->ts.type != BT_REAL)
              gfc_error ("%s REDUCTION variable '%s' must be "
                         "INTEGER or REAL at %L",
                         list == ACC_LIST_MAX ? "MAX" : "MIN",
                         n->sym->name, &code->loc);
            break;
          case ACC_LIST_IAND:
          case ACC_LIST_IOR:
          case ACC_LIST_IEOR:
            if (n->sym->ts.type != BT_INTEGER)
              gfc_error ("%s REDUCTION variable '%s' must be INTEGER "
                         "at %L",
                         list == ACC_LIST_IAND ? "IAND"
                         : list == ACC_LIST_MULT ? "IOR" : "IEOR",
                         n->sym->name, &code->loc);
            break;
          }
      }
}

static void
resolve_acc_loop(gfc_code *code)
{
  gfc_code *do_code, *c;
  int i, collapse;
  gfc_symbol *dovar;
  int list;

  if (code->ext.acc_clauses)
    resolve_acc_clauses (code);

  for (list = ACC_LIST_REDUCTION_FIRST; list <= ACC_LIST_REDUCTION_LAST; list++)
    if (code->ext.acc_clauses->lists[list])
      {
        bool kernels_region = false;
        struct acc_context *c;

        if (code->op == EXEC_ACC_KERNELS_LOOP)
          kernels_region = true;
        if (!kernels_region)
          for (c = acc_current_ctx; c != NULL && !kernels_region; c = c->previous)
            if (c->code->op == EXEC_ACC_KERNELS)
              kernels_region = true;

        if (kernels_region)
          {
            gfc_error ("REDUCTION clause is not allowed in KERNELS region at %L",
                &code->loc);
            break;
          }
      }

  do_code = code->block->next;
  collapse = code->ext.acc_clauses->collapse;

  if (collapse <= 0)
    collapse = 1;
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
                  gfc_error ("!$ACC LOOP collapsed loops don't form rectangular iteration space at %L",
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
            gfc_error ("collapsed !$ACC LOOP loops not perfectly nested at %L",
                       &c->loc);
            break;
          }
      if (c)
        break;
      do_code = do_code->block;
      if (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE)
        {
          gfc_error ("not enough DO loops for collapsed !$ACC LOOP at %L",
                     &code->loc);
          break;
        }
      do_code = do_code->next;
      if (do_code == NULL
          || (do_code->op != EXEC_DO && do_code->op != EXEC_DO_WHILE))
        {
          gfc_error ("not enough DO loops for collapsed !$ACC LOOP at %L",
                     &code->loc);
          break;
        }
    }
}

static void
resolve_acc_cache (gfc_code *code)
{
  gfc_namelist* n;

  for (n = code->ext.acc_clauses->lists[ACC_LIST_CACHE]; n; n = n->next)
    if (!n->acc_subarray)
      gfc_error ("Only single array elements and subarrays are allowed in ACC CACHE directive %L",
                 &code->loc);
}

/* Resolve OpenACC directive clauses and check various requirements
   of each directive.  */

void
gfc_resolve_acc_directive (gfc_code *code, gfc_namespace *ns ATTRIBUTE_UNUSED)
{
  switch (code->op)
    {
    case EXEC_ACC_CACHE:
      resolve_acc_cache (code);
      break;
    case EXEC_ACC_WAIT:
      break;
    case EXEC_ACC_UPDATE:
    case EXEC_ACC_HOST_DATA:
    case EXEC_ACC_DATA:
    case EXEC_ACC_KERNELS:
    case EXEC_ACC_PARALLEL:
    case EXEC_ACC_ENTER_DATA:
    case EXEC_ACC_EXIT_DATA:
      resolve_acc_clauses (code);
      break;
    case EXEC_ACC_LOOP:
    case EXEC_ACC_KERNELS_LOOP:
    case EXEC_ACC_PARALLEL_LOOP:
      resolve_acc_loop (code);
      break;
    default:
      break;
    }
}
