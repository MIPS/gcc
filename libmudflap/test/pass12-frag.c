int i = 10;
int *x = (int *) malloc (i * sizeof (int));

while (--i)
{
  ++x;
  *x = 0;
}
