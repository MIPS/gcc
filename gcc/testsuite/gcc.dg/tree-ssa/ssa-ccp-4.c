/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ccp" } */

extern void link_error (void);

/* some array tests -- more to come */


void test14 (int *intarr)
{
  intarr[0] = 0;
  if (intarr[0] != 0)
    link_error ();
}


void test15 (int *intarr)
{
  intarr[0] = 0;
  intarr[1] = 1;
  if (intarr[0] != 0)
    link_error ();
  if (intarr[1] != 1)
    link_error ();
}


void test16 (int *intarr, short *shortarr)
{
  intarr[0] = 1;
  shortarr[0] = 1;
  intarr[0] += 1;
  shortarr[0] += 1;

  if (intarr[0] != 2)
    link_error ();

  if (shortarr[0] != 2)
    link_error ();
}


/* There should be not link_error calls, if there is any the
   optimization has failed */
/* { dg-final { scan-tree-dump-times "link_error" 0 "ccp"} } */

