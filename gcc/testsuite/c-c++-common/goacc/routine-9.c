/* Test invalid use of clauses with routine.  */
/* { dg-do compile } */

void Bar ();

void Foo ()
{
  Bar ();
}

#pragma acc routine (Bar) // { dg-error "must be applied before use" }

#pragma acc routine (Foo) gang // { dg-error "must be applied before definition" }

#pragma acc routine (Baz) // { dg-error "not been declared" }

#pragma acc routine
int i;
// { dg-error "not followed by single function" "" { target c } 17 }
// { dg-error "not followed by function declaration or definition" "" { target c++ } 17 }
