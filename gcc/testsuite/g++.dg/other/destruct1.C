// { dg-do run }

int i, j, k;
extern "C" void abort ();

struct S
{
  S () { ++i; }
  S (const S &x) { ++k; }
  S &operator= (const S &x) { abort (); return *this; }
  ~S () { ++j; }
};

const S foo ()
{
  S s;
  return s;
}

S bar (S x)
{
  return S ();
}

S baz (S x)
{
  return x;
}

void test ()
{
  S a = bar (foo ());
  S b = baz (foo ());
}

int main ()
{
  test ();
  if (i != 3 || j != 4 || k != 1)
    abort ();
}
