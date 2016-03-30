/* CMSE wrapper function used to save, clear and restore callee saved registers
   for cmse_nonsecure_call's.

   Copyright (C) 2016 Free Software Foundation, Inc.
   Contributed by ARM Ltd.

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

.syntax unified
.thumb
.global __gnu_cmse_nonsecure_call
__gnu_cmse_nonsecure_call:
#if defined(__ARM_ARCH_8M_MAIN__)
push	    {r5-r11,lr}
mov	    r5, r4
mov	    r6, r4
mov	    r7, r4
mov	    r8, r4
mov	    r9, r4
mov	    r10, r4
mov	    r11, r4
mov	    ip, r4
#if !defined(__SOFTFP__)
vpush.f64   {d8-d15}
#if __ARM_FP & 0x04
vmov.f32    s16, #1.00000
vmov.f32    s17, #1.00000
vmov.f32    s18, #1.00000
vmov.f32    s19, #1.00000
vmov.f32    s20, #1.00000
vmov.f32    s21, #1.00000
vmov.f32    s22, #1.00000
vmov.f32    s23, #1.00000
vmov.f32    s24, #1.00000
vmov.f32    s25, #1.00000
vmov.f32    s26, #1.00000
vmov.f32    s27, #1.00000
vmov.f32    s28, #1.00000
vmov.f32    s29, #1.00000
vmov.f32    s30, #1.00000
vmov.f32    s31, #1.00000
#elif __ARM_FP & 0x08
vmov.f64    d8, #1.00000
vmov.f64    d9, #1.00000
vmov.f64    d10, #1.00000
vmov.f64    d11, #1.00000
vmov.f64    d12, #1.00000
vmov.f64    d13, #1.00000
vmov.f64    d14, #1.00000
vmov.f64    d15, #1.00000
#else
#error "Half precision implementation not supported."
#endif
vmsr	    fpscr, r4
#endif
#if defined(__ARM_FEATURE_DSP)
msr	    APSR_nzcvqg, r4
#else
msr	    APSR_nzcvq, r4
#endif
blxns	    r4
#if !defined(__SOFTFP__)
vpop.f64    {d8-d15}
#endif
pop	    {r5-r11}
#elif defined (__ARM_ARCH_8M_BASE__)
push	    {r5-r7}
mov	    r5, r8
mov	    r6, r9
mov	    r7, r10
push	    {r5-r7}
mov	    r5, r11
mov	    r6, lr
push	    {r5,r6}
mov	    r5, r4
mov	    r6, r4
mov	    r7, r4
mov	    r8, r4
mov	    r9, r4
mov	    r10, r4
mov	    r11, r4
mov	    ip, r4
msr	    APSR_nzcvq, r4
blxns	    r4
pop	    {r5, r6}
mov	    lr, r6
mov	    r11, r5
pop	    {r5-r7}
mov	    r10, r7
mov	    r9, r6
mov	    r8, r5
pop	    {r5-r7}
blx	    lr
#else
#error "This should only be used for armv8-m base- and mainline."
#endif
