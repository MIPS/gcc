/* { dg-options "(-mips16) -G4 -mno-abicalls -mcode-readable=no" } */
typedef struct { int a, b, c, d; } A[5000];
int a, b;
extern void bar (int, int, A);

void
foo (int p1)
{
  A c;
  while (p1)
    bar (a, b, c);
}
