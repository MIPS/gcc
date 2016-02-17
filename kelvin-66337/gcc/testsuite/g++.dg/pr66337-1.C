/* { dg-do run { target { powerpc*-*-* } } } */
/* { dg-options "-std=c++11 -malign-power -O2" } */

/* Power ABI for 32-bit and 64-bit compilers place the same alignment
   restrictions on long longs and doubles. */

typedef double _Tp;

struct _Tp2 { 
  char b;
  int i;
  char c;
  long long l;
  _Tp _M_t; 
};

extern _Tp2 tp2e;

struct _ST1 {
  char a;
};

struct _ST2 {
  int b;
};

struct _ST3 {
  float f;
};

struct _ST4 {
  double d;
};

struct _ST5 {
  char a;
  double d;
};

struct _ST6 {
  double d;
  char a;
};

int main ()
{
  int a = alignof (_Tp2);
  int b = __alignof__(_Tp2::_M_t);
  int c = alignof(_Tp);
  int d = __alignof__(tp2e._M_t);
  int e = alignof(_Tp2::_M_t);

  int f = __alignof__(_Tp2::l);
  int g = alignof (long long);
  int h = __alignof__(tp2e.l);
  int i = alignof(_Tp2::l);

  if ((a != 8) || (b != 8) || (c != 8) || (d != 8) || (e != 8) 
      || (f != 8) || (g != 8) || (h != 8) || (i != 8))
    return -1;

  a = sizeof (_ST1);
  b = sizeof (_ST2);
  c = sizeof (_ST3);
  d = sizeof (_ST4);
  e = sizeof (_ST5);
  f = sizeof (_ST6);
  g = sizeof (_Tp2);

  if ((a != 1) || (b != 4) || (c != 4) || (d != 8) 
      || (e != 16) || (f != 16) || (g != 32))
    return -2;

  /* success */
  return 0;
}
  
