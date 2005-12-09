/* APPLE LOCAL file 4285232 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
extern void bar(char[4]);
int a, b, c, d;
void foo() {
  char x[4] = { a, b, c, d };
  bar(x);
}
/* { dg-final { scan-assembler-not "\\\$0, " { target i?86-*-darwin* } } } */

