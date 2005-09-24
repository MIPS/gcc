/* { dg-do compile } */
/* This test won't currently work on targets with types larger than 8 bytes. */
/* { dg-xfail-if "" { "i?86-*-*" } { "*" } { "" } } */
/* { dg-xfail-if "" { "x86_64-*-*" } { "*" } { "" } } */

long double z;

void f3(void)
{
  #pragma omp atomic
    z++;
  #pragma omp atomic
    z--;
  #pragma omp atomic
    ++z;
  #pragma omp atomic
    --z;
  #pragma omp atomic
    z += 1;
  #pragma omp atomic
    z *= 3;
  #pragma omp atomic
    z /= 3;
}
