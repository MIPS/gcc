/* APPLE LOCAL file lvalue cast */
/* { dg-do compile } */
/* { dg-options "-flvalue-cast-assign -faltivec" } */

int foo(void) {

  char *p;
  long l;
  short s;
  vector unsigned int vui;
  volatile int *pvi;

  (long *)p = &l; /* { dg-warning "lvalue cast idiom is deprecated" } */
  ((long *)p)++;  /* { dg-warning "lvalue cast idiom is deprecated" } */
  (short)l = 2;   /* { dg-error "invalid lvalue" } */
  (long)s = 3;    /* { dg-error "invalid lvalue" } */
  (int)pvi = 4;   /* { dg-warning "lvalue cast idiom is deprecated" } */
  (int)pvi &= 5;  /* { dg-warning "lvalue cast idiom is deprecated" } */

  (vector float)vui = (vector float)(1.0, 2.0, 3.0, 4.0); /* { dg-warning "lvalue cast idiom is deprecated" } */

  return 0;
}
