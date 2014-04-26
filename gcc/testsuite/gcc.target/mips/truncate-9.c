/* { dg-options "-mgp64 isa_rev>=2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "dext"  } } */
/* { dg-final { scan-assembler-not "andi" } } */

void abort (void);
void exit (int);

typedef struct
{
  unsigned char a;
  unsigned short b __attribute__ ((packed));
}
three_byte_t;

unsigned char f ();
unsigned short g ();

void h(three_byte_t *);

main ()
{
  three_byte_t three_byte;
  three_byte.a = f ();
  three_byte.b = g ();
  h(&three_byte);
  if (three_byte.a != 0xab || three_byte.b != 0x1234)
    abort ();
  exit (0);
}
