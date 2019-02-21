/* { dg-options "-m32 -march=32r6 -mpid -fno-pic -mcmodel=auto" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0"} { "" } } */

struct {
	int i32;
	signed char i8[2];
	short i16;
} g_var;

char foo ()
{
  return g_var.i8[1];
}

short bar ()
{
  return g_var.i16;
}

/* { dg-final { scan-assembler-not "\taddiu\\.w\t\\\$\[ast0-9\]+,\\\$gp,%gprel\\(g_var.*\\)\n" } } */
