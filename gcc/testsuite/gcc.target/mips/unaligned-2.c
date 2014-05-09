/* { dg-do compile } */
/* { dg-options "isa_rev>=2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* Load from an unaligned short, should only produce an ins and
   no other sign or zero extend instructions. */
/* { dg-final { scan-assembler "ins\t" } } */
/* { dg-final { scan-assembler-not "\texts\t" } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */
/* { dg-final { scan-assembler-not "\tdsll\t" } } */
/* { dg-final { scan-assembler-not "\tandi\t" } } */
/* { dg-final { scan-assembler-not "\tor\t" } } */

struct s 
{ 
  char c; 
  unsigned short s; 
} __attribute__ ((packed));

int f (struct s *s) 
{ 
  return s->s;
} 
