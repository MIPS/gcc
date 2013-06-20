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

// Algorithms and data structures for the decomposition and validation
// of logical formulas.

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

#include <list>

namespace {

// Helper algorithms 

// Increment iter distance(first, last) times.
template<typename I1, typename I2, typename I3>
  I1 next_by_distance (I1 iter, I2 first, I3 last)
  {
    for ( ; first != last; ++first, ++iter) 
      ;
    return iter;
  }


// -------------------------------------------------------------------------- //
// Term List
//
// A term list is a list of propositions in the constraint language. It is
// used to maintain the lists of assumptions and conclusions in a proof goal.
//
// Each term list maintains an iterator that refers to the current term. This
// can be used by various tactics to support iteration and stateful
// manipulation of the list.
struct term_list : std::list<tree>
{
  term_list ();
  term_list (const term_list &x);
  term_list& operator= (const term_list &x);

  tree       current_term ()       { return *current; }
  const_tree current_term () const { return *current; }


  void insert (tree t);
  tree erase ();

  void start ();
  void next ();
  bool done() const;

  iterator current;
};

inline
term_list::term_list ()
  : std::list<tree> (), current (end ())
{ }

inline
term_list::term_list (const term_list &x)
  : std::list<tree> (x)
  , current (next_by_distance (begin (), x.begin (), x.current))
{ }

inline term_list&
term_list::operator= (const term_list &x)
{
  std::list<tree>::operator=(x);
  current = next_by_distance (begin (), x.begin (), x.current);
  return *this;
}

// Insert the term T into the list before the current position, making
// this term current.
inline void
term_list::insert (tree t)
{
  current = std::list<tree>::insert (current, t);
}

// Remove the current term form the list, repositioning to the term
// following the removed term. Note that the new position could be past 
// the end of the list.
//
// The removed term is returned.
inline tree
term_list::erase ()
{
  tree t = *current;
  current = std::list<tree>::erase (current);
  return t;
}

// Initialize the current term to the first in the list.
inline void
term_list::start ()
{
  current = begin ();
}

// Advance to the next term in the list.
inline void
term_list::next ()
{
  ++current;
}

// Returns true when the current position is past the end.
inline bool
term_list::done () const
{
  return current == end ();
}


// -------------------------------------------------------------------------- //
// Proof Goal
//
// A goal (or subgoal) models a sequent of the form 'A |- C' where A and C are
// lists of assumptions and conclusions written as propositions in the
// constraint language (i.e., lists of trees).
struct proof_goal
{
  term_list assumptions;
  term_list conclusions;
};


// -------------------------------------------------------------------------- //
// Proof State
//
// A proof state owns a list of goals and tracks the current sub-goal.
// The class also provides facilities for managing subgoals and constructing
// term lists.
struct proof_state : std::list<proof_goal>
{
  proof_state ();

  iterator branch (iterator i);
};

// An alias for proof state iterators.
typedef proof_state::iterator goal_iterator;

// Initialize the state with a single empty goal, and set that goal as the
// current subgoal.
inline
proof_state::proof_state ()
  : std::list<proof_goal> (1)
{ }


// Branch the current goal by creating a new subgoal, returning a reference to
// the new object. This does not update the current goal.
inline proof_state::iterator
proof_state::branch (iterator i)
{
  return insert (++i, *i);
}


// -------------------------------------------------------------------------- //
// Logical Rules
//
// These functions modify the current state and goal by decomposing
// logical expressions using the logical rules of sequent calculus for
// first order logic.
//
// Note that in each decomposition rule, the term T has been erased
// from term list before the specific rule is applied.

// Left-and logical rule.
//
//  Gamma, P, Q |- Delta
//  -------------------------
//  Gamma, P and Q |- Delta
inline void 
left_and (proof_state &, goal_iterator i, tree t)
{
  gcc_assert (TREE_CODE (t) == TRUTH_ANDIF_EXPR);
  
  // Insert the operands into the current branch. Note that the
  // final order of insertion is left-to-right.
  term_list &l = i->assumptions;
  l.insert (TREE_OPERAND (t, 1));
  l.insert (TREE_OPERAND (t, 0));
}

// Left-or logical rule.
//
//  Gamma, P |- Delta    Gamma, Q |- Delta
//  -----------------------------------------
//  Gamma, P or Q |- Delta
inline void
left_or (proof_state &s, goal_iterator i, tree t)
{
  gcc_assert (TREE_CODE (t) == TRUTH_ORIF_EXPR);

  // Branch the current subgoal.
  goal_iterator j = s.branch (i);
  term_list &l1 = i->assumptions;
  term_list &l2 = j->assumptions;

  // Insert operands into the different branches.
  l1.insert (TREE_OPERAND (t, 0));
  l2.insert (TREE_OPERAND (t, 1));
}

// Right-and logical rule.
//
//  Gamma |- P, Delta    Gamma |- Q, Delta
//  -----------------------------------------
//  Gamma |- P and Q, Delta
inline void
right_and (proof_state &s, goal_iterator i, tree t)
{
  gcc_assert (TREE_CODE (t) == TRUTH_ORIF_EXPR);

  // Branch the current subgoal.
  goal_iterator j = s.branch (i);
  term_list &l1 = i->conclusions;
  term_list &l2 = j->conclusions;

  // Insert operands into the different branches.
  l1.insert (TREE_OPERAND (t, 0));
  l2.insert (TREE_OPERAND (t, 1));
}

// Right-or logical rule.
//
//  Gamma |- P, Q, Delta
//  ------------------------
//  Gamma |- P or Q, Delta
inline void
right_or (proof_state &, goal_iterator i, tree t)
{
  gcc_assert (TREE_CODE (t) == TRUTH_ANDIF_EXPR);
  
  // Insert the operands into the current branch. Note that the
  // final order of insertion is left-to-right.
  term_list &l = i->conclusions;
  l.insert (TREE_OPERAND (t, 1));
  l.insert (TREE_OPERAND (t, 0));
}


// -------------------------------------------------------------------------- //
// Decomposition
//
// The following algorithms decompose expressions into sets of atomic
// propositions.
//
// These decomposition strategies do not branch conclusions so that
// only a single term exists on the right side. These algorithms decompose
// expressions until we have a set of atomic sequent that contain no
// more logical expressions.

// Left decomposition.
// Continually decompose assumptions until there are no terms in any
// subgoal that can be further decomposed.

void
decompose_left_term (proof_state &s, goal_iterator i)
{
  term_list &l = i->assumptions;
  tree t = l.current_term ();
  switch (TREE_CODE (t))
    {
    case TRUTH_ANDIF_EXPR:
      left_and (s, i, l.erase ());
      break;
    case TRUTH_ORIF_EXPR:
      left_or (s, i, l.erase ());
      break;
    default:
      l.next ();
      break;
    }
}

void 
decompose_left_goal (proof_state &s, goal_iterator i)
{
  term_list& l = i->assumptions;
  l.start ();
  while (!l.done ())
    decompose_left_term (s, i);
}

inline void
decompose_left (proof_state& s)
{
  goal_iterator iter = s.begin ();
  goal_iterator end = s.end ();
  for ( ; iter != end; ++iter)
    decompose_left_goal (s, iter);
}


// -------------------------------------------------------------------------- //
// Term Extraction
//
// Extract a list of term lists from a proof state, and return it as a
// a tree (a vector of vectors).

// Returns a vector of terms from the given term list.
tree
extract_terms (term_list& l)
{
  tree result = make_tree_vec (l.size());
  term_list::iterator iter = l.begin();
  term_list::iterator end = l.end();
  for (int n = 0; iter != end; ++iter, ++n)
    TREE_VEC_ELT (result, n) = *iter;
  return result;
}

// Extract the assumption vector from the proof state s.
inline tree
extract_assumptions (proof_state& s)
{
  tree result = make_tree_vec (s.size ());
  goal_iterator iter = s.begin ();
  goal_iterator end = s.end ();
  for (int n = 0; iter != end; ++iter, ++n)
    TREE_VEC_ELT (result, n) = extract_terms (iter->assumptions);
  return result;
}

} // namespace


// Decompose the required expression T into a constraint set: a
// vector of vectors containing only atomic propositions.
tree
decompose_assumptions (tree t)
{
  // Create a proof state, and insert T as the sole assumption.
  proof_state s;
  term_list &l = s.begin ()->assumptions;
  l.insert (t);
  
  // Decompose the expression into a constraint set, and then
  // extract the terms for the AST.
  decompose_left (s);
  return extract_assumptions (s);
}


// -------------------------------------------------------------------------- //
// Subsumption and Entailment
//
// The following framework implements the subsumption and entailment
// logic used for overload resolution and lookup.

namespace {

bool subsumes_prop(tree, tree);
bool subsumes_atom(tree, tree);
bool subsumes_and(tree, tree);
bool subsumes_or(tree, tree);

// Returns true if the assumption A matches the conclusion C. This
// is generally the case when A and C have the same syntax.
//
// TODO: Implement special cases for:
//    * __is_same_as |- __is_convertible_to
//    * __is_same_as |- __is_derived_from
//    * Any other built-in predicates?
bool
match_terms (tree a, tree c)
{
  // TODO: Add special cases for __is_same, __is_convertible, 
  // and __is_base_of.
  return cp_tree_equal (a, c);
}

// Returns true if the list of assumptions AS subsume the atomic 
// proposition C. This is the case when we can find a proposition in
// AS that entails the conclusion C.
bool
subsumes_atom (tree as, tree c)
{
  for (int i = 0; i < TREE_VEC_LENGTH (as); ++i)
    if (match_terms (TREE_VEC_ELT (as, i), c))
      return true;
  return false;
}

// Returns true when both operands of C are subsumed by the assumptions AS.
inline bool 
subsumes_and (tree as, tree c)
{
  tree l = TREE_OPERAND (c, 0);
  tree r = TREE_OPERAND (c, 1);
  return subsumes_prop (as, l) && subsumes_prop (as, r);
}

// Returns true when either operand of C is subsumed by the assumptions AS.
inline bool
subsumes_or (tree as, tree c)
{
  tree l = TREE_OPERAND (c, 0);
  tree r = TREE_OPERAND (c, 1);
  return subsumes_prop (as, l) || subsumes_prop (as, r);
}

// Returns true when the list of assumptions AS subsumes the 
// concluded proposition C.
//
// This is a simple recursive descent on C, matching against
// propositions in the assumption list AS.
bool 
subsumes_prop (tree as, tree c)
{
  switch (TREE_CODE (c))
    {
    case TRUTH_ANDIF_EXPR:
      return subsumes_and (as, c);
    case TRUTH_ORIF_EXPR:
      return subsumes_or (as, c);
    default:
      return subsumes_atom (as, c);
    }
}

// Returns true the LEFT constraints subsume the RIGHT constraints.
// This is done by checking that the RIGHT requirements follow from
// each of the LEFT subgoals.
bool
subsumes_constraints (tree left, tree right)
{
  gcc_assert (check_constraint_info (left));
  gcc_assert (check_constraint_info (right));

  // Check that the required expression in RIGHT is subsumed by each
  // subgoal in the assumptions of LEFT.
  tree as = CI_ASSUMPTIONS (left);
  tree c = reduce_requirements (CI_REQUIREMENTS (right));
  for (int i = 0; i < TREE_VEC_LENGTH (as); ++i)
    if (!subsumes_prop (TREE_VEC_ELT (as, i), c))
      return false;
  return true;
}

} // end namespace


// Returns true the LEFT constraints subsume the RIGHT constraints. Note
// that subsumption is a reflexive relation (e.g., <=)
bool
subsumes (tree left, tree right)
{
  if (left == right)
    return true;
  if (!left)
    return false;
  if (!right)
    return true;
  return subsumes_constraints (left, right);
}


