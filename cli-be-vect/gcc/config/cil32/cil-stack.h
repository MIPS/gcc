/* Prototypes for the stack information functionality.

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

#ifndef CIL_STACK_H
#define CIL_STACK_H

#include "tree.h"
#include "tree-flow.h"
#include "cil-types.h"
#include "cil-stmt.h"

/******************************************************************************
 * CIL stack types                                                            *
 ******************************************************************************/

/* Represents CIL types on the stack including types which wouldn't be on a
   real CIL evaluation stack. The extra types are useful for optimizations
   which rely on precise stack typization. */

enum cil_type
{
#define CIL_TYPEDEF(A,B) A,
#include "cil-types.def"
#undef CIL_TYPEDEF
CIL_NUM_STACKTYPES
};

typedef enum cil_type cil_type_t;

/* Represents the CIL stack.  */

DEF_VEC_I(cil_type_t);
DEF_VEC_ALLOC_I(cil_type_t, heap);

struct cil_stack_d
{
  VEC(cil_type_t, heap) *vec;
};

typedef struct cil_stack_d *cil_stack;
typedef const struct cil_stack_d *const_cil_stack;

/* Container used for holding a stack for each basic block in the current
   function.  */

struct cil_bb_stacks_d
{
  cil_stack *stacks;
};

typedef struct cil_bb_stacks_d *cil_bb_stacks;
typedef const struct cil_bb_stacks_d *const_cil_bb_stacks;

/******************************************************************************
 * Stack information interface                                                *
 ******************************************************************************/

extern cil_stack cil_stack_create (void);
extern cil_stack cil_stack_copy (const_cil_stack);
extern void cil_stack_free (cil_stack);
extern void cil_stack_after_stmt (cil_stack, cil_stmt);
extern size_t cil_stack_depth (const_cil_stack);
extern cil_type_t cil_stack_top (const_cil_stack);
extern cil_type_t cil_stack_peek (const_cil_stack, size_t);

extern cil_bb_stacks cil_bb_stacks_create (void);
extern void cil_bb_stacks_destroy (cil_bb_stacks);
extern cil_stack cil_stack_for_bb (const_cil_bb_stacks, basic_block);
extern void cil_set_stack_for_bb (cil_bb_stacks, basic_block, cil_stack);

/******************************************************************************
 * CIL type interface                                                         *
 ******************************************************************************/

extern bool cil_integer_p (cil_type_t);
extern bool cil_native_integer_p (cil_type_t);
extern bool cil_complex_p (cil_type_t);
extern bool cil_float_p (cil_type_t);
extern bool cil_vector_p (cil_type_t);
extern bool cil_pointer_p (cil_type_t);
extern bool cil_int_or_smaller_p (cil_type_t);
extern bool cil_long_p (cil_type_t);
extern bool cil_unsigned_int_p (cil_type_t);
extern cil_type_t complex_to_cil (const_tree);
extern cil_type_t scalar_to_cil (const_tree);
extern cil_type_t vector_to_cil (const_tree);
extern cil_type_t type_to_cil (const_tree);
extern cil_type_t type_to_cil_on_stack (const_tree);
extern bool cil_pointer_type_p (const_tree);
extern bool cil_value_type_p (const_tree);
extern bool cil_vector_type_p (const_tree);

#endif /* CIL_STACK_H */
