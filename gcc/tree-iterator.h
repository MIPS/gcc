/* Iterator routines for manipulating GENERIC and GIMPLE tree statements.  
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod  <amacleod@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* This file is dependant upon the implementation of tree's. It provides an
   abstract interface to the tree objects such that if all tree creation and 
   manipulations are done through this interface, we can easily change the 
   implementation of tree's, and not impact other code.

   In particular, we wish to replace the current linking scheme which uses 
   COMPOUND_EXPR nodes to link statements. We'd like to either use doubly
   linked lists, or another mechanism which takes the links completely out
   of the tree nodes all together. Before this can be done, the front end
   will have to be modified to use these routines to build up the initial
   GENERIC representation of the function tree.  */

/** @file tree-iterator.c
    @brief Routines for manipulating tree statements. */

#ifndef _TREE_ITERATOR_H
#define _TREE_ITERATOR_H 1

/* Iterator object for GENERIC or GIMPLE TREE statements.  */

typedef struct {
  tree *tp;
} tree_stmt_iterator;

static inline tree_stmt_iterator tsi_start 	PARAMS ((tree *));
static inline bool tsi_end_p			PARAMS ((tree_stmt_iterator));
static inline void tsi_next			PARAMS ((tree_stmt_iterator *));
static inline void tsi_prev			PARAMS ((tree_stmt_iterator *));
static inline tree tsi_stmt			PARAMS ((tree_stmt_iterator));
static inline tree *tsi_stmt_ptr		PARAMS ((tree_stmt_iterator));
static inline tree *tsi_container		PARAMS ((tree_stmt_iterator));


static inline tree_stmt_iterator
tsi_start (tp)
     tree *tp;
{
  tree_stmt_iterator i;
  i.tp = tp;
  return i;
}

static inline bool
tsi_end_p (i)
     tree_stmt_iterator i;
{
  return (i.tp == NULL || *(i.tp) == error_mark_node);
}

static inline void
tsi_next (i)
     tree_stmt_iterator *i;
{
  tree t = *(i->tp);
  STRIP_NOPS (t);
  if (TREE_CODE (t) == COMPOUND_EXPR)
    i->tp = &(TREE_OPERAND (t, 1));
  else
    i->tp = NULL;
}

static inline void
tsi_prev (i)
     tree_stmt_iterator *i;
{
  printf (" tsi_prev (%p) is not implemented yet\n",(void *)i);
  abort();
}

static inline tree *
tsi_stmt_ptr (i)
     tree_stmt_iterator i;
{
  tree t;

#if defined ENABLE_CHECKING
  if (i.tp == NULL || *i.tp == NULL_TREE)
    abort ();
#endif

  t = *(i.tp);
  STRIP_NOPS (t);

  if (TREE_CODE (t) == COMPOUND_EXPR)
    return &TREE_OPERAND (t, 0);
  else
    return i.tp;
}

static inline tree
tsi_stmt (i)
     tree_stmt_iterator i;
{
  tree t = *(tsi_stmt_ptr (i));
  STRIP_NOPS (t);
  if (t == empty_stmt_node || t == error_mark_node)
    t = NULL_TREE;
  return t;
}

static inline tree *
tsi_container (i)
     tree_stmt_iterator i;
{
  return i.tp;
}


/* Abstract interface for linking and chaining stmts.  Declared in tree.c.  */

/* A tree_stmt_anchor is used as the root of a stmt list.  */
typedef tree tree_stmt_anchor;
#define EMPTY_ANCHOR	NULL_TREE

enum tsi_iterator_update
{
  TSI_NEW_STMT,
  TSI_SAME_STMT
};

void tsi_link_before			PARAMS ((tree_stmt_iterator *, tree, enum tsi_iterator_update));
void tsi_link_after			PARAMS ((tree_stmt_iterator *, tree, enum tsi_iterator_update));
void tsi_delink				PARAMS ((tree_stmt_iterator *));
tree_stmt_iterator tsi_new_stmt_list	PARAMS ((tree, tree_stmt_anchor *));
tree_stmt_iterator tsi_stmt_list_head	PARAMS ((tree_stmt_anchor));

#endif /* _TREE_ITERATOR_H  */
