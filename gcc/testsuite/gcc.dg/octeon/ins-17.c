/* Bug #105 */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */

typedef unsigned int uint32;
 
typedef struct bitfield_s {
  uint32      a:11;
  uint32      b:8;
  uint32      c:1;
  uint32      d:1;
  uint32      e:3;
  uint32      f:8;
} bitfield_t;
 
bitfield_t b;
int x = 10, y= 20;

void bar2(bitfield_t *foo)
{

  foo->d = 1;
  foo->c = 0;
 
  foo->a = x;
  foo->f = y;
 
  foo->b = x + y;
}
 
int main()
{
  bar2 (&b);
}
