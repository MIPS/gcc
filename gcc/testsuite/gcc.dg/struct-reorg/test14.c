#define N 1000
#define str_n 5

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
  
  return f() + g(); 
}

int f (void)
{
  struct str str_f[str_n];
  int i,j;

  for (j=0; j < str_n; j++)
    {
      for (i=0; i < N; i++)
	{
	  str_f[j].a = str_f[j].a + str_f[j].b;
	  str_f[j].b = str_f[j].a;
	}
    }
  return str_f[0].a;
 
}

int g (void)
{
  struct str str_g[str_n];
  int i,j;
  
  for (j=0; j < str_n; j++)
    {
      for (i=0; i < N; i++)
	{
	  str_g[j].c = str_g[j].c + str_g[j].d;
	  str_g[j].d = str_g[j].c;
	}
    }  
  return str_g[0].c;
}
