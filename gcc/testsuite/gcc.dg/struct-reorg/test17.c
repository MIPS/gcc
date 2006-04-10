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

int f (void);
int g (void);

int main (void)
{
  
  return f() + g();
}

int f (void)
{
  struct str1 str_f = {1,2,3,4};
  struct str2 str_g = {1,2,3,4};
  int i;

  for (i=0; i < N; i++)
    {
      str_f.a = str_g.a + str_g.b + str_f.b;
      str_g.a = str_f.a;
      str_g.b = str_f.a;
      str_f.b = str_f.a;
    }

  return str_f.a;
 
}

int g (void)
{
  struct str1 str_f = {1,2,3,4};
  struct str2 str_g = {1,2,3,4};
  int i;

  for (i=0; i < N; i++)
    {
      str_f.c = str_g.c + str_g.d + str_f.d;
      str_g.c = str_f.c;
      str_g.d = str_f.c;
      str_f.d = str_f.c;
    }

  return str_f.c;
 
}

