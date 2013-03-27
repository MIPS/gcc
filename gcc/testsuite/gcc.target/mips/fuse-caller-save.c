/* { dg-do compile } */
/* { dg-options "-fuse-caller-save" } */
/* { dg-skip-if "" { *-*-* }  { "*" } { "-Os" } } */
/* Testing -fuse-caller-save optimization option.  */

static int __attribute__((noinline)) NOCOMPRESSION
bar (int x)
{
  return x + 3;
}

int __attribute__((noinline)) NOCOMPRESSION
foo (int y)
{
  return y + bar (y);
}

int NOCOMPRESSION
main (void)
{
  return !(foo (5) == 13);
}

/* Check that there are only 2 stack-saves: r31 in main and foo.  */

/* Check that there only 2 sw/sd.  */
/* { dg-final { scan-assembler-times "(?n)s\[wd\]\t\\\$.*,.*\\(\\\$sp\\)" 2 } } */

/* Check that the first caller-save register is unused.  */
/* { dg-final { scan-assembler-not "\\\$16" } } */
