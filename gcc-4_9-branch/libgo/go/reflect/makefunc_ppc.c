// Copyright 2015 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "runtime.h"
#include "go-panic.h"

extern void MakeFuncStubGo(void *, void *) asm ("reflect.MakeFuncStubGo");

/* Structure to store all registers used for parameter passing.  */
typedef struct
{
	long r3;
	long r4;
	long r5;
	long r6;
	long r7;
	long r8;
	long r9;
	long r10;
	/* Pointer to non-register arguments on the stack.  */
	long stack_args;
	double f1;
	double f2;
	double f3;
	double f4;
	double f5;
	double f6;
	double f7;
	double f8;
	double f9;
	double f10;
	double f11;
	double f12;
	double f13;
} ppcRegs;

void makeFuncStub(
	long r3, long r4, long r5, long r6, long r7, long r8, long r9, long r10,
	long stackargs,
	double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8,
	double f9, double f10, double f11, double f12, double f13)
	asm ("reflect.makeFuncStub");

void makeFuncStub(
	long r3, long r4, long r5, long r6, long r7, long r8, long r9, long r10,
	long stackargs,
	double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8,
	double f9, double f10, double f11, double f12, double f13)
{
	ppcRegs regs;
	void *closure;

	/* Store the registers in a structure that is passed on to the Go stub
	   function.  */
	regs.r3 = r3;
	regs.r4 = r4;
	regs.r5 = r5;
	regs.r6 = r6;
	regs.r7 = r7;
	regs.r8 = r8;
	regs.r9 = r9;
	regs.r10 = r10;
	regs.stack_args = (long)&stackargs;
	regs.f1 = f1;
	regs.f2 = f2;
	regs.f3 = f3;
	regs.f4 = f4;
	regs.f5 = f5;
	regs.f6 = f6;
	regs.f7 = f7;
	regs.f8 = f8;
	regs.f9 = f9;
	regs.f10 = f10;
	regs.f11 = f11;
	regs.f12 = f12;
	regs.f13 = f13;

	__go_makefunc_can_recover( __builtin_return_address (0));
	/* Call the Go stub function.  */
	closure = __go_get_closure();
	MakeFuncStubGo(&regs, closure);
	/* MakeFunc functions can no longer call recover.  */
	__go_makefunc_returning();
	/* Restore all possible return registers.  */

	asm volatile ("ld\t3,0(%0)" : : "a" (&regs.r3) : "r3" );
	asm volatile ("ld\t4,0(%0)" : : "a" (&regs.r4) : "r4" );
	asm volatile ("lfd\t1,0(%0)" : : "a" (&regs.f1) : "fr1" );
	asm volatile ("lfd\t2,0(%0)" : : "a" (&regs.f2) : "fr2" );
	asm volatile ("lfd\t3,0(%0)" : : "a" (&regs.f3) : "fr3" );
	asm volatile ("lfd\t4,0(%0)" : : "a" (&regs.f4) : "fr4" );

	return;
}
