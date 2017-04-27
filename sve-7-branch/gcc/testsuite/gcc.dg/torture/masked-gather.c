void
f (int *__restrict out, int *__restrict lookup,
   int *__restrict index, int n)
{
  for (int i = 0; i < n; ++i)
    {
      int x = index[i];
      if (x >= 0)
	x = lookup[x];
      out[i] = x;
    }
}
