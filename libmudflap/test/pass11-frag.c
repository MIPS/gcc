int i = 10;
char *x = (void *)malloc(i * sizeof (char));

while (--i)
{
  ++x;
  *x = 0;
}
