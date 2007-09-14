/* Pretty formatting of GIMPLE statements and expressions.
   Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
   Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com> and
   Diego Novillo <dnovillo@google.com>

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
#include "tree.h"
#include "diagnostic.h"
#include "real.h"
#include "hashtab.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "gimple.h"
#include "value-prof.h"

#define INDENT(SPACE)							\
  do { int i; for (i = 0; i < SPACE; i++) pp_space (buffer); } while (0)

static pretty_printer buffer;
static bool initialized = false;

#define GIMPLE_NIY do_niy(buffer,gs)

/* Try to print on BUFFER a default message for the unrecognized
   gimple statement GS.  */

static void
do_niy (pretty_printer *buffer, gimple gs)
{
  pp_printf (buffer, "<<< Unknown GIMPLE statement: %s >>>\n",
	     gimple_code_name[(int) gimple_code (gs)]);
}


/* Initialize the pretty printer on FILE if needed.  */

static void
maybe_init_pretty_print (FILE *file)
{
  if (!initialized)
    {
      pp_construct (&buffer, NULL, 0);
      pp_needs_newline (&buffer) = true;
      initialized = true;
    }

  buffer.buffer->stream = file;
}


/* Emit a newline and SPC indentantion spaces to BUFFER.  */

static void
newline_and_indent (pretty_printer *buffer, int spc)
{
  pp_newline (buffer);
  INDENT (spc);
}


/* Print the GIMPLE statement GS on stderr.  */

void
debug_gimple_stmt (gimple gs)
{
  print_gimple_stmt (stderr, gs, 0, TDF_VOPS|TDF_MEMSYMS);
  fprintf (stderr, "\n");
}


/* Dump GIMPLE statement G to FILE using SPC indentantion spaces and
   FLAGS as in dump_gimple_stmt.  */

void
print_gimple_stmt (FILE *file, gimple g, int spc, int flags)
{
  maybe_init_pretty_print (file);
  dump_gimple_stmt (&buffer, g, spc, flags);
  pp_flush (&buffer);
}


/* Print the GIMPLE sequence SEQ on BUFFER using SPC indentantion
   spaces and FLAGS as in dump_gimple_stmt.  */

static void
dump_gimple_seq (pretty_printer *buffer, gimple_seq seq, int spc, int flags)
{
  gimple_stmt_iterator *i;

  for (i = gsi_start (seq); !gsi_end_p (i); gsi_next (i))
    {
      gimple gs = gsi_stmt (i);
      if (flags & TDF_DETAILS)
	pp_string (buffer, "gimpleir: ");
      dump_gimple_stmt (buffer, gs, spc, flags);
    }
}


/* Dump GIMPLE sequence SEQ to FILE using SPC indentantion spaces and
   FLAGS as in dump_gimple_stmt.  */

void
print_gimple_seq (FILE *file, gimple_seq seq, int spc, int flags)
{
  maybe_init_pretty_print (file);
  dump_gimple_seq (&buffer, seq, spc, flags);
  pp_flush (&buffer);
}


/* Print the GIMPLE sequence SEQ on stderr.  */

void
debug_gimple_seq (gimple_seq seq)
{
  print_gimple_seq (stderr, seq, 0, TDF_VOPS|TDF_MEMSYMS);
}


/* Helper for dump_gimple_assign.  Print the unary RHS of the
   assignment GS.  BUFFER, SPC and FLAGS are as in dump_gimple_stmt.  */

static void
dump_unary_rhs (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  enum tree_code rhs_code = gimple_assign_subcode (gs);
  tree lhs = gimple_assign_lhs (gs);
  tree rhs = gimple_assign_rhs1 (gs);

  switch (rhs_code)
    {
    case FIXED_CONVERT_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case CONVERT_EXPR:
      pp_string (buffer, tree_code_name [rhs_code]);
      pp_string (buffer, " <");
      dump_generic_node (buffer, TREE_TYPE (lhs), spc, flags, false);
      pp_string (buffer, ", ");
      dump_generic_node (buffer, rhs, spc, flags, false);
      pp_string (buffer, ">");
      break;

    case NOP_EXPR:
      pp_string (buffer, "(");
      dump_generic_node (buffer, TREE_TYPE (lhs), spc, flags, false);
      pp_string (buffer, ") ");
      dump_generic_node (buffer, rhs, spc, flags, false);
      break;

    default:
      if (TREE_CODE_CLASS (rhs_code) == tcc_declaration
	  || TREE_CODE_CLASS (rhs_code) == tcc_constant
	  || TREE_CODE_CLASS (rhs_code) == tcc_reference
	  || rhs_code == SSA_NAME)
	; /* do nothing.  */
      else if (rhs_code == ADDR_EXPR)
	pp_string (buffer, "&");
      else if (rhs_code == BIT_NOT_EXPR)
	pp_string (buffer, "~");
      else if (rhs_code == TRUTH_NOT_EXPR)
	pp_string (buffer, "!");
      else
	{
	  pp_string (buffer, "[");
	  pp_string (buffer, tree_code_name [rhs_code]);
	  pp_string (buffer, "] ");
	}

      dump_generic_node (buffer, rhs, spc, flags, false);
      break;
    }
}


/* Helper for dump_gimple_assign.  Print the binary RHS of the
   assignment GS.  BUFFER, SPC and FLAGS are as in dump_gimple_stmt.  */

static void
dump_binary_rhs (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  switch (gimple_assign_subcode (gs))
    {
    case COMPLEX_EXPR:
      pp_string (buffer, "COMPLEX_EXPR <");
      dump_generic_node (buffer, gimple_assign_rhs1 (gs), spc, flags, false);
      pp_string (buffer, ", ");
      dump_generic_node (buffer, gimple_assign_rhs2 (gs), spc, flags, false);
      pp_string (buffer, ">");
      break;

    default:
      dump_generic_node (buffer, gimple_assign_rhs1 (gs), spc, flags, false);
      pp_space (buffer);
      pp_string (buffer, op_symbol_code (gimple_assign_subcode (gs)));
      pp_space (buffer);
      dump_generic_node (buffer, gimple_assign_rhs2 (gs), spc, flags, false);
    }
}


/* Dump the gimple assignment GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_assign (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  dump_generic_node (buffer, gimple_assign_lhs (gs), spc, flags, false);
  pp_space (buffer);
  pp_character (buffer, '=');
  pp_space (buffer);

  if (gimple_num_ops (gs) == 2)
    dump_unary_rhs (buffer, gs, spc, flags);
  else if (gimple_num_ops (gs) == 3)
    dump_binary_rhs (buffer, gs, spc, flags);
  else
    gcc_unreachable ();
}


/* Dump the return statement GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_return (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  tree t;

  pp_string (buffer, "return");
  t = gimple_return_retval (gs);
  if (t)
    {
      pp_space (buffer);
      if (TREE_CODE (t) == GIMPLE_MODIFY_STMT)
	dump_generic_node (buffer, GIMPLE_STMT_OPERAND (t, 1),
	    spc, flags, false);
      else
	dump_generic_node (buffer, t, spc, flags, false);
    }
}


/* Dump the call statement GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_call (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  size_t i;
  tree lhs = gimple_call_lhs (gs);

  if (lhs)
    {
      dump_generic_node (buffer, lhs, spc, flags, false);
      pp_string (buffer, " = ");
    }

  dump_generic_node (buffer, gimple_call_fn (gs), spc, flags, false);
  pp_string (buffer, " (");

  for (i = 0; i < gimple_call_nargs (gs); i++)
    {
      dump_generic_node (buffer, gimple_call_arg (gs, i), 0, flags, false);
      if (i < gimple_call_nargs (gs) - 1)
	pp_string (buffer, ", ");
    }

  pp_string (buffer, ")");
}


/* Dump the switch statement GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_switch (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  unsigned int i;

  GIMPLE_CHECK (gs, GIMPLE_SWITCH);
  pp_string (buffer, "switch (");
  dump_generic_node (buffer, gimple_switch_index (gs), spc, flags, true);
  pp_string (buffer, ") <");
  for (i = 0; i < gimple_switch_num_labels (gs); i++)
    {
      dump_generic_node (buffer, gimple_switch_label (gs, i), spc, flags,
                         false);
      if (i < gimple_switch_num_labels (gs) - 1)
        pp_string (buffer, ", ");
    }
  pp_string (buffer, ">");
}


/* Dump the gimple conditional GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_cond (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GIMPLE_COND tuple");

  pp_string (buffer, "if (");
  dump_generic_node (buffer, gimple_cond_lhs (gs), spc, flags, false);
  pp_space (buffer);
  pp_string (buffer, tree_code_name [gimple_cond_code (gs)]);
  pp_space (buffer);
  dump_generic_node (buffer, gimple_cond_rhs (gs), spc, flags, false);
  pp_string (buffer, ") goto ");
  dump_generic_node (buffer, gimple_cond_true_label (gs), spc, flags, false);
  pp_string (buffer, " else goto ");
  dump_generic_node (buffer, gimple_cond_false_label (gs), spc, flags, false);
}


/* Dump a GIMPLE_LABEL tuple on the pretty_printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h)*/

static void
dump_gimple_label (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  tree label = gimple_label_label (gs);
  dump_generic_node (buffer, label, spc, flags, false);
  if (TREE_CODE (label) != CASE_LABEL_EXPR )
    pp_string (buffer, ":");
}


/* Dump a GIMPLE_BIND tuple on the pretty_printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h).  */

static void
dump_gimple_bind (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GIMPLE_BIND tuple");

  pp_character (buffer, '{');
  newline_and_indent (buffer, spc + 2);
  if (!(flags & TDF_SLIM))
    {
      tree var;

      for (var = gimple_bind_vars (gs); var; var = TREE_CHAIN (var))
	{
	  print_declaration (buffer, var, spc, flags);
	  newline_and_indent (buffer, spc + 2);
	}
      newline_and_indent (buffer, spc + 2);
    }

  dump_gimple_seq (buffer, gimple_bind_body (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  pp_character (buffer, '}');
}


/* Dump a GIMPLE_TRY tuple on the pretty_printer BUFFER, SPC spaces of
   indent.  FLAGS specifies details to show in the dump (see TDF_* in
   tree.h).  */

static void
dump_gimple_try (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GIMPLE_TRY tuple");

  pp_string (buffer, "try {");
  newline_and_indent (buffer, spc + 2);
  dump_gimple_seq (buffer, gimple_try_eval (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  if (gimple_try_kind (gs) == GIMPLE_TRY_CATCH)
    pp_string (buffer, "} catch {");
  else if (gimple_try_kind (gs) == GIMPLE_TRY_FINALLY)
    pp_string (buffer, "} finally {");
  else
    pp_string (buffer, "} <UNKNOWN GIMPLE_TRY> {");
  newline_and_indent (buffer, spc + 2);
  dump_gimple_seq (buffer, gimple_try_cleanup (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  pp_character (buffer, '}');
}

/* Dump a GIMPLE_ASM tuple on the pretty_printer BUFFER, SPC spaces of
   indent.  FLAGS specifies details to show in the dump (see TDF_* in
   tree.h).  */

static void
dump_gimple_asm (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  unsigned int i;

  pp_string (buffer, "__asm__ (");
  pp_string (buffer, gimple_asm_string (gs));

  if (gimple_asm_ninputs (gs)
     || gimple_asm_noutputs (gs) 
     || gimple_asm_nclobbered (gs))
    {
      pp_character (buffer, ' ');
      for (i = 0; i < gimple_asm_ninputs (gs); i++)
        {
          dump_generic_node (buffer, gimple_asm_input_op (gs,i),spc, flags,
                             false);
          if (i < gimple_asm_ninputs (gs) -1)
            pp_string (buffer, ": ");
        }
      pp_string (buffer, ": ");
      for (i = 0; i < gimple_asm_noutputs (gs); i++)
        {
          dump_generic_node (buffer, gimple_asm_output_op (gs,i),spc, flags,
                             false);
          if ( i < gimple_asm_noutputs (gs) -1)
            pp_string (buffer, ", ");
        }
      pp_string (buffer, ": ");
      
      for (i = 0; i < gimple_asm_nclobbered (gs); i++)
        {
          dump_generic_node (buffer, gimple_asm_clobber_op (gs,i),spc, flags,
                             false);
          if ( i < gimple_asm_nclobbered (gs) -1)
            pp_string (buffer, ", ");
        }
    }
  pp_character (buffer, ')');
}


/* Dump the set of decls SYMS.  BUFFER, SPC and FLAGS are as in
   dump_generic_node.  */

static void
dump_symbols (pretty_printer *buffer, bitmap syms, int flags)
{
  unsigned i;
  bitmap_iterator bi;

  if (syms == NULL)
    pp_string (buffer, "NIL");
  else
    {
      pp_string (buffer, " { ");

      EXECUTE_IF_SET_IN_BITMAP (syms, 0, i, bi)
	{
	  tree sym = referenced_var_lookup (i);
	  dump_generic_node (buffer, sym, 0, flags, false);
	  pp_string (buffer, " ");
	}

      pp_string (buffer, "}");
    }
}


/* Dump a PHI node PHI.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gimple_phi (pretty_printer *buffer, gimple phi, int spc, int flags)
{
  size_t i;

  dump_generic_node (buffer, gimple_phi_result (phi), spc, flags, false);
  pp_string (buffer, " = PHI <");
  for (i = 0; i < gimple_phi_num_args (phi); i++)
    {
      dump_generic_node (buffer, gimple_phi_arg_def (phi, i), spc, flags,
			 false);
      pp_string (buffer, "(");
      pp_decimal_int (buffer, gimple_phi_arg_edge (phi, i)->src->index);
      pp_string (buffer, ")");
      if (i < gimple_phi_num_args (phi) - 1)
	pp_string (buffer, ", ");
    }
  pp_string (buffer, ">");

  if (stmt_references_memory_p (phi) && (flags & TDF_MEMSYMS))
    dump_symbols (buffer, gimple_stored_syms (phi), flags);
}

/* Dump the gimple statement GS on the pretty printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h).  */

void
dump_gimple_stmt (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (!gs)
    return;

  switch (gimple_code (gs))
    {
    case GIMPLE_ASM:
      dump_gimple_asm (buffer, gs, spc, flags);
      break;

    case GIMPLE_ASSIGN:
      dump_gimple_assign (buffer, gs, spc, flags);
      break;

    case GIMPLE_BIND:
      dump_gimple_bind (buffer, gs, spc, flags);
      break;

    case GIMPLE_CALL:
      dump_gimple_call (buffer, gs, spc, flags);
      break;

    case GIMPLE_COND:
      dump_gimple_cond (buffer, gs, spc, flags);
      break;

    case GIMPLE_LABEL:
      dump_gimple_label (buffer, gs, spc, flags);
      break;

    case GIMPLE_GOTO:
      pp_string (buffer, "goto ");
      dump_generic_node (buffer, gimple_goto_dest (gs), spc, flags, false);
      break;

    case GIMPLE_NOP:
      pp_string (buffer, "GIMPLE_NOP");
      break;

    case GIMPLE_RETURN:
      dump_gimple_return (buffer, gs, spc, flags);
      break;

    case GIMPLE_SWITCH:
      dump_gimple_switch (buffer, gs, spc, flags);
      break;

    case GIMPLE_TRY:
      dump_gimple_try (buffer, gs, spc, flags);
      break;

    case GIMPLE_PHI:
      dump_gimple_phi (buffer, gs, spc, flags);
      break;

    default:
      GIMPLE_NIY;
    }

  newline_and_indent (buffer, spc);
  pp_write_text_to_stream (buffer);
}


/* Dumps header of basic block BB to buffer BUFFER indented by INDENT
   spaces and details described by flags.  */

static void
dump_bb_header (pretty_printer *buffer, basic_block bb, int indent, int flags)
{
  edge e;
  gimple stmt;
  edge_iterator ei;

  if (flags & TDF_BLOCKS)
    {
      INDENT (indent);
      pp_string (buffer, "# BLOCK ");
      pp_decimal_int (buffer, bb->index);
      if (bb->frequency)
	{
          pp_string (buffer, " freq:");
          pp_decimal_int (buffer, bb->frequency);
	}
      if (bb->count)
	{
          pp_string (buffer, " count:");
          pp_widest_integer (buffer, bb->count);
	}

      if (flags & TDF_LINENO)
	{
	  gimple_stmt_iterator *gsi;

	  for (gsi = gsi_start (bb_seq (bb)); !gsi_end_p (gsi); gsi_next (gsi))
	    if (get_lineno (gsi_stmt (gsi)) != -1)
	      {
		pp_string (buffer, ", starting at line ");
		pp_decimal_int (buffer, get_lineno (gsi_stmt (gsi)));
		break;
	      }
	}
      newline_and_indent (buffer, indent);

      pp_string (buffer, "# PRED:");
      pp_write_text_to_stream (buffer);
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (flags & TDF_SLIM)
	  {
	    pp_string (buffer, " ");
	    if (e->src == ENTRY_BLOCK_PTR)
	      pp_string (buffer, "ENTRY");
	    else
	      pp_decimal_int (buffer, e->src->index);
	  }
	else
	  dump_edge_info (buffer->buffer->stream, e, 0);
      pp_newline (buffer);
    }
  else
    {
      stmt = first_stmt (bb);
      if (!stmt || gimple_code (stmt) != GIMPLE_LABEL)
	{
	  INDENT (indent - 2);
	  pp_string (buffer, "<bb ");
	  pp_decimal_int (buffer, bb->index);
	  pp_string (buffer, ">:");
	  pp_newline (buffer);
	}
    }
  pp_write_text_to_stream (buffer);
  check_bb_profile (bb, buffer->buffer->stream);
}


/* Dumps end of basic block BB to buffer BUFFER indented by INDENT
   spaces.  */

static void
dump_bb_end (pretty_printer *buffer, basic_block bb, int indent, int flags)
{
  edge e;
  edge_iterator ei;

  INDENT (indent);
  pp_string (buffer, "# SUCC:");
  pp_write_text_to_stream (buffer);
  FOR_EACH_EDGE (e, ei, bb->succs)
    if (flags & TDF_SLIM)
      {
	pp_string (buffer, " ");
	if (e->dest == EXIT_BLOCK_PTR)
	  pp_string (buffer, "EXIT");
	else
	  pp_decimal_int (buffer, e->dest->index);
      }
    else
      dump_edge_info (buffer->buffer->stream, e, 1);
  pp_newline (buffer);
}


/* Dump PHI nodes of basic block BB to BUFFER with details described
   by FLAGS and indented by INDENT spaces.  */

static void
dump_phi_nodes (pretty_printer *buffer, basic_block bb, int indent, int flags)
{
  gimple_stmt_iterator *i;

  for (i = gsi_start (phi_nodes (bb)); !gsi_end_p (i); gsi_next (i))
    {
      gimple phi = gsi_stmt (i);
      if (is_gimple_reg (gimple_phi_result (phi)) || (flags & TDF_VOPS))
        {
          INDENT (indent);
          pp_string (buffer, "# ");
          dump_gimple_phi (buffer, phi, indent, flags);
          pp_newline (buffer);
        }
    }
}


/* Dumps basic block BB to buffer BUFFER with details described by FLAGS and
   indented by INDENT spaces.  */

static void
gimple_dump_bb_buff (pretty_printer *buffer, basic_block bb, int indent,
		     int flags)
{
  gimple_stmt_iterator *gsi;
  gimple stmt;
  int label_indent = indent - 2;

  if (label_indent < 0)
    label_indent = 0;

  dump_bb_header (buffer, bb, indent, flags);
  dump_phi_nodes (buffer, bb, indent, flags);

  for (gsi = gsi_start (bb_seq (bb)); !gsi_end_p (gsi); gsi_next (gsi))
    {
      int curr_indent;

      stmt = gsi_stmt (gsi);

      curr_indent = gimple_code (stmt) == GIMPLE_LABEL ? label_indent : indent;

      INDENT (curr_indent);
      dump_gimple_stmt (buffer, stmt, curr_indent, flags);
      dump_histograms_for_stmt (cfun, buffer->buffer->stream, stmt);
    }

  /*dump_implicit_edges (buffer, bb, indent, flags);*/

  if (flags & TDF_BLOCKS)
    dump_bb_end (buffer, bb, indent, flags);
}


/* Dumps basic block BB to FILE with details described by FLAGS and
   indented by INDENT spaces.  */

void
gimple_dump_bb (basic_block bb, FILE *file, int indent, int flags)
{
  maybe_init_pretty_print (file);
  gimple_dump_bb_buff (&buffer, bb, indent, flags);
  pp_flush (&buffer);
}
