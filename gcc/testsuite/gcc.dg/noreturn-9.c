/* { dg-options "-Wmissing-noreturn -fdiagnostics-show-caret -fblt" } */

extern void test_1(void) __attribute__ ((__noreturn__));

extern void exit(int status) __attribute__ ((__noreturn__));

void
test_1 (void)
{
} /* { dg-warning "'noreturn' function does return" "detect falling off end of noreturn" } */

void
test_2 (void)
{
  exit (0);
}
