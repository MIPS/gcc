/* Interface to the double linked statement chain. 
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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


#define PREV_STMT(NODE) TREE_TYPE (NODE)
#define NEXT_STMT(NODE) TREE_CHAIN (NODE)

/* In tree-dchain.c  */
extern void double_chain_stmts             PARAMS ((tree));
extern void double_chain_free              PARAMS ((tree));
extern void dchain_insert_stmt_after       PARAMS ((tree, tree));
extern void dchain_insert_stmt_before      PARAMS ((tree, tree));
extern void dchain_delete_stmts            PARAMS ((tree, tree));
extern void dchain_insert_chain            PARAMS ((tree, tree, tree, tree));
extern void dchain_two_chains              PARAMS ((tree, tree, tree, tree, 
						    tree, tree, tree, tree));
extern tree dchain_deep_copy_node          PARAMS ((tree));
extern void dchain_replace_stmt            PARAMS ((tree, tree));
extern void dchain_build_scope             PARAMS ((tree *));
extern tree dchain_declare_tmp_vars        PARAMS ((tree, tree));
extern void dchain_delete_labels           PARAMS ((tree));


/* Define a double linked list interface for the COMPOUND_STMT path tree. 
   This interface is used in goto and break elimination passes. 
   The first element pointed by PATH_TOP is the innermost statement.  
   This implementation uses a TREE_LIST node as its basic element.  */
#define PATH_TYPE tree
#define PATH_TOP compound_stmt_path
#define PATH_NULL NULL_TREE
#define PATH_FOREACH(E, P) for (E = PATH_ANCESTOR (P); E != PATH_NULL; E = PATH_CHILD (E))
#define PATH_COPY_ELT(P) copy_node (P)
#define PATH_COPY(N, P) do {                            \
  PATH_TYPE t, elt;                                     \
  (N) = PATH_NULL;                                      \
  PATH_FOREACH (elt, (P))                               \
    {                                                   \
      t = PATH_COPY_ELT (elt);                          \
      PATH_PARENT (t) = (N);                            \
      if (N)                                            \
	PATH_CHILD (N) = t;                             \
      (N) = t;                                          \
    }                                                   \
} while (0)
#define PATH_NEW(NODE)   tree_cons (NODE, PATH_NULL, PATH_NULL)
#define PATH_ANCESTOR(E) tree_last (E)
#define PATH_PARENT(E)   NEXT_STMT (E)
#define PATH_CHILD(E)    PREV_STMT (E)
#define PATH_STMT(E)     TREE_PURPOSE (E)
#define PATH_PUSH(NODE) do {                            \
  PATH_TYPE p1, p2;                                     \
  p1 = PATH_NEW (NODE);                                 \
  p2 = PATH_TOP;                                        \
  PATH_PARENT (p1) = p2;                                \
  if (p2)                                               \
    PATH_CHILD (p2) = p1;                               \
  PATH_TOP = p1;                                        \
} while (0)
#define PATH_POP() do {                                 \
  PATH_TOP = PATH_PARENT (PATH_TOP);                    \
  if (PATH_TOP)                                         \
    PATH_CHILD (PATH_TOP) = PATH_NULL;                  \
} while (0)


/* In simple-break-elim.c  */
extern void break_continue_elimination     PARAMS ((tree));
extern int chain_contains_stmt_p           PARAMS ((tree, enum tree_code));
extern void flow_out_of                    PARAMS ((PATH_TYPE, PATH_TYPE, tree, tree, void (*) 
						    PARAMS ((tree, tree, tree, tree))));
extern void insert_andif_in_condition      PARAMS ((tree, tree));
extern void insert_orif_in_condition       PARAMS ((tree, tree));
extern void eliminate_compound_stmts       PARAMS ((tree));


/* In simple-goto-elim.c  */
extern void goto_elimination               PARAMS ((tree));
extern void eliminate_with_if              PARAMS ((tree, tree, tree, 
						    void (*) PARAMS ((tree, tree, tree, tree))));
