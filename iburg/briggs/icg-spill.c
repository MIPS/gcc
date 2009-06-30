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
#include "flags.h"
#include "tm_p.h"
#include "output.h"

#include "icg.h"
#include "icg-opcode.h"


/*
 * The structure of the stack frame differs so significantly depending on whether
 * or not a frame pointer is used that I'm going to talk about the two cases
 * completely seperately.  The code will also deal with the two cases quite seperately.
 *
 * First though, I'll talk a bit about terminology.
 * In the RTL as it arrives on our doorstep, gcc refers to a FRAMEP and an ARGP
 * (frame pointer and argument pointer).  Offsets from the frame pointer are used
 * to refer to variables in the stack frame and offsets from the argument pointer
 * are used to refer to incoming argumnets stored in the stack frame.  Note that 
 * the first 6 arguments are usually passed in registers and the rest are stored in
 * memory.
 *
 * One of the jobs of the register allocator is to rewrite references to FRAMEP and ARGP
 * as references to either the base pointer (if we are using a frame pointer) or to the
 * stack pointer (if we are not using a frame pointer).  Normally, our code is compiled
 * with the flag -fno-omit-frame-pointer which causes the compiler to use a frame pointer
 * all the time.  This is actually a bit slower, but is apparently useful for some of
 * tools.
 *
 * There's an optimization possible for leaf routines.
 * Since the calling convention specifies that the 128 bytes
 * immediately below the SP are a "red zone" that may be used
 * as a scratch area, we can point the stack pointer up to 128
 * bytes higher in the stack frame.  The advantage is that offsets
 * frame the SP will be slightly smaller as will the stack adjustment,
 * perhaps saving a bit of encoding space.  Luckily, there's a phase
 * that runs downstream that will perform the necessary adjustments.
 * We need only set the flag current_function_is_leaf (when we're certain
 * the function is a leaf!).  Currently, we do this as part of icg-build-tree.
 *
 * The stack pointer is always kept aligned to a 16-byte boundary,
 * so sometimes we'll see the frame be 8 bytes larger than otherwise necessary.
 *
 * This has all proved pretty tedious to get correct;
 * hence the relative extensive documentation.
 *
 *
 *
 * Here's a picture of the stack frame, with a frame pointer
 *
 *          |    ...            |
 *          | high addresses    |
 *          +-------------------+
 *          |                   |
 *          | args              |
 * 16(%rbp) |                   | 0(ARGP)
 *          +-------------------+
 *  8(%rbp) | return            |
 *          +-------------------+
 *  0(%rbp) | old BP            | 0(FRAMEP)
 *          +-------------------+
 *          |                   |
 *          | callee-saves      |
 *          | registers         |
 *          |                   |
 *          +-------------------+
 *          |                   |
 *          | fix-sized         |
 *          | variables         |
 *          |                   |
 *          +-------------------+
 *          |                   |
 *          | spills            |
 *          |                   |
 *          +-------------------+
 *          |                   |
 *          | dynamically-sized |
 *          | variables         |
 *  0(%rsp) |                   |
 *          +-------------------+
 *          | low addresses     |
 *          |    ...            |
 *
 *
 * The frame pointer will be required if the size of variables is not known
 * at compile time (e.g., if there's a call to alloca).  It'll also be
 * required if the routine is compiled using -fno-omit-frame-pointer.
 * We don't actually to figure it out; the variable frame_pointer_needed
 * (set in icg.c) gives us the answer.
 *
 * If the frame pointer is used, then everything will be referenced via the 
 * base pointer.  The stack pointer is used only to support things like alloca()
 * and ever deeper calls.

 * The interesting problem is in converting references to the ARGP and FRAMEP
 * (names used in the RTL entering the allocator) to either RBP or RSP (actual
 * machine registers used after the allocator).
 *
 * If we're using the frame pointer, then we need to convert all references to
 * ARGP into references to RBP, including an adjustment of offsets.  Similarly,
 * references to FRAMEP will be changed into references to RBP.  Note that
 * adjusting the offsets to the FRAMEP will depend on the number of callee-saves
 * registers.  The same is true with spilled values.
 *
 * In my experiments, it appears that gcc (downstream of the allocator) inserts
 * code to save and restore the callee-saves registers, though I must deal with
 * the space requirements for them; that is, I must correctly adjust offsets
 * from the RBP or RSP to reach across the space required for the callee-saves
 * registers.  Care is required here, since the number of callee-saves registers
 * may change (up or down) from pass to pass of the allocator.
 *
 * Generally, my approach is to keep track of the total adjustments I've applied
 * in the passes so far, then compute the new adjustment required.  In this fashion,
 * we can avoid any last second scramble to handle offsets greater than 2 GBytes
 * (which require an additional register); it's just handled naturally as part
 * of the allocators iterative structure. 
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * Here's a picture of the stack frame, when the frame pointer is omitted.
 *
 *          |    ...            |
 *          | high addresses    |
 *          +-------------------+
 *          |                   |
 *          | args              |
 *          |                   | 0(ARGP)
 *          +-------------------+
 *          | return            | 0(FRAMEP)
 *          +-------------------+
 *          |                   |
 *          | callee-saves      |
 *          | registers         |
 *          |                   |
 *          +-------------------+
 *          |                   |
 *          | fix-sized         |
 *          | variables         |
 *          |                   |
 *          +-------------------+
 *          |                   |
 *          | spills            |
 *  0(%rsp) |                   |
 *          +-------------------+
 *          | low addresses     |
 *          |     ...           |
 *
 *
 * If the frame pointer is omitted, then all variables, spills, and args
 * will be addressed by an offset from the stack pointer.
 *
 * The interesting problem is in converting references to the ARGP and FRAMEP
 * (names used in the RTL entering the allocator) to either RBP or RSP (actual
 * machine registers used after the allocator).
 *
 * If we're using the base pointer, then we need to convert all references to
 * ARGP into references to RBP, including an adjustment of offsets.  Similarly,
 * references to FRAMEP will be changed into references to RBP.  Note that
 * adjusting the offsets to the FRAMEP will depend on the number of callee-saves
 * registers.
 *
 * If we're not using the base pointer (and we'd prefer not to, since avoiding it
 * will save space, time, and a register), more adjustments are required.
 * Both the ARGP and FRAMEP are rewritten to refer to the RSP.  Offsets will
 * need to be adjusted and the adjustment will depend on the number of callee-saves
 * registers and the number of spills.  Since instruction selection can depend on
 * the size of offsets, we'll need to redo instruction selection whenever an offset
 * changes (though only for the local tree).
 *
 * In my experiments, it appears that gcc (downstream of the allocator) inserts
 * code to save and restore the callee-saves registers, though I must deal with
 * the space requirements for them; that is, I must correctly adjust offsets
 * from the RSP to reach across the space required for the callee-saves
 * registers.  Care is required here, since the number of callee-saves registers
 * may change (up or down) from pass to pass of the allocator.  Similarly, the
 * amount of space required by the spilled values will tend to increase from
 * pass to pass.
 *
 * Generally, my approach is to keep track of the total adjustments I've applied
 * in the passes so far, then compute the new adjustment required.  In this fashion,
 * we can avoid any last second scramble to handle offsets greater than 2 GBytes
 * (which require an additional register); it's just handled naturally as part
 * of the allocators iterative structure. 
 *
 */


long long int accumulated_fp_adjustment   = 0;
long long int accumulated_argp_adjustment = 0;

long long int current_fp_adjustment   = 0;
long long int current_argp_adjustment = 0;


icg_opcodes icg_op_of_constant(long long int offset) {
  if      (offset == -1)  return CONST_N1;
  else if (offset == 0)   return CONST_0;
  else if (offset == 1)   return CONST_P1;
  else if (offset == 2)   return CONST_P2;
  else if (offset == 3)   return CONST_P3;
  else if (offset == 4)   return CONST_P4;
  else if (offset == 8)   return CONST_P8;
  else if (offset == 15)  return CONST_P15;
  else if (offset == 16)  return CONST_P16;
  else if (offset == 24)  return CONST_P24;
  else if (offset == 31)  return CONST_P31;
  else if (offset == 32)  return CONST_P32;
  else if (offset == 48)  return CONST_P48;
  else if (offset == 56)  return CONST_P56;
  else if (offset == 63)  return CONST_P63;
  else if (offset == 255) return CONST_P255;

  else if (offset > 0x7FFFFFFFLL) return CONST64P;
  else if (offset > 0x3FFFFFFFLL) return CONST32P;
  else if (offset > 0x1FFFFFFFLL) return CONST31P;
  else if (offset > 0xFFFFFFFLL)  return CONST30P;
  else if (offset > 0x7FFF)       return CONST29P;
  else if (offset > 0x3FFF)       return CONST16P;
  else if (offset > 0x1FFF)       return CONST15P;
  else if (offset > 0xFFF)        return CONST14P;
  else if (offset > 0x7FF)        return CONST13P;
  else if (offset > 0x7F)         return CONST12P;
  else if (offset > 0x3F)         return CONST8P;
  else if (offset > 0x1F)         return CONST7P;
  else if (offset > 0xF)          return CONST6P;
  else if (offset > 0x7)          return CONST5P;
  else if (offset > 0)            return CONST4P;

  else if (offset < -0x80000000LL) return CONST64N;
  else if (offset < -0x40000000LL) return CONST32N;
  else if (offset < -0x20000000LL) return CONST31N;
  else if (offset < -0x10000000LL) return CONST30N;
  else if (offset < -0x8000)       return CONST29N;
  else if (offset < -0x4000)       return CONST16N;
  else if (offset < -0x2000)       return CONST15N;
  else if (offset < -0x1000)       return CONST14N;
  else if (offset < -0x800)        return CONST13N;
  else if (offset < -0x80)         return CONST12N;
  else if (offset < -0x40)         return CONST8N;
  else if (offset < -0x20)         return CONST7N;
  else if (offset < -0x10)         return CONST6N;
  else if (offset < -0x8)          return CONST5N;
  else                             return CONST4N;
}


static
icg_node *kind_of_constant(long long int offset)
{
  icg_node *node = (icg_node *) icg_calloc(sizeof(icg_node));
  node->val = offset;
  node->a.string = NULL;
  node->op = icg_op_of_constant(offset);
  return node;
}


/*
 * The function make_spill_code is to be called from
 * within the spill { ... } arm of the grammar files.
 *
 * Return true if spill code was made
 */
static
bool make_spill_code(unsigned r, icg_node *p)
{
  if (icg_reg_vector[r].color < 0) {
    int new_op;
    icg_node *plus = (icg_node *) icg_calloc(sizeof(icg_node));
    icg_node *left  = (icg_node *) icg_calloc(sizeof(icg_node));
    icg_node *right = kind_of_constant(icg_reg_vector[r].offset + accumulated_fp_adjustment);

    if (dump_file) {
      fprintf(dump_file, "spilling: make_spill_code r=%3d root=%3d, offset=%lld\n",
	      r, icg_reg_vector[r].root, icg_reg_vector[r].offset);
    }

    switch (p->op) {
    case REG_DI: case REGX_DI: new_op = MEM_DI; break;
    case REG_SI: case REGX_SI: new_op = MEM_SI; break;
    case REG_HI: case REGX_HI: new_op = MEM_HI; break;
    case REG_QI: case REGX_QI: new_op = MEM_QI; break;
    case REG_DF: case REGX_DF: new_op = MEM_DF; break;
    case REG_SF: case REGX_SF: new_op = MEM_SF; break;
    default:
      fprintf(stdout, "make_spill_code: unexpected op\n");
      abort();
    }

    left->op = REG_DI;
    left->r = left->rx = REG_FP;
    plus->op = PLUS_DI;
    plus->left = left;
    plus->right = right;
    if (dump_file) {
      fprintf(dump_file,
	      "spilling:   smashing p->op from %d/%s to %d/%s (l=%s:%d r=%s:%lld)\n",
	      p->op, get_icg_opcode_name(p->op),
	      new_op, get_icg_opcode_name(new_op),
	      get_icg_opcode_name(left->op),
	      REG_FP,
	      get_icg_opcode_name(right->op),
	      icg_reg_vector[r].offset + accumulated_fp_adjustment);
    }
    p->op = new_op;
    p->left = plus;
    return true;
  }
  else
    return false;
}


#define find(r) (icg_reg_vector[r].root)


static
bool reduce_spill(icg_node *p, NT goalNT)
{
  bool dirty = false;
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    dirty |= reduce_spill(kid[i], nts[i]);
  switch (rule) {
#include "icg-spill.cases"
  }
  return dirty;
}


static
long long int extract_offset(const_rtx mem_rtx)
{
  const enum rtx_code mem_opcode = GET_CODE(mem_rtx);
  if (mem_opcode == MEM) {
    const_rtx plus_rtx = XEXP(mem_rtx, 0);
    const enum rtx_code plus_opcode = GET_CODE(plus_rtx);
    if (plus_opcode == PLUS) {
      const_rtx left_rtx = XEXP(plus_rtx, 0);
      const enum rtx_code left_opcode = GET_CODE(left_rtx);
      if (left_opcode == CONST_INT)
	return INTVAL(left_rtx);
      else {
	const_rtx right_rtx = XEXP(plus_rtx, 1);
	const enum rtx_code right_opcode = GET_CODE(right_rtx);
	if (right_opcode == CONST_INT)
	  return INTVAL(right_rtx);
      }
    }
  }
  fflush(stdout);
  fprintf(stderr, "extract_offset: poorly formated spill code\n");
  abort();
}


/*
 * Overwrite the register tree node with (PLUS_DI tree adjustment)
 */

static
bool replace_with_sum(icg_node *tree, long long int adjustment) {
  if (adjustment) {
    icg_node *c = kind_of_constant(adjustment);
    icg_node *r = (icg_node *) icg_calloc(sizeof(icg_node));
    memcpy(r, tree, sizeof(icg_node));
    tree->op = PLUS_DI;
    tree->left = r;
    tree->right = c;
    return true;
  }
  return false;
}


/*
 * return true if the sum rooted by tree contains a
 * reference to reg
 */

static
bool sum_contains_reg(icg_node *tree, unsigned reg) {
  if (tree) {
    if ((tree->op == REG_DI  && tree->r  == reg) ||
	(tree->op == REGX_DI && tree->rx == reg))
      return true;
    else if (tree->op == PLUS_DI)
      return sum_contains_reg(tree->left, reg) ||
	     sum_contains_reg(tree->right, reg);
  }
  return false;
}


/*
 * return true if the sum rooted by tree contains a
 * constant
 */

static
bool sum_contains_const(icg_node *tree) {
  if (tree) {
    if (tree->op >= CONST_0 && tree->op <= CONST64N)
      return true;
    else if (tree->op == PLUS_DI)
      return sum_contains_const(tree->left) ||
	     sum_contains_const(tree->right);
  }
  return false;
}


/*
 * Walk over the sum rooted by tree,
 * looking for a constant.  It's guaranteed to be there.
 * When found, add adjustment to it.
 */

static
bool adjust_const(icg_node *tree, long long int adjustment) {
  if (tree) {
    if (tree->op >= CONST_0 && tree->op <= CONST64N) {
      tree->val += adjustment;
      tree->op = icg_op_of_constant(tree->val);
      return true;
    }
    else if (tree->op == PLUS_DI)
      return adjust_const(tree->left, adjustment) ||
	     adjust_const(tree->right, adjustment);
  }
  return false;
}


static
bool adjust_register(icg_node *tree, unsigned reg, long long int adjustment);

static
void adjust_children_of_sum(icg_node *tree, unsigned reg, long long int adjustment) {
  if (tree) {
    if (tree->op == PLUS_DI) {
      adjust_children_of_sum(tree->left, reg, adjustment);
      adjust_children_of_sum(tree->right, reg, adjustment);
    }
    else if (tree->op != REG_DI)
      (void) adjust_register(tree, reg, adjustment);
  }
}


/*
 * Update all existing FP and ARGP references based on icg_current_adjustment.
 * Return true if any updates.
 */

static
bool adjust_register(icg_node *tree, unsigned reg, long long int adjustment) {
  if (tree) {
    if ((tree->op == REG_DI  && tree->r  == reg) ||
	(tree->op == REGX_DI && tree->rx == reg))
      return replace_with_sum(tree, adjustment);
    else if (tree->op == PLUS_DI &&
	     sum_contains_reg(tree, reg) &&
	     sum_contains_const(tree)) {
      (void) adjust_const(tree, adjustment);
      adjust_children_of_sum(tree, reg, adjustment);
      return true;
    }
    else /* please don't change the '|' below into '||' */
      return adjust_register(tree->left,  reg, adjustment) |
	     adjust_register(tree->right, reg, adjustment);
  }
  return false;
}


long long int register_save_space;

long long int size_variables;
int size_callee_saves;
int size_spills;

bool icg_spill(unsigned pass)
{
  unsigned r;
  basic_block bb;
  long long int temp;
  bool all_done = true;

  current_function_is_leaf = 0;  /* TODO: someday explore making this work again */

  if (dump_file) {
    fprintf(dump_file, "\n");
    fprintf(dump_file, "spilling: started {\n");
    if (frame_pointer_needed)
      fprintf(dump_file, "frame pointer needed\n");
    else
      fprintf(dump_file, "frame pointer not needed\n");
  }

  if (pass == 0) {
    accumulated_fp_adjustment   = 0;
    accumulated_argp_adjustment = 0;
  }

  /* find size of area for callee_saves register (may wobble up and down from pass to pass) */
  {
    bool used[FIRST_PSEUDO_REGISTER];
    size_callee_saves = 0;
    for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
      used[r] = false;
    for (r = FIRST_PSEUDO_REGISTER; r < icg_interior_regs; r++)
      if (icg_reg_vector[r].root == r &&
	  icg_reg_vector[r].color >= 0)
	used[icg_reg_vector[r].color] = true;
    if (used[41])
      size_callee_saves += 8;
    if (used[42])
      size_callee_saves += 8;
    if (used[43])
      size_callee_saves += 8;
    if (used[44])
      size_callee_saves += 8;
    if (used[REG_RBX])
      size_callee_saves += 8;
    if (used[REG_RBP])
      size_callee_saves += 8;
    size_variables = get_frame_size();
    if (dump_file)
      fprintf(dump_file, "callee saves = %d, vars = %lld, outgoing args = %d\n",
	      size_callee_saves, size_variables, crtl->outgoing_args_size);
  }


  if (frame_pointer_needed) {
    if (dump_file) {
      fprintf(dump_file, "spilling: assigning spill offsets\n");
      fprintf(dump_file, "\n");
    }

    for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++) {
      if (icg_reg_vector[r].root == r && icg_reg_vector[r].color < 0) {
	const_rtx x;
	all_done = false;
	if (icg_reg_vector[r].kind == INT_REGISTER)
	  x = assign_stack_local(DImode, 8, 8);
	else
	  x = assign_stack_local(V2DFmode, 16, 16);
	if (dump_file) {
	  fprintf(dump_file, "register %d\n", r);
	  print_inline_rtx(dump_file, x, 0);
	  fprintf(dump_file, "\n\n");
	}
	icg_reg_vector[r].offset = extract_offset(x) - size_callee_saves;
      }
    }

    if (pass == 0) {
      INITIAL_ELIMINATION_OFFSET(FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM, current_fp_adjustment);
      INITIAL_ELIMINATION_OFFSET(ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM, current_argp_adjustment);
      current_fp_adjustment   -= accumulated_fp_adjustment;
      current_argp_adjustment -= accumulated_argp_adjustment;
      INITIAL_ELIMINATION_OFFSET(FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM, accumulated_fp_adjustment);
      INITIAL_ELIMINATION_OFFSET(ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM, accumulated_argp_adjustment);
      
      /* update all existing FP and ARGP references based on current_adjustment */
      FOR_EACH_BB(bb) {
	rtx insn;
	FOR_BB_INSNS_REVERSE(bb, insn) {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree) {
	    icg_insn2dirty[id]  = adjust_register(tree, REG_FP,   current_fp_adjustment - size_callee_saves);
	    icg_insn2dirty[id] |= adjust_register(tree, REG_ARGP, current_argp_adjustment);
	  }
	}
      }
    }

    /* insert spill code based on accumulated_fp_adjustment */
    FOR_EACH_BB(bb) {
      rtx insn;
      FOR_BB_INSNS_REVERSE(bb, insn) {
	unsigned id = INSN_UID(insn);
	icg_node *tree = icg_insn2tree[id];
	if (tree) {
	  icg_insn2dirty[id] |= reduce_spill(tree, burm_goal_NT);
	}
      }
    }
  }





  /* deal with case when no frame pointer is required */

  else {
    if (dump_file) {
      fprintf(dump_file, "spilling: assigning spill offsets\n");
      fprintf(dump_file, "\n");
    }

    for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++) {
      if (icg_reg_vector[r].root == r && icg_reg_vector[r].color < 0) {
	const_rtx x;
	if (icg_reg_vector[r].kind == INT_REGISTER)
	  x = assign_stack_local(DImode, 8, 8);
	else
	  x = assign_stack_local(V2DFmode, 16, 16);
	if (dump_file) {
	  fprintf(dump_file, "register %d\n", r);
	  print_inline_rtx(dump_file, x, 0);
	  fprintf(dump_file, "\n\n");
	}
	icg_reg_vector[r].offset = extract_offset(x);
      }
    }

    /* I did a lot of experimentation to fine this formula for current_argp_adjustment.  Hope it's right! */
    INITIAL_ELIMINATION_OFFSET(ARG_POINTER_REGNUM, STACK_POINTER_REGNUM, current_argp_adjustment);
    current_argp_adjustment += (size_callee_saves >> 4) << 4;
    temp = current_argp_adjustment;
    current_argp_adjustment -= accumulated_argp_adjustment;
    accumulated_argp_adjustment = temp;
    

    INITIAL_ELIMINATION_OFFSET(FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM, current_fp_adjustment);
    current_fp_adjustment   -= accumulated_fp_adjustment;
    INITIAL_ELIMINATION_OFFSET(FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM, accumulated_fp_adjustment);

    if (dump_file) {
      fprintf(dump_file, "adjusting fp offsets by %lld\n", current_fp_adjustment);
      fprintf(dump_file, "adjusting argp offsets by %lld\n", current_argp_adjustment);
    }

    FOR_EACH_BB(bb) {
      rtx insn;
      FOR_BB_INSNS(bb, insn) {
	unsigned id = INSN_UID(insn);
	icg_node *tree = icg_insn2tree[id];
	if (tree) {
	  bool adjusted_fp   = current_fp_adjustment   == 0 ? false : adjust_register(tree, REG_FP, current_fp_adjustment);
	  bool adjusted_argp = current_argp_adjustment == 0 ? false : adjust_register(tree, REG_ARGP, current_argp_adjustment);
	  bool spilled       = reduce_spill(tree, burm_goal_NT);
	  icg_insn2dirty[id] = adjusted_fp || adjusted_argp || spilled;
	  if (icg_insn2dirty[id])
	    all_done = false;
	}
      }
    }
  }
    
  if (dump_file)
    fprintf(dump_file, "spilling: finished}\n");
  return all_done;
}
