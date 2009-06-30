/* Allocate registers within a basic block, for GNU compiler.
   Copyright (C) 2008  Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include <math.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "tm_p.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "df.h"
#include "icg.h"
#include "icg-opcode.h"
#include "icg-ruleinfo.h"
#include "langhooks.h"

struct obstack icg_obstack;
struct obstack icg_graph_obstack;

icg_phi_node **icg_bb2phi;
unsigned icg_ssa_regs;
unsigned icg_live_ranges;
unsigned int *icg_ssa_to_lr;
icg_names **icg_ssa_liveout;
icg_names **icg_lr_liveout;


void icg_print_opcode (FILE *fp, icg_node *node, int indent, bool indent_p)
{
  if (fp == 0) fp = stdout;
  if (node == NULL) {
    fprintf (fp, "NULL\n");
  } else {
    if (node->op == SYMBOL_REF_DI) {
      fprintf (fp, "%s:%s", get_icg_opcode_name(node->op), node->a.string);
    } else if (icg_reg_p (node)) {
	const int reg = (icg_dead_reg_p (node) ? node->rx : node->r);
	if (reg < FIRST_PSEUDO_REGISTER) {
	  fprintf (fp, "%s:%d[%s]", get_icg_opcode_name(node->op),
		   reg, reg_names[reg]);
	} else {
	  fprintf (fp, "%s:%d", get_icg_opcode_name(node->op), reg);
	}
    } else if (icg_const_p (node)) {
      fprintf (fp, "%s:%lld", get_icg_opcode_name(node->op), node->val);
    } else {
      fprintf (fp, "%s", get_icg_opcode_name(node->op));
    }
    if (node->state_label) {
      fprintf (fp, " <iburg handle %p>", node->state_label);
      /*
       * the state_label is really an opaque pointer
       * to an opaque iburg specific data structure icg_burm_state,
       * which is declared and defined in the icg-burg.c file from burg
       * TODO
       */
    } else {
      fprintf (fp, " *** unmatched");
    }
    fprintf(fp, " {node %p}", (void *)node);
    if (flag_icg_trace & (1<<0)) {
      icg_diagnose_match_candidates(fp, node->state_label, node,
	indent, indent_p);
    }
    fprintf (fp, "\n");
  }
}

#define inf (1.0/0.0)
void icg_diagnose_match_candidates(FILE *dump_file,
  const icg_state_type state, icg_node *p,
  int indent, bool indent_p)
{
  int NT;
  char spaces[BUFSIZ];
  int i;
  const char *sep = "\n";
  if (indent && indent_p) {
    for (i = 0; i < indent + 4 && i < BUFSIZ-1; i++) {
      spaces[i] = ' ';
    }
    spaces[i] = 0;
  }
  (void)state;
  /*
   * TODO: 77 is the number of NTs at one point in the development of the
   * x86 grammar.  This needs to come from some burg definition file.
   */
  for (NT = 1; NT <= 77; NT++) {
    /*
     * icg_burm_rule can't necessarily be called with any NT,
     * as we valgrind shows that we may use uninitialized variables
     * internal to burg.
     */
    const RuleNumber ruleno = icg_burm_rule(p->state_label, NT);
    const float        cost = icg_burm_get_cost(p->state_label, NT);
    if (ruleno > 0 && cost != inf) {
      if (indent && indent_p) {
	fprintf(dump_file, "%s%s%6g:%5d: %s",
	  sep, spaces,
	  cost, ruleno, icg_burm_string[ruleno]);
      } else {
	fprintf(dump_file, " %6g:%5d:%s",
	  cost, ruleno, icg_burm_ntname[NT]);
      }
    }
  }
}

void *
icg_alloc (size_t size)
{
  return obstack_alloc (&icg_obstack, size);
}

void *
icg_calloc (size_t size)
{
  void *p = obstack_alloc (&icg_obstack, size);
  memset (p, 0, size);
  return p;
}

bool icg_reg_p (icg_node *pn)
{
  return (pn->op >= REG_DI && pn->op <= REGX_SF);
}

bool icg_dead_reg_p (icg_node *pn)
{
  return (pn->op >= REGX_DI && pn->op <= REGX_SF);
}

bool icg_const_p (icg_node *pn)
{
  return (pn->op >= CONST_0 && pn->op <= CONST64N);
}

icg_node *
first_bb_icg_tree (basic_block bb)
{
  rtx insn;
  FOR_BB_INSNS (bb, insn) {
    if (INSN_P (insn)
	&& icg_insn2tree[INSN_UID (insn)]) {
      return icg_insn2tree[INSN_UID (insn)];
    }
  }
  return 0;
}


icg_node *
next_bb_icg_tree (basic_block bb, icg_node* pn)
{
  rtx insn;
  for (insn = NEXT_INSN (pn->insn);
       insn && insn != NEXT_INSN (BB_END (bb));
       insn = NEXT_INSN (insn)) {
    if (INSN_P (insn)
	&& icg_insn2tree[INSN_UID (insn)]) {
      return icg_insn2tree[INSN_UID (insn)];
    }
  }
  return 0;
}

/* Helper functions for for_reach_icg_tree_(def|use). */

static bool
icg_subreg_p (icg_node *pn)
{
  return (pn->op >= SUBREG_DI && pn->op <= SUBREG_QI);
}

static int
mode_width (enum machine_mode mode)
{
  switch (mode)
    {
    case (QImode):
      return 1;
    case (HImode):
      return 2;
    case (SImode):
      return 4;
    case (DImode):
      return 8;
    default:
      gcc_assert(0);
      return 0;
    }
}

static int
for_each_icg_tree_1 (icg_node *pn, bool use,
		     int (*f)(icg_node *, void *), void *data)
{
  int ret = 0;
  if (pn == NULL)
    ret = 0;
  else if (icg_reg_p (pn) && use)
    ret = (*f)(pn, data);
  else
    {
      /* Handle sets of REGs and SUBREGs. */
      if (pn->op == SET_ALL
	  && (icg_reg_p (pn->left)
	      || icg_subreg_p (pn->left)))
	{
	  if (icg_reg_p (pn->left) && !use)
	    ret = (*f)(pn->left, data);
	  else if (icg_subreg_p (pn->left))
	    {
	      gcc_assert (pn->left->left
			  && icg_reg_p (pn->left->left));
	      /* If width of subreg is less than width of reg, then
		 this SET is an update (ie, both a use and def),
		 otherwise it is just a def. */
	      if (mode_width (pn->left->mode)
		  < mode_width (pn->left->left->mode))
		ret = (*f)(pn->left->left, data);
	      else if (!use)
		ret = (*f)(pn->left->left, data);
	    }
	}
      else if (pn->op == DEF_ALL)
	{
	  if (!use)
	    {
	      gcc_assert (pn->left && icg_reg_p (pn->left));
	      ret = (*f)(pn->left, data);
	    }
	}
      else
	ret = for_each_icg_tree_1 (pn->left, use, f, data);
      if (!ret)
	ret = for_each_icg_tree_1 (pn->right, use, f, data);
    }
  return ret;
}

/* Calls F on each icg_node in the icg tree PN which is a register
   def. DATA is a used as a parameter to F.  If F returns a nonzero
   value at any time, the tree search stops and the value is returned
   by the function. */

int
for_each_icg_tree_def (icg_node *pn, int (*f)(icg_node *, void *), void *data)
{
  return for_each_icg_tree_1 (pn, false, f, data);
}

/* Like for_each_icg_tree_def, but F is called only on register
   uses. */

int
for_each_icg_tree_use (icg_node *pn, int (*f)(icg_node *, void *), void *data)
{
  return for_each_icg_tree_1 (pn, true, f, data);
}


/* Prints icg_node NODE to FP in a compact representation, all on one
   line. */

void
print_inline_icg_tree (FILE *fp, icg_node *node)
{
  if (fp == 0) fp = stdout;
  fprintf (fp, "(");
  icg_print_opcode (fp, node, 0, 0);
  if (node->left)
    {
      fprintf (fp, " ");
      print_inline_icg_tree (fp, node->left);
    }
  if (node->right)
    {
      fprintf (fp, " ");
      print_inline_icg_tree (fp, node->right);
    }
  fprintf (fp, ")");
}


/* Helper function for print_icg_node. */

static void
print_icg_tree_1 (FILE *fp, icg_node *tree, int indent, bool indent_p)
{
  int i;
  if (fp == 0) fp = stdout;
  if (indent_p) {
    for (i = 0; i < indent; i++) {
      fprintf (fp, " ");
    }
  }
  icg_print_opcode (fp, tree, indent, indent_p);
  if (tree->left)
    print_icg_tree_1 (fp, tree->left, indent + 4, true);
  if (tree->right)
    print_icg_tree_1 (fp, tree->right, indent + 4, true);
}

/* Prints icg_node TREE to FP in a multi-line pretty-printed indented
   format.  Output is prefixed on the same line by string specified by
   FMT and ARGS in fprintf format, eg:
     print_icg_tree (fp, n, "insn %d: ", id) */

void
print_icg_tree (FILE *fp, icg_node *tree, const char *fmt, ...)
{
  int len;
  va_list arglist;
  char str[BUFSIZ];
  va_start (arglist, fmt);
  if (fp == 0) fp = stdout;
  if (fmt == 0) fmt = "";
  vsnprintf (str, BUFSIZ, fmt, arglist);
  len = strlen (str);
  fprintf (fp, "%s", str);
  print_icg_tree_1 (fp, tree, len, false);
}

/* Dump all of the icg trees of the current function to FP. */

void
dump_icg_function (FILE *fp)
{
  basic_block bb;
  if (fp == 0) fp = stdout;
  FOR_EACH_BB (bb) {
    dump_icg_bb (fp, bb);
  }
}

/* Dump all of the icg trees of basic block BB to FP. */

void
dump_icg_bb (FILE *fp, basic_block bb)
{
  icg_node *pn;
  if (fp == 0) fp = stdout;
  FOR_BB_ICG_TREES (bb, pn) {
    print_icg_tree (fp, pn, "insn %d: ", INSN_UID (pn->insn));
  }
}


/* Dump the icg_tree TREE to stderr, for use in a debugger. */

void
debug_icg (icg_node *tree)
{
  char str[1] = ""; /* avoid gcc warning about zero length string. */
  print_icg_tree (stderr, tree, str);
}

#include <stdarg.h>
extern void icg_nyi_handler(const char *filename, int lineno, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  if (dump_file) {
    fflush(dump_file);
  }
  fprintf(stderr, "%s:%d: icg not yet implemented: ", filename, lineno);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  abort();
  /*NOTREACHED*/
  exit(1);
  /*NOTREACHED*/
}


/* Pass implementing new code generator (combined instruction
   selection and graph-coloring register allocator) */

static unsigned
rest_of_handle_icg (void)
{
  void *firstobj;
  void *graph_mark;
  unsigned pass = 0;

  df_note_add_problem ();
  df_scan_alloc (NULL);
  df_scan_blocks ();
  df_analyze ();

  gcc_obstack_init (&icg_graph_obstack);
  graph_mark = obstack_alloc(&icg_graph_obstack, 0);
  gcc_obstack_init (&icg_obstack);
  firstobj = icg_alloc (0);

  /*
   * Results accumulate in the vector icg_insn2tree and icg_insn2dirty
   */
  icg_create_side_tree ();
  if (dump_file)
    {
      fprintf (dump_file, "*** Initial ICG tree:\n");
      dump_icg_function (dump_file);
    }

  icg_build_ssa ();
  icg_build_live_ranges ();

  /* copied from reload1.c's computation of frame_pointer_needed */
  frame_pointer_needed = !flag_omit_frame_pointer
    || (cfun->calls_alloca && EXIT_IGNORE_STACK)
    || crtl->accesses_prior_frames
    || FRAME_POINTER_REQUIRED;

  icg_find_updates();
  icg_forward_prop();

  for (pass = 0; 1; pass++) {
    bool all_done;
    icg_reassociate();
    icg_select();
    icg_find_names();
    /*    icg_supairs(); */		/* As of 18Aug2008 not tested */
    icg_graph(pass);
    icg_remat();
    icg_costs(pass);
    icg_color();
    all_done = icg_spill(pass);
    if (pass > 0 && all_done) {
      /* If spill code is required, we must do another pass.
       * But we always do at least two passes, whether or not there is spill code.
       *
       * Why?  On pass 0, I don't allow machine registers to be coalesced with
       * symbolic registers, since such coalesces will often create very long
       * live ranges that cannot be spilled.  Therefore, we make an initial
       * pass reducing register pressure to a reasonable level (inserting some spill code),
       * then make at least one more pass, inserting final touchups.
       *
       * In addition, we must run icg_spill() at least once so that references
       * from ARGP are correctly adjusted.
       */
      break;
    }
    obstack_free (&icg_graph_obstack, graph_mark);
    icg_find_updates();
  }

  icg_final();

  if (dump_file) {
    icg_debug("final");
  }

  icg_emit();

  /*
   * icg_final and icg_debug refer to icg_reg_vector,
   * so we can't free up icg_graph_obstack until we are all done
   */
  obstack_free (&icg_graph_obstack, graph_mark);
  obstack_free (&icg_obstack, firstobj);

  reload_completed = true;
  df_finish_pass (true);
  if (optimize > 1)
    df_live_add_problem ();
  df_scan_alloc (NULL);
  df_scan_blocks ();

  if (optimize)
    df_analyze ();

  return 0;
}



static bool
gate_icg (void)
{
  return flag_icg;
}


struct rtl_opt_pass pass_icg =
{
  {
    RTL_PASS,
    "icg",                                /* name */
    gate_icg,                             /* gate */
    rest_of_handle_icg,                   /* execute */
    NULL,                                 /* sub */
    NULL,                                 /* next */
    0,                                    /* static_pass_number */
    0,                                    /* tv_id */
    0,                                    /* properties_required */
    0,                                    /* properties_provided */
    0,                                    /* properties_destroyed */
    0,                                    /* todo_flags_start */
    TODO_dump_func,                       /* todo_flags_finish */
  }
};
