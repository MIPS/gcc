/* PR preprocessor/25240 */
/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-Werror" } */

#define weak_extern(symbol) _weak_extern (weak symbol)
#define _weak_extern(expr) _Pragma (#expr)
extern void foo (void);
weak_extern (foo)

void bar (void)
{
  if (foo)
    foo ();
}
