/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx2 -mavx -mtune=sandybridge" } */

extern char *strcpy (char *, const char *);

void
foo (char *s)
{
  strcpy (s,
	  "1234567890abcdef123456abcdef5678123456abcdef567abcdef678"
	  "1234567");
}

/* { dg-final { scan-assembler-times "vmovdqa\[ \\t\]+\[^\n\]*%xmm" 4 } } */
/* { dg-final { scan-assembler-times "vmovups\[ \\t\]+\[^\n\]*%xmm" 4 } } */
/* No need to dynamically realign the stack here.  */
/* { dg-final { scan-assembler-not "and\[^\n\r]*%\[re\]sp" } } */
/* Nor use a frame pointer.  */
/* { dg-final { scan-assembler-not "%\[re\]bp" } } */
