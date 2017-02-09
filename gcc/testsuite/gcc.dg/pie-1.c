/* { dg-do compile { target { ! { *-*-darwin* hppa*-*-* } } } } */
/* { dg-options "-fpie" } */
/* { dg-options "-fpie -mabicalls -mno-gpopt" { target mips*-*-* } } */
/* { dg-require-effective-target fpic } */

#if __PIC__ != 1
# error __PIC__ is not 1!
#endif

#if __PIE__ != 1
# error __PIE__ is not 1!
#endif
