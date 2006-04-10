#define N 1000

struct str {
  int a;
  int b;
  int c;
  int d;
};

struct str str_f; 
struct str str_g; 

int f (void);
int g (void);

int main (void)
{
  
  return f() + g(); 
}

int f (void)
{
  str_f.a = 1;
  str_f.b = 2;
  str_f.c = 3;
  str_f.d = 4;

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
  str_g.a = 1;
  str_g.b = 2;
  str_g.c = 3;
  str_g.d = 4;

  int i;
  
  for (i=0; i < N; i++)
    {
      str_g.c = str_g.c + str_g.d;
      str_g.d = str_g.c;
    }
  
  return str_g.c;
}
