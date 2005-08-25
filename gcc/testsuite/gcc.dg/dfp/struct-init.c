/* { dg-do run } */
/* { dg-options "" } */

#include <stdio.h>
extern void abort (void);

union u
{
  _Decimal128 d128;
  double d;
};

union n
{
  double d;
  _Decimal64 d64;
};


struct bar
{
  int i;
  _Decimal64 d64;
  long j;
  union u un;
  _Decimal32 d32;
};

int main()
{
  static union u u1 = { 4.2dl, 1.2 }; /* { dg-warning "excess elements in union |near initialization" "initialization of union" } */
  static union u u2 = { 4.2dl };
  static union u u3 = { 1.2f };
  static union u u4 = { 0.0dl,0.0f };   /* { dg-warning "excess elements in union |near initialization" "initialization of union" } */
 
  static struct bar b = { 1, 3.2dd, 1.2, { 4.2dl,1.2 } , 2.2df }; /* { dg-warning "excess elements in union |near initialization" "initialization of union" } */

  static union n n1 = {2.2df};
  static union n n2 = {3.2f};

#if 0
  /* Do you mean 2.2df and 3.2dd?  */
  /* Besides, this test incorrectly assumes exact dfp->fp->dfp conversion. */
  if (n1.d64 != 22.df)
    abort();
  if (n2.d64 != 32.dd)
    abort();
#endif

  if (b.d64 != 3.2dd)
    abort();

  if (b.un.d128 != 4.2dl)
    abort();
  
  if (b.d32 != 2.2df)
    abort();
  
  if (u2.d128 != 4.2dl)
    abort();
  
  if (u4.d128 != 0.0dl)
    abort();

  if (u4.d != 0)
    abort();

#if 0
  /* 1.2f != 1.2dl.  */
  if (u3.d128 != 1.2dl)
    abort();
#endif

  return 0;
}
