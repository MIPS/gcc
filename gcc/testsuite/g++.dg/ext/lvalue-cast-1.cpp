/* APPLE LOCAL file lvalue cast */
/* { dg-do run } */
/* { dg-options "-flvalue-cast-assign" } */

#include <stdlib.h>
#define CHECK_IF(expr) if (!(expr)) abort ()

static int global;

void f(int &) { global = 35; }
void f(const int &) { global = 78; }

long long_arr[2];

int main(void) {

  char *p;

  (long *)p = long_arr; /* { dg-warning "lvalue cast idiom is deprecated" } */
  ((long *)p)++; /* { dg-warning "lvalue cast idiom is deprecated" } */
  *(long *)p = -1;

  *p = -2;
  CHECK_IF(p[-1] == 0 && p[0] == -2 && p[1] == -1);

  (long *)p += 2; /* { dg-warning "lvalue cast idiom is deprecated" } */
  (long *)p -= 2; /* { dg-warning "lvalue cast idiom is deprecated" } */

  long x = 0;
  f((int)x);
  CHECK_IF(global == 78);
  
  return 0;
}
