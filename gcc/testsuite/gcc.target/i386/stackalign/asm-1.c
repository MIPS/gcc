/* { dg-do compile } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-mpreferred-stack-boundary=2" } */

f(){asm("%0"::"r"(1.5F));}g(){asm("%0"::"r"(1.5));}
