/* PR optimization/13488  */
extern void abort (void);
extern void exit (int);

#ifdef __i386__
# define regparm __attribute__((regparm (1)))
#else
# define regparm
#endif

regparm long
ll2c_1 (int a, long long x)
{
  x = (signed char) x;
  return x;
}

regparm long
ll2c_2 (int a, long long x)
{
  return (signed char) x;
}

regparm long
ll2s_1 (int a, long long x)
{
  x = (short) x;
  return x;
}

regparm long
ll2s_2 (int a, long long x)
{
  return (short) x;
}

regparm long
ll2i_1 (int a, long long x)
{
  x = (int) x;
  return x;
}

regparm long
ll2i_2 (int a, long long x)
{
  return (int) x;
}

int
main (void)
{
  long long a;
  for (a = 0LL; a < 100000000000LL; a += 10000001000LL)
    {
      if (ll2c_1 (0, a) != ll2c_2 (0, a)
	  || ll2s_1 (0, a) != ll2s_2 (0, a)
	  || ll2i_1 (0, a) != ll2i_2 (0, a))
	abort ();
    }
  exit (0);
}
