/* Variable tracking routines for the GNU compiler.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.

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
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "basic-block.h"
#include "output.h"
#include "sbitmap.h"
#include "fibheap.h"
#include "hashtab.h"

/* The purpose that the location (REG or MEM) has in RTL.  */
enum location_type
{
  LT_PARAM,	/* Location is a parameter of instruction.  */
  LT_SET_DEST,	/* Location is the destination of the SET.  */
  LT_CLOBBERED	/* Location is the "destination" of CLOBBER.  */
};

/* Operation that is performed when scanning RTL.  */
enum scan_operation
{
  SO_COUNT,	/* Count the number of locations.  */
  SO_STORE,	/* Store the locatons to the array VTI (bb)->locs.  */
  SO_SKIP	/* Skip current node, and scan and store its internals.  */
};

/* Where shall the note be emited? BEFORE or AFTER the instruction.  */
enum where_emit_note
{
  EMIT_NOTE_BEFORE_INSN,
  EMIT_NOTE_AFTER_INSN
};

/* Structute for remembering the location (register or memory).  */
typedef struct location_def
{
  /* The location itself.  */
  rtx loc;

  /* The instruction that the location is in.  */
  rtx insn;

  /* Type of the location.  */
  enum location_type type;
} location;

/* Structure for passing some other parameters to functions count_locations and
   add_locations.  */
typedef struct scan_for_locations_data_def
{
  /* Instruction that the pattern is in.  */
  rtx insn;

  /* What type of the location are we scanning now?  */
  enum location_type type;

  /* What are we doing now?  */
  enum scan_operation oper;
} scan_for_locations_data;

/* Structure for passing some other parameters to function
   emit_note_if_var_changed.  */
typedef struct emit_note_data_def
{
  /* The instruction that the note will be emited before/after.  */
  rtx insn;

  /* Where the note will be emited (before/after)?  */
  enum where_emit_note where;
} emit_note_data;

/* The variables that are stored in a register are remembered in a link-list
   because there should not be many variables in one register so
   the link-list would be the fastest solution.  */
/* The variables stored in memory are remembered in a hash table, locations for
   each variable are remembered in a link-list.  */
/* TODO: Replace the link-lists by splay-trees?  */

/* The node of the link-list for describing the attributes of a variable.  */
typedef struct attrs_list_def
{
  /* Pointer to next member of the list.  */
  struct attrs_list_def *next;

  /* The rtx of register/memory.  */
  rtx loc;

  /* The declaration corresponding to LOC.  */
  tree decl;

  /* Offset from start of DECL.  */
  HOST_WIDE_INT offset;
} *attrs_list;

/* The structure (one for each basic block) containing the information
   needed for dataflow analysis (variable tracking).  */
typedef struct var_tracking_info_def
{
  /* Number of locations in the LOCS array.  */
  int n_locs;

  /* The array of locations.  */
  location *locs;

  /* Input attributes for registers (lists of attrs).  */
  attrs_list in[FIRST_PSEUDO_REGISTER];

  /* Input attributes for memory.  */
  htab_t mem_in;

  /* Output attributes for registers.  */
  attrs_list out[FIRST_PSEUDO_REGISTER];

  /* Output attributes for memory.  */
  htab_t mem_out;
} *var_tracking_info;

/* Structure for description of one part of variable location.  */
typedef struct location_part_def
{
  /* The location (REG or MEM).  */
  rtx loc;

  /* The offset in the variable.  */
  HOST_WIDE_INT offset;

  /* Shall this struct be deleted?  */
  bool delete_p;
} location_part;

/* Maximum number of location parts.  */
#define MAX_LOC_PARTS 16

/* Structure for description where the variable is located.  */
typedef struct variable_def
{
  /* The declaration of the variable.  */
  tree decl;

  /* Number of locations.  */
  int n_location_parts;

  /* The locations.  */
  location_part location_part[MAX_LOC_PARTS];

  /* Changed?  */
  bool changed;
} *variable;

/* The hashtable of STRUCT VARIABLE_DEF.  */
static htab_t variable_htab;

/* Hash function for MEM_IN and MEM_OUT.  */
#define MEM_HASH_VAL(mem) ((size_t) DECL_RTL ((tree) MEM_EXPR (mem)))

/* Hash function for VARIABLE_HTAB.  */
#define VARIABLE_HASH_VAL(decl) ((size_t) decl)

/* Get the pointer to the BB's information specific to var-tracking pass.  */
#define VTI(BB) ((var_tracking_info) (BB)->aux)

/* Local function prototypes.  */
static hashval_t mem_htab_hash		PARAMS ((const void *));
static int mem_htab_eq			PARAMS ((const void *, const void *));
static hashval_t variable_htab_hash	PARAMS ((const void *));
static int variable_htab_eq		PARAMS ((const void *, const void *));

static void init_attrs_list_set		PARAMS ((attrs_list *));
static void attrs_list_clear		PARAMS ((attrs_list *));
static attrs_list attrs_list_member	PARAMS ((attrs_list, tree,
						 HOST_WIDE_INT));
static void attrs_list_insert		PARAMS ((attrs_list *, tree,
						 HOST_WIDE_INT, rtx));
static void attrs_list_delete		PARAMS ((attrs_list *, tree,
						 HOST_WIDE_INT));
static void attrs_list_copy		PARAMS ((attrs_list *, attrs_list));
static void attrs_list_union		PARAMS ((attrs_list *,attrs_list));
static bool attrs_list_different	PARAMS ((attrs_list, attrs_list));

static void attrs_htab_insert		PARAMS ((htab_t, rtx));
static void attrs_htab_delete		PARAMS ((htab_t, rtx));
static int attrs_htab_different_1	PARAMS ((void **, void *));
static bool attrs_htab_different	PARAMS ((htab_t, htab_t));
static int attrs_htab_copy_1		PARAMS ((void **, void *));
static void attrs_htab_copy		PARAMS ((htab_t, htab_t));
static int attrs_htab_union_1		PARAMS ((void **, void *));
static void attrs_htab_union		PARAMS ((htab_t, htab_t));
static void attrs_htab_clear		PARAMS ((htab_t));
static void attrs_htab_cleanup		PARAMS ((void *));

static bool track_expr_p		PARAMS ((tree));
static int scan_for_locations		PARAMS ((rtx *, void *));
static bool compute_bb_dataflow		PARAMS ((basic_block));
static void hybrid_search		PARAMS ((basic_block, sbitmap,
						 sbitmap));
static void iterative_dataflow		PARAMS ((int *));
static inline void dump_attrs_list	PARAMS ((attrs_list));
static int dump_mem_attrs_list		PARAMS ((void **, void *));
static void dump_attrs_list_sets	PARAMS ((void));

static void note_insn_var_location_emit	PARAMS ((rtx, enum where_emit_note,
						 variable));
static void set_location_part		PARAMS ((tree, HOST_WIDE_INT, rtx,
						 rtx, enum where_emit_note));
static void delete_location_part	PARAMS ((tree, HOST_WIDE_INT, rtx,
						 enum where_emit_note));
static int process_location_parts	PARAMS ((void **, void *));
static void emit_note_if_var_changed	PARAMS ((void **, void *));
static inline void process_bb_delete	PARAMS ((attrs_list *, htab_t, int,
						 rtx, rtx,
						 enum where_emit_note));
static void process_bb			PARAMS ((basic_block));
static int mark_variables_for_deletion	PARAMS ((void **, void *));
static int add_and_unmark_variables	PARAMS ((void **, void *));
static void var_tracking_emit_notes	PARAMS ((void));

static void var_tracking_initialize	PARAMS ((void));
static void var_tracking_finalize	PARAMS ((void));

/* Return the hash value for X.  */

static hashval_t
mem_htab_hash (x)
     const void *x;
{
  const attrs_list list = (const attrs_list) x;

  return (MEM_HASH_VAL (list->loc));
}

/* Shall X and Y go to the same hash slot?  */

static int
mem_htab_eq (x, y)
     const void *x;
     const void *y;
{
  const attrs_list list = (const attrs_list) x;
  const rtx mem = (const rtx) y;

  return (MEM_HASH_VAL (list->loc) == MEM_HASH_VAL (mem));
}

/* The hash function for variable_htab, computes the hash value
   of the variable X declaration.  */

static hashval_t
variable_htab_hash (x)
     const void *x;
{
  const variable v = (const variable) x;

  return (VARIABLE_HASH_VAL (v->decl));
}

/* Compare the declaration of variable X with declaration Y.  */

static int
variable_htab_eq (x, y)
     const void *x;
     const void *y;
{
  const variable v = (const variable) x;
  const tree decl = (const tree) y;

  return (VARIABLE_HASH_VAL (v->decl) == VARIABLE_HASH_VAL (decl));
}

/* Initialize the set (array) SET of the attrs_lists to empty lists.  */

static void
init_attrs_list_set (set)
     attrs_list *set;
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    set[i] = NULL;
}

/* Make the list *LISTP empty.  */

static void
attrs_list_clear (listp)
     attrs_list *listp;
{
  attrs_list list, next_list;

  for (list = *listp; list; list = next_list)
    {
      next_list = list->next;
      free (list);
    }
  *listp = NULL;
}

/* Return true if the pair of DECL and OFFSET is the member of the LIST.  */

static attrs_list
attrs_list_member (list, decl, offset)
     attrs_list list;
     tree decl;
     HOST_WIDE_INT offset;
{
  for (; list; list = list->next)
    if (list->decl == decl && list->offset == offset)
      return list;
  return NULL;
}

/* Insert the triplet DECL, OFFSET, LOC to the list *LISTP.  */

static void
attrs_list_insert (listp, decl, offset, loc)
     attrs_list *listp;
     tree decl;
     HOST_WIDE_INT offset;
     rtx loc;
{
  attrs_list list;

  list = xmalloc (sizeof (*list));
  list->loc = loc;
  list->decl = decl;
  list->offset = offset;
  list->next = *listp;
  *listp = list;
}

/* Delete all occurences of the pairs DECL and OFFSET from the list *LISTP.  */

static void
attrs_list_delete (listp, decl, offset)
     attrs_list *listp;
     tree decl;
     HOST_WIDE_INT offset;
{
  attrs_list list, next_list, prev_list = NULL;

  for (list = *listp; list; list = next_list)
    {
      next_list = list->next;
      if (list->decl == decl && list->offset == offset)
	{
	  if (prev_list)
	    prev_list->next = next_list;
	  else
	    *listp = next_list;
	  free (list);
	}
      else
	prev_list = list;
    }
}

/* Make a copy of all nodes from SRC and make a list *DSTP of the copies.  */

static void
attrs_list_copy (dstp, src)
     attrs_list *dstp;
     attrs_list src;
{
  attrs_list n;

  attrs_list_clear (dstp);
  for (; src; src = src->next)
    {
      n = xmalloc (sizeof (*n));
      n->loc = src->loc;
      n->decl = src->decl;
      n->offset = src->offset;
      n->next = *dstp;
      *dstp = n;
    }
}

/* Add all nodes from SRC which are not in *DSTP to *DSTP.  */

static void
attrs_list_union (dstp, src)
     attrs_list *dstp;
     attrs_list src;
{
  for (; src; src = src->next)
    {
      if (!attrs_list_member (*dstp, src->decl, src->offset))
	attrs_list_insert (dstp, src->decl, src->offset, src->loc);
    }
}

/* Return true if the lists A and B do not have the same DECL and OFFSET values
   in nodes.  */

static bool
attrs_list_different (a, b)
     attrs_list a;
     attrs_list b;
{
  attrs_list i;

  for (i = a; i; i = i->next)
    if (!attrs_list_member (b, i->decl, i->offset))
      return true;

  for (i = b; i; i = i->next)
    if (!attrs_list_member (a, i->decl, i->offset))
      return true;

  return false;
}

/* Insert MEM attributes to appropriate list in HTAB.  */

static void
attrs_htab_insert (htab, mem)
     htab_t htab;
     rtx mem;
{
  attrs_list list, *listp;

  listp = (attrs_list *) htab_find_slot_with_hash (htab, mem,
						   MEM_HASH_VAL (mem),
						   NO_INSERT);
  if (!listp)
    {
      listp = (attrs_list *) htab_find_slot_with_hash (htab, mem,
						       MEM_HASH_VAL (mem),
						       INSERT);
    }

  list = xmalloc (sizeof (*list));
  list->loc = mem;
  list->decl = MEM_EXPR (mem);
  list->offset = MEM_OFFSET (mem) ? INTVAL (MEM_OFFSET (mem)) : 0;
  list->next = *listp;
  *listp = list;
}

/* Delete MEM attributes from HTAB.  */

static void
attrs_htab_delete (htab, mem)
     htab_t htab;
     rtx mem;
{
  attrs_list list, next_list, prev_list;
  attrs_list *listp;
  HOST_WIDE_INT offset;
  tree decl;

  listp = (attrs_list *) htab_find_slot_with_hash (htab, mem,
						   MEM_HASH_VAL (mem),
						   NO_INSERT);
  if (!listp)
    return;

  offset = MEM_OFFSET (mem) ? INTVAL (MEM_OFFSET (mem)) : 0;
  decl = MEM_EXPR (mem);

  for (list = *listp; list; list = list->next)
    if (list->decl != decl || list->offset != offset)
      break;

  if (list)	/* There is at least 1 node that will not be deleted.  */
    {
      prev_list = NULL;
      for (list = *listp; list; list = next_list)
	{
	  next_list = list->next;
	  if (list->decl == decl && list->offset == offset)
	    {
	      if (prev_list)
		prev_list->next = next_list;
	      else
		*listp = next_list;
	      free (list);
	    }
	  else
	    prev_list = list;
	}
    }
  else
    {
      htab_clear_slot (htab, (void **) listp);
    }
}

/* Flag whether two hash-tables being compared contain different data.  */
static bool
attrs_htab_different_value;

/* Compare one SLOT with the corresponding slot from hash-table DATA.  */
   
static int
attrs_htab_different_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = (htab_t) data;
  attrs_list list1, list2, list;

  list1 = *(attrs_list *) slot;
  list2 = (attrs_list) htab_find_with_hash (htab, list1->loc,
					    MEM_HASH_VAL (list1->loc));

  if (!list1)
    abort ();
  for (; list1; list1 = list1->next)
    {
      for (list = list2; list; list = list->next)
	if (list->decl == list1->decl && list->offset == list1->offset)
	  break;
      if (!list)
	{
	  attrs_htab_different_value = true;
	  return 0;
	}
    }
  return 1;
}

/* Return true if HTAB1 and HTAB2 contain different data.  */

static bool
attrs_htab_different (htab1, htab2)
     htab_t htab1;
     htab_t htab2;
{
  attrs_htab_different_value = false;
  htab_traverse (htab1, attrs_htab_different_1, htab2);
  if (!attrs_htab_different_value)
    htab_traverse (htab2, attrs_htab_different_1, htab1);

  return attrs_htab_different_value;
}

/* Copy one SLOT to hash-table DATA.  */

static int
attrs_htab_copy_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t dst = (htab_t) data;
  attrs_list src, *dstp, list;

  src = *(attrs_list *) slot;
  dstp = (attrs_list *) htab_find_slot_with_hash (dst, src->loc,
						  MEM_HASH_VAL (src->loc),
						  INSERT);
  for (; src; src = src->next)
    {
      list = xmalloc (sizeof (*list));
      list->loc = src->loc;
      list->decl = src->decl;
      list->offset = src->offset;
      list->next = *dstp;
      *dstp = list;
    }
  return 1;
}

/* Copy the content of hash-table SRC to DST.  */

static void
attrs_htab_copy (dst, src)
     htab_t dst;
     htab_t src;
{
  attrs_htab_clear (dst);
  htab_traverse (src, attrs_htab_copy_1, dst);
}

/* Add all nodes from list in SLOT which are not in hash DATA to DATA.  */

static int
attrs_htab_union_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = (htab_t) data;
  attrs_list src, *dstp, list;

  src = *(attrs_list *) slot;
  dstp = (attrs_list *) htab_find_slot_with_hash (htab, src->loc,
						  MEM_HASH_VAL (src->loc),
						  NO_INSERT);
  if (!dstp)
    {
      dstp = (attrs_list *) htab_find_slot_with_hash (htab, src->loc,
						      MEM_HASH_VAL (src->loc),
						      INSERT);
    }
  for (; src; src = src->next)
    {
      for (list = *dstp; list; list = list->next)
	if (list->decl == src->decl && list->offset == src->offset)
	  break;
      if (!list)
	{
	  list = xmalloc (sizeof (*list));
	  list->loc = src->loc;
	  list->decl = src->decl;
	  list->offset = src->offset;
	  list->next = *dstp;
	  *dstp = list;
	}
    }
  return 1;
}

/* Add all nodes from SRC which are not in hash DST to DST.  */

static void
attrs_htab_union (dst, src)
     htab_t dst;
     htab_t src;
{
  htab_traverse (src, attrs_htab_union_1, dst);
}

/* Delete all data from HTAB.  */

static void
attrs_htab_clear (htab)
     htab_t htab;
{
  htab_empty (htab);
}

/* Delete data from SLOT.  */

static void
attrs_htab_cleanup (slot)
     void *slot;
{
  attrs_list list, next_list;

  for (list = (attrs_list) slot; list; list = next_list)
    {
      next_list = list->next;
      free (list);
    }
}

/* Shall EXPR be tracked?  */

static bool
track_expr_p (expr)
     tree expr;
{
  rtx decl_rtl;

  /* If EXPR is not a parameter or a variable do not track it.  */
  if (TREE_CODE (expr) != VAR_DECL && TREE_CODE (expr) != PARM_DECL)
    return 0;
 
  /* It also must have a name...  */
  if (!DECL_NAME (expr))
    return 0;
  
  /* ... and a RTL assigned to it.  */
  decl_rtl = DECL_RTL (expr);
  if (!decl_rtl)
    return 0;
    
  /* If RTX is a memory it should not be very large (because it would be an array
     or struct).  */
  if (GET_CODE (decl_rtl) == MEM)
    {
      if (MEM_SIZE (decl_rtl) && INTVAL (MEM_SIZE (decl_rtl)) > MAX_LOC_PARTS)
	return 0;
    }
 
  return 1;
}

/* Scan rtx X for registers and memory references. Other parameters are
   in struct scan_for_locations_data passed in DATA.  */

static int
scan_for_locations (x, data)
     rtx *x;
     void *data;
{
  scan_for_locations_data *d = (scan_for_locations_data *) data;
  basic_block bb = BLOCK_FOR_INSN (d->insn);
  location *l;
  enum location_type old_type;

  switch (d->oper)
    {
      case SO_SKIP:	/* Do not process current rtx X.  */
	/* Store the locations from subexpressions.  */
	d->oper = SO_STORE;
	return 0;

      case SO_COUNT:	/* Count the locations.  */
	switch (GET_CODE (*x))
	  {
	    case REG:
#ifdef ENABLE_CHECKING
	      if (REGNO (*x) >= FIRST_PSEUDO_REGISTER)
		abort ();
#endif
	      VTI (bb)->n_locs++;
	      /* Continue traversing.  */
	      return 0;

	    case MEM:
	      if (MEM_EXPR (*x) && track_expr_p (MEM_EXPR (*x)))
		VTI (bb)->n_locs++;
	      /* Continue traversing.  */
	      return 0;

	    default:
	      /* Continue traversing.  */
	      return 0;
	  }
	break;

      case SO_STORE:	/* Store the locations.  */
	switch (GET_CODE (*x))
	  {
	    case SET:
	      old_type = d->type;
	      d->type = LT_PARAM;
	      for_each_rtx (&SET_SRC (*x), scan_for_locations, data);
	      d->type = LT_SET_DEST;
	      for_each_rtx (&SET_DEST (*x), scan_for_locations, data);
	      d->type = old_type;
	      /* Do not traverse sub-expressions.  */
	      return -1;

	    case CLOBBER:
	      old_type = d->type;
	      d->type = LT_CLOBBERED;
	      for_each_rtx (&SET_DEST (*x), scan_for_locations, data);
	      d->type = old_type;
	      /* Do not traverse sub-expressions.  */
	      return -1;

	    case REG:
	      l = VTI (bb)->locs + VTI (bb)->n_locs++;
	      l->loc = *x;
	      l->insn = d->insn;
	      l->type = d->type;
	      /* Do not traverse sub-expressions.  */
	      return -1;

	    case MEM:
	      if (MEM_EXPR (*x) && track_expr_p (MEM_EXPR (*x)))
		{
		  l = VTI (bb)->locs + VTI (bb)->n_locs++;
		  l->loc = *x;
		  l->insn = d->insn;
		  l->type = d->type;
		  if (d->type != LT_PARAM)
		    {
		      /* Scan for params in subexpressions.  */
		      old_type = d->type;
		      d->oper = SO_SKIP;
		      d->type = LT_PARAM;
		      for_each_rtx (x, scan_for_locations, data);
		      d->type = old_type;
		      /* Do not traverse sub-expressions.  */
		      return -1;
		    }
		}
	      /* Continue traversing.  */
	      return 0;

	    default:
	      /* Continue traversing.  */
	      return 0;
	  }
	break;
    }

  return 0;
}

/* Compute the changes of variable locations in the basic block BB.  */

static bool
compute_bb_dataflow (bb)
     basic_block bb;
{
  int i, n;
  bool changed;

  attrs_list old_out[FIRST_PSEUDO_REGISTER];
  htab_t old_mem_out;
  attrs_list *in = VTI (bb)->in;
  attrs_list *out = VTI (bb)->out;

  init_attrs_list_set (old_out);
  old_mem_out = htab_create (htab_elements (VTI (bb)->mem_out) + 3,
			     mem_htab_hash, mem_htab_eq, attrs_htab_cleanup);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      attrs_list_copy (&old_out[i], out[i]);
      attrs_list_copy (&out[i], in[i]);
    }
  attrs_htab_copy (old_mem_out, VTI (bb)->mem_out);
  attrs_htab_copy (VTI (bb)->mem_out, VTI (bb)->mem_in);

  n = VTI (bb)->n_locs;
  for (i = 0; i < n; i++)
    {
      rtx loc = VTI (bb)->locs[i].loc;

      if (GET_CODE (loc) == REG)
	{
	  attrs_list_clear (&out[REGNO (loc)]);
	  if (VTI (bb)->locs[i].type == LT_PARAM
	      || VTI (bb)->locs[i].type == LT_SET_DEST)
	    {
	      if (REG_EXPR (loc) && track_expr_p (REG_EXPR (loc)))
		{
		  attrs_list_insert (&out[REGNO (loc)], REG_EXPR (loc),
				     REG_OFFSET (loc), loc);
		}
	    }
	}
      else if (GET_CODE (loc) == MEM
	       && MEM_EXPR (loc)
	       && track_expr_p (MEM_EXPR (loc)))
	{
	  int j;
	  tree decl = MEM_EXPR (loc);
	  HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

	  attrs_htab_delete (VTI (bb)->mem_out, loc);
	  if (VTI (bb)->locs[i].type == LT_PARAM
	      || VTI (bb)->locs[i].type == LT_SET_DEST)
	    {
	      /* The variable is no longer in any register.  */
	      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
		attrs_list_delete (&out[j], decl, offset);
	      attrs_htab_insert (VTI (bb)->mem_out, loc);
	    }
	}
    }

  changed = false;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (attrs_list_different (old_out[i], out[i]))
      {
	changed = true;
	break;
      }
  if (!changed)
    changed = attrs_htab_different (old_mem_out, VTI (bb)->mem_out);

  attrs_list_clear (old_out);
  attrs_htab_clear (old_mem_out);
  htab_delete (old_mem_out);
  return changed;
}

/* Hybrid search.  It is the same as hybrid_search_bitmap in df.c but modified
   for different data structures.
   Search from basic block BB and use the bitmaps VISITED and PENDING
   to control the searching.  */

static void
hybrid_search (bb, visited, pending)
     basic_block bb;
     sbitmap visited;
     sbitmap pending;
{
  bool changed;
  edge e;
  int i;

  SET_BIT (visited, bb->index);
  if (TEST_BIT (pending, bb->index))
    {
      /* Clear the IN list set.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	attrs_list_clear (&VTI (bb)->in[i]);
      attrs_htab_clear (VTI (bb)->mem_in);

      /* Calculate the union of predecessor outs.  */
      for (e = bb->pred; e; e = e->pred_next)
	{
	  attrs_list *bb_in = VTI (bb)->in;
	  attrs_list *pred_out = VTI (e->src)->out;

	  if (e->src == ENTRY_BLOCK_PTR)
	    continue;

	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    attrs_list_union (&bb_in[i], pred_out[i]);
	  attrs_htab_union (VTI (bb)->mem_in, VTI (e->src)->mem_out);
	}

      RESET_BIT (pending, bb->index);
      changed = compute_bb_dataflow (bb);
      if (changed)
	{
	  for (e = bb->succ; e != 0; e = e->succ_next)
	    {
	      if (e->dest == EXIT_BLOCK_PTR || e->dest->index == bb->index)
		continue;
	      SET_BIT (pending, e->dest->index);
	    }
	}
    }
  for (e = bb->succ; e != 0; e = e->succ_next)
    {
      if (e->dest == EXIT_BLOCK_PTR || e->dest->index == bb->index)
	continue;
      if (!TEST_BIT (visited, e->dest->index))
	hybrid_search (e->dest, visited, pending);
    }
}

/* This function will perform iterative dataflow, producing the in and out
   sets.  It is the same as iterative_dataflow_bitmap in df.c but modified
   for different data structures.
   BB_ORDER is the order to iterate in (Should map block numbers -> order).
   Because this is a forward dataflow problem we pass in a mapping of block
   number to rc_order (like df->inverse_rc_map).  */

static void
iterative_dataflow (bb_order)
     int *bb_order;
{
  fibheap_t worklist;
  basic_block bb;
  sbitmap visited, pending;

  pending = sbitmap_alloc (last_basic_block);
  visited = sbitmap_alloc (last_basic_block);
  sbitmap_zero (pending);
  sbitmap_zero (visited);
  worklist = fibheap_new ();

  FOR_EACH_BB (bb)
    {
      fibheap_insert (worklist, bb_order[bb->index], bb);
      SET_BIT (pending, bb->index);
    }
  while (sbitmap_first_set_bit (pending) != -1)
    {
      while (!fibheap_empty (worklist))
	{
	  bb = fibheap_extract_min (worklist);
	  if (!TEST_BIT (visited, bb->index))
	    hybrid_search (bb, visited, pending);
	}
      if (sbitmap_first_set_bit (pending) != -1)
	{
	  FOR_EACH_BB (bb)
	    {
	      fibheap_insert (worklist, bb_order[bb->index], bb);
	    }
	  sbitmap_zero (visited);
	}
      else
	{
	  break;
	}
    }
  sbitmap_free (pending);
  sbitmap_free (visited);
  fibheap_delete (worklist);
}

/* Print the content of the LIST to dump file.  */

static inline void
dump_attrs_list (list)
     attrs_list list;
{
  for (; list; list = list->next)
    {
      print_mem_expr (rtl_dump_file, list->decl);
      fprintf (rtl_dump_file, "{%p}[", (void*) list->decl);
      fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, list->offset);
      fprintf (rtl_dump_file, "], ");
    }
}

/* Print the content of the list in SLOT to dump file.  */

static int
dump_mem_attrs_list (slot, data)
     void **slot;
     void *data ATTRIBUTE_UNUSED;
{
  dump_attrs_list (*(attrs_list *) slot);
  return 1;
}

/* Print the IN and OUT sets for each basic block to dump file.  */

static void
dump_attrs_list_sets ()
{
  int i;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      fprintf (rtl_dump_file, "\nBasic block %d:\n", bb->index);

      fprintf (rtl_dump_file, "  IN: ");
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  if (VTI (bb)->in[i])
	    {
	      fprintf (rtl_dump_file, "Reg %d:", i);
	      dump_attrs_list (VTI (bb)->in[i]);
	      fprintf (rtl_dump_file, "; ");
	    }
	}
      if (htab_elements (VTI (bb)->mem_in) > 0)
	{
	  fprintf (rtl_dump_file, "Mem:");
	  htab_traverse (VTI (bb)->mem_in, dump_mem_attrs_list, NULL);
	}
      fprintf (rtl_dump_file, "\n");

      fprintf (rtl_dump_file, "  OUT: ");
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  if (VTI (bb)->out[i])
	    {
	      fprintf (rtl_dump_file, "Reg %d:", i);
	      dump_attrs_list (VTI (bb)->out[i]);
	      fprintf (rtl_dump_file, "; ");
	    }
	}
      if (htab_elements (VTI (bb)->mem_out) > 0)
	{
	  fprintf (rtl_dump_file, "Mem:");
	  htab_traverse (VTI (bb)->mem_out, dump_mem_attrs_list, NULL);
	}
      fprintf (rtl_dump_file, "\n");
    }
}

/* Emit the NOTE_INSN_VAR_LOCATION for variable VAR. WHERE specifies
   whether the note shall be emited before of after instruction INSN.  */

static void
note_insn_var_location_emit (insn, where, var)
     rtx insn;
     enum where_emit_note where;
     variable var;
{
  rtx note;

#ifdef ENABLE_CHECKING
  if (!var->decl)
    abort ();
#endif

  if (where == EMIT_NOTE_AFTER_INSN)
    note = emit_note_after (NOTE_INSN_VAR_LOCATION, insn);
  else
    note = emit_note_before (NOTE_INSN_VAR_LOCATION, insn);

  if (var->n_location_parts == 1)
    {
      rtx expr_list
	= gen_rtx_EXPR_LIST (VOIDmode,
			     var->location_part[0].loc,
			     GEN_INT (var->location_part[0].offset));
      rtx var_location = gen_rtx_VAR_LOCATION (VOIDmode, var->decl, expr_list);
      NOTE_VAR_LOCATION (note) = var_location;
    }
  else if (var->n_location_parts)
    {
      int i;
      rtx argp[MAX_LOC_PARTS];
      rtx parallel;
      rtx var_location;

      for (i = 0; i < var->n_location_parts; i++)
	argp[i] = gen_rtx_EXPR_LIST (VOIDmode,
				     var->location_part[i].loc,
				     GEN_INT (var->location_part[i].offset));
      parallel = gen_rtx_PARALLEL (VOIDmode,
				   gen_rtvec_v (var->n_location_parts, argp));
      var_location = gen_rtx_VAR_LOCATION (VOIDmode, var->decl, parallel);
      NOTE_VAR_LOCATION (note) = var_location;
    }
  else
    abort ();
}

/* Set the part of variable's location. The variable part is specified
   by variable's declaration DECL and offset OFFSET and the part's location
   by LOC. The INSN and WHERE parameters specify where the note will be emited
   (see note_insn_var_location_emit).  */

static void
set_location_part (decl, offset, loc, insn, where)
     tree decl;
     HOST_WIDE_INT offset;
     rtx loc;
     rtx insn;
     enum where_emit_note where;
{
  int k;
  variable var = htab_find_with_hash (variable_htab, decl,
				      VARIABLE_HASH_VAL (decl));
  if (!var)
    {
      void **slot;
      /* Create new variable information.  */
      var = xmalloc (sizeof (*var));
      var->decl = decl;
      var->n_location_parts = 0;

      slot = htab_find_slot_with_hash (variable_htab, decl,
				       VARIABLE_HASH_VAL (decl), INSERT);
#ifdef ENABLE_CHECKING
      if (*slot)
	abort ();
#endif
      *slot = (void *) var;
      k = 0;
    }
  else
    {
      /* Find the location part.  */
      for (k = 0; k < var->n_location_parts; k++)
	if (var->location_part[k].offset == offset)
	  break;
    }
  if (k == var->n_location_parts)
    {
      /* Did not find the part, create new one.  */
      if (var->n_location_parts >= MAX_LOC_PARTS)
	abort ();
      var->n_location_parts++;
      var->location_part[k].offset = offset;
      var->location_part[k].loc = NULL;
    }

  /* Mark the location part that it should not be deleted.  */
  var->location_part[k].delete_p = false;
  if (var->location_part[k].loc != loc)
    {
      emit_note_data emit_note_data;
      void *slot;

      var->location_part[k].loc = loc;
      var->changed = true;
      emit_note_data.insn = insn;
      emit_note_data.where = where;
      slot = var;
      emit_note_if_var_changed ((void **) &slot, &emit_note_data);
      var = slot;
    }
}

/* Delete the part of variable's location. The variable part is specified
   by variable's declaration DECL and offset OFFSET.
   The INSN and WHERE parameters specify where the note will be emited
   (see note_insn_var_location_emit).  */

static void
delete_location_part (decl, offset, insn, where)
     tree decl;
     HOST_WIDE_INT offset;
     rtx insn;
     enum where_emit_note where;
{
  int i;
  void **slot = htab_find_slot_with_hash (variable_htab, decl,
					  VARIABLE_HASH_VAL (decl), NO_INSERT);
  if (slot && *slot)
    {
      variable var = (variable) *slot;

      /* Delete the location part.  */
      for (i = 0; i < var->n_location_parts; i++)
	if (var->location_part[i].offset == offset)
	  {
	    emit_note_data emit_note_data;

	    var->n_location_parts--;
	    var->location_part[i] = var->location_part[var->n_location_parts];
	    var->changed = true;
	    emit_note_data.insn = insn;
	    emit_note_data.where = where;
	    emit_note_if_var_changed (slot, &emit_note_data);
	    break;
	  }
    }
}

/* Delete marked location parts of a variable and emit notes if needed.
   SLOT is the address of pointer to description of parts of the variable.
   AUX contains a pointer to data passed to functions emiting the notes.  */

static int
process_location_parts (slot, aux)
     void **slot;
     void *aux;
{
  variable var = *slot;
  int i;

  for (i = 0; i < var->n_location_parts; i++)
    if (var->location_part[i].delete_p)
      {
	var->n_location_parts--;
	var->location_part[i] = var->location_part[var->n_location_parts];
	var->changed = true;
      }
  emit_note_if_var_changed (slot, aux);
  return 1;
}

/* Emit NOTE_INSN_VAR_LOCATION notes for description **SLOT of a variable
   if any of the variable parts has changed.
   AUX contains data describing where the note will be emited (see
   note_insn_var_location_emit).  */

static void
emit_note_if_var_changed (slot, aux)
     void **slot;
     void *aux;
{
  variable var = *slot;
  emit_note_data *data = (emit_note_data *) aux;

  if (var->n_location_parts == 0)
    var->changed = false;

  if (var->changed)
    {
      var->changed = false;
      note_insn_var_location_emit (data->insn, data->where, var);
    }
  if (var->n_location_parts == 0)
    htab_clear_slot (variable_htab, slot);
}

/* Delete the location part of variable corresponding to LOC from all
   register locations (in LISTS) and memory locations (in HTAB) and emit
   notes before/after (parameter WHERE) INSN. The location part which
   has the same decl and offset as LOC is deleted from HTAB only when
   DELETE_LOC_FROM_HTAB is true.  */

static inline void
process_bb_delete (lists, htab, delete_loc_from_htab, loc, insn, where)
     attrs_list *lists;
     htab_t htab;
     int delete_loc_from_htab;
     rtx loc;
     rtx insn;
     enum where_emit_note where;
{
  attrs_list list;
  int i;

  tree decl = MEM_EXPR (loc);
  HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      for (list = lists[i]; list; list = list->next)
	if (list->offset == offset && list->decl == decl)
	  delete_location_part (list->decl, list->offset, insn, where);
      attrs_list_delete (&lists[i], decl, offset);
    }

  if (delete_loc_from_htab)
    {
      list = (attrs_list) htab_find_with_hash (htab, loc, MEM_HASH_VAL (loc));
      for (; list; list = list->next)
	if (list->offset == offset && list->decl == decl)
	  delete_location_part (list->decl, list->offset, insn, where);
      attrs_htab_delete (htab, loc);
    }
}

/* Emit the notes for changes of location parts of a variable in the basic
   block BB.  */

static void
process_bb (bb)
     basic_block bb;
{
  int i, n;
  attrs_list reg[FIRST_PSEUDO_REGISTER];
  htab_t mem;

  init_attrs_list_set (reg);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_copy (&reg[i], VTI (bb)->in[i]);
  mem = htab_create (htab_elements (VTI (bb)->mem_in) + 3, mem_htab_hash,
			   mem_htab_eq, attrs_htab_cleanup);
  attrs_htab_copy (mem, VTI (bb)->mem_in);

  n = VTI (bb)->n_locs;
  for (i = 0; i < n; i++)
    {
      rtx insn = VTI (bb)->locs[i].insn;
      rtx loc = VTI (bb)->locs[i].loc;

      if (GET_CODE (loc) == REG)
	{
	  attrs_list l;
	  tree decl = REG_EXPR (loc);
	  HOST_WIDE_INT offset = REG_OFFSET (loc);
	  enum where_emit_note where = EMIT_NOTE_AFTER_INSN;

	  switch (VTI (bb)->locs[i].type)
	    {
	      case LT_PARAM:
		where = EMIT_NOTE_BEFORE_INSN;
	      case LT_SET_DEST:
		for (l = reg[REGNO (loc)]; l; l = l->next)
		  if (l->decl != decl || l->offset != offset)
		    delete_location_part (l->decl, l->offset, insn, where);
		attrs_list_clear (&reg[REGNO (loc)]);
		if (decl && track_expr_p (decl))
		  {
		    set_location_part (decl, offset, loc, insn, where);
		    attrs_list_insert (&reg[REGNO (loc)], decl, offset, loc);
		  }
		else
		  delete_location_part (decl, offset, insn, where);
		break;

	      case LT_CLOBBERED:
		for (l = reg[REGNO (loc)]; l; l = l->next)
		  delete_location_part (l->decl, l->offset, insn,
					EMIT_NOTE_AFTER_INSN);
		attrs_list_clear (&reg[REGNO (loc)]);
		break;
	    }
	}
      else if (GET_CODE (loc) == MEM
	       && MEM_EXPR (loc)
	       && track_expr_p (MEM_EXPR (loc)))
	{
	  enum where_emit_note where = EMIT_NOTE_AFTER_INSN;
	  tree decl = MEM_EXPR (loc);
	  HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

	  switch (VTI (bb)->locs[i].type)
	    {
	      case LT_PARAM:
		where = EMIT_NOTE_BEFORE_INSN;
	      case LT_SET_DEST:
		process_bb_delete (reg, mem, false, loc, insn, where);
		set_location_part (decl, offset, loc, insn, where);
		attrs_htab_insert (mem, loc);
		break;

	      case LT_CLOBBERED:
		process_bb_delete (reg, mem, true, loc, insn,
				   EMIT_NOTE_AFTER_INSN);
		break;
	    }
	}
    }
  htab_delete (mem);
}

/* Mark the location parts for the variables from the list *SLOT
   to be deleted.  */

static int
mark_variables_for_deletion (slot, data)
     void **slot;
     void *data ATTRIBUTE_UNUSED;
{
  attrs_list l;

  for (l = *(attrs_list *) slot; l; l = l->next)
    {
      variable var = htab_find_with_hash (variable_htab, l->decl,
					  VARIABLE_HASH_VAL (l->decl));
      if (var)
	{
	  int k;
	  for (k = 0; k < var->n_location_parts; k++)
	    if (var->location_part[k].offset == l->offset)
	      {
		var->location_part[k].delete_p = true;
		break;
	      }
	}
    }
  return 1;
}

/* Add the location parts for variables from the list *SLOT and emit notes
   before insn DATA.  */

static int
add_and_unmark_variables (slot, data)
     void **slot;
     void *data;
{
  attrs_list l;
  rtx insn = (rtx) data;

  for (l = *(attrs_list *) slot; l; l = l->next)
    {
      set_location_part (l->decl, l->offset, l->loc, insn,
			 EMIT_NOTE_BEFORE_INSN);
    }
  return 1;
}

/* Emit notes for the whole function.  */

static void
var_tracking_emit_notes ()
{
  basic_block bb;
  int i;
  attrs_list *last_out, *in;
  attrs_list empty[FIRST_PSEUDO_REGISTER];
  htab_t last_htab;

  init_attrs_list_set (empty);
  last_out = empty;
  last_htab = htab_create (7, mem_htab_hash, mem_htab_eq, attrs_htab_cleanup);

  FOR_EACH_BB (bb)
    {
      emit_note_data emit_note_data;

      /* Emit the notes for changes of variable locations between two
	 sequential basic blocks.  */
      /* Mark the variables of previous OUT set.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	mark_variables_for_deletion ((void **) &last_out[i], NULL);
      htab_traverse (last_htab, mark_variables_for_deletion, NULL);

      /* Add the variables of IN set and unmark them.  */
      in = VTI (bb)->in;
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	add_and_unmark_variables ((void **) &in[i], bb->head);
      htab_traverse (last_htab, add_and_unmark_variables, bb->head);

      /* Delete the marked location parts and emit the notes.  */
      emit_note_data.insn = bb->head;
      emit_note_data.where = EMIT_NOTE_BEFORE_INSN;
      htab_traverse (variable_htab, process_location_parts, &emit_note_data);

      /* Emit the notes for the changes in the basic block itself.  */
      process_bb (bb);

      last_out = VTI (bb)->out;
      attrs_htab_copy (last_htab, VTI (bb)->mem_out);
    }
  htab_delete (last_htab);
}

/* Allocate and initialize the data structures for variable tracking
   and parse the RTL to get the REG and MEM references.  */

static void
var_tracking_initialize ()
{
  basic_block bb;
  scan_for_locations_data data;

  alloc_aux_for_blocks (sizeof (struct var_tracking_info_def));

  FOR_EACH_BB (bb)
    {
      rtx insn;

      /* Count the number of stores.  */
      VTI (bb)->n_locs = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      data.oper = SO_COUNT;
	      data.insn = insn;
	      for_each_rtx (&PATTERN (insn), scan_for_locations, &data);
	    }
	}

      /* Add the stores to the array.  */
      VTI (bb)->locs = xmalloc (VTI (bb)->n_locs
				* sizeof (struct location_def));
      VTI (bb)->n_locs = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      int n1, n2;

	      data.type = LT_PARAM;
	      data.oper = SO_STORE;
	      data.insn = insn;
	      n1 = VTI (bb)->n_locs;
	      for_each_rtx (&PATTERN (insn), scan_for_locations, &data);
	      n2 = VTI (bb)->n_locs - 1;

	      /* Order the locations so that the locations of type LT_PARAM are
		 before others.  */
	      while (n1 < n2)
		{
		  while (n1 < n2 && VTI (bb)->locs[n1].type == LT_PARAM)
		    n1++;
		  while (n1 < n2 && VTI (bb)->locs[n2].type != LT_PARAM)
		    n2--;
		  if (n1 < n2)
		    {
		      location sw;
		      sw = VTI (bb)->locs[n1];
		      VTI (bb)->locs[n1] = VTI (bb)->locs[n2];
		      VTI (bb)->locs[n2] = sw;
		    }
		}

	      /* Now the LT_PARAMs are first, order the rest so that the
		 LT_SET_DESTs are before LT_CLOBBEREDs.  */
	      n2 = VTI (bb)->n_locs - 1;
	      while (n1 < n2)
		{
		  while (n1 < n2 && VTI (bb)->locs[n1].type == LT_SET_DEST)
		    n1++;
		  while (n1 < n2 && VTI (bb)->locs[n2].type == LT_CLOBBERED)
		    n2--;
		  if (n1 < n2)
		    {
		      location sw;
		      sw = VTI (bb)->locs[n1];
		      VTI (bb)->locs[n1] = VTI (bb)->locs[n2];
		      VTI (bb)->locs[n2] = sw;
		    }
		}
	    }
	}

      /* Init the IN and OUT arrays.  */
      init_attrs_list_set (VTI (bb)->in);
      init_attrs_list_set (VTI (bb)->out);
      VTI (bb)->mem_in = htab_create (7, mem_htab_hash, mem_htab_eq,
				      attrs_htab_cleanup);
      VTI (bb)->mem_out = htab_create (7, mem_htab_hash, mem_htab_eq,
				       attrs_htab_cleanup);
    }

  variable_htab = htab_create (37, variable_htab_hash, variable_htab_eq, free);
}

/* Free the data structures needed for variable tracking.  */

static void
var_tracking_finalize ()
{
  basic_block bb;
  int i;

  FOR_EACH_BB (bb)
    {
      free (VTI (bb)->locs);

      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  attrs_list_clear (&VTI (bb)->in[i]);
	  attrs_list_clear (&VTI (bb)->out[i]);
	}
      htab_delete (VTI (bb)->mem_in);
      htab_delete (VTI (bb)->mem_out);
    }
  free_aux_for_blocks ();
}

/* The entry point to variable tracking pass.  */

void
variable_tracking_main ()
{
  int i;
  int *rc_order;
  int *bb_order;

  var_tracking_initialize ();

  /* Compute depth first search order of the CFG so that
     the dataflow run possibly faster.  */
  rc_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  bb_order = (int *) xmalloc (last_basic_block * sizeof (int));
  flow_depth_first_order_compute (NULL, rc_order);
  for (i = 0; i < n_basic_blocks; i++)
    bb_order[rc_order[i]] = i;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "RC order: \n");
      for (i = 0; i < n_basic_blocks; i++)
	fprintf (rtl_dump_file, "%d ", rc_order[i]);
      fprintf (rtl_dump_file, "\n");
    }

  iterative_dataflow (bb_order);
  var_tracking_emit_notes ();

  if (rtl_dump_file)
    {
      dump_attrs_list_sets ();
      dump_flow_info (rtl_dump_file);
    }

  free (rc_order);
  free (bb_order);
  var_tracking_finalize ();
}
