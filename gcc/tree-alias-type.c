/* Tree based linear points-to analysis
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "config.h"
#include "system.h"
#include "ggc.h"
#include "tree-alias-type.h"
#include "tree-alias-ecr.h"

static unsigned int alias_type_num = 1;

/* For walking operations, we color the alias types rather than have
   to reset marks. */

unsigned int next_color = 1;

static void void_unify PARAMS ((alias_type, alias_type));
static varray_type null_pointsto PARAMS ((alias_type));
static void location_unify PARAMS ((alias_type, alias_type));
static varray_type location_pointsto PARAMS ((alias_type));
static void function_unify PARAMS ((alias_type, alias_type));
static varray_type function_pointsto PARAMS ((alias_type));

/* BOTTOM */
extern GTY(()) alias_type alias_bottom;
#include "gt-tree-alias-type.h"

alias_type alias_bottom = NULL;

void
init_alias_type ()
{
  if (!alias_bottom)
    {
      alias_bottom = ggc_alloc (sizeof (union alias_type_def));
      alias_bottom->common.kind = BOTTOM_ATYPE;
      alias_bottom->common.id = 0;
      alias_bottom->common.pointsto = null_pointsto;
      alias_bottom->common.unify = void_unify;
    }
}

/* Unification routine that does nothing.  */
static void
void_unify (a, b)
     alias_type a ATTRIBUTE_UNUSED;
     alias_type b ATTRIBUTE_UNUSED;
{
}

/* Points-to routine that returns an empty points-to set.  */
static varray_type
null_pointsto (a)
     alias_type a ATTRIBUTE_UNUSED;
{
  return NULL;
}

/* Create a new alias value type */
alias_type
alias_vtype_new ()
{
  return alias_vtype_new_with_lf (ECR_new (), ECR_new ());
}

/* Create a new alias value type with a given location and
   function. */ 
alias_type
alias_vtype_new_with_lf (loc, func)
     ECR loc;
     ECR func;
{
  alias_type ret = ggc_alloc (sizeof (union alias_type_def));
  ALIAS_TYPE_KIND (ret) = VALUE_ATYPE;
  ALIAS_TYPE_ID (ret) = alias_type_num++;
  SET_ALIAS_TYPE_UNIFY (ret, void_unify);
  SET_ALIAS_TYPE_POINTSTO (ret, null_pointsto);
  ALIAS_VTYPE_FUNC (ret) = func;
  ALIAS_VTYPE_LOC (ret) = loc;
  return ret;
}

/* Return the alias value type's location ECR. */
ECR
alias_vtype_loc (vt)
     alias_type vt;
{
  return ECR_find (ALIAS_VTYPE_LOC (vt));
}

/* Return the alias value type's function ECR. */
ECR
alias_vtype_func (vt)
     alias_type vt;
{
  return ECR_find (ALIAS_VTYPE_FUNC (vt));
}

/* Create a new location alias type.  */
alias_type
alias_ltype_new ()
{
  return alias_ltype_new_with_lf (ECR_new (), ECR_new ());
}

/* Create a new location alias type with a given location and
   function.  */
alias_type
alias_ltype_new_with_lf (loc, func)
     ECR loc;
     ECR func;
{
  alias_type ret = ggc_alloc (sizeof (union alias_type_def));
  ALIAS_TYPE_KIND (ret) = LOCATION_ATYPE;
  ALIAS_TYPE_ID (ret) = alias_type_num++;
  SET_ALIAS_TYPE_UNIFY (ret, location_unify);
  SET_ALIAS_TYPE_POINTSTO (ret, location_pointsto);
  ALIAS_LTYPE_FUNC (ret) = func;
  ALIAS_LTYPE_LOC (ret) = loc;
  return ret;
}

/* Return a given alias location type's location ECR.  */
ECR
alias_ltype_loc (vt)
     alias_type vt;
{
  return ECR_find (ALIAS_LTYPE_LOC (vt));
}

/* Return a given alias location type's function ECR.  */
ECR
alias_ltype_func (vt)
     alias_type vt;
{
  return ECR_find (ALIAS_LTYPE_FUNC (vt));
}

static void
location_unify (t1, t2)
     alias_type t1;
     alias_type t2;
{
  ECR tau1 = alias_ltype_loc (t1);
  ECR tau2 = alias_ltype_loc (t2);
  ECR lambda1 = alias_ltype_func (t1);
  ECR lambda2 = alias_ltype_func (t2);

  if (!ECR_equiv (tau1, tau2))
    ECR_join (tau1, tau2);

  if (!ECR_equiv (lambda1, lambda2))
    ECR_join (lambda1, lambda2);
}

static varray_type
location_pointsto (t)
     alias_type t;
{
  varray_type ret;
  VARRAY_GENERIC_PTR_INIT (ret, 1, "Points-to");
  if (ECR_get_type (ALIAS_LTYPE_LOC (t)) != alias_bottom)
    VARRAY_PUSH_GENERIC_PTR (ret, ALIAS_LTYPE_LOC (t));
  if (ECR_get_type (ALIAS_LTYPE_FUNC (t)) != alias_bottom)
    VARRAY_PUSH_GENERIC_PTR (ret, ALIAS_LTYPE_FUNC (t));
  return ret;
}


alias_typevar
alias_tvar_new_with_at (decl, type)
     tree decl;
     alias_type type;
{
  alias_typevar ret = ggc_alloc (sizeof (struct alias_typevar_def));
  ret->decl = decl;
  ret->ecr = ECR_new_with_type (type, ret);
  return ret;
}

alias_typevar
alias_tvar_new (decl)
     tree decl;
{
  return alias_tvar_new_with_at (decl, alias_ltype_new ());
}

alias_typevar
alias_tvar_new_equiv_to (decl, var)
     tree decl;
     alias_typevar var;
{
  alias_typevar ret = ggc_alloc (sizeof (struct alias_typevar_def));
  ret->decl = decl;
  ret->ecr = ECR_new_with_type (ECR_get_type (alias_tvar_get_ECR (var)), ret);
  ECR_union (ret->ecr, alias_tvar_get_ECR (var));
  return ret;
}

ECR
alias_tvar_get_ECR (var)
     alias_typevar var;
{
  return ECR_find (var->ecr);
}

ECR
alias_tvar_get_orig_ECR (var)
     alias_typevar var;
{
  return var->ecr;
}

bool
alias_tvar_is_alias (var)
     alias_typevar var;
{
  return ECR_size (var->ecr) > 1;
}

varray_type
alias_tvar_pointsto (var)
     alias_typevar var;
{
  varray_type v = ALIAS_TYPE_POINTSTO (ECR_get_type (var->ecr));
  varray_type p;
  size_t i, l;

  VARRAY_GENERIC_PTR_INIT (p, 1, "Points to set");
  l = VARRAY_ACTIVE_SIZE (v);
  for (i = 0; i < l; i++)
    {
      ECR ecr1 = VARRAY_GENERIC_PTR (v, i);
      size_t j;
      varray_type e2 = ECR_elems (ecr1);
      for (j = 0; j < VARRAY_ACTIVE_SIZE (e2); j++)
	{
	  ECR ecr2 = VARRAY_GENERIC_PTR (e2, j);
	  if (ECR_get_typevar (ecr2) != NULL)
	    {
	      VARRAY_PUSH_GENERIC_PTR (p, ECR_get_typevar (ecr2));
	    }
	}
      VARRAY_CLEAR (e2);
    }
  VARRAY_CLEAR (v);
  return p;
}

void
alias_tvar_allpointsto (var, tv)
     alias_typevar var;
     varray_type *tv;
{
  ECR e;
  if (ECR_get_type (var->ecr) == alias_bottom)
    return;
  e = alias_ltype_loc (ECR_get_type (var->ecr));

  if (e->color == next_color)
    return;

  if (ECR_get_type (e) == alias_bottom)
    return;

  VARRAY_PUSH_GENERIC_PTR ((*tv), e);
  e->color = next_color;
  if (ECR_get_typevar (e) == NULL)
    return;
  alias_tvar_allpointsto (ECR_get_typevar (e), tv);
}

alias_type
alias_ftype_new ()
{
  alias_type ret = ggc_alloc (sizeof (union alias_type_def));
  ALIAS_TYPE_KIND (ret) = FUNCTION_ATYPE;
  ALIAS_TYPE_ID (ret) = alias_type_num++;
  SET_ALIAS_TYPE_UNIFY (ret, function_unify);
  SET_ALIAS_TYPE_POINTSTO (ret, function_pointsto);
  ALIAS_FTYPE_RETVAL (ret) = NULL;
  VARRAY_GENERIC_PTR_INIT (ALIAS_FTYPE_ARGUMENTS (ret), 1,
			   "Function arguments");
  return ret;
}

void
alias_ftype_add_argument (ftype, arg)
     alias_type ftype;
     alias_type arg;
{
  VARRAY_PUSH_GENERIC_PTR (ALIAS_FTYPE_ARGUMENTS (ftype), arg);
}

void
alias_ftype_add_new_arguments (ftype, num)
     alias_type ftype;
     int num;
{
  int i;
  for (i = 0; i < num; i++)
    alias_ftype_add_argument (ftype, alias_vtype_new ());
}

static varray_type
function_pointsto (ftype)
     alias_type ftype ATTRIBUTE_UNUSED;
{
  varray_type ret;
  VARRAY_GENERIC_PTR_INIT (ret, 1, "Points-to");
  return ret;
}

static void
function_unify (t1, t2)
     alias_type t1;
     alias_type t2;
{
  varray_type args1 = ALIAS_FTYPE_ARGUMENTS (t1);
  varray_type args2 = ALIAS_FTYPE_ARGUMENTS (t2);
  size_t l1 = VARRAY_ACTIVE_SIZE (args1);
  size_t l2 = VARRAY_ACTIVE_SIZE (args2);
  size_t l = l1;
  alias_type alpha2i;
  size_t i;
  alias_type alpha1, alpha2;
  ECR tau1, tau2;
  ECR lambda1, lambda2;


  if (l2 > l)
    l = l2;
  /* First process the arguments. */
  alpha2i = NULL;
  for (i = 0; i < l; i++)
    {

      /* Get the next argument for this function (t1). if there isn't any,
         create one. */
      if (i >= l1)
	{
	  alpha1 = alias_vtype_new ();
	  alias_ftype_add_argument (t1, alpha1);
	}
      else
	{
	  alpha1 = VARRAY_GENERIC_PTR (args1, i);
	}

      /* Get the next argument for the given function (t2). If there
         isn't any, create one. */
      if (i >= l2)
	{
	  if (alpha2i == NULL)
	    alpha2i = alias_vtype_new ();
	  alpha2 = alpha2i;
	}
      else
	{
	  alpha2 = VARRAY_GENERIC_PTR (args2, i);
	}
      tau1 = alias_vtype_loc (alpha1);
      tau2 = alias_vtype_loc (alpha2);
      /* XXX: Is this supposed to be an equivalence check? */
      if (tau1 != tau2)
	ECR_join (tau1, tau2);
      lambda1 = alias_vtype_func (alpha1);
      lambda2 = alias_vtype_func (alpha2);
      /* XXX: Ditto */
      if (lambda1 != lambda2)
	ECR_join (lambda1, lambda2);
    }
  /* Now, process the return type. */
  alpha1 = ALIAS_FTYPE_RETVAL (t1);
  alpha2 = ALIAS_FTYPE_RETVAL (t2);

  tau1 = alias_vtype_loc (alpha1);
  tau2 = alias_vtype_loc (alpha2);
  /* XXX: Is this supposed to be an equivalence check? */
  if (tau1 != tau2)
    ECR_join (tau1, tau2);
  lambda1 = alias_vtype_func (alpha1);
  lambda2 = alias_vtype_func (alpha2);
  /* XXX: Ditto */
  if (lambda1 != lambda2)
    ECR_join (lambda1, lambda2);
}
