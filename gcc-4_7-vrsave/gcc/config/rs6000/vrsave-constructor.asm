/*
 * Special support for using a constructor to setup VRSAVE to indicate we will
 * be using saved VMX registers.  Unlike the original altivec specification, in
 * Linux, the individual vector registers are not used in the VRSAVE register.
 * Instead, setjmp/longjmp just look to see if it is 0 or non-0.  This function
 * is invoked as a constructor in functions that use the VMX registers.
 *
 * Copyright (C) 2011
 * Free Software Foundation, Inc.
 * Written By Michael Meissner
 * 
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option) any
 * later version.
 * 
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 */ 

/* Do any initializations needed for the linux environment */

	.section ".text"
	#include "ppc-asm.h"

	CFI_STARTPROC
HIDDEN_FUNC(__use_vmx_registers)
	li 0,-1
	mtvrsave 0
	blr
FUNC_END(__use_vmx_registers)
CFI_ENDPROC
