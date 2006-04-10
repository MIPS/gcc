#define N 1000

struct str1 {
  int a;
  int b;
  int c;
  int d;
};

struct str2 {
  int a;
  int b;
  int c;
  int d;
};

int f1 (void);
int g1 (void);
int f2 (void);
int g2 (void);

int main (void)
{
  
  return f1() + g1() + f2() + g2(); 
}

int f1 (void)
{
  struct str1 str_f = {1,2,3,4};
  int i;

  for (i=0; i < N; i++)
    {
      str_f.a = str_f.a + str_f.b;
      str_f.b = str_f.a;
    }

  return str_f.a;
 
}

int g1 (void)
{
  struct str1 str_g = {1,2,3,4};
  int i;
  
  for (i=0; i < N; i++)
    {
      str_g.c = str_g.c + str_g.d;
      str_g.d = str_g.c;
    }
  
  return str_g.c;
}

int f2 (void)
{
  struct str2 str_f = {1,2,3,4};
  int i;

  for (i=0; i < N; i++)
    {
      str_f.a = str_f.a + str_f.b;
      str_f.b = str_f.a;
    }

  return str_f.a;
 
}

int g2 (void)
{
  struct str2 str_g = {1,2,3,4};
  int i;
  
  for (i=0; i < N; i++)
    {
      str_g.c = str_g.c + str_g.d;
      str_g.d = str_g.c;
    }
  
  return str_g.c;
}
