/* Database of entities referenced in a compilation unit.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
Erven Rohou             <erven.rohou@st.com>
*/

#ifndef CIL_REFS_H
#define CIL_REFS_H

#include "coretypes.h"
#include "ggc.h"
#include "debug.h"
#include "vec.h"
#include "tree.h"
#include "cil-types.h"

/******************************************************************************
 * Macros                                                                     *
 ******************************************************************************/

/* Nonzero for a type which is at file scope.  */
#define TYPE_FILE_SCOPE_P(EXP)                                  \
  (! TYPE_CONTEXT (EXP)                                         \
   || TREE_CODE (TYPE_CONTEXT (EXP)) == TRANSLATION_UNIT_DECL)

/* Nonzero for a zero-length array type */
#define ARRAY_TYPE_ZEROLENGTH(EXP)                              \
  (TYPE_SIZE (EXP) == NULL_TREE)

/* Nonzero for a variable-length array type */
#define ARRAY_TYPE_VARLENGTH(EXP)                               \
  (TYPE_SIZE (EXP) != NULL_TREE && TREE_CODE (TYPE_SIZE (EXP)) != INTEGER_CST)

/* Length of compacted identifiers (in characters) */
#define COMPACT_ID_LENGTH 16

/*****************************************************************************
 * Initialization and teardown                                               *
 *****************************************************************************/

extern void refs_init (void);
extern void refs_fini (void);
extern void refs_begin_new_function (void);

/*****************************************************************************
 * Types                                                                     *
 *****************************************************************************/

extern void mark_referenced_type (tree);

/* Referenced pinvoke functions iterator.  */

struct types_iterator_d
{
  /* Vector of referenced types.  */
  VEC (tree, heap) *types;

  /* Current type.  */
  unsigned int i;
};

typedef struct types_iterator_d *types_iterator;

extern types_iterator rti_begin (void);
static inline void rti_destroy (types_iterator);
static inline bool rti_end_p (types_iterator);
static inline tree rti_type (types_iterator);
static inline void rti_next (types_iterator);

/* Disposes of the referenced types iterator specified by RTI.  */

static inline void
rti_destroy (types_iterator rti)
{
  VEC_free (tree, heap, rti->types);
  XDELETE (rti);
}

/* Returns false if some referenced types which haven't yet been iterated
   over are present, true otherwise.  */

static inline bool
rti_end_p (types_iterator rti)
{
  if (rti->i == VEC_length (tree, rti->types))
    return true;
  else
    return false;
}

/* Returns the TYPE_DECL of the current referenced type.  */

static inline tree
rti_type (types_iterator rti)
{
  return VEC_index (tree, rti->types, rti->i);
}

/* Move to the next type.  */

static inline void
rti_next (types_iterator rti)
{
  gcc_assert (rti->i < VEC_length (tree, rti->types));

  rti->i++;
}

/******************************************************************************
 * Strings                                                                    *
 ******************************************************************************/

extern tree mark_referenced_string (tree);
extern unsigned int get_string_cst_id (tree);

/* Referenced strings vector.  */

DEF_VEC_P(str_ref);
DEF_VEC_ALLOC_P(str_ref, heap);

/* Referenced strings iterator.  */

struct ref_str_iterator_d
{
  /* Vector of referenced strings.  */
  VEC (str_ref, heap) *strings;

  /* Current string.  */
  unsigned int i;
};

typedef struct ref_str_iterator_d *ref_str_iterator;

extern ref_str_iterator rsi_begin (void);
static inline void rsi_destroy (ref_str_iterator);
static inline bool rsi_end_p (ref_str_iterator);
static inline tree rsi_string (ref_str_iterator);
static inline unsigned int rsi_id (ref_str_iterator);
static inline void rsi_next (ref_str_iterator);

/* Disposes of the string reference iterator specified by RSI.  */

static inline void
rsi_destroy (ref_str_iterator rsi)
{
  VEC_free (str_ref, heap, rsi->strings);
  XDELETE (rsi);
}

/* Returns false if some strings which haven't yet been iterated over are
   present, true otherwise.  */

static inline bool
rsi_end_p (ref_str_iterator rsi)
{
  if (rsi->i == VEC_length (str_ref, rsi->strings))
    return true;
  else
    return false;
}

/* Returns the STRING_CST associated with the current string.  */

static inline tree
rsi_string (ref_str_iterator rsi)
{
  str_ref elem = VEC_index (str_ref, rsi->strings, rsi->i);

  return elem->cst;
}

/* Return the unique ID associated with the current string.  */

static inline unsigned int
rsi_id (ref_str_iterator rsi)
{
  return VEC_index (str_ref, rsi->strings, rsi->i)->id;
}

/* Move to the next string.  */

static inline void
rsi_next (ref_str_iterator rsi)
{
  gcc_assert (rsi->i < VEC_length (str_ref, rsi->strings));

  rsi->i++;
}

/******************************************************************************
 * Functions                                                                  *
 ******************************************************************************/

extern void cil_add_pinvoke (tree);

/* Referenced pinvoke functions iterator.  */

struct pinvoke_iterator_d
{
  /* Vector of referenced pinvoke functions.  */
  VEC (tree, heap) *functions;

  /* Current function.  */
  unsigned int i;
};

typedef struct pinvoke_iterator_d *pinvoke_iterator;

extern pinvoke_iterator rpi_begin (void);
static inline void rpi_destroy (pinvoke_iterator);
static inline bool rpi_end_p (pinvoke_iterator);
static inline tree rpi_function (pinvoke_iterator);
static inline void rpi_next (pinvoke_iterator);

/* Disposes of the pinvoked function iterator specified by RPI.  */

static inline void
rpi_destroy (pinvoke_iterator rpi)
{
  VEC_free (tree, heap, rpi->functions);
  XDELETE (rpi);
}

/* Returns false if some pinvoked functions which haven't yet been iterated
   over are present, true otherwise.  */

static inline bool
rpi_end_p (pinvoke_iterator rpi)
{
  if (rpi->i == VEC_length (tree, rpi->functions))
    return true;
  else
    return false;
}

/* Returns the FUNCTION_DECL of the current pinvoked function.  */

static inline tree
rpi_function (pinvoke_iterator rpi)
{
  return VEC_index (tree, rpi->functions, rpi->i);
}

/* Move to the next function.  */

static inline void
rpi_next (pinvoke_iterator rpi)
{
  gcc_assert (rpi->i < VEC_length (tree, rpi->functions));

  rpi->i++;
}

/******************************************************************************
 * Labels                                                                     *
 ******************************************************************************/

extern void record_addr_taken_label (tree);
extern tree get_addr_taken_label_id (tree);
extern tree *get_label_addrs ( void );
extern unsigned int get_label_addrs_n ( void );

#endif /* !CIL_REFS_H */
