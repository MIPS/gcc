/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom3" } */
    
struct rtx_def;
typedef struct rtx_def *rtx;



union rtunion_def
{
  int rtint;
};
typedef union rtunion_def rtunion;



struct rtx_def
{
  rtunion fld[1];

};

static int *uid_cuid;
static int max_uid_cuid;

static rtx
bar (rtx r)
{
  rtx place = r;

  if (place->fld[0].rtint <= max_uid_cuid
      && (place->fld[0].rtint > max_uid_cuid ? insn_cuid (place) :
	  uid_cuid[place->fld[0].rtint]))
    return r;
  
  return 0;
}

/* There should be three IF conditionals.  The two inner conditionals in the
   GIMPLE form are dead.  */
/* { dg-final { scan-tree-dump-times "if " 3 "dom3"} } */
 

