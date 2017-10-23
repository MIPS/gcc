/* Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of GCC.

   This file is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#define STARTFN(NAME) .globl NAME; .ent NAME; .type NAME, @function; NAME:
#define ENDFN(NAME) .end NAME; .size NAME, . - NAME

#if defined(__MIPSEB__)
#define A0 $a1
#define A1 $a0
#else
#define A0 $a0
#define A1 $a1
#endif

#ifdef L_ashrdi3
STARTFN(__ashrdi3)
	subu	$a3,$zero,$a2
	srl	A0,A0,$a2
	sll	$a3,A1,$a3
	sra	$a4,A1,$a2
	movz	$a3,$zero,$a2
	sra	A1,A1,31
	slti	$a2,$a2,32
	or	A0,$a3,A0
	movz	A0,$a4,$a2
	movn	A1,$a4,$a2
	jrc	$ra
	ENDFN(__ashrdi3)
#endif

#ifdef L_ashldi3
STARTFN(__ashldi3)
	subu	$a3,$zero,$a2
	sll	A1,A1,$a2
	srl	$a3,A0,$a3
	sll	A0,A0,$a2
	movz	$a3,$zero,$a2
	slti	$a2,$a2,32
	or	A1,$a3,A1
	movz	A1,A0,$a2
	movz	A0,$zero,$a2
	jrc	$ra
	ENDFN(__ashldi3)
#endif

#ifdef L_lshrdi3
STARTFN(__lshrdi3)
	subu	$a3,$zero,$a2
	srl	A0,A0,$a2
	sll	$a3,A1,$a3
	srl	A1,A1,$a2
	movz	$a3,$zero,$a2
	slti	$a2,$a2,32
	or	A0,$a3,A0
	movz	A0,A1,$a2
	movz	A1,$zero,$a2
	jrc	$ra
	ENDFN(__lshrdi3)
#endif
