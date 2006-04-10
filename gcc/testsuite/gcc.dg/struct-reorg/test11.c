#define N 1000

struct str {
  int a;
  int b;
  int c;
  int d;
};

struct str str = {1,2,3,4};

int f (void);
int g (void);

int main (void)
{
  
  return f() + g(); 
}

int f (void)
{
  int i;

  for (i=0; i < N; i++)
    {
      str.a = str.a + str.b;
      str.b = str.a;
    }

  return str.a;
 
}

int g (void)
{
  int i;
  
  for (i=0; i < N; i++)
    {
      str.c = str.c + str.d;
      str.d = str.c;
    }
  
  return str.c;
}
