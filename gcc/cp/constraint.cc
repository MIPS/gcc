/* Processing rules for constraints.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "print-tree.h"
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

// Transform the list of expressions in the T into a conjunction
// of requirements. T must be a TREE_VEC.
tree 
conjoin_requirements (tree t)
{
  gcc_assert (TREE_CODE (t) == TREE_VEC);
  tree r = NULL_TREE;
  for (int i = 0; i < TREE_VEC_LENGTH (t); ++i)
    r = conjoin_requirements (r, TREE_VEC_ELT (t, i));
  return r;
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
      // Get the next template overload.
      tree tmpl = OVL_CURRENT (p);
      if (TREE_CODE (tmpl) != TEMPLATE_DECL)
        continue;

      // Don't try to deduce checks for non-concept-like. We often
      // end up trying to resolve constraints in functional casts
      // as part of a post-fix expression. We can save time and
      // headaches by not instantiating those declarations.
      //
      // NOTE: This masks a potential error, caused by instantiating
      // non-deduced contexts using placeholder arguments.
      tree fn = DECL_TEMPLATE_RESULT (tmpl);
      if (DECL_ARGUMENTS (fn))
        continue;
      if (!DECL_DECLARED_CONCEPT_P (fn))
        continue;

      // Remember the candidate if we can deduce a substitution.
      ++processing_template_decl;
      tree parms = TREE_VALUE (DECL_TEMPLATE_PARMS (tmpl));
      if (tree subst = coerce_template_parms (parms, args, tmpl))
        if (subst != error_mark_node)
          cands = tree_cons (subst, fn, cands);
      --processing_template_decl;
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

// Given a call expression to a concept, possibly including a placeholder
// argument, deduce the concept being checked and the prototype paraemter.
// Returns true if the constraint and prototype can be deduced and false
// otherwise. Note that the CHECK and PROTO arguments are set to NULL_TREE
// if this returns false.
bool
deduce_constrained_parameter (tree call, tree& check, tree& proto)
{
  // Resolve the constraint check to deduce the declared parameter.
  if (tree info = resolve_constraint_check (call))
    {
      // Get function and argument from the resolved check expression and
      // the prototype parameter. Note that if the first argument was a
      // pack, we need to extract the first element ot get the prototype.
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
static tree reduce_requires    (tree);
static tree reduce_expr_req    (tree);
static tree reduce_type_req    (tree);
static tree reduce_nested_req  (tree);
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

    case REQUIRES_EXPR:
      return reduce_requires (t);

    case EXPR_REQ:
      return reduce_expr_req (t);

    case TYPE_REQ:
      return reduce_type_req (t);

    case NESTED_REQ:
      return reduce_nested_req (t);

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
  error_at (EXPR_LOCATION (t), "invalid requirement");
  inform (EXPR_LOCATION (t), "did you mean %qE", c);
  return c;
}


// Reduce an expression requirement as a conjunction of its
// individual constraints.
tree
reduce_expr_req (tree t) 
{
  tree r = NULL_TREE;
  for (tree l = TREE_OPERAND (t, 0); l; l = TREE_CHAIN (l))
    r = conjoin_requirements (r, reduce_expr (TREE_VALUE (l)));
  return r;
}

// Reduce a type requirement by returing its underlying
// constraint.
tree
reduce_type_req (tree t) 
{
  return TREE_OPERAND (t, 0);
}

// Reduce a nested requireemnt by returing its only operand.
tree
reduce_nested_req (tree t) 
{
  return TREE_OPERAND (t, 0);
}

// Reduce a requires expr by reducing each requirement in turn,
// rewriting the list of requirements so that we end up with a
// list of expressions, some of which may be conjunctions.
tree
reduce_requires (tree t)
{
  for (tree l = TREE_OPERAND (t, 1); l; l = TREE_CHAIN (l))
    TREE_VALUE (l) = reduce_expr (TREE_VALUE (l));
  return t;
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
        lhs = conjoin_requirements (lhs, rhs);
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

// -------------------------------------------------------------------------- //
// Constraint Semantic Processing
//
// The following functions are called by the parser and substitution rules
// to create and evaluate constraint-related nodes.

// Create a constraint-info node from the specified requirements.
tree 
make_constraints (tree reqs)
{
  // No requirements == no constraints
  if (!reqs)
    return NULL_TREE;

  // Reduce the requirements into a single expression of constraints.
  tree expr = reduce_requirements (reqs);
  if (expr == error_mark_node)
    return error_mark_node;

  // Decompose those expressions into lists of lists of atomic
  // propositions.
  tree assume = decompose_assumptions (expr);

  // Build the constraint info.
  tree_constraint_info *cinfo = 
    (tree_constraint_info *)make_node (CONSTRAINT_INFO);
  cinfo->spelling = reqs;
  cinfo->requirements = expr;
  cinfo->assumptions = assume;
  return (tree)cinfo;
}

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

// Returns a conjunction of shorthand requirements for the template
// parameter list PARMS. Note that the requirements are stored in
// the TYPE of each tree node.
tree
get_shorthand_requirements (tree parms)
{
  tree reqs = NULL_TREE;
  parms = INNERMOST_TEMPLATE_PARMS (parms);
  for (int i = 0; i < TREE_VEC_LENGTH (parms); ++i)
    {
      tree parm = TREE_VEC_ELT (parms, i);
      reqs = conjoin_requirements(reqs, TREE_TYPE (parm));
    }
  return reqs;
}

// Finish the template requirement, EXPR, by translating it into
// a constraint information record.
tree
finish_template_requirements (tree expr)
{
  if (expr == error_mark_node)
    return NULL_TREE;
  else
    return make_constraints (expr);
}

tree
build_requires_expr (tree parms, tree reqs)
{
  // Modify the declared parameters by removing their context (so they
  // don't refer to the enclosing scope), and marking them constant (so
  // we can actually check constexpr properties).
  for (tree p = parms; p && !VOID_TYPE_P (TREE_VALUE (p)); p = TREE_CHAIN (p))
    {
      tree parm = TREE_VALUE (p);
      DECL_CONTEXT (parm) = NULL_TREE;
      TREE_CONSTANT (parm) = true;
    }

  // Build the node.
  tree r = build_min (REQUIRES_EXPR, boolean_type_node, parms, reqs);
  TREE_SIDE_EFFECTS (r) = false;
  TREE_CONSTANT (r) = true;
  return r;
}

// Evaluate an instantiatd requires expr, returning the truth node
// only when all sub-requirements have evaluated to true.
tree
eval_requires_expr (tree reqs)
{
  for (tree t = reqs ; t; t = TREE_CHAIN (t)) {
    tree r = TREE_VALUE (t);
    r = fold_non_dependent_expr (r);
    r = maybe_constant_value (r);
    if (r != boolean_true_node)
      return boolean_false_node;
  }
  return boolean_true_node;
}

// Finish a requires expression, returning a node wrapping the parameters,
// PARMS, and the list of requirements REQS.
tree
finish_requires_expr (tree parms, tree reqs)
{
  if (processing_template_decl)
    return build_requires_expr (parms, reqs);
  else
    return eval_requires_expr (reqs);
}

// Construct a unary expression that evaluates properties of the
// expression or type T, and has a boolean result type.
static inline tree
build_check_expr (tree_code c, tree t)
{
  tree r = build_min (c, boolean_type_node, t);
  TREE_SIDE_EFFECTS (r) = false;
  TREE_READONLY (r) = true;
  TREE_CONSTANT (r) = true;
  return r;
}

// Finish a syntax requirement, constructing a list embodying a sequence
// of checks for the validity of EXPR and TYPE, the convertibility of
// EXPR to TYPE, and the expression properties specified in SPECS.
tree
finish_expr_requirement (tree expr, tree type, tree specs)
{
  gcc_assert (processing_template_decl);

  // Build a list of checks, starting with the valid expression.
  tree result = tree_cons (NULL_TREE, finish_validexpr_expr (expr), NULL_TREE);

  // If a type requirement was provided, build the result type checks.
  if (type)
    {
      // If the type is dependent, ensure that it can be validly
      // instantiated.
      //
      // NOTE: We can also disregard checks that result in the template
      // parameter.
      if (dependent_type_p (type))
        {
          tree treq = finish_type_requirement (type);
          result = tree_cons (NULL_TREE, treq, result);
        }

      // Ensure that the result of the expression can be converted to
      // the result type.
      tree decl_type = finish_decltype_type (expr, false, tf_none);
      tree creq = finish_trait_expr (CPTK_IS_CONVERTIBLE_TO, decl_type, type);
      result = tree_cons (NULL_TREE, creq, result);
    }

  // If constraint specifiers are present, make them part of the
  // list of constraints.
  if (specs)
    {
      TREE_CHAIN (tree_last (specs)) = result;
      result = specs;
    }

  // Finally, construct the syntactic requirement.
  return build_check_expr (EXPR_REQ, nreverse (result));
}

// Finish a simple syntax requirement, returning a node representing
// a check that EXPR is a valid expression.
tree
finish_expr_requirement (tree expr)
{
  gcc_assert (processing_template_decl);
  tree req = finish_validexpr_expr (expr);
  tree reqs = tree_cons (NULL_TREE, req, NULL_TREE);
  return build_check_expr (EXPR_REQ, reqs);
}

// Finish a type requirement, returning a node representing a check
// that TYPE will result in a valid type when instantiated.
tree
finish_type_requirement (tree type)
{
  gcc_assert (processing_template_decl);
  tree req = finish_validtype_expr (type);
  return build_check_expr (TYPE_REQ, req);
}

tree
finish_nested_requirement (tree expr)
{
  gcc_assert (processing_template_decl);
  return build_check_expr (NESTED_REQ, expr);
}

// Finish a constexpr requirement, returning a node representing a
// check that EXPR, when instantiated, may be evaluated at compile time.
tree
finish_constexpr_requirement (tree expr)
{
  gcc_assert (processing_template_decl);
  return finish_constexpr_expr (expr);
}

// Finish the noexcept requirement by constructing a noexcept 
// expression evaluating EXPR.
tree
finish_noexcept_requirement (tree expr)
{
  gcc_assert (processing_template_decl);
  return finish_noexcept_expr (expr, tf_none);
}

// Returns the true or false node depending on the truth value of B.
static inline tree
truth_node (bool b)
{
  return b ? boolean_true_node : boolean_false_node;
}

// Returns a finished validexpr-expr. Returns the true or false node
// depending on whether EXPR denotes a valid expression. This is the case
// when the expression has been successfully type checked.
//
// When processing a template declaration, the result is an expression 
// representing the check.
tree
finish_validexpr_expr (tree expr)
{
  if (processing_template_decl)
    return build_check_expr (VALIDEXPR_EXPR, expr);
  return truth_node (expr && expr != error_mark_node);
}

// Returns a finished validtype-expr. Returns the true or false node
// depending on whether T denotes a valid type name.
//
// When processing a template declaration, the result is an expression 
// representing the check.
//
// FIXME: Semantics need to be aligned with the new version of the
// specificaiton (i.e., we must be able to invent a function and
// perform argument deduction against it).
tree
finish_validtype_expr (tree type)
{
  if (is_auto (type))
    {
      sorry ("%<auto%< not supported in result type constraints\n");
      return error_mark_node;
    }

  if (processing_template_decl)
    return build_check_expr (VALIDTYPE_EXPR, type);
  return truth_node (type && TYPE_P (type));
}

// Returns a finished constexpr-expr. Returns the true or false node
// depending on whether the expression T may be evaluated at compile
// time.
//
// When processing a template declaration, the result is an expression 
// representing the check.
tree
finish_constexpr_expr (tree expr)
{
  if (processing_template_decl)
    return build_check_expr (CONSTEXPR_EXPR, expr);

  // TODO: Actually check that the expression can be constexpr
  // evaluatd. 
  // 
  // return truth_node (potential_constant_expression (expr));
  sorry ("constexpr requirement");
  return NULL_TREE;
}

// Check that a constrained friend declaration function declaration,
// FN, is admissable. This is the case only when the declaration depends 
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
  tree parms = DECL_ARGUMENTS (fn);
  tree result = TREE_TYPE (TREE_TYPE (fn));
  if (!(parms && uses_template_parms (parms)) && !uses_template_parms (result))
    {
      error ("constrained friend does not depend on template parameters");
      return;
    }
}

namespace {
  // Build a new call expression, but don't actually generate a new 
  // function call. We just want the tree, not the semantics.
inline tree
build_call_check (tree id)
{
  ++processing_template_decl;
  vec<tree, va_gc> *fargs = make_tree_vector();
  tree call = finish_call_expr (id, &fargs, false, false, tf_none);
  --processing_template_decl;
  return call;
}
} // namespace

// Construct a concept check for the overloaded function, where the
// template arguments are the list given by ARG and REST. That is, it
// build the call expression OVL<ARG, REST>(). If REST is null, then
// the resulting constraint is OVL<ARG>().
//
// TODO: Extend this to take a variable concept also.
tree
build_concept_check (tree ovl, tree arg, tree rest) 
{
  gcc_assert (TREE_CODE (ovl) == OVERLOAD);
  gcc_assert (rest ? TREE_CODE (rest) == TREE_VEC : true);

  // Build a template-id that acts as the call target using OVL as
  // the template and ARG as the only explicit argument.
  int n = rest ? TREE_VEC_LENGTH (rest) : 0;
  tree targs = make_tree_vec (n + 1);
  TREE_VEC_ELT (targs, 0) = arg;
  if (rest)
    for (int i = 0; i < n; ++i)
      TREE_VEC_ELT (targs, i + 1) = TREE_VEC_ELT (rest, i);
  SET_NON_DEFAULT_TEMPLATE_ARGS_COUNT (targs, n + 1);
  tree id = lookup_template_function (ovl, targs);
  return build_call_check (id);
}

// Returns a TYPE_DECL that contains sufficient information to build
// a template parameter of the same kind as PROTO and constrained
// by the concept declaration FN. PROTO is saved as the initializer of
// the new type decl, and the constraining function is saved in
// DECL_SIZE_UNIT.
//
// If specified ARGS provides additional arguments to the constraint
// check. These are stored in the DECL_SIZE field.
tree
build_constrained_parameter (tree fn, tree proto, tree args) 
{
  tree name = DECL_NAME (fn);
  tree type = TREE_TYPE (proto);
  tree decl = build_decl (input_location, TYPE_DECL, name, type);
  DECL_INITIAL (decl) = proto;  // Describing parameter
  DECL_SIZE_UNIT (decl) = fn;   // Constraining function declaration
  DECL_SIZE (decl) = args;      // Extra template arguments.
  return decl;
}

// Create a requirement expression for the given DECL that evaluates the
// requirements specified by CONSTR, a TYPE_DECL that contains all the 
// information necessary to build the requirements (see finish_concept_name 
// for the layout of that TYPE_DECL).
//
// Note that the constraints are neither reduced nor decomposed. That is
// done only after the requires clause has been parsed (or not).
tree
finish_shorthand_requirement (tree decl, tree constr)
{
  // No requirements means no constraints.
  if (!constr)
    return NULL_TREE;

  tree proto = DECL_INITIAL (constr); // The prototype declaration
  tree con = DECL_SIZE_UNIT (constr); // The concept declaration
  tree args = DECL_SIZE (constr);     // Extra template arguments

  // If the parameter declaration is variadic, but the concept is not 
  // then we need to apply the concept to every element in the pack.
  bool is_proto_pack = template_parameter_pack_p (proto);
  bool is_decl_pack = template_parameter_pack_p (decl);
  bool apply_to_all_p = is_decl_pack && !is_proto_pack;

  // Get the argument and overload used for the requirement. Adjust
  // if we're going to expand later.
  tree arg = template_parm_to_arg (build_tree_list (NULL_TREE, decl));
  if (apply_to_all_p)
      arg = PACK_EXPANSION_PATTERN (TREE_VEC_ELT (ARGUMENT_PACK_ARGS (arg), 0));

  // Build the concept check. If it the constraint needs to be applied
  // to all elements of the parameter pack, then expand make the constraint
  // an expansion.
  tree ovl = build_overload (DECL_TI_TEMPLATE (con), NULL_TREE);
  tree check = build_concept_check (ovl, arg, args);
  if (apply_to_all_p)
    {
      check = make_pack_expansion (check);

      // Set the type to indicate that this expansion will get special
      // treatment during instantiation. 
      //
      // TODO: Maybe this should be a different kind of node... one that
      // has all the same properties as a pack expansion, but has a definite
      // expansion when instantiated as part of an expression.
      //
      // As of now, this is a hack.
      TREE_TYPE (check) = boolean_type_node;
    }

  return check;
 }

// -------------------------------------------------------------------------- //
// Substitution Rules
//
// The following functions implement substitution rules for constraints.

namespace {
// In an unevaluated context, the substitution of parm decls are not
// properly chained during substitution. Do that here.
tree
fix_local_parms (tree sparms)
{
  if (!sparms)
    return sparms;

  tree p = TREE_CHAIN (sparms);
  tree q = sparms;
  while (p && TREE_VALUE (p) != void_type_node)
    {
      DECL_CHAIN (TREE_VALUE (q)) = TREE_VALUE (p);
      q = p;
      p = TREE_CHAIN (p);
    }
  return sparms;
}

// Register local specializations for each of tparm and the corresponding
// sparm. This is a helper function for tsubst_requires_expr.
void
declare_local_parms (tree tparms, tree sparms)
{
  tree s = TREE_VALUE (sparms);
  for (tree p = tparms; p && !VOID_TYPE_P (TREE_VALUE (p)); p = TREE_CHAIN (p))
    {
      tree t = TREE_VALUE (p);
      if (DECL_PACK_P (t))
        {
          tree pack = extract_fnparm_pack (t, &s);
          register_local_specialization (pack, t);
        }
      else 
        {
          register_local_specialization (s, t);
          s = TREE_CHAIN (s);
        }      
    }
}

// Substitute ARGS into the parameter list T, producing a sequence of
// local parameters (variables) in the current scope.
tree
tsubst_local_parms (tree t,
                    tree args, 
                    tsubst_flags_t complain, 
                    tree in_decl)
{
  tree r = fix_local_parms (tsubst (t, args, complain, in_decl));
  if (r == error_mark_node)
    return error_mark_node;

  // Register the instantiated args as local parameters.
  if (t)
    declare_local_parms (t, r);
  
  return r;
}

// Substitute ARGS into the requirement body (list of requirements), T.
tree
tsubst_requirement_body (tree t, tree args, tree in_decl)
{
  cp_unevaluated guard;
  tree r = NULL_TREE;
  while (t)
    {
      // If any substitutions fail, then this is equivalent to
      // returning false.
      tree e = tsubst_expr (TREE_VALUE (t), args, tf_none, in_decl, false);
      if (e == error_mark_node)
        e = boolean_false_node;
      r = tree_cons (NULL_TREE, e, r);
      t = TREE_CHAIN (t);
    }
  return r;
}
} // namespace

// Substitute ARGS into the requires expression T.
tree
tsubst_requires_expr (tree t, tree args, tsubst_flags_t complain, tree in_decl)
{
  local_specialization_stack stack;
  tree p = tsubst_local_parms (TREE_OPERAND (t, 0), args, complain, in_decl);
  tree r = tsubst_requirement_body (TREE_OPERAND (t, 1), args, in_decl);
  return finish_requires_expr (p, r);
}

// Substitute ARGS into the valid-expr expression T.
tree
tsubst_validexpr_expr (tree t, tree args, tree in_decl)
{
  tree r = tsubst_expr (TREE_OPERAND (t, 0), args, tf_none, in_decl, false);
  return finish_validexpr_expr (r);
}

// Substitute ARGS into the valid-type expression T.
tree
tsubst_validtype_expr (tree t, tree args, tree in_decl)
{
  tree r = tsubst (TREE_OPERAND (t, 0), args, tf_none, in_decl);
  return finish_validtype_expr (r);
}

// Substitute ARGS into the constexpr expression T.
tree
tsubst_constexpr_expr (tree t, tree args, tree in_decl)
{
  tree r = tsubst_expr (TREE_OPERAND (t, 0), args, tf_none, in_decl, false);
  return finish_constexpr_expr (r);
}

// Substitute ARGS into the expr requirement T. Note that a requirement 
// node is instantiated from a non-reduced context (e.g., static_assert).
tree
tsubst_expr_req (tree t, tree args, tree in_decl)
{
  tree r = NULL_TREE;
  for (tree l = TREE_OPERAND (t, 0); l; l = TREE_CHAIN (l))
    {
      tree e = tsubst_expr (TREE_VALUE (l), args, tf_none, in_decl, false);
      r = conjoin_requirements (r, e);
    }
  return r;
}

// Substitute ARGS into the type requirement T. Note that a requirement 
// node is instantiated from a non-reduced context (e.g., static_assert).
tree
tsubst_type_req (tree t, tree args, tree in_decl)
{
  return tsubst_expr (TREE_OPERAND (t, 0), args, tf_none, in_decl, false);
}

// Substitute ARGS into the nested requirement T. Note that a requirement 
// node is instantiated from a non-reduced context (e.g., static_assert).
tree
tsubst_nested_req (tree t, tree args, tree in_decl)
{
  return tsubst_expr (TREE_OPERAND (t, 0), args, tf_none, in_decl, false);
}

// Substitute the template arguments ARGS into the requirement
// expression REQS. Errors resulting from substitution are not
// diagnosed.
tree
instantiate_requirements (tree reqs, tree args)
{
  return tsubst_expr (reqs, args, tf_none, NULL_TREE, false);
}

// -------------------------------------------------------------------------- //
// Constraint Satisfaction
//
// The following functions are responsible for the instantiation and
// evaluation of constraints.

namespace {
// Returns true if the requirements expression REQS is satisfied
// and false otherwise. The requirements are checked by simply 
// evaluating REQS as a constant expression.
static inline bool
check_requirements (tree reqs)
{
  // Reduce any remaining TRAIT_EXPR nodes before evaluating.
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
  // If any arguments are dependent, then we can't check the
  // requirements. Just return true.
  if (uses_template_parms (args))
    return true;

  // Instantiate and evaluate the requirements.
  reqs = instantiate_requirements (reqs, args);
  if (reqs == error_mark_node)
    return false;
  return check_requirements (reqs);
}
} // namespace

// Check the instantiated declaration constraints.
bool
check_constraints (tree cinfo)
{
  // No constraints? Satisfied.
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

  // Dependent arguments? Satisfied. They won't reduce to true or false.
  if (uses_template_parms (args))
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

// -------------------------------------------------------------------------- //
// Constraint Relations
//
// Interfaces for determining equivalency and ordering of constraints.

// Returns true when A and B are equivalent constraints.
bool
equivalent_constraints (tree a, tree b)
{
  if (a == b)
    return true;
  else
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
      case CPTK_IS_CONVERTIBLE_TO:
        inform (loc, "  %qT is not convertible to %qT", t1, t2);
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

// Diagnose specific constraint failures.
void
diagnose_requires (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;

  ++processing_template_decl;
  tree subst = instantiate_requirements (t, args);
  --processing_template_decl;

  // Print the header for the requires expression.
  tree parms = TREE_OPERAND (subst, 0);
  if (!VOID_TYPE_P (TREE_VALUE (parms)))
    inform (loc, "  requiring syntax with values %Z", TREE_OPERAND (subst, 0));

  // Create a new local specialization binding for the arguments. 
  // This lets us instantiate sub-expressions separately from the 
  // requires clause.
  local_specialization_stack locals;
  declare_local_parms (TREE_OPERAND (t, 0), TREE_OPERAND (subst, 0));

  // Iterate over the sub-requirements and try instantiating each.
  for (tree l = TREE_OPERAND (t, 1); l; l = TREE_CHAIN (l))
    diagnose_node (loc, TREE_VALUE (l), args);
}

static void
diagnose_validexpr (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;
  inform (loc, "    %qE is not a valid expression", TREE_OPERAND (t, 0));
}

static void
diagnose_validtype (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;

  // Substitute into the qualified name.
  tree name = TREE_OPERAND (t, 0);
  if (tree cxt = TYPE_CONTEXT (name))
    {
      tree id = TYPE_IDENTIFIER (name);
      cxt = tsubst (cxt, args, tf_none, NULL_TREE);
      name = build_qualified_name (NULL_TREE, cxt, id, false);
      inform (loc, "    %qE does not name a valid type", name);
    }
  else
    {
      inform (loc, "    %qT does not name a valid type", name);
    }
}

static void
diagnose_constexpr (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;
  inform (loc, "    %qE is not a constant expression", TREE_OPERAND (t, 0));
}

static void
diagnose_noexcept (location_t loc, tree t, tree args)
{
  if (check_requirements (t, args))
    return;
  inform (loc, "    %qE propagates exceptions", TREE_OPERAND (t, 0)); 
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
      // TODO: Design better diagnostics for dijunctions.
      diagnose_other (loc, t, args);
      break;

    case TRAIT_EXPR:
      diagnose_trait (loc, t, args);
      break;
    
    case CALL_EXPR:
      diagnose_call (loc, t, args);
      break;

    case REQUIRES_EXPR:
      diagnose_requires (loc, t, args);
      break;

    case VALIDEXPR_EXPR:
      diagnose_validexpr (loc, t, args);
      break;
    
    case VALIDTYPE_EXPR:
      diagnose_validtype (loc, t, args);
      break;
    
    case CONSTEXPR_EXPR:
      diagnose_constexpr (loc, t, args);
      break;

    case NOEXCEPT_EXPR:
      diagnose_noexcept (loc, t, args);
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

} // namespace

// Emit diagnostics detailing the failure ARGS to satisfy the constraints
// of the template declaration, TMPL.
void
diagnose_constraints (location_t loc, tree tmpl, tree args)
{
  inform (loc, "  constraints not satisfied %S", make_subst (tmpl, args));

  // Diagnose the constraints by recursively decomposing and
  // evaluating the template requirements.
  tree reqs = CI_SPELLING (DECL_CONSTRAINTS (tmpl));
  diagnose_requirements (loc, reqs, args);
}


