/* Yet Another Register Allocator entry point.
   Contributed by Vladimir Makarov.
   Copyright (C) 2005, 2006 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "target.h"
#include "regs.h"
#include "flags.h"
#include "obstack.h"
#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-codes.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "errors.h"
#include "params.h"
#include "timevar.h"
#include "tree-pass.h"
#include "output.h"
#include "integrate.h"
#include "yara-int.h"
#include "cgraph.h"
#include "function.h"

FILE *yara_dump_file;
int yara_max_uid; /* before the allocation */
unsigned char mode_inner_mode [NUM_MACHINE_MODES];

/* The following array is a map hard regs X modes -> number registers
   for store value of given mode starting with given hard regs.  An
   element is defined only if the corresponding value of
   HARD_REGNO_MODE_OK is true.  */
HARD_REG_SET reg_mode_hard_regset [FIRST_PSEUDO_REGISTER] [NUM_MACHINE_MODES];
int memory_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [2];
int register_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [N_REG_CLASSES];
bool class_subset_p [N_REG_CLASSES] [N_REG_CLASSES];



static void
set_inner_mode (void)
{
  int i;
  enum machine_mode wider;

  for (i = 0; i < NUM_MACHINE_MODES; i++)
    mode_inner_mode [i] = VOIDmode;
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    {
      wider = GET_MODE_WIDER_MODE (i);
      if (wider != VOIDmode)
	{
	  yara_assert (mode_inner_mode [wider] == VOIDmode);
	  mode_inner_mode [wider] = i;
	}
    }
}



/* The function sets up map REG_MODE_HARD_REGSET.  */
static void
set_reg_mode_hard_regset (void)
{
  int i, m, hard_regno;

  for (m = 0; m < NUM_MACHINE_MODES; m++)
    for (hard_regno = 0; hard_regno < FIRST_PSEUDO_REGISTER; hard_regno++)
      {
	CLEAR_HARD_REG_SET (reg_mode_hard_regset [hard_regno] [m]);
	if (HARD_REGNO_MODE_OK (hard_regno, m))
	  for (i = hard_regno_nregs [hard_regno] [m] - 1; i >= 0; i--)
	    {
	      yara_assert (hard_regno + i < FIRST_PSEUDO_REGISTER);
	      SET_HARD_REG_BIT (reg_mode_hard_regset [hard_regno] [m],
				hard_regno + i);
	    }
      }
}



static void
set_class_subset_and_move_costs (void)
{
  int cl, cl2;
  enum machine_mode mode;

  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	{
	  memory_move_cost [mode] [cl] [0] = MEMORY_MOVE_COST (mode, cl, 0);
	  memory_move_cost [mode] [cl] [1] = MEMORY_MOVE_COST (mode, cl, 1);
	}

      for (cl2 = (int) N_REG_CLASSES - 1; cl2 >= 0; cl2--)
	{
	  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
	    register_move_cost [mode] [cl] [cl2]
	      = REGISTER_MOVE_COST (mode, cl, cl2);
	  GO_IF_HARD_REG_SUBSET (reg_class_contents[cl],
				 reg_class_contents[cl2],
				 subset);
	  class_subset_p [cl] [cl2] = false;
	  continue;
	  
	subset:
	  class_subset_p [cl] [cl2] = true;
	}
    }
}



short class_hard_regs [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];
int class_hard_regs_num [N_REG_CLASSES];
short class_hard_reg_index [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

#ifdef REG_ALLOC_ORDER
static int hard_reg_alloc_order[FIRST_PSEUDO_REGISTER] = REG_ALLOC_ORDER;
#endif

static void
setup_class_hard_regs (void)
{
  int cl, i, hard_regno, n;
  HARD_REG_SET hard_reg_set;

  yara_assert (SHRT_MAX >= FIRST_PSEUDO_REGISTER);
  
  for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
    {
      COPY_HARD_REG_SET (hard_reg_set, reg_class_contents [cl]);
      AND_COMPL_HARD_REG_SET (hard_reg_set, no_alloc_regs);
      for (n = 0, i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
#ifdef REG_ALLOC_ORDER
	  hard_regno = hard_reg_alloc_order [i];
#else
	  hard_regno = i;
#endif	  
	  if (! TEST_HARD_REG_BIT (hard_reg_set, hard_regno))
	    class_hard_reg_index [cl] [hard_regno] = -1;
	  else
	    {
	      class_hard_reg_index [cl] [hard_regno] = n;
	      class_hard_regs [cl] [n++] = hard_regno;
	    }
	}
      class_hard_regs_num [cl] = n;
    }
}

/* Number of class hard registers available for the register
   allocation.  */
int available_class_regs [N_REG_CLASSES];

/* Function setting up AVAILABLE_CLASS_REGS.  */
static void
setup_available_class_regs (void)
{
  int i, j;
  HARD_REG_SET temp_set;

  memset (available_class_regs, 0, sizeof (available_class_regs));
  for (i = 0; i < N_REG_CLASSES; i++)
    {
      COPY_HARD_REG_SET (temp_set, reg_class_contents [i]);
      AND_COMPL_HARD_REG_SET (temp_set, no_alloc_regs);
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (temp_set, j))
	  available_class_regs [i]++;
    }
}

HARD_REG_SET no_alloc_regs;

void
set_non_alloc_regs (bool use_hard_frame_p)
{
  COPY_HARD_REG_SET (no_alloc_regs, fixed_reg_set);
  if (! use_hard_frame_p)
    SET_HARD_REG_BIT (no_alloc_regs, HARD_FRAME_POINTER_REGNUM);
  setup_class_hard_regs ();
  setup_available_class_regs ();
}



static void print_disposition (FILE *);

static bool check_insn (rtx);
static void check_allocation (void) ATTRIBUTE_UNUSED;



static struct obstack yara_obstack;
static struct bitmap_obstack yara_bitmap_obstack;

#define YARA_OBSTACK

void *
yara_allocate (size_t len)
{
  void *res;

#ifdef YARA_OBSTACK
  res = obstack_alloc (&yara_obstack, len);
#else
  res = xmalloc (len);
#endif
  return res;
}

void
yara_free (void *addr ATTRIBUTE_UNUSED)
{
#ifdef YARA_OBSTACK
  /* do nothing */
#else
  free (addr);
#endif
}

bitmap
yara_allocate_bitmap (void)
{
  return BITMAP_ALLOC (&yara_bitmap_obstack);
}

void
yara_free_bitmap (bitmap b ATTRIBUTE_UNUSED)
{
  /* do nothing */
}

regset
yara_allocate_regset (void)
{
  return ALLOC_REG_SET (&yara_bitmap_obstack);
}

void
yara_free_regset (regset r ATTRIBUTE_UNUSED)
{
  /* do nothing */
}



static void
print_disposition (FILE *f)
{
  int i;
  allocno_t a;

  fprintf (f, "Disposition:");
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a == NULL)
	continue;
      if (i % 4 == 0)
	fprintf (f, "\n");
      if (ALLOCNO_HARD_REGNO (a) >= 0)
	{
	  if (ALLOCNO_REGNO (a) >= 0)
	    fprintf (f, " %4d:%-4d in %-5d", ALLOCNO_NUM (a),
		     ALLOCNO_REGNO (a), ALLOCNO_HARD_REGNO (a));
	  else
	    fprintf (f, " %4d      in %-5d", ALLOCNO_NUM (a),
		     ALLOCNO_HARD_REGNO (a));
	}
      else if (ALLOCNO_MEMORY_SLOT (a) != NULL)
	{
	  if (ALLOCNO_MEMORY_SLOT (a)->mem != NULL_RTX)
	    {
	      yara_assert (ALLOCNO_REGNO (a) >= 0);
	      fprintf (f, " %4d:%-4d on eqmem",
		       ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
	    }
	  else if (ALLOCNO_REGNO (a) >= 0)
	    fprintf (f, " %4d:%-4d on s%-4d", ALLOCNO_NUM (a),
		     ALLOCNO_REGNO (a), ALLOCNO_MEMORY_SLOT (a)->start);
	  else
	    fprintf (f, " %4d      on s%-4d", ALLOCNO_NUM (a),
		     ALLOCNO_MEMORY_SLOT (a)->start);
	}
      else if (ALLOCNO_USE_EQUIV_CONST_P (a))
	fprintf (f, " %4d:%-4d eq const", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
      else if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
	{
	  if (INSN_ALLOCNO_CONST_POOL_P (a))
	    fprintf (f, " %4d const pool   ", ALLOCNO_NUM (a));
	  else if (INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a))
	    fprintf (f, " %4d no change    ", ALLOCNO_NUM (a));
	  else
	    gcc_unreachable ();
	}
      else if (ALLOCNO_DST_COPIES (a) == NULL
	       || ALLOCNO_SRC_COPIES (a) == NULL)
	/* It is an allocno died after abnormal edge allocno
	   coalescing.  */
	fprintf (f, " %4d dead             ", ALLOCNO_NUM (a));
      else
	gcc_unreachable ();
    }
  fprintf (f, "\nstack memory size = %d\n", slot_memory_size);
}

void
debug_disposition (void)
{
  print_disposition (stderr);
}



static bool
check_insn (rtx insn)
{
  int code;

  if (asm_noperands (PATTERN (insn)) >= 0)
    return true;
  code = recog_memoized (insn);
  yara_assert (code >= 0 ||GET_CODE (PATTERN (insn)) == ASM_INPUT
	       || asm_noperands (PATTERN (insn)) >= 0);
  extract_insn (insn);
  /* We want constrain operands to treat this insn strictly
     in its validity determination, i.e., the way it would
     after reload has completed.  */
  return constrain_operands (1);
}

static void
check_allocation (void)
{
  basic_block bb;
  rtx bound, insn;

  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	/* ????!!!! We should check somehow code inside use clobber and
	   asm.  */
	if (INSN_P (insn) && GET_CODE (PATTERN (insn)) != USE
	    && GET_CODE (PATTERN (insn)) != CLOBBER)
	  {
	    if (! check_insn (insn))
	      gcc_unreachable ();
	  }
    }
}



HARD_REG_SET zero_hard_reg_set;
HARD_REG_SET one_hard_reg_set;

void
yara_init_once (void)
{
  CLEAR_HARD_REG_SET (zero_hard_reg_set);
  SET_HARD_REG_SET (one_hard_reg_set);
  set_inner_mode ();
  set_reg_mode_hard_regset ();
  set_class_subset_and_move_costs ();
  set_non_alloc_regs (flag_omit_frame_pointer);
  yara_ir_init_once ();
  yara_trans_init_once ();
  yara_color_init_once ();
}



void
yara (FILE *f)
{
  basic_block bb;
  rtx bound, insn;

  yara_max_uid = get_max_uid ();
  /* Our code is based on assumption that there is no subreg of hard
     registers and memory.  */
  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  cleanup_subreg_operands (insn);
    }
  yara_dump_file = f;
  gcc_obstack_init (&yara_obstack);
  bitmap_obstack_initialize (&yara_bitmap_obstack);
  CLEAR_HARD_REG_SET (cfun->emit->call_used_regs);
  yara_ir_init ();
  yara_trans_init ();
  yara_insn_init ();
  yara_color ();
  if (yara_dump_file != NULL)
    print_disposition (yara_dump_file);
  yara_rewrite ();
  yara_insn_finish ();
  yara_trans_finish ();
  yara_ir_finish ();
  bitmap_obstack_release (&yara_bitmap_obstack);
  obstack_free (&yara_obstack, NULL);
  reload_completed = 1;
#ifdef ENABLE_YARA_CHECKING
  check_allocation ();
#endif
}



static bool
gate_yara (void)
{
  return flag_yara;
}

/* Run yet another register allocator.  */
static unsigned int
rest_of_handle_yara (void)
{
  compact_blocks ();

  /* Determine if the current function is a leaf before running YARA
     since this can impact optimizations done by the prologue and
     epilogue thus changing register elimination offsets.  */
  current_function_is_leaf = leaf_function_p ();

  /* Allocate the reg_renumber array.  */
  allocate_reg_info (max_regno, FALSE, TRUE);

  /* And the reg_equiv_memory_loc array.  */
  VARRAY_GROW (reg_equiv_memory_loc_varray, max_regno);
  reg_equiv_memory_loc = &VARRAY_RTX (reg_equiv_memory_loc_varray, 0);

  allocate_initial_values (reg_equiv_memory_loc);
  if (YARA_PARAMS & YARA_REGCLASS_BEFORE)
    /*?? We should implement own pass. */
    regclass (get_insns (), max_reg_num ());

  if ((YARA_PARAMS & YARA_NO_UPDATE_EQUIV_REGS) == 0)
    update_equiv_regs ();
  /* We need an acurate live analysis before the allocation.  */
  life_analysis (PROP_DEATH_NOTES | PROP_LOG_LINKS | PROP_REG_INFO);

  yara (dump_file);
  if (YARA_PARAMS & YARA_CLEAN_AFTER)
    {
      cleanup_cfg (CLEANUP_EXPENSIVE);
      /* The allocator makes live register information inaccurate.  */
      life_analysis (PROP_DEATH_NOTES | PROP_LOG_LINKS | PROP_REG_INFO);
      reg_scan (get_insns (), max_reg_num ());
      /* ??? We need it because subsequent optimization like post-reload
	 needs it.  */
      regclass (get_insns (), max_reg_num ());
    }
  return 0;
}

struct tree_opt_pass pass_yara =
{
  "yara",                               /* name */
  gate_yara,                            /* gate */
  rest_of_handle_yara,		        /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_YARA,	                        /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'y'                                   /* letter */
};

