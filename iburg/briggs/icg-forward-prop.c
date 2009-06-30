/* 
 * Performs a pass of forward propagation of the side-tree data structure.
 * Intended to run after the update finder and aims to catch opportunities
 * exposed by the update finder (i.e., when what used to be 2 references
 * becomes a single reference).
 *
 * The idea is to examine all the expressions defined in each basic block.
 * If the expression is used once inside the block (and nowhere else),
 * then we'd like to propagate it forward to its use.  This'll allow
 * better matching during instruction selection.
 */

#include "obstack.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "icg.h"
#include "icg-opcode.h"
#include "insn-constants.h"
#include "sparseset.h"



sparseset live_set, interesting, dup_set;

static
void check_refs(icg_node *tree) {
  if (tree) {
    check_refs(tree->left);
    check_refs(tree->right);
    if (REG_DI <= tree->op && tree->op <= REG_SF) {
      if (!sparseset_bit_p(live_set, tree->r)) {
	if (sparseset_bit_p(interesting, tree->r))
	  sparseset_set_bit(dup_set, tree->r);
	else
	  sparseset_set_bit(interesting, tree->r);
      }
    }
  }
}
  


static
void check_lhs(icg_node *tree) {
  if (REG_DI <= tree->op && tree->op <= REG_SF)
    ;
  else if (REGX_DI <= tree->op && tree->op <= REGX_SF)
    ;
  else if (SUBREG_DI <= tree->op && tree->op <= SUBREG_SF)
    check_lhs(tree->left);
  else
    check_refs(tree);
}



static
void rewrite_refs(icg_node *tree) {
  if (tree) {
    rewrite_refs(tree->left);
    rewrite_refs(tree->right);
    if (REG_DI <= tree->op && tree->op <= REG_SF && sparseset_bit_p(interesting, tree->r)) {
      tree->rx = tree->r;
      tree->op = tree->op - REG_DI + REGX_DI;
    }
  }
}
  


static
void rewrite_lhs(icg_node *tree) {
  if (REG_DI <= tree->op && tree->op <= REG_SF)
    ;
  else if (REGX_DI <= tree->op && tree->op <= REGX_SF)
    ;
  else if (SUBREG_DI <= tree->op && tree->op <= SUBREG_SF)
    rewrite_lhs(tree->left);
  else
    rewrite_refs(tree);
}



static
void update_refs(icg_node *tree) {
  if (tree) {
    update_refs(tree->left);
    update_refs(tree->right);
    if (REG_DI <= tree->op && tree->op <= REG_SF)
      sparseset_set_bit(live_set, tree->rx);
    else if (REGX_DI <= tree->op && tree->op <= REGX_SF)
      sparseset_set_bit(live_set, tree->rx);
  }
}
  


static
void update_lhs(icg_node *tree) {
  if (REG_DI <= tree->op && tree->op <= REG_SF)
    sparseset_clear_bit(live_set, tree->r);
  else if (REGX_DI <= tree->op && tree->op <= REGX_SF)
    sparseset_clear_bit(live_set, tree->rx);
  else if (SUBREG_DI <= tree->op && tree->op <= SUBREG_SF)
    update_lhs(tree->left);
  else
    update_refs(tree);
}



/*
 * We need to walk backwards over the basic block,
 * looking for oppoertunities to rewrite REG references to REGX references.
 * These opportunities occure naturally when finding updates.
 * They're important to get because they indicate when a register goes dead,
 * which in turn helps control the forward propagation.
 */

static
void redo_dfa(basic_block bb) {
  rtx insn;
  unsigned i;
  unsigned *live_out = icg_lr_liveout[bb->index]->name;
  unsigned n = icg_lr_liveout[bb->index]->size;

  sparseset_clear(live_set);
  for (i = 0; i < n; i++)
    sparseset_set_bit(live_set, live_out[i]);

  if (dump_file) {
    fprintf(dump_file, "\n\tlive-out = { ");
    for (i = 0; i < icg_live_ranges; i++)
      if (sparseset_bit_p(live_set, i))
	fprintf(dump_file, "%d ", i);
    fprintf(dump_file, "}\n");
  }

  FOR_BB_INSNS_REVERSE(bb, insn) {
    unsigned id = INSN_UID(insn);
    icg_node *tree = icg_insn2tree[id];
    if (tree) {
      /* look for REG refs that should be REGX */
      sparseset_clear(interesting);
      sparseset_clear(dup_set);
      if (tree->op == SET_ALL) {
	check_lhs(tree->left);
	check_refs(tree->right);
      }
      else
	check_refs(tree);
      sparseset_and_compl(interesting, interesting, dup_set);

      /* rewrite interesting REG refs to REGX */
      if (sparseset_cardinality(interesting) > 0) {
	if (tree->op == SET_ALL) {
	  rewrite_lhs(tree->left);
	  rewrite_refs(tree->right);
	}
	else
	  rewrite_refs(tree);
      }

      /* update live_set */
      if (tree->op == SET_ALL) {
	update_lhs(tree->left);
	update_refs(tree->right);
      }
      else
	update_refs(tree);
    }
  }

  if (dump_file) {
    fprintf(dump_file, "\tlive-in = { ");
    for (i = 0; i < icg_live_ranges; i++)
      if (sparseset_bit_p(live_set, i))
	fprintf(dump_file, "%d ", i);
    fprintf(dump_file, "}\n");
  }
}






struct obstack prop_obstack;

typedef struct dep {
  unsigned lr;
  struct dep *next;
} Dep;


Dep *mem_deps;
Dep **reg_dep;

struct _icg_node **reg_def;

static
void cleanup(Dep *dep) {
  while (dep) {
    reg_def[dep->lr] = 0;
    dep = dep->next;
  }
}


/*
 * if we see a REG reference, then we delete any associated definition,
 * since we don't want to propagate things that have multiple uses.
 */

static
void look_for_reg_refs(icg_node *tree) {
  if (tree) {
    look_for_reg_refs(tree->left);
    look_for_reg_refs(tree->right);
    if (REG_DI <= tree->op && tree->op <= REG_QI)
      reg_def[tree->r] = 0;
  }
}


static
Dep *add_dep_to_list(unsigned r, Dep *list) {
  Dep *dep = (Dep *) obstack_alloc(&prop_obstack, sizeof(Dep));
  dep->lr = r;
  dep->next = list;
  return dep;
}


static
void collect_dependences(unsigned r, icg_node *tree) {
  if (tree) {
    collect_dependences(r, tree->left);
    collect_dependences(r, tree->right);
    if (REGX_DI <= tree->op && tree->op <= REGX_QI)
      reg_dep[tree->rx] = add_dep_to_list(r, reg_dep[tree->rx]);
    else if (REG_DI <= tree->op && tree->op <= REG_QI)
      reg_dep[tree->r] = add_dep_to_list(r, reg_dep[tree->r]);
    else if (MEM_DI <= tree->op && tree->op <= MEM_QI)
      mem_deps = add_dep_to_list(r, mem_deps);
  }
}


static
void handle_def(unsigned r, icg_node *expr) {
  reg_def[r] = expr;
  collect_dependences(r, expr);
}


static
void forward_prop(basic_block bb) {
  rtx insn;
  unsigned i;
  mem_deps = 0;
  for (i = 0; i < icg_live_ranges; i++) {
    reg_dep[i] = 0;
    reg_def[i] = 0;
  }
  FOR_BB_INSNS(bb, insn) {
    unsigned id = INSN_UID(insn);
    icg_node *tree = icg_insn2tree[id];
    if (tree) {
      look_for_reg_refs(tree);
      switch (tree->op) {
      case SET_ALL: {
	icg_node *left = tree->left;
	if (REGX_DI <= left->op && left->op <= REGX_QI)
	  handle_def(left->rx, tree->right);
	else if (REG_DI  <= left->op && left->op <= REG_QI)
	  handle_def(left->r, tree->right);
	else if (SUBREG_DI <= left->op && left->op <= SUBREG_QI) {
	  if (REGX_DI <= left->left->op && left->left->op <= REGX_QI)
	    handle_def(left->rx, tree->right);
	  else if (REG_DI  <= left->left->op && left->left->op <= REG_QI)
	    handle_def(left->r, tree->right);
	  else
	    cleanup(mem_deps);
	}
	else
	  cleanup(mem_deps);
	break;
      }
      case MEM_PLUS_DI:     case MEM_PLUS_SI:     case MEM_PLUS_HI:     case MEM_PLUS_QI:
      case MEM_MINUS_DI:    case MEM_MINUS_SI:    case MEM_MINUS_HI:    case MEM_MINUS_QI:
      case MEM_NEG_DI:      case MEM_NEG_SI:      case MEM_NEG_HI:      case MEM_NEG_QI:
      case MEM_NOT_DI:      case MEM_NOT_SI:      case MEM_NOT_HI:      case MEM_NOT_QI:
      case MEM_AND_DI:      case MEM_AND_SI:      case MEM_AND_HI:      case MEM_AND_QI:
      case MEM_IOR_DI:      case MEM_IOR_SI:      case MEM_IOR_HI:      case MEM_IOR_QI:
      case MEM_XOR_DI:      case MEM_XOR_SI:      case MEM_XOR_HI:      case MEM_XOR_QI:
      case MEM_ASHIFT_DI:   case MEM_ASHIFT_SI:   case MEM_ASHIFT_HI:   case MEM_ASHIFT_QI:
      case MEM_LSHIFTRT_DI: case MEM_LSHIFTRT_SI: case MEM_LSHIFTRT_HI: case MEM_LSHIFTRT_QI:
      case MEM_ASHIFTRT_DI: case MEM_ASHIFTRT_SI: case MEM_ASHIFTRT_HI: case MEM_ASHIFTRT_QI:
      case MEM_ROTATE_DI:   case MEM_ROTATE_SI:   case MEM_ROTATE_HI:   case MEM_ROTATE_QI:
      case MEM_ROTATERT_DI: case MEM_ROTATERT_SI: case MEM_ROTATERT_HI: case MEM_ROTATERT_QI: {
	icg_node *mem = tree->left;
	icg_node *addr = mem->left;
	if (REGX_DI <= addr->op && addr->op <= REGX_QI) {
	  unsigned r = addr->rx;
	  if (reg_def[r]) {
	    
	  }
	}
	cleanup(mem_deps);
	break;
      }
      default:
	cleanup(mem_deps);
	break;
      }
    }
  }
}





void icg_forward_prop(void) {
  basic_block bb;
  void *big_mark;

  if (dump_file)
    fprintf(dump_file, "\nstarting forward prop\n");

  live_set = sparseset_alloc(icg_live_ranges);
  dup_set = sparseset_alloc(icg_live_ranges);
  interesting = sparseset_alloc(icg_live_ranges);

  gcc_obstack_init(&prop_obstack);
  big_mark = obstack_alloc(&prop_obstack, 0);
  reg_dep = (Dep **) obstack_alloc(&prop_obstack, icg_live_ranges*sizeof(Dep *));
  reg_def = (struct _icg_node **) obstack_alloc(&prop_obstack, icg_live_ranges*sizeof(struct _icg_node *));
  FOR_EACH_BB(bb) {
    void *prop_mark = obstack_alloc(&prop_obstack, 0);
    redo_dfa(bb);
    forward_prop(bb);
    obstack_free(&prop_obstack, prop_mark);
  }
  obstack_free(&prop_obstack, big_mark);
  sparseset_free(live_set);
  sparseset_free(dup_set);
  sparseset_free(interesting);

  if (dump_file)
    fprintf(dump_file, "finished forward prop\n\n");
}
