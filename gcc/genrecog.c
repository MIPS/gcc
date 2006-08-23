/* Generate code from machine description to recognize rtl as insns.
   Copyright (C) 1987, 1988, 1992, 1993, 1994, 1995, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */


/* This program is used to produce insn-recog.c, which contains a
   function called `recog' plus its subroutines.  These functions
   contain a decision tree that recognizes whether an rtx, the
   argument given to recog, is a valid instruction.

   recog returns -1 if the rtx is not valid.  If the rtx is valid,
   recog returns a nonnegative number which is the insn code number
   for the pattern that matched.  This is the same as the order in the
   machine description of the entry that matched.  This number can be
   used as an index into various insn_* tables, such as insn_template,
   insn_outfun, and insn_n_operands (found in insn-output.c).

   The third argument to recog is an optional pointer to an int.  If
   present, recog will accept a pattern if it matches except for
   missing CLOBBER expressions at the end.  In that case, the value
   pointed to by the optional pointer will be set to the number of
   CLOBBERs that need to be added (it should be initialized to zero by
   the caller).  If it is set nonzero, the caller should allocate a
   PARALLEL of the appropriate size, copy the initial entries, and
   call add_clobbers (found in insn-emit.c) to fill in the CLOBBERs.

   This program also generates the function `split_insns', which
   returns 0 if the rtl could not be split, or it returns the split
   rtl as an INSN list.

   This program also generates the function `peephole2_insns', which
   returns 0 if the rtl could not be matched.  If there was a match,
   the new rtl is returned in an INSN list, and LAST_INSN will point
   to the last recognized insn in the old sequence.  */

#include "bconfig.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "errors.h"
#include "gensupport.h"
#include "gendtree.h"

/* Next number to use as an insn_code.  */

static int next_insn_code;

/* Begin the output file.  */

static void
write_header (void)
{
  puts ("\
/* Generated automatically by the program `genrecog' from the target\n\
   machine description file.  */\n\
\n\
#include \"config.h\"\n\
#include \"system.h\"\n\
#include \"coretypes.h\"\n\
#include \"tm.h\"\n\
#include \"rtl.h\"\n\
#include \"tm_p.h\"\n\
#include \"function.h\"\n\
#include \"insn-config.h\"\n\
#include \"recog.h\"\n\
#include \"real.h\"\n\
#include \"output.h\"\n\
#include \"flags.h\"\n\
#include \"hard-reg-set.h\"\n\
#include \"resource.h\"\n\
#include \"toplev.h\"\n\
#include \"reload.h\"\n\
#include \"regs.h\"\n\
#include \"addresses.h\"\n\
#include \"tm-constrs.h\"\n\
\n");

  puts ("\n\
/* `recog' contains a decision tree that recognizes whether the rtx\n\
   X0 is a valid instruction.\n\
\n\
   recog returns -1 if the rtx is not valid.  If the rtx is valid, recog\n\
   returns a nonnegative number which is the insn code number for the\n\
   pattern that matched.  This is the same as the order in the machine\n\
   description of the entry that matched.  This number can be used as an\n\
   index into `insn_data' and other tables.\n");
  puts ("\
   The third argument to recog is an optional pointer to an int.  If\n\
   present, recog will accept a pattern if it matches except for missing\n\
   CLOBBER expressions at the end.  In that case, the value pointed to by\n\
   the optional pointer will be set to the number of CLOBBERs that need\n\
   to be added (it should be initialized to zero by the caller).  If it");
  puts ("\
   is set nonzero, the caller should allocate a PARALLEL of the\n\
   appropriate size, copy the initial entries, and call add_clobbers\n\
   (found in insn-emit.c) to fill in the CLOBBERs.\n\
");

  puts ("\n\
   The function split_insns returns 0 if the rtl could not\n\
   be split or the split rtl as an INSN list if it can be.\n\
\n\
   The function peephole2_insns returns 0 if the rtl could not\n\
   be matched. If there was a match, the new rtl is returned in an INSN list,\n\
   and LAST_INSN will point to the last recognized insn in the old sequence.\n\
*/\n\n");
}


static void
print_header_recog (const char *prefix, const char *ext)
{
  printf ("%sint\n\
recog%s (rtx x0 ATTRIBUTE_UNUSED,\n\trtx insn ATTRIBUTE_UNUSED,\n\tint *pnum_clobbers ATTRIBUTE_UNUSED)\n", prefix, ext ? ext : "");
}

static void
print_locals_recog (void)
{
  printf ("  int tem ATTRIBUTE_UNUSED;\n");
}

static void
print_maybe_return_recog (const char *indent, const char *var)
{
  printf ("%sif (%s >= 0)\n%s  return %s;\n", indent, var, indent, var);
}

static void
print_default_return_recog (void)
{
  printf ("  return -1;\n");
}

static void
print_accept_insn_recog (const char *indent, struct decision_test *test,
			      struct decision *p ATTRIBUTE_UNUSED)
{
  if (test->u.insn.num_clobbers_to_add != 0)
    printf ("%s*pnum_clobbers = %d;\n",
	    indent, test->u.insn.num_clobbers_to_add);
  printf ("%sreturn %d;  /* %s */\n", indent,
	  test->u.insn.code_number,
	  get_insn_name (test->u.insn.code_number));
}

static int
is_unconditional_recog (struct decision_test *t)
{
  return (t->u.insn.num_clobbers_to_add == 0);
}

static void
print_header_split (const char *prefix, const char *ext)
{
  printf ("%srtx\n\
split%s (rtx x0 ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)\n", prefix, ext ? ext : "_insns");
}

static void
print_locals_split (void)
{
  printf ("  rtx tem ATTRIBUTE_UNUSED;\n");
}

static void
print_maybe_return_split (const char *indent, const char *var)
{
  printf ("%sif (%s != 0)\n%s  return %s;\n", indent, var, indent, var);
}

static void
print_default_return_split (void)
{
  printf ("  return 0;\n");
}

static void
print_accept_insn_split (const char *indent, struct decision_test *test,
			      struct decision *p ATTRIBUTE_UNUSED)
{
  printf ("%sreturn gen_split_%d (insn, operands);\n",
	  indent, test->u.insn.code_number);
}

static int
is_unconditional_split (struct decision_test *t ATTRIBUTE_UNUSED)
{
  return 1;
}

static void
print_header_peephole2 (const char *prefix, const char *ext)
{
  printf ("%srtx\n\
peephole2%s (rtx x0 ATTRIBUTE_UNUSED,\n\trtx insn ATTRIBUTE_UNUSED,\n\tint *_pmatch_len ATTRIBUTE_UNUSED)\n", prefix, ext ? ext : "_insns");
}

static void
print_accept_insn_peephole2 (const char *indent, struct decision_test *test,
			      struct decision *p)
{
  int match_len = 0, i;

  for (i = strlen (p->position) - 1; i >= 0; --i)
    if (ISUPPER (p->position[i]))
      {
	match_len = p->position[i] - 'A';
	break;
      }
  printf ("%s*_pmatch_len = %d;\n", indent, match_len);
  printf ("%stem = gen_peephole2_%d (insn, operands);\n",
	  indent, test->u.insn.code_number);
  printf ("%sif (tem != 0)\n%s  return tem;\n", indent, indent);
}

static int
is_unconditional_peephole2 (struct decision_test *t ATTRIBUTE_UNUSED)
{
  return -1;
}

const struct print_dtree_hooks recog_hooks = {
  print_header_recog,
  print_locals_recog,
  print_maybe_return_recog,
  print_default_return_recog,
  print_accept_insn_recog,
  is_unconditional_recog,
  "recog", ", pnum_clobbers"
};

const struct print_dtree_hooks split_hooks = {
  print_header_split,
  print_locals_split,
  print_maybe_return_split,
  print_default_return_split,
  print_accept_insn_split,
  is_unconditional_split,
  "split", ""
};

const struct print_dtree_hooks peephole2_hooks = {
  print_header_peephole2,
  print_locals_split,
  print_maybe_return_split,
  print_default_return_split,
  print_accept_insn_peephole2,
  is_unconditional_peephole2,
  "peephole2", ", _pmatch_len"
};



/* Search for and return operand N, stop when reaching node STOP.  */

static rtx
find_operand (rtx pattern, int n, rtx stop)
{
  const char *fmt;
  RTX_CODE code;
  int i, j, len;
  rtx r;

  if (pattern == stop)
    return stop;

  code = GET_CODE (pattern);
  if ((code == MATCH_SCRATCH
       || code == MATCH_OPERAND
       || code == MATCH_OPERATOR
       || code == MATCH_PARALLEL)
      && XINT (pattern, 0) == n)
    return pattern;

  fmt = GET_RTX_FORMAT (code);
  len = GET_RTX_LENGTH (code);
  for (i = 0; i < len; i++)
    {
      switch (fmt[i])
	{
	case 'e': case 'u':
	  if ((r = find_operand (XEXP (pattern, i), n, stop)) != NULL_RTX)
	    return r;
	  break;

	case 'V':
	  if (! XVEC (pattern, i))
	    break;
	  /* Fall through.  */

	case 'E':
	  for (j = 0; j < XVECLEN (pattern, i); j++)
	    if ((r = find_operand (XVECEXP (pattern, i, j), n, stop))
		!= NULL_RTX)
	      return r;
	  break;

	case 'i': case 'w': case '0': case 's':
	  break;

	default:
	  gcc_unreachable ();
	}
    }

  return NULL;
}

/* Search for and return operand M, such that it has a matching
   constraint for operand N.  */

static rtx
find_matching_operand (rtx pattern, int n)
{
  const char *fmt;
  RTX_CODE code;
  int i, j, len;
  rtx r;

  code = GET_CODE (pattern);
  if (code == MATCH_OPERAND
      && (XSTR (pattern, 2)[0] == '0' + n
	  || (XSTR (pattern, 2)[0] == '%'
	      && XSTR (pattern, 2)[1] == '0' + n)))
    return pattern;

  fmt = GET_RTX_FORMAT (code);
  len = GET_RTX_LENGTH (code);
  for (i = 0; i < len; i++)
    {
      switch (fmt[i])
	{
	case 'e': case 'u':
	  if ((r = find_matching_operand (XEXP (pattern, i), n)))
	    return r;
	  break;

	case 'V':
	  if (! XVEC (pattern, i))
	    break;
	  /* Fall through.  */

	case 'E':
	  for (j = 0; j < XVECLEN (pattern, i); j++)
	    if ((r = find_matching_operand (XVECEXP (pattern, i, j), n)))
	      return r;
	  break;

	case 'i': case 'w': case '0': case 's':
	  break;

	default:
	  gcc_unreachable ();
	}
    }

  return NULL;
}

/* Check for various errors in patterns.  SET is nonnull for a destination,
   and is the complete set pattern.  SET_CODE is '=' for normal sets, and
   '+' within a context that requires in-out constraints.  */

static void
validate_pattern (rtx pattern, rtx insn, rtx set, int set_code)
{
  const char *fmt;
  RTX_CODE code;
  size_t i, len;
  int j;

  code = GET_CODE (pattern);
  switch (code)
    {
    case MATCH_SCRATCH:
      return;
    case MATCH_DUP:
    case MATCH_OP_DUP:
    case MATCH_PAR_DUP:
      if (find_operand (insn, XINT (pattern, 0), pattern) == pattern)
	{
	  message_with_line (pattern_lineno,
			     "operand %i duplicated before defined",
			     XINT (pattern, 0));
          error_count++;
	}
      break;
    case MATCH_OPERAND:
    case MATCH_OPERATOR:
      {
	const char *pred_name = XSTR (pattern, 1);
	const struct pred_data *pred;
	const char *c_test;

	if (GET_CODE (insn) == DEFINE_INSN)
	  c_test = XSTR (insn, 2);
	else
	  c_test = XSTR (insn, 1);

	if (pred_name[0] != 0)
	  {
	    pred = lookup_predicate (pred_name);
	    if (!pred)
	      message_with_line (pattern_lineno,
				 "warning: unknown predicate '%s'",
				 pred_name);
	  }
	else
	  pred = 0;

	if (code == MATCH_OPERAND)
	  {
	    const char constraints0 = XSTR (pattern, 2)[0];

	    /* In DEFINE_EXPAND, DEFINE_SPLIT, and DEFINE_PEEPHOLE2, we
	       don't use the MATCH_OPERAND constraint, only the predicate.
	       This is confusing to folks doing new ports, so help them
	       not make the mistake.  */
	    if (GET_CODE (insn) == DEFINE_EXPAND
		|| GET_CODE (insn) == DEFINE_SPLIT
		|| GET_CODE (insn) == DEFINE_PEEPHOLE2)
	      {
		if (constraints0)
		  message_with_line (pattern_lineno,
				     "warning: constraints not supported in %s",
				     rtx_name[GET_CODE (insn)]);
	      }

	    /* A MATCH_OPERAND that is a SET should have an output reload.  */
	    else if (set && constraints0)
	      {
		if (set_code == '+')
		  {
		    if (constraints0 == '+')
		      ;
		    /* If we've only got an output reload for this operand,
		       we'd better have a matching input operand.  */
		    else if (constraints0 == '='
			     && find_matching_operand (insn, XINT (pattern, 0)))
		      ;
		    else
		      {
			message_with_line (pattern_lineno,
					   "operand %d missing in-out reload",
					   XINT (pattern, 0));
			error_count++;
		      }
		  }
		else if (constraints0 != '=' && constraints0 != '+')
		  {
		    message_with_line (pattern_lineno,
				       "operand %d missing output reload",
				       XINT (pattern, 0));
		    error_count++;
		  }
	      }
	  }

	/* Allowing non-lvalues in destinations -- particularly CONST_INT --
	   while not likely to occur at runtime, results in less efficient
	   code from insn-recog.c.  */
	if (set && pred && pred->allows_non_lvalue)
	  message_with_line (pattern_lineno,
			     "warning: destination operand %d "
			     "allows non-lvalue",
			     XINT (pattern, 0));

	/* A modeless MATCH_OPERAND can be handy when we can check for
	   multiple modes in the c_test.  In most other cases, it is a
	   mistake.  Only DEFINE_INSN is eligible, since SPLIT and
	   PEEP2 can FAIL within the output pattern.  Exclude special
	   predicates, which check the mode themselves.  Also exclude
	   predicates that allow only constants.  Exclude the SET_DEST
	   of a call instruction, as that is a common idiom.  */

	if (GET_MODE (pattern) == VOIDmode
	    && code == MATCH_OPERAND
	    && GET_CODE (insn) == DEFINE_INSN
	    && pred
	    && !pred->special
	    && pred->allows_non_const
	    && strstr (c_test, "operands") == NULL
	    && ! (set
		  && GET_CODE (set) == SET
		  && GET_CODE (SET_SRC (set)) == CALL))
	  message_with_line (pattern_lineno,
			     "warning: operand %d missing mode?",
			     XINT (pattern, 0));
	return;
      }

    case SET:
      {
	enum machine_mode dmode, smode;
	rtx dest, src;

	dest = SET_DEST (pattern);
	src = SET_SRC (pattern);

	/* STRICT_LOW_PART is a wrapper.  Its argument is the real
	   destination, and it's mode should match the source.  */
	if (GET_CODE (dest) == STRICT_LOW_PART)
	  dest = XEXP (dest, 0);

	/* Find the referent for a DUP.  */

	if (GET_CODE (dest) == MATCH_DUP
	    || GET_CODE (dest) == MATCH_OP_DUP
	    || GET_CODE (dest) == MATCH_PAR_DUP)
	  dest = find_operand (insn, XINT (dest, 0), NULL);

	if (GET_CODE (src) == MATCH_DUP
	    || GET_CODE (src) == MATCH_OP_DUP
	    || GET_CODE (src) == MATCH_PAR_DUP)
	  src = find_operand (insn, XINT (src, 0), NULL);

	dmode = GET_MODE (dest);
	smode = GET_MODE (src);

	/* The mode of an ADDRESS_OPERAND is the mode of the memory
	   reference, not the mode of the address.  */
	if (GET_CODE (src) == MATCH_OPERAND
	    && ! strcmp (XSTR (src, 1), "address_operand"))
	  ;

        /* The operands of a SET must have the same mode unless one
	   is VOIDmode.  */
        else if (dmode != VOIDmode && smode != VOIDmode && dmode != smode)
	  {
	    message_with_line (pattern_lineno,
			       "mode mismatch in set: %smode vs %smode",
			       GET_MODE_NAME (dmode), GET_MODE_NAME (smode));
	    error_count++;
	  }

	/* If only one of the operands is VOIDmode, and PC or CC0 is
	   not involved, it's probably a mistake.  */
	else if (dmode != smode
		 && GET_CODE (dest) != PC
		 && GET_CODE (dest) != CC0
		 && GET_CODE (src) != PC
		 && GET_CODE (src) != CC0
		 && GET_CODE (src) != CONST_INT)
	  {
	    const char *which;
	    which = (dmode == VOIDmode ? "destination" : "source");
	    message_with_line (pattern_lineno,
			       "warning: %s missing a mode?", which);
	  }

	if (dest != SET_DEST (pattern))
	  validate_pattern (dest, insn, pattern, '=');
	validate_pattern (SET_DEST (pattern), insn, pattern, '=');
        validate_pattern (SET_SRC (pattern), insn, NULL_RTX, 0);
        return;
      }

    case CLOBBER:
      validate_pattern (SET_DEST (pattern), insn, pattern, '=');
      return;

    case ZERO_EXTRACT:
      validate_pattern (XEXP (pattern, 0), insn, set, set ? '+' : 0);
      validate_pattern (XEXP (pattern, 1), insn, NULL_RTX, 0);
      validate_pattern (XEXP (pattern, 2), insn, NULL_RTX, 0);
      return;

    case STRICT_LOW_PART:
      validate_pattern (XEXP (pattern, 0), insn, set, set ? '+' : 0);
      return;

    case LABEL_REF:
      if (GET_MODE (XEXP (pattern, 0)) != VOIDmode)
	{
	  message_with_line (pattern_lineno,
			     "operand to label_ref %smode not VOIDmode",
			     GET_MODE_NAME (GET_MODE (XEXP (pattern, 0))));
	  error_count++;
	}
      break;

    default:
      break;
    }

  fmt = GET_RTX_FORMAT (code);
  len = GET_RTX_LENGTH (code);
  for (i = 0; i < len; i++)
    {
      switch (fmt[i])
	{
	case 'e': case 'u':
	  validate_pattern (XEXP (pattern, i), insn, NULL_RTX, 0);
	  break;

	case 'E':
	  for (j = 0; j < XVECLEN (pattern, i); j++)
	    validate_pattern (XVECEXP (pattern, i, j), insn, NULL_RTX, 0);
	  break;

	case 'i': case 'w': case '0': case 's':
	  break;

	default:
	  gcc_unreachable ();
	}
    }
}

/* Extract the pattern from a DEFINE_INSN or DEFINE_SPLIT, adding an implicit
   PARALLEL if the pattern has more than one element.  */
static rtx
extract_pattern (rtx insn, int op)
{
  if (XVECLEN (insn, op) == 1)
    return XVECEXP (insn, op, 0);
  else
    {
      rtx x = rtx_alloc (PARALLEL);
      XVEC (x, 0) = XVEC (insn, op);
      PUT_MODE (x, VOIDmode);
      return x;
    }
}


/* Construct and return a sequence of decisions that will recognize a
   DEFINE_INSN, INSN.  */

static struct decision_head
make_recog_sequence (rtx insn)
{
  rtx x;
  const char *c_test = XSTR (insn, 2);
  struct decision *last;
  struct decision_head head = { NULL, NULL };
  int i;

  x = extract_pattern (insn, 1);
  validate_pattern (x, insn, NULL_RTX, 0);
  last = add_to_sequence (x, &head, "");
  finish_sequence (last, "", c_test, false, next_insn_code, 0);

  /* If X is a PARALLEL, see if it ends with a group of CLOBBERs of (hard)
     registers or MATCH_SCRATCHes.  If so, set up to recognize the pattern
     without these CLOBBERs.  */

  if (GET_CODE (x) == PARALLEL)
    {
      /* Find the last non-clobber in the parallel.  */
      for (i = XVECLEN (x, 0); i > 0; i--)
	{
	  rtx y = XVECEXP (x, 0, i - 1);
	  if (GET_CODE (y) != CLOBBER
	      || (!REG_P (XEXP (y, 0))
		  && GET_CODE (XEXP (y, 0)) != MATCH_SCRATCH))
	    break;
	}

      if (i != XVECLEN (x, 0))
	{
	  rtx new;
	  struct decision_head clobber_head = { NULL, NULL };

	  /* Build a similar insn without the clobbers.  */
	  if (i == 1)
	    new = XVECEXP (x, 0, 0);
	  else
	    {
	      int j;

	      new = rtx_alloc (PARALLEL);
	      XVEC (new, 0) = rtvec_alloc (i);
	      for (j = i - 1; j >= 0; j--)
		XVECEXP (new, 0, j) = XVECEXP (x, 0, j);
	    }

	  /* Recognize it.  */
	  last = add_to_sequence (new, &clobber_head, "");
	  finish_sequence (last, "", c_test, true, next_insn_code, XVECLEN (x, 0) - i);

	  merge_trees (&head, &clobber_head);
	}
    }

  return head;
}

/* Construct and return a sequence of decisions that will recognize a
   DEFINE_SPLIT, INSN.  */

static struct decision_head
make_split_sequence (rtx insn)
{
  rtx x;
  const char *c_test = XSTR (insn, 1);
  struct decision *last;
  struct decision_head head = { NULL, NULL };

  x = extract_pattern (insn, 0);
  validate_pattern (x, insn, NULL_RTX, 0);
  last = add_to_sequence (x, &head, "");
  finish_sequence (last, "", c_test, false, next_insn_code, 0);

  /* Define the subroutine we will call below and emit in genemit.  */
  printf ("extern rtx gen_split_%d (rtx, rtx *);\n", next_insn_code);

  return head;
}


/* Construct and return a sequence of decisions that will recognize a
   DEFINE_PEEPHOLE2, INSN.  */

static struct decision_head
make_peephole2_sequence (rtx insn)
{
  rtx x;
  const char *c_test = XSTR (insn, 1);
  struct decision *last;
  struct decision_test *test, **place;
  struct decision_head *next, head = { NULL, NULL };
  char c_test_pos[2];
  int i, num_insns;
  char subpos[2] = "A";

  /* peephole2 gets special treatment, as we remove all traces of
     outer-level match_scratch and match_dup expressions here.
     For simplicity, X always gets an outer parallel even if it's only
     one entry.  */
  x = rtx_alloc (PARALLEL);
  PUT_MODE (x, VOIDmode);
  XVEC (x, 0) = rtvec_alloc (XVECLEN (insn, 0));
  for (i = num_insns = 0; i < XVECLEN (insn, 0); i++)
    {
      rtx tmp = XVECEXP (insn, 0, i);
      if (GET_CODE (tmp) != MATCH_SCRATCH && GET_CODE (tmp) != MATCH_DUP)
	{
	  XVECEXP (x, 0, num_insns) = tmp;
	  num_insns++;
	}
    }
  XVECLEN (x, 0) = num_insns;

  c_test_pos[0] = 'A' + num_insns - 1;
  c_test_pos[1] = '\0';

  validate_pattern (x, insn, NULL_RTX, 0);
  last = new_decision ("", &head);

  /* Check we have sufficient insns.  This avoids complications
     because we then know peep2_next_insn never fails.  */
  if (num_insns > 1)
    {
      place = &last->tests;
      test = new_decision_test (DT_num_insns, &place);
      test->u.num_insns = num_insns;
      next = &last->success;
    }
  else
    {
      head.first = head.last = NULL;
      next = &head;
   }

  for (i = 0; i < XVECLEN (x, 0); i++, subpos[0]++)
    {
      /* Which insn we're looking at is represented by A-Z. We don't
         ever use 'A', however; it is always implied.  */

      last = add_to_sequence (XVECEXP (x, 0, i), next, i ? subpos : "");
      next = &last->success;
    }

  finish_sequence (last, c_test_pos, c_test, false, next_insn_code, 0);

  /* Define the subroutine we will call below and emit in genemit.  */
  printf ("extern rtx gen_peephole2_%d (rtx, rtx *);\n", next_insn_code);

  return head;
}

int
main (int argc, char **argv)
{
  rtx desc;
  struct decision_head recog_tree, split_tree, peephole2_tree, h;

  progname = "genrecog";

  memset (&recog_tree, 0, sizeof recog_tree);
  memset (&split_tree, 0, sizeof split_tree);
  memset (&peephole2_tree, 0, sizeof peephole2_tree);

  if (init_md_reader_args (argc, argv) != SUCCESS_EXIT_CODE)
    return (FATAL_EXIT_CODE);

  next_insn_code = 0;

  write_header ();

  /* Read the machine description.  */

  while (1)
    {
      desc = read_md_rtx (&pattern_lineno, &next_insn_code);
      if (desc == NULL)
	break;

      switch (GET_CODE (desc))
	{
	case DEFINE_PREDICATE:
	case DEFINE_SPECIAL_PREDICATE:
	  process_define_predicate (desc);
	  break;

	case DEFINE_INSN:
	  h = make_recog_sequence (desc);
	  merge_trees (&recog_tree, &h);
	  break;

	case DEFINE_SPLIT:
	  h = make_split_sequence (desc);
	  merge_trees (&split_tree, &h);
	  break;

	case DEFINE_PEEPHOLE2:
	  h = make_peephole2_sequence (desc);
	  merge_trees (&peephole2_tree, &h);

	default:
	  /* do nothing */;
	}
    }

  if (error_count || have_error)
    return FATAL_EXIT_CODE;

  puts ("\n\n");

  process_tree (&recog_tree, &recog_hooks);
  process_tree (&split_tree, &split_hooks);

  if (peephole2_tree.first != NULL)
    process_tree (&peephole2_tree, &peephole2_hooks);

  fflush (stdout);
  return (ferror (stdout) != 0 ? FATAL_EXIT_CODE : SUCCESS_EXIT_CODE);
}


