#define N 1000

struct b {
  int b_a;
  int b_b;
};

struct str {
  int a;
  struct b str_b;
  int c;
};

int f (void);
int g (void);

int main (void)
{
  
  return f() + g(); 
}

int f (void)
{
  struct str str_f = {1,{2,3},4};
  struct b b_f;
  int i;

  for (i=0; i < N; i++)
    {
      str_f.a = str_f.a + str_f.str_b.b_a;
      str_f.str_b.b_a = str_f.a;

      b_f.b_a = str_f.a;
    }

  return str_f.a;
 
}

int g (void)
{
  struct str str_g = {1,{2,3},4};
  struct b b_g;
  int i;
  
  for (i=0; i < N; i++)
    {
      str_g.c = str_g.c + str_g.str_b.b_b;
      str_g.str_b.b_b = str_g.c;

      b_g.b_b = str_g.a;
    }
  
  return str_g.c;
}
