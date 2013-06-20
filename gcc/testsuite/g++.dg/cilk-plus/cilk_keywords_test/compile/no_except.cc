/* { dg-do compile } */
/* { dg-options "-fno-exceptions" } */

void foo() {
      _Cilk_spawn foo();
}

