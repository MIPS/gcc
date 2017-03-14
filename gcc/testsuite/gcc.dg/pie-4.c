/* { dg-do compile { target { ! { *-*-darwin* hppa*64*-*-* } } } } */
/* { dg-options "-fno-PIE" } */
/* { dg-options "-fno-PIE -mno-abicalls" { target mips*-*-* } } */

#ifdef __PIC__
# error __PIC__ is defined!
#endif

#ifdef __PIE__
# error __PIE__ is defined!
#endif
