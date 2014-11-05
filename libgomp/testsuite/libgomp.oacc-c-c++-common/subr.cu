
extern "C" __global__ void
delay (clock_t * d_o, clock_t delay)
{
  clock_t start, ticks;

  start = clock ();

  ticks = 0;

  while (ticks < delay)
    ticks = clock () - start;
}

extern "C" __global__ void
delay2 (unsigned long *d_o, clock_t delay, unsigned long tid)
{
  clock_t start, ticks;

  start = clock ();

  ticks = 0;

  while (ticks < delay)
    ticks = clock () - start;

  d_o[0] = tid;
}

extern "C" __global__ void
sum (clock_t * d_o, int N)
{
  int i;
  clock_t sum;
  __shared__ clock_t ticks[32];

  sum = 0;

  for (i = threadIdx.x; i < N; i += blockDim.x)
    sum += d_o[i];

  ticks[threadIdx.x] = sum;

  syncthreads ();

  for (i = 16; i >= 1; i >>= 1)
    {
      if (threadIdx.x < i)
	ticks[threadIdx.x] += ticks[threadIdx.x + i];

      syncthreads ();
    }

  d_o[0] = ticks[0];
}

extern "C" __global__ void
mult (int n, float *x, float *y)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  for (i = 0; i < n; i++)
    y[i] = x[i] * x[i];
}
