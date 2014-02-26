/* { dg-do compile } */
/* { dg-options " -O2 " } */

char foo_c (char a, char b)
{
  if (a > 9 && b < 20)
    return 4;
  else
    return 26;
}

char foo_c_1 (char a, char b, char c)
{
  if (a > c && b == 20)
    return 4;
  else
    return 26;
}

char foo_c_2 (char a, char b, char c)
{
  if (a < 9 && b > c)
    return 4;
  else
    return 26;
}

unsigned short foo_us (unsigned short a, unsigned short b)
{
  if (a > 9 && b < 20)
    return 4;
  else
    return 26;
}

unsigned short foo_us_1 (unsigned short a, unsigned short b, unsigned short c)
{
  if (a > c && b == 20)
    return 4;
  else
    return 26;
}

unsigned short foo_us_2 (unsigned short a, unsigned short b, unsigned short c)
{
  if (a < 9 && b > c)
    return 4;
  else
    return 26;
}

int foo_w (int a, int b)
{
  if (a > 9 && b < 20)
    return 4;
  else
    return 26;
}

int foo_w_1 (int a, int b, int c)
{
  if (a > c && b == 20)
    return 4;
  else
    return 26;
}

int foo_w_2 (int a, int b, int c)
{
  if (a < 9 && b > c)
    return 4;
  else
    return 26;
}

unsigned foo_uw (unsigned a, unsigned b)
{
  if (a > 9 && b < 20)
    return 4;
  else
    return 26;
}

unsigned foo_uw_1 (unsigned a, unsigned b, unsigned c)
{
  if (a > c && b == 20)
    return 4;
  else
    return 26;
}

unsigned foo_uw_2 (unsigned a, unsigned b, unsigned c)
{
  if (a < 9 && b > c)
    return 4;
  else
    return 26;
}

long foo_l (long a, long b)
{
  if (a > 9 && b < 20)
    return 4;
  else
    return 26;
}

long foo_l_1 (long a, long b, long c)
{
  if (a > c && b == 20)
    return 4;
  else
    return 26;
}

long foo_l_2 (long a, long b, long c)
{
  if (a < 9 && b > c)
    return 4;
  else
    return 26;
}

/* { dg-final { scan-assembler-times "ccmp" 16 } } */
