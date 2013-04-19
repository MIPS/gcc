/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */

#if HAVE_IO
#include <stdio.h>
#endif

#define N 256

template <class T>
int
main2 (int argc, char **argv)
{ 
  T A[N], B[N];
  T A2[N][N], B2[N][N];
  T A3[N][N][N], B3[N][N][N];
  T A4[N][N][N][N], B4[N][N][N][N];
    A[0:(N / 4)] = A[4]+ B[0:(N / 2):2]; /* { dg-error "length mismatch between" } */ 
    A[0:(N / 4)] = B[0:(N / 2):2] + N; /* { dg-error "length mismatch between" } */
    A[0:(N / 4)] = B[0:(N / 2):2] + A[4]; /* { dg-error "length mismatch between" } */
    A[0:10:1] = B[0:10:1] + A[0:5:1]; /* { dg-error "length mismatch in " } */
    A[0:10:1] = B[0:5:1] + A[0:10:1]; /* { dg-error "length mismatch in " } */
    A2[:][:] = B2[:][0:5] + B2[:][0:10]; /* { dg-error "length mismatch in " } */
    A2[5][:] = B2[5][0:5] + B2[3][0:10]; /* { dg-error "length mismatch in " } */
    A3[:][5][:] = B3[:][5][0:5] + B3[:][3][0:10]; /* { dg-error "length mismatch in " } */
    A3[:][5][:] = B3[5][:][0:5] + B3[:][3][0:10]; /* { dg-error "length mismatch in " } */
    A4[:][:][:][:] = B4[:][:][0:5][:] + B4[:][:][:][0:5:1]; /* { dg-error "length mismatch in " } */
    return 0;
}

int main (int argc, char **argv)
{
  main2 <char> (argc, argv);
  main2 <short> (argc, argv);
  main2 <int> (argc, argv);
  main2 <long> (argc, argv);
  main2 <long long> (argc, argv);
  return 0;
}
