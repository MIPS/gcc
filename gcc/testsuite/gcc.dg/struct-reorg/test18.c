#define N 1000
#include <stdlib.h>

typedef struct {
  double *a;
  double b;
  double c;
  double d;
} str1;

str1 * str1_p;
int num, cp; 

void init_str1 (void);
int f (void);
int g (void);

int main (void)
{
  num = 100;
  cp = 0;
  double f_res, g_res;

  init_str1 ();
  f_res = f();
  cp = 1;
  g_res = g();
  return f_res + g_res; 
}

void init_str1 (void)
{
  int i;
  str1_p = (str1 *)malloc (num * sizeof (str1));
  
  for (i=0; i < num; i++)
    {
      str1_p[i].b = 0.1;
      str1_p[i].c = 0.2;
      str1_p[i].d = 0.3;

      str1_p[i].a = (double *)malloc (2 * sizeof (double));
      str1_p[i].a[0] = 0.4;      
      str1_p[i].a[1] = 0.5;
    }

}

int f (void)
{
  int i;
  
  for (i=0; i < num; i++)
    {

      str1_p[i].a[cp] = str1_p[i].a[cp] + str1_p[i].b;
      str1_p[i].b = str1_p[i].a[cp];
    }

  return str1_p[num-1].a[cp]; 
}

int g (void)
{
  int i;
  
  for (i=0; i < num; i++)
    {
      str1_p[i].c = str1_p[i].c + str1_p[i].d;
      str1_p[i].d = str1_p[i].c + str1_p[i].a[cp];
    }
  
  return str1_p[num-1].c;
}
