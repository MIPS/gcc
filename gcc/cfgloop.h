/* Natural loop functions
   Copyright (C) 1987, 1997, 1998, 1999, 2000, 2001, 2002, 2003
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* Structure to hold decision about unrolling/peeling.  */
enum lpt_dec
{
  LPT_NONE,
  LPT_PEEL_COMPLETELY,
  LPT_PEEL_SIMPLE,
  LPT_UNROLL_CONSTANT,
  LPT_UNROLL_RUNTIME,
  LPT_UNROLL_STUPID
};

struct lpt_decision
{
  enum lpt_dec decision;
  unsigned times;
};

/* Description of loop for simple loop unrolling.  */
struct loop_desc
{
  int postincr;		/* 1 if increment/decrement is done after loop exit condition.  */
  rtx stride;		/* Value added to VAR in each iteration.  */
  rtx var;		/* Loop control variable.  */
  rtx var_alts;		/* List of definitions of its initial value.  */
  rtx lim;		/* Expression var is compared with.  */
  rtx lim_alts;		/* List of definitions of its initial value.  */
  bool const_iter;      /* True if it iterates constant number of times.  */
  unsigned HOST_WIDE_INT niter;
			/* Number of iterations if it is constant.  */
  unsigned HOST_WIDE_INT niter_max;
			/* Upper bound on number of iterations.  */
  bool may_be_zero;     /* If we cannot determine that the first iteration will pass.  */
  enum rtx_code cond;	/* Exit condition.  */
  int neg;		/* Set to 1 if loop ends when condition is satisfied.  */

  			/* All of the above is depredicated and will be removed
			   soon.  */
  rtx assumptions;	/* Condition under that the values below are correct.
			   For now we just ignore the loop if it is not always
			   true.  Later we might consider an unswitching-like
			   solution.  */
  rtx noloop_assumptions; /* Condition under that the loop does not roll at all.  */
  rtx infinite;		/* Condition under that the loop is infinite.  */
  rtx niter_expr;	/* The expression to count the number of iterations.  */
  enum machine_mode mode; /* The more in that the expression should be evaluated.  */
  int signed_p;		/* And signedness of its operands (niter_expr itself
			   must always be taken as unsigned, otherwise it would
			   overflow on things like for (i = -max; i < max; i++).  */
  edge out_edge;	/* The exit edge.  */
  edge in_edge;		/* And the other one.  */
  int n_branches;	/* Number of branches inside the loop.  */
};

/* Structure to hold information for each natural loop.  */
struct loop
{
  /* Index into loops array.  */
  int num;

  /* Basic block of loop header.  */
  basic_block header;

  /* Basic block of loop latch.  */
  basic_block latch;

  /* Histogram for a loop.  */
  struct loop_histogram *histogram;

  /* For loop unrolling/peeling decision.  */
  struct lpt_decision lpt_decision;

  /* Simple loop description.  */
  int simple;
  struct loop_desc desc;

  /* Various information about loop.  */
  struct loop_info *info;

  /* Number of loop insns.  */
  unsigned ninsns;

  /* Average number of executed insns per iteration.  */
  unsigned av_ninsns;

  /* Landing pad, if the loop has one.  */
  basic_block landing_pad;

  /* The loop nesting depth.  */
  int depth;

  /* The level of loops inside it.  */
  int level;

  /* Number of blocks of the loop.  */
  unsigned num_nodes;

  /* Superloops of the loop.  */
  struct loop **pred;

  /* The outer (parent) loop or NULL if outermost loop.  */
  struct loop *outer;

  /* The first inner (child) loop or NULL if innermost loop.  */
  struct loop *inner;

  /* Link to the next (sibling) loop.  */
  struct loop *next;

  /* Loop that is copy of this loop.  */
  struct loop *copy;

  /* Auxiliary info specific to a pass.  */
  void *aux;
};

/* Information pertaining to a loop.  */

struct loop_info
{
  /* Nonzero if there is a subroutine call in the current loop.  */
  int has_call;
  /* Body of the loop.  */
  basic_block *body;
  /* Bitmap of blocks that don't have to be reached because the control
     may exit the loop without entering them.  */
  sbitmap bb_after_exit;
  /* Bitmap of blocks that don't have to be reached because they are after
     call that does not have to return.  */
  sbitmap bb_after_call;
  /* List of MEMs that are stored in this loop.  */
  rtx store_mems;
  /* Nonzero if we don't know what MEMs were changed in the current
     loop.  This happens if the loop contains a call (in which case
     `has_call' will also be set) or if we store into more than
     NUM_STORES MEMs.  */
  int unknown_address_altered;
  /* The above doesn't count any readonly memory locations that are
     stored.  This does.  */
  int unknown_constant_address_altered;
};

/* Histogram of a loop.  */
struct loop_histogram
{
  unsigned steps;
  gcov_type *counts;
  gcov_type more;
};

/* Flags for state of loop structure.  */
enum
{
  LOOPS_HAVE_PREHEADERS = 1,
  LOOPS_HAVE_SIMPLE_PREHEADERS = 2,
  LOOPS_HAVE_SIMPLE_LATCHES = 4,
  LOOPS_HAVE_MARKED_IRREDUCIBLE_REGIONS = 8,
  LOOPS_HAVE_HISTOGRAMS_ON_EDGES = 16,
  LOOPS_HAVE_LANDING_PADS = 32
};

/* Structure to hold CFG information about natural loops within a function.  */
struct loops
{
  /* Number of natural loops in the function.  */
  unsigned num;

  /* Maximum nested loop level in the function.  */
  unsigned levels;

  /* Array of natural loop descriptors (scanning this array in reverse order
     will find the inner loops before their enclosing outer loops).  */
  struct loop *array;

  /* The above array is unused in new loop infrastructure and is kept only for
     purposes of the old loop optimizer.  Instead we store just pointers to
     loops here.  */
  struct loop **parray;

  /* Pointer to root of loop hierarchy tree.  */
  struct loop *tree_root;

  /* Information derived from the CFG.  */
  struct cfg
  {
    /* The bitmap vector of dominators or NULL if not computed.  */
    dominance_info dom;

    /* The ordering of the basic blocks in a depth first search.  */
    int *dfs_order;

    /* The reverse completion ordering of the basic blocks found in a
       depth first search.  */
    int *rc_order;
  } cfg;

  /* State of loops.  */
  int state;
};

/* Flags for loop discovery.  */

#define LOOP_TREE		1	/* Build loop hierarchy tree.  */
#define LOOP_PRE_HEADER		2	/* Analyze loop preheader.  */
#define LOOP_ENTRY_EDGES	4	/* Find entry edges.  */
#define LOOP_EXIT_EDGES		8	/* Find exit edges.  */
#define LOOP_EDGES		(LOOP_ENTRY_EDGES | LOOP_EXIT_EDGES)
#define LOOP_ALL	       15	/* All of the above  */

/* The maximal register number considered in iv analysis.  */
extern unsigned loop_max_regno;

/* An array that holds some temporary values of registers.  Used during
   the iv analysis, then left for free use by anyone to save time with
   allocating/freeing it.  */
extern rtx *iv_register_values;

/* Sbitmap of registers that are interesting for induction variable
   analysis.  */
extern sbitmap iv_interesting_reg;

/* The induction variables at loop entries.  */
extern rtx **loop_entry_values;

/* The values of registers at entries to the loops.  */
extern rtx **initial_values;

/* The induction variable occurences.  */
struct iv_occurence_base_class;
struct iv_occurence_step_class;

struct iv_occurence
{
  struct iv_occurence_base_class *base_class;
					/* The base class it belongs to.  */
  struct iv_occurence *oc_next;		/* The next occurence.  */
  enum machine_mode real_mode;		/* Mode in that iv iterates.  */
  enum machine_mode extended_mode;	/* Mode to that that iv is extended.  */
  enum rtx_code extend;			/* Type of extend used for it.  */
  rtx delta;				/* The induction variable is in the
					   form base + delta + iteration * step,
					   where delta is const_int;
					   base and step can be obtained
					   from base_class.  */
  rtx local_base;			/* The "local" definition of base,
					   without all the strange stuff
					   introduced by substituting the
					   initial values.  It includes value
					   of delta, i.e. the iv has also
					   value local_base + iteration * step.  */
  rtx value;				/* And the value itself.  */
  rtx insn;				/* The insn where the iv occurs.  */
  rtx *occurence;			/* The occurence itself. Either
					   a set with this value, a mem
					   whose address is this value or
					   comparison for that this is the
					   ARGth argument.  */
  int arg;				/* Of the occurence this iv_occ
					   represents in case it is
					   comparison.  */
  void *aux;				/* Pass specific information.  */
};

struct iv_occurence_base_class
{
  struct iv_occurence_step_class *step_class;
					/* The step class it belongs to.  */
  struct iv_occurence_base_class *bc_next;
					/* Next occurence with a different base.  */
  struct iv_occurence *oc_first;	/* The occurences.  */
  rtx base;				/* Base of ivs in this class; step can
					   be obtained from step_class.  */
};

struct iv_occurence_step_class
{
  struct iv_occurence_step_class *sc_next;
					/* Next occurence with a different step.  */
  struct iv_occurence_base_class *bc_first;
					/* The occurences with this step.  */
  rtx step;				/* Step of ivs in this class.  */
};

/* List of movables.  */
struct movable_list
{
  struct movable *elt;
  struct movable_list *next;
};

/* Actions scheduled by strength reduction.  */
struct ivopt_actions
{
  struct str_red *ivs;
  struct repl *replacements;
  struct fval_repl *repl_final_value;
};

/* Information added to dataflow by loop optimizer.  */
struct loop_df_info
{
  rtx value;			/* For induction variable analysis.  */
  struct movable *movable;	/* For loop invariant motion.  */
};
#define DF_REF_AUX_VALUE(REF) (((struct loop_df_info *) (REF)->aux)->value)
#define DF_REF_AUX_MOVABLE(REF) (((struct loop_df_info *) (REF)->aux)->movable)

/* For each loop, a linklist of induction variable occurences.  */
extern struct iv_occurence_step_class **iv_occurences;

/* The dataflow information.  */
extern struct df *loop_df;

/* The list of blocks in dominance order.  */
extern basic_block *block_dominance_order;

/* Estimate on number of available registers shared between invariant motion
   and induction variable optimizations.  */
extern int *loop_avail_regs;

/* Loop recognition.  */
extern int flow_loops_find (struct loops *, int flags);
extern int flow_loops_update (struct loops *, int flags);
extern void flow_loops_free (struct loops *);
extern void flow_loops_dump (const struct loops *, FILE *,
			     void (*)(const struct loop *, FILE *, int), int);
extern void flow_loop_dump (const struct loop *, FILE *,
			    void (*)(const struct loop *, FILE *, int), int);
extern void flow_loop_free (struct loop *);
void mark_irreducible_loops (struct loops *);

/* Loop datastructure manipulation/querying.  */
extern void flow_loop_tree_node_add (struct loop *, struct loop *);
extern void flow_loop_tree_node_remove (struct loop *);
extern bool flow_loop_outside_edge_p (const struct loop *, edge);
extern bool flow_loop_nested_p (const struct loop *, const struct loop *);
extern bool flow_bb_inside_loop_p (const struct loop *, const basic_block);
extern struct loop *find_common_loop (struct loop *, struct loop *);
extern int num_loop_insns (struct loop *);
extern int average_num_loop_insns (struct loop *);
extern int get_loop_level (const struct loop *);

/* Loops & cfg manipulation.  */
extern basic_block *get_loop_body (const struct loop *);
extern edge *get_loop_exit_edges (const struct loop *, unsigned *);
extern rtx loop_first_insn (const struct loop *);

extern edge loop_preheader_edge (const struct loop *);
extern edge loop_latch_edge (const struct loop *);

extern void add_bb_to_loop (basic_block, struct loop *);
extern void remove_bb_from_loops (basic_block);

extern void cancel_loop (struct loops *, struct loop *);
extern void cancel_loop_tree (struct loops *, struct loop *);

extern basic_block loop_split_edge_with (edge, rtx, struct loops *);
extern int fix_loop_placement (struct loop *);

enum
{
  CP_SIMPLE_PREHEADERS = 1,
  CP_INSIDE_CFGLAYOUT = 2
};

extern void create_preheaders (struct loops *, int);
extern void force_single_succ_latches (struct loops *);
extern void create_landing_pads (struct loops *);

extern void move_histograms_to_loops (struct loops *);
extern struct loop_histogram *copy_histogram (struct loop_histogram *, int);
extern void add_histogram (struct loop_histogram *, struct loop_histogram *,
			   int);
extern void free_histogram (struct loop_histogram *);

extern void verify_loop_structure (struct loops *);

/* Loop analysis.  */
extern void compute_simple_loop_info (struct loops *);
extern bool simple_loop_p (struct loops *, struct loop *, struct loop_desc *);
extern rtx count_loop_iterations (struct loop_desc *, rtx, rtx);
extern bool just_once_each_iteration_p (struct loops *,struct loop *,
					basic_block);
extern unsigned expected_loop_iterations (const struct loop *);
unsigned HOST_WIDE_INT loop_iterations_max (struct loop *);

/* Loop manipulation.  */
extern bool can_duplicate_loop_p (struct loop *loop);

#define DLTHE_FLAG_UPDATE_FREQ		1
#define DLTHE_PROB_UPDATING(X)		(X & 6)
#define DLTHE_USE_HISTOGRAM_PROB	0
#define DLTHE_USE_WONT_EXIT		2
extern int duplicate_loop_to_header_edge (struct loop *, edge,
					  struct loops *, unsigned,
					  sbitmap, edge, edge *,
					  unsigned *, int);
extern struct loop *loopify (struct loops *, edge, edge, basic_block);
extern void unloop (struct loops *, struct loop *);
extern bool remove_path (struct loops *, edge);
extern edge split_loop_bb (struct loops *, basic_block, rtx);

/* Induction variables analysis.  */
extern void initialize_iv_analysis (struct loops *);
extern void finalize_iv_analysis (void);
extern void analyse_induction_variables (void);

/* Functions to query and manipulate iv analysis results.  */
extern void iv_load_used_values (rtx, rtx *);
extern rtx get_def_value (rtx, unsigned);
extern rtx get_use_value (rtx, unsigned);
extern rtx iv_get_condition_value (edge);

/* Some functions to manipulate iv rtxes.  */
extern rtx iv_simplify_using_initial_values (enum rtx_code, rtx, struct loop *);
extern rtx iv_omit_initial_values (rtx);
extern rtx iv_simplify_using_branches (enum rtx_code, rtx, struct loop *);

/* Functions to alter insns while keeping the iv information up-to-date.  */
extern rtx iv_emit_insn_before (rtx, rtx);
extern rtx iv_emit_insn_after (rtx, rtx);

/* Utility functions shared by induction variable optimizations and
   invariant motion.  */
extern void hoist_insn_to_depth (struct loop *, int, rtx, int);
extern struct ref *find_single_def_use (rtx, struct loop *, rtx);

/* Loop optimizer driver.  */
extern struct loops *loop_optimizer_init (FILE *);
extern void loop_optimizer_optimize (struct loops *);
extern void loop_optimizer_finalize (struct loops *, FILE *);

/* Optimization passes.  */
extern void copy_loop_headers (void);
extern void unswitch_loops (struct loops *);

enum
{
  UAP_PEEL = 1,		/* Enables loop peeling.  */
  UAP_UNROLL = 2,	/* Enables peeling of loops if it seems profitable.  */
  UAP_UNROLL_ALL = 4	/* Enables peeling of all loops.  */
};

extern void decide_unrolling_and_peeling (struct loops *, int);
extern void unroll_and_peel_loops (struct loops *);
extern void doloop_optimize_loops (struct loops *);
extern void prefetch_loop_arrays (struct loops *);
extern bool reroll_loops (void);
extern struct movable_list *find_movables (struct loops *);
extern void loops_invariant_motion (struct loops *, struct movable_list *);
extern void detect_strength_reductions (struct loops *loops,
					 struct ivopt_actions *);
extern void execute_strength_reductions (struct loops *,
					 struct ivopt_actions *);
