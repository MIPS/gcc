/*
 * Copyright (c) 2008 Google Inc. All rights reserved.
 *
 * $Header: $
 */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "sparseset.h"

#include "icg.h"

#define forall(i, s) EXECUTE_IF_SET_IN_SPARSESET(s, i)
#define k FIRST_PSEUDO_REGISTER


/* see Preston's thesis, Section 8.8 */


static
unsigned simplify_graph(void)
{
  unsigned r;
  unsigned sp = 0;
  sparseset low  = sparseset_alloc(icg_interior_regs);
  sparseset high = sparseset_alloc(icg_interior_regs);
  sparseset stacked = sparseset_alloc(icg_interior_regs);

  if (dump_file) fprintf(dump_file, "\n" "starting simplify {\n");

  for (r = k; r < icg_interior_regs; r++)
    if (icg_reg_vector[r].root == r)
      {
	if (icg_reg_vector[r].degree < k)
	  sparseset_set_bit(low, r);
	else if (icg_reg_vector[r].cost < 1.0/0.0)
	  sparseset_set_bit(high, r);
      }
  if (dump_file) fprintf(dump_file, "\t" "finished init\n");
  while (1)
    {
      unsigned m, best;
      float best_ratio;
      while (sparseset_cardinality(low) > 0)
	{
	  unsigned i;
	  icg_names *av;


	  /*
	   * in the future, choose i at random from [0 .. members)
	   * to reduce effect of arbitrary selection.
	   * probably want to control with a flag.
	   */

	  i = low->members - 1;
	  m = low->dense[i];
	  low->members = i;
	  if (dump_file) fprintf(dump_file, "\t" "popping %3d\n", m);

	  av = icg_reg_vector[m].av;
	  for (i = 0; i < av->size; i++)
	    {
	      unsigned n = av->name[i];
	      if (n >= k)
		{
		  icg_reg_vector[n].degree--;
		  if (n >= k && icg_reg_vector[n].degree < k && !sparseset_bit_p(stacked, n))
		    {
		      sparseset_clear_bit(high, n);
		      sparseset_set_bit(low, n);
		    }
		}
	    }
	  icg_reg_vector[m].stack = sp;
	  sparseset_set_bit(stacked, m);
	  sp = m;
	}
      if (sparseset_cardinality(high) == 0) break;
      best_ratio = 1.0/0.0;
      forall(m, high)
	{
	  float ratio = icg_reg_vector[m].cost / icg_reg_vector[m].degree;
	  if (ratio < best_ratio)
	    {
	      best = m;
	      best_ratio = ratio;
	    }
	}
      if (dump_file) fprintf(dump_file, "\t" "spill possibility %d\n", best);
      sparseset_clear_bit(high, best);
      sparseset_set_bit(low, best);
    }
  sparseset_free(low);
  sparseset_free(high);
  sparseset_free(stacked);

  if (dump_file) fprintf(dump_file, "\n" "finished simplify }\n");

  return sp;
}

static
void dump_sparse_set(FILE *fp, const sparseset set, const char *name)
{
  unsigned r;
  const size_t size = sparseset_size(set);
  unsigned *p = (unsigned *)icg_alloc(size*sizeof(int));

  size_t n = 0;		/* number of items in the set */
  forall(r, set) {
    p[n] = r;
    n += 1;
  }
  if (fp == 0) {
    fp = stdout;
  }
  fprintf(fp, "%s:\n", name);
  icg_print_runs(fp, p, n);	/* ends with a new line */
}

static
void select_colors(unsigned sp)
{
  unsigned m;
  sparseset caller_saves = sparseset_alloc(k);
  sparseset callee_saves = sparseset_alloc(k);

  if (dump_file) fprintf(dump_file, "\n" "starting select\n");

  for (m = 0; m < k; m++)
    sparseset_set_bit(caller_saves, m);

  sparseset_set_bit(callee_saves, 3);     /* %rbx */
  sparseset_set_bit(callee_saves, 6);     /* %rbp */
  sparseset_set_bit(callee_saves, 41);    /* %r12 */
  sparseset_set_bit(callee_saves, 42);    /* %r13 */
  sparseset_set_bit(callee_saves, 43);    /* %r14 */
  sparseset_set_bit(callee_saves, 44);    /* %r15 */

  sparseset_and_compl(caller_saves, caller_saves, callee_saves);

  if (dump_file) {
    dump_sparse_set(dump_file, caller_saves, "caller saves");
    dump_sparse_set(dump_file, callee_saves, "callee saves");
  }

  while (sp)
    {
      m = sp;
      if (icg_reg_vector[m].cost > 0.0)
	{
	  unsigned i;
	  icg_names *av;
	  bool used[k];
	  unsigned c;
	  for (c = 0; c < k; c++)
	    used[c] = false;
	  av = icg_reg_vector[m].av;
	  for (i = 0; i < av->size; i++)
	    {
	      unsigned n = av->name[i];
	      int c = icg_reg_vector[n].color;
	      if (c >= 0)
		used[c] = true;
	    }
	  forall(c, caller_saves)
	    if (!used[c]) {
	      icg_reg_vector[m].color = c;
	      break;
	    }
	  if (icg_reg_vector[m].color < 0)
	    forall(c, callee_saves)
	      if (!used[c]) {
		icg_reg_vector[m].color = c;
		break;
	      }
	}
      sp = icg_reg_vector[sp].stack;
    }

  sparseset_free(caller_saves);
  sparseset_free(callee_saves);

  if (dump_file) fprintf(dump_file, "\n" "finished select\n");

  if (dump_file)
    for (m = k; m < icg_interior_regs; m++)
      if (icg_reg_vector[m].root == m)
	fprintf(dump_file, "live range %3d got %3d\n", m, icg_reg_vector[m].color);

  for (m = k; m < icg_live_ranges; m++)
    if (icg_reg_vector[m].root == m && icg_reg_vector[m].color < 0)
      return;

  /* check to be sure all interior registers got colors */
  for (m = icg_live_ranges; m < icg_interior_regs;  m++)
    if (icg_reg_vector[m].root == m && icg_reg_vector[m].color < 0)
      {
	fprintf(stderr, "\n" "an interior register didn't get colored\n");
	exit(0);
      }
}



void icg_color(void)
{
  unsigned r;
  for (r = 0; r < k; r++)
    icg_reg_vector[r].color = r;
  for (r = k; r < icg_interior_regs; r++)
    icg_reg_vector[r].color = -1;

  select_colors(simplify_graph());
}
