/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom2" } */
struct rtx_def;
typedef struct rtx_def *rtx;
enum rtx_code
{
  CALL_INSN,
  EXPR_LIST,
  NOTE
};

struct rtx_def
{

  enum rtx_code code:16;
};

static int
nonlocal_mentioned_p (x)
     rtx x;
{
  if (x->code == CALL_INSN)
    {
      rtx const _rtx = ((x));
      if (_rtx->code != CALL_INSN
	  && _rtx->code != NOTE
	  && _rtx->code != EXPR_LIST)
	abort ();
    }

  blah (&x);
}

/* There should be precisely one cast to a short unsigned int.  If there is
   more than one, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times "\\(short unsigned int\\)" 1 "dom2"} } */
                                                                                
/* There should be one IF conditional.  */
/* { dg-final { scan-tree-dump-times "if " 1 "dom2"} } */
                                                                                

