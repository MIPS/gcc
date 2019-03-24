/* { dg-do compile { target { ! { *-*-darwin* hppa*64*-*-* } } } } */
/* { dg-options "-fno-pie" } */
/* { dg-options "-fno-pie -mno-abicalls" { target mips*-*-* } } */
/* { dg-xfail-if "FAIL if PIC is enabled by default" { ! nonpic } } */

#ifdef __PIC__
# error __PIC__ is defined!
#endif

#ifdef __PIE__
# error __PIE__ is defined!
#endif
