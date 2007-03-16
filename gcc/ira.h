/* Communication between the Integrated Register Allocator and rest of
   the compiler.
   Copyright (C) 2006, 2007
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
02110-1301, USA.  */


/* If secondary reloads are the same for inputs and outputs, define those
   macros here.  */

#ifdef SECONDARY_RELOAD_CLASS
#define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#endif

/* If either macro is defined, show that we need secondary reloads.  */
#if defined(SECONDARY_INPUT_RELOAD_CLASS) || defined(SECONDARY_OUTPUT_RELOAD_CLASS)
#define HAVE_SECONDARY_RELOADS
#endif

/* If MEMORY_MOVE_COST isn't defined, give it a default here.  */
#ifndef MEMORY_MOVE_COST
#ifdef HAVE_SECONDARY_RELOADS
#define MEMORY_MOVE_COST(MODE,CLASS,IN) \
  (4 + memory_move_secondary_cost ((MODE), (CLASS), (IN)))
#else
#define MEMORY_MOVE_COST(MODE,CLASS,IN) 4
#endif
#endif

extern GTY (()) struct varray_head_tag *reg_equiv_memory_loc_varray;
extern rtx *reg_equiv_constant;
extern rtx *reg_equiv_memory_loc;
extern rtx *reg_equiv_address;
extern rtx *reg_equiv_mem;

extern void init_ira_once (void);
extern rtx ira_eliminate_regs (rtx, enum machine_mode);

extern void ira (FILE *);
extern void mark_allocation_change (int);
extern void retry_ira_color (int, HARD_REG_SET);
extern rtx reuse_stack_slot (int, unsigned int, unsigned int);
extern void mark_new_stack_slot (rtx, int, unsigned int);
extern void collect_pseudo_call_clobbered_regs (int, HARD_REG_SET *);
