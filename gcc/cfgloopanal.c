#include "config.h"
#include "system.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "df.h"
#include "output.h"
#include "function.h"
#include "cfglayout.h"
#include "loop.h"
#include "expr.h"

typedef int (*loop_function)	 PARAMS ((rtx insn, void *data));
static int cfg_for_each_insn_in_loop PARAMS ((struct loop *, loop_function, void *));
static int mark_def_varies	 PARAMS ((rtx insn, void *data));
static int collect_stores	 PARAMS ((rtx insn, void *data));
static void note_mem_store	 PARAMS ((rtx, rtx, void *));
static int discover_invariant	 PARAMS ((rtx insn, void *data));
static int not_invariant_rtx	 PARAMS ((rtx *rtxp, void *data));
static bool mark_maybe_invariant_set PARAMS ((rtx, rtx, rtx, struct loop_invariants *));
static bool cbp_enum_p PARAMS ((basic_block, void *));
static bool invariant_in_blocks_p PARAMS ((rtx, basic_block *, int));
static rtx test_for_iteration PARAMS ((struct loop_desc *desc,
				       unsigned HOST_WIDE_INT));
static bool constant_iterations PARAMS ((struct loop_desc *,
					 unsigned HOST_WIDE_INT *,
					 bool *));
static bool simple_loop_exit_p PARAMS ((struct loops *, struct loop *,
					edge, basic_block *,
					struct loop_desc *));
static rtx variable_initial_value PARAMS ((rtx, basic_block, rtx, rtx *));
static rtx variable_initial_values PARAMS ((edge, rtx));
static bool simple_condition_p PARAMS ((struct loop *, basic_block *,
					rtx, struct loop_desc *));
static basic_block simple_increment PARAMS ((struct loops *, struct loop *,
					     basic_block *,
					     struct loop_desc *));

static int
cfg_for_each_insn_in_loop (loop, fun, data)
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

  cfg_for_each_insn_in_loop (loop, mark_def_varies, inv);

  s.unknown_address_altered = false;
  s.unknown_constant_address_altered = false;
  s.inv = inv;
  cfg_for_each_insn_in_loop (loop, collect_stores, &s);

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
  while (cfg_for_each_insn_in_loop (loop, discover_invariant, inv))
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

/* Creates a pre-header for a LOOP.  Returns newly created block.  Unless
   CP_SIMPLE_PREHEADERS is set in FLAGS, we only force LOOP to have single
   entry; otherwise we also force preheader block to have only one successor.
   */
basic_block
create_preheader (loop, dom, flags)
     struct loop *loop;
     sbitmap *dom;
     int flags;
{
  edge e, fallthru;
  basic_block dummy;
  basic_block jump, src;
  struct loop *cloop = NULL;
  int nentry = 0;
  rtx insn;

  cloop = loop->outer;

  for (e = loop->header->pred; e; e = e->pred_next)
    {
      if (e->src == loop->latch)
	continue;
      nentry++;
    }
  if (!nentry)
    abort ();
  if (nentry == 1)
    {
      for (e = loop->header->pred; e->src == loop->latch; e = e->pred_next);
      if (!(flags & CP_SIMPLE_PREHEADERS)
	  || !e->src->succ->succ_next)
	return NULL;
    }

  insn = first_insn_after_basic_block_note (loop->header);
  if (insn)
    insn = PREV_INSN (insn);
  else
    insn = get_last_insn ();
  fallthru = split_block (loop->header, insn);
  dummy = fallthru->src;
  if (loop->latch == loop->header)
    loop->latch = fallthru->dest;
  loop->header = fallthru->dest;

  if (flags & CP_INSIDE_CFGLAYOUT)
    alloc_aux_for_block (fallthru->dest, sizeof (struct reorder_block_def));
  
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
  dummy->count -= e->count;
  fallthru->count -= e->count;
  if (flags & CP_INSIDE_CFGLAYOUT)
    cfg_layout_redirect_edge (e, loop->header);
  else
    {
      jump = redirect_edge_and_branch_force (e, loop->header);
      if (jump)
	{
	  set_immediate_dominator (dom, jump, src);
	  add_bb_to_loop (jump, loop);
	}
    }

  /* Update structures.  */
  redirect_immediate_dominators (dom, dummy, loop->header);
  set_immediate_dominator (dom, loop->header, dummy);
  loop->header->loop_father = loop;
  add_bb_to_loop (dummy, cloop);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Created preheader block for loop %i\n",
	     loop->num);

  return dummy;
}

/* Create preheaders for each loop.  */
void
create_preheaders (loops, flags)
     struct loops *loops;
     int flags;
{
  int i;
  for (i = 1; i < loops->num; i++)
    create_preheader (loops->parray[i], loops->cfg.dom, flags);
}

/* Enumeration predicate for just_once_each_iteration_p.  */
static bool
cbp_enum_p (bb, data)
     basic_block bb;
     void *data;
{
  struct loop *loop = data;
  return bb != loop->header
	 && flow_bb_inside_loop_p (loop, bb);
}

/* Checks whether BB is executed exactly once in each LOOP iteration.  */
bool
just_once_each_iteration_p (loops, loop, bb)
     struct loops *loops;
     struct loop *loop;
     basic_block bb;
{
  basic_block *bbs;
  int i, n;
  edge e;

  /* It must be executed at least once each iteration.  */
  if (!dominated_by_p (loops->cfg.dom, loop->latch, bb))
    return false;

  /* And just once.  */
  if (bb->loop_father != loop)
    return false;

  /* But this was not enough.  We might have some irreducible loop here.  */
  if (bb == loop->header)
    return true;

  bbs = xcalloc (loop->num_nodes, sizeof (basic_block));
  n = dfs_enumerate_from (bb, 0, cbp_enum_p, bbs, loop->num_nodes,
			  (void *) loop);
  for (i = 0; i < n; i++)
    for (e = bbs[i]->succ; e; e = e->succ_next)
      if (e->dest == bb)
	{
	  free (bbs);
	  return false;
	}

  free (bbs);
  return true;
}

/* Counts number of insns inside LOOP.  */
int
num_loop_insns (loop)
     struct loop *loop;
{
  basic_block *bbs, bb;
  int i, ninsns = 0;
  rtx insn;

  bbs = get_loop_body (loop);
  for (i = 0; i < loop->num_nodes; i++)
    {
      bb = bbs[i];
      ninsns++;
      for (insn = bb->head; insn != bb->end; insn = NEXT_INSN (insn))
	ninsns++;
    }
  free(bbs);
  
  return ninsns;
}

/* Checks whether EXPR is invariant in basic blocks BBS.  */
static bool
invariant_in_blocks_p (expr, bbs, nbbs)
    rtx expr;
    basic_block *bbs;
    int nbbs;
{
  int i;

  for (i = 0; i < nbbs; i++)
    if (modified_between_p (expr, bbs[i]->head, NEXT_INSN (bbs[i]->end)))
      return false;

  return true;
}

/* Checks whether CONDITION is a simple comparison in that one of operands
   is register and the other one is invariant in the LOOP. Fills var, lim
   and cond fields in DESC.  */
static bool
simple_condition_p (loop, body, condition, desc)
     struct loop *loop;
     basic_block *body;
     rtx condition;
     struct loop_desc *desc;
{
  rtx op0, op1;
  edge e = loop_preheader_edge (loop);

  /* Check condition.  */
  switch (GET_CODE (condition))
    {
      case EQ:
      case NE:
      case LE:
      case LT:
      case GE:
      case GT:
      case GEU:
      case GTU:
      case LEU:
      case LTU:
	break;
      default:
	return false;
    }

  /* Of integers or pointers.  */
  if (GET_MODE_CLASS (GET_MODE (XEXP (condition, 0))) != MODE_INT
      && GET_MODE_CLASS (GET_MODE (XEXP (condition, 0))) != MODE_PARTIAL_INT)
    return false;

  /* One of operands must be a simple register.  */
  op0 = XEXP (condition, 0);
  op1 = XEXP (condition, 1);
  
  /* One of operands must be invariant.  */
  if (invariant_in_blocks_p (op0, body, loop->num_nodes))
    {
      /* And the other one must be a register.  */
      if (!REG_P (op1))
	return false;
      desc->var = op1;
      desc->lim = op0;

      desc->lim_alts = variable_initial_values (e, op0);

      desc->cond = swap_condition (GET_CODE (condition));
      if (desc->cond == UNKNOWN)
	return false;
      return true;
    }

  /* Check the other operand. */
  if (!invariant_in_blocks_p (op1, body, loop->num_nodes))
    return false;
  if (!REG_P (op0))
    return false;

  desc->var = op0;
  desc->lim = op1;

  desc->lim_alts = variable_initial_values (e, op1);

  desc->cond = GET_CODE (condition);

  return true;
}

/* Checks whether DESC->var is incremented/decremented exactly once each
   iteration.  Fills in DESC->stride and returns block in that DESC->var is
   modified.  */
static basic_block
simple_increment (loops, loop, body, desc)
     struct loops *loops;
     struct loop *loop;
     basic_block *body;
     struct loop_desc *desc;
{
  rtx mod_insn, insn, set, set_src, set_add;
  basic_block mod_bb = NULL;
  int i;

  /* Find insn that modifies var.  */
  mod_insn = NULL;
  for (i = 0; i < loop->num_nodes; i++)
    if (modified_between_p
	(desc->var, body[i]->head, NEXT_INSN (body[i]->end)))
      {
	for (insn = NEXT_INSN (body[i]->head);
	     insn != NEXT_INSN (body[i]->end); insn = NEXT_INSN (insn))
	  if (modified_between_p
	      (desc->var, PREV_INSN (insn), NEXT_INSN (insn)))
	    {
	      if (mod_insn)
		return NULL;
	      else
		mod_insn = insn;
	    }
	mod_bb = body[i];
      }
  if (!mod_insn)
    return NULL;

  /* Check that it is executed exactly once each iteration.  */
  if (!just_once_each_iteration_p (loops, loop, mod_bb))
    return NULL;

  /* mod_insn must be a simple increment/decrement.  */
  set = single_set (mod_insn);
  if (!set)
    return NULL;
  if (!rtx_equal_p (SET_DEST (set), desc->var))
    return NULL;

  set_src = SET_SRC (set);
  if (GET_CODE (set_src) != PLUS)
    return NULL;
  if (!rtx_equal_p (XEXP (set_src, 0), desc->var))
    return NULL;

  /* Set desc->stride.  */
  set_add = XEXP (set_src, 1);
  if (CONSTANT_P (set_add))
    desc->stride = set_add;
  else
    return NULL;

  return mod_bb;
}

/* Tries to find initial value of VAR in INSN.  This value must be valid in
   END_BB too.  If SET_INSN is not NULL, insn in that var is set is placed
   here.  */
static rtx
variable_initial_value (insn, end_bb, var, set_insn)
     rtx insn;
     basic_block end_bb;
     rtx var;
     rtx *set_insn;
{
  basic_block bb;
  rtx set;

  /* Go back through cfg.  */
  bb = BLOCK_FOR_INSN (insn);
  while (1)
    {
      for (; insn != bb->head; insn = PREV_INSN (insn))
	if (modified_between_p (var, PREV_INSN (insn), NEXT_INSN (insn)))
	  break;

      if (insn != bb->head)
	{
	  /* We found place where var is set.  */
	  rtx set_dest;
	  basic_block b;
	  rtx val;
	  rtx note;
          
	  set = single_set (insn);
	  if (!set)
	    return NULL;
	  set_dest = SET_DEST (set);
	  if (!rtx_equal_p (set_dest, var))
	    return NULL;

	  note = find_reg_equal_equiv_note (insn);
	  if (note && GET_CODE (XEXP (note, 0)) != EXPR_LIST)
	    val = XEXP (note, 0);
	  else
	    val = SET_SRC (set);
	  if (modified_between_p (val, insn, NEXT_INSN (bb->end)))
	    return NULL;
	  for (b = end_bb; b != bb; b = b->pred->src)
	    if (modified_between_p (val, b->head, NEXT_INSN (b->end)))
	      return NULL;

	  if (set_insn)
	    *set_insn = insn;
	  return val;
	}

      if (bb->pred->pred_next || bb->pred->src == ENTRY_BLOCK_PTR)
	return NULL;

      bb = bb->pred->src;
      insn = bb->end;
    }

  return NULL;
}

/* Returns list of definitions of initial value of VAR at Edge.  */
static rtx
variable_initial_values (e, var)
     edge e;
     rtx var;
{
  rtx set_insn, list;

  list = alloc_EXPR_LIST (0, copy_rtx (var), NULL);

  if (e->src == ENTRY_BLOCK_PTR)
    return list;

  set_insn = e->src->end;
  while (REG_P (var)
	 && (var = variable_initial_value (set_insn, e->src, var, &set_insn)))
    list = alloc_EXPR_LIST (0, copy_rtx (var), list);

  return list;
}

/* Counts constant number of iterations of the loop described by DESC;
   returns false if impossible.  */
static bool
constant_iterations (desc, niter, may_be_zero)
     struct loop_desc *desc;
     unsigned HOST_WIDE_INT *niter;
     bool *may_be_zero;
{
  rtx test, expr;
  rtx ainit, alim;

  test = test_for_iteration (desc, 0);
  if (test == const0_rtx)
    {
      *niter = 0;
      *may_be_zero = false;
      return true;
    }

  *may_be_zero = (test != const_true_rtx);

  for (ainit = desc->var_alts; ainit; ainit = XEXP (ainit, 1))
    for (alim = desc->lim_alts; alim; alim = XEXP (alim, 1))
      {
	if (!(expr = count_loop_iterations (desc, XEXP (ainit, 0), XEXP (alim, 0))))
	  abort ();
	if (GET_CODE (expr) == CONST_INT)
	  {
	    *niter = INTVAL (expr);
	    return true;
	  }
      }

  return false;
}

/* Return RTX expression representing number of iterations of loop as bounded
   by test described by DESC (in the case loop really has multiple exit
   edges, fewer iterations may happen in the practice).  

   Return NULL if it is unknown.  Additionally the value may be invalid for
   paradoxical loop (lets define paradoxical loops as loops whose test is
   failing at -1th iteration, for instance "for (i=5;i<1;i++);").
   
   These cases needs to be eighter cared by copying the loop test in the front
   of loop or keeping the test in first iteration of loop.
   
   When INIT/LIM are set, they are used instead of var/lim of DESC. */
rtx
count_loop_iterations (desc, init, lim)
     struct loop_desc *desc;
     rtx init;
     rtx lim;
{
  enum rtx_code cond = desc->cond;
  rtx stride = desc->stride;
  rtx mod, exp;

  /* Give up on floating point modes and friends.  It can be possible to do
     the job for constant loop bounds, but it is probably not worthwhile.  */
  if (!INTEGRAL_MODE_P (GET_MODE (desc->var)))
    return NULL;

  init = copy_rtx (init ? init : desc->var);
  lim = copy_rtx (lim ? lim : desc->lim);

  /* Ensure that we always handle the condition to stay inside loop.  */
  if (desc->neg)
    cond = reverse_condition (cond);

  /* Compute absolute value of the difference of initial and final value.  */
  if (INTVAL (stride) > 0)
    {
      /* Bypass nonsential tests.  */
      if (cond == EQ || cond == GE || cond == GT || cond == GEU
	  || cond == GTU)
	return NULL;
      exp = simplify_gen_binary (MINUS, GET_MODE (desc->var),
				 lim, init);
    }
  else
    {
      /* Bypass nonsential tests.  */
      if (cond == EQ || cond == LE || cond == LT || cond == LEU
	  || cond == LTU)
	return NULL;
      exp = simplify_gen_binary (MINUS, GET_MODE (desc->var),
				 init, lim);
      stride = simplify_gen_unary (NEG, GET_MODE (desc->var),
				   stride, GET_MODE (desc->var));
    }

  /* Normalize difference so the value is always first examined
     and later incremented.  */

  if (!desc->postincr)
    exp = simplify_gen_binary (MINUS, GET_MODE (desc->var),
			       exp, stride);

  /* Determine delta caused by exit condition.  */
  switch (cond)
    {
    case NE:
      /* For NE tests, make sure that the iteration variable won't miss
	 the final value.  If EXP mod STRIDE is not zero, then the
	 iteration variable will overflow before the loop exits, and we
	 can not calculate the number of iterations easilly.  */
      if (stride != const1_rtx
	  && (simplify_gen_binary (UMOD, GET_MODE (desc->var), exp, stride)
              != const0_rtx))
	return NULL;
      break;
    case LT:
    case GT:
    case LTU:
    case GTU:
      break;
    case LE:
    case GE:
    case LEU:
    case GEU:
      exp = simplify_gen_binary (PLUS, GET_MODE (desc->var),
				 exp, const1_rtx);
      break;
    default:
      abort ();
    }

  if (stride != const1_rtx)
    {
      /* Number of iterations is now (EXP + STRIDE - 1 / STRIDE),
	 but we need to take care for overflows.   */

      mod = simplify_gen_binary (UMOD, GET_MODE (desc->var), exp, stride);

      /* This is dirty trick.  When we can't compute number of iterations
	 to be constant, we simply ignore the possible overflow, as
	 runtime unroller always use power of 2 amounts and does not
	 care about possible lost bits.  */

      if (GET_CODE (mod) != CONST_INT)
	{
	  rtx stridem1 = simplify_gen_binary (PLUS, GET_MODE (desc->var),
					      stride, constm1_rtx);
	  exp = simplify_gen_binary (PLUS, GET_MODE (desc->var),
				     exp, stridem1);
	  exp = simplify_gen_binary (UDIV, GET_MODE (desc->var), exp, stride);
	}
      else
	{
	  exp = simplify_gen_binary (UDIV, GET_MODE (desc->var), exp, stride);
	  if (mod != const0_rtx)
	    exp = simplify_gen_binary (PLUS, GET_MODE (desc->var),
				       exp, const1_rtx);
	}
    }

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, ";  Number of iterations: ");
      print_simple_rtl (rtl_dump_file, exp);
      fprintf (rtl_dump_file, "\n");
    }

  return exp;
}

/* Return simplified RTX expression representing the value of test
   described of DESC at given iteration of loop.  */

static rtx
test_for_iteration (desc, iter)
     struct loop_desc *desc;
     unsigned HOST_WIDE_INT iter;
{
  enum rtx_code cond = desc->cond;
  rtx exp = XEXP (desc->var_alts, 0);
  rtx addval;

  /* Give up on floating point modes and friends.  It can be possible to do
     the job for constant loop bounds, but it is probably not worthwhile.  */
  if (!INTEGRAL_MODE_P (GET_MODE (desc->var)))
    return NULL;

  /* Ensure that we always handle the condition to stay inside loop.  */
  if (desc->neg)
    cond = reverse_condition (cond);

  /* Compute the value of induction variable.  */
  addval = simplify_gen_binary (MULT, GET_MODE (desc->var),
				desc->stride,
				gen_int_mode (desc->postincr
					      ? iter : iter + 1,
					      GET_MODE (desc->var)));
  exp = simplify_gen_binary (PLUS, GET_MODE (desc->var), exp, addval);
  /* Test at given condtion.  */
  exp = simplify_gen_relational (cond, SImode,
				 GET_MODE (desc->var), exp, desc->lim);

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file,
	       ";  Conditional to continue loop at %i th iteration: ", iter);
      print_simple_rtl (rtl_dump_file, exp);
      fprintf (rtl_dump_file, "\n");
    }
  return exp;
}


/* Tests whether exit at EXIT_EDGE from LOOP is simple.  Returns simple loop
   description joined to it in in DESC.  */
static bool
simple_loop_exit_p (loops, loop, exit_edge, body, desc)
     struct loops *loops;
     struct loop *loop;
     edge exit_edge;
     basic_block *body;
     struct loop_desc *desc;
{
  basic_block mod_bb, exit_bb;
  int fallthru_out;
  rtx condition;
  edge ei, e;

  exit_bb = exit_edge->src;

  fallthru_out = (exit_edge->flags & EDGE_FALLTHRU);

  if (!exit_bb)
    return false;

  /* It must be tested once during any iteration.  */
  if (!just_once_each_iteration_p (loops, loop, exit_bb))
    return false;

  /* It must end in a simple conditional jump.  */
  if (!any_condjump_p (exit_bb->end))
    return false;

  ei = exit_bb->succ;
  if ((ei->flags & EDGE_FALLTHRU) && fallthru_out)
    ei = exit_bb->succ->succ_next;

  desc->out_edge = exit_edge;
  desc->in_edge = ei;

  /* Condition must be a simple comparison in that one of operands
     is register and the other one is invariant.  */
  if (!(condition = get_condition (exit_bb->end, NULL)))
    return false;

  if (!simple_condition_p (loop, body, condition, desc))
    return false;

  /*  Var must be simply incremented or decremented in exactly one insn that
     is executed just once every iteration.  */
  if (!(mod_bb = simple_increment (loops, loop, body, desc)))
    return false;

  /* OK, it is simple loop.  Now just fill in remaining info.  */
  desc->postincr = !dominated_by_p (loops->cfg.dom, exit_bb, mod_bb);
  desc->neg = !fallthru_out;

  /* Find initial value of var.  */
  e = loop_preheader_edge (loop);
  desc->var_alts = variable_initial_values (e, desc->var);

  /* Number of iterations. */
  if (!count_loop_iterations (desc, NULL, NULL))
    return false;
  desc->const_iter =
    constant_iterations (desc, &desc->niter, &desc->may_be_zero);
  return true;
}

/* Tests whether LOOP is simple for loop.  Returns simple loop description
   in DESC.  */
bool
simple_loop_p (loops, loop, desc)
     struct loops *loops;
     struct loop *loop;
     struct loop_desc *desc;
{
  int i;
  basic_block *body;
  edge e;
  struct loop_desc act;
  bool any = false;
  
  body = get_loop_body (loop);

  for (i = 0; i < loop->num_nodes; i++)
    {
      for (e = body[i]->succ; e; e = e->succ_next)
	if (!flow_bb_inside_loop_p (loop, e->dest)
	    && simple_loop_exit_p (loops, loop, e, body, &act))
	  {
	    /* Prefer constant iterations; the less the better.  */
	    if (!any)
	      any = true;
	    else if (!act.const_iter
		     || (desc->const_iter && act.niter >= desc->niter))
	      continue;
	    *desc = act;
	  }
    }

  if (rtl_dump_file && any)
    {
      fprintf (rtl_dump_file, "; Simple loop %i\n", loop->num);
      if (desc->postincr)
	fprintf (rtl_dump_file,
		 ";  does postincrement after loop exit condition\n");

      fprintf (rtl_dump_file, ";  Induction variable:");
      print_simple_rtl (rtl_dump_file, desc->var);
      fputc ('\n', rtl_dump_file);

      fprintf (rtl_dump_file, ";  Initial values:");
      print_simple_rtl (rtl_dump_file, desc->var_alts);
      fputc ('\n', rtl_dump_file);

      fprintf (rtl_dump_file, ";  Stride:");
      print_simple_rtl (rtl_dump_file, desc->stride);
      fputc ('\n', rtl_dump_file);

      fprintf (rtl_dump_file, ";  Compared with:");
      print_simple_rtl (rtl_dump_file, desc->lim);
      fputc ('\n', rtl_dump_file);

      fprintf (rtl_dump_file, ";  Alternative values:");
      print_simple_rtl (rtl_dump_file, desc->lim_alts);
      fputc ('\n', rtl_dump_file);

      fprintf (rtl_dump_file, ";  Exit condition:");
      if (desc->neg)
	fprintf (rtl_dump_file, "(negated)");
      fprintf (rtl_dump_file, "%s\n", GET_RTX_NAME (desc->cond));
      fputc ('\n', rtl_dump_file);
    }

  free (body);
  return any;
}

/* Functions below are expected to be called between
   cfg_layout_initialize and cfg_layout_finalize.  */

/* A quite stupid function to put INSNS on E. They are supposed to form
   just one basic block. Jumps out are not handled, so cfg do not have to
   be ok after this function.  */
basic_block
loop_split_edge_with (e, insns, loops)
     edge e;
     rtx insns;
     struct loops *loops;
{
  basic_block src, dest, new_bb;
  struct loop *loop_c;
  edge new_e;
  
  src = e->src;
  dest = e->dest;

  loop_c = find_common_loop (src->loop_father, dest->loop_father);

  /* Create basic block for it.  */

  new_bb = create_basic_block (NULL_RTX, NULL_RTX, EXIT_BLOCK_PTR->prev_bb);
  add_bb_to_loop (new_bb, loop_c);
  new_bb->flags = BB_SUPERBLOCK;

  new_e = make_edge (new_bb, dest, EDGE_FALLTHRU);
  new_e->probability = REG_BR_PROB_BASE;
  new_e->count = e->count;

  new_bb->count = e->count;

  new_bb->frequency = EDGE_FREQUENCY (e);
  cfg_layout_redirect_edge (e, new_bb);

  alloc_aux_for_block (new_bb, sizeof (struct reorder_block_def));
  if (insns)
    {
      start_sequence ();
      emit_insn (insns);
      insns = get_insns ();
      end_sequence ();
      emit_insns_after (insns, new_bb->end);
    }

  set_immediate_dominator (loops->cfg.dom, new_bb, src);
  set_immediate_dominator (loops->cfg.dom, dest,
    recount_dominator (loops->cfg.dom, dest));

  if (dest->loop_father->latch == src)
    dest->loop_father->latch = new_bb;
  
  return new_bb;
}

/* Forces all loop latches to have only single successor.  */
void
force_single_succ_latches (loops)
     struct loops *loops;
{
  int i;
  struct loop *loop;
  edge e;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop->latch->succ->succ_next)
	continue;
 
      for (e = loop->header->pred; e->src != loop->latch; e = e->pred_next);
      loop_split_edge_with (e, NULL_RTX, loops);
    }
}
