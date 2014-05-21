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
#include "hsa.h"
#include "tm.h"
#include "tree.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "gimple.h"
#include "basic-block.h"
#include "vec.h"
#include "dumpfile.h"
#include "gimple-pretty-print.h"


/* Process a PHI node PHI of basic block BB as a part of naive out-f-ssa.  */

static void
naive_process_phi (hsa_insn_phi *phi)
{
  for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
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

  hsa_cfun.in_ssa = false;

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

struct reg_class_desc
{
  int next_avail, max_num;
  char cl_char;
};

/* Do stupid register allocation in the basic block BB.  CLASSES is array of
   four stupid global register class states.  */

static void
stupid_regalloc_bb (basic_block bb, struct reg_class_desc *classes)
{
  hsa_bb *hbb = hsa_bb_for_bb (bb);
  hsa_insn_basic *insn, *next_insn;

  for (insn = hbb->first_insn; insn; insn = next_insn)
    {
      next_insn = insn->next;
      for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
	{
	  hsa_op_base *op = insn->operands[i];
	  hsa_op_reg **regaddr = NULL;
	  hsa_op_reg *reg;

	  if (!op)
	    break;
	  reg = dyn_cast <hsa_op_reg *> (op);
	  if (!reg)
	    {
	      hsa_op_address *addr = dyn_cast <hsa_op_address *> (op);
	      if (addr && addr->reg)
		{
		  reg = addr->reg;
		  regaddr = &addr->reg;
		}
	      else
		continue;
	    }
	  if (reg->reg_class)
	    continue;

	  int cl = reg_class_for_type (reg->type);
	  if (!reg->spill_sym)
	    {
	      if (classes[cl].next_avail < classes[cl].max_num)
		{
		  reg->reg_class = classes[cl].cl_char;
		  reg->hard_num = (char) classes[cl].next_avail++;
		}
	      else
		{
		  BrigType16_t type = reg->type;
		  if (type == BRIG_TYPE_B1)
		    type = BRIG_TYPE_U8;
		  reg->spill_sym = hsa_get_spill_symbol (type);
		  reg->spill_sym->name_number = reg->order;
		}
	    }

	  if (reg->spill_sym)
	    {
	      hsa_op_reg *tmp, *tmp2;
	      if (hsa_opcode_op_output_p (insn->opcode, i))
		tmp = hsa_spill_out (insn, reg, &tmp2);
	      else
		tmp = hsa_spill_in (insn, reg, &tmp2);

	      if (regaddr)
		*regaddr = tmp;
	      else
		insn->operands[i] = tmp;

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

/* Stupid register allocation.  */

static void
stupid_regalloc (void)
{
  basic_block bb;
  reg_class_desc classes[4];

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

  stupid_regalloc_bb (ENTRY_BLOCK_PTR_FOR_FN (cfun), classes);
  FOR_EACH_BB_FN (bb, cfun)
    stupid_regalloc_bb (bb, classes);
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

  stupid_regalloc ();

  if (dump_file)
    {
      fprintf (dump_file, "------- After register allocation: -------\n");
      dump_hsa_cfun (dump_file);
    }
}
