/* Test function return values.  This test includes scalar types that
   are supported by va_arg.  */

/* { dg-require-compat-dfp "" } */

extern void scalar_return_dfp_3_x(void);
extern void exit (int);
int fails;

int
main ()
{
  scalar_return_dfp_3_x ();
  exit (0);
}
