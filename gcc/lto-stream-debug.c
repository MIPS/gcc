/* LTO stream debugging.

   Copyright 2006 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "toplev.h"
#include "flags.h"
#include "params.h"
#include "input.h"
#include "debug.h"
#include "lto-header.h"
#include "tree.h"
#include <ctype.h>

#ifdef LTO_STREAM_DEBUGGING

/* Print VALUE to CONTEXT.  */

static void
lto_debug_string_internal (struct lto_debug_context *context, 
		  const char *value, int len)
{
  int i;
  for (i=0; i<len; i++)
    context->out (context, value[i]);
}


/* Print VALUE in hex to CONTEXT.  */

static void 
lto_debug_wide_internal (struct lto_debug_context *context,
			 HOST_WIDE_INT value)
{
  char buffer[100];
  sprintf (buffer, HOST_WIDE_INT_PRINT_HEX, value);
  lto_debug_string_internal (context, buffer, strlen (buffer));
}


/* Write TAG to CONTEXT and increment indent.  */

void 
lto_debug_indent (struct lto_debug_context *context, int tag)
{
  int i;
  const char *tag_name = context->tag_names[tag];
  context->out (context, '\n');
  for (i=0; i<context->indent; i++)
    context->out (context, ' ');
  context->out (context, '(');

  lto_debug_string_internal (context, tag_name, strlen (tag_name));
  lto_debug_string_internal (context, ":", 1);
  context->indent++;
}


/* Write indented TAG to CONTEXT.  */

void 
lto_debug_indent_token (struct lto_debug_context *context, 
			const char * value)
{
  int i;
  context->out (context, '\n');
  for (i=0; i<context->indent; i++)
    context->out (context, ' ');
  lto_debug_string_internal (context, value, strlen (value));
}


/* Write TAG, HIGH, and LOW to CONTEXT.  */

void
lto_debug_integer (struct lto_debug_context *context, 
		   const char *tag, HOST_WIDE_INT high, 
		   HOST_WIDE_INT low)
{
  lto_debug_string_internal (context, tag, strlen (tag));
  lto_debug_wide_internal (context, high);
  lto_debug_wide_internal (context, low);
}


/* Write the string VALUE surronded by quotes to CONTEXT.  */

void
lto_debug_string (struct lto_debug_context *context, 
		  const char *value, int len)
{
  context->out (context, '"');
  lto_debug_string_internal (context, value, len);
  context->out (context, '"');
}


/* Write the string VALUE to CONTEXT.  */

void
lto_debug_token (struct lto_debug_context *context, 
		 const char *value)
{
  lto_debug_string_internal (context, value, strlen (value));
}


/* Write a function name for FN to CONTEXT.  */

void
lto_debug_fn_name (struct lto_debug_context *context, 
		  const tree fn)
{
  const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fn));
  char *dot = index (name, '.');
  int len;

  /* The dwarf reader sometimes adds .nnn to the end of function
     names.  This will mess up the verification protocol since the
     input will not exactly the output.  */
  if (dot)
    len = dot - name;
  else
    len = strlen (name);

  lto_debug_string_internal (context, " ", 1);
  lto_debug_string_internal (context, name, len);
  lto_debug_string_internal (context, " ", 1);
}


/* Write ")\n" to CONTEXT and decrement indent.  */

void
lto_debug_undent (struct lto_debug_context *context)
{
  context->out (context, ')');
  context->out (context, '\n');
  context->indent--;
}


/* Write TAG and VALUE to CONTEXT.  */

void
lto_debug_wide (struct lto_debug_context *context, 
		const char *tag, HOST_WIDE_INT value)
{
  lto_debug_string_internal (context, tag, strlen (tag));
  lto_debug_wide_internal (context, value);
}

#endif
