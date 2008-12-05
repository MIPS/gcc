// { dg-do link }
// { dg-options "{-fwhopr -O3}" }
extern void foo (void);

int
main ()
{
  foo ();
  return 0;
}
