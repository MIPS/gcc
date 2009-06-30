/*
 * Copyright (c) 2008 Google Inc. All rights reserved.
 *
 * $Header: $
 */

/* Select instructions.
   Zips through all instructions in all basic blocks
   and invokes burm_label on each.
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree-pass.h"
#include "function.h"
#include "basic-block.h"

#include "icg.h"
#include "icg-opcode.h"


static void
print_icg_tree_1 (FILE *fp, icg_node *tree, int indent)
{
  int i;
  if (fp == 0) fp = stdout;
  for (i = 0; i < indent; i++)
    fprintf (fp, " ");
  icg_print_opcode (fp, tree, indent, 1);
  if (tree->left)
    print_icg_tree_1 (fp, tree->left, indent + 4);
  if (tree->right)
    print_icg_tree_1 (fp, tree->right, indent + 4);
}


static
void print_icg_tree_states (FILE *fp, icg_node *tree)
{
  if (fp == 0) fp = stdout;
  print_icg_tree_1 (fp, tree, 0);
}


void icg_select(void)
{
  basic_block bb;
  FOR_EACH_BB(bb) {
    rtx insn;
    FOR_BB_INSNS(bb, insn) {
      int id = INSN_UID(insn);
      if (icg_insn2dirty[id]) {
	icg_node *tree = icg_insn2tree[id];
	if (tree) {
	  const icg_state_type state = icg_burm_label(tree);
	  if (dump_file) {
	    if (state == 0) {
	      fprintf(dump_file,
		"\n" "select: Can not match this icg tree: dump1 (\n");
	      print_icg_tree(dump_file, tree, "no match: ");
	      fprintf(dump_file, "end of dump1 )\n");

	      fprintf(dump_file,
		"\n" "select: Can not match this rtx tree: dump2 (\n");
	      print_inline_rtx(dump_file, tree->rtl, 0);
	      fprintf(dump_file, "\n" "end of dump2 )\n");

	      fprintf(dump_file, "\n\n");

	      flag_icg_trace |=  (1<<0);	/* print cover info */
	      print_icg_tree_states(dump_file, tree);
	      fflush(dump_file);
	      flag_icg_trace &= ~(1<<0);	/* do not print cover info */
	    }
	    print_icg_tree_states(dump_file, tree);
	    fflush(dump_file);
	  }
	  if (state == 0) {
	    icg_nyi("BURG blocks: can not parse expr tree");
	  }
	}
      }
    }
  }
}
