/* { dg-do compile { target powerpc64-*-linux* } } */
/* { dg-options "-O3" } */

/* This test case comes from the original PR.  */

struct list
{
  unsigned short type;
  unsigned int len;
  void *data;
  short *ind;
};

extern int foo (int *);
extern int bar (void);
extern int qwe (struct list *);
extern long check (int *);
extern void display (int *);
extern void trace (int *);

int
myfunc (int *opts, char *str)
{
  short num;
  int loc;
  int start;
  int len;
  int code;
  int rc = 0;

  if (code && (rc = check (opts)))
    goto exit;

  foo (&code);
  {
    struct list mylist[1];
    mylist[0].type = 960;
    mylist[0].len = 4;
    mylist[0].data = (void *) &loc;
    mylist[0].ind = 0L;
    qwe (mylist);
  }
  bar ();

  if (code && (rc = check (opts)))
    goto exit;

  foo (&code);
  bar ();

  if (code && (rc = check (opts)))
    goto exit;

  if (opts)
    trace (opts);

  display (opts);

  while (code >= 0)
    {
      foo (&code);
      {
        struct list mylist[1];
        mylist[0].type = 500;
        mylist[0].len = 2;
        mylist[0].data = (void *) &num;
        mylist[0].ind = 0L;
        qwe (mylist);
      }
      bar ();
      {
        struct list mylist[3];
        mylist[0].type = 960;
        mylist[0].len = 4;
        mylist[0].data = (void *) &loc;
        mylist[0].ind = 0L;
        mylist[1].type = 496;
        mylist[1].len = 4;
        mylist[1].data = (void *) &start;
        mylist[1].ind = 0L;
        mylist[2].type = 496;
        mylist[2].len = 4;
        mylist[2].data = (void *) &len;
        mylist[2].ind = 0L;
        qwe (mylist);
      }
      {
        struct list mylist[4];
        mylist[0].type = 460;
        mylist[0].len = 129;
        mylist[0].data = (void *) str;
        mylist[0].ind = 0L;
        mylist[1].type = 460;
        mylist[1].len = 129;
        mylist[1].data = (void *) str;
        mylist[1].ind = 0L;
        mylist[2].type = 460;
        mylist[2].len = 9;
        mylist[2].data = (void *) str;
        mylist[2].ind = 0L;
        mylist[3].type = 500;
        mylist[3].len = 2;
        mylist[3].data = (void *) str;
        mylist[3].ind = 0L;
        qwe (mylist);
      }
    }

 exit:
  {
    struct list mylist[1];
    mylist[0].data = (void *) &loc;
    mylist[0].ind = 0L;
    qwe (mylist);
  }
  return (rc);
}
