/* { dg-do compile { target { elf } } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler ".type\[^\\n\]*object, \(%|@\)object" } } */
/* { dg-final { scan-assembler ".type\[^\\n\]*function, \(%|@\)function" } } */

/* Verify that we generate proper symbol types for external reference.  */

extern int object;
void function (void);

int
foo (void)
{
  function ();
  return object;
}
