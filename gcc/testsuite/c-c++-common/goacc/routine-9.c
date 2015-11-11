/* Test invalid use of clauses with routine.  */
/* { dg-do compile } */
/* { dg-additional-options "-W */

void Bar ();

void Foo ()
{
  Bar ();
}

#pragma acc routine (Bar) // { dg-error "must be applied before use" }

#pragma acc routine (Foo) gang // { dg-error "must be applied before definition" }

#pragma acc routine (Baz) // { dg-error "not been declared" }

#pragma acc routine  // { dg-error "not followed by single function" }
int i;
