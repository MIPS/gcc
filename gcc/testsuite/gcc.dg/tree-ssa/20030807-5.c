/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ssa" } */
                                                                                
struct rtx_def;
typedef struct rtx_def *rtx;


struct rtx_def
{

  int code;
  unsigned int unchanging:1;

};
static rtx current_sym_addr;

static int
foo ()
{
  if (current_sym_addr->code == 42
      && (({
	       rtx _rtx = current_sym_addr;
	       if (((_rtx)->code) != 42)
	         abort ();
	       _rtx;}
	   )->unchanging))
    ;
}

/* There should be precisely one load of ->code.  If there is
   more than, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times "->code" 1 "ssa"} } */

/* There should be precisely one IF statement.  If there is
   more than, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times "if " 1 "ssa"} } */
