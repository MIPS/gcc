int i = 10;
char *x = (char *) malloc (i * sizeof (char));

while (i--)
{
  ++x;
  *x = 0;
}
