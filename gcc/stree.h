/* S-tree representation manipulation.
   Copyright (C) 2004 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef GCC_STREE_H
#define GCC_STREE_H

/* S-trees are a compressed representation of trees, for declarations
   (and types).  They are motivated by the observation that GCC spends
   a lot of its time allocating memory for, and filling in, trees for
   declarations that are subsequently never used again.  */

/* An index of a s_tree.  */
typedef unsigned long s_tree_i;

#define NULL_S_TREE_I 0

/* Given an s_tree_i, return the corresponding tree.  */
extern tree s_tree_to_tree (tree, s_tree_i);

/* Remember that T is the tree for ST.  */
extern void set_tree_for_s_tree (s_tree_i st, tree t);

/* Return the tree for ST, or NULL if there is no tree exists yet.  */
extern tree get_tree_for_s_tree (s_tree_i st);

/* Parse a s_tree.  */
typedef unsigned char * s_tree_iter;
extern void get_s_tree_iter (s_tree_iter *, s_tree_i);
extern tree sti_const_int_tree (s_tree_iter *);
extern tree sti_tree (s_tree_iter *ti);
extern s_tree_i sti_s_tree (s_tree_iter *ti);

static inline unsigned char
sti_uchar (s_tree_iter *ti)
{
  unsigned char u = **ti;
  (*ti)++;
  return u;
}

/* Build a s-tree.  */
enum {
  ST_LAST,
  ST_TREE,
  ST_TREE_INT,
  ST_STREE
};

extern s_tree_i build_s_tree (unsigned char, ...);

#endif /* GCC_STREE_H */
