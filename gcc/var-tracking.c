/* Variable tracking routines for the GNU compiler.
   Copyright (C) 2002 Free Software Foundation, Inc.

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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "tree.h"
#include "output.h"
#include "sbitmap.h"
#include "fibheap.h"
#include "hashtab.h"

#define IN_OUT_SIZE (FIRST_PSEUDO_REGISTER + 1)
#define MEMORY_VAR FIRST_PSEUDO_REGISTER

#define DEBUG_REG_LOC 1
#define DEBUG_MEM_LOC 1

/* The purpose that the location (REG or MEM) has in RTL.  */
enum location_type
{
  LT_PARAM,   /* Location is a parameter of instruction.  */
  LT_SET_DEST,        /* Location is the destination of the SET.  */
  LT_CLOBBERED        /* Location is the "destination" of CLOBBER.  */
};

/* Operation that is performed when scanning RTL.  */
enum scan_operation
{
  SO_COUNT,   /* Count the number of locations.  */
  SO_STORE,   /* Store the locatons to the array VTI (bb)->locs.  */
  SO_SKIP     /* Skip current node, and scan and store its internals.  */
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
/* The variables stored in memory are NOW also remembered in a link-list.
   This will be changed to some better data structure because there can be
   many variables in function (stored in memory).  */

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

  /* Input attributes (lists of attrs).  */
  attrs_list in[IN_OUT_SIZE];

  /* Output attributes.  */
  attrs_list out[IN_OUT_SIZE];
} *var_tracking_info;

#define VTI(BB) ((var_tracking_info) (BB)->aux)

#define MAX_LOC 16

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

/* Structure for description where the variable is located.  */
typedef struct variable_def
{
  /* The declaration of the variable.  */
  tree decl;

  /* Number of locations.  */
  int n_location_parts;

  /* The locations.  */
  location_part location_part[MAX_LOC];

  /* Changed?  */
  bool changed;
} *variable;

/* The hashtable of STRUCT VARIABLE_DEF.  */
static htab_t variable_htab;

/* Local function prototypes.  */
static void init_attrs_list_set               PARAMS ((attrs_list *));
static void attrs_list_clear          PARAMS ((attrs_list *));
static attrs_list attrs_list_member   PARAMS ((attrs_list, tree,
                                               HOST_WIDE_INT));
static attrs_list attrs_list_member_alias     PARAMS ((attrs_list,
                                                       HOST_WIDE_INT,
                                                       HOST_WIDE_INT));
static void attrs_list_insert         PARAMS ((attrs_list *, tree,
                                               HOST_WIDE_INT, rtx));
static void attrs_list_delete         PARAMS ((attrs_list *, tree,
                                               HOST_WIDE_INT));
static void attrs_list_delete_alias   PARAMS ((attrs_list *, HOST_WIDE_INT,
                                               HOST_WIDE_INT));
static void attrs_list_copy           PARAMS ((attrs_list *, attrs_list));
static bool attrs_list_different      PARAMS ((attrs_list, attrs_list));
static bool attrs_list_different_alias        PARAMS ((attrs_list, attrs_list));


static int scan_for_locations         PARAMS ((rtx *, void *));
static bool compute_bb_dataflow               PARAMS ((basic_block));
static void hybrid_search             PARAMS ((basic_block, sbitmap,
                                               sbitmap));
static void iterative_dataflow                PARAMS ((int *));
static void dump_attrs_list_sets      PARAMS ((void));

static hashval_t variable_htab_hash   PARAMS ((const void *));
static int variable_htab_eq           PARAMS ((const void *, const void *));
static void note_insn_var_location_emit       PARAMS ((rtx, enum where_emit_note,
                                               variable));
static void set_location_part         PARAMS ((tree, HOST_WIDE_INT, rtx,
                                               rtx, enum where_emit_note));
static void delete_location_part      PARAMS ((tree, HOST_WIDE_INT, rtx,
                                               enum where_emit_note));
static int process_location_parts     PARAMS ((void **, void *));
static void emit_note_if_var_changed  PARAMS ((void **, void *));
static void process_bb                        PARAMS ((basic_block));
static void var_tracking_emit_notes   PARAMS ((void));

static void var_tracking_initialize   PARAMS ((void));
static void var_tracking_finalize     PARAMS ((void));

/* Initialize the set (array) SET of the attrs_lists to empty lists.  */

static void init_attrs_list_set (set)
     attrs_list *set;
{
  int i;

  for (i = 0; i < IN_OUT_SIZE; i++)
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

/* Return true if there is a node in the list LIST that has the memory alias
   set of node->loc same as ALIAS and the offset equal to OFFSET.  */

static attrs_list
attrs_list_member_alias (list, alias, offset)
     attrs_list list;
     HOST_WIDE_INT alias;
     HOST_WIDE_INT offset;
{
  for (; list; list = list->next)
    if (MEM_ALIAS_SET (list->loc) == alias && list->offset == offset)
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
  attrs_list list, next_list, prev_list;

  for (list = *listp; list; list = next_list)
    {
      next_list = list->next;
      if (list->decl == decl && list->offset == offset)
      {
        if (list == *listp)
          *listp = next_list;
        else
          prev_list->next = next_list;
        free (list);
      }
      else
      prev_list = list;
    }
}

/* Delete all occurences from the list *LISTP that have the offset equal to
   OFFSET and memory alias set of node->loc equal to ALIAS.  */

static void
attrs_list_delete_alias (listp, alias, offset)
     attrs_list *listp;
     HOST_WIDE_INT alias;
     HOST_WIDE_INT offset;
{
  attrs_list list, next_list, prev_list;

  for (list = *listp; list; list = next_list)
    {
      next_list = list->next;
      if (MEM_ALIAS_SET (list->loc) == alias && list->offset == offset)
      {
        if (list == *listp)
          *listp = next_list;
        else
          prev_list->next = next_list;
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

/* Return true if the lists A and B do not have the same OFFSETs
   and memory alias sets loc in nodes.  */

static bool
attrs_list_different_alias (a, b)
     attrs_list a;
     attrs_list b;
{
  attrs_list i;

  for (i = a; i; i = i->next)
    if (!attrs_list_member_alias (b, MEM_ALIAS_SET (i->loc), i->offset))
      return true;

  for (i = b; i; i = i->next)
    if (!attrs_list_member_alias (a, MEM_ALIAS_SET (i->loc), i->offset))
      return true;

  return false;
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
      case SO_SKIP:   /* Do not process current rtx X.  */
      /* Store the locations from subexpressions.  */
      d->oper = SO_STORE;
      return 0;

      case SO_COUNT:  /* Count the locations.  */
      switch (GET_CODE (*x))
        {
          case REG:
            if (REGNO (*x) >= FIRST_PSEUDO_REGISTER)
              abort ();
            VTI (bb)->n_locs++;
            /* Continue traversing.  */
            return 0;

          case MEM:
            if (MEM_EXPR (*x))
              VTI (bb)->n_locs++;
            /* Continue traversing.  */
            return 0;

          default:
            /* Continue traversing.  */
            return 0;
        }
      break;

      case SO_STORE:  /* Store the locations.  */
      switch (GET_CODE (*x))
        {
          case SET:
            old_type = d->type;
            d->type = LT_SET_DEST;
            for_each_rtx (&SET_DEST (*x), scan_for_locations, data);
            d->type = LT_PARAM;
            for_each_rtx (&SET_SRC (*x), scan_for_locations, data);
            d->type = old_type;
            /* Stop traversing.  */
            return -1;

          case CLOBBER:
            old_type = d->type;
            d->type = LT_CLOBBERED;
            for_each_rtx (&SET_DEST (*x), scan_for_locations, data);
            d->type = old_type;
            /* Stop traversing.  */
            return -1;

          case REG:
            l = VTI (bb)->locs + VTI (bb)->n_locs++;
            l->loc = *x;
            l->insn = d->insn;
            l->type = d->type;
            /* Stop traversing.  */
            return -1;

          case MEM:
            if (MEM_EXPR (*x))
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
                    /* Stop traversing.  */
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

  attrs_list old_out[IN_OUT_SIZE];
  attrs_list *in = VTI (bb)->in;
  attrs_list *out = VTI (bb)->out;

  init_attrs_list_set (old_out);
  for (i = 0; i < IN_OUT_SIZE; i++)
    {
      attrs_list_copy (&old_out[i], out[i]);
      attrs_list_copy (&out[i], in[i]);
    }

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
            if (REG_EXPR (loc))
              {
                tree decl = REG_EXPR (loc);
                attrs_list_insert (&out[REGNO (loc)], REG_EXPR (loc),
                                   REG_OFFSET (loc), loc);
#if DEBUG_REG_LOC
                if (rtl_dump_file)
                  {
                    print_mem_expr (rtl_dump_file, REG_EXPR (loc));
                    print_rtl_single (rtl_dump_file, loc);
                    fprintf (rtl_dump_file, "\n");
                  }
#endif
              }
          }
      }
      else if (GET_CODE (loc) == MEM && MEM_EXPR (loc))
      {
        int j;
        tree decl = MEM_EXPR (loc);
        HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

#if DEBUG_MEM_LOC
        if (rtl_dump_file)
          {
            print_mem_expr (rtl_dump_file, decl);
            print_rtl_single (rtl_dump_file, loc);
            fprintf (rtl_dump_file, "\n");
          }
#endif
        attrs_list_delete_alias (&out[MEMORY_VAR], MEM_ALIAS_SET (loc),
                                 offset);
        if (VTI (bb)->locs[i].type == LT_PARAM
            || VTI (bb)->locs[i].type == LT_SET_DEST)
          {
            /* The variable is no longer in any register.  */
            for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
              attrs_list_delete (&out[j], decl, offset);
            attrs_list_insert (&out[MEMORY_VAR], decl, offset, loc);
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
    changed = attrs_list_different_alias (old_out[MEMORY_VAR],
                                        out[MEMORY_VAR]);
  attrs_list_clear (old_out);
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
      for (i = 0; i < IN_OUT_SIZE; i++)
      attrs_list_clear (&VTI (bb)->in[i]);

      /* Calculate the union of predecessor outs.  */
      for (e = bb->pred; e; e = e->pred_next)
      {
        attrs_list *bb_in = VTI (bb)->in;
        attrs_list *pred_out = VTI (e->src)->out;
        attrs_list l;
       
        if (e->src == ENTRY_BLOCK_PTR)
          continue;

        for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
          {
            for (l = pred_out[i]; l; l = l->next)
              {
                if (!attrs_list_member (bb_in[i], l->decl, l->offset))
                  attrs_list_insert (&bb_in[i], l->decl, l->offset, l->loc);
              }
          }
        for (l = pred_out[MEMORY_VAR]; l; l = l->next)
          {
            if (!attrs_list_member_alias (bb_in[MEMORY_VAR],
                                          MEM_ALIAS_SET (l->loc), l->offset))
              attrs_list_insert (&bb_in[MEMORY_VAR], l->decl, l->offset,
                                 l->loc);
          }
      }

      if (0 && rtl_dump_file)
      fprintf (rtl_dump_file, "Clearing pending %d\n", bb->index);

      RESET_BIT (pending, bb->index);
      changed = compute_bb_dataflow (bb);
      if (changed)
      {
        if (0 && rtl_dump_file)
          fprintf (rtl_dump_file, "BB %d changed\n", bb->index);
        for (e = bb->succ; e != 0; e = e->succ_next)
          {
            if (e->dest == EXIT_BLOCK_PTR || e->dest->index == bb->index)
              continue;
            if (0 && rtl_dump_file)
              fprintf (rtl_dump_file, "Setting  pending %d\n", e->dest->index);
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
   BB_ORDER is the order to iterate in (Should map block numbers-> order).
   Because this is a forward dataflow problem we pass in a mapping of block
   number to rc_order (like df->inverse_rc_map).  */

static void
iterative_dataflow (bb_order)
     int *bb_order;
{
  int i;
  fibheap_t worklist;
  basic_block bb;
  sbitmap visited, pending;

  pending = sbitmap_alloc (n_basic_blocks);
  visited = sbitmap_alloc (n_basic_blocks);
  sbitmap_zero (pending);
  sbitmap_zero (visited);
  worklist = fibheap_new ();

  for (i = 0; i < n_basic_blocks; i++)
    {
      fibheap_insert (worklist, bb_order[i], (void *) (size_t) i);
      SET_BIT (pending, i);
    }
  while (sbitmap_first_set_bit (pending) != -1)
    {
      if (0 && rtl_dump_file)
      fprintf (rtl_dump_file, "Iterative dataflow\n");
      while (!fibheap_empty (worklist))
      {
        i = (size_t) fibheap_extract_min (worklist);
        bb = BASIC_BLOCK (i);
        if (!TEST_BIT (visited, bb->index))
          hybrid_search (bb, visited, pending);
      }
      if (sbitmap_first_set_bit (pending) != -1)
      {
        for (i = 0; i < n_basic_blocks; i++)
          {
            fibheap_insert (worklist, bb_order[i], (void *) (size_t) i);
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

/* Print the IN and OUT sets for each basic block to dump file.  */

static void
dump_attrs_list_sets ()
{
  int i, j;
  basic_block bb;

  for (i = 0; i < n_basic_blocks; i++)
    {
      bb = BASIC_BLOCK (i);
      fprintf (rtl_dump_file, "\nBasic block %d:\n", i);

      fprintf (rtl_dump_file, "  IN: ");
      for (j = 0; j < IN_OUT_SIZE; j++)
      {
        attrs_list l = VTI (bb)->in[j];

        if (l)
          {
            if (j < FIRST_PSEUDO_REGISTER)
              fprintf (rtl_dump_file, "Reg %d:", j);
            else
              fprintf (rtl_dump_file, "Mem:");
            for (; l; l = l->next)
              {
                print_mem_expr (rtl_dump_file, l->decl);
                fprintf (rtl_dump_file, "[%d], ", l->offset);
              }
            fprintf (rtl_dump_file, "; ");
          }
      }
      fprintf (rtl_dump_file, "\n");

      fprintf (rtl_dump_file, "  OUT: ");
      for (j = 0; j < IN_OUT_SIZE; j++)
      {
        attrs_list l = VTI (bb)->out[j];

        if (l)
          {
            if (j < FIRST_PSEUDO_REGISTER)
              fprintf (rtl_dump_file, "Reg %d:", j);
            else
              fprintf (rtl_dump_file, "Mem:");
            for (; l; l = l->next)
              {
                print_mem_expr (rtl_dump_file, l->decl);
                fprintf (rtl_dump_file, "[%d], ", l->offset);
              }
            fprintf (rtl_dump_file, "; ");
          }
      }
      fprintf (rtl_dump_file, "\n");
    }
}

/* Computes the hash value for DECL.  */
#define HASH_VAL(decl) ((size_t) decl)

/* The hash function for variable_htab, computes the hash value
   of the variable X declaration.  */

static hashval_t
variable_htab_hash (x)
     const void *x;
{
  variable v = (variable) x;

  return ((size_t) v->decl);
}

/* Compare the declaration of variable X with declaration Y.  */

static int
variable_htab_eq (x, y)
     const void *x;
     const void *y;
{
  variable v = (variable) x;
  tree decl = (tree) y;

  return (v->decl == decl);
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
  else
    {
      int i;
      rtx argp[MAX_LOC];
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
  variable var = htab_find_with_hash (variable_htab, decl, HASH_VAL (decl));
  if (!var)
    {
      void **slot;
      /* Create new variable information.  */
      var = xmalloc (sizeof (*var));
      var->decl = decl;
      var->n_location_parts = 0;

      slot = htab_find_slot_with_hash (variable_htab, decl, HASH_VAL (decl),
                                     INSERT);
      if (*slot)
      abort ();
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
      var->n_location_parts++;
      var->location_part[k].offset = offset;
      var->location_part[k].loc = NULL;
    }
  var->location_part[k].delete_p = false;
  if (var->location_part[k].loc != loc)
    {
      emit_note_data emit_note_data;

      var->location_part[k].loc = loc;
      var->changed = true;
      emit_note_data.insn = insn;
      emit_note_data.where = where;
      emit_note_if_var_changed ((void **) &var, &emit_note_data);
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
  void **slot = htab_find_slot_with_hash (variable_htab, decl, HASH_VAL (decl),
                                        NO_INSERT);
  if (slot && *slot)
    {
      variable var = *slot;
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

/* Emit the notes for changes of location parts of a variable in the basic
   block BB.  */

static void
process_bb (bb)
     basic_block bb;
{
  int i, n;
  attrs_list attrs[IN_OUT_SIZE];

  init_attrs_list_set (attrs);
  for (i = 0; i < IN_OUT_SIZE; i++)
    attrs_list_copy (&attrs[i], VTI (bb)->in[i]);
 
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
              for (l = attrs[REGNO (loc)]; l; l = l->next)
                if (l->decl != decl || l->offset != offset)
                  delete_location_part (l->decl, l->offset, insn, where);
              attrs_list_clear (&attrs[REGNO (loc)]);
              if (decl)
                {
                  set_location_part (decl, offset, loc, insn, where);
                  attrs_list_insert (&attrs[REGNO (loc)], decl, offset, loc);
                }
              break;

            case LT_CLOBBERED:
              for (l = attrs[REGNO (loc)]; l; l = l->next)
                delete_location_part (l->decl, l->offset, insn,
                                      EMIT_NOTE_AFTER_INSN);
              attrs_list_clear (&attrs[REGNO (loc)]);
              break;
          }
      }
      else if (GET_CODE (loc) == MEM && MEM_EXPR (loc))
      {
        attrs_list l;
        tree decl = MEM_EXPR (loc);
        HOST_WIDE_INT alias = MEM_ALIAS_SET (loc);
        HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;
        enum where_emit_note where = EMIT_NOTE_AFTER_INSN;

        switch (VTI (bb)->locs[i].type)
          {
            case LT_PARAM:
              where = EMIT_NOTE_BEFORE_INSN;
            case LT_SET_DEST:
              for (l = attrs[MEMORY_VAR]; l; l = l->next)
                if (MEM_ALIAS_SET (l->loc) == alias && l->offset == offset
                    && l->decl != decl)
                  delete_location_part (l->decl, l->offset, insn, where);
              attrs_list_delete_alias (&attrs[MEMORY_VAR], alias, offset);
              set_location_part (decl, offset, loc, insn, where);
              attrs_list_insert (&attrs[MEMORY_VAR], decl, offset, loc);
              break;

            case LT_CLOBBERED:
              for (l = attrs[MEMORY_VAR]; l; l = l->next)
                if (MEM_ALIAS_SET (l->loc) == alias && l->offset == offset)
                  delete_location_part (l->decl, l->offset, insn,
                                        EMIT_NOTE_AFTER_INSN);
              attrs_list_delete_alias (&attrs[MEMORY_VAR], alias, offset);
              break;
          }
      }
    }
}

/* Emit notes for the whole function.  */

static void
var_tracking_emit_notes ()
{
  int i, j;
  attrs_list *last_out, *in;
  attrs_list empty[IN_OUT_SIZE];

  init_attrs_list_set (empty);
  last_out = empty;

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
      attrs_list l;
      emit_note_data emit_note_data;

      /* Emit the notes for changes of variable locations between two
       sequential basic blocks.  */
      /* Mark the variables of previous OUT set.  */
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
      for (l = last_out[j]; l; l = l->next)
        {
          variable var = htab_find_with_hash (variable_htab, l->decl,
                                              HASH_VAL (l->decl));
          if (var)
            {
              int k;
              for (k = 0; k < var->n_location_parts; k++)
                if (var->location_part[k].offset == last_out[j]->offset)
                  {
                    var->location_part[k].delete_p = true;
                    break;
                  }
            }
        }
      /* Add the variables of IN set and unmark them.  */
      in = VTI (bb)->in;
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
      for (l = in[j]; l; l = l->next)
        {
          set_location_part (l->decl, l->offset, l->loc, bb->head,
                             EMIT_NOTE_AFTER_INSN);
        }
      /* Emit the notes and delete the marked location parts.  */
      emit_note_data.insn = bb->head;
      emit_note_data.where = EMIT_NOTE_AFTER_INSN;
      htab_traverse (variable_htab, process_location_parts, &emit_note_data);

      /* Emit the notes for the changes in the basic block itself.  */
      process_bb (bb);

      last_out = VTI (bb)->out;
    }
}

/* Allocate and initialize the data structures for variable tracking
   and parse the RTL to get the REG and MEM references.  */

static void
var_tracking_initialize ()
{
  int i;
  scan_for_locations_data data;

  alloc_aux_for_blocks (sizeof (struct var_tracking_info_def));

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
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
    }

  variable_htab = htab_create (37, variable_htab_hash, variable_htab_eq,
                             free);
}

/* Free the data structures needed for variable tracking.  */

static void
var_tracking_finalize ()
{
  int i, j;

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      free (VTI (bb)->locs);

      for (j = 0; j < IN_OUT_SIZE; j++)
      {
        attrs_list_clear (&VTI (bb)->in[j]);
        attrs_list_clear (&VTI (bb)->out[j]);
      }
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
  bb_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  if (!rc_order || !bb_order)
    abort ();
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
