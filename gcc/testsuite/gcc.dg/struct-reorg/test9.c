#define N1 1000
#define N2 10000

int main (void)
{
  struct str {
    int a;
    int b;
    int c;
    int d;
  };
  
  struct str str_f = {1,2,3,4};
  long val[N2];
  int i;

  val[0] = 11;

  for (i=0; i < N1; i++)
    {
      str_f.a = str_f.a + str_f.b;
      str_f.b = str_f.a;
    }

  for (i=0; i < N2-1; i++)
    val[i+1] = val[i] + 1;

  for (i=0; i < N1; i++)
    {
      str_f.c = str_f.c + str_f.d;
      str_f.d = str_f.c;
    }

  return str_f.d + str_f.c + str_f.a + str_f.b;
}

