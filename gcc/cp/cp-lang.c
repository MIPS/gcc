/* Language-dependent hooks for C++.
   Copyright 2001, 2002, 2004 Free Software Foundation, Inc.
   Contributed by Alexandre Oliva  <aoliva@redhat.com>

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-common.h"
#include "toplev.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "diagnostic.h"
#include "cxx-pretty-print.h"
#include "debug.h"
#include "cp-objcp-common.h"

enum c_language_kind c_language = clk_cxx;

/* Lang hooks common to C++ and ObjC++ are declared in cp/cp-objcp-common.h;
   consequently, there should be very few hooks below.  */

static int cxx_types_compatible_p (tree, tree);

#undef LANG_HOOKS_NAME
#define LANG_HOOKS_NAME "GNU C++"
#undef LANG_HOOKS_INIT
#define LANG_HOOKS_INIT cxx_init
/* APPLE LOCAL begin Objective-C++ */
/* MERGE FIXME - redundant with def below? */
#undef LANG_HOOKS_FINISH_FILE
#define LANG_HOOKS_FINISH_FILE finish_file
/* APPLE LOCAL end Objective-C++ */
#undef LANG_HOOKS_DECL_PRINTABLE_NAME
#define LANG_HOOKS_DECL_PRINTABLE_NAME	cxx_printable_name
#undef LANG_HOOKS_TYPES_COMPATIBLE_P
#define LANG_HOOKS_TYPES_COMPATIBLE_P cxx_types_compatible_p
/* APPLE LOCAL begin Objective-C++  */
/* Redefine the hooks that need to be different for ObjC++.  */
#ifdef OBJCPLUS
static void objcplus_init_options                   PARAMS ((void));
#include "objc/objc-act.h"
#undef LANG_HOOKS_NAME
#define LANG_HOOKS_NAME "GNU Objective-C++"
#undef LANG_HOOKS_INIT
#define LANG_HOOKS_INIT objc_init
#undef LANG_HOOKS_FINISH_FILE
#define LANG_HOOKS_FINISH_FILE objc_finish_file
#undef LANG_HOOKS_INIT_OPTIONS
#define LANG_HOOKS_INIT_OPTIONS objcplus_init_options
#endif /* OBJCPLUS */
/* APPLE LOCAL end Objective-C++ */

/* Each front end provides its own lang hook initializer.  */
const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

/* Tree code classes.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) TYPE,

const char tree_code_type[] = {
#include "tree.def"
  'x',
#include "c-common.def"
  'x',
#include "cp-tree.def"
/* APPLE LOCAL begin Objective-C++ */
#ifdef OBJCPLUS
  'x',
#include "objc-tree.def"
#endif
/* APPLE LOCAL end Objective-C++ */
};
#undef DEFTREECODE

/* Table indexed by tree code giving number of expression
   operands beyond the fixed part of the node structure.
   Not used for types or decls.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) LENGTH,

const unsigned char tree_code_length[] = {
#include "tree.def"
  0,
#include "c-common.def"
  0,
#include "cp-tree.def"
/* APPLE LOCAL begin Objective-C++ */
#ifdef OBJCPLUS
  0,
#include "objc-tree.def"
#endif
/* APPLE LOCAL end Objective-C++ */
};
#undef DEFTREECODE

/* Names of tree components.
   Used for printing out the tree and error messages.  */
#define DEFTREECODE(SYM, NAME, TYPE, LEN) NAME,

const char *const tree_code_name[] = {
#include "tree.def"
  "@@dummy",
#include "c-common.def"
  "@@dummy",
#include "cp-tree.def"
/* APPLE LOCAL begin Objective-C++ */
#ifdef OBJCPLUS
  "@@dummy",
#include "objc-tree.def"
#endif
/* APPLE LOCAL end Objective-C++ */
};
#undef DEFTREECODE

/* APPLE LOCAL begin Objective-C++ */
#ifdef OBJCPLUS
static void 
objcplus_init_options (void)
{
  flag_objc = 1;
  cxx_init_options ();
}
#endif
/* APPLE LOCAL end */

/* Lang hook routines common to C++ and ObjC++ appear in cp/cp-objcp-common.c;
   there should be very few routines below.  */

/* This compares two types for equivalence ("compatible" in C-based languages).
   This routine should only return 1 if it is sure.  It should not be used
   in contexts where erroneously returning 0 causes problems.  */

static int cxx_types_compatible_p (tree x, tree y)
{
  if (same_type_ignoring_top_level_qualifiers_p (x, y))
    return 1;

  /* Once we get to the middle-end, references and pointers are
     interchangeable.  FIXME should we try to replace all references with
     pointers?  */
  if (POINTER_TYPE_P (x) && POINTER_TYPE_P (y)
      && same_type_p (TREE_TYPE (x), TREE_TYPE (y)))
    return 1;

  return 0;
}

/* The following function does something real, but only in Objective-C++.  */

tree
objcp_tsubst_copy_and_build (tree t ATTRIBUTE_UNUSED, tree args ATTRIBUTE_UNUSED, 
			     tsubst_flags_t complain ATTRIBUTE_UNUSED, 
			     tree in_decl ATTRIBUTE_UNUSED,
			     bool function_p ATTRIBUTE_UNUSED)
{
  return NULL_TREE;
}

void
finish_file (void)
{
  cp_finish_file ();
}

/* APPLE LOCAL begin Objective-C++ */
/* Include the GC roots here instead of in cp/decl.c, so we can
   conditionalize on OBJCPLUS.  */
#include "decl.h"
#include "debug.h"
#include "lex.h"
#include "gt-cp-cp-tree-h.h"
#if 0
/* MERGE FAILURE! This file is not generated. Fix it */
#include "gt-cp-decl-h.h"
#endif
#ifdef OBJCPLUS
tree objcp_dummy = 0;
#include "gtype-objcp.h"
#else
tree cp_dummy = 0;
#include "gtype-cp.h"
#endif
/* APPLE LOCAL end Objective-C++ */
