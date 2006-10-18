/* Test passing scalars by value.  This test includes scalar types that
   are supported by va_arg.  */

/* { dg-require-compat-dfp "" } */

extern void scalar_by_value_dfp_2_x (void);
extern void exit (int);
int fails;

int
main ()
{
  scalar_by_value_dfp_2_x ();
  exit (0);
}
