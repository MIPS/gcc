/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "\\tdext" 1 } } */

struct a   
{ 
  unsigned long long c:4; 
  unsigned long long b:40; 
  unsigned long long a:20; 
}; 
 
unsigned long long 
f (struct a a) 
{  
  return a.a; 
} 
