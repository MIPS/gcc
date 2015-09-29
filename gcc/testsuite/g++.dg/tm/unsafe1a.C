// Transaction-unsafe testcase from TM TS.
// { dg-options -fgnu-tm }

volatile int * p = 0;
int f() transaction_safe {
  int x = 0;	     // ok: not volatile
  p = &x;	     // ok: the pointer is not volatile
  int i = *p;	     // { dg-error "volatile" "read through volatile glvalue" }
}
