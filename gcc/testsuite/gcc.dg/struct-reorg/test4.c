#define N 1000

struct str {
  int a;
  int b;
  int c;
  int d;
};

void init_str (int *);
void g (struct str *);

int main (void)
{
  struct str a;
  
  init_str (&(a.a));
  init_str (&(a.b));
  init_str (&(a.c));
  init_str (&(a.d));

  g (&a);

  return (a.a + a.b + a.c + a.d); 
}

void init_str (int * field_p)
{
  static val = 11;
  
  *field_p = val++;
 
}

void g (struct str * str_p)
{
  int i;
  
  for (i=0; i < N; i++)
    {
      str_p->a = str_p->a + str_p->b;
      str_p->b = str_p->c + str_p->c;
      str_p->c = str_p->c + str_p->d;
      str_p->d = str_p->d + str_p->a;
    }  
}
