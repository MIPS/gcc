/* Expression translation
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Paul Brook <paul@nowt.org>
   and Steven Bosscher <s.bosscher@student.tudelft.nl>

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

/* trans-expr.c-- generate SIMPLE trees for gfc_expr.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include <stdio.h>
#include <string.h>
#include "ggc.h"
#include "toplev.h"
#include "real.h"
#include "tree-simple.h"
#include "flags.h"
#include <gmp.h>
#include <assert.h>
#include "gfortran.h"
#include "intrinsic.h"
#include "trans.h"
#include "trans-const.h"
#include "trans-types.h"
#include "trans-array.h"
#include "defaults.h"
/* Only for gfc_trans_assign and gfc_trans_pointer_assign.  */
#include "trans-stmt.h"

/* This maps fortran intrinsic math functions to external library or GCC
   builtin functions.  */
typedef struct gfc_intrinsic_map_t	GTY(())
{
  /* The explicit enum is required to work around inadequacies in the
     garbage collection/gengtype parsing mechanism.  */
  const enum gfc_generic_isym_id id;
  const char *name;
  const int code4;
  const int code8;
  tree GTY(()) real4_decl;
  tree GTY(()) real8_decl;
  tree GTY(()) complex4_decl;
  tree GTY(()) complex8_decl;
}
gfc_intrinsic_map_t;

#define I_LIB(id, name) {GFC_ISYM_ ## id, name, \
    END_BUILTINS, END_BUILTINS, \
    NULL_TREE, NULL_TREE, NULL_TREE, NULL_TREE},
#define DEFINE_MATH_BUILTIN(id, name, nargs) {GFC_ISYM_ ## id, name, \
    BUILT_IN_ ## id ## F, BUILT_IN_ ## id, \
    NULL_TREE, NULL_TREE, NULL_TREE, NULL_TREE},
static GTY(()) gfc_intrinsic_map_t gfc_intrinsic_map[] =
{
  /* Math functions.  These are in libm.  */
  I_LIB	    (ACOS,  "acos")
  I_LIB	    (ASIN,  "asin")
  I_LIB	    (COSH,  "cosh")
  I_LIB	    (LOG10, "log10")
  I_LIB	    (SINH,  "sinh")
  I_LIB	    (TANH,  "tanh")
  /* Also the builtin math functions.  */
#include "mathbuiltins.def"

  I_LIB (NONE, NULL)
};
#undef I_LIB
#undef I_BUILTIN

/* Structure for storing components of a floating number to be used by
   elemental functions to manipulate reals.  */
typedef struct
{
  tree arg;     /* Variable tree to view convert to integer.   */ 
  tree expn;    /* Variable tree to save exponent.  */
  tree frac;    /* Variable tree to save fraction.  */
  tree smask;   /* Constant tree of sign's mask.  */
  tree emask;   /* Constant tree of exponent's mask.  */
  tree fmask;   /* Constant tree of fraction's mask.  */
  tree edigits; /* Constant tree of bit numbers of exponent.  */
  tree fdigits; /* Constant tree of bit numbers of fraction.  */
  tree f1;      /* Constant tree of the f1 defined in the real model.  */
  tree bias;    /* Constant tree of the bias of exponent in the memory.  */
  tree type;    /* Type tree of arg1.  */
  tree mtype;   /* Type tree of integer type. Kind is that of arg1.  */
}
real_compnt_info;

/* Table of the <kind, range> pair defined in the integer model. The
   first element is the numbers of the pairs.  */
tree integer_kind_info = NULL;
/* Table of the <kind, precision, range> defined in real model. The
   first element is the numbers of the triples.  */
tree real_kind_info = NULL;


/* Evaluate the arguments to an intrinsic function.  */

static tree
gfc_conv_intrinsic_function_args (gfc_se * se, gfc_expr * expr)
{
  gfc_actual_arglist *actual;
  tree args;
  gfc_se argse;

  args = NULL_TREE;
  for (actual = expr->value.function.actual; actual; actual = actual->next)
    {
      /* Skip ommitted optional arguments.  */
      if (!actual->expr)
	continue;

      /* Evaluate the parameter.  This will substitute scalarized
         references automatically. */
      gfc_init_se (&argse, se);

      if (actual->expr->ts.type == BT_CHARACTER)
	{
	  gfc_conv_expr (&argse, actual->expr);
	  gfc_conv_string_parameter (&argse);
	  args = gfc_chainon_list (args, argse.string_length);
	}
      else
        gfc_conv_expr_val (&argse, actual->expr);

      gfc_add_block_to_block (&se->pre, &argse.pre);
      gfc_add_block_to_block (&se->post, &argse.post);
      args = gfc_chainon_list (args, argse.expr);
    }
  return args;
}


/* Conversions between different types are output by the frontend as
   intrinsic functions.  We implement these directly with inline code.  */

static void
gfc_conv_intrinsic_conversion (gfc_se * se, gfc_expr * expr)
{
  tree type;
  tree arg;

  /* Evaluate the argument.  */
  type = gfc_typenode_for_spec (&expr->ts);
  assert (expr->value.function.actual->expr);
  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);

  /* Conversion from complex to non-complex involves taking the real
     component of the value.  */
  if (TREE_CODE (TREE_TYPE (arg)) == COMPLEX_TYPE
      && expr->ts.type != BT_COMPLEX)
    {
      tree artype;

      artype = TREE_TYPE (TREE_TYPE (arg));
      arg = build1 (REALPART_EXPR, artype, arg);
    }

  se->expr = convert (type, arg);
}


/* This is needed because the gcc backend only implements FIX_TRUNC_EXPR
   TRUNC(x) = INT(x) <= x ? INT(x) : INT(x) - 1
   Similarly for CEILING.  */

static tree
build_fixbound_expr (stmtblock_t * pblock, tree arg, tree type, int up)
{
  tree tmp;
  tree cond;
  tree argtype;
  tree intval;

  argtype = TREE_TYPE (arg);
  arg = gfc_evaluate_now (arg, pblock);

  intval = convert (type, arg);
  intval = gfc_evaluate_now (intval, pblock);

  tmp = convert (argtype, intval);
  cond = build (up ? GE_EXPR : LE_EXPR, boolean_type_node, tmp, arg);

  tmp = build (up ? PLUS_EXPR : MINUS_EXPR, type, intval, integer_one_node);
  tmp = build (COND_EXPR, type, cond, intval, tmp);
  return tmp;
}


/* This is needed because the gcc backend only implements FIX_TRUNC_EXPR
   NINT(x) = INT(x + ((x > 0) ? 0.5 : -0.5)).  */

static tree
build_round_expr (stmtblock_t * pblock, tree arg, tree type)
{
  tree tmp;
  tree cond;
  tree neg;
  tree pos;
  tree argtype;
  REAL_VALUE_TYPE r;

  argtype = TREE_TYPE (arg);
  arg = gfc_evaluate_now (arg, pblock);

  real_from_string (&r, "0.5");
  pos = build_real (argtype, r);

  real_from_string (&r, "-0.5");
  neg = build_real (argtype, r);

  tmp = gfc_build_const (argtype, integer_zero_node);
  cond = fold (build (GT_EXPR, boolean_type_node, arg, tmp));

  tmp = fold (build (COND_EXPR, argtype, cond, pos, neg));
  tmp = fold (build (PLUS_EXPR, argtype, arg, tmp));
  return fold (build1 (FIX_TRUNC_EXPR, type, tmp));
}


/* Convert a real to an integer using a specific rounding mode.
   Ideally we would just build the corresponding GENERIC node,
   however the RTL expander only actually supports FIX_TRUNC_EXPR.  */

static tree
build_fix_expr (stmtblock_t * pblock, tree arg, tree type, int op)
{
  switch (op)
    {
    case FIX_FLOOR_EXPR:
      return build_fixbound_expr (pblock, arg, type, 0);
      break;

    case FIX_CEIL_EXPR:
      return build_fixbound_expr (pblock, arg, type, 1);
      break;

    case FIX_ROUND_EXPR:
      return build_round_expr (pblock, arg, type);

    default:
      return build1 (op, type, arg);
    }
}


/* Round a real value using the specified rounding mode.
   We use a temporary integer of that same kind size as the result.
   Values larger than can be represented by this kind are unchanged, as
   will not be accurate enough to represent the rounding.
    huge = HUGE (KIND (a))
    aint (a) = ((a > huge) || (a < -huge)) ? a : (real)(int)a
   */

static void
gfc_conv_intrinsic_aint (gfc_se * se, gfc_expr * expr, int op)
{
  tree type;
  tree itype;
  tree arg;
  tree tmp;
  tree cond;
  mpf_t huge;
  int n;
  int kind;

  kind = expr->ts.kind;

  n = END_BUILTINS;
  /* We have builtin functions for some cases.  */
  switch (op)
    {
    case FIX_ROUND_EXPR:
      switch (kind)
	{
	case 4:
	  n = BUILT_IN_ROUNDF;
	  break;

	case 8:
	  n = BUILT_IN_ROUND;
	  break;
	}
      break;

    case FIX_FLOOR_EXPR:
      switch (kind)
	{
	case 4:
	  n = BUILT_IN_FLOORF;
	  break;

	case 8:
	  n = BUILT_IN_FLOOR;
	  break;
	}
    }

  /* Evaluate the argument.  */
  assert (expr->value.function.actual->expr);
  arg = gfc_conv_intrinsic_function_args (se, expr);

  /* Use a builtin function if one exists.  */
  if (n != END_BUILTINS)
    {
      tmp = built_in_decls[n];
      se->expr = gfc_build_function_call (tmp, arg);
      return;
    }

  /* This code is probably redundant, but we'll keep it lying around just
     in case.  */
  type = gfc_typenode_for_spec (&expr->ts);
  arg = TREE_VALUE (arg);
  arg = gfc_evaluate_now (arg, &se->pre);

  /* Test if the value is too large to handle sensibly.  */
  mpf_init (huge);
  n = gfc_validate_kind (BT_INTEGER, kind);
  mpf_set_z (huge, gfc_integer_kinds[n].huge);
  tmp = gfc_conv_mpf_to_tree (huge, kind);
  cond = build (LT_EXPR, boolean_type_node, arg, tmp);

  mpf_neg (huge, huge);
  tmp = gfc_conv_mpf_to_tree (huge, kind);
  tmp = build (GT_EXPR, boolean_type_node, arg, tmp);
  cond = build (TRUTH_AND_EXPR, boolean_type_node, cond, tmp);
  itype = gfc_get_int_type (kind);

  tmp = build_fix_expr (&se->pre, arg, itype, op);
  tmp = convert (type, tmp);
  se->expr = build (COND_EXPR, type, cond, tmp, arg);
}


/* Convert to an integer using the specified rounding mode.  */

static void
gfc_conv_intrinsic_int (gfc_se * se, gfc_expr * expr, int op)
{
  tree type;
  tree arg;

  /* Evaluate the argument.  */
  type = gfc_typenode_for_spec (&expr->ts);
  assert (expr->value.function.actual->expr);
  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);

  if (TREE_CODE (TREE_TYPE (arg)) == INTEGER_TYPE)
    {
      /* Conversion to a different integer kind.  */
      se->expr = convert (type, arg);
    }
  else
    {
      /* Conversion from complex to non-complex involves taking the real
         component of the value.  */
      if (TREE_CODE (TREE_TYPE (arg)) == COMPLEX_TYPE
	  && expr->ts.type != BT_COMPLEX)
	{
	  tree artype;

	  artype = TREE_TYPE (TREE_TYPE (arg));
	  arg = build1 (REALPART_EXPR, artype, arg);
	}

      se->expr = build_fix_expr (&se->pre, arg, type, op);
    }
}


/* Get the imaginary component of a value.  */

static void
gfc_conv_intrinsic_imagpart (gfc_se * se, gfc_expr * expr)
{
  tree arg;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);
  se->expr = build1 (IMAGPART_EXPR, TREE_TYPE (TREE_TYPE (arg)), arg);
}


/* Get the complex conjugate of a value.  */

static void
gfc_conv_intrinsic_conjg (gfc_se * se, gfc_expr * expr)
{
  tree arg;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);
  se->expr = build1 (CONJ_EXPR, TREE_TYPE (arg), arg);
}


/* Initialize function decls for library functions.  The external functions
   are created as required.  Builtin functions are added here.  */

void
gfc_build_intrinsic_lib_fndecls (void)
{
  gfc_intrinsic_map_t *m;

  /* Add GCC builtin functions.  */
  for (m = gfc_intrinsic_map; m->id != GFC_ISYM_NONE; m++)
    {
      if (m->code4 != END_BUILTINS)
        m->real4_decl = built_in_decls[m->code4];
      if (m->code8 != END_BUILTINS)
	m->real8_decl = built_in_decls[m->code8];
    }
}


/* Create a fndecl for a simple intrinsic library function.  */

static tree
gfc_get_intrinsic_lib_fndecl (gfc_intrinsic_map_t * m, gfc_expr * expr)
{
  tree type;
  tree argtypes;
  tree fndecl;
  gfc_actual_arglist *actual;
  tree *pdecl;
  gfc_typespec *ts;
  char name[GFC_MAX_SYMBOL_LEN + 3];

  ts = &expr->ts;
  name[0] = 0;
  if (ts->type == BT_REAL)
    {
      switch (ts->kind)
	{
	case 4:
	  pdecl = &m->real4_decl;
	  break;
	case 8:
	  pdecl = &m->real8_decl;
	  break;
	default:
	  abort ();
	}
    }
  else if (ts->type == BT_COMPLEX)
    {
      name[0] = 'c';
      name[1] = 0;
      switch (ts->kind)
	{
	case 4:
	  pdecl = &m->complex4_decl;
	  break;
	case 8:
	  pdecl = &m->complex8_decl;
	  break;
	default:
	  abort ();
	}
    }
  else
    abort ();

  if (*pdecl)
    return *pdecl;

  type = gfc_typenode_for_spec (ts);
  argtypes = NULL_TREE;

  for (actual = expr->value.function.actual; actual; actual = actual->next)
    {
      if (!gfc_compare_types (&actual->expr->ts, ts))
	{
	  internal_error ("arg types for intrinsic %s do not match",
			  expr->value.function.name);
	}
      argtypes = gfc_chainon_list (argtypes, type);
    }

  strcat (name, m->name);
  if (ts->kind == 4)
    strcat (name, "f");
  else
    assert (ts->kind == 8);

  argtypes = gfc_chainon_list (argtypes, void_type_node);
  type = build_function_type (type, argtypes);
  fndecl = build_decl (FUNCTION_DECL, get_identifier (name), type);

  /* Mark the decl as external.  */
  DECL_EXTERNAL (fndecl) = 1;
  TREE_PUBLIC (fndecl) = 1;

  rest_of_decl_compilation (fndecl, NULL, 1, 0);

  (*pdecl) = fndecl;
  return fndecl;
}


/* Convert an intrinsic function into an external or builtin call.  */

static void
gfc_conv_intrinsic_lib_function (gfc_se * se, gfc_expr * expr)
{
  gfc_intrinsic_map_t *m;
  tree args;
  tree fndecl;
  gfc_generic_isym_id id;

  id = expr->value.function.isym->generic_id;
  /* Find the entry for this function.  */
  for (m = gfc_intrinsic_map; m->id != GFC_ISYM_NONE; m++)
    {
      if (id == m->id)
	break;
    }

  if (m->id == GFC_ISYM_NONE)
    {
      internal_error ("Intrinsic function %s(%d) not recognized",
		      expr->value.function.name, id);
    }

  /* Get the decl and generate the call.  */
  args = gfc_conv_intrinsic_function_args (se, expr);
  fndecl = gfc_get_intrinsic_lib_fndecl (m, expr);
  se->expr = gfc_build_function_call (fndecl, args);
}


/* Evaluate a single upper or lower bound.  */
/* TODO: bound intrinsic generates way too much unneccessary code.  */

static void
gfc_conv_intrinsic_bound (gfc_se * se, gfc_expr * expr, int upper)
{
  gfc_actual_arglist *arg;
  gfc_actual_arglist *arg2;
  tree desc;
  tree type;
  tree bound;
  tree tmp;
  tree cond;
  gfc_se argse;
  gfc_ss *ss;
  int i;

  gfc_init_se (&argse, NULL);
  arg = expr->value.function.actual;
  arg2 = arg->next;

  if (se->ss)
    {
      /* Create an implicit second parameter from the loop variable.  */
      assert (!arg2->expr);
      assert (se->loop->dimen == 1);
      assert (se->ss->expr == expr);
      gfc_advance_se_ss_chain (se);
      bound = se->loop->loopvar[0];
      bound = fold (build (MINUS_EXPR, gfc_array_index_type, bound,
                           se->loop->from[0]));
    }
  else
    {
      /* use the passed argument.  */
      assert (arg->next->expr);
      gfc_init_se (&argse, NULL);
      gfc_conv_expr_type (&argse, arg->next->expr, gfc_array_index_type);
      gfc_add_block_to_block (&se->pre, &argse.pre);
      bound = argse.expr;
      /* Convert from one based to zero based.  */
      bound = fold (build (MINUS_EXPR, gfc_array_index_type, bound,
                    integer_one_node));
    }

  /* TODO: don't re-evaluate the descriptor on each iteration.  */
  /* Get a descriptor for the first parameter.  */
  ss = gfc_walk_expr (arg->expr);
  assert (ss != gfc_ss_terminator);
  argse.want_pointer = 0;
  gfc_conv_expr_descriptor (&argse, arg->expr, ss);
  gfc_add_block_to_block (&se->pre, &argse.pre);
  gfc_add_block_to_block (&se->post, &argse.post);

  desc = argse.expr;

  if (INTEGER_CST_P (bound))
    {
      assert (TREE_INT_CST_HIGH (bound) == 0);
      i = TREE_INT_CST_LOW (bound);
      assert (i >= 0 && i < GFC_TYPE_ARRAY_RANK (TREE_TYPE (desc)));
    }
  else
    {
      if (flag_bounds_check)
        {
          bound = gfc_evaluate_now (bound, &se->pre);
          cond = fold (build (LT_EXPR, boolean_type_node, bound,
                              integer_zero_node));
          tmp = gfc_rank_cst[GFC_TYPE_ARRAY_RANK (TREE_TYPE (desc))];
          tmp = fold (build (GE_EXPR, boolean_type_node, bound, tmp));
          cond = fold(build (TRUTH_ORIF_EXPR, boolean_type_node, cond, tmp));
          gfc_trans_runtime_check (cond, gfc_strconst_fault, &se->pre);
        }
    }

  if (upper)
    se->expr = gfc_conv_descriptor_ubound(desc, bound);
  else
    se->expr = gfc_conv_descriptor_lbound(desc, bound);

  type = gfc_typenode_for_spec (&expr->ts);
  se->expr = convert (type, se->expr);
}


static void
gfc_conv_intrinsic_abs (gfc_se * se, gfc_expr * expr)
{
  tree args;
  tree val;
  tree fndecl;

  args = gfc_conv_intrinsic_function_args (se, expr);
  assert (args && TREE_CHAIN (args) == NULL_TREE);
  val = TREE_VALUE (args);

  switch (expr->value.function.actual->expr->ts.type)
    {
    case BT_INTEGER:
    case BT_REAL:
      se->expr = build1 (ABS_EXPR, TREE_TYPE (val), val);
      break;

    case BT_COMPLEX:
      switch (expr->ts.kind)
	{
	case 4:
	  fndecl = gfor_fndecl_math_cabsf;
	  break;
	case 8:
	  fndecl = gfor_fndecl_math_cabs;
	  break;
	default:
	  abort ();
	}
      se->expr = gfc_build_function_call (fndecl, args);
      break;

    default:
      abort ();
    }
}


/* Create a complex value from one or two real components.  */

static void
gfc_conv_intrinsic_cmplx (gfc_se * se, gfc_expr * expr, int both)
{
  tree arg;
  tree real;
  tree imag;
  tree type;

  type = gfc_typenode_for_spec (&expr->ts);
  arg = gfc_conv_intrinsic_function_args (se, expr);
  real = convert (TREE_TYPE (type), TREE_VALUE (arg));
  arg = TREE_CHAIN (arg);
  if (both)
    imag = convert (TREE_TYPE (type), TREE_VALUE (arg));
  else
    imag = build_real_from_int_cst (TREE_TYPE (type), integer_zero_node);

  se->expr = fold (build (COMPLEX_EXPR, type, real, imag));
}

/* Remainder function MOD(A, P) = A - INT(A / P) * P.
   MODULO(A, P) = (A==0 .or. !(A>0 .xor. P>0))? MOD(A,P):MOD(A,P)+P.  */
/* TODO: MOD(x, 0)  */

static void
gfc_conv_intrinsic_mod (gfc_se * se, gfc_expr * expr, int modulo)
{
  tree arg;
  tree arg2;
  tree type;
  tree itype;
  tree tmp;
  tree zero;
  tree test;
  tree test2;
  mpf_t huge;
  int n;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  switch (expr->ts.type)
    {
    case BT_INTEGER:
      /* Integer case is easy, we've got a builtin op.  */
      se->expr = build (TRUNC_MOD_EXPR, type, arg, arg2);
      break;

    case BT_REAL:
      /* Real values we have to do the hard way.  */
      arg = gfc_evaluate_now (arg, &se->pre);
      arg2 = gfc_evaluate_now (arg2, &se->pre);

      tmp = build (RDIV_EXPR, type, arg, arg2);
      /* Test if the value is too large to handle sensibly.  */
      mpf_init (huge);
      n = gfc_validate_kind (BT_INTEGER, expr->ts.kind);
      mpf_set_z (huge, gfc_integer_kinds[n].huge);
      test = gfc_conv_mpf_to_tree (huge, expr->ts.kind);
      test2 = build (LT_EXPR, boolean_type_node, tmp, test);

      mpf_neg (huge, huge);
      test = gfc_conv_mpf_to_tree (huge, expr->ts.kind);
      test = build (GT_EXPR, boolean_type_node, tmp, test);
      test2 = build (TRUTH_AND_EXPR, boolean_type_node, test, test2);

      itype = gfc_get_int_type (expr->ts.kind);
      tmp = build_fix_expr (&se->pre, tmp, itype, FIX_TRUNC_EXPR);
      tmp = convert (type, tmp);
      tmp = build (COND_EXPR, type, test2, tmp, arg);
      tmp = build (MULT_EXPR, type, tmp, arg2);
      se->expr = build (MINUS_EXPR, type, arg, tmp);
      break;

    default:
      abort ();
    }

  if (modulo)
    {
     zero = gfc_build_const (type, integer_zero_node);
     /* Build !(A > 0 .xor. P > 0).  */
     test = build (GT_EXPR, boolean_type_node, arg, zero);
     test2 = build (GT_EXPR, boolean_type_node, arg2, zero);
     test = build (TRUTH_XOR_EXPR, boolean_type_node, test, test2);
     test = build1 (TRUTH_NOT_EXPR, boolean_type_node, test);
     /* Build (A == 0) .or. !(A > 0 .xor. P > 0).  */
     test2 = build (EQ_EXPR, boolean_type_node, arg, zero);
     test = build (TRUTH_OR_EXPR, boolean_type_node, test, test2);

     se->expr = build (COND_EXPR, type, test, se->expr, 
               build (PLUS_EXPR, type, se->expr, arg2));
    }
}

/* Positive difference DIM (x, y) = ((x - y) < 0) ? 0 : x - y.  */

static void
gfc_conv_intrinsic_dim (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree val;
  tree tmp;
  tree type;
  tree zero;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  val = build (MINUS_EXPR, type, arg, arg2);
  val = gfc_evaluate_now (val, &se->pre);

  zero = gfc_build_const (type, integer_zero_node);
  tmp = build (LE_EXPR, boolean_type_node, val, zero);
  se->expr = build (COND_EXPR, type, tmp, zero, val);
}


/* SIGN(A, B) is absolute value of A times sign of B.
   The real value versions use library functions to ensure the correct
   handling of negative zero.  Integer case implemented as:
   SIGN(A, B) = ((a >= 0) .xor. (b >= 0)) ? a : -a
  */

static void
gfc_conv_intrinsic_sign (gfc_se * se, gfc_expr * expr)
{
  tree tmp;
  tree arg;
  tree arg2;
  tree type;
  tree zero;
  tree testa;
  tree testb;


  arg = gfc_conv_intrinsic_function_args (se, expr);
  if (expr->ts.type == BT_REAL)
    {
      switch (expr->ts.kind)
	{
	case 4:
	  tmp = gfor_fndecl_math_sign4;
	  break;
	case 8:
	  tmp = gfor_fndecl_math_sign8;
	  break;
	default:
	  abort ();
	}
      se->expr = gfc_build_function_call (tmp, arg);
      return;
    }

  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);
  zero = gfc_build_const (type, integer_zero_node);

  testa = fold (build (GE_EXPR, boolean_type_node, arg, zero));
  testb = fold (build (GE_EXPR, boolean_type_node, arg2, zero));
  tmp = fold (build (TRUTH_XOR_EXPR, boolean_type_node, testa, testb));
  se->expr = fold (build (COND_EXPR, type, tmp,
			  build1 (NEGATE_EXPR, type, arg), arg));
}


/* Test for the presence of an optional argument.  */

static void
gfc_conv_intrinsic_present (gfc_se * se, gfc_expr * expr)
{
  gfc_expr *arg;

  arg = expr->value.function.actual->expr;
  assert (arg->expr_type == EXPR_VARIABLE);
  se->expr = gfc_conv_expr_present (arg->symtree->n.sym);
  se->expr = convert (gfc_typenode_for_spec (&expr->ts), se->expr);
}


/* Calculate the double precision product of two single precision values.  */

static void
gfc_conv_intrinsic_dprod (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);

  /* Convert the args to double precision before multiplying.  */
  type = gfc_typenode_for_spec (&expr->ts);
  arg = convert (type, arg);
  arg2 = convert (type, arg2);
  se->expr = build (MULT_EXPR, type, arg, arg2);
}


/* Return a length one character string containing an ascii character.  */

static void
gfc_conv_intrinsic_char (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree var;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);

  /* We currently don't support character types != 1.  */
  assert (expr->ts.kind == 1);
  type = gfc_get_character_type (expr->ts.kind, expr->ts.cl);
  var = gfc_create_var (type, "char");
  TREE_ADDRESSABLE (var) = 1;

  arg = convert (gfc_character1_type_node, arg);
  var = build (ARRAY_REF, gfc_character1_type_node, var, integer_one_node);
  gfc_add_modify_expr (&se->pre, var, arg);
  se->expr = build1 (ADDR_EXPR, build_pointer_type (type), var);
  se->string_length = integer_one_node;
}


/* Get the minimum/maximum value of all the parameters.
    minmax (a1, a2, a3, ...)
    {
      if (a2 .op. a1)
        mvar = a2;
      else
        mvar = a1;
      if (a3 .op. mvar)
        mvar = a3;
      ...
      return mvar
    }
 */

static void
gfc_conv_intrinsic_minmax (gfc_se * se, gfc_expr * expr, int op)
{
  tree limit;
  tree tmp;
  tree mvar;
  tree val;
  tree thencase;
  tree elsecase;
  tree arg;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  type = gfc_typenode_for_spec (&expr->ts);

  limit = TREE_VALUE (arg);
  /* Only evaluate the argument once.  */
  if (TREE_CODE (limit) != VAR_DECL || !TREE_CONSTANT (limit))
    limit = gfc_evaluate_now(limit, &se->pre);

  mvar = gfc_create_var (type, "M");
  elsecase = build_v (MODIFY_EXPR, mvar, limit);
  for (arg = TREE_CHAIN (arg); arg != NULL_TREE; arg = TREE_CHAIN (arg))
    {
      val = TREE_VALUE (arg);
      /* Only evaluate the argument once.  */
      if (TREE_CODE (val) != VAR_DECL || !TREE_CONSTANT (val))
        val = gfc_evaluate_now(val, &se->pre);

      thencase = build_v (MODIFY_EXPR, mvar, val);

      tmp = build (op, boolean_type_node, val, limit);
      tmp = build_v (COND_EXPR, tmp, thencase, elsecase);
      gfc_add_expr_to_block (&se->pre, tmp);
      elsecase = build_empty_stmt ();
      limit = mvar;
    }
  se->expr = mvar;
}


/* Create a symbol node for this intrinsic.  The symbol form the frontend
   is for the generic name.  */

static gfc_symbol *
gfc_get_symbol_for_expr (gfc_expr * expr)
{
  gfc_symbol *sym;

  /* TODO: Add symbols for intrinsic function to the global namespace.  */
  assert (strlen (expr->value.function.name) <= GFC_MAX_SYMBOL_LEN - 5);
  sym = gfc_new_symbol (expr->value.function.name, NULL);

  sym->ts = expr->ts;
  sym->attr.external = 1;
  sym->attr.function = 1;
  sym->attr.always_explicit = 1;
  sym->attr.proc = PROC_INTRINSIC;
  sym->attr.flavor = FL_PROCEDURE;
  sym->result = sym;
  if (expr->rank > 0)
    {
      sym->attr.dimension = 1;
      sym->as = gfc_get_array_spec ();
      sym->as->type = AS_ASSUMED_SHAPE;
      sym->as->rank = expr->rank;
    }

  /* TODO: proper argument lists for external intrinsics.  */
  return sym;
}

/* Generate a call to an external intrinsic function.  */
static void
gfc_conv_intrinsic_funcall (gfc_se * se, gfc_expr * expr)
{
  gfc_symbol *sym;

  assert (!se->ss || se->ss->expr == expr);

  if (se->ss)
    assert (expr->rank > 0);
  else
    assert (expr->rank == 0);

  sym = gfc_get_symbol_for_expr (expr);
  gfc_conv_function_call (se, sym, expr->value.function.actual);
  gfc_free (sym);
}

/* ANY and ALL intrinsics. ANY->op == NE_EXPR, ALL->op == EQ_EXPR.
   Implemented as
    any(a)
    {
      forall (i=...)
        if (a[i] != 0)
          return 1
      end forall
      return 0
    }
    all(a)
    {
      forall (i=...)
        if (a[i] == 0)
          return 0
      end forall
      return 1
    }
 */
static void
gfc_conv_intrinsic_anyall (gfc_se * se, gfc_expr * expr, int op)
{
  tree resvar;
  stmtblock_t block;
  stmtblock_t body;
  tree type;
  tree tmp;
  tree found;
  gfc_loopinfo loop;
  gfc_actual_arglist *actual;
  gfc_ss *arrayss;
  gfc_se arrayse;
  tree exit_label;

  if (se->ss)
    {
      gfc_conv_intrinsic_funcall (se, expr);
      return;
    }

  actual = expr->value.function.actual;
  type = gfc_typenode_for_spec (&expr->ts);
  /* Initialize the result.  */
  resvar = gfc_create_var (type, "test");
  if (op == EQ_EXPR)
    tmp = convert (type, boolean_true_node);
  else
    tmp = convert (type, boolean_false_node);
  gfc_add_modify_expr (&se->pre, resvar, tmp);

  /* Walk the arguments.  */
  arrayss = gfc_walk_expr (actual->expr);
  assert (arrayss != gfc_ss_terminator);

  /* Initialize the scalarizer.  */
  gfc_init_loopinfo (&loop);
  exit_label = gfc_build_label_decl (NULL_TREE);
  TREE_USED (exit_label) = 1;
  gfc_add_ss_to_loop (&loop, arrayss);

  /* Initialize the loop.  */
  gfc_conv_ss_startstride (&loop);
  gfc_conv_loop_setup (&loop);

  gfc_mark_ss_chain_used (arrayss, 1);
  /* Generate the loop body.  */
  gfc_start_scalarized_body (&loop, &body);

  /* If the condition matches then set the return value.  */
  gfc_start_block (&block);
  if (op == EQ_EXPR)
    tmp = convert (type, boolean_false_node);
  else
    tmp = convert (type, boolean_true_node);
  gfc_add_modify_expr (&block, resvar, tmp);

  /* And break out of the loop.  */
  tmp = build1_v (GOTO_EXPR, exit_label);
  gfc_add_expr_to_block (&block, tmp);

  found = gfc_finish_block (&block);

  /* Check this element.  */
  gfc_init_se (&arrayse, NULL);
  gfc_copy_loopinfo_to_se (&arrayse, &loop);
  arrayse.ss = arrayss;
  gfc_conv_expr_val (&arrayse, actual->expr);

  gfc_add_block_to_block (&body, &arrayse.pre);
  tmp = build (op, boolean_type_node, arrayse.expr, integer_zero_node);
  tmp = build_v (COND_EXPR, tmp, found, build_empty_stmt ());
  gfc_add_expr_to_block (&body, tmp);
  gfc_add_block_to_block (&body, &arrayse.post);

  gfc_trans_scalarizing_loops (&loop, &body);

  /* Add the exit label.  */
  tmp = build1_v (LABEL_EXPR, exit_label);
  gfc_add_expr_to_block (&loop.pre, tmp);

  gfc_add_block_to_block (&se->pre, &loop.pre);
  gfc_add_block_to_block (&se->pre, &loop.post);
  gfc_cleanup_loop (&loop);

  se->expr = resvar;
}

/* COUNT(A) = Number of true elements in A.  */
static void
gfc_conv_intrinsic_count (gfc_se * se, gfc_expr * expr)
{
  tree resvar;
  tree type;
  stmtblock_t body;
  tree tmp;
  gfc_loopinfo loop;
  gfc_actual_arglist *actual;
  gfc_ss *arrayss;
  gfc_se arrayse;

  if (se->ss)
    {
      gfc_conv_intrinsic_funcall (se, expr);
      return;
    }

  actual = expr->value.function.actual;

  type = gfc_typenode_for_spec (&expr->ts);
  /* Initialize the result.  */
  resvar = gfc_create_var (type, "count");
  gfc_add_modify_expr (&se->pre, resvar, integer_zero_node);

  /* Walk the arguments.  */
  arrayss = gfc_walk_expr (actual->expr);
  assert (arrayss != gfc_ss_terminator);

  /* Initialize the scalarizer.  */
  gfc_init_loopinfo (&loop);
  gfc_add_ss_to_loop (&loop, arrayss);

  /* Initialize the loop.  */
  gfc_conv_ss_startstride (&loop);
  gfc_conv_loop_setup (&loop);

  gfc_mark_ss_chain_used (arrayss, 1);
  /* Generate the loop body.  */
  gfc_start_scalarized_body (&loop, &body);

  tmp = build (PLUS_EXPR, TREE_TYPE (resvar), resvar, integer_one_node);
  tmp = build_v (MODIFY_EXPR, resvar, tmp);

  gfc_init_se (&arrayse, NULL);
  gfc_copy_loopinfo_to_se (&arrayse, &loop);
  arrayse.ss = arrayss;
  gfc_conv_expr_val (&arrayse, actual->expr);
  tmp = build_v (COND_EXPR, arrayse.expr, tmp, build_empty_stmt ());

  gfc_add_block_to_block (&body, &arrayse.pre);
  gfc_add_expr_to_block (&body, tmp);
  gfc_add_block_to_block (&body, &arrayse.post);

  gfc_trans_scalarizing_loops (&loop, &body);

  gfc_add_block_to_block (&se->pre, &loop.pre);
  gfc_add_block_to_block (&se->pre, &loop.post);
  gfc_cleanup_loop (&loop);

  se->expr = resvar;
}

/* Inline implementation of the sum and product intrinsics.  */
static void
gfc_conv_intrinsic_arith (gfc_se * se, gfc_expr * expr, int op)
{
  tree resvar;
  tree type;
  stmtblock_t body;
  stmtblock_t block;
  tree tmp;
  gfc_loopinfo loop;
  gfc_actual_arglist *actual;
  gfc_ss *arrayss;
  gfc_ss *maskss;
  gfc_se arrayse;
  gfc_se maskse;
  gfc_expr *arrayexpr;
  gfc_expr *maskexpr;

  if (se->ss)
    {
      gfc_conv_intrinsic_funcall (se, expr);
      return;
    }

  type = gfc_typenode_for_spec (&expr->ts);
  /* Initialize the result.  */
  resvar = gfc_create_var (type, "val");
  if (op == PLUS_EXPR)
    tmp = gfc_build_const (type, integer_zero_node);
  else
    tmp = gfc_build_const (type, integer_one_node);

  gfc_add_modify_expr (&se->pre, resvar, tmp);

  /* Walk the arguments.  */
  actual = expr->value.function.actual;
  arrayexpr = actual->expr;
  arrayss = gfc_walk_expr (arrayexpr);
  assert (arrayss != gfc_ss_terminator);

  actual = actual->next->next;
  assert (actual);
  maskexpr = actual->expr;
  if (maskexpr)
    {
      maskss = gfc_walk_expr (maskexpr);
      assert (maskss != gfc_ss_terminator);
    }
  else
    maskss = NULL;

  /* Initialize the scalarizer.  */
  gfc_init_loopinfo (&loop);
  gfc_add_ss_to_loop (&loop, arrayss);
  if (maskss)
    gfc_add_ss_to_loop (&loop, maskss);

  /* Initialize the loop.  */
  gfc_conv_ss_startstride (&loop);
  gfc_conv_loop_setup (&loop);

  gfc_mark_ss_chain_used (arrayss, 1);
  if (maskss)
    gfc_mark_ss_chain_used (maskss, 1);
  /* Generate the loop body.  */
  gfc_start_scalarized_body (&loop, &body);

  /* If we have a mask, only add this element if the mask is set.  */
  if (maskss)
    {
      gfc_init_se (&maskse, NULL);
      gfc_copy_loopinfo_to_se (&maskse, &loop);
      maskse.ss = maskss;
      gfc_conv_expr_val (&maskse, maskexpr);
      gfc_add_block_to_block (&body, &maskse.pre);

      gfc_start_block (&block);
    }
  else
    gfc_init_block (&block);

  /* Do the actual summation/product.  */
  gfc_init_se (&arrayse, NULL);
  gfc_copy_loopinfo_to_se (&arrayse, &loop);
  arrayse.ss = arrayss;
  gfc_conv_expr_val (&arrayse, arrayexpr);
  gfc_add_block_to_block (&block, &arrayse.pre);

  tmp = build (op, type, resvar, arrayse.expr);
  gfc_add_modify_expr (&block, resvar, tmp);
  gfc_add_block_to_block (&block, &arrayse.post);

  if (maskss)
    {
      /* We enclose the above in if (mask) {...} .  */
      tmp = gfc_finish_block (&block);

      tmp = build_v (COND_EXPR, maskse.expr, tmp, build_empty_stmt ());
    }
  else
    tmp = gfc_finish_block (&block);
  gfc_add_expr_to_block (&body, tmp);

  gfc_trans_scalarizing_loops (&loop, &body);
  gfc_add_block_to_block (&se->pre, &loop.pre);
  gfc_add_block_to_block (&se->pre, &loop.post);
  gfc_cleanup_loop (&loop);

  se->expr = resvar;
}

static void
gfc_conv_intrinsic_minmaxloc (gfc_se * se, gfc_expr * expr, int op)
{
  stmtblock_t body;
  stmtblock_t block;
  stmtblock_t ifblock;
  tree limit;
  tree type;
  tree tmp;
  tree ifbody;
  gfc_loopinfo loop;
  gfc_actual_arglist *actual;
  gfc_ss *arrayss;
  gfc_ss *maskss;
  gfc_se arrayse;
  gfc_se maskse;
  gfc_expr *arrayexpr;
  gfc_expr *maskexpr;
  tree pos;
  int n;

  if (se->ss)
    {
      gfc_conv_intrinsic_funcall (se, expr);
      return;
    }

  type = gfc_typenode_for_spec (&expr->ts);
  /* Initialize the result.  */
  limit = gfc_create_var (type, "limit");
  pos = gfc_create_var (gfc_array_index_type, "pos");
  n = gfc_validate_kind (expr->ts.type, expr->ts.kind);
  switch (expr->ts.type)
    {
    case BT_REAL:
      tmp = gfc_conv_mpf_to_tree (gfc_real_kinds[n].huge, expr->ts.kind);
      break;

    case BT_INTEGER:
      tmp = gfc_conv_mpz_to_tree (gfc_integer_kinds[n].huge, expr->ts.kind);
      break;

    default:
      abort ();
    }

  /* Most negative(+HUGE) for maxval, most negative (-HUGE) for minval.  */
  if (op == GT_EXPR)
    tmp = fold (build1 (NEGATE_EXPR, TREE_TYPE (tmp), tmp));
  gfc_add_modify_expr (&se->pre, limit, tmp);

  /* Walk the arguments.  */
  actual = expr->value.function.actual;
  arrayexpr = actual->expr;
  arrayss = gfc_walk_expr (arrayexpr);
  assert (arrayss != gfc_ss_terminator);

  actual = actual->next->next;
  assert (actual);
  maskexpr = actual->expr;
  if (maskexpr)
    {
      maskss = gfc_walk_expr (maskexpr);
      assert (maskss != gfc_ss_terminator);
    }
  else
    maskss = NULL;

  /* Initialize the scalarizer.  */
  gfc_init_loopinfo (&loop);
  gfc_add_ss_to_loop (&loop, arrayss);
  if (maskss)
    gfc_add_ss_to_loop (&loop, maskss);

  /* Initialize the loop.  */
  gfc_conv_ss_startstride (&loop);
  gfc_conv_loop_setup (&loop);

  assert (loop.dimen == 1);

  gfc_mark_ss_chain_used (arrayss, 1);
  if (maskss)
    gfc_mark_ss_chain_used (maskss, 1);
  /* Generate the loop body.  */
  gfc_start_scalarized_body (&loop, &body);

  /* If we have a mask, only check this element if the mask is set.  */
  if (maskss)
    {
      gfc_init_se (&maskse, NULL);
      gfc_copy_loopinfo_to_se (&maskse, &loop);
      maskse.ss = maskss;
      gfc_conv_expr_val (&maskse, maskexpr);
      gfc_add_block_to_block (&body, &maskse.pre);

      gfc_start_block (&block);
    }
  else
    gfc_init_block (&block);

  /* Compare with the current limit.  */
  gfc_init_se (&arrayse, NULL);
  gfc_copy_loopinfo_to_se (&arrayse, &loop);
  arrayse.ss = arrayss;
  gfc_conv_expr_val (&arrayse, arrayexpr);
  gfc_add_block_to_block (&block, &arrayse.pre);

  /* We do the following if this is a more extreme value.  */
  gfc_start_block (&ifblock);

  /* Assign the value to the limit...  */
  gfc_add_modify_expr (&ifblock, limit, arrayse.expr);

  /* Remember where we are.  */
  gfc_add_modify_expr (&ifblock, pos, loop.loopvar[0]);

  ifbody = gfc_finish_block (&ifblock);

  /* If it is a more extreme value.  */
  tmp = build (op, boolean_type_node, arrayse.expr, limit);
  tmp = build_v (COND_EXPR, tmp, ifbody, build_empty_stmt ());
  gfc_add_expr_to_block (&block, tmp);

  if (maskss)
    {
      /* We enclose the above in if (mask) {...}.  */
      tmp = gfc_finish_block (&block);

      tmp = build_v (COND_EXPR, maskse.expr, tmp, build_empty_stmt ());
    }
  else
    tmp = gfc_finish_block (&block);
  gfc_add_expr_to_block (&body, tmp);

  gfc_trans_scalarizing_loops (&loop, &body);

  gfc_add_block_to_block (&se->pre, &loop.pre);
  gfc_add_block_to_block (&se->pre, &loop.post);
  gfc_cleanup_loop (&loop);

  /* Return a value in the range 1..SIZE(array).  */
  tmp = fold (build (MINUS_EXPR, gfc_array_index_type, loop.from[0],
		     integer_one_node));
  tmp = fold (build (MINUS_EXPR, gfc_array_index_type, pos, tmp));
  /* And convert to the required type.  */
  se->expr = convert (type, pos);
}

static void
gfc_conv_intrinsic_minmaxval (gfc_se * se, gfc_expr * expr, int op)
{
  tree limit;
  tree type;
  tree tmp;
  tree ifbody;
  stmtblock_t body;
  stmtblock_t block;
  gfc_loopinfo loop;
  gfc_actual_arglist *actual;
  gfc_ss *arrayss;
  gfc_ss *maskss;
  gfc_se arrayse;
  gfc_se maskse;
  gfc_expr *arrayexpr;
  gfc_expr *maskexpr;
  int n;

  if (se->ss)
    {
      gfc_conv_intrinsic_funcall (se, expr);
      return;
    }

  type = gfc_typenode_for_spec (&expr->ts);
  /* Initialize the result.  */
  limit = gfc_create_var (type, "limit");
  n = gfc_validate_kind (expr->ts.type, expr->ts.kind);
  switch (expr->ts.type)
    {
    case BT_REAL:
      tmp = gfc_conv_mpf_to_tree (gfc_real_kinds[n].huge, expr->ts.kind);
      break;

    case BT_INTEGER:
      tmp = gfc_conv_mpz_to_tree (gfc_integer_kinds[n].huge, expr->ts.kind);
      break;

    default:
      abort ();
    }

  /* Most negative(-HUGE) for maxval, most positive (-HUGE) for minval.  */
  if (op == GT_EXPR)
    tmp = fold (build1 (NEGATE_EXPR, TREE_TYPE (tmp), tmp));
  gfc_add_modify_expr (&se->pre, limit, tmp);

  /* Walk the arguments.  */
  actual = expr->value.function.actual;
  arrayexpr = actual->expr;
  arrayss = gfc_walk_expr (arrayexpr);
  assert (arrayss != gfc_ss_terminator);

  actual = actual->next->next;
  assert (actual);
  maskexpr = actual->expr;
  if (maskexpr)
    {
      maskss = gfc_walk_expr (maskexpr);
      assert (maskss != gfc_ss_terminator);
    }
  else
    maskss = NULL;

  /* Initialize the scalarizer.  */
  gfc_init_loopinfo (&loop);
  gfc_add_ss_to_loop (&loop, arrayss);
  if (maskss)
    gfc_add_ss_to_loop (&loop, maskss);

  /* Initialize the loop.  */
  gfc_conv_ss_startstride (&loop);
  gfc_conv_loop_setup (&loop);

  gfc_mark_ss_chain_used (arrayss, 1);
  if (maskss)
    gfc_mark_ss_chain_used (maskss, 1);
  /* Generate the loop body.  */
  gfc_start_scalarized_body (&loop, &body);

  /* If we have a mask, only add this element if the mask is set.  */
  if (maskss)
    {
      gfc_init_se (&maskse, NULL);
      gfc_copy_loopinfo_to_se (&maskse, &loop);
      maskse.ss = maskss;
      gfc_conv_expr_val (&maskse, maskexpr);
      gfc_add_block_to_block (&body, &maskse.pre);

      gfc_start_block (&block);
    }
  else
    gfc_init_block (&block);

  /* Compare with the current limit.  */
  gfc_init_se (&arrayse, NULL);
  gfc_copy_loopinfo_to_se (&arrayse, &loop);
  arrayse.ss = arrayss;
  gfc_conv_expr_val (&arrayse, arrayexpr);
  gfc_add_block_to_block (&block, &arrayse.pre);

  /* Assign the value to the limit...  */
  ifbody = build_v (MODIFY_EXPR, limit, arrayse.expr);

  /* If it is a more extreme value.  */
  tmp = build (op, boolean_type_node, arrayse.expr, limit);
  tmp = build_v (COND_EXPR, tmp, ifbody, build_empty_stmt ());
  gfc_add_expr_to_block (&block, tmp);
  gfc_add_block_to_block (&block, &arrayse.post);

  tmp = gfc_finish_block (&block);
  if (maskss)
    {
      /* We enclose the above in if (mask) {...}.  */
      tmp = build (COND_EXPR, maskse.expr, tmp, build_empty_stmt ());
    }
  gfc_add_expr_to_block (&body, tmp);

  gfc_trans_scalarizing_loops (&loop, &body);

  gfc_add_block_to_block (&se->pre, &loop.pre);
  gfc_add_block_to_block (&se->pre, &loop.post);
  gfc_cleanup_loop (&loop);

  se->expr = limit;
}

/* BTEST (i, pos) = (i & (1 << pos)) != 0.  */
static void
gfc_conv_intrinsic_btest (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree type;
  tree tmp;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  tmp = build (LSHIFT_EXPR, type, integer_one_node, arg2);
  tmp = build (BIT_AND_EXPR, type, arg, tmp);
  tmp = fold (build (NE_EXPR, boolean_type_node, tmp, integer_zero_node));
  type = gfc_typenode_for_spec (&expr->ts);
  se->expr = convert (type, tmp);
}

/* Generate code to perform the specified operation.  */
static void
gfc_conv_intrinsic_bitop (gfc_se * se, gfc_expr * expr, int op)
{
  tree arg;
  tree arg2;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  se->expr = fold (build (op, type, arg, arg2));
}

/* Bitwise not.  */
static void
gfc_conv_intrinsic_not (gfc_se * se, gfc_expr * expr)
{
  tree arg;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (arg);

  se->expr = build1 (BIT_NOT_EXPR, TREE_TYPE (arg), arg);
}

/* Set or clear a single bit.  */
static void
gfc_conv_intrinsic_singlebitop (gfc_se * se, gfc_expr * expr, int set)
{
  tree arg;
  tree arg2;
  tree type;
  tree tmp;
  int op;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  tmp = fold (build (LSHIFT_EXPR, type, integer_one_node, arg2));
  if (set)
    op = BIT_IOR_EXPR;
  else
    {
      op = BIT_AND_EXPR;
      tmp = fold (build1 (BIT_NOT_EXPR, type, tmp));
    }
  se->expr = fold (build (op, type, arg, tmp));
}

/* Extract a sequence of bits.
    IBITS(I, POS, LEN) = (I >> POS) & ~((~0) << LEN).  */
static void
gfc_conv_intrinsic_ibits (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree arg3;
  tree type;
  tree tmp;
  tree mask;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_CHAIN (arg);
  arg3 = TREE_VALUE (TREE_CHAIN (arg2));
  arg = TREE_VALUE (arg);
  arg2 = TREE_VALUE (arg2);
  type = TREE_TYPE (arg);

  mask = build_int_2 (-1, ~(unsigned HOST_WIDE_INT) 0);
  mask = build (LSHIFT_EXPR, type, mask, arg3);
  mask = build1 (BIT_NOT_EXPR, type, mask);

  tmp = build (RSHIFT_EXPR, type, arg, arg2);

  se->expr = fold (build (BIT_AND_EXPR, type, tmp, mask));
}

/* ISHFT (I, SHIFT) = (shift >= 0) ? i << shift : i >> -shift.  */
static void
gfc_conv_intrinsic_ishft (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree type;
  tree tmp;
  tree lshift;
  tree rshift;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_VALUE (TREE_CHAIN (arg));
  arg = TREE_VALUE (arg);
  type = TREE_TYPE (arg);

  /* Left shift if positive.  */
  lshift = build (LSHIFT_EXPR, type, arg, arg2);

  /* Right shift if negative.  This will perform an arithmetic shift as
     we are dealing with signed integers.  Section 13.5.7 allows this.  */
  tmp = build1 (NEGATE_EXPR, TREE_TYPE (arg2), arg2);
  rshift = build (RSHIFT_EXPR, type, arg, tmp);

  tmp = build (GT_EXPR, boolean_type_node, arg2, integer_zero_node);
  rshift = build (COND_EXPR, type, tmp, lshift, rshift);

  /* Do nothing if shift == 0.  */
  tmp = build (EQ_EXPR, boolean_type_node, arg2, integer_zero_node);
  se->expr = build (COND_EXPR, type, tmp, arg, rshift);
}

/* Circular shift.  AKA rotate or barrel shift.  */
static void
gfc_conv_intrinsic_ishftc (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree arg2;
  tree arg3;
  tree type;
  tree tmp;
  tree lrot;
  tree rrot;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg2 = TREE_CHAIN (arg);
  arg3 = TREE_CHAIN (arg2);
  if (arg3)
    {
      /* Use a library function for the 3 parameter version.  */
      type = TREE_TYPE (TREE_VALUE (arg));
      /* Convert all args to the same type otherwise we need loads of library
         functions.  SIZE and SHIFT cannot have values > BIT_SIZE (I) so the
         conversion is safe.  */
      tmp = convert (type, TREE_VALUE (arg2));
      TREE_VALUE (arg2) = tmp;
      tmp = convert (type, TREE_VALUE (arg3));
      TREE_VALUE (arg3) = tmp;

      switch (expr->ts.kind)
	{
	case 4:
	  tmp = gfor_fndecl_math_ishftc4;
	  break;
	case 8:
	  tmp = gfor_fndecl_math_ishftc8;
	  break;
	default:
	  abort ();
	}
      se->expr = gfc_build_function_call (tmp, arg);
      return;
    }
  arg = TREE_VALUE (arg);
  arg2 = TREE_VALUE (arg2);
  type = TREE_TYPE (arg);

  /* Rotate left if positive.  */
  lrot = build (LROTATE_EXPR, type, arg, arg2);

  /* Rotate right if negative.  */
  tmp = build1 (NEGATE_EXPR, TREE_TYPE (arg2), arg2);
  rrot = build (RROTATE_EXPR, type, arg, tmp);

  tmp = build (GT_EXPR, boolean_type_node, arg2, integer_zero_node);
  rrot = build (COND_EXPR, type, tmp, lrot, rrot);

  /* Do nothing if shift == 0.  */
  tmp = build (EQ_EXPR, boolean_type_node, arg2, integer_zero_node);
  se->expr = build (COND_EXPR, type, tmp, arg, rrot);
}

/* The length of a character string.  */
static void
gfc_conv_intrinsic_len (gfc_se * se, gfc_expr * expr)
{
  tree len;
  tree type;
  tree decl;
  gfc_se argse;
  gfc_expr *arg;

  assert (!se->ss);

  arg = expr->value.function.actual->expr;

  type = gfc_typenode_for_spec (&expr->ts);
  switch (arg->expr_type)
    {
    case EXPR_VARIABLE:
      decl = gfc_get_symbol_decl (arg->symtree->n.sym);
      assert (GFC_DECL_STRING (decl));
      len = GFC_DECL_STRING_LENGTH (decl);
      assert (len);
      break;

    case EXPR_CONSTANT:
      len = build_int_2 (arg->value.character.length, 0);
      break;

    default:
      /* Anybody stupid enough to do this deserves inefficient code.  */
      gfc_init_se (&argse, se);
      gfc_conv_expr (&argse, expr->value.function.actual->expr);
      gfc_add_block_to_block (&se->pre, &argse.pre);
      gfc_add_block_to_block (&se->post, &argse.post);
      len = argse.string_length;
      break;
    }
  se->expr = convert (type, len);
}

/* The length of a character string not including trailing blanks.  */
static void
gfc_conv_intrinsic_len_trim (gfc_se * se, gfc_expr * expr)
{
  tree args;
  tree type;

  args = gfc_conv_intrinsic_function_args (se, expr);
  type = gfc_typenode_for_spec (&expr->ts);
  se->expr = gfc_build_function_call (gfor_fndecl_string_len_trim, args);
  se->expr = convert (type, se->expr);
}


/* Returns the starting position of a substring within a string.  */

static void
gfc_conv_intrinsic_index (gfc_se * se, gfc_expr * expr)
{
  tree args;
  tree back;
  tree type;
  tree tmp;

  args = gfc_conv_intrinsic_function_args (se, expr);
  type = gfc_typenode_for_spec (&expr->ts);
  tmp = gfc_advance_chain (args, 3);
  if (TREE_CHAIN (tmp) == NULL_TREE)
    {
      back = convert (gfc_logical4_type_node, integer_one_node);
      back = tree_cons (NULL_TREE, integer_zero_node, NULL_TREE);
      TREE_CHAIN (tmp) = back;
    }
  else
    {
      back = TREE_CHAIN (tmp);
      TREE_VALUE (back) = convert (gfc_logical4_type_node, TREE_VALUE (back));
    }

  se->expr = gfc_build_function_call (gfor_fndecl_string_index, args);
  se->expr = convert (type, se->expr);
}

/* The ascii value for a single character.  */
static void
gfc_conv_intrinsic_ichar (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  arg = TREE_VALUE (TREE_CHAIN (arg));
  assert (POINTER_TYPE_P (TREE_TYPE (arg)));
  arg = build1 (NOP_EXPR, pchar_type_node, arg);
  type = gfc_typenode_for_spec (&expr->ts);

  se->expr = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (arg)), arg);
  se->expr = convert (type, se->expr);
}


/* MERGE (tsource, fsource, mask) = mask ? tsource : fsource.  */

static void
gfc_conv_intrinsic_merge (gfc_se * se, gfc_expr * expr)
{
  tree arg;
  tree tsource;
  tree fsource;
  tree mask;
  tree type;

  arg = gfc_conv_intrinsic_function_args (se, expr);
  tsource = TREE_VALUE (arg);
  arg = TREE_CHAIN (arg);
  fsource = TREE_VALUE (arg);
  arg = TREE_CHAIN (arg);
  mask = TREE_VALUE (arg);

  type = TREE_TYPE (tsource);
  se->expr = fold (build (COND_EXPR, type, mask, tsource, fsource));
}


static void
gfc_conv_intrinsic_size (gfc_se * se, gfc_expr * expr)
{
  gfc_actual_arglist *actual;
  tree args;
  tree type;
  tree fndecl;
  gfc_se argse;
  gfc_ss *ss;

  gfc_init_se (&argse, NULL);
  actual = expr->value.function.actual;

  ss = gfc_walk_expr (actual->expr);
  assert (ss != gfc_ss_terminator);
  argse.want_pointer = 1;
  gfc_conv_expr_descriptor (&argse, actual->expr, ss);
  gfc_add_block_to_block (&se->pre, &argse.pre);
  gfc_add_block_to_block (&se->post, &argse.post);
  args = gfc_chainon_list (NULL_TREE, argse.expr);

  actual = actual->next;
  if (actual->expr)
    {
      gfc_init_se (&argse, NULL);
      gfc_conv_expr_type (&argse, actual->expr, gfc_array_index_type);
      gfc_add_block_to_block (&se->pre, &argse.pre);
      args = gfc_chainon_list (args, argse.expr);
      fndecl = gfor_fndecl_size1;
    }
  else
    fndecl = gfor_fndecl_size0;

  se->expr = gfc_build_function_call (fndecl, args);
  type = gfc_typenode_for_spec (&expr->ts);
  se->expr = convert (type, se->expr);
}


/* Intrinsic string comparison functions.  */

  static void
gfc_conv_intrinsic_strcmp (gfc_se * se, gfc_expr * expr, int op)
{
  tree type;
  tree args;

  args = gfc_conv_intrinsic_function_args (se, expr);
  /* Build a call for the comparison.  */
  se->expr = gfc_build_function_call (gfor_fndecl_compare_string, args);

  type = gfc_typenode_for_spec (&expr->ts);
  se->expr = build (op, type, se->expr, integer_zero_node);
}

/* Generate a call to the adjustl/adjustr library function.  */
static void
gfc_conv_intrinsic_adjust (gfc_se * se, gfc_expr * expr, tree fndecl)
{
  tree args;
  tree len;
  tree type;
  tree var;
  tree tmp;

  args = gfc_conv_intrinsic_function_args (se, expr);
  len = TREE_VALUE (args);

  type = TREE_TYPE (TREE_VALUE (TREE_CHAIN (args)));
  var = gfc_conv_string_tmp (se, type, len);
  args = tree_cons (NULL_TREE, var, args);

  tmp = gfc_build_function_call (fndecl, args);
  gfc_add_expr_to_block (&se->pre, tmp);
  se->expr = var;
  se->string_length = len;
}


/* Scalar transfer statement.
   TRANSFER (source, mold) = *(typeof<mould> *)&source  */

static void
gfc_conv_intrinsic_transfer (gfc_se * se, gfc_expr * expr)
{
  gfc_actual_arglist *arg;
  gfc_se argse;
  tree type;
  tree ptr;
  gfc_ss *ss;

  assert (!se->ss);

  /* Get a pointer to the source.  */
  arg = expr->value.function.actual;
  ss = gfc_walk_expr (arg->expr);
  gfc_init_se (&argse, NULL);
  if (ss == gfc_ss_terminator)
    gfc_conv_expr_reference (&argse, arg->expr);
  else
    gfc_conv_array_parameter (&argse, arg->expr, ss, 1);
  gfc_add_block_to_block (&se->pre, &argse.pre);
  gfc_add_block_to_block (&se->post, &argse.post);
  ptr = argse.expr;

  arg = arg->next;
  type = gfc_typenode_for_spec (&expr->ts);
  ptr = convert (build_pointer_type (type), ptr);
  if (expr->ts.type == BT_CHARACTER)
    {
      gfc_init_se (&argse, NULL);
      gfc_conv_expr (&argse, arg->expr);
      gfc_add_block_to_block (&se->pre, &argse.pre);
      gfc_add_block_to_block (&se->post, &argse.post);
      se->expr = ptr;
      se->string_length = argse.string_length;
    }
  else
    {
      se->expr = build1 (INDIRECT_REF, type, ptr);
    }
}


/* Generate code for the ALLOCATED intrinsic.
   Generate inline code that directly check the address of the argument.  */

static void
gfc_conv_allocated (gfc_se *se, gfc_expr *expr)
{
  gfc_actual_arglist *arg1;
  gfc_se arg1se;
  gfc_ss *ss1;
  tree tmp;

  gfc_init_se (&arg1se, NULL);
  arg1 = expr->value.function.actual;
  ss1 = gfc_walk_expr (arg1->expr);
  arg1se.descriptor_only = 1;
  gfc_conv_expr_descriptor (&arg1se, arg1->expr, ss1);

  tmp = gfc_conv_descriptor_data (arg1se.expr);
  tmp = build (NE_EXPR, boolean_type_node, tmp, null_pointer_node);
  se->expr = convert (gfc_typenode_for_spec (&expr->ts), tmp);
}


/* Generate code for the ASSOCIATED intrinsic.
   If both POINTER and TARGET are arrays, generate a call to library function
   _gfor_associated, and pass descriptors of POINTER and TARGET to it.
   In other cases, generate inline code that directly compare the address of
   POINTER with the address of TARGET.  */

static void
gfc_conv_associated (gfc_se *se, gfc_expr *expr)
{
  gfc_actual_arglist *arg1;
  gfc_actual_arglist *arg2;
  gfc_se arg1se;
  gfc_se arg2se;
  tree tmp2;
  tree tmp;
  tree args, fndecl;
  gfc_ss *ss1, *ss2;

  gfc_init_se (&arg1se, NULL);
  gfc_init_se (&arg2se, NULL);
  arg1 = expr->value.function.actual;
  arg2 = arg1->next;
  ss1 = gfc_walk_expr (arg1->expr);

  if (!arg2->expr)
    {
      /* No optional target.  */
      if (ss1 == gfc_ss_terminator)
        {
          /* A pointer to a scalar.  */
          arg1se.want_pointer = 1;
          gfc_conv_expr (&arg1se, arg1->expr);
          tmp2 = arg1se.expr;
        }
      else
        {
          /* A pointer to an array.  */
          arg1se.descriptor_only = 1;
          gfc_conv_expr_lhs (&arg1se, arg1->expr);
          tmp2 = gfc_conv_descriptor_data (arg1se.expr);
        }
      tmp = build (NE_EXPR, boolean_type_node, tmp2, null_pointer_node);
      se->expr = tmp;
    }
  else
    {
      /* An optional target.  */
      ss2 = gfc_walk_expr (arg2->expr);
      if (ss1 == gfc_ss_terminator)
        {
          /* A pointer to a scalar.  */
          assert (ss2 == gfc_ss_terminator);
          arg1se.want_pointer = 1;
          gfc_conv_expr (&arg1se, arg1->expr);
          arg2se.want_pointer = 1;
          gfc_conv_expr (&arg2se, arg2->expr);
          tmp = build (EQ_EXPR, boolean_type_node, arg1se.expr, arg2se.expr);
          se->expr = tmp;
        }
      else
        {
          /* A pointer to an array, call library function _gfor_associated.  */
          assert (ss2 != gfc_ss_terminator);
          args = NULL_TREE;
          arg1se.want_pointer = 1;
          gfc_conv_expr_descriptor (&arg1se, arg1->expr, ss1);
          args = gfc_chainon_list (args, arg1se.expr);
          arg2se.want_pointer = 1;
          gfc_conv_expr_descriptor (&arg2se, arg2->expr, ss2);
          gfc_add_block_to_block (&se->pre, &arg2se.pre);
          gfc_add_block_to_block (&se->post, &arg2se.post);
          args = gfc_chainon_list (args, arg2se.expr);
          fndecl = gfor_fndecl_associated;
          se->expr = gfc_build_function_call (fndecl, args);
        }
     }
  se->expr = convert (gfc_typenode_for_spec (&expr->ts), se->expr);
}


/* Scan a string for any one of the characters in a set of characters.   */

static void
gfc_conv_intrinsic_scan (gfc_se * se, gfc_expr * expr)
{
  tree args;
  tree back;
  tree type;
  tree tmp;

  args = gfc_conv_intrinsic_function_args (se, expr);
  type = gfc_typenode_for_spec (&expr->ts);
  tmp = gfc_advance_chain (args, 3);
  if (TREE_CHAIN (tmp) == NULL_TREE)
    {
      back = convert (gfc_logical4_type_node, integer_one_node);
      back = tree_cons (NULL_TREE, integer_zero_node, NULL_TREE);
      TREE_CHAIN (tmp) = back;
    }
  else
    {
      back = TREE_CHAIN (tmp);
      TREE_VALUE (back) = convert (gfc_logical4_type_node, TREE_VALUE (back));
    }

  se->expr = gfc_build_function_call (gfor_fndecl_string_scan, args);
  se->expr = convert (type, se->expr);
}


/* Verify that a set of characters contains all the characters in a string
   by indentifying the position of the first character in a string of
   characters that does not appear in a given set of characters.  */

static void
gfc_conv_intrinsic_verify (gfc_se * se, gfc_expr * expr)
{
  tree args;
  tree back;
  tree type;
  tree tmp;

  args = gfc_conv_intrinsic_function_args (se, expr);
  type = gfc_typenode_for_spec (&expr->ts);
  tmp = gfc_advance_chain (args, 3);
  if (TREE_CHAIN (tmp) == NULL_TREE)
    {
      back = convert (gfc_logical4_type_node, integer_one_node);
      back = tree_cons (NULL_TREE, integer_zero_node, NULL_TREE);
      TREE_CHAIN (tmp) = back;
    }
  else
    {
      back = TREE_CHAIN (tmp);
      TREE_VALUE (back) = convert (gfc_logical4_type_node, TREE_VALUE (back));
    }

  se->expr = gfc_build_function_call (gfor_fndecl_string_verify, args);
  se->expr = convert (type, se->expr);
}

/* Prepare components and related information of a real number which is
   the first argument of a elemental functions to manipulate reals.  */

static
void prepare_arg_info (gfc_se * se, gfc_expr * expr, 
                       real_compnt_info * rcs, int all)
{
   tree arg;
   tree masktype;
   tree tmp;
   tree wbits;
   tree one;
   tree exponent, fraction;
   int n;
   gfc_expr *a1;

   if (TARGET_FLOAT_FORMAT != IEEE_FLOAT_FORMAT)
     gfc_todo_error ("Non-IEEE floating format");
    
   assert (expr->expr_type == EXPR_FUNCTION);

   arg = gfc_conv_intrinsic_function_args (se, expr);
   arg = TREE_VALUE (arg);
   rcs->type = TREE_TYPE (arg);

   /* Force arg'type to integer by unaffected convert  */
   a1 = expr->value.function.actual->expr;
   masktype = gfc_get_int_type (a1->ts.kind);
   rcs->mtype = masktype;
   tmp = build1 (VIEW_CONVERT_EXPR, masktype, arg);
   arg = gfc_create_var (masktype, "arg");
   gfc_add_modify_expr(&se->pre, arg, tmp);
   rcs->arg = arg;

   /* Caculate the numbers of bits of exponent, fraction and word  */
   n = gfc_validate_kind (a1->ts.type, a1->ts.kind);
   tmp = build_int_2 (gfc_real_kinds[n].digits - 1, 0);
   rcs->fdigits = convert (masktype, tmp);
   wbits = build_int_2 (TYPE_PRECISION (rcs->type) - 1, 0);
   wbits = convert (masktype, wbits);
   rcs->edigits = fold (build (MINUS_EXPR, masktype, wbits, tmp));

   /* Form masks for exponent/fraction/sign  */
   one = gfc_build_const (masktype, integer_one_node);
   rcs->smask = fold (build (LSHIFT_EXPR, masktype, one, wbits));
   rcs->f1 = fold (build (LSHIFT_EXPR, masktype, one, rcs->fdigits));
   rcs->emask = fold (build (MINUS_EXPR, masktype, rcs->smask, rcs->f1));
   rcs->fmask = fold (build (MINUS_EXPR, masktype, rcs->f1, one));
   /* Form bias.  */
   tmp = fold (build (MINUS_EXPR, masktype, rcs->edigits, one));
   tmp = fold (build (LSHIFT_EXPR, masktype, one, tmp));
   rcs->bias = fold (build (MINUS_EXPR, masktype, tmp ,one));

   if (all)
   { 
     /* exponent, and fraction  */
     tmp = build (BIT_AND_EXPR, masktype, arg, rcs->emask);
     tmp = build (RSHIFT_EXPR, masktype, tmp, rcs->fdigits);
     exponent = gfc_create_var (masktype, "exponent");
     gfc_add_modify_expr(&se->pre, exponent, tmp);
     rcs->expn = exponent;

     tmp = build (BIT_AND_EXPR, masktype, arg, rcs->fmask);
     fraction = gfc_create_var (masktype, "fraction");
     gfc_add_modify_expr(&se->pre, fraction, tmp);
     rcs->frac = fraction;
  }
}

/* Build a call to __builtin_clz.  */

static tree
call_builtin_clz (tree result_type, tree op0)
{
  tree fn, parms, call;
  enum machine_mode op0_mode = TYPE_MODE (TREE_TYPE (op0));

  if (op0_mode == TYPE_MODE (integer_type_node))
    fn = built_in_decls[BUILT_IN_CLZ];
  else if (op0_mode == TYPE_MODE (long_integer_type_node))
    fn = built_in_decls[BUILT_IN_CLZL];
  else if (op0_mode == TYPE_MODE (long_long_integer_type_node))
    fn = built_in_decls[BUILT_IN_CLZLL];
  else
    abort ();

  parms = tree_cons (NULL, op0, NULL);
  call = gfc_build_function_call (fn, parms);

  return convert (result_type, call);
}


/*  Generate code for the SET_EXPONENT intrinsic.
    SET_EXPONENT (s, i) = s * 2^(i-e).
    We generate:
    bias = bias - 1;
    full_1_expn = emask >> BITS_OF_FRACTION_OF(s)
    if (s == 0 || expn == full_1_expn) // s is a NaN or Inf or Zero
    {
      res = s
      goto exit
    }
    if (expn != 0)  // s is normalized
    {
      expn = arg2 + bias
      if (expn <= 0)
      {
        frac = frac | (1 <<  (BITS_OF_FRACTION_OF));
        frac = frac >>(-expn + 1);
        expn = 0;
      } 
      if (expn >= full_1_expn)
        expn = full_1_expn
    }
    else  // s is denormalized 
    { 
      expn = arg2 + bias
      t1 = frac << PRECISION_OF_TYPE(s) - BITS_OF_FRACTION_OF(s);
      lz = leadzero(t1)
      if (expn > 0)
        frac = frac << (lz + 1)
      else
      {
        diff = expn + lz
        frac = (diff >= 0) ? frac << diff : frac >> (-diff)
        expn = 0
      }
    }
    res = sign | expn | frac    
    exit :  

 */
 
static void
gfc_conv_intrinsic_set_exponent (gfc_se * se, gfc_expr * expr)
{
   tree arg, args;
   tree arg2;
   tree masktype;
   tree tmp, t1, t2;
   tree leadzero, one, diff, zero, bias;
   tree sign, exponent, fraction, full_1_expn;
   tree cond, cond1, res, exit_label;
   tree norm_case, nnorm_case, nnorm_case_1, nnorm_case_2;
   stmtblock_t block, block1;
   real_compnt_info rcs;
   
   arg = gfc_conv_intrinsic_function_args (se, expr);
   args = TREE_VALUE (arg);
   arg2 = TREE_VALUE (TREE_CHAIN (arg));

   prepare_arg_info (se, expr, &rcs, 1);
   arg = rcs.arg;
   masktype = rcs.mtype;
   exponent = rcs.expn;
   fraction = rcs.frac;

   arg2 = convert (masktype, arg2);
   one = gfc_build_const (masktype, integer_one_node);
   zero = gfc_build_const (masktype, integer_zero_node);

   full_1_expn = fold (build (RSHIFT_EXPR, masktype, rcs.emask, rcs.fdigits));
   
   /* Creat variables for the result and tmporarilly using.  */
   res = gfc_create_var (rcs.type, "set_exponent");
   leadzero = gfc_create_var (masktype, "LZ");
   diff = gfc_create_var (masktype, "diff");

   bias = fold (build (MINUS_EXPR, masktype, rcs.bias, one));

   exit_label = gfc_build_label_decl (NULL_TREE);
   TREE_USED (exit_label) = 1;
 
   /* Code for s being Zero or NaN or Inf  */
   gfc_start_block (&block);
   gfc_add_modify_expr (&block, res, args);
   tmp = build_v (GOTO_EXPR, exit_label);
   gfc_add_expr_to_block (&block, tmp);
   t1 = gfc_finish_block (&block);
   cond = build (EQ_EXPR, boolean_type_node, exponent, full_1_expn);
   tmp = build (EQ_EXPR, boolean_type_node, arg, zero);
   cond = build (TRUTH_OR_EXPR, boolean_type_node, tmp, cond);
   tmp = build_v (COND_EXPR, cond, t1, build_empty_stmt ());
   gfc_add_expr_to_block (&se->pre, tmp);

   /* Generate code for normalized case */ 
   gfc_start_block (&block);
   tmp = build (PLUS_EXPR, masktype, bias, arg2); 
   gfc_add_modify_expr(&block, exponent, tmp);

   cond = build (LE_EXPR, boolean_type_node, exponent, zero);

   gfc_start_block (&block1);
   t1 = build (BIT_IOR_EXPR, masktype, fraction, rcs.f1);
   tmp = build1 (NEGATE_EXPR, masktype, exponent);
   tmp = build (PLUS_EXPR, masktype, tmp, one);
   tmp = build (RSHIFT_EXPR, masktype, t1, tmp);
   gfc_add_modify_expr (&block1, fraction, tmp);
   gfc_add_modify_expr (&block1, exponent, zero);
   t1 = gfc_finish_block (&block1);

   cond1 = build (GE_EXPR, boolean_type_node, exponent, full_1_expn);
   tmp = build (MODIFY_EXPR, masktype, exponent, full_1_expn);
   t2 = build (COND_EXPR, masktype, cond1, tmp, build_empty_stmt ());

   tmp = build_v (COND_EXPR, cond, t1, t2);
   gfc_add_expr_to_block (&block, tmp);

   norm_case = gfc_finish_block (&block);
 
   /* Denormalized case  */
   gfc_start_block(&block); 
   tmp = build (PLUS_EXPR, masktype, bias, arg2); 
   gfc_add_modify_expr(&block, exponent, tmp);
     
   t2 = fold (build (PLUS_EXPR, masktype, rcs.edigits, one));
   t1 = build (LSHIFT_EXPR, masktype, fraction, t2);
   tmp = call_builtin_clz (masktype, t1);
   gfc_add_modify_expr (&block, leadzero, tmp); 

   /* expn > 0  */
   gfc_start_block (&block1);
   t1 = build (PLUS_EXPR, masktype, leadzero, one);
   t1 = build (LSHIFT_EXPR, masktype, fraction, t1);
   tmp = build (BIT_AND_EXPR, masktype, t1, rcs.fmask);
   gfc_add_modify_expr (&block1, fraction, tmp);
   nnorm_case_1 = gfc_finish_block (&block1);
 
   /* expn <= 0 */
   gfc_start_block (&block1);
   t1 = build (PLUS_EXPR, masktype, exponent, leadzero);
   gfc_add_modify_expr(&block1, diff, t1);

   t1 = build (LSHIFT_EXPR, masktype, fraction, diff);
   t2 = build1 (NEGATE_EXPR, masktype, diff);
   t2 = build (RSHIFT_EXPR, masktype, fraction, t2);
   cond1 = build (GT_EXPR, boolean_type_node, diff, integer_zero_node);
   tmp = build (COND_EXPR, masktype, cond1, t1, t2);
   gfc_add_modify_expr (&block1, fraction, tmp);
   nnorm_case_2 = gfc_finish_block (&block1);
        
   cond = build (GT_EXPR, boolean_type_node, exponent, zero);
                                                                              
   tmp = build_v (COND_EXPR, cond, nnorm_case_1, nnorm_case_2);
 
   gfc_add_expr_to_block (&block, tmp);
   nnorm_case = gfc_finish_block (&block);
  
   cond =  build (NE_EXPR, boolean_type_node, exponent, integer_zero_node);
   tmp = build_v (COND_EXPR, cond, norm_case, nnorm_case);
   gfc_add_expr_to_block (&se->pre, tmp);

   sign = build (BIT_AND_EXPR, masktype, arg, rcs.smask);
   t1 = build (LSHIFT_EXPR, masktype, exponent, rcs.fdigits);
   t2 = build (BIT_IOR_EXPR,  masktype, sign, t1);
   tmp = build (BIT_IOR_EXPR, masktype, t2, fraction);
   tmp = build1 (VIEW_CONVERT_EXPR, rcs.type, tmp);
   gfc_add_modify_expr (&se->pre, res, tmp);
 
   /* add the exit label  */
   tmp = build1_v (LABEL_EXPR, exit_label);
   gfc_add_expr_to_block (&se->pre, tmp);
 
   se->expr = res;
}  


/*  Generate code for the SCALE intrinsic. SCALE (s, i) = s * 2^i.
    We generate:

    full_1_expn = emask >> BITS_OF_FRACTION_OF(s)

    if (expn == full_1_expn) // s is a NaN or Inf
    {
      res = s
      goto exit
    }
    if (expn != 0)  // s is normalized
    {
      expn = i
      if (expn < 0)
      {
        frac = frac | (1 <<  (BITS_OF_FRACTION_OF));
        frac = frac >>(-expn + 1);
        expn = 0;
      } 
      if (expn >= full_1_expn)
        expn = full_1_expn
    }
    else  // s is denormalized 
    { 
      t1 = frac << PRECISION_OF_TYPE(s) - BITS_OF_FRACTION_OF(s);
      lz = leadzero(t1)
      diff = i - lz
      if (diff > 0) 
      {
        frac = frac << (lz + 1)
        expn = expn + diff
      }
      else
      {
        frac = (i >= 0) ? frac << i : frac >> (-i)
      }
    }
    res = sign | expn | frac    
    exit :  

 */
 
static void
gfc_conv_intrinsic_scale (gfc_se * se, gfc_expr * expr)
{
   tree arg, args;
   tree arg2;
   tree masktype;
   tree tmp, t1, t2;
   tree leadzero, one, diff, zero;
   tree sign, exponent, fraction, full_1_expn;
   tree cond, cond1, res, exit_label;
   tree norm_case, nnorm_case, nnorm_case_1, nnorm_case_2;
   stmtblock_t block, block1;
   real_compnt_info rcs;
   
   arg = gfc_conv_intrinsic_function_args (se, expr);
   arg2 = TREE_VALUE (TREE_CHAIN (arg));
   args = TREE_VALUE (arg);

   prepare_arg_info (se, expr, &rcs, 1);
   arg = rcs.arg;
   masktype = rcs.mtype;
   exponent = rcs.expn;
   fraction = rcs.frac;

   arg2 = convert (masktype, arg2);
   one = gfc_build_const (masktype, integer_one_node);
   zero = gfc_build_const (masktype, integer_zero_node);

   full_1_expn = fold (build (RSHIFT_EXPR, masktype, rcs.emask, rcs.fdigits));
   
   /* Creat variables for the result and tmporarilly using.  */
   res = gfc_create_var (rcs.type, "scale");
   leadzero = gfc_create_var (masktype, "LZ");
   diff = gfc_create_var (masktype, "diff");

   /* Code for s being NaN or Inf  */
   gfc_start_block (&block);
   exit_label = gfc_build_label_decl (NULL_TREE);
   TREE_USED (exit_label) = 1;
   gfc_add_modify_expr (&block, res, args);
   tmp = build_v (GOTO_EXPR, exit_label);
   gfc_add_expr_to_block (&block, tmp);
   tmp = gfc_finish_block (&block);
   cond = build (EQ_EXPR, boolean_type_node, exponent, full_1_expn);
   tmp = build_v (COND_EXPR, cond, tmp, build_empty_stmt ());
   gfc_add_expr_to_block (&se->pre, tmp);

   /* Generate code for normalized case */ 
   gfc_start_block (&block);
   tmp = build (PLUS_EXPR, masktype, exponent, arg2); 
   gfc_add_modify_expr(&block, exponent, tmp);
   cond = build (LT_EXPR, boolean_type_node, exponent, zero);

   gfc_start_block (&block1);
   t1 = build (BIT_IOR_EXPR, masktype, fraction, rcs.f1);
   tmp = build1 (NEGATE_EXPR, masktype, exponent);
   tmp = build (PLUS_EXPR, masktype, tmp, one);
   tmp = build (RSHIFT_EXPR, masktype, t1, tmp);
   gfc_add_modify_expr (&block1, fraction, tmp);
   gfc_add_modify_expr (&block1, exponent, zero);
   t1 = gfc_finish_block (&block1);

   gfc_start_block (&block1);
   cond1 = build (GE_EXPR, boolean_type_node, exponent, full_1_expn);
   tmp = build (MODIFY_EXPR, masktype, exponent, full_1_expn);
   tmp = build (COND_EXPR, masktype, cond1, tmp, build_empty_stmt ());
   gfc_add_expr_to_block (&block1, tmp);
   t2 = gfc_finish_block (&block1);

   tmp = build_v (COND_EXPR, cond, t1, t2);
   gfc_add_expr_to_block (&block, tmp);

   norm_case = gfc_finish_block (&block);
 
   /* Denormalized case  */
   gfc_start_block(&block); 

   t2 = fold (build (PLUS_EXPR, masktype, rcs.edigits, one));
   t1 = build (LSHIFT_EXPR, masktype, fraction, t2);
   tmp = call_builtin_clz (masktype, t1);
   gfc_add_modify_expr (&block, leadzero, tmp); 
   t1 = build (MINUS_EXPR, masktype, arg2, leadzero);
   gfc_add_modify_expr(&block, diff, t1);

   /* diff > 0  */
   gfc_start_block (&block1);
   t1 = build (PLUS_EXPR, masktype, leadzero, one);
   t1 = build (LSHIFT_EXPR, masktype, fraction, t1);
   tmp = build (BIT_AND_EXPR, masktype, t1, rcs.fmask);
   gfc_add_modify_expr (&block1, fraction, tmp);
   t1 = build (PLUS_EXPR, masktype, exponent, diff);
   gfc_add_modify_expr(&block1, exponent, t1);
   nnorm_case_1 = gfc_finish_block (&block1);
 
   /* diff <= 0 */
   gfc_start_block (&block1);
   t1 = build (LSHIFT_EXPR, masktype, fraction, arg2);
   t2 = build1 (NEGATE_EXPR, masktype, arg2);
   t2 = build (RSHIFT_EXPR, masktype, fraction, t2);
   cond1 = build (GE_EXPR, boolean_type_node, arg2, integer_zero_node);
   tmp = build (COND_EXPR, masktype, cond1, t1, t2);
   gfc_add_modify_expr (&block1, fraction, tmp);
   nnorm_case_2 = gfc_finish_block (&block1);
        
   cond = build (GT_EXPR, boolean_type_node, diff, zero);
                                                                              
   tmp = build_v (COND_EXPR, cond, nnorm_case_1, nnorm_case_2);
 
   gfc_add_expr_to_block (&block, tmp);
   nnorm_case = gfc_finish_block (&block);
  
   cond =  build (NE_EXPR, boolean_type_node, exponent, integer_zero_node);
   tmp = build_v (COND_EXPR, cond, norm_case, nnorm_case);
   gfc_add_expr_to_block (&se->pre, tmp);

   sign = build (BIT_AND_EXPR, masktype, arg, rcs.smask);
   t1 = build (LSHIFT_EXPR, masktype, exponent, rcs.fdigits);
   t2 = build (BIT_IOR_EXPR,  masktype, sign, t1);
   tmp = build (BIT_IOR_EXPR, masktype, t2, fraction);
   tmp = build1 (VIEW_CONVERT_EXPR, rcs.type, tmp);
   gfc_add_modify_expr (&se->pre, res, tmp);
 
   /* add the exit label  */
   tmp = build1_v (LABEL_EXPR, exit_label);
   gfc_add_expr_to_block (&se->pre, tmp);
 
   se->expr = res;
}  


/*  Generate code for the NEAREST intrinsic.
    We generate:
  {
                                                                               
    if (expn == full_1_expn) // s is a NaN or Inf
    {
      res = s;
    }
    else if ((s<<1) == 0)
    {
      res = 1 << (BITS_OF_FRACTION_OF);
      if (r < 0)
        res = res | (1 <<  (PRECISION_OF_TYPE-1));
    }
    else
    {
      if (r >= 0)
        delta = 1;
      else
        delta = -1;
                                                                                
      if (s < 0) delta = -delta;
                                                                                
      res = s + delta;
    }
  }
*/

static void
gfc_conv_intrinsic_nearest (gfc_se * se, gfc_expr * expr)
{
   tree arg, args;
   tree arg1;
   tree masktype;
   tree tmp, t1;
   tree one, zero;
   tree cond, cond1, cond2;
   tree res, delta;
   tree case1, case2, case3;
   stmtblock_t block;
   real_compnt_info rcs;

   arg = gfc_conv_intrinsic_function_args (se, expr);
   arg1 = TREE_VALUE (TREE_CHAIN (arg));
   args = TREE_VALUE (arg);

   prepare_arg_info (se, expr, &rcs, 0);
   arg = rcs.arg;
   masktype = rcs.mtype;

   one = gfc_build_const (masktype, integer_one_node);
   zero = gfc_build_const (masktype, integer_zero_node);

   arg1 = build1 (VIEW_CONVERT_EXPR, masktype, arg1);
   /* Creat variables for the result and temporarilly using.  */
   res = gfc_create_var (masktype, "res");
   delta = gfc_create_var (masktype, "delta");

   tmp = build (BIT_AND_EXPR, masktype, arg, rcs.emask);
   tmp = build (BIT_XOR_EXPR, masktype, tmp, rcs.emask);
   cond1 = build (EQ_EXPR, boolean_type_node, tmp, zero);

   /* Code for S being NaN or Inf  */
   gfc_start_block (&block);
   gfc_add_modify_expr (&block, res, arg);
   case1 = gfc_finish_block (&block);

   tmp = build (LSHIFT_EXPR, masktype, arg, one);
   cond2 = build (EQ_EXPR, boolean_type_node, tmp, zero);

   /* Code for S == 0  */

   gfc_start_block (&block);
   t1 = fold (build (PLUS_EXPR, masktype, rcs.edigits, rcs.fdigits));
   t1 = fold (build (LSHIFT_EXPR, masktype, one, t1));
   tmp = build (GT_EXPR, boolean_type_node, arg1, integer_zero_node);
   t1 = build (COND_EXPR, masktype, tmp, zero, t1);
   tmp = build (BIT_IOR_EXPR, masktype, rcs.f1, t1);
   tmp = build (MODIFY_EXPR, masktype, res, tmp);
   gfc_add_expr_to_block (&block, tmp);
   case2 = gfc_finish_block (&block);

   /* Code for S !=0 && S != Inf/NaN  */
   gfc_start_block (&block);
   cond = build (GE_EXPR, boolean_type_node, arg1, integer_zero_node);
   tmp = build (COND_EXPR, masktype, cond, one, integer_minus_one_node);
   gfc_add_modify_expr (&block, delta, tmp);

   cond = build (LT_EXPR, boolean_type_node, arg, zero);
   tmp = build1 (NEGATE_EXPR, masktype, delta);
   tmp = build (MODIFY_EXPR, masktype, delta, tmp);
   tmp = build_v (COND_EXPR, cond, tmp, build_empty_stmt ()); 
   gfc_add_expr_to_block (&block, tmp);
   
   tmp = build (PLUS_EXPR, masktype, arg, delta);
   gfc_add_modify_expr (&block, res, tmp);
   case3 = gfc_finish_block (&block);                                      

   tmp = build_v (COND_EXPR, cond2, case2, case3);
   tmp = build_v (COND_EXPR, cond1, case1, tmp);
   gfc_add_expr_to_block (&se->pre, tmp);

   /* Force the result's type back to its original type  */
   tmp = build1 (VIEW_CONVERT_EXPR, rcs.type, res);
   se->expr = tmp;
}


/* Generate code for FRACTION(X) intrinsic function. We generate:

  if (X = 0)
    result = X
  else
  {
    if (expn == 0) // X is denormalized.
    {
      sedigits = (PRECISION_OF_TYPE (X) - BITS_OF_FRACTION (X) + 1)
      frac = frac << sedigits
      t1 = leadzero(frac) + 1
      frac = frac << t1
      frac = frac >> sedigits
    }
    result = sign | bias-1 | frac
  }
*/

static void
gfc_conv_intrinsic_fraction (gfc_se * se, gfc_expr * expr)
{
   tree arg;
   tree masktype;
   tree tmp, t1, t2;
   tree sedigits;
   tree one, zero;
   tree sign, fraction;
   tree cond;
   real_compnt_info rcs;

   prepare_arg_info (se, expr, &rcs, 1);
   arg = rcs.arg;
   masktype = rcs.mtype;
   fraction = rcs.frac;

   one = gfc_build_const (masktype, integer_one_node);
   sedigits = fold (build (PLUS_EXPR, masktype, rcs.edigits, one));
   /* arg != 0.  */
   /* Caculate denormalized fraction.  */
   t2 = build_int_2 (2, 0);
   t2 = convert (masktype, t2);
   t1 = call_builtin_clz (masktype, fraction);
   t1 = build (PLUS_EXPR, masktype, t1, one);
   tmp = build (LSHIFT_EXPR, masktype, fraction, t1);
   tmp = build (RSHIFT_EXPR, masktype, tmp, sedigits);

   zero = gfc_build_const (masktype, integer_zero_node);
   cond = build (EQ_EXPR, boolean_type_node, rcs.expn, zero);
   fraction = build (COND_EXPR, masktype, cond, tmp, fraction);

   /* Form exponent.  */
   tmp = fold(build (MINUS_EXPR, masktype, rcs.bias, one));
   tmp = fold(build (LSHIFT_EXPR, masktype, tmp, rcs.fdigits));

   sign = build (BIT_AND_EXPR, masktype, arg, rcs.smask);
   tmp = build (BIT_IOR_EXPR, masktype, sign, tmp);
   tmp = build (BIT_IOR_EXPR, masktype, tmp, fraction);

   cond = build (EQ_EXPR, boolean_type_node, arg, zero);
   tmp = build (COND_EXPR, masktype, cond, arg, tmp);
   se->expr = build1 (VIEW_CONVERT_EXPR, rcs.type, tmp);
}


/* Generate code for EXPONENT(X) intrinsic function. We generate:
                                                              
    if (s == 0)
      res = 0
    else
    if (expn == 0)
    {
      t = leadzero(frac)
      res = - (t - edigits - 1) - bias + 1 // -t + edigits - bias + 2
    }
    else
      res = expn - bias + 1
*/

static void
gfc_conv_intrinsic_exponent (gfc_se * se, gfc_expr * expr)
{
   tree arg;
   tree type, masktype;
   tree tmp, t1, t2;
   tree exponent;
   tree one, zero;
   tree cond;
   real_compnt_info rcs;

   prepare_arg_info (se, expr, &rcs, 1);
   arg = rcs.arg;
   masktype = rcs.mtype;
   exponent = rcs.expn;

   one = gfc_build_const (masktype, integer_one_node);
   zero = gfc_build_const (masktype, integer_zero_node);

   /* arg != 0.  */
   /* exponent == 0  */
   t2 = fold (build (PLUS_EXPR, masktype, rcs.edigits, one));
   t2 = fold (build (MINUS_EXPR, masktype, t2, rcs.bias));
   t2 = fold (build (PLUS_EXPR, masktype, t2, one));
   t1 = call_builtin_clz (masktype, rcs.frac);
   t1 = build (MINUS_EXPR, masktype, t2, t1);
   /* exponent != 0  */
   t2 = fold (build (MINUS_EXPR, masktype, rcs.bias, one));
   t2 = build (MINUS_EXPR, masktype, exponent, t2);

   cond = build (EQ_EXPR, boolean_type_node, exponent, zero);
   t1 = build (COND_EXPR, masktype, cond, t1, t2);

   cond = build (EQ_EXPR, boolean_type_node, arg, zero);
   exponent = build (COND_EXPR, masktype, cond, zero, t1);
   type = gfc_typenode_for_spec (&expr->ts);
   tmp = convert (type, exponent); 
   se->expr = tmp;
}

/* Generate code for SPACING (X) intrinsic function. We generate:
                                                                                
    t = expn - (BITS_OF_FRACTION)
    res = t << (BITS_OF_FRACTION)
    if (t < 0)
      res = tiny(X)
*/

static void
gfc_conv_intrinsic_spacing (gfc_se * se, gfc_expr * expr)
{
   tree arg;
   tree masktype;
   tree tmp, t1, cond;
   tree tiny, zero;
   tree fdigits;
   real_compnt_info rcs;

   prepare_arg_info (se, expr, &rcs, 0);
   arg = rcs.arg;
   masktype = rcs.mtype;
   fdigits = rcs.fdigits;
   tiny = rcs.f1;
   zero = gfc_build_const (masktype, integer_zero_node);
   tmp = build (BIT_AND_EXPR, masktype, rcs.emask, arg);
   tmp = build (RSHIFT_EXPR, masktype, tmp, fdigits);
   tmp = build (MINUS_EXPR, masktype, tmp, fdigits);
   cond = build (LE_EXPR, boolean_type_node, tmp, zero);
   t1 = build (LSHIFT_EXPR, masktype, tmp, fdigits);
   tmp = build (COND_EXPR, masktype, cond, tiny, t1);
   tmp = build1 (VIEW_CONVERT_EXPR, rcs.type, tmp);

   se->expr = tmp;
}

/* Generate code for RRSPACING (X) intrinsic function. We generate:                                                                            
    sedigits = edigits + 1;
    if (expn == 0)
    {
      t1 = leadzero (frac);
      frac = frac << (t1 + sedigits);
      frac = frac >> (sedigits);
    }
    t = bias + BITS_OF_FRACTION_OF;
    res = (t << BITS_OF_FRACTION_OF) | frac;
*/

static void
gfc_conv_intrinsic_rrspacing (gfc_se * se, gfc_expr * expr)
{
   tree masktype;
   tree tmp, t1, t2, cond;
   tree one, zero;
   tree fdigits, fraction;
   real_compnt_info rcs;

   prepare_arg_info (se, expr, &rcs, 1);
   masktype = rcs.mtype;
   fdigits = rcs.fdigits;
   fraction = rcs.frac;
   one = gfc_build_const (masktype, integer_one_node);
   zero = gfc_build_const (masktype, integer_zero_node);
   t2 = build (PLUS_EXPR, masktype, rcs.edigits, one);

   t1 = call_builtin_clz (masktype, fraction);
   tmp = build (PLUS_EXPR, masktype, t1, one);
   tmp = build (LSHIFT_EXPR, masktype, fraction, tmp);
   tmp = build (RSHIFT_EXPR, masktype, tmp, t2);
   cond = build (EQ_EXPR, boolean_type_node, rcs.expn, zero);
   fraction = build (COND_EXPR, masktype, cond, tmp, fraction);

   tmp = build (PLUS_EXPR, masktype, rcs.bias, fdigits);
   tmp = build (LSHIFT_EXPR, masktype, tmp, fdigits);
   tmp = build (BIT_IOR_EXPR, masktype, tmp, fraction);

   tmp = build1 (VIEW_CONVERT_EXPR, rcs.type, tmp);
   se->expr = tmp;
}

/* Generate code for SELECTED_INT_KIND (R) intrinsic function.  */

static void
gfc_conv_intrinsic_si_kind (gfc_se * se, gfc_expr * expr)
{
  tree args;

  args = gfc_conv_intrinsic_function_args (se, expr);
  args = TREE_VALUE (args);
  args = build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (args)), args);
  args = tree_cons (NULL_TREE, args, NULL_TREE);
  se->expr = gfc_build_function_call (gfor_fndecl_si_kind, args);
}

/* Generate code for SELECTED_REAL_KIND (P, R) intrinsic function.  */

static void
gfc_conv_intrinsic_sr_kind (gfc_se * se, gfc_expr * expr)
{
  gfc_actual_arglist *actual;
  tree args;
  gfc_se argse;

  args = NULL_TREE;
  for (actual = expr->value.function.actual; actual; actual = actual->next)
    {
      gfc_init_se (&argse, se);

      /* Pass a NULL pointer for an absent arg.  */
      if (actual->expr == NULL)
        argse.expr = null_pointer_node;
      else
        gfc_conv_expr_reference (&argse, actual->expr);

      gfc_add_block_to_block (&se->pre, &argse.pre);
      gfc_add_block_to_block (&se->post, &argse.post);
      args = gfc_chainon_list (args, argse.expr);
    }
  se->expr = gfc_build_function_call (gfor_fndecl_sr_kind, args);
}


/* Generate code for an intrinsic function.  Some map directly to library
   calls, others get special handling.  In some cases the name of the function
   used depends on the type specifiers.  */

void
gfc_conv_intrinsic_function (gfc_se * se, gfc_expr * expr)
{
  gfc_intrinsic_sym *isym;
  char *name;
  int lib;

  isym = expr->value.function.isym;

  name = &expr->value.function.name[2];

  if (expr->rank > 0)
    {
      lib = gfc_is_intrinsic_libcall (expr);
      if (lib != 0)
	{
	  if (lib == 1)
	    se->ignore_optional = 1;
	  gfc_conv_intrinsic_funcall (se, expr);
	  return;
	}
    }

  switch (expr->value.function.isym->generic_id)
    {
    case GFC_ISYM_NONE:
      abort ();

    case GFC_ISYM_CSHIFT:
    case GFC_ISYM_REPEAT:
    case GFC_ISYM_TRIM:
      gfc_todo_error ("Intrinsic %s", expr->value.function.name);

    case GFC_ISYM_SI_KIND:
      gfc_conv_intrinsic_si_kind (se, expr);
      break;

    case GFC_ISYM_SR_KIND:
      gfc_conv_intrinsic_sr_kind (se, expr);
      break;

    case GFC_ISYM_EXPONENT:
      gfc_conv_intrinsic_exponent (se, expr);
      break;

    case GFC_ISYM_FRACTION:
      gfc_conv_intrinsic_fraction(se, expr);
      break;
                                                                                
    case GFC_ISYM_NEAREST:
      gfc_conv_intrinsic_nearest(se, expr);
      break;

    case GFC_ISYM_SCALE:
      gfc_conv_intrinsic_scale (se, expr);
      break;

    case GFC_ISYM_SET_EXPONENT:
      gfc_conv_intrinsic_set_exponent (se, expr);
      break;

    case GFC_ISYM_SPACING:
      gfc_conv_intrinsic_spacing (se, expr);
      break;

    case GFC_ISYM_RRSPACING:
      gfc_conv_intrinsic_rrspacing (se, expr);
      break;

    case GFC_ISYM_SCAN:
      gfc_conv_intrinsic_scan (se, expr);
      break;

    case GFC_ISYM_VERIFY:
      gfc_conv_intrinsic_verify (se, expr);
      break;

    case GFC_ISYM_ALLOCATED:
      gfc_conv_allocated (se, expr);
      break;

    case GFC_ISYM_ASSOCIATED:
      gfc_conv_associated(se, expr);
      break;

    case GFC_ISYM_ABS:
      gfc_conv_intrinsic_abs (se, expr);
      break;

    case GFC_ISYM_ADJUSTL:
      gfc_conv_intrinsic_adjust (se, expr, gfor_fndecl_adjustl);
      break;

    case GFC_ISYM_ADJUSTR:
      gfc_conv_intrinsic_adjust (se, expr, gfor_fndecl_adjustr);
      break;

    case GFC_ISYM_AIMAG:
      gfc_conv_intrinsic_imagpart (se, expr);
      break;

    case GFC_ISYM_AINT:
      gfc_conv_intrinsic_aint (se, expr, FIX_TRUNC_EXPR);
      break;

    case GFC_ISYM_ALL:
      gfc_conv_intrinsic_anyall (se, expr, EQ_EXPR);
      break;

    case GFC_ISYM_ANINT:
      gfc_conv_intrinsic_aint (se, expr, FIX_ROUND_EXPR);
      break;

    case GFC_ISYM_ANY:
      gfc_conv_intrinsic_anyall (se, expr, NE_EXPR);
      break;

    case GFC_ISYM_BTEST:
      gfc_conv_intrinsic_btest (se, expr);
      break;

    case GFC_ISYM_ACHAR:
    case GFC_ISYM_CHAR:
      gfc_conv_intrinsic_char (se, expr);
      break;

    case GFC_ISYM_CONVERSION:
    case GFC_ISYM_REAL:
    case GFC_ISYM_LOGICAL:
    case GFC_ISYM_DBLE:
      gfc_conv_intrinsic_conversion (se, expr);
      break;

      /* Integer conversions are handled seperately to make sure we get the
         correct rounding mode.  */
    case GFC_ISYM_INT:
      gfc_conv_intrinsic_int (se, expr, FIX_TRUNC_EXPR);
      break;

    case GFC_ISYM_NINT:
      gfc_conv_intrinsic_int (se, expr, FIX_ROUND_EXPR);
      break;

    case GFC_ISYM_CEILING:
      gfc_conv_intrinsic_int (se, expr, FIX_CEIL_EXPR);
      break;

    case GFC_ISYM_FLOOR:
      gfc_conv_intrinsic_int (se, expr, FIX_FLOOR_EXPR);
      break;

    case GFC_ISYM_MOD:
      gfc_conv_intrinsic_mod (se, expr, 0);
      break;

    case GFC_ISYM_MODULO:
      gfc_conv_intrinsic_mod (se, expr, 1);
      break;

    case GFC_ISYM_CMPLX:
      gfc_conv_intrinsic_cmplx (se, expr, name[5] == '1');
      break;

    case GFC_ISYM_CONJG:
      gfc_conv_intrinsic_conjg (se, expr);
      break;

    case GFC_ISYM_COUNT:
      gfc_conv_intrinsic_count (se, expr);
      break;

    case GFC_ISYM_DIM:
      gfc_conv_intrinsic_dim (se, expr);
      break;

    case GFC_ISYM_DPROD:
      gfc_conv_intrinsic_dprod (se, expr);
      break;

    case GFC_ISYM_IAND:
      gfc_conv_intrinsic_bitop (se, expr, BIT_AND_EXPR);
      break;

    case GFC_ISYM_IBCLR:
      gfc_conv_intrinsic_singlebitop (se, expr, 0);
      break;

    case GFC_ISYM_IBITS:
      gfc_conv_intrinsic_ibits (se, expr);
      break;

    case GFC_ISYM_IBSET:
      gfc_conv_intrinsic_singlebitop (se, expr, 1);
      break;

    case GFC_ISYM_IACHAR:
    case GFC_ISYM_ICHAR:
      /* We assume ASCII character sequence.  */
      gfc_conv_intrinsic_ichar (se, expr);
      break;

    case GFC_ISYM_IEOR:
      gfc_conv_intrinsic_bitop (se, expr, BIT_XOR_EXPR);
      break;

    case GFC_ISYM_INDEX:
      gfc_conv_intrinsic_index (se, expr);
      break;

    case GFC_ISYM_IOR:
      gfc_conv_intrinsic_bitop (se, expr, BIT_IOR_EXPR);
      break;

    case GFC_ISYM_ISHFT:
      gfc_conv_intrinsic_ishft (se, expr);
      break;

    case GFC_ISYM_ISHFTC:
      gfc_conv_intrinsic_ishftc (se, expr);
      break;

    case GFC_ISYM_LBOUND:
      gfc_conv_intrinsic_bound (se, expr, 0);
      break;

    case GFC_ISYM_LEN:
      gfc_conv_intrinsic_len (se, expr);
      break;

    case GFC_ISYM_LEN_TRIM:
      gfc_conv_intrinsic_len_trim (se, expr);
      break;

    case GFC_ISYM_LGE:
      gfc_conv_intrinsic_strcmp (se, expr, GE_EXPR);
      break;

    case GFC_ISYM_LGT:
      gfc_conv_intrinsic_strcmp (se, expr, GT_EXPR);
      break;

    case GFC_ISYM_LLE:
      gfc_conv_intrinsic_strcmp (se, expr, LE_EXPR);
      break;

    case GFC_ISYM_LLT:
      gfc_conv_intrinsic_strcmp (se, expr, LT_EXPR);
      break;

    case GFC_ISYM_MAX:
      gfc_conv_intrinsic_minmax (se, expr, GT_EXPR);
      break;

    case GFC_ISYM_MAXLOC:
      gfc_conv_intrinsic_minmaxloc (se, expr, GT_EXPR);
      break;

    case GFC_ISYM_MAXVAL:
      gfc_conv_intrinsic_minmaxval (se, expr, GT_EXPR);
      break;

    case GFC_ISYM_MERGE:
      gfc_conv_intrinsic_merge (se, expr);
      break;

    case GFC_ISYM_MIN:
      gfc_conv_intrinsic_minmax (se, expr, LT_EXPR);
      break;

    case GFC_ISYM_MINLOC:
      gfc_conv_intrinsic_minmaxloc (se, expr, LT_EXPR);
      break;

    case GFC_ISYM_MINVAL:
      gfc_conv_intrinsic_minmaxval (se, expr, LT_EXPR);
      break;

    case GFC_ISYM_NOT:
      gfc_conv_intrinsic_not (se, expr);
      break;

    case GFC_ISYM_PRESENT:
      gfc_conv_intrinsic_present (se, expr);
      break;

    case GFC_ISYM_PRODUCT:
      gfc_conv_intrinsic_arith (se, expr, MULT_EXPR);
      break;

    case GFC_ISYM_SIGN:
      gfc_conv_intrinsic_sign (se, expr);
      break;

    case GFC_ISYM_SIZE:
      gfc_conv_intrinsic_size (se, expr);
      break;

    case GFC_ISYM_SUM:
      gfc_conv_intrinsic_arith (se, expr, PLUS_EXPR);
      break;

    case GFC_ISYM_TRANSFER:
      gfc_conv_intrinsic_transfer (se, expr);
      break;

    case GFC_ISYM_UBOUND:
      gfc_conv_intrinsic_bound (se, expr, 1);
      break;

    case GFC_ISYM_DOT_PRODUCT:
    case GFC_ISYM_MATMUL:
      gfc_conv_intrinsic_funcall (se, expr);
      break;

    default:
      gfc_conv_intrinsic_lib_function (se, expr);
      break;
    }
}


/* This generates code to execute before entering the scalarization loop.
   Currently does nothing.  */

void
gfc_add_intrinsic_ss_code (gfc_loopinfo * loop ATTRIBUTE_UNUSED, gfc_ss * ss)
{
  switch (ss->expr->value.function.isym->generic_id)
    {
    case GFC_ISYM_UBOUND:
    case GFC_ISYM_LBOUND:
      break;

    default:
      abort ();
      break;
    }
}


/* UBOUND and LBOUND intrinsics with one parameter are expanded into code
   inside the scalarization loop.  */

static gfc_ss *
gfc_walk_intrinsic_bound (gfc_ss * ss, gfc_expr * expr)
{
  gfc_ss *newss;

  /* The two argument version returns a scalar.  */
  if (expr->value.function.actual->next->expr)
    return ss;

  newss = gfc_get_ss ();
  newss->type = GFC_SS_INTRINSIC;
  newss->expr = expr;
  newss->next = ss;

  return newss;
}


/* Walk an intrinsic array libcall.  */

static gfc_ss *
gfc_walk_intrinsic_libfunc (gfc_ss * ss, gfc_expr * expr)
{
  gfc_ss *newss;

  assert (expr->rank > 0);

  newss = gfc_get_ss ();
  newss->type = GFC_SS_FUNCTION;
  newss->expr = expr;
  newss->next = ss;
  newss->data.info.dimen = expr->rank;

  return newss;
}


/* Returns nonzero if the specified intrinsic function call maps directly to a
   an external library call.  Should only be used for functions that return
   arrays.  */

int
gfc_is_intrinsic_libcall (gfc_expr * expr)
{
  assert (expr->expr_type == EXPR_FUNCTION && expr->value.function.isym);
  assert (expr->rank > 0);

  switch (expr->value.function.isym->generic_id)
    {
    case GFC_ISYM_ALL:
    case GFC_ISYM_ANY:
    case GFC_ISYM_COUNT:
    case GFC_ISYM_MATMUL:
    case GFC_ISYM_MAXLOC:
    case GFC_ISYM_MAXVAL:
    case GFC_ISYM_MINLOC:
    case GFC_ISYM_MINVAL:
    case GFC_ISYM_PRODUCT:
    case GFC_ISYM_SUM:
    case GFC_ISYM_SHAPE:
    case GFC_ISYM_SPREAD:
    case GFC_ISYM_TRANSPOSE:
      /* Ignore absent optional parameters.  */
      return 1;

    case GFC_ISYM_RESHAPE:
    case GFC_ISYM_EOSHIFT:
    case GFC_ISYM_PACK:
    case GFC_ISYM_UNPACK:
      /* Pass absent optional parameters.  */
      return 2;

    default:
      return 0;
    }
}

/* Walk an intrinsic function.  */
gfc_ss *
gfc_walk_intrinsic_function (gfc_ss * ss, gfc_expr * expr,
			     gfc_intrinsic_sym * isym)
{
  assert (isym);

  if (isym->elemental)
    return gfc_walk_elemental_function_args (ss, expr, GFC_SS_SCALAR);

  if (expr->rank == 0)
    return ss;

  if (gfc_is_intrinsic_libcall (expr))
    return gfc_walk_intrinsic_libfunc (ss, expr);

  /* Special cases.  */
  switch (isym->generic_id)
    {
    case GFC_ISYM_LBOUND:
    case GFC_ISYM_UBOUND:
      return gfc_walk_intrinsic_bound (ss, expr);

    default:
      /* This probably meant someone forgot to add an intrinsic to the above
         list(s) when they implemented it, or something's gone horribly wrong.
       */
      gfc_todo_error ("Scalarization of non-elemental intrinsic: %s",
		      expr->value.function.name);
    }
}

#include "gt-fortran-trans-intrinsic.h"
