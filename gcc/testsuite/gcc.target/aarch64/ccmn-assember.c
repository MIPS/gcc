/* { dg-do compile } */
/* { dg-options " -O2 " } */

char foo_c (char a, signed char b)
{
  if (a > 9 && b > -20)
    return 4;
  else
    return 26;
}

char foo_c_1 (char a, signed char b, char c)
{
  if (a > c && b == -20)
    return 4;
  else
    return 26;
}

unsigned short foo_us (unsigned short a, short b)
{
  if (a > 9 && b < -20)
    return 4;
  else
    return 26;
}

unsigned short foo_us_1 (unsigned short a, short b, unsigned short c)
{
  if (a > c && b == -20)
    return 4;
  else
    return 26;
}

int foo_w (int a, int b)
{
  if (a > 9 && b < -20)
    return 4;
  else
    return 26;
}

int foo_w_1 (int a, int b, int c)
{
  if (a > c && b == -20)
    return 4;
  else
    return 26;
}

unsigned foo_uw (unsigned a, int b)
{
  if (a > 9 && b < -20)
    return 4;
  else
    return 26;
}

unsigned foo_uw_1 (unsigned a, int b, unsigned c)
{
  if (a > c && b == -20)
    return 4;
  else
    return 26;
}

long foo_l (long a, long b)
{
  if (a > 9 && b < -20)
    return 4;
  else
    return 26;
}

long foo_l_1 (long a, long b, long c)
{
  if (a > c && b == -20)
    return 4;
  else
    return 26;
}

/* { dg-final { scan-assembler-times "ccmn" 11 } } */
