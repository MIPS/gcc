struct a
{
  int t, t1;
};

static struct a *b;

void *xmalloc(int);


void f(void)
{
  b = xmalloc (sizeof(*b));
}

int g(void)
{
 return b->t;
}
