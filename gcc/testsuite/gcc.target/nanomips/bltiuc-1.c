/* { dg-options "-m32 isa_rev>=6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void abort (void);

static
long long min ()
{
  return -__LONG_LONG_MAX__ - 1;
}

void
foo (long long j)
{
  if (j > 10 || j < min ())
    abort ();
}

/* { dg-final { scan-assembler "\tbltiuc\t\\\$a\[0-9\]+,11,\[\\\$.\]L" } } */
