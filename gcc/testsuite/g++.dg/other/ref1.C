// { dg-do compile }
// { dg-options "-O2" }

struct A
{
  A ();
  ~A ();
};

extern const A &bar ();
extern A baz ();
extern int operator!= (const A &x, const A &y);

void foo (const A &x)
{
  /* This has undefined behaviour, as a is used before it is initialized.
     Still, we shouldn't ICE on it.  */
  const A &a = a != bar () ? x : baz ();
}
