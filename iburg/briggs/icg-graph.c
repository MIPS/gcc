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
#include "sparseset.h"
#include "tree-pass.h"
#include "hard-reg-set.h"
#include "flags.h"

#include "icg.h"

#define forall(i, s) EXECUTE_IF_SET_IN_SPARSESET(s, i)

#define add_copy_edges(dst, src, live) add_copy_interferences(add_edges, dst, src, live)


/*
 * An exercise for the future is to eliminate
 * adjacency vectors for the machine registers,
 * saving plenty of space.
 *
 * This does all the work of building the interference graph
 * and coalescing.  Initially, we fill in the bit matrix and
 * accumulate the total degree for each live range.  Then we
 * iterate.  At the beginning of each iteration, we allocate
 * adjacency vectors, then fill them in.  Then we attempt to
 * coalesce copies.  When no more coalesces are possible,
 * the iteration terminates and the bit matrix is freed.
 * The adjacenct vectors are preserved for use during coloring.
 *
 * When coalesces succeed, we must update the bit matrix and
 * the degrees of the various live ranges, being careful so
 * that when we next allocate adjacency vectors, they will be
 * of the correct length.
 *
 * We keep track of coalesces via a union-find tree.
 *
 */

static
char *bit_matrix = 0;

icg_reg_info *icg_reg_vector = 0;

static
void allocate_bit_matrix(unsigned regs)
{
  unsigned i;
  unsigned size = (regs*regs + 15) >> 4;
  bit_matrix = (char *) xcalloc(size, sizeof(char));
  for (i = 0; i < regs; i++)
    icg_reg_vector[i].degree = 0;
}


static
void clear_graph(unsigned regs)
{
  unsigned size = (regs*regs + 15) >> 4;
  memset(bit_matrix, 0, size);
}



static
void allocate_adjacency_vectors(unsigned regs)
{
  unsigned i;
  for (i = 0; i < regs; i++)
    {
      if (icg_reg_vector[i].root == i)
	{
	  unsigned degree = icg_reg_vector[i].degree;
	  size_t size = sizeof(icg_names) + (degree - 1)*sizeof(unsigned);
	  icg_names *av = (icg_names *)obstack_alloc(&icg_graph_obstack, size);
	  av->next = NULL;
	  av->size = 0;
	  icg_reg_vector[i].av = av;
	}
      else
	icg_reg_vector[i].av = NULL;
    }
}


unsigned icg_path_compress(unsigned r)
{
  unsigned root;
  root = icg_reg_vector[r].root;
  if (root != r)
    {
      root = icg_path_compress(root);
      icg_reg_vector[r].root = root;
    }
  return root;
}


#define find(r) (icg_reg_vector[r].root)


static
void compress_all(unsigned regs) {
  unsigned r;
  for (r = 0; r < regs; r++)
    {
      unsigned root = icg_reg_vector[r].root;
      if (root < r)
	icg_reg_vector[r].root = find(root);
    }
}


static
void add_addr(sparseset live, icg_node *addr)
{
  if (addr->a.base_valid) {
    sparseset_set_bit(live, find(addr->a.base));
  }
  if (addr->a.scale > 0) {
    sparseset_set_bit(live, find(addr->a.index));
  }
}



/* this needs to be fast, hence the twiddly hand optimizations */

static
void add_edges_to_bit_matrix(unsigned r, sparseset live)
{
  unsigned x;
  icg_reg_info *rv = icg_reg_vector; /* these are all loop invariant, but      */
  unsigned rr = r*r;                 /* will (probably) be missed by compiler. */
  unsigned rd = rv[r].degree;
  int rk = rv[r].kind;
  char *bm = bit_matrix;
  unsigned bogus_lastx = ~0x0 - 1;
  unsigned lastx = bogus_lastx;
  bool in_run = false;
  if (dump_file) fprintf(dump_file, "%3d {", r);
  forall(x, live)
    {
      if (x != r && (rk & rv[x].kind))
	{
	  unsigned index = (x < r) ? (x + rr/2) : (r + x*x/2);
	  unsigned bit_index = index & 7;
	  unsigned byte_index = index >> 3;
	  unsigned mask = 1 << bit_index;
	  unsigned byte = bm[byte_index];
	  if (!(byte & mask))
	    {
	      bm[byte_index] = byte | mask;
	      rv[x].degree++;
	      rd++;
	    }
	  if (dump_file)
	    {
	      if (!in_run) {
	        fprintf(dump_file, "%3d", x);
		in_run = true;
	      } else {
		if (lastx + 1 != x) {
		  fprintf(dump_file, "-%3d ", lastx);
		  fprintf(dump_file, "%3d", x);
		  in_run = true;
		}
	      }
	      lastx = x;
	    }
	}
    }
  rv[r].degree = rd;
  if (dump_file) {
    if (in_run) {
      fprintf(dump_file, "-%3d ", lastx);
    }
    fprintf(dump_file, " }\n");
  }
}


/* same here */

static
void add_edges_to_graph(unsigned r, sparseset live)
{
  unsigned x;
  icg_reg_info *rv = icg_reg_vector;
  unsigned rr = r*r;
  unsigned rd = rv[r].av->size;
  unsigned *rav = rv[r].av->name;
  int rk = rv[r].kind;
  char *bm = bit_matrix;
  forall(x, live)
    {
      if (x != r && (rk & rv[x].kind))
	{
	  unsigned index = x < r ? x + rr/2 : r + x*x/2;
	  unsigned bit_index = index & 7;
	  unsigned byte_index = index >> 3;
	  unsigned mask = 1 << bit_index;
	  unsigned byte = bm[byte_index];
	  if (!(byte & mask))
	    {
	      bm[byte_index] = byte | mask;
	      rv[x].av->name[rv[x].av->size++] = r;
	      rav[rd++] = x;
	    }
	}
    }
  rv[r].av->size = rd;
}


/* and here */

static
void add_more_edges(unsigned r, icg_names *avs)
{
  unsigned i;
  icg_reg_info *rv = icg_reg_vector;
  unsigned rr = r*r;
  unsigned rd = rv[r].degree;
  int rk = rv[r].kind;
  char *bm = bit_matrix;
  unsigned size = avs->size;
  for (i = 0; i < size; i++)
    {
      unsigned x = icg_path_compress(avs->name[i]);
      if (r != x && (rk & rv[x].kind))
	{
	  unsigned index = x < r ? x + rr/2 : r + x*x/2;
	  unsigned bit_index = index & 7;
	  unsigned byte_index = index >> 3;
	  unsigned mask = 1 << bit_index;
	  unsigned byte = bm[byte_index];
	  if (!(byte & mask))
	    {
	      bit_matrix[byte_index] = byte | mask;
	      rd++;
	    }
	}
    }
  rv[r].degree = rd;
}



static
unsigned interfere(unsigned i, unsigned j)
{
  /* assert i < j */
  unsigned index = i + j*j/2;
  unsigned bit_index = index & 7;
  unsigned byte_index = index >> 3;
  unsigned mask = 1 << bit_index;
  return bit_matrix[byte_index] & mask;
}


static
unsigned attempt_coalesce(unsigned pass, const unsigned original_x, const unsigned original_y)
{
  icg_names *avs;
  unsigned father, son;
  const unsigned x = icg_path_compress(original_x);
  const unsigned y = icg_path_compress(original_y);
  if (x == y)
    return 0;
  if (x < y)
    {
      father = x;
      son = y;
    }
  else
    {
      father = y;
      son = x;
    }
  if (pass == 0 && father < FIRST_PSEUDO_REGISTER)
    return 0;
  if (interfere(father, son))
    return 0;
  icg_reg_vector[son].root = father;
  avs = icg_reg_vector[son].av;
  while (avs) {
    icg_names *next = avs->next;
    add_more_edges(father, avs);
    avs->next = icg_reg_vector[father].av;
    icg_reg_vector[father].av = avs;
    avs = next;
  }
  if (dump_file) fprintf(dump_file, "coalesced %3d and %3d\n", father, son);
  return 1;
}


static
unsigned reduce_coalesce(icg_node *p, NT goalNT, unsigned pass)
{
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  unsigned i;
  unsigned coalesces = 0;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    coalesces += reduce_coalesce(kid[i], nts[i], pass);
  switch (rule)
    {
#include "icg-coalesce.cases"
    }
  return coalesces;
}


static
unsigned coalesce(unsigned pass)
{
  unsigned coalesces = 0;
  basic_block bb;
  unsigned r;

  /*** checking to be sure degree and size match at this point
  for (r = 0; r < icg_interior_regs; r++)
    if (icg_reg_vector[r].root == r)
      if (icg_reg_vector[r].degree < icg_reg_vector[r].av->size)
	fprintf(stderr, "r = %d, degree %d, size %d\n", r,
		icg_reg_vector[r].degree,
		icg_reg_vector[r].av->size);
  ***/

  if (dump_file) fprintf(dump_file, "\n" "starting coalesce(%d)\n", pass);
  FOR_EACH_BB(bb)
    {
      rtx insn;
      FOR_BB_INSNS(bb, insn)
        {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree)
	    coalesces += reduce_coalesce(tree, 1, pass);
        }
    }
  if (dump_file) {
    fprintf(dump_file, "\n");
    fprintf(dump_file, "found %3d coalesces\n", coalesces);
  }
  if (coalesces == 0)
    for (r = 0; r < icg_interior_regs; r++)
      if (icg_reg_vector[r].root == r)
	icg_reg_vector[r].degree = icg_reg_vector[r].av->size;
  return coalesces;
}




static
void handle_call_kills(void (*add_edges)(unsigned, sparseset), sparseset live)
{
  unsigned r;
  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    if (TEST_HARD_REG_BIT(regs_invalidated_by_call, r))
      {
	sparseset_clear_bit(live, r);
	add_edges(r, live);
      }
}


static
void add_copy_interferences(void (*add_edges)(unsigned, sparseset),
  const unsigned original_dst, const unsigned original_src, sparseset live)
{
  unsigned dst = find(original_dst);
  unsigned src = find(original_src);
  if (src != dst)
    {
      sparseset_clear_bit(live, dst);
      sparseset_clear_bit(live, src);
      add_edges(dst, live);
    }
  sparseset_set_bit(live, src);
}


static
sparseset reduce_build(icg_node *p, NT goalNT,
  void (*add_edges)(unsigned, sparseset), sparseset live)
{
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];

  icg_burm_kids(p, rule, kid);
  switch (rule) {
#include "icg-build.cases"
  }
  /*
   * The visit permutation is only valid for non-chain rules with >= 2 nonterminals
   */
  if (nts[0] && nts[1] && p->perm_kids >= 2) {
    int i;
    for (i = p->perm_kids - 1; i >= 0; i--) {
      const int k = p->perm[i];
      live = reduce_build(kid[k], nts[k], add_edges, live);
    }
  }
  else {
    int i;
    for (i = 0; nts[i]; i++) {
      live = reduce_build(kid[i], nts[i], add_edges, live);
    }
  }
  return live;
}


static
void init_from_live_out(basic_block bb, sparseset live)
{
  unsigned *live_out = icg_lr_liveout[bb->index]->name;
  unsigned n = icg_lr_liveout[bb->index]->size;
  unsigned i;

  sparseset_clear(live);
  for (i = 0; i < n; i++)
    sparseset_set_bit(live, find(live_out[i]));
  if (!frame_pointer_needed)
    sparseset_clear_bit(live, 6);

  if (dump_file)
    {
      fprintf(dump_file, "\n" "live-out = { ");
      for (i = 0; i < icg_live_ranges; i++)
	if (sparseset_bit_p(live, i))
	  fprintf(dump_file, "%d ", i);
      fprintf(dump_file, "}\n");
    }
}


static
void build(const char *what, void (*add_edges)(unsigned, sparseset))
{
  unsigned r;
  basic_block bb;
  sparseset live = sparseset_alloc(icg_interior_regs);
  compress_all(icg_interior_regs);

  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    icg_reg_vector[r].kind = ~0;

  /* set interferences based on kind */
  for (r = 0; r < 8; r++)		/* ax, dx, cx, bx, si, di, bp, sp */
    icg_reg_vector[r].kind &= ~INT_REGISTER;
  for (r = 21; r < 29; r++)		/* xmm0-xmm7 */
    icg_reg_vector[r].kind &= ~FLOAT_REGISTER;
  for (r = 37; r < 45; r++)		/* r8-r15 */
    icg_reg_vector[r].kind &= ~INT_REGISTER;
  for (r = 45; r < 53; r++)		/* xmm8-xmm15 */
    icg_reg_vector[r].kind &= ~FLOAT_REGISTER;

  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    sparseset_set_bit(live, r);
  for (r = FIRST_PSEUDO_REGISTER; r < icg_interior_regs; r++)
    if (icg_reg_vector[r].root == r)
      add_edges(r, live);

  /* make machine regs interfere with each other */
  for (r = 0; r < 8; r++)		/* ax, dx, cx, si, di, bp, sp */
    icg_reg_vector[r].kind = INT_REGISTER;
  for (r = 21; r < 29; r++)		/* xmm0-xmm7 */
    icg_reg_vector[r].kind = FLOAT_REGISTER;
  for (r = 37; r < 45; r++)		/* r8-r15 */
    icg_reg_vector[r].kind = INT_REGISTER;
  for (r = 45; r < 53; r++)		/* xmm8-xmm15 */
    icg_reg_vector[r].kind = FLOAT_REGISTER;

  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    add_edges(r, live);

  {
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), "build %s", what);
    icg_debug(buf);
  }
  FOR_EACH_BB(bb)
    {
      rtx insn;
      init_from_live_out(bb, live);
      FOR_BB_INSNS_REVERSE(bb, insn)
        {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree)
	    live = reduce_build(tree, 1, add_edges, live);
        }
    }
  sparseset_free(live);
}

static
int sort_icg_unsigneds(const void *vp0, const void *vp1)
{
  const unsigned *p0  = (const unsigned *)(vp0);
  const unsigned *p1  = (const unsigned *)(vp1);
  return *p0 - *p1;
}

/*
 * print vector vp to file fp, ending with a new line
 */
void icg_print_runs(FILE *fp, const unsigned *vp, size_t size)
{
  unsigned i;
  size_t runlg  = 0;
  unsigned bogus_lastx = ~0x0 - 1;
  unsigned lastx = bogus_lastx;

  /*
   * make a copy of the incoming vector so we can sort it
   */
  unsigned *p = (unsigned *)icg_alloc(sizeof(vp[0])*size);
  for (i = 0; i < size; i++) {
    p[i] = vp[i];
  }
  qsort(p, size, sizeof(p[0]), sort_icg_unsigneds);

  fprintf(fp, "(%3d)", size);
  for (i = 0; i < size; i++) {
      const unsigned x = p[i];
      if (runlg == 0) {
	fprintf(fp, "%3d", x);
	runlg = 1;
      } else {
	if (lastx + 1 != x) {
	  if (runlg > 1){
	    fprintf(fp, "-%3d ", lastx);
	  }
	  fprintf(fp, "%3d", x);
	  runlg = 1;
	} else {
	  runlg += 1;
	}
      }
      lastx = x;
  }
  if (runlg > 1) {
    fprintf(fp, "-%3d ", lastx);
  }
  fprintf(fp, "\n");
}

static
void print_icg_names(FILE* fp, const icg_names *av)
{
    if (fp == 0) {
        return;
    }
    for (; av; av = av->next) {
      icg_print_runs(fp, av->name, av->size);
    }
}


static
void dump_ig(void)
{
  unsigned r;
  fprintf(dump_file, "live ranges\n");
  for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++)
    {
      fprintf(dump_file, "%3d", r);
      if (icg_reg_vector[r].root == r)
	{
	  icg_names *av = icg_reg_vector[r].av;
	  fprintf(dump_file, ", degree = %3d, interferes with:\n",
	    icg_reg_vector[r].degree);
	  print_icg_names(dump_file, av);

	}
      else
        {
	  fprintf(dump_file, " coalesced with %3d\n", icg_reg_vector[r].root);
	}
    }

  fprintf(dump_file, "interior registers\n");
  for (r = icg_live_ranges; r < icg_interior_regs; r++)
    {
      fprintf(dump_file, "%3d", r);
      if (icg_reg_vector[r].root == r)
	{
	  icg_names *av = icg_reg_vector[r].av;
	  fprintf(dump_file, ", degree = %3d, interferes with:\n",
	    icg_reg_vector[r].degree);
	  print_icg_names(dump_file, av);
	}
      else
        {
	  fprintf(dump_file, " coalesced with %3d\n", icg_reg_vector[r].root);
	}
    }
}


void icg_graph(unsigned pass)
{
  int icg_graph_pass_number = 0;
  void *local_mark = obstack_alloc(&icg_graph_obstack, 0);
  allocate_bit_matrix(icg_interior_regs);
  if (dump_file) fprintf(dump_file, "\n" "building bit matrix\n");
  build("add_edges_to_bit_matrix", add_edges_to_bit_matrix);
  for (icg_graph_pass_number = 0; 1; icg_graph_pass_number += 1) {
    obstack_free(&icg_graph_obstack, local_mark);
    allocate_adjacency_vectors(icg_interior_regs);
    clear_graph(icg_interior_regs);
    if (dump_file) fprintf(dump_file, "\n" "building adj vectors\n");
    build("add_edges_to_graph", add_edges_to_graph);
    if (dump_file) dump_ig();
    if (!coalesce(pass)) {
      break;
    }
  }
  free(bit_matrix);
}
