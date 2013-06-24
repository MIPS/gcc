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

// Components for processing constraints and evaluating constraints.

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
// TODO: Simply assigning boolean_type_node to the result type of the 
// expression seems right for constraints, but in the long-term we might want
// to be more flexible (i.e., allow some form of overload resolution?).

// Create a new logical node joining the subexpressions a and b.
static inline tree
join_requirements (tree_code c, tree a, tree b)
{
  gcc_assert (a != NULL_TREE && b != NULL_TREE);
  gcc_assert (c == TRUTH_ANDIF_EXPR || c == TRUTH_ORIF_EXPR);
  return build_min (c, boolean_type_node, a, b);
}

// Returns the conjunction of two requirements A and B, where A and B are
// reduced terms in the constraints language. Note that conjoining a non-null 
// expression with  NULL_TREE is an identity operation. That is, for some 
// non-null A,
//
//    conjoin_requirements(a, NULL_TREE) == a
//
// If both A and B are NULL_TREE, the result is also NULL_TREE.
tree
conjoin_requirements (tree a, tree b)
{
  if (a)
    return b ? join_requirements (TRUTH_ANDIF_EXPR, a, b) : a;
  else if (b)
    return b;
  else
    return NULL_TREE;
}


// -------------------------------------------------------------------------- //
// Constraint Resolution
//
// This facility is used to resolve constraint checks from requirement
// expressions. A constraint check is a call to a function template, declared
// concept.
//
// The result of resolution is a pair (a list node) whose value is the
// matched declaration, and whose purpose contains the coerced template
// arguments that can be substituted into the call.

namespace {

// Given an overload set, try to find a unique definition that can be
// instantiated by the template arguments.
//
// This function is not called for arbitrary call expressions. In particular,
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
      if (tree subst = coerce_template_parms (parms, args, tmpl))
        if (subst != error_mark_node)
          cands = tree_cons (subst, DECL_TEMPLATE_RESULT (tmpl), cands);
    }

  // If we didn't find a unique candidate, then this is
  // not a constraint check.
  if (!cands || TREE_CHAIN (cands))
    return NULL_TREE;

  // Constraints must be declared concepts.
  tree decl = TREE_VALUE (cands);
  if (!DECL_DECLARED_CONCEPT_P (decl))
    return NULL_TREE;

  // Concept declarations must have a corresponding definition.
  //
  // TODO: This should be part of the up-front checking for 
  // a concept declaration.
  if (!DECL_SAVED_TREE (decl))
    {
      error_at (DECL_SOURCE_LOCATION (decl),
                "concept %q#D has no definition", decl);
      return NULL;
    }

  return cands;
}

// Determine if the the call expression CALL is a constraint check, and
// return the concept declaration and arguments being checked. If CALL
// does not denote a constraint check, return NULL.
tree
resolve_constraint_check (tree call)
{
  gcc_assert (TREE_CODE (call) == CALL_EXPR);

  // A constraint check must be only be a template-id expression.
  tree target = CALL_EXPR_FN (call);
  if (TREE_CODE (target) != TEMPLATE_ID_EXPR)
    return NULL_TREE;

  // Get the overload set and template arguments and try to
  // resolve the target.
  tree ovl = TREE_OPERAND (target, 0);
  tree args = TREE_OPERAND (target, 1);
  return resolve_constraint_check (ovl, args);
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
// If T is a call to a constraint instantiate its definition and
// recursively reduce its returned expression.
tree
reduce_call (tree t)
{
  // Is the function call actually a constraint check?
  tree check = resolve_constraint_check (t);
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
  error_at (EXPR_LOC_OR_HERE (t), "invalid requirement");
  inform (EXPR_LOC_OR_HERE (t), "did you mean %qE", c);
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


// Reduce the requirement REQS into a logical formula written in terms of
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


// -------------------------------------------------------------------------- //
// Get Constraints

// Returns the template constraints of declaration T. If T is not a
// template, this return NULL_TREE. Note that T must be non-null.
tree
get_constraints (tree t)
{
  gcc_assert (DECL_P (t));
  if (TREE_CODE (t) != TEMPLATE_DECL)
    {
      if (!DECL_TEMPLATE_INFO (t))
        return NULL_TREE;
      else
        return DECL_CONSTRAINTS (DECL_TI_TEMPLATE (t));
    }
  return DECL_CONSTRAINTS (t);
}


// -------------------------------------------------------------------------- //
// Check Constraints

// Returns true if the requirements expression REQS is satisfied
// and false otherwise. The requirements are checked by simply 
// evaluating REQS as a constant expression.
static inline bool
check_requirements (tree reqs)
{
  // Simplify the expression before evaluating it. This will
  // cause TRAIT_EXPR nodes to be reduced before constexpr
  // evaluation.
  reqs = fold_non_dependent_expr (reqs);
  
  // Requirements are satisfied when REQS evaluates to true.
  return cxx_constant_value (reqs) == boolean_true_node;
}

// Returns true if the requirements expression REQS is satisfied 
// and false otherwise. The requirements are checked by first
// instantiating REQS and then evaluating it as a constant expression.
static inline bool
check_requirements (tree reqs, tree args)
{
  reqs = instantiate_requirements (reqs, args);
  if (reqs == error_mark_node)
    return false;
  return check_requirements (reqs);
}

// Check the instantiated declaration constraints.
bool
check_constraints (tree cinfo)
{
  if (!cinfo)
    return true;
  return check_requirements (CI_REQUIREMENTS (cinfo));
}

// Check the constraints in CINFO against the given ARGS, returning
// true when the constraints are satisfied and false otherwise.
bool 
check_constraints (tree cinfo, tree args)
{
  // No constraints? Satisfied.
  if (!cinfo)
    return true;
  return check_requirements (CI_REQUIREMENTS (cinfo), args);
}

// Check the constraints of the declaration or type T, against 
// the specified arguments. Returns true if the constraints are 
// satisfied and false otherwise.
bool
check_template_constraints (tree t, tree args)
{
  return check_constraints (DECL_CONSTRAINTS (t), args);
}

// Returns true when A and B are equivalent constraints.
bool
equivalent_constraints (tree a, tree b)
{
  return subsumes (a, b) && subsumes (b, a);
}

// Returns true if the template declarations A and B have equivalent
// constraints. This is the case when A's constraints subsume B's and
// when B's also constrain A's.
bool 
equivalently_constrained (tree a, tree b)
{
  gcc_assert (TREE_CODE (a) == TREE_CODE (b));
  return equivalent_constraints (DECL_CONSTRAINTS (a), DECL_CONSTRAINTS (b));
}

// Returns true when the A contains more atomic properties than B.
bool
more_constraints (tree a, tree b)
{
  return subsumes (a, b);
}

// Returns true when the template declaration A's constraints subsume
// those of the template declaration B.
bool 
more_constrained (tree a, tree b)
{
  gcc_assert (TREE_CODE (a) == TREE_CODE (b));
  return more_constraints (DECL_CONSTRAINTS (a), DECL_CONSTRAINTS (b));
}


// -------------------------------------------------------------------------- //
// Constraint Diagnostics

namespace {

void diagnose_node (location_t, tree, tree);

// Diagnose a constraint failure for type trait expressions.
void
diagnose_trait (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;

  ++processing_template_decl;
  tree subst = instantiate_requirements (t, args);
  --processing_template_decl;

  if (subst == error_mark_node)
    {
      inform (input_location, "  substitution failure in %qE", t);
      return;
    }

  tree t1 = TRAIT_EXPR_TYPE1 (subst);
  tree t2 = TRAIT_EXPR_TYPE2 (subst);
  switch (TRAIT_EXPR_KIND (t))
    {
      case CPTK_HAS_NOTHROW_ASSIGN:
        inform (loc, "  %qT is not nothrow assignable", t1);
        break;
      case CPTK_HAS_NOTHROW_CONSTRUCTOR:
        inform (loc, "  %qT is not nothrow constructible", t1);
        break;
      case CPTK_HAS_NOTHROW_COPY:
        inform (loc, "  %qT is not nothrow copyable", t1);
        break;
      case CPTK_HAS_TRIVIAL_ASSIGN:
        inform (loc, "  %qT is not trivially assignable", t1);
        break;
      case CPTK_HAS_TRIVIAL_CONSTRUCTOR:
        inform (loc, "  %qT is not trivially constructible", t1);
        break;
      case CPTK_HAS_TRIVIAL_COPY:
        inform (loc, "  %qT is not trivially copyable", t1);
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

// Diagnose a failed concept check in concept indicated by T, where
// T is the result of resolve_constraint_check. Recursively analyze
// the nested requiremets for details.
void
diagnose_check (location_t loc, tree t, tree args)
{
  tree fn = TREE_VALUE (t);
  tree targs = TREE_PURPOSE (t);
  tree body = DECL_SAVED_TREE (fn);
  if (!body)
    return;
  
  inform (loc, "  failure in constraint %q#D", DECL_TI_TEMPLATE (fn));

  // Perform a mini-reduction on the constraint.
  if (TREE_CODE (body) == BIND_EXPR)
    body = BIND_EXPR_BODY (body);
  if (TREE_CODE (body) == RETURN_EXPR)
    body = TREE_OPERAND (body, 0);

  // Locally instantiate the body with the call's template args, 
  // and recursively diagnose.
  ++processing_template_decl;
  body = instantiate_requirements (body, targs);
  --processing_template_decl;
  
  diagnose_node (loc, body, args);
}

// Diagnose constraint failures from the call expression T.
void
diagnose_call (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;

  // If this is a concept, we're going to recurse.
  // If it's just a call, then we can emit a simple message.
  if (tree check = resolve_constraint_check (t))
    diagnose_check (loc, check, args);
  else
    inform (loc, "  %qE evaluated to false", t);
}

// Diagnose a constraint failure in the expression T.
void
diagnose_other (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;
  inform (loc, "  %qE evaluated to false", t);
}

// Diagnose a constraint failure in the subtree T.
void
diagnose_node (location_t loc, tree t, tree args)
{
  switch (TREE_CODE (t))
    {
    case TRUTH_ANDIF_EXPR:
      diagnose_node (loc, TREE_OPERAND (t, 0), args);
      diagnose_node (loc, TREE_OPERAND (t, 1), args);
      break;

    case TRUTH_ORIF_EXPR:
      sorry ("cannot diagnose disjunctions just yet");
      break;

    case TRAIT_EXPR:
      diagnose_trait (loc, t, args);
      break;
    case CALL_EXPR:
      diagnose_call (loc, t, args);
      break;
    default:
      diagnose_other (loc, t, args);
      break;
    }
}

// Diagnose a constraint failure in the requirements expression REQS.
inline void 
diagnose_requirements (location_t loc, tree reqs, tree args)
{
  diagnose_node (loc, reqs, args);
}

// Create a tree node representing the substitution of ARGS into
// the parameters of TMPL. The resulting structure is passed as an
// for diagnosing substitutions.
inline tree
make_subst (tree tmpl, tree args)
{
  tree subst = tree_cons (NULL_TREE, args, NULL_TREE);
  TREE_TYPE (subst) = DECL_TEMPLATE_PARMS (tmpl);
  return subst;
}

} // namesapce

// Emit diagnostics detailing the failure ARGS to satisfy the constraints
// of the template declaration, TMPL.
void
diagnose_constraint_failure (location_t loc, tree tmpl, tree args)
{
  inform (loc, "  constraints not satisfied %S", make_subst (tmpl, args));

  // Diagnose the constraints by recursively decomposing and
  // evaluating the template requirements.
  tree reqs = CI_SPELLING (DECL_CONSTRAINTS (tmpl));
  diagnose_requirements (loc, reqs, args);
}


