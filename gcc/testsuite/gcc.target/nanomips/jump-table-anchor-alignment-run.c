/* { dg-do run } */
/* { dg-additional-options "-falign-labels=8 -mjump-table-opt" } */

int __attribute__((noinline))
jumptab (int a)
{
  int b = a;
  switch (a)
    {
    case 0: b = a * a * 3; break;     /* 0   */
    case 1: b = a / 3 + a + 1; break; /* 2   */
    case 2: b = a + 3 / a; break;     /* 3   */
    case 3: b = a - 3 + a * 3; break; /* 9   */
    case 4: b = a * a * a * a; break; /* 256 */
    case 5: b = a + a * a * 8; break; /* 205 */
    default: break;
    }
  return b;
}

int main (void)
{
  int a;
  int b = 0;
  for (a = 0; a <= 6; a++)
    {
      switch (a)
	{
	case 0: b = a * a * 3; break;     /* 0   */
	case 1: b = a / 3 + a + 1; break; /* 2   */
	case 2: b = a + 3 / a; break;     /* 3   */
	case 3: b = a - 3 + a * 3; break; /* 9   */
	case 4: b = a * a * a * a; break; /* 256 */
	case 5: b = a + a * a * 8; break; /* 205 */
	default: b = a; break;
	}
      if (jumptab(a) != b)
	return 1;
    }
  return 0;
}
