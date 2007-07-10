/* CIL Compiler almost main (cil1)
   Called by GCC's toplev.c

   Copyright (C) 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

   Author:
     Ricardo Fernandez Pascual <ricardof@um.es>

*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "flags.h"
#include "toplev.h"
#include "version.h"

#include "ggc.h"
#include "tree.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "opts.h"
#include "options.h"
#include "value-prof.h"

#include "cil.h"
#include "cil-tree.h"
#include "parser.h"

/* Language for usage for messages.  */
const char *const language_string = "CIL front end for GCC ";

/* Prepare to handle switches.  */
unsigned int
cil_init_options (unsigned int argc ATTRIBUTE_UNUSED,
                  const char **argv ATTRIBUTE_UNUSED)
{
  return CL_CIL;
}

/* Process a switch - called by opts.c.  */
int
cil_handle_option (size_t scode, const char *arg, int value)
{
  enum opt_code code = (enum opt_code) scode;
  switch (code)
    {
    case OPT_fcompile_only_reachable:
      flag_parse_only_reachable = true;
      break;
    case OPT_ferror_unsupported:
      flag_unsupported_method_behavior = UMB_ERROR;
      break;
    case OPT_fcbuiltin:
      flag_no_cbuiltin = !value;
      break;
    default:
      gcc_unreachable ();
    }
  return 1;
}

/* Language dependent parser setup.  */

bool
cil_init (void)
{
#ifndef USE_MAPPED_LOCATION
  input_filename = main_input_filename;
#else
  linemap_add (&line_table, LC_ENTER, false, main_input_filename, 1);
#endif

  /* This error will not happen from GCC as it will always create a
     fake input file.  */
  if (!input_filename || input_filename[0] == ' ' || !input_filename[0])
    {
      fprintf (stderr, "No input file specified, try --help for help\n");
      exit (1);
      return false;
    }

#ifdef USE_MAPPED_LOCATION
  linemap_add (&line_table, LC_RENAME, false, "<built-in>", 1);
  linemap_line_start (&line_table, 0, 1);
#endif

  /* Disable strict aliasing rules. CIL semantics require that
     pointers of any type can alias pointer of any other types (unlike
     C), or at least that's our current understanding. Hence, strict
     aliasing is useless (and produces worng code). */
  flag_strict_aliasing = 0;

  /* Init decls, etc.  */
  cil_init_decl_processing ();

  return true;
}

/* Language dependent wrapup.  */

void
cil_finish (void)
{
}

/* Parse a file.  Debug flag doesn't seem to work. */

void
cil_parse_file (int debug_flag ATTRIBUTE_UNUSED)
{
#ifdef USE_MAPPED_LOCATION
  source_location s;
  linemap_add (&line_table, LC_RENAME, false, main_input_filename, 1);
  s = linemap_line_start (&line_table, 1, 80);
  input_location = s;
#else
  input_line = 1;
#endif

  parser_parse_file (input_filename);

  cgraph_finalize_compilation_unit ();
#ifdef USE_MAPPED_LOCATION
  linemap_add (&line_table, LC_LEAVE, false, NULL, 0);
#endif
  cgraph_optimize ();
}

#include "debug.h" /* for debug_hooks */
#include "gtype-cil.h"
