/* Functions for tracking values of register parameters.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.

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
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "flags.h"
#include "output.h"
#include "dwarf2out.h"
#include "insn-config.h"
#include "reload.h"
#include "sbitmap.h"
#include "alloc-pool.h"
#include "fibheap.h"
#include "hashtab.h"
#include "varray.h"

/* Type of operation.  */
enum operation_type
{
  OP_REGMEM_TO_REGMEM,	/* Store REG/MEM to REG/MEM.  */
  OP_UNKNOWN_TO_REGMEM,	/* Store unknown value to REG/MEM.  */
  OP_CALL,		/* Clobber registers not preserved by
			   a function call.  */
  OP_ADJUST		/* Adjust stack pointer. */
};

/* Structure holding information about operation.  */
typedef struct operation_def
{
  /* Type of operation.  */
  enum operation_type type;

  union {
    /* Operands of the operation.  */
    struct {
      rtx dst;
      rtx src;
    } s;

    /* Stack adjustment.  */
    HOST_WIDE_INT adjust;
  } u;

  /* The instruction which the operation is in.  */
  rtx insn;
} operation;

/* Structure describing the location of register parameter.  */
typedef struct location_def
{
  /* The location itself.  */
  rtx loc;

  /* The register number of the register parameter this location is associated
     with.  */
  unsigned int regno;

  /* Double-linked list.  */
  struct location_def *next, *prev;
} *location;

/* The description of locations for each hard register.  */
typedef struct locations_def
{
  /* Array of doubly linked lists with pointers to heads and tails.  */
  location head[FIRST_PSEUDO_REGISTER];
  location tail[FIRST_PSEUDO_REGISTER];

  /* Locations of the regparams.  */
  htab_t htab;

  /* Reference count.  */
  int refcount;
} *locations;

/* Structure describing location of Canonical Frame Address.  */
typedef struct cfa_location_def
{
  unsigned int regno;
  HOST_WIDE_INT offset;
} cfa_location;

/* Structure holding the IN or OUT set for a basic block.  */
typedef struct dataflow_set_def
{
  /* Adjustment of stack offset.  */
  HOST_WIDE_INT stack_adjust;

  /* Locations of hard registers.  */
  locations locs;

  /* Location of Canonical Frame Address.  */
  cfa_location cfa;
} dataflow_set;

/* The structure (one for each basic block) containing the information
   needed for register parameter tracking.  */
typedef struct register_parameter_info_def
{
  /* Number of operations stored in the MOS array.  */
  int n_opers;

  /* The array of operations.  */
  operation *opers;

  /* The IN and OUT set for dataflow analysis.  */
  dataflow_set in;
  dataflow_set out;

  /* Has the block been already visited?  */
  bool visited;
} *register_parameter_info;

/* Pointer to the BB's information specific to variable tracking pass.  */
#define RPI(BB) ((register_parameter_info) (BB)->aux)

/* Alloc pool for struct location_def.  */
static alloc_pool location_pool;

/* Notes scheduled to be emitted.  */
static varray_type scheduled_notes;

/* Which registers should be tracked?  */
static bool track_reg_p[FIRST_PSEUDO_REGISTER];

/* RTX for REGNO.  */
static rtx reg_rtx[FIRST_PSEUDO_REGISTER];

/* Shall notes be emitted before insn?  */
static bool emit_notes_before_insn = false;

/* Special values of stack adjust.  */
#define INVALID_STACK_ADJUST						\
  (((unsigned HOST_WIDE_INT) 1 << (HOST_BITS_PER_WIDE_INT - 1)) - 1)
#define UNKNOWN_STACK_ADJUST						\
  (((unsigned HOST_WIDE_INT) 1 << (HOST_BITS_PER_WIDE_INT - 1)) - 1)

/* Local function prototypes.  */
static void stack_adjust_offset_pre_post	PARAMS ((rtx, HOST_WIDE_INT *,
							 HOST_WIDE_INT *));
static void insn_stack_adjust_offset_pre_post	PARAMS ((rtx, HOST_WIDE_INT *,
							 HOST_WIDE_INT *));
static int contains_reg			PARAMS ((rtx, unsigned int,
						 unsigned int));
static rtx adjust_stack_reference	PARAMS ((rtx, HOST_WIDE_INT,
						 cfa_location *));
static hashval_t regparam_rtx_hash	PARAMS ((const rtx));
static hashval_t location_htab_hash	PARAMS ((const void *));
static int location_htab_eq		PARAMS ((const void *, const void *));

static location location_lookup		PARAMS ((dataflow_set *, rtx));
static bool location_insert		PARAMS ((dataflow_set *, int, rtx));
static bool location_delete		PARAMS ((dataflow_set *, rtx));

static locations init_locations		PARAMS ((dataflow_set *));
static locations unshare_locations	PARAMS ((dataflow_set *));
static void clear_locations		PARAMS ((dataflow_set *));
static void free_locations		PARAMS ((dataflow_set *));

static void dataflow_set_init		PARAMS ((dataflow_set *));
static void dataflow_set_copy		PARAMS ((dataflow_set *,
						 dataflow_set *));
static void dataflow_set_confluence	PARAMS ((dataflow_set *,
						 dataflow_set *));
static bool dataflow_set_different	PARAMS ((dataflow_set *,
						 dataflow_set *));
static void dataflow_set_destroy	PARAMS ((dataflow_set *));

static bool track_mem_p			PARAMS ((rtx, rtx *));
static void count_operations		PARAMS ((rtx, rtx, void *));
static void add_operations		PARAMS ((rtx, rtx, void *));
static bool find_locations_in_bb	PARAMS ((basic_block));
static void find_locations		PARAMS ((void));

static void dump_operations		PARAMS ((basic_block));
static void dump_locs			PARAMS ((location));
static void dump_dataflow_set		PARAMS ((dataflow_set *));
static void dump_dataflow_sets		PARAMS ((void));

static void schedule_regno_note_to_emit	PARAMS ((dataflow_set *, int));
static void schedule_note_to_emit	PARAMS ((int, rtx));
static void emit_notes_for_differences	PARAMS ((rtx, dataflow_set *,
						 dataflow_set *));
static void emit_notes_for_bb		PARAMS ((basic_block));
static void emit_scheduled_notes	PARAMS ((rtx));
static void regparam_emit_notes		PARAMS ((void));

static void regparam_initialize		PARAMS ((void));
static void regparam_finalize		PARAMS ((void));

/* Given a SET, calculate the amount of stack adjustment it contains
   PRE- and POST-modifying stack pointer.
   This function is similar to stack_adjust_offset.  */

static void
stack_adjust_offset_pre_post (pattern, pre, post)
     rtx pattern;
     HOST_WIDE_INT *pre;
     HOST_WIDE_INT *post;
{
  rtx src = SET_SRC (pattern);
  rtx dest = SET_DEST (pattern);
  enum rtx_code code;

  if (dest == stack_pointer_rtx)
    {
      /* (set (reg sp) (plus (reg sp) (const_int))) */
      code = GET_CODE (src);
      if (! (code == PLUS || code == MINUS)
	  || XEXP (src, 0) != stack_pointer_rtx
	  || GET_CODE (XEXP (src, 1)) != CONST_INT)
	return;

      if (code == MINUS)
	*post += INTVAL (XEXP (src, 1));
      else
	*post -= INTVAL (XEXP (src, 1));
    }
  else if (GET_CODE (dest) == MEM)
    {
      /* (set (mem (pre_dec (reg sp))) (foo)) */
      src = XEXP (dest, 0);
      code = GET_CODE (src);

      switch (code)
	{
	case PRE_MODIFY:
	case POST_MODIFY:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      rtx val = XEXP (XEXP (src, 1), 1);
	      /* We handle only adjustments by constant amount.  */
	      if (GET_CODE (XEXP (src, 1)) != PLUS ||
		  GET_CODE (val) != CONST_INT)
		abort ();
	      if (code == PRE_MODIFY)
		*pre -= INTVAL (val);
	      else
		*post -= INTVAL (val);
	      break;
	    }
	  return;

	case PRE_DEC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *pre += GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case POST_DEC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *post += GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case PRE_INC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *pre -= GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case POST_INC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *post -= GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	default:
	  return;
	}
    }
}

/* Given an INSN, calculate the amount of stack adjustment it contains
   PRE- and POST-modifying stack pointer.  */

static void
insn_stack_adjust_offset_pre_post (insn, pre, post)
     rtx insn;
     HOST_WIDE_INT *pre;
     HOST_WIDE_INT *post;
{
  *pre = 0;
  *post = 0;

  if (GET_CODE (PATTERN (insn)) == SET)
    stack_adjust_offset_pre_post (PATTERN (insn), pre, post);
  else if (GET_CODE (PATTERN (insn)) == PARALLEL
	   || GET_CODE (PATTERN (insn)) == SEQUENCE)
    {
      int i;

      /* There may be stack adjustments inside compound insns.  Search
	 for them.  */
      for (i = XVECLEN (PATTERN (insn), 0) - 1; i >= 0; i--)
	if (GET_CODE (XVECEXP (PATTERN (insn), 0, i)) == SET)
	  stack_adjust_offset_pre_post (XVECEXP (PATTERN (insn), 0, i),
					pre, post);
    }
}

/* Return 1 if RTL X contains a register number REGNO1,
   return 2 if RTL X contains a register number REGNO2, return 0 otherwise.  */

static int
contains_reg (x, regno1, regno2)
     rtx x;
     unsigned int regno1;
     unsigned int regno2;
{
  const char *fmt;
  RTX_CODE code;
  int i;
  int which;

  if (!x)
    return 0;

  code = GET_CODE (x);
  if (code == REG)
    {
      if (REGNO (x) == regno1)
	return 1;
      if (REGNO (x) == regno2)
	return 2;
      return 0;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  which = contains_reg (XEXP (x, i), regno1, regno2);
	  if (which)
	    return which;
	}
      else if (fmt[i] == 'E')
	{
	  int j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    {
	      which = contains_reg (XVECEXP (x, i, j), regno1, regno2);
	      if (which)
		return which;
	    }
	}
    }

  return 0;
}

/* Adjust stack reference X.  If X contains STACK_POINTER_RTX adjust it by
   OFFSET bytes, if X contains CFA->REGNO adjust it by CFA->OFFSET.  */

static rtx
adjust_stack_reference (x, offset, cfa)
     rtx x;
     HOST_WIDE_INT offset;
     cfa_location *cfa;
{
  rtx new_x;
  rtx tmp;
  int which;

  if (GET_CODE (x) != MEM)
    return x;

  which = contains_reg (x, STACK_POINTER_REGNUM, cfa->regno);
  if (!which)
    return x;

  new_x = copy_rtx (x);
  switch (which)
    {
      case 1:
	XEXP (new_x, 0) = replace_rtx (XEXP (new_x, 0),
				       stack_pointer_rtx,
				       gen_rtx_PLUS (Pmode, stack_pointer_rtx,
						     GEN_INT (offset)));
	break;

      case 2:
	XEXP (new_x, 0) = replace_rtx (XEXP (new_x, 0),
				       reg_rtx[cfa->regno],
				       gen_rtx_PLUS (Pmode, reg_rtx[cfa->regno],
						     GEN_INT (-cfa->offset)));
	break;
    }

  tmp = simplify_rtx (XEXP (new_x, 0));
  if (tmp)
    XEXP (new_x, 0) = tmp;

  return new_x;
}

/* Hash function computing the hash value for location_htab from RTL X.  */

static hashval_t
regparam_rtx_hash (x)
     const rtx x;
{
  if (GET_CODE (x) == REG)
    return REGNO (x);

  if (GET_CODE (x) == MEM)
    {
      rtx inside = XEXP (x, 0);
      if (inside == stack_pointer_rtx
	  || inside == hard_frame_pointer_rtx)
	return 0x1000000;

      if (GET_CODE (inside) == PLUS
	  && (XEXP (inside, 0) == stack_pointer_rtx
	      || XEXP (inside, 0) == hard_frame_pointer_rtx)
	  && GET_CODE (XEXP (inside, 1)) == CONST_INT)
	return 0x1000000 + INTVAL (XEXP (inside, 1));
    }

  return (size_t) x;
}

/* The hash function for location_htab, computes the hash value
   from the declaration of variable X.  */

static hashval_t
location_htab_hash (x)
     const void *x;
{
  const location loc = (const location) x;

  return regparam_rtx_hash (loc->loc);
}

/* Compare the declaration of variable X with declaration Y.  */

static int
location_htab_eq (x, y)
     const void *x;
     const void *y;
{
  const location loc1 = (const location) x;
  const rtx loc2 = (const rtx) y;

  if (GET_CODE (loc1->loc) != GET_CODE (loc2))
    return 0;

  if (GET_CODE (loc2) == REG)
    return REGNO (loc1->loc) == REGNO (loc2);

  return rtx_equal_p (loc1->loc, loc2);
}

/* Lookup location X in the dataflow set SET.  */

static location
location_lookup (set, x)
     dataflow_set *set;
     rtx x;
{
  location loc;

  if (!set->locs)
    return NULL;

  loc = htab_find_with_hash (set->locs->htab, x, regparam_rtx_hash (x));

  return loc;
}

/* Insert location X to the list of locations for register number REGNO
   of the dataflow set SET.
   Return true if the preferred location has changed.  */

static bool
location_insert (set, regno, x)
     dataflow_set *set;
     int regno;
     rtx x;
{
  void **slot;
  locations locs;
  location loc;

  if (!set->locs)
    init_locations (set);
  else if (set->locs->refcount > 1)
    unshare_locations (set);
  locs = set->locs;

  slot = htab_find_slot_with_hash (locs->htab, x, regparam_rtx_hash (x),
				   INSERT);
  if (*slot)
    return false;

  loc = pool_alloc (location_pool);
  loc->loc = x;
  loc->regno = regno;
  *slot = loc;

  if (GET_CODE (loc->loc) == REG)
    {
      /* Add the register to the tail of the chain.  */
      if (locs->tail[regno])
	locs->tail[regno]->next = loc;
      loc->prev = locs->tail[regno];
      loc->next = NULL;
      locs->tail[regno] = loc;
      if (!locs->head[regno])
	{
	  locs->head[regno] = loc;
	  return true;
	}
    }
  else
    {
      /* Add the memory to the head of the chain.  */
      if (locs->head[regno])
	locs->head[regno]->prev = loc;
      loc->next = locs->head[regno];
      loc->prev = NULL;
      locs->head[regno] = loc;
      if (!locs->tail[regno])
	locs->tail[regno] = loc;
      return true;
    }

  return false;
}

/* Delete location X from the list of locations for register number REGNO
   of the dataflow set SET.
   Return true if the preferred location has changed.  */

static bool
location_delete (set, x)
     dataflow_set *set;
     rtx x;
{
  void **slot;
  locations locs;
  location loc;
  int regno;
  bool r;

  if (!set->locs)
    return false;

  locs = set->locs;
  slot = htab_find_slot_with_hash (locs->htab, x, regparam_rtx_hash (x),
				   NO_INSERT);
  if (!slot)
    return false;

  if (locs->refcount > 1)
    {
      locs = unshare_locations (set);
      slot = htab_find_slot_with_hash (locs->htab, x, regparam_rtx_hash (x),
				       NO_INSERT);
#ifdef ENABLE_CHECKING
      if (!slot)
	abort ();
#endif
    }

  r = false;
  loc = *slot;
  regno = loc->regno;
  if (!loc->prev)
    {
      locs->head[regno] = loc->next;
      if (!loc->next)
	locs->tail[regno] = NULL;
      else
	loc->next->prev = NULL;
      r = true;
    }
  else if (!loc->next)
    {
      locs->tail[regno] = loc->prev;
      if (!loc->prev)
	{
	  locs->head[regno] = NULL;
	  r = true;
	}
      else
	loc->prev->next = NULL;
    }
  else
    {
      loc->next->prev = loc->prev;
      loc->prev->next = loc->next;
    }
  htab_clear_slot (locs->htab, slot);
  pool_free (location_pool, loc);
  return r;
}

/* Initialize locations in dataflow set SET.  */

static locations
init_locations (set)
     dataflow_set *set;
{
  locations locs;
  int i;

  locs = xmalloc (sizeof (*locs));
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      locs->head[i] = NULL;
      locs->tail[i] = NULL;
    }
  locs->htab = htab_create (5, location_htab_hash, location_htab_eq, NULL);
  locs->refcount = 1;

  set->locs = locs;
  return locs;
}

/* Unshare locations in dataflow set SET.  */

static locations
unshare_locations (set)
     dataflow_set *set;
{
  locations locs, old_locs;
  int i;

  locs = xmalloc (sizeof (*locs));
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      locs->head[i] = NULL;
      locs->tail[i] = NULL;
    }
  locs->htab = htab_create (5, location_htab_hash, location_htab_eq, NULL);
  locs->refcount = 1;

  old_locs = set->locs;
  set->locs->refcount--;
  set->locs = locs;

  /* Copy the elements.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (track_reg_p[i])
	{
	  location loc;

	  for (loc = old_locs->head[i]; loc; loc = loc->next)
	    location_insert (set, i, loc->loc);
	}
    }

  return locs;
}

/* Delete all locations in dataflow set SET.  */

static void
clear_locations (set)
     dataflow_set *set;
{
  int i;

  if (set->locs->refcount > 1)
    {
      set->locs->refcount--;
      set->locs = NULL;
      return;
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (track_reg_p[i])
	{
	  while (set->locs->head[i])
	    location_delete (set, set->locs->head[i]->loc);
	}
#ifdef ENABLE_CHECKING
      else if (set->locs->head[i])
	abort ();
#endif
    }
#ifdef ENABLE_CHECKING
  if (htab_elements (set->locs->htab))
    abort ();
#endif

  free_locations (set);
}

/* Free locations in dataflow set SET.  */

static void
free_locations (set)
     dataflow_set *set;
{
  htab_delete (set->locs->htab);
  free (set->locs);
  set->locs = NULL;
}

/* Initialize dataflow set SET to be empty.  */

static void
dataflow_set_init (set)
     dataflow_set *set;
{
  set->stack_adjust = UNKNOWN_STACK_ADJUST;
  set->locs = NULL;
  set->cfa.regno = 0;
  set->cfa.offset = 0;
}

/* Copy the contents of dataflow set SRC to DST.  */

static void
dataflow_set_copy (dst, src)
     dataflow_set *dst;
     dataflow_set *src;
{
  if (dst->locs)
    {
      if (dst->locs->refcount > 1)
	dst->locs->refcount--;
      else
	clear_locations (dst);
    }
  dst->locs = src->locs;
  if (dst->locs)
    dst->locs->refcount++;

  dst->stack_adjust = src->stack_adjust;
  dst->cfa.regno = src->cfa.regno;
  dst->cfa.offset = src->cfa.offset;
}

/* Compute union of dataflow sets SRC and DST and store it to DST.  */

static void
dataflow_set_confluence (dst, src)
     dataflow_set *dst;
     dataflow_set *src;
{
  int i;
  location loc, next;
  void **slot;

  if (dst->stack_adjust == UNKNOWN_STACK_ADJUST)
    dst->stack_adjust = src->stack_adjust;
  else if (src->stack_adjust != UNKNOWN_STACK_ADJUST
	   && src->stack_adjust != dst->stack_adjust)
    dst->stack_adjust = INVALID_STACK_ADJUST;

  if (!dst->cfa.regno)
    dst->cfa = src->cfa;
#ifdef ENABLE_CHECKING
  /* If this check fails
     the code of function being compiled is inconsistent.  */
  else if (src->cfa.regno
      && (src->cfa.regno != dst->cfa.regno
	  || src->cfa.offset != dst->cfa.offset))
      abort ();
#endif

  /* If DST has no locations we can't delete a location from it.  */
  if (!dst->locs)
    return;

  /* If SRC has no locations we can optimize deleting DST's locations.  */
  if (!src->locs)
    {
      if (dst->locs->refcount > 1)
	{
	  dst->locs->refcount--;
	  dst->locs = NULL;
	}
      else
	clear_locations (dst);
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (track_reg_p[i])
	{
	  for (loc = dst->locs->head[i]; loc; loc = next)
	    {
	      next = loc->next;
	      slot = htab_find_slot_with_hash (src->locs->htab, loc->loc,
					       regparam_rtx_hash (loc->loc),
					       NO_INSERT);
	      if (!slot)
		{
		  /* LOC is in DST but is not in SRC,
		     delete it from DST.  */
		  location_delete (dst, loc->loc);
		}
	    }
	}
#ifdef ENABLE_CHECKING
      else if (src->locs->head[i])
	abort ();
#endif
    }

  if (htab_elements (dst->locs->htab) == 0)
    free_locations (dst);
}

/* Return true if dataflow sets OLD_SET and NEW_SET differ.  */

static bool
dataflow_set_different (old_set, new_set)
     dataflow_set *old_set;
     dataflow_set *new_set;
{
  int i;

  if (old_set->locs == new_set->locs)
    return false;
  if (old_set->locs == NULL || new_set->locs == NULL)
    return true;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (track_reg_p[i])
      {
	location loc1, loc2;

	for (loc1 = old_set->locs->head[i], loc2 = new_set->locs->head[i];
	     loc1 && loc2;
	     loc1 = loc1->next, loc2 = loc2->next)
	  ;
	if (loc1 || loc2)
	  {
	    /* The lists do not have the same length.  */
	    return true;
	  }

	/* Check whether the same locations are in both lists.  */
	for (loc1 = old_set->locs->head[i]; loc1; loc1 = loc1->next)
	  {
	    for (loc2 = new_set->locs->head[i]; loc2; loc2 = loc2->next)
	      {
		if ((GET_CODE (loc1->loc) == REG
		     && GET_CODE (loc2->loc) == REG
		     && REGNO (loc1->loc) == REGNO (loc2->loc))
		    || rtx_equal_p (loc1->loc, loc2->loc))
		  break;
	      }

	    if (!loc2)
	      {
		/* LOC1 is not in list in OLD_SET.  */
		return true;
	      }
	  }
      }

  return false;
}

/* Free the contents of dataflow set SET.  */

static void
dataflow_set_destroy (set)
     dataflow_set *set;
{
  if (set->locs)
    clear_locations (set);
}

/* Shall mem X be tracked?  If so store possibly modified mem to OUTP. */

static bool
track_mem_p (x, outp)
     rtx x;
     rtx *outp;
{
  rtx inside;

#ifdef ENABLE_CHECKING
  if (GET_CODE (x) != MEM)
    abort ();
#endif

  *outp = x;
  inside = XEXP (x, 0);
  if (inside == stack_pointer_rtx
      || inside == hard_frame_pointer_rtx)
    return true;

  if ((GET_CODE (inside) == PRE_DEC
       || GET_CODE (inside) == PRE_INC
       || GET_CODE (inside) == POST_DEC
       || GET_CODE (inside) == POST_INC)
      && XEXP (inside, 0) == stack_pointer_rtx)
    {
      *outp = gen_rtx_MEM (GET_MODE (x), XEXP (inside, 0));
      return true;
    }

  if ((GET_CODE (inside) == PLUS || GET_CODE (inside) == MINUS)
      && (XEXP (inside, 0) == stack_pointer_rtx
	  || XEXP (inside, 0) == hard_frame_pointer_rtx)
      && GET_CODE (XEXP (inside, 1)) == CONST_INT)
    return true;

  return false;
}

/* Count stores (register and memory references) LOC which will be tracked.
   INSN is instruction which the LOC is part of.  */

static void
count_operations (loc, pattern, insn)
     rtx loc;
     rtx pattern ATTRIBUTE_UNUSED;
     void *insn;
{
  basic_block bb = BLOCK_FOR_INSN ((rtx) insn);

#ifdef ENABLE_CHECKING
  if (GET_CODE (pattern) != SET && GET_CODE (pattern) != CLOBBER)
    abort ();
#endif

  if (GET_CODE (loc) == REG)
    {
#ifdef ENABLE_CHECKING
	if (REGNO (loc) >= FIRST_PSEUDO_REGISTER)
	  abort ();
#endif
	RPI (bb)->n_opers++;
    }
  else if (GET_CODE (loc) == MEM
	   && RTX_FRAME_RELATED_P ((rtx) insn)
	   && track_mem_p (loc, &loc))
    {
	  RPI (bb)->n_opers++;
    }
}

/* Add stores (register and memory references) LOC which will be tracked
   to RPI (bb)->opers. PATTERN is the RTL expr containing the store.
   INSN is instruction which the LOC is part of.  */

static void
add_operations (loc, pattern, insn)
     rtx loc;
     rtx pattern;
     void *insn;
{
  if (GET_CODE (loc) == REG
      || (GET_CODE (loc) == MEM
	  && RTX_FRAME_RELATED_P ((rtx) insn)
	  && track_mem_p (loc, &loc)))
    {
      basic_block bb = BLOCK_FOR_INSN ((rtx) insn);

      if ((GET_CODE (loc) != MEM && loc != SET_DEST (pattern))
	  || GET_CODE (pattern) == CLOBBER
	  || (GET_CODE (SET_SRC (pattern)) != REG
	      && GET_CODE (SET_SRC (pattern)) != MEM))
	{
	  operation *oper = RPI (bb)->opers + RPI (bb)->n_opers++;
	  oper->type = OP_UNKNOWN_TO_REGMEM;
	  oper->u.s.dst = loc;
	  if (GET_CODE (pattern) == SET
	      /* Avoid computing stack pointer adjust twice.  */
	      && !(loc == stack_pointer_rtx
		   && contains_reg (SET_SRC (pattern), STACK_POINTER_REGNUM,
				    STACK_POINTER_REGNUM)))
	    oper->u.s.src = SET_SRC (pattern);
	  else
	    oper->u.s.src = NULL;
	  oper->insn = insn;
	}
      else
	{
	  operation *oper = RPI (bb)->opers + RPI (bb)->n_opers++;
	  oper->type = OP_REGMEM_TO_REGMEM;
	  oper->u.s.dst = loc;
	  oper->u.s.src = SET_SRC (pattern);
	  oper->insn = insn;
	}
    }
}

/* Compute the changes of variable locations in the basic block BB.  */

static bool
find_locations_in_bb (bb)
     basic_block bb;
{
  int i, n, r;
  bool changed;
  dataflow_set old_out;
  dataflow_set *in = &RPI (bb)->in;
  dataflow_set *out = &RPI (bb)->out;
  rtx src, dst;
  location src_loc, dst_loc;

  RPI (bb)->visited = true;
  dataflow_set_init (&old_out);
  dataflow_set_copy (&old_out, out);
  dataflow_set_copy (out, in);

  n = RPI (bb)->n_opers;
  for (i = 0; i < n; i++)
    {
      switch (RPI (bb)->opers[i].type)
	{
	  case OP_REGMEM_TO_REGMEM:
	    src = adjust_stack_reference (RPI (bb)->opers[i].u.s.src,
					  -out->stack_adjust, &out->cfa);
	    dst = adjust_stack_reference (RPI (bb)->opers[i].u.s.dst,
					  -out->stack_adjust, &out->cfa);
	    if (RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn)
		&& GET_CODE (src) == REG && REGNO (src) == out->cfa.regno
		&& (dst == stack_pointer_rtx || dst == hard_frame_pointer_rtx))
	      {
		if (GET_CODE (dst) != REG)
		  abort ();

		out->cfa.regno = REGNO (dst);
		if (dst == stack_pointer_rtx)
		  out->stack_adjust = out->cfa.offset;
		else if (src == stack_pointer_rtx)
		  {
		    if (out->stack_adjust == INVALID_STACK_ADJUST)
		      abort ();
		    out->cfa.offset = out->stack_adjust;
		  }
	      }

	    src_loc = location_lookup (out, src);
	    if (src_loc)
	      {
		dst_loc = location_lookup (out, dst);
		if (dst_loc)
		  {
		    if (src_loc->regno != dst_loc->regno)
		      {
			location_delete (out, dst);
			location_insert (out, src_loc->regno, dst);
		      }
		  }
		else
		  {
		    location_insert (out, src_loc->regno, dst);
		  }
		if (GET_CODE (src) == MEM
		    && RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn))
		  location_delete (out, src);
	      }
	    else
	      {
		location_delete (out, dst);
	      }
	    break;

	  case OP_UNKNOWN_TO_REGMEM:
	    dst = adjust_stack_reference (RPI (bb)->opers[i].u.s.dst,
					  -out->stack_adjust, &out->cfa);
	    src = RPI (bb)->opers[i].u.s.src;
	    if (RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn)
		&& GET_CODE (dst) == REG && src
		&& (dst == stack_pointer_rtx || dst == hard_frame_pointer_rtx)
		&& (GET_CODE (src) == PLUS || GET_CODE (src) == MINUS)
		&& GET_CODE (XEXP (src, 0)) == REG
		&& REGNO (XEXP (src, 0)) == out->cfa.regno
		&& GET_CODE (XEXP (src, 1)) == CONST_INT)
	      {
		HOST_WIDE_INT offset = INTVAL (XEXP (src, 1));

		if (GET_CODE (XEXP (src, 0)) == MINUS)
		  offset = -offset;

		out->cfa.regno = REGNO (dst);
		out->cfa.offset -= offset;
		if (dst == stack_pointer_rtx)
		  out->stack_adjust = out->cfa.offset;
	      }

	    dst_loc = location_lookup (out, dst);
	    if (dst_loc)
	      location_delete (out, dst);
	    break;

	  case OP_CALL:
	    for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
	      if (TEST_HARD_REG_BIT (call_used_reg_set, r))
		location_delete (out, reg_rtx[r]);
	    break;

	  case OP_ADJUST:
	    if (out->cfa.regno == STACK_POINTER_REGNUM)
	      {
		if (out->stack_adjust == INVALID_STACK_ADJUST)
		  abort ();
		out->cfa.offset += RPI (bb)->opers[i].u.adjust;
	      }
	    out->stack_adjust += RPI (bb)->opers[i].u.adjust;
	    break;
	}
    }

  if (out->locs && htab_elements (out->locs->htab) == 0)
    free_locations (out);

#ifdef ENABLE_CHECKING
    {
      edge e;

      for (e = bb->succ; e; e = e->succ_next)
	{
	  dataflow_set *set = &RPI (e->dest)->in;

	  if (set->cfa.regno
	      && (set->cfa.regno != out->cfa.regno
		  || set->cfa.offset != out->cfa.offset))
	    abort ();
	}
    }
#endif

  changed = dataflow_set_different (&old_out, out);
  dataflow_set_destroy (&old_out);
  return changed;
}

/* Find the locations of variables in the whole function.  */

static void
find_locations ()
{
  fibheap_t worklist, pending, fibheap_swap;
  sbitmap visited, in_worklist, in_pending, sbitmap_swap;
  basic_block bb;
  edge e;
  int *bb_order;
  int *rc_order;
  int i;

  /* Compute reverse completion order of depth first search of the CFG
     so that the data-flow runs faster.  */
  rc_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  bb_order = (int *) xmalloc (last_basic_block * sizeof (int));
  flow_depth_first_order_compute (NULL, rc_order);
  for (i = 0; i < n_basic_blocks; i++)
    bb_order[rc_order[i]] = i;
  free (rc_order);

  worklist = fibheap_new ();
  pending = fibheap_new ();
  visited = sbitmap_alloc (last_basic_block);
  in_worklist = sbitmap_alloc (last_basic_block);
  in_pending = sbitmap_alloc (last_basic_block);
  sbitmap_zero (in_worklist);
  sbitmap_zero (in_pending);

  FOR_EACH_BB (bb)
    {
      fibheap_insert (pending, bb_order[bb->index], bb);
      SET_BIT (in_pending, bb->index);
    }

  mark_dfs_back_edges ();
  while (!fibheap_empty (pending))
    {
      fibheap_swap = pending;
      pending = worklist;
      worklist = fibheap_swap;
      sbitmap_swap = in_pending;
      in_pending = in_worklist;
      in_worklist = sbitmap_swap;

      sbitmap_zero (visited);

      while (!fibheap_empty (worklist))
	{
	  bb = fibheap_extract_min (worklist);
	  RESET_BIT (in_worklist, bb->index);
	  if (!TEST_BIT (visited, bb->index))
	    {
	      bool changed;

	      SET_BIT (visited, bb->index);

	      /* Calculate the IN set as union of predecessor OUT sets.  */
	      if ((e = bb->pred) != NULL)
		{
		  bool valid = false;

		  while (e && e->flags & EDGE_DFS_BACK)
		    e = e->pred_next;
#ifdef ENABLE_CHECKING
		  if (!e)
		    abort ();
#endif

		  dataflow_set_copy (&RPI (bb)->in, &RPI (e->src)->out);
		  valid = RPI (e->src)->visited;
		  for (e = e->pred_next; e; e = e->pred_next)
		    if (!(e->flags & EDGE_DFS_BACK))
		      {
			if (!valid)
			  {
			    dataflow_set_copy (&RPI (bb)->in,
					       &RPI (e->src)->out);
			    valid = RPI (e->src)->visited;
			  }
			else if (RPI (e->src)->visited)
			  {
			    dataflow_set_confluence (&RPI (bb)->in,
						     &RPI (e->src)->out);
			    valid = true;
			  }
		      }
		}
#ifdef ENABLE_CHECKING
	      if (!RPI (bb)->in.cfa.regno)
		abort ();
#endif

	      changed = find_locations_in_bb (bb);
	      if (changed)
		{
		  for (e = bb->succ; e; e = e->succ_next)
		    {
		      if (e->dest == EXIT_BLOCK_PTR)
			continue;

		      if (e->dest == bb)
			continue;

		      if (TEST_BIT (visited, e->dest->index))
			{
			  if (!TEST_BIT (in_pending, e->dest->index))
			    {
			      /* Send E->DEST to next round.  */
			      SET_BIT (in_pending, e->dest->index);
			      fibheap_insert (pending, bb_order[e->dest->index],
					      e->dest);
			    }
			}
		      else if (!TEST_BIT (in_worklist, e->dest->index))
			{
			  /* Add E->DEST to current round.  */
			  SET_BIT (in_worklist, e->dest->index);
			  fibheap_insert (worklist, bb_order[e->dest->index],
					  e->dest);
			}
		    }
		}
	    }
	}
    }

  free (bb_order);
  fibheap_delete (worklist);
  fibheap_delete (pending);
  sbitmap_free (visited);
  sbitmap_free (in_worklist);
  sbitmap_free (in_pending);
}

/* Dump operations in basic blocks BB.  */

static void
dump_operations (bb)
     basic_block bb;
{
  int i;

  for (i = 0; i < RPI (bb)->n_opers; i++)
    {
      fprintf (rtl_dump_file, "%d ", INSN_UID (RPI (bb)->opers[i].insn));
      switch (RPI (bb)->opers[i].type)
	{
	  case OP_REGMEM_TO_REGMEM:
	    fprintf (rtl_dump_file, "REG/MEM <- REG/MEM\n");
	    fprintf (rtl_dump_file, "  ");
	    print_rtl_single (rtl_dump_file, RPI (bb)->opers[i].u.s.dst);
	    fprintf (rtl_dump_file, "  ");
	    print_rtl_single (rtl_dump_file, RPI (bb)->opers[i].u.s.src);
	    break;

	  case OP_UNKNOWN_TO_REGMEM:
	    fprintf (rtl_dump_file, "REG/MEM <- UNKNOWN\n");
	    fprintf (rtl_dump_file, "  ");
	    print_rtl_single (rtl_dump_file, RPI (bb)->opers[i].u.s.dst);
	    if (RPI (bb)->opers[i].u.s.src)
	      {
		fprintf (rtl_dump_file, "  ");
		print_rtl_single (rtl_dump_file, RPI (bb)->opers[i].u.s.src);
	      }
	    break;

	  case OP_CALL:
	    fprintf (rtl_dump_file, "CALL\n");
	    break;

	  case OP_ADJUST:
	    fprintf (rtl_dump_file, "ADJUST ");
	    fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC,
		     RPI (bb)->opers[i].u.adjust);
	    fprintf (rtl_dump_file, "\n");
	    break;
	}
    }
}

/* Print the information about variables from hash table VARS to dump file.  */
static void
dump_locs (loc)
     location loc;
{
  while (loc)
    {
      fprintf (rtl_dump_file, "      ");
      print_rtl_single (rtl_dump_file, loc->loc);
      loc = loc->next;
    }
}

/* Print the dataflow set SET to dump file.  */

static void
dump_dataflow_set (set)
     dataflow_set *set;
{
  int i;

  fprintf (rtl_dump_file, "Stack adjustment: ");
  fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, set->stack_adjust);
  fprintf (rtl_dump_file, "\n");
  fprintf (rtl_dump_file, "CFA: offset ");
  fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, set->cfa.offset);
  fprintf (rtl_dump_file, " register ");
  print_rtl_single (rtl_dump_file, reg_rtx[set->cfa.regno]);

  fprintf (rtl_dump_file, "Locations: %p\n", set->locs);
  if (set->locs)
    for (i = 1; i < FIRST_PSEUDO_REGISTER; i++)
      if (track_reg_p[i] && set->locs->head[i])
	{
	  fprintf (rtl_dump_file, "  Reg %d ", i);
	  print_rtl_single (rtl_dump_file, reg_rtx[i]);
	  dump_locs (set->locs->head[i]);
	}
  fprintf (rtl_dump_file, "\n");
}

/* Print the IN and OUT sets for each basic block to dump file.  */

static void
dump_dataflow_sets ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      fprintf (rtl_dump_file, "\nBasic block %d:\n", bb->index);
      fprintf (rtl_dump_file, "IN:\n");
      dump_dataflow_set (&RPI (bb)->in);
      fprintf (rtl_dump_file, "OUT:\n");
      dump_dataflow_set (&RPI (bb)->out);
    }
}

/* Create REG_LOCATION for register number REGNO in dataflow set SET
   and schedule it to emit.  */

static void
schedule_regno_note_to_emit (set, regno)
     dataflow_set *set;
     int regno;
{
  rtx reg_loc;

  if (set->locs && set->locs->head[regno])
    reg_loc = gen_rtx_REG_LOCATION (VOIDmode, regno,
				    set->locs->head[regno]->loc);
  else
    reg_loc = gen_rtx_REG_LOCATION (VOIDmode, regno, NULL_RTX);

  VARRAY_PUSH_RTX (scheduled_notes, reg_loc);
}

/* Create REG_LOCATION for register number REGNO in dataflow set SET
   with location LOC and schedule it to emit.  */

static void
schedule_note_to_emit (regno, loc)
     int regno;
     rtx loc;
{
  rtx reg_loc;

  reg_loc = gen_rtx_REG_LOCATION (VOIDmode, regno, loc);

  VARRAY_PUSH_RTX (scheduled_notes, reg_loc);
}

/* Emit notes before INSN for differences between dataflow sets OLD_SET and
   NEW_SET.  */

static void
emit_notes_for_differences (insn, old_set, new_set)
     rtx insn;
     dataflow_set *old_set;
     dataflow_set *new_set;
{
  int i;

  if (new_set->cfa.regno != old_set->cfa.regno
      || new_set->cfa.offset != old_set->cfa.offset)
    {
      schedule_note_to_emit (STACK_POINTER_REGNUM,
			     gen_rtx_PLUS (Pmode,
					   reg_rtx[new_set->cfa.regno],
					   GEN_INT (new_set->cfa.offset)));
    }

  if (old_set->locs != NULL)
    {
      if (new_set->locs != NULL)
	{
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    if (track_reg_p[i])
	      {
		if (old_set->locs->head[i] == NULL
		    && new_set->locs->head[i] == NULL)
		  continue;

		if (old_set->locs->head[i] != NULL
		    && new_set->locs->head[i] != NULL)
		  {
		    if (!location_htab_eq (old_set->locs->head[i],
					   new_set->locs->head[i]->loc))
		      schedule_regno_note_to_emit (new_set, i);
		  }
		else
		  {
		    schedule_regno_note_to_emit (new_set, i);
		  }
	      }
	}
      else
	{
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    if (track_reg_p[i])
	      if (old_set->locs->head[i] != NULL)
		schedule_regno_note_to_emit (new_set, i);
	}
    }
  else
    {
      if (new_set->locs != NULL)
	{
	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    if (track_reg_p[i])
	      if (new_set->locs->head[i] != NULL)
		schedule_regno_note_to_emit (new_set, i);
	}
    }

  emit_notes_before_insn = true;
  emit_scheduled_notes (insn);
  emit_notes_before_insn = false;
}

/* Emit the notes for changes of location parts in the basic block BB.  */

static void
emit_notes_for_bb (bb)
     basic_block bb;
{
  int i, n, r;
  dataflow_set set;
  rtx src, dst;
  location src_loc, dst_loc;

  dataflow_set_init (&set);
  dataflow_set_copy (&set, &RPI (bb)->in);

  n = RPI (bb)->n_opers;
  for (i = 0; i < n; i++)
    {
      rtx insn = RPI (bb)->opers[i].insn;

      switch (RPI (bb)->opers[i].type)
	{
	  case OP_REGMEM_TO_REGMEM:
	    src = adjust_stack_reference (RPI (bb)->opers[i].u.s.src,
					  -set.stack_adjust, &set.cfa);
	    dst = adjust_stack_reference (RPI (bb)->opers[i].u.s.dst,
					  -set.stack_adjust, &set.cfa);
	    if (RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn)
		&& GET_CODE (src) == REG && REGNO (src) == set.cfa.regno
		&& (dst == stack_pointer_rtx || dst == hard_frame_pointer_rtx))
	      {
		set.cfa.regno = REGNO (dst);
		if (dst == stack_pointer_rtx)
		  set.stack_adjust = set.cfa.offset;
		else if (src == stack_pointer_rtx)
		  set.cfa.offset = set.stack_adjust;
		schedule_note_to_emit (STACK_POINTER_REGNUM,
				       gen_rtx_PLUS (Pmode,
						     reg_rtx[set.cfa.regno],
						     GEN_INT (set.cfa.offset)));
	      }

	    src_loc = location_lookup (&set, src);
	    if (src_loc)
	      {
		dst_loc = location_lookup (&set, dst);
		if (dst_loc)
		  {
		    if (src_loc->regno != dst_loc->regno)
		      {
			int regno = dst_loc->regno;

			if (location_delete (&set, dst))
			  schedule_regno_note_to_emit (&set, regno);
			if (location_insert (&set, src_loc->regno, dst))
			  schedule_regno_note_to_emit (&set, src_loc->regno);
		      }
		  }
		else
		  {
		    if (location_insert (&set, src_loc->regno, dst))
		      schedule_regno_note_to_emit (&set, src_loc->regno);
		  }
		if (GET_CODE (src) == MEM
		    && RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn))
		  {
		    int regno = src_loc->regno;

		    if (location_delete (&set, src))
		      schedule_regno_note_to_emit (&set, regno);
		  }
	      }
	    else
	      {
		dst_loc = location_lookup (&set, dst);
		if (dst_loc)
		  {
		    int regno = dst_loc->regno;

		    if (location_delete (&set, dst))
		      schedule_regno_note_to_emit (&set, regno);
		  }
	      }
	    break;

	  case OP_UNKNOWN_TO_REGMEM:
	    dst = adjust_stack_reference (RPI (bb)->opers[i].u.s.dst,
					  -set.stack_adjust, &set.cfa);
	    src = RPI (bb)->opers[i].u.s.src;
	    if (RTX_FRAME_RELATED_P (RPI (bb)->opers[i].insn)
		&& GET_CODE (dst) == REG && src
		&& (dst == stack_pointer_rtx || dst == hard_frame_pointer_rtx)
		&& (GET_CODE (src) == PLUS || GET_CODE (src) == MINUS)
		&& GET_CODE (XEXP (src, 0)) == REG
		&& REGNO (XEXP (src, 0)) == set.cfa.regno
		&& GET_CODE (XEXP (src, 1)) == CONST_INT)
	      {
		HOST_WIDE_INT offset = INTVAL (XEXP (src, 1));

		if (GET_CODE (XEXP (src, 0)) == MINUS)
		  offset = -offset;

		set.cfa.regno = REGNO (dst);
		set.cfa.offset -= offset;
		if (dst == stack_pointer_rtx)
		  set.stack_adjust = set.cfa.offset;
		schedule_note_to_emit (STACK_POINTER_REGNUM,
				       gen_rtx_PLUS (Pmode,
						     reg_rtx[set.cfa.regno],
						     GEN_INT (set.cfa.offset)));
	      }

	    dst_loc = location_lookup (&set, dst);
	    if (dst_loc)
	      {
		int regno = dst_loc->regno;

		if (location_delete (&set, dst))
		  schedule_regno_note_to_emit (&set, regno);
	      }
	    break;

	  case OP_CALL:
	    for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
	      if (TEST_HARD_REG_BIT (call_used_reg_set, r))
		{
		  location loc;
		  int regno;

		  loc = location_lookup (&set, reg_rtx[r]);
		  if (loc)
		    {
		      regno = loc->regno;
		      if (location_delete (&set, reg_rtx[r]))
			schedule_regno_note_to_emit (&set, regno);
		    }
		}
	    break;

	  case OP_ADJUST:
	    if (set.cfa.regno == STACK_POINTER_REGNUM)
	      {
		set.cfa.offset += RPI (bb)->opers[i].u.adjust;
		schedule_note_to_emit (STACK_POINTER_REGNUM,
				       gen_rtx_PLUS (Pmode,
						     reg_rtx[set.cfa.regno],
						     GEN_INT (set.cfa.offset)));
	      }
	    set.stack_adjust += RPI (bb)->opers[i].u.adjust;
	    break;
	}

      if (i + 1 >= n || insn != RPI (bb)->opers[i + 1].insn)
	emit_scheduled_notes (insn);
    }
  dataflow_set_destroy (&set);
}

/* Emit scheduled notes after insn INSN.  */

static void
emit_scheduled_notes (insn)
     rtx insn;
{
  bool emitted[FIRST_PSEUDO_REGISTER];
  int i;
  rtx reg_loc;
  rtx note;

  memset (emitted, 0, sizeof (emitted));
  if (emit_notes_before_insn)
    {
      for (i = VARRAY_ACTIVE_SIZE (scheduled_notes) - 1; i >= 0; i--)
	{
	  reg_loc = VARRAY_RTX (scheduled_notes, i);
	  if (!emitted[REG_LOCATION_REGNO (reg_loc)])
	    {
	      note = emit_note_before (NOTE_INSN_REG_LOCATION, insn);
	      NOTE_REG_LOCATION (note) = reg_loc;
	      emitted[REG_LOCATION_REGNO (reg_loc)] = true;
	      insn = note;
	    }
	}
    }
  else
    {
      for (i = VARRAY_ACTIVE_SIZE (scheduled_notes) - 1; i >= 0; i--)
	{
	  reg_loc = VARRAY_RTX (scheduled_notes, i);
	  if (!emitted[REG_LOCATION_REGNO (reg_loc)])
	    {
	      note = emit_note_after (NOTE_INSN_REG_LOCATION, insn);
	      NOTE_REG_LOCATION (note) = reg_loc;
	      emitted[REG_LOCATION_REGNO (reg_loc)] = true;
	    }
	}
    }

  VARRAY_CLEAR (scheduled_notes);
}

/* Emit notes for the whole function.  */

static void
regparam_emit_notes ()
{
  basic_block bb;
  dataflow_set *last_out;

  last_out = &RPI (ENTRY_BLOCK_PTR)->out;

  FOR_EACH_BB (bb)
    {
      /* Emit the notes for changes of variable locations between two
	 subsequent basic blocks.  */
      emit_notes_for_differences (bb->head, last_out, &RPI (bb)->in);

      /* Emit the notes for the changes in the basic block itself.  */
      emit_notes_for_bb (bb);

      last_out = &RPI (bb)->out;
    }
}

/* Allocate and initialize the data structures for variable tracking
   and parse the RTL to get the micro operations.  */

static void
regparam_initialize ()
{
  basic_block bb;
  unsigned int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
#if defined (DWARF_FRAME_REGISTERS) && defined (DBX_REGISTER_NUMBER)
      track_reg_p[i] = (DBX_REGISTER_NUMBER (i) >= 0
			&& DBX_REGISTER_NUMBER (i) < DWARF_FRAME_REGISTERS
			&& (!TEST_HARD_REG_BIT (call_used_reg_set, i)
			    || FUNCTION_ARG_REGNO_P (i)));
#else
      track_reg_p[i] = (!TEST_HARD_REG_BIT (call_used_reg_set, i)
			|| FUNCTION_ARG_REGNO_P (i));
#endif
      reg_rtx[i] = gen_rtx_REG (Pmode, i);
    }

  alloc_aux_for_blocks (sizeof (struct register_parameter_info_def));

  FOR_EACH_BB (bb)
    {
      rtx insn;
      HOST_WIDE_INT pre, post;

      /* Count the number of micro operations.  */
      RPI (bb)->n_opers = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      insn_stack_adjust_offset_pre_post (insn, &pre, &post);
	      if (pre)
		RPI (bb)->n_opers++;
	      if (post)
		RPI (bb)->n_opers++;
	      note_stores (PATTERN (insn), count_operations, insn);
	      if (GET_CODE (insn) == CALL_INSN)
		RPI (bb)->n_opers++;
	    }
	}

      /* Add the nicro-operations to the array.  */
      RPI (bb)->opers = xmalloc (RPI (bb)->n_opers
				 * sizeof (struct operation_def));
      RPI (bb)->n_opers = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      insn_stack_adjust_offset_pre_post (insn, &pre, &post);
	      if (pre)
		{
		  operation *oper = RPI (bb)->opers + RPI (bb)->n_opers++;

		  oper->type = OP_ADJUST;
		  oper->u.adjust = pre;
		  oper->insn = insn;
		}

	      if (GET_CODE (insn) == CALL_INSN)
		{
		  operation *oper = RPI (bb)->opers + RPI (bb)->n_opers++;

		  oper->type = OP_CALL;
		  oper->insn = insn;
		}

	      note_stores (PATTERN (insn), add_operations, insn);

	      if (post)
		{
		  operation *oper = RPI (bb)->opers + RPI (bb)->n_opers++;

		  oper->type = OP_ADJUST;
		  oper->u.adjust = post;
		  oper->insn = insn;
		}
	    }
	}
    }

  /* Init the IN and OUT sets.  */
  FOR_ALL_BB (bb)
    {
      RPI (bb)->visited = false;
      dataflow_set_init (&RPI (bb)->in);
      dataflow_set_init (&RPI (bb)->out);
    }

  location_pool = create_alloc_pool ("location_pool",
				     sizeof (struct location_def), 1020);
  VARRAY_RTX_INIT (scheduled_notes, 10, "scheduled_notes");

  /* Initialize OUT set of ENTRY_BLOCK.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (track_reg_p[i])
      location_insert (&RPI (ENTRY_BLOCK_PTR)->out, i, reg_rtx[i]);

  RPI (ENTRY_BLOCK_PTR)->visited = true;
  RPI (ENTRY_BLOCK_PTR)->out.stack_adjust = 0;
  RPI (ENTRY_BLOCK_PTR)->out.cfa.regno = STACK_POINTER_REGNUM;
  RPI (ENTRY_BLOCK_PTR)->out.cfa.offset = 0;
}

/* Free the data structures needed for variable tracking.  */

static void
regparam_finalize ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      free (RPI (bb)->opers);
    }

  FOR_ALL_BB (bb)
    {
      dataflow_set_destroy (&RPI (bb)->in);
      dataflow_set_destroy (&RPI (bb)->out);
    }
  free_aux_for_blocks ();
  free_alloc_pool (location_pool);
}

/* The entry point to variable tracking pass.  */

void
regparam_main ()
{
  regparam_initialize ();

  if (rtl_dump_file)
    {
      basic_block bb;

      FOR_EACH_BB (bb)
	{
	  fprintf (rtl_dump_file, "\nBasic block %d:\n", bb->index);
	  dump_operations (bb);
	}
      fflush (rtl_dump_file);
    }

  find_locations ();
  regparam_emit_notes ();

  if (rtl_dump_file)
    {
      dump_dataflow_sets ();
      dump_flow_info (rtl_dump_file);
    }

  regparam_finalize ();
}
