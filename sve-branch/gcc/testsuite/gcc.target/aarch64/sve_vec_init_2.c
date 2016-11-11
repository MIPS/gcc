/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=256" } */

typedef unsigned int v8si __attribute__ ((vector_size(32)));

void
f (v8si *ptr, int x)
{
  *ptr += (v8si) { x, x, 1, 2, 3, x, x, 4 };
}
