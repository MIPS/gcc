/* Test valid usages of the if_present clause.  */

/* { dg-additional-options "-fdump-tree-omplower" } */

template<typename T>
void
t ()
{
  T a, b, c[10];

#pragma acc update self(a) if_present
#pragma acc update device(b) async if_present
#pragma acc update host(c[1:3]) wait(4) if_present
#pragma acc update self(c) device(b) host (a) async(10) if (a == 5) if_present
}

class
C
{
private:
  bool a;
  float b;

public:
  bool operator== (int x)
  {
    return a == x;
  }
};

void
f ()
{
  t<C> ();
}

/* { dg-final { scan-tree-dump-times "pragma omp target oacc_update if_present" 4 "omplower" } } */
