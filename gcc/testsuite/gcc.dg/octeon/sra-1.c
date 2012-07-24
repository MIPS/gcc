/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */


struct bar 
{ 
  unsigned long long a:15; 
  long long b:48; 
  unsigned long long c:1; 
}; 

long long 
g1 (struct bar s) 
{ 
  return s.b; 
} 

/* This really should just produce a dsll followed by a dsra. */
/* { dg-final { scan-assembler-not "xor" } } */
/* { dg-final { scan-assembler-not "dext" } } */
/* { dg-final { scan-assembler "dsll" } } */
/* { dg-final { scan-assembler "dsra" } } */


