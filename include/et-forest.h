/* Et-forest data structure implementation.  
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* This package implements ET forest data structure. Each tree in 
   the structure maintains a tree structure and offers logarithmic time
   for tree operations (insertion and removal of nodes and edges) and
   poly-logarithmic time for nearest common ancesto.

   Node values are generic pointers.
 
   ET tree strores its structue as a sequence of symbols obtained 
   by dfs(root)

   dfs (node) 
   {
     s = node;
     for each child c of node do
       s = concat (s, c, node);
     return s;
   }
   
   For example for tree
 
            1
          / | \
         2  3  4
       / |
      4  5
 
   the sequence is 1 2 4 2 5 3 1 3 1 4 1.
 
   The sequence is stored in a sligtly modified splay tree.
   In order to support various types of node values, a hashtable
   is used to convert node values to the internal representation.  */

#ifndef _ET_TREE_H
#define _ET_TREE_H

#include <ansidecl.h>
#include <stddef.h>
#include "hashtab.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void * et_forest_value;

/* The function for computing hash value of vertices.  */
typedef hashval_t (*et_forest_hash) PARAMS ((const et_forest_value));

/* The vertex comparsion function.  */
typedef int (*et_forest_eq) PARAMS ((const et_forest_value,
				     const et_forest_value));

/* Cleanup function. Called in destroy and remove.  */
typedef void (*et_forest_del) PARAMS ((et_forest_value));


/* The ET-forest type.  */
struct et_forest
{
  /* The hash table.  */
  htab_t table;

  /* Pointer to the hash function.  */
  et_forest_hash hash_f;

  /* Pointer to the comparsion function.  */
  et_forest_eq eq_f;

  /* Pointer to the delete function.  */
  et_forest_del del_f;
};

typedef struct et_forest *et_forest_t;

extern et_forest_t et_forest_create PARAMS ((size_t, et_forest_hash,
                                         et_forest_eq, et_forest_del));

extern void et_forest_delete PARAMS ((et_forest_t));

extern void et_forest_add_vertex PARAMS ((et_forest_t, et_forest_value));
extern void et_forest_add_edge PARAMS ((et_forest_t, et_forest_value, 
					et_forest_value));
extern void et_forest_remove_vertex PARAMS ((et_forest_t, et_forest_value));
extern void et_forest_remove_edge PARAMS ((et_forest_t, et_forest_value,
					   et_forest_value));
extern et_forest_value et_forest_parent PARAMS ((et_forest_t, et_forest_value));
extern et_forest_value et_forest_common_ancestor PARAMS ((et_forest_t,
							  et_forest_value,
							  et_forest_value));
extern int et_forest_contains_vertex PARAMS ((et_forest_t, et_forest_value));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ET_TREE_H */
