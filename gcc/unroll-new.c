/* Loop unrolling and peeling.
   Copyright (C) 2002 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
#include "obstack.h"
#include "function.h"
#include "expr.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-config.h"
#include "regs.h"
#include "recog.h"
#include "flags.h"
#include "real.h"
#include "cselib.h"
#include "except.h"
#include "toplev.h"
#include "predict.h"
#include "insn-flags.h"
#include "cfglayout.h"
#include "loop.h"
#include "params.h"
#include "output.h"

static basic_block simple_exit PARAMS ((struct loops *, struct loop *,
					basic_block *, int *));
static bool simple_condition_p PARAMS ((struct loop *, basic_block *,
					rtx, struct loop_desc *));
static basic_block simple_increment PARAMS ((struct loops *, struct loop *,
					     basic_block *,
					     struct loop_desc *));
static rtx variable_initial_value PARAMS ((struct loop *, rtx));
static bool simple_loop_p PARAMS ((struct loops *, struct loop *,
				   struct loop_desc *));
static rtx count_loop_iterations PARAMS ((struct loop_desc *));
static void unroll_or_peel_loop PARAMS ((struct loops *, struct loop *, int));
static bool peel_loop_simple PARAMS ((struct loops *, struct loop *, int));
static bool peel_loop_completely PARAMS ((struct loops *, struct loop *,
					  struct loop_desc *));
static bool unroll_loop_stupid PARAMS ((struct loops *, struct loop *, int));
static bool unroll_loop_constant_iterations PARAMS ((struct loops *,
						     struct loop *,
						     int, struct loop_desc *));
static bool unroll_loop_runtime_iterations PARAMS ((struct loops *,
						    struct loop *, int,
						    struct loop_desc *));
static rtx test_for_iteration PARAMS ((struct loop_desc *desc,
				       unsigned HOST_WIDE_INT));

/* Unroll and peel (depending on FLAGS) LOOPS.  */
void
unroll_and_peel_loops (loops, flags)
     struct loops *loops;
     int flags;
{
  struct loop *loop = loops->tree_root, *next;

  while (loop->inner)
    loop = loop->inner;

  /* Scan the loops, inner ones first.  */
  while (loop != loops->tree_root)
    {
      if (loop->next)
	{
	  next = loop->next;
	  while (next->inner)
	    next = next->inner;
	}
      else
	next = loop->outer;

      unroll_or_peel_loop (loops, loop, flags);
#ifdef ENABLE_CHECKING
      verify_dominators ();
      verify_loop_structure (loops, VLS_FOR_LOOP_NEW);
#endif
      loop = next;
    }
}

/* Checks whether LOOP (consisting of BODY -- just not to have to find
   it again and again) have simple exit (i.e. exit is in exactly one block
   that is executed in every iteration exactly once). FALLTRHU_OUT
   is set if the exit edge is fallthru.  Exit block is returned.  */
static basic_block
simple_exit (loops, loop, body, fallthru_out)
     struct loops *loops;
     struct loop *loop;
     basic_block *body;
     int *fallthru_out;
{
  basic_block exit_bb;
  int i;
  edge e;

  /* Loop must have single exit only.  */
  exit_bb = NULL;
  for (i = 0; i < loop->num_nodes; i++)
    for (e = body[i]->succ; e; e = e->succ_next)
      if (!flow_bb_inside_loop_p (loop, e->dest))
	{
	  if (exit_bb)
	    return NULL;
	  else
	    exit_bb = body[i];
	  *fallthru_out = (e->flags & EDGE_FALLTHRU);
	}
  if (!exit_bb)
    return NULL;

  /* And it must be tested once during any iteration.  */
  if (!just_once_each_iteration_p (loops, loop, exit_bb))
    return NULL;

  /* It must end in a simple conditional jump.  */
  if (!any_condjump_p (exit_bb->end))
    return NULL;

  return exit_bb;
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
  int i;

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
  for (i = 0; i < loop->num_nodes; i++)
    if (modified_between_p (op0, body[i]->head, NEXT_INSN (body[i]->end)))
      break;
  if (i == loop->num_nodes)
    {
      /* And the other one must be a register.  */
      if (!REG_P (op1))
	return false;
      desc->var = op1;
      desc->lim = op0;
      desc->cond = swap_condition (GET_CODE (condition));
      if (desc->cond == UNKNOWN)
	return false;
      return true;
    }

  /* Check the other operand. */
  for (i = 0; i < loop->num_nodes; i++)
    if (modified_between_p (op1, body[i]->head, NEXT_INSN (body[i]->end)))
      break;
  if (i != loop->num_nodes)
    return false;
  if (!REG_P (op0))
    return false;

  desc->var = op0;
  desc->lim = op1;
  desc->cond = GET_CODE (condition);

  return true;
}

/* Checks whether DESC->var is incremented/decremented exactly once each
   iteration.  Fills in DESC->grow and returns block in that DESC->var is
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
    if (modified_between_p (desc->var, body[i]->head, NEXT_INSN (body[i]->end)))
      {
	for (insn = NEXT_INSN (body[i]->head);
	     insn != NEXT_INSN (body[i]->end);
	     insn = NEXT_INSN (insn))
	  if (modified_between_p (desc->var, PREV_INSN (insn), NEXT_INSN (insn)))
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

  /* Set desc->grow.  */
  set_add = XEXP (set_src, 1);
  if (set_add == const1_rtx)
    desc->grow = 1;
  else if (set_add == constm1_rtx)
    desc->grow = 0;
  else
    return NULL;

  return mod_bb;
}

/* Tries to find initial value of VAR in LOOP.  */
static rtx
variable_initial_value (loop, var)
     struct loop *loop;
     rtx var;
{
  edge e;
  basic_block bb;
  rtx set, insn;

  /* Go back through cfg.  */
  e = loop_preheader_edge (loop);
  for (bb = e->src; bb->pred; bb = bb->pred->src)
    {
      for (insn = bb->end;
	   insn != bb->head;
	   insn = PREV_INSN (insn))
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
	  if (note)
	    val = XEXP (note, 0);
	  else
	    val = SET_SRC (set);
	  if (modified_between_p (val, insn, bb->end))
	    return NULL;
	  for (b = e->src; b != bb; b = b->pred->src)
	    if (modified_between_p (val, b->head, b->end))
	      return NULL;
	  return val;
	}
      if (bb->pred->pred_next)
	return NULL;
    }

  return NULL;
}

/* Tests whether LOOP is simple for loop.  Returns simple loop description
   in DESC.  */
static bool
simple_loop_p (loops, loop, desc)
     struct loops *loops;
     struct loop *loop;
     struct loop_desc *desc;
{
  basic_block exit_bb, *body, mod_bb;
  int fallthru_out;
  rtx condition;
  edge ei, eo, tmp;

  body = get_loop_body (loop);

  /* There must be only a single exit from loop.  */
  if (!(exit_bb = simple_exit (loops, loop, body, &fallthru_out)))
    goto ret_false;

  ei = exit_bb->succ;
  eo = exit_bb->succ->succ_next;
  if ((ei->flags & EDGE_FALLTHRU) && fallthru_out)
    {
      tmp = ei;
      ei = eo;
      eo = tmp;
    }
  desc->out_edge = eo;
  desc->in_edge = ei;

  /* Condition must be a simple comparison in that one of operands
     is register and the other one is invariant.  */
  if (!(condition = get_condition (exit_bb->end, NULL)))
    goto ret_false;
 
  if (!simple_condition_p (loop, body, condition, desc))
    goto ret_false;
 
  /*  Var must be simply incremented or decremented in exactly one insn that
      is executed just once every iteration.  */
  if (!(mod_bb = simple_increment (loops, loop, body, desc)))
    goto ret_false;

  /* OK, it is simple loop.  Now just fill in remaining info.  */
  desc->postincr = !dominated_by_p (loops->cfg.dom, exit_bb, mod_bb);
  desc->neg = !fallthru_out;

  /* Find initial value of var.  */
  desc->init = variable_initial_value (loop, desc->var);

  /* Find numeric values of bounds.  */
  if (GET_CODE (desc->lim) == CONST_INT)
    desc->lim_n = INTVAL (desc->lim);
  if (desc->init && GET_CODE (desc->init) == CONST_INT)
    desc->init_n = INTVAL (desc->init);

  desc->const_iter = GET_CODE (desc->lim) == CONST_INT
  		     && desc->init
		     && GET_CODE (desc->init) == CONST_INT;

  free (body);

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "; Simple loop %i\n", loop->num);
      if (desc->postincr)
	fprintf (rtl_dump_file,
		 ";  does postincrement after loop exit condition\n");
      if (desc->var)
	{
	  fprintf (rtl_dump_file, ";  Induction variable:");
	  print_simple_rtl (rtl_dump_file, desc->var);
	  fputc ('\n', rtl_dump_file);
	}
      fprintf (rtl_dump_file,
	       desc->grow ? ";  Counter grows\n": ";  Counter decreases\n");
      if (desc->init)
	{
	  fprintf (rtl_dump_file, ";  Initial value:");
	  print_simple_rtl (rtl_dump_file, desc->init);
	  fputc ('\n', rtl_dump_file);
	}
      if (desc->lim)
	{
	  fprintf (rtl_dump_file, ";  Compared with:");
	  print_simple_rtl (rtl_dump_file, desc->lim);
	  fputc ('\n', rtl_dump_file);
	}
      if (desc->cond)
	{
	  fprintf (rtl_dump_file, ";  Exit condtion:");
	  if (desc->neg)
	    fprintf (rtl_dump_file, "(negated)");
	  fprintf (rtl_dump_file, "%s\n", GET_RTX_NAME (desc->cond));
	  fputc ('\n', rtl_dump_file);
	}
    }
  return true;

  ret_false:
  free (body);
  return false;
}

/* Return RTX expression representing number of iterations of loop as bounded
   by test described by DESC (in the case loop really has multiple exit
   edges, fewer iterations may happen in the practice).  

   Return NULL if it is unknown.  Additionally the value may be invalid for
   paradoxical loop (lets define paradoxical loops as loops whose test is
   failing at -1th iteration, for instance "for (i=5;i<1;i++);").
   
   These cases needs to be eighter cared by copying the loop test in the front
   of loop or keeping the test in first iteration of loop.  */
static rtx
count_loop_iterations (desc)
     struct loop_desc *desc;
{
  int delta;
  enum rtx_code cond = desc->cond;
  rtx exp = desc->init ? copy_rtx (desc->init) : desc->var;

  /* Give up on floating point modes and friends.  It can be possible to do
     the job for constant loop bounds, but it is probably not worthwhile.  */
  if (!INTEGRAL_MODE_P (GET_MODE (desc->var)))
    return NULL;

  /* Ensure that we always handle the condition to stay inside loop.  */
  if (desc->neg)
    cond = reverse_condition (cond);

  /* Compute absolute value of the difference of initial and final value.  */
  if (desc->grow)
    {
      /* Bypass nonsential tests.  */
      if (cond == EQ || cond == GE || cond == GT || cond == GEU
	  || cond == GTU)
	return NULL;
      exp = simplify_gen_binary (MINUS, GET_MODE (desc->var),
				 copy_rtx (desc->lim), exp);
    }
  else
    {
      /* Bypass nonsential tests.  */
      if (cond == EQ || cond == LE || cond == LT || cond == LEU
	  || cond == LTU)
	return NULL;
      exp = simplify_gen_binary (MINUS, GET_MODE (desc->var),
				 exp, copy_rtx (desc->lim));
    }

  delta = 0;
  if (!desc->postincr)
    delta--;

  /* Determine delta caused by exit condition.  */
  switch (cond)
    {
    case NE:
    case LT:
    case GT:
    case LTU:
    case GTU:
      break;
    case LE:
    case GE:
    case LEU:
    case GEU:
      delta++;
      break;
    default:
      abort ();
    }

  if (delta)
    exp = simplify_gen_binary (PLUS, GET_MODE (desc->var),
			       exp, GEN_INT (delta));

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
  rtx exp = desc->init ? copy_rtx (desc->init) : desc->var;
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
				desc->grow ? const1_rtx : constm1_rtx,
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

/* Peel NPEEL iterations from LOOP, remove exit edges (and cancel the loop
   completely).  */
static bool
peel_loop_completely (loops, loop, desc)
     struct loops *loops;
     struct loop *loop;
     struct loop_desc *desc;
{
  sbitmap wont_exit;
  unsigned HOST_WIDE_INT npeel;
  edge e;
  rtx exp;

  exp = count_loop_iterations (desc);
  if (!exp || GET_CODE (exp) != CONST_INT)
    abort ();
  npeel = INTVAL (exp);

  wont_exit = sbitmap_alloc (npeel + 2);
  sbitmap_ones (wont_exit);
  RESET_BIT (wont_exit, 0);
  RESET_BIT (wont_exit, npeel + 1);

  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
	loops, npeel + 1, wont_exit, DLTHE_FLAG_ALL))
    abort ();

  free (wont_exit);

  /* Now remove the loop.  */
  for (e = RBI (desc->in_edge->src)->copy->succ;
       e->dest != RBI (desc->in_edge->dest)->copy;
       e = e->succ_next);

  remove_path (loops, e);

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Peeled loop completely, %d times\n",npeel);

  return true;
}

/* Unroll LOOP with constant number of iterations described by DESC.
   MAX_UNROLL is maximal number of allowed unrollings.  */
static bool
unroll_loop_constant_iterations (loops, loop, max_unroll, desc)
     struct loops *loops;
     struct loop *loop;
     int max_unroll;
     struct loop_desc *desc;
{
  unsigned HOST_WIDE_INT niter, exit_mod;
  sbitmap wont_exit;
  rtx exp;

  /* Normalization.  */
  exp = count_loop_iterations (desc);
  if (!exp || GET_CODE (exp) != CONST_INT)
    abort ();
  niter = INTVAL (exp);

  if (niter <= (unsigned) max_unroll)
    abort ();  /* Should get into peeling instead.  */

  wont_exit = sbitmap_alloc (max_unroll + 1);
  sbitmap_ones (wont_exit);
  exit_mod = niter % (max_unroll + 1);

  if (desc->postincr)
    {
      /* Counter is incremented after the exit test; leave exit test
	 in the first copy.  */

      if (rtl_dump_file)
        fprintf (rtl_dump_file, ";; Condition on beginning of loop.\n");

      /* Peel exit_mod iterations.  */
      RESET_BIT (wont_exit, 0);

      if (exit_mod
	  && !duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, exit_mod, wont_exit, DLTHE_FLAG_ALL))
	abort ();
    }
  else
    {
      /* Leave exit test in last copy.  */

      if (rtl_dump_file)
        fprintf (rtl_dump_file, ";; Condition on end of loop.\n");

      /* We know that niter >= max_unroll + 1; so we do not need to care of
	 case when we would exit before reaching the loop.  So just peel
	 exit_mod + 1 iterations.
	 */
      if (exit_mod != (unsigned) max_unroll)
	{
	  RESET_BIT (wont_exit, 0);

	  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, exit_mod + 1, wont_exit, DLTHE_FLAG_ALL))
	    abort ();

	  SET_BIT (wont_exit, 0);
	}

      RESET_BIT (wont_exit, max_unroll);
    }

  /* Now unroll the loop.  */
  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
		loops, max_unroll, wont_exit, DLTHE_FLAG_ALL))
    abort ();

  free (wont_exit);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unrolled loop %d times, constant # of iterations\n",max_unroll);
  
  return true;
}

/* Unroll LOOP for that we are able to count number of iterations in runtime.
   MAX_UNROLL is maximal number of allowed unrollings.  DESC describes the loop.  */
static bool
unroll_loop_runtime_iterations (loops, loop, max_unroll, desc)
     struct loops *loops;
     struct loop *loop;
     int max_unroll;
     struct loop_desc *desc;
{
  rtx niter, init_code, branch_code;
  rtx loop_beg_label;
  int i;
  basic_block fake, preheader, *body, dom;
  edge e;
  sbitmap wont_exit;
  int may_exit_copy, n_peel;

  /* Force max_unroll + 1 to be power of 2.  */
  for (i = 1; 2 * i <= max_unroll + 1; i *= 2);
  max_unroll = i - 1;

  /* Normalization.  */
  start_sequence ();
  niter = count_loop_iterations (desc);
  if (!niter)
    abort ();
  niter = force_operand (niter, NULL);

  /* Count modulo by ANDing it with max_unroll.  */
  niter = expand_simple_binop (GET_MODE (desc->var), AND,
			       niter,
			       GEN_INT (max_unroll),
			       NULL_RTX, 0, OPTAB_LIB_WIDEN);

  if (desc->postincr)
    {
      /* Leave exit in first copy.  */
      may_exit_copy = 0;
      n_peel = max_unroll;
    }
  else
    {
      /* Leave exit in last copy.  */
      may_exit_copy = max_unroll;
      niter = expand_simple_binop (GET_MODE (desc->var), PLUS,
				   niter,
				   const1_rtx, NULL_RTX, 0, OPTAB_LIB_WIDEN);
      n_peel = max_unroll + 1;
      /* First check for zero is obviously unnecessary now; it might seem
         we could do better by increasing it before AND; but we must have
         guaranteed that exit condition will be checked in first iteration,
         so that we won't miscompile loop with negative number of iterations.  */
    }

  niter = expand_simple_binop (GET_MODE (desc->var), PLUS,
			       niter,
			       const1_rtx, NULL_RTX, 0, OPTAB_LIB_WIDEN);

  init_code = gen_sequence ();
  end_sequence ();

  /* Precondition the loop.  */
  loop_split_edge_with (loop_preheader_edge (loop), init_code, loops);

  /* Fake block, to record edges we need to redirect.  */
  fake = create_basic_block (n_basic_blocks, NULL, NULL);
  loop_beg_label = block_label (fake);

  for (i = 0; i < n_peel; i++)
    {
      start_sequence ();
      niter = expand_simple_binop (GET_MODE (desc->var), MINUS,
				   niter, const1_rtx,
				   NULL_RTX, 0, OPTAB_LIB_WIDEN);
      do_compare_rtx_and_jump (copy_rtx (niter), const0_rtx, EQ, 0,
			       GET_MODE (desc->var), NULL_RTX, NULL_RTX,
			       loop_beg_label);
      JUMP_LABEL (get_last_insn ()) = loop_beg_label;
      LABEL_NUSES (loop_beg_label)++;
      branch_code = gen_sequence ();
      end_sequence ();

      preheader =
	loop_split_edge_with (loop_preheader_edge (loop), branch_code, loops);
      make_edge (preheader, fake, 0);

      wont_exit = sbitmap_alloc (2);
      sbitmap_zero (wont_exit);
      /* We must be a bit careful here, as we might have negative
         number of iterations.  Also, in case of postincrement we do
         not know whether we should not exit before reaching the loop.  */
      if (desc->postincr && (i || desc->cond == NE))
	SET_BIT (wont_exit, 1);

      if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
					  loops, 1, wont_exit,
					  DLTHE_FLAG_ALL))
	abort ();
      free (wont_exit);
    }

  /* Now redirect the edges from fake.  */
  preheader =
    loop_split_edge_with (loop_preheader_edge (loop), NULL_RTX, loops);
  loop_beg_label = block_label (preheader);

  for (e = fake->pred; e; e = fake->pred)
    {
      if (!redirect_edge_and_branch (e, preheader))
	abort ();
    }

  dom = recount_dominator (loops->cfg.dom, preheader);
  set_immediate_dominator (loops->cfg.dom, preheader, dom);

  if (desc->cond != NE || !desc->postincr)
    {
      /* Recount dominators for outer blocks.  */
      body = get_loop_body (loop);
      for (i = 0; i < loop->num_nodes; i++)
	for (e = body[i]->succ; e; e = e->succ_next)
	  {
	    if (flow_bb_inside_loop_p (loop, e->dest))
	      continue;
	    set_immediate_dominator (loops->cfg.dom, e->dest,
				     nearest_common_dominator (loops->cfg.dom,
							       e->dest, dom));
	  }
      free (body);
    }

  /* Get rid of fake.  */
  flow_delete_block (fake);

  /* And unroll loop.  */

  wont_exit = sbitmap_alloc (max_unroll + 1);
  sbitmap_ones (wont_exit);
  RESET_BIT (wont_exit, may_exit_copy);

  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
				      loops, max_unroll, wont_exit,
				      DLTHE_FLAG_ALL))
    abort ();

  free (wont_exit);
  if (rtl_dump_file)
    fprintf (rtl_dump_file,
	     ";; Unrolled loop %d times, counting # of iterations in runtime\n",
	     max_unroll);

  return true;
}
  
/* Peel a LOOP.  Returs 0 if impossible, 1 otherwise.  */
static bool
peel_loop_simple (loops, loop, npeel)
     struct loops *loops;
     struct loop *loop;
     int npeel;
{
  sbitmap wont_exit;

  wont_exit = sbitmap_alloc (npeel + 1);
  sbitmap_zero (wont_exit);

  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, npeel, wont_exit, DLTHE_FLAG_ALL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Peeling unsuccessful\n");
      return false;
    }
  
  free (wont_exit);

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Peeling loop %d times\n", npeel);

  return true;
}
  
/* Unroll a LOOP.  Returs 0 if impossible, 1 otherwise.  */
static bool
unroll_loop_stupid (loops, loop, nunroll)
     struct loops *loops;
     struct loop *loop;
     int nunroll;
{
  sbitmap wont_exit;

  wont_exit = sbitmap_alloc (nunroll + 1);
  sbitmap_zero (wont_exit);

  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
		loops, nunroll, wont_exit, DLTHE_FLAG_ALL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";;  Not unrolling loop, can't duplicate\n");
      return false;
    }

  free (wont_exit);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unrolled loop %d times\n", nunroll);
	  
  return true;
}

/* Unroll or peel (depending on FLAGS) LOOP.  */
static void
unroll_or_peel_loop (loops, loop, flags)
     struct loops *loops;
     struct loop *loop;
     int flags;
{
  int ninsns;
  unsigned HOST_WIDE_INT nunroll, npeel, niter = 0;
  struct loop_desc desc;
  bool simple, exact;

  /* Do not unroll/peel cold areas.  */
  if (!maybe_hot_bb_p (loop->header))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling/peeling loop, cold area\n");
      return;
    }

  /* Can the loop be manipulated?  */
  if (!can_duplicate_loop_p (loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 ";; Not unrolling/peeling loop, cannot duplicate\n");
      return;
    }

  /* Only peel innermost loops.  */
  if (loop->inner)
    {
      if ((flags & UAP_PEEL) && rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not peeling loop, not innermost loop\n");
      flags &= ~UAP_PEEL;
    }

  /* Count maximal number of unrollings/peelings.  */
  ninsns = num_loop_insns (loop);

  /* npeel = number of iterations to peel. */
  npeel = PARAM_VALUE (PARAM_MAX_PEELED_INSNS) / ninsns - 1;
  if (npeel > (unsigned) PARAM_VALUE (PARAM_MAX_PEEL_TIMES))
    npeel = PARAM_VALUE (PARAM_MAX_PEEL_TIMES);

  /* nunroll = total number of copies of the original loop body in
     unrolled loop (i.e. if it is 2, we have to duplicate loop body once.  */
  nunroll = PARAM_VALUE (PARAM_MAX_UNROLLED_INSNS) / ninsns;
  if (nunroll > (unsigned) PARAM_VALUE (PARAM_MAX_UNROLL_TIMES))
    nunroll = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);

  /* Skip big loops.  */
  if (nunroll <= 1)
    {
      if ((flags & UAP_UNROLL) && rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling loop, is too big\n");
      flags &= ~(UAP_UNROLL | UAP_UNROLL_ALL);
    }

  if (npeel <= 0)
    {
      if ((flags & UAP_PEEL) && rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not peeling loop, is too big\n");
      flags &= ~UAP_PEEL;
    }

  /* Shortcut.  */
  if (!flags)
    return;

  /* Check for simple loops.  */
  simple = simple_loop_p (loops, loop, &desc);
  if (!simple && !(flags & UAP_UNROLL_ALL))
    {
      if ((flags & UAP_UNROLL) && rtl_dump_file)
	fprintf (rtl_dump_file, ";;  Not unrolling loop, isn't simple\n");
      flags &= ~UAP_UNROLL;
    }

  /* Try to guess number of iterations.  */
  exact = false;
  if (simple)
    {
      rtx exp = count_loop_iterations (&desc);
      rtx test = test_for_iteration (&desc, 0);

      /* Bypass loops iterating 0 times.  These should really be
         elliminated earlier, but we may create them by other transformations.
         CSE will kill them later.  */

      if (test && test == const0_rtx)
	{
	  if ((flags & UAP_UNROLL) && rtl_dump_file)
	    fprintf (rtl_dump_file, ";;  Not unrolling nor peeling loop, iterates 0 times\n");
	}

      /* Loop with constant number of iterations?  */
      if (!exp)
	simple = false;
      else if (GET_CODE (exp) == CONST_INT
	       && test && test == const_true_rtx)
	exact = true, niter = INTVAL (exp);
      else
	exact = false;
    }

  if (!exact)
    {
      /* Use profile information.  */
      niter = expected_loop_iterations (loop);
      if (flag_branch_probabilities)
	exact = true;
    }

  if (exact)
    {
      /* If estimate is good, use it to decide and bound number of peelings.  */
      if (niter > npeel)
	{
	  if ((flags & UAP_PEEL) && rtl_dump_file)
	    fprintf (rtl_dump_file,
		     ";; Not peeling loop, rolls too much (%d iterations > %d [maximum peelings])\n",
		     niter, npeel);
	  flags &= ~UAP_PEEL;
	}
      npeel = niter;

      /* And unrollings.  */
      if (niter < 2 * nunroll)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, ";; Not unrolling loop, doesn't roll\n");
	  flags &= ~(UAP_UNROLL | UAP_UNROLL_ALL);
	}
    }

  /* We might have lost simpleness when counting loop iterations.  */
  if (!simple && !(flags & UAP_UNROLL_ALL))
    {
      if ((flags & UAP_UNROLL) && rtl_dump_file)
	fprintf (rtl_dump_file, ";;  Not unrolling loop, isn't simple\n");
      flags &= ~UAP_UNROLL;
    }

  /* Shortcut.  */
  if (!flags)
    return;

  /* If we still may both unroll and peel, then unroll.  */
  if ((flags & UAP_UNROLL) && (flags & UAP_PEEL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";;  Not peelling loop, unrolling instead\n");
      flags &= ~UAP_PEEL;
    }

  /* Now we have several cases:  */
  if (flags & UAP_UNROLL)
    {
      /* Unrolling:  */

      if (simple)
	{
	  if (desc.const_iter)
	    /* Loops with constant number of iterations.  */
	    unroll_loop_constant_iterations (loops, loop, (int) nunroll,
					     &desc);
	  else
	    /* Loops with countable number of iterations.  */
	    unroll_loop_runtime_iterations (loops, loop, (int) nunroll,
					    &desc);
	}
      else
	/* Stupid unrolling.  */
	unroll_loop_stupid (loops, loop, (int) nunroll);
    }
  else
    {
      /* Peeling:  */

      if (simple && desc.const_iter)
	/* Peel and remove the loop completely.  */
	peel_loop_completely (loops, loop, &desc);
      else
	/* Simple loop peeling.  */
	peel_loop_simple (loops, loop, (int) npeel);
    }

  return;
}
