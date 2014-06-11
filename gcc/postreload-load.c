/* Post reload partially redundant load elimination
   Copyright (C) 2007 Cavium Networks.

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
#include "diagnostic-core.h"
#include "rtl.h"
#include "obstack.h"
#include "basic-block.h"
#include "flags.h"
#include "hashtab.h"
#include "tree-pass.h"
#include "timevar.h"
#include "expr.h"
#include "recog.h"
#include "tm_p.h"

/* This file implements some parts of the future enhancement discussed
   in the comment of the file postreload-gcse.  It is a local
   optimization and performs this optimization:

      store/load $2, <a>
      ...
      $2 <- ...		(first kill)
      ...		(no use of $3)
      load $3, <a>
   =>
      store/load $2, <a>
      ...
      move $3, $2
      $2 <- ...		(first kill)
      ...		(no use of $3)
*/


/* A load or store at which point we know that reg == <mem>.  */
struct load
{
  rtx mem;
  rtx reg;
  rtx reg_kill;
};

static htab_t htab_load;

/* Equality function for the load hashtable.  X is an entry in the
   hashtable.  Y is a mem expression.  */

static int
load_htab_eq (const void *x, const void *y)
{
  return rtx_equal_p (((const struct load *) x)->mem, (const_rtx) y);
}

/* Compute hash value of mem expression X.  Used with htab_*_with_hash
   functions.  */

static hashval_t
load_rtx_hash (const void *x)
{
  int do_not_record = 0;
  return hash_rtx ((const_rtx) x, GET_MODE ((const_rtx) x), &do_not_record, NULL, false);
}

/* Compute has value for a load entry.  Used as the hash table's hash
   function when expanding the table.  */

static hashval_t
load_htab_hash (const void *x)
{
  return load_rtx_hash (((const struct load *) x)->mem);
}

/* Decide whether this is a load/store we want to trace.  */

static bool
interesting_load (rtx set)
{
  rtx mem, reg;

  if (!set)
    return false;

  /* Handle store just like loads.  */
  mem = SET_DEST (set);
  if (MEM_P (mem))
    reg = SET_SRC (set);
  else
    {
      reg = mem;
      mem = SET_SRC (set);
    }

  return MEM_P (mem) && !MEM_VOLATILE_P (mem) && REG_P (reg);
}

/* Add the load/store SET into the hashtable.  Initialize its
   fields.  */

static struct load *
alloc_load (rtx set)
{
  struct load **slot;
  rtx mem, reg;

  mem = SET_DEST (set);
  if (MEM_P (mem))
    reg = SET_SRC (set);
  else
    {
      reg = mem;
      mem = SET_SRC (set);
    }

  slot = (struct load **)
    htab_find_slot_with_hash (htab_load, mem, load_rtx_hash (mem), INSERT);
  if (*slot == NULL)
    {
      *slot = (struct load*) xcalloc (1, sizeof (struct load));
      (*slot)->mem = mem;
    }
  else
    (*slot)->reg_kill = 0;

  (*slot)->reg = reg;

  return *slot; 
}

/* Return TRUE along with the setting *LOAD if this is a load that we
   can eliminate.  */

static bool
interesting_second_load (rtx set, struct load ***load, rtx insn)
{
  rtx mem, reg;

  if (!set)
    return false;

  mem = SET_SRC (set);
  reg = SET_DEST (set);
  if (!MEM_P (mem) || MEM_VOLATILE_P (mem) || !REG_P (reg))
    return false;

  *load = (struct load **)
    htab_find_slot_with_hash (htab_load, mem, load_rtx_hash (mem),
			      NO_INSERT);
  if (!*load)
    return false;

  /* Don't work on cases that never happen: if there is no kill, we
     would have inherited the reload; if the store and load regs are
     the same we would need to find an available register.  If the
     kill insn was already replaced by a move this information is
     stale, disregard it.  */
  if (rtx_equal_p (reg, (**load)->reg)
      || !(**load)->reg_kill
      || INSN_DELETED_P ((**load)->reg_kill)
      || reg_used_between_p (reg, PREV_INSN ((**load)->reg_kill),
			     NEXT_INSN (insn))
      || reg_set_between_p (reg, PREV_INSN ((**load)->reg_kill), insn))
    {
      if (dump_file)
	{
	  fputs ("\nCan't insert the move before the kill for this load:\n  ",
		 dump_file);
	  print_inline_rtx (dump_file, insn, 2);
	  fputs ("\n\n", dump_file);
	}
      return false;
    }

  return true;
}

/* Call be htab_traverse_noresize to set reg_kill and clear
   invalidated entries in the load hash table.  ARG is the current
   insn we are processing.  */

static int
find_reg_kill_and_mem_invalidate (void **slot, void *arg)
{
  struct load *load = (struct load *) *slot;
  rtx insn = (rtx)arg;

  /* Record the farthest one from the load.  Ignore the effect of a
     store we just added.  */
  if (!load->reg_kill && modified_in_p (load->reg, insn))
    load->reg_kill = insn;

  if (modified_in_p (load->mem, insn))
    htab_clear_slot (htab_load, slot);

  return 1;
}

namespace {

const pass_data pass_data_postreload_load =
{
  RTL_PASS, /* type */
  "postreload_load", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* has_execute */
  TV_LOAD_AFTER_RELOAD,	/* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_postreload_load : public rtl_opt_pass
{
public:
  pass_postreload_load (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_postreload_load, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_postreload_load (m_ctxt); }
  virtual bool gate (function *) { return flag_postreload_load; }
  virtual unsigned int execute (function *);
};  // class pass_postreload_load

}  // anon namespace

/* This is the main function of the pass.  */

unsigned int
pass_postreload_load::execute (function*)
{
  basic_block bb;

  init_alias_analysis ();

  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx insn;

      htab_load = htab_create (10, load_htab_hash, load_htab_eq, NULL);

      FOR_BB_INSNS (bb, insn)
	{
	  rtx set;
	  struct load **load;

	  /* Set reg_kill, invalidate entries if there is an
	     aliasing store or if the registers making up the address
	     change.  */
	  htab_traverse_noresize
	    (htab_load, find_reg_kill_and_mem_invalidate, insn);	

	  set = single_set (insn);
	  if (interesting_second_load (set, &load, insn))
	    {
	      rtx move;

	      move = gen_move_insn (SET_DEST (set), (*load)->reg);
	      /* Make sure we can generate a move.  */
	      extract_insn (move);
	      if (! constrain_operands (1))
		continue;

	      move = emit_insn_before (move, (*load)->reg_kill);
	      delete_insn (insn);

	      if (dump_file)
		{
		  fputs ("Replaced this load:\n  ", dump_file);
		  print_inline_rtx (dump_file, insn, 2);
		  fputs ("\n  with this move:\n  ", dump_file);
		  print_inline_rtx (dump_file, move, 2);
		  fputs ("\n\n", dump_file);
		}
	    }
	  else if (interesting_load (set))
	    alloc_load (set);
	  else if (CALL_P (insn))
	    htab_empty (htab_load);
	}

      htab_empty (htab_load);
    }

  end_alias_analysis ();

  return 0;
}



rtl_opt_pass * 
make_pass_postreload_load (gcc::context *ctxt)
{
  return new pass_postreload_load (ctxt);
}
