/* Instruction scheduling pass.  Log dumping infrastructure.
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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
#include "toplev.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "regs.h"
#include "function.h"
#include "flags.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "params.h"
#include "output.h"
#include "basic-block.h"
#include "cselib.h"
#include "sel-sched-ir.h"
#include "sel-sched-dump.h"

#include "pwd.h"
#include <fcntl.h>
#include <unistd.h>

static FILE *sched_dump1;

/* File for dumping statistics into the one directory for
   the whole make run.  */
static FILE *sel_stat_file = NULL;



/* These variables control high-level pretty printing.  */
static int sel_dump_cfg_flags = SEL_DUMP_CFG_FLAGS;
static int sel_debug_cfg_flags = SEL_DUMP_CFG_FLAGS;

/* True when a cfg should be dumped (either flag_sel_sched_dump cfg
   or the below flag is set.  */
static bool sel_dump_cfg_p;

/* Variables that are used to build the cfg dump file name.  */
static const char * const sel_debug_cfg_root = "./";
static const char * const sel_debug_cfg_root_postfix_default = "";
static const char *sel_debug_cfg_root_postfix = "";
static int sel_dump_cfg_fileno = -1;
static int sel_debug_cfg_fileno = -1;

/* When this flag is on, we are dumping to the .dot file.
   When it is off, we are dumping to log.
   This is useful to differentiate formatting between log and .dot
   files.  */
bool sched_dump_to_dot_p = false;

/* Controls how insns from a fence list should be dumped.  */
static int dump_flist_insn_flags = (DUMP_INSN_UID | DUMP_INSN_BBN
                                    | DUMP_INSN_SEQNO);

/* Stores an expression according to which either insns will be scheduled 
   using all features of the selective scheduling or the corresponding 
   code motion will be skipped.  */
const char *flag_insn_range = NULL;
static char sel_stat_output_buf[16384];


static void
switch_dump (void)
{
  FILE *f = sched_dump1;

  sched_dump1 = sched_dump;
  sched_dump = f;
}

void 
print_marker_to_log (void)
{
  sel_print ("Marker: %d\n:", sel_dump_cfg_fileno);
}

void
setup_sched_dumps (void)
{
  sched_verbose = sched_verbose_param;
  if (sched_verbose_param == 0 && dump_file)
    sched_verbose = 1;
  sched_dump = ((sched_verbose_param >= 10 || !dump_file)
		? stderr : dump_file);
  sched_dump1 = stderr;
}


/* Functions for dumping instructions, av sets, and exprs.  */ 

static int dump_all = 0;
static int dump_insn_rtx_flags = DUMP_INSN_RTX_PATTERN;
static int dump_vinsn_flags = (DUMP_VINSN_INSN_RTX | DUMP_VINSN_TYPE
			       | DUMP_VINSN_COUNT);
static int debug_vinsn_flags = 1;
static int debug_insn_rtx_flags = 1;
static int dump_expr_flags = DUMP_EXPR_ALL;
static int debug_expr_flags = 1;

/* Controls how an insn should be dumped.  It can be changed from debugger.  */
static int dump_insn_flags = (DUMP_INSN_EXPR | DUMP_INSN_SCHED_CYCLE);
static int debug_insn_flags = 1;


/* Print an rtx X.  */
void
sel_print_rtl (rtx x)
{
  print_rtl_single (sched_dump, x);
}

void
dump_insn_rtx_1 (rtx insn, int flags)
{
  int all;

  all = (flags & 1) | dump_all;
  if (all)
    flags |= DUMP_INSN_RTX_ALL;

  sel_print ("(");

  if (flags & DUMP_INSN_RTX_UID)
    sel_print ("%d;", INSN_UID (insn));

  if (flags & DUMP_INSN_RTX_PATTERN)
    {
      char buf[2048];

      print_insn (buf, insn, 0);
      sel_print ("%s;", buf);
    }

  if (flags & DUMP_INSN_RTX_BBN)
    {
      basic_block bb = BLOCK_FOR_INSN (insn);

      sel_print ("bb:%d;", bb != NULL ? bb->index : -1);
    }
}

void
dump_insn_rtx (rtx insn)
{
  dump_insn_rtx_1 (insn, dump_insn_rtx_flags);
}

void
debug_insn_rtx (rtx insn)
{
  switch_dump ();
  dump_insn_rtx_1 (insn, debug_insn_rtx_flags);
  sel_print (")\n");
  switch_dump ();
}

void
dump_vinsn_1 (vinsn_t vi, int flags)
{
  int all;

  all = (flags & 1) | dump_all;
  if (all)
    flags |= DUMP_VINSN_ALL;

  sel_print ("(");

  if (flags & DUMP_VINSN_INSN_RTX)
    dump_insn_rtx_1 (VINSN_INSN_RTX (vi), dump_insn_rtx_flags | all);

  if (flags & DUMP_VINSN_TYPE)
    sel_print ("type:%s;", GET_RTX_NAME (VINSN_TYPE (vi)));

  if (flags & DUMP_VINSN_COUNT)
    sel_print ("count:%d;", VINSN_COUNT (vi));

  if (flags & DUMP_VINSN_COST)
    {
      int cost = vi->cost;

      if (cost != -1)
	sel_print ("cost:%d;", cost);
    }
}

void
dump_vinsn (vinsn_t vi)
{
  dump_vinsn_1 (vi, dump_vinsn_flags);
}

void
debug_vinsn (vinsn_t vi)
{
  switch_dump ();
  dump_vinsn_1 (vi, debug_vinsn_flags);
  sel_print (")\n"); 
  switch_dump ();
}

/* Dump EXPR.  */
void
dump_expr_1 (expr_t expr, int flags)
{
  int all;

  all = (flags & 1) | dump_all;
  if (all)
    flags |= DUMP_EXPR_ALL;

  sel_print ("[");

  if (flags & DUMP_EXPR_VINSN)
    dump_vinsn_1 (EXPR_VINSN (expr), dump_vinsn_flags | all);

  if (flags & DUMP_EXPR_SPEC)
    {
      int spec = EXPR_SPEC (expr);

      if (spec != 0)
	sel_print ("spec:%d;", spec);
    }

  if (flags & DUMP_EXPR_USEFULNESS)
    {
      int use = EXPR_USEFULNESS (expr);

      if (use != REG_BR_PROB_BASE)
        sel_print ("use:%d;", use);
    }

  if (flags & DUMP_EXPR_PRIORITY)
    sel_print ("prio:%d;", EXPR_PRIORITY (expr));

  if (flags & DUMP_EXPR_SCHED_TIMES)
    {
      int times = EXPR_SCHED_TIMES (expr);

      if (times != 0)
	sel_print ("times:%d;", times);
    }

  if (flags & DUMP_EXPR_SPEC_DONE_DS)
    {
      ds_t spec_done_ds = EXPR_SPEC_DONE_DS (expr);

      if (spec_done_ds != 0)
	sel_print ("ds:%d;", spec_done_ds);
    }

  if (flags & DUMP_EXPR_ORIG_BB)
    {
      int orig_bb = EXPR_ORIG_BB_INDEX (expr);

      if (orig_bb != 0)
	sel_print ("orig_bb:%d;", orig_bb);
    }
  
  if (EXPR_TARGET_AVAILABLE (expr) < 1)
    sel_print ("target:%d;", EXPR_TARGET_AVAILABLE (expr));
  sel_print ("]");
}

void
dump_expr (expr_t expr)
{
  dump_expr_1 (expr, dump_expr_flags);
}

void
debug_expr (expr_t expr)
{
  switch_dump ();
  dump_expr_1 (expr, debug_expr_flags);
  sel_print ("\n");
  switch_dump ();
}

/* Dump insn I honoring FLAGS.  */
void
dump_insn_1 (insn_t i, int flags)
{
  int all;

  all = (flags & 1) | dump_all;
  if (all)
    flags |= DUMP_INSN_ALL;

  if (!sched_dump_to_dot_p)
    sel_print ("(");

  if (flags & DUMP_INSN_ASM_P)
    flags = flags;

  if (flags & DUMP_INSN_SCHED_NEXT)
    flags = flags;

  if (flags & DUMP_INSN_EXPR)
    {
      dump_expr_1 (INSN_EXPR (i), dump_expr_flags | all);
      sel_print (";");
    }
  else if (flags & DUMP_INSN_PATTERN)
    {
      dump_insn_rtx_1 (i, DUMP_INSN_RTX_PATTERN | all);
      sel_print (";");
    }
  else if (flags & DUMP_INSN_UID)
    sel_print ("uid:%d;", INSN_UID (i));

  if (flags & DUMP_INSN_AV)
    flags = flags;

  if (flags & DUMP_INSN_SEQNO)
    sel_print ("seqno:%d;", INSN_SEQNO (i));

  if (flags & DUMP_INSN_AFTER_STALL_P)
    flags = flags;
 
  if (flags & DUMP_INSN_SCHED_CYCLE)
    {
      int cycle = INSN_SCHED_CYCLE (i);

      if (cycle != 0)
	sel_print ("cycle:%d;", cycle);
    }

  if (!sched_dump_to_dot_p)
    sel_print (")");
}

/* Dump insn I with default flags.  */
void
dump_insn (insn_t i)
{
  dump_insn_1 (i, dump_insn_flags);
}

void
debug_insn (insn_t insn)
{
  switch_dump ();
  dump_insn_1 (insn, debug_insn_flags);
  sel_print ("\n");
  switch_dump ();
}

/* Dumps av_set AV.  */
void
dump_av_set (av_set_t av)
{
  av_set_iterator i;
  expr_t expr;
  
  if (!sched_dump_to_dot_p)
    sel_print ("{");
  
  FOR_EACH_EXPR (expr, i, av)
    {
      dump_expr (expr);
      if (!sched_dump_to_dot_p)
        sel_print (" ");
      else
        sel_print ("\n");
    }
  
  if (!sched_dump_to_dot_p)
    sel_print ("}");
}

/* Dumps lvset LV.  */
void
dump_lv_set (regset lv)
{
  sel_print ("{");

  /* This code was adapted from flow.c: dump_regset ().  */
  if (lv == NULL)
    sel_print ("nil");
  else
    {
      unsigned i;
      reg_set_iterator rsi;
      int count = 0;
      
      EXECUTE_IF_SET_IN_REG_SET (lv, 0, i, rsi)
        {
          sel_print (" %d", i);
          if (i < FIRST_PSEUDO_REGISTER)
            {
              sel_print (" [%s]", reg_names[i]);
              ++count;
            }
          
          ++count;
          
          if (sched_dump_to_dot_p && count == 12)
            {
              count = 0;
              sel_print ("\n");
            }
        }
    }
  
  sel_print ("}\n");
}

/* Dumps a list of instructions pointed to by P.  */
static void
dump_ilist (ilist_t p)
{
  while (p)
    {
      dump_insn (ILIST_INSN (p));
      p = ILIST_NEXT (p);
    }
}

/* Dumps a list of boundaries pointed to by BNDS.  */
void
dump_blist (blist_t bnds)
{
  for (; bnds; bnds = BLIST_NEXT (bnds))
    {
      bnd_t bnd = BLIST_BND (bnds);
      
      sel_print ("[to: %d; ptr: ", INSN_UID (BND_TO (bnd)));
      dump_ilist (BND_PTR (bnd));
      sel_print ("] ");
    }
}

/* Dumps a list of fences pointed to by L.  */
void
dump_flist (flist_t l)
{
  while (l)
    {
      dump_insn_1 (FENCE_INSN (FLIST_FENCE (l)), dump_flist_insn_flags);
      sel_print (" ");
      l = FLIST_NEXT (l);
    }
}

/* Dumps an insn vector SUCCS.  */
void
dump_insn_vector (rtx_vec_t succs)
{
  int i;
  rtx succ;
  
  for (i = 0; VEC_iterate (rtx, succs, i, succ); i++)
    if (succ)
      dump_insn (succ);
    else
      sel_print ("NULL ");
}

/* Dumps a hard reg set SET to FILE using PREFIX. */
static void
print_hard_reg_set (FILE *file, const char *prefix, HARD_REG_SET set)
{
  int i;

  fprintf (file, "%s{ ", prefix);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (set, i))
	fprintf (file, "%d ", i);
    }
  fprintf (file, "}\n");
}

/* Dumps a hard reg set SET using PREFIX.  */
void
dump_hard_reg_set (const char *prefix, HARD_REG_SET set)
{
  print_hard_reg_set (sched_dump, prefix, set);
}

/* Pretty print INSN.  This is used as a hook.  */
const char *
sel_print_insn (const_rtx insn, int aligned ATTRIBUTE_UNUSED)
{
  static char buf[80];

  /* '+' before insn means it is a new cycle start and it's not been 
     scheduled yet.  '>' - has been scheduled.  */
  if (s_i_d && INSN_LUID (insn) > 0)
    if (GET_MODE (insn) == TImode)
      sprintf (buf, "%s %4d", 
               INSN_SCHED_TIMES (insn) > 0 ? "> " : "< ", 
               INSN_UID (insn));
    else
      sprintf (buf, "%s %4d", 
               INSN_SCHED_TIMES (insn) > 0 ? "! " : "  ", 
               INSN_UID (insn));
  else
    if (GET_MODE (insn) == TImode)
      sprintf (buf, "+ %4d", INSN_UID (insn));
    else
      sprintf (buf, "  %4d", INSN_UID (insn));

  return buf;
}


/* Functions for pretty printing of CFG.  */

/* Replace all occurencies of STR1 to STR2 in BUF.
   The BUF must be large enough to hold the result.  */
static void
replace_str_in_buf (char *buf, const char *str1, const char *str2)
{
  int buf_len = strlen (buf);
  int str1_len = strlen (str1);
  int str2_len = strlen (str2);
  int diff = str2_len - str1_len;

  char *p = buf;
  do
    {
      p = strstr (p, str1);
      if (p) 
	{
	  char *p1 = p + str1_len;
	  /* Copy the rest of buf and '\0'.  */
	  int n = buf + buf_len - p1;
	  int i;

	  /* Shift str by DIFF chars.  */
	  if (diff > 0)
            for (i = n; i >= 0; i--)
              p1[i + diff] = p1[i];
	  else
            for (i = 0; i <= n; i++)
              p1[i + diff] = p1[i];

	  /* Copy str2.  */
	  for (i = 0; i < str2_len; i++)
	    p[i] = str2[i];
	    
	  p += str2_len;
	  buf_len += diff;
	}

    }
  while (p);
}

/* Replace characters in BUF that have special meaning in .dot file.  */
void
sel_prepare_string_for_dot_label (char *buf)
{
  char specials_from[7][2] = { "<", ">", "{", "|", "}", "\"",
			       "\n" };
  char specials_to[7][3] = { "\\<", "\\>", "\\{", "\\|", "\\}", "\\\"",
			     "\\l" };
  unsigned i;

  for (i = 0; i < 7; i++)
    replace_str_in_buf (buf, specials_from[i], specials_to[i]);
}

/* Dump INSN with FLAGS.  */
void
sel_dump_cfg_insn (insn_t insn, int flags)
{
  int insn_flags = DUMP_INSN_UID | DUMP_INSN_PATTERN;

  if (sched_luids != NULL && INSN_LUID (insn) > 0)
    {
      if (flags & SEL_DUMP_CFG_INSN_SEQNO)
	insn_flags |= DUMP_INSN_SEQNO | DUMP_INSN_SCHED_CYCLE | DUMP_INSN_EXPR;
    }

  dump_insn_1 (insn, insn_flags);
}

/* Dump E to the dot file F.  */
void
sel_dump_cfg_edge (FILE *f, edge e)
{
  int w;
  const char *color;

  if (e->flags & EDGE_FALLTHRU)
    {
      w = 10;
      color = ", color = red";
    }
  else if (e->src->next_bb == e->dest)
    {
      w = 3;
      color = ", color = blue";
    }
  else
    {
      w = 1;
      color = "";
    }

  fprintf (f, "\tbb%d -> bb%d [weight = %d%s];\n",
	   e->src->index, e->dest->index, w, color);
}


/* Return true if BB has a predesessor from current region.
   TODO: Either make this function to trace back through empty block
   or just remove those empty blocks.  */
static bool
has_preds_in_current_region_p (basic_block bb)
{
  edge e;
  edge_iterator ei;

  gcc_assert (!in_current_region_p (bb));

  FOR_EACH_EDGE (e, ei, bb->preds)
    if (in_current_region_p (e->src))
      return true;

  return false;
}

/* Dump a cfg region to the dot file F honoring FLAGS.  */
void
sel_dump_cfg_2 (FILE *f, int flags)
{
  basic_block bb;
  FILE *tf;

  sched_dump_to_dot_p = true;
  tf = sched_dump1;
  sched_dump1 = f;
  switch_dump ();

  fprintf (f, "digraph G {\n"
	   "\tratio = 2.25;\n"
	   "\tnode [shape = record, fontsize = 9];\n");

  if (flags & SEL_DUMP_CFG_FUNCTION_NAME)
    fprintf (f, "function [label = \"%s\"];\n", current_function_name ());

  FOR_EACH_BB (bb)
    {
      insn_t insn = BB_HEAD (bb);
      insn_t next_tail = NEXT_INSN (BB_END (bb));
      edge e;
      edge_iterator ei;
      bool in_region_p = ((flags & SEL_DUMP_CFG_CURRENT_REGION)
			  && in_current_region_p (bb));
      bool full_p = (!(flags & SEL_DUMP_CFG_CURRENT_REGION)
		     || in_region_p);
      bool some_p = full_p || has_preds_in_current_region_p (bb);
      const char *color;
      const char *style;

      if (!some_p)
	continue;

      if ((flags & SEL_DUMP_CFG_CURRENT_REGION)
	  && in_current_region_p (bb)
	  && BLOCK_TO_BB (bb->index) == 0)
	color = "color = green, ";
      else
	color = "";

      if ((flags & SEL_DUMP_CFG_FENCES)
	  && in_region_p)
	{
	  style = "";

	  if (!sel_bb_empty_p (bb))
	    {
	      bool first_p = true;
	      insn_t tail = BB_END (bb);
	      insn_t cur_insn;

	      cur_insn = bb_note (bb);

	      do
		{
		  fence_t fence;

		  cur_insn = NEXT_INSN (cur_insn);
		  fence = flist_lookup (fences, cur_insn);

		  if (fence != NULL)
		    {
		      if (!FENCE_SCHEDULED_P (fence))
			{
			  if (first_p)
			    color = "color = red, ";
			  else
			    color = "color = yellow, ";
			}
		      else
			color = "color = blue, ";
		    }

		  first_p = false;
		}
	      while (cur_insn != tail);
	    }
	}
      else if (!full_p)
	style = "style = dashed, ";
      else
	style = "";

      fprintf (f, "\tbb%d [%s%slabel = \"{Basic block %d", bb->index,
	       style, color, bb->index);

      if ((flags & SEL_DUMP_CFG_BB_LOOP)
	  && bb->loop_father != NULL)
	fprintf (f, ", loop %d", bb->loop_father->num);

      if (full_p
	  && (flags & SEL_DUMP_CFG_BB_NOTES_LIST))
	{
	  insn_t notes = BB_NOTE_LIST (bb);

	  if (notes != NULL_RTX)
	    {
	      fprintf (f, "|");

	      /* For simplicity, we dump notes from note_list in reversed order
		 to that what they will appear in the code.  */
	      while (notes != NULL_RTX)
		{
		  sel_dump_cfg_insn (notes, flags);
		  fprintf (f, "\\l");

		  notes = PREV_INSN (notes);
		}
	    }
	}

      if (full_p
	  && (flags & SEL_DUMP_CFG_AV_SET)
	  && in_current_region_p (bb)
	  && !sel_bb_empty_p (bb))
	{
	  fprintf (f, "|");

	  if (BB_AV_SET_VALID_P (bb))
	    dump_av_set (BB_AV_SET (bb));
	  else if (BB_AV_LEVEL (bb) == -1)
	    fprintf (f, "AV_SET needs update");
	}

      if ((flags & SEL_DUMP_CFG_LV_SET)
	  && !sel_bb_empty_p (bb))
 	{
	  fprintf (f, "|");

	  if (BB_LV_SET_VALID_P (bb))
	    dump_lv_set (BB_LV_SET (bb));
	  else
	    fprintf (f, "LV_SET needs update");
	}

      if (full_p
	  && (flags & SEL_DUMP_CFG_BB_INSNS))
	{
	  fprintf (f, "|");
	  while (insn != next_tail)
	    {
	      sel_dump_cfg_insn (insn, flags);
	      fprintf (f, "\\l");

	      insn = NEXT_INSN (insn);
	    }
	}

      fprintf (f, "}\"];\n");

      FOR_EACH_EDGE (e, ei, bb->succs)
	if (full_p || in_current_region_p (e->dest))
	  sel_dump_cfg_edge (f, e);
    }

  fprintf (f, "}");

  switch_dump ();
  sched_dump1 = tf;
  sched_dump_to_dot_p = false;
}

/* Dump a cfg region to the file specified by TAG honoring flags.  
   The file is created by the function.  */
void
sel_dump_cfg_1 (const char *tag, int flags)
{
  char *buf;
  int i;
  FILE *f;

  ++sel_dump_cfg_fileno;

  if (!sel_dump_cfg_p)
    return;

  i = 1 + snprintf (NULL, 0, "%s/%s%05d-%s.dot", sel_debug_cfg_root,
		    sel_debug_cfg_root_postfix, sel_dump_cfg_fileno, tag);
  buf = xmalloc (i * sizeof (*buf));
  snprintf (buf, i, "%s/%s%05d-%s.dot", sel_debug_cfg_root,
	    sel_debug_cfg_root_postfix, sel_dump_cfg_fileno, tag);

  f = fopen (buf, "w");

  if (f == NULL)
    fprintf (stderr, "Can't create file: %s.\n", buf);
  else
    {
      sel_dump_cfg_2 (f, flags);

      fclose (f);
    }

  free (buf);
}


/* Setup cfg dumping flags.  */
void
setup_dump_cfg_params (void)
{
  int dump_flags = PARAM_VALUE (PARAM_SELSCHED_DUMP_CFG_FLAGS);

  if (dump_flags != 0)
    sel_dump_cfg_flags = dump_flags;
  else
    sel_dump_cfg_flags = SEL_DUMP_CFG_FLAGS;

  sel_dump_cfg_p = (flag_sel_sched_dump_cfg != 0);
  sel_debug_cfg_root_postfix = sel_debug_cfg_root_postfix_default;
}

/* Dump a cfg region to the file specified to by TAG with default flags.  */
void
sel_dump_cfg (const char *tag)
{
  sel_dump_cfg_1 (tag, sel_dump_cfg_flags);
}


/* Debug a cfg region with FLAGS.  */
void
sel_debug_cfg_1 (int flags)
{
  bool t1 = sel_dump_cfg_p;
  int t2 = sel_dump_cfg_fileno;

  sel_dump_cfg_p = true;
  sel_dump_cfg_fileno = ++sel_debug_cfg_fileno;

  sel_dump_cfg_1 ("sel-debug-cfg", flags);

  sel_dump_cfg_fileno = t2;
  sel_dump_cfg_p = t1;
}


/* Functions callable from a debugger.  */
int
insn_uid (rtx insn)
{
  return INSN_UID (insn);
}

basic_block
block_for_insn (rtx insn)
{
  return BLOCK_FOR_INSN (insn);
}

av_set_t
bb_av_set (basic_block bb)
{
  return BB_AV_SET (bb);
}

rtx insn_pattern (rtx insn)
{
  return PATTERN (insn);
}

int insn_code (rtx insn)
{
  return GET_CODE (insn);
}

bool insn_is_set_p (rtx insn)
{
  return GET_CODE (PATTERN (insn)) == SET;
}

int
hard_regno_rename_ok (int i ATTRIBUTE_UNUSED, int j ATTRIBUTE_UNUSED)
{
  return HARD_REGNO_RENAME_OK (i, j);
}

/* Dumps av_set AV to stderr. */
void
debug_av_set (av_set_t av)
{
  switch_dump ();
  dump_av_set (av);
  switch_dump ();
}

/* Dump LV to stderr.  */
void
debug_lv_set (regset lv)
{
  switch_dump ();
  dump_lv_set (lv);
  switch_dump ();
}

/* Dump an instruction list P to stderr.  */
void
debug_ilist (ilist_t p)
{
  switch_dump ();
  dump_ilist (p);
  switch_dump ();
}

/* Dump a boundary list BNDS to stderr.  */
void
debug_blist (blist_t bnds)
{
  switch_dump ();
  dump_blist (bnds);
  switch_dump ();
}

/* Dump an insn vector SUCCS.  */
void
debug_insn_vector (rtx_vec_t succs)
{
  switch_dump ();
  dump_insn_vector (succs);
  switch_dump ();
}

/* Dump a hard reg set SET to stderr.  */
void
debug_hard_reg_set (HARD_REG_SET set)
{
  switch_dump ();
  dump_hard_reg_set ("", set);
  switch_dump ();
}

/* Debug a cfg region with default flags.  */
void
sel_debug_cfg (void)
{
  sel_debug_cfg_1 (sel_debug_cfg_flags);
}

/* Print a current cselib value for X's address to stderr.  */
rtx
debug_mem_addr_value (rtx x)
{
  rtx t, addr;

  gcc_assert (MEM_P (x));
  t = shallow_copy_rtx (x);
  if (cselib_lookup (XEXP (t, 0), Pmode, 0))
    XEXP (t, 0) = cselib_subst_to_values (XEXP (t, 0));

  t = canon_rtx (t);
  addr = get_addr (XEXP (t, 0));
  debug_rtx (t);
  debug_rtx (addr);
  return t;
}

/* Test a memory allocating N pointers.  */
void
mem_test (int n)
{
  int **p, i;

  p = xmalloc (n * sizeof (*p));

  for (i = 0; i < n; i++)
    *p++ = xmalloc ((n * i) * sizeof (**p));

  for (i = 0; i < n; i++)
    free (*--p);

  free (p);
}

/* Helper function for in_range_p.  */
static int in_range_p_1 (int val, const char *expr, int i1, int i2, bool *err)
{
  int br = 0;
  int i = -1, x;
  char ops[] = "|&-";
  char *p;
  char c = 0;

  if (i1 > i2)
    *err = true;
    
  if (*err)
    return 0;    
  
  for (p = ops; *p; p++) 
    {
      for (i = i2; i >= i1; i--) 
	{
	  c = expr[i];
	  if (c == ')')
	    br++;
	  else if (c == '(')
	    br--;
	  
	 if (!br && c == *p)
	   goto l;
	}    
    }

  l: if (br)
    {
      *err = 1;
      return 0;
    }

  if (*p) {
    if (c == '&')
      return in_range_p_1 (val, expr, i1, i-1, err)
               && in_range_p_1 (val, expr, i+1, i2, err);

    if (c == '|')
      return in_range_p_1 (val, expr, i1, i-1, err)
             || in_range_p_1 (val, expr, i+1, i2, err);
  }	      

  if (expr[i1] == '(' && expr[i2] == ')')
    return in_range_p_1 (val, expr, i1+1, i2-1, err);

  if (expr[i1] == '!')
    return !in_range_p_1 (val, expr, i1+1, i2, err);

  if (*p && c == '-')
      return (in_range_p_1 (val, expr, i1, i-1, err) <= val)
              && val <= in_range_p_1 (val, expr, i+1, i2, err);
      
  sscanf (expr+i1, "%d%n", &x, &i);
  if (i1 + i != i2 + 1)
    {
      *err = true;
      return false;
    }
  else
    return x;
}


/* Returns whether VAL is within the range given by the EXPR.  
   E.g. "30-40&!32-34|33-33" will return true only for the following values:
   30 31 33 35 36 37 38 39 40.  The expression may consist only from the
   numbers, operators "-", "&", "|" and "!".  Ranges containing only the
   single integer N should be written as "N-N", the expession should not 
   contain any spaces.  If the expression is not valid, ERR is set to TRUE.  */

bool in_range_p (int val, const char *expr, bool *err)
{
  return in_range_p_1 (val, expr, 0, strlen (expr) - 1, err);
}

/* sel_sched_fix_param() is called from toplev.c upon detection
   of the -fsel-insn-range=EXPR option.  */
void
sel_sched_fix_param (const char *param, const char *val)
{
  if (!strcmp (param, "insn-range"))
    flag_insn_range = val;
  else
    warning (0, "sel_sched_fix_param: unknown param: %s", param);
}
      
/* The file name is either copied from the SEL_STAT_FILE variable, or generated
   from the current time.  If the environment variable is set, then
   all the statistics will be written to the single file, otherwise
   statistics will be stored in separate files in the ~/sel-stat directory.  */
static void
sel_get_stat_filename (char *buf)
{
  struct passwd *rpw = getpwuid (getuid());
  char buf2[1024];
  char *stat_file_name = getenv ("SEL_STAT_FILE");

  strcpy (buf, rpw->pw_dir);
  strcat (buf, "/sel-stat");
  mkdir (buf, 0775);

  if (stat_file_name)
    sprintf (buf2, "/%s", stat_file_name);
  else
    sprintf (buf2, "/stat-%d.txt", (int) time (NULL));

  strcat (buf, buf2);
}

/* Get a pointer to a FILE where various per-build statistics could be stored.
   See sel_get_stat_filename for how this file name is determined.
   It's guaranteed that every line of output (unless its greater than 1024 
   chars) will not be intermixed with output from any other process that
   may also be writing to the same file.  */
FILE *
sel_get_stat_file (void)
{
  if (!sel_stat_file)
    {
      char stat_filename[1024];

      sel_get_stat_filename (stat_filename);
      sel_stat_file = fopen (stat_filename, "at");
    }

  if (sel_stat_file)
    {
      setvbuf (sel_stat_file, sel_stat_output_buf, _IOLBF,
              sizeof sel_stat_output_buf);

      return sel_stat_file;
    }
  else
    return stderr;
}

