/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ssa" } */
     

oof ()
{
  int live_head;
  int * live = &live_head;

  if (live)
   bitmap_clear (live);
}

                                                                               
/* There should be no IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 0 "ssa"} } */
