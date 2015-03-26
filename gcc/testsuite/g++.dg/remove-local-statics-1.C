/* Verify that we do not eliminate a static variable in
   main::Local::Foo.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-dse2-details  -fdump-tree-dse1-details" } */

int
main (void)
{
   static int thestatic = 0;
   struct Local {
     __attribute__((__noinline__))
     static void Foo () { thestatic = 1; }
   };

   thestatic = 2;
   Local::Foo();

   return thestatic++;
}
/* { dg-final { scan-tree-dump-times "static variables to consider" 0 "dse1" } } */
/* { dg-final { scan-tree-dump-times "static variables to consider" 0 "dse2" } } */
/* { dg-final { cleanup-tree-dump "dse1" } } */
/* { dg-final { cleanup-tree-dump "dse2" } } */
