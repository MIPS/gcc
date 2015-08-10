/* GOMP atomic routines

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   Contributed by Mentor Embedded.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

__asm__ ("// BEGIN VAR DEF: libgomp_ptx_lock\n"
	 ".global .align 4 .u32 libgomp_ptx_lock;\n"
	 "\n"
	 "// BEGIN GLOBAL FUNCTION DECL: GOMP_atomic_start\n"
	 ".visible .func GOMP_atomic_start;\n"
	 "// BEGIN GLOBAL FUNCTION DEF: GOMP_atomic_start\n"
	 ".visible .func GOMP_atomic_start\n"
	 "{\n"
	 "	.reg .pred 	%p<2>;\n"
	 "	.reg .s32 	%r<2>;\n"
	 "	.reg .s64 	%rd<2>;\n"
	 "BB5_1:\n"
	 "	mov.u64 	%rd1, libgomp_ptx_lock;\n"
	 "	atom.global.cas.b32 	%r1, [%rd1], 0, 1;\n"
	 "	setp.ne.s32	%p1, %r1, 0;\n"
	 "	@%p1 bra 	BB5_1;\n"
	 "	ret;\n"
	 "	}\n"
	 "// BEGIN GLOBAL FUNCTION DECL: GOMP_atomic_end\n"
	 ".visible .func GOMP_atomic_end;\n"
	 "// BEGIN GLOBAL FUNCTION DEF: GOMP_atomic_end\n"
	 ".visible .func GOMP_atomic_end\n"
	 "{\n"
	 "	.reg .s32 	%r<2>;\n"
	 "	.reg .s64 	%rd<2>;\n"
	 "	mov.u64 	%rd1, libgomp_ptx_lock;\n"
	 "	atom.global.exch.b32 	%r1, [%rd1], 0;\n"
	 "	ret;\n"
	 "	}");
