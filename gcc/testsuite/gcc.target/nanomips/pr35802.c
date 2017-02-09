/* { dg-options "-mgp32" } */
__thread int x __attribute__((tls_model("initial-exec")));
__thread int y __attribute__((tls_model("initial-exec")));

int bar (void);

void
foo (int n)
{
  if (n > 5)
    {
      y = 0;
      do
	x += bar ();
      while (n--);
    }
}
