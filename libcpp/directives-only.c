/* CPP Library - directive only preprocessing for distributed compilation.
   Copyright (C) 2007-2019 Free Software Foundation, Inc.
   Contributed by Ollie Wild <aaw@google.com>.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "cpplib.h"
#include "internal.h"

/* DO (Directive only) flags. */
#define DO_BOL		 (1 << 0) /* At the beginning of a logical line. */
#define DO_STRING	 (1 << 1) /* In a string constant. */
#define DO_CHAR		 (1 << 2) /* In a character constant. */
#define DO_BLOCK_COMMENT (1 << 3) /* In a block comment. */
#define DO_LINE_COMMENT	 (1 << 4) /* In a single line "//-style" comment. */

#define DO_LINE_SPECIAL (DO_STRING | DO_CHAR | DO_LINE_COMMENT)
#define DO_SPECIAL	(DO_LINE_SPECIAL | DO_BLOCK_COMMENT)

/* Writes out the preprocessed file, handling spacing and paste
   avoidance issues.  */
void
_cpp_preprocess_dir_only (cpp_reader *pfile,
			  const struct _cpp_dir_only_callbacks *cb)
{
  struct cpp_buffer *buffer;
  const unsigned char *cur, *base, *next_line, *rlimit;
  cppchar_t c, last_c;
  unsigned flags;
  linenum_type lines;
  int col;
  location_t loc;

 restart:
  /* Buffer initialization ala _cpp_clean_line(). */
  buffer = pfile->buffer;
  buffer->cur_note = buffer->notes_used = 0;
  buffer->cur = buffer->line_base = buffer->next_line;
  buffer->need_line = false;

  /* This isn't really needed.  It prevents a compiler warning, though. */
  loc = pfile->line_table->highest_line;

  /* Scan initialization. */
  next_line = cur = base = buffer->cur;
  rlimit = buffer->rlimit;
  flags = DO_BOL;
  lines = 0;
  col = 1;

  for (last_c = '\n', c = *cur; cur < rlimit; last_c = c, c = *++cur, ++col)
    {
      /* Skip over escaped newlines. */
      if (__builtin_expect (c == '\\', false))
	{
	  const unsigned char *tmp = cur + 1;

	  while (is_nvspace (*tmp) && tmp < rlimit)
	    tmp++;
	  if (*tmp == '\r')
	    tmp++;
	  if (*tmp == '\n' && tmp < rlimit)
	    {
	      CPP_INCREMENT_LINE (pfile, 0);
	      lines++;
	      col = 0;
	      cur = tmp;
	      c = last_c;
	      continue;
	    }
	}

      if (__builtin_expect (last_c == '#', false) && !(flags & DO_SPECIAL))
	{
	  if (c != '#' && (flags & DO_BOL) && !buffer->ignore_directives)
	  {
	    class line_maps *line_table;

	    if (!pfile->state.skipping && next_line != base)
	      cb->print_lines (lines, base, next_line - base);

	    /* Prep things for directive handling. */
	    buffer->next_line = cur;
	    buffer->need_line = true;
	    _cpp_get_fresh_line (pfile);

	    /* Ensure proper column numbering for generated error messages. */
	    buffer->line_base -= col - 1;

            cpp_token start;
            start.type = CPP_HASH;
            start.flags = 0; /* ignore indented */
            start.src_loc = pfile->line_table->highest_line; // ??? Location?

	    _cpp_handle_directive (pfile, &start);

	    /* Sanitize the line settings.  Duplicate #include's can mess
	       things up. */
	    line_table = pfile->line_table;
	    line_table->highest_location = line_table->highest_line;

	    /* The if block prevents us from outputing line information when
	       the file ends with a directive and no newline.  Note that we
	       must use pfile->buffer, not buffer. */
	    if (pfile->buffer->next_line < pfile->buffer->rlimit)
	      cb->maybe_print_line (pfile->line_table->highest_line);

	    goto restart;
	  }

	  flags &= ~DO_BOL;
	  pfile->mi_valid = false;
	}
      else if (__builtin_expect (last_c == '/', false) \
	       && !(flags & DO_SPECIAL) && c != '*' && c != '/')
	{
	  /* If a previous slash is not starting a block comment, clear the
	     DO_BOL flag.  */
	  flags &= ~DO_BOL;
	  pfile->mi_valid = false;
	}
      //
      // @@ We probably should keep it separate because of module_p.
      // @@ We want it *after* previous if-else clears DO_BOL (/import).
      //
      /* Note: we always have a newline at the end, even if the file does not
	 end with one.  As a result, if the current character is not a
	 newline, we can safely look at the next one without checking rlimit.
	 Check the first three characters to quickly distinguish from other
	 common names (like extern).  */

      else if ((__builtin_expect (c      == 'i' &&
				  cur[1] == 'm' &&
				  cur[2] == 'p', false) ||
		__builtin_expect (c      == 'e' &&
				  cur[1] == 'x' &&
				  cur[2] == 'p', false)) &&
	       (flags & DO_BOL) && !(flags & DO_SPECIAL) &&
	       !buffer->ignore_directives)
	{
	  const cpp_token *token;
	  const unsigned char *p = cur + 3;

	  /* Handle the 'export' prefix.  */
	  if (c == 'e')
	    {
	      if (*p++ != 'o' || *p++ != 'r' || *p++ != 't' ||
		  !is_nvspace (*p++))
		goto skip;

	      while (is_nvspace (*p))
		p++;

	      if (*p++ != 'i' || *p++ != 'm' || *p++ != 'p')
		goto skip;
	    }

	  if (*p++ != 'o' || *p++ != 'r' || *p++ != 't' ||
	      (!is_nvspace (*p) && *p != '<' && *p != '"'))
	    goto skip;

	  /* Similar code to the real directive above, except here we don't
	     need to worry about the line information. (@@ Is that really the
	     case?)  */

	  next_line = cur;

	  struct line_maps *line_table;

	  if (!pfile->state.skipping && next_line != base)
	    cb->print_lines (lines, base, next_line - base);

	  /* Prep things for directive handling. */
	  buffer->next_line = cur;
	  buffer->need_line = true;
	  _cpp_get_fresh_line (pfile);

	  /* Ensure proper column numbering for generated error messages. */
	  buffer->line_base -= col - 1;

	  // @@ Create a fake or just lex from the beginning? If fake, then
	  //    will need to adjust column number. If fusing then fake is
	  //    probably easier. Also will allow to factor to sep. function.
	  //
	  // @@ TODO: redo by skiping import/export import.

	  token = _cpp_lex_direct (pfile);
	  gcc_assert (token->type == CPP_NAME);

	  cpp_token start_token;
	  if (c == 'e')
	    {
	      /* Fuse into a single 'export import' token. */
	      start_token = *token;
	      start_token.val.node.node =
		start_token.val.node.spelling =
		_cpp_lex_identifier (pfile, "export import");
	      token = &start_token;
	    }

	  _cpp_handle_directive (pfile, token);

	  /* Sanitize the line settings.  Duplicate #include's can mess
	     things up. */
	  line_table = pfile->line_table;
	  line_table->highest_location = line_table->highest_line;

	  /* The if block prevents us from outputing line information when
	     the file ends with a directive and no newline.  Note that we
	     must use pfile->buffer, not buffer. */
	  if (pfile->buffer->next_line < pfile->buffer->rlimit)
	    cb->maybe_print_line (pfile->line_table->highest_line);

	  /* If we get something other than padding, then we assume it's a
	     module import and write the original line.  */
	  if (pfile->directive_result.type == CPP_PADDING)
	    goto restart;

	  /* We need to drop any tokens that _cpp_handle_directive() may have
	     backed up (failed that, we will see them when handling the next
	     directive).  Also clean up any macro expansion contexts.  */
	  while (pfile->context->prev)
	    _cpp_pop_context (pfile);

	  pfile->cur_token += pfile->lookaheads;
	  pfile->lookaheads = 0;

	  /* We've already written lines up to cur.  */
	  base = cur;

	skip:
	  ;
	}

      switch (c)
	{
	case '/':
	  if ((flags & DO_BLOCK_COMMENT) && last_c == '*')
	    {
	      flags &= ~DO_BLOCK_COMMENT;
	      c = 0;
	    }
	  else if (!(flags & DO_SPECIAL) && last_c == '/')
	    flags |= DO_LINE_COMMENT;
	  else if (!(flags & DO_SPECIAL))
	    /* Mark the position for possible error reporting. */
	    loc = linemap_position_for_column (pfile->line_table, col);
	  /* Don't update DO_BOL yet. */
	  break;

	case '*':
	  if (!(flags & DO_SPECIAL))
	    {
	      if (last_c == '/')
		flags |= DO_BLOCK_COMMENT;
	      else
		{
		  flags &= ~DO_BOL;
		  pfile->mi_valid = false;
		}
	    }

	  break;

	case '\'':
	case '"':
	  {
	    unsigned state = (c == '"') ? DO_STRING : DO_CHAR;

	    if (!(flags & DO_SPECIAL))
	      {
		flags |= state;
		flags &= ~DO_BOL;
		pfile->mi_valid = false;
	      }
	    else if ((flags & state) && last_c != '\\')
	      flags &= ~state;

	    break;
	  }

	case '\\':
	  {
	    if ((flags & (DO_STRING | DO_CHAR)) && last_c == '\\')
	      c = 0;

	    if (!(flags & DO_SPECIAL))
	      {
		flags &= ~DO_BOL;
		pfile->mi_valid = false;
	      }

	    break;
	  }

	case '\n':
	  CPP_INCREMENT_LINE (pfile, 0);
	  lines++;
	  col = 0;
	  flags &= ~DO_LINE_SPECIAL;
	  if (!(flags & DO_SPECIAL))
	    flags |= DO_BOL;
	  break;

	case '#':
	  next_line = cur;
	  /* Don't update DO_BOL yet. */
	  break;

	case ' ': case '\t': case '\f': case '\v': case '\0':
	  break;

	default:
	  if (!(flags & DO_SPECIAL))
	    {
	      flags &= ~DO_BOL;
	      pfile->mi_valid = false;
	    }
	  break;
	}
    }

  if (flags & DO_BLOCK_COMMENT)
    cpp_error_with_line (pfile, CPP_DL_ERROR, loc, 0, "unterminated comment");

  if (!pfile->state.skipping && cur != base)
    {
      /* If the file was not newline terminated, add rlimit, which is
         guaranteed to point to a newline, to the end of our range.  */
      if (cur[-1] != '\n')
	{
	  cur++;
	  CPP_INCREMENT_LINE (pfile, 0);
	  lines++;
	}

      cb->print_lines (lines, base, cur - base);
    }

  _cpp_pop_buffer (pfile);
  if (pfile->buffer)
    goto restart;
}
