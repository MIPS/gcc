#define N 1000


int main (void)
{
  int i;
  struct str {
    int a;
    int b;
    int c;
    int d;
  } str_f;
  
  str_f.a = 1;
  str_f.b = 2;
  str_f.c = 3;
  str_f.d = 4;
  
  for (i=0; i < N; i++)
    {
      str_f.a = str_f.a + str_f.b;
      str_f.b = str_f.a;
      str_f.c = str_f.c + str_f.d;
      str_f.d = str_f.c;
    }

  return str_f.a + str_f.b +  str_f.c +  str_f.d;
}

