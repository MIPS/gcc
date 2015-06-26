/* HSAIL IL Register allocation and out-of-SSA.
   Copyright (C) 2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "is-a.h"
#include "defaults.h"
#include "hard-reg-set.h"
#include "hash-set.h"
#include "vec.h"
#include "symtab.h"
#include "input.h"
#include "alias.h"
#include "double-int.h"
#include "inchash.h"
#include "tree.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "dominance.h"
#include "cfg.h"
#include "function.h"
#include "predict.h"
#include "basic-block.h"
#include "fold-const.h"
#include "gimple.h"
#include "bitmap.h"
#include "dumpfile.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "cfganal.h"
#include "hsa.h"


/* Process a PHI node PHI of basic block BB as a part of naive out-f-ssa.  */

static void
naive_process_phi (hsa_insn_phi *phi)
{
  unsigned count = phi->operands.length ();
  for (unsigned i = 0; i < count; i++)
    {
      hsa_op_base *op = phi->operands[i];
      hsa_bb *hbb;
      edge e;

      if (!op)
	break;

      e = EDGE_PRED (phi->bb, i);
      if (single_succ_p (e->src))
	hbb = hsa_bb_for_bb (e->src);
      else
	hbb = hsa_init_new_bb (split_edge (e));

      hsa_build_append_simple_mov (phi->dest, op, hbb);
    }
}

/* Naive out-of SSA.  */

static void
naive_outof_ssa (void)
{
  basic_block bb;

  hsa_cfun->in_ssa = false;

  FOR_EACH_BB_FN (bb, cfun)
  {
    hsa_bb *hbb = hsa_bb_for_bb (bb);
    hsa_insn_phi *phi;

    for (phi = hbb->first_phi;
	 phi;
	 phi = phi->next ? as_a <hsa_insn_phi *> (phi->next): NULL)
      naive_process_phi (phi);

    /* Zap PHI nodes, they will be deallocated when everything else will.  */
    hbb->first_phi = NULL;
    hbb->last_phi = NULL;
  }
}

/* Return register class number for the given HSA TYPE.  0 means the 'c' one
   bit register clas, 1 means 's' 32 bit class, 2 stands for 'd' 64 bit class
   and 3 for 'q' 128 bit class.  */

static int
reg_class_for_type (BrigType16_t type)
{
  switch (type)
    {
    case BRIG_TYPE_B1:
      return 0;

    case BRIG_TYPE_U8:
    case BRIG_TYPE_U16:
    case BRIG_TYPE_U32:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_F16:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_B8:
    case BRIG_TYPE_B16:
    case BRIG_TYPE_B32:
    case BRIG_TYPE_U8X4:
    case BRIG_TYPE_S8X4:
    case BRIG_TYPE_U16X2:
    case BRIG_TYPE_S16X2:
    case BRIG_TYPE_F16X2:
      return 1;

    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
    case BRIG_TYPE_F64:
    case BRIG_TYPE_B64:
    case BRIG_TYPE_U8X8:
    case BRIG_TYPE_S8X8:
    case BRIG_TYPE_U16X4:
    case BRIG_TYPE_S16X4:
    case BRIG_TYPE_F16X4:
    case BRIG_TYPE_U32X2:
    case BRIG_TYPE_S32X2:
    case BRIG_TYPE_F32X2:
      return 2;

    case BRIG_TYPE_B128:
    case BRIG_TYPE_U8X16:
    case BRIG_TYPE_S8X16:
    case BRIG_TYPE_U16X8:
    case BRIG_TYPE_S16X8:
    case BRIG_TYPE_F16X8:
    case BRIG_TYPE_U32X4:
    case BRIG_TYPE_U64X2:
    case BRIG_TYPE_S32X4:
    case BRIG_TYPE_S64X2:
    case BRIG_TYPE_F32X4:
    case BRIG_TYPE_F64X2:
      return 3;

    default:
      gcc_unreachable ();
    }
}

/* If the Ith operands of INSN is or contains a register (in an address),
   return the address of that register operand.  If not return NULL.  */

static hsa_op_reg **
insn_reg_addr (hsa_insn_basic *insn, int i)
{
  hsa_op_base *op = insn->operands[i];
  if (!op)
    return NULL;
  hsa_op_reg *reg = dyn_cast <hsa_op_reg *> (op);
  if (reg)
    return (hsa_op_reg **) &insn->operands[i];
  hsa_op_address *addr = dyn_cast <hsa_op_address *> (op);
  if (addr && addr->reg)
    return &addr->reg;
  return NULL;
}

struct reg_class_desc
{
  unsigned next_avail, max_num;
  unsigned used_num, max_used;
  uint64_t used[2];
  char cl_char;
};

/* Rewrite the instructions in BB to observe spilled live ranges.
   CLASSES is the global register class state.  */

static void
rewrite_code_bb (basic_block bb, struct reg_class_desc *classes)
{
  hsa_bb *hbb = hsa_bb_for_bb (bb);
  hsa_insn_basic *insn, *next_insn;

  for (insn = hbb->first_insn; insn; insn = next_insn)
    {
      next_insn = insn->next;
      unsigned count = insn->operands.length ();
      for (unsigned i = 0; i < count; i++)
	{
	  gcc_checking_assert (insn->operands[i]);
	  hsa_op_reg **regaddr = insn_reg_addr (insn, i);

	  if (regaddr)
	    {
	      hsa_op_reg *reg = *regaddr;
	      if (reg->reg_class)
		continue;
	      gcc_assert (reg->spill_sym);

	      int cl = reg_class_for_type (reg->type);
	      hsa_op_reg *tmp, *tmp2;
	      if (hsa_opcode_op_output_p (insn->opcode, i))
		tmp = hsa_spill_out (insn, reg, &tmp2);
	      else
		tmp = hsa_spill_in (insn, reg, &tmp2);

	      *regaddr = tmp;

	      tmp->reg_class = classes[cl].cl_char;
	      tmp->hard_num = (char) (classes[cl].max_num + i);
	      if (tmp2)
		{
		  gcc_assert (cl == 0);
		  tmp2->reg_class = classes[1].cl_char;
		  tmp2->hard_num = (char) (classes[1].max_num + i);
		}
	    }
	}
    }
}

/* Return the number of destination operands for this INSN.  */

static int
hsa_num_def_ops (hsa_insn_basic *insn)
{
  switch (insn->opcode)
    {
      default:
	return 1;

      case BRIG_OPCODE_NOP:
      case HSA_OPCODE_CALL_BLOCK:
	return 0;

      case BRIG_OPCODE_EXPAND:
	sorry ("HSA expand opcode not handled");
	return 1;

      case BRIG_OPCODE_LD:
	/* ld_v[234] not yet handled.  */
	return 1;

      case BRIG_OPCODE_ST:
	return 0;

      case BRIG_OPCODE_ATOMICNORET:
	return 0;

      case BRIG_OPCODE_SIGNAL:
	return 1;
      case BRIG_OPCODE_SIGNALNORET:
	return 0;

      case BRIG_OPCODE_MEMFENCE:
	return 0;

      case BRIG_OPCODE_RDIMAGE:
      case BRIG_OPCODE_LDIMAGE:
      case BRIG_OPCODE_STIMAGE:
      case BRIG_OPCODE_QUERYIMAGE:
      case BRIG_OPCODE_QUERYSAMPLER:
	sorry ("HSA image ops not handled");
	return 0;

      case BRIG_OPCODE_CBR:
      case BRIG_OPCODE_BR:
	return 0;

      case BRIG_OPCODE_SBR:
	return 0; /* ??? */

      case BRIG_OPCODE_WAVEBARRIER:
	return 0; /* ??? */

      case BRIG_OPCODE_BARRIER:
      case BRIG_OPCODE_ARRIVEFBAR:
      case BRIG_OPCODE_INITFBAR:
      case BRIG_OPCODE_JOINFBAR:
      case BRIG_OPCODE_LEAVEFBAR:
      case BRIG_OPCODE_RELEASEFBAR:
      case BRIG_OPCODE_WAITFBAR:
	return 0;

      case BRIG_OPCODE_LDF:
	return 1;

      case BRIG_OPCODE_ACTIVELANECOUNT:
      case BRIG_OPCODE_ACTIVELANEID:
      case BRIG_OPCODE_ACTIVELANEMASK:
      case BRIG_OPCODE_ACTIVELANEPERMUTE:
	return 1; /* ??? */

      case BRIG_OPCODE_CALL:
      case BRIG_OPCODE_SCALL:
      case BRIG_OPCODE_ICALL:
	return 1; /* ??? */

      case BRIG_OPCODE_RET:
	return 0;

      case BRIG_OPCODE_ALLOCA:
	return 1;

      case BRIG_OPCODE_CLEARDETECTEXCEPT:
	return 0;

      case BRIG_OPCODE_SETDETECTEXCEPT:
	return 0;

      case BRIG_OPCODE_PACKETCOMPLETIONSIG:
      case BRIG_OPCODE_PACKETID:
      case BRIG_OPCODE_ADDQUEUEWRITEINDEX:
      case BRIG_OPCODE_CASQUEUEWRITEINDEX:
      case BRIG_OPCODE_LDQUEUEREADINDEX:
      case BRIG_OPCODE_LDQUEUEWRITEINDEX:
      case BRIG_OPCODE_STQUEUEREADINDEX:
      case BRIG_OPCODE_STQUEUEWRITEINDEX:
	return 1; /* ??? */

      case BRIG_OPCODE_DEBUGTRAP:
	return 0;

      case BRIG_OPCODE_GROUPBASEPTR:
      case BRIG_OPCODE_KERNARGBASEPTR:
	return 1; /* ??? */
    }
}

/* Dump current function to dump file F, with info specific
   to register allocation.  */

void
dump_hsa_cfun_regalloc (FILE *f)
{
  basic_block bb;

  fprintf (f, "\nHSAIL IL for %s\n", hsa_cfun->name);

  dump_hsa_bb (f, &hsa_cfun->prologue);
  FOR_EACH_BB_FN (bb, cfun)
  {
    hsa_bb *hbb = (struct hsa_bb *) bb->aux;
    bitmap_print (dump_file, hbb->livein, "livein  ", "\n");
    dump_hsa_bb (f, hbb);
    bitmap_print (dump_file, hbb->liveout, "liveout ", "\n");
  }
}

/* Given the global register allocation state CLASSES and a
   register REG, try to give it a hardware register.  If successful,
   store that hardreg in REG and return it, otherwise return -1.
   Also changes CLASSES to accomodate for the allocated register.  */

static int
try_alloc_reg (struct reg_class_desc *classes, hsa_op_reg *reg)
{
  int cl = reg_class_for_type (reg->type);
  int ret = -1;
  if (classes[1].used_num + classes[2].used_num * 2 + classes[3].used_num * 4
      >= 128 - 5)
    return -1;
  if (classes[cl].used_num < classes[cl].max_num)
    {
      unsigned int i;
      classes[cl].used_num++;
      if (classes[cl].used_num > classes[cl].max_used)
	classes[cl].max_used = classes[cl].used_num;
      for (i = 0; i < classes[cl].used_num; i++)
	if (! (classes[cl].used[i / 64] & (((uint64_t)1) << (i & 63))))
	  break;
      ret = i;
      classes[cl].used[i / 64] |= (((uint64_t)1) << (i & 63));
      reg->reg_class = classes[cl].cl_char;
      reg->hard_num = i;
    }
  return ret;
}

/* Free up hardregs used by REG, into allocation state CLASSES.  */

static void
free_reg (struct reg_class_desc *classes, hsa_op_reg *reg)
{
  int cl = reg_class_for_type (reg->type);
  int ret = reg->hard_num;
  gcc_assert (reg->reg_class == classes[cl].cl_char);
  classes[cl].used_num--;
  classes[cl].used[ret / 64] &= ~(((uint64_t)1) << (ret & 63));
}

/* Note that the live range for REG ends at least at END.  */

static void
note_lr_end (hsa_op_reg *reg, int end)
{
  if (reg->lr_end < end)
    reg->lr_end = end;
}

/* Note that the live range for REG starts at least at BEGIN.  */

static void
note_lr_begin (hsa_op_reg *reg, int begin)
{
  if (reg->lr_begin > begin)
    reg->lr_begin = begin;
}

/* Given two registers A and B, return -1, 0 or 1 if A's live range
   starts before, at or after B's live range.  */

static int
cmp_begin (const void *a, const void *b)
{
  const hsa_op_reg * const *rega = (const hsa_op_reg * const *)a;
  const hsa_op_reg * const *regb = (const hsa_op_reg * const *)b;
  int ret;
  if (rega == regb)
    return 0;
  ret = (*rega)->lr_begin - (*regb)->lr_begin;
  if (ret)
    return ret;
  return ((*rega)->order - (*regb)->order);
}

/* Given two registers REGA and REGB, return true if REGA's
   live range ends after REGB's.  This results in a sorting order
   with earlier end points at the end.  */

static bool
cmp_end (hsa_op_reg * const &rega, hsa_op_reg * const &regb)
{
  int ret;
  if (rega == regb)
    return false;
  ret = (regb)->lr_end - (rega)->lr_end;
  if (ret)
    return ret < 0;
  return (((regb)->order - (rega)->order)) < 0;
}

/* Expire all old intervals in ACTIVE (a per-regclass vector),
   that is, those that end before the interval REG starts.  Give
   back resources freed so into the state CLASSES.  */

static void
expire_old_intervals (hsa_op_reg *reg, vec<hsa_op_reg*> *active,
		      struct reg_class_desc *classes)
{
  for (int i = 0; i < 4; i++)
    while (!active[i].is_empty ())
      {
	hsa_op_reg *a = active[i].pop ();
	if (a->lr_end > reg->lr_begin)
	  {
	    active[i].quick_push (a);
	    break;
	  }
	free_reg (classes, a);
      }
}

/* The interval REG didn't get a hardreg.  Spill it or one of those
   from ACTIVE (if the latter, then REG will become allocated to the
   hardreg that formerly was used by it).  */

static void
spill_at_interval (hsa_op_reg *reg, vec<hsa_op_reg*> *active)
{
  int cl = reg_class_for_type (reg->type);
  gcc_assert (!active[cl].is_empty ());
  hsa_op_reg *cand = active[cl][0];
  if (cand->lr_end > reg->lr_end)
    {
      reg->reg_class = cand->reg_class;
      reg->hard_num = cand->hard_num;
      active[cl].ordered_remove (0);
      unsigned place = active[cl].lower_bound (reg, cmp_end);
      active[cl].quick_insert (place, reg);
    }
  else
    cand = reg;

  gcc_assert (!cand->spill_sym);
  BrigType16_t type = cand->type;
  if (type == BRIG_TYPE_B1)
    type = BRIG_TYPE_U8;
  cand->reg_class = 0;
  cand->spill_sym = hsa_get_spill_symbol (type);
  cand->spill_sym->name_number = cand->order;
}

/* Visit instruction INSN and fill in vector IND2REG if the instruction
   contains a register (in memory).  Number all insns by a global counter,
   passed by INSN_ORDER argument.  */

static void
visit_insn (hsa_insn_basic *insn, vec<hsa_op_reg*> &ind2reg, int &insn_order)
{
  unsigned opi;
  unsigned count = insn->operands.length ();
  insn->number = insn_order++;
  for (opi = 0; opi < count; opi++)
    {
      gcc_checking_assert (insn->operands[opi]);
      hsa_op_reg **regaddr = insn_reg_addr (insn, opi);
      if (regaddr)
	ind2reg[(*regaddr)->order] = *regaddr;
    }
}

/* Remove definition in INSN according to bitmap WORK list.  */

static void
remove_def_in_insn (bitmap &work, hsa_insn_basic *insn)
{
  unsigned opi;
  unsigned ndefs = hsa_num_def_ops (insn);
  for (opi = 0; opi < ndefs && insn->operands[opi]; opi++)
    {
      gcc_checking_assert (insn->operands[opi]);
      hsa_op_reg **regaddr = insn_reg_addr (insn, opi);
      if (regaddr)
	bitmap_clear_bit (work, (*regaddr)->order);
    }
  unsigned count = insn->operands.length ();
  for (; opi < count; opi++)
    {
      gcc_checking_assert (insn->operands[opi]);
      hsa_op_reg **regaddr = insn_reg_addr (insn, opi);
      if (regaddr)
	bitmap_set_bit (work, (*regaddr)->order);
    }
}

/* Merge live range for an instruction INSN.  */

static void
merge_live_range_for_insn (hsa_insn_basic *insn)
{
  unsigned opi;
  unsigned ndefs = hsa_num_def_ops (insn);
  unsigned count = insn->operands.length ();
  for (opi = 0; opi < count; opi++)
    {
      gcc_checking_assert (insn->operands[opi]);
      hsa_op_reg **regaddr = insn_reg_addr (insn, opi);
      if (regaddr)
	{
	  hsa_op_reg *reg = *regaddr;
	  if (opi < ndefs)
	    note_lr_begin (reg, insn->number);
	  else
	    note_lr_end (reg, insn->number);
	}
    }
}

/* Given the global register state CLASSES allocate all HSA virtual
   registers either to hardregs or to a spill symbol.  */

static void
linear_scan_regalloc (struct reg_class_desc *classes)
{
  /* Compute liveness.  */
  bool changed;
  int i, n;
  int insn_order;
  int *bbs = XNEWVEC (int, n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS);
  bitmap work = BITMAP_ALLOC (NULL);
  vec<hsa_op_reg*> ind2reg = vNULL;
  vec<hsa_op_reg*> active[4] = {vNULL, vNULL, vNULL, vNULL};
  hsa_insn_basic *last_insn;

  /* We will need the reverse post order for linearization,
     and the post order for liveness analysis, which is the same
     backward.  */
  n = pre_and_rev_post_order_compute (NULL, bbs, false);
  ind2reg.safe_grow_cleared (hsa_cfun->reg_count);

  /* Give all instructions a linearized number, at the same time
     build a mapping from register index to register.  */
  insn_order = 1;
  for (i = 0; i < n; i++)
    {
      basic_block bb = BASIC_BLOCK_FOR_FN (cfun, bbs[i]);
      hsa_bb *hbb = hsa_bb_for_bb (bb);
      hsa_insn_basic *insn;
      for (insn = hbb->first_insn; insn; insn = insn->next)
	{
	  visit_insn (insn, ind2reg, insn_order);

	  if (is_a <hsa_insn_call_block *> (insn))
	    {
	      /* HSA call block insn contains insns that are used for passing
		 arguments and getting a return value, if returned.  */
	      hsa_insn_call_block *call = dyn_cast <hsa_insn_call_block *>
		(insn);
	      for (unsigned j = 0; j < call->input_arg_insns.length (); j++)
		visit_insn (call->input_arg_insns[j], ind2reg, insn_order);
	    }
	}
    }

  /* Initialize all live ranges to [after-end, 0).  */
  for (i = 0; i < hsa_cfun->reg_count; i++)
    if (ind2reg[i])
      ind2reg[i]->lr_begin = insn_order, ind2reg[i]->lr_end = 0;

  /* Classic liveness analysis, as long as something changes:
       liveout is union (livein of successors)
       livein is liveout minus defs plus uses.  */
  do
    {
      changed = false;
      for (i = n - 1; i >= 0; i--)
	{
	  edge e;
	  edge_iterator ei;
	  basic_block bb = BASIC_BLOCK_FOR_FN (cfun, bbs[i]);
	  hsa_bb *hbb = hsa_bb_for_bb (bb);

	  /* Union of successors livein (or empty if none).  */
	  bool first = true;
	  FOR_EACH_EDGE (e, ei, bb->succs)
	    if (e->dest != EXIT_BLOCK_PTR_FOR_FN (cfun))
	      {
		hsa_bb *succ = hsa_bb_for_bb (e->dest);
		if (first)
		  {
		    bitmap_copy (work, succ->livein);
		    first = false;
		  }
		else
		  bitmap_ior_into (work, succ->livein);
	      }
	  if (first)
	    bitmap_clear (work);

	  bitmap_copy (hbb->liveout, work);

	  /* Remove defs, include uses in a backward insn walk.  */
	  hsa_insn_basic *insn;
	  for (insn = hbb->last_insn; insn; insn = insn->prev)
	    {
	      remove_def_in_insn (work, insn);
	      if (is_a <hsa_insn_call_block *> (insn))
		{
		  /* HSA call block insn contains insns that are used for
		     passing arguments and getting a return value,
		     if returned.  */
		  hsa_insn_call_block *call = dyn_cast <hsa_insn_call_block *>
		    (insn);

		  for (int j = call->input_arg_insns.length () - 1; j >= 0; j--)
		    remove_def_in_insn (work, call->input_arg_insns[j]);
		}
	    }

	  /* Note if that changed something.  */
	  if (bitmap_ior_into (hbb->livein, work))
	    changed = true;
	}
    }
  while (changed);

  /* Make one pass through all instructions in linear order,
     noting and merging possible live range start and end points.  */
  last_insn = NULL;
  for (i = n - 1; i >= 0; i--)
    {
      basic_block bb = BASIC_BLOCK_FOR_FN (cfun, bbs[i]);
      hsa_bb *hbb = hsa_bb_for_bb (bb);
      hsa_insn_basic *insn;
      int after_end_number;
      unsigned bit;
      bitmap_iterator bi;

      if (last_insn)
	after_end_number = last_insn->number;
      else
	after_end_number = insn_order;
      /* Everything live-out in this BB has at least an end point
         after us. */
      EXECUTE_IF_SET_IN_BITMAP (hbb->liveout, 0, bit, bi)
	note_lr_end (ind2reg[bit], after_end_number);

      for (insn = hbb->last_insn; insn; insn = insn->prev)
	{
	  merge_live_range_for_insn (insn);

	  if (is_a <hsa_insn_call_block *> (insn))
	    {
	      /* HSA call block insn contains insns that are used for passing
		 arguments and getting a return value, if returned.  */
	      hsa_insn_call_block *call = dyn_cast <hsa_insn_call_block *>
		(insn);

	      for (int j = call->input_arg_insns.length () - 1; j >= 0; j--)
		merge_live_range_for_insn (call->input_arg_insns[j]);
	    }
	}
      if (hbb->first_insn)
	last_insn = hbb->first_insn;
    }

  /* All regs that have still their start at after all code actually
     are defined at the start of the routine (prologue).  */
  for (i = 0; i < hsa_cfun->reg_count; i++)
    if (ind2reg[i] && ind2reg[i]->lr_begin == insn_order)
      ind2reg[i]->lr_begin = 0;

  /* Sort all intervals by increasing start point.  */
  gcc_assert (ind2reg.length () == (size_t) hsa_cfun->reg_count);
  ind2reg.qsort (cmp_begin);
  for (i = 0; i < 4; i++)
    active[i].reserve_exact (hsa_cfun->reg_count);

  /* Now comes the linear scan allocation.  */
  for (i = 0; i < hsa_cfun->reg_count; i++)
    {
      hsa_op_reg *reg = ind2reg[i];
      if (!reg)
	continue;
      expire_old_intervals (reg, active, classes);
      int cl = reg_class_for_type (reg->type);
      if (try_alloc_reg (classes, reg) >= 0)
	{
	  unsigned place = active[cl].lower_bound (reg, cmp_end);
	  active[cl].quick_insert (place, reg);
	}
      else
	spill_at_interval (reg, active);

      /* Some interesting dumping as we go.  */
      if (dump_file)
	{
	  fprintf (dump_file, "  reg%d: [%5d, %5d)->",
		   reg->order, reg->lr_begin, reg->lr_end);
	  if (reg->reg_class)
	    fprintf (dump_file, "$%c%i", reg->reg_class, reg->hard_num);
	  else
	    fprintf (dump_file, "[%%__%s_%i]",
		     hsa_seg_name(reg->spill_sym->segment),
		     reg->spill_sym->name_number);
	  for (int cl = 0; cl < 4; cl++)
	    {
	      bool first = true;
	      hsa_op_reg *r;
	      fprintf (dump_file, " {");
	      for (int j = 0; active[cl].iterate (j, &r); j++)
		if (first)
		  {
		    fprintf (dump_file, "%d", r->order);
		    first = false;
		  }
		else
		  fprintf (dump_file, ", %d", r->order);
	      fprintf (dump_file, "}");
	    }
	  fprintf (dump_file, "\n");
	}
    }

  BITMAP_FREE (work);
  free (bbs);

  if (dump_file)
    {
      fprintf (dump_file, "------- After liveness: -------\n");
      dump_hsa_cfun_regalloc (dump_file);
      fprintf (dump_file, "  ----- Intervals:\n");
      for (i = 0; i < hsa_cfun->reg_count; i++)
	{
	  hsa_op_reg *reg = ind2reg[i];
	  if (!reg)
	    continue;
	  fprintf (dump_file, "  reg%d: [%5d, %5d)->", reg->order, reg->lr_begin, reg->lr_end);
	  if (reg->reg_class)
	    fprintf (dump_file, "$%c%i\n", reg->reg_class, reg->hard_num);
	  else
	    fprintf (dump_file, "[%%__%s_%i]\n",
		     hsa_seg_name(reg->spill_sym->segment),
		     reg->spill_sym->name_number);
	}
    }

  for (i = 0; i < 4; i++)
    active[i].release ();
  ind2reg.release ();
}

/* Entry point for register allocation.  */

static void
regalloc (void)
{
  basic_block bb;
  reg_class_desc classes[4];

  /* If there are no registers used in the function, exit right away. */
  if (hsa_cfun->reg_count == 0)
    return;

  memset (classes, 0, sizeof (classes));
  classes[0].next_avail = 0;
  classes[0].max_num = 7;
  classes[0].cl_char = 'c';
  classes[1].cl_char = 's';
  classes[2].cl_char = 'd';
  classes[3].cl_char = 'q';

  for (int i = 1; i < 4; i++)
    {
      classes[i].next_avail = 0;
      classes[i].max_num = 20;
    }

  linear_scan_regalloc (classes);

  rewrite_code_bb (ENTRY_BLOCK_PTR_FOR_FN (cfun), classes);
  FOR_EACH_BB_FN (bb, cfun)
    rewrite_code_bb (bb, classes);
}

/* Out of SSA and register allocation on HSAIL IL.  */

void
hsa_regalloc (void)
{
  naive_outof_ssa ();

  if (dump_file)
    {
      fprintf (dump_file, "------- After out-of-SSA: -------\n");
      dump_hsa_cfun (dump_file);
    }

  regalloc ();

  if (dump_file)
    {
      fprintf (dump_file, "------- After register allocation: -------\n");
      dump_hsa_cfun (dump_file);
    }
}
