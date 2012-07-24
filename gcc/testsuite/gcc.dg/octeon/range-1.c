/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "andi\t" } } */

typedef unsigned char ee_u8;
static ee_u8 ee_isdigit(ee_u8 c) {
	ee_u8 retval;
	retval = ((c>='0') & (c<='9')) ? 1 : 0;
	return retval;
}

int f(void);
int f1(void);

int g(ee_u8 *a)
{
  if (ee_isdigit (*a))
    f();
  else
    f1();
  return 1;
}
