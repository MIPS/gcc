/* { dg-do compile { target { ! { *-*-darwin* hppa*64*-*-* } } } } */
/* { dg-options "-fno-PIE" } */
/* { dg-options "-fno-PIE -mno-abicalls" { target mips*-*-* } } */
/* { dg-xfail-if "FAIL if PIC is enabled by default" { ! nonpic } } */

#ifdef __PIC__
# error __PIC__ is defined!
#endif

#ifdef __PIE__
# error __PIE__ is defined!
#endif
