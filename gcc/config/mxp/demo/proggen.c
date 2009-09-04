#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define bool int

#define SDM_USABLE 62000

/* Delta > 0: output becomes new value.
   Delta == 0: output replaces an input
   Delta < 0: output replaces one input, the other input dies.
   Return value: new number of live variables.  */
static int
binary (const char *indent, int n_live, int delta)
{
  const char *btab[] = { "+", "-", "&", "|", "^", "*" };
  const char *op = btab[(int) (drand48 () * (sizeof btab / sizeof btab[0]))];
  bool not_same = delta < 0 || *op == '-';
  int in0 = drand48 () * n_live;
  int in1 = drand48 () * (not_same ? n_live - 1 : n_live);
  int out = delta <= 0 ? in0 : n_live++;

  if (not_same && in1 == in0)
    in1 = n_live - 1;
  if (drand48 () < 0.5)
    printf ("%st%d = t%d %s t%d;\n", indent, out, in0, op, in1);
  else
    printf ("%st%d = t%d %s t%d;\n", indent, out, in1, op, in0);
  if (delta < 0)
    printf ("%st%d = t%d;\n", indent, in1, --n_live);
  return n_live;
}

static int
unary (const char *indent, int n_live, int delta)
{
  const char *utab[] = { "~", "-", "4711+" };
  int in0 = drand48 () * n_live;
  int out = delta <= 0 ? in0 : n_live++;
  const char *op = utab[(int) (drand48 () * (sizeof utab / sizeof utab[0]))];

  printf ("%st%d = %s t%d;\n", indent, out, op, in0);
  return n_live;
}

int
main (void)
{
  int n_in = (srand48 (time (NULL)), 6 * drand48 () + 1);
  int n_out = 6 * drand48 () + 1;
  int type_sz = 2 +  (lrand48 () & 2);
  const char *t_name = type_sz == 2 ? "short" : "int";
  int max_sz = SDM_USABLE / type_sz / (n_in + n_out);
  int sz = 1 + drand48 () * max_sz;
  int v_max = 1 + drand48 () * 20;
  int steps = (3 << (int) (drand48 () * 7)) * drand48 () + 1;
  int i;

  if (v_max < n_out + 1)
    v_max = n_out + 1;
  if (v_max < n_in)
    v_max = n_in;
  for (i = 0; i < n_in; i++)
    printf ("%s ai%d[%d];\n", t_name, i, sz);
  for (i = 0; i < n_out; i++)
    printf ("%s ao%d[%d];\n", t_name, i, sz);
  printf (
    "void\n"
    "f (void)\n"
    "{\n"
    "  int i;\n"
    "\n"
    "  for (i = 0; i < %d; i++)\n"
    "    {\n", sz);
  for (i = 0; i < v_max; i++)
    printf ("      %s t%d;\n", t_name, i);
  printf ("\n");
  for (i = 0; i < n_in; i++)
    printf ("      t%d = ai%d[i];\n", i, i);
  if (drand48 () > 0.5 && n_in < v_max)
    printf ("      t%d = i;\n", n_in++);
  while (--steps >= 0 || n_in != n_out)
    {
      const char *indent = "      ";

      if (n_in > n_out
	  && (n_in - n_out >= steps || drand48 () > 0.7))
	n_in = binary (indent, n_in, -1);
      else
	n_in = ((drand48 () > 0.6 ? unary : binary)
		  (indent, n_in,
		   (n_in < v_max) ? (steps < 0 || drand48 () > 0.5) : 0));
    }
  for (i = 0; i < n_out; i++)
    printf ("      ao%d[i] = t%d;\n", i, i);
  printf (
    "    }\n"
    "}\n");
  return 0;
}
