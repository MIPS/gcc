/* Bonus location tree information.
   Copyright (C) 2017 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_BLT_H
#define GCC_BLT_H

/* Sometimes we need additional location information.

   The "tree" type represents a node within an abstract syntax tree,
   but this is sometimes too abstract - sometimes we want the locations
   of the clauses and tokens that are abstracted away by the frontends.

   In theory we could generate the full parse tree ("concrete syntax tree"),
   showing every production followed to parse the input, but it is likely
   to be unwieldy: large and difficult to navigate.

   So this file implements a middle-ground: an additional tree of parse
   information, more concrete than "tree", but not the full parse tree.

   There is a partial mapping between "tree" and blt_node: a blt_node
   can reference a tree, and a tree can reference a blt_node (though
   typically the mapping is very sparse; most don't).  This allows us
   to go from e.g. a function_decl in the tree world and navigate
   pertinent parts of the syntax that was used to declare it.

   Working title = "BLT" as a backronym for "bonus location tree" (but
   actually a reference to a sandwich) - doesn't clash with anything else
   in the source tree ("Parse Tree" would be "PT" which clashes with
   "points-to", and "Concrete Syntax Tree" would be "CST" which clashes
   with our abbreviation for "constant").  */

#include "cpplib.h"

/* An ID for a kind of node within the tree.  */

enum blt_kind
{
#define DEF_BLT_NODE(ENUM_NAME, PRETTY_NAME) ENUM_NAME,
#include "blt.def"
#undef DEF_BLT_NODE
};

class blt_node;
class blt_context;

/* Would use a lambda.  */

class blt_predicate
{
 public:
  virtual bool satisfied_by_node_p (const blt_node &node) = 0;
};

/* A syntax node: either a token, a non-terminal, or a simplified set of
   non-terminals.  */

class blt_node
{
public:
  blt_node (enum blt_kind kind, location_t start);
  ~blt_node ();

  /* Structural manipulation.  */
  void add_child (blt_node *child);
  void replace_child (blt_node *old, blt_node *new_);
  void make_orphan ();

  /* Modification.  */

  void set_finish (location_t loc) { m_finish = ::get_finish (loc); }
  void set_tree (tree t, blt_context *ctxt);
  void set_kind (enum blt_kind kind) { m_kind = kind; }

  /* Dumping.  */
  void dump (FILE *) const;
  void dump (pretty_printer *pp, const char *prefix,
	     const blt_node *parent, bool is_last_child) const;

  /* Accessors.  */
  enum blt_kind get_kind () const { return m_kind; }
  const char *get_name () const;
  blt_node *get_parent () const { return m_parent; }
  location_t get_start () const { return m_start; }
  location_t get_finish () const { return m_finish; }
  location_t get_range () const;
  tree get_tree () const { return m_tree; }

  blt_node *get_first_child_of_kind (enum blt_kind kind) const;
  void get_children_of_kind (auto_vec<blt_node *> &out,
			     enum blt_kind kind) const;

  int get_index_of_child (blt_node *needle) const;

  blt_node *get_ancestor_of_kind (enum blt_kind kind) const;

  blt_node *find_descendant_satisfying (blt_predicate &pred);

  blt_node *get_descendant_at_location (const char *filename, int line,
					int character);

  bool contains_location_p (const char *filename, int line,
			    int character) const;

private:
  void assert_invariants () const;

private:
  enum blt_kind m_kind;
  blt_node *m_parent;
  blt_node *m_first_child;
  blt_node *m_last_child;
  blt_node *m_prev_sibling;
  blt_node *m_next_sibling;
#if 1
  location_t m_start;
  location_t m_finish;
#else
  // Tokens are currently released after lexing...
  cp_token *m_first_token;
  cp_token *m_last_token;
#endif
  tree m_tree;
};

extern void DEBUG_FUNCTION debug (blt_node *);

/* FIXME.  */

class blt_context
{
 public:
  blt_context ();
  ~blt_context ();

  blt_node *get_root_node () const { return m_root_node; }
  void set_root_node (blt_node *n) { m_root_node = n; }

  blt_node *get_node_for_tree (tree);
  void set_node_for_tree (tree, blt_node *);

 private:
  friend class blt_node;

  typedef hash_map <tree, blt_node *> tree_to_blt_map_t;
  tree_to_blt_map_t *m_tree_to_blt_map;
  blt_node *m_root_node;
};

extern blt_context *the_blt_ctxt;

extern blt_node *blt_get_root_node ();
extern blt_node *blt_get_node_for_tree (tree);
extern void blt_set_node_for_tree (tree, blt_node *);

#endif  /* GCC_BLT_H  */
