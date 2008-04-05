/* Communication between the Integrated Register Allocator (IRA) and
   the rest of the compiler.
   Copyright (C) 2006, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

extern void init_ira_once (void);
extern void init_ira (void);
extern void finish_ira_once (void);
extern rtx ira_eliminate_regs (rtx, enum machine_mode);
extern void sort_insn_chain (int);

extern void sort_regnos_for_alter_reg (int *, int, unsigned int *);
extern void mark_allocation_change (int);
extern void mark_memory_move_deletion (int, int);
extern int reassign_pseudos (int *, int, HARD_REG_SET, HARD_REG_SET *,
			     HARD_REG_SET *, bitmap);
extern rtx reuse_stack_slot (int, unsigned int, unsigned int);
extern void mark_new_stack_slot (rtx, int, unsigned int);
extern int better_spill_reload_regno_p (int *, int *, rtx, rtx, rtx);
extern void collect_pseudo_call_clobbered_regs (int, HARD_REG_SET *);

