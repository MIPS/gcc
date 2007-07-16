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
#include "gimple-iterator.h"
#include "tree-pass.h"
#include "gimple.h"

#define INDENT(SPACE) do { \
  int i; for (i = 0; i<SPACE; i++) pp_space (buffer); } while (0)

static pretty_printer buffer;
static bool initialized = false;

#define GS_NIY do_niy(buffer,gs)

/* Try to print on BUFFER a default message for the unrecognized
   gimple statement GS.  */

static void
do_niy (pretty_printer *buffer, gimple gs)
{
  pp_printf (buffer, "<<< Unknown GIMPLE statement: %s >>>\n",
	     gs_code_name[(int) GS_CODE (gs)]);
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
dump_gimple_seq (pretty_printer *buffer, gs_seq seq, int spc, int flags)
{
  gimple_stmt_iterator i;

  for (i = gsi_start (seq); !gsi_end_p (i); gsi_next (&i))
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
print_gimple_seq (FILE *file, gs_seq seq, int spc, int flags)
{
  maybe_init_pretty_print (file);
  dump_gimple_seq (&buffer, seq, spc, flags);
  pp_flush (&buffer);
}


/* Print the GIMPLE sequence SEQ on stderr.  */

void
debug_gimple_seq (gs_seq seq)
{
  print_gimple_seq (stderr, seq, 0, TDF_VOPS|TDF_MEMSYMS);
}


/* Dump the gimple assignment GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gs_assign (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  enum gimple_statement_structure_enum gss;

  dump_generic_node (buffer, gs_assign_lhs (gs), spc, flags, false);
  pp_space (buffer);
  pp_character (buffer, '=');
  pp_space (buffer);

  gss = gss_for_assign (GS_SUBCODE_FLAGS (gs));
  switch (gss)
    {
      case GSS_ASSIGN_BINARY:
	dump_generic_node (buffer, gs_assign_binary_rhs1 (gs), spc,
			   flags, false);
	pp_space (buffer);
	pp_string (buffer, op_symbol_code (GS_SUBCODE_FLAGS (gs)));
	pp_space (buffer);
	dump_generic_node (buffer, gs_assign_binary_rhs2 (gs), spc,
			   flags, false);
	break;

      case GSS_ASSIGN_UNARY_REG:
      case GSS_ASSIGN_UNARY_MEM:
	dump_generic_node (buffer, gs_assign_unary_rhs (gs), spc, flags, false);
	break;

      default:
	gcc_unreachable ();
    }
}


/* Dump the return statement GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gs_return (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  tree t;

  pp_string (buffer, "return");
  t = gs_return_retval (gs);
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
dump_gs_call (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  size_t i;
  tree lhs = gs_call_lhs (gs);

  if (lhs)
    {
      dump_generic_node (buffer, lhs, spc, flags, false);
      pp_string (buffer, " = ");
    }

  dump_generic_node (buffer, gs_call_fn (gs), spc, flags, false);
  pp_string (buffer, " (");

  for (i = 0; i < gs_call_nargs (gs); i++)
    {
      dump_generic_node (buffer, gs_call_arg (gs, i), 0, flags, false);
      if (i < gs_call_nargs (gs) - 1)
	pp_string (buffer, ", ");
    }

  pp_string (buffer, ")");
}


/* Dump the switch statement GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gs_switch (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  unsigned int i;
  GS_CHECK (gs, GS_SWITCH);
  pp_string (buffer, "switch (");
  dump_generic_node (buffer, gs_switch_index (gs), spc, flags, true);
  pp_string (buffer, ") <");
  for (i = 0; i < gs_switch_nlabels (gs); i++)
    {
      dump_generic_node (buffer, gs_switch_label (gs, i), spc, flags, false);
      if (i < gs_switch_nlabels (gs) - 1)
        pp_string (buffer, ", ");
    }
  pp_string (buffer, ">");
}


/* Return the symbol associated with the GS_COND predicate PRED.  */

static const char *
op_gs_cond (enum gs_cond pred)
{
  /* These must be in sync with enum gs_cond.  */
  static const char *table[] =
    { "<", ">", "<=", ">=", "==", "!=" };

  return table[(int) pred];
}


/* Dump the gimple conditional GS.  BUFFER, SPC and FLAGS are as in
   dump_gimple_stmt.  */

static void
dump_gs_cond (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GS_COND tuple");

  newline_and_indent (buffer, spc);
  pp_string (buffer, "if (");
  dump_generic_node (buffer, gs_cond_lhs (gs), spc, flags, false);
  pp_space (buffer);
  pp_string (buffer, op_gs_cond (GS_SUBCODE_FLAGS (gs)));
  pp_space (buffer);
  dump_generic_node (buffer, gs_cond_rhs (gs), spc, flags, false);
  pp_character (buffer, ')');
  newline_and_indent (buffer, spc + 2);
  pp_character (buffer, '{');
  newline_and_indent (buffer, spc + 4);
  pp_string (buffer, "goto ");
  dump_generic_node (buffer, gs_cond_true_label (gs), spc, flags, false);
  pp_character (buffer, ';');
  newline_and_indent (buffer, spc + 2);
  pp_character (buffer, '}');
  newline_and_indent (buffer, spc);
  pp_string (buffer, "else");
  newline_and_indent (buffer, spc + 2);
  pp_character (buffer, '{');
  newline_and_indent (buffer, spc + 4);
  pp_string (buffer, "goto ");
  dump_generic_node (buffer, gs_cond_false_label (gs), spc, flags, false);
  pp_character (buffer, ';');
  newline_and_indent (buffer, spc + 2);
  pp_character (buffer, '}');
}


/* Dump a GS_LABEL tuple on the pretty_printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h)*/

static void
dump_gs_label (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  tree label = gs_label_label (gs);
  dump_generic_node (buffer, label, spc, flags, false);
  if (TREE_CODE (label) != CASE_LABEL_EXPR )
    pp_string (buffer, ":");
}


/* Dump a GS_BIND tuple on the pretty_printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h).  */

static void
dump_gs_bind (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GS_BIND tuple");

  newline_and_indent (buffer, spc);

  pp_character (buffer, '{');
  newline_and_indent (buffer, spc + 2);
  if (!(flags & TDF_SLIM))
    {
      tree var;

      for (var = gs_bind_vars (gs); var; var = TREE_CHAIN (var))
	{
	  print_declaration (buffer, var, spc, flags);
	  newline_and_indent (buffer, spc + 2);
	}
      newline_and_indent (buffer, spc + 2);
    }

  dump_gimple_seq (buffer, gs_bind_body (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  pp_character (buffer, '}');
}


/* Dump a GS_TRY tuple on the pretty_printer BUFFER, SPC spaces of
   indent.  FLAGS specifies details to show in the dump (see TDF_* in
   tree.h).  */

static void
dump_gs_try (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (flags & TDF_DETAILS)
    pp_string (buffer, "GS_TRY tuple");

  newline_and_indent (buffer, spc);
  pp_string (buffer, "try {");
  newline_and_indent (buffer, spc + 2);
  dump_gimple_seq (buffer, gs_try_eval (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  if (GS_SUBCODE_FLAGS (gs) == GS_TRY_CATCH)
    pp_string (buffer, "} catch {");
  else
    pp_string (buffer, "} finally {");
  newline_and_indent (buffer, spc + 2);
  dump_gimple_seq (buffer, gs_try_cleanup (gs), spc + 2, flags);
  newline_and_indent (buffer, spc);
  pp_character (buffer, '}');
}


/* Dump the gimple statement GS on the pretty_printer BUFFER, SPC
   spaces of indent.  FLAGS specifies details to show in the dump (see
   TDF_* in tree.h).  */

void
dump_gimple_stmt (pretty_printer *buffer, gimple gs, int spc, int flags)
{
  if (!gs)
    return;

  switch (GS_CODE (gs))
    {
    case GS_ASSIGN:
      dump_gs_assign (buffer, gs, spc, flags);
      break;

    case GS_BIND:
      dump_gs_bind (buffer, gs, spc, flags);
      break;

    case GS_CALL:
      dump_gs_call (buffer, gs, spc, flags);
      break;

    case GS_COND:
      dump_gs_cond (buffer, gs, spc, flags);
      break;

    case GS_LABEL:
      dump_gs_label (buffer, gs, spc, flags);
      break;

    case GS_GOTO:
      pp_string (buffer, "goto ");
      dump_generic_node (buffer, gs_goto_dest (gs), spc, flags, false);
      break;

    case GS_NOP:
      pp_string (buffer, "GS_NOP");
      break;

    case GS_RETURN:
      dump_gs_return (buffer, gs, spc, flags);
      break;

    case GS_SWITCH:
      dump_gs_switch (buffer, gs, spc, flags);
      break;

    case GS_TRY:
      dump_gs_try (buffer, gs, spc, flags);
      break;

    default:
      GS_NIY;
    }

  newline_and_indent (buffer, spc);
  pp_write_text_to_stream (buffer);
}
