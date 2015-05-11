void *malloc (__SIZE_TYPE__);
void free (void *);

#pragma acc routine (fact)

int
fact (int n)
{
  if (n == 0 || n == 1)
    return 1;

  return n * fact (n - 1);
}

int
main(int argc, char **argv)
{
  int *a, i, n = 10;

  a = (int *)malloc (sizeof (int) * n);

#pragma acc parallel copy (a[0:n]) vector_length (5)
  {
#pragma acc loop
    for (i = 0; i < n; i++)
      a[i] = fact (i);
  }

  for (i = 0; i < n; i++)
    if (fact (i) != a[i])
      return -1;

  free (a);

  return 0;
}
