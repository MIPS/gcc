/* RTL dead store elimination.
   Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.

   Contributed by Richard Sandiford <rsandifor@codesourcery.com>
   and Kenneth Zadeck <zadeck@naturalbridge.com>

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
#include "hashtab.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "df.h"
#include "cselib.h"
#include "timevar.h"
#include "tree-pass.h"
#include "alloc-pool.h"
#include "alias.h"
#include "dse.h"
#include "dbgcnt.h"

/* This file contains three techniques for performing Dead Store
   Elimination (dse).  

   * The first technique performs dse locally on any base address.  It
   is based on the cselib which is a local value numbering technique.
   This technique is local to a basic block but deals with a fairly
   general addresses.
 
   * The second technique performs dse globally but is restricted to
   base addresses that are either constant or are relative to the
   frame_pointer.

   * The third technique, (which is only done after register allocation)
   processes the spill spill slots.  This differs from the second
   technique because it takes advantage of the fact that spilling is
   completely free from the effects of aliasing.

   Logically, dse is a backwards dataflow problem.  A store can be
   deleted if it if cannot be reached in the backward direction by any
   use of the value being stored.  However, the local technique uses a
   forwards scan of the basic block because cselib requires that the
   block be processed in that order.

   The pass is logically broken into 7 steps:

   0) Initialization.

   1) The local algorithm, as well as scanning the insns for the two
   global algorithms.

   2) Analysis to see if the global algs are necessary.  In the case
   of stores base on a constant address, there must be at least two
   stores to that address, to make it possible to delete some of the
   stores.  In the case of stores off of the frame or spill related
   stores, only one store to an address is necessary because those
   stores die at the end of the function.

   3) Set up the global dataflow equations based on processing the 
   info parsed in the first step.

   4) Solve the dataflow equations.

   5) Delete the insns that the global analysis has indicated are
   unnecessary.

   6) Cleanup.

   This step uses cselib and canon_rtx to build the largest expression
   possible for each address.  This pass is a forwards pass through
   each basic block.  From the point of view of the global technique,
   the first pass could examine a block in either direction.  The
   forwards ordering is to accomodate cselib.

   We a simplifying assumption: addresses fall into four broad
   categories:

   1) base has rtx_varies_p == false, offset is constant.
   2) base has rtx_varies_p == false, offset variable.
   3) base has rtx_varies_p == true, offset constant.
   4) base has rtx_varies_p == true, offset variable.

   The local passes are able to process all 4 kinds of addresses.  The
   global pass only handles (1).

   The global problem is formulated as follows:

     A store, S1, to address A, where A is not relative to the stack
     frame, can be eliminated if all paths from S1 to the end of the
     of the function contain another store to A before a read to A.

     If the address A is relative to the stack frame, a store S2 to A
     can be eliminated if there are no paths from S1 that reach the
     end of the function that read A before another store to A.  In
     this case S2 can be deleted if there are paths to from S2 to the
     end of the function that have no reads or writes to A.  This
     second case allows stores to the stack frame to be deleted that
     would otherwise die when the function returns.  This cannot be
     done if stores_off_frame_dead_at_return is not true.  See the doc
     for that variable for when this variable is false.

     The global problem is formulated as a backwards set union
     dataflow problem where the stores are the gens and reads are the
     kills.  Set union problems are rare and require some special
     handling given our representation of bitmaps.  A straightforward
     implementation of requires a lot of bitmaps filled with 1s.
     These are expensive and cumbersome in our bitmap formulation so
     care has been taken to avoid large vectors filled with 1s.  See
     the comments in bb_info and in the dataflow confluence functions
     for details.  

   There are two places for further enhancements to this algorithm:
   
   1) The original dse which was embedded in a pass called flow also
   did local address forwarding.  For example in

   A <- r100
   ... <- A

   flow would replace the right hand side of the second insn with a
   reference to r100.  Most of the infomation is available to add this
   to this pass.  It has not done it because it is a lot of work in
   the case that either r100 is assigned to between the first and
   second insn and/or the second insn is a load of part of the value
   stored by the first insn.

   2) The cleaning up of spill code is quite profitable.  It currently
   depends on reading tea leaves and chicken entrails left by reload.
   This pass depends on reload creating a singleton alias set for each
   spill slot and telling the next dse pass which of these alias sets
   are the singletons.  Rather that analyze the addresses of the
   spills, dse's spill processing just does analysis of the loads and
   stores that use those alias sets.  There are three cases where this
   falls short:

     a) Reload sometimes creates the slot for one mode of access, and
     then inserts loads and/or stores for a smaller mode.  In this
     case, the current code just punts on the slot.  The proper thing
     to do is to back out and use one bit vector position for each
     byte of the entity associated with the slot.  This depends on
     KNOWING that reload always generates the accesses for each of the
     bytes in some canonical (read that easy to understand several
     passes after reload happens) way.

     b) Reload sometimes decides that spill slot it allocated was not
     large enough for the mode and goes back and allocates more slots
     with the same mode and alias set.  The backout in this case is a
     little more graceful than (a).  In this case the slot is unmarked
     as being a spill slot and if final address comes out to be based
     off the frame pointer, the global algorithm handles this slot.  

     c) For any pass that may prespill, there is currently no
     mechanism to tell the dse pass that the slot being used has the
     special properties that reload uses.  It may be that all that is
     requirred is to have those passes make the same calls that reload
     does, assuming that the alias sets can be manipulated in the same
     way.  
*/

#undef BASELINE

/* There are limits to the size of constant offsets we model for the
   global problem.  There are certainly test cases, that exceed this
   limit, however, it is unlikely that there are important programs
   that really have constant offsets this size.  */
#define MAX_OFFSET (64 * 1024)


static bitmap scratch = NULL;
struct insn_info;

/* This structure holds information about a candidate store.  */
struct store_info {
  /* The id of the mem group of the base address.  If rtx_varies_p is
     true, this is -1.  Otherwise, it is the index into the group
     table.  */
  int group_id;
  
  /* This is the cselib value.  */
  cselib_val *cse_base;

  /* This original mem.  */
  rtx mem;

  /* This canonized address of mem.  */
  rtx address;

  /* If this is non-zero, it is the alias set of a spill location.  */
  HOST_WIDE_INT alias_set;

  /* The offset of the first and byte before the last byte associated
     with the operation.  */
  HOST_WIDE_INT begin, end;

  /* An sbitmap as wide as the number of bytes in the word that
     contains a 1 if the byte may be needed.  The store is unused if
     all of the bits are 0.  */
  sbitmap positions_needed;

  /* The next store info for this insn.  */
  struct store_info *next;
};

typedef struct store_info *store_info_t;
static alloc_pool cse_store_info_pool;
static alloc_pool rtx_store_info_pool;

/* This structure holds information about a load.  These are only
   built for rtx bases.  */
struct read_info {
  /* The id of the mem group of the base address.  */
  int group_id;

  /* If this is non-zero, it is the alias set of a spill location.  */
  HOST_WIDE_INT alias_set;

  /* The offset of the first and byte after the last byte associated
     with the operation.  If begin == end == 0, the read did not have
     a constant offset.  */
  HOST_WIDE_INT begin, end;

  /* The mem being read.  */
  rtx mem;

  /* The next read_info for this insn.  */
  struct read_info *next;
};
typedef struct read_info *read_info_t;
static alloc_pool read_info_pool;


/* One of these records is created for each insn.  */

struct insn_info {
  /* Set true if the insn contains a store but the insn itself cannot
     be deleted.  This is set if the insn is a parallel and there is
     more than one non dead output or if the insn is in some way
     volatile.  */
  bool cannot_delete;

  /* This field is only used by the global algorithm.  It is set true
     if the insn contains any read of mem except for a (1).  This is
     also set if the insn is a call or has a clobber mem.  If the insn
     contains a wild read, the use_rec will be null.  */
  bool wild_read;

  /* This is true if any of the sets within the store contains a
     cselib base.  Such stores can only be deleted by the local
     algorithm.  */
  bool contains_cselib_groups;

  /* The insn. */
  rtx insn;

  /* Since we only delete insns with a single mem store, this is a
     pointer to the parsed representation of that store(s).  */
  store_info_t store_rec;

  /* A pointer to the linked list of mem uses in this insn.  Only the
     reads from rtx bases are listed here.  The reads to cselib bases
     are completely processed during the first scan and so are never
     created.  */
  read_info_t read_rec;

  /* The prev insn in the basic block.  */
  struct insn_info * prev_insn;

  /* The linked list of insns that are in consideration for removal in
     the forwards pass thru the basic block.  This pointer may be
     trash as it is not cleared when a wild read occurs.  The only
     time it is guaranteed to be correct is when the traveral starts
     at active_local_stores.  */
  struct insn_info * next_local_store;
};

typedef struct insn_info *insn_info_t;
static alloc_pool insn_info_pool;

/* The linked list of stores that are under consideration in this
   basic block.  */   
static insn_info_t active_local_stores;

struct bb_info {

  /* Pointer to the insn info for the last insn in the block.  These
     are linked so this is how all of the insns are reached.  During
     scanning this is the current insn being scanned.  */
  insn_info_t last_insn;

  /* The info for the global dataflow problem.  */


  /* This is set if the transfer function should and in the wild_read
     bitmap before applying the kill and gen sets.  That vector knocks
     out most of the bits in the bitmap and thus speeds up the
     operations.  */
  bool apply_wild_read;

  /* The set of store positions that exist in this block before a wild read.  */
  bitmap gen;
  
  /* The set of load positions that exist in this block above the
     same position of a store.  */
  bitmap kill;

  /* The set of stores that reach the top of the block without being
     killed by a read.

     Do not represent the in if it is all ones.  Note that this is
     what the bitvector should logically be initialized to for a set
     intersection problem.  However, like the kill set, this is too
     expensive.  So initially, the in set will only be created for the
     exit block and any block that contains a wild read.  */
  bitmap in;

  /* The set of stores that reach the bottom of the block from it's
     successors.

     Do not represent the in if it is all ones.  Note that this is
     what the bitvector should logically be initialized to for a set
     intersection problem.  However, like the kill and in set, this is
     too expensive.  So what is done is that the confluence operator
     just initializes the vector from one of the out sets of the
     successors of the block.  */
  bitmap out;
};

typedef struct bb_info *bb_info_t;
static alloc_pool bb_info_pool;

/* Table to hold all bb_infos.  */
static bb_info_t *bb_table;

/* There is a group_info for each rtx base that is used to reference
   memory.  There are also not many of the rtx bases because they are
   very limited in scope.  */

struct group_info {
  /* The actual base of the address.  */
  rtx rtx_base;

  /* The sequential id of the base.  This allows us to have a
     canonical ordering of these that is not based on addresses.  */
  int id;

  /* A mem wrapped around the base pointer for the group in order to
     do read dependency.  */
  rtx base_mem;
  
  /* Canonized version of base_mem, most likely the same thing.  */
  rtx canon_base_mem;

  /* These two sets of two bitmaps are used to keep track of how many
     stores are actually referencing that postion from this base.  We
     only do this for rtx bases as this will be used to assign
     postions in the bitmaps for the global problem.  Bit N is set in
     store1 on the first store for offset N.  Bit N is set in store2
     for the second store to offset N.  This is all we need since we
     only care about offsets that have two or more stores for them.

     The "_n" suffix is for offsets less than 0 and the "_p" suffix is
     for 0 and greater offsets.

     There is one special case here, for stores into the stack frame,
     we will or store1 into store2 before deciding which stores look
     at globally.  This is because stores to the stack frame that have
     no other reads before the end of the function can also be
     deleted.  */
  bitmap store1_n, store1_p, store2_n, store2_p;

  /* The postions in this bitmap have the same assignments as the in,
     out, gen and kill bitmaps.  This bitmap is all zeros except for
     the postions that are occupied by stores for this group.  */
  bitmap group_kill;

  /* True if there are any positions that are to be processed
     globally.  */
  bool process_globally;

  /* The offset_map is used to map the offsets from this base into
     postions in the global bitmaps.  It is only created after all of
     the all of stores have been scanned and we know which ones we
     care about.  */
  int *offset_map_n, *offset_map_p; 
  int offset_map_size_n, offset_map_size_p; 
};
typedef struct group_info *group_info_t;
static alloc_pool rtx_group_info_pool;

/* Tables of group_info structures, hashed by base value.  */
static htab_t rtx_group_table;

/* Index into the rtx_group_vec.  */
static int rtx_group_next_id;

DEF_VEC_P(group_info_t);
DEF_VEC_ALLOC_P(group_info_t,heap);

static VEC(group_info_t,heap) *rtx_group_vec;

/* This are used to hold the alias sets of spill variables.  Since
   these are never aliased and there may be a lot of them, it makes
   sense to treat them specially.  This bitvector is only allocated in
   calls from dse_record_singleton_alias_set which currently is only
   made during reload1.  So when dse is called before reload this
   mechanism does nothing.  */

static bitmap clear_alias_sets = NULL;

/* The set of clear_alias_sets that have been disqualified because
   there are loads or stores using a different mode than the alias set
   was registered with.  */ 
static bitmap disqualified_clear_alias_sets = NULL;

/* The group that holds all of the clear_alias_sets.  */
static group_info_t clear_alias_group;

/* The modes of the clear_alias_sets.  */
static htab_t clear_alias_mode_table;

/* Hash table element to look up the mode for an alias set.  */
struct clear_alias_mode_holder
{
  HOST_WIDE_INT alias_set;
  enum machine_mode mode;
};

static alloc_pool clear_alias_mode_pool;

/* This is true except for two cases:
   (1) current_function_stdarg -- i.e. we cannot do this 
       for vararg functions because they play games with the frame.  
   (2) In ada, it is sometimes not safe to do assume that any stores
       based off the stack frame go dead at the exit to a function.  */
static bool stores_off_frame_dead_at_return;

/* Counter for stats.  */
static int globally_deleted; 
static int locally_deleted; 
static int spill_deleted; 
      
static bitmap all_blocks;

/* The number of bits used in the global bitmaps.  */
static unsigned int current_position;


static bool gate_dse (void);


/*----------------------------------------------------------------------------
   Zeroth step.

   Initialization.  
----------------------------------------------------------------------------*/

/* Hashtable callbacks for maintaining the "bases" field of
   store_group_info, given that the addresses are function invariants.  */

static int
clear_alias_mode_eq (const void *p1, const void *p2)
{
  const struct clear_alias_mode_holder * h1 
    = (const struct clear_alias_mode_holder *) p1;
  const struct clear_alias_mode_holder * h2 
    = (const struct clear_alias_mode_holder *) p2;
  return h1->alias_set == h2->alias_set;
}


static hashval_t
clear_alias_mode_hash (const void *p)
{
  const struct clear_alias_mode_holder *holder 
    = (const struct clear_alias_mode_holder *) p;
  return holder->alias_set;
}


/* Hashtable callbacks for maintaining the "bases" field of
   store_group_info, given that the addresses are function invariants.  */

static int
invariant_group_base_eq (const void *p1, const void *p2)
{
  const group_info_t gi1 = (const group_info_t) p1;
  const group_info_t gi2 = (const group_info_t) p2;
  return rtx_equal_p (gi1->rtx_base, gi2->rtx_base);
}


static hashval_t
invariant_group_base_hash (const void *p)
{
  const group_info_t gi = (const group_info_t) p;
  int do_not_record;
  return hash_rtx (gi->rtx_base, Pmode, &do_not_record, NULL, false);
}


/* Get the GROUP for BASE.  Add a new group if it is not there.  */

static group_info_t
get_group_info (rtx base)
{
  struct group_info tmp_gi; 
  group_info_t gi; 
  void **slot;

  if (base)
    {
      /* Find the store_base_info structure for BASE, creating a new one
	 if necessary.  */
      tmp_gi.rtx_base = base;
      slot = htab_find_slot (rtx_group_table, &tmp_gi, INSERT);
      gi = (group_info_t) *slot;
    }
  else
    {
      if (!clear_alias_group)
	{
	  clear_alias_group = gi = pool_alloc (rtx_group_info_pool);
	  memset (gi, 0, sizeof (struct group_info));
	  gi->id = rtx_group_next_id++;
	  gi->store1_n = BITMAP_ALLOC (NULL);
	  gi->store1_p = BITMAP_ALLOC (NULL);
	  gi->store2_n = BITMAP_ALLOC (NULL);
	  gi->store2_p = BITMAP_ALLOC (NULL);
	  gi->group_kill = BITMAP_ALLOC (NULL);
	  gi->process_globally = false;
	  gi->offset_map_size_n = 0;
	  gi->offset_map_size_p = 0;
	  gi->offset_map_n = NULL;
	  gi->offset_map_p = NULL;
	  VEC_safe_push (group_info_t, heap, rtx_group_vec, gi);
	}
      return clear_alias_group;
    }

  if (gi == NULL)
    {
      *slot = gi = pool_alloc (rtx_group_info_pool);
      gi->rtx_base = base;
      gi->id = rtx_group_next_id++;
      gi->base_mem = gen_rtx_MEM (QImode, base);
      gi->canon_base_mem = canon_rtx (gi->base_mem);
      gi->store1_n = BITMAP_ALLOC (NULL);
      gi->store1_p = BITMAP_ALLOC (NULL);
      gi->store2_n = BITMAP_ALLOC (NULL);
      gi->store2_p = BITMAP_ALLOC (NULL);
      gi->group_kill = BITMAP_ALLOC (NULL);
      gi->process_globally = false;
      gi->offset_map_size_n = 0;
      gi->offset_map_size_p = 0;
      gi->offset_map_n = NULL;
      gi->offset_map_p = NULL;
      VEC_safe_push (group_info_t, heap, rtx_group_vec, gi);
    }

  return gi;
}


/* Initialization of data structures.  */

static void
step0 (void)
{
  locally_deleted = 0;
  globally_deleted = 0;
  spill_deleted = 0;

  scratch = BITMAP_ALLOC (NULL);

  rtx_store_info_pool
    = create_alloc_pool ("rtx_store_info_pool", 
			 sizeof (struct store_info), 100);
  read_info_pool
    = create_alloc_pool ("read_info_pool", 
			 sizeof (struct read_info), 100);
  insn_info_pool
    = create_alloc_pool ("insn_info_pool", 
			 sizeof (struct insn_info), 100);
  bb_info_pool
    = create_alloc_pool ("bb_info_pool", 
			 sizeof (struct bb_info), 100);
  rtx_group_info_pool
    = create_alloc_pool ("rtx_group_info_pool", 
			 sizeof (struct group_info), 100);

  rtx_group_table = htab_create (11, invariant_group_base_hash,
				 invariant_group_base_eq, NULL);

  bb_table = XCNEWVEC (bb_info_t, last_basic_block);
  rtx_group_next_id = 0;

  stores_off_frame_dead_at_return = 
    (!(TREE_CODE (TREE_TYPE (current_function_decl)) == FUNCTION_TYPE
       && (TYPE_RETURNS_STACK_DEPRESSED (TREE_TYPE (current_function_decl)))))
    && (!current_function_stdarg);

  init_alias_analysis ();
  
  if (clear_alias_sets)
    clear_alias_group = get_group_info (NULL);
  else
    clear_alias_group = NULL;
}



/*----------------------------------------------------------------------------
   First step.

   Scan all of the insns.  Any random ordering of the blocks is fine.
   Each block is scanned in forward order to accomodate cselib which
   is used to remove stores with non-constant bases.
----------------------------------------------------------------------------*/

/* Delete all of the store_info recs from INSN_INFO.  */

static void 
free_store_info (insn_info_t insn_info)
{
  store_info_t store_info = insn_info->store_rec;
  while (store_info)
    {
      store_info_t next = store_info->next;
      sbitmap_free (store_info->positions_needed);
      if (store_info->cse_base)
	pool_free (cse_store_info_pool, store_info);
      else
	pool_free (rtx_store_info_pool, store_info);
      store_info = next;
    }

  insn_info->cannot_delete = true;
  insn_info->contains_cselib_groups = false;
  insn_info->store_rec = NULL;
}


struct insn_size {
  int size;
  rtx insn;
};


/* Add an insn to do the add inside a x if it is a
   PRE/POST-INC/DEC/MODIFY.  D is an structure containing the insn and
   the size of the mode of the MEM that this is inside of.  */

static int
replace_inc_dec (rtx *r, void *d)
{
  rtx x = *r;
  struct insn_size *data = (struct insn_size *)d;
  switch (GET_CODE (x))
    {
    case PRE_INC:
    case POST_INC:
      {
	rtx r1 = XEXP (x, 0);
	rtx c = gen_int_mode (Pmode, data->size);
	add_insn_before (data->insn, 
			 gen_rtx_SET (Pmode, r1, 
				      gen_rtx_PLUS (Pmode, r1, c)),
			 NULL);
	return -1;
      }
		 
    case PRE_DEC:
    case POST_DEC:
      {
	rtx r1 = XEXP (x, 0);
	rtx c = gen_int_mode (Pmode, -data->size);
	add_insn_before (data->insn, 
			 gen_rtx_SET (Pmode, r1, 
				      gen_rtx_PLUS (Pmode, r1, c)),
			 NULL);
	return -1;
      }
	
    case PRE_MODIFY:
    case POST_MODIFY:
      {
	/* We can resuse the add because we are about to delete the
	   insn that contained it.  */
	rtx add = XEXP (x, 0);
	rtx r1 = XEXP (add, 0);
	add_insn_before (data->insn, 
			 gen_rtx_SET (Pmode, r1, add), NULL);
	return -1;
      }

    default:
      return 0;
    }
}
			 

/* If X is a MEM, check the address to see if it is PRE/POST-INC/DEC/MODIFY
   and generate an add to replace that.  */

static int
replace_inc_dec_mem (rtx *r, void *d)
{
  rtx x = *r;
  if (GET_CODE (x) == MEM)
    {
      struct insn_size data;

      data.size = GET_MODE_SIZE (GET_MODE (x));
      data.insn = (rtx)d;

      for_each_rtx (&XEXP (x, 0), replace_inc_dec, &data);
	
      return -1;
    }
  return 0;
}

/* Before we delete INSN, make sure that the auto inc/dec, if it is
   there, is split into a separate insn.  */

static void
check_for_inc_dec (rtx insn)
{
  rtx note = find_reg_note (insn, REG_INC, NULL_RTX);
  if (note)
    for_each_rtx (&insn, replace_inc_dec_mem, insn);
}


/* Delete the insn and free all of the fields inside INSN_INFO.  */ 

static void
delete_dead_store_insn (insn_info_t insn_info)
{
  read_info_t read_info;

  if (!dbg_cnt (dse))
    return;

  check_for_inc_dec (insn_info->insn);
  if (dump_file)
    {
      fprintf (dump_file, "Locally deleting insn %d ", 
	       INSN_UID (insn_info->insn));
      if (insn_info->store_rec->alias_set)
	fprintf (dump_file, "alias set %d\n", 
		 (int)insn_info->store_rec->alias_set);
      else
	fprintf (dump_file, "\n");
    }

  free_store_info (insn_info);
  read_info = insn_info->read_rec;
	
  while (read_info)
    {
      read_info_t next = read_info->next;
      pool_free (read_info_pool, read_info);
      read_info = next;
    }
  insn_info->read_rec = NULL;

  delete_insn (insn_info->insn);
  locally_deleted++;
  insn_info->insn = NULL;

  insn_info->wild_read = false;
}


/* Set the store* bitmaps offset_map_size* fields in GROUP based on
   OFFSET and WIDTH.  */

static void
set_usage_bits (group_info_t group, HOST_WIDE_INT offset, HOST_WIDE_INT width)
{
  HOST_WIDE_INT i;

  if ((offset > -MAX_OFFSET) && (offset < MAX_OFFSET))
    for (i=offset; i<offset+width; i++)
      {
	bitmap store1;
	bitmap store2;
	int ai;
	if (i < 0)
	  {
	    store1 = group->store1_n;
	    store2 = group->store2_n;
	    ai = -i;
	  }
	else
	  {
	    store1 = group->store1_p;
	    store2 = group->store2_p;
	    ai = i;
	  }
	
	if (bitmap_bit_p (store1, ai))
	  bitmap_set_bit (store2, ai);
	else 
	  {
	    bitmap_set_bit (store1, ai);
	    if (i < 0)
	      {
		if (group->offset_map_size_n < ai)
		  group->offset_map_size_n = ai;
	      }
	    else
	      {
		if (group->offset_map_size_p < ai)
		  group->offset_map_size_p = ai;
	      }
	  }
      }
}


/* Set the BB_INFO so that the last insn is marked as a wild read.  */

static void
add_wild_read (bb_info_t bb_info)
{
  insn_info_t insn_info = bb_info->last_insn;
  read_info_t ptr = insn_info->read_rec;

  while (ptr)
    {
      read_info_t next = ptr->next;
      pool_free (read_info_pool, ptr);
      ptr = next;
    }
  insn_info->read_rec = NULL;
  insn_info->wild_read = true;
  active_local_stores = NULL;
}


/* Take all reasonable action to put the address of MEM into the form 
   that we can do analysis on.  

   The gold standard is to get the address into the form: ADDRESS +
   OFFSET where ADDRESS is something that rtx_varies_p considers a
   constant.  When we can get the address in this form, we can do
   global analysis on it.  

   If that fails, we try cselib to get a value we can at least use
   locally.  If that fails we return false.  
   
   The GROUP_ID is set to -1 for cselib bases and the index of the
   group for non_varying bases.

   FOR_READ is true if this is a mem read and false if not.  */

static bool
canon_address (bool for_read, bb_info_t bb_info, rtx mem,
	       HOST_WIDE_INT *alias_set_out,
	       rtx *address_out, int *group_id,
	       HOST_WIDE_INT *offset, 
	       cselib_val **base)
{
  rtx mem_address = XEXP (mem, 0);
  rtx expanded_address, simplified_address, address;
  /* Make sure that cselib is has initialized all of the operands of
     the address before asking it to do the subst.  */

  if (clear_alias_sets)
    {
      /* If this is a spill, do not do any further processing.  */
      HOST_WIDE_INT alias_set = MEM_ALIAS_SET (mem);
      if (dump_file)
	fprintf (dump_file, "found alias set %d\n", (int)alias_set);
      if (bitmap_bit_p (clear_alias_sets, alias_set))
	{
	  struct clear_alias_mode_holder tmp_holder;
	  struct clear_alias_mode_holder *entry;
	  void **slot;

	  *alias_set_out = alias_set;
	  *group_id = clear_alias_group->id;

	  tmp_holder.alias_set = alias_set;
	  slot = htab_find_slot (clear_alias_mode_table, &tmp_holder, NO_INSERT);
	  gcc_assert (*slot);

	  entry = *slot;
	  /* If the modes do not match, we cannot process this set.  */
	  if (entry->mode != GET_MODE (mem))
	    {
	      if (dump_file)
		fprintf (dump_file, "disqualifying alias set %d\n", (int)alias_set);
	      
	      bitmap_set_bit (disqualified_clear_alias_sets, alias_set);
	      return false;
	    }
	  return true;
	}
    }

  *alias_set_out = 0;

  cselib_lookup (mem_address, Pmode, 1);

  /* Use cselib to replace all of the reg references with the full
     expression.  This will take care of the case where we have 

     r_x = base + offset;
     val = *r_x;
   
     by making it into 

     val = *(base + offset);  
  */

  expanded_address = cselib_expand_value_rtx (mem_address, scratch);

  /* If this fails, just go with the mem_address.  */
  if (!expanded_address)
    expanded_address = mem_address;
  
  /* Do whatever simplifications we can over the resulting expression.  */
  simplified_address = simplify_rtx (expanded_address);
  if (!simplified_address)
    simplified_address = expanded_address;

  /* Split the address into canonical BASE + OFFSET terms.  */
  address = canon_rtx (simplified_address);
  *address_out = address;

  *offset = 0;

  if (dump_file)
    {
      fprintf (dump_file, "  mem: ");
      print_inline_rtx (dump_file, mem, 0);

      fprintf (dump_file, "\n   address: ");
      print_inline_rtx (dump_file, address, 0);
      fprintf (dump_file, "\n");
    }

  if (GET_CODE (address) == CONST)
    address = XEXP (address, 0);

  if (GET_CODE (address) == PLUS && GET_CODE (XEXP (address, 1)) == CONST_INT)
    {
      *offset = INTVAL (XEXP (address, 1));
      address = XEXP (address, 0);
    }

  if (rtx_varies_p (address, false))
    {
      *base = cselib_lookup (address, Pmode, true);
      *group_id = -1;
      if (*base == NULL)
	{
	  if (for_read)
	    {
	      add_wild_read (bb_info);
	      if (dump_file)
		fprintf (dump_file, " adding wild read, no cselib val.\n");
	    }
	  return false;
	}
      if (dump_file)
	fprintf (dump_file, "  varying cselib base=%d\n", (*base)->value);
    }
  else
    {
      group_info_t group = get_group_info (address);

      if (dump_file)
	fprintf (dump_file, "  const base = %d\n", group->id);
      *base = NULL;
      *group_id = group->id;
    }
  return true;
}


/* BODY is an instruction pattern that belongs to INSN.  Return 1 if
   there is a candidate store, after adding it to the appropriate
   local store group if so.  */

static int
record_store (rtx body, bb_info_t bb_info)
{
  rtx mem;
  rtx address = NULL;
  HOST_WIDE_INT offset = 0;
  HOST_WIDE_INT width = 0;
  HOST_WIDE_INT spill_alias_set;
  insn_info_t insn_info = bb_info->last_insn;
  store_info_t store_info = NULL;
  int group_id;
  cselib_val *base = NULL;
  insn_info_t ptr, last;
  bool store_is_unused;

  if (GET_CODE (body) != SET && GET_CODE (body) != CLOBBER)
    return 0;

  /* If this is not used, then this cannot be used to keep the insn
     from being deleted.  On the other hand, it does provide something
     that can be used to prove that another store is dead.  */
  store_is_unused
    = (find_reg_note (insn_info->insn, REG_UNUSED, body) != NULL);

  /* Check whether that value is a suitable memory location.  */
  mem = SET_DEST (body);
  if (!MEM_P (mem))
    {
      /* If the set or clobber is unused, then it does not effect our
	 ability to get rid of the entire insn.  */
      if (!store_is_unused)
	insn_info->cannot_delete = true;
      return 0;
    }

  /* At this point we know mem is a mem. */
  if (GET_MODE (mem) == BLKmode)
    {
      if (GET_CODE (XEXP (mem, 0)) == SCRATCH)
	{
	  if (dump_file) 
	    fprintf (dump_file, " adding wild read for (clobber (mem:BLK (scratch))\n");
	  add_wild_read (bb_info);
	  insn_info->cannot_delete = true;
	}
      else if (!store_is_unused)
      /* If the set or clobber is unused, then it does not effect our
	 ability to get rid of the entire insn.  */
	insn_info->cannot_delete = true;
      return 0;
    }

  /* We can still process a volatile mem, we just cannot delete it.  */
  if (MEM_VOLATILE_P (mem))
      insn_info->cannot_delete = true;

  if (!canon_address (false, bb_info, mem, 
		      &spill_alias_set, &address, &group_id, &offset, &base))
    return 0;

  width = GET_MODE_SIZE (GET_MODE (mem));

  if (spill_alias_set)
    {
      bitmap store1 = clear_alias_group->store1_p;
      bitmap store2 = clear_alias_group->store2_p;
      
      if (bitmap_bit_p (store1, spill_alias_set))
	bitmap_set_bit (store2, spill_alias_set);
      else 
	bitmap_set_bit (store1, spill_alias_set);
	
      if (clear_alias_group->offset_map_size_p < spill_alias_set)
	clear_alias_group->offset_map_size_p = spill_alias_set;
  
      store_info = pool_alloc (rtx_store_info_pool);

      if (dump_file)
	fprintf (dump_file, " processing spill store %d\n",
		 (int)spill_alias_set);
    }
  else if (group_id >= 0)
    {
      /* In the restrictive case where the base is a constant or the
	 frame pointer we can do global analysis.  */
      
      group_info_t group 
	= VEC_index (group_info_t, rtx_group_vec, group_id);
      
      store_info = pool_alloc (rtx_store_info_pool);
      set_usage_bits (group, offset, width);

      if (dump_file)
	fprintf (dump_file, " processing const base store %d[%d..%d)\n",
		 group_id, (int)offset, (int)(offset+width));


    }
  else
    {
      store_info = pool_alloc (cse_store_info_pool);
      insn_info->contains_cselib_groups = true;
      group_id = -1;

      if (dump_file)
	fprintf (dump_file, " processing cselib store [%d..%d)\n",
		 (int)offset, (int)(offset+width));

    }

  /* Check to see if this stores causes some other stores to be
     dead.  */
  ptr = active_local_stores;
  last = NULL;

  while (ptr)
    {
      insn_info_t next = ptr->next_local_store;
      store_info_t s_info = ptr->store_rec;
      bool delete = true;

      /* Loop thru the store_info's for ptr and see if we can make all
	 of the positions in them dead.  Some positions may already be
	 dead from prior insns or from prior stores in this insn.  */ 
      while (s_info)
	{ 

	  if (s_info->alias_set != spill_alias_set)
	    delete = false;
	  else if (s_info->alias_set)
	    {
	      delete = !bitmap_bit_p (disqualified_clear_alias_sets, spill_alias_set);
	      if (dump_file)
		fprintf (dump_file, "    trying spill store in insn=%d alias_set=%d\n",
			 INSN_UID (ptr->insn), (int)s_info->alias_set);
	    }
	  else if ((s_info->group_id == group_id) 
	      && (s_info->cse_base == base))
	    {
	      HOST_WIDE_INT i;
	      if (dump_file)
		fprintf (dump_file, "    trying store in insn=%d gid=%d[%d..%d)\n",
			 INSN_UID (ptr->insn), s_info->group_id, 
			 (int)s_info->begin, (int)s_info->end);
	      for (i = offset; i < offset+width; i++)
		if (i >= s_info->begin && i < s_info->end)
		  RESET_BIT (s_info->positions_needed, i - s_info->begin);
	    }

	  /* An insn can be deleted if every position of every one of
	     its s_infos is zero.  */
	  if (!sbitmap_empty_p (s_info->positions_needed))
	    delete = false;

	  s_info = s_info->next;
	}

      if (delete)
	{
	  insn_info_t insn_to_delete = ptr;

	  if (last)
	    last->next_local_store = ptr->next_local_store;
	  else
	    active_local_stores = ptr->next_local_store;
	  
	  delete_dead_store_insn (insn_to_delete);
	}
      else
	last = ptr;

      ptr = next;
    }

  /* Finish filling in the store_info.  */
  store_info->next = insn_info->store_rec;
  insn_info->store_rec = store_info;
  store_info->mem = mem;
  store_info->alias_set = spill_alias_set;
  store_info->address = address;
  store_info->cse_base = base;
  store_info->positions_needed = sbitmap_alloc (width);
  sbitmap_ones (store_info->positions_needed);
  store_info->group_id = group_id;
  store_info->begin = offset;
  store_info->end = offset + width;

  return 1;
}


static void
dump_insn_info (const char * start, insn_info_t insn_info)
{
  fprintf (dump_file, "%s insn=%d %s\n", start, 
	   INSN_UID (insn_info->insn),
	   insn_info->store_rec ? "has store" : "naked");
}


/* A for_each_rtx callback in which DATA is the bb_info.  Check to see
   if LOC is a mem and if it is look at the address and kill any
   appropriate stores that may be active.  */

static int
check_mem_read_rtx (rtx *loc, void *data)
{
  rtx mem = *loc;
  rtx address;
  bb_info_t bb_info;
  insn_info_t insn_info;
  HOST_WIDE_INT offset = 0;
  HOST_WIDE_INT width = 0;
  HOST_WIDE_INT spill_alias_set = 0;
  cselib_val *base = NULL;  
  int group_id;
  read_info_t read_info;

  if (!mem || !MEM_P (mem))
    return 0;

  bb_info = (bb_info_t) data;
  insn_info = bb_info->last_insn;

  if (insn_info->wild_read)
    return 0;

  if ((MEM_ALIAS_SET (mem) == ALIAS_SET_MEMORY_BARRIER)
      || (MEM_VOLATILE_P (mem)))
    {
      if (dump_file)
	fprintf (dump_file, " adding wild read, volatile or barrier.\n");
      add_wild_read (bb_info);
      insn_info->cannot_delete = true;
      return 0;
    }

  /* If it is reading readonly mem, then there can be no conflict with
     another write. */
  if (MEM_READONLY_P (mem))
    return 0;

  if (!canon_address (true, bb_info, mem, 
		      &spill_alias_set, &address, &group_id, &offset, &base))
    {
      if (dump_file)
	fprintf (dump_file, " adding wild read, canon_address failure.\n");
      add_wild_read (bb_info);
      return 0;
    }

  if (GET_MODE (mem) == BLKmode)
    width = -1;
  else
    width = GET_MODE_SIZE (GET_MODE (mem));

  read_info = pool_alloc (read_info_pool);
  read_info->group_id = group_id;
  read_info->mem = mem;
  read_info->alias_set = spill_alias_set;
  read_info->begin = offset;
  read_info->end = offset + width;
  read_info->next = insn_info->read_rec;
  insn_info->read_rec = read_info;

  if (spill_alias_set)
    {
      insn_info_t i_ptr = active_local_stores;
      insn_info_t last = NULL;

      if (dump_file)
	fprintf (dump_file, " processing spill load %d\n",
		 (int)spill_alias_set);

      while (i_ptr)
	{
	  int remove = false;
	  store_info_t store_info = i_ptr->store_rec;

	  while (store_info && !remove) 
	    {
	      if (store_info->alias_set == spill_alias_set)
		remove = true;
	      store_info = store_info->next;
	    }	      

	  if (remove)
	    {
	      if (dump_file)
		dump_insn_info ("removing from active", i_ptr);

	      if (last)
		last->next_local_store = i_ptr->next_local_store;
	      else
		active_local_stores = i_ptr->next_local_store;
	    }
	  else
	    last = i_ptr;
	  i_ptr = i_ptr->next_local_store;
	}
    }
  else if (group_id >= 0)
    {
      /* This is the restricted case where the base is a constant or
	 the frame pointer and offset is a constant.  */
      insn_info_t i_ptr = active_local_stores;
      insn_info_t last = NULL;
      
      if (dump_file)
	fprintf (dump_file, " processing const load %d[%d..%d)\n",
		 group_id, (int)offset, (int)(offset+width));

      while (i_ptr)
	{
	  bool remove = false;
	  store_info_t store_info = i_ptr->store_rec;
	  
	  while (store_info && !remove) 
	    {
	      /* There are three cases here.  */
	      if (store_info->group_id < 0)
		/* We have a cselib store followed by a read from a
		   const base. */
		remove 
		  = canon_true_dependence (store_info->mem, 
					   GET_MODE (store_info->mem),
					   store_info->address,
					   mem, rtx_varies_p);

	      else if (group_id == store_info->group_id)
		{
		  /* The bases are the same, just see if the offsets
		     overlap.  */
		  if ((width == -1)
		      || ((offset < store_info->end) 
			  && (offset + width > store_info->begin)))
		    remove = true;
		}
	      
	      /* else 
		 The else case that is missing here is that the
		 bases are constant but different.  There is nothing
		 to do here because there is no overlap.  */
	      store_info = store_info->next;
	    }	      
	  
	  if (remove)
	    {
	      if (dump_file)
		dump_insn_info ("removing from active", i_ptr);

	      if (last)
		last->next_local_store = i_ptr->next_local_store;
	      else
		active_local_stores = i_ptr->next_local_store;
	    }
	  else
	    last = i_ptr;
	  i_ptr = i_ptr->next_local_store;
	}
    }
  else 
    {
      insn_info_t i_ptr = active_local_stores;
      insn_info_t last = NULL;
      if (dump_file)
	fprintf (dump_file, " processing cselib load\n");

      while (i_ptr)
	{
	  bool remove = false;
	  store_info_t store_info = i_ptr->store_rec;
	  
	  if (dump_file)
	    fprintf (dump_file, " processing cselib load against insn %d\n",
		     INSN_UID (i_ptr->insn));
	  while (store_info && !remove)
	    {
	      if (!store_info->alias_set)
		{
#if 1
		  if (dump_file)
		    {
		      fprintf (dump_file, "  store_info->mem: ");
		      print_inline_rtx (dump_file, store_info->mem, 0);
		      
		      fprintf (dump_file, "  store_info->address: ");
		      print_inline_rtx (dump_file, store_info->address, 0);
		      
		      fprintf (dump_file, "\n   mem: ");
		      print_inline_rtx (dump_file, mem, 0);
		      fprintf (dump_file, "\n");
		    }
#endif
		  
		  remove 
		    = canon_true_dependence (store_info->mem, 
					     GET_MODE (store_info->mem),
					     store_info->address,
					     mem, rtx_varies_p);
		}
	      
	      store_info = store_info->next;
	    }		      
	  
	  if (remove)
	    {
	      if (dump_file)
		dump_insn_info ("removing from active", i_ptr);
	      
	      if (last)
		last->next_local_store = i_ptr->next_local_store;
	      else
		active_local_stores = i_ptr->next_local_store;
	    }
	  else
	    last = i_ptr;
	  i_ptr = i_ptr->next_local_store;
	}
    }
  return 0;
}

/* A for_each_rtx callback in which DATA points the INSN_INFO for 
   as check_mem_read_rtx.  Nullify the pointer if i_m_r_m_r returns
   true for any part of *LOC.  */

static void
check_mem_read_use (rtx *loc, void *data)
{
  for_each_rtx (loc, check_mem_read_rtx, data);
}

/* Apply record_store to all candidate stores in INSN.  Mark INSN
   if some part of it is not a candidate store and assigns to a
   non-register target.  */

static void
scan_insn (bb_info_t bb_info, rtx insn)
{
  rtx body;
  insn_info_t insn_info = pool_alloc (insn_info_pool);
  int mems_found = 0;
  memset (insn_info, 0, sizeof (struct insn_info));

  if (dump_file)
    fprintf (dump_file, "\n**scanning insn=%d\n",
	     INSN_UID (insn));

  insn_info->prev_insn = bb_info->last_insn;
  insn_info->insn = insn;
  bb_info->last_insn = insn_info;
  
  if (CALL_P (insn))
    {
      insn_info->cannot_delete = true;
      /* Const functions cannot do anything bad i.e. read memory, so
	 they are just noise with respect to this analysis.  */
      if (CONST_OR_PURE_CALL_P (insn) && !pure_call_p (insn))
	{
	  if (dump_file)
	    fprintf (dump_file, "const call %d\n", INSN_UID (insn));
	  return;
	}

      /* Every other call, including pure functions may read memory.  */
      add_wild_read (bb_info);
      return;
    }

  /* Cselib clears the table for this case, so we have to essentually
     do the same.  */
  if (NONJUMP_INSN_P (insn)
      && GET_CODE (PATTERN (insn)) == ASM_OPERANDS
      && MEM_VOLATILE_P (PATTERN (insn)))
    {
      add_wild_read (bb_info);
      insn_info->cannot_delete = true;
      return;
    }

  /* Look at all of the uses in the insn.  */
  note_uses (&PATTERN (insn), check_mem_read_use, bb_info);

#if 0
  /* Look for memory reference in the notes.  */
  for_each_rtx (&REG_NOTES (insn), check_mem_read_rtx, bb_info);
#endif

  /* Assuming that there are sets in these insns, we cannot delete
     them.  */
  if ((GET_CODE (PATTERN (insn)) == CLOBBER)
      || volatile_insn_p (PATTERN (insn))
      || (flag_non_call_exceptions && may_trap_p (PATTERN (insn)))
      || (RTX_FRAME_RELATED_P (insn))
      || find_reg_note (insn, REG_FRAME_RELATED_EXPR, NULL_RTX))
    insn_info->cannot_delete = true;
  
  body = PATTERN (insn);
  if (GET_CODE (body) == PARALLEL)
    {
      int i;
      for (i = 0; i < XVECLEN (body, 0); i++)
	mems_found += record_store (XVECEXP (body, 0, i), bb_info);
    }
  else
    mems_found += record_store (body, bb_info);

  if (dump_file)
    fprintf (dump_file, "mems_found = %d, cannot_delete = %s\n", 
	     mems_found, insn_info->cannot_delete ? "true" : "false");

  /* If we found some sets of mems, and the insn has not been marked
     cannot delete, add it into the active_local_stores so that it can
     be locally deleted if found dead.  Otherwise mark it as cannot
     delete.  This simplifies the processing later.  */ 
  if (mems_found > 0 && !insn_info->cannot_delete)
    {
      insn_info->next_local_store = active_local_stores;
      active_local_stores = insn_info;
    }
  else
    insn_info->cannot_delete = true;
}


/* Remove BASE from the set of active_local_stores.  This is a
   callback from cselib that is used to get rid of the stores in
   active_local_stores.  */

static void
remove_useless_values (cselib_val *base)
{
  insn_info_t insn_info = active_local_stores;
  insn_info_t last = NULL;

  while (insn_info)
    {
      store_info_t store_info = insn_info->store_rec;
      bool delete = false;

      /* If ANY of the store_infos match the cselib group that is
	 being deleted, then the insn can not be deleted.  */
      while (store_info)
	{
	  if ((store_info->group_id == -1) 
	      && (store_info->cse_base == base))
	    {
	      delete = true;
	      break;
	    }
	  store_info = store_info->next;
	}

      if (delete)
	{
	  if (last)
	    last->next_local_store = insn_info->next_local_store;
	  else
	    active_local_stores = insn_info->next_local_store;
	  free_store_info (insn_info);
	}
      else
	last = insn_info;
  
      insn_info = insn_info->next_local_store;
    }
}


/* Do all of step 1.  */

static void
step1 (void)
{
  basic_block bb;

  cselib_init (false);
  all_blocks = BITMAP_ALLOC (NULL);
  bitmap_set_bit (all_blocks, ENTRY_BLOCK);
  bitmap_set_bit (all_blocks, EXIT_BLOCK);

  FOR_ALL_BB (bb)
    {
      insn_info_t ptr;
      bb_info_t bb_info = pool_alloc (bb_info_pool);

      memset (bb_info, 0, sizeof (struct bb_info));
      bitmap_set_bit (all_blocks, bb->index);

      bb_table[bb->index] = bb_info;
      cselib_discard_hook = remove_useless_values;

      if (bb->index >= NUM_FIXED_BLOCKS)
	{
	  rtx insn;

	  cse_store_info_pool
	    = create_alloc_pool ("cse_store_info_pool", 
				 sizeof (struct store_info), 100);
	  active_local_stores = NULL;
	  cselib_clear_table ();
	  
	  /* Scan the insns.  */
	  FOR_BB_INSNS (bb, insn)
	    {
	      if (INSN_P (insn))
		scan_insn (bb_info, insn);
	      cselib_process_insn (insn);
	    }
	  
	  /* Get rid of all of the cselib based store_infos in this
	     block and mark the containing insns as not being
	     deletable.  */
	  ptr = bb_info->last_insn;
	  while (ptr)
	    {
	      if (ptr->contains_cselib_groups)
		free_store_info (ptr);
	      ptr = ptr->prev_insn;
	    }

	  free_alloc_pool (cse_store_info_pool);
	}
    }

  cselib_finish ();
  htab_empty (rtx_group_table);
}


/*----------------------------------------------------------------------------
   Second step.

   Assign each byte position in the stores that we are going to
   analyze globally to a position in the bitmaps.  Returns true if
   there are any bit postions assigned.
----------------------------------------------------------------------------*/

static void
step2_init (void)
{
  unsigned int i;
  group_info_t group;

  for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
    {
      /* For all non stack related bases, we only consider a store to
	 be deletable if there are two or more stores for that
	 position.  This is because it takes one store to make the
	 other store redundant.  However, for the stores that are
	 stack related, we consider them if there is only one store
	 for the position.  We do this because the stack related
	 stores can be deleted if their is no read between them and
	 the end of the function.
	 
	 To make this work in the current framework, we take the stack
	 related bases add all of the bits from store1 into store2.
	 This has the effect of making the eligible even if there is
	 only one store.   */

      if (stores_off_frame_dead_at_return
	  && ((group->rtx_base == frame_pointer_rtx)
	      || (group->rtx_base == hard_frame_pointer_rtx)))
	{
	  bitmap_ior_into (group->store2_n, group->store1_n);
	  bitmap_ior_into (group->store2_p, group->store1_p);
	  if (dump_file)
	    fprintf (dump_file, "group %d is frame related ", i); 
	}

      group->offset_map_size_n++;
      group->offset_map_n = XNEWVEC (int, group->offset_map_size_n);
      group->offset_map_size_p++;
      group->offset_map_p = XNEWVEC (int, group->offset_map_size_p);
      group->process_globally = false;
#if 1
      if (dump_file)
	{
	  fprintf (dump_file, "group %d(%d+%d): ", i, 
		   (int)bitmap_count_bits (group->store2_n),
		   (int)bitmap_count_bits (group->store2_p));
	  bitmap_print (dump_file, group->store2_n, "n ", " ");
	  bitmap_print (dump_file, group->store2_p, "p ", "\n");
	}
#endif 
    }
}


/* Init the offset tables for the normal case.  */

static bool
step2_nospill (void)
{
  unsigned int i;
  group_info_t group;
  /* Position 0 is unused because 0 is used in the maps to mean
     unused.  */
  current_position = 1;

  for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
    {
      bitmap_iterator bi;
      unsigned int j;

      if (group == clear_alias_group)
	continue;

      memset (group->offset_map_n, 0, sizeof(int) * group->offset_map_size_n);
      memset (group->offset_map_p, 0, sizeof(int) * group->offset_map_size_p);
      bitmap_clear (group->group_kill);

      EXECUTE_IF_SET_IN_BITMAP (group->store2_n, 0, j, bi)
	{
	  bitmap_set_bit (group->group_kill, current_position);
	  group->offset_map_n[j] = current_position++;
	  group->process_globally = true;
	}
      EXECUTE_IF_SET_IN_BITMAP (group->store2_p, 0, j, bi)
	{
	  bitmap_set_bit (group->group_kill, current_position); 
	  group->offset_map_p[j] = current_position++;
	  group->process_globally = true;
	}
    }
  return current_position != 1;
}


/* Init the offset tables for the spill case.  */

static bool
step2_spill (void)
{
  unsigned int j;
  group_info_t group = clear_alias_group;
  bitmap_iterator bi;

  /* Position 0 is unused because 0 is used in the maps to mean
     unused.  */
  current_position = 1;

  if (dump_file)
    {
      bitmap_print (dump_file, clear_alias_sets, 
		    "clear alias sets              ", "\n");
      bitmap_print (dump_file, disqualified_clear_alias_sets, 
		    "disqualified clear alias sets ", "\n");
    }

  memset (group->offset_map_n, 0, sizeof(int) * group->offset_map_size_n);
  memset (group->offset_map_p, 0, sizeof(int) * group->offset_map_size_p);
  bitmap_clear (group->group_kill);
  
  /* Remove the disqualified positions from the store2_p set.  */
  bitmap_and_compl_into (group->store2_p, disqualified_clear_alias_sets);
  
  /* We do not need to process the store2_n set because
     alias_sets are always positive.  */
  EXECUTE_IF_SET_IN_BITMAP (group->store2_p, 0, j, bi)
    {
      bitmap_set_bit (group->group_kill, current_position); 
      group->offset_map_p[j] = current_position++;
      group->process_globally = true;
    }

  return current_position != 1;
}



/*----------------------------------------------------------------------------
  Third step.
  
  Build the bit vectors for the transfer functions.
----------------------------------------------------------------------------*/


/* Note that this is NOT a general purpose function.  Any mem that has
   an alias set registered here expected to be COMPLETELY unaliased:
   i.e it's addresses are not and need not be examined.  

   It is known that all references to this address will have this
   alias set and there are NO other references to this address in the
   function.  

   Currently the only place that is known to be clean enough to use
   this interface is the code that assigns the spill locations.  

   All of the mems that have alias_sets registered are subjected to a
   very powerful form of dse where function calls, volatile reads and
   writes, and reads from random location are not taken into account.  

   It is also assumed that these locations go dead when the function
   returns.  This assumption could be relaxed if there were found to
   be places that this assumption was not correct.

   The MODE is passed in and saved.  The mode of each load or store to
   a mem with ALIAS_SET is checked against MEM.  If the size of that
   load or store is different from MODE, processing is halted on this
   alias set.  For the vast majority of aliases sets, all of the loads
   and stores will use the same mode.  But vectors are treated
   differently: the alias set is established for the entire vector,
   but reload will insert loads and stores for individual elements and
   we do not necessarily have the information to track those separate
   elements.  So when we see a mode mismatch, we just bail.  */


void 
dse_record_singleton_alias_set (HOST_WIDE_INT alias_set, 
				enum machine_mode mode)
{
  struct clear_alias_mode_holder tmp_holder;
  struct clear_alias_mode_holder *entry;
  void **slot;

  /* If we are not going to run dse, we need to return now or there
     will be problems with allocating the bitmaps.  */
  if ((!gate_dse()) || !alias_set)
    return;

  if (!clear_alias_sets)
    {
      clear_alias_sets = BITMAP_ALLOC (NULL);
      disqualified_clear_alias_sets = BITMAP_ALLOC (NULL);
      clear_alias_mode_table = htab_create (11, clear_alias_mode_hash,
					    clear_alias_mode_eq, NULL);
      clear_alias_mode_pool = create_alloc_pool ("clear_alias_mode_pool", 
						 sizeof (struct clear_alias_mode_holder), 100);
    }

#if 0
  fprintf (stderr, "registering alias set %d(%s)\n", (int)alias_set, GET_MODE_NAME (mode));
#endif 
 
  bitmap_set_bit (clear_alias_sets, alias_set);

  tmp_holder.alias_set = alias_set;

  slot = htab_find_slot (clear_alias_mode_table, &tmp_holder, INSERT);
  gcc_assert (*slot == NULL);

  *slot = entry = pool_alloc (clear_alias_mode_pool);
  entry->alias_set = alias_set;
  entry->mode = mode;
}


/* Remove ALIAS_SET from the sets of stack slots being considered.  */

void 
dse_invalidate_singleton_alias_set (HOST_WIDE_INT alias_set)
{
  if ((!gate_dse()) || !alias_set)
    return;

  bitmap_clear_bit (clear_alias_sets, alias_set);
}


/* Look up the bitmap index for OFFSET in GROUP_INFO.  If it is not
   there, return 0.  */

static int
get_bitmap_index (group_info_t group_info, HOST_WIDE_INT offset)
{
  if (offset < 0)
    {
      HOST_WIDE_INT offset_p = -offset;
      if (offset_p >= group_info->offset_map_size_n)
	return 0;
      return group_info->offset_map_n[offset_p];
    }
  else
    {
      if (offset >= group_info->offset_map_size_p)
	return 0;
      return group_info->offset_map_p[offset];
    }
}


/* Process the STORE_INFOs into the bitmaps into GEN and KILL.  KILL
   may be NULL. */

static void 
scan_stores_nospill (store_info_t store_info, bitmap gen, bitmap kill)
{
  while (store_info)
    {
      HOST_WIDE_INT i;
      group_info_t group_info 
	= VEC_index (group_info_t, rtx_group_vec, store_info->group_id);
      if (group_info->process_globally)
	for (i = store_info->begin; i < store_info->end; i++)
	  {
	    int index = get_bitmap_index (group_info, i);
	    if (index != 0)
	      {
		bitmap_set_bit (gen, index);
		if (kill)
		  bitmap_clear_bit (kill, index);
	      }
	  }
      store_info = store_info->next;
    }
}


/* Process the STORE_INFOs into the bitmaps into GEN and KILL.  KILL
   may be NULL. */

static void 
scan_stores_spill (store_info_t store_info, bitmap gen, bitmap kill)
{
  while (store_info)
    {
      if (store_info->alias_set)
	{
	  int index = get_bitmap_index (clear_alias_group, 
					store_info->alias_set);
	  if (index != 0)
	    {
	      bitmap_set_bit (gen, index);
	      if (kill)
		bitmap_clear_bit (kill, index);
	    }
	}
      store_info = store_info->next;
    }
}


/* Process the READ_INFOs into the bitmaps into GEN and KILL.  KILL
   may be NULL.  */

static void
scan_reads_nospill (read_info_t read_info, bitmap gen, bitmap kill)
{
  while (read_info)
    {
      int i;
      group_info_t group;
      
      for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
	{
	  if (group->process_globally)
	    {
	      if (i == read_info->group_id)
		{
		  if (read_info->begin > read_info->end)
		    {
		      /* Begin > end for block mode reads.  */
		      if (kill)
			bitmap_ior_into (kill, group->group_kill);
		      bitmap_and_compl_into (gen, group->group_kill);
		    }
		  else
		    {
		      /* The groups are the same, just process the
			 offsets.  */
		      HOST_WIDE_INT j;
		      for (j = read_info->begin; j < read_info->end; j++)
			{
			  int index = get_bitmap_index (group, j);
			  if (index != 0)
			    {
			      if (kill)
				bitmap_set_bit (kill, index);
			      bitmap_clear_bit (gen, index);
			    }
			}
		    }
		}
	      else
		{
		  /* The groups are different, if the alias sets
		     conflict, clear the entire group.  We only need
		     to apply this test if the read_info is a cselib
		     read.  Anything with a constant base cannot alias
		     something else with a different constant
		     base.  */
		  if ((read_info->group_id < 0)
		      && canon_true_dependence (group->base_mem, 
						QImode,
						group->canon_base_mem,
						read_info->mem, rtx_varies_p))
		    {
		      if (kill)
			bitmap_ior_into (kill, group->group_kill);
		      bitmap_and_compl_into (gen, group->group_kill);
		    }
		}
	    }
	}
      
      read_info = read_info->next;
    }
}

/* Process the READ_INFOs into the bitmaps into GEN and KILL.  KILL
   may be NULL.  */

static void
scan_reads_spill (read_info_t read_info, bitmap gen, bitmap kill)
{
  while (read_info)
    {
      if (read_info->alias_set)
	{
	  int index = get_bitmap_index (clear_alias_group, 
					read_info->alias_set);
	  if (index != 0)
	    {
	      if (kill)
		bitmap_set_bit (kill, index);
	      bitmap_clear_bit (gen, index);
	    }
	}
      
      read_info = read_info->next;
    }
}


/* Return the insn in BB_INFO before the first wild read or if there
   are no wild reads in the block, return the last insn.  */

static insn_info_t
find_insn_before_first_wild_read (bb_info_t bb_info)
{
  insn_info_t insn_info = bb_info->last_insn;
  insn_info_t last_wild_read = NULL;

  while (insn_info)
    {
      if (insn_info->wild_read)
	{
	  last_wild_read = insn_info->prev_insn;
	  /* Block starts with wild read.  */
	  if (!last_wild_read)
	    return NULL;
	}

      insn_info = insn_info->prev_insn;
    }

  if (last_wild_read)
    return last_wild_read;
  else
    return bb_info->last_insn;
}


/* Scan the insns in BB_INFO starting at PTR and going to the top of
   the block in order to build the gen and kill sets for the block.
   We start at ptr which may be the last insn in the block or may be
   the first insn with a wild read.  In the latter case we are able to
   skip the rest of the block because it just does not matter:
   anything that happens is hidden by the wild read.  */

static void
step3_scan (bool for_spills, basic_block bb)
{
  bb_info_t bb_info = bb_table[bb->index];
  insn_info_t insn_info;

  if (for_spills)
    /* There are no wild reads in the spill case.  */
    insn_info = bb_info->last_insn;
  else
    insn_info = find_insn_before_first_wild_read (bb_info);
    
  /* In the spill case or in the no_spill case if there is no wild
     read in the block, we will need a kill set.  */
  if (insn_info == bb_info->last_insn)
    {
      if (bb_info->kill)
	bitmap_clear (bb_info->kill);
      else
	bb_info->kill = BITMAP_ALLOC (NULL);
    }
  else 
    if (bb_info->kill)
      BITMAP_FREE (bb_info->kill);

  while (insn_info)
    {
      /* There may have been code deleted by the dce pass run before
	 this phase.  */
      if (insn_info->insn && INSN_P (insn_info->insn))
	{
	  /* Process the read(s) last.  */ 
	  if (for_spills)
	    {
	      scan_stores_spill (insn_info->store_rec, bb_info->gen, bb_info->kill);
	      scan_reads_spill (insn_info->read_rec, bb_info->gen, bb_info->kill);
	    }
	  else
	    {
	      scan_stores_nospill (insn_info->store_rec, bb_info->gen, bb_info->kill);
	      scan_reads_nospill (insn_info->read_rec, bb_info->gen, bb_info->kill);
	    }
	}	  

      insn_info = insn_info->prev_insn;
    }
}


/* Set the gen set of the exit block, and also any block with no
   successors that does not have a wild read.  */

static void
step3_exit_block_scan (bb_info_t bb_info)
{
  /* The gen set is all 0's for the exit block except for the
     frame_pointer_group.  */
  
  if (stores_off_frame_dead_at_return)
    {
      unsigned int i;
      group_info_t group;
      
      for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
	{
	  if (group->process_globally
	      && ((group->rtx_base == frame_pointer_rtx)
		  || (group->rtx_base == hard_frame_pointer_rtx)))
	    bitmap_ior_into (bb_info->gen, group->group_kill);
	}
    }
}


/* Find all of the blocks that are not backwards reachable from the
   exit block or any block with no successors (BB).  These are the
   infinite loops or infinite self loops.  These blocks will still
   have their bits set in UNREACHABLE_BLOCKS.  */

static void
mark_reachable_blocks (sbitmap unreachable_blocks, basic_block bb)
{
  edge e;
  edge_iterator ei;

  if (TEST_BIT (unreachable_blocks, bb->index))
    {
      RESET_BIT (unreachable_blocks, bb->index);
      FOR_EACH_EDGE (e, ei, bb->preds)
	{								
	  mark_reachable_blocks (unreachable_blocks, e->src);
	}								
    }
}

/* Build the transfer functions for the function.  */

static void
step3 (bool for_spills)
{
  basic_block bb;
  sbitmap unreachable_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_iterator sbi;
  bitmap all_ones = NULL;
  unsigned int i;
  
  sbitmap_ones (unreachable_blocks);

  FOR_ALL_BB (bb)
    {
      bb_info_t bb_info = bb_table[bb->index];
      if (bb_info->gen)
	bitmap_clear (bb_info->gen);
      else
	bb_info->gen = BITMAP_ALLOC (NULL);

      if (bb->index == ENTRY_BLOCK)
	;
      else if (bb->index == EXIT_BLOCK)
	step3_exit_block_scan (bb_info);
      else
	step3_scan (for_spills, bb);
      if (EDGE_COUNT (bb->succs) == 0)
	mark_reachable_blocks (unreachable_blocks, bb);

      /* If this is the second time dataflow is run, delete the old
	 sets.  */
      if (bb_info->in)
	BITMAP_FREE (bb_info->in);
      if (bb_info->out)
	BITMAP_FREE (bb_info->out);
    }

  /* For any block in an infinite loop, we must initialize the out set
     to all ones.  This could be expensive, but almost never occurs in
     practice. However, it is common in regression tests.  */
  EXECUTE_IF_SET_IN_SBITMAP (unreachable_blocks, 0, i, sbi)
    {
      if (bitmap_bit_p (all_blocks, i))
	{
	  bb_info_t bb_info = bb_table[i];
	  if (!all_ones)
	    {
	      unsigned int j;
	      group_info_t group;

	      all_ones = BITMAP_ALLOC (NULL);
	      for (j = 0; VEC_iterate (group_info_t, rtx_group_vec, j, group); j++)
		bitmap_ior_into (all_ones, group->group_kill);
	    }
	  if (!bb_info->out)
	    {
	      bb_info->out = BITMAP_ALLOC (NULL);
	      bitmap_copy (bb_info->out, all_ones);
	    }
	}
    }

  if (all_ones)
    BITMAP_FREE (all_ones);
  sbitmap_free (unreachable_blocks);
}



/*----------------------------------------------------------------------------
   Fourth step.

   Solve the bitvector equations.
----------------------------------------------------------------------------*/


/* Confluence function for blocks with no successors.  Create an out
   set from the gen set of the exit block.  This block logically has
   the exit block as a successor.  */



static void
dse_confluence_0 (basic_block bb)
{
  bb_info_t bb_info = bb_table[bb->index];

#if 0
  if (dump_file)
    fprintf (dump_file, "confluence_0 called with %d\n", 
	     bb->index);
#endif
  if (bb->index == EXIT_BLOCK)
    return;

  if (!bb_info->out)
    {
      bb_info->out = BITMAP_ALLOC (NULL);
      bitmap_copy (bb_info->out, bb_table[EXIT_BLOCK]->gen);
    }
}

/* Propagate the information from the in set of the dest of E to the
   out set of the src of E.  If the various in or out sets are not
   there, that means they are all ones.  */

static void
dse_confluence_n (edge e)
{
  bb_info_t src_info = bb_table[e->src->index];
  bb_info_t dest_info = bb_table[e->dest->index];

#if 0
  if (dump_file)
    {
      fprintf (dump_file, "confluence_n called with %d->%d\n", 
	       e->src->index, e->dest->index);
      df_print_bb_index (e->src, dump_file);
      df_print_bb_index (e->dest, dump_file);
    }
#endif

  if (dest_info->in)
    {
      if (src_info->out)
	bitmap_and_into (src_info->out, dest_info->in);
      else
	{
	  src_info->out = BITMAP_ALLOC (NULL);
	  bitmap_copy (src_info->out, dest_info->in);
	}
    }
}


/* Propagate the info from the out to the in set of BB_INDEX's basic
   block.  There are three cases:  

   1) The block has no kill set.  In this case the kill set is all
   ones.  It does not matter what the out set of the block is, none of
   the info can reach the top.  The only thing that reaches the top is
   the gen set and we just copy the set.

   2) There is a kill set but no out set and bb has successors.  In
   this case we just return. Eventually an out set will be created and
   it is better to wait than to create a set of ones.

   3) There is both a kill and out set.  We apply the obvious transfer
   function.
*/

static bool
dse_transfer_function (int bb_index)
{
  bb_info_t bb_info = bb_table[bb_index];

#if 0
  if (dump_file)
    fprintf (dump_file, "transfer called with %d\n", 
	     bb_index);
#endif
  if (bb_info->kill)
    {
      if (bb_info->out)
	{
	  /* Case 3 above.  */
	  if (bb_info->in)
	    return bitmap_ior_and_compl (bb_info->in, bb_info->gen, 
					 bb_info->out, bb_info->kill);
	  else
	    {
	      bb_info->in = BITMAP_ALLOC (NULL);
	      bitmap_ior_and_compl (bb_info->in, bb_info->gen, 
				    bb_info->out, bb_info->kill);
	      return true;
	    }
	}
      else
	/* Case 2 above.  */
	return false;
    }
  else
    {
      /* Case 1 above.  If there is already an in set, nothing
	 happens.  */
      if (bb_info->in)
	return false;
      else
	{
	  bb_info->in = BITMAP_ALLOC (NULL);
	  bitmap_copy (bb_info->in, bb_info->gen);
	  return true;
	}
    }
}

#ifdef BASELINE
static struct df_problem problem =
{
  0,                          /* Problem id.  */
  DF_BACKWARD,                /* Direction.  */
  NULL,                       /* Allocate the problem specific data.  */
  NULL,                       /* Reset global information.  */
  NULL,                       /* Free basic block info.  */
  NULL,                       /* Local compute function.  */
  NULL,                       /* Init the solution specific data.  */
  df_iterative_dataflow,      /* Iterative solver.  */
  dse_confluence_0,           /* Confluence operator 0.  */ 
  dse_confluence_n,           /* Confluence operator n.  */ 
  dse_transfer_function,      /* Transfer function.  */
  NULL,                       /* Finalize function.  */
  NULL,                       /* Free all of the problem information.  */
  NULL,                       /* Debugging.  */
  NULL,                       /* Dependent problem.  */
  0                           /* Changeable flags.  */
};
#endif

#ifdef BASELINE
static struct dataflow dflow;
#endif

/* Solve the dataflow equations.  */

static void
step4 (void)
{
#ifdef BASELINE
  int *postorder = XNEWVEC (int, last_basic_block);
  int  n_blocks = post_order_compute (postorder, true);
#endif

#ifdef BASELINE
  dflow.flags = 0;
  dflow.df = shared_df;
  dflow.problem = &problem;
  df_analyze_problem (&dflow, all_blocks, all_blocks, all_blocks,
		      postorder, n_blocks, false);
  free (postorder);
#else
  df_simple_dataflow (DF_BACKWARD, NULL, dse_confluence_0, 
		      dse_confluence_n, dse_transfer_function, 
	   	      all_blocks, df_get_postorder (DF_BACKWARD), 
		      df_get_n_blocks (DF_BACKWARD));
#endif
  if (dump_file)
    {
      basic_block bb;

      fprintf (dump_file, "\n\n*** Global dataflow info after analysis.\n");
      FOR_ALL_BB (bb)
	{
	  bb_info_t bb_info = bb_table[bb->index];

	  df_print_bb_index (bb, dump_file);
	  if (bb_info->in)
	    bitmap_print (dump_file, bb_info->in, "  in:   ", "\n");
	  else
	    fprintf (dump_file, "  in:   *MISSING*\n");
	  if (bb_info->gen)
	    bitmap_print (dump_file, bb_info->gen, "  gen:  ", "\n");
	  else
	    fprintf (dump_file, "  gen:  *MISSING*\n");
	  if (bb_info->kill)
	    bitmap_print (dump_file, bb_info->kill, "  kill: ", "\n");
	  else
	    fprintf (dump_file, "  kill: *MISSING*\n");
	  if (bb_info->out)
	    bitmap_print (dump_file, bb_info->out, "  out:  ", "\n");
	  else
	    fprintf (dump_file, "  out:  *MISSING*\n\n");
	}
    }
}



/*----------------------------------------------------------------------------
   Fifth step.

   Delete the stores that can only be deleted using the global informantion.
----------------------------------------------------------------------------*/


static void
step5_nospill (void)
{
  basic_block bb;
  FOR_EACH_BB (bb)
    {
      bb_info_t bb_info = bb_table[bb->index];
      insn_info_t insn_info = bb_info->last_insn;
      bitmap v = bb_info->out;

      while (insn_info)
	{
	  bool deleted = false;
	  if (dump_file && insn_info->insn)
	    {
	      fprintf (dump_file, "starting to process insn %d\n",
		       INSN_UID (insn_info->insn));
	      bitmap_print (dump_file, v, "  v:  ", "\n");
	    }

	  /* There may have been code deleted by the dce pass run before
	     this phase.  */
	  if (insn_info->insn 
	      && INSN_P (insn_info->insn)
	      && (!insn_info->cannot_delete)
	      && (!bitmap_empty_p (v)))
	    {
	      store_info_t store_info = insn_info->store_rec;

	      /* Try to delete the current insn.  */
	      deleted = true;
	      
	      while (store_info)
		{
		  if (store_info->alias_set)
		    deleted = false;
		  else
		    {
		      HOST_WIDE_INT i;
		      group_info_t group_info 
			= VEC_index (group_info_t, rtx_group_vec, store_info->group_id);
		      
		      for (i = store_info->begin; i < store_info->end; i++)
			{
			  int index = get_bitmap_index (group_info, i);
			  
#if 1
			  if (dump_file)
			    fprintf (dump_file, "i = %d, index = %d\n", (int)i, index); 
#endif
			  if (index == 0 || !bitmap_bit_p (v, index))
			    {
#if 1
			      if (dump_file)
				fprintf (dump_file, "failing at i = %d\n", (int)i); 
#endif
			      deleted = false;
			      break;
			    }
			}
		    }
		  
		  store_info = store_info->next;
		}
	      if (deleted)
		{
		  if (dbg_cnt (dse))
		    {
		      check_for_inc_dec (insn_info->insn);
		      delete_insn (insn_info->insn);
		      insn_info->insn = NULL;
		      globally_deleted++;
		    }
		}
	    
	    }
	  /* We do want to process the local info if the insn was
	     deleted.  For insntance, if the insn did a wild read, we
	     no longer need to trash the info.  */
	  if (insn_info->insn 
	      && INSN_P (insn_info->insn)
	      && (!deleted))
	    {
	      scan_stores_nospill (insn_info->store_rec, v, NULL);
	      if (insn_info->wild_read)
		{
		  if (dump_file)
		    fprintf (dump_file, "wild read\n");
		  bitmap_clear (v);
		}
	      else if (insn_info->read_rec)
		{
		  if (dump_file)
		    fprintf (dump_file, "regular read\n");
		  scan_reads_nospill (insn_info->read_rec, v, NULL);
		}
	    }
	      
	  insn_info = insn_info->prev_insn;
	}
    }
}


static void
step5_spill (void)
{
  basic_block bb;
  FOR_EACH_BB (bb)
    {
      bb_info_t bb_info = bb_table[bb->index];
      insn_info_t insn_info = bb_info->last_insn;
      bitmap v = bb_info->out;

      while (insn_info)
	{
	  bool deleted = false;
	  /* There may have been code deleted by the dce pass run before
	     this phase.  */
	  if (insn_info->insn 
	      && INSN_P (insn_info->insn)
	      && (!insn_info->cannot_delete)
	      && (!bitmap_empty_p (v)))
	    {
	      /* Try to delete the current insn.  */
	      store_info_t store_info = insn_info->store_rec;
	      deleted = true;
	      
	      while (store_info)
		{
		  if (store_info->alias_set)
		    {
		      int index = get_bitmap_index (clear_alias_group, 
						    store_info->alias_set);
		      if (index == 0 || !bitmap_bit_p (v, index))
			{
			  deleted = false;
			  break;
			}
		    }
		  else 
		    deleted = false;
		  store_info = store_info->next;
		}
	      if (deleted)
		{
		  if (dbg_cnt (dse))
		    {
		      if (dump_file)
			fprintf (dump_file, "Spill deleting insn %d\n", 
				 INSN_UID (insn_info->insn));
		      check_for_inc_dec (insn_info->insn);
		      delete_insn (insn_info->insn);
		      spill_deleted++;
		      insn_info->insn = NULL;
		    }
		}
	    }
	  
	  if (insn_info->insn 
	      && INSN_P (insn_info->insn)
	      && (!deleted))
	    {
	      scan_stores_spill (insn_info->store_rec, v, NULL);
	      scan_reads_spill (insn_info->read_rec, v, NULL);
	    }
	      
	  insn_info = insn_info->prev_insn;
	}
    }
}



/*----------------------------------------------------------------------------
   Sixth step.

   Destroy everything left standing. 
----------------------------------------------------------------------------*/

static void 
step6 (bool global_done)
{
  unsigned int i;
  group_info_t group;
  basic_block bb;
  
  if (global_done)
    {
      for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
	{
	  free (group->offset_map_n);
	  free (group->offset_map_p);
	  BITMAP_FREE (group->store1_n);
	  BITMAP_FREE (group->store1_p);
	  BITMAP_FREE (group->store2_n);
	  BITMAP_FREE (group->store2_p);
	  BITMAP_FREE (group->group_kill);
	}

      FOR_ALL_BB (bb)
	{
	  bb_info_t bb_info = bb_table[bb->index];
	  BITMAP_FREE (bb_info->gen);
	  if (bb_info->kill)
	    BITMAP_FREE (bb_info->kill);
	  if (bb_info->in)
	    BITMAP_FREE (bb_info->in);
	  if (bb_info->out)
	    BITMAP_FREE (bb_info->out);
	}
    }
  else
    {
      for (i = 0; VEC_iterate (group_info_t, rtx_group_vec, i, group); i++)
	{
	  BITMAP_FREE (group->store1_n);
	  BITMAP_FREE (group->store1_p);
	  BITMAP_FREE (group->store2_n);
	  BITMAP_FREE (group->store2_p);
	  BITMAP_FREE (group->group_kill);
	}
    }

  /* Get rid of the positions_needed bitmaps.  */
  FOR_EACH_BB (bb)
    {
      bb_info_t bb_info = bb_table[bb->index];
      insn_info_t insn_info = bb_info->last_insn;
      while (insn_info)
	{
	  store_info_t store_info= insn_info->store_rec;
	  while (store_info)
	    {
	      sbitmap_free (store_info->positions_needed);
	      store_info = store_info->next;
	    }
	  insn_info = insn_info->prev_insn;
	}
    }

  if (clear_alias_sets)
    {
      BITMAP_FREE (clear_alias_sets);
      BITMAP_FREE (disqualified_clear_alias_sets);
      free_alloc_pool (clear_alias_mode_pool);
      htab_delete (clear_alias_mode_table);
    }

  end_alias_analysis ();
  free (bb_table);
  htab_delete (rtx_group_table);
  VEC_free (group_info_t, heap, rtx_group_vec);
  BITMAP_FREE (all_blocks);
  BITMAP_FREE (scratch);

  free_alloc_pool (rtx_store_info_pool);
  free_alloc_pool (read_info_pool);
  free_alloc_pool (insn_info_pool);
  free_alloc_pool (bb_info_pool);
  free_alloc_pool (rtx_group_info_pool);
}



/* -------------------------------------------------------------------------
   DSE
   ------------------------------------------------------------------------- */

/* Callback for running pass_rtl_dse.  */

static unsigned int
rest_of_handle_dse (void)
{
  bool did_global = false;
  FILE * saved_dump_file = dump_file;
#if 0
  dump_file = stderr;
#endif

  step0 ();
  step1 ();
  step2_init ();
  if (step2_nospill ())
    {
#ifndef BASELINE 
      df_set_flags (DF_LR_RUN_DCE);
      df_analyze ();
#endif
      did_global = true;
      if (dump_file)
	fprintf (dump_file, "doing global processing\n");
      step3 (false);
      step4 ();
      step5_nospill ();
    }

  /* For the instance of dse that runs after reload, we make a special
     pass to process the spills.  These are special in that they are
     totally transparent, i.e, there is no aliasing issues that need
     to be considered.  This means that the wild reads that kill
     everything else do not apply here.  */ 
  if (clear_alias_sets && step2_spill ())
    {
#ifndef BASELINE 
      if (!did_global)
	{
	  df_set_flags (DF_LR_RUN_DCE);
	  df_analyze ();
	}
#endif
      did_global = true;
      if (dump_file)
	fprintf (dump_file, "doing global spill processing\n");
      step3 (true);
      step4 ();
      step5_spill ();
    }
  
  step6 (did_global);

  if (dump_file)
    fprintf (dump_file, "dse: local deletions = %d, global deletions = %d, spill deletions = %d\n",
	     locally_deleted, globally_deleted, spill_deleted);
  dump_file = saved_dump_file;
  return 0;
}

static bool
gate_dse (void)
{
#ifdef BASELINE
  return optimize > 0;
#else
  return optimize > 0 && flag_dse;
#endif
}

struct tree_opt_pass pass_rtl_dse1 =
{
  "dse1",                               /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE1,                              /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
#ifndef BASELINE
  TODO_df_finish |
#endif
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};

struct tree_opt_pass pass_rtl_dse2 =
{
  "dse2",                               /* name */
  gate_dse,                             /* gate */
  rest_of_handle_dse,                   /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_DSE2,                              /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
#ifndef BASELINE
  TODO_df_finish |
#endif
  TODO_ggc_collect,                     /* todo_flags_finish */
  'w'                                   /* letter */
};

