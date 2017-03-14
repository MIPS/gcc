/* { dg-do compile { target { ! { *-*-darwin* hppa*64*-*-* } } } } */
/* { dg-options "-fno-PIC" } */
/* { dg-options "-fno-PIC -mno-abicalls" { target mips*-*-* } } */

#ifdef __PIC__
# error __PIC__ is defined!
#endif

#ifdef __PIE__
# error __PIE__ is defined!
#endif
