/* APPLE LOCAL file STABS */
/* Check if the final SO stab goes into the .text segment */
/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-g" } */

@interface MyClass
+ newWithArg: arg;
@end

@implementation MyClass
+ newWithArg: arg
{
}
@end

/* { dg-final { scan-assembler ".text\n\t.stabs.*100,0,0,Letext\[0-9\]*\nLetext" } } */
