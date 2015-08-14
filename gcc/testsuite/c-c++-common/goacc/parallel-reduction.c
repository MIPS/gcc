int
main ()
{
  int sum = 0;

#pragma acc parallel num_gangs (10) copy (sum) reduction (+:sum)
  {
    int v = 5;
    sum += 10 + v;
  }

  return sum;
}
