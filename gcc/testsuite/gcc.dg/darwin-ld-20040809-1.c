/* APPLE LOCAL added in FSF mainline on Aug 12, 2004  */
/* Test -dead_strip support.  */
/* Contributed by Devang Patel  <dpatel@apple.com>  */

/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-dead_strip" } */


int
main ()
{
  return 0;
}

