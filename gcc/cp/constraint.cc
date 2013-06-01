/* Process declarations and variables for C++ compiler.
   Copyright (C) 2013 Free Software Foundation, Inc.
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

// Components for process constraints and evaluating constraints.

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-family/c-common.h"
#include "c-family/c-objc.h"
#include "tree-inline.h"
#include "intl.h"
#include "toplev.h"
#include "flags.h"
#include "timevar.h"
#include "diagnostic.h"
#include "cgraph.h"
#include "tree-iterator.h"
#include "vec.h"
#include "target.h"
#include "gimple.h"
#include "bitmap.h"


// -------------------------------------------------------------------------- //
// Requirement Construction
//
// Facilities for building and manipulating template requirements. 
//
// TODO: Simply assinging boolean_type_node to the result type of the expression
// seems right thing for constraints, but in the long-term we might want to be
// more flexible (i.e., allow some form of overload resolution?).


// Returns the conjunction of two requirements A and B, where A and B are
// reduced terms in the constraints languaage. Returns NULL_TREE if either A or
// B are NULL_TREE.
tree
conjoin_requirements (tree a, tree b)
{
  if (a && b)
    return build_min (TRUTH_ANDIF_EXPR, boolean_type_node, a, b);
  else
    return NULL_TREE;
}

// Returns the disjunction of two requirements A and B, where A and B are
// reduced terms in the constraints languaage. Returns NULL_TREE if either A or
// B are NULL_TREE.
tree
disjoin_requirements (tree a, tree b)
{
  if (a && b)
    return build_min (TRUTH_ORIF_EXPR, boolean_type_node, a, b);
  else
    return NULL_TREE;
}


// -------------------------------------------------------------------------- //
// Constraint Resolution
//
// This facility is used to resolve constraint checks from requirement
// expressions. A constraint check is a call to a constraint predicate:
// a constexpr, nullary function teplate whose result can be converted
// to bool.
//
// The result of resolution is a pair (a list node) whose value is the
// matched declaration, and whose purpose contains the coerced template
// arguments that can be substituted into the call.

namespace {

// Returns true if the function decl F is a constraint predicate.
// It must be a constexpr, nullary function with a boolean result
// type.
static bool
is_constraint (tree f)
{
  gcc_assert (TREE_CODE (f) == FUNCTION_DECL);

  // A constraint is nullary.
  if (DECL_ARGUMENTS (f))
    return false;

  // A constraint is declared constexpr
  if (!DECL_DECLARED_CONSTEXPR_P (f))
    return false;

  // Whose result must be convertible to bool.
  if (!can_convert (TREE_TYPE (TREE_TYPE (f)), boolean_type_node, tf_none))
    return false;

  // A constraint can only be checked if it is defined.
  if (!DECL_SAVED_TREE (f))
    return false;

  return true;
}

// Given an OVL set of constraint candidates, try to find a unique definition
// satisfying the requirements of a constraint.
//
// This function is not called for abitrary call expressions. In particul,
// the call expression must be written with explicit template arguments
// and no function arguments. For example:
//
//      f<T, U>()
//
// The overload set will contain only template declarations.
//
// If a single definition is found, this returns a list node whose VALUE
// is the constraint function (not the template), and its PURPOSE is
// the complete set of arguments substituted into the parameter list.
static tree
resolve_constraint_check (tree ovl, tree args)
{
  tree cands = NULL_TREE;
  for (tree p = ovl; p != NULL_TREE; p = OVL_NEXT (p))
    {
      tree tmpl = OVL_FUNCTION (p);
      tree parms = TREE_VALUE (DECL_TEMPLATE_PARMS (tmpl));

      // Remember the candidate if we can deduce a substitution.
      if (tree subst = substitute_template_parameters (parms, args))
        if (subst != error_mark_node)
          cands = tree_cons (subst, DECL_TEMPLATE_RESULT (tmpl), cands);
    }

  // If there are multiple candidates, then we have not found
  // a unique definition.
  if (TREE_CHAIN (cands))
    return NULL_TREE;

  if (!is_constraint (TREE_VALUE (cands)))
    return NULL_TREE;

  return cands;
}

// If the call express T is a check expression, return a singleton tree
// list whose VALUE is the checked constraint predicate, and whose
// PURPOSE contains substitution arguments for the constraint. If the 
// call does not denote a check, return NULL_TREE.
//
// Note that a call expression is a check expression if it refers to a
// unique, nullary function template via lightweight overload resolution.
tree
maybe_constraint_check (tree call)
{
  gcc_assert (TREE_CODE (call) == CALL_EXPR);

  // A constraint check must be a call to a function template with 
  // arguments given explicitly.
  tree f = CALL_EXPR_FN (call);
  if (TREE_CODE (f) != TEMPLATE_ID_EXPR)
    return NULL_TREE;

  // Also, make sure that there are no arguments to the call expression.
  // If there are, then we are guaranteed that this is a regular call.
  if (call_expr_nargs (call))
    return NULL_TREE;

  // Determine which constraint is actually referred to by the
  // call expression.
  tree tmpl = TREE_OPERAND (f, 0);
  tree args = TREE_OPERAND (f, 1);
  return resolve_constraint_check (tmpl, args);
}
  
} // end namespace


// -------------------------------------------------------------------------- //
// Requirement Reduction
//
// Reduces a template requirement to a logical formula written in terms of
// atomic propositions, returing the new expression.  If the expression cannot
// be reduced, a NULL_TREE is returned, indicating failure to reduce the
// original requirment. 

namespace {

// Helper functions
static tree reduce_node (tree);
static tree reduce_expr (tree);
static tree reduce_stmt (tree);
static tree reduce_decl (tree);
static tree reduce_misc (tree);

static tree reduce_logical     (tree);
static tree reduce_call        (tree);
static tree reduce_template_id (tree);
static tree reduce_stmt_list   (tree);

// Reduce the requirement T into a logical formula written in terms of
// atomic propositions.
tree 
reduce_node (tree t)
{
  switch (TREE_CODE_CLASS (TREE_CODE (t))) 
    {
    case tcc_unary:
    case tcc_binary:
    case tcc_expression:
    case tcc_vl_exp:
      return reduce_expr (t);
    
    case tcc_statement:   
      return reduce_stmt (t);
    
    case tcc_declaration: 
      return reduce_decl (t);
    
    case tcc_exceptional: 
      return reduce_misc (t);
    
    // These kinds of expressions are atomic.
    case tcc_constant:
    case tcc_reference:
    case tcc_comparison:
      return t;

    default:
      gcc_unreachable ();
    }
  return NULL_TREE;
}

// Reduction rules for the expression node T.
tree
reduce_expr (tree t)
{
  switch (TREE_CODE (t))
    {
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:  
      return reduce_logical (t);

    case CALL_EXPR:        
      return reduce_call (t);

    case TEMPLATE_ID_EXPR: 
      return reduce_template_id (t);

    case CAST_EXPR:        
      return reduce_node (TREE_VALUE (TREE_OPERAND (t, 0)));
    
    case BIND_EXPR:        
      return reduce_node (BIND_EXPR_BODY (t));

    // Do not recurse.
    case TAG_DEFN:         
      return NULL_TREE;

    // Everything else is atomic.
    default:
      return t;
    }
}


// Reduction rules for the statement T.
tree
reduce_stmt (tree t)
{
  switch (TREE_CODE (t))
    {
    // Reduce the returned expression.
    case RETURN_EXPR:
      return reduce_node (TREE_OPERAND (t, 0));

    // These statements do not introduce propositions
    // in the constraints language. Do not recurse.
    case DECL_EXPR:
    case USING_STMT:
      return NULL_TREE;
    
    default:
      gcc_unreachable ();
    }
  return NULL_TREE;
}

// Reduction rules for the declaration T.
tree
reduce_decl (tree t)
{
  switch (TREE_CODE (t))
    {
    // References to var decls are atomic.
    case VAR_DECL:
      return t;
    
    default:
      gcc_unreachable ();
    }
  return NULL_TREE;
}

// Reduction rules for the node T.
tree
reduce_misc (tree t)
{
  switch (TREE_CODE (t))
    {
    // Errors and traits are atomic.
    case ERROR_MARK:
    case TRAIT_EXPR:
      return t;

    case STATEMENT_LIST:
      return reduce_stmt_list (t);
    
    default:
      gcc_unreachable ();
    }
  return NULL_TREE;
}

// Reduction rules for the binary logical expression T (&& and ||).
//
// Generate a new expression from the reduced operands. If either operand
// cannot be reduced, then the resulting expression is null.
tree
reduce_logical (tree t)
{
  tree l = reduce_expr (TREE_OPERAND (t, 0));
  tree r = reduce_expr (TREE_OPERAND (t, 1));
  if (l && r)
    {
      t = copy_node (t);
      TREE_OPERAND (t, 0) = l;
      TREE_OPERAND (t, 1) = r;
      return t;
    }
  else
    return NULL_TREE;
}

// Reduction rules for the call expression T.
//
// If T is a call to a constraint instantiate it's definition and
// recursively reduce its returned expression.
tree
reduce_call (tree t)
{
  // Is the function call actually a constraint check?
  tree check = maybe_constraint_check (t);
  if (!check)
    return t;

  tree fn = TREE_VALUE (check);
  tree args = TREE_PURPOSE (check);

  // Reduce the body of the function into the constriants language.
  tree body = reduce_requirements (DECL_SAVED_TREE (fn));
  if (!body)
    {
      error ("could not inline requirements from %qD", fn);
      return error_mark_node;
    }

  // Instantiate the reduced results using the deduced args.
  tree result = instantiate_requirements (body, args);
  if (result == error_mark_node)
    {
      error ("could not instantiate requirements from %qD", fn);
      return error_mark_node;
    }
  return result;
}

// Reduction rules for the template-id T.
//
// It turns out that we often get requirements being written like this:
//
//    template<typename T>
//      requires Foo<T>
//    void f()
//
// Where Foo<T> should actually be written as Foo<T>(). Generate an
// error and suggest the improved writing.
tree
reduce_template_id (tree t)
{
  vec<tree, va_gc>* args = NULL;
  tree c = finish_call_expr (t, &args, true, false, 0);
  error ("invalid requirement");
  inform (input_location, "did you mean %qE", c);
  return NULL_TREE;
}

// Reduction rules for the statement list STMTS.
//
// Recursively reduce each statement in the list, concatenating each
// reduced result into a conjunction of requirements. 
//
// A constexpr function may include statements other than a return
// statement. The primary purpose of these rules is to filter those
// non-return statements from the constraints language.
tree
reduce_stmt_list (tree stmts)
{
  tree lhs = NULL_TREE;
  tree_stmt_iterator i = tsi_start (stmts);
  while (!tsi_end_p (i))
    {
      if (tree rhs = reduce_node (tsi_stmt (i)))
        {
          if (!lhs)
            lhs = rhs;
          else
            lhs = conjoin_requirements (lhs, rhs);
        }
      tsi_next (&i);
    }
  return lhs;
}

} // end namespace


// Reduce the requirement T into a logical formula written in terms of
// atomic propositions.
tree
reduce_requirements (tree reqs)
{
  return reduce_node (reqs);
}

// Create a constraint-info node from the specified requirements.
tree 
make_constraints (tree reqs)
{
  // No requirements == no constraints
  if (!reqs)
    return NULL_TREE;

  // Reduce the requirements into atoms and break them into
  // sets of atomic propositions.
  tree atomic = reduce_requirements (reqs);
  if (atomic == error_mark_node)
    return error_mark_node;
  tree assume = decompose_assumptions (atomic);

  tree_constraint_info *cinfo = 
    (tree_constraint_info *)make_node (CONSTRAINT_INFO);
  cinfo->spelling = reqs;
  cinfo->requirements = atomic;
  cinfo->assumptions = assume;
    
  return (tree)cinfo;
}
