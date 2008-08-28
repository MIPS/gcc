/* Language-dependent trees for LTO.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef GCC_LTO_TREE_H
#define GCC_LTO_TREE_H

struct lang_identifier GTY(())
{
  struct tree_identifier base;
  /* LTO_IDENTIFIER_DECL */
  tree decl;
};

struct lang_decl GTY(())
{
  int dummy;  /* Added because ggc does not like empty structs.  */
};

struct lang_type GTY(())
{
  int dummy;  /* Added because ggc does not like empty structs.  */
};

struct language_function GTY(())
{
  int dummy;  /* Added because ggc does not like empty structs.  */
};

enum lto_tree_node_structure_enum {
  TS_LTO_GENERIC
};

union lang_tree_node GTY(
 (desc ("lto_tree_node_structure (&%h)"),
  chain_next ("(union lang_tree_node *)TREE_CHAIN (&%h.generic)")))
{
  union tree_node GTY ((tag ("TS_LTO_GENERIC"),
			desc ("tree_node_structure (&%h)"))) generic;
};

/* Return NODE (an IDENTIFIER_NODE) as a pointer to a
   "lang_identifier".  */
#define LANG_IDENTIFIER_CAST(NODE) \
  ((struct lang_identifier*)IDENTIFIER_NODE_CHECK (NODE))

/* Return the VAR_DECL or FUNCTION_DECL with external linkage whose
   DECL_ASSEMBLER_NAME is NODE, or NULL_TREE if there is no such
   declaration.  */ 
#define LTO_IDENTIFIER_DECL(NODE)		\
  (LANG_IDENTIFIER_CAST (NODE)->decl)

/* Vector to keep track of external variables we've seen so far.  */
extern GTY(()) VEC(tree,gc) *lto_global_var_decls;


/* lto-symtab.c */

/* The NEW_VAR (a VAR_DECL) has just been read.  If there is an
   existing variable with the same name, merge the declaration for
   NEW_VAR with the previous declaration and return the previous
   declaration.  In this case, NEW_VAR must no longer be used by the
   caller.  All other entities referenced from NEW_VAR (including, in
   particular, its type) must already have been merged before this
   function is called.  If the merge fails (due to inconsistencies
   between the declarations), an error message is issued, and
   error_mark_node is returned.  If there is no previous declaration,
   NEW_VAR is returned.  */
extern tree lto_symtab_merge_var (tree new_var);

/* Like lto_symtab_merge_var, but for functions.  */
extern tree lto_symtab_merge_fn (tree new_fn);

#endif /* GCC_LTO_TREE_H */
