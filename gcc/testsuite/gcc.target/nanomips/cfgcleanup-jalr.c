/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic -mcmodel=auto" } */

extern void foo(void*);
extern void bar(void*);
void test(void* p)
{ if (!p) foo(p); else bar(p); }

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_call\\(foo\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "\\\.reloc\t1f,R_NANOMIPS_JALR16,foo" } } */
/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_call\\(bar\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "\\\.reloc\t1f,R_NANOMIPS_JALR16,bar" } } */