/* ABI Instrumentation
   Copyright (C) 2000-2013 Free Software Foundation, Inc.
   Written by Dodji Seketeli <dodji@redhat.com>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_ABI_INSTR_H
#define GCC_ABI_INSTR_H

void abi_instr_init ();
void abi_instr_finish ();
bool abi_instr_emit_type (const_tree);
bool abi_instr_emit_vars (tree *, int);
bool abi_instr_emit_function(const_tree);
void abi_instr_dump_file ();

#endif	/* ! GCC_ABI_INSTR_H */
