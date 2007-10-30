/* Language-dependent hooks for LTO.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "flags.h"
#include "tm.h"
#include "tree.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "debug.h"
#include "lto-tree.h"
#include "lto.h"
#include "toplev.h"

/* Tables of information about tree codes.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) TYPE,
const enum tree_code_class tree_code_type[] = {
#include "tree.def"
};
#undef DEFTREECODE

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) LENGTH,
const unsigned char tree_code_length[] = {
#include "tree.def"
};
#undef DEFTREECODE

#define DEFTREECODE(SYM, NAME, TYPE, LEN) NAME,
const char *const tree_code_name[] = {
#include "tree.def"
};
#undef DEFTREECODE

/* This variable keeps a table for types for each precision so that we only 
   allocate each of them once. Signed and unsigned types are kept separate.  */
static GTY(()) tree signed_and_unsigned_types[MAX_BITS_PER_WORD + 1][2];

/* Language hooks.  */

static bool 
lto_mark_addressable (tree t ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

static tree
lto_type_for_size (unsigned precision ATTRIBUTE_UNUSED, 
		   int unsignedp ATTRIBUTE_UNUSED)
{
  tree t = NULL_TREE;

  if (precision <= MAX_BITS_PER_WORD
      && signed_and_unsigned_types[precision][unsignedp] != 0)
    return signed_and_unsigned_types[precision][unsignedp];

  if (unsignedp)
    t = signed_and_unsigned_types[precision][1]
      = make_unsigned_type (precision);
  else
    t = signed_and_unsigned_types[precision][0]
      = make_signed_type (precision);
  
  return t;
}

static tree
lto_type_for_mode (enum machine_mode mode ATTRIBUTE_UNUSED, 
		   int unsigned_p ATTRIBUTE_UNUSED)
{
  /* This hook is called by the middle-end.  For example,
     assign_stack_local_1 uses this hook to determine whether
     additional alignment is required for stack variables for which no
     explicit alignment is provided.  */
  if (SCALAR_INT_MODE_P (mode))
    return lto_type_for_size (GET_MODE_BITSIZE (mode), unsigned_p);
  else
    return NULL_TREE;
}

static int
lto_global_bindings_p (void) 
{
  gcc_unreachable ();
}

static void
lto_insert_block (tree block ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

static void
lto_set_decl_assembler_name (tree decl)
{
  /* This is lhd_set_decl_assembler_name without performing name
     mangling.  */
  const char *name = IDENTIFIER_POINTER (DECL_NAME (decl));
  char *label;

  ASM_FORMAT_PRIVATE_NAME (label, name, DECL_UID (decl));
  SET_DECL_ASSEMBLER_NAME (decl, get_identifier (label));
}

static tree
lto_pushdecl (tree t ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

static tree
lto_getdecls (void)
{
  gcc_unreachable ();
}

static void
lto_write_globals (void)
{
  tree *vec = VEC_address (tree, lto_global_var_decls);
  int len = VEC_length (tree, lto_global_var_decls);
  emit_debug_global_declarations (vec, len);
  VEC_free (tree, gc, lto_global_var_decls);
}

static tree
lto_builtin_function (const char *name ATTRIBUTE_UNUSED,
		      tree type ATTRIBUTE_UNUSED,
		      int function_code ATTRIBUTE_UNUSED,
		      enum built_in_class bt_class ATTRIBUTE_UNUSED,
		      const char *library_name ATTRIBUTE_UNUSED,
		      tree attrs ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

/* Perform LTO-specific initialization.  */

static bool
lto_init (void)
{
  /* Always operate in unit-at-time mode so that we can defer
     decisions about what to output.  */
  flag_unit_at_a_time = 1;

  /* Create the basic integer types.  */
  build_common_tree_nodes (flag_signed_char, 
			   /*signed_sizetype=*/false);
  /* Tell the middle end what type to use for the size of objects.  */
  if (strcmp (SIZE_TYPE, "unsigned int") == 0)
    {
      set_sizetype (unsigned_type_node);
      size_type_node = unsigned_type_node;
    }
  else if (strcmp (SIZE_TYPE, "long unsigned int") == 0)
    {
      set_sizetype (long_unsigned_type_node);
      size_type_node = long_unsigned_type_node;
    }
  else
    gcc_unreachable();
  /* Create other basic types.  */
  build_common_tree_nodes_2 (/*short_double=*/false);

  /* Initialize LTO-specific data structures.  */
  lto_global_var_decls = VEC_alloc (tree, gc, 256);
  in_lto_p = true;

  return true;
}

#define LANG_HOOKS_MARK_ADDRESSABLE lto_mark_addressable
#define LANG_HOOKS_TYPE_FOR_MODE lto_type_for_mode
#define LANG_HOOKS_TYPE_FOR_SIZE lto_type_for_size
#undef LANG_HOOKS_SET_DECL_ASSEMBLER_NAME
#define LANG_HOOKS_SET_DECL_ASSEMBLER_NAME lto_set_decl_assembler_name
#undef LANG_HOOKS_GLOBAL_BINDINGS_P
#define LANG_HOOKS_GLOBAL_BINDINGS_P lto_global_bindings_p
#undef LANG_HOOKS_INSERT_BLOCK
#define LANG_HOOKS_INSERT_BLOCK lto_insert_block
#undef LANG_HOOKS_PUSHDECL
#define LANG_HOOKS_PUSHDECL lto_pushdecl
#undef LANG_HOOKS_GETDECLS
#define LANG_HOOKS_GETDECLS lto_getdecls
#undef LANG_HOOKS_WRITE_GLOBALS
#define LANG_HOOKS_WRITE_GLOBALS lto_write_globals
#undef LANG_HOOKS_BUILTIN_FUNCTION
#define LANG_HOOKS_BUILTIN_FUNCTION lto_builtin_function
#undef LANG_HOOKS_INIT
#define LANG_HOOKS_INIT lto_init
#undef LANG_HOOKS_PARSE_FILE
#define LANG_HOOKS_PARSE_FILE lto_main
#undef LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION
#define LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION tree_rest_of_compilation

const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

/* Language hooks that are not part of lang_hooks.  */

tree
convert (tree type ATTRIBUTE_UNUSED, tree expr ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

/* Tree walking support.  */

static enum lto_tree_node_structure_enum
lto_tree_node_structure (union lang_tree_node *t ATTRIBUTE_UNUSED)
{
  return TS_LTO_GENERIC;
}

#include "ggc.h"
#include "gtype-lto.h"
#include "gt-lto-lto-lang.h"
