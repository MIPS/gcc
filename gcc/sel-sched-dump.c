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
#include "basic-block.h"
#include "cselib.h"
#include "sel-sched-ir.h"
#include "sel-sched-dump.h"


/* These variables hold parameters for pretty printing from recursive 
   functions.  */
int indent = -1;
static int *indents = NULL;
static int indents_n = 0, indents_sz = 0;

bool new_line = true;
static bool *new_lines = NULL;
static int new_lines_n = 0, new_lines_sz = 0;

bool print_block = true;
static bool *print_blocks = NULL;
static int print_blocks_n = 0, print_blocks_sz = 0;

static FILE *sched_dump1;
static int indent1 = 0;
static bool new_line1 = true;
static bool print_block1 = true;


/* These variables control high-level pretty printing.  */
static int sel_dump_cfg_flags = SEL_DUMP_CFG_FLAGS;
static int sel_debug_cfg_flags = SEL_DUMP_CFG_FLAGS;

/* True when a cfg should be dumped (either flag_sel_sched_dump cfg
   or the below flag is set.  */
static bool sel_dump_cfg_p;

/* True when a verbose information about pipelining should be provided.  */
static bool sel_pipelining_verbose_p;

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

/* Controls how an insn should be dumped.  */
static int dump_insn_flags = (DUMP_INSN_UID | DUMP_INSN_BBN | DUMP_INSN_COUNT);

/* Controls how an insn from RHS should be dumped.  */
static int dump_rhs_insn_flags = DUMP_INSN_UID | DUMP_INSN_COUNT;

/* Controls how insns from a fence list should be dumped.  */
static int dump_flist_insn_flags = (DUMP_INSN_UID | DUMP_INSN_BBN
                                    | DUMP_INSN_SEQNO);


/* Core functions for pretty printing.  */

static void
push_indent (int new)
{
  if (indents_n == indents_sz)
    indents = xrealloc (indents, ((indents_sz = indents_sz * 2 + 1)
				  * sizeof (*indents)));
  indents[indents_n++] = indent;
  indent = new;
}

static void
pop_indent (void)
{
  indent = indents[--indents_n];
}

static void
push_new_line (bool new)
{
  if (new_lines_n == new_lines_sz)
    new_lines = xrealloc (new_lines, ((new_lines_sz = new_lines_sz * 2 + 1)
				      * sizeof (*new_lines)));
  new_lines[new_lines_n++] = new_line;
  new_line = new;
}

static void
pop_new_line (void)
{
  new_line = new_lines[--new_lines_n];
}

void
block_start (void)
{  
  indent++;

  if (print_blocks_n == print_blocks_sz)
    print_blocks = xrealloc (print_blocks,
			     ((print_blocks_sz = print_blocks_sz * 2 + 1)
			      * sizeof (*print_blocks)));

  print_blocks[print_blocks_n++] = print_block;
  print_block = indent < sched_verbose;
}

void
block_finish (void)
{
  print_block = print_blocks[--print_blocks_n];
  indent--;
}

void
line_start (void)
{
  push_new_line (false);
  print ("%s", "");
  push_indent (0);
}

void
line_finish (void)
{
  pop_new_line ();
  print ("%s", "");
  pop_indent ();
}

static void
switch_dump (void)
{
  FILE *f = sched_dump1;
  int i = indent1;
  bool b = new_line1;
  bool b2 = print_block1;

  sched_dump1 = sched_dump;
  sched_dump = f;
  
  indent1 = indent;
  indent = i; 

  new_line1 = new_line;
  new_line = b;

  print_block1 = print_block;
  print_block = b2;
}

void 
print_marker_to_log (void)
{
  print ("Marker: %d", sel_dump_cfg_fileno);
}

void
setup_sched_dump_to_stderr (void)
{
  sched_dump1 = stderr;
}
 

void
free_sel_dump_data (void)
{
  gcc_assert (!indents_n && !new_lines_n && !print_blocks_n);
  indents_sz = 0;
  free (indents);
  indents = NULL;
  new_lines_sz = 0;
  free (new_lines);
  new_lines = NULL;
  print_blocks_sz = 0;
  free (print_blocks);
  print_blocks = NULL;
}


/* Functions for dumping instructions, av sets, and rhses.  */ 

/* Print an rtx X.  */
void
sel_print_rtl (rtx x)
{
  if (print_block)	  
    {
      print_rtl_single (sched_dump, x);
    }  
}

/* Dump insn I honoring FLAGS.  */
void
dump_insn_1 (insn_t i, int flags)
{
  if (print_block)
    {
      line_start ();

      if (!sched_dump_to_dot_p)
	print ("(");

      if (flags & DUMP_INSN_PATTERN)
	{
	  char buf[2048];

	  print_insn (buf, i, 0);
	  print ("pat:%s;", buf);
	}
      else if (flags & DUMP_INSN_UID)
	print ("uid:%d;", INSN_UID (i));

      if (flags & DUMP_INSN_BBN)
	{
	  basic_block bb = BLOCK_FOR_INSN (i);

	  print ("bb:%d;", bb ? bb->index : -1);
	}

      if (flags & DUMP_INSN_SEQNO)
	print ("seqno:%d;", INSN_SEQNO (i));

      if (flags & DUMP_INSN_COUNT)
	print ("count:%d;", VINSN_COUNT (INSN_VI (i)));

      if (flags & DUMP_INSN_CYCLE)
	print ("cycle:%d;", INSN_SCHED_CYCLE (i));

      if (!sched_dump_to_dot_p)
	print (")");

      line_finish ();
    }
}

/* Dump insn I with default flags.  */
void
dump_insn (insn_t i)
{
  dump_insn_1 (i, dump_insn_flags);
}

/* Dump RHS.  */
void
dump_rhs (rhs_t rhs)
{
  if (print_block)
    {
      insn_t i = RHS_INSN (rhs);
      
      line_start ();
      print ("[");
      dump_insn_1 (i, dump_rhs_insn_flags | DUMP_INSN_PATTERN);
      print (";as_rhs:%d", RHS_SCHEDULE_AS_RHS (rhs));
      print (";spec:%d]", RHS_SPEC (rhs));
      line_finish ();
    }
}

/* Dump used regs from USED_REGS bitmap.  */
void
dump_used_regs (bitmap used_regs)
{
  if (print_block)
    {
      line_start ();
      bitmap_print (sched_dump, used_regs, "Used regs: { ", " }");
      line_finish ();
    }
}

/* Dumps av_set AV.  */
void
dump_av_set (av_set_t av)
{
  if (print_block)
    {
      av_set_iterator i;
      rhs_t rhs;

      line_start ();

      if (!sched_dump_to_dot_p)
	print ("{");

      FOR_EACH_RHS (rhs, i, av)
        {
          dump_rhs (rhs);
	  if (!sched_dump_to_dot_p)
	    print (" ");
	  else
	    print ("\n");
        }

      if (!sched_dump_to_dot_p)
	print ("}");

      line_finish ();
    }
}

/* Dumps lvset LV.  */
void
dump_lv_set (regset lv)
{
  if (print_block)
    {
      line_start ();
      print ("{");

      /* This code was adapted from flow.c: dump_regset ().  */
      if (lv == NULL)
	print ("nil");
      else
	{
	  unsigned i;
	  reg_set_iterator rsi;
	  int count = 0;

	  EXECUTE_IF_SET_IN_REG_SET (lv, 0, i, rsi)
	    {
	      print (" %d", i);
	      if (i < FIRST_PSEUDO_REGISTER)
		{
		  print (" [%s]", reg_names[i]);
		  ++count;
		}

	      ++count;

	      if (sched_dump_to_dot_p && count == 12)
		{
		  count = 0;
		  print ("\n");
		}
	    }
	}

      print ("}");
      line_finish ();
    }
}

/* Dumps a list of instructions pointed to by P.  */
static void
dump_ilist (ilist_t p)
{
  if (print_block)
    {
      line_start ();
      while (p)
        {
          dump_insn (ILIST_INSN (p));
          p = ILIST_NEXT (p);
        }
      line_finish ();
    }
}

/* Dumps a list of boundaries pointed to by BNDS.  */
void
dump_blist (blist_t bnds)
{
  if (print_block)
    {
      line_start ();
      for (; bnds; bnds = BLIST_NEXT (bnds))
        {
          bnd_t bnd = BLIST_BND (bnds);

          print ("[to: %d; ptr: ", INSN_UID (BND_TO (bnd)));
          dump_ilist (BND_PTR (bnd));
          print ("] ");
        }
      line_finish ();
    }
}

/* Dumps a list of fences pointed to by L.  */
void
dump_flist (flist_t l)
{
  if (print_block)
    {
      line_start ();
      while (l)
        {
          dump_insn_1 (FENCE_INSN (FLIST_FENCE (l)), dump_flist_insn_flags);
          print (" ");
          l = FLIST_NEXT (l);
        }
      line_finish ();
    }
}

/* Dumps an insn array SUUCS with N elements.  */
void
dump_insn_array (rtx *succs, int n)
{
  if (print_block)
    {
      int i;

      line_start ();
      for (i = 0; i < n; i++)
        if (succs[i])
          dump_insn (succs[i]);
        else
          print ("NULL ");
      line_finish ();
    }
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
	fprintf (stderr, "%d ", i);
    }
  fprintf (stderr, "}\n");
}

/* Dumps a hard reg set SET using PREFIX.  */
void
dump_hard_reg_set (const char *prefix, HARD_REG_SET set)
{
  if (print_block)
    {
      line_start ();
      print_hard_reg_set (sched_dump, prefix, set);
      line_finish ();
    }
}

/* Pretty print INSN.  This is used as a hook.  */
const char *
sel_print_insn (rtx insn, int aligned ATTRIBUTE_UNUSED)
{
  static char buf[80];

  /* '+' before insn means it is a new cycle start and it's not been 
     scheduled yet.  '>' - has been scheduled.  */
  if (s_i_d && INSN_VI (insn) && INSN_UID (insn) < sel_max_uid)
    if (GET_MODE (insn) == TImode)
      sprintf (buf, "%s %4d", 
               (VINSN_SCHED_TIMES (INSN_VI (insn)) > 0) ? "> " : "< ", 
               INSN_UID (insn));
    else
      sprintf (buf, "%s %4d", 
               VINSN_SCHED_TIMES (INSN_VI (insn)) > 0 ? "! " : "  ", 
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

  if ((flags & SEL_DUMP_CFG_INSN_SEQNO)
      && TEST_BIT (sched_insns, INSN_UID (insn)))
    insn_flags |= DUMP_INSN_SEQNO | DUMP_INSN_CYCLE;

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
	  && in_region_p && !bb_empty_p (bb)
	  && IN_CURRENT_FENCE_P (NEXT_INSN (bb_note (bb))))
	{
	  /*style = "style = dotted, ";*/
	  style = "";
	  color = "color = red, ";
	}
      else if (!full_p)
	style = "style = dashed, ";
      else
	style = "";

      fprintf (f, "\tbb%d [%s%slabel = \"{Basic block %d", bb->index,
	       style, color, bb->index);

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
	  && !bb_empty_p (bb))
	{
	  insn_t head = NEXT_INSN (bb_note (bb));

	  fprintf (f, "|");

	  if (INSN_AV_VALID_P (head))
	    dump_av_set (AV_SET (head));
	  else
	    {
	      fprintf (f, "!!! Wrong AV_SET%s",
		       (AV_LEVEL (head) == -1) ? ": but ok" : "");
	    }
	}

      if ((flags & SEL_DUMP_CFG_LV_SET)
	  && !bb_empty_p (bb))
	{
	  insn_t head;
	  insn_t tail;

	  get_ebb_head_tail (bb, bb, &head, &tail);

	  fprintf (f, "|");

	  if (INSN_P (head) && LV_SET_VALID_P (head))
	    dump_lv_set (LV_SET (head));
	  else
	    fprintf (f, "!!! Wrong LV_SET");
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
setup_dump_cfg_params (bool pipelining_p)
{
  int dump_flags = PARAM_VALUE (PARAM_SELSCHED_DUMP_CFG_FLAGS);

  if (dump_flags != 0)
    sel_dump_cfg_flags = dump_flags;
  else
    sel_dump_cfg_flags = SEL_DUMP_CFG_FLAGS;

  sel_dump_cfg_p = (flag_sel_sched_dump_cfg != 0);
  sel_debug_cfg_root_postfix = sel_debug_cfg_root_postfix_default;

  sel_pipelining_verbose_p = (flag_sel_sched_pipelining_verbose != 0);
  if (pipelining_p && sel_pipelining_verbose_p)
    {
      sel_dump_cfg_p = true;
      sel_debug_cfg_root_postfix = main_input_filename;
    }
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

void
debug_rhs (rhs_t r)
{
  switch_dump ();
  dump_rhs (r);
  switch_dump ();
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

/* Dump an insn array SUCCS with N elements.  */
void
debug_insn_array (rtx *succs, int n)
{
  switch_dump ();
  dump_insn_array (succs, n);
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

/* Find a loop in the instruction stream.  */
bool
debug_find_insn_loop (void)
{
  sbitmap insn_uids;
  insn_t insn;
  bool res = false;

  insn_uids = sbitmap_alloc (get_max_uid () + 1);
  sbitmap_zero (insn_uids);

  for (insn = get_insns (); insn != NULL_RTX; insn = NEXT_INSN (insn))
    {
      insn_t prev = PREV_INSN (insn);
      insn_t next = NEXT_INSN (insn);

      if ((prev && NEXT_INSN (prev) != insn)
	  || (next && PREV_INSN (next) != insn))
	{
	  fprintf (stderr, "wrong!!!\n");
	  res = true;
	  break;
	}

      if (TEST_BIT (insn_uids, INSN_UID (insn)))
	{
	  fprintf (stderr, "loop!!!\n");
	  res = true;
	  break;
	}

      SET_BIT (insn_uids, INSN_UID (insn));
    }

  sbitmap_free (insn_uids);

  return res;
}

/* Find unreachable basic blocks.  */
bool
debug_find_unreachable_blocks (void)
{
  int *pre_order = NULL;
  int *rev_post_order = NULL;
  edge_iterator *stack;
  int sp;
  int pre_order_num = 0;
  int rev_post_order_num = n_basic_blocks - 1;
  sbitmap visited;
  basic_block bb;

  /* Allocate stack for back-tracking up CFG.  */
  stack = XNEWVEC (edge_iterator, n_basic_blocks + 1);
  sp = 0;

  rev_post_order_num -= NUM_FIXED_BLOCKS;

  /* Allocate bitmap to track nodes that have been visited.  */
  visited = sbitmap_alloc (last_basic_block);

  /* None of the nodes in the CFG have been visited yet.  */
  sbitmap_zero (visited);

  /* Push the first edge on to the stack.  */
  stack[sp++] = ei_start (ENTRY_BLOCK_PTR->succs);

  while (sp)
    {
      edge_iterator ei;
      basic_block src;
      basic_block dest;

      /* Look at the edge on the top of the stack.  */
      ei = stack[sp - 1];
      src = ei_edge (ei)->src;
      dest = ei_edge (ei)->dest;

      /* Check if the edge destination has been visited yet.  */
      if (dest != EXIT_BLOCK_PTR && ! TEST_BIT (visited, dest->index))
	{
	  /* Mark that we have visited the destination.  */
	  SET_BIT (visited, dest->index);

	  if (pre_order)
	    pre_order[pre_order_num] = dest->index;

	  pre_order_num++;

	  if (EDGE_COUNT (dest->succs) > 0)
	    /* Since the DEST node has been visited for the first
	       time, check its successors.  */
	    stack[sp++] = ei_start (dest->succs);
	  else if (rev_post_order)
	    /* There are no successors for the DEST node so assign
	       its reverse completion number.  */
	    rev_post_order[rev_post_order_num--] = dest->index;
	}
      else
	{
	  if (ei_one_before_end_p (ei) && src != ENTRY_BLOCK_PTR
	      && rev_post_order)
	    /* There are no more successors for the SRC node
	       so assign its reverse completion number.  */
	    rev_post_order[rev_post_order_num--] = src->index;

	  if (!ei_one_before_end_p (ei))
	    ei_next (&stack[sp - 1]);
	  else
	    sp--;
	}
    }

  free (stack);

  FOR_EACH_BB (bb)
    if (!TEST_BIT (visited, bb->index))
      fprintf (stderr, "Unreachable: %d\n", bb->index);

  sbitmap_free (visited);

  /* The number of nodes visited should be the number of blocks minus
     the entry and exit blocks which are not visited here.  */

  return !(pre_order_num == n_basic_blocks - NUM_FIXED_BLOCKS);
}
