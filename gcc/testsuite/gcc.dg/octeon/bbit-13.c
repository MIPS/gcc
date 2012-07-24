/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -mbranch-likely" } */
/* { dg-final { scan-assembler "bbit\[01\]\t" } } */
/* { dg-final { scan-assembler-not "bbit\[01\]l\t" } } */
/* { dg-final { scan-assembler "bnel\t" } } */
/* { dg-final { scan-assembler-not "bne\t" } } */

int 
f (int n, int i) 
{ 
  int s = 0; 
  for (; i & 1; i++) 
    s += i; 
  return s; 
} 

int 
g (int n, int i) 
{ 
  int s = 0; 
  for (i = 0; i < n; i++) 
    s += i; 
  return s; 
} 
