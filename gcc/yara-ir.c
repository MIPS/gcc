/* Build IR for Yet Another Register Allocator.
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
#include "tree.h"
#include "tm_p.h"
#include "target.h"
#include "varray.h"
#include "regs.h"
#include "flags.h"
#include "sbitmap.h"
#include "bitmap.h"
#include "toplev.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-codes.h"
#include "insn-config.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "cfgloop.h"
#include "hashtab.h"
#include "errors.h"
#include "ggc.h"
#include "output.h"
#include "params.h"
#include "langhooks.h"
#include "cgraph.h"
#include "function.h"
#include "yara-int.h"

struct loops yara_loops;
struct yara_loop_tree_node *yara_loop_tree_root;
struct yara_loop_tree_node *yara_bb_nodes;
struct yara_loop_tree_node *yara_loop_nodes;
int *reg_max_ref_size, *reg_max_ref_align;
int equiv_memory_num, *reg_equiv_memory_index;
bool *reg_equiv_set_p;
#if 0
rtx *reg_equiv_init;
#endif
struct insn_op_info **insn_infos;
struct insn_op_info **insn_code_infos;
allocno_t *insn_allocnos;
allocno_t **insn_op_allocnos;
copy_t *before_insn_copies, *after_insn_copies;
copy_t *at_bb_start_copies, *at_bb_end_copies;
copy_t *at_edge_start_copies, *at_edge_end_copies;



/* This is information from reg_eliminate for elimination frame
   pointer to stack pointer or hard frame pointer.  */
bool stack_frame_pointer_can_be_eliminated_p;
bool obligatory_stack_frame_pointer_elimination_p;

/* Array of references to allocnos after forming all of them.  */
allocno_t *allocnos;

/* Size of the previous array.  */
int allocnos_num;

/* Array of references to copies after forming all of them.  */
copy_t *copies;

/* Size of the previous array.  */
int copies_num;

/* Element of the following array is bitmap representing conflicting
   registers.  */
bitmap *regno_conflicts;

#ifdef SECONDARY_MEMORY_NEEDED
/* Element of the following array is bitmap representing conflicting
   copies.  */
bitmap *regno_copy_conflicts;
#endif

int **slotno_conflicts;

int *slotno_max_ref_align;
int *slotno_max_ref_size;



static void setup_reg_class_nregs (void);
static void setup_spill_class_mode (void);

static void mark_set_reg_not_eliminable (rtx, rtx, void *);
static int mark_used_reg_not_eliminable_1 (rtx *, void *);
static void mark_used_reg_not_eliminable (rtx *, void *);
static void find_possible_eliminations (void);
static void make_eliminations_consistent (void);
static void remove_unnecessary_eliminations (void);
static void modify_offsets_and_frame_pointer_needed (void);
static void verify_elimination_table (void);
static void init_elim_table (void);
static bool contains_eliminable_reg (rtx);
static void verify_elim_offsets (void);

static void make_edge_enumeration (void);
static void finish_edge_enumeration (void);

static void create_loop_tree_nodes (void);
static void finish_loop_tree_nodes (void);
static void add_loop_to_tree (struct loop *);
static void form_loop_tree (void);

static void set_insn_regno_regs (rtx);
static void set_bb_regno_refs (struct yara_loop_tree_node *);
static void set_loop_regno_refs (struct yara_loop_tree_node *);

static bool contained_pseudo_reg_p (rtx);
static void scan_rtx_for_reg_size_alignment (rtx);
#if 0
static int function_invariant_p (rtx);
#endif
static bool scan_insn_for_reg_equivs (rtx);
static void initiate_equivs (void);
static void finish_equivs (void);

static void print_hard_reg_set (FILE *, HARD_REG_SET);

static int point_freq (struct point *);

static void initiate_allocnos (void);
static allocno_t create_allocno (enum allocno_type, int, enum machine_mode);
static void print_copy (FILE *, copy_t, bool);
static void print_allocno (FILE *, allocno_t);
static void print_allocnos (FILE *f);
static void finish_allocnos (void);

static void initiate_copies (void);
static copy_t create_copy (allocno_t, allocno_t, struct point, rtx);
static copy_t find_copy (allocno_t, allocno_t);
static copy_t sort_copy_list (copy_t, int (*) (const void *, const void *));
static void finish_copies (void);

static void setup_mode_multi_reg_p (void);

static void initiate_conflicts (void);
static void create_conflict (allocno_t, allocno_t);
static void create_copy_conflict (allocno_t, copy_t);
static void finish_conflicts (void);

static void create_regno_allocno_maps (void);
static void delete_regno_allocno_maps (void);
static void allocate_loop_regno_allocno_map (struct yara_loop_tree_node *);
static void free_loop_regno_allocno_map (struct yara_loop_tree_node *);

static void create_insn_maps (void);
static void finish_insn_maps (void);

static allocno_t create_insn_allocno (enum op_type, int, int,
				      enum machine_mode, rtx, rtx *, rtx *,
				      bool, allocno_t);
static rtx *get_through_addr_subreg (rtx *);
static void process_address_for_allocno (rtx, rtx *, rtx *, allocno_t);
static void process_base_index_register_for_allocno (rtx, rtx *, allocno_t);
static struct insn_op_info *create_insn_info (rtx);
static void process_live_allocnos (void (*) (allocno_t, void *, int), void *);
static void set_hard_reg_conflict (allocno_t, void *, int);
static void mark_hard_reg_live (int, enum machine_mode);
static void mark_hard_reg_death (int, enum machine_mode);
static int find_local_live_allocno (allocno_t);
static void set_allocno_conflict (allocno_t, void *, int);
static void mark_allocno_live (allocno_t, bool);
static void mark_allocno_death (allocno_t);
static void process_non_operand_hard_regs (rtx *, bool);

static void set_call_info (allocno_t, void *, int);
static void set_single_hard_reg_allocno_info (allocno_t, void *, int);
static enum reg_class single_alt_reg_class (const char *,
					    struct insn_op_info *, int);
static enum reg_class single_reg_operand_class (rtx insn, int);

static void set_copy_conflict (allocno_t, void *, int);
static void build_insn_allocno_copy_conflicts (copy_t, rtx, bool);
static void set_call_info (allocno_t, void *, int);
static void build_insn_allocno_conflicts (rtx, op_set_t);
static allocno_t create_region_allocno (int, struct yara_loop_tree_node *,
					rtx);
static int before_insn_copy_compare (const void *, const void *);
static int after_insn_copy_compare (const void *, const void *);
static void merge_the_same_regno_source_copies (copy_t);
static void process_const_operands_in_elimination_part (rtx);
static void setup_insn_elimination_part_const_p (void);
static void create_insn_copies (rtx);
static allocno_t create_loc_insn_allocnos (rtx, rtx *, enum machine_mode,
					   bool, enum op_type, int);
static void create_insn_allocnos (rtx);
static void create_bb_insn_allocnos (struct yara_loop_tree_node *);
static void add_live_through_allocno (allocno_t, bool, basic_block, edge);
static void create_bb_allocno (int, struct yara_loop_tree_node *,
			       bool, bitmap, bool);
static bool abnormal_edge_p (VEC(edge,gc) *);
static void create_block_allocnos (struct yara_loop_tree_node *);
static void create_edge_allocno (int, edge,
				 struct yara_loop_tree_node *, bool, bitmap);
static void create_edge_allocnos (edge, struct yara_loop_tree_node *,
				  bool, bitmap);
static void create_loop_allocnos (struct yara_loop_tree_node *);
static void create_subloop_allocnos (struct yara_loop_tree_node *);
static void create_all_allocnos (void);


static void sort_copies_on_edges (void);

static void add_subsequent_copy_conflicts (copy_t);
static void add_region_allocno_list_conflicts (copy_t, HARD_REG_SET,
					       HARD_REG_SET);
static void add_region_allocno_copy_conflicts (void);

#ifdef ENABLE_YARA_CHECKING
static void check_abnormal_copy_list (copy_t, bitmap);
static void check_abnormal_edges (void);
#endif

static bool propagate_value (allocno_t, allocno_t);
static bool propagate_copy_value (copy_t);
static void make_vn (void);

static void setup_allocno_frequency (allocno_t);
static void setup_all_allocno_frequencies (void);

static void initiate_cans (void);
static void finish_cans (void);

static void create_cans (void);

static void find_conflicting_cans (can_t);



/* Map: register class x machine mode -> number of hard registers of
   given class needed to store value of given mode.  If the number is
   different, the size will be negative.  */
int reg_class_nregs [N_REG_CLASSES] [MAX_MACHINE_MODE];

/* Function forming reg_class_nregs map.  */
static void
setup_reg_class_nregs (void)
{
  int m;
  enum reg_class cl;

  for (cl = 0; cl < N_REG_CLASSES; cl++)
    for (m = 0; m < MAX_MACHINE_MODE; m++)
      reg_class_nregs [cl] [m] = CLASS_MAX_NREGS (cl, m);
}

/* ??? implement better class SImode instead of HImode for QImode.  */

enum machine_mode spill_class_mode [N_REG_CLASSES] [MAX_MACHINE_MODE];
enum machine_mode spill_mode [MAX_MACHINE_MODE];

static void
setup_spill_class_mode (void)
{
  int mode, m, cl, size;
  enum reg_class interm_class;
  rtx x;

  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      spill_mode [mode] = mode;
      for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
	spill_class_mode [cl] [mode] = mode;
    }
#ifdef HAVE_SECONDARY_RELOADS
  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    for (cl = (int) N_REG_CLASSES - 1; cl >= 0; cl--)
      {
	size = reg_class_nregs [cl] [mode];
	if (size < 0)
	  continue;
	for (m = mode;
	     m != VOIDmode && reg_class_nregs [cl] [m] == size;
	     m = GET_MODE_WIDER_MODE (m))
	  {
#ifdef CANNOT_CHANGE_MODE_CLASS
	    if (CANNOT_CHANGE_MODE_CLASS (mode, m, cl))
	      continue;
#endif
	    x = gen_rtx_MEM (mode, gen_rtx_PLUS (Pmode, stack_pointer_rtx,
						 const0_rtx));
#ifdef SECONDARY_INPUT_RELOAD_CLASS
	    interm_class = SECONDARY_INPUT_RELOAD_CLASS (cl, m, x);
	    if (interm_class != NO_REGS)
	      continue;
#endif
#ifdef SECONDARY_OUTPUT_RELOAD_CLASS
	    interm_class = SECONDARY_OUTPUT_RELOAD_CLASS (cl, m, x);
	    if (interm_class == NO_REGS)
	      break;
#endif
	  }
	if (m != VOIDmode && reg_class_nregs [cl] [m] == size)
	  spill_class_mode [cl] [mode] = m;
      }
#endif
  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    {
      m = spill_class_mode [0] [mode];
      for (cl = 1; cl < (int) N_REG_CLASSES; cl++)
	if (m != (int) spill_class_mode [cl] [mode])
	  {
	    yara_assert ((int) spill_mode [mode] == mode
			 || (int) spill_mode [mode] == m);
	    spill_mode [mode] = m;
	  }
    }
}



#if defined ENABLE_YARA_CHECKING && (GCC_VERSION >= 2007)

/* Complain that the allocno type of A does not match the expected
   TYPE.  FILE, LINE, and FUNCTION are of the caller.  */

void
allocno_check_failed (const allocno_t a, const char *file,
		      int line, const char *function, int type)
{
  const char *expected;
  const char *given;

  expected = (type == INSN_ALLOCNO ? "INSN ALLOCNO" : "REGION ALLOCNO");
  given = (ALLOCNO_TYPE (a) ? "INSN ALLOCNO" : "REGION_ALLOCNO");
  internal_error ("allocno check: expected %s, have %s in %s, at %s:%d",
		  expected, given, function, trim_filename (file), line);
}

#endif



/* This page contains data structure and function to eliminate
   hard registers.  */

/* The following table stores possible eliminations from each hard
   register.  */
struct reg_eliminate *reg_eliminate [FIRST_PSEUDO_REGISTER];

/* List of all elimination structures. */
static struct reg_eliminate *reg_eliminations;

/* name initial ???  Addition needed to transform address using frame
   pointer to stack pointer.  It is roughly local variables size.  And
   it does not changed during allocation until final allocation stack
   slots to some pseudo-registers. */
static int frame_stack_pointer_initial_offset;
int frame_stack_pointer_offset;
/* name initial ??? Addition needed to transform address using frame
   pointer to hard frame pointer.  The value can be changed during
   allocation because some allocated hard registers should be saved on
   the stack.  */
static int frame_hard_frame_pointer_initial_offset;
int frame_hard_frame_pointer_offset;

/* This is an intermediate structure to initialize the table.  It has
   exactly the members provided by ELIMINABLE_REGS.  */
static const struct elim_table_1
{
  const int from;
  const int to;
} reg_eliminate_1[] =

/* If a set of eliminable registers was specified, define the table from it.
   Otherwise, default to the normal case of the frame pointer being
   replaced by the stack pointer.  */

#ifdef ELIMINABLE_REGS
  ELIMINABLE_REGS;
#else
  {{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}};
#endif

#define NUM_ELIMINABLE_REGS ARRAY_SIZE (reg_eliminate_1)

/* Given X, a SET or CLOBBER of DEST, if DEST is the target of a register
   replacement we currently believe is valid, mark it as not eliminable if X
   modifies DEST in any way other than by adding a constant integer to it.

   If DEST is the frame pointer, we do nothing because we assume that
   all assignments to the hard frame pointer are nonlocal gotos and are being
   done at a time when they are valid and do not disturb anything else.
   Some machines want to eliminate a fake argument pointer with either the
   frame or stack pointer.  Assignments to the hard frame pointer must not
   prevent this elimination.

   Called via note_stores.  */

static void
mark_set_reg_not_eliminable (rtx dest, rtx x, void *data ATTRIBUTE_UNUSED)
{
  bool incr_p;
  struct reg_eliminate *ep;

  /* A SUBREG of a hard register here is just changing its mode.  We
     should not see a SUBREG of an eliminable hard register, but check
     just in case.  */
  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);

#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  /* Special case frame = bp.  Just ignore.  */
  if (dest == hard_frame_pointer_rtx
      || (dest == frame_pointer_rtx
	  && GET_CODE (x) == SET && SET_SRC (x) == hard_frame_pointer_rtx))
    return;
#endif

  incr_p = (GET_CODE (x) == SET && GET_CODE (SET_SRC (x)) == PLUS
	    && XEXP (SET_SRC (x), 0) == dest
	    && GET_CODE (XEXP (SET_SRC (x), 1)) == CONST_INT);
  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    if (ep->can_eliminate && REG_P (dest)
	&& (REGNO (dest) == (unsigned) ep->to
	    || REGNO (dest) == (unsigned) ep->from))
      {
	if (! incr_p)
	  ep->can_eliminate = false;
	else if (REGNO (dest) == (unsigned) ep->to)
	  ep->offset += INTVAL (XEXP (SET_SRC (x), 1));
	else
	  {
	    yara_assert (REGNO (dest) == (unsigned) ep->from);
	    ep->offset -= INTVAL (XEXP (SET_SRC (x), 1));
	  }
      }
}

/* The function checks that hard-register *X can not be eliminated
   because it was changed improperly.  */

static int
mark_used_reg_not_eliminable_1 (rtx *x, void *data ATTRIBUTE_UNUSED)
{
  int regno;
  struct reg_eliminate *ep;

  if (GET_CODE (*x) == REG && HARD_REGISTER_P (*x))
    {
      regno = REGNO (*x);
      for (ep = reg_eliminate [regno]; ep != NULL; ep = ep->from_next)
	if (ep->can_eliminate && ep->initial_offset != ep->offset)
	  ep->can_eliminate = false;
    }
  return 0;
}

/* The function processes all hard registers in *X with the aid of
   previous function.  */

static void
mark_used_reg_not_eliminable (rtx *x, void *data)
{
  for_each_rtx (x, mark_used_reg_not_eliminable_1, data);
}

/* If we have some registers we think can be eliminated, scan all
   insns to see if there is an insn that sets one of these registers
   to something other than itself plus a constant.  If so, the
   register cannot be eliminated.  Doing this scan here eliminates an
   extra pass through the main reload loop in the most common case
   where register elimination cannot be done.  */
  
static void
find_possible_eliminations (void)
{
  basic_block bb;
  rtx insn, bound;
  struct reg_eliminate *ep;

  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    note_stores (PATTERN (insn), mark_set_reg_not_eliminable, NULL);
	  else if (GET_CODE (insn) == CLOBBER)
	    mark_set_reg_not_eliminable (XEXP (insn, 0), insn, NULL);
	  else if (GET_CODE (insn) == USE)
	    mark_used_reg_not_eliminable (&XEXP (insn, 0), NULL);
	  if (LABEL_P (insn) || JUMP_P (insn) || CALL_P (insn))
	    {
	      for (ep = reg_eliminations; ep != NULL; ep = ep->next)
		if (ep->can_eliminate && ep->offset != ep->initial_offset)
		  ep->can_eliminate = false;
	    }
	}
    }
}

/* Remove unnecessary structures to speed up work with the table.  */

static void
remove_unnecessary_eliminations (void)
{
  int i;
  struct reg_eliminate *ep, *next_ep, *prev_ep;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (prev_ep = NULL, ep = reg_eliminate [i]; ep != NULL; ep = next_ep)
      {
	next_ep = ep->from_next;
	if (ep->can_eliminate && ep->to != i)
	  {
	    if (ep->obligatory)
	      {
		/* We can have only one obligatory elimination */
		reg_eliminate [i] = ep;
		ep->from_next = NULL;
		break;
	      }
	    prev_ep = ep;
	  }
	else
	  {
	    ep->from = -1; /* Mark it for freeing.  */
	    if (prev_ep == NULL)
	      reg_eliminate [i] = next_ep;
	    else
	      prev_ep->from_next = next_ep;
	  }
      }
  /* Remove them from list of all structures.  */
  for (prev_ep = NULL, ep = reg_eliminations; ep != NULL; ep = next_ep)
    {
      next_ep = ep->next;
      if (ep->from >= 0)
	prev_ep = ep;
      else
	{
	  if (prev_ep == NULL)
	    reg_eliminations = next_ep;
	  else
	    prev_ep->next = next_ep;
	  yara_free (ep);
	}
    }
}

/* Look for the case where we have discovered that we can't replace
   register A with register B and that means that we will now be
   trying to replace register A with register C.  This means we can no
   longer replace register C with register B and we need to disable
   such an elimination, if it exists.  This occurs often with A == ap,
   B == sp, and C == fp.  */

static void
make_eliminations_consistent (void)
{
  struct reg_eliminate *ep, *ep1, *ep2;

  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    if (! ep->can_eliminate)
      {
	/* Find the current elimination from EP->FROM, if there is a new
	   one.  */
	for (ep1 = reg_eliminate [ep->from]; ep1 != NULL; ep1 = ep1->from_next)
	  if (ep1->can_eliminate)
	    {
	      for (ep2 = reg_eliminate [ep1->to];
		   ep2 != NULL;
		   ep2 = ep2->from_next)
		if (ep2->to == ep->to)
		  ep2->can_eliminate = false;
	    }
      }
}

static void
modify_offsets_and_frame_pointer_needed (void)
{
  struct reg_eliminate *ep;

  frame_pointer_needed = 1;
  for (ep = reg_eliminate [FRAME_POINTER_REGNUM];
       ep != NULL;
       ep = ep->from_next)
    if (ep->obligatory && ep->to == STACK_POINTER_REGNUM)
      {
	yara_assert (ep->can_eliminate);
	frame_pointer_needed = 0;
      }
  
  yara_assert (reg_eliminate [FRAME_POINTER_REGNUM] != NULL);
  stack_frame_pointer_can_be_eliminated_p = false;
#ifdef ELIMINABLE_REGS
  INITIAL_ELIMINATION_OFFSET (FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM,
			      frame_stack_pointer_initial_offset);
#else
  INITIAL_FRAME_POINTER_OFFSET (frame_stack_pointer_initial_offset);
#endif
  frame_stack_pointer_offset = frame_stack_pointer_initial_offset;
  frame_hard_frame_pointer_initial_offset = 0;
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  INITIAL_ELIMINATION_OFFSET (FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM,
			      frame_hard_frame_pointer_initial_offset);
#endif
  frame_hard_frame_pointer_offset = frame_hard_frame_pointer_initial_offset;
  /* Changing frame pointer needed might change offsets.  Therefore we
     calculate the real values here.  */
  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    {
      yara_assert (ep->can_eliminate);
#ifdef ELIMINABLE_REGS
      INITIAL_ELIMINATION_OFFSET (ep->from, ep->to, ep->initial_offset);
#else
      yara_assert (ep->from == FRAME_POINTER_REGNUM
		   && ep->to == STACK_POINTER_REGNUM);
      INITIAL_FRAME_POINTER_OFFSET (ep->initial_offset);
#endif
      ep->offset = ep->initial_offset;
      if (ep->from == FRAME_POINTER_REGNUM && ep->to == STACK_POINTER_REGNUM)
	{
	  stack_frame_pointer_can_be_eliminated_p = true;
	  obligatory_stack_frame_pointer_elimination_p = ep->obligatory;
	}
    }
  /* Check that NO_ALLOC_REGS should be changed.  */
  if (stack_frame_pointer_can_be_eliminated_p
      && obligatory_stack_frame_pointer_elimination_p)
    {
      if (TEST_HARD_REG_BIT (no_alloc_regs, HARD_FRAME_POINTER_REGNUM))
	set_non_alloc_regs (true);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, HARD_FRAME_POINTER_REGNUM))
    set_non_alloc_regs (false);
}

static void
verify_elimination_table (void)
{
  struct reg_eliminate *ep;
  
  /* Verify elimination table */
  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    yara_assert (ep->can_eliminate
		 && (ep->from == HARD_FRAME_POINTER_REGNUM
		     || TEST_HARD_REG_BIT (fixed_reg_set, ep->from)));
}

static void
clear_frame_pointer_liveness (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      CLEAR_REGNO_REG_SET (bb->il.rtl->global_live_at_start,
			   HARD_FRAME_POINTER_REGNUM);
      CLEAR_REGNO_REG_SET (bb->il.rtl->global_live_at_end,
			   HARD_FRAME_POINTER_REGNUM);
    }
}

/* Initialize the table of registers to eliminate.  */

static void
init_elim_table (void)
{
  int from;
  struct reg_eliminate *ep;
#ifdef ELIMINABLE_REGS
  const struct elim_table_1 *ep1;
#endif

  memset (&reg_eliminate, 0, sizeof (reg_eliminate));
  reg_eliminations = NULL;

  /* Does this function really require a frame pointer?  */
  frame_pointer_needed = (FRAME_POINTER_REQUIRED
			  /* ?? If EXIT_IGNORE_STACK is set, we will
			     not save and restore sp for alloca.  So
			     we can't eliminate the frame pointer in
			     that case.  At some point, we should
			     improve this by emitting the sp-adjusting
			     insns for this case.  */
			  || (current_function_calls_alloca
			      && EXIT_IGNORE_STACK));

#ifdef ELIMINABLE_REGS
  for (ep1 = reg_eliminate_1;
       ep1 < &reg_eliminate_1 [NUM_ELIMINABLE_REGS];
       ep1++)
    {
      from = ep1->from;
      ep = xmalloc (sizeof (struct reg_eliminate));
      ep->from_next = reg_eliminate [from];
      reg_eliminate [from] = ep;
      ep->next = reg_eliminations;
      reg_eliminations = ep;
      ep->from = from;
      ep->to = ep1->to;
      ep->can_eliminate
	= (! (from == HARD_FRAME_POINTER_REGNUM && FRAME_POINTER_REQUIRED)
	   && CAN_ELIMINATE (from, ep->to)
	   && ! (ep->to == STACK_POINTER_REGNUM && frame_pointer_needed));
      ep->obligatory = (flag_omit_frame_pointer
			? ep->to == STACK_POINTER_REGNUM : false);
      /* We need it for check changes of the registers in
	 find_possible_eliminations.  */
      ep->offset = ep->initial_offset = 0;
    }
#else
  from = reg_eliminate_1 [0].from;
  ep = reg_eliminate [from]
    = reg_eliminations = xmalloc (sizeof (struct reg_eliminate));
  ep->from = from;
  ep->to = reg_eliminate_1 [0].to;
  ep->can_eliminate = ! frame_pointer_needed;
  ep->obligatory = flag_omit_frame_pointer;
  /* We need it for check changes of the registers in
     find_possible_eliminations.  */
  ep->offset = ep->initial_offset = 0;
  ep->from_next = NULL;
  ep->next = NULL;
#endif

  find_possible_eliminations ();
  make_eliminations_consistent ();
  remove_unnecessary_eliminations ();
  modify_offsets_and_frame_pointer_needed ();
  verify_elimination_table ();
  
  if (! frame_pointer_needed)
    clear_frame_pointer_liveness ();
}

static bool
contains_eliminable_reg (rtx x)
{
  int i;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  /* Ignore registers in memory.  */
  if (code == MEM)
    return false;

  if (code == REG && HARD_REGISTER_P (x))
    return reg_eliminate [REGNO (x)] != NULL;
  
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt [i] == 'e')
	{
	  if (contains_eliminable_reg (XEXP (x, i)))
	    return true;
	}
      else if (fmt [i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    if (contains_eliminable_reg (XVECEXP (x, i, j)))
	      return true;
	}
    }
  return false;
}

void
update_elim_offsets (void)
{
  struct reg_eliminate *ep;

  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    {
      yara_assert (ep->can_eliminate);
#ifdef ELIMINABLE_REGS
      INITIAL_ELIMINATION_OFFSET (ep->from, ep->to, ep->offset);
#else
      yara_assert (ep->from == FRAME_POINTER_REGNUM
		   && ep->to == STACK_POINTER_REGNUM);
      INITIAL_FRAME_POINTER_OFFSET (ep->offset);
#endif
    }
  /* ??? */
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  INITIAL_ELIMINATION_OFFSET (FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM,
			      frame_hard_frame_pointer_offset);
#endif
#ifdef ELIMINABLE_REGS
  INITIAL_ELIMINATION_OFFSET (FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM,
			      frame_stack_pointer_offset);
#else
  INITIAL_FRAME_POINTER_OFFSET (frame_stack_pointer_offset);
#endif
}

/* Verify that the elimination offsets did not change since the call
   to init_elim_table.  This is used to catch cases where something
   illegal happened during allocation that could cause incorrect code
   to be generated if we did not check for it.  */

static void
verify_elim_offsets (void)
{
  HOST_WIDE_INT t;

#ifdef ELIMINABLE_REGS
  struct reg_eliminate *ep;

  for (ep = reg_eliminations; ep != NULL; ep = ep->next)
    {
      yara_assert (ep->can_eliminate);
      INITIAL_ELIMINATION_OFFSET (ep->from, ep->to, t);
      ep->offset = t;
    }
#else
  INITIAL_FRAME_POINTER_OFFSET (t);
  reg_eliminate [FRAME_POINTER_REGNUM].offset = t;
#endif
}



static void
make_edge_enumeration (void)
{
  basic_block bb;
  int n = 0;

  FOR_EACH_BB (bb)
    {
      edge_iterator ei;
      edge e;

      FOR_EACH_EDGE (e, ei, bb->succs)
	e->aux = (void *) (size_t) n++;
    }
  yara_assert (n <= n_edges);
}

static void
finish_edge_enumeration (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      edge_iterator ei;
      edge e;

      FOR_EACH_EDGE (e, ei, bb->succs)
	e->aux = NULL;
    }
}



/* The number of basic blocks before the register allocation.  */
static int bbs_num;

static void
create_loop_tree_nodes (void)
{
  int i;

  bbs_num = n_basic_blocks;
  yara_bb_nodes
    = yara_allocate (sizeof (struct yara_loop_tree_node) * bbs_num);
  for (i = 0; i < bbs_num; i++)
    {
      yara_bb_nodes [i].regno_refs = yara_allocate_bitmap ();
      yara_bb_nodes [i].allocno_live_at_start = yara_allocate_bitmap ();
      yara_bb_nodes [i].can_through = yara_allocate_bitmap ();
    }
  yara_loop_nodes = yara_allocate (sizeof (struct yara_loop_tree_node)
				   * yara_loops.num);
  for (i = 0; i < (int) yara_loops.num; i++)
    {
      yara_loop_nodes [i].regno_refs = yara_allocate_bitmap ();
      yara_loop_nodes [i].allocno_live_at_start = yara_allocate_bitmap ();
      yara_loop_nodes [i].can_through = yara_allocate_bitmap ();
    }
}

static void
finish_loop_tree_nodes (void)
{
  int i;

  for (i = 0; i < (int) yara_loops.num; i++)
    {
      yara_free_bitmap (yara_loop_nodes [i].can_through);
      yara_free_bitmap (yara_loop_nodes [i].allocno_live_at_start);
      yara_free_bitmap (yara_loop_nodes [i].regno_refs);
    }
  yara_free (yara_loop_nodes);
  for (i = 0; i < bbs_num; i++)
    {
      yara_free_bitmap (yara_bb_nodes [i].can_through);
      yara_free_bitmap (yara_bb_nodes [i].allocno_live_at_start);
      yara_free_bitmap (yara_bb_nodes [i].regno_refs);
    }
  yara_free (yara_bb_nodes);
}



/* The following recursive functions adds loop to the loop tree
   hierarchy.  The loop is added only once.  */
static void
add_loop_to_tree (struct loop *loop)
{
  struct yara_loop_tree_node *loop_node, *father_node;

  /* Can not use loop node access macros because of potential checking
     and because the nodes are not initialized yet.  */
  if (loop->outer != NULL)
    add_loop_to_tree (loop->outer);
  if (yara_loop_nodes [loop->num].inner == NULL)
    {
      /* We have not added loop node to the tree yet.  */
      loop_node = &yara_loop_nodes [loop->num];
      loop_node->loop = loop;
      loop_node->bb = NULL;
      if (loop->outer == NULL)
	{
	  loop_node->next = NULL;
	  loop_node->father = NULL;
	}
      else
	{
	  father_node = &yara_loop_nodes [loop->outer->num];
	  loop_node->next = father_node->inner;
	  father_node->inner = loop_node;
	  loop_node->father = father_node;
	}
    }
}

/* The following function creates the loop tree.  The algorithm is
   designed to provide correct order of loops (by the last loop bb)
   and basic blocks in chain formed by next.  */
static void
form_loop_tree (void)
{
  int i;
  basic_block bb;
  struct loop *father;
  struct yara_loop_tree_node *bb_node, *loop_node;

  /* Can not use loop/bb node access macros because of potential
     checking and the nodes are not initialized yet.  */
  YARA_FOR_ALL_LOOP_INDEX (i)
    {
      yara_loop_nodes [i].inner = NULL;
    }
  FOR_EACH_BB_REVERSE (bb)
    {
      bb_node = &yara_bb_nodes [bb->index];
      bb_node->bb = bb;
      bb_node->loop = NULL;
      bb_node->inner = NULL;
      bb_node->next = NULL;
      father = bb->loop_father;
      add_loop_to_tree (father);
      loop_node = &yara_loop_nodes [father->num];
      bb_node->next = loop_node->inner;
      bb_node->father = loop_node;
      loop_node->inner = bb_node;
    }
  yara_loop_tree_root = YARA_LOOP_NODE_BY_INDEX (yara_loops.tree_root->num);
}



/* Loop tree node for the current basic block.  */
static struct yara_loop_tree_node *curr_bb_node;

static void
set_insn_regno_regs (rtx x)
{
  int i, j;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);

  if (code == REG)
    {
      bitmap_set_bit (curr_bb_node->regno_refs, REGNO (x));
      return;
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	set_insn_regno_regs (XEXP (x, i));
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  set_insn_regno_regs (XVECEXP (x, i, j));
    }
}

static void
set_bb_regno_refs (struct yara_loop_tree_node *bb_node)
{
  rtx insn, bound;
  basic_block bb;

  curr_bb_node = bb_node;
  bb = bb_node->bb;
  bound = NEXT_INSN (BB_END (bb));
  for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      set_insn_regno_regs (PATTERN (insn));
}

static void
set_loop_regno_refs (struct yara_loop_tree_node *loop_node)
{
  struct yara_loop_tree_node *subloop_node;

  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    {
      if (subloop_node->bb == NULL)
	set_loop_regno_refs (subloop_node);
      else
	set_bb_regno_refs (subloop_node);
      bitmap_ior_into (loop_node->regno_refs, subloop_node->regno_refs);
    }
}



int
get_stack_align (enum machine_mode mode)
{
  int align;
  tree type;

  if (mode == BLKmode)
    align = BIGGEST_ALIGNMENT;
  else
    align = GET_MODE_ALIGNMENT (mode);
  
  type = lang_hooks.types.type_for_mode (mode, 0);

  if (type)
    align = LOCAL_ALIGNMENT (type, align);

  return align;
}

/* Find all paradoxical subregs within X and update reg_max_ref_size.  */

static void
scan_rtx_for_reg_size_alignment (rtx x)
{
  int i, regno, align;
  const char *fmt;
  enum machine_mode mode;
  enum rtx_code code = GET_CODE (x);

  if (code == REG || (code == SUBREG && REG_P (SUBREG_REG (x))))
    {
      mode = GET_MODE (x);
      mode = spill_mode [mode];
      regno = (code == REG ? REGNO (x) : REGNO (SUBREG_REG (x)));
      if (GET_MODE_SIZE (mode) > reg_max_ref_size [regno])
	reg_max_ref_size [regno] = GET_MODE_SIZE (mode);
      align = get_stack_align (mode);
      if (mode == BLKmode)
	reg_max_ref_align [regno] = BIGGEST_ALIGNMENT / BITS_PER_UNIT;
      else if (align / BITS_PER_UNIT > reg_max_ref_align [regno])
	reg_max_ref_align [regno] = align / BITS_PER_UNIT;
    }
  
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	scan_rtx_for_reg_size_alignment (XEXP (x, i));
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    scan_rtx_for_reg_size_alignment (XVECEXP (x, i, j));
	}
    }
}

#if 0
/* Return nonzero if the rtx X is invariant over the current function.  */
/* ??? Actually, the places where we use this expect exactly what is
   tested here, and not everything that is function invariant.  In
   particular, the frame pointer and arg pointer are special cased;
   pic_offset_table_rtx is not, and this will cause aborts when we go
   to spill these things to memory.  */

static int
function_invariant_p (rtx x)
{
  if (CONSTANT_P (x))
    return 1;
  if (x == frame_pointer_rtx || x == arg_pointer_rtx)
    return 1;
  if (GET_CODE (x) == PLUS
      && (XEXP (x, 0) == frame_pointer_rtx || XEXP (x, 0) == arg_pointer_rtx)
      && CONSTANT_P (XEXP (x, 1)))
    return 1;
  return 0;
}
#endif

static bool
contained_pseudo_reg_p (rtx x)
{
  enum rtx_code code;
  const char *fmt;
  int i, j;

  code = GET_CODE (x);
  if (code == REG)
    return ! HARD_REGISTER_P (x);
  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++)
    {
      if (fmt[i] == 'e')
	{
	  if (contained_pseudo_reg_p (XEXP (x, i)))
	    return true;
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (contained_pseudo_reg_p (XVECEXP (x, i, j)))
	    return true;
    }
  return false;
}

static bool
scan_insn_for_reg_equivs (rtx insn)
{
  int i;
  rtx set = single_set (insn);
  bool eliminable_invariant_p;
  eliminable_invariant_p = false;
  if (set != NULL_RTX && REG_P (SET_DEST (set)))
    {
      rtx note = find_reg_note (insn, REG_EQUIV, NULL_RTX);

      if (note
	  && (! function_invariant_p (XEXP (note, 0))
	      || ! flag_pic
	      /* A function invariant is often CONSTANT_P but may
		 include a register.  We promise to only pass
		 CONSTANT_P objects to LEGITIMATE_PIC_OPERAND_P.  */
	      || (CONSTANT_P (XEXP (note, 0))
		  && LEGITIMATE_PIC_OPERAND_P (XEXP (note, 0)))))
	{
	  rtx x = XEXP (note, 0);

	  i = REGNO (SET_DEST (set));
	  /* ??? How to make pseudo working.  */
	  if (i > LAST_VIRTUAL_REGISTER && ! contained_pseudo_reg_p (x))
	    {
	      /* It can happen that a REG_EQUIV note contains a MEM
		 that is not a legitimate memory operand.  As later
		 stages of reload assume that all addresses found in
		 the reg_equiv_* arrays were originally legitimate, we
		 ignore such REG_EQUIV notes.  */
	      if (memory_operand (x, VOIDmode))
		{
		  /* ??? All equiv memory for different registers should
		     be not intersected.  */
		  if (reg_equiv_memory_loc [i] == NULL)
		    {
		      reg_equiv_memory_index [i] = equiv_memory_num;
		      equiv_memory_num += GET_MODE_SIZE (GET_MODE (x));
		    }
		  /* Always unshare the equivalence, so we can
		     substitute into this insn without touching the
		     equivalence.  */
		  reg_equiv_memory_loc [i] = copy_rtx (x);
		}
	      else if (function_invariant_p (x))
		{
		  if (GET_CODE (x) == PLUS)
		    {
		      /* This is PLUS of frame pointer and a constant,
			 and might be shared.  Unshare it.  */
		      if (flag_rematerialize)
			reg_equiv_constant [i] = copy_rtx (x);
		      eliminable_invariant_p = true;
		    }
		  else if (x == frame_pointer_rtx || x == arg_pointer_rtx)
		    {
		      if (flag_rematerialize)
			reg_equiv_constant [i] = x;
		      eliminable_invariant_p = true;
		    }
		  else if (LEGITIMATE_CONSTANT_P (x))
		    {
		      if (flag_rematerialize)
			reg_equiv_constant [i] = x;
		    }
#if 0
		  else
		    {
		      bool set_p = reg_equiv_memory_loc [i] == NULL;

		      reg_equiv_memory_loc [i]
			= force_const_mem (GET_MODE (SET_DEST (set)), x);
		      if (! reg_equiv_memory_loc [i])
			return false;
		      if (set_p)
			reg_equiv_memory_index [i] = equiv_memory_num++;
		    }
#endif
		}
	      else
		return false;
	      
	      /* If this register is being made equivalent to a MEM
		 and the MEM is not SET_SRC, the equivalencing insn is
		 one with the MEM as a SET_DEST and it occurs later.
		 So don't mark this insn now.  */
	      if (! MEM_P (x) || rtx_equal_p (SET_SRC (set), x))
		{
		  reg_equiv_set_p [i] = true;
#if 0
		  reg_equiv_init [i]
		    = gen_rtx_INSN_LIST (VOIDmode, insn, reg_equiv_init [i]);
#endif
		}
	    }
	}
    }
#if 0  
  /* If this insn is setting a MEM from a register equivalent to it,
     this is the equivalencing insn.  */
  else if (set && MEM_P (SET_DEST (set)) && REG_P (SET_SRC (set))
	   && reg_equiv_memory_loc [REGNO (SET_SRC (set))]
	   && rtx_equal_p (SET_DEST (set),
			   reg_equiv_memory_loc [REGNO (SET_SRC (set))]))
    reg_equiv_init [REGNO (SET_SRC (set))]
      = gen_rtx_INSN_LIST (VOIDmode, insn,
			   reg_equiv_init [REGNO (SET_SRC (set))]);
#endif
  return eliminable_invariant_p;
}


static void
initiate_equivs (void)
{
  rtx insn, bound;
  basic_block bb;

  reg_equiv_constant = yara_allocate (max_regno * sizeof (rtx));
  memset (reg_equiv_constant, 0, max_regno * sizeof (rtx));
  reg_equiv_mem = yara_allocate (max_regno * sizeof (rtx));
  memset (reg_equiv_mem, 0, max_regno * sizeof (rtx));
  reg_equiv_set_p = yara_allocate (max_regno * sizeof (bool));
  memset (reg_equiv_set_p, 0, max_regno * sizeof (bool));
#if 0
  reg_equiv_init = yara_allocate (max_regno * sizeof (rtx));
  memset (reg_equiv_init, 0, max_regno * sizeof (rtx));
#endif
  reg_equiv_address = yara_allocate (max_regno * sizeof (rtx));
  memset (reg_equiv_address, 0, max_regno * sizeof (rtx));
  reg_max_ref_size = yara_allocate (max_regno * sizeof (int));
  memset (reg_max_ref_size, 0, max_regno * sizeof (int));
  reg_max_ref_align = yara_allocate (max_regno * sizeof (int));
  memset (reg_max_ref_align, 0, max_regno * sizeof (int));
  reg_equiv_memory_index = yara_allocate (max_regno * sizeof (int));
  equiv_memory_num = 0;
  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  {
	    scan_rtx_for_reg_size_alignment (PATTERN (insn));
	    (void) /* ??? */ scan_insn_for_reg_equivs (insn);
	  }
    }
}

static void
finish_equivs (void)
{
  VARRAY_GROW (reg_equiv_memory_loc_varray, 0);
  reg_equiv_memory_loc = 0;
  yara_free (reg_equiv_memory_index);
  yara_free (reg_max_ref_align);
  yara_free (reg_max_ref_size);
  yara_free (reg_equiv_address);
  yara_free (reg_equiv_set_p);
#if 0
  yara_free (reg_equiv_init);
#endif
  yara_free (reg_equiv_mem);
  yara_free (reg_equiv_constant);
}



static void
print_hard_reg_set (FILE *f, HARD_REG_SET set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (set, i))
	fprintf (f, "%s ", reg_names [i]);
    }
}

void
print_point (FILE *f, struct point *p)
{
  if (p->point_type == BEFORE_INSN)
    fprintf (f, "bi%d", INSN_UID (p->u.insn));
  else if (p->point_type == AFTER_INSN)
    fprintf (f, "ai%d", INSN_UID (p->u.insn));
  else if (p->point_type == AT_BB_START)
    fprintf (f, "bbs%d", p->u.bb->index);
  else if (p->point_type == AT_BB_END)
    fprintf (f, "bbe%d", p->u.bb->index);
  else if (p->point_type == ON_EDGE_SRC)
    fprintf (f, "es[%d->%d:d%d]", p->u.e.e->src->index, p->u.e.e->dest->index,
	     p->u.e.loop_node->loop->depth);
  else
    {
      yara_assert (p->point_type == ON_EDGE_DST);
      fprintf (f, "ed[%d->%d:d%d]", p->u.e.e->src->index,
	       p->u.e.e->dest->index, p->u.e.loop_node->loop->depth);
    }
}



static int
point_freq (struct point *point)
{
  int freq;

  switch (point->point_type)
    {
    case BEFORE_INSN:
    case AFTER_INSN:
      freq = BLOCK_FOR_INSN (point->u.insn)->frequency;
      break;

    case AT_BB_START:
    case AT_BB_END:
      freq = point->u.bb->frequency;
      break;

    case ON_EDGE_SRC:
    case ON_EDGE_DST:
      freq = EDGE_FREQUENCY (point->u.e.e);
      break;

    default:
      gcc_unreachable ();
    }
  return (freq == 0 ? 1 : freq);
}



/* Varray containing references to allocnos.  */
static varray_type allocno_varray;
static int insn_allocnos_num, region_allocnos_num;
static int all_insn_allocnos_num = 0;
static int all_region_allocnos_num = 0;
static int all_allocnos_num = 0;

static void
initiate_allocnos (void)
{
  insn_allocnos_num = region_allocnos_num = 0;
  VARRAY_GENERIC_PTR_NOGC_INIT (allocno_varray, yara_max_uid * 5, "allocnos");
}

static allocno_t
create_allocno (enum allocno_type type, int regno, enum machine_mode mode)
{
  allocno_t a;
  int size;

  if (type == INSN_ALLOCNO)
    {
      size = sizeof (struct insn_allocno);
      insn_allocnos_num++;
    }
  else
    {
      size = sizeof (struct region_allocno);
      region_allocnos_num++;
    }
  a = yara_allocate (size);
  ALLOCNO_NUM (a) = VARRAY_ACTIVE_SIZE (allocno_varray);
  VARRAY_PUSH_GENERIC_PTR (allocno_varray, a);
  ALLOCNO_TYPE (a) = type;
  ALLOCNO_REGNO (a) = regno;
  ALLOCNO_MODE (a) = mode;
  ALLOCNO_HARD_REGNO (a) = -1;
  CLEAR_HARD_REG_SET (ALLOCNO_HARD_REGSET (a));
  ALLOCNO_MEMORY_SLOT (a) = NULL;
  ALLOCNO_MEMORY_SLOT_OFFSET (a) = 0;
  CLEAR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a));
  ALLOCNO_USE_EQUIV_CONST_P (a) = false;
  ALLOCNO_DST_COPIES (a) = ALLOCNO_SRC_COPIES (a) = NULL;
  ALLOCNO_CONFLICT_VEC (a) = NULL;
  ALLOCNO_CONFLICT_VEC_LEN (a) = 0;
  ALLOCNO_COPY_CONFLICT_VEC_LEN (a) = 0;
  ALLOCNO_COPY_CONFLICT_VEC (a) = NULL;
#ifdef CONFLICT_CACHE
  CLEAR_HARD_REG_SET (ALLOCNO_CACHED_CONFLICT_HARD_REGS (a));
  ALLOCNO_CACHE_VALID_P (a) = true;
#endif
  ALLOCNO_CALL_CROSS_P (a) = false;
  ALLOCNO_CALL_FREQ (a) = 0;
  CLEAR_HARD_REG_SET (ALLOCNO_CLOBBERED_REGS (a));
  return a;
}

static void
print_copy (FILE *f, copy_t cp, bool dst_p)
{
  int i;
  allocno_t a = (dst_p ? COPY_DST (cp) : COPY_SRC (cp));
  bool new_line_p = false;
  allocno_t *vec;

  fprintf (f, "%5d:", COPY_NUM (cp));
  if (a == NULL)
    fprintf (f, "    nil(");
  else
    fprintf (f, "%5d(", ALLOCNO_NUM (a));
  print_point (f, &COPY_POINT (cp));
#ifdef HAVE_SECONDARY_RELOADS
  if (COPY_CHANGE_ADDR (cp) != NULL)
    {
      if (COPY_INTERM_REGNO (cp) >= 0)
	fprintf (f, ", interm. %d(%s)", COPY_INTERM_REGNO (cp),
		 GET_MODE_NAME (COPY_INTERM_MODE (cp)));
      if (COPY_SCRATCH_REGNO (cp) >= 0)
	fprintf (f, ", scratch %d(%s)", COPY_SCRATCH_REGNO (cp),
		 GET_MODE_NAME (COPY_SCRATCH_MODE (cp)));
      fprintf (f, ", conflict hard regs=");
      print_hard_reg_set (f, COPY_HARD_REG_CONFLICTS (cp));
      new_line_p = true;
    }
#endif
  if (COPY_CHANGE_ADDR (cp) != NULL && COPY_INTERM_EQUIV_CONST_REGNO (cp) >= 0)
    fprintf (f, ", equiv interm. %d(%s)", COPY_INTERM_EQUIV_CONST_REGNO (cp),
	     GET_MODE_NAME (COPY_INTERM_EQUIV_CONST_MODE (cp)));
#ifdef SECONDARY_MEMORY_NEEDED
  if (COPY_CHANGE_ADDR (cp) != NULL)
    {
      if (COPY_USER_DEFINED_MEMORY (cp) != NULL_RTX)
	{
	  fprintf (f, ",\n     2nd memory rtx=");
	  print_inline_rtx (f, COPY_USER_DEFINED_MEMORY (cp), 26);
	}
      if (COPY_MEMORY_SLOT (cp) != NULL)
	print_memory_slot (f, ",\n    2nd memory slot", 0,
			   COPY_MEMORY_SLOT (cp));
      new_line_p = true;
    }
#endif
  fprintf (f, new_line_p ? ")\n" : ")");
  fprintf (f, "    Copy conflicts:");
  vec = COPY_ALLOCNO_CONFLICT_VEC (cp);
  if (vec != NULL)
    for (i = 0; (a = vec [i]) != NULL; i++)
      fprintf (f, "%5d", ALLOCNO_NUM (a));
  fprintf (f, "\n");
}

const char *
allocno_type_name (allocno_t a)
{
  return (ALLOCNO_TYPE (a) == INSN_ALLOCNO ? "insn" : "region");
}

static void
print_allocno (FILE *f, allocno_t a)
{
  int i;
  bool the_same_regno_p;
  allocno_t another_a;
  allocno_t *vec;
  copy_t cp;

  fprintf (f, "\n%s allocno #%d: reg %d, %s, hard %d, freq %d (call %d)%s",
	   allocno_type_name (a), ALLOCNO_NUM (a), ALLOCNO_REGNO (a),
	   GET_MODE_NAME (ALLOCNO_MODE (a)),
	   ALLOCNO_HARD_REGNO (a), ALLOCNO_FREQ (a), ALLOCNO_CALL_FREQ (a),
	   (ALLOCNO_CALL_CROSS_P (a) ? ", cross calls" : ""));
  if (ALLOCNO_TYPE (a) == REGION_ALLOCNO)
    {
      if (REGION_ALLOCNO_NODE (a)->bb == NULL)
	fprintf (f, " loop #%d (header %d)",
		 REGION_ALLOCNO_NODE (a)->loop->num,
		 REGION_ALLOCNO_NODE (a)->loop->header->index);
      else
	fprintf (f, " bb #%d", REGION_ALLOCNO_NODE (a)->bb->index);
      if (REGION_ALLOCNO_START_INSN (a) != NULL_RTX)
	fprintf (f, ", start = %d", INSN_UID (REGION_ALLOCNO_START_INSN (a)));
      if (REGION_ALLOCNO_STOP_INSN (a) != NULL_RTX)
	fprintf (f, ", stop = %d", INSN_UID (REGION_ALLOCNO_STOP_INSN (a)));
    }
  else
    {
      yara_assert (ALLOCNO_TYPE (a) == INSN_ALLOCNO);
      fprintf (f, " insn %d, ", INSN_UID (INSN_ALLOCNO_INSN (a)));
      if (INSN_ALLOCNO_BIGGEST_MODE (a) != ALLOCNO_MODE (a))
	fprintf (f, "bigmode = %s, ",
		 GET_MODE_NAME (INSN_ALLOCNO_BIGGEST_MODE (a)));
      if (INSN_ALLOCNO_OP_MODE (a) == OP_IN)
	fprintf (f, "In ");
      else if (INSN_ALLOCNO_OP_MODE (a) == OP_OUT)
	fprintf (f, "Out ");
      else
	fprintf (f, "InOut ");
      if (INSN_ALLOCNO_TYPE (a) == BASE_REG)
	fprintf (f, "Base reg");
      else if (INSN_ALLOCNO_TYPE (a) == INDEX_REG)
	fprintf (f, "Index reg");
      else if (INSN_ALLOCNO_TYPE (a) == NON_OPERAND)
	fprintf (f, "Non-operand");
      else
	fprintf (f, "Op #%d", INSN_ALLOCNO_TYPE (a) - OPERAND_BASE);
      if (INSN_ALLOCNO_COMMUTATIVE (a) != NULL)
	fprintf (f, ", commut. with %d",
		 ALLOCNO_NUM (INSN_ALLOCNO_COMMUTATIVE (a)));
      if (INSN_ALLOCNO_EARLY_CLOBBER (a))
	fprintf (f, ", early clobber");
      if (INSN_ALLOCNO_ELIMINATION_P (a))
	fprintf (f, ", elimination");
      if (INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a))
	fprintf (f, ", elim. const part");
      if (INSN_ALLOCNO_ELIMINATION (a) != NULL)
	fprintf (f, " to %d", INSN_ALLOCNO_ELIMINATION (a)->to);
      if (INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a) >= 0)
	fprintf (f, "(%d)", INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a));
      if (! EQ_ALT_SET (INSN_ALLOCNO_POSSIBLE_ALTS (a), ZERO_ALT_SET))
	{
	  int i;

	  fprintf (f, ", alts");
	  FOR_EACH_ALT (INSN_ALLOCNO_POSSIBLE_ALTS (a), i,
	    {
	      fprintf (f, " %d", i);
	    });
	}
      if (INSN_ALLOCNO_NEXT (a) != NULL)
	fprintf (f, ", next insn allocno %d,",
		 ALLOCNO_NUM (INSN_ALLOCNO_NEXT (a)));
      fprintf (f, "loc = ");
      print_inline_rtx (f, *INSN_ALLOCNO_LOC (a), 6);
    }
  if (ALLOCNO_MEMORY_SLOT (a) != NULL)
    print_memory_slot (f, ",\nmemory slot", 0, ALLOCNO_MEMORY_SLOT (a));
  if (ALLOCNO_MEMORY_SLOT_OFFSET (a) != 0)
    fprintf (f, ", + %d", ALLOCNO_MEMORY_SLOT_OFFSET (a));
  if (ALLOCNO_USE_EQUIV_CONST_P (a))
    fprintf (f, ",\nequiv const ");
  if (ALLOCNO_CALL_CROSS_P (a))
    {
      fprintf (f, "\n Call clobbered regs:");
      print_hard_reg_set (f, ALLOCNO_CLOBBERED_REGS (a));
    }
  fprintf (f, "\n Hard reg conflicts:");
  print_hard_reg_set (f, ALLOCNO_HARD_REG_CONFLICTS (a));
  fprintf (f, "\n Allocno conflicts:");
  vec = ALLOCNO_CONFLICT_VEC (a);
  if (vec != NULL)
    for (i = 0; (another_a = vec [i]) != NULL; i++)
      {
	fprintf (f, "%5d", ALLOCNO_NUM (another_a));
	the_same_regno_p = (ALLOCNO_REGNO (a) >= 0
			    && ALLOCNO_REGNO (a) == ALLOCNO_REGNO (another_a)
			    && ALLOCNO_MODE (a) == ALLOCNO_MODE (another_a));
	if (the_same_regno_p)
	  fprintf (f, "*");
      }
  fprintf (f, "\n  Copied to:\n");
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    print_copy (f, cp, true);
  fprintf (f, "\n  Copied from:\n");
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    print_copy (f, cp, false);
  fprintf (f, "\n");
}

void
debug_allocno (allocno_t a)
{
  print_allocno (stderr, a);
}

static void
print_allocnos (FILE *f)
{
  int i;

  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (allocno_varray); i++)
    print_allocno (f, VARRAY_GENERIC_PTR (allocno_varray, i));
}

void
debug_allocnos (void)
{
  print_allocnos (stderr);
}

static void
free_allocno (allocno_t a)
{
  if (ALLOCNO_CONFLICT_VEC (a) != NULL)
    yara_free (ALLOCNO_CONFLICT_VEC (a));
  if (ALLOCNO_COPY_CONFLICT_VEC (a) != NULL)
    yara_free (ALLOCNO_COPY_CONFLICT_VEC (a));
  yara_free (a);
}

static void
finish_allocnos (void)
{
  int i;
  allocno_t a;

  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (allocno_varray); i++)
    {
      a = VARRAY_GENERIC_PTR (allocno_varray, i);
      if (a != NULL)
	free_allocno (a);
    }
  VARRAY_FREE (allocno_varray);
}



/* Varray containing references to copies.  */
static varray_type copy_varray;

static varray_type temp_sort_varray;

static int all_copies_num = 0;

static void
initiate_copies (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (temp_sort_varray, 50,
				"array to sort list of copies");
  VARRAY_GENERIC_PTR_NOGC_INIT (copy_varray, yara_max_uid * 5, "copies");
  before_insn_copies = yara_allocate (yara_max_uid * sizeof (copy_t));
  memset (before_insn_copies, 0, yara_max_uid * sizeof (copy_t));
  after_insn_copies = yara_allocate (yara_max_uid * sizeof (copy_t));
  memset (after_insn_copies, 0, yara_max_uid * sizeof (copy_t));
  at_bb_start_copies = yara_allocate (last_basic_block * sizeof (copy_t));
  memset (at_bb_start_copies, 0, last_basic_block * sizeof (copy_t));
  at_bb_end_copies = yara_allocate (last_basic_block * sizeof (copy_t));
  memset (at_bb_end_copies, 0, last_basic_block * sizeof (copy_t));
  at_edge_start_copies = yara_allocate (n_edges * sizeof (copy_t));
  memset (at_edge_start_copies, 0, n_edges * sizeof (copy_t));
  at_edge_end_copies = yara_allocate (n_edges * sizeof (copy_t));
  memset (at_edge_end_copies, 0, n_edges * sizeof (copy_t));
}

static int all_copies, all_ident_copies;

static copy_t
create_copy (allocno_t dst, allocno_t src, struct point point, rtx insn)
{
  copy_t cp, prev_cp, next_cp;

  yara_assert (dst != NULL || src != NULL);
  all_copies++;
  if (dst == src)
    all_ident_copies++;
  yara_assert (dst == NULL || src == NULL
	       || (! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (src))
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst))));
  cp = yara_allocate (sizeof (struct copy));
  COPY_NUM (cp) = VARRAY_ACTIVE_SIZE (copy_varray);
  VARRAY_PUSH_GENERIC_PTR (copy_varray, cp);
  COPY_DST (cp) = dst;
  COPY_SRC (cp) = src;
  COPY_NEXT_DST_COPY (cp) = COPY_NEXT_SRC_COPY (cp) = NULL;
  if (dst != NULL)
    {
      COPY_NEXT_DST_COPY (cp) = ALLOCNO_DST_COPIES (dst);
      ALLOCNO_DST_COPIES (dst) = cp;
    }
  if (src != NULL)
    {
      COPY_NEXT_SRC_COPY (cp) = ALLOCNO_SRC_COPIES (src);
      ALLOCNO_SRC_COPIES (src) = cp;
    }
  COPY_POINT (cp) = point;
  COPY_ALLOCNO_CONFLICT_VEC (cp) = NULL;
  COPY_ALLOCNO_CONFLICT_VEC_LEN (cp) = 0;
  CLEAR_HARD_REG_SET (COPY_HARD_REG_CONFLICTS (cp));

  COPY_CHANGE_ADDR (cp) = NULL;

  COPY_FREQ (cp) = point_freq (&point);
  COPY_SYNC_P (cp) = false;
  COPY_SUBST_SRC_HARD_REGNO (cp) = -1;

  if (src != NULL && ALLOCNO_TYPE (src) == REGION_ALLOCNO)
    REGION_ALLOCNO_STOP_INSN (src) = insn;
  switch (point.point_type)
    {
    case BEFORE_INSN:
      COPY_NEXT_COPY (cp) = before_insn_copies [INSN_UID (point.u.insn)];
      before_insn_copies [INSN_UID (point.u.insn)] = cp;
      break;
    case AFTER_INSN:
      COPY_NEXT_COPY (cp) = after_insn_copies [INSN_UID (point.u.insn)];
      after_insn_copies [INSN_UID (point.u.insn)] = cp;
      break;
    case AT_BB_START:
      COPY_NEXT_COPY (cp) = at_bb_start_copies [point.u.bb->index];
      at_bb_start_copies [point.u.bb->index] = cp;
      break;
    case AT_BB_END:
      /* Insn allocnos can be copied at the end of basic block if the
	 block finishes at a jump.  We prefer to put copies from insn
	 allocnos first on the list.  */
      for (prev_cp = NULL, next_cp = at_bb_end_copies [point.u.bb->index];
	   next_cp != NULL
	     && ALLOCNO_TYPE (COPY_SRC (next_cp)) == INSN_ALLOCNO;
	   prev_cp = next_cp, next_cp = COPY_NEXT_COPY (next_cp))
	;
      if (prev_cp == NULL)
	{
	  COPY_NEXT_COPY (cp) = at_bb_end_copies [point.u.bb->index];
	  at_bb_end_copies [point.u.bb->index] = cp;
	}
      else
	{
	  COPY_NEXT_COPY (prev_cp) = cp;
	  COPY_NEXT_COPY (cp) = next_cp;
	}
      break;
    case ON_EDGE_SRC:
      COPY_NEXT_COPY (cp) = at_edge_start_copies [(size_t) point.u.e.e->aux];
      at_edge_start_copies [(size_t) point.u.e.e->aux] = cp;
      break;
    case ON_EDGE_DST:
      COPY_NEXT_COPY (cp) = at_edge_end_copies [(size_t) point.u.e.e->aux];
      at_edge_end_copies [(size_t) point.u.e.e->aux] = cp;
      break;
    default:
      gcc_unreachable ();
    }
  return cp;
}

static copy_t
find_copy (allocno_t dst, allocno_t src)
{
  copy_t cp;

  for (cp = ALLOCNO_SRC_COPIES (src); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    if (COPY_DST (cp) == dst)
      break;
#ifdef ENABLE_YARA_CHECKING
  if (cp != NULL)
    {
      copy_t next_cp;
      
      /* Check that we have no more copy with the same source and
	 destination.  */
      for (next_cp = COPY_NEXT_SRC_COPY (cp);
	   next_cp != NULL;
	   next_cp = COPY_NEXT_SRC_COPY (next_cp))
	if (COPY_DST (next_cp) == dst)
	  gcc_unreachable ();
    }
#endif
  return cp;
}

static copy_t
sort_copy_list (copy_t list,
		int (*copy_compare_func) (const void *, const void *))
{
  copy_t cp;
  int i;

  if (list == NULL)
    return NULL;
  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    VARRAY_PUSH_GENERIC_PTR (temp_sort_varray, cp);
  qsort (&VARRAY_GENERIC_PTR (temp_sort_varray, 0),
	 VARRAY_ACTIVE_SIZE (temp_sort_varray),
	 sizeof (copy_t), copy_compare_func);
  list = VARRAY_GENERIC_PTR (temp_sort_varray, 0);
  for (i = 1; i < (int) VARRAY_ACTIVE_SIZE (temp_sort_varray); i++)
    COPY_NEXT_COPY (((copy_t) VARRAY_GENERIC_PTR (temp_sort_varray, i - 1)))
      = VARRAY_GENERIC_PTR (temp_sort_varray, i);
  COPY_NEXT_COPY (((copy_t) VARRAY_GENERIC_PTR (temp_sort_varray, i - 1)))
    = NULL;
  VARRAY_POP_ALL (temp_sort_varray);
  return list;
}

static void
free_copy (copy_t cp)
{
  if (COPY_ALLOCNO_CONFLICT_VEC (cp) != NULL)
    yara_free (COPY_ALLOCNO_CONFLICT_VEC (cp));
  yara_free (cp);
}

static void
finish_copies (void)
{
  int i;
  copy_t cp;

  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (copy_varray); i++)
    {
      cp = VARRAY_GENERIC_PTR (copy_varray, i);
      if (cp != NULL)
	free_copy (cp);
    }
  VARRAY_FREE (copy_varray);
  yara_free (at_edge_end_copies);
  yara_free (at_edge_start_copies);
  yara_free (at_bb_end_copies);
  yara_free (at_bb_start_copies);
  yara_free (after_insn_copies);
  yara_free (before_insn_copies);
  VARRAY_FREE (temp_sort_varray);
}



/* Element of the following array is false if each hard register is of
   enough size to contain value of the corresponding mode.  */
static bool mode_multi_reg_p [MAX_MACHINE_MODE];

static void
setup_mode_multi_reg_p (void)
{
  int i, mode;

  for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
    mode_multi_reg_p [mode] = false;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    for (mode = 0; mode < MAX_MACHINE_MODE; mode++)
      if (hard_regno_nregs [i] [mode] > 1)
	mode_multi_reg_p [mode] = true;
}



/* Varray of pending conflicts.  */
static varray_type pending_allocno_conflict_varray;
static varray_type pending_allocno_copy_conflict_varray;

static int conflicts_num, copy_conflicts_num;
static int all_conflicts_num, all_copy_conflicts_num;

static void
initiate_conflicts (void)
{
  conflicts_num = copy_conflicts_num = 0;
  VARRAY_GENERIC_PTR_NOGC_INIT (pending_allocno_conflict_varray,
				yara_max_uid * 100,
				"pending allocno conflicts");
  VARRAY_GENERIC_PTR_NOGC_INIT (pending_allocno_copy_conflict_varray,
				yara_max_uid * 100,
				"pending allocno copy conflicts");
}

bool
conflict_p (allocno_t a1, allocno_t a2)
{
  int i;
  allocno_t another_a;
  allocno_t *vec;

  if (a1 == a2)
    return false;
  vec = ALLOCNO_CONFLICT_VEC (a1);
  if (vec != NULL)
    for (i = 0; (another_a = vec [i]) != NULL; i++)
      if (another_a == a2)
	return true;
  return false;
}

static void
create_conflict (allocno_t a1, allocno_t a2)
{
  allocno_t temp_a;
  int regno1;

  if (a1 == a2)
    return;
  if ((regno1 = ALLOCNO_REGNO (a1)) >= 0 && regno1 == ALLOCNO_REGNO (a2))
    {
      if ((ALLOCNO_TYPE (a1) != INSN_ALLOCNO
	   && ALLOCNO_TYPE (a2) != INSN_ALLOCNO)
	  || (ALLOCNO_TYPE (a1) == INSN_ALLOCNO
	      && ALLOCNO_TYPE (a2) == INSN_ALLOCNO
	      && ! INSN_ALLOCNO_EARLY_CLOBBER (a1)
	      && ! INSN_ALLOCNO_EARLY_CLOBBER (a2)
	      && rtx_equal_p (*INSN_ALLOCNO_LOC (a1), *INSN_ALLOCNO_LOC (a2))
	      && (HARD_REGISTER_NUM_P (regno1)
		  || ! mode_multi_reg_p [ALLOCNO_MODE (a1)]))
	  || (ALLOCNO_TYPE (a1) == INSN_ALLOCNO
	      && ALLOCNO_TYPE (a2) != INSN_ALLOCNO
	      && REG_P (*INSN_ALLOCNO_LOC (a1))
	      && ! mode_multi_reg_p [ALLOCNO_MODE (a1)])
	  || (ALLOCNO_TYPE (a1) != INSN_ALLOCNO
	      && ALLOCNO_TYPE (a2) == INSN_ALLOCNO
	      && REG_P (*INSN_ALLOCNO_LOC (a2))
	      && ! mode_multi_reg_p [ALLOCNO_MODE (a1)]))
	return;
    }
  if (a1 > a2)
    {
      temp_a = a1;
      a1 = a2;
      a2 = temp_a;
    }
  VARRAY_PUSH_GENERIC_PTR (pending_allocno_conflict_varray, a1);
  VARRAY_PUSH_GENERIC_PTR (pending_allocno_conflict_varray, a2);
  ALLOCNO_CONFLICT_VEC_LEN (a1)++;
  ALLOCNO_CONFLICT_VEC_LEN (a2)++;
  return;
}

static void
commit_conflicts (void)
{
  int i, j, n;
  int *check_allocno;
  allocno_t a1, a2, *vec;
  int a2_num;
  void **conflicts;

  conflicts
    = (void **)&VARRAY_GENERIC_PTR (pending_allocno_conflict_varray, 0);
  for (i = 0; i < allocnos_num; i++)
    {
      a1 = allocnos [i];
      n = ALLOCNO_CONFLICT_VEC_LEN (a1);
      ALLOCNO_CONFLICT_VEC_LEN (a1) = 0;
      ALLOCNO_CONFLICT_VEC (a1) = yara_allocate ((n + 1) * sizeof (allocno_t));
      ALLOCNO_CONFLICT_VEC (a1)	[n] = NULL;
    }
  for (i = VARRAY_ACTIVE_SIZE (pending_allocno_conflict_varray) - 2;
       i >= 0;
       i -= 2)
    {
      a1 = conflicts [i];
      a2 = conflicts [i + 1];
      ALLOCNO_CONFLICT_VEC (a1) [ALLOCNO_CONFLICT_VEC_LEN (a1)++] = a2;
      ALLOCNO_CONFLICT_VEC (a2) [ALLOCNO_CONFLICT_VEC_LEN (a2)++] = a1;
    }
  check_allocno = yara_allocate (allocnos_num * sizeof (int));
  for (i = 0; i < allocnos_num; i++)
    check_allocno [i] = -1;
  /* We might have duplicated conflicts for region allocnos because
     our algorithm of conflicts building.  Remove the duplicates.  It
     does not change result but it speeds conflict processing up
     during subsequent allocation.  */
  conflicts_num = 0;
  for (i = 0; i < allocnos_num; i++)
    {
      a1 = allocnos [i];
      if (ALLOCNO_TYPE (a1) == INSN_ALLOCNO)
	continue;
      vec = ALLOCNO_CONFLICT_VEC (a1);
      for (n = j = 0; (a2 = vec [j]) != NULL; j++)
	{
	  a2_num = ALLOCNO_NUM (a2);
	  if (check_allocno [a2_num] != i)
	    {
	      check_allocno [a2_num] = i;
	      vec [n++] = a2;
	    }
	  else
	    conflicts_num++;
	}
      vec [n] = NULL;
    }
  conflicts_num = ((int) VARRAY_ACTIVE_SIZE (pending_allocno_conflict_varray)
		   - conflicts_num) / 2;
  yara_free (check_allocno);
  if (yara_dump_file != NULL)
    fprintf (yara_dump_file,
	     "Allocno conflicts (pending/final) %d/%d=%d%%\n",
	     (int) VARRAY_ACTIVE_SIZE (pending_allocno_conflict_varray) / 2,
	     conflicts_num,
	     (int) VARRAY_ACTIVE_SIZE (pending_allocno_conflict_varray) * 50
	     / (conflicts_num ? conflicts_num : 1));
  VARRAY_FREE (pending_allocno_conflict_varray);
}

static void
create_copy_conflict (allocno_t a, copy_t cp)
{
 VARRAY_PUSH_GENERIC_PTR (pending_allocno_copy_conflict_varray, a);
 VARRAY_PUSH_GENERIC_PTR (pending_allocno_copy_conflict_varray, cp);
 ALLOCNO_COPY_CONFLICT_VEC_LEN (a)++;
 COPY_ALLOCNO_CONFLICT_VEC_LEN (cp)++;
 return;
}

static void
commit_copy_conflicts (void)
{
  int i, n;
  allocno_t a;
  copy_t cp;
  void **conflicts;

  conflicts = &VARRAY_GENERIC_PTR (pending_allocno_copy_conflict_varray, 0);
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      n = ALLOCNO_COPY_CONFLICT_VEC_LEN (a);
      ALLOCNO_COPY_CONFLICT_VEC_LEN (a) = 0;
      ALLOCNO_COPY_CONFLICT_VEC (a)
	= yara_allocate ((n + 1) * sizeof (copy_t));
      ALLOCNO_COPY_CONFLICT_VEC (a) [n] = NULL;
    }
  for (i = 0; i < copies_num; i++)
    {
      cp = copies [i];
      n = COPY_ALLOCNO_CONFLICT_VEC_LEN (cp);
      COPY_ALLOCNO_CONFLICT_VEC_LEN (cp) = 0;
      COPY_ALLOCNO_CONFLICT_VEC (cp)
	= yara_allocate ((n + 1) * sizeof (allocno_t));
      COPY_ALLOCNO_CONFLICT_VEC (cp) [n] = NULL;
    }
  for (i = VARRAY_ACTIVE_SIZE (pending_allocno_copy_conflict_varray) - 2;
       i >= 0;
       i -= 2)
    {
      a = conflicts [i];
      cp = conflicts [i + 1];
      ALLOCNO_COPY_CONFLICT_VEC (a) [ALLOCNO_COPY_CONFLICT_VEC_LEN (a)++] = cp;
      COPY_ALLOCNO_CONFLICT_VEC (cp) [COPY_ALLOCNO_CONFLICT_VEC_LEN (cp)++]
	= a;
    }
  copy_conflicts_num
    = (int) VARRAY_ACTIVE_SIZE (pending_allocno_copy_conflict_varray) / 2;
  if (yara_dump_file != NULL)
    fprintf (yara_dump_file, "Copy conflicts %d\n", copy_conflicts_num);
  VARRAY_FREE (pending_allocno_copy_conflict_varray);
}

static void
finish_conflicts (void)
{
}



static varray_type free_regno_allocno_maps;

/* The following is array of new basic block allocnos for
   pseudo-registers used, defined, or modified in insn which live
   after the insn  */
static allocno_t *new_regno_allocno_map;

/* The following array is used for checking that we defined a new
   region allocno after the insn for given pseudo-register.  If we
   defined the new allocno, the array element corresponding to given
   pseudo-register will have value equal to the insn 2 * uid for
   output allocno and 2 * uid + 1 for input allocno.  */
static int *new_regno_allocno_map_check;

static void
create_regno_allocno_maps (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (free_regno_allocno_maps, 50,
				"free regno allocno maps");
  new_regno_allocno_map = yara_allocate (sizeof (allocno_t) * max_regno);
  memset (new_regno_allocno_map, 0, sizeof (allocno_t) * max_regno);
  new_regno_allocno_map_check = yara_allocate (sizeof (int) * max_regno);
  memset (new_regno_allocno_map_check, 0, sizeof (int) * max_regno);
}

static void
delete_regno_allocno_maps (void)
{
  int i;

  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (free_regno_allocno_maps); i++)
    yara_free (VARRAY_GENERIC_PTR (free_regno_allocno_maps, i));
  VARRAY_FREE (free_regno_allocno_maps);
  yara_free (new_regno_allocno_map_check);
  yara_free (new_regno_allocno_map);
}

static void
allocate_loop_regno_allocno_map (struct yara_loop_tree_node *node)
{
  allocno_t *map;
  size_t size;

  size = VARRAY_ACTIVE_SIZE (free_regno_allocno_maps);
  if (size == 0)
    map = yara_allocate (sizeof (allocno_t) * max_regno);
  else
    {
      map = VARRAY_GENERIC_PTR (free_regno_allocno_maps, size - 1);
      VARRAY_POP (free_regno_allocno_maps);
    }
  memset (map, 0, sizeof (allocno_t) * max_regno);
  node->regno_allocno_map = map;
}

static void
free_loop_regno_allocno_map (struct yara_loop_tree_node *node)
{
  VARRAY_PUSH_GENERIC_PTR (free_regno_allocno_maps, node->regno_allocno_map);
  node->regno_allocno_map = NULL;
}



static void
create_insn_maps (void)
{
  insn_code_infos = yara_allocate (sizeof (struct insn_op_info *)
				   * (CODE_FOR_nothing + 1));
  memset (insn_code_infos, 0,
	  sizeof (struct insn_op_info *) * (CODE_FOR_nothing + 1));
  insn_infos = yara_allocate (sizeof (struct insn_op_info *) * yara_max_uid);
  memset (insn_infos, 0, sizeof (struct insn_op_info *) * yara_max_uid);
  insn_op_allocnos = yara_allocate (sizeof (allocno_t *) * yara_max_uid);
  memset (insn_op_allocnos, 0, sizeof (allocno_t *) * yara_max_uid);
  insn_allocnos = yara_allocate (sizeof (allocno_t) * yara_max_uid);
  memset (insn_allocnos, 0, sizeof (allocno_t) * yara_max_uid);
}

static void
finish_insn_maps (void)
{
  int i;
    
  yara_free (insn_allocnos);
  for (i = 0; i < yara_max_uid; i++)
    if (insn_op_allocnos [i] != NULL)
      yara_free (insn_op_allocnos [i]);
  yara_free (insn_op_allocnos);
  for (i = 0; i < yara_max_uid; i++)
    if (insn_infos [i] != NULL && insn_infos [i]->code < 0
	&& insn_infos [i]->op_constraints != NULL)
      yara_free (insn_infos [i]->op_constraints);
  yara_free (insn_infos);
  for (i = 0; i <= CODE_FOR_nothing; i++)
    if (insn_code_infos [i] != NULL
	&& insn_code_infos [i]->op_constraints != NULL)
      {
	yara_assert (i == insn_code_infos [i]->code);
	yara_free (insn_code_infos [i]->op_constraints);
      }
  yara_free (insn_code_infos);
}




/* List of allocnos of the current insn.  */
static allocno_t curr_insn_allocnos;


static allocno_t
create_insn_allocno (enum op_type op_mode, int type, int regno,
		     enum machine_mode mode, rtx insn, rtx *loc,
		     rtx *container_loc, bool early_clobber,
		     allocno_t addr_output_allocno)
{
  allocno_t a;
  rtx x, set;

  yara_assert ((type != BASE_REG && type != INDEX_REG) || op_mode != OP_OUT);
  a = create_allocno (INSN_ALLOCNO, regno, mode);
  for (x = GET_CODE (*container_loc) == SUBREG ? *container_loc : *loc;
       GET_CODE (x) == STRICT_LOW_PART || GET_CODE (x) == SUBREG
	 || GET_CODE (x) == ZERO_EXTRACT;
       x = XEXP (x, 0))
    if (GET_CODE (x) == SUBREG
	&& GET_MODE_SIZE (GET_MODE (x)) > GET_MODE_SIZE (mode))
      mode = GET_MODE (*container_loc);
  if (op_mode != OP_IN && regno >= 0
      && reg_equiv_memory_loc [regno] != NULL_RTX
      && ! HARD_REGISTER_NUM_P (regno)
      && reg_equiv_set_p [regno])
    {
      if ((set = single_set (insn)) == NULL_RTX)
	reg_equiv_set_p [regno] = false;
      else
	{
	  yara_assert (REG_P (SET_DEST (set))
		       && REGNO (SET_DEST (set)) == regno)
	    if (! rtx_equal_p (SET_SRC (set), reg_equiv_memory_loc [regno]))
	      reg_equiv_set_p [regno] = false;
	}
    }
  INSN_ALLOCNO_BIGGEST_MODE (a) = mode;
  INSN_ALLOCNO_OP_MODE (a) = op_mode;
  INSN_ALLOCNO_TYPE (a) = type;
  INSN_ALLOCNO_INSN (a) = insn;
  INSN_ALLOCNO_LOC (a) = loc;
  INSN_ALLOCNO_CONTAINER_LOC (a) = container_loc;
  INSN_ALLOCNO_COMMUTATIVE (a) = NULL;
  INSN_ALLOCNO_EARLY_CLOBBER (a) = early_clobber;
  INSN_ALLOCNO_ELIMINATION_P (a) = contains_eliminable_reg (*loc);
  INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a) = false;
  CLEAR_HARD_REG_SET (INSN_ALLOCNO_INTERM_ELIMINATION_REGSET (a));
  INSN_ALLOCNO_ELIMINATION (a) = NULL;
  INSN_ALLOCNO_INTERM_ELIMINATION_REGNO (a) = -1;
  CLEAR_ALT_SET (INSN_ALLOCNO_POSSIBLE_ALTS (a));
  INSN_ALLOCNO_ADDR_OUTPUT_ALLOCNO (a) = addr_output_allocno;
  INSN_ALLOCNO_ORIGINAL_P (a) = false;
  INSN_ALLOCNO_CONST_POOL_P (a) = false;
  INSN_ALLOCNO_USE_WITHOUT_CHANGE_P (a) = false;
  INSN_ALLOCNO_NEXT (a) = curr_insn_allocnos;
  curr_insn_allocnos = a;
  return a;
}

/* In (1 + 2) + 3 make constant last (3rd).  */
void
make_const_last (rtx **first_loc_ptr, rtx **second_loc_ptr,
		 rtx **third_loc_ptr)
{
  rtx *tmp;
  enum rtx_code code;
    
  code = GET_CODE (**first_loc_ptr);
  if (code == CONST_INT || code == SYMBOL_REF
      || code == LABEL_REF || code == CONST)
    {
      tmp = *first_loc_ptr;
      *first_loc_ptr = *second_loc_ptr;
      *second_loc_ptr = *third_loc_ptr;
      *third_loc_ptr = tmp;
    }
  else
    {
      code = GET_CODE (**second_loc_ptr);
      if (code == CONST_INT || code == SYMBOL_REF
	  || code == LABEL_REF || code == CONST)
	{
	  tmp = *second_loc_ptr;
	  *second_loc_ptr = *third_loc_ptr;
	  *third_loc_ptr = tmp;
	}
    }
}

void
make_canon_plus (rtx **first_loc_ptr, rtx **second_loc_ptr)
{
  enum rtx_code code;
  rtx *tmp;
  
  if (GET_CODE (**second_loc_ptr) == MULT
      || (code = GET_CODE (**first_loc_ptr)) == CONST_INT || code == SYMBOL_REF
      || code == LABEL_REF || code == CONST || code == UNSPEC)
    {
      tmp = *second_loc_ptr;
      *second_loc_ptr = *first_loc_ptr;
      *first_loc_ptr = tmp;
    }
}

static rtx *
get_through_addr_subreg (rtx *x)
{
  if (GET_CODE (*x) == SUBREG)
    {
      yara_assert (SUBREG_BYTE (*x) == 0 && REG_P (SUBREG_REG (*x)));
      x = &SUBREG_REG (*x);
    }
  return x;
}

#if 1

static bool
decode_address_1 (rtx *loc, rtx **container_loc_ptr,
		  rtx **base_reg_loc_ptr, rtx **disp_loc,
		  rtx **index_reg_loc_ptr, HOST_WIDE_INT *scale,
		  bool no_subreg_p, bool in_mult_p)
{
  int i;
  const char *fmt;
  enum rtx_code code;

  if (! no_subreg_p)
    loc = get_through_addr_subreg (loc);
  code = GET_CODE (*loc);
  if (code == UNSPEC)
    return true;
  if (code == MEM)
    {
      if (*container_loc_ptr != NULL)
	return false;
      *container_loc_ptr = loc;
      return decode_address_1 (&XEXP (*loc, 0), container_loc_ptr,
			       base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			       scale, no_subreg_p, in_mult_p);
    }
  if (code == REG || (code == SUBREG && REG_P (SUBREG_REG (*loc))))
    {
      if (in_mult_p)
	{
	  if (*index_reg_loc_ptr != NULL)
	    return false;
	  *index_reg_loc_ptr = loc;
	}
      else if (*base_reg_loc_ptr != NULL)
	{
	  if (*index_reg_loc_ptr != NULL)
	    return false;
	  *index_reg_loc_ptr = loc;
	}
      else
	*base_reg_loc_ptr = loc;
      return true;
    }
  else if (code == PLUS)
    {
      if (in_mult_p)
	return false;
      if (! decode_address_1 (&XEXP (*loc, 0), container_loc_ptr,
			      base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			      scale, no_subreg_p, false))
	return false;
      return decode_address_1 (&XEXP (*loc, 1), container_loc_ptr,
			       base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			       scale, no_subreg_p, false);
    }
  else if (code == MULT)
    {
      if (in_mult_p)
	return false;
      if (! decode_address_1 (&XEXP (*loc, 0), container_loc_ptr,
			      base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			      scale, no_subreg_p, true))
	return false;
      return decode_address_1 (&XEXP (*loc, 1), container_loc_ptr,
			       base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			       scale, no_subreg_p, true);
    }
  else if (code == CONST_INT)
    {
      if (in_mult_p)
	*scale = INTVAL (*loc);
      else if (*disp_loc != NULL)
	return false;
      else
	*disp_loc = loc;
      return true;
    }
  else if (code == CONST)
    {
      if (*disp_loc != NULL)
	return false;
      *disp_loc = loc;
      return true;
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (! decode_address_1 (&XEXP (*loc, i), container_loc_ptr,
				  base_reg_loc_ptr, disp_loc,
				  index_reg_loc_ptr,
				  scale, no_subreg_p, in_mult_p))
	    return false;
	}
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    {
	      if (! decode_address_1 (&XVECEXP (*loc, i, j), container_loc_ptr,
				      base_reg_loc_ptr, disp_loc,
				      index_reg_loc_ptr,
				      scale, no_subreg_p, in_mult_p))
		return false;
	    }
	}
    }
  return true;
}

bool
decode_address (rtx *addr_ptr, rtx **container_loc_ptr,
		rtx **base_reg_loc_ptr, rtx **disp_loc,
		rtx **index_reg_loc_ptr, HOST_WIDE_INT *scale,
		bool no_subreg_p)
{
  rtx *temp_container_loc;

  temp_container_loc = NULL;
  *base_reg_loc_ptr = NULL;
  *disp_loc = NULL;
  *index_reg_loc_ptr = NULL;
  *scale = 1;
  if (! decode_address_1 (addr_ptr, &temp_container_loc,
			  base_reg_loc_ptr, disp_loc,
			  index_reg_loc_ptr, scale, no_subreg_p, false))
    return false;
  if (temp_container_loc != NULL)
    *container_loc_ptr = temp_container_loc;
  if (*base_reg_loc_ptr != NULL && *index_reg_loc_ptr != NULL
      && *scale == 1 && (REGNO (**index_reg_loc_ptr) == FRAME_POINTER_REGNUM
			 || REGNO (**index_reg_loc_ptr) == ARG_POINTER_REGNUM))
    {
      /* We prefer to use eliminated register as the base register
	 because probability that REGNO_OK_FOR_BASE_P returns true for
	 the register is more than for REGNO_OK_FOR_INDEX_P.  */
      rtx *tmp = *base_reg_loc_ptr;
      
      *base_reg_loc_ptr = *index_reg_loc_ptr;
      *index_reg_loc_ptr = tmp;
    }
  return true;
}

#else

bool
decode_address (rtx *addr_ptr, rtx **container_loc_ptr,
		rtx **base_reg_loc_ptr, rtx **disp_loc,
		rtx **index_reg_loc_ptr, HOST_WIDE_INT *scale,
		bool no_subreg_p)
{
  enum rtx_code code;

  /* We assume that the base and index registers are not in
     subresgisters.  */
  if (! no_subreg_p)
    addr_ptr = get_through_addr_subreg (addr_ptr);
  code = GET_CODE (*addr_ptr);
  *base_reg_loc_ptr = NULL;
  *index_reg_loc_ptr = NULL;
  *disp_loc = NULL;
  *scale = 1;
  if (code == REG)
    *base_reg_loc_ptr = addr_ptr;
  else if (code == CONST_INT || code == SYMBOL_REF
	   || code == LABEL_REF || code == CONST || code == UNSPEC)
    *disp_loc = addr_ptr;
  else if (code == MULT)
    {
      rtx *reg_ptr = &XEXP (*addr_ptr, 0);
      
      if (! no_subreg_p)
	reg_ptr = get_through_addr_subreg (reg_ptr);
      if (! REG_P (*reg_ptr)
	  || GET_CODE (XEXP (*addr_ptr, 1)) != CONST_INT)
	return false;
      *index_reg_loc_ptr = reg_ptr;
      *scale = INTVAL (XEXP (*addr_ptr, 1));
    }
  else if (code == PLUS)
    {
      rtx *first_loc = &XEXP (*addr_ptr, 0);
      rtx *second_loc = &XEXP (*addr_ptr, 1);

      if (! no_subreg_p)
	first_loc = get_through_addr_subreg (first_loc);
      if (! no_subreg_p)
	second_loc = get_through_addr_subreg (second_loc);
      make_canon_plus (&first_loc, &second_loc);
      if (((code = GET_CODE (*first_loc)) == CONST_INT || code == SYMBOL_REF
	   || code == LABEL_REF || code == CONST || code == UNSPEC)
	  && ((code = GET_CODE (*second_loc)) == CONST_INT
	      || code == SYMBOL_REF || code == LABEL_REF
	      || code == CONST || code == UNSPEC))
	*disp_loc = addr_ptr;
      else if (REG_P (*first_loc))
	{
	  *base_reg_loc_ptr = first_loc;
	  code = GET_CODE (*second_loc);
	  if (REG_P (*second_loc))
	    *index_reg_loc_ptr = second_loc;
	  else if (code == CONST_INT || code == SYMBOL_REF
		   || code == LABEL_REF || code == CONST || code == UNSPEC)
	    *disp_loc = second_loc;
	  else
	    return false;
	}
      else if (GET_CODE (*first_loc) == MULT)
	{
	  rtx *reg_ptr = &XEXP (*first_loc, 0);
	  
	  if (! no_subreg_p)
	    reg_ptr = get_through_addr_subreg (reg_ptr);
	  if (! REG_P (*reg_ptr)
	      || GET_CODE (XEXP (*first_loc, 1)) != CONST_INT)
	    return false;
	  *index_reg_loc_ptr = reg_ptr;
	  *scale = INTVAL (XEXP (*first_loc, 1));
	  code = GET_CODE (*second_loc);
	  if (REG_P (*second_loc))
	    *base_reg_loc_ptr = second_loc;
	  else if (code == CONST_INT || code == SYMBOL_REF
		   || code == LABEL_REF || code == CONST || code == UNSPEC)
	    *disp_loc = second_loc;
	  else
	    return false;
	}
      else if (GET_CODE (*first_loc) == PLUS)
	{
	  rtx *third_loc;

	  third_loc = second_loc;
	  second_loc = &XEXP (*first_loc, 1);
	  first_loc = &XEXP (*first_loc, 0);
	  if (! no_subreg_p)
	    first_loc = get_through_addr_subreg (first_loc);
	  if (! no_subreg_p)
	    second_loc = get_through_addr_subreg (second_loc);
	  if (! no_subreg_p)
	    third_loc = get_through_addr_subreg (third_loc);
	  make_const_last (&first_loc, &second_loc, &third_loc);
	  code = GET_CODE (*third_loc);
	  if (code != CONST_INT && code != SYMBOL_REF
	      && code != LABEL_REF && code != CONST && code != UNSPEC)
	    return false;
	  *disp_loc = third_loc;
	  make_canon_plus (&first_loc, &second_loc);
	  if (REG_P (*first_loc))
	    {
	      if (! REG_P (*second_loc))
		return false;
	      *base_reg_loc_ptr = first_loc;
	      *index_reg_loc_ptr = second_loc;
	    }
	  else if (GET_CODE (*first_loc) != MULT)
	    return false;
	  else
	    {
	      rtx *reg_ptr = &XEXP (*first_loc, 0);

	      if (! no_subreg_p)
		reg_ptr = get_through_addr_subreg (reg_ptr);
	      if (! REG_P (*reg_ptr)
		  || (GET_CODE (XEXP (*first_loc, 1)) != CONST_INT)
		  || ! REG_P (*second_loc))
		return false;
	      else
		{
		  *base_reg_loc_ptr = second_loc;
		  *index_reg_loc_ptr = reg_ptr;
		  *scale = INTVAL (XEXP (*first_loc, 1));
		}
	    }
	}
      else
	return false;
    }
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC)
    {
      rtx *reg_ptr = &XEXP (*addr_ptr, 0);

      if (! no_subreg_p)
	reg_ptr = get_through_addr_subreg (reg_ptr);
      if (! REG_P (*reg_ptr))
	return false;
      *base_reg_loc_ptr = reg_ptr;
    }
  else if (code == POST_MODIFY || code == PRE_MODIFY)
    {
      rtx *reg_ptr = &XEXP (*addr_ptr, 0);

      if (! no_subreg_p)
	reg_ptr = get_through_addr_subreg (reg_ptr);
      /* ??? We does not support memory here.  I don't know such
	 ports.  */
      if (! REG_P (*reg_ptr)
	  || GET_CODE (XEXP (*addr_ptr, 1)) != CONST_INT)
	return false;
      *base_reg_loc_ptr = reg_ptr;
      *disp_loc = &XEXP (*addr_ptr, 1);
    }
  else if (code == MEM)
    {
      *container_loc_ptr = addr_ptr;
      return decode_address (&XEXP (*addr_ptr, 0), container_loc_ptr,
			     base_reg_loc_ptr, disp_loc, index_reg_loc_ptr,
			     scale, no_subreg_p);
    }
  else
    return false;
  if (*base_reg_loc_ptr != NULL && *index_reg_loc_ptr != NULL
      && *scale == 1 && (REGNO (**index_reg_loc_ptr) == FRAME_POINTER_REGNUM
			 || REGNO (**index_reg_loc_ptr) == ARG_POINTER_REGNUM))
    {
      /* We prefer to use eliminated register as the base register
	 because probability that REGNO_OK_FOR_BASE_P returns true for
	 the register is more than for REGNO_OK_FOR_INDEX_P.  */
      rtx *tmp = *base_reg_loc_ptr;
      
      *base_reg_loc_ptr = *index_reg_loc_ptr;
      *index_reg_loc_ptr = tmp;
    }
  return true;
}

#endif

static void
process_address_for_allocno (rtx insn, rtx *loc_addr, rtx *container_loc,
			     allocno_t out_allocno)
{
  enum rtx_code code;
  rtx *base_reg_loc, *disp_loc, *index_reg_loc;
  HOST_WIDE_INT scale;
  bool inc_p;
  
  if (! decode_address (loc_addr, &container_loc, &base_reg_loc, &disp_loc,
			&index_reg_loc, &scale, false))
    gcc_unreachable ();
  code = GET_CODE (*loc_addr);
  inc_p = (code == PRE_DEC || code == POST_DEC || code == PRE_INC
	   || code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY);
  if (base_reg_loc != NULL)
    (void) create_insn_allocno ((inc_p ? OP_INOUT : OP_IN), BASE_REG,
				REGNO (*base_reg_loc),
				GET_MODE (*base_reg_loc), insn, base_reg_loc,
				container_loc, false, out_allocno);
  if (index_reg_loc != NULL)
    (void) create_insn_allocno (OP_IN, INDEX_REG, REGNO (*index_reg_loc),
				GET_MODE (*index_reg_loc), insn, index_reg_loc,
				container_loc, false, out_allocno);
}

/* OUT_P is true if the operand which contains it is an output.  */
static void
process_base_index_register_for_allocno (rtx insn, rtx *loc,
					 allocno_t out_allocno)
{
  enum rtx_code code;
  const char *fmt;
  int i, j;

  code = GET_CODE (*loc);
  if (code == MEM)
    {
      process_address_for_allocno (insn, &XEXP (*loc, 0), loc, out_allocno);
      return;
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = 0; i < GET_RTX_LENGTH (code); i++)
    {
      if (fmt[i] == 'e')
	process_base_index_register_for_allocno
	  (insn, &XEXP (*loc, i), out_allocno);
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (*loc, i); j++)
	  process_base_index_register_for_allocno
	    (insn, &XVECEXP (*loc, i, j), out_allocno);
    }
}

static struct insn_op_info *
create_insn_info (rtx insn)
{
  int code, len, i, j;
  struct insn_op_info *info;
  char *str, c;

  code = INSN_CODE (insn);
  if (code >= 0 && insn_code_infos [code] != NULL)
    {
      insn_infos [INSN_UID (insn)] = insn_code_infos [code];
      return insn_code_infos [code];
    }
  yara_assert (recog_data.n_alternatives <= MAX_ALT_SET_SIZE);
  insn_infos [INSN_UID (insn)]
    = info = yara_allocate (sizeof (struct insn_op_info));
  if (code >= 0)
    insn_code_infos [code] = info;
  info->code = code;
  info->n_operands = recog_data.n_operands;
  info->n_alts = recog_data.n_alternatives;
  if (recog_data.n_alternatives == 0 || recog_data.n_operands == 0)
    info->op_constraints = NULL;
  else
    info->op_constraints
      = yara_allocate (sizeof (char *) * recog_data.n_operands
		       * recog_data.n_alternatives);
  info->commutative_op_p = false;
  CLEAR_OP_SET (info->single_reg_op_set);
  for (i = 0; i < recog_data.n_operands; i++)
    {
      str = yara_allocate (sizeof (char)
			   * (strlen (recog_data.constraints [i]) + 1));
      strcpy (str, recog_data.constraints [i]);
      yara_assert (recog_data.n_alternatives >= 0);
      for (j = 0; j < recog_data.n_alternatives; j++)
	{
	  info->op_constraints [i * recog_data.n_alternatives + j] = str;
	  for (;;)
	    {
	      c = *str;
	      if (c == '\0')
		break;
	      len = CONSTRAINT_LEN (c, p);
	      str += len;
	      if (c == '%')
		info->commutative_op_p = true;
	      else if (c == '#')
		str [-len] = '\0';
	      else if (c == ',')
		{
		  str [-len] = '\0';
		  break;
		}
	    }
	}
    }
  for (i = 0; i < recog_data.n_operands; i++)
    if (single_reg_operand_class (insn, i) != NO_REGS)
      SET_OP (info->single_reg_op_set, i);
  return info;
}

/* Set of hard regs currently live.  */
static HARD_REG_SET live_hard_regs;

/* Set of pseudo regs currently live.  */
static int *live_pseudo_reg_indexes;
static int *live_pseudo_regs;
static int live_pseudo_regs_num;

static varray_type local_live_allocnos;

static void
process_live_allocnos (void (*fun) (allocno_t, void *, int), void *data)
{
  int i, j, k;
  allocno_t live_a;

  /* We process local allocnos first.  */
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (local_live_allocnos); i++)
    {
      live_a = VARRAY_GENERIC_PTR (local_live_allocnos, i);
      (*fun) (live_a, data, i);
    }
  for (k = 0; k < live_pseudo_regs_num; k++)
    {
      i = live_pseudo_regs [k];
      live_a = curr_bb_node->regno_allocno_map [i];
      for (j = (int) VARRAY_ACTIVE_SIZE (local_live_allocnos) - 1; j >= 0; j--)
	if (live_a == VARRAY_GENERIC_PTR (local_live_allocnos, j))
	  break;
      if (j < 0)
	/* Don't process live allocno twice.  */
	(*fun) (live_a, data, -1);
    }
}

static void
set_hard_reg_conflict (allocno_t live_a, void *data,
		       int local_live_index ATTRIBUTE_UNUSED)
{
  int regno = (size_t) data;

  SET_HARD_REG_BIT (ALLOCNO_HARD_REG_CONFLICTS (live_a), regno);
}


static void
mark_hard_reg_live (int regno, enum machine_mode mode)
{
  int last;

  yara_assert (HARD_REGISTER_NUM_P (regno));
  last = regno + hard_regno_nregs [regno] [mode];
  while (regno < last)
    {
      process_live_allocnos (set_hard_reg_conflict, (void *) (size_t) regno);
      SET_HARD_REG_BIT (live_hard_regs, regno);
      regno++;
    }
}

static void
mark_hard_reg_death (int regno, enum machine_mode mode)
{
  int last;

  yara_assert (HARD_REGISTER_NUM_P (regno));
  last = regno + hard_regno_nregs [regno] [mode];
  while (regno < last)
    {
      CLEAR_HARD_REG_BIT (live_hard_regs, regno);
      regno++;
    }
}

copy_t
find_post_insn_allocno_copy (allocno_t src, rtx insn)
{
  copy_t cp;

  for (cp = ALLOCNO_SRC_COPIES (src); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    if (cp->point.point_type == AFTER_INSN && cp->point.u.insn == insn
	&& COPY_DST (cp) != NULL)
      break;
#ifdef ENABLE_YARA_CHECKING
  if (cp != NULL)
    {
      copy_t next_cp;
      
      /* Check that we have no more copies into region allocnos.  */
      for (next_cp = COPY_NEXT_SRC_COPY (cp);
	   next_cp != NULL;
	   next_cp = COPY_NEXT_SRC_COPY (next_cp))
	if (next_cp->point.point_type == AFTER_INSN
	    && COPY_DST (next_cp) != NULL)
	  gcc_unreachable ();
    }
#endif
  return cp;
}

static int
find_local_live_allocno (allocno_t a)
{
  int i, len;

  len = (int) VARRAY_ACTIVE_SIZE (local_live_allocnos);
  for (i = 0; i < len; i++)
    if (a == VARRAY_GENERIC_PTR (local_live_allocnos, i))
      return i;
  return -1;
}

/* ??? */
static void
set_allocno_conflict (allocno_t live_a, void *data, int local_live_index)
{
  copy_t cp;
  allocno_t a = data;

  if (local_live_index < 0)
    /* It is lived through.  */
    create_conflict (a, live_a);
  else
    {
      yara_assert (live_a == VARRAY_GENERIC_PTR (local_live_allocnos,
						 local_live_index));
      if ((cp = find_copy (a, live_a)) != NULL)
	/* Allocnos in copy before or after the insn never
	   conflict.  */
	return;
      if (ALLOCNO_TYPE (live_a) == INSN_ALLOCNO
	  && ALLOCNO_TYPE (a) == INSN_ALLOCNO
	  && INSN_ALLOCNO_OP_MODE (a) == OP_IN
	  && INSN_ALLOCNO_CONTAINER_LOC (live_a) == INSN_ALLOCNO_LOC (a))
	{
	  /* We can use the same hard register for insn allocnos
	     designating address and memory containing the address. */
	  yara_assert (find_post_insn_allocno_copy
		       (live_a, INSN_ALLOCNO_INSN (a)) == NULL);
	}
      else 
	create_conflict (a, live_a);
    }
}

static void
mark_allocno_live (allocno_t a, bool no_map_change_p)
{
  int i, regno;

  i = find_local_live_allocno (a);

  if (i < 0)
    {
      /* We process local live allocnos first.  */
      process_live_allocnos (set_allocno_conflict, a);
      
      /* Set up conflicts with live hard registers.  It might be made
         alive more one time (after one insn and before the next
         insn.  */
      IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a), live_hard_regs);

      VARRAY_PUSH_GENERIC_PTR (local_live_allocnos, a);
    }

  if (! no_map_change_p && (regno = ALLOCNO_REGNO (a)) >= 0
      && ! HARD_REGISTER_NUM_P (regno))
    {
      curr_bb_node->regno_allocno_map [regno] = a;
      if (live_pseudo_reg_indexes [regno] < 0)
	{
	  live_pseudo_reg_indexes [regno] = live_pseudo_regs_num;
	  live_pseudo_regs [live_pseudo_regs_num++] = regno;
	}
    }
}

/* Mark REG as being dead.  */

static void
mark_allocno_death (allocno_t a)
{
  int i, len, regno;

  i = find_local_live_allocno (a);

  if (i >= 0)
    {
      len = (int) VARRAY_ACTIVE_SIZE (local_live_allocnos);
      if (i + 1 < len)
	{
	  memmove (&VARRAY_GENERIC_PTR (local_live_allocnos, i),
		   &VARRAY_GENERIC_PTR (local_live_allocnos, i + 1),
		   sizeof (allocno_t) * (len - i - 1));
	}
      VARRAY_POP (local_live_allocnos);
    }

  if ((regno = ALLOCNO_REGNO (a)) >= 0 && ! HARD_REGISTER_NUM_P (regno))
    {
      if (curr_bb_node->regno_allocno_map [regno] == a)
	{
	  /* We don't care if it still conditionally live because it
	     again will be set up as live by the copy destination.  */
	  if ((i = live_pseudo_reg_indexes [regno]) >= 0)
	    {
	      live_pseudo_reg_indexes [regno] = -1;
	      live_pseudo_regs_num--;
	      if (i != live_pseudo_regs_num)
		{
		  regno = live_pseudo_regs [live_pseudo_regs_num];
		  if (regno >= max_regno)
		    abort ();
		  live_pseudo_regs [i] = regno;
		  live_pseudo_reg_indexes [regno] = i;
		}
	    }
	}
    }
}

static void
set_copy_conflict (allocno_t live_a, void *data,
		   int local_live_index ATTRIBUTE_UNUSED)
{
  copy_t cp = data;

  create_copy_conflict (live_a, cp);
}

static bool
copy_src_p (copy_t list, allocno_t src)
{
  copy_t cp;

  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    if (COPY_SRC (cp) == src)
      return true;
  return false;
}

static void
build_insn_allocno_copy_conflicts (copy_t list, rtx insn, bool after_insn_p)
{
  int regno;
  allocno_t src, curr_a;
  copy_t cp;
  bool no_map_change_p;

  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    {
      no_map_change_p = false;
      if ((src = COPY_SRC (cp)) != NULL)
	{
	  regno = ALLOCNO_REGNO (src);
	  if (! after_insn_p)
	    {
	      if (find_regno_note (insn, REG_DEAD, regno) != NULL_RTX
		  && ! copy_src_p (COPY_NEXT_COPY (cp), src))
		mark_allocno_death (src);
	      else
		no_map_change_p = true;
	    }
	  else
	    {
	      mark_allocno_death (src);
	      if (regno < 0)
		{
		  yara_assert (INSN_ALLOCNO_OP_MODE (src) == OP_OUT
			       || INSN_ALLOCNO_OP_MODE (src) == OP_INOUT);
		  /* We assume that each addr allocno can occur at most
		     once in output memory allocno.  */
		  if (GET_CODE (*INSN_ALLOCNO_LOC (src)) == MEM)
		    for (curr_a = curr_insn_allocnos;
			 curr_a != NULL;
			 curr_a = INSN_ALLOCNO_NEXT (curr_a))
		      if (INSN_ALLOCNO_ADDR_OUTPUT_ALLOCNO (curr_a) == src)
			{
			  yara_assert
			    (find_post_insn_allocno_copy (curr_a, insn)
			     == NULL);
			  mark_allocno_death (curr_a);
			  INSN_ALLOCNO_ADDR_OUTPUT_ALLOCNO (curr_a) = NULL;
			}
		}
	      else if (HARD_REGISTER_NUM_P (regno))
		{
		  /* It is a copy of out/inout allocno into hard
		     register.  */
		  yara_assert (COPY_DST (cp) == NULL
			       /* We assume that there are no hard
				  registers in subregisters. If it is
				  not true we could use
				  get_allocation_mode result as the
				  mode.  */
			       && (ALLOCNO_TYPE (src) != INSN_ALLOCNO
				   || GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC
						(src)) != SUBREG));
		  mark_hard_reg_live (regno, ALLOCNO_MODE (src));
		}
	    }
	}

      process_live_allocnos (set_copy_conflict, cp);
      COPY_HARD_REG_SET (COPY_HARD_REG_CONFLICTS (cp), live_hard_regs);

      if (COPY_DST (cp) != NULL)
	mark_allocno_live (COPY_DST (cp), no_map_change_p);
    }
}

static rtx output_insn_hard_regs [FIRST_PSEUDO_REGISTER];
static int output_insn_hard_reg_num;

static void
process_non_operand_hard_regs (rtx *loc, bool output_p)
{
  int i;
  const char *fmt;
  enum rtx_code code = GET_CODE (*loc);
  allocno_t a;

  code = GET_CODE (*loc);
  if (code == REG && HARD_REGISTER_P (*loc))
    {
      for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
	if (INSN_ALLOCNO_LOC (a) == loc)
	  break;
      if (a == NULL)
	{
	  if (output_p)
	    output_insn_hard_regs [output_insn_hard_reg_num++] = *loc;
	}
    }
  else if (code == SET)
    {
      process_non_operand_hard_regs (&SET_DEST (*loc), true);
      process_non_operand_hard_regs (&SET_SRC (*loc), false);
    }
  else if (code == CLOBBER)
    process_non_operand_hard_regs (&XEXP (*loc, 0), true);
  else if (code == MEM)
    process_non_operand_hard_regs (&XEXP (*loc, 0), false);
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY)
    {
      process_non_operand_hard_regs (&XEXP (*loc, 0), true);
      process_non_operand_hard_regs (&XEXP (*loc, 0), false);
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	process_non_operand_hard_regs (&XEXP (*loc, i), output_p);
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    process_non_operand_hard_regs (&XVECEXP (*loc, i, j), output_p);
	}
    }
}

/* Unsaved registers invalidated by function whose call is currently
   being processed.  */
static HARD_REG_SET curr_call_used_function_regs;

/* The function sets up call_p and increment call_freq for allocno
   LIVE_A living through call insn given by DATA.  The function is
   called by generic traverse function process_live_allocnos.  */
static void
set_call_info (allocno_t live_a, void *data,
	       int local_live_index ATTRIBUTE_UNUSED)
{
  rtx insn = data;

  ALLOCNO_CALL_CROSS_P (live_a) = true;
  ALLOCNO_CALL_FREQ (live_a) += BLOCK_FOR_INSN (insn)->frequency;
  IOR_HARD_REG_SET (ALLOCNO_CLOBBERED_REGS (live_a),
		    curr_call_used_function_regs);
}

/* The function sets up hard registers conflicting with allocno
   LIVE_A.  The hard registers are given by their class passed through
   DATA.  The function is called by generic traverse function
   process_live_allocnos.  */
static void
set_single_hard_reg_allocno_info (allocno_t live_a, void *data,
				  int local_live_index ATTRIBUTE_UNUSED)
{
  enum reg_class cl = (enum reg_class) data;

  if (ALLOCNO_TYPE (live_a) == INSN_ALLOCNO)
    return;
  IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (live_a),
		    reg_class_contents [cl]);
}

/* The function checks that CONSTRAINTS of alternative ALT_NUM of insn
   with INFO permits to use only one hard register.  If it is so, the
   function returns the class of the hard register.  Otherwise it
   returns NO_REGS.  */
static enum reg_class
single_alt_reg_class (const char *constraints, struct insn_op_info *info,
		      int alt_num)
{
  enum reg_class cl, next_cl;
  int c;

  cl = NO_REGS;
  for (; (c = *constraints); constraints += CONSTRAINT_LEN (c, constraints))
    switch (c)
      {
      case ' ':
      case '\t':
      case '=':
      case '+':
      case '*':
      case '&':
      case '%':
      case '!':
      case '?':
	break;
	
	/* ??? what about memory */
      case 'r':
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      case 'h': case 'j': case 'k': case 'l':
      case 'q': case 't': case 'u':
      case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D':
      case 'Q': case 'R': case 'S': case 'T': case 'U':
      case 'W': case 'Y': case 'Z':
	next_cl = (c == 'r'
		   ? GENERAL_REGS
		   : REG_CLASS_FROM_CONSTRAINT (c, constraints));
	if ((cl != NO_REGS && next_cl != cl)
	    || available_class_regs [next_cl] > 1)
	  return NO_REGS;
	cl = next_cl;
	break;
	
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	next_cl = single_alt_reg_class
	          (info->op_constraints [(c - '0') * info->n_alts + alt_num],
		   info, alt_num);
	if ((cl != NO_REGS && next_cl != cl) || next_cl == NO_REGS
	    || available_class_regs [next_cl] > 1)
	  return NO_REGS;
	cl = next_cl;
	break;

      default:
	yara_assert (c != '#');
	return NO_REGS;
      }
  return cl;
}

/* The function checks that insn allocno A can use only one hard
   register.  If it is so, the function returns the class of the hard
   register.  Otherwise it returns NO_REGS.  */
static enum reg_class
single_reg_operand_class (rtx insn, int op_num)
{
  enum reg_class cl, next_cl;
  struct insn_op_info *info;
  int i, n_alts;
  char **constraints;

  info = insn_infos [INSN_UID (insn)];
  if (op_num < 0 || (n_alts = info->n_alts) == 0)
    return NO_REGS;
  cl = NO_REGS;
  constraints = &info->op_constraints [op_num * n_alts];
  for (i = 0; i < n_alts; i++)
    {
      next_cl = single_alt_reg_class (constraints [i], info, i);
      if ((cl != NO_REGS && next_cl != cl)
	  || available_class_regs [next_cl] > 1)
	return NO_REGS;
      cl = next_cl;
    }
  return cl;
}

/* The function create conflicts for allocnos of INSN whose operands
   requiring a single hard register given by SINGLE_REG_OP_SET.  */
static void
build_insn_allocno_conflicts (rtx insn, op_set_t single_reg_op_set)
{
  int i, hard_regno, op_num;
  allocno_t a;
  rtx link;

  build_insn_allocno_copy_conflicts
    (before_insn_copies [INSN_UID (insn)], insn, false);

  output_insn_hard_reg_num = 0;
  process_non_operand_hard_regs (&PATTERN (insn), false);

  /* Early clobbers: */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    if (INSN_ALLOCNO_EARLY_CLOBBER (a))
      {
	yara_assert (INSN_ALLOCNO_OP_MODE (a) == OP_OUT);
	mark_allocno_live (a, false);
      }

  if (INSN_CODE (insn) < 0)
    /* It is an asm insn and all output hard regs are clobbered.  */
    for (i = 0; i < output_insn_hard_reg_num; i++)
      {
	hard_regno = REGNO (output_insn_hard_regs [i]);
#ifdef STACK_REGS
	/* Cloberred stack registers in asm denotes that it is
	   implictly popped.  So we ignore them in this case.  */
	if (hard_regno < FIRST_STACK_REG || hard_regno > LAST_STACK_REG)
#endif
	  mark_hard_reg_live (hard_regno,
			      GET_MODE (output_insn_hard_regs [i]));
      }

  
  if (! EQ_OP_SET (single_reg_op_set, ZERO_OP_SET))
    for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
      if (INSN_ALLOCNO_OP_MODE (a) == OP_IN
	  && (op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE) >= 0
	  && TEST_OP (single_reg_op_set, op_num))
	{
	  enum reg_class cl;
	  
	  cl = single_reg_operand_class (insn, op_num);
	  yara_assert (cl != NO_REGS);
	  process_live_allocnos (set_single_hard_reg_allocno_info,
				 (void *) cl);
	}

  /* Death used allocnos: */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    if (INSN_ALLOCNO_OP_MODE (a) == OP_IN)
      {
	if (INSN_ALLOCNO_ADDR_OUTPUT_ALLOCNO (a) != NULL)
	  /* It is an address in an output operand so we can not kill
	     it now.  */
	  ;
	else
	  {
	    yara_assert (find_post_insn_allocno_copy (a, insn) == NULL);
            mark_allocno_death (a);
          }
      }
  
  /* Death hard regs without allocnos: */
  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
    if (REG_NOTE_KIND (link) == REG_DEAD && REG_P (XEXP (link, 0))
	&& HARD_REGISTER_P (XEXP (link, 0)))
      mark_hard_reg_death (REGNO (XEXP (link, 0)), GET_MODE (XEXP (link, 0)));

  if (CALL_P (insn))
    {
      get_call_invalidated_used_regs (insn, &curr_call_used_function_regs,
				      false);
      IOR_HARD_REG_SET (cfun->emit->call_used_regs,
			curr_call_used_function_regs);
      process_live_allocnos (set_call_info, insn);
    }

  /* Set up allocnos:  */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    if (INSN_ALLOCNO_OP_MODE (a) == OP_OUT
	|| INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
      mark_allocno_live (a, false);

  if (! EQ_OP_SET (single_reg_op_set, ZERO_OP_SET))
    for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
      if ((INSN_ALLOCNO_OP_MODE (a) == OP_OUT
	   || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
	  && (op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE) >= 0
	  && TEST_OP (single_reg_op_set, op_num))
	{
	  enum reg_class cl;
	  
	  cl = single_reg_operand_class (insn, op_num);
	  yara_assert (cl != NO_REGS);
	  process_live_allocnos (set_single_hard_reg_allocno_info,
				 (void *) cl);
	}

  /* Set up hard regs without allocnos */
  for (i = 0; i < output_insn_hard_reg_num; i++)
    {
      hard_regno = REGNO (output_insn_hard_regs [i]);
      if (INSN_CODE (insn) >= 0
#ifdef STACK_REGS
	  || 
	  (hard_regno >= FIRST_STACK_REG && hard_regno <= LAST_STACK_REG)
#endif
	  )
 	mark_hard_reg_live (hard_regno, GET_MODE (output_insn_hard_regs [i]));
    }

  /* Unused allocnos: */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    if ((INSN_ALLOCNO_OP_MODE (a) == OP_OUT
	 || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
	&& find_post_insn_allocno_copy (a, insn) == NULL)
      /* We should not worry about changing liveness of the hard
	 register corresponding the allocno here because the hard
	 register is not marked to live here yet (see
	 build_insn_allocno_copy_conflicts).  */
      mark_allocno_death (a);

  /* Unused hard regs without alocnos:  */
  for (i = 0; i < output_insn_hard_reg_num; i++)
    if (find_regno_note (insn, REG_UNUSED,
			 REGNO (output_insn_hard_regs [i])) != NULL_RTX)
      mark_hard_reg_death (REGNO (output_insn_hard_regs [i]),
			   GET_MODE (output_insn_hard_regs [i]));
  
  build_insn_allocno_copy_conflicts
    (after_insn_copies [INSN_UID (insn)], insn, true);
}

static allocno_t
create_region_allocno (int regno, struct yara_loop_tree_node *node,
		       rtx start_insn)
{
  allocno_t a;

  yara_assert (! HARD_REGISTER_NUM_P (regno));
  a = create_allocno (REGION_ALLOCNO, regno, PSEUDO_REGNO_MODE (regno));
  REGION_ALLOCNO_NODE (a) = node;
  REGION_ALLOCNO_START_INSN (a) = start_insn;
  REGION_ALLOCNO_STOP_INSN (a) = NULL_RTX;
  return a;
}

static int
before_insn_copy_compare (const void *v1p, const void *v2p)
{
  copy_t cp1 = *(copy_t *) v1p;
  copy_t cp2 = *(copy_t *) v2p;
#if 0
  allocno_t dst1 = COPY_DST (cp1);
  allocno_t dst2 = COPY_DST (cp2);
  
  yara_assert (dst1 != NULL && dst2 != NULL);
  if (ALLOCNO_REGNO (dst1) >= 0 && ALLOCNO_REGNO (dst1) == ALLOCNO_REGNO (dst2)
      && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (dst1)) == SUBREG
      && GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (dst2)) == SUBREG)
    return (SUBREG_BYTE (*INSN_ALLOCNO_CONTAINER_LOC (dst2))
	    - SUBREG_BYTE (*INSN_ALLOCNO_CONTAINER_LOC (dst1)));
#endif
  return COPY_NUM (cp1) - COPY_NUM (cp2);
}

static int
after_insn_copy_compare (const void *v1p, const void *v2p)
{
  copy_t cp1 = *(copy_t *) v1p;
  copy_t cp2 = *(copy_t *) v2p;
  
  yara_assert (COPY_SRC (cp1) != NULL && COPY_SRC (cp2) != NULL);
  if (COPY_DST (cp1) == NULL && COPY_DST (cp2) != NULL)
    return -1;
  else if (COPY_DST (cp1) != NULL && COPY_DST (cp2) == NULL)
    return 1;
  else
    return COPY_NUM (cp1) - COPY_NUM (cp2);
}

static void
merge_the_same_regno_source_copies (copy_t list)
{
  copy_t cp, cp2, next_cp, prev_cp;

  for (cp = list; cp != NULL; cp = next_cp)
    {
      next_cp = COPY_NEXT_COPY (cp);
      if (COPY_SRC (cp) == NULL || ALLOCNO_REGNO (COPY_SRC (cp)) < 0)
	continue;
      for (prev_cp = cp, cp2 = next_cp;
	   cp2 != NULL;
	   prev_cp = cp2, cp2 = COPY_NEXT_COPY (cp2))
	if (COPY_SRC (cp2) != NULL
	    && ALLOCNO_REGNO (COPY_SRC (cp)) == ALLOCNO_REGNO (COPY_SRC (cp2)))
	  break;
      if (cp2 != NULL)
	{
	  COPY_NEXT_COPY (prev_cp) = COPY_NEXT_COPY (cp2);
	  COPY_NEXT_COPY (cp2) = COPY_NEXT_COPY (cp);
	  next_cp = COPY_NEXT_COPY (cp) = cp2;
	}
    }
}

/* The following function creates new basic block allocnos for
   pseudo-register living after INSN and used, defined, or modified in
   INSN.  It updates bb regno_allocno_map for the pseudo-registers and
   creates copies connecting basic block allocnos and the
   corresponding insn allocnos and copies for non pseudo-register insn
   allocnos.  */

static void
create_insn_copies (rtx insn)
{
  struct point before, after;
  allocno_t a, prev_a;
  int regno, uid;
  allocno_t src, dst;
  enum op_type op_mode;
  rtx x;
  struct insn_op_info *info;

  uid = INSN_UID (insn);
  after.point_type = AFTER_INSN;
  after.u.insn = insn;
  /* Create new basic block allocnos if necessary for modified
     pseudo-register and corresponding copies.  */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    {
      regno = ALLOCNO_REGNO (a);
      op_mode = INSN_ALLOCNO_OP_MODE (a);
      if ((op_mode == OP_OUT || op_mode == OP_INOUT)
	  && find_reg_note (insn, REG_UNUSED,
			    *INSN_ALLOCNO_LOC (a)) == NULL_RTX)
	{
	  prev_a = NULL;
	  if (regno < FIRST_PSEUDO_REGISTER)
	    dst = NULL;
	  else
	    {
	      SKIP_TO_REG (x, *INSN_ALLOCNO_LOC (a));
	      if (GET_MODE_SIZE (ALLOCNO_MODE (a))
		  < GET_MODE_SIZE (GET_MODE (x))
		  && (GET_CODE (*INSN_ALLOCNO_LOC (a)) == STRICT_LOW_PART
		      || (GET_CODE (*INSN_ALLOCNO_CONTAINER_LOC (a))
			  == STRICT_LOW_PART)
		      || mode_multi_reg_p [GET_MODE (x)]))
		{
		  /* It is potentially a part of multi-register.  */
		  prev_a = curr_bb_node->regno_allocno_map [regno];
		  yara_assert (prev_a != NULL);
		}
	      if (new_regno_allocno_map_check [regno] == 2 * uid)
		dst = new_regno_allocno_map [regno];
	      else
		{
		  new_regno_allocno_map_check [regno] = 2 * uid;
		  dst = new_regno_allocno_map [regno]
		    = create_region_allocno (regno, curr_bb_node, insn);
		}
	    }
	  if (prev_a == NULL)
	    create_copy (dst, a, after, insn);
	  else
	    {
	      create_copy (prev_a, a, after, insn);
	      create_copy (dst, prev_a, after, insn);
	    }
	}
    }
  before.point_type = BEFORE_INSN;
  before.u.insn = insn;
  /* Connect basic block allocnos and used insn allocnos before the
     insn.  Create new basic block allocnos if necessary for used
     pseudo-registers and connect them with used insn allocnos.  */
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    {
      regno = ALLOCNO_REGNO (a);
      op_mode = INSN_ALLOCNO_OP_MODE (a);
      if (op_mode == OP_IN || op_mode == OP_INOUT)
	{
	  if (regno < FIRST_PSEUDO_REGISTER)
	    src = NULL;
	  else
	    {
	      src = curr_bb_node->regno_allocno_map [regno];
	      yara_assert (src != NULL);
	    }
	  create_copy (a, src, before, insn);
	  /* What about predicates??? (p ? p := something).  */
	}
    }
  /* We want copies before the insns in the same order as allocnos
     (e.g. base and index registers are at the start of the insn
     allocno list).  Function create_copy forms lists in reverse
     order.  */
  before_insn_copies [INSN_UID (insn)]
    = sort_copy_list (before_insn_copies [INSN_UID (insn)],
		      before_insn_copy_compare);
  merge_the_same_regno_source_copies (before_insn_copies [INSN_UID (insn)]);
  /* Put copies with null destination first.  */
  after_insn_copies [INSN_UID (insn)]
    = sort_copy_list (after_insn_copies [INSN_UID (insn)],
		      after_insn_copy_compare);
  info = insn_infos [INSN_UID (insn)];
  build_insn_allocno_conflicts
    (insn, info == NULL ? ZERO_OP_SET : info->single_reg_op_set);
}

static void
process_const_operands_in_elimination_part (rtx x)
{
  int i;
  allocno_t a;
  const char *fmt;
  enum rtx_code code = GET_CODE (x);
  
  if (CONSTANT_P (x))
    {
      for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
	if (*INSN_ALLOCNO_LOC (a) == x)
	  {
	    INSN_ALLOCNO_ELIMINATION_PART_CONST_P (a) = true;
	    break;
	  }
      return;
    }
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	process_const_operands_in_elimination_part (XEXP (x, i));
      else if (fmt[i] == 'E')
	{
	  int j;
	  
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    process_const_operands_in_elimination_part (XVECEXP (x, i, j));
	}
    }
}

static void
setup_insn_elimination_part_const_p (void)
{
  allocno_t a;
  rtx addr, *container_loc;
  
  for (a = curr_insn_allocnos; a != NULL; a = INSN_ALLOCNO_NEXT (a))
    /* We are not interested in constants which are not operands.  */
    if (INSN_ALLOCNO_ELIMINATION_P (a) && ALLOCNO_REGNO (a) >= 0)
      {
	container_loc = INSN_ALLOCNO_CONTAINER_LOC (a);
	if (INSN_ALLOCNO_TYPE (a) == BASE_REG
	    || INSN_ALLOCNO_TYPE (a) == INDEX_REG)
	  addr = (GET_CODE (*container_loc) == MEM
		  ? XEXP (*container_loc, 0) : *container_loc);
	else if (GET_CODE (*container_loc) == PLUS
		 && GET_CODE (XEXP (*container_loc, 1)) == CONST_INT)
	  addr = *container_loc;
	else
	  continue;
	process_const_operands_in_elimination_part (addr);
      }
}

/* Return container location of the address LOC.  */
rtx *
get_container_loc (rtx *loc, rtx *in_loc)
{
  rtx *container_loc;
  enum rtx_code code;
  const char *fmt;
  int i, j;

  if (*in_loc == NULL_RTX)
    return NULL;
  code = GET_CODE (*in_loc);
  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (loc == &XEXP (*in_loc, i))
	    return in_loc;
	  else if ((container_loc = get_container_loc (loc,
						       &XEXP (*in_loc, i)))
		   != NULL)
	    return container_loc;
	}
      else if (fmt[i] == 'u' && loc == &XEXP (*in_loc, i))
	return in_loc;
      else if (fmt[i] == 'E')
	for (j = XVECLEN (*in_loc, i) - 1; j >= 0; j--)
	  {
	    if (loc == &XVECEXP (*in_loc, i, j))
	      return in_loc;
	    else if ((container_loc
		      = get_container_loc (loc, &XVECEXP (*in_loc, i, j)))
		     != NULL)
	      return container_loc;
	  }
    }
  return NULL;
}

/* Create INSN allocnos from location LOC of MODE.  The function
   returns allocno creates immediately from LOC.  Its attributes are
   EARLY_CLOBBER, OP_MODE and INSN_ALLOCNO_TYPE.  */
static allocno_t
create_loc_insn_allocnos (rtx insn, rtx *loc, enum machine_mode mode,
			  bool early_clobber, enum op_type op_mode,
			  int insn_allocno_type)
{
  allocno_t loc_a;
  int regno;
  rtx *container_loc, x;

  regno = -1;
  x = *loc;
  container_loc = get_container_loc (loc, &PATTERN (insn));
  SKIP_TO_REG (x, x);
  if (REG_P (x))
    regno = REGNO (x);

  loc_a = create_insn_allocno (op_mode, insn_allocno_type, regno, mode, insn,
			       loc, container_loc, early_clobber, NULL);
  
  /* We process base/index register after the operand to get them
     first in the list of insn allocnos.  */
  process_base_index_register_for_allocno
    (insn, loc, op_mode == OP_OUT || op_mode == OP_INOUT ? loc_a : NULL);
  return loc_a;
}

static void
clobbered_all_hard_regs (allocno_t live_a, void *data ATTRIBUTE_UNUSED,
			 int local_live_index ATTRIBUTE_UNUSED)
{
  SET_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (live_a));
}


static void
create_insn_allocnos (rtx insn)
{
  int commutative;
  enum op_type op_mode;
  bool early_clobber, address_p, non_white_p;
  rtx operand, *loc, pattern;
  int i, j;
  allocno_t *op_allocno;
  struct insn_op_info *info;

  curr_insn_allocnos = NULL;
  extract_insn (insn);
  pattern = PATTERN (insn);
  if (GET_CODE (pattern) == CLOBBER || GET_CODE (pattern) == USE)
    {
      loc = &XEXP (pattern, 0);
      yara_assert (recog_data.n_operands == 0
		   && (GET_CODE (*loc) == REG || GET_CODE (*loc) == MEM
		       || GET_CODE (*loc) == CONST_INT
		       || GET_CODE (*loc) == CONST
		       || GET_CODE (*loc) == SYMBOL_REF));
      /* Ignore (mem:BLK (scratch)) and hard regno.  */
      if ((GET_CODE (*loc) != MEM || GET_MODE (*loc) != BLKmode
	   || GET_CODE (XEXP (*loc, 0)) != SCRATCH)
	  && (! REG_P (*loc) || ! HARD_REGISTER_P (*loc)))
	(void) create_loc_insn_allocnos (insn, loc, GET_MODE (*loc), false,
					 (GET_CODE (pattern) == CLOBBER
					  ? OP_OUT : OP_IN), NON_OPERAND);
    }
  else if (current_function_has_nonlocal_label
	   && GET_CODE (PATTERN (insn)) == ASM_INPUT)
    {
      /* It is blockage used for nonlocal goto:
	 all registers are clobbered.  */
      yara_assert (recog_data.n_operands == 0);
      process_live_allocnos (clobbered_all_hard_regs, NULL);
    }
  if (recog_data.n_operands == 0)
    insn_op_allocnos [INSN_UID (insn)] = op_allocno = NULL;
  else
    insn_op_allocnos [INSN_UID (insn)] = op_allocno
      = yara_allocate (recog_data.n_operands * sizeof (allocno_t));
  info = create_insn_info (insn);
  commutative = -1;

  /* We process opeands in the reverse order to get insn allocno list
     in the right order.  */
  for (i = info->n_operands - 1; i >= 0 ; i--)
    {
      rtx *loc;
      char *p;
      
      if (*recog_data.constraints [i] == '\0')
	/* Ignore things match_operator operands.  */
	continue;
      operand = recog_data.operand [i];
      early_clobber = false;
      op_mode = OP_IN;

      /* Scan this operand's constraint to see if it is an output
	 operand, an in-out operand, is commutative, or might be early
	 clobber.  */

      address_p = false;
      non_white_p = false;
      for (j = info->n_alts - 1; j >= 0; j--)
	for (p = info->op_constraints [i * info->n_alts + j]; *p != '\0'; p++)
	  {
	    if (*p != ' ' && *p != '\t' && *p != 'p')
	      non_white_p = true;
	    switch (*p)
	      {
	      case 'p':
		address_p = true;
		break;
	      case '=':
		op_mode = OP_OUT;
		break;
	      case '+':
		op_mode = OP_INOUT;
		break;
	      case '&':
		early_clobber = true;
		break;
	      case '%':
		{
		  /* The last operand should not be marked commutative.  */
		  yara_assert (i != info->n_operands - 1);
		  
		  /* We currently only support one commutative pair of
		     operands.  Some existing asm code currently uses
		     more than one pair.  We continue supporting that
		     case as well as we can by silently ignoring all
		     but the first pair.  */
		  if (commutative < 0)
		    commutative = i;
		  else
		    {
		      yara_assert (INSN_CODE (insn) < 0); /* an asm insn */
		    }
		}
		break;
	      }
	  }
      yara_assert (! address_p || ! non_white_p);
      yara_assert ((op_mode != OP_OUT && op_mode != OP_INOUT)
		   || GET_CODE (operand) == REG
		   || GET_CODE (operand) == SUBREG
		   || GET_CODE (operand) == MEM
		   || GET_CODE (operand) == SCRATCH);
      loc = recog_data.operand_loc [i];
      if (address_p)
	{
	  yara_assert (op_mode != OP_OUT && op_mode != OP_INOUT);
	  process_address_for_allocno (insn, loc, loc, NULL);
	}
      else
	op_allocno [i]
	  = create_loc_insn_allocnos (insn, loc, recog_data.operand_mode [i],
				      early_clobber, op_mode,
				      OPERAND_BASE + i);
    }
  if (commutative >= 0 && op_allocno [commutative] != NULL
      && op_allocno [commutative + 1] != NULL)
    {
      INSN_ALLOCNO_COMMUTATIVE (op_allocno [commutative])
	= op_allocno [commutative + 1];
      INSN_ALLOCNO_COMMUTATIVE (op_allocno [commutative + 1])
	= op_allocno [commutative];
    }
  insn_allocnos [INSN_UID (insn)] = curr_insn_allocnos;
  setup_insn_elimination_part_const_p ();
  create_insn_copies (insn);
}

allocno_t
insn_allocno (rtx x, rtx insn)
{
  allocno_t a;

  for (a = insn_allocnos [INSN_UID (insn)];
       a != NULL;
       a = INSN_ALLOCNO_NEXT (a))
    if (*INSN_ALLOCNO_LOC (a) == x)
      return a;
  gcc_unreachable ();
}

static int all, moves;

static void
create_bb_insn_allocnos (struct yara_loop_tree_node *bb_node)
{
  rtx insn, next_insn, prev_insn, set, bound;
  basic_block bb;

  curr_bb_node = bb_node;
  bb = bb_node->bb;
  bound = NEXT_INSN (BB_END (bb));
  for (prev_insn = NULL_RTX, insn = BB_HEAD (bb);
       insn != bound;
       insn = next_insn)
    if (! INSN_P (insn))
      next_insn = NEXT_INSN (insn);
    else
      {
	all++;
	VARRAY_POP_ALL (local_live_allocnos);
	next_insn = NEXT_INSN (insn);
	/* Special case frame = bp.  Just ignore.  */
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
	if ((set = single_set (insn)) != NULL_RTX
	    && SET_DEST (set) == frame_pointer_rtx
	    && SET_SRC (set) == hard_frame_pointer_rtx)
	  continue;
#endif
	prev_insn = insn;
	create_insn_allocnos (insn);
	if ((set = single_set (insn)) != NULL_RTX
	    && REG_P (SET_DEST (set))
	    && ! HARD_REGISTER_P (SET_DEST (set))
	    && REG_P (SET_SRC (set))
	    && !HARD_REGISTER_P (SET_SRC (set))
	    && !side_effects_p (set)
	    && find_reg_note (insn, REG_RETVAL, NULL_RTX) == NULL_RTX)
	  moves++;
      }
}

/* Allocnos living on abnormal edges should not get the following hard
   registers.  */
static HARD_REG_SET prohibited_abnormal_edge_hard_regs;

/* Info about allocno living through basic block or edge.  This
   allocno has no copy on start/end of the basic block or edge.  */
struct live_allocno
{
  allocno_t a;
  /* The filed value is defined if BB is not NULL.  */
  bool bb_start_p;
  /* Either BB or e is not NULL.  */
  basic_block bb;
  edge e;
};

/* Info about allocnos living through basic blocks or CFG edges and
   not haveing copy on start/end of the basic block or edge.  */
static varray_type live_through_allocnos_varray;

/* Bitmap used to calculate register living through abnormal edges.  */
static bitmap temp_live_through_abnormal;

static void
add_live_through_allocno (allocno_t a, bool bb_start_p, basic_block bb, edge e)
{
  struct live_allocno *live_allocno;

  yara_assert ((bb != NULL && e == NULL) || (bb == NULL && e != NULL));
  live_allocno = yara_allocate (sizeof (struct live_allocno));
  live_allocno->a = a;
  live_allocno->bb_start_p = bb_start_p;
  live_allocno->bb = bb;
  live_allocno->e = e;
  VARRAY_PUSH_GENERIC_PTR (live_through_allocnos_varray, live_allocno);
}

static void
create_bb_allocno (int regno, struct yara_loop_tree_node *bb_node,
		   bool start_p, bitmap live_through_abnormal, bool abnormal_p)
{
  allocno_t a, outside_a;
  copy_t cp;
  struct point point;
  bool live_through_abnormal_p;

  point.u.bb = bb_node->bb;
  live_through_abnormal_p = (live_through_abnormal != NULL
			     && bitmap_bit_p (live_through_abnormal, regno));
  if (! bitmap_bit_p (bb_node->regno_refs, regno) || live_through_abnormal_p)
    {
      if ((outside_a = bb_node->father->regno_allocno_map [regno]) == NULL)
	{
	  if ((outside_a = bb_node->regno_allocno_map [regno]) != NULL)
	    {
	      yara_assert (! start_p);
	      bb_node->father->regno_allocno_map [regno] = outside_a;
	    }
	  else
	    outside_a = bb_node->father->regno_allocno_map [regno]
	      = create_region_allocno (regno, bb_node->father, NULL_RTX);
	}
      if (start_p)
	{
	  bb_node->regno_allocno_map [regno] = outside_a;
	  bitmap_set_bit (bb_node->allocno_live_at_start,
			  ALLOCNO_NUM (outside_a));
	}
      add_live_through_allocno (outside_a, start_p, bb_node->bb, NULL);
    }
  else if (start_p)
    {
      if ((outside_a = bb_node->father->regno_allocno_map [regno]) == NULL)
	outside_a = bb_node->father->regno_allocno_map [regno]
	  = create_region_allocno (regno, bb_node->father, NULL_RTX);
      if (bitmap_bit_p (bb_node->regno_refs, regno)
	  || bitmap_bit_p (bb_node->father->regno_refs, regno))
	a = bb_node->regno_allocno_map [regno]
	  = create_region_allocno (regno, bb_node, NULL_RTX);
      else
	{
	  gcc_unreachable ();
	  a = outside_a;
	  bb_node->regno_allocno_map [regno] = a;
	}
      point.point_type = AT_BB_START;
      cp = create_copy (a, outside_a, point, NULL_RTX);
      bitmap_set_bit (bb_node->allocno_live_at_start, ALLOCNO_NUM (a));
    }
  else
    {
      if ((outside_a = bb_node->father->regno_allocno_map [regno]) == NULL)
	outside_a = bb_node->father->regno_allocno_map [regno]
	  = create_region_allocno (regno, bb_node->father, NULL_RTX);
      a = bb_node->regno_allocno_map [regno];
      /* It should be created at the start or from an insn inside the
	 block.  */
      yara_assert (a != NULL && a != outside_a);
      point.point_type = AT_BB_END;
      cp = create_copy (outside_a, a, point, NULL_RTX);
    }
  if (abnormal_p)
    IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (outside_a),
		      prohibited_abnormal_edge_hard_regs);
}

/* The function returns true if there is an abnormal edge in
   EDGES.  */
static bool
abnormal_edge_p (VEC(edge,gc) *edges)
{
  edge e;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, edges)
    if (e->flags & EDGE_COMPLEX)
      return true;
  return false;
}

/* The function create copies and allocnos for registers living in
   basic block represented by node LOOP.  */
static void
create_block_allocnos (struct yara_loop_tree_node *loop)
{
  basic_block bb = loop->bb;
  bitmap_iterator bi;
  bool abnormal_p;
  int i;
  unsigned int k;
  edge_iterator ei;
  edge e;
  bitmap live_through_abnormal = NULL;

  yara_assert (bb != NULL);
  REG_SET_TO_HARD_REG_SET (live_hard_regs, bb->il.rtl->global_live_at_start);
  for (i = 0; i < live_pseudo_regs_num; i++)
    live_pseudo_reg_indexes [live_pseudo_regs [i]] = -1;
  live_pseudo_regs_num = 0;
  EXECUTE_IF_SET_IN_REG_SET (bb->il.rtl->global_live_at_start,
			     FIRST_PSEUDO_REGISTER, k, bi)
    {
      live_pseudo_reg_indexes [k] = live_pseudo_regs_num;
      live_pseudo_regs [live_pseudo_regs_num++] = k;
    }
  FOR_EACH_EDGE (e, ei, bb->succs)
    if (e->flags & EDGE_COMPLEX)
      {
	if (live_through_abnormal == NULL)
	  {
	    live_through_abnormal = yara_allocate_bitmap ();
	    bitmap_and (live_through_abnormal, bb->il.rtl->global_live_at_end,
			e->dest->il.rtl->global_live_at_start);
	  }
	else
	  {
	    bitmap_and (temp_live_through_abnormal,
			bb->il.rtl->global_live_at_end,
			e->dest->il.rtl->global_live_at_start);
	    bitmap_ior_into (live_through_abnormal,
			     temp_live_through_abnormal);
	  }
      }
  abnormal_p = abnormal_edge_p (bb->preds);
  for (i = 0; i < live_pseudo_regs_num; i++)
    create_bb_allocno (live_pseudo_regs [i], loop, true,
		       live_through_abnormal, abnormal_p);
  create_bb_insn_allocnos (loop);
  abnormal_p = abnormal_edge_p (bb->succs);
  EXECUTE_IF_SET_IN_REG_SET (bb->il.rtl->global_live_at_end,
			     FIRST_PSEUDO_REGISTER, k, bi)
    {
      create_bb_allocno (k, loop, false, live_through_abnormal, abnormal_p);
    }
  if (live_through_abnormal != NULL)
    yara_free_bitmap (live_through_abnormal);
}

static void
create_edge_allocno (int regno, edge e, struct yara_loop_tree_node *loop_node,
		     bool entry_p, bitmap live_through_abnormal)
{
  allocno_t a, outside_a;
  copy_t cp;
  struct point point;
  bool live_through_abnormal_p;
  
  point.u.e.e = e;
  point.u.e.loop_node = loop_node;
  live_through_abnormal_p = (live_through_abnormal != NULL
			     && bitmap_bit_p (live_through_abnormal, regno));
  if ((e->flags & EDGE_COMPLEX) != 0 || live_through_abnormal_p)
    {
      if ((outside_a = loop_node->father->regno_allocno_map [regno]) == NULL)
	outside_a = loop_node->father->regno_allocno_map [regno]
	  = create_region_allocno (regno, loop_node->father, NULL_RTX);
      if ((e->flags & EDGE_COMPLEX) != 0)
	IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (outside_a),
			  prohibited_abnormal_edge_hard_regs);
      if (entry_p)
	{
	  loop_node->regno_allocno_map [regno] = outside_a;
	  bitmap_set_bit (loop_node->allocno_live_at_start,
			  ALLOCNO_NUM (outside_a));
	}
      add_live_through_allocno (outside_a, false, NULL, e);
    }
  else if (entry_p)
    {
      if ((outside_a = loop_node->father->regno_allocno_map [regno]) == NULL)
	outside_a = loop_node->father->regno_allocno_map [regno]
	  = create_region_allocno (regno, loop_node->father, NULL_RTX);
      if (bitmap_bit_p (loop_node->regno_refs, regno)
	  || bitmap_bit_p (loop_node->father->regno_refs, regno))
	{
	  a = loop_node->regno_allocno_map [regno];
	  if (a == NULL)
	    a = loop_node->regno_allocno_map [regno]
	      = create_region_allocno (regno, loop_node, NULL_RTX);
	}
      else
	{
	  a = outside_a;
	  loop_node->regno_allocno_map [regno] = a;
	}
      bitmap_set_bit (loop_node->allocno_live_at_start, ALLOCNO_NUM (a));
      if (a == outside_a)
	add_live_through_allocno (outside_a, false, NULL, e);
      else
	{
	  point.point_type = ON_EDGE_DST;
	  cp = create_copy (a, outside_a, point, NULL_RTX);
	}
    }
  else
    {
      /* ??? No change necessary for loop_node->regno_refs.  */
      if ((outside_a = loop_node->father->regno_allocno_map [regno]) == NULL)
	outside_a = loop_node->father->regno_allocno_map [regno]
	  = create_region_allocno (regno, loop_node->father, NULL_RTX);
      a = loop_node->regno_allocno_map [regno];
      if (a == outside_a)
	add_live_through_allocno (outside_a, false, NULL, e);
      else
	{
	  /* It should be created inside the loop or at the entry of
	     the loop.  */
	  yara_assert (a != NULL);
	  point.point_type = ON_EDGE_SRC;
	  cp = create_copy (outside_a, a, point, NULL_RTX);
	}
    }
}

static void
create_edge_allocnos (edge e, struct yara_loop_tree_node *loop_node,
		      bool entry_p, bitmap live_through_abnormal)
{
  basic_block src = e->src, dst = e->dest;
  bitmap_iterator bi;
  unsigned int i;

  yara_assert (loop_node->loop != NULL);
  EXECUTE_IF_SET_IN_REG_SET (src->il.rtl->global_live_at_end,
			     FIRST_PSEUDO_REGISTER, i, bi)
    {
      if (REGNO_REG_SET_P (dst->il.rtl->global_live_at_start, i))
	create_edge_allocno (i, e, loop_node, entry_p, live_through_abnormal);
    }
}

static void
create_loop_allocnos (struct yara_loop_tree_node *loop_node)
{
  edge_iterator ei;
  edge e, *edges;
  int i;
  unsigned int edges_num;
  bitmap live_through_abnormal = NULL;

  yara_assert (loop_node->bb == NULL);
  edges = get_loop_exit_edges (loop_node->loop, &edges_num);
  for (i = 0 ; i < (int) edges_num; i++)
    if (edges [i]->flags & EDGE_COMPLEX)
      {
	if (live_through_abnormal == NULL)
	  {
	    live_through_abnormal = yara_allocate_bitmap ();
	    bitmap_and (live_through_abnormal,
			edges [i]->src->il.rtl->global_live_at_end,
			edges [i]->dest->il.rtl->global_live_at_start);
	  }
	else
	  {
	    bitmap_and (temp_live_through_abnormal,
			edges [i]->src->il.rtl->global_live_at_end,
			edges [i]->dest->il.rtl->global_live_at_start);
	    bitmap_ior_into (live_through_abnormal,
			     temp_live_through_abnormal);
	  }
      }
  FOR_EACH_EDGE (e, ei, loop_node->loop->header->preds)
    if (e->src != loop_node->loop->latch && (e->flags & EDGE_COMPLEX))
      {
	if (live_through_abnormal == NULL)
	  {
	    live_through_abnormal = yara_allocate_bitmap ();
	    bitmap_and (live_through_abnormal,
			e->src->il.rtl->global_live_at_end,
			e->dest->il.rtl->global_live_at_start);
	  }
	else
	  {
	    bitmap_and (temp_live_through_abnormal,
			e->src->il.rtl->global_live_at_end,
			e->dest->il.rtl->global_live_at_start);
	    bitmap_ior_into (live_through_abnormal,
			     temp_live_through_abnormal);
	  }
      }
  FOR_EACH_EDGE (e, ei, loop_node->loop->header->preds)
    if (e->src != loop_node->loop->latch)
      create_edge_allocnos (e, loop_node, true, live_through_abnormal);
  create_subloop_allocnos (loop_node);
  for (i = 0 ; i < (int) edges_num; i++)
    create_edge_allocnos (edges [i], loop_node, false, live_through_abnormal);
  if (live_through_abnormal != NULL)
    yara_free_bitmap (live_through_abnormal);
}

static void
create_subloop_allocnos (struct yara_loop_tree_node *loop_node)
{
  struct yara_loop_tree_node *subloop_node;

  for (subloop_node = loop_node->inner;
       subloop_node != NULL;
       subloop_node = subloop_node->next)
    {
      allocate_loop_regno_allocno_map (subloop_node);
      if (subloop_node->bb == NULL)
	create_loop_allocnos (subloop_node);
      else
	create_block_allocnos (subloop_node);
      free_loop_regno_allocno_map (subloop_node);
    }
}

static void
create_all_allocnos (void)
{
  all = moves = 0;
  temp_live_through_abnormal = yara_allocate_bitmap ();
  COPY_HARD_REG_SET (prohibited_abnormal_edge_hard_regs, call_used_reg_set);
#ifdef STACK_REGS
  {
    int i;

    for (i = FIRST_STACK_REG; i <= LAST_STACK_REG; i++)
      SET_HARD_REG_BIT (prohibited_abnormal_edge_hard_regs, i);
  }
#endif
  VARRAY_GENERIC_PTR_NOGC_INIT (local_live_allocnos, MAX_RECOG_OPERANDS * 4,
				"allocnos whose life changes in given insn");
  create_subloop_allocnos (yara_loop_tree_root);
  VARRAY_FREE (local_live_allocnos);
  yara_free_bitmap (temp_live_through_abnormal);
#if 0
  if (all != 0 && yara_dump_file != NUL)
    fprintf (yara_dump_file, "Moves vs all insns: %d of %d - %d%%\n",
	     moves, all, (moves * 100 / all));
#endif
}



static int
edge_copy_compare (const void *v1p, const void *v2p)
{
  copy_t cp1 = *(copy_t *) v1p;
  copy_t cp2 = *(copy_t *) v2p;
  int regno1, regno2, diff, live1, live2;
  struct yara_loop_tree_node *loop_node1 = COPY_POINT (cp1).u.e.loop_node;
  struct yara_loop_tree_node *loop_node2 = COPY_POINT (cp2).u.e.loop_node;

  yara_assert (COPY_POINT (cp1).point_type == COPY_POINT (cp2).point_type
	       && (COPY_POINT (cp1).point_type == ON_EDGE_SRC
		   || COPY_POINT (cp1).point_type == ON_EDGE_DST));
#if 0
  if (loop_node1 == NULL)
    {
      if (loop_node2 == NULL)
	return COPY_NUM (cp1) - COPY_NUM (cp2);
      return -1;
    }
  else if (loop_node2 == NULL)
    return 1;
#endif
  regno1 = ALLOCNO_REGNO (COPY_SRC (cp1));
  regno2 = ALLOCNO_REGNO (COPY_SRC (cp2));
  yara_assert (regno1 == ALLOCNO_REGNO (COPY_DST (cp1))
	       && regno2 == ALLOCNO_REGNO (COPY_DST (cp2)));
  live1 = bitmap_bit_p (loop_node1->regno_refs, regno1);
  live2 = bitmap_bit_p (loop_node2->regno_refs, regno2);
  if (COPY_POINT (cp1).point_type == ON_EDGE_SRC)
    {
      if ((diff = (loop_node2->loop->depth - loop_node1->loop->depth)) != 0)
	return diff;
      return live2 - live1;
    }
  else if ((diff = loop_node1->loop->depth - loop_node2->loop->depth) != 0)
    return diff;
  else
    return live1 - live2;
#if 0
  return (COPY_POINT (cp1).point_type == ON_EDGE_SRC
	  ? COPY_POINT (cp2).u.e.depth - COPY_POINT (cp1).u.e.depth
	  : COPY_POINT (cp1).u.e.depth - COPY_POINT (cp2).u.e.depth);
#endif
}

static void
sort_copies_on_edges (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;

  FOR_EACH_BB (bb)
    {
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  at_edge_start_copies [(size_t) e->aux]
	    = sort_copy_list (at_edge_start_copies [(size_t) e->aux],
			      edge_copy_compare);
	  at_edge_end_copies [(size_t) e->aux]
	    = sort_copy_list (at_edge_end_copies [(size_t) e->aux],
			      edge_copy_compare);
	}
    }
}



#define REGION_ALLOCNO_CONFLICTS

#ifdef REGION_ALLOCNO_CONFLICTS
static sbitmap region_conflict_sbitmap;
#endif

static void
process_allocno_and_copy_list_for_conflicts (allocno_t a, copy_t list)
{
  int a_start, num;
  copy_t cp;

  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    {
      if (COPY_DST (cp) != COPY_SRC (cp))
	create_copy_conflict (a, cp);
#ifdef REGION_ALLOCNO_CONFLICTS
      a_start = ALLOCNO_NUM (a) * allocnos_num;
      num = a_start + ALLOCNO_NUM (COPY_DST (cp));
      if (! TEST_BIT (region_conflict_sbitmap, num))
	{
	  SET_BIT (region_conflict_sbitmap, num);
	  create_conflict (a, COPY_DST (cp));
	}
	
#else
      create_conflict (a, COPY_DST (cp));
#endif
#ifdef REGION_ALLOCNO_CONFLICTS
      num = a_start + ALLOCNO_NUM (COPY_SRC (cp));
      if (! TEST_BIT (region_conflict_sbitmap, num))
	{
	  SET_BIT (region_conflict_sbitmap, num);
	  create_conflict (a, COPY_SRC (cp));
	}
#else
      create_conflict (a, COPY_SRC (cp));
#endif
    }
}

static void
add_conflict_to_live_through_allocno (struct live_allocno *live_allocno)
{
  allocno_t a;
  basic_block bb;
  edge e;
  HARD_REG_SET hard_regs;

  a = live_allocno->a;
  if ((bb = live_allocno->bb) != NULL)
    {
      if (live_allocno->bb_start_p)
	{
	  REG_SET_TO_HARD_REG_SET (hard_regs,
				   bb->il.rtl->global_live_at_start);
	  IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a), hard_regs);
	  process_allocno_and_copy_list_for_conflicts
	    (a, at_bb_start_copies [bb->index]);
	}
      else
	{
	  REG_SET_TO_HARD_REG_SET (hard_regs, bb->il.rtl->global_live_at_end);
	  IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a), hard_regs);
	  process_allocno_and_copy_list_for_conflicts
	    (a, at_bb_end_copies [bb->index]);
	}
    }
  else
    {
      e = live_allocno->e;
      yara_assert (e != NULL);
      REG_SET_TO_HARD_REG_SET (hard_regs,
			       e->src->il.rtl->global_live_at_end);
      IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a), hard_regs);
      REG_SET_TO_HARD_REG_SET (hard_regs,
			       e->dest->il.rtl->global_live_at_start);
      IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (a), hard_regs);
      process_allocno_and_copy_list_for_conflicts
	(a, at_edge_start_copies [(size_t) e->aux]);
      process_allocno_and_copy_list_for_conflicts
	(a, at_edge_end_copies [(size_t) e->aux]);
    }
}

static void
add_subsequent_copy_conflicts (copy_t cp)
{
  allocno_t dst = COPY_DST (cp);
  allocno_t src = COPY_SRC (cp);
  copy_t next_cp;
#ifdef REGION_ALLOCNO_CONFLICTS
  int dst_start, num;
#endif

  for (next_cp = COPY_NEXT_COPY (cp);
       next_cp != NULL;
       next_cp = COPY_NEXT_COPY (next_cp))
    {
      yara_assert (COPY_DST (next_cp) != NULL && COPY_SRC (next_cp) != NULL);
      /* We have situation: a1 <- a0 ... a2 <- a1.  a1 in this case is
         dying??? in the 2nd copy and there are no conflicts a2 with
         the copies, a2, and subsequent copies and their
         destinations.  */
      if (dst == COPY_SRC (next_cp))
	break;
      if (COPY_DST (next_cp) != COPY_SRC (next_cp))
	create_copy_conflict (dst, next_cp);
#ifdef REGION_ALLOCNO_CONFLICTS
      dst_start = ALLOCNO_NUM (dst) * allocnos_num;
      num = dst_start + ALLOCNO_NUM (COPY_DST (next_cp));
      if (! TEST_BIT (region_conflict_sbitmap, num))
	{
	  SET_BIT (region_conflict_sbitmap, num);
	  create_conflict (dst, COPY_DST (next_cp));
	}
	
#else
      create_conflict (dst, COPY_DST (next_cp));
#endif
#ifdef REGION_ALLOCNO_CONFLICTS
      num = dst_start + ALLOCNO_NUM (COPY_SRC (next_cp));
      if (! TEST_BIT (region_conflict_sbitmap, num))
	{
	  SET_BIT (region_conflict_sbitmap, num);
	  create_conflict (dst, COPY_SRC (next_cp));
	}
#else
      create_conflict (dst, COPY_SRC (next_cp));
#endif
#ifdef REGION_ALLOCNO_CONFLICTS
      /* We still need to check this because source of the next copy
	 might be assigned at the end of the source basic block.  */
      num = (ALLOCNO_NUM (src) * allocnos_num
	     + ALLOCNO_NUM (COPY_SRC (next_cp)));
      if (! TEST_BIT (region_conflict_sbitmap, num))
	{
	  SET_BIT (region_conflict_sbitmap, num);
	  create_conflict (src, COPY_SRC (next_cp));
	}
#else
      create_conflict (src, COPY_SRC (next_cp));
#endif
      if (COPY_DST (cp) != COPY_SRC (cp))
	create_copy_conflict (COPY_SRC (next_cp), cp);
    }
}

static void
add_region_allocno_list_conflicts (copy_t list, HARD_REG_SET dst_live,
				   HARD_REG_SET src_live)
{
  copy_t cp;

  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    {
      add_subsequent_copy_conflicts (cp);
      IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (COPY_DST (cp)), dst_live);
      IOR_HARD_REG_SET (ALLOCNO_HARD_REG_CONFLICTS (COPY_SRC (cp)), src_live);
    }
}

/* It should be called after sorting copies on edges.  */
static void
add_region_allocno_copy_conflicts (void)
{
  int i;
  HARD_REG_SET hard_regs, hard_regs2;
  basic_block bb;
  edge_iterator ei;
  edge e;

#ifdef REGION_ALLOCNO_CONFLICTS
  region_conflict_sbitmap = sbitmap_alloc (allocnos_num * allocnos_num);
  sbitmap_zero (region_conflict_sbitmap);
#endif
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (live_through_allocnos_varray); i++)
    add_conflict_to_live_through_allocno
      (VARRAY_GENERIC_PTR (live_through_allocnos_varray, i));
  FOR_EACH_BB (bb)
    {
      REG_SET_TO_HARD_REG_SET (live_hard_regs,
			       bb->il.rtl->global_live_at_start);
      add_region_allocno_list_conflicts
	(at_bb_start_copies [bb->index], live_hard_regs, live_hard_regs);
      REG_SET_TO_HARD_REG_SET (hard_regs, bb->il.rtl->global_live_at_end);
      add_region_allocno_list_conflicts
	(at_bb_end_copies [bb->index], hard_regs, hard_regs);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  REG_SET_TO_HARD_REG_SET (hard_regs,
				   e->src->il.rtl->global_live_at_end);
	  REG_SET_TO_HARD_REG_SET (hard_regs2,
				   e->dest->il.rtl->global_live_at_start);
	  IOR_HARD_REG_SET (hard_regs, hard_regs2);
	  add_region_allocno_list_conflicts
	    (at_edge_start_copies [(size_t) e->aux], hard_regs, hard_regs2);
	  add_region_allocno_list_conflicts
	    (at_edge_end_copies [(size_t) e->aux], hard_regs, hard_regs2);
	}
    }
#ifdef REGION_ALLOCNO_CONFLICTS
  sbitmap_free (region_conflict_sbitmap);
#endif
}



#ifdef ENABLE_YARA_CHECKING

/* The function checks that list of copies with regnos in
   LIVE_THROUGH_ABNORMAL have the same source and destination and
   conflicting with prohibited abnormal hard registers.  */
static void
check_abnormal_copy_list (copy_t cp, bitmap live_through_abnormal)
{
  for (; cp != NULL; cp = COPY_NEXT_COPY (cp))
    {
      if (live_through_abnormal != NULL
	  && ! bitmap_bit_p (live_through_abnormal,
			     ALLOCNO_REGNO (COPY_SRC (cp))))
	continue;
      if (COPY_SRC (cp) != COPY_DST (cp))
	abort ();
      GO_IF_HARD_REG_SUBSET (prohibited_abnormal_edge_hard_regs,
			     ALLOCNO_HARD_REG_CONFLICTS (COPY_SRC (cp)), ok1);
      abort ();
    ok1:
      GO_IF_HARD_REG_SUBSET (prohibited_abnormal_edge_hard_regs,
			     ALLOCNO_HARD_REG_CONFLICTS (COPY_DST (cp)), ok2);
      abort ();
    ok2:
      ;
    }
}

/* The function checks that copy on abnormal edges will be not
   generated and that allocnos living on abnormal edges conflicts with
   hard registers clobbered by calls.  */
static void
check_abnormal_edges (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;
  bitmap live_through_abnormal, bb_live_through_abnormal, temp;

  live_through_abnormal = yara_allocate_bitmap ();
  temp = yara_allocate_bitmap ();
  FOR_EACH_BB (bb)
    {
      bb_live_through_abnormal = NULL;
      FOR_EACH_EDGE (e, ei, bb->succs)
	if ((e->flags & EDGE_COMPLEX) != 0)
	  {
	    if (bb_live_through_abnormal == NULL)
	      {
		bb_live_through_abnormal = live_through_abnormal;
		bitmap_and (bb_live_through_abnormal,
			    e->src->il.rtl->global_live_at_end,
			    e->dest->il.rtl->global_live_at_start);
	      }
	    else
	      {
		bitmap_and (temp,
			    e->src->il.rtl->global_live_at_end,
			    e->dest->il.rtl->global_live_at_start);
		bitmap_ior_into (bb_live_through_abnormal, temp);
	      }
	    check_abnormal_copy_list (at_edge_start_copies [(size_t) e->aux],
				      NULL);
	    check_abnormal_copy_list (at_edge_end_copies [(size_t) e->aux],
				      NULL);
	  }
      if (bb_live_through_abnormal != NULL)
 	check_abnormal_copy_list (at_bb_end_copies [bb->index],
				  bb_live_through_abnormal);
    }
  yara_free_bitmap (temp);
  yara_free_bitmap (live_through_abnormal);
}

#endif /* #ifdef ENABLE_YARA_CHECKING */



static int *allocno_value_vec;

static bool
propagate_value (allocno_t dst, allocno_t src)
{
  int old_val;
  int src_num, dst_num;
  rtx x;

  if (src == NULL || dst == NULL
      || (ALLOCNO_TYPE (dst) == INSN_ALLOCNO
	  && INSN_ALLOCNO_OP_MODE (dst) == OP_INOUT))
    return false;
  if (ALLOCNO_TYPE (src) == INSN_ALLOCNO)
    {
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (src));
      if (GET_CODE (x) == SUBREG
	  && (GET_MODE_SIZE (GET_MODE (x))
	      != GET_MODE_SIZE (GET_MODE (SUBREG_REG (x)))))
	return false;
    }
  if (ALLOCNO_TYPE (dst) == INSN_ALLOCNO)
    {
      SKIP_TO_SUBREG (x, *INSN_ALLOCNO_LOC (dst));
      if (GET_CODE (x) == SUBREG
	  && (GET_MODE_SIZE (GET_MODE (x))
	      != GET_MODE_SIZE (GET_MODE (SUBREG_REG (x)))))
	return false;
    }
  dst_num = ALLOCNO_NUM (dst);
  src_num = ALLOCNO_NUM (src);
  old_val = allocno_value_vec [dst_num];
  if (allocno_value_vec [src_num] == -1)
    allocno_value_vec [dst_num] = dst_num;
  else if (old_val != allocno_value_vec [src_num])
    {
      if (old_val != -1)
	allocno_value_vec [dst_num] = dst_num;
      else
	allocno_value_vec [dst_num] = allocno_value_vec [src_num];
    }
  return old_val != allocno_value_vec [dst_num];
}


static bool
propagate_copy_value (copy_t list)
{
  copy_t cp;
  bool changed_p = false;

  for (cp = list; cp != NULL; cp = COPY_NEXT_COPY (cp))
    changed_p = (propagate_value (COPY_DST (cp), COPY_SRC (cp)) || changed_p);
  return changed_p;
}

static void
make_vn (void)
{
  bool changed_p;
  int i, j, k, n, a_num, val;
  int *rts_order;
  basic_block bb;
  edge_iterator ei;
  edge e;
  rtx insn, bound, set;
  allocno_t a, a2;
  allocno_t *vec;

  allocno_value_vec = yara_allocate (sizeof (int) * allocnos_num);
  for (i = 0; i < allocnos_num; i++)
    allocno_value_vec [i] = -1;
  rts_order = yara_allocate (sizeof (int)
			     * (n_basic_blocks - NUM_FIXED_BLOCKS));
  post_order_compute (rts_order, false);
  n = 0;
  for (;;)
    {
      changed_p = false;
      if (yara_dump_file != NULL)
	fprintf (yara_dump_file, "Allocno VN Iteration %d\n", ++n);
      for (i = n_basic_blocks - NUM_FIXED_BLOCKS - 1; i >= 0; i--)
	{
	  yara_assert (rts_order [i] >= 0);
	  bb = BASIC_BLOCK (rts_order [i]);
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    {
	      if (propagate_copy_value
		  (at_edge_start_copies [(size_t) e->aux]))
		changed_p = true;
	      if (propagate_copy_value (at_edge_end_copies [(size_t) e->aux]))
		changed_p = true;
	    }
	  if (propagate_copy_value (at_bb_start_copies [bb->index]))
	    changed_p = true;
	  bound = NEXT_INSN (BB_END (bb));
	  for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	    if (INSN_P (insn))
	      {
		if (propagate_copy_value (before_insn_copies
					  [INSN_UID (insn)]))
		  changed_p = true;
		if ((set = single_set (insn)) != NULL_RTX
		    && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
		    && ! HARD_REGISTER_P (SET_DEST (set))
		    && ! HARD_REGISTER_P (SET_SRC (set))
		    && ! side_effects_p (set)
		    && ! mode_multi_reg_p [GET_MODE (SET_DEST (set))])
		  {
		    a = insn_allocno (SET_SRC (set), insn);
		    a2 = insn_allocno (SET_DEST (set), insn);
		    if (propagate_value (a2, a))
		      changed_p = true;
		  }
		else
		  {
		    for (a = insn_allocnos [INSN_UID (insn)];
			 a != NULL;
			 a = INSN_ALLOCNO_NEXT (a))
		      if (INSN_ALLOCNO_OP_MODE (a) == OP_OUT
			  || INSN_ALLOCNO_OP_MODE (a) == OP_INOUT)
			{
			  a_num = ALLOCNO_NUM (a);
			  if (allocno_value_vec [a_num] != a_num)
			    changed_p = true;
			  allocno_value_vec [a_num] = a_num;
			}
		  }
		if (propagate_copy_value (after_insn_copies [INSN_UID (insn)]))
		  changed_p = true;
	      }
	  if (propagate_copy_value (at_bb_end_copies [bb->index]))
	    changed_p = true;
	}
      if (! changed_p)
	break;
    }
  yara_free (rts_order);
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      val = allocno_value_vec [i];
      if (val < 0 || mode_multi_reg_p [ALLOCNO_MODE (a)])
	/* We don't remove conflicts for multi-reg pseudos because
	   they might overlap.  */
	continue;
      vec = ALLOCNO_CONFLICT_VEC (a);
      for (j = 0; (a2 = vec [j]) != NULL; j++)
	{
	  if (val == allocno_value_vec [ALLOCNO_NUM (a2)])
	    {
	      if (yara_dump_file != NULL)
		fprintf (yara_dump_file, "Removing conflict %d(%d)->%d(%d)\n",
			 ALLOCNO_NUM (a), ALLOCNO_REGNO (a),
			 ALLOCNO_NUM (a2), ALLOCNO_REGNO (a2));
	      for (k = j; vec [k] != NULL; k++)
		vec [k] = vec [k + 1];
	      j--;
	    }
	}
    }
  yara_free (allocno_value_vec);
}



static void
setup_allocno_frequency (allocno_t a)
{
  int freq;
  copy_t cp;

  freq = (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	  ? BLOCK_FOR_INSN (INSN_ALLOCNO_INSN (a))->frequency : 0);
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = COPY_NEXT_SRC_COPY (cp))
    if (COPY_SRC (cp) != COPY_DST (cp))
      freq += COPY_FREQ (cp);
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = COPY_NEXT_DST_COPY (cp))
    if (COPY_SRC (cp) != COPY_DST (cp))
      freq += COPY_FREQ (cp);
  ALLOCNO_FREQ (a) = freq;
}

static void
setup_all_allocno_frequencies (void)
{
  int i;

  for (i = 0; i < allocnos_num; i++)
    setup_allocno_frequency (allocnos [i]);
}



/* Array of all cans and its size.  */
can_t *cans;
int cans_num;

/* Function allocating space for cans.  */
static void
initiate_cans (void)
{
  cans = yara_allocate (sizeof (can_t) * allocnos_num);
  cans_num = 0;
}

/* Function creating a new can.  */
can_t
create_can (void)
{
  can_t can;

  can = yara_allocate (sizeof (struct can));
  CAN_NUM (can) = cans_num;
  cans [cans_num++] = can;
  CAN_ALLOCNOS (can) = NULL;
  CAN_CONFLICT_CAN_VEC (can) = NULL;
  CLEAR_HARD_REG_SET (CAN_CONFLICT_HARD_REGS (can));
  CLEAR_HARD_REG_SET (CAN_BIASED_HARD_REGS (can));
  CAN_FREQ (can) = 0;
  CAN_IN_GRAPH_P (can) = false;
  CAN_ASSIGNED_P (can) = false;
  CAN_HARD_REGNO (can) = -1;
  CAN_SLOTNO (can) = CAN_NUM (can);
  CAN_CALL_FREQ (can) = 0;
  CAN_CALL_P (can) = false;
  CAN_GLOBAL_P (can) = false || (YARA_PARAMS & YARA_NO_LOCAL_CAN) == 0;
  CAN_MODE (can) = VOIDmode;
  CAN_HARD_REG_COSTS (can) = NULL;
  CAN_COPIES (can) = NULL;
  CAN_LEFT_CONFLICTS_NUM (can) = -1;
  CAN_COVER_CLASS (can) = NO_REGS;
  CAN_MEMORY_COST (can) = CAN_COVER_CLASS_COST (can) = 0;
  CAN_NEXT_BUCKET_CAN (can) = CAN_PREV_BUCKET_CAN (can) = NULL;
  return can;
}

void
setup_can_call_info (can_t can)
{
  allocno_t a, *can_allocnos;
  int i;

  CAN_CALL_P (can) = false;
  can_allocnos = CAN_ALLOCNOS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    if (ALLOCNO_CALL_CROSS_P (a))
      {
	CAN_CALL_FREQ (can) += ALLOCNO_CALL_FREQ (a);
	CAN_CALL_P (can) = true;
      }
}

/* Function freeing space for cans.  */
static void
finish_cans (void)
{
  int i;
  can_t can;
  struct can_copy *cp, *next_cp;

  for (i = cans_num - 1; i >= 0; i--)
    {
      can = cans [i];
      if (CAN_HARD_REG_COSTS (can) != NULL)
	yara_free (CAN_HARD_REG_COSTS (can));
      for (cp = CAN_COPIES (can); cp != NULL; cp = next_cp)
	{
	  next_cp = cp->next_can_copy;
	  yara_free (cp);
	}
      if (CAN_CONFLICT_CAN_VEC (can) != NULL)
	yara_free (CAN_CONFLICT_CAN_VEC (can));
      yara_free (can);
    }
  yara_free (cans);
}



/* Bitmap used to prevent repeated processing of copy to calculate
   splitted allocno frequencies.  */
static bitmap can_allocno_copy_bitmap;

int
can_freq (can_t can)
{
  int freq, i;
  allocno_t a, *can_allocnos;
  copy_t cp;

  freq = 0;
  bitmap_clear (can_allocno_copy_bitmap);
  can_allocnos = CAN_ALLOCNOS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    {
      for (cp = ALLOCNO_DST_COPIES (a);
	   cp != NULL;
	   cp = COPY_NEXT_DST_COPY (cp))
	if (COPY_SRC (cp) != COPY_DST (cp)
	    && (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		|| ALLOCNO_TYPE (COPY_SRC (cp)) == INSN_ALLOCNO)
	    && ! bitmap_bit_p (can_allocno_copy_bitmap, COPY_NUM (cp)))
	  {
	    freq += COPY_FREQ (cp);
	    bitmap_set_bit (can_allocno_copy_bitmap, COPY_NUM (cp));
	  }
      for (cp = ALLOCNO_SRC_COPIES (a);
	   cp != NULL;
	   cp = COPY_NEXT_SRC_COPY (cp))
	if (COPY_SRC (cp) != COPY_DST (cp)
	    && (ALLOCNO_TYPE (a) == INSN_ALLOCNO
		|| ALLOCNO_TYPE (COPY_DST (cp)) == INSN_ALLOCNO)
	    && ! bitmap_bit_p (can_allocno_copy_bitmap, COPY_NUM (cp)))
	  {
	    freq += COPY_FREQ (cp);
	    bitmap_set_bit (can_allocno_copy_bitmap, COPY_NUM (cp));
	  }
    }
  return freq;
}



static allocno_t *union_first;
static allocno_t *union_next;

/* Return true if an allocno from the union given by A1 conflicts with
   an allocno from the union given by A2.  */
static bool
union_conflict_p (allocno_t a1, allocno_t a2)
{
  allocno_t curr_a1, curr_a2;

  for (curr_a1 = a1;;)
    {
      for (curr_a2 = a2;;)
	{
	  if (ALLOCNO_REGNO (curr_a1) != ALLOCNO_REGNO (curr_a2)
	      && conflict_p (curr_a1, curr_a2))
	    return true;
	  curr_a2 = union_next [ALLOCNO_NUM (curr_a2)];
	  if (curr_a2 == a2)
	    break;
	}
      curr_a1 = union_next [ALLOCNO_NUM (curr_a1)];
      if (curr_a1 == a1)
	break;
    }
  return false;
}

static void
merge_allocnos (allocno_t to, allocno_t from)
{
  allocno_t a, last_a, tmp_a;

  yara_assert (to != from && to != NULL && from != NULL
	       && ALLOCNO_NUM (to) < ALLOCNO_NUM (from));
  yara_assert (to == union_first [ALLOCNO_NUM (to)]
	       && from == union_first [ALLOCNO_NUM (from)]);
  /* Finding last and seting up ALLOCNO_UNION_FIRST in set given by
     from.  */
  for (a = from;;)
    {
      last_a = a;
      union_first [ALLOCNO_NUM (a)] = to;
      a = union_next [ALLOCNO_NUM (a)];
      if (a == from)
	break;
    }
  /* Merge set given by FROM into one given by FIRST.  */
  tmp_a = union_next [ALLOCNO_NUM (to)];
  union_next [ALLOCNO_NUM (to)] = from;
  union_next [ALLOCNO_NUM (last_a)] = tmp_a;
}

static void
remove_allocnos_from_union (allocno_t a1, allocno_t a2)
{
  allocno_t first, second, a, last_a;
  int i;

  yara_assert (a1 != a2);
  first = union_first [ALLOCNO_NUM (a1)];
  second = union_first [ALLOCNO_NUM (a2)];
  yara_assert (first == second);
  for (i = 0, last_a = a = first; i != 2;)
    {
      a = union_next [ALLOCNO_NUM (a)];
      if (a == a1 || a == a2)
	{
	  union_next [ALLOCNO_NUM (last_a)] = union_next [ALLOCNO_NUM (a)];
	  i++;
	}
      else
	last_a = a;
    }
}

/* Function making allocnos partition for cans.  It is a typical
   union-find algorithm.  */
static void
make_initial_allocno_can_partition (void)
{
  int i, regno;
  copy_t cp;
  allocno_t a, curr_a, src, dst, tmp_a;

  /* Initial partition: each allocno forms own set.  */
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      union_first [i] = a;
      union_next [i] = a;
    }
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a != NULL && (regno = ALLOCNO_REGNO (a)) >= 0
	  && ! HARD_REGISTER_NUM_P (regno)
	  && ALLOCNO_SRC_COPIES (a) == NULL && ALLOCNO_DST_COPIES (a) == NULL
	  && ALLOCNO_TYPE (a) == INSN_ALLOCNO)
	{
	  /* Special case unconnected pseudo-register insn allocno.
	     Attach it with insn allocno with the same register.  */
	  for (curr_a = insn_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))];
	       curr_a != NULL;
	       curr_a = INSN_ALLOCNO_NEXT (curr_a))
	    if (a != curr_a && regno == ALLOCNO_REGNO (curr_a))
	      {
		src = union_first [ALLOCNO_NUM (a)];
		dst = union_first [ALLOCNO_NUM (curr_a)];
		yara_assert (! union_conflict_p (src, dst));
		if (ALLOCNO_NUM (src) > ALLOCNO_NUM (dst))
		  merge_allocnos (dst, src);
		else
		  merge_allocnos (src, dst);
		break;
	      }
	}
    }
  /* Unite allocnos connected by a copy.  */
  for (i = 0; i < copies_num; i++)
    {
      cp = copies [i];
      src = COPY_SRC (cp);
      dst = COPY_DST (cp);
      if (src == NULL || dst == NULL)
	continue;
      yara_assert (ALLOCNO_REGNO (src) >= 0
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (src))
		   && ALLOCNO_REGNO (dst) >= 0
		   && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst)));
      src = union_first [ALLOCNO_NUM (src)];
      dst = union_first [ALLOCNO_NUM (dst)];
      if (src == dst)
	continue;
      if (ALLOCNO_NUM (src) > ALLOCNO_NUM (dst))
	{
	  tmp_a = src;
	  src = dst;
	  dst = tmp_a;
	}
      merge_allocnos (src, dst);
    }
}

static void
remove_allocno_from_vec (allocno_t *allocno_vec, allocno_t allocno_to_remove)
{
  int i, j;
  allocno_t a;

  for (i = 0; (a = allocno_vec [i]) != NULL; i++)
    if (a == allocno_to_remove)
      {
	for (j = i; allocno_vec [j] != NULL; j++)
	  allocno_vec [j] = allocno_vec [j + 1];
	break;
      }
  yara_assert (a != NULL);
}

static void
remove_copy_from_vec (copy_t *copy_vec, copy_t cp_to_remove)
{
  int i, j;
  copy_t cp;

  for (i = 0; (cp = copy_vec [i]) != NULL; i++)
    if (cp == cp_to_remove)
      {
	for (j = i; copy_vec [j] != NULL; j++)
	  copy_vec [j] = copy_vec [j + 1];
	break;
      }
  yara_assert (cp != NULL);
}

static copy_t
remove_copy_from_next_copy_list (copy_t list, copy_t copy_to_remove)
{
  copy_t prev, curr;

  for (prev = NULL, curr = list;
       curr != NULL;
       prev = curr, curr = COPY_NEXT_COPY (curr))
    if (curr == copy_to_remove)
      {
	if (prev == NULL)
	  return COPY_NEXT_COPY (curr);
	COPY_NEXT_COPY (prev) = COPY_NEXT_COPY (curr);
	return list;
      }
  gcc_unreachable ();
}

static copy_t
remove_copy_from_allocno_copy_list (copy_t list, copy_t copy_to_remove,
				    bool src_p)
{
  copy_t prev, curr;

  if (src_p)
    {
      for (prev = NULL, curr = list;
	   curr != NULL;
	   prev = curr, curr = COPY_NEXT_SRC_COPY (curr))
	if (curr == copy_to_remove)
	  {
	    if (prev == NULL)
	      return COPY_NEXT_SRC_COPY (curr);
	    COPY_NEXT_SRC_COPY (prev) = COPY_NEXT_SRC_COPY (curr);
	    return list;
	  }
    }
  else
    {
      for (prev = NULL, curr = list;
	   curr != NULL;
	   prev = curr, curr = COPY_NEXT_DST_COPY (curr))
	if (curr == copy_to_remove)
	  {
	    if (prev == NULL)
	      return COPY_NEXT_DST_COPY (curr);
	    COPY_NEXT_DST_COPY (prev) = COPY_NEXT_DST_COPY (curr);
	    return list;
	  }
    }
  gcc_unreachable ();
}

static void
remove_copy (copy_t cp)
{
  int i;
  allocno_t a, src, dst;
  allocno_t *vec;
  
  vec = COPY_ALLOCNO_CONFLICT_VEC (cp);
  if (vec != NULL)
    for (i = 0; (a = vec [i]) != NULL; i++)
      remove_copy_from_vec (ALLOCNO_COPY_CONFLICT_VEC (a), cp);
  switch (cp->point.point_type)
    {
    case BEFORE_INSN:
      before_insn_copies [INSN_UID (cp->point.u.insn)]
	= remove_copy_from_next_copy_list (before_insn_copies
					   [INSN_UID (cp->point.u.insn)], cp);
      break;
    case AFTER_INSN:
      after_insn_copies [INSN_UID (cp->point.u.insn)]
	= remove_copy_from_next_copy_list (after_insn_copies
					   [INSN_UID (cp->point.u.insn)], cp);
      break;
    case AT_BB_START:
      at_bb_start_copies [cp->point.u.bb->index]
	= remove_copy_from_next_copy_list (at_bb_start_copies
					   [cp->point.u.bb->index], cp);
      break;
    case AT_BB_END:
      at_bb_end_copies [cp->point.u.bb->index]
	= remove_copy_from_next_copy_list (at_bb_end_copies
					   [cp->point.u.bb->index], cp);
      break;
    case ON_EDGE_SRC:
      at_edge_start_copies [(size_t) cp->point.u.e.e->aux]
	= remove_copy_from_next_copy_list (at_edge_start_copies
					   [(size_t) cp->point.u.e.e->aux],
					   cp);
      break;
    case ON_EDGE_DST:
      at_edge_end_copies [(size_t) cp->point.u.e.e->aux]
	= remove_copy_from_next_copy_list (at_edge_end_copies
					   [(size_t) cp->point.u.e.e->aux],
					   cp);
      break;
    default:
      gcc_unreachable ();
    }
  src = COPY_SRC (cp);
  ALLOCNO_SRC_COPIES (src)
    = remove_copy_from_allocno_copy_list (ALLOCNO_SRC_COPIES (src), cp, true);
  dst = COPY_DST (cp);
  ALLOCNO_DST_COPIES (dst)
    = remove_copy_from_allocno_copy_list (ALLOCNO_DST_COPIES (dst), cp, false);
  copies [COPY_NUM (cp)] = NULL;
  free_copy (cp);
}

static allocno_t
remove_allocno_from_next_allocno_list (allocno_t list,
				       allocno_t allocno_to_remove)
{
  allocno_t prev, curr;

  for (prev = NULL, curr = list;
       curr != NULL;
       prev = curr, curr = INSN_ALLOCNO_NEXT (curr))
    if (curr == allocno_to_remove)
      {
	if (prev == NULL)
	  return INSN_ALLOCNO_NEXT (curr);
	INSN_ALLOCNO_NEXT (prev) = INSN_ALLOCNO_NEXT (curr);
	return list;
      }
  gcc_unreachable ();
}

static void
remove_allocno (allocno_t a)
{
  int i;
  allocno_t another_a;
  allocno_t *vec;
  copy_t *copy_vec;
  copy_t cp, next_cp;

  vec = ALLOCNO_CONFLICT_VEC (a);
  if (vec != NULL)
    for (i = 0; (another_a = vec [i]) != NULL; i++)
      remove_allocno_from_vec (ALLOCNO_CONFLICT_VEC (another_a), a);
  copy_vec = ALLOCNO_COPY_CONFLICT_VEC (a);
  if (copy_vec != NULL)
    for (i = 0; (cp = copy_vec [i]) != NULL; i++)
      remove_allocno_from_vec (COPY_ALLOCNO_CONFLICT_VEC (cp), a);
  for (cp = ALLOCNO_SRC_COPIES (a); cp != NULL; cp = next_cp)
    {
      next_cp = COPY_NEXT_SRC_COPY (cp);
      remove_copy (cp);
    }
  for (cp = ALLOCNO_DST_COPIES (a); cp != NULL; cp = next_cp)
    {
      next_cp = COPY_NEXT_DST_COPY (cp);
      remove_copy (cp);
    }
  yara_assert (ALLOCNO_DST_COPIES (a) == NULL
	       && ALLOCNO_SRC_COPIES (a) == NULL);
  if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
    insn_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))]
      = remove_allocno_from_next_allocno_list
        (insn_allocnos [INSN_UID (INSN_ALLOCNO_INSN (a))], a);
  allocnos [ALLOCNO_NUM (a)] = NULL;
  free_allocno (a);
}

static void
get_move_insn_allocnos_and_copies (rtx move_insn,
				   allocno_t *insn_src, allocno_t *insn_dst,
				   copy_t *src_copy, copy_t *dst_copy)
{
  allocno_t curr_a;

  *insn_src = NULL;
  *insn_dst = NULL;
  for (curr_a = insn_allocnos [INSN_UID (move_insn)];
       curr_a != NULL;
       curr_a = INSN_ALLOCNO_NEXT (curr_a))
    if (INSN_ALLOCNO_OP_MODE (curr_a) == OP_IN)
      {
	yara_assert (*insn_src == NULL);
	*insn_src = curr_a;
      }
    else if (INSN_ALLOCNO_OP_MODE (curr_a) == OP_OUT)
      {
	yara_assert (*insn_dst == NULL);
	*insn_dst = curr_a;
      }
    else
      gcc_unreachable ();
  yara_assert (*insn_src != NULL && *insn_dst != NULL);
  *src_copy = ALLOCNO_DST_COPIES (*insn_src);
  *dst_copy = ALLOCNO_SRC_COPIES (*insn_dst);
  yara_assert (*src_copy != NULL && *dst_copy != NULL
	       && ALLOCNO_SRC_COPIES (*insn_src) == NULL
	       && ALLOCNO_DST_COPIES (*insn_dst) == NULL
	       && COPY_NEXT_DST_COPY (*src_copy) == NULL
	       && COPY_NEXT_SRC_COPY (*dst_copy) == NULL);
  
}

struct move_info
{
  allocno_t first, second;
  int freq;
};

static allocno_t
get_duplication_allocno (allocno_t a, bool commutative_p)
{
  int op_num, curr_alt, c, original;
  const char *constraints;
  struct insn_op_info *info;
  rtx insn;
  allocno_t another_a;

  op_num = INSN_ALLOCNO_TYPE (a) - OPERAND_BASE;
  if (op_num < 0)
    return NULL;
  insn = INSN_ALLOCNO_INSN (a);
  info = insn_infos [INSN_UID (insn)];
  if (info == NULL || info->n_alts == 0)
    return NULL;
  for (original = -1, curr_alt = 0; curr_alt < info->n_alts; curr_alt++)
    {
      constraints = info->op_constraints [op_num * info->n_alts + curr_alt];
      for (;
	   (c = *constraints);
	   constraints += CONSTRAINT_LEN (c, constraints))
	if (c == ' ' || c == '\t' || c == '=' || c == '+' || c == '*'
	    || c == '&' || c == '%' || c == '?' || c == '!')
	  ;
	else if ('0' <= c && c <= '9')
	  {
	    if (original != -1 && original != c)
	      break;
	    original = c;
	  }
	else
	  {
	    yara_assert (c != '#');
	    break;
	  }
    }
  if (curr_alt < info->n_alts || original == -1)
    {
      another_a = INSN_ALLOCNO_COMMUTATIVE (a);
      if (commutative_p && another_a != NULL)
	return get_duplication_allocno (another_a, false);
      return NULL;
    }
  return insn_op_allocnos [INSN_UID (insn)] [original - '0'];
}

static struct move_info *
get_move (allocno_t src, allocno_t dst, int freq)
{
  allocno_t first, second, temp;
  struct move_info *move;

  first = union_first [ALLOCNO_NUM (src)];
  second = union_first [ALLOCNO_NUM (dst)];
  if (ALLOCNO_NUM (first) > ALLOCNO_NUM (second))
    {
      temp = first;
      first = second;
      second = temp;
    }
  move = yara_allocate (sizeof (struct move_info));
  move->first = first;
  move->second = second;
  move->freq = freq;
  return move;
}

static int
move_freq_compare_func (const void *v1p, const void *v2p)
{
  struct move_info *m1 = *(struct move_info **) v1p;
  struct move_info *m2 = *(struct move_info **) v2p;

  return m1->freq - m2->freq;
}

static void
add_copy_to_list_end (copy_t *head_loc, copy_t cp)
{
  copy_t prev, curr;

  for (prev = NULL, curr = *head_loc;
       curr != NULL;
       prev = curr, curr = COPY_NEXT_COPY (curr))
    ;
  if (prev != NULL)
    COPY_NEXT_COPY (prev) = cp;
  else
    {
      COPY_NEXT_COPY (cp) = *head_loc;
      *head_loc = cp;
    }
}

static int
move_pair_compare_func (const void *v1p, const void *v2p)
{
  struct move_info *m1 = *(struct move_info **) v1p;
  struct move_info *m2 = *(struct move_info **) v2p;
  int diff = ALLOCNO_NUM (m1->first) - ALLOCNO_NUM (m2->first);

  if (diff != 0)
    return diff;
  return ALLOCNO_NUM (m1->second) - ALLOCNO_NUM (m2->second);
}

static void
change_move_insn_by_copy (rtx move_insn, copy_t dst_copy, copy_t src_copy)
{
  rtx prev_insn;
  struct point point;
  allocno_t src, dst, insn_src, insn_dst;

  yara_assert (COPY_NEXT_DST_COPY (src_copy) == NULL
	       && COPY_NEXT_SRC_COPY (dst_copy) == NULL);
  src = COPY_SRC (src_copy);
  insn_src = COPY_DST (src_copy);
  dst = COPY_DST (dst_copy);
  insn_dst = COPY_SRC (dst_copy);
  ALLOCNO_DST_COPIES (insn_src) = NULL;
  COPY_DST (src_copy) = dst;
  COPY_NEXT_DST_COPY (src_copy) = ALLOCNO_DST_COPIES (dst);
  ALLOCNO_DST_COPIES (dst) = src_copy;
  yara_assert (COPY_POINT (src_copy).point_type == BEFORE_INSN
	       && COPY_POINT (src_copy).u.insn == move_insn
	       && before_insn_copies [INSN_UID (move_insn)] == src_copy
	       && COPY_NEXT_COPY (src_copy) == NULL);
  before_insn_copies [INSN_UID (move_insn)] = NULL;
  prev_insn = prev_nonnote_insn (move_insn);
  if (prev_insn != NULL
      && (BLOCK_FOR_INSN (prev_insn) != BLOCK_FOR_INSN (move_insn)
          || ! INSN_P (prev_insn)))
    prev_insn = NULL;
  if (prev_insn == NULL_RTX)
    {
      point.point_type = AT_BB_START;
      point.u.bb = BLOCK_FOR_INSN (move_insn);
      add_copy_to_list_end (&at_bb_start_copies [point.u.bb->index],
			    src_copy);
    }
  else
    {
      point.point_type = AFTER_INSN;
      point.u.insn = prev_insn;
      add_copy_to_list_end (&after_insn_copies [INSN_UID (prev_insn)],
			    src_copy);
    }
  COPY_POINT (src_copy) = point;
  remove_allocnos_from_union (insn_src, insn_dst);
  remove_allocno (insn_src);
  remove_allocno (insn_dst);
  /* ??? Conflicts.  */
  if (yara_dump_file != NULL)
    fprintf (yara_dump_file, "changing move insn %d by copy %d\n",
	     INSN_UID (move_insn), COPY_NUM (src_copy));
  remove_insn (move_insn);
}

static GTY(()) varray_type move_insn_varray;

static void
make_aggressive_coalescing (void)
{
  int i, n, moves_to_sort;
  rtx move_insn;
  allocno_t src, dst, insn_src, insn_dst, first, second, temp;
  copy_t src_copy, dst_copy;
  struct move_info *move, *last;
  varray_type move_varray;
  basic_block bb;
  rtx insn, bound, set;

  VARRAY_RTX_INIT (move_insn_varray, yara_max_uid, "move insns");
  VARRAY_GENERIC_PTR_NOGC_INIT (move_varray, yara_max_uid, "moves");
  FOR_EACH_BB (bb)
    {
      bound = NEXT_INSN (BB_END (bb));
      for (insn = BB_HEAD (bb); insn != bound; insn = NEXT_INSN (insn))
	if (INSN_P (insn))
	  {
	    if ((set = single_set (insn)) != NULL_RTX
		&& REG_P (SET_DEST (set))
		&& ! HARD_REGISTER_P (SET_DEST (set))
		&& REG_P (SET_SRC (set))
		&& ! HARD_REGISTER_P (SET_SRC (set))
		&& ! side_effects_p (set)
		&& find_reg_note (insn, REG_DEAD, SET_SRC (set)) != NULL_RTX
		&& find_reg_note (insn, REG_RETVAL, NULL_RTX) == NULL_RTX)
	      {
		if (! flag_optimistic_coalescing)
		  continue;
		get_move_insn_allocnos_and_copies (insn, &insn_src, &insn_dst,
						   &src_copy, &dst_copy);
		src = COPY_SRC (src_copy);
		dst = COPY_DST (dst_copy);
		if (reg_equiv_constant [ALLOCNO_REGNO (src)] != NULL_RTX
		    && reg_equiv_constant [ALLOCNO_REGNO (dst)] == NULL_RTX)
		  /* Coalescing cans with allocnos which have equiv
		     constant can create problem during global
		     allocations because such move might need an
		     intermediate register.  Even if we fix that I
		     believe it will not generate a better code.  */
		  continue;
		move = get_move (src, dst, BLOCK_FOR_INSN (insn)->frequency);
		VARRAY_PUSH_GENERIC_PTR (move_varray, move);
		VARRAY_PUSH_RTX (move_insn_varray, insn);
	      }
	    else if (flag_extended_coalescing)
	      {
		for (src = insn_allocnos [INSN_UID (insn)];
		     src != NULL;
		     src = INSN_ALLOCNO_NEXT (src))
		  /* How to handle SUBREGs???  */
		  if (INSN_ALLOCNO_OP_MODE (src) == OP_IN
		      && REG_P (*INSN_ALLOCNO_LOC (src))
		      && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (src))
		      && find_reg_note (insn, REG_DEAD,
					*INSN_ALLOCNO_LOC (src)) != NULL_RTX
		      && (dst = get_duplication_allocno (src, true)) != NULL
		      && REG_P (*INSN_ALLOCNO_LOC (dst))
		      && ! HARD_REGISTER_NUM_P (ALLOCNO_REGNO (dst))
		      && ALLOCNO_MODE (src) == ALLOCNO_MODE (dst))
		    {
		      move = get_move (src, dst,
				       BLOCK_FOR_INSN (insn)->frequency);
		      VARRAY_PUSH_GENERIC_PTR (move_varray, move);
		    }
	      }
	  }
    }
  moves_to_sort = VARRAY_ACTIVE_SIZE (move_varray);
  for (; moves_to_sort != 0;)
    {
      qsort (&VARRAY_GENERIC_PTR (move_varray, 0), moves_to_sort,
	     sizeof (struct move_info *), move_pair_compare_func);
      for (n = 0, i = 1; i < moves_to_sort; i++)
	{
	  last = VARRAY_GENERIC_PTR (move_varray, n);
	  move = VARRAY_GENERIC_PTR (move_varray, i);
	  yara_assert (move->first != move->second);
	  if (last->first == move->first && last->second == move->second)
	    {
	      last->freq += move->freq;
	      yara_free (move);
	    }
	  else
	    {
	      n++;
	      VARRAY_GENERIC_PTR (move_varray, n) = move;
	    }
	}
      for (; n != (int) VARRAY_ACTIVE_SIZE (move_varray) - 1;)
	VARRAY_POP (move_varray);
      qsort (&VARRAY_GENERIC_PTR (move_varray, 0), n,
	     sizeof (struct move_info *), move_freq_compare_func);
      for (i = n; i >= 0; i--)
	{
	  move = VARRAY_GENERIC_PTR (move_varray, i);
	  first = move->first;
	  second = move->second;
	  if (! union_conflict_p (move->first, move->second))
	    break;
	}
      if (i < 0)
	break;
      merge_allocnos (first, second);
      moves_to_sort = i;
      for (i = 0; i < moves_to_sort; i++)
	{
	  move = VARRAY_GENERIC_PTR (move_varray, i);
	  if (move->first == second)
	    move->first = first;
	  if (move->second == second)
	    move->second = first;
	  if (ALLOCNO_NUM (move->first) > ALLOCNO_NUM (move->second))
	    {
	      temp = move->first;
	      move->first = move->second;
	      move->second = temp;
	    }
	  yara_assert
	    (union_first [ALLOCNO_NUM (move->first)] == move->first
	     && union_first [ALLOCNO_NUM (move->second)] == move->second);
	}
    }
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (move_varray); i++)
    yara_free (VARRAY_GENERIC_PTR (move_varray, i));
  VARRAY_FREE (move_varray);
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (move_insn_varray); i++)
    {
      move_insn = VARRAY_RTX (move_insn_varray, i);
      get_move_insn_allocnos_and_copies (move_insn, &insn_src, &insn_dst,
					 &src_copy, &dst_copy);
      src = COPY_SRC (src_copy);
      dst = COPY_DST (dst_copy);
      first = union_first [ALLOCNO_NUM (src)];
      second = union_first [ALLOCNO_NUM (dst)];
      if (first == second)
	change_move_insn_by_copy (move_insn, dst_copy, src_copy);
    }
}

/* Function creating all cans.  */
static void
create_cans (void)
{
  int i, can_allocnos_num;
  enum machine_mode mode;
  allocno_t a, first, next, *can_allocnos;
  can_t can;

  union_first = yara_allocate (sizeof (allocno_t) * allocnos_num);
  union_next = yara_allocate (sizeof (allocno_t) * allocnos_num);
  make_initial_allocno_can_partition ();
  make_aggressive_coalescing ();
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (a == NULL)
	continue;
      if (union_next [i] == NULL)  /* already processed */
	continue;
      if (union_next [i] == a &&  ALLOCNO_TYPE (a) != INSN_ALLOCNO
	  && (ALLOCNO_DST_COPIES (a) == NULL
	      || ALLOCNO_SRC_COPIES (a) == NULL))
	{
	  ALLOCNO_CAN (a) = NULL;
	  /* It is an allocno died after abnormal edge allocno
	     coalescing.  */
	  continue;
	}
      if (ALLOCNO_TYPE (a) == INSN_ALLOCNO
	  && ALLOCNO_REGNO (a) < 0
	  && union_next [i] == a)
	{
	  ALLOCNO_CAN (a) = NULL;
	  continue;
	}
      first = union_first [i];
      can = create_can ();
      mode = VOIDmode;
      can_allocnos_num = 0;
      for (a = union_next [ALLOCNO_NUM (first)];;
	   a = union_next [ALLOCNO_NUM (a)])
	{
	  can_allocnos_num++;
	  if (ALLOCNO_TYPE (a) != INSN_ALLOCNO)
	    {
	      if (mode == VOIDmode)
		mode = ALLOCNO_MODE (a);
	      yara_assert (mode == ALLOCNO_MODE (a));
	    }
	  if (a == first)
	    break;
	}
      CAN_MODE (can) = (mode == VOIDmode ? ALLOCNO_MODE (a) : mode);
      can_allocnos = CAN_ALLOCNOS (can)
	= yara_allocate ((can_allocnos_num + 1) * sizeof (allocno_t));
      can_allocnos_num = 0;
      for (a = union_next [ALLOCNO_NUM (first)];; a = next)
	{
	  if (! CAN_GLOBAL_P (can)
	      && ALLOCNO_TYPE (a) == REGION_ALLOCNO
	      && (REGION_ALLOCNO_START_INSN (a) == NULL
		  || REGION_ALLOCNO_STOP_INSN (a) == NULL
		  || BLOCK_NUM (REGION_ALLOCNO_START_INSN (a))
		     != BLOCK_NUM (REGION_ALLOCNO_STOP_INSN (a))))
	    CAN_GLOBAL_P (can) = true;
	  ALLOCNO_CAN (a) = can;
	  can_allocnos [can_allocnos_num++] = a;
	  next = union_next [ALLOCNO_NUM (a)];
	  /* Mark it as processed.  */
	  union_next [ALLOCNO_NUM (a)] = NULL;
	  if (a == first)
	    break;
	}
      can_allocnos [can_allocnos_num] = NULL;
      CAN_FREQ (can) = can_freq (can);
      setup_can_call_info (can);
    }
  yara_free (union_next);
  yara_free (union_first);
}

void
print_can (FILE *f, can_t can)
{
  static const char *const reg_class_names [] = REG_CLASS_NAMES;
  int i;
  allocno_t a, *can_allocnos;

  fprintf
    (f, "%scan#%d (%s %s(%d,%d) freq %d (call %d) r%d m%d %s%sconfl %d) allocnos:\n  ",
     (CAN_GLOBAL_P (can) ? "g" : ""), CAN_NUM (can),
     GET_MODE_NAME (CAN_MODE (can)), reg_class_names [CAN_COVER_CLASS (can)],
     CAN_COVER_CLASS_COST (can), CAN_MEMORY_COST (can),
     CAN_FREQ (can), CAN_CALL_FREQ (can), CAN_HARD_REGNO (can),
     CAN_SLOTNO (can),
     (CAN_CALL_P (can) ? "call " : ""), (CAN_IN_GRAPH_P (can) ? "in " : ""),
     CAN_LEFT_CONFLICTS_NUM (can));
  can_allocnos = CAN_ALLOCNOS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    {
      if (i != 0)
	fprintf (f, ", ");
      if (ALLOCNO_TYPE (a) == INSN_ALLOCNO)
	{
	  fprintf (f, "%d(i%d)",
		   ALLOCNO_NUM (a), INSN_UID (INSN_ALLOCNO_INSN (a)));
	}
      else
	{
	  fprintf (f, "%d", ALLOCNO_NUM (a));
	  if (REGION_ALLOCNO_NODE (a)->bb == NULL)
	    fprintf (f, "(l%d, h%d",
		     REGION_ALLOCNO_NODE (a)->loop->num,
		     REGION_ALLOCNO_NODE (a)->loop->header->index);
	  else
	    fprintf (f, "(bb%d", REGION_ALLOCNO_NODE (a)->bb->index);
	  if (REGION_ALLOCNO_START_INSN (a) != NULL_RTX)
	    fprintf (f, ", si%d", INSN_UID (REGION_ALLOCNO_START_INSN (a)));
	  if (REGION_ALLOCNO_STOP_INSN (a) != NULL_RTX)
	    fprintf (f, ", ei%d", INSN_UID (REGION_ALLOCNO_STOP_INSN (a)));
	  fprintf (f, ")");
	}
    }
  fprintf (f, "\n");
}

void
debug_can (can_t can)
{
  print_can (stderr, can);
}

void
print_cans (FILE *f)
{
  int i;

  for (i = 0; i < cans_num; i++)
    print_can (f, cans [i]);
}

void
debug_cans (void)
{
  print_cans (stderr);
}



/* Array of cans conflicting with given can and the array size.  */
can_t *conflict_cans;
int conflict_cans_num;

/* Hard registers conflicting with given can.  */
HARD_REG_SET conflict_hard_regs;

/* If value of the array element corresponding to a can is equal to
   CAN_CONFLICT_TICK, the can is already in the array of conflicting
   cans.  The two variables are used to form the array of the
   conflicting cans.  */ 
static int can_conflict_tick;
static int *can_conflict_check_vector;

/* Function forming the array of cans conflicting with allocno A with
   nullifying the array if REWRITE_P.  False value of rewrite_p can be
   used to add conflicting cans of A to the array.  */
void
find_allocno_conflicting_cans (allocno_t a, bool rewrite_p)
{
  int i;
  allocno_t conflict_a;
  can_t can, conflict_can;
  allocno_t *vec;

  if (rewrite_p)
    {
      conflict_cans_num = 0;
      CLEAR_HARD_REG_SET (conflict_hard_regs);
      can_conflict_tick++;
    }
  can = ALLOCNO_CAN (a);
  yara_assert (can != NULL);
  IOR_HARD_REG_SET (conflict_hard_regs, ALLOCNO_HARD_REG_CONFLICTS (a));
  vec = ALLOCNO_CONFLICT_VEC (a);
  for (i = 0; (conflict_a = vec [i]) != NULL; i++)
    {
      conflict_can = ALLOCNO_CAN (conflict_a);
      if (conflict_can != NULL
	  /* ??? Allocnos from the same CAN may conflict with each other.
	     E.g. this is the case when region allocno being copied
	     into an insn allocno lives after the insn.  We don't
	     create such conflicts, the correct assignment is
	     guaranteed on the allocno level.  */
	  && conflict_can != can
	  && (can_conflict_check_vector [CAN_NUM (conflict_can)]
	      != can_conflict_tick))
	{
	  can_conflict_check_vector [CAN_NUM (conflict_can)]
	    = can_conflict_tick;
	  conflict_cans [conflict_cans_num++] = conflict_can;
	}
    }
}

/* Function forming the array of cans conflicting with CAN.  */
static void
find_conflicting_cans (can_t can)
{
  allocno_t a, *can_allocnos;
  int i;

  conflict_cans_num = 0;
  CLEAR_HARD_REG_SET (conflict_hard_regs);
  can_conflict_tick++;
  can_allocnos = CAN_ALLOCNOS (can);
  for (i = 0; (a = can_allocnos [i]) != NULL; i++)
    find_allocno_conflicting_cans (a, false);
}

void
create_can_conflicts (can_t can)
{
  find_conflicting_cans (can);
  COPY_HARD_REG_SET (CAN_CONFLICT_HARD_REGS (can), conflict_hard_regs);
  AND_COMPL_HARD_REG_SET (CAN_CONFLICT_HARD_REGS (can), no_alloc_regs);
  CAN_CONFLICT_CAN_VEC (can)
    = yara_allocate (sizeof (can_t) * (conflict_cans_num + 1));
  memcpy (CAN_CONFLICT_CAN_VEC (can), conflict_cans,
	  sizeof (can_t) * conflict_cans_num);
  CAN_CONFLICT_CAN_VEC (can) [conflict_cans_num] = NULL;
}

static void
create_all_can_conflicts (void)
{
  int i;

  for (i = 0; i < cans_num; i++)
    create_can_conflicts (cans [i]);
}



static int max_slotno;

static void
print_slotno_conflict (FILE *f, int slotno)
{
  int i, another_slotno;

  fprintf (f, ";; %d conflicts:", slotno);
  if (slotno_conflicts [slotno] != NULL)
    for (i = 0; (another_slotno = slotno_conflicts [slotno] [i]) >= 0; i++)
      fprintf (f, " %d", another_slotno);
  fprintf (f, "\n");
}

void
debug_slotno_conflict (int slotno)
{
  print_slotno_conflict (stderr, slotno);
}

static void
print_all_slotno_conflicts (FILE *f)
{
  int i;

  for (i = 0; i < max_slotno; i++)
    print_slotno_conflict (f, i);
}

void
debug_all_slotno_conflicts (void)
{
  print_all_slotno_conflicts (stderr);
}

static void
initiate_slotno_conflicts (void)
{
  int i, j;
  can_t can, another_can;
  can_t *vec;

  max_slotno = cans_num;
  slotno_conflicts = yara_allocate (sizeof (bitmap) * max_slotno);
  memset (slotno_conflicts, 0, sizeof (bitmap) * max_slotno);
  for (i = 0; i < cans_num; i++)
    {
      can = cans [i];
      vec = CAN_CONFLICT_CAN_VEC (can);
      for (j = 0; vec [j] != NULL; j++)
	;
      slotno_conflicts [i] = yara_allocate (sizeof (int) * (j + 1));
      for (j = 0; (another_can = vec [j]) != NULL; j++)
	slotno_conflicts [i] [j] = CAN_NUM (another_can);
      slotno_conflicts [i] [j] = -1;
    }
}

static void
finish_slotno_conflicts (void)
{
  int i;

  for (i = 0; i < max_slotno; i++)
    {
      if (slotno_conflicts [i] != NULL)
	yara_free (slotno_conflicts [i]);
    }
  yara_free (slotno_conflicts);
}



static void
setup_slotno_max_ref_align_size (void)
{
  int i, j, num, regno;
  can_t can;
  allocno_t a, *can_allocnos;

  memset (slotno_max_ref_align, 0, cans_num * sizeof (int));
  memset (slotno_max_ref_size, 0, cans_num * sizeof (int));
  for (i = 0; i < cans_num; i++)
    {
      can = cans [i];
      num = CAN_SLOTNO (can);
      can_allocnos = CAN_ALLOCNOS (can);
      for (j = 0; (a = can_allocnos [j]) != NULL; j++)
	{
	  regno = ALLOCNO_REGNO (a);
	  if (regno >= 0)
	    {
	      if (slotno_max_ref_align [num] < reg_max_ref_align [regno])
		slotno_max_ref_align [num] = reg_max_ref_align [regno];
	      if (slotno_max_ref_size [num] < reg_max_ref_size [regno])
		slotno_max_ref_size [num] = reg_max_ref_size [regno];
	    }
	}
    }
}



/* The function is called once for each compilation file.  It does
   initialization of data common for all RTL functions.  */
void
yara_ir_init_once (void)
{
  setup_mode_multi_reg_p ();
  setup_reg_class_nregs ();
  setup_spill_class_mode ();
}

/* The function builds IR of the allocator (alocnos, copies, CANs) and
   initializes local file data used for each RTL function.  */
void
yara_ir_init (void)
{
  int i;

  global_allocation_cost = 0;
  init_elim_table ();
  flow_loops_find (&yara_loops);
  create_loop_tree_nodes ();
  form_loop_tree ();
  set_loop_regno_refs (yara_loop_tree_root);
  make_edge_enumeration ();
  initiate_equivs ();
  initiate_allocnos ();
  initiate_copies ();
  initiate_conflicts ();
  create_insn_maps ();
  live_pseudo_reg_indexes = yara_allocate (max_regno * sizeof (int));
  live_pseudo_regs = yara_allocate (max_regno * sizeof (int));
  live_pseudo_regs_num = 0;
  for (i = 0; i < max_regno; i++)
    live_pseudo_reg_indexes [i] = -1;
  create_regno_allocno_maps ();
  allocate_loop_regno_allocno_map (yara_loop_tree_root);
  VARRAY_GENERIC_PTR_NOGC_INIT
    (live_through_allocnos_varray, max_regno * 2 * n_basic_blocks,
     "allocno living through basic blocks and egdes");
  create_all_allocnos ();
  allocnos = (allocno_t *) &VARRAY_GENERIC_PTR (allocno_varray, 0);
  allocnos_num = VARRAY_ACTIVE_SIZE (allocno_varray);
  all_allocnos_num += allocnos_num;
  all_insn_allocnos_num += insn_allocnos_num;
  all_region_allocnos_num += region_allocnos_num;
  free_loop_regno_allocno_map (yara_loop_tree_root);
  sort_copies_on_edges ();
  add_region_allocno_copy_conflicts ();
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (live_through_allocnos_varray); i++)
    yara_free (VARRAY_GENERIC_PTR (live_through_allocnos_varray, i));
  VARRAY_FREE (live_through_allocnos_varray);
  delete_regno_allocno_maps ();
  copies = (copy_t *) &VARRAY_GENERIC_PTR (copy_varray, 0);
  copies_num = VARRAY_ACTIVE_SIZE (copy_varray);
  commit_conflicts ();
  commit_copy_conflicts ();
#ifdef ENABLE_YARA_CHECKING
  check_abnormal_edges ();
#endif
  all_copies_num += copies_num;
  if ((YARA_PARAMS & YARA_NO_GVN) == 0)
    make_vn ();
  setup_all_allocno_frequencies ();
  if (yara_dump_file != NULL)
    print_allocnos (yara_dump_file);
  initiate_cans ();
  conflict_cans = yara_allocate (sizeof (can_t) * allocnos_num);
  can_conflict_tick = 0;
  can_conflict_check_vector = yara_allocate (sizeof (int) * allocnos_num);
  memset (can_conflict_check_vector, 0, sizeof (int) * allocnos_num);
  can_allocno_copy_bitmap = yara_allocate_bitmap ();
  create_cans ();
  create_all_can_conflicts ();
  initiate_slotno_conflicts ();
  slotno_max_ref_align = yara_allocate (cans_num * sizeof (int));
  slotno_max_ref_size = yara_allocate (cans_num * sizeof (int));
  setup_slotno_max_ref_align_size ();
  if (yara_dump_file != NULL)
    print_all_slotno_conflicts (yara_dump_file);
}


/* The function frees the allocator IR and local file data used for
   each RTL function.  */
void
yara_ir_finish (void)
{
  yara_free (slotno_max_ref_align);
  yara_free (slotno_max_ref_size);
  finish_slotno_conflicts ();
  yara_free_bitmap (can_allocno_copy_bitmap);
  yara_free (can_conflict_check_vector);
  yara_free (conflict_cans);
  finish_cans ();
  yara_free (live_pseudo_regs);
  yara_free (live_pseudo_reg_indexes);
  finish_insn_maps ();
  finish_conflicts ();
  finish_copies ();
  finish_allocnos ();
  finish_equivs ();
  finish_edge_enumeration ();
  finish_loop_tree_nodes ();
  flow_loops_free (&yara_loops);
  free_dominance_info (CDI_DOMINATORS);
  verify_elim_offsets ();
  all_conflicts_num += conflicts_num;
  all_copy_conflicts_num += copy_conflicts_num;
  if (yara_dump_file != NULL)
    {
      fprintf (yara_dump_file,
	       "Allocnos insn=%d(%d),region=%d(%d)\n",
	       insn_allocnos_num, all_insn_allocnos_num,
	       region_allocnos_num, all_region_allocnos_num);
      fprintf (yara_dump_file,
	       "Copies=%d(%d), Conflicts=%d(%d), Copy conflicts=%d(%d)\n",
	       copies_num, all_copies_num, conflicts_num, all_conflicts_num,
	       copy_conflicts_num, all_copy_conflicts_num);
    }
}


#include "gt-yara-ir.h"
