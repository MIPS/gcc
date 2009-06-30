/* Integrated code generator (ICG), combined instruction selector and
   register allocator.  Copyright (C) 2008 Free Software Foundation, Inc.

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

#ifndef GCC_ICG_H
#define GCC_ICG_H

#include "obstack.h"

/* icg_state_type is an opaque type that points to a burg private data struct */
typedef void *icg_state_type;


#define MAX_KIDS 10	/* maximum number of NT children in pattern */

#include "icg-opcode.h"

enum {
  burm_goal_NT=1	/* stmt */
};
typedef short NT;	/* type must agree with burm */
typedef int RuleNumber;	/* type must agree with burm */

typedef struct _amode {
  unsigned char base_valid;	/* 1 if base valid; 0 if base invalid */
  unsigned char base_rip;	/* 1 if base is really %rip */
  unsigned base;		/* base register, iff valid */
  int scale;			/* scale value (1,2,4,8) */
  unsigned index;		/* if scale > 0, then associated index reg */
  int disp;			/* displacement value */
  const char *string;		/* symbol name */
} amode;

typedef struct _icg_node {
  icg_opcodes op;

  struct _icg_node *left;	/* left child */
  struct _icg_node *right;	/* right child */
  icg_state_type state_label;	/* from burg */
  NT goalNT;			/* goal non terminal for best match */

  rtx rtl;		/* rtx of operation */
  rtx insn;		/* rtx of containing insn */

  unsigned ssa_name;	/* real register REGNO or SSA name if symbolic */
  unsigned live_range;	/* real register REGNO or live range name if symbolic */

  unsigned r;		/* register that may NOT be reused */
  unsigned rx;		/* register that   might be reused */

  bool spilled;         /* indicates that an lhs register has been spilled (replace with a mem reference) */

  amode a;		/* matched addressing mode */

  long long int val;	/* value of constant */

  enum rtx_code code;	/* used to indicate type of condition: LE, LTU, ... */
  enum machine_mode cc_code;	/* one of: CCmode CCFPU CCFPmode */

  unsigned extra;
  unsigned freed;

  /*
   * perm controls the visit order to the children.
   */
  short perm[MAX_KIDS];	/* visit order */
  /*
   * perm_kids is the number of kids iff this node is covered by the root of a pattern
   * that has >=2 non terminals.  The primary purpose of this is to avoid
   * setting up a permutation for a chain rule that clobbers the permutation
   * for a rule where the permutation really matters.
   */
  int perm_kids;		/* access control on perm: either 0 or >= 2 */

  unsigned id;			/* instruction id (?) */

  /*
   * mode reflects whatever the opcode said: ADD_DI would be DImode and so on.
   */
  enum machine_mode mode;

} icg_node;

typedef icg_node *icg_nodes;

extern      bool *icg_insn2dirty;
extern icg_node **icg_insn2tree;
extern       int *icg_insn2goalNT;

#define FOR_BB_ICG_TREES(BB, PT) \
  for ((PT) = first_bb_icg_tree (BB); (PT); (PT) = next_bb_icg_tree (BB, PT))

int for_each_icg_tree_def (icg_node*, int (*)(icg_node *, void *), void*);
int for_each_icg_tree_use (icg_node*, int (*)(icg_node *, void *), void*);
icg_node *first_bb_icg_tree (basic_block);
icg_node *next_bb_icg_tree (basic_block, icg_node*);
icg_node *icg_tree_rewrite (icg_node*);

/* List of SSA phi nodes associated with a particular basic block,
   computed in icg-ssa.c */
typedef struct _icg_phi_node {
  struct _icg_phi_node *next;
  /* regno of register in original RTL */
  unsigned reg;
  unsigned def;
  /* Dynamically-sized array.  Size of USES is number of predecessors
     of basic block. */
  unsigned uses[1];
} icg_phi_node;

/* List of phi nodes for each BB, indexed by basic block index. */
extern icg_phi_node **icg_bb2phi;

typedef struct _icg_names {
  struct _icg_names *next;
  unsigned size;
  unsigned name[1];
} icg_names;


typedef struct {
  unsigned root;   /* for union-find structure                       */
  unsigned degree; /* number of neighbors in the interference graph  */
  float cost;      /* spill cost, possibly infinite                  */
  float loads;     /* weighted number of loads                       */
  float stores;    /* weighted number of stores                      */
  float copies;    /* weighted number of copies                      */
  float points;    /* weighted number of points                      */
  int color;       /* assigned during coloring, -1 indicates spilled */
  unsigned kind;   /* bit vector to indicated kind of register       */
  unsigned stack;  /* used to implement stack during coloring        */
  long long int offset; /* if spilled, location in stack frame       */
  icg_names *av;   /* adjacency vectors                              */
  bool leaf;       /* used during spill-cost calculation             */
} icg_reg_info;

#define INT_REGISTER   (1 << 0)
#define FLOAT_REGISTER (1 << 1)

extern icg_reg_info *icg_reg_vector;
extern unsigned icg_ssa_regs;
extern unsigned icg_live_ranges;
extern unsigned icg_interior_regs;

/* Mapping from SSA name to live range name. */
extern unsigned int *icg_ssa_to_lr;
/* SSA variables live out of each basic block.  Indexed by basic block index. */
extern icg_names **icg_ssa_liveout;
/* Live ranges live out of each basic block. Indexed by basic block index. */
extern icg_names **icg_lr_liveout;


extern struct obstack icg_obstack;
extern struct obstack icg_graph_obstack;

#define FOR_BB_ICG_PHI_NODES(BB, PHI) \
  for ((PHI) = icg_bb2phi[(BB)->index]; (PHI); (PHI) = (PHI)->next)

/* In icg.c. */
extern void *icg_alloc (size_t size);
extern void *icg_calloc (size_t size);
extern bool icg_reg_p (icg_node*);
extern bool icg_dead_reg_p (icg_node*);
extern bool icg_const_p (icg_node*);
extern void print_inline_icg_tree (FILE*, icg_node*);
extern void print_icg_tree (FILE*, icg_node*, const char*, ...)
  __attribute__((format(printf, 3, 4)));
extern void dump_icg_function (FILE*);
extern void dump_icg_bb (FILE*, basic_block);
extern void debug_icg (icg_node*);

/* In icg-emit.c */
extern void icg_print_rulecount (FILE *dump_file, int dump_code);
extern void icg_dump_rulecount (void);
extern void icg_read_rulecount (FILE *read_file);

/* In icg-build-tree.c */
extern unsigned icg_create_side_tree(void);
typedef enum _icg_analyze_asm_key {
  ICG_ASM_PHASE_FILTER,
  ICG_ASM_PHASE_BUILD,
  ICG_ASM_PHASE_DEBUG
} icg_analyze_asm_key;
extern int icg_analyze_asm(rtx expr, icg_analyze_asm_key how);

/* In icg-ssa.c */
extern void icg_build_ssa (void);
extern void icg_build_live_ranges (void);
extern void dump_icg_rtl (FILE *fp);
extern void dump_icg_ssa (FILE *fp);
extern void dump_icg_live_ranges (FILE *fp);

/* In icg-burg.c (generated by iburg) */
extern icg_state_type icg_burm_label(icg_nodes p);
extern RuleNumber icg_burm_rule(icg_state_type state, NT goalNT);
extern float icg_burm_get_cost(icg_state_type state, NT goalNT);
extern const NT *const icg_burm_nts[];
extern icg_nodes *icg_burm_kids(icg_nodes p, RuleNumber eruleno, icg_nodes kids[]);

/* In icg-debug.c */
extern void icg_trace_rule(FILE *dump_file, const icg_node *p,
  int depth, NT goalNT);

/* In icg-burg.c (generated by iburg -I) */
extern const char *const icg_burm_ntname[];
extern const char *const icg_burm_opname[];
extern const char *const icg_burm_string[];
extern       char  const icg_burm_arity[];

/*
 * These are enumerations in gcc's i386 register space
 */
enum _gcc_i386_registers_t {
  REG_RAX=0,		/* temporary register */
  REG_RBX=3,		/* callee-saved */
  REG_RCX=2,		/* 4th argument to functions */
  REG_RDX=1,		/* 3rd argument to functions */
  REG_RSI=4,		/* 2nd argument to functions */
  REG_RDI=5,		/* 1st argument to functions */
  REG_RBP=6,		/* HARD_FRAME_POINTER_REGNUM */
  REG_RSP=7,		/* STACK_POINTER_REGNUM */
  REG_ARGP=16,          /* argument pointer */
  REG_CC=17,		/* condition code/flags */
  REG_CCFPU=19,		/* floating condition code/flags */
  REG_FP=20             /* frame pointer */
};


extern unsigned icg_path_compress(unsigned);

/* In icg-reassociate.c */
extern void icg_reassociate(void);
extern void icg_forward_prop(void);
extern void icg_select(void);
extern void icg_find_names(void);
extern void icg_supairs(void);
extern void icg_graph(unsigned pass);
extern void icg_costs(unsigned pass);
extern void icg_color(void);
extern void icg_remat(void);
extern bool icg_spill(unsigned pass);
extern void icg_find_updates(void);
extern void icg_final(void);
extern void icg_emit(void);
extern void icg_debug(const char *whence);
extern icg_opcodes icg_op_of_constant(long long int offset);

extern void icg_print_opcode(FILE *fp, icg_node *node,
  int indent, bool indent_p);
extern void icg_print_runs(FILE *fp, const unsigned *p, size_t size);
void icg_diagnose_match_candidates(FILE *dump_file,
  const icg_state_type state, icg_node *node,
  int indent, bool indent_p);

#define icg_nyi(fmt, ...) icg_nyi_handler(__FILE__, __LINE__, fmt, ## __VA_ARGS__)
extern void icg_nyi_handler(const char *filename, int lineno,
  const char *fmt, ...) __attribute__((format (printf, 3, 4)));

#endif /* GCC_ICG_H */
