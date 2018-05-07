/* Subroutines used for MIPS register recosting.
   Copyright (C) 2018 Free Software Foundation, Inc.

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

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "stringpool.h"
#include "attribs.h"
#include "optabs.h"
#include "regs.h"
#include "emit-rtl.h"
#include "recog.h"
#include "insn-attr.h"
#include "tm-constrs.h"
#include "ira.h"
#include "ira-int.h"

/* Post IRA recolor data and functions.  */

static bool
mips_check_16b_add_imm (rtx reg_op, rtx const_op)
{
  if (!CONST_INT_P (const_op))
    gcc_unreachable ();

  if (addiur2_operand (const_op, GET_MODE (const_op))
      || sb4_operand (const_op, GET_MODE (const_op)))
    return true;

  if (((REGNO (reg_op) == STACK_POINTER_REGNUM)
       || (REGNO (reg_op) == ARG_POINTER_REGNUM)
       || (REGNO (reg_op) == FRAME_POINTER_REGNUM))
      && addiusp_operand (const_op, GET_MODE (const_op)))
    return true;

  return false;
}

static attr_subset_16bit
mips_get_subset_mem (rtx mem_op, machine_mode mode, bool load)
{
  struct mips_address_info addr;

  if (!MEM_P (mem_op))
    gcc_unreachable ();

  if (!mips_classify_address (&addr, XEXP (mem_op, 0), GET_MODE (mem_op),
			      false))
    return SUBSET_16BIT_NO;

  if (addr.type == ADDRESS_REG_REG)
    return SUBSET_16BIT_STD;

  if (addr.type != ADDRESS_REG)
    return SUBSET_16BIT_NO;

  if (HARD_REGISTER_P (addr.reg))
    {
      switch (GET_MODE_SIZE (mode))
	{
	  case 4:
	    if (lwsp_swsp_operand (mem_op, GET_MODE (mem_op)))
	      return SUBSET_16BIT_NO;
	    else if (lw4x4_sw4x4_operand (mem_op, GET_MODE (mem_op)))
	      return SUBSET_16BIT_4X4;
	    else if (lw16_sw16_operand (mem_op, GET_MODE (mem_op)))
	      return SUBSET_16BIT_STD;
	    return SUBSET_16BIT_NO;
	  case 2:
	    return lhu16_sh16_operand (mem_op, GET_MODE (mem_op))
		     ? SUBSET_16BIT_STD : SUBSET_16BIT_NO;
	  case 1:
	    if (load)
	      return lbu16_operand (mem_op, GET_MODE (mem_op))
		       ? SUBSET_16BIT_STD : SUBSET_16BIT_NO;
	    else
	      return sb16_operand (mem_op, GET_MODE (mem_op))
		       ? SUBSET_16BIT_STORE : SUBSET_16BIT_NO;
	  default:
	    return SUBSET_16BIT_NO;
	}
    }
  else
    {
      switch (GET_MODE_SIZE (mode))
	{
	  case 4:
	    if ((REGNO (addr.reg) == ARG_POINTER_REGNUM)
		|| (REGNO (addr.reg) == FRAME_POINTER_REGNUM))
	      return SUBSET_16BIT_NO;
	    else if (uw2_operand (addr.offset, GET_MODE (mem_op)))
	      return SUBSET_16BIT_4X4;
	    else
	      return uw4_operand (addr.offset, GET_MODE (mem_op))
		       ? SUBSET_16BIT_STD : SUBSET_16BIT_NO;
	  case 2:
	      return uh2_operand (addr.offset, GET_MODE (mem_op))
		       ? SUBSET_16BIT_STD : SUBSET_16BIT_NO;
	  case 1:
	      if (load)
		return ub2_operand (addr.offset, GET_MODE (mem_op))
			 ? SUBSET_16BIT_STD : SUBSET_16BIT_NO;
	      else
		return ub2_operand (addr.offset, GET_MODE (mem_op))
			 ? SUBSET_16BIT_STORE : SUBSET_16BIT_NO;
	  default:
	    return SUBSET_16BIT_NO;
	}
    }
  return SUBSET_16BIT_NO;
}

static attr_subset_16bit
check_attr_subset_16bit (attr_subset_16bit attr, rtx_insn *insn)
{
  switch (attr)
    {
      case SUBSET_16BIT_SUB_LOAD:
	extract_insn (insn);
	return mips_get_subset_mem (recog_data.operand[1],
				   GET_MODE (recog_data.operand[1]), true);
      case SUBSET_16BIT_SUB_STORE:
	extract_insn (insn);
	return mips_get_subset_mem (recog_data.operand[0],
				   GET_MODE (recog_data.operand[0]), false);
      default:
	return attr;
    }
}

static bool
mips_check_16b_mem (rtx mem_op, machine_mode mode, bool load)
{
  struct mips_address_info addr;

  if (!MEM_P (mem_op))
    gcc_unreachable ();

  if (!mips_classify_address (&addr, XEXP (mem_op, 0), GET_MODE (mem_op),
			      false))
    return false;

  if (addr.type == ADDRESS_REG_REG && (GET_MODE_SIZE (mode) == 4))
    return true;

  if (addr.type != ADDRESS_REG)
    return false;

  if (HARD_REGISTER_P (addr.reg))
    {
      switch (GET_MODE_SIZE (mode))
	{
	  case 4:
	    if (lwsp_swsp_operand (mem_op, GET_MODE (mem_op)))
	      return false;
	    return lw16_sw16_operand (mem_op, GET_MODE (mem_op));
	  case 2:
	    return lhu16_sh16_operand (mem_op, GET_MODE (mem_op));
	  case 1:
	    if (load)
	      return lbu16_operand (mem_op, GET_MODE (mem_op));
	    else
	      return sb16_operand (mem_op, GET_MODE (mem_op));
	  default:
	    return false;
	}
    }
  else
    {
      switch (GET_MODE_SIZE (mode))
	{
	  case 4:
	    if ((REGNO (addr.reg) == ARG_POINTER_REGNUM)
		|| (REGNO (addr.reg) == FRAME_POINTER_REGNUM))
	      return false;
	    else
	      return uw4_operand (addr.offset, GET_MODE (mem_op));
	  case 2:
	      return uh2_operand (addr.offset, GET_MODE (mem_op));
	  case 1:
	      return ub2_operand (addr.offset, GET_MODE (mem_op));
	  default:
	    return false;
	}
    }
  return false;
}

/* Returns true if given register is short.  */

static bool mips_short_reg_p (int reg_no)
{
  return M16_REG_P (reg_no);
}

/* Returns true if all assigned register operands are short.  */

static bool
mips_check_regs (rtx *opnds, int regs_num)
{
  for (int i = 0; i < regs_num; i++)
    {
      rtx reg = opnds[i];
      if (GET_CODE (reg) == SUBREG)
	reg = SUBREG_REG (reg);
      if (!REG_P (reg))
	continue;
      if (HARD_REGISTER_P (reg) && !mips_short_reg_p (REGNO (reg)))
	return false;
    }
  return true;
}

/* Returns true if given nanoMIPS instruction has 16-bit equivalent.  */

static bool
mips_insn_16bit_ver_p (rtx_insn *insn)
{
  attr_has_16bit_ver has_16bit_ver;
  int imm_val;

  if (!INSN_P (insn) || DEBUG_INSN_P (insn))
    return false;

  if (GET_CODE (PATTERN (insn)) == USE)
    return false;

  if (GET_CODE (PATTERN (insn)) == CLOBBER)
    return false;

  has_16bit_ver = get_attr_has_16bit_ver (insn);

  extract_insn (insn);

  if (get_attr_enabled (insn) == ENABLED_NO)
    return false;

  switch (has_16bit_ver)
    {
      case HAS_16BIT_VER_YES:
	return true;
      case HAS_16BIT_VER_RR:
	return mips_check_regs (recog_data.operand, 2);
      case HAS_16BIT_VER_RRR:
	return mips_check_regs (recog_data.operand, 3);
      case HAS_16BIT_VER_RI_LI:
	imm_val = INTVAL (recog_data.operand[1]);
	if (imm_val >= -1 && imm_val <= 126)
	  return true;
	return false;
      case HAS_16BIT_VER_RRI_LOAD:
	return mips_check_16b_mem (recog_data.operand[1],
				   GET_MODE (recog_data.operand[1]), true);
      case HAS_16BIT_VER_RRI_STORE:
	return mips_check_16b_mem (recog_data.operand[0],
				   GET_MODE (recog_data.operand[0]), false);
      case HAS_16BIT_VER_RRI_AND:
	return andi16_operand (recog_data.operand[2],
			       GET_MODE (recog_data.operand[2]));
      case HAS_16BIT_VER_RRI_SLL:
	return ib3_operand (recog_data.operand[2],
			    GET_MODE (recog_data.operand[2]));
      case HAS_16BIT_VER_RRI_BEQZC:
      case HAS_16BIT_VER_RRI_BEQC:
	return true;
      case HAS_16BIT_VER_RRI_ADD:
	return mips_check_16b_add_imm (recog_data.operand[1],
				       recog_data.operand[2]);
      default:
	return false;
    }
}

/* Number of instruction categories.  */
#define INSN_CATEGORIES_NUM 6

/* Maximum number of instruction operands.  */
#define MAX_OPERAND_NUM 3

/* Short register which can be used in 16bit instructions.  */
#define RECOLOR_TYPE_SHORT 0
/* Long register which cannot be used in 16bit instructions.  */
#define RECOLOR_TYPE_LONG  1

/* Recolor data associated with every allocno.  */
struct recolor_allocno_data {
  /* Related allocno.  */
  int allocno;
  /* Short or long register.  */
  int type;
  /* Rating associated with allocno.  */
  int rating;
  /* Cumulative rating associated with allocno.  */
  int crating;
  /* Number of instructions with this allocno in every category.  */
  int categories[INSN_CATEGORIES_NUM];
  /* Register class prefered by register.  */
  enum reg_class prefered_class;
  /* Mark allocno as processed to terminate recursion.  */
  bool processed;
  /* Mark allocno as recolored to terminate recursion.  */
  bool recolored;
  /* Values for which register costs are modified.  */
  int mod_vals[32];
  /* Cumulative values from same copy thread for which register costs are
     modified.  */
  int cmod_vals[32];
};

/* Pointer to array containing data for all allocnos.  */
typedef struct recolor_allocno_data *recolor_allocno_data_t;

/* Used for restoring original cost values.  */
struct static_recolor_allocno_data {
  int mod_val;
  int rating;
  int crating;
  bool s_regs_used;
  int mod_vals[32];
  int cmod_vals[32];
  int conflict_vals[32];
};

typedef struct static_recolor_allocno_data static_recolor_allocno_data_t;

/* Data about instructions relevant for recoloring.  */
typedef struct recolor_insn_data
{
  /* Number of register operands.  */
  int reg_num;
  /* Number of short register operands.  */
  int short_reg_num;
  /* Allocnos associated with register operands.  */
  int allocnos[MAX_OPERAND_NUM];
  /* Register operands.  */
  rtx regs[MAX_OPERAND_NUM];
} insn_data_t;

/* Coefficients used to calculate allocno rating (defines recoloring
   priority).   */
static int recolor_coeffs[INSN_CATEGORIES_NUM] = {0, -3, -3, -3, -3, -3};

/* Data used in cost adjust process.  */
recolor_allocno_data_t recolor_allocnos_data;

static_recolor_allocno_data_t static_recolor_allocnos_data[1000];

/* Registers subsets used by 16-bit instructions.  */

int adjust_cost_reg_subsets_hard[6][32] = {
				       /* Standard list of 8 GPRs used by most
					  16 bit instructions.  */
				       {0x7f,16,17,18,19,4,5,6,7,-1},
				       /* Source GPR used by 16 bit store
					  instructions.  */
				       {0x7f,0,17,18,19,4,5,6,7,-1},
				       /* Used for move instructions.  */
				       {0x7f,4,5,6,7,8,9,10,16,17,18,19,20,21,
					22,23,-1},
				       /* 4x4 instructions.  */
				       {0x7f,4,5,6,7,8,9,10,11,16,17,
					18,19,20,21,22,23,-1}};

int adjust_cost_reg_subsets[6][46] = {/* Standard list of 8 GPRs used by most
					 16 bit instructions.  */
				      {0x7f,5,1,6,1,7,1,8,1,3,1,2,1,1,1,0,1,
				       -1},
				      /* Source GPR used by 16 bit store
					 instructions.  */
				      {0x7f,6,1,7,1,8,1,3,1,2,1,1,1,0,1,-1},
				      /* Used for move instructions to
					 increase chances for usage of
					 MOVE.BALC and MOVEP instructions.  */
				      {0x7f,0,1,1,1,2,1,3,1,14,1,15,1,16,1,17,
				       1,5,1,6,1,7,1,8,1,9,1,10,1,11,1,12,1,
				       -1},
				      /* 4x4 instructions.  */
				      {0x7f,0,1,1,1,2,1,3,1,5,1,6,1,7,1,8,1,9,
				       1,10,1,11,1,12,1,14,1,15,1,16,1,17,1,
				       -1}};

/* Vals for which corresponding register cost will be additionally
   increased.  */
int increase_short_reg_costs[32] = {0,0,0,0,4,5,4,4,1,0,0,0,0,0,0,0,2,3,3,3,0,
				    0,0,0,0,0,0,0,0,0,0,0};

/* Initialization function.  */

static void
mips_adjust_costs_init (void)
{
  recolor_allocnos_data
    = (recolor_allocno_data_t) ira_allocate (
	 sizeof (struct recolor_allocno_data) * ira_allocnos_num);
  memset (recolor_allocnos_data, 0, sizeof (struct recolor_allocno_data)
				      * ira_allocnos_num);
}

/* Cleanup function.  */

static void
mips_adjust_costs_finish ()
{
  if (recolor_allocnos_data)
    ira_free (recolor_allocnos_data);
}

/* Returns allocno numbers that are in same copy thread as given allocno.  */
vec<int> get_copy_thread_allocnos (ira_allocno_t a);

/* Calculate cumulative rating for given allocno using values for allocnos from
   same copy thread.  */

static int
get_cumulative_rating (ira_allocno_t a)
{
  int i, cp_a_num;
  int rating = 0;
  vec<int> copies;

  copies = get_copy_thread_allocnos (a);

  rating = recolor_allocnos_data[ALLOCNO_NUM (a)].rating;

  if (copies != vNULL)
    {
      FOR_EACH_VEC_ELT (copies, i, cp_a_num)
	if (! (ALLOCNO_HARD_REGNO (ira_allocnos[cp_a_num]) > 0
	       && !N16_4X4_REG_P (ALLOCNO_HARD_REGNO (ira_allocnos[cp_a_num]))))
	  rating += recolor_allocnos_data[cp_a_num].rating;

      copies.release ();
    }

  return rating;
}

/* Calculate cumulative modification values for hw registers using values for
   registers from same copy thread.  */

static void
mips_create_cumulative_vals (ira_allocno_t a)
{

  int i, j, cp_a_num;
  vec<int> copies;

  for (i=0; i<32; i++)
    recolor_allocnos_data[ALLOCNO_NUM (a)].cmod_vals[i]
      = recolor_allocnos_data[ALLOCNO_NUM (a)].mod_vals[i];

  copies = get_copy_thread_allocnos (a);

  if (copies != vNULL)
    {
      FOR_EACH_VEC_ELT (copies, i, cp_a_num)
	{
	  if (ALLOCNO_HARD_REGNO (ira_allocnos[cp_a_num]) < 0)
	    for (j=0; j<32; j++)
	      recolor_allocnos_data[ALLOCNO_NUM (a)].cmod_vals[j]
		+= recolor_allocnos_data[cp_a_num].mod_vals[j];
	}
      copies.release ();
    }
}

/* Returns allocno associated with given regno.  */

static int
mips_get_allocno_num_for_regno (int regno)
{
  int i;
  for (i=0; i <  ira_allocnos_num; i++)
    if (ira_allocnos[i] && ALLOCNO_REGNO (ira_allocnos[i]) == regno)
      return ALLOCNO_NUM (ira_allocnos[i]);

  return -1;
}

/* Get recoloring data for given instruction.  */

static int
mips_get_insn_data (rtx_insn *insn, void *res)
{
  insn_data_t *i_data = (insn_data_t *)res;

  if (!INSN_P (insn) || (TARGET_NANOMIPS
			      && !mips_insn_16bit_ver_p (insn)))
    return -1;

  extract_insn (insn);

  for (int i=0; i <  recog_data.n_operands; i++)
    {
      int regno[2];
      rtx regs[2];
      int reg_count = 0;
      rtx operand = recog_data.operand[i];

      /* Collect registers from instruction operands.  */
      if (GET_CODE (operand) == SUBREG)
	operand = SUBREG_REG (operand);

      if (REG_P (operand))
	{
	  regno[0] = REGNO (operand);
	  regs[0] = operand;
	  reg_count++;
	}
      else if (MEM_P (operand))
	{
	  if (REG_P (XEXP (operand, 0)))
	    {
	      regno[0] = REGNO (XEXP (operand, 0));
	      regs[0] = XEXP (operand, 0);
	      reg_count++;
	    }
	  else if (GET_CODE (XEXP (operand, 0)) == PLUS
		   && REG_P (XEXP (XEXP (operand, 0), 0)))
	    {
	      regno[0] = REGNO (XEXP (XEXP (operand, 0), 0));
	      regs[0] = XEXP (XEXP (operand, 0), 0);
	      reg_count++;
	    }
	  else if (GET_CODE (XEXP (operand, 0)) == PLUS
		   && GET_CODE (XEXP (XEXP (operand, 0), 0))
		     == MULT
		   && REG_P (XEXP (XEXP (operand, 0), 1))
		   && REG_P (XEXP (XEXP (XEXP (operand, 0), 0), 0)))
	    {
	      regno[0] = REGNO (XEXP (XEXP (operand, 0), 1));
	      regs[0] = XEXP (XEXP (operand, 0), 1);
	      reg_count++;
	      regno[1] = REGNO (XEXP
				  (XEXP (XEXP (operand, 0), 0), 0));
	      regs[1] = XEXP (XEXP (XEXP (operand, 0), 0), 0);
	      reg_count++;
	    }
	}

      /* Analyze register operand data.  */
      for (int j=0; j < reg_count; j++)
	{
	  int allocno_num = mips_get_allocno_num_for_regno (regno[j]);
	  if (allocno_num >= 0)
	    {
	      if ( mips_short_reg_p (ALLOCNO_HARD_REGNO
				      (ira_allocnos[allocno_num])))
		i_data->short_reg_num++;
	      i_data->regs[i_data->reg_num] = regs[j];
	      i_data->allocnos[i_data->reg_num++] = allocno_num;
	    }
	}
    }

  return 0;
}

/* Set cost modification values for hw registers that belong to given register
   subset.  */

static void
mips_set_mod_vals (int *allocnos, int allocno_num,
		   attr_subset_16bit subset_16bit)
{
  int op_pos;
  int pos = 0;
  int op_rep = 0;
  if (subset_16bit == SUBSET_16BIT_NO)
    return;

  for (int i = 0; i < allocno_num; i++)
    {
      if (!op_rep)
	{
	  op_rep = adjust_cost_reg_subsets[subset_16bit][pos++];
	  op_pos = pos;
	}
      else
	pos = op_pos;

      while (adjust_cost_reg_subsets[subset_16bit][pos] >= 0)
	{
	  int val_ix = adjust_cost_reg_subsets[subset_16bit][pos];
	  recolor_allocnos_data[allocnos[i]].mod_vals[val_ix]
	    += adjust_cost_reg_subsets[subset_16bit][pos+1];
	  pos += 2;
	}

      op_rep--;
      pos++;
    }
}


/* Return true if all already assigned hard registers satisfy requirements for
   given register subset.  */

static bool
mips_adjust_short_reg_p (int *allocnos, int allocno_num,
			 attr_subset_16bit subset_16bit)
{
  int op_pos;
  int pos = 0;
  int op_rep = 0;
  if (subset_16bit == SUBSET_16BIT_NO)
    return false;

  for (int i = 0; i < allocno_num; i++)
    {
      int hreg = ALLOCNO_HARD_REGNO (ira_allocnos[allocnos[i]]);
      bool hreg_ok = false;
      if (!op_rep)
	{
	  op_rep = adjust_cost_reg_subsets[subset_16bit][pos++];
	  op_pos = pos;
	}
      else
	pos = op_pos;

      while (adjust_cost_reg_subsets_hard[subset_16bit][pos] >= 0)
	{
	  if (hreg >= 0
	      && hreg == adjust_cost_reg_subsets_hard[subset_16bit][pos])
	    hreg_ok = true;
	  pos++;
	}

      if (hreg >=0 && !hreg_ok)
	return false;

      op_rep--;
      pos++;
    }

  return true;
}

/* Collect all recoloring data for current function.  */

static void
mips_collect_recolor_data (bool &s_regs_used)
{
  basic_block bb;
  rtx_insn *insn;
  int i, j;

  s_regs_used = false;

  FOR_EACH_BB_FN (bb, cfun)
    for (insn = BB_HEAD (bb); insn != NEXT_INSN (BB_END (bb));
	 insn = NEXT_INSN (insn))
      {
	insn_data_t i_data;
	memset (&i_data, 0, sizeof (insn_data_t));

	if (mips_get_insn_data (insn, &i_data) >= 0)
	  {
	    /* Call corresponding function and increment category counter if
	       needed.  */
	    bool short_insn = true;
	    attr_subset_16bit subset_16bit
	      = check_attr_subset_16bit (get_attr_subset_16bit (insn),
					     insn);

	    /* Update corresponding data in recolor_allocnos_data.  */
	    if (!mips_adjust_short_reg_p (i_data.allocnos, i_data.reg_num,
					  subset_16bit))
	      short_insn = false;

	    if (short_insn)
	      {
		for (j = 0; j < i_data.reg_num; j++)
		  recolor_allocnos_data[i_data.allocnos[j]]
		    .categories[i_data.reg_num]++;

		  mips_set_mod_vals (i_data.allocnos, i_data.reg_num,
				       subset_16bit);
	      }

	    for (i = 0; i < i_data.reg_num; i++)
	      {
		int hreg = ALLOCNO_HARD_REGNO
			     (ira_allocnos[i_data.allocnos[i]]);
		if (hreg >= 16 && hreg <= 23)
		  {
		    s_regs_used = true;
		    break;
		  }
	      }
	  }
      }

  /* Set additional recolor_allocnos_data data.  */
  for (i=0; i<ira_allocnos_num; i++)
    {
      recolor_allocnos_data[i].allocno = i;

      if (!ira_allocnos[i])
	continue;

      /* Calculate allocno rating.  */
      for (j=0; j < INSN_CATEGORIES_NUM; j++)
	{
	  int tmp;
	  tmp = recolor_allocnos_data[i].categories[j] * recolor_coeffs[j];
	  recolor_allocnos_data[i].rating += tmp;
	}
   }

  for (i=0; i<ira_allocnos_num; i++)
    {
      if (!ira_allocnos[i] || ALLOCNO_ASSIGNED_P (ira_allocnos[i]))
	continue;

      recolor_allocnos_data[i].crating
	= get_cumulative_rating (ira_allocnos[i]);
      static_recolor_allocnos_data[i].crating
	= recolor_allocnos_data[i].crating;
      mips_create_cumulative_vals (ira_allocnos[i]);
    }
}

/* Compares two allocnos (used for sorting with qsort).  */

static int
mips_compare_allocno_data (const void *ad1, const void *ad2)
{
  int r1 = (*(const recolor_allocno_data_t*)ad1)->crating;
  int r2 = (*(const recolor_allocno_data_t*)ad2)->crating;

  if ( r1 > r2)
    return 1;
  else if (r1 == r2)
    return 0;
  else
    return -1;
}

void
mips_adjust_register_costs (ira_allocno_t a, int val, bool s_regs_used,
			    bool revert, int *conflict_vals)
{
  int j;
  int *cvals = revert ? static_recolor_allocnos_data[ALLOCNO_NUM (a)].cmod_vals
		       : recolor_allocnos_data[ALLOCNO_NUM (a)].cmod_vals;
  reg_class_t aclass = ALLOCNO_CLASS (a);

  if (aclass == NO_REGS)
    return;

  ira_allocate_and_set_costs (&ALLOCNO_HARD_REG_COSTS (a), aclass,
			      ALLOCNO_CLASS_COST (a));
  for (j = 0; j < ira_class_hard_regs_num[aclass]; j++)
    if (TEST_HARD_REG_BIT (reg_class_contents[N16_4X4_REGS],
			     ira_class_hard_regs[aclass][j]))
      {
	int s_correction_val = revert ? -1 : 1;
	int cost_val = cvals[j] * val;
	if (cvals[j]==0)
	  {
	    unsigned reg = ira_class_hard_regs[aclass][j];
	    int cost_incr = increase_short_reg_costs[reg];
	    cost_val = abs (val)+ cost_incr;

	    if (revert)
	      cost_val = -cost_val;
	  }

	if ((!s_regs_used) && ((ira_class_hard_regs[aclass][j] >= 16)
			     && (ira_class_hard_regs[aclass][j] <= 23)))
	  {
	    if (optimize_size)
	      cost_val += s_correction_val;
	    else if ((cost_val < 0 && !revert) || (cost_val > 0 && revert))
	      cost_val = 0;
	  }
	if ((!optimize_size) && (!s_regs_used))
	  cost_val = 0;
	cost_val = cost_val * conflict_vals[j];
	ALLOCNO_HARD_REG_COSTS (a)[j] += cost_val;

	if (ALLOCNO_UPDATED_HARD_REG_COSTS (a))
	  ALLOCNO_UPDATED_HARD_REG_COSTS (a)[j] += cost_val;

	if (ALLOCNO_HARD_REG_COSTS (a)[j] < ALLOCNO_CLASS_COST (a))
	  ALLOCNO_CLASS_COST (a) = ALLOCNO_HARD_REG_COSTS (a)[j];
      }
}

static void
mips_get_conflict_allocnos (ira_allocno_t a,
			    recolor_allocno_data_t recolor_data,
			    vec <recolor_allocno_data_t> *sorted_allocno_data)
{
  int i;
  int nobj = ALLOCNO_NUM_OBJECTS (a);

  sorted_allocno_data->safe_push (&recolor_data[ALLOCNO_NUM (a)]);

  for (i = 0; i < nobj; i++)
    {
      ira_object_t obj = ALLOCNO_OBJECT (a, i);
      ira_object_t conflict_obj;
      ira_object_conflict_iterator oci;

      FOR_EACH_OBJECT_CONFLICT (obj, conflict_obj, oci)
	{
	  ira_allocno_t conflict_a = OBJECT_ALLOCNO (conflict_obj);
	  sorted_allocno_data->safe_push (&recolor_data[ALLOCNO_NUM
							  (conflict_a)]);
	}
    }
}

/* Change val for maximum one register if it is important for given
   allocno.  */
static void
mips_reserve_reg_for_allocno (ira_allocno_t allocno, int *conflict_vals,
			      HARD_REG_SET conflicting_hard_regs)
{
  int *cvals = recolor_allocnos_data[ALLOCNO_NUM (allocno)].cmod_vals;
  reg_class_t aclass = ALLOCNO_CLASS (allocno);
  int best_idx[2] = {0,0};
  int best_cval[2] = {0,0};

  for (int i=0; i<32; i++)
    {
      if (cvals[i] && !TEST_HARD_REG_BIT (conflicting_hard_regs,
					  ira_class_hard_regs[aclass][i]))
	{
	  if (cvals[i] > best_cval[0])
	    {
	      best_cval[1] = best_cval[0];
	      best_cval[0] = cvals[i];
	      best_idx[1] = best_idx[0];
	      best_idx[0] = i;
	    }
	  else if (cvals[i] > best_cval[1])
	    {
	      best_cval[1] = cvals[i];
	      best_idx[1] = i;
	    }
	}
    }

  if (best_cval[1] == 0)
    /* There is only one available register left.  */
    conflict_vals[best_idx[0]] = -1;
  else if (((float)(best_cval[0] * 0.67)) >= ((float)best_cval[1]))
    /* First register is much better choice then all others available.  */
    conflict_vals[best_idx[0]] = 0;
}

static void
mips_check_available_regs_for_allocno (ira_allocno_t allocno,
				       int *conflict_vals)
{
  int n;
  int i;
  HARD_REG_SET conflicting_hard_regs;

  CLEAR_HARD_REG_SET (conflicting_hard_regs);
  n = ALLOCNO_NUM_OBJECTS (allocno);

  for (i = 0; i < n; i++)
    {
      ira_object_t obj = ALLOCNO_OBJECT (allocno, i);
      ira_object_t conflict_obj;
      ira_object_conflict_iterator oci;

      if (OBJECT_CONFLICT_ARRAY (obj) == NULL)
	continue;
      IOR_HARD_REG_SET (conflicting_hard_regs,
			OBJECT_TOTAL_CONFLICT_HARD_REGS (obj));
      AND_COMPL_HARD_REG_SET (conflicting_hard_regs, ira_no_alloc_regs);
      AND_HARD_REG_SET (conflicting_hard_regs,
			reg_class_contents[ALLOCNO_CLASS (allocno)]);

      FOR_EACH_OBJECT_CONFLICT (obj, conflict_obj, oci)
	{
	  ira_allocno_t conflict_a = OBJECT_ALLOCNO (conflict_obj);
	  if (ALLOCNO_HARD_REGNO (conflict_a) > 0)
	    SET_HARD_REG_BIT (conflicting_hard_regs,
			      ALLOCNO_HARD_REGNO (conflict_a));
	}
    }

  mips_reserve_reg_for_allocno (allocno, conflict_vals, conflicting_hard_regs);
}

static void
mips_check_available_regs (ira_allocno_t allocno,
			   vec <recolor_allocno_data_t> sorted_allocno_data,
			   int *conflict_vals)
{
  for (unsigned int j = 0; j < sorted_allocno_data.length (); j++)
    {
      int c_allocno_num = sorted_allocno_data[j]->allocno;
      if (ALLOCNO_NUM (allocno) == c_allocno_num)
	break;
      ira_allocno_t c_allocno = ira_allocnos[c_allocno_num];
      if (ALLOCNO_HARD_REGNO (c_allocno) <= 0)
	mips_check_available_regs_for_allocno (c_allocno, conflict_vals);
    }
}

#include <vector>
#include <algorithm>

ira_allocno_t
get_first_allocno_from_thread (ira_allocno_t a);

/* Threshold for recoloring if not all related allocnos can be recolored.  */
#define MAX_RECOLOR_ALLOCNO_NUM 1000
#define SMALL_REGNO_THRESHOLD 16

/* Entry function for post IRA recoloring.  */

void
mips_adjust_reg_costs (void *p, int func)
{
  int i;
  int rating, crating;
  int val = 4;
  bool s_regs_used;
  ira_allocno_t allocno = (ira_allocno_t)p;
  vec <recolor_allocno_data_t> sorted_allocno_data;
  static ira_loop_tree_node_t current_loop_tree_node = NULL;
  ira_loop_tree_node_t allocno_loop_tree_node
    = ALLOCNO_LOOP_TREE_NODE (allocno);
  int allocno_num = ALLOCNO_NUM (allocno);
  int conflict_vals[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			   1,1,1,1,1,1};

  /* Do not optimize if there is no sufficient data or number of allocnos
     too large.  */

  if (!TARGET_NANOMIPS || !ira_allocnos || !ira_conflicts_p
      || ira_allocnos_num > MAX_RECOLOR_ALLOCNO_NUM)
    return;

  if (func == 2)
    {
      s_regs_used
	= static_recolor_allocnos_data[allocno_num].s_regs_used;
      val = -static_recolor_allocnos_data[allocno_num].mod_val;

      int *c_vals = static_recolor_allocnos_data[allocno_num].conflict_vals;

      if (static_recolor_allocnos_data[allocno_num].crating
	  <= static_recolor_allocnos_data[allocno_num].rating)
	mips_adjust_register_costs (allocno, val, s_regs_used, true, c_vals);

      return;
    }

  mips_adjust_costs_finish ();

  if (current_loop_tree_node != allocno_loop_tree_node)
    {
      memset (static_recolor_allocnos_data, 0,
	sizeof (struct static_recolor_allocno_data) * ira_allocnos_num);
      current_loop_tree_node = allocno_loop_tree_node;
    }
  /* Collect necessary data.  */
  mips_adjust_costs_init ();
  mips_collect_recolor_data (s_regs_used);

  rating = recolor_allocnos_data[allocno_num].rating;
  crating = recolor_allocnos_data[allocno_num].crating;

  if (crating < 0)
    {
      int pos = 0;
      std::vector<int> copy_threads;

      sorted_allocno_data.create (0);
      mips_get_conflict_allocnos (allocno, recolor_allocnos_data,
				  &sorted_allocno_data);
      sorted_allocno_data.qsort (mips_compare_allocno_data);
      for (unsigned int j = 0; j < sorted_allocno_data.length (); j++)
	{
	  int c_allocno_num = sorted_allocno_data[j]->allocno;
	  ira_allocno_t c_allocno = ira_allocnos[c_allocno_num];
	  int thread_allocno = ALLOCNO_NUM
				 (get_first_allocno_from_thread (c_allocno));
	  if (std::find (copy_threads.begin (), copy_threads.end (),
			 thread_allocno) == copy_threads.end ())
	    copy_threads.push_back (thread_allocno);

	  if (thread_allocno
		== ALLOCNO_NUM (get_first_allocno_from_thread (allocno)))
	    pos = copy_threads.size ();

	  if (sorted_allocno_data[j]->allocno == allocno_num)
	    {
	      if (pos < SMALL_REGNO_THRESHOLD)
		{
		  /* Decrease costs of small registers.  */
		  val = -4;
		  /* Check if higher placed allocnos need some of available
		     registers.  */
		  mips_check_available_regs (allocno, sorted_allocno_data,
					     conflict_vals);
		}

	      break;
	    }
	}
      sorted_allocno_data.release ();
    }

  if (rating)
    val *= abs (rating);

  if (!optimize_size && !s_regs_used)
    val = 0;

  mips_adjust_register_costs (allocno, val, s_regs_used, false, conflict_vals);
  static_recolor_allocnos_data[allocno_num].mod_val = val;
  static_recolor_allocnos_data[allocno_num].rating = rating;
  static_recolor_allocnos_data[allocno_num].crating = crating;
  static_recolor_allocnos_data[allocno_num].s_regs_used
    = s_regs_used;

  for (i=0; i<32; i++)
    {
      static_recolor_allocnos_data[allocno_num].mod_vals[i]
	= recolor_allocnos_data[allocno_num].mod_vals[i];
      static_recolor_allocnos_data[allocno_num].cmod_vals[i]
	= recolor_allocnos_data[allocno_num].cmod_vals[i];
      static_recolor_allocnos_data[allocno_num].conflict_vals[i]
	= conflict_vals[i];
    }
}
