/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt1-details" } */
 

void abort(void);
void exit(int);

int x, y;

static void
init_xy(void)
{
  x = 3;
  y = 2;
}

void
test4(void)
{
  init_xy();
  if ((x < y ? x++ : y++) != 2)
    abort ();
}

int
main(){
  test4 ();
  exit (0);
}

/* We should convert two COND_EXPRs into straightline code.  */
/* { dg-final { scan-tree-dump-times "straightline" 2 "phiopt1"} } */
