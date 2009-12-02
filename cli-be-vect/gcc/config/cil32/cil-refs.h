/* Database of entities referenced in a compilation unit.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#ifndef CIL_REFS_H
#define CIL_REFS_H

#include "coretypes.h"
#include "ggc.h"
#include "debug.h"
#include "hashtab.h"
#include "tree.h"
#include "cil-types.h"

/******************************************************************************
 * Macros                                                                     *
 ******************************************************************************/

/* Nonzero for a type which is at file scope.  */
#define TYPE_FILE_SCOPE_P(EXP) \
  (!TYPE_CONTEXT (EXP) \
   || TREE_CODE (TYPE_CONTEXT (EXP)) == TRANSLATION_UNIT_DECL)

/* Nonzero for a zero-length array type */
#define ARRAY_TYPE_ZEROLENGTH(EXP) \
  ((TYPE_SIZE (EXP) == NULL_TREE) || integer_zerop (TYPE_SIZE (EXP)))

/* Nonzero for a variable-length array type */
#define ARRAY_TYPE_VARLENGTH(EXP) \
  (TYPE_SIZE (EXP) != NULL_TREE && TREE_CODE (TYPE_SIZE (EXP)) != INTEGER_CST)

/* Length of compacted identifiers (in characters) */
#define COMPACT_ID_LENGTH 16

/*****************************************************************************
 * Initialization and teardown                                               *
 *****************************************************************************/

extern void refs_init (void);
extern void refs_fini (void);

/*****************************************************************************
 * Referenced assemblies                                                     *
 *****************************************************************************/

extern void add_referenced_assembly (const char *);
extern htab_t pending_assemblies_htab ( void );
extern void mark_pending_assemblies ( void );

/*****************************************************************************
 * Types                                                                     *
 *****************************************************************************/

extern void mark_referenced_type (tree);
extern htab_t referenced_types_htab ( void );
extern tree promote_type_for_vararg (tree);
extern tree promote_local_var_type (tree);
extern tree get_integer_type (unsigned int, bool);

/******************************************************************************
 * Strings                                                                    *
 ******************************************************************************/

extern tree mark_referenced_string (tree);
extern unsigned int get_string_cst_id (tree);
extern htab_t referenced_strings_htab ( void );

/******************************************************************************
 * Functions                                                                  *
 ******************************************************************************/

extern void add_pinvoke (tree);
extern htab_t pinvokes_htab ( void );

/******************************************************************************
 * Labels                                                                     *
 ******************************************************************************/

extern void record_addr_taken_label (tree);
extern tree get_addr_taken_label_id (tree);
extern tree get_label_addrs ( void );

/******************************************************************************
 * Constructors                                                               *
 ******************************************************************************/

extern void record_ctor (tree);
extern VEC(tree, gc) *pending_ctors_vec ( void );
extern cil_seq expand_init_to_cil_seq (tree, tree);
extern void cil_lower_init (cil_seq);


#endif /* !CIL_REFS_H */
