/* APPLE LOCAL file 4209014 */
/* { dg-do compile } */
/* { dg-options "-Os" } */
/* { dg-final { scan-assembler-not "\t(call\t|bl )_inlinex25" } } */
/* { dg-final { scan-assembler "\t(call\t|bl )_calleex25" } } */

/* Insure non-trivial callees (up to 450 "estimated insns") are
   inlined only if marked 'inline' in C/Obj-C.  */

extern void bulk (int);	/* CALLs are assumed to generate 10 'estimated insns'.  */

void
calleex25 (void)
{
  bulk ( 1); bulk ( 2); bulk ( 3); bulk ( 4); bulk ( 5);
  bulk ( 6); bulk ( 7); bulk ( 8); bulk ( 9); bulk (10);
  bulk (11); bulk (12); bulk (13); bulk (14); bulk (15);
  bulk (16); bulk (17); bulk (18); bulk (19); bulk (20);
  bulk (21); bulk (22); bulk (23); bulk (24); bulk (25);
}

void inline
inlinex25 (void)
{
  bulk ( 1); bulk ( 2); bulk ( 3); bulk ( 4); bulk ( 5);
  bulk ( 6); bulk ( 7); bulk ( 8); bulk ( 9); bulk (10);
  bulk (11); bulk (12); bulk (13); bulk (14); bulk (15);
  bulk (16); bulk (17); bulk (18); bulk (19); bulk (20);
  bulk (21); bulk (22); bulk (23); bulk (24); bulk (25);
}

int
main ()
{
  calleex25 ();
  inlinex25 ();
  return 0;
}
