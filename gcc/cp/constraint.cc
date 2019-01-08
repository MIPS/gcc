/* Processing rules for constraints.
   Copyright (C) 2013-2019 Free Software Foundation, Inc.
   Contributed by Andrew Sutton (andrew.n.sutton@gmail.com)

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
#include "timevar.h"
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "intl.h"
#include "flags.h"
#include "cp-tree.h"
#include "c-family/c-common.h"
#include "c-family/c-objc.h"
#include "cp-objcp-common.h"
#include "tree-inline.h"
#include "decl.h"
#include "toplev.h"
#include "type-utils.h"
#include "print-tree.h"


static int parsing_constraint_expr = 0;

parsing_constraint_expression_sentinel::
parsing_constraint_expression_sentinel ()
{
  ++parsing_constraint_expr;
}

parsing_constraint_expression_sentinel::
~parsing_constraint_expression_sentinel()
{
  --parsing_constraint_expr;
}

bool
parsing_constraint_expression_p ()
{
  return parsing_constraint_expr != 0;
}

/*---------------------------------------------------------------------------
		       Constraint expressions
---------------------------------------------------------------------------*/

/* Information provided to substitution.  */

struct subst_info
{
  subst_info (tsubst_flags_t cmp, tree in)
    : complain (cmp), in_decl (in)
  { }

  tsubst_flags_t complain;
  tree in_decl;
};

/* Validate the semantic properties of the constraint.

   FIXME: What happens if we find an overloaded operator?  */
static tree
finish_constraint_binary_op (location_t loc, tree_code code, tree lhs, tree rhs)
{
  gcc_assert (parsing_constraint_expression_p ());
  tree overload;
  tree expr = build_x_binary_op (loc, code,
				 lhs, TREE_CODE (lhs),
				 rhs, TREE_CODE (rhs),
				 &overload, tf_none);
  if (expr == error_mark_node)
    return error_mark_node;
  SET_EXPR_LOCATION (expr, loc);
  return expr;
}

tree
finish_constraint_or_expr (location_t loc, tree lhs, tree rhs)
{
  return finish_constraint_binary_op (loc, TRUTH_ORIF_EXPR, lhs, rhs);
}

tree
finish_constraint_and_expr (location_t loc, tree lhs, tree rhs)
{
  return finish_constraint_binary_op (loc, TRUTH_ANDIF_EXPR, lhs, rhs);
}

tree
finish_constraint_primary_expr (location_t loc, tree expr)
{
  if (CAN_HAVE_LOCATION_P (expr) && !EXPR_HAS_LOCATION (expr))
    SET_EXPR_LOCATION (expr, loc);
  return expr;
}

/* Combine two constraint-expressions with a logical-and.  */

tree
combine_constraint_expressions (tree lhs, tree rhs)
{
  parsing_constraint_expression_sentinel pce;
  if (!lhs)
    return rhs;
  if (!rhs)
    return lhs;
  return finish_constraint_and_expr (input_location, lhs, rhs);
}

/* Extract the concept definition and template arguments from the concept
   check. Sets DEF to be the template declaration of the concept and
   ARGS to be template arguments of the check.  */

static void
unpack_concept_check (tree t, tree& def, tree& args)
{
  gcc_assert (concept_check_p (t));

  if (TREE_CODE (t) == CALL_EXPR)
    t = CALL_EXPR_FN (t);

  if (TREE_CODE (t) == TEMPLATE_ID_EXPR)
    {
      def = TREE_OPERAND (t, 0);

      /* T may have been the target of a call.  */
      if (TREE_CODE (def) == OVERLOAD)
	{
	  gcc_assert (!OVL_CHAIN (def));
	  def = OVL_FIRST (def);
	}

      args = TREE_OPERAND (t, 1);
    }
}

/* Returns true if any of the arguments in the template argument list is
   a wildcard or wildcard pack.  */

bool
contains_wildcard_p (tree args)
{
  for (int i = 0; i < TREE_VEC_LENGTH (args); ++i)
    {
      tree arg = TREE_VEC_ELT (args, i);
      if (TREE_CODE (arg) == WILDCARD_DECL)
	return true;
    }
  return false;
}

/*---------------------------------------------------------------------------
		    Resolution of qualified concept names
---------------------------------------------------------------------------*/

/* This facility is used to resolve constraint checks from requirement
   expressions. A constraint check is a call to a function template declared
   with the keyword 'concept'.

   The result of resolution is a pair (a TREE_LIST) whose value is the
   matched declaration, and whose purpose contains the coerced template
   arguments that can be substituted into the call.  */

/* Given an overload set OVL, try to find a unique definition that can be
   instantiated by the template arguments ARGS.

   This function is not called for arbitrary call expressions. In particular,
   the call expression must be written with explicit template arguments
   and no function arguments. For example:

        f<T, U>()

   If a single match is found, this returns a TREE_LIST whose VALUE
   is the constraint function (not the template), and its PURPOSE is
   the complete set of arguments substituted into the parameter list.  */

static tree
resolve_constraint_check (tree ovl, tree args)
{
  int nerrs = 0;
  tree cands = NULL_TREE;
  for (lkp_iterator iter (ovl); iter; ++iter)
    {
      tree tmpl = *iter;
      if (TREE_CODE (tmpl) != TEMPLATE_DECL)
	continue;

      /* Don't try to deduce checks for non-concepts. We often end up trying
         to resolve constraints in functional casts as part of a
         postfix-expression. We can save time and headaches by not
         instantiating those declarations.

         NOTE: This masks a potential error, caused by instantiating
         non-deduced contexts using placeholder arguments. */
      tree fn = DECL_TEMPLATE_RESULT (tmpl);
      if (DECL_ARGUMENTS (fn))
	continue;
      if (!DECL_DECLARED_CONCEPT_P (fn))
	continue;

      /* Remember the candidate if we can deduce a substitution.  */
      ++processing_template_decl;
      tree parms = TREE_VALUE (DECL_TEMPLATE_PARMS (tmpl));
      if (tree subst = coerce_template_parms (parms, args, tmpl))
	{
	  if (subst == error_mark_node)
	    ++nerrs;
	  else
	    cands = tree_cons (subst, fn, cands);
	}
      --processing_template_decl;
    }

  if (!cands)
    /* We either had no candidates or failed deductions.  */
    return nerrs ? error_mark_node : NULL_TREE;
  else if (TREE_CHAIN (cands))
    /* There are multiple candidates.  */
    return error_mark_node;

  return cands;
}

/* Determine if the the call expression CALL is a constraint check, and
   return the concept declaration and arguments being checked. If CALL
   does not denote a constraint check, return NULL.  */

tree
resolve_constraint_check (tree call)
{
  gcc_assert (TREE_CODE (call) == CALL_EXPR);

  /* A constraint check must be only a template-id expression. If it's a call
     to a base-link, its function(s) should be a template-id expression. If
     this is not a template-id, then it cannot be a concept-check.  */
  tree target = CALL_EXPR_FN (call);
  if (BASELINK_P (target))
    target = BASELINK_FUNCTIONS (target);
  if (TREE_CODE (target) != TEMPLATE_ID_EXPR)
    return NULL_TREE;

  // Get the overload set and template arguments and try to
  // resolve the target.
  tree ovl = TREE_OPERAND (target, 0);

  /* This is a function call of a variable concept... ill-formed. */
  if (TREE_CODE (ovl) == TEMPLATE_DECL)
    {
      error_at (location_of (call),
		"function call of variable concept %qE", call);
      return error_mark_node;
    }

  tree args = TREE_OPERAND (target, 1);
  return resolve_constraint_check (ovl, args);
}

/* Returns a pair containing instantiated arguments and the concept
   definition.  */

tree
resolve_concept_definition_check (tree id)
{
  gcc_assert (concept_definition_p (TREE_OPERAND (id, 0)));
  tree tmpl = TREE_OPERAND (id, 0);
  tree args = TREE_OPERAND (id, 1);
  tree parms = INNERMOST_TEMPLATE_PARMS (DECL_TEMPLATE_PARMS (tmpl));
  ++processing_template_decl;
  tree result = coerce_template_parms (parms, args, tmpl, tf_error);
  --processing_template_decl;
  if (result == error_mark_node)
    return error_mark_node;
  return build_tree_list (result, DECL_TEMPLATE_RESULT (tmpl));
}

/* Returns a pair containing the checked variable concept and its associated
   prototype parameter.  The result is a TREE_LIST whose TREE_VALUE is the
   variable concept and whose TREE_PURPOSE is the prototype parameter.  */

tree
resolve_variable_concept_check (tree id)
{
  tree tmpl = TREE_OPERAND (id, 0);
  tree args = TREE_OPERAND (id, 1);

  if (!variable_concept_p (tmpl))
    return NULL_TREE;

  /* Make sure that we have the right parameters before
     assuming that it works.  Note that failing to deduce
     will result in diagnostics.  */
  tree parms = INNERMOST_TEMPLATE_PARMS (DECL_TEMPLATE_PARMS (tmpl));
  ++processing_template_decl;
  tree result = coerce_template_parms (parms, args, tmpl);
  --processing_template_decl;
  if (result != error_mark_node)
    {
      tree decl = DECL_TEMPLATE_RESULT (tmpl);
      return build_tree_list (result, decl);
    }
  else
    return error_mark_node;
}

/* Returns a pair containing the checked concept and its associated
   prototype parameter. The result is a TREE_LIST whose TREE_VALUE
   is the concept (non-template) and whose TREE_PURPOSE contains
   the converted template arguments, including the deduced prototype
   parameter (in position 0). */

tree
resolve_concept_check (tree id)
{
  gcc_assert (concept_check_p (id));
  tree tmpl = TREE_OPERAND (id, 0);
  tree args = TREE_OPERAND (id, 1);
  tree parms = INNERMOST_TEMPLATE_PARMS (DECL_TEMPLATE_PARMS (tmpl));
  ++processing_template_decl;
  tree result = coerce_template_parms (parms, args, tmpl);
  --processing_template_decl;
  if (result == error_mark_node)
    return error_mark_node;
  return build_tree_list (result, DECL_TEMPLATE_RESULT (tmpl));
}

/* Given a call expression or template-id expression to a concept EXPR
   possibly including a wildcard, deduce the concept being checked and
   the prototype parameter. Returns true if the constraint and prototype
   can be deduced and false otherwise.  Note that the CHECK and PROTO
   arguments are set to NULL_TREE if this returns false.  */

bool
deduce_constrained_parameter (tree expr, tree& check, tree& proto)
{
  tree info = NULL_TREE;
  if (TREE_CODE (expr) == TEMPLATE_ID_EXPR)
    {
      if (concept_definition_p (TREE_OPERAND (expr, 0)))
	info = resolve_concept_check (expr);
      else
	info = resolve_variable_concept_check (expr);
    }
  else if (TREE_CODE (expr) == CALL_EXPR)
    info = resolve_constraint_check (expr);
  else
    gcc_unreachable ();

  if (info && info != error_mark_node)
    {
      check = TREE_VALUE (info);
      tree arg = TREE_VEC_ELT (TREE_PURPOSE (info), 0);
      if (ARGUMENT_PACK_P (arg))
	arg = TREE_VEC_ELT (ARGUMENT_PACK_ARGS (arg), 0);
      proto = TREE_TYPE (arg);
      return true;
    }

  check = proto = NULL_TREE;
  return false;
}

// Given a call expression or template-id expression to a concept, EXPR,
// deduce the concept being checked and return the template arguments.
// Returns NULL_TREE if deduction fails.
static tree
deduce_concept_introduction (tree expr)
{
  tree info = NULL_TREE;
  if (TREE_CODE (expr) == TEMPLATE_ID_EXPR)
    info = resolve_variable_concept_check (expr);
  else if (TREE_CODE (expr) == CALL_EXPR)
    info = resolve_constraint_check (expr);
  else
    gcc_unreachable ();

  if (info && info != error_mark_node)
    return TREE_PURPOSE (info);
  return NULL_TREE;
}

/*---------------------------------------------------------------------------
		       Expansion of concept definitions
---------------------------------------------------------------------------*/

/* Returns the expression of a function concept. */

static tree
get_returned_expression (tree fn)
{
  /* Extract the body of the function minus the return expression.  */
  tree body = DECL_SAVED_TREE (fn);
  if (!body)
    return error_mark_node;
  if (TREE_CODE (body) == BIND_EXPR)
    body = BIND_EXPR_BODY (body);
  if (TREE_CODE (body) != RETURN_EXPR)
    return error_mark_node;

  return TREE_OPERAND (body, 0);
}

/* Returns the initializer of a variable concept. */

static tree
get_variable_initializer (tree var)
{
  tree init = DECL_INITIAL (var);
  if (!init)
    return error_mark_node;
  return init;
}

/* Returns the definition of a variable or function concept.  */

static tree
get_concept_definition (tree decl)
{
  if (TREE_CODE (decl) == TEMPLATE_DECL)
    decl = DECL_TEMPLATE_RESULT (decl);

  if (TREE_CODE (decl) == CONCEPT_DECL)
    return DECL_INITIAL (decl);
  if (VAR_P (decl))
    return get_variable_initializer (decl);
  if (TREE_CODE (decl) == FUNCTION_DECL)
    return get_returned_expression (decl);
  gcc_unreachable ();
}

/*---------------------------------------------------------------------------
		Stepwise normalization of expressions

This set of functions will transform an expression into a constraint
in a sequence of steps.
---------------------------------------------------------------------------*/

static tree normalize_expression (tree, tree, subst_info);

/* Transform a logical-or or logical-and expression into either
   a conjunction or disjunction. */

static tree
normalize_logical_operation (tree t, tree args, tree_code c, subst_info info)
{
  tree t0 = normalize_expression (TREE_OPERAND (t, 0), args, info);
  tree t1 = normalize_expression (TREE_OPERAND (t, 1), args, info);
  return build_nt (c, t0, t1);
}

/* For a template-id referring to a variable concept, returns
   a check constraint. Otherwise, returns a predicate constraint. */

static tree
normalize_variable_concept_check (tree t, tree, subst_info)
{
  /* FIXME: This is never actually called. Remove this or merge it
     into normalize_concept_check. */
  gcc_assert (false);

  tree tmpl = TREE_OPERAND (t, 0);
  tree check;
  if (concept_definition_p (tmpl))
    check = resolve_concept_definition_check (t);
  else if (variable_template_p (tmpl))
    check = resolve_variable_concept_check (t);
  else
    {
      /* Check that we didn't refer to a function concept like a variable.  */
      tree fn = OVL_FIRST (TREE_OPERAND (t, 0));
      if (TREE_CODE (fn) == TEMPLATE_DECL
	  && DECL_DECLARED_CONCEPT_P (DECL_TEMPLATE_RESULT (fn)))
	{
	  error_at (location_of (t),
		    "invalid reference to function concept %qD", fn);
	  return error_mark_node;
	}
      return build_nt (ATOMIC_CONSTR, t);
    }
  gcc_assert (check);

  if (check == error_mark_node)
    {
      /* We get this when the template arguments don't match
	 the variable concept. */
      error_at (location_of (t), "invalid reference to concept %qE", t);
      return error_mark_node;
    }

   gcc_assert (false);
}

/* For a call expression to a function concept, returns a check
   constraint. Otherwise, returns a predicate constraint. */

static tree
normalize_function_concept_check (tree t, tree, subst_info info)
{
  /* This is likely never called, but it should be since we have
     to do overload resolution for function concepts (right?). */
  gcc_assert (false);

  /* Try to resolve this function call as a concept.  If not, then
     it can be returned as a predicate constraint.  */
  tree check = resolve_constraint_check (t);
  gcc_assert (check);
  if (check == error_mark_node)
    {
      /* TODO: Improve diagnostics. We could report why the reference
	 is invalid. */
      error ("invalid reference to concept %qE", t);
      return error_mark_node;
    }

  tree subst = TREE_PURPOSE (check);
  tree fn = TREE_VALUE (check);
  tree def = get_concept_definition (fn);

  return normalize_expression (def, subst, info);
}

static tree
normalize_concept_check (tree t, tree, subst_info info)
{
  tree tmpl;
  tree args;
  unpack_concept_check (t, tmpl, args);

  /* FIXME: Handle variable and function concept checks.  */

  tree parms = TREE_VALUE (DECL_TEMPLATE_PARMS (tmpl));
  tree subst = coerce_template_parms (parms, TREE_OPERAND (t, 1), tmpl);
  if (subst == error_mark_node)
    return error_mark_node;
  tree def = get_concept_definition (DECL_TEMPLATE_RESULT (tmpl));
  return normalize_expression (def, subst, info);
}

/* Associate each parameter in PARMS with its corresponding template
   argument in ARGS.  */

static tree
map_arguments (tree parms, tree args)
{
  for (tree p = parms; p; p = TREE_CHAIN (p))
    {
      int level;
      int index;
      template_parm_level_and_index (TREE_VALUE (p), &level, &index);
      TREE_PURPOSE (p) = TMPL_ARG (args, level, index);
    }
  return parms;
}

/* Build the parameter mapping for EXPR using ARGS.  */

static tree
build_parameter_mapping (tree expr, tree args)
{
  tree parms = find_template_parameters (expr);
  return map_arguments (parms, args);
}

/* The normal form of an atom depends on the expression. The normal
   form of a function call to a function concept is a check constraint
   for that concept. The normal form of a reference to a variable
   concept is a check constraint for that concept. Otherwise, the
   constraint is a predicate constraint.  */

static tree
normalize_atom (tree t, tree args, subst_info info)
{
  /* Concept checks are not atomic.  */
  if (concept_check_p (t))
    return normalize_concept_check (t, args, info);

  /* Build the parameter mapping for the atom by a) finding the template
     parameters involved in T, and b) finding and associating the template
     arguments that would provide their values in the current substitution.
     This becomes the "type" of the constraint.  */
  tree map = build_parameter_mapping (t, args);

  return build1 (ATOMIC_CONSTR, map, t);
}

/* Returns the normal form of an expression. */

static tree
normalize_expression (tree t, tree args, subst_info info)
{
  if (!t)
    return NULL_TREE;

  if (t == error_mark_node)
    return error_mark_node;

  switch (TREE_CODE (t))
    {
    case TRUTH_ANDIF_EXPR:
      return normalize_logical_operation (t, args, CONJ_CONSTR, info);
    case TRUTH_ORIF_EXPR:
      return normalize_logical_operation (t, args, DISJ_CONSTR, info);
    default:
      return normalize_atom (t, args, info);
    }
}

// -------------------------------------------------------------------------- //
// Constraint Semantic Processing
//
// The following functions are called by the parser and substitution rules
// to create and evaluate constraint-related nodes.

// The constraints associated with the current template parameters.
tree
current_template_constraints (void)
{
  if (!current_template_parms)
    return NULL_TREE;
  tree tmpl_constr = TEMPLATE_PARM_CONSTRAINTS (current_template_parms);
  return build_constraints (tmpl_constr, NULL_TREE);
}

// If the recently parsed TYPE declares or defines a template or template
// specialization, get its corresponding constraints from the current
// template parameters and bind them to TYPE's declaration.
tree
associate_classtype_constraints (tree type)
{
  if (!type || type == error_mark_node || TREE_CODE (type) != RECORD_TYPE)
    return type;

  // An explicit class template specialization has no template
  // parameters.
  if (!current_template_parms)
    return type;

  if (CLASSTYPE_IS_TEMPLATE (type) || CLASSTYPE_TEMPLATE_SPECIALIZATION (type))
    {
      tree decl = TYPE_STUB_DECL (type);
      tree ci = current_template_constraints ();

      // An implicitly instantiated member template declaration already
      // has associated constraints. If it is defined outside of its
      // class, then we need match these constraints against those of
      // original declaration.
      if (tree orig_ci = get_constraints (decl))
	{
	  if (!equivalent_constraints (ci, orig_ci))
	    {
	      // FIXME: Improve diagnostics.
	      error ("%qT does not match any declaration", type);
	      return error_mark_node;
	    }
	  return type;
	}
      set_constraints (decl, ci);
    }
  return type;
}

/* Create an empty constraint info block.  */

static inline tree_constraint_info*
build_constraint_info ()
{
  return (tree_constraint_info *)make_node (CONSTRAINT_INFO);
}

/* Build a constraint-info object that contains the associated constraints
   of a declaration.  This also includes the declaration's template
   requirements (TREQS) and any trailing requirements for a function
   declarator (DREQS).  Note that both TREQS and DREQS must be constraints.

   If the declaration has neither template nor declaration requirements
   this returns NULL_TREE, indicating an unconstrained declaration.  */

tree
build_constraints (tree tr, tree dr)
{
  if (!tr && !dr)
    return NULL_TREE;

  tree_constraint_info* ci = build_constraint_info ();
  ci->template_reqs = tr;
  ci->declarator_reqs = dr;
  ci->associated_constr = combine_constraint_expressions (tr, dr);

  return (tree)ci;
}

/* Returns the template-head requires clause for the template
   declaration T or NULL_TREE if none.  */

tree
get_template_head_requirements (tree t)
{
  tree ci = get_constraints (t);
  if (!ci)
    return NULL_TREE;
  return CI_TEMPLATE_REQS (ci);
}

/* Returns the trailing requires clause of the declarator of
   a template declaration T or NULL_TREE if none.  */

tree
get_trailing_function_requirements (tree t)
{
  tree ci = get_constraints (t);
  if (!ci)
    return NULL_TREE;
  return CI_DECLARATOR_REQS (ci);
}

/* Construct a sequence of template arguments by prepending
   ARG to REST. Either ARG or REST may be null. */
static tree
build_concept_check_arguments (tree arg, tree rest)
{
  gcc_assert (rest ? TREE_CODE (rest) == TREE_VEC : true);
  tree args;
  if (arg)
    {
      int n = rest ? TREE_VEC_LENGTH (rest) : 0;
      args = make_tree_vec (n + 1);
      TREE_VEC_ELT (args, 0) = arg;
      if (rest)
	for (int i = 0; i < n; ++i)
	  TREE_VEC_ELT (args, i + 1) = TREE_VEC_ELT (rest, i);
      int def = rest ? GET_NON_DEFAULT_TEMPLATE_ARGS_COUNT (rest) : 0;
      SET_NON_DEFAULT_TEMPLATE_ARGS_COUNT (args, def + 1);
    }
  else
    {
      gcc_assert (rest != NULL_TREE);
      args = rest;
    }
  return args;
}

/* Builds an id-expression of the form `C<Args...>()` where C is a function
   concept.  */

static tree
build_function_check(tree tmpl, tree args, tsubst_flags_t complain)
{
  /* FIXME: Don't use lookup template function. Just build
     the check directly.  */
  tree id = lookup_template_function (tmpl, args);

  ++processing_template_decl;
  vec<tree, va_gc> *fargs = make_tree_vector();
  tree call = finish_call_expr (id, &fargs, false, false, tf_none);
  release_tree_vector (fargs);
  --processing_template_decl;
  return call;
}

/* Builds an id-expression of the form `C<Args...>` where C is a variable
   concept.  */

static tree
build_variable_check(tree tmpl, tree args, tsubst_flags_t complain)
{
  gcc_assert (variable_concept_p (tmpl));
  gcc_assert (TREE_CODE (tmpl) == TEMPLATE_DECL);
  tree parms = INNERMOST_TEMPLATE_PARMS (DECL_TEMPLATE_PARMS (tmpl));
  args = coerce_template_parms (parms, args, tmpl, complain);
  if (args == error_mark_node)
    return error_mark_node;
  return build2 (TEMPLATE_ID_EXPR, boolean_type_node, tmpl, args);
}

/* Builds an id-expression of the form `C<Args...>` where C is a standard
   concept.  */

static tree
build_standard_check(tree tmpl, tree args, tsubst_flags_t complain)
{
  gcc_assert (standard_concept_p (tmpl));
  gcc_assert (TREE_CODE (tmpl) == TEMPLATE_DECL);
  tree parms = INNERMOST_TEMPLATE_PARMS (DECL_TEMPLATE_PARMS (tmpl));
  args = coerce_template_parms (parms, args, tmpl, complain);
  if (args == error_mark_node)
    return error_mark_node;
  return build2 (TEMPLATE_ID_EXPR, boolean_type_node, tmpl, args);
}

/* Construct an expression that checks TARGET using ARGS.  */

tree
build_concept_check (tree target, tree args, tsubst_flags_t complain)
{
  return build_concept_check (target, NULL_TREE, args, complain);
}

/* Construct an expression that checks the concept given by DECL. If
   concept_definition_p (DECL) is false, this returns null.  */

tree
build_concept_check (tree decl, tree arg, tree rest, tsubst_flags_t complain)
{
  tree args = build_concept_check_arguments (arg, rest);

  if (standard_concept_p (decl))
    return build_standard_check (decl, args, complain);
  if (variable_concept_p (decl))
    return build_variable_check (decl, args, complain);
  if (function_concept_p (decl))
    return build_function_check (decl, args, complain);

  return error_mark_node;
}


/* Returns a TYPE_DECL that contains sufficient information to
   build a template parameter of the same kind as PROTO and
   constrained by the concept declaration CNC.  Note that PROTO
   is the first template parameter of CNC.

   If specified, ARGS provides additional arguments to the
   constraint check.  */
tree
build_constrained_parameter (tree cnc, tree proto, tree args)
{
  tree name = DECL_NAME (cnc);
  tree type = TREE_TYPE (proto);
  tree decl = build_decl (input_location, TYPE_DECL, name, type);
  CONSTRAINED_PARM_PROTOTYPE (decl) = proto;
  CONSTRAINED_PARM_CONCEPT (decl) = cnc;
  CONSTRAINED_PARM_EXTRA_ARGS (decl) = args;
  return decl;
}

/* Create a constraint expression for the given DECL that evaluates the
   requirements specified by CONSTR, a TYPE_DECL that contains all the
   information necessary to build the requirements (see finish_concept_name
   for the layout of that TYPE_DECL).

   Note that the constraints are neither reduced nor decomposed. That is
   done only after the requires clause has been parsed (or not).  */
tree
finish_shorthand_constraint (tree decl, tree constr)
{
  /* No requirements means no constraints.  */
  if (!constr)
    return NULL_TREE;

  if (error_operand_p (constr))
    return NULL_TREE;

  tree proto = CONSTRAINED_PARM_PROTOTYPE (constr);
  tree con = CONSTRAINED_PARM_CONCEPT (constr);
  tree args = CONSTRAINED_PARM_EXTRA_ARGS (constr);

  /* If the parameter declaration is variadic, but the concept
     is not then we need to apply the concept to every element
     in the pack.  */
  bool variadic_concept_p = template_parameter_pack_p (proto);
  bool declared_pack_p = template_parameter_pack_p (decl);
  bool apply_to_all_p = declared_pack_p && !variadic_concept_p;

  /* Get the argument and overload used for the requirement
     and adjust it if we're going to expand later.  */
  tree arg = template_parm_to_arg (build_tree_list (NULL_TREE, decl));
  if (apply_to_all_p)
    arg = PACK_EXPANSION_PATTERN (TREE_VEC_ELT (ARGUMENT_PACK_ARGS (arg), 0));

  /* Build the concept constraint-expression.  */
  tree tmpl = DECL_TI_TEMPLATE (con);
  tree check = tmpl;
  if (TREE_CODE (con) == FUNCTION_DECL)
    check = ovl_make (tmpl);
  check = build_concept_check (check, arg, args, tf_warning_or_error);

  /* Make the check a fold-expression if needed.  */
  if (apply_to_all_p)
    check = finish_left_unary_fold_expr (check, TRUTH_ANDIF_EXPR);

  return check;
}

/* Returns a conjunction of shorthand requirements for the template
   parameter list PARMS. Note that the requirements are stored in
   the TYPE of each tree node. */

tree
get_shorthand_constraints (tree parms)
{
  tree result = NULL_TREE;
  parms = INNERMOST_TEMPLATE_PARMS (parms);
  for (int i = 0; i < TREE_VEC_LENGTH (parms); ++i)
    {
      tree parm = TREE_VEC_ELT (parms, i);
      tree constr = TEMPLATE_PARM_CONSTRAINTS (parm);
      result = combine_constraint_expressions (result, constr);
    }
  return result;
}

/* Get the deduced wildcard from a DEDUCED placeholder.  If the deduced
   wildcard is a pack, return the first argument of that pack.  */

static tree
get_deduced_wildcard(tree wildcard)
{
  if (ARGUMENT_PACK_P (wildcard))
    wildcard = TREE_VEC_ELT (ARGUMENT_PACK_ARGS (wildcard), 0);
  gcc_assert (TREE_CODE (wildcard) == WILDCARD_DECL);
  return wildcard;
}

/* Returns the prototype parameter for the nth deduced wildcard.  */

static tree
get_introdcution_prototype(tree wildcards, int index)
{
  return TREE_TYPE (get_deduced_wildcard (TREE_VEC_ELT (wildcards, index)));
}

/* Introduce a type template parameter.  */

static tree
introduce_type_template_parameter(tree wildcard, bool& non_type_p)
{
  non_type_p = false;
  return finish_template_type_parm (class_type_node, DECL_NAME (wildcard));
}

/* Introduce a template template parameter.  */

static tree
introduce_template_template_parameter(tree wildcard, bool& non_type_p)
{
  non_type_p = false;
  begin_template_parm_list ();
  current_template_parms = DECL_TEMPLATE_PARMS (TREE_TYPE (wildcard));
  end_template_parm_list ();
  return finish_template_template_parm (class_type_node, DECL_NAME (wildcard));
}

/* Introduce a template non-type parameter.  */

static tree
introduce_nontype_template_parameter(tree wildcard, bool& non_type_p)
{
  non_type_p = true;
  tree parm = copy_decl (TREE_TYPE (wildcard));
  DECL_NAME (parm) = DECL_NAME (wildcard);
  return parm;
}

/* Introduce a single template parameter.  */

static tree
build_introduced_template_parameter(tree wildcard, bool& non_type_p)
{
  tree proto = TREE_TYPE (wildcard);

  tree parm;
  if (TREE_CODE (proto) == TYPE_DECL)
    parm = introduce_type_template_parameter (wildcard, non_type_p);
  else if (TREE_CODE (proto) == TEMPLATE_DECL)
    parm = introduce_template_template_parameter (wildcard, non_type_p);
  else
    parm = introduce_nontype_template_parameter (wildcard, non_type_p);

  /* Wrap in a TREE_LIST for process_template_parm. Note that introduced
     parameters do not retain the defaults from the source parameter.  */
  return build_tree_list (NULL_TREE, parm);
}

/* Introduce a single template parameter.  */

static tree
introduce_template_parameter(tree parms, tree wildcard)
{
  gcc_assert (!ARGUMENT_PACK_P (wildcard));
  tree proto = TREE_TYPE (wildcard);
  location_t loc = DECL_SOURCE_LOCATION (wildcard);

  /* Diagnose the case where we have C{...Args}.  */
  if (WILDCARD_PACK_P (wildcard))
    {
      tree id = DECL_NAME (wildcard);
      error_at (loc, "%qE cannot be introduced with an ellipsis %<...%>", id);
      inform(DECL_SOURCE_LOCATION (proto), "prototype declared here");
    }

  bool non_type_p;
  tree parm = build_introduced_template_parameter (wildcard, non_type_p);
  return process_template_parm (parms, loc, parm, non_type_p, false);
}

/* Introduce a template parameter pack.  */

static tree
introduce_template_parameter_pack(tree parms, tree wildcard)
{
  bool non_type_p;
  tree parm = build_introduced_template_parameter (wildcard, non_type_p);
  location_t loc = DECL_SOURCE_LOCATION (wildcard);
  return process_template_parm (parms, loc, parm, non_type_p, true);
}

/* Introduce the nth template parameter.  */

static tree
introduce_template_parameter(tree parms, tree wildcards, int& index)
{
  tree deduced = TREE_VEC_ELT (wildcards, index++);
  return introduce_template_parameter (parms, deduced);
}

/* Introduce either a template parameter pack or a list of template
   parameters.  */

static tree
introduce_template_parameters(tree parms, tree wildcards, int& index)
{
  /* If the prototype was a parameter, we better have deduced an
     argument pack, and that argument must be the last deduced value
     in the wildcard vector.  */
  tree deduced = TREE_VEC_ELT (wildcards, index++);
  gcc_assert (ARGUMENT_PACK_P (deduced));
  gcc_assert (index == TREE_VEC_LENGTH (wildcards));

  /* Introduce each element in the pack.  */
  tree args = ARGUMENT_PACK_ARGS (deduced);
  for (int i = 0; i < TREE_VEC_LENGTH (args); ++i)
    {
      tree arg = TREE_VEC_ELT (args, i);
      if (WILDCARD_PACK_P (arg))
	parms = introduce_template_parameter_pack (parms, arg);
      else
      	parms = introduce_template_parameter (parms, arg);
    }

  return parms;
}

/* Builds the template parameter list PARMS by chaining introduced
   parameters from the WILDCARD vector.  INDEX is the position of
   the current parameter.  */

static tree
process_introduction_parms (tree parms, tree wildcards, int& index)
{
  tree proto = get_introdcution_prototype (wildcards, index);
  if (template_parameter_pack_p (proto))
    return introduce_template_parameters (parms, wildcards, index);
  else
    return introduce_template_parameter (parms, wildcards, index);
}

/* Associates a constraint check to the current template based on the
   introduction parameters.  INTRO_LIST must be a TREE_VEC of WILDCARD_DECLs
   containing a chained PARM_DECL which contains the identifier as well as
   the source location. TMPL_DECL is the decl for the concept being used.
   If we take a concept, C, this will form a check in the form of
   C<INTRO_LIST> filling in any extra arguments needed by the defaults
   deduced.

   Returns NULL_TREE if no concept could be matched and error_mark_node if
   an error occurred when matching.  */
tree
finish_template_introduction (tree tmpl_decl, tree intro_list)
{
  /* Build a concept check to deduce the actual parameters.  */
  tree expr = build_concept_check (tmpl_decl, intro_list, tf_none);
  if (expr == error_mark_node)
    {
      /* FIXME: The source location is wrong.  */
      error_at (input_location, "cannot deduce template parameters from "
				"introduction list");
      return error_mark_node;
    }
  tree parms = deduce_concept_introduction (expr);
  if (!parms)
    return NULL_TREE;

  /* Build template parameter scope for introduction.  */
  tree parm_list = NULL_TREE;
  begin_template_parm_list ();
  int nargs = MIN (TREE_VEC_LENGTH (parms), TREE_VEC_LENGTH (intro_list));
  for (int n = 0; n < nargs; )
    parm_list = process_introduction_parms (parm_list, parms, n);
  parm_list = end_template_parm_list (parm_list);

  /* Update the number of arguments to reflect the number of introductions.  */
  nargs = TREE_VEC_LENGTH (parm_list);

  /* Determine if any errors occurred during matching.  */
  for (int i = 0; i < TREE_VEC_LENGTH (parm_list); ++i)
    if (TREE_VALUE (TREE_VEC_ELT (parm_list, i)) == error_mark_node)
      {
	end_template_decl ();
	return error_mark_node;
      }

  /* Build a concept check for our constraint.  */
  tree check_args = make_tree_vec (nargs);
  int n = 0;
  for (; n < TREE_VEC_LENGTH (parm_list); ++n)
    {
      tree parm = TREE_VEC_ELT (parm_list, n);
      TREE_VEC_ELT (check_args, n) = template_parm_to_arg (parm);
    }
  SET_NON_DEFAULT_TEMPLATE_ARGS_COUNT (check_args, n);

  /* If the template expects more parameters we should be able
     to use the defaults from our deduced concept.  */
  for (; n < TREE_VEC_LENGTH (parms); ++n)
    TREE_VEC_ELT (check_args, n) = TREE_VEC_ELT (parms, n);

  /* Associate the constraint.  */
  tree check = build_concept_check (tmpl_decl,
  				    check_args,
  				    tf_warning_or_error);
  TEMPLATE_PARMS_CONSTRAINTS (current_template_parms) = check;

  return parm_list;
}


/* Given the predicate constraint T from a constrained-type-specifier, extract
   its TMPL and ARGS.  FIXME why do we need two different forms of
   constrained-type-specifier?  */

void
placeholder_extract_concept_and_args (tree t, tree &tmpl, tree &args)
{
  if (TREE_CODE (t) == TEMPLATE_ID_EXPR)
    {
      tmpl = TREE_OPERAND (t, 0);
      args = TREE_OPERAND (t, 1);
      return;
    }

  if (TREE_CODE (t) == TYPE_DECL)
    {
      /* A constrained parameter.  Build a constraint check
	 based on the prototype parameter and then extract the
	 arguments from that.  */
      tree proto = CONSTRAINED_PARM_PROTOTYPE (t);
      tree check = finish_shorthand_constraint (proto, t);
      placeholder_extract_concept_and_args (check, tmpl, args);
      return;
    }
}

/* Returns true iff the placeholders C1 and C2 are equivalent.  C1
   and C2 can be either TEMPLATE_TYPE_PARM or template-ids.  */

bool
equivalent_placeholder_constraints (tree c1, tree c2)
{
  if (c1 && TREE_CODE (c1) == TEMPLATE_TYPE_PARM)
    /* A constrained auto.  */
    c1 = PLACEHOLDER_TYPE_CONSTRAINTS (c1);
  if (c2 && TREE_CODE (c2) == TEMPLATE_TYPE_PARM)
    c2 = PLACEHOLDER_TYPE_CONSTRAINTS (c2);

  if (c1 == c2)
    return true;
  if (!c1 || !c2)
    return false;
  if (c1 == error_mark_node || c2 == error_mark_node)
    /* We get here during satisfaction; when a deduction constraint
       fails, substitution can produce an error_mark_node for the
       placeholder constraints.  */
    return false;

  tree t1, t2, a1, a2;
  placeholder_extract_concept_and_args (c1, t1, a1);
  placeholder_extract_concept_and_args (c2, t2, a2);

  if (t1 != t2)
    return false;

  int len1 = TREE_VEC_LENGTH (a1);
  int len2 = TREE_VEC_LENGTH (a2);
  if (len1 != len2)
    return false;

  /* Skip the first argument so we don't infinitely recurse.
     Also, they may differ in template parameter index.  */
  for (int i = 1; i < len1; ++i)
    {
      tree t1 = TREE_VEC_ELT (a1, i);
      tree t2 = TREE_VEC_ELT (a2, i);
      if (!template_args_equal (t1, t2))
      return false;
    }
  return true;
}

/* Return a hash value for the placeholder ATOMIC_CONSTR C.  */

hashval_t
hash_placeholder_constraint (tree c)
{
  tree t, a;
  placeholder_extract_concept_and_args (c, t, a);

  /* Like hash_tmpl_and_args, but skip the first argument.  */
  hashval_t val = iterative_hash_object (DECL_UID (t), 0);

  for (int i = TREE_VEC_LENGTH (a)-1; i > 0; --i)
    val = iterative_hash_template_arg (TREE_VEC_ELT (a, i), val);

  return val;
}

/* Substitute through the simple requirement.  */

static tree
tsubst_valid_expression_requirement (tree t, tree args, subst_info info)
{
  /* Don't diagnose access checks immediately.  */
  deferring_access_check_sentinel acs (dk_no_deferred);

  return tsubst_expr (t, args, info.complain, info.in_decl, false);
}


/* Substitute through the simple requirement.  */

static tree
tsubst_simple_requirement (tree t, tree args, subst_info info)
{
  tree t0 = TREE_OPERAND (t, 0);
  tree expr = tsubst_valid_expression_requirement (t0, args, info);
  if (expr == error_mark_node)
    return error_mark_node;
  return finish_simple_requirement (expr);
}

/* Substitute through the type requirement.  */

static tree
tsubst_type_requirement (tree t, tree args, subst_info info)
{
  /* Don't diagnose access checks immediately.  */
  deferring_access_check_sentinel acs (dk_no_deferred);

  tree t0 = TREE_OPERAND (t, 0);
  tree type = tsubst (t0, args, info.complain, info.in_decl);
  if (type == error_mark_node)
    return error_mark_node;
  return finish_type_requirement (type);
}

/* True if TYPE can be deduced from EXPR.

   FIXME: This doesn't appear to support generalized auto.

   FIXME: Perform decltype deduction, not template argument deduction.  */

static bool
type_deducible_p (tree expr, tree type, tree placeholder, tree args,
                  subst_info info)
{
  /* Replace the constraints with the instantiated constraints.  */
  tree saved_constr = PLACEHOLDER_TYPE_CONSTRAINTS (placeholder);
  PLACEHOLDER_TYPE_CONSTRAINTS (placeholder)
    = tsubst_expr (saved_constr,
		   args,
		   info.complain | tf_partial,
		   info.in_decl,
		   false);

  /* Unlink the canonical type.  */
  tree saved_type = TYPE_CANONICAL (placeholder);
  TYPE_CANONICAL (placeholder) = NULL_TREE;

  tree deduced_type
    = do_auto_deduction (type,
			 expr,
			 placeholder,
			 info.complain,
			 adc_requirement);

  PLACEHOLDER_TYPE_CONSTRAINTS (placeholder) = saved_constr;
  TYPE_CANONICAL (placeholder) = saved_type;

  if (deduced_type == error_mark_node)
    return false;

  return true;
}

/* True if EXPR can not be converted to TYPE.  */

static bool
expression_convertible_p (tree expr, tree type, subst_info info)
{
  tree conv =
    perform_direct_initialization_if_possible (type, expr, false, info.complain);
  if (conv == error_mark_node)
    return false;
  if (conv == NULL_TREE)
    {
      if (info.complain & tf_error)
        {
          location_t loc = EXPR_LOC_OR_LOC (expr, input_location);
          error_at (loc, "cannot convert %qE to %qT", expr, type);
        }
      return false;
    }
  return true;
}


/* Substitute through the compound requirement.  */

static tree
tsubst_compound_requirement (tree t, tree args, subst_info info)
{
  tree t0 = TREE_OPERAND (t, 0);
  tree t1 = TREE_OPERAND (t, 1);
  tree expr = tsubst_valid_expression_requirement (t0, args, info);
  if (expr == error_mark_node)
    return error_mark_node;

  /* Check the noexcept condition.  */
  bool noexcept_p = COMPOUND_REQ_NOEXCEPT_P (t);
  if (noexcept_p && !expr_noexcept_p (expr, tf_none))
    return error_mark_node;

  tree type = tsubst (t1, args, info.complain, info.in_decl);
  if (type == error_mark_node)
    return error_mark_node;

  /* Check expression against the result type.  */
  if (type)
    {
      if (tree placeholder = type_uses_auto (type))
	{
	  if (!type_deducible_p (expr, type, placeholder, args, info))
	    return error_mark_node;
	}
      else if (!expression_convertible_p (expr, type, info))
	return error_mark_node;
    }

  return finish_compound_requirement (expr, type, noexcept_p);
}

static tree
tsubst_nested_requirement (tree t, tree args, subst_info info)
{
  tree t0 = TREE_OPERAND (t, 0);
  tree expr = tsubst_expr (t0, args, info.complain, info.in_decl, false);
  if (expr == error_mark_node)
    return error_mark_node;

  return finish_nested_requirement (expr);
}

/* Substitute ARGS into the requirement T.  */

static tree
tsubst_requirement (tree t, tree args, subst_info info)
{
  switch (TREE_CODE (t))
    {
    case SIMPLE_REQ:
      return tsubst_simple_requirement (t, args, info);
    case TYPE_REQ:
      return tsubst_type_requirement (t, args, info);
    case COMPOUND_REQ:
      return tsubst_compound_requirement (t, args, info);
    case NESTED_REQ:
      return tsubst_nested_requirement (t, args, info);
    default:
      break;
    }
  gcc_unreachable ();
}

/* Substitute ARGS into the list of requirements T. Note that
   substitution failures here result in ill-formed programs. */

static tree
tsubst_requirement_body (tree t, tree args, subst_info info)
{
  tree result = NULL_TREE;
  while (t)
    {
      tree req = tsubst_requirement (TREE_VALUE (t), args, info);
      if (req == error_mark_node)
	return error_mark_node;
      result = tree_cons (NULL_TREE, req, result);
      t = TREE_CHAIN (t);
    }
  return nreverse (result);
}

static tree
declare_constraint_vars (tree parms, tree vars)
{
  tree s = vars;
  for (tree t = parms; t; t = DECL_CHAIN (t))
    {
      if (DECL_PACK_P (t))
       {
         tree pack = extract_fnparm_pack (t, &s);
         register_local_specialization (pack, t);
       }
      else
       {
         register_local_specialization (s, t);
         s = DECL_CHAIN (s);
       }
    }
  return vars;
}

/* A subroutine of tsubst_parameterized_constraint. Substitute ARGS
   into the parameter list T, producing a sequence of constraint
   variables, declared in the current scope.

   Note that the caller must establish a local specialization stack
   prior to calling this function since this substitution will
   declare the substituted parameters. */

static tree
tsubst_constraint_variables (tree t, tree args, subst_info info)
{
  /* Clear cp_unevaluated_operand across tsubst so that we get a proper chain
     of PARM_DECLs.  */
  int saved_unevaluated_operand = cp_unevaluated_operand;
  cp_unevaluated_operand = 0;
  tree vars = tsubst (t, args, info.complain, info.in_decl);
  cp_unevaluated_operand = saved_unevaluated_operand;
  if (vars == error_mark_node)
    return error_mark_node;
  return declare_constraint_vars (t, vars);
}

/* Substitute ARGS into the requires-expression T. [8.4.7]p6. The
   substitution of template arguments into a requires-expression
   may result in the formation of invalid types or expressions
   in its requirements ... In such cases, the expression evaluates
   to false; it does not cause the program to be ill-formed.

   However, there are cases where substitution must produce a
   new requires-expression, that is not a template constraint.
   For example:

        template<typename T>
        class X {
          template<typename U>
          static constexpr bool var = requires (U u) { T::fn(u); };
        };

   In the instantiation of X<Y> (assuming Y defines fn), then the
   instantiated requires-expression would include Y::fn(u). If any
   substitution in the requires-expression fails, we can immediately
   fold the expression to false, as would be the case e.g., when
   instantiation X<int>.  */

tree
tsubst_requires_expr (tree t, tree args,
		      tsubst_flags_t complain, tree in_decl)
{
  local_specialization_stack stack;

  subst_info info(complain, in_decl);

  tree parms = TREE_OPERAND (t, 0);
  if (parms)
    {
      parms = tsubst_constraint_variables (parms, args, info);
      if (parms == error_mark_node)
	return boolean_false_node;
    }

  tree reqs = TREE_OPERAND (t, 1);
  reqs = tsubst_requirement_body (reqs, args, info);
  if (reqs == error_mark_node)
    return boolean_false_node;

  /* In certain cases, produce a new requires-expression.
     Otherwise the value of the expression is true.  */
  if (processing_template_decl && uses_template_parms (args))
    return finish_requires_expr (parms, reqs);

  return boolean_true_node;
}

/* Substitute ARGS into the constraint information CI, producing a new
   constraint record.  */

tree
tsubst_constraint_info (tree t, tree args,
			tsubst_flags_t complain, tree in_decl)
{
  if (!t || t == error_mark_node || !check_constraint_info (t))
    return NULL_TREE;

  tree tr = tsubst_expr (CI_TEMPLATE_REQS (t), args, complain, in_decl, true);
  tree dr = tsubst_expr (CI_DECLARATOR_REQS (t), args, complain, in_decl, true);
  return build_constraints (tr, dr);
}

/*---------------------------------------------------------------------------
			Constraint satisfaction
---------------------------------------------------------------------------*/

static int satisfying_constraint = 0;

/* Returns true if we are currently satisfying a constraint.

   This is used to guard against recursive calls to evaluate_concept_check
   during template argument substitution.

   TODO: Do we get equivalent behavior if we allow substitution to
   recursively invoke satisfaction for concept checks?  */

bool
satisfying_constraint_p ()
{
  return satisfying_constraint;
}

static tree satisfy_expression (tree, tree, subst_info info);

/* Compute the satisfaction of a conjunction.  */

static tree
satisfy_conjunction (tree expr, tree args, subst_info info)
{
  tree lhs = satisfy_expression (TREE_OPERAND (expr, 0), args, info);
  if (lhs == error_mark_node || lhs == boolean_false_node)
    return lhs;
  return satisfy_expression (TREE_OPERAND (expr, 1), args, info);
}

/* Compute the satisfaction of a disjunction.  */

static tree
satisfy_disjunction (tree expr, tree args, subst_info info)
{
  tree lhs = satisfy_expression (TREE_OPERAND (expr, 0), args, info);
  if (lhs == boolean_true_node)
    return boolean_true_node;
  return satisfy_expression (TREE_OPERAND (expr, 1), args, info);
}

/* Compute the satisfaction of an concept check constraint. Note that
   EXPR is a substituted template-id whose arguments will become the
   innermost arguments for the recursive substitution into the
   definition of named concept.  */

static tree
satisfy_check (tree expr, tree args, subst_info info)
{
  /* Unpack the id-expression from a function concept check.  */
  /* FIXME: Handle any kind of concept check.  */
  gcc_assert (TREE_CODE (expr) != CALL_EXPR);
  if (TREE_CODE (expr) == CALL_EXPR)
    expr = CALL_EXPR_FN (expr);

  /* Apply the parameter mapping (i.e., substitute), if needed.  */
  ++satisfying_constraint;
  expr = tsubst_expr (expr, args, info.complain, info.in_decl, false);
  --satisfying_constraint;
  if (expr == error_mark_node)
    return error_mark_node;

  gcc_assert (TREE_CODE (expr) == TEMPLATE_ID_EXPR);
  tree tmpl = TREE_OPERAND (expr, 0);
  tree targs = TREE_OPERAND (expr, 1);

  /* Reconstitute the parameter mapping by making the innermost arguments
     those instantiated with the concept check.  */
  int n = TREE_VEC_LENGTH (args);
  tree subst = make_tree_vec (n);
  for (int i = 2; i <= n; ++i)
    SET_TMPL_ARGS_LEVEL (subst, i, TMPL_ARGS_LEVEL (args, i));
  SET_TMPL_ARGS_LEVEL (subst, 1, targs);

  gcc_assert (TREE_CODE (tmpl) == TEMPLATE_DECL);
  tree def = get_concept_definition (tmpl);

  info.in_decl = tmpl;
  return satisfy_expression (def, subst, info);
}

/* Ensures that T is a truth value and not (accidentally, as sometimes
   happens) an integer value.  */

static tree
satisfaction_value (tree t)
{
  if (t == error_mark_node)
    return t;
  if (t == boolean_true_node || t == integer_one_node)
    return t;
  if (t == boolean_false_node || t == integer_zero_node)
    return t;

  /* Anything else should be invalid.  */
  gcc_assert (false);
}

/* Compute the satisfaction of an atomic constraint.  */

static tree
satisfy_atom (tree expr, tree args, subst_info info)
{
  /* Handle concept checks differently than normal atoms.  */
  if (concept_check_p (expr))
    return satisfy_check (expr, args, info);

  /* Apply the parameter mapping (i.e., just substitute).  */
  tree result = tsubst_expr (expr, args, info.complain, info.in_decl, false);
  if (result == error_mark_node)
    return error_mark_node;

  /* [17.4.1.2] ... lvalue-to-value conversion is performed as necessary,
     and EXPR shall be a constant expression of type bool.  */
  result = force_rvalue (result, tf_error);
  if (result == error_mark_node)
    return error_mark_node;
  if (cv_unqualified (TREE_TYPE (result)) != boolean_type_node)
    return error_mark_node;

  /* If we already have value, don't bother folding.  */
  if (result == boolean_true_node || result == boolean_false_node)
    return result;

  /* Compute the value of the constraint.  */
  result = cxx_constant_value (result);
  return satisfaction_value(result);
}

/* Determine if the expression T, when normalized, is satisfied.
   Returns boolean_true_node if the expression/constraint is
   satisfied and boolean_false_node otherwise.

   True normalization is not explicitly required here; we can
   simply decompose the expression as normalizing the constraint.
   The earlier, TS-based implementation normalized prior to
   satisfaction, which could produce substitution failures in
   cases where they might have been short-circuited. In this
   implementation, we only substitute into atomic constraints.

   Note that we need to propagate a multi-level template
   argument list through the satisfaction algorithm. That the
   list is multi-leveled allows us to fully defer substitutions
   through constraints of constrained members.

   The parameter mapping of atomic constraints is simply the
   set of template arguments that will be substituted into
   the expression, regardless of template parameters appearing
   withing. We could make this explicit, but it doesn't seem
   to offer any real benefit.  */

static tree
satisfy_expression (tree expr, tree args, subst_info info)
{
  /* Sometimes an invalid parse yields a null expression. Interpret that
     as false, just so we don't ICE.  */
  if (!expr)
    return boolean_false_node;

  switch (TREE_CODE (expr))
    {
      case TRUTH_ANDIF_EXPR:
	return satisfy_conjunction (expr, args, info);
      case TRUTH_ORIF_EXPR:
	return satisfy_disjunction (expr, args, info);
      default:
	return satisfy_atom (expr, args, info);
    }
}

/* Check that the constraint is satisfied.  */

static tree
satisfy_constraint (tree t, tree args)
{
  auto_timevar time (TV_CONSTRAINT_SAT);

  /* Turn off template processing. Constraint satisfaction only applies
     to non-dependent terms, so we want to ensure full checking here.  */
  processing_template_decl_sentinel proc (true);

  /* Avoid early exit in tsubst and tsubst_copy from null args; since earlier
     substitution was done with processing_template_decl forced on, there will
     be expressions that still need semantic processing, possibly buried in
     decltype or a template argument.  */
  if (args == NULL_TREE)
    args = make_tree_vec (1);

  subst_info info (tf_none, NULL_TREE);
  return satisfy_expression (t, args, info);
}

/* Check the associated constraints in CI against the given
   ARGS, returning true when the constraints are satisfied
   and false otherwise.  */

static tree
satisfy_associated_constraints (tree ci, tree args)
{
  /* If there are no constraints then this is trivially satisfied. */
  if (!ci)
    return boolean_true_node;

  /* If any arguments depend on template parameters, we can't
     check constraints.

     FIXME: We should never be asking for the satisfaction of
     associated constraints with dependent template arguments.  */
  if (args && uses_template_parms (args))
    return boolean_true_node;

  return satisfy_constraint (CI_ASSOCIATED_CONSTRAINTS (ci), args);

#if 0
  /* Check if we've seen a previous result. */
  if (tree prev = lookup_constraint_satisfaction (ci, args))
    return prev;

  /* Actually test for satisfaction. */
  tree result = satisfy_constraint (CI_ASSOCIATED_CONSTRAINTS (ci), args);
  return memoize_constraint_satisfaction (ci, args, result);
#endif
}

/* Evaluate the given constraint, returning boolean_true_node if the
   constraint is satisfied and boolean_false_node otherwise. */

tree
evaluate_constraints (tree constr, tree args)
{
  return satisfy_constraint (constr, args);
}

/* Evaluate a concept check of the form C<ARGS>, returning either TRUE
   or FALSE. If ARGS contains any template parameters, this returns the
   check. If satisfaction yields a hard error, diagnose the error.  */

tree
evaluate_concept_check (tree check)
{
  if (check == error_mark_node)
    return error_mark_node;

  gcc_assert (TREE_CODE (check) == TEMPLATE_ID_EXPR);

  /* If the arguments are dependent in any way, preserve the check.  */
  if (uses_template_parms (TREE_OPERAND (check, 1)))
    return check;

  tree result = satisfy_constraint (check, NULL_TREE);
  if (result == error_mark_node)
    {
      /* FIXME: Improve the diagnostic by replaying it?  */
      location_t loc = EXPR_LOC_OR_LOC (check, input_location);
      error_at (loc, "concept satisfaction failed");
    }

  return result;
}

/* Returns true if C<ARGS> is satisfied and false otherwise. This also
   handles cases where C is a variable or function concept.  */

tree
evaluate_concept (tree c, tree args)
{
  if (variable_concept_p (c))
    return evaluate_variable_concept (c, args);
  if (function_concept_p (c))
    return evaluate_function_concept (c, args);

  tree t = build_nt (TEMPLATE_ID_EXPR, c, args);
  return evaluate_concept_check (t);
}

/* Evaluate the function concept FN by substituting its own args
   into its definition and evaluating that as the result. Returns
   boolean_true_node if the constraints are satisfied and
   boolean_false_node otherwise.  */

tree
evaluate_function_concept (tree fn, tree args)
{
  tree t = build_nt (TEMPLATE_ID_EXPR, DECL_TI_TEMPLATE (fn), args);
  return evaluate_concept_check (t);
}

/* Evaluate the variable concept VAR by substituting its own args into
   its initializer and checking the resulting constraint. Returns
   boolean_true_node if the constraints are satisfied and
   boolean_false_node otherwise.  */

tree
evaluate_variable_concept (tree var, tree args)
{
  tree t = build_nt (TEMPLATE_ID_EXPR, DECL_TI_TEMPLATE (var), args);
  return evaluate_concept_check (t);
}

/* Evaluate EXPR as a constraint expression using ARGS.  */

tree
evaluate_constraint_expression (tree expr, tree args)
{
  return satisfy_constraint (expr, args);
}

/* Evaluate EXPR as a constraint.  */

tree
evaluate_constraint_expression (tree expr)
{
  return satisfy_constraint (expr, NULL_TREE);
}

/* Returns true if the DECL's constraints are satisfied.
   This is used in cases where a declaration is formed but
   before it is used (e.g., overload resolution). */

bool
constraints_satisfied_p (tree decl)
{
  /* Get the constraints to check for satisfaction. This depends
     on whether we're looking at a template specialization or not. */
  tree ci;
  tree args = NULL_TREE;
  if (tree ti = DECL_TEMPLATE_INFO (decl))
    {
      tree tmpl = TI_TEMPLATE (ti);
      ci = get_constraints (tmpl);

      /* The initial parameter mapping is the complete set of
         template arguments substituted into the declaration.  */
      args = TI_ARGS (ti);
    }
  else
    {
      ci = get_constraints (decl);
    }

  tree eval = satisfy_associated_constraints (ci, args);
  return eval == boolean_true_node;
}

/* Returns true if the constraints are satisfied by ARGS.
   Here, T can be either a constraint or a constrained
   declaration.  */

bool
constraints_satisfied_p (tree t, tree args)
{
  tree eval;
  if (DECL_P (t))
    eval = satisfy_associated_constraints (get_constraints (t), args);
  else
    eval = evaluate_constraints (t, args);
  return eval == boolean_true_node;
}

/*---------------------------------------------------------------------------
		Semantic analysis of requires-expressions
---------------------------------------------------------------------------*/

/* Finish a requires expression for the given PARMS (possibly
   null) and the non-empty sequence of requirements.  */

tree
finish_requires_expr (tree parms, tree reqs)
{
  /* Modify the declared parameters by removing their context
     so they don't refer to the enclosing scope and explicitly
     indicating that they are constraint variables. */
  for (tree parm = parms; parm; parm = DECL_CHAIN (parm))
    {
      DECL_CONTEXT (parm) = NULL_TREE;
      CONSTRAINT_VAR_P (parm) = true;
    }

  /* Build the node. */
  tree r = build_min (REQUIRES_EXPR, boolean_type_node, parms, reqs);
  TREE_SIDE_EFFECTS (r) = false;
  TREE_CONSTANT (r) = true;
  return r;
}

/* Construct a requirement for the validity of EXPR.   */

tree
finish_simple_requirement (tree expr)
{
  return build_nt (SIMPLE_REQ, expr);
}

/* Construct a requirement for the validity of TYPE.  */

tree
finish_type_requirement (tree type)
{
  return build_nt (TYPE_REQ, type);
}

/* Construct a requirement for the validity of EXPR, along with
   its properties. if TYPE is non-null, then it specifies either
   an implicit conversion or argument deduction constraint,
   depending on whether any placeholders occur in the type name.
   NOEXCEPT_P is true iff the noexcept keyword was specified.  */

tree
finish_compound_requirement (tree expr, tree type, bool noexcept_p)
{
  tree req = build_nt (COMPOUND_REQ, expr, type);
  COMPOUND_REQ_NOEXCEPT_P (req) = noexcept_p;
  return req;
}

/* Finish a nested requirement.  */

tree
finish_nested_requirement (tree expr)
{
  return build_nt (NESTED_REQ, expr);
}

// Check that FN satisfies the structural requirements of a
// function concept definition.
tree
check_function_concept (tree fn)
{
  // Check that the function is comprised of only a single
  // return statement.
  tree body = DECL_SAVED_TREE (fn);
  if (TREE_CODE (body) == BIND_EXPR)
    body = BIND_EXPR_BODY (body);

  // Sometimes a function call results in the creation of clean up
  // points. Allow these to be preserved in the body of the
  // constraint, as we might actually need them for some constexpr
  // evaluations.
  if (TREE_CODE (body) == CLEANUP_POINT_EXPR)
    body = TREE_OPERAND (body, 0);

  /* Check that the definition is written correctly. */
  if (TREE_CODE (body) != RETURN_EXPR)
    {
      location_t loc = DECL_SOURCE_LOCATION (fn);
      if (TREE_CODE (body) == STATEMENT_LIST && !STATEMENT_LIST_HEAD (body))
	{
	  if (seen_error ())
	    /* The definition was probably erroneous, not empty.  */;
	  else
	    error_at (loc, "definition of concept %qD is empty", fn);
	}
      else
	error_at (loc, "definition of concept %qD has multiple statements", fn);
    }

  return NULL_TREE;
}


// Check that a constrained friend declaration function declaration,
// FN, is admissible. This is the case only when the declaration depends
// on template parameters and does not declare a specialization.
void
check_constrained_friend (tree fn, tree reqs)
{
  if (fn == error_mark_node)
    return;
  gcc_assert (TREE_CODE (fn) == FUNCTION_DECL);

  // If there are not constraints, this cannot be an error.
  if (!reqs)
    return;

  // Constrained friend functions that don't depend on template
  // arguments are effectively meaningless.
  if (!uses_template_parms (TREE_TYPE (fn)))
    {
      error_at (location_of (fn),
		"constrained friend does not depend on template parameters");
      return;
    }
}

/*---------------------------------------------------------------------------
			Equivalence of constraints
---------------------------------------------------------------------------*/

/* Returns true when A and B are equivalent constraints.  */
bool
equivalent_constraints (tree a, tree b)
{
  gcc_assert (!a || TREE_CODE (a) == CONSTRAINT_INFO);
  gcc_assert (!b || TREE_CODE (b) == CONSTRAINT_INFO);
  return cp_tree_equal (a, b);
}

/* Returns true if the template declarations A and B have equivalent
   constraints. This is the case when A's constraints subsume B's and
   when B's also constrain A's.  */
bool
equivalently_constrained (tree d1, tree d2)
{
  gcc_assert (TREE_CODE (d1) == TREE_CODE (d2));
  return equivalent_constraints (get_constraints (d1), get_constraints (d2));
}

/*---------------------------------------------------------------------------
		     Partial ordering of constraints
---------------------------------------------------------------------------*/

/* Returns true when the the constraints in A subsume those in B.  */

bool
subsumes_constraints (tree a, tree b)
{
  gcc_assert (!a || TREE_CODE (a) == CONSTRAINT_INFO);
  gcc_assert (!b || TREE_CODE (b) == CONSTRAINT_INFO);
  return subsumes (a, b);
}

/* Returns the normalized constraints from a constraint-info object
   or NULL_TREE if the constraints are null. ARGS provide the initial
   arguments for normalization and IN_DECL provides the declaration
   to which the constraints belong.  */

static tree
get_normalized_constraints_from_info (tree ci, tree args, tree in_decl)
{
  if (ci == NULL_TREE)
    return NULL_TREE;

  /* Substitution errors during normalization are fatal.  */
  subst_info info (tf_warning_or_error, in_decl);
  return normalize_expression (CI_ASSOCIATED_CONSTRAINTS (ci), args, info);
}

/* Returns the normalized constraints for the declaration D.  */

static tree
get_normalized_constraints_from_decl (tree d)
{
  tree tmpl;
  tree decl;
  if (TREE_CODE (d) == TEMPLATE_DECL)
    {
      tmpl = d;
      decl = DECL_TEMPLATE_RESULT (tmpl);
    }
  else
    {
      if (tree ti = DECL_TEMPLATE_INFO (d))
	tmpl = TI_TEMPLATE (ti);
      else
	tmpl = NULL_TREE;
      decl = d;
    }

  tree args;
  if (TREE_CODE (decl) == TYPE_DECL)
    args = CLASSTYPE_TI_ARGS (TREE_TYPE (decl));
  else
    args = tmpl ? DECL_TI_ARGS (decl) : NULL_TREE;

  tree ci = get_constraints (decl);

  return get_normalized_constraints_from_info (ci, args, tmpl);
}

/* Returns true when the the constraints in CI (with arguments
   ARGS) strictly subsume the associated constraints of TMPL.  */

bool
strictly_subsumes (tree ci, tree args, tree tmpl)
{
  tree n1 = get_normalized_constraints_from_info (ci, args, NULL_TREE);
  tree n2 = get_normalized_constraints_from_decl (tmpl);

  return subsumes (n1, n2) && !subsumes (n2, n1);
}

/* REturns true when the constraints in CI (with arguments ARGS) subsume
   the associated constraints of TMPL.  */

bool
weakly_subsumes (tree ci, tree args, tree tmpl)
{
  tree n1 = get_normalized_constraints_from_info (ci, args, NULL_TREE);
  tree n2 = get_normalized_constraints_from_decl (tmpl);

  return subsumes (n1, n2);
}

/* Determines which of the declarations, A or B, is more constrained.
   That is, which declaration's constraints subsume but are not subsumed
   by the other's?

   Returns 1 if D1 is more constrained than D2, -1 if D2 is more constrained
   than D1, and 0 otherwise. */

int
more_constrained (tree d1, tree d2)
{
  tree n1 = get_normalized_constraints_from_decl (d1);
  tree n2 = get_normalized_constraints_from_decl (d2);

  int winner = 0;
  if (subsumes (n1, n2))
    ++winner;
  if (subsumes (n2, n1))
    --winner;
  return winner;
}

/* Returns true if D1 is at least as constrained as D2. That is, the
   associated constraints of D1 subsume those of D2, or both declarations
   are unconstrained. */

bool
at_least_as_constrained (tree d1, tree d2)
{
  tree c1 = get_constraints (d1);
  tree c2 = get_constraints (d2);
  return subsumes_constraints (c1, c2);
}


/*---------------------------------------------------------------------------
			Constraint diagnostics
---------------------------------------------------------------------------*/

/* The number of detailed constraint failures.  */

int constraint_errors = 0;

/* Do not generate errors after diagnosing this number of constraint
   failures.

   FIXME: This is a really arbitrary number. Provide better control of
   constraint diagnostics with a command line option.  */

int constraint_threshold = 20;


/* Returns true if we should elide the diagnostic for a constraint failure.
   This is the case when the number of errors has exceeded the pre-configured
   threshold.  */

inline bool
elide_constraint_failure_p ()
{
  bool ret = constraint_threshold <= constraint_errors;
  ++constraint_errors;
  return ret;
}

/* Returns the number of undiagnosed errors. */

inline int
undiagnosed_constraint_failures ()
{
  return constraint_errors - constraint_threshold;
}

static bool diagnose_constraint (tree, tree, tree);

/* Returns the location of the constraint expression.

  FIXME: For some reason locations are not being correctly associated
  with constraints. I'm not sure why.  */
static inline location_t
get_constraint_location (tree expr)
{
  return EXPR_LOC_OR_LOC (expr, input_location);
}

/* Emit a diagnostic for a failed concept check.  */

void
diagnose_check (tree expr, tree args, tree in_decl)
{
  location_t eloc = get_constraint_location (expr);

  tree orig_expr = expr;
  ++satisfying_constraint;
  expr = tsubst_expr (expr, args, tf_none, in_decl, false);
  --satisfying_constraint;
  if (expr == error_mark_node)
    {
      inform (eloc, "invalid use of the concept %qE", orig_expr);
      /* FIXME: This produces a correct diagnostic, but it needs to
         be a note, not a second error.  */
      /* tsubst_expr (orig_expr, args, tf_error, in_decl, false);  */
      return;
    }

  gcc_assert (TREE_CODE (expr) == TEMPLATE_ID_EXPR);
  tree tmpl = TREE_OPERAND (expr, 0);
  tree targs = TREE_OPERAND (expr, 1);


  /* A function concept may be represented as an overload set.  */
  if (OVL_P (tmpl))
    tmpl = OVL_FIRST (tmpl);

  /* Provide context for the error.  */
  tree subst = build_tree_list (tmpl, targs);
  location_t dloc = DECL_SOURCE_LOCATION (DECL_TEMPLATE_RESULT (tmpl));
  inform (dloc, "within %qS", subst);

  /* Instantiate the concept definition...  */
  tree def = get_concept_definition (tmpl);
  if (!def || def == error_mark_node)
    {
      inform (eloc, "invalid concept definition");
      return;
    }

  expr = tsubst_expr (def, targs, tf_none, NULL_TREE, true);
  if (expr == error_mark_node)
    {
      inform (eloc, "in the expansion of concept %<%E %S%>", expr, subst);
      tsubst_expr (def, targs, tf_warning_or_error, NULL_TREE, false);
    }

  /* ... and recursively search for diagnosable errors.  */
  diagnose_constraint (def, targs, tmpl);
}

/* Emit a diagnostic for a failed trait.  */

void
diagnose_trait (tree expr, tree args)
{
  location_t loc = get_constraint_location (expr);

  /* Build a "fake" version of the instantiated trait, so we can
     get the instantiated types from result.  */
  ++processing_template_decl;
  expr = tsubst_expr (expr, args, tf_none, NULL_TREE, false);
  --processing_template_decl;

  tree t1 = TRAIT_EXPR_TYPE1 (expr);
  tree t2 = TRAIT_EXPR_TYPE2 (expr);
  switch (TRAIT_EXPR_KIND (expr))
    {
    case CPTK_HAS_NOTHROW_ASSIGN:
      inform (loc, "  %qT is not nothrow copy assignable", t1);
      break;
    case CPTK_HAS_NOTHROW_CONSTRUCTOR:
      inform (loc, "  %qT is not nothrow default constructible", t1);
      break;
    case CPTK_HAS_NOTHROW_COPY:
      inform (loc, "  %qT is not nothrow copy constructible", t1);
      break;
    case CPTK_HAS_TRIVIAL_ASSIGN:
      inform (loc, "  %qT is not trivially copy assignable", t1);
      break;
    case CPTK_HAS_TRIVIAL_CONSTRUCTOR:
      inform (loc, "  %qT is not trivially default constructible", t1);
      break;
    case CPTK_HAS_TRIVIAL_COPY:
      inform (loc, "  %qT is not trivially copy constructible", t1);
      break;
    case CPTK_HAS_TRIVIAL_DESTRUCTOR:
      inform (loc, "  %qT is not trivially destructible", t1);
      break;
    case CPTK_HAS_VIRTUAL_DESTRUCTOR:
      inform (loc, "  %qT does not have a virtual destructor", t1);
      break;
    case CPTK_IS_ABSTRACT:
      inform (loc, "  %qT is not an abstract class", t1);
      break;
    case CPTK_IS_BASE_OF:
      inform (loc, "  %qT is not a base of %qT", t1, t2);
      break;
    case CPTK_IS_CLASS:
      inform (loc, "  %qT is not a class", t1);
      break;
    case CPTK_IS_EMPTY:
      inform (loc, "  %qT is not an empty class", t1);
      break;
    case CPTK_IS_ENUM:
      inform (loc, "  %qT is not an enum", t1);
      break;
    case CPTK_IS_FINAL:
      inform (loc, "  %qT is not a final class", t1);
      break;
    case CPTK_IS_LITERAL_TYPE:
      inform (loc, "  %qT is not a literal type", t1);
      break;
    case CPTK_IS_POD:
      inform (loc, "  %qT is not a POD type", t1);
      break;
    case CPTK_IS_POLYMORPHIC:
      inform (loc, "  %qT is not a polymorphic type", t1);
      break;
    case CPTK_IS_SAME_AS:
      inform (loc, "  %qT is not the same as %qT", t1, t2);
      break;
    case CPTK_IS_STD_LAYOUT:
      inform (loc, "  %qT is not an standard layout type", t1);
      break;
    case CPTK_IS_TRIVIAL:
      inform (loc, "  %qT is not a trivial type", t1);
      break;
    case CPTK_IS_UNION:
      inform (loc, "  %qT is not a union", t1);
      break;
    default:
      gcc_unreachable ();
    }
}

static tree
diagnose_valid_expression (tree expr, tree args, tree in_decl)
{
  ++processing_template_decl;
  tree result = tsubst_expr (expr, args, tf_none, in_decl, false);
  --processing_template_decl;

  location_t loc = get_constraint_location (expr);
  inform (loc, "the required expression %qE is invalid", expr);

  /* Replay the substitution to diagnose the error.
     FIXME: This will emit a 2nd error diagnostic.  */
  if (result != error_mark_node)
    tsubst_expr (expr, args, tf_error, in_decl, false);

  return error_mark_node;
}

static tree
diagnose_valid_type (tree type, tree args, tree in_decl)
{
  ++processing_template_decl;
  tree result = tsubst (type, args, tf_none, in_decl);
  --processing_template_decl;

  /* FIXME: The type probably does not have a location.  */
  location_t loc = get_constraint_location (type);
  inform (loc, "the required type %qT is invalid", type);

  /* Replay the substitution to diagnose the error.
     FIXME: This will emit a 2nd error diagnostic.  */
  if (result != error_mark_node)
    tsubst (type, args, tf_error, in_decl);

  return error_mark_node;
}

static void
diagnose_simple_requirement (tree req, tree args, tree in_decl)
{
  tree expr = TREE_OPERAND (req, 0);
  diagnose_valid_expression (expr, args, in_decl);
}

static void
diagnose_compound_requirement (tree req, tree args, tree in_decl)
{
  tree expr = TREE_OPERAND (req, 0);
  expr = diagnose_valid_expression (expr, args, in_decl);
  if (expr == error_mark_node)
    return;

  /* FIXME: What's the right location?  */
  location_t loc = get_constraint_location (expr);

  /* Check the noexcept condition.  */
  if (COMPOUND_REQ_NOEXCEPT_P (req) && !expr_noexcept_p (expr, tf_none))
    inform (loc, "%qE is not %<noexcept%>", expr);

  tree type = TREE_OPERAND (req, 1);
  type = diagnose_valid_type (type, args, in_decl);
  if (type == error_mark_node)
    return;

  if (type)
    {
      subst_info quiet_subst(tf_none, in_decl);

      /* Check the expression against the result type.  */
      if (tree placeholder = type_uses_auto (type))
	{
	  if (!type_deducible_p (expr, type, placeholder, args, quiet_subst))
	    {
	      subst_info noisy_subst(tf_none, in_decl);
	      inform (loc, "cannot deduce a type from %qE", expr);
	      type_deducible_p (expr, type, placeholder, args, noisy_subst);
	    }
	}
      else if (!expression_convertible_p (expr, type, quiet_subst))
      {
	inform (loc, "cannot convert %qE to %qT", expr, type);
	/* FIXME: This generates multiple error diagnostics.  */
	/* expression_convertible_p (expr, type, tf_warning_or_error); */
      }
    }
}

static void
diagnose_type_requirement (tree req, tree args, tree in_decl)
{
  tree type = TREE_OPERAND (req, 0);
  diagnose_valid_type (type, args, in_decl);
}

static void
diagnose_nested_requirement (tree req, tree args)
{
  tree expr = TREE_OPERAND (req, 0);
  if (constraints_satisfied_p (req, args))
    return;
  location_t loc = get_constraint_location (expr);
  inform (loc, "nested requirement %qE is not satisfied", expr);
}

static void
diagnose_requiremnt (tree req, tree args, tree in_decl)
{
  switch (TREE_CODE (req))
    {
    case SIMPLE_REQ:
      return diagnose_simple_requirement (req, args, in_decl);
    case COMPOUND_REQ:
      return diagnose_compound_requirement (req, args, in_decl);
    case TYPE_REQ:
      return diagnose_type_requirement (req, args, in_decl);
    case NESTED_REQ:
      return diagnose_nested_requirement (req, args);
    default:
       gcc_unreachable ();
    }
}

static void
diagnose_requires (tree expr, tree args, tree in_decl)
{
  local_specialization_stack stack;
  tree parms = TREE_OPERAND (expr, 0);
  tree body = TREE_OPERAND (expr, 1);

  subst_info info(tf_warning_or_error, NULL_TREE);
  tree vars = tsubst_constraint_variables (parms, args, info);
  if (vars == error_mark_node)
    return;

  /* TODO: It would be better write these in a list. */
  while (vars)
    {
      location_t loc = DECL_SOURCE_LOCATION (vars);
      inform (loc, "with constraint variable %q#D", vars);
      vars = TREE_CHAIN (vars);
    }

  tree p = body;
  while (p)
    {
      tree req = TREE_VALUE (p);
      diagnose_requiremnt (req, args, in_decl);
      p = TREE_CHAIN (p);
    }
}

/* Diagnose a conjunction.  Only search one branch; this prevents us
   from getting duplicate diagnostics.

   TODO: We could maintain a set of found diagnostics and suppress
   redundant checks.  */
static bool
diagnose_and (tree expr, tree args, tree in_decl)
{
  if (diagnose_constraint (TREE_OPERAND (expr, 0), args, in_decl))
    return true;
  return diagnose_constraint (TREE_OPERAND (expr, 1), args, in_decl);
}

/* Diagnose a disjunction. If only one operand fails, search that
   branch for a diagnostic. If both fail, just diagnose that property;
   don't recurse.  */
static bool
diagnose_or (tree expr, tree args, tree in_decl)
{
  tree t1 = TREE_OPERAND (expr, 0);
  tree t2 = TREE_OPERAND (expr, 1);

  /* Check satisfaction independently.  */
  bool sat1 = constraints_satisfied_p (t1, args);
  bool sat2 = constraints_satisfied_p (t2, args);

  location_t loc = get_constraint_location (expr);

  if (sat1)
    {
      if (sat2)
        /* There are diagnostics in neither operand.  */
        return false;
      else
        /* Search t2 for diagnostics.  */
        return diagnose_constraint (t2, args, in_decl);
    }
  if (sat2)
    /* Search t1 for diagnostics.  */
    return diagnose_constraint (t1, args, in_decl);

  /* Neither operand is satisfied, but don't recurse in both.  */
  inform (loc, "neither %qE nor %qE is satisfied", t1, t2);
  return true;
}

static void
diagnose_atom (tree expr, tree args, tree in_decl)
{
  location_t loc = get_constraint_location (expr);

  /* Substitute through the expression quietly. If that fails,
     replay the error.  */
  tree result = tsubst_expr (expr, args, tf_none, in_decl, false);
  if (result == error_mark_node)
    {
      tsubst_expr (expr, args, tf_none, in_decl, false);
      return;
    }

  /* Convert to an rvalue and check for type bool.  */
  result = force_rvalue (result, tf_warning_or_error);
  if (result == error_mark_node)
    return;
  if (cv_unqualified (TREE_TYPE (result)) != boolean_type_node)
    {
      inform (loc, "atomic constraint %qE does not have type %<bool%>", expr);
      return;
    }

  /* Provide precise diagnostics.  */
  switch (TREE_CODE (expr))
    {
    case TRAIT_EXPR:
      diagnose_trait (expr, args);
      break;
    case REQUIRES_EXPR:
      diagnose_requires (expr, args, in_decl);
      break;
    case INTEGER_CST:
      inform (loc, "%qE is never satisfied", expr);
      break;
    default:
      inform (loc, "the constraint %qE evaluated to %<false%>", expr);
      break;
    }
}

/* Emit a diagnostic for the constraint. Returns true if any diagnostics
   have been emitted.  */

static bool
diagnose_constraint (tree expr, tree args, tree in_decl)
{
  /* Handle logical operators separately.  */
  switch (TREE_CODE (expr))
    {
    case TRUTH_ANDIF_EXPR:
      return diagnose_and (expr, args, in_decl);
    case TRUTH_ORIF_EXPR:
      return diagnose_or (expr, args, in_decl);
    default:
      break;
    }

  /* If the constraint is satisfied, there are no diagnostics to emit.  */
  if (constraints_satisfied_p (expr, args))
    return false;

  /* Search for diagnostics.  */
  if (concept_check_p (expr))
    diagnose_check (expr, args, in_decl);
  else
    diagnose_atom (expr, args, in_decl);
  return true;
}

/* Diagnose the reason(s) why ARGS do not satisfy the constraints
   of declaration DECL. */

static void
diagnose_declaration_constraints (location_t loc, tree decl, tree args)
{
  inform (loc, "constraints not satisfied");

  /* Constraints are attached to the template.  */
  if (tree ti = DECL_TEMPLATE_INFO (decl))
    {
      decl = TI_TEMPLATE (ti);
      if (!args)
	args = TI_ARGS (ti);
    }

  /* Turn off template processing, regardless of context.  */
  processing_template_decl_sentinel proc (true);

  /* Diagnose the failed constraint.  */
  tree expr = CI_ASSOCIATED_CONSTRAINTS (get_constraints (decl));
  diagnose_constraint (expr, args, decl);
}

/* Emit diagnostics detailing the failure ARGS to satisfy the
   constraints of T. Here, T can be either a constraint
   or a declaration.  */

void
diagnose_constraints (location_t loc, tree t, tree args)
{
  constraint_errors = 0;

  if (DECL_P (t))
    diagnose_declaration_constraints (loc, t, args);
  else
    diagnose_constraint (t, args, NULL_TREE);

  /* Note the number of elided failures. */
  int n = undiagnosed_constraint_failures ();
  if (n > 0)
    inform (loc, "... and %d more constraint errors not shown", n);
}
