#define N 1000

struct str {
  int a;
  int b;
  int c;
  int d;
};

int f (void);
int g (void);

int main (void)
{
  long f_res = f ();
  long g_res;
  int i, b[N], c[N];
  
  for (i = 0; i < N; i++)
    {
      b[i] = 12;
      c[i] = 15;
    }

  for (i = 0; i < N; i++)
      b[i] = b[i] + c[i];
  g_res = g ();
  return f_res + g_res; 
}

int f (void)
{
  struct str str_f = {1,2,3,4};
  int i;

  for (i=0; i < N; i++)
    {
      str_f.a = str_f.a + str_f.b;
      str_f.b = str_f.a;
    }

  return str_f.a;
 
}

int g (void)
{
  struct str str_g = {1,2,3,4};
  int i;
  
  for (i=0; i < N; i++)
    {
      str_g.c = str_g.c + str_g.d;
      str_g.d = str_g.c;
    }
  
  return str_g.c;
}
