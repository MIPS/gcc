/* Supporting functions for resolving DATA statement.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Lifang Zeng <zlf605@hotmail.com>

This file is part of GNU G95.

GNU G95 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU G95 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU G95; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* Notes for DATA statement implementation:
                                                                                
   We first assign initial value to each symbol by gfc_assign_data_value
   during resolveing DATA statement. Refer to check_data_variable and
   traverse_data_list in resolve.c.
                                                                                
   The complexity exists in the handleing of array section, implied do
   and array of struct appeared in DATA statement.
                                                                                
   We call gfc_conv_structure, gfc_con_array_array_initializer,
   etc., to convert the initial value. Refer to trans-expr.c and
   trans-array.c.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "toplev.h"
#include "gfortran.h"
#include "assert.h"
#include "trans.h"

/* Stack to push the current expr when we descend to a nested constructor
   of struct arrays.  */

typedef struct gfc_expr_stack
{
  gfc_expr *expr;
  struct gfc_expr_stack *next;
}
gfc_expr_stack;

/* For array of struct.  */

gfc_expr_stack *expr_stack = NULL;  

static void formalize_init_expr (gfc_expr *);

/* Calculate the array element offset.  */

static void
get_array_index (gfc_array_ref * ar, mpz_t * offset)
{
  gfc_expr *e;
  int i;
  try re;
  mpz_t delta;
  mpz_t tmp;

  mpz_init (tmp);
  mpz_set_si (*offset, 0);
  mpz_init_set_si (delta, 1);
  for (i = 0; i < ar->dimen; i++)
    {
      e = gfc_copy_expr (ar->start[i]);
      re = gfc_simplify_expr (e, 1);

      if ((gfc_is_constant_expr (ar->as->lower[i]) == 0)
	  || (gfc_is_constant_expr (ar->as->upper[i]) == 0)
	  || (gfc_is_constant_expr (e) == 0))
	gfc_error ("non-constant array in DATA statement %L.", &ar->where);        
      mpz_set (tmp, e->value.integer);
      mpz_sub (tmp, tmp, ar->as->lower[i]->value.integer);
      mpz_mul (tmp, tmp, delta);
      mpz_add (*offset, tmp, *offset);

      mpz_sub (tmp, ar->as->upper[i]->value.integer,
      ar->as->lower[i]->value.integer);
      mpz_add_ui (tmp, tmp, 1);
      mpz_mul (delta, tmp, delta);
    }
  mpz_clear (delta);
  mpz_clear (tmp);
}


/* Find if there is a constructor which offset is equal to OFFSET.  */

static gfc_expr *
find_exp_in_con (mpz_t offset, gfc_constructor *con)
{
  gfc_constructor *con1;

  for (con1 = con; con1; con1 = con1->next)
    {
      if (mpz_cmp (offset, con1->n.offset) == 0)
        return con1->expr;
    }
  return NULL;
}


/* Find if there is a constructor which component is equal to COM.  */

static gfc_constructor *
find_component_in_con (gfc_component *com, gfc_constructor *con)
{
  gfc_constructor *con1;
 
  for (con1 = con; con1; con1 = con1->next)
    {
      if (com == con1->n.component)
        return con1;
    }
  return NULL;
}


/* Allocate space for expr stack.  */

static gfc_expr_stack *
gfc_get_expr_stack (void)
{
  gfc_expr_stack *tmp;

  tmp = gfc_getmem (sizeof (gfc_expr_stack));

  return tmp;
}


/* Push EXP to expr stack.  */

static void
gfc_expr_push (gfc_expr *exp)
{
  gfc_expr_stack *tmp;

  tmp = gfc_get_expr_stack ();

  tmp->expr = exp;
  tmp->next = expr_stack;
  expr_stack = tmp;
}


/* Pop expr stack.  */

static gfc_expr_stack *
gfc_expr_pop (void)
{
  gfc_expr_stack *tmp;

  tmp = expr_stack;
  if (tmp)
    expr_stack = expr_stack->next;
  return tmp;
}


/* Assign the initial value RVALUE to  LVALUE's symbol->value.  */

void
gfc_assign_data_value (gfc_expr * lvalue, gfc_expr * rvalue, int mark,
                       mpz_t index)
{
  gfc_symbol *symbol;
  gfc_expr *value;
  gfc_ref *ref;
  ref_type type;
  gfc_expr *exp;
  gfc_constructor *con, *con1, *con2;
  gfc_typespec ts;
  mpz_t offset;
  gfc_expr_stack *exp1, *exp2;
  int result;

  mpz_init_set_si (offset, 0);
  symbol = lvalue->symtree->n.sym;
  value = symbol->value;
  ref = lvalue->ref;

  ts = lvalue->ts;
  if (ts.type != rvalue->ts.type)
    gfc_convert_type (rvalue, &ts, 0);

  /* If the symbol already has partial init value, Find the correct position to
     insert the current init expression.  */
  if (value == NULL)
    {
      while (ref != NULL)
        {
          type = ref->type;
          exp = gfc_get_expr ();
          con = gfc_get_constructor();
          switch (type)
            {
            /* Array reference.  */
            case REF_ARRAY:
              exp->expr_type = EXPR_ARRAY;
              /* Array of struct.  */
              if (ref->next)
                get_array_index (&ref->u.ar, &offset);
              else /* Scalar array.  */
                {
                  /* Full array or array section.  */
                  if (mark )
                    mpz_set (offset, index);
                  else /* Array element.  */
                    get_array_index (&ref->u.ar, &offset);
                }
              mpz_set (con->n.offset, offset);
              result = mpz_get_si (con->n.offset);
              break;

            /* Struct component reference.  */
            case REF_COMPONENT:
              exp->expr_type = EXPR_STRUCTURE;
              exp->ts.type = BT_DERIVED;
              exp->ts.derived = ref->u.c.sym;
              con->n.component = ref->u.c.component;
              break;

            default:
              gfc_todo_error ("substring reference in DATA statement");
              break;
            }
          /* Generate the current expression and push it to the stack.  */
          exp->value.constructor = con;
          gfc_expr_push (exp);
          /* Goto the next reference level.  */
          ref = ref->next;
        }
    }
  else
    {
      /* Generate the first partial init value.  */
      while (ref != NULL)
        {
          type = ref->type;
          /* Array reference.  */
          if (type == REF_ARRAY)
            {
              assert (value->expr_type == EXPR_ARRAY);
              con1 = value->value.constructor;
              /* Array of struct.  */
              if (ref->next)
                get_array_index (&ref->u.ar, &offset);
              else /* Scalar array.  */
                {
                  /* Full array or array section.  */
                  if (mark)
                    mpz_set (offset, index);
                  else /* Array element.  */
                    get_array_index (&ref->u.ar, &offset);
                }
              /* Find the same value in constructor CON1.  */
              exp = find_exp_in_con (offset, con1);
              /* If find the value, Record it in value.  */
              if (exp)
                value = exp;
              else /* Generate a new expression to store the current value.  */
                {
                  exp = gfc_get_expr ();
                  exp->expr_type = EXPR_ARRAY;
                  con = gfc_get_constructor();
                  mpz_set (con->n.offset, offset);
                  exp->value.constructor = con;
                  /* Push to expr stack.  */
                  gfc_expr_push (exp);
                  ref = ref->next;
                  break;
                }
            }
          else if (type == REF_COMPONENT) /* Struct component reference.  */
            {
              assert (value->expr_type == EXPR_STRUCTURE);
              con1 = value->value.constructor;
              con2 = find_component_in_con (ref->u.c.component, con1);
              /* If already exists, records it in VALUE.  */
              if (con2)
                value = con2->expr;
              else /* Generate a new expression to store the value and push to
                      the expr stack.  */
                {
                  exp = gfc_get_expr ();
                  exp->expr_type = EXPR_STRUCTURE;
                  exp->ts.type = BT_DERIVED;
                  exp->ts.derived = ref->u.c.sym;
                  con = gfc_get_constructor();
                  con->n.component = ref->u.c.component;
                  exp->value.constructor = con;
                  gfc_expr_push (exp);
                  ref = ref->next;
                  break;
                }
            }
          else
            {
              gfc_todo_error ("substring reference in DATA statement");
              break;
            }
          ref = ref->next;
        }

      /* dealing with the rest reference expressions.  */
      while (ref != NULL)
        {
          type = ref->type;
          exp = gfc_get_expr ();
          con = gfc_get_constructor();
          switch (type)
            {
            case REF_ARRAY:
              exp->expr_type = EXPR_ARRAY;
              if (ref->next)
                get_array_index (&ref->u.ar, &offset);
              else
                {
                  if (mark)
                    mpz_set (offset, index);
                  else
                    get_array_index (&ref->u.ar, &offset);
                }
              mpz_set (con->n.offset, offset);
              break;

            case REF_COMPONENT:
              exp->expr_type = EXPR_STRUCTURE;
              exp->ts.type = BT_DERIVED;
              exp->ts.derived = ref->u.c.sym;
              con->n.component = ref->u.c.component;
              break;

            default:
              gfc_todo_error ("substring reference in DATA statement");
	      break;
            }
          exp->value.constructor = con;
          gfc_expr_push (exp);
          ref = ref->next;
        }
     }

  mpz_clear (offset);
  /* Pop the expr stack and form the final expression.  */
  exp1 = gfc_expr_pop ();
  if (exp1 != NULL)
    {
      exp1->expr->value.constructor->expr = gfc_copy_expr (rvalue);
      exp2 = gfc_expr_pop ();
      while (exp2)
        {
          exp2->expr->value.constructor->expr = exp1->expr;
          exp1 = exp2;
          exp2 = gfc_expr_pop ();
        }
  
     if (value)
       gfc_insert_constructor (value, exp1->expr);
     else
       value = exp1->expr;
    }
  else
    value = gfc_copy_expr (rvalue);

  if (symbol->value == NULL)
    symbol->value = value;
}


/* Modify the index of array section and re-calculate the array offset.  */

void 
gfc_modify_index_and_calculate_offset (mpz_t *section_index, gfc_array_ref *ar,
                                       mpz_t *offset_ret)
{
  int i;
  mpz_t delta;
  mpz_t tmp; 

  for (i = 0; i < ar->dimen; i++)
    {
      mpz_add (section_index[i], section_index[i],
	       ar->stride[i]->value.integer);
      
      if (mpz_cmp_si (ar->stride[i]->value.integer, 0) >= 0)
        {
          /* End the current loop and reset index to start.  */
          if (mpz_cmp (section_index[i], ar->end[i]->value.integer) > 0)
            mpz_set (section_index[i], ar->start[i]->value.integer);
          else
            break;
        }
      else
        {
          if (mpz_cmp (section_index[i], ar->end[i]->value.integer) < 0)
            mpz_set (section_index[i], ar->start[i]->value.integer);
          else
            break;
        }
    }

  mpz_set_si (*offset_ret, 0);
  mpz_init_set_si (delta, 1);
  mpz_init (tmp);
  for (i = 0; i < ar->dimen; i++)
    {
      mpz_sub (tmp, section_index[i], ar->as->lower[i]->value.integer);
      mpz_mul (tmp, tmp, delta);
      mpz_add (*offset_ret, tmp, *offset_ret);

      mpz_sub (tmp, ar->as->upper[i]->value.integer, 
               ar->as->lower[i]->value.integer);
      mpz_add_ui (tmp, tmp, 1);
      mpz_mul (delta, tmp, delta);
    }
  mpz_clear (tmp);
  mpz_clear (delta);
}


/* Rearrange a structure constructor so the elements are in the specified
   order.  Also insert NULL entries if neccessary.  */

static void
formalize_structure_cons (gfc_expr * expr)
{
  gfc_constructor *head;
  gfc_constructor *tail;
  gfc_constructor *cur;
  gfc_constructor *last;
  gfc_constructor *c;
  gfc_component *order;

  c = expr->value.constructor;

  /* Constructor is already fomalized.  */
  if (c->n.component == NULL)
    return;

  head = tail = NULL;
  for (order = expr->ts.derived->components; order; order = order->next)
    {
      /* Find the next component.  */
      last = NULL;
      cur = c;
      while (cur != NULL && cur->n.component != order)
	{
	  last = cur;
	  cur = cur->next;
	}

      if (cur == NULL)
	{
	  /* Create a new one.  */
	  cur = gfc_get_constructor ();
	}
      else
	{
	  /* Remove it from the chain.  */
	  if (last == NULL)
	    c = cur->next;
	  else
	    last->next = cur->next;
	  cur->next = NULL;

	  formalize_init_expr (cur->expr);
	}

      /* Add it to the new constructor.  */
      if (head == NULL)
	head = tail = cur;
      else
	{
	  tail->next = cur;
	  tail = tail->next;
	}
    }
  assert (c == NULL);
  expr->value.constructor = head;
}


/* Make sure an initialization expression is in normalized form.  Ie. all
   elements of the constructors are in the correct order.  */

static void
formalize_init_expr (gfc_expr * expr)
{
  expr_t type;
  gfc_constructor *c;

  if (expr == NULL)
    return;

  type = expr->expr_type;
  switch (type)
    {
    case EXPR_ARRAY:
      c = expr->value.constructor;
      while (c)
	{
	  formalize_init_expr (c->expr);
	  c = c->next;
	}
      break;

    case EXPR_STRUCTURE:
      formalize_structure_cons (expr);
      break;

    default:
      break;
    }
}


/* Resolve symbol's initial value after all data statement.  */

void
gfc_formalize_init_value (gfc_symbol *sym)
{
  formalize_init_expr (sym->value);
}


/* Get the integer value into RET_AS and SECTION from AS and AR, and return
   offset.  */
 
void
gfc_get_section_index (gfc_array_ref *ar, mpz_t *section_index, mpz_t *offset)
{
  int i;
  mpz_t delta;
  mpz_t tmp;

  mpz_set_si (*offset, 0);
  mpz_init (tmp);
  mpz_init_set_si (delta, 1);
  for (i = 0; i < ar->dimen; i++)
    {
      mpz_init (section_index[i]);
      mpz_sub (tmp, ar->start[i]->value.integer,
               ar->as->lower[i]->value.integer);
      mpz_mul (tmp, tmp, delta);
      mpz_add (*offset, tmp, *offset);

      mpz_sub (tmp, ar->as->upper[i]->value.integer, 
               ar->as->lower[i]->value.integer);
      mpz_add_ui (tmp, tmp, 1);
      mpz_mul (delta, tmp, delta);

      if (mpz_cmp_si (ar->start[i]->value.integer, 0) == 0)
        mpz_set (section_index[i], ar->as->lower[i]->value.integer);
      else
        mpz_set (section_index[i], ar->start[i]->value.integer);
    }

  mpz_clear (tmp);
  mpz_clear (delta);
}

