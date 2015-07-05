/* { dg-do compile { target { elf } } } */
/* { dg-require-effective-target tls } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not ".type\[^\\n\]*object, \(%|@\)object" } } */

/* Verify that we generate proper symbol types for external reference.  */

extern __thread int object;

int
foo (void)
{
  return object;
}
