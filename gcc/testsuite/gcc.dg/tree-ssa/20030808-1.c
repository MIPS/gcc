/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom2" } */
      

struct rtx_def;
typedef struct rtx_def *rtx;
enum rtx_code
{
  UNKNOWN,
  CODE_LABEL,
  NOTE,
  LAST_AND_UNUSED_RTX_CODE
};
typedef union rtunion_def rtunion;
struct rtx_def
{
  enum rtx_code code:16;
};
void
delete_dead_jumptables ()
{
  rtx insn, next;
  if (insn->code == CODE_LABEL)
    {
      rtx const _rtx = insn;
      if (_rtx->code != CODE_LABEL && _rtx->code != NOTE)
	abort ();
    }
  ;
}

/* There should be precisely one load of ->code.  If there is
   more than, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times "->code" 1 "dom2"} } */
   
/* There should be one IF statement.  */
/* { dg-final { scan-tree-dump-times "if " 1 "dom2"} } */

