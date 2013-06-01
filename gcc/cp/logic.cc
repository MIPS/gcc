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

#include <algorithm>
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

  iterator cur;
};

inline
term_list::term_list ()
  : std::list<tree> (), cur (end ())
{ }

inline
term_list::term_list (const term_list &x)
  : std::list<tree> (x), cur (::next_by_distance (begin (), x.begin (), x.cur))
{ }

inline term_list&
term_list::operator= (const term_list &x)
{
  std::list<tree>::operator=(x);
  cur = next_by_distance (begin (), x.begin (), x.cur);
  return *this;
}

// Insert the term t into the list at the current position, making
// this term current.
inline void
insert_term (term_list& l, tree t)
{
  l.cur = l.insert (l.cur, t);
}

// Remove the current term form the list, repositioning the current
// position at the next term in the list.
inline tree
remove_term (term_list& l)
{
  tree t = *l.cur;
  l.cur = l.erase (l.cur);
  return t;
}


// -------------------------------------------------------------------------- //
// Proof Goal
//
// A goal (or subgoal) models a sequent of the form 'A |- C' where A and C are
// lists of assumptions and conclusions written as propositions in the
// constraint language (i.e., lists of trees).
struct proof_goal
{
  term_list as; // Assumptions
  term_list cs; // Conclusions   
};

// Return the list of assumed terms for the goal g.
inline term_list &
assumptions (proof_goal &g) { return g.as; }

inline const term_list &
assumptions (const proof_goal &g) { return g.as; }

// Return the list of concluded terms for the goal g.
inline term_list &
conclusions (proof_goal &g) { return g.cs; }

inline const term_list &
conclusions (const proof_goal &g) { return g.cs; }


// -------------------------------------------------------------------------- //
// Proof State
//
// A proof state owns a list of goals and tracks the current sub-goal.
// The class also provides facilities for managing subgoals and constructing
// term lists.
struct proof_state : std::list<proof_goal>
{
  proof_state ();

  iterator cur;
};

// Initialize the state with a single empty goal, and set
// that goal as the current subgoal.
inline
proof_state::proof_state ()
  : std::list<proof_goal> (1), cur (begin ())
{ }


// Return the current goal. Provided for notational symmetry
inline proof_goal &
current_goal (proof_state &s) { return *s.cur; }

inline const proof_goal &
current_goal (const proof_state &s) { return *s.cur; }

// Return the current list of assumed terms.
inline term_list &
assumptions (proof_state &s) { return assumptions (current_goal (s)); }

inline const term_list &
assumptions (const proof_state &s) { return assumptions (current_goal (s)); }

// Return the current list of concluded terms.
inline term_list &
conclusions (proof_state &s) { return conclusions (current_goal (s)); }

inline const term_list &
conclusions (const proof_state &s) { return conclusions (current_goal (s)); }

// Return the current assumption.
inline tree
assumption (const proof_state &s) { return *assumptions (s).cur; }

// Return the current conclision.
inline tree
conclusion (const proof_state &s) { return *conclusions (s).cur; }

// Move to the next goal.
inline void
next_goal (proof_state &s) { ++s.cur; }

// Move to the next assumption.
inline void
next_assumption (proof_state &s) { ++assumptions (s).cur; }

// Move to the next conclusion.
inline void
next_conclusion (proof_state &s) { ++conclusions (s).cur; }


// -------------------------------------------------------------------------- //
// Term List Manipulation
//
// The following functions are used to manage the insertation and removal
// goals and terms in the proof state.

// Assume the term e in the current goal. The current assumption is set to the
// new term.
inline void
assume_term (proof_goal &g, tree e) { insert_term (assumptions (g), e); }

inline void 
assume_term (proof_state &s, tree e) { assume_term (current_goal (s), e); }

// Forget the current assumption, removing it from the context of the current
// goal. Returns the forgotten assumption.
inline tree
forget_term (proof_goal &g) { return remove_term (assumptions (g)); }

inline tree
forget_term (proof_state &s) { return forget_term (current_goal (s)); }

// Conclude the term e in the current goal. The current conclusion is set to 
// the new term.
inline void
conclude_term (proof_goal &g, tree e) { insert_term (conclusions (g), e); }

inline void
conclude_term (proof_state &s, tree e) { conclude_term (current_goal (s), e); }

// Ignore the current conclusion, removing it from the conclusions
// of the current goal. Returns the ignored conclusion.
inline tree
ignore_term (proof_goal &g) { return remove_term (conclusions (g)); }

inline tree
ignore_term (proof_state &s) { return ignore_term (current_goal (s)); }

// Branch the current goal by creating a new subgoal, returning a reference to
// the new objet. This does not update the current goal.
inline proof_goal &
branch_goal (proof_state &s)
{
  proof_state::iterator p = s.cur;
  p = s.insert (++p, *s.cur);
  return *p;
}


// -------------------------------------------------------------------------- //
// Debugging
//
// Helper functions for debugging the proof state.

void
debug (term_list &l)
{
  term_list::iterator i = l.begin ();
  term_list::iterator last = --l.end ();
  term_list::iterator end = l.end ();
  for ( ; i != end; ++i)
    {
      debug (*i);
      if (i != last)
        fprintf (stderr, " ");
    }
}

void
debug (proof_goal &g)
{
  debug (g.as);
  fprintf (stderr, " |- ");
  debug (g.cs);
}

void
debug (proof_state& s)
{
  proof_state::iterator i = s.begin ();
  proof_state::iterator end = s.end ();
  for ( ; i != end; ++i)
    {
      debug (*i);
      fprintf (stderr, "\n");
    }
}


// -------------------------------------------------------------------------- //
// Logical Rules
//
// These functions modify the current state and goal by decomposing
// logical expressions using the logical rules of sequent calculus for
// first order logic.


// And left logical rule.
inline void 
and_left (proof_state &s)
{
  gcc_assert (TREE_CODE (assumption (s)) == TRUTH_ANDIF_EXPR);
  tree t = forget_term (s);
  assume_term (s, TREE_OPERAND (t, 1));
  assume_term (s, TREE_OPERAND (t, 0));
}

// And right logical rule.
inline void
and_right (proof_state &s)
{
  gcc_assert (TREE_CODE (conclusion (s)) == TRUTH_ANDIF_EXPR);
  tree t = ignore_term (s);
  conclude_term (branch_goal (s), TREE_OPERAND (t, 1));
  conclude_term (current_goal (s), TREE_OPERAND (t, 0));
}

// Or left logical rule.
inline void
or_left (proof_state& s)
{
  gcc_assert (TREE_CODE (assumption (s)) == TRUTH_ORIF_EXPR);
  tree t = forget_term (s);
  assume_term (branch_goal (s), TREE_OPERAND (t, 1));
  assume_term (current_goal (s), TREE_OPERAND (t, 0));
}

// Or right logical rule.
inline void
or_right (proof_state &s)
{
  gcc_assert (TREE_CODE (conclusion (s)) == TRUTH_ORIF_EXPR);
  tree t = ignore_term (s);
  conclude_term (s, TREE_OPERAND (t, 1));
  conclude_term (s, TREE_OPERAND (t, 0));
}


// -------------------------------------------------------------------------- //
// Algorithms
//
// Generic algorithms for querying or manipulating a proof state.

// Apply fn for each goal and returns fn. The current goal is set prior to
// calling fn.
template<typename F>
  inline F
  for_each_goal (proof_state &s, F fn)
  {
    for (s.cur = s.begin (); s.cur != s.end (); ++s.cur)
      fn (s);
    return fn;
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
decompose_left_term (proof_state &s)
{
  tree e = assumption (s);
  if (TREE_CODE (e) == TRUTH_ANDIF_EXPR)
    and_left (s);
  else if (TREE_CODE (e) == TRUTH_ORIF_EXPR)
    or_left (s);
  else
    next_assumption (s);
}

void 
decompose_left_goal (proof_state &s)
{
  term_list& l = assumptions (s);
  for (l.cur = l.begin (); l.cur != l.end (); )
    decompose_left_term (s);
}

inline void
decompose_left (proof_state& s)
{
  for_each_goal (s, decompose_left_goal);
}

// Right decomposition.
// Continually decompose conclusions until there are no terms in any
// subgoal that can be further decomposed.

void
decompose_right_term (proof_state &s)
{
  tree e = conclusion (s);
  if (TREE_CODE (e) == TRUTH_ANDIF_EXPR)
    and_right (s);
  else if (TREE_CODE (e) == TRUTH_ORIF_EXPR)
    or_right (s);
  else
    next_conclusion (s);
}

void
decompose_right_goal (proof_state &s)
{
  term_list& l = conclusions (s);
  for (l.cur = l.begin (); l.cur != l.end (); )
    decompose_right_term (s);
}


inline void
decompose_right (proof_state& s)
{
  for_each_goal (s, decompose_right_goal);
}


// -------------------------------------------------------------------------- //
// Term Extraction
//
// Extract a list of term lists from a proof state, and return it as a
// a tree (a vetor of vectors).

// Returns a vector of terms from the given term list.
tree
extract_terms (term_list& l)
{
  tree result = make_tree_vec (l.size());
  term_list::iterator i = l.begin();
  term_list::iterator e = l.end();
  for (int n = 0; i != e; ++i, ++n)
    TREE_VEC_ELT (result, n) = *i;
  return result;
}

// Extract a vector of term vectors from s. The selected set of terms is given
// by the projection function proj. This is generally either assumptions or
// conclusions.
template<typename F>
  tree
  extract_goals (proof_state& s, F proj)
  {
    tree result = make_tree_vec (s.size ());
    proof_state::iterator i = s.begin ();
    proof_state::iterator e = s.end ();
    for (int n = 0; i != e; ++i, ++n)
      TREE_VEC_ELT (result, n) = extract_terms (proj (*i));
    return result;
  }

// Extract the assumption vector from the proof state s.
inline tree
extract_assumptions (proof_state& s)
{
  term_list& (*proj)(proof_goal&) = assumptions;
  return extract_goals (s, proj);
}

// Extract the conclusion vector from the proof state s.
inline tree
extract_conclusions (proof_state& s)
{
  term_list& (*proj)(proof_goal&) = conclusions;
  return extract_goals (s, proj);
}

} // namespace


// Decompose the requirement R into a set of assumptions, returing a
// vector of vectors containing atomic propositions.
tree
decompose_assumptions (tree r)
{
  proof_state s;
  assume_term (s, r);
  decompose_left (s);
  return extract_assumptions (s);
}

// Decompose the requirement R into a set of conclusionss, returning a
// vector of vectors containing atomic propositions.
tree
decompose_conclusions (tree r)
{
  proof_state s;
  conclude_term (s, r);
  decompose_right (s);
  return extract_conclusions (s);
}


// -------------------------------------------------------------------------- //
// Subsumption and Entailment
//
// The following framework implements the subsumption and entailment
// logic used for overload resolution and lookup.

namespace {

// Returns true if the assumed proposition A entails the conclusion C.
// In general, this is the case when A has equivalent spelling to C,
// although there will be some special cases.
bool
entails_atom (tree a, tree c)
{
  // TODO: Add special cases for __is_same, __is_convertible, 
  // and __is_base_of.
  return cp_tree_equal (a, c);
}

// Returns true if the list of assumptions AS subsume the atomic 
// proposition C. This is the case when we can find a proposition in
// HS that entails the conclusion C.
bool
subsumes_atom (tree as, tree c)
{
  for (int i = 0; i < TREE_VEC_LENGTH (as); ++i)
    if (entails_atom (TREE_VEC_ELT (as, i), c))
      return true;
  return false;
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
      return subsumes_prop (as, TREE_OPERAND (c, 0)) 
          && subsumes_prop (as, TREE_OPERAND (c, 1));
    case TRUTH_ORIF_EXPR:
      return subsumes_prop (as, TREE_OPERAND (c, 0)) 
          || subsumes_prop (as, TREE_OPERAND (c, 1));
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
  gcc_assert (constraint_info_p (left));
  gcc_assert (constraint_info_p (right));

  // Check that c is subsumed by each subproblem in h.
  // If it is not, then h does not subsume c.
  tree as = CI_ASSUMPTIONS (left);
  tree c = reduce_requirements (CI_REQUIREMENTS (right));
  for (int i = 0; i < TREE_VEC_LENGTH (as); ++i)
    if (!subsumes_prop (TREE_VEC_ELT (as, i), c))
      return false;
  return true;
}

} // end namespace


// Returns true the LEFT constraints subsume the RIGHT constraints. Note
// that subsumption is a reflexive relation.
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

