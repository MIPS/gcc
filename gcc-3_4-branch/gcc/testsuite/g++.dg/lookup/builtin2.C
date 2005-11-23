// PR c++/18838
// { dg-do compile }

extern "C" {
  extern double fabs (double __x) throw () __attribute__ ((__const__));

  __inline double
  fabs (double __x) throw () { return __builtin_fabs (__x); }
}

double fail_me(double __x) { return fabs(__x); }

namespace std
{
  using ::fabs;
}

typedef double (*realfn) (double);

using std::fabs;

int main ()
{
  realfn myfn = fabs;
}
