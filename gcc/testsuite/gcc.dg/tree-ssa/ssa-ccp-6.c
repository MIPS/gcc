/* { dg-do compile } */
/* { dg-options "-O1 -fno-tree-dominator-opts -fdump-tree-ccp" } */

extern void link_error (void);

/* tests to check if some identities can be proven by the compiler  */


void test6 (unsigned int a)
{
  if (a != a)
    link_error ();
}

void test66 (unsigned int a)
{
  unsigned int b = a;
  if (b != a)
    link_error ();
}

void test666 (unsigned int a)
{
  unsigned int b = a - 32;
  if (b != (a - 32))
    link_error ();
}

void test6666 (unsigned int a)
{
  if (a-32 != a-32)
    link_error ();
}



/* 
   This should be moved to a place that scans the copyprop dump
*/

void test66666 (int a)
{
  int b = a;
  if (a > 25)
    {
      int c = b;
      if (c < 30)
        {
          int d = c;
          if (d != a)
            link_error ();

        }
    }
}


/* There should be not link_error calls, if there is any the
   optimization has failed */
/* { dg-final { scan-tree-dump-times "link_error" 0 "ccp"} } */

