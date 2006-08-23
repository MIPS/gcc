/* Generate code from machine description to recognize rtl as insns.
   Copyright (C) 1987, 1988, 1992, 1993, 1994, 1995, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */


/* A listhead of decision trees.  The alternatives to a node are kept
   in a doubly-linked list so we can easily add nodes to the proper
   place when merging.  */

struct decision_head
{
  struct decision *first;
  struct decision *last;
};

/* A single test.  The two accept types aren't tests per-se, but
   their equality (or lack thereof) does affect tree merging so
   it is convenient to keep them here.  */

struct decision_test
{
  /* A linked list through the tests attached to a node.  */
  struct decision_test *next;

  /* These types are roughly in the order in which we'd like to test them.  */
  enum decision_type
    {
      DT_num_insns,
      DT_mode, DT_code, DT_veclen,
      DT_elt_zero_int, DT_elt_one_int, DT_elt_zero_wide, DT_elt_zero_wide_safe,
      DT_const_int,
      DT_veclen_ge, DT_dup, DT_pred, DT_c_test,
      DT_accept_op, DT_accept_insn
    } type;

  union
  {
    int num_insns;		/* Number if insn in a define_peephole2.  */
    enum machine_mode mode;	/* Machine mode of node.  */
    RTX_CODE code;		/* Code to test.  */

    struct
    {
      const char *name;		/* Predicate to call.  */
      const struct pred_data *data;
                                /* Optimization hints for this predicate.  */
      enum machine_mode mode;	/* Machine mode for node.  */
    } pred;

    const char *c_test;		/* Additional test to perform.  */
    int veclen;			/* Length of vector.  */
    int dup;			/* Number of operand to compare against.  */
    HOST_WIDE_INT intval;	/* Value for XINT for XWINT.  */
    int opno;			/* Operand number matched.  */

    struct {
      int code_number;		/* Insn number matched.  */
      int lineno;		/* Line number of the insn.  */
      int num_clobbers_to_add;	/* Number of CLOBBERs to be added.  */
    } insn;
  } u;
};

/* Data structure for decision tree for recognizing legitimate insns.  */

struct decision
{
  struct decision_head success;	/* Nodes to test on success.  */
  struct decision *next;	/* Node to test on failure.  */
  struct decision *prev;	/* Node whose failure tests us.  */
  struct decision *afterward;	/* Node to test on success,
				   but failure of successor nodes.  */

  const char *position;		/* String denoting position in pattern.  */

  struct decision_test *tests;	/* The tests for this node.  */

  int number;			/* Node number, used for labels */
  int subroutine_number;	/* Number of subroutine this node starts */
  int need_label;		/* Label needs to be output.  */
};

struct print_dtree_hooks
{
  void (*print_header) (const char *, const char *);
  void (*print_locals) (void);
  void (*print_maybe_return) (const char *, const char *);
  void (*print_default_return) (void);
  void (*print_accept_insn) (const char *, struct decision_test *,
			     struct decision *);
  int (*is_unconditional) (struct decision_test *);
  const char *name_prefix;
  const char *call_suffix;
};

extern int pattern_lineno;
extern int error_count;

void process_define_predicate (rtx desc);
struct decision *new_decision (const char *, struct decision_head *);
struct decision_test *new_decision_test (enum decision_type,
					 struct decision_test ***);
struct decision *add_to_sequence (rtx, struct decision_head *, const char *);
void finish_sequence (struct decision *, const char *, const char *, int, int);
void merge_trees (struct decision_head *, struct decision_head *);
void process_tree (struct decision_head *, const struct print_dtree_hooks *);
