#include "config.h"
#include "system.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "df.h"
#include "output.h"
#include "function.h"

typedef int (*loop_function)	 PARAMS ((rtx insn, void *data));
static int for_each_insn_in_loop PARAMS ((struct loop *, loop_function, void *));
static int mark_def_varies	 PARAMS ((rtx insn, void *data));
static int collect_stores	 PARAMS ((rtx insn, void *data));
static void note_mem_store	 PARAMS ((rtx, rtx, void *));
static int discover_invariant	 PARAMS ((rtx insn, void *data));
static int not_invariant_rtx	 PARAMS ((rtx *rtxp, void *data));
static bool mark_maybe_invariant_set PARAMS ((rtx, rtx, rtx, struct loop_invariants *));

static int
for_each_insn_in_loop (loop, fun, data)
     struct loop *loop;
     loop_function fun;
     void *data;
{
  int i;
  int val;
  int retval = 0;
  basic_block *body;

  body = get_loop_body (loop);
  for (i = 0; i < loop->num_nodes; i++)
    {
      basic_block bb = body[i];
      rtx insn;

      for (insn = bb->head; insn != NEXT_INSN (bb->end);
        insn = NEXT_INSN (insn))
	{
	  val = (*fun) (insn, data);
	  if (val < 0)
	    retval = val;
	  else if (val)
	    return val;
	}
    }
  free (body);
  return retval;
}

/* Mark each definition in the INSN as varying.  */

static int
mark_def_varies (insn, data)
     rtx insn;
     void *data;
{
  struct loop_invariants *inv = (struct loop_invariants *) data;
  struct df_link *defs = DF_INSN_DEFS (inv->df, insn);

  while (defs)
    {
      bitmap_set_bit (inv->def_varies, defs->ref->id);

      defs = defs->next;
    }
  return 0;
}

/* Create list of all memory references stores inside loop.  */

struct collect_stores_data
{
  bool unknown_address_altered, unknown_constant_address_altered;
  struct loop_invariants *inv;
};

static void
note_mem_store (x, y, data)
     rtx x;
     rtx y ATTRIBUTE_UNUSED;
     void *data;
{
  struct collect_stores_data *s = (struct collect_stores_data *) data;
  struct loop_invariants *inv = s->inv;

  if (x == 0 || GET_CODE (x) != MEM)
    return;

  /* BLKmode MEM means all memory is clobbered.  */
  if (GET_MODE (x) == BLKmode)
    {
      if (RTX_UNCHANGING_P (x))
	s->unknown_constant_address_altered = true;
      else
	s->unknown_address_altered = true;

      return;
    }

  inv->store_mems = gen_rtx_EXPR_LIST (VOIDmode, x, inv->store_mems);
}

static int
collect_stores (insn, data)
     rtx insn;
     void *data;
{
  struct collect_stores_data *s = (struct collect_stores_data *) data;

  if (!INSN_P (insn))
    return 0;
  if (GET_CODE (insn) == CALL_INSN && !CONST_OR_PURE_CALL_P (insn))
    s->unknown_address_altered = true;
  note_stores (PATTERN (insn), note_mem_store, s);
  return 0;
}

/* Attempt to prove that expression RTXP is invariant.
   Called via for_each_rtx.  */

struct not_invariant_rtx_data
{
  struct loop_invariants *inv;
  rtx insn;
};

static int
not_invariant_rtx (rtxp, data)
     rtx *rtxp;
     void *data;
{
  struct not_invariant_rtx_data *d = (struct not_invariant_rtx_data *) data;
  struct loop_invariants *inv = d->inv;
  struct df_link *uses;
  struct df_link *def;
  rtx insn = d->insn;
  rtx x = *rtxp;
  rtx mem_list_entry;

  if (!*rtxp)
    return 0;

  switch (GET_CODE (x))
    {
    case CALL:
      if (!CONST_OR_PURE_CALL_P (d->insn))
	return 1;
      if (!loop_invariant_rtx_p
	  (inv, d->insn, CALL_INSN_FUNCTION_USAGE (d->insn)))
	return 1;
      break;

    case CC0:
      return !loop_invariant_rtx_p (inv, prev_nonnote_insn (d->insn),
				    SET_SRC (single_set
					     (prev_nonnote_insn (d->insn))));

    case UNSPEC_VOLATILE:
      return 1;

    case MEM:
      if (MEM_VOLATILE_P (x))
	return 1;

      mem_list_entry = inv->store_mems;
      while (mem_list_entry)
	{
	  if (true_dependence (XEXP (mem_list_entry, 0), VOIDmode,
			       x, rtx_varies_p))
	    return 1;

	  mem_list_entry = XEXP (mem_list_entry, 1);
	}
      break;

    case REG:

      if ((x == frame_pointer_rtx || x == hard_frame_pointer_rtx
	   || x == arg_pointer_rtx) && !current_function_has_nonlocal_goto)
	return 1;

      for (uses = DF_INSN_USES (inv->df, insn); uses; uses = uses->next)
	if (REGNO (DF_REF_REAL_REG (uses->ref)) == REGNO (x))
	  break;

      if (!uses)
	abort ();

      def = DF_REF_CHAIN (uses->ref);

      /* Uninitialized def may be considered invariant.  */

      if (!def)
	return 0;

      if (bitmap_bit_p (inv->def_varies, DF_REF_ID (def->ref)))
	return 1;

      /* In case some definition is inside loop, it must be exactly one,
         as otherwise the value depends on the way we reached the insn.  */

      if (flow_bb_inside_loop_p (inv->loop, DF_REF_BB (def->ref)))
	return (def->next != 0);

      /* Otherwise verify that all other defs are from outside loop.  */

      for (def = def->next; def; def = def->next)
	if (flow_bb_inside_loop_p (inv->loop, DF_REF_BB (def->ref)))
	  return 1;

      return 0;
      break;

    case ASM_OPERANDS:
      if (MEM_VOLATILE_P (x))
	return 1;

    default:
      break;
    }
  return 0;
}

/* In case SRC is invariant expression, mark definition definining
   DEST as invariant.  */

static bool
mark_maybe_invariant_set (src, dest, insn, inv)
     rtx src, dest, insn;
     struct loop_invariants *inv;
{
  struct df_link *defs = DF_INSN_DEFS (inv->df, insn);

  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);
  if (!REG_P (dest))
    return false;
  if (!loop_invariant_rtx_p (inv, insn, src))
    return false;

  for (defs = DF_INSN_DEFS (inv->df, insn); defs; defs = defs->next)
    if (REGNO (DF_REF_REAL_REG (defs->ref)) == REGNO (dest))
      break;

  if (!defs)
    abort ();

  if (!bitmap_bit_p (inv->def_varies, DF_REF_ID (defs->ref)))
    return false;
  bitmap_clear_bit (inv->def_varies, DF_REF_ID (defs->ref));
  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Invariant set of register %i found in insn %i\n",
	       REGNO (dest), INSN_UID (insn));
    }
  return true;
}

/* Search INSN for possibly invariant sets and mark them.  */

static int
discover_invariant (insn, data)
     rtx insn;
     void *data;
{
  struct loop_invariants *inv = (struct loop_invariants *) data;
  int i;
  rtx pat;
  rtx set;
  rtx note;
  bool marked = false;

  if (!INSN_P (insn))
    return 0;

  pat = PATTERN (insn);

  if (GET_CODE (pat) == SET)
    marked = mark_maybe_invariant_set (SET_SRC (pat), SET_DEST (pat),
				       insn, inv);
  else if (GET_CODE (pat) == PARALLEL)
    {
      for (i = 0; i < XVECLEN (pat, 0); i++)
	{
	  rtx x = XVECEXP (pat, 0, i);

	  if (GET_CODE (x) == SET)
	    marked |= mark_maybe_invariant_set (SET_SRC (x), SET_DEST (x),
						insn, inv);
	}
    }

  /* In case we failed to prove invariantness for insn, we still may prove
     it using REG_EQUAL note.  */

  if (!marked && (set = single_set (insn))
      && GET_CODE (set) == SET
      && ((note = find_reg_note (insn, REG_EQUAL, NULL_RTX))
	  || (note = find_reg_note (insn, REG_EQUIV, NULL_RTX))))
    marked |=
      mark_maybe_invariant_set (XEXP (note, 0), SET_DEST (pat), insn, inv);

  return marked ? -1 : 0;
}

/* Initialize the invariant datastructure.  */

struct loop_invariants *
init_loop_invariants (loop, df)
     struct loop *loop;
     struct df *df;
{
  struct loop_invariants *inv = xcalloc (sizeof (struct loop_invariants), 1);
  struct collect_stores_data s;
  int i = 0;

  inv->loop = loop;
  inv->df = df;
  inv->def_varies = BITMAP_XMALLOC ();

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Searching invariants in loop %i\n\n", loop->num);

  /* Mark everything inside loop as not invariant first.  */

  for_each_insn_in_loop (loop, mark_def_varies, inv);

  s.unknown_address_altered = false;
  s.unknown_constant_address_altered = false;
  s.inv = inv;
  for_each_insn_in_loop (loop, collect_stores, &s);

  if (s.unknown_address_altered)
    {
      rtx mem = gen_rtx_MEM (BLKmode, const0_rtx);

      inv->store_mems = gen_rtx_EXPR_LIST (VOIDmode, mem, inv->store_mems);
    }
  if (s.unknown_constant_address_altered)
    {
      rtx mem = gen_rtx_MEM (BLKmode, const0_rtx);

      RTX_UNCHANGING_P (mem) = 1;
      inv->store_mems = gen_rtx_EXPR_LIST (VOIDmode, mem, inv->store_mems);
    }

  /* Attempt to prove invariantness of each insn.
     Iterate until process stabilize.  */
  while (for_each_insn_in_loop (loop, discover_invariant, inv))
    i++;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "%i iterations needed.\n\n", i);

  return inv;
}

/* Free the invariants datastructure.  */

void
free_loop_invariants (inv)
     struct loop_invariants *inv;
{
  free_EXPR_LIST_list (&inv->store_mems);
  BITMAP_XFREE (inv->def_varies);
  free (inv);
}

/* Return trye in case X is the invariant if computed in INSN.
   X must use only subset of registers used by INSN.

   Function is somewhat aggressive in a way that partially initialized
   or uninitialized values are considered invariants - that means that
   the value can be made invariant by code hoisting, but it may not be
   in current program representation.  Lets see how many problems
   this can cause.  */

bool
loop_invariant_rtx_p (inv, insn, x)
     struct loop_invariants *inv;
     rtx insn;
     rtx x;
{
  struct not_invariant_rtx_data data;

  data.inv = inv;
  data.insn = insn;
  return (!for_each_rtx (&x, not_invariant_rtx, (void *) &data));
}

/* Analyze each loop and dump invariants to debug output.  */

void
test_invariants (loops)
     struct loops *loops;
{
  if (loops->num > 1)
    {
      struct df *df = df_init ();
      int i;

      df_analyse (df, 0, DF_UD_CHAIN | DF_HARD_REGS | DF_EQUIV_NOTES);

#if 0
      if (rtl_dump_file)
	df_dump (df, DF_UD_CHAIN, rtl_dump_file);
#endif

      for (i = 1; i < loops->num; i++)
	free_loop_invariants (init_loop_invariants (loops->parray[i], df));

      df_finish (df);
    }
}

/* Creates a pre-header for a LOOP.  Returns newly created block.  */
basic_block
create_preheader (loop, dom)
     struct loop *loop;
     sbitmap *dom;
{
  edge e, fallthru;
  basic_block dummy;
  basic_block jump, src;
  struct loop *cloop = NULL;
  int nentry = 0;
  rtx insn;

  for (e = loop->header->pred; e; e = e->pred_next)
    {
      if (e->src == loop->latch)
	continue;
      cloop = find_common_loop (cloop, e->src->loop_father);
      nentry++;
    }
  if (!nentry)
    abort ();
  if (nentry == 1)
    return NULL;

  insn = PREV_INSN (first_insn_after_basic_block_note (loop->header));
  fallthru = split_block (loop->header, insn);
  dummy = fallthru->src;
  if (loop->latch == loop->header)
    loop->latch = fallthru->dest;
  loop->header = fallthru->dest;
  
  /* Redirect edges. */
  for (e = dummy->pred; e; e = e->pred_next)
    {
      src = e->src;
      if (src == loop->latch)
	break;
    }
  if (!e)
    abort ();

  dummy->frequency -= EDGE_FREQUENCY (e);
  jump = redirect_edge_and_branch_force (e, loop->header);
  if (jump)
    {
      set_immediate_dominator (dom, jump, src);
      add_bb_to_loop (jump, loop);
    }

  /* Update structures.  */
  set_immediate_dominator (dom, loop->header, dummy);
  loop->header->loop_father = loop;
  add_bb_to_loop (dummy, cloop);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Created preheader block for loop %i\n",
	     loop->num);

  return dummy;
}

/* Create preheader each loop.  */
void
create_preheaders (loops)
     struct loops *loops;
{
  int i;
  for (i = 1; i < loops->num; i++)
    create_preheader (loops->parray[i], loops->cfg.dom);
}
