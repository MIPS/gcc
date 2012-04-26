/* Test for power7 defaults for -mvrsave and -mvrsave-setjmp.  */
/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -mcpu=power7" } */
/* { dg-final { scan-assembler-times "mtvrsave" 2 } } */
/* { dg-final { scan-assembler-times "mfvrsave" 1 } } */

extern void func (void);
extern void nonleaf (vector int *, vector int *, vector int *, int);
extern void nonleaf2 (vector int *, vector int *, vector int *, int);
extern void leaf (vector int *, vector int *, vector int *);

/* This should generate mtvrsave instructions even if -mvrsave-setjmp.  */
void
nonleaf (vector int *a, vector int *b, vector int *c, int n)
{
  int i;
  vector int s = *b;
  vector int t = *c;

  for (i = 0; i < n; i++)
    {
      s += t;
      func ();
    }

  *a = s;
  return;
}

/* This should not generate mtvrsave instructions if -mvrsave-setjmp.  */
void
nonleaf2 (vector int *a, vector int *b, vector int *c, int n)
{
  int i;
  unsigned int vrsave = 0;
  *a = *b + *c;

  for (i = 0; i < n; i++)
    func ();

  return;
}

/* This should not generate mtvrsave instructions if -mvrsave-setjmp.  */
void
leaf (vector int *a, vector int *b, vector int *c)
{
  *a = *b + *c;
  return;
}
