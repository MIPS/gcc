/* Derivation and subsumption rules for constraints.
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
#define INCLUDE_LIST
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

static bool 
parameter_mapping_equivalent_p (tree t1, tree t2)
{
  tree map1 = TREE_TYPE (t1);
  tree map2 = TREE_TYPE (t2);
  while (map1 && map2)
    {
      tree arg1 = TREE_PURPOSE (map1);
      tree arg2 = TREE_PURPOSE (map2);
      if (!cp_tree_equal (arg1, arg2))
	return false;
      map1 = TREE_CHAIN (map1);
      map2 = TREE_CHAIN (map2);
    }
  return true;
}

/* 17.4.1.2p2. Two constraints are identical if they are formed
   from the same expression and the targets of the parameter mapping
   are equivalent.  */

static bool
constraint_identical_p (tree t1, tree t2)
{
  if (ATOMIC_CONSTR_EXPR (t1) != ATOMIC_CONSTR_EXPR (t2))
    return false;

  if (!parameter_mapping_equivalent_p (t1, t2))
    return false;
  
  return true;
}

static hashval_t
hash_atomic_constraint (tree t)
{
  /* Hash the identity of the expression.  */
  hashval_t val = htab_hash_pointer (ATOMIC_CONSTR_EXPR (t));
    
  /* Hash the targets of the parameter map.  */
  tree p = TREE_TYPE (t);
  while (p)
    {
      val = iterative_hash_template_arg (TREE_PURPOSE (p), val);
      p = TREE_CHAIN (p);
    }

  return val;
}

/* Hash functions for atomic constrains.  */

struct constraint_hash : default_hash_traits<tree>
{
  static hashval_t hash (tree t)
  {
    return hash_atomic_constraint (t);
  }

  static bool equal (tree t1, tree t2)
  {
    return constraint_identical_p (t1, t2);
  }
};


/* A conjunctive or disjunctive clause.

   Each clause maintains an iterator that refers to the current
   term, which is used in the linear decomposition of a formula
   into CNF or DNF.  */

struct clause
{
  typedef std::list<tree>::iterator iterator;
  typedef std::list<tree>::const_iterator const_iterator;

  /* Initialize a clause with an initial term.  */

  clause (tree t)
  {
    m_terms.push_back (t);
    if (TREE_CODE (t) == ATOMIC_CONSTR)
      m_set.add (t);

    m_current = m_terms.begin();
  }

  /* Create a copy of the current term. The current
     iterator is set to point to the same position in the
     copied list of terms.  */

  clause (clause const& c)
    : m_terms (c.m_terms), m_set(c.m_set), m_current (m_terms.begin ())
  {
    std::advance (m_current, std::distance (c.begin (), c.current ()));
  }

  /* Returns true when all terms are atoms.  */

  bool done() const
  {
    return m_current == end ();
  }

  /* Advance to the next term.  */

  void advance ()
  {
    gcc_assert (!done ());
    ++m_current;
  }

  /* Replaces the current term at position ITER with T.  If
     T is an atomic constraint that already appears in the
     clause, remove but do not replace ITER. Returns a pair
     containing an iterator to the replace object or past
     the erased object and a boolean value which is true if
     an object was erased.  */

  std::pair<iterator, bool> replace (iterator iter, tree t)
  {
    gcc_assert (TREE_CODE (*iter) != ATOMIC_CONSTR);
    if (TREE_CODE (t) == ATOMIC_CONSTR)
      {
	if (m_set.add (t))
	  return std::make_pair (m_terms.erase(iter), true);
      }
    *iter = t;
    return std::make_pair (iter, false);
  }

  /* Inserts T before ITER in the list of terms.  If T has 
     already is an atomic constraint that already appears in
     the clause, no action is taken, and the current iterator
     is returned. Returns a pair of an iterator to the inserted
     object or ITER if no insertion occurred and a boolean
     value which is true if an object was inserted.  */

  std::pair<iterator, bool> insert (iterator iter, tree t)
  {
    if (TREE_CODE (t) == ATOMIC_CONSTR)
    {
      if (m_set.add (t))
      	return std::make_pair (iter, false);
    }
    return std::make_pair (m_terms.insert (iter, t), true);
  }

  /* Replaces the current term with T. In the case where the
     current term is erased (because T is redundant), update
     the position of the current term to the next term.  */
  
  void replace (tree t)
  {
    m_current = replace (m_current, t).first;
  }
  
  /* Replace the current term with T1 and T2, in that order.  */

  void replace (tree t1, tree t2)
  {
    /* Replace the current term with t1. Ensure that iter points 
       to the term before which t2 will be inserted.  Update the
       current term as needed.  */
    std::pair<iterator, bool> rep = replace (m_current, t1);
    if (rep.second)
      m_current = rep.first;
    else
      ++rep.first;
    
    /* Insert the t2. Make this the current term if we erased
       the prior term.  */
    std::pair<iterator, bool> ins = insert (rep.first, t2);
    if (rep.second && ins.second)
      m_current = ins.first;
  }

  /* Returns true if the clause contains the term T.  */

  bool contains (tree t)
  {
    gcc_assert (TREE_CODE (t) == ATOMIC_CONSTR);
    return m_set.contains (t);
  }


  /* Returns an iterator to the first clause in the formula.  */

  iterator begin ()
  {
    return m_terms.begin();
  }

  /* Returns an iterator to the first clause in the formula.  */

  const_iterator begin () const
  {
    return m_terms.begin();
  }

  /* Returns an iterator past the last clause in the formula.  */

  iterator end ()
  {
    return m_terms.end();
  }

  /* Returns an iterator past the last clause in the formula.  */

  const_iterator end () const
  {
    return m_terms.end();
  }

  /* Returns the current iterator.  */

  const_iterator current () const
  {
    return m_current;
  }

  std::list<tree> m_terms; /* The list of terms.  */
  hash_set<tree, constraint_hash> m_set; /* The set of atomic constraints.  */
  iterator m_current; /* The current term.  */
};


/* A proof state owns a list of goals and tracks the
   current sub-goal. The class also provides facilities
   for managing subgoals and constructing term lists. */

struct formula
{
  typedef std::list<clause>::iterator iterator;
  typedef std::list<clause>::const_iterator const_iterator;

  /* Construct a formula with an initial formula in a
     single clause.  */

  formula (tree t)
  {
    /* This should call emplace_back(). There's a an extra copy being
       invoked by using push_back().  */
    m_clauses.push_back (t);
    m_current = m_clauses.begin ();
  }

  /* Returns true when all clauses are atomic.  */
  bool done () const
  {
    return m_current == end ();
  }

  /* Advance to the next term.  */
  void advance ()
  {
    gcc_assert (!done ());
    ++m_current;
  }

  /* Insert a copy of clause into the formula. This corresponds 
     to a distribution of one logical operation over the other.  */

  clause& branch ()
  {
    gcc_assert (!done ());
    m_clauses.push_back (*m_current);
    return m_clauses.back();
  }

  /* Returns the position of the current clause.  */

  iterator current ()
  {
    return m_current;
  }

  /* Returns an iterator to the first clause in the formula.  */

  iterator begin ()
  {
    return m_clauses.begin();
  }

  /* Returns an iterator to the first clause in the formula.  */

  const_iterator begin () const
  {
    return m_clauses.begin();
  }

  /* Returns an iterator past the last clause in the formula.  */

  iterator end ()
  {
    return m_clauses.end();
  }

  /* Returns an iterator past the last clause in the formula.  */

  const_iterator end () const
  {
    return m_clauses.end();
  }

  std::list<clause> m_clauses; /* The list of clauses.  */
  iterator m_current; /* The current clause.  */
};

void
debug (clause& c)
{
  for (clause::iterator i = c.begin(); i != c.end(); ++i)
    verbatim ("  # %E", *i);
}

void
debug (formula& f)
{
  for (formula::iterator i = f.begin(); i != f.end(); ++i)
    {
      verbatim ("(((");
      debug (*i);
      verbatim (")))");
    }
}

/* The logical rules used to analyze a logical formula. The
   "left" and "right" refer to the position of formula in a
   sequent (as in sequent calculus).  */

enum rules 
{
  left, right
};

/* Returns true if t distributes over its operands.  */

static bool
distributes_p (tree t)
{
  tree t1 = TREE_OPERAND (t, 0);
  tree t2 = TREE_OPERAND (t, 1);
  if (TREE_CODE (t) == CONJ_CONSTR)
    return TREE_CODE (t1) == DISJ_CONSTR && TREE_CODE (t2) == DISJ_CONSTR;
  if (TREE_CODE (t) == DISJ_CONSTR)
    return TREE_CODE (t1) == CONJ_CONSTR && TREE_CODE (t2) == CONJ_CONSTR;
  return false;
}

static int count_terms (tree, rules);

/* The maximum number of allowable terms in a constraint.  */

static int max_size = 4096;

/* Returns the sum of a and b. If the result would overflow,
   returns -1 to indicate an error condition.  */

static inline int
add_clamped (int a, int b)
{
  long long n = (long long)a + (long long)b;
  if (n > max_size)
    return -1;
  return n;
}

/* Returns the product of a and b. If the result would overflow,
   returns -1 to indicate an error condition.  */

static inline int
mul_clamped (int a, int b)
{
  long long n = (long long) a * (long long)b;
  if (n > max_size)
    return -1;
  return n;
}

/* Returns the number of clauses for a conjunction. When converting
   to DNF (when R == LEFT), a conjunction of disjunctions (i.e., 
   terms in CNF-like form) can grow exponentially.  */

int
count_conjunction (tree t, rules r)
{
  int n1 = count_terms (TREE_OPERAND (t, 0), r);
  int n2 = count_terms (TREE_OPERAND (t, 1), r);
  if (n1 == -1 || n2 == -1)
    return -1;
  if (r == left && distributes_p (t))
    return mul_clamped (n1, n2);
  return add_clamped (n1, n2);
}

/* Returns the number of clauses for a conjunction. When converting
   to CNF (when R == RIGHT), a disjunction of conjunctions (i.e., 
   terms in DNF-like form) can grow exponentially.  */

static int
count_disjunction (tree t, rules r)
{
  int n1 = count_terms (TREE_OPERAND (t, 0), r);
  int n2 = count_terms (TREE_OPERAND (t, 1), r);
  if (n1 == -1 || n2 == -1)
    return -1;
  if (r == right && distributes_p (t))
    return mul_clamped (n1, n2);
  return add_clamped (n1, n2);
}

/* Count the number of subproblems in T.  */

static int
count_terms (tree t, rules r)
{
  switch (TREE_CODE (t))
    {
    case CONJ_CONSTR:
      return count_conjunction (t, r);
    case DISJ_CONSTR:
      return count_disjunction (t, r);
    default:
      return 1;
    }
}

/* Returns the maximum number of terms in T if it were
   converted to DNF.  Returns -1 if the count exceeds the
   maximum formula size.  */

static int
dnf_size (tree t)
{
  return count_terms (t, left);
}

/* Returns the maximum number of terms in T if it were 
   converted to CNF.  Returns -1 if the count exceeds the
   maximum formula size.  */

static int
cnf_size (tree t)
{
  return count_terms (t, right);
}

/* A left-conjunction is replaced by its operands.  */

void
replace_term (clause& c, tree t)
{
  tree t1 = TREE_OPERAND (t, 0);
  tree t2 = TREE_OPERAND (t, 1);
  return c.replace(t1, t2);
}

/* Create a new clause in the formula by copying the current
   clause. In the current clause, the term at CI is replaced 
   by the first operand, and in the new clause, it is replaced 
   by the second.  */

void
branch_clause (formula& f, clause& c1, tree t)
{
  tree t1 = TREE_OPERAND (t, 0);
  tree t2 = TREE_OPERAND (t, 1);
  clause& c2 = f.branch ();
  c1.replace (t1);
  c2.replace (t2);
}

/* Decompose t1 /\ t2 according to the rules R.  */

inline void
decompose_conjuntion (formula& f, clause& c, tree t, rules r)
{
  if (r == left)
    replace_term (c, t);
  else
    branch_clause (f, c, t);
}

/* Decompose t1 \/ t2 according to the rules R.  */

inline void
decompose_disjunction (formula& f, clause& c, tree t, rules r)
{
  if (r == right)
    replace_term (c, t);
  else
    branch_clause (f, c, t);
}

/* An atomic constraint is already decomposed.  */
inline void
decompose_atom (clause& c)
{
  c.advance ();
}

/* Decompose a term of clause C (in formula F) according to the
   logical rules R. */

void
decompose_term (formula& f, clause& c, tree t, rules r)
{
  switch (TREE_CODE (t))
    {
      case CONJ_CONSTR:
        return decompose_conjuntion (f, c, t, r);
      case DISJ_CONSTR:
	return decompose_disjunction (f, c, t, r);
      default:
	return decompose_atom (c);
    }
}

/* Decompose C (in F) using the logical rules R until it 
   is comprised of only atomic constraints.  */

void
decompose_clause (formula& f, clause& c, rules r)
{
  while (!c.done ())
    decompose_term (f, c, *c.current (), r);
  f.advance ();
}

/* Decompose the logical formula F according to the logical
   rules determined by R.  The result is a formula containing
   clauses that contain only atomic terms.  */

void
decompose_formula (formula& f, rules r)
{
  while (!f.done ())
    decompose_clause (f, *f.current (), r);
}

/* Fully decomposing T into a list of sequents, each comprised of
   a list of atomic constraints, as if T were an antecedent.  */

static formula 
decompose_antecedent (tree t)
{
  formula f (t);
  decompose_formula (f, left);
  return f;
}

/* Fully decomposing T into a list of sequents, each comprised of
   a list of atomic constraints, as if T were a consequent.  */

static formula
decompose_consequents (tree t)
{
  formula f (t);
  decompose_formula (f, right);
  return f;
}

static bool derive_proof (clause&, tree, rules);

/* Derive a proof of both operands of T.  */

static bool
derive_proof_for_both_operands (clause& c, tree t, rules r)
{
  if (!derive_proof (c, TREE_OPERAND (t, 0), r))
    return false;
  return derive_proof (c, TREE_OPERAND (t, 1), r);
}

/* Derive a proof of either operand of T.  */

static bool
derive_proof_for_either_operand (clause& c, tree t, rules r)
{
  if (derive_proof (c, TREE_OPERAND (t, 0), r))
    return true;
  return derive_proof (c, TREE_OPERAND (t, 1), r);
}

/* Derive a proof of the atomic constraint T in clause C.  */

static bool
derive_atomic_proof (clause& c, tree t)
{
  return c.contains (t);
}

/* Derive a proof of T from the terms in C.  */

static bool
derive_proof (clause& c, tree t, rules r)
{
  switch (TREE_CODE (t))
  {
    case CONJ_CONSTR:
      if (r == left)
        return derive_proof_for_both_operands (c, t, r);
      else
      	return derive_proof_for_either_operand (c, t, r);
    case DISJ_CONSTR:
      if (r == left)
        return derive_proof_for_either_operand (c, t, r);
      else
      	return derive_proof_for_both_operands (c, t, r);
    default:
      return derive_atomic_proof (c, t);
  }
}

/* Derive a proof of T from disjunctive clauses in F.  */

static bool
derive_proofs (formula& f, tree t, rules r)
{
  for (formula::iterator i = f.begin(); i != f.end(); ++i)
    if (!derive_proof (*i, t, r))
      return false;
  return true;
}

static inline bool
diagnose_constraint_size (tree t)
{
  error_at (input_location, "%qE exceeds the maximum constraint complexity", t);
  return false;
}

/* Returns true if the LEFT constraint subsume the RIGHT constraints.
   This is done by deriving a proof of the conclusions on the RIGHT
   from the assumptions on the LEFT assumptions.  */

static bool
subsumes_constraints_nonnull (tree lhs, tree rhs)
{
  auto_timevar time (TV_CONSTRAINT_SUB);

  int n1 = dnf_size (lhs);
  int n2 = cnf_size (rhs);
  
  /* If either constraint would overflow complexity, bail.  */
  if (n1 == -1)
    return diagnose_constraint_size (lhs);
  if (n2 == -1)
    return diagnose_constraint_size (rhs);
  
  /* Decompose the smaller of the two formulas, and recursively
     check the implication using the larger.  */
  if (n1 <= n2)
    {
      formula dnf = decompose_antecedent (lhs);
      return derive_proofs (dnf, rhs, left);
    }
  else
    {
      formula cnf = decompose_consequents (rhs);
      return derive_proofs (cnf, lhs, right);
    }
}

/* Returns true if the LEFT constraints subsume the RIGHT
   constraints.  */

bool
subsumes (tree lhs, tree rhs)
{
  if (lhs == rhs)
    return true;
  if (!lhs)
    return false;
  if (!rhs)
    return true;
  return subsumes_constraints_nonnull (lhs, rhs);
}
