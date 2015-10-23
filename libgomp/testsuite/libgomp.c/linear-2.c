#pragma omp declare target
int a[256];
#pragma omp end declare target

__attribute__((noinline, noclone)) void
f1 (int i)
{
  #pragma omp target teams distribute parallel for linear (i: 4)
  for (int j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f2 (short int i, char k)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1)
  for (long j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f3 (long long int i, long long int k)
{
  #pragma omp target teams distribute parallel for linear (i: k)
  for (short j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f4 (int i)
{
  #pragma omp target teams distribute parallel for linear (i: 4) schedule(static, 3)
  for (int j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f5 (short int i, char k)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1) schedule(static, 5)
  for (long j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f6 (long long int i, long long int k)
{
  #pragma omp target teams distribute parallel for linear (i: k) schedule(static, 7)
  for (short j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f7 (int i)
{
  #pragma omp target teams distribute parallel for linear (i: 4) schedule(dynamic, 3)
  for (int j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f8 (short int i, char k)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1) schedule(dynamic, 5)
  for (long j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f9 (long long int i, long long int k)
{
  #pragma omp target teams distribute parallel for linear (i: k) schedule(dynamic, 7)
  for (short j = 16; j < 64; j++)
    {
      a[i] = j;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f10 (int i, char start, long step)
{
  #pragma omp target teams distribute parallel for linear (i: 4)
  for (int j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f11 (short int i, char k, long start, char step)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1)
  for (long j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f12 (long long int i, long long int k, long long int start, int step)
{
  #pragma omp target teams distribute parallel for linear (i: k)
  for (short j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f13 (int i, int start, long long int step)
{
  #pragma omp target teams distribute parallel for linear (i: 4) schedule(static, 3)
  for (int j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f14 (short int i, char k, unsigned long long int start, int step)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1) schedule(static, 5)
  for (long j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f15 (long long int i, long long int k, char start, long int step)
{
  #pragma omp target teams distribute parallel for linear (i: k) schedule(static, 7)
  for (short j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f16 (int i, int start, long long int step)
{
  #pragma omp target teams distribute parallel for linear (i: 4) schedule(dynamic, 3)
  for (int j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f17 (short int i, char k, long start, int step)
{
  #pragma omp target teams distribute parallel for linear (i: k + 1) schedule(dynamic, 5)
  for (long j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

__attribute__((noinline, noclone)) void
f18 (long long int i, long long int k, short start, long int step)
{
  #pragma omp target teams distribute parallel for linear (i: k) schedule(dynamic, 7)
  for (short j = start; j < 112; j += step)
    {
      a[i] = j / 2 + 8;
      i += 4;
    }
}

void
verify (void)
{
  int err;
  #pragma omp target map(from:err)
  {
    err = 0;
    for (int i = 0; i < 256; i++)
      if (a[i] != (((i & 3) == 0 && i >= 8
		    && i < 8 + 48 * 4)
		   ? ((i - 8) / 4) + 16 : 0))
	err = 1;
    __builtin_memset (a, 0, sizeof (a));
  }
  if (err)
    __builtin_abort ();
}

int
main ()
{
#define TEST(x) x; verify ()
  TEST (f1 (8));
  TEST (f2 (8, 3));
  TEST (f3 (8LL, 4LL));
  TEST (f4 (8));
  TEST (f5 (8, 3));
  TEST (f6 (8LL, 4LL));
  TEST (f7 (8));
  TEST (f8 (8, 3));
  TEST (f9 (8LL, 4LL));
  TEST (f10 (8, 16, 2));
  TEST (f11 (8, 3, 16, 2));
  TEST (f12 (8LL, 4LL, 16, 2));
  TEST (f13 (8, 16, 2));
  TEST (f14 (8, 3, 16, 2));
  TEST (f15 (8LL, 4LL, 16, 2));
  TEST (f16 (8, 16, 2));
  TEST (f17 (8, 3, 16, 2));
  TEST (f18 (8LL, 4LL, 16, 2));
  return 0;
}
