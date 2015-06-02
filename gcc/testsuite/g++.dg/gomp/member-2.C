// { dg-do compile }
// { dg-options "-fopenmp" }

int d;

struct A
{
  A () : a(2), b(3), c(d) {}
  int a;
  A (const A &);
  A &operator= (const A &);
  const A &operator= (const A &) const;
  mutable int b;
  int &c;
};

struct B : public A
{
  B () : h(5) {}
  ~B ();
  B (const B &);
  A e;
  mutable A f;
  const A g;
  const int h;
  int m1 ();
  int m2 ();
  int m3 () const;
  int m4 () const;
};

int
B::m1 ()
{
  #pragma omp parallel private (a, b, c, e, f, g)
    ;
  #pragma omp parallel firstprivate (a, b, c, e, f, g)
    ;
  #pragma omp parallel for lastprivate (a, b, c, e, f, g)
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp simd linear (a, b, c : 1)
    for (int i = 0; i < 10; i++)
      {
	a++;
	b++;
	c++;
      }
  return 0;
}

int
B::m2 ()
{
  #pragma omp parallel private (h)	// { dg-error "is predetermined .shared. for .private." }
    ;
  #pragma omp parallel firstprivate (h)
    ;
  #pragma omp parallel for lastprivate (h)	// { dg-error "is predetermined .shared. for .lastprivate." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp simd linear (h : 1)	// { dg-error "is predetermined .shared. for .linear." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp parallel shared (a)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (b)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (c)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (e)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (f)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (g)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (h)	// { dg-error "is not a variable in clause" }
    ;
  return 0;
}

int
B::m3 () const
{
  #pragma omp parallel private (b, c, e, f, g)
    ;
  #pragma omp parallel firstprivate (b, c, e, f, g)
    ;
  #pragma omp parallel for lastprivate (b, c, e, f, g)
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp simd linear (b, c : 1)
    for (int i = 0; i < 10; i++)
      {
	b++;
	c++;
      }
  return 0;
}

int
B::m4 () const
{
  #pragma omp parallel private (a)	// { dg-error "is predetermined .shared. for .private." }
    ;
  #pragma omp parallel firstprivate (a)
    ;
  #pragma omp parallel for lastprivate (a)	// { dg-error "is predetermined .shared. for .lastprivate." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp simd linear (a : 1)	// { dg-error "is predetermined .shared. for .linear." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp parallel private (h)	// { dg-error "is predetermined .shared. for .private." }
    ;
  #pragma omp parallel firstprivate (h)
    ;
  #pragma omp parallel for lastprivate (h)	// { dg-error "is predetermined .shared. for .lastprivate." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp simd linear (h : 1)	// { dg-error "is predetermined .shared. for .linear." }
    for (int i = 0; i < 10; i++)
      ;
  #pragma omp parallel shared (a)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (b)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (c)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (e)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (f)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (g)	// { dg-error "is not a variable in clause" }
    ;
  #pragma omp parallel shared (h)	// { dg-error "is not a variable in clause" }
    ;
  return 0;
}
