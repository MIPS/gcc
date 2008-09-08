/* { dg-do assemble } */
/* FIXME lto.  We should use 'dg-do compile', but -S and -flto-single
   confuse cc1 because the flags specify two different output files.  */

void foo(void) {
 char *bar;
 int baz;
 while (1)
   {
     if (baz)
       {
         baz = -baz;
         do
           *bar++ = 0;
         while (++baz);
       }
     ++baz;
   }
}
