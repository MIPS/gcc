/* { dg-do compile } */
/* { dg-options "-O2 -fuse-caller-save -fdump-rtl-reload" } */
/* Testing -fuse-caller-save optimization option.  */

static int __attribute__((noinline))
bar (int x)
{
  return x + 3;
}

int __attribute__((noinline))
foo (int y)
{
  return y + bar (y);
}

int
main (void)
{
  return !(foo (5) == 13);
}

/* { dg-final { scan-rtl-dump-times "Found 1 pseudo.* with a call used reg crossing a call" 1 "reload" } } */
/* { dg-final { scan-rtl-dump-times "Found .* pseudo.* with a call used reg crossing a call" 1 "reload" } } */
/* { dg-final { scan-rtl-dump-times "Allowing due to -fuse-caller-save" 1 "reload" } } */
/* { dg-final { cleanup-rtl-dump "reload" } } */
