/* Test call to static variable.  */

/* MIPS compressed ISAs require the LSB of an address to indicate which
   ISA mode to use.  This test cannot do that and raises an assembler
   warning (binutils 2.29 onwards) of a branch to a different ISA.  */
/* { dg-skip-if "" { mips_compressed } } */

typedef struct
{
  long long a[10];
} A;
  
void bar (A *);
  
typedef int (*B)(int);
  
void foo (void)
{
  static A a;
  bar (&a);
  (*(B)&a) (1);
}
