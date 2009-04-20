/* Emit hints about the CIL program.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Roberto Costa
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "bit-stream.h"
#include "emit-hints.h"

struct coding
{
  int num_bits;
  int code;
};

/* Local functions, macros and variables.  */
static void dump_compressed_int (FILE *, unsigned int);
static void dump_basic_block_frequency (unsigned char);
static void dump_branch_probability (unsigned char);

static unsigned char *bb_freq_coded;
static FILE *stream = NULL;

/* Basic block frequencies to be emitted in JITMethodAttribute
   are quantized into values from 0 to 15 (4 bits) and then
   coded with a Huffmann coding with the fixed table CODING.
   The table has been computed by assuming the following distribution
   of frequencies, measured for a set of about 30 benchmarks:

   Basic block frequency      Frequency
     0                          41.8%
     1                          16.4%
     2                           5.2%
     3                           4.7%
     4                           3.5%
     5                           1.7%
     6                           2.8%
     7                           3.2%
     8                           0.5%
     9                           0.7%
    10                           2.4%
    11                           1.2%
    12                           1.0%
    13                           4.4%
    14                           1.3%
    15                           9.2%   */

static const struct coding coding[16] =
{
  {1, 0x01},  /*  0 */
  {3, 0x03},  /*  1 */
  {4, 0x03},  /*  2 */
  {5, 0x09},  /*  3 */
  {5, 0x03},  /*  4 */
  {6, 0x05},  /*  5 */
  {5, 0x00},  /*  6 */
  {5, 0x01},  /*  7 */
  {7, 0x10},  /*  8 */
  {7, 0x11},  /*  9 */
  {5, 0x03},  /* 10 */
  {7, 0x09},  /* 11 */
  {7, 0x08},  /* 12 */
  {5, 0x08},  /* 13 */
  {6, 0x09},  /* 14 */
  {4, 0x05},  /* 15 */
};

/* A list of branch probabilities for the function being compiled that
   will be emitted as a custom attribute. There is an entry for each
   branch (not switches). */

struct branch_prob_list_node
{
  struct branch_prob_list_node *next;
  int branch_prob;
};

/* Local functions, macros and variables.  */

static struct branch_prob_list_node *branch_prob_list_head = NULL;
static struct branch_prob_list_node *branch_prob_list_tail = NULL;


/* Given COND_EXPR statement NODE, register the probability that is taken.
   Also dump a comment with such a probability to FILE.   */

void
branch_probability_add (FILE *file ATTRIBUTE_UNUSED, tree node)
{
  edge e;
  struct branch_prob_list_node *n;

  gcc_assert (TREE_CODE (node) == COND_EXPR);

  {
    edge true_edge;
    edge false_edge;
    basic_block src_bb = get_stmt_ann (node)->bb;
    extract_true_false_edges_from_block (src_bb, &true_edge, &false_edge);
    e = true_edge;
    gcc_assert (e);
  }

  /* Initialize new branch probability node */
  n = XNEW (struct branch_prob_list_node);
  n->next = NULL;
  n->branch_prob = e->probability;

  /* Update branch probability list */
  if (branch_prob_list_head)
    {
      gcc_assert (branch_prob_list_tail && !branch_prob_list_tail->next);
      branch_prob_list_tail->next = n;
      branch_prob_list_tail = n;
    }
  else
    {
      gcc_assert (!branch_prob_list_tail);
      branch_prob_list_head = n;
      branch_prob_list_tail = n;
    }
}

/* Emit the custom attribute encoding branch probabilities and reset
   the list of branch probabilities.   */

void
branch_probability_emit_and_reset (FILE *file)
{
  struct branch_prob_list_node *n, *next;
  int length = 0;

  /* Calculate the length of the list */
  for (n = branch_prob_list_head; n; n = n->next)
    ++length;

  /* Emit */
  fprintf (file,
	   "\n\t.custom instance void class [gcc4net]gcc4net."
	   "JitCompilationHints.BranchProbabilitiesAttribute::.ctor("
	   "unsigned int8[]) =  (01 00 %02x %02x %02x %02x",
	   length & 0xff,
	   (length >> 8) & 0xff,
	   (length >> 16) & 0xff,
	   (length >> 24) & 0xff);

  for (n = branch_prob_list_head; n; n = n->next)
    fprintf (file, " %02x", n->branch_prob * 256 / REG_BR_PROB_BASE);

  fputs (" 00 00 ) ", file);

  /* Free the list */
  for (n = branch_prob_list_head; n; n = next)
    {
      next = n->next;
      free (n);
    }

  /* Reset list */
  branch_prob_list_head = NULL;
  branch_prob_list_tail = NULL;
}

static void
dump_compressed_int (FILE *file, unsigned int val)
{
  gcc_assert (val < 0x20000000);

  if (val <= 0x7f)
    fprintf (file, "%02x ", val);
  else if (val < 0x3fff)
    fprintf (file, "%02x %02x ", (val >> 8) | 0x80, val & 0xff);
  else
    fprintf (file, "%02x %02x %02x %02x ",
	     (val >> 24) | 0xa0,
	     (val & 0xffffff) >> 16,
	     (val & 0xffff) >> 8,
	     val & 0xff);
}

static void
dump_basic_block_frequency (unsigned char c)
{
  *bb_freq_coded = c;
  ++bb_freq_coded;
}

static void
dump_branch_probability (unsigned char c)
{
  fprintf (stream, "%02x ", c);
}

/* Emit the custom attribute encoding basic block frequencies.   */

void
basic_block_frequency_emit (FILE *file)
{
  int emitted_bbs = n_basic_blocks - 2;
  basic_block bb;
  struct bit_stream_emitter bse;
  unsigned char *start_bb_freq_coded, *tmp_ptr;
  struct branch_prob_list_node *n;
  int branch_prob_list_length;

  /* Don't emit info for single basic block functions */
  if (emitted_bbs < 2)
    return;

  bit_stream_emitter_init (&bse, dump_basic_block_frequency);

  /* Count number of emitted basic blocks.
     The count is initialized with the number of GIMPLE blocks
     minus the entry and the exit. Then, the number is:
     -) incremented in case of basic blocks ending with a
	COND_EXPR statement in which the ELSE clause is not
	a fall-through; as a matter of fact, in this case a
	single GIMPLE basic block is emitted as two basic blocks.
     -) decremented in case of an empty basic block; as a matter
	of fact, in this case no basic block is emitted for
	a GIMPLE basic block.   */

  FOR_EACH_BB (bb)
    {
      block_stmt_iterator last_bsi = bsi_last (bb);

      if (bsi_end_p (last_bsi))
	--emitted_bbs;
      else
	{
	  tree last = bsi_stmt (last_bsi);

	  if (TREE_CODE (last) == COND_EXPR)
	    {
	      edge true_edge;
	      edge false_edge;
	      extract_true_false_edges_from_block (bb, &true_edge, &false_edge);

	      if (false_edge->dest != bb->next_bb)
		++emitted_bbs;
	    }
	}
    }

  /* Allocate memory for basic block frequencies bit stream */
  start_bb_freq_coded = (unsigned char *) xmalloc (emitted_bbs);
  bb_freq_coded = start_bb_freq_coded;

  /* Emit number of basic blocks */
  fprintf (file,
	   "\n\t.custom instance void class "
	   "[gcc4net]gcc4net.JitCompilationHints."
	   "BasicBlockFrequenciesAttribute::.ctor(unsigned int8[]) = "
	   "(01 00 %02x %02x %02x %02x ",
	   emitted_bbs & 0xff,
	   (emitted_bbs >> 8) & 0xff,
	   (emitted_bbs >> 16) & 0xff,
	   (emitted_bbs >> 24) & 0xff);

  /* Emit frequency for each basic block.
     Beware that some GIMPLE blocks are emitted as two blocks!   */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator last_bsi = bsi_last (bb);
      int freq_class;

      /* Frequency is not emitted for an empty basic block */
      if (bsi_end_p (last_bsi))
        continue;

      fprintf (file, "%02x ", bb->frequency * 100 / BB_FREQ_MAX);

      freq_class = (bb->frequency * 15 * 256 / BB_FREQ_MAX) / 256;
      gcc_assert (freq_class >= 0 && freq_class < 16);
      bit_stream_emitter_push_bits (&bse,
				    coding[freq_class].num_bits,
				    coding[freq_class].code);

      if (TREE_CODE (bsi_stmt (last_bsi)) == COND_EXPR)
	{
	  edge true_edge;
	  edge false_edge;
	  basic_block dest_bb;

	  extract_true_false_edges_from_block (bb, &true_edge, &false_edge);
	  dest_bb = false_edge->dest;

	  if (dest_bb != bb->next_bb)
	    {
	      edge split_edge = find_edge (bb, dest_bb);
	      int new_bb_frequency;

	      gcc_assert (split_edge);
	      new_bb_frequency =
		(bb->frequency * split_edge->probability) / REG_BR_PROB_BASE;
	      fprintf (file, "%02x ",
		       new_bb_frequency * 100 / BB_FREQ_MAX);

	      freq_class = (new_bb_frequency * 15 * 256 / BB_FREQ_MAX) / 256;
	      gcc_assert (freq_class >= 0 && freq_class < 16);
	      bit_stream_emitter_push_bits (&bse,
					    coding[freq_class].num_bits,
					    coding[freq_class].code);
	    }
	}
    }

  bit_stream_emitter_pad_byte (&bse);
  bit_stream_emitter_fini (&bse);

  fputs ("00 00 ) ", file);

  fputs ("\n\t.custom instance void class "
	 "[gcc4net]gcc4net.JitCompilationHints."
	 "JITMethodAttribute::.ctor(unsigned int8*, unsigned int8 *) = (01 00 ",
         file);
  dump_compressed_int (file, emitted_bbs);

  for (tmp_ptr = start_bb_freq_coded; tmp_ptr < bb_freq_coded; ++tmp_ptr)
    fprintf (file, "%02x ", *tmp_ptr);

  fputs ("00 00/*,*/ 01 00 ", file);

  /* Free memory for basic block frequencies bit stream */
  gcc_assert (bb_freq_coded - start_bb_freq_coded <= emitted_bbs);
  free (start_bb_freq_coded);

  /* Calculate the length of the branch probability list */
  branch_prob_list_length = 0;

  for (n = branch_prob_list_head; n; n = n->next)
    ++branch_prob_list_length;

  dump_compressed_int (file, branch_prob_list_length);

  stream = file;
  bit_stream_emitter_init (&bse, dump_branch_probability);

  for (n = branch_prob_list_head; n; n = n->next)
    {
      int prob_class =
	((n->branch_prob * 256 - REG_BR_PROB_BASE * 256 / 16)
	 * 7 / REG_BR_PROB_BASE) / 256;

      gcc_assert (prob_class >= -1 && prob_class <= 7);

      if (prob_class < 0)
	prob_class = 0;

      bit_stream_emitter_push_bits (&bse, 3, prob_class);
    }

  bit_stream_emitter_pad_byte (&bse);
  fputs ("00 00 ) ", file);
  bit_stream_emitter_fini (&bse);
  stream = NULL;
}
