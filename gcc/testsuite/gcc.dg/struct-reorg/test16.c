#include <stdlib.h>

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
  int i, j = 0;
  struct str * str_p = calloc (str_n, sizeof (struct str));

  for (j=0; j< str_n; j++)
    {
      for (i=0; i < N; i++)
	{
	  str_p->a = str_p->a + str_p->b;
	  str_p->b = str_p->a;
	}
      str_p++;
    }
  
  str_p--;
  return str_p->a;
 
}

int g (void)
{
  struct str str_g[str_n];
  int i,j = 0;
  struct str * str_p = calloc (str_n, sizeof (struct str));
  
  for (j=0; j< str_n; j++)
    {
      for (i=0; i < N; i++)
	{
	  str_p->c = str_p->c + str_p->d;
	  str_p->d = str_p->c;
	}
      str_p++;
    }
  str_p--;
  return str_p->c;
}
