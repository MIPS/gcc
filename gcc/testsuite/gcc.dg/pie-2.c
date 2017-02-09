/* { dg-do compile { target { ! { mips*-*-* } } } } */
/* { dg-options "-fPIE" } */
/* { dg-options "-fPIE -mabicalls" { target mips*-*-* } } */
/* { dg-require-effective-target fpic } */

#if __PIC__ != 2
# error __PIC__ is not 2!
#endif

#if __PIE__ != 2
# error __PIE__ is not 2!
#endif
