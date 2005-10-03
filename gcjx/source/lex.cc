// Implementation of lexing for Java.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "typedefs.hh"

#include <string.h>

#include "source/lex.hh"
#include "source/chartables.h"
#include "source/keyword.h"
#include "exception.hh"

// Number of sub-parts to use when computing integer value.
#define NUM_BITS 5

lexer::lexer (ucs2_reader *source, const char *file)
  : input_filter (source),
    backslash_count (0),
    unget_value (UNICODE_W_NONE),
    was_return (false),
    cooked_unget_value (UNICODE_W_NONE),
    scratch (NULL),
    scratch_size (0),
    filename (file),
    line (1),
    column (0),
    tab_width (global->get_compiler ()->get_tab_width ())
{
}

lexer::~lexer ()
{
  delete input_filter;
  if (scratch)
    free (scratch);
}



bool
lexer::identifier_part_p (unicode_w_t c)
{
  unsigned int hi = (c / 256) & 0xff;
  const char *const page = type_table[hi];
  unsigned long val = (unsigned long) page;
  int flags;

  if ((val & ~ LETTER_MASK) != 0)
    flags = page[c & 255];
  else
    flags = val;

  return (flags & LETTER_PART) != 0;
}

bool
lexer::white_space_p (unicode_w_t c)
{
  unsigned int hi = (c / 256) & 0xff;
  const char *const page = type_table[hi];
  unsigned long val = (unsigned long) page;
  int flags;

  if ((val & ~ LETTER_MASK) != 0)
    flags = page[c & 255];
  else
    flags = val;

  return (flags & LETTER_SPACE) != 0;
}

bool
lexer::identifier_start_p (unicode_w_t c)
{
  unsigned int hi = (c / 256) & 0xff;
  const char *const page = type_table[hi];
  unsigned long val = (unsigned long) page;
  int flags;

  if ((val & ~ LETTER_MASK) != 0)
    flags = page[c & 255];
  else
    flags = val;

  return (flags & LETTER_START) != 0;
}

inline bool
lexer::ascii_digit_p (unicode_w_t c)
{
  return c >= UNICODE_DIGIT_ZERO && c <= UNICODE_DIGIT_NINE;
}

int
lexer::hex_value (unicode_w_t c)
{
  if (ascii_digit_p (c))
    return c - UNICODE_DIGIT_ZERO;
  if (c >= UNICODE_LATIN_SMALL_LETTER_A
      && c <= UNICODE_LATIN_SMALL_LETTER_F)
    return c - UNICODE_LATIN_SMALL_LETTER_A + 10;
  if (c >= UNICODE_LATIN_CAPITAL_LETTER_A
      && c <= UNICODE_LATIN_CAPITAL_LETTER_F)
    return c - UNICODE_LATIN_CAPITAL_LETTER_A + 10;
  return -1;
}

inline bool
lexer::fp_char_p (unicode_w_t c)
{
  return ((c >= UNICODE_LATIN_SMALL_LETTER_D
	   && c <= UNICODE_LATIN_SMALL_LETTER_F)
	  || (c >= UNICODE_LATIN_CAPITAL_LETTER_D
	      && c <= UNICODE_LATIN_CAPITAL_LETTER_F)
	  || c == UNICODE_PERIOD
	  || ascii_digit_p (c));
}



void
lexer::ensure (int size)
{
  if (scratch == NULL)
    {
      scratch_size = 50;
      if (scratch_size < size)
	scratch_size = size;
      scratch = (char *) malloc (scratch_size);
    }
  else if (scratch_size < size)
    {
      scratch_size *= 2;
      if (scratch_size < size)
	scratch_size = size;
      scratch = (char *) realloc (scratch, scratch_size);
    }
}

int
lexer::scratch_insert_utf8 (int offset, unicode_w_t c)
{
  // Make sure there's enough room.
  ensure (offset + 3);
  offset += ucs2_to_utf8 (&scratch[offset], c);
  return offset;
}

char *
lexer::print_unicode (unicode_w_t c)
{
  ensure (7);
  if (c >= UNICODE_SPACE && c <= 0x007f)
    // FIXME: ASCII host assumption and a hard-coded constant.
    sprintf (scratch, "%c", (char) c);
  else
    sprintf (scratch, "\\u%04x", (int) c);
  return scratch;
}

bool
lexer::extract_identifier (unicode_w_t c)
{
  bool all_ascii = true;
  bool dollar = false;

  int offset = 0;
  for (; identifier_part_p (c); c = get ())
    {
      int save = offset;
      offset = scratch_insert_utf8 (offset, c);
      if (offset - save > 1)
	all_ascii = false;
      if (! dollar && c == UNICODE_DOLLAR_SIGN)
	{
	  dollar = true;
	  if (global->get_compiler ()->warn_dollar_identifier ())
	    std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_dollar_identifier ()),
						  here (),
						  "use of %<$%> in identifier"));
	}
    }
  unget (c);
  ensure (offset + 1);
  scratch[offset] = '\0';

  return all_ascii;
}



unicode_w_t
lexer::get_raw ()
{
  unicode_w_t c;
  if (unget_value != UNICODE_W_NONE)
    {
      c = unget_value;
      unget_value = UNICODE_W_NONE;
    }
  else
    {
      try
	{
	  c = input_filter->get ();
	}
      catch (conversion_error &exc)
	{
	  exc.set_location (here ());
	  throw exc;
	}

      if (c == UNICODE_TAB)
	{
	  // Advance to next multiple of tab width.  Note we don't
	  // subtract one from tab_width since we start columns at
	  // zero.
	  column = ((column + tab_width) / tab_width) * tab_width;
	}
      else
	++column;
    }
  return c;
}

unicode_w_t
lexer::read_handling_escapes ()
{
  unicode_w_t c = get_raw ();
  if (c != UNICODE_BACKSLASH)
    {
      backslash_count = 0;
      return c;
    }

  ++backslash_count;
  if ((backslash_count % 2) == 1)
    {
      c = get_raw ();
      if (c == UNICODE_LATIN_SMALL_LETTER_U)
	{
	  unicode_w_t result = 0;

	  // Skip any number of 'u' for \u processing.
	  while ((c = get_raw ()) == UNICODE_LATIN_SMALL_LETTER_U)
	    ;

	  for (int shift = 12; shift >= 0; shift -= 4)
	    {
	      if (c == UNICODE_EOF)
		throw lex_error (here (),
				 "prematurely terminated \\u sequence");
	      int n = hex_value (c);
	      if (n < 0)
		throw lex_error (here (), "non-hex digit in \\u sequence");
	      result |= (unicode_w_t) (n << shift);
	      if (shift > 0)
		c = get_raw ();
	    }

	  if (result <= 127 && global->get_compiler ()->warn_ascii_escape ())
	    std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_ascii_escape ()),
						  here (),
						  "ASCII character "
						  "represented by "
						  "\\u escape"));

	  backslash_count = 0;
	  return result;
	}

      assert (unget_value == UNICODE_W_NONE);
      unget_value = c;
    }

  return UNICODE_BACKSLASH;
}

// Get a single character, having already processed \u sequences.
// Collapse line terminators.
unicode_w_t
lexer::get ()
{
  unicode_w_t c;
  if (cooked_unget_value != UNICODE_W_NONE)
    {
      c = cooked_unget_value;
      cooked_unget_value = UNICODE_W_NONE;
    }
  else
    c = read_handling_escapes ();

  if (was_return && c == UNICODE_LINE_FEED)
    {
      // Saw \r\n, so read again.
      c = read_handling_escapes ();
    }

  was_return = (c == UNICODE_CARRIAGE_RETURN);
  if (was_return)
    {
      // Return the kind of newline that the rest of the lexer
      // understands.
      c = UNICODE_LINE_FEED;
    }

  if (c == UNICODE_LINE_FEED)
    {
      // Note that it is somewhat bogus for this to be here, since it
      // means an escape like \u00a0 will increment the line number.
      // (Jacks tests for this, but I think the Jacks test is wrong
      // because most editors do not work this way, and the line
      // numbers are mostly useful for interacting with editors.)  We
      // do it this way because it is more work to do anything else.
      // One approach might be to have read_handling_escapes tell us
      // whether a character came from an escape sequence.  It isn't
      // extremely important, I suspect, since newlines-as-escapes are
      // probably quite rare.
      column = 0;
      ++line;
    }

  return c;
}

unicode_w_t
lexer::get_no_eof ()
{
  unicode_w_t c = get ();
  if (c == UNICODE_EOF)
    throw lex_error (here (), "unexpected end of file");
  return c;
}

void
lexer::unget (unicode_w_t c)
{
  if (c != UNICODE_EOF)
    {
      assert (cooked_unget_value == UNICODE_W_NONE);
      cooked_unget_value = c;
    }
}

unicode_w_t
lexer::peek ()
{
  unicode_w_t c = get ();
  // Don't push back an EOF marker.
  if (c != UNICODE_EOF)
    unget (c);
  return c;
}

unicode_w_t
lexer::escape_sequence ()
{
  unicode_w_t c = get ();
  switch (c)
    {
    case UNICODE_LATIN_SMALL_LETTER_B:
      return UNICODE_BACKSPACE;
    case UNICODE_LATIN_SMALL_LETTER_T:
      return UNICODE_TAB;
    case UNICODE_LATIN_SMALL_LETTER_N:
      return UNICODE_LINE_FEED;
    case UNICODE_LATIN_SMALL_LETTER_F:
      return UNICODE_FORM_FEED;
    case UNICODE_LATIN_SMALL_LETTER_R:
      return UNICODE_CARRIAGE_RETURN;
    case UNICODE_QUOTATION_MARK:
    case UNICODE_APOSTROPHE:
    case UNICODE_BACKSLASH:
      return c;

    case UNICODE_DIGIT_ZERO:
    case UNICODE_DIGIT_ONE:
    case UNICODE_DIGIT_TWO:
    case UNICODE_DIGIT_THREE:
    case UNICODE_DIGIT_FOUR:
    case UNICODE_DIGIT_FIVE:
    case UNICODE_DIGIT_SIX:
    case UNICODE_DIGIT_SEVEN:
      {
	unicode_w_t value = 0;
	int octal_escape_index = 0;
	int max = 3;

	// Avoid the last digit in something like \477.
	if (c > UNICODE_DIGIT_THREE)
	  --max;

	for (; octal_escape_index < max
	       && c >= UNICODE_DIGIT_ZERO
	       && c <= UNICODE_DIGIT_SEVEN;
	     c = get ())
	  {
	    value *= 8;
	    value += c - UNICODE_DIGIT_ZERO;
	    ++octal_escape_index;
	  }

	// Put back the final character.
	unget (c);

	return value;
      }

    default:
      throw lex_error (here (), "Invalid character %1 in escape sequence")
	% print_unicode (c);
    }
}

location
lexer::here ()
{
  return location (filename, column, line);
}



void
lexer::single_line_comment ()
{
  unicode_w_t c;
  do
    {
      c = get ();
      if (c == UNICODE_EOF)
	{
	  // We used to throw an error here if pedantic.  But probably
	  // the JLS will change to just allow this, and Jacks checks
	  // for it, and anyway what is it good for?
	  break;
	}
    }
  while (c != UNICODE_LINE_FEED);
}

void
lexer::end_comment ()
{
  bool slash = false;
  for (unicode_w_t c = get_no_eof (); ; c = get_no_eof ())
    {
      if (c == UNICODE_ASTERISK)
	slash = true;
      else if (slash)
	{
	  if (c == UNICODE_SLASH)
	    break;
	  if (c != UNICODE_ASTERISK)
	    slash = false;
	}
    }
}

// Parse a javadoc comment, looking for the "@deprecated" tag.
token
lexer::parse_javadoc (unicode_w_t c)
{
  ref_javadoc javadoc (new model_javadoc (here ()));
  while (true)
    {
      // Skip to the start of the text of this line.  We use a
      // restricted notion of whitespace here -- which is weird, but
      // we follow jacks.
      while (c == UNICODE_LINE_FEED || c == UNICODE_SPACE
	     || c == UNICODE_TAB || c == UNICODE_FORM_FEED
	     || c == UNICODE_SPACE || c == UNICODE_ASTERISK)
	{
	  unicode_w_t save = c;
	  c = get_no_eof ();
	  if (save == UNICODE_ASTERISK && c == UNICODE_SLASH)
	    {
	      // Comment terminator.
	      return token (TOKEN_JAVADOC, javadoc);
	    }
	}

      if (c == UNICODE_COMMERCIAL_AT)
	{
	  // FIXME: we assume the host execution character set is
	  // ASCII-compatible here.
	  const char *value = "deprecated";
	  const char *iter = value;
	  for (c = get_no_eof ();
	       *iter && c == unicode_w_t (*iter);
	       c = get_no_eof (), ++iter)
	    ;

	  // If seen, must be followed by space, newline, or '*'.
	  if ((c == UNICODE_SPACE || c == UNICODE_LINE_FEED
	       || c == UNICODE_ASTERISK || white_space_p (c))
	      && ! *iter)
	    {
	      javadoc->set_deprecated ();
	      // Might as well look for the comment end now.
	      unget (c);
	      end_comment ();
	      return token (TOKEN_JAVADOC, javadoc);
	    }
	}

      // Skip to the next line.
      while (c != UNICODE_LINE_FEED)
	{
	  unicode_w_t save = c;
	  c = get_no_eof ();
	  if (save == UNICODE_ASTERISK && c == UNICODE_SLASH)
	    return token (TOKEN_JAVADOC, javadoc);
	}

      // Eat the line feed.
      c = get_no_eof ();
    }

  abort ();
}

token
lexer::multi_line_comment ()
{
  // We know this character is a '*'.
  get ();
  // Look at the character after the comment opener.
  unicode_w_t c = get ();
  if (c == UNICODE_ASTERISK)
    {
      c = get ();
      if (c == UNICODE_SLASH)
	{
	  // An empty comment is considered to be a degenerate form of
	  // a javadoc comment.
	  ref_javadoc javadoc (new model_javadoc (here ()));
	  return token (TOKEN_JAVADOC , javadoc);
	}
      else
	return parse_javadoc (c);
    }
  end_comment ();
  return TOKEN_REPEAT;
}

unicode_w_t
lexer::skip_whitespace (location &where)
{
  int prev_line = line, prev_column = column;
  unicode_w_t c = get ();

  // This uses a particularly strict definition of whitespace.
  while (c == UNICODE_LINE_FEED || c == UNICODE_SPACE
	 || c == UNICODE_TAB || c == UNICODE_FORM_FEED)
    {
      prev_line = line;
      prev_column = column;
      c = get ();
    }

  // Return the location of the character we are returning.
  where = location (filename, prev_column, prev_line);
  return c;
}

token
lexer::character_literal ()
{
  location where = here ();
  unicode_w_t c = get ();
  unicode_w_t char_lit;
  if (c == UNICODE_BACKSLASH)
    char_lit = escape_sequence ();
  else if (c == UNICODE_LINE_FEED)
    throw lex_error (where, "line terminator invalid in character literal");
  else if (c == UNICODE_APOSTROPHE)
    // Not the best message, perhaps.
    throw lex_error (where, "single quote invalid in character literal");
  else
    char_lit = c;

  c = get ();
  if (c == UNICODE_LINE_FEED || c == UNICODE_EOF)
    throw lex_error (where, "character literal unterminated at end of line");
  else if (c != UNICODE_APOSTROPHE)
    throw lex_error (where, "unterminated character literal");

  // FIXME: best location?
  ref_char_literal val (new model_char_literal (where, char_lit));
  return token (TOKEN_CHAR_LIT, val);
}

token
lexer::string_literal ()
{
  location where = here ();
  unicode_w_t c;
  int offset = 0;
  for (c = get ();
       (c != UNICODE_EOF && c != UNICODE_QUOTATION_MARK
	&& c != UNICODE_LINE_FEED);
       c = get ())
    {
      if (c == UNICODE_BACKSLASH)
	c = escape_sequence ();

      // FIXME: error handling.

      offset = scratch_insert_utf8 (offset, c);
    }
  ensure (offset + 1);
  scratch[offset] = '\0';

  if (c == UNICODE_LINE_FEED || c == UNICODE_EOF)
    {
      // FIXME: probably better to just terminate the string and
      // recover here.
      throw lex_error (here (), "string literal unterminated at end of line");
    }

  ref_string_literal val (new model_string_literal (where,
						    std::string (scratch)));
  return token (TOKEN_STRING_LIT, val);
}

bool
lexer::multiply_add (unsigned int bits[], int radix, int val)
{
  unsigned int carry = val;
  for (int i = 0; i < NUM_BITS; ++i)
    {
      bits[i] = bits[i] * radix + carry;
      carry = (bits[i] & ~ 0xffff) >> 16;
      bits[i] &= 0xffff;
    }
  return ! carry;
}

token
lexer::create_int_literal (const location &where, int offset, int radix,
			   bool is_long)
{
  // We build up a value by computing it in sections, then we merge
  // them together at the end.  For each word, we only use the
  // low-order 16 bits, to make it simpler to handle overflow.
  unsigned int bits[NUM_BITS];
  for (int i = 0; i < NUM_BITS; ++i)
    bits[i] = 0;

  // Compute the value in parts.
  for (int i = 0; i < offset; ++i)
    {
      int val = hex_value (scratch[i]);
      assert (val >= 0 && val < radix);
      if (! multiply_add (bits, radix, val))
	throw lex_error (where, "integral constant overflow");
    }

  // Check for overflow according to the desired result type.
  int last_index = is_long ? 4 : 2;
  for (int i = last_index; i < NUM_BITS; ++i)
    {
      if (bits[i] != 0)
	throw lex_error (where, "integral constant overflow");
    }

  // Compute the real result.
  jlong full_value = 0;
  bool is_most_negative = true;
  for (int i = 0; i < last_index; ++i)
    {
      full_value |= jlong (bits[i] & 0xffff) << (16 * i);
      if ((i == last_index - 1 && bits[i] != 0x8000)
	  || (i < last_index - 1 && bits[i] != 0))
	is_most_negative = false;
    }

  // For decimal representation only, we allow the most negative value
  // of a given type, but no other negative values.
  if (radix == 10 && ! is_most_negative
      && (bits[last_index - 1] & 0x8000) != 0)
    throw lex_error (where, "integral decimal constant overflow");

  // We have a special "decimal" token, because the parser checks
  // whether the most negative value is in fact the argument of a
  // unary "-", and it only does this for decimal literals.
  if (is_long)
    return token (radix == 10 ? TOKEN_DECIMAL_LONG_LIT : TOKEN_LONG_LIT,
		  where,
		  new model_long_literal (where, jlong (full_value)));
  else
    return token (radix == 10 ? TOKEN_DECIMAL_INT_LIT : TOKEN_INT_LIT,
		  where,
		  new model_int_literal (where, jint (full_value)));
}

token
lexer::parse_float_literal (const location &where, int offset,
			    bool leading_zero, unicode_w_t c)
{
  // If we had a leading '0' and nothing else, go ahead and insert it.
  if (offset == 0 && leading_zero)
    offset = scratch_insert_utf8 (offset, UNICODE_DIGIT_ZERO);

  if (c == UNICODE_PERIOD)
    {
      offset = scratch_insert_utf8 (offset, c);
      // We know we'll always see a digit here if need one; this is
      // ensured by a caller.
      for (c = get (); ascii_digit_p (c); c = get())
	offset = scratch_insert_utf8 (offset, c);
    }

  if (c == UNICODE_LATIN_SMALL_LETTER_E || c == UNICODE_LATIN_CAPITAL_LETTER_E)
    {
      offset = scratch_insert_utf8 (offset, c);
      c = get ();
      if (c == UNICODE_PLUS_SIGN || c == UNICODE_HYPHEN_MINUS)
	{
	  offset = scratch_insert_utf8 (offset, c);
	  c = get ();
	}
      // Now require one or more digits.
      if (! ascii_digit_p (c))
	throw lex_error (where, "digit required in float-point "
			 "literal exponent");
      while (ascii_digit_p (c))
	{
	  offset = scratch_insert_utf8 (offset, c);
	  c = get ();
	}
    }

  bool is_float = false;
  if (c == UNICODE_LATIN_SMALL_LETTER_F || c == UNICODE_LATIN_CAPITAL_LETTER_F)
    is_float = true;
  else if (c == UNICODE_LATIN_SMALL_LETTER_D
	   || c == UNICODE_LATIN_CAPITAL_LETTER_D)
    {
      // Nothing.
    }
  else
    unget (c);

  scratch[offset] = '\0';
  if (is_float)
    {
      jfloat result;
      if (! string_to_float (scratch, result))
	throw lex_error (where, "invalid 'float' value");
      return token (TOKEN_FLOAT_LIT, where,
		    new model_float_literal (where, result));
    }
  else
    {
      jdouble result;
      if (! string_to_double (scratch, result))
	throw lex_error (where, "invalid 'double' value");
      return token (TOKEN_DOUBLE_LIT, where,
		    new model_double_literal (where, result));
    }
}

token
lexer::numeric_literal (unicode_w_t c)
{
  bool can_be_int = true;
  bool leading_zero = false;
  int radix = 10;

  location where = here ();

  // First see if we might have an octal or hex literal.
  if (c == UNICODE_DIGIT_ZERO)
    {
      leading_zero = true;
      c = get ();
      if (c == UNICODE_EOF)
	{
	  // A plain old "0" is fine.
	  return token (TOKEN_INT_LIT, where,
			new model_int_literal (where, 0));
	}
      if (c == UNICODE_LATIN_SMALL_LETTER_X
	  || c == UNICODE_LATIN_CAPITAL_LETTER_X)
	{
	  radix = 16;
	  c = get ();
	}
      else
	radix = 8;
    }

  // Read all leading digits.
  int offset = 0;
  while (true)
    {
      int val = hex_value (c);
      if (val < 0)
	break;
      // Special case for floating point numbers that start with "0".
      // No special handling for hex is needed, something like "90e0"
      // will parse correctly as float because "e" is not a base 10
      // digit.
      if (radix == 8 && (val >= radix && val < 10))
	{
	  can_be_int = false;
	  radix = 10;
	}
      if (val >= radix)
	break;
      offset = scratch_insert_utf8 (offset, c);
      c = get ();
    }

  // 0x requires some digits.
  if (radix == 16 && offset == 0)
    throw lex_error (where, "'0x' must be followed by digits");

  // See if we've got a floating point literal of some kind.
  if (c == UNICODE_PERIOD
      || c == UNICODE_LATIN_SMALL_LETTER_E
      || c == UNICODE_LATIN_CAPITAL_LETTER_E
      || c == UNICODE_LATIN_SMALL_LETTER_F
      || c == UNICODE_LATIN_CAPITAL_LETTER_F
      || c == UNICODE_LATIN_SMALL_LETTER_D
      || c == UNICODE_LATIN_CAPITAL_LETTER_D)
    return parse_float_literal (where, offset, leading_zero, c);

  // This can happen if we saw something like "09", which is invalid.
  // We have to check here since "09f" or "09." are valid.
  if (! can_be_int)
    throw lex_error (where, "invalid integral literal");

  // Trailing 'L' means we have a 'long' literal.
  bool is_long = false;
  if (c == UNICODE_LATIN_SMALL_LETTER_L || c == UNICODE_LATIN_CAPITAL_LETTER_L)
    is_long = true;
  else
    unget (c);

  // Integer literal.
  return create_int_literal (where, offset, radix, is_long);
}

bool
lexer::handle_keyword (char *buffer, token &result)
{
  const keyword *kw = find_keyword (buffer, strlen (buffer));
  if (kw == NULL)
    return false;
  assert (kw->value != TOKEN_INVALID);

  switch (kw->value)
    {
    case TOKEN_ASSERT:
      // For older sources, we don't recognize assert as a keyword.
      // If we return, this will later be turned into an identifier.
      if (! global->get_compiler ()->feature_assert ())
	{
	  if (global->get_compiler ()->warn_assert ())
	    std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_assert ()),
						  here (),
						  "%<assert%> keyword used "
						  "as identifier"));
	  return false;
	}
      result = token (kw->value);
      break;

    case TOKEN_ENUM:
      if (! global->get_compiler ()->feature_enum ())
	{
	  if (global->get_compiler ()->warn_enum ())
	    std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_enum ()),
						  here (),
						  "%<enum%> keyword used "
						  "as identifier"));
	  return false;
	}
      result = token (kw->value);
      break;

    default:
      // FIXME: could create a new model element in many of these
      // cases... anywhere the meaning is unambiguous.
      result = token (kw->value);
      return true;
    }

  assert (result != TOKEN_INVALID);
  return true;
}

bool
lexer::separator_p (unicode_w_t c, token &result)
{
  struct sep_map
  {
    unicode_w_t ch;
    token_value tv;
  };

  static const sep_map separators[] =
  {
    { UNICODE_LEFT_PARENTHESIS, TOKEN_OPEN_PAREN },
    { UNICODE_RIGHT_PARENTHESIS, TOKEN_CLOSE_PAREN },
    { UNICODE_LEFT_CURLY_BRACKET, TOKEN_OPEN_BRACE },
    { UNICODE_RIGHT_CURLY_BRACKET, TOKEN_CLOSE_BRACE },
    { UNICODE_LEFT_SQUARE_BRACKET, TOKEN_OPEN_BRACKET },
    { UNICODE_RIGHT_SQUARE_BRACKET, TOKEN_CLOSE_BRACKET },
    { UNICODE_SEMICOLON, TOKEN_SEMI },
    { UNICODE_COMMA, TOKEN_COMMA },
    { UNICODE_QUESTION_MARK, TOKEN_QUERY },
    { UNICODE_COLON, TOKEN_COLON },
    { UNICODE_TILDE, TOKEN_BITWISE_NOT },
    { UNICODE_EOF, TOKEN_INVALID }
  };

  // We could do a binary search if we sorted this list, but it seems
  // unimportant.
  for (int i = 0; separators[i].ch != UNICODE_EOF; ++i)
    {
      if (separators[i].ch == c)
	{
	  result = separators[i].tv;
	  return true;
	}
    }
  return false;
}

bool
lexer::operator_p (unicode_w_t c, token &result)
{
  switch (c)
    {
    case UNICODE_EQUALS_SIGN:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	result = TOKEN_EQUALS;
      else
	{
	  unget (c);
	  // FIXME: also used in variable declarators.  Does that matter?
	  // Maybe we want an assignment there.
	  ref_assignment a (new model_assignment (here ()));
	  result = token (TOKEN_ASSIGN, a);
	}
      break;

    case UNICODE_GREATER_THAN_SIGN:
      c = get ();
      switch (c)
	{
	case UNICODE_EQUALS_SIGN:
	  // ">="
	  result = TOKEN_GREATER_EQUALS;
	  break;
	case UNICODE_GREATER_THAN_SIGN:
	  c = get ();
	  switch (c)
	    {
	    case UNICODE_GREATER_THAN_SIGN:
	      // ">>>" or ">>>="
	      c = get ();
	      if (c == UNICODE_EQUALS_SIGN)
		{
		  ref_assignment a (new model_urs_equal (here ()));
		  result = token (TOKEN_UNSIGNED_RIGHT_SHIFT_ASSIGN, a);
		}
	      else
		{
		  unget (c);
		  result = TOKEN_UNSIGNED_RIGHT_SHIFT;
		}
	      break;
	    case UNICODE_EQUALS_SIGN:
	      // ">>="
	      {
		ref_assignment a (new model_rs_equal (here ()));
		result = token (TOKEN_RIGHT_SHIFT_ASSIGN, a);
	      }
	      break;
	    default:
	      // Just ">>".
	      unget (c);
	      result = TOKEN_RIGHT_SHIFT;
	      break;
	    }
	  break;
	default:
	  // Just ">".
	  unget (c);
	  result = TOKEN_GREATER_THAN;
	  break;
	}
      break;

    case UNICODE_LESS_THAN_SIGN:
      c = get ();
      switch (c)
	{
	case UNICODE_EQUALS_SIGN:
	  // "<=".
	  result = TOKEN_LESS_EQUALS;
	  break;
	case UNICODE_LESS_THAN_SIGN:
	  c = get ();
	  if (c == UNICODE_EQUALS_SIGN)
	    {
	      // "<<=".
	      ref_assignment a (new model_ls_equal (here ()));
	      result = token (TOKEN_LEFT_SHIFT_ASSIGN, a);
	    }
	  else
	    {
	      // "<<".
	      unget (c);
	      result = TOKEN_LEFT_SHIFT;
	    }
	  break;
	default:
	  // "<".
	  unget (c);
	  result = TOKEN_LESS_THAN;
	  break;
	}
      break;

    case UNICODE_AMPERSAND:
      c = get ();
      switch (c)
	{
	case UNICODE_AMPERSAND:
	  // "&&"
	  result = TOKEN_LOGICAL_AND;
	  break;
	case UNICODE_EQUALS_SIGN:
	  // "&=".
	  {
	    ref_assignment a (new model_and_equal (here ()));
	    result = token (TOKEN_BITWISE_AND_ASSIGN, a);
	  }
	  break;
	default:
	  // "&"
	  unget (c);
	  result = TOKEN_BITWISE_AND;
	  break;
	}
      break;

    case UNICODE_VERTICAL_LINE:
      c = get ();
      switch (c)
	{
	case UNICODE_VERTICAL_LINE:
	  // "||"
	  result = TOKEN_LOGICAL_OR;
	  break;
	case UNICODE_EQUALS_SIGN:
	  // "|=".
	  {
	    ref_assignment a (new model_or_equal (here ()));
	    result = token (TOKEN_BITWISE_OR_ASSIGN, a);
	  }
	  break;
	default:
	  // "|"
	  unget (c);
	  result = TOKEN_BITWISE_OR;
	  break;
	}
      break;

    case UNICODE_PLUS_SIGN:
      c = get ();
      switch (c)
	{
	case UNICODE_PLUS_SIGN:
	  // "++"
	  result = TOKEN_INCREMENT;
	  break;
	case UNICODE_EQUALS_SIGN:
	  // "+="
	  {
	    ref_assignment a (new model_plus_equal (here ()));
	    result = token (TOKEN_PLUS_ASSIGN, a);
	  }
	  break;
	default:
	  unget (c);
	  result = TOKEN_PLUS;
	  break;
	}
      break;

    case UNICODE_HYPHEN_MINUS:
      c = get ();
      switch (c)
	{
	case UNICODE_HYPHEN_MINUS:
	  // "--"
	  result = TOKEN_DECREMENT;
	  break;
	case UNICODE_EQUALS_SIGN:
	  // "-="
	  {
	    ref_assignment a (new model_minus_equal (here ()));
	    result = token (TOKEN_MINUS_ASSIGN, a);
	  }
	  break;
	default:
	  unget (c);
	  result = TOKEN_MINUS;
	  break;
	}
      break;

    case UNICODE_ASTERISK:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	{
	  // "*="
	  ref_assignment a (new model_mult_equal (here ()));
	  result = token (TOKEN_MULTIPLY_ASSIGN, a);
	}
      else
	{
	  unget (c);
	  result = TOKEN_MULTIPLY;
	}
      break;

    case UNICODE_SLASH:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	{
	  // "/="
	  ref_assignment a (new model_div_equal (here ()));
	  result = token (TOKEN_DIVIDE_ASSIGN, a);
	}
      else
	{
	  unget (c);
	  result = TOKEN_DIVIDE;
	}
      break;

    case UNICODE_CIRCUMFLEX_ACCENT:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	{
	  // "^="
	  ref_assignment a (new model_xor_equal (here ()));
	  result = token (TOKEN_BITWISE_XOR_ASSIGN, a);
	}
      else
	{
	  unget (c);
	  result = TOKEN_BITWISE_XOR;
	}
      break;

    case UNICODE_PERCENT_SIGN:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	{
	  // "%="
	  ref_assignment a (new model_mod_equal (here ()));
	  result = token (TOKEN_MOD_ASSIGN, a);
	}
      else
	{
	  unget (c);
	  result = TOKEN_MOD;
	}
      break;

    case UNICODE_EXCLAMATION_MARK:
      c = get ();
      if (c == UNICODE_EQUALS_SIGN)
	result = TOKEN_NOT_EQUALS;
      else
	{
	  unget (c);
	  result = TOKEN_LOGICAL_NOT;
	}
      break;

    default:
      return false;
    }

  assert (result != TOKEN_INVALID);
  return true;
}

token
lexer::get_token_internal (location &where)
{
  unicode_w_t c = skip_whitespace (where);

  // Weird name, this is just Control-Z.
  if (c == UNICODE_SUBSTITUTE)
    {
      // Control-Z is ok just before EOF, but invalid elsewhere.  We
      // handle the error later.
      if (peek () == UNICODE_EOF)
	return TOKEN_EOF;
    }

  if (c == UNICODE_EOF)
    return TOKEN_EOF;

  if (c == UNICODE_SLASH)
    {
      unicode_w_t c2 = peek ();
      if (c2 == UNICODE_SLASH)
	{
	  single_line_comment ();
	  return TOKEN_REPEAT;
	}
      else if (c2 == UNICODE_ASTERISK)
	return multi_line_comment ();
    }

  if (c == UNICODE_APOSTROPHE)
    return character_literal ();

  if (c == UNICODE_QUOTATION_MARK)
    return string_literal ();

  if (c == UNICODE_COMMERCIAL_AT)
    return TOKEN_AT;

  if (c == UNICODE_PERIOD)
    {
      c = peek ();
      if (ascii_digit_p (c))
	return numeric_literal (UNICODE_PERIOD);
      if (c == UNICODE_PERIOD)
	{
	  // We know the character is a '.' so we ignore the return.
	  get ();
	  if (peek () == UNICODE_PERIOD)
	    {
	      get ();
	      return TOKEN_DOTDOTDOT;
	    }
	  // Unget the second '.' and return the first one as a
	  // separator.
	  unget (c);
	}
      return TOKEN_DOT;
    }

  if (ascii_digit_p (c))
    return numeric_literal (c);

  token result;
  if (separator_p (c, result) || operator_p (c, result))
    return result;

  if (extract_identifier (c)
      && scratch[0]
      && handle_keyword (scratch, result))
    return result;

  // Note that at this point, C is still the first character of the
  // potential identifier.
  if (identifier_start_p (c))
    {
      ref_identifier id (new model_identifier (where,
					       std::string (scratch)));
      return token (TOKEN_IDENTIFIER, id);
    }

  throw lex_error (here (), "invalid character %1 in input")
    % print_unicode (c);
}

token
lexer::get_token ()
{
  token result;
  location where;
  do
    {
      result = get_token_internal (where);
    }
  while (result == TOKEN_REPEAT);

  assert (result != TOKEN_REPEAT);
  result.set_location (where);
  return result;
}

const char *
lexer::get_line (int line_number)
{
  // Save the relevant state of the lexer and the input reader before we
  // rewind the input reader.
  int save_line = line;
  int save_column = column;
  int save_backslash_count = backslash_count;
  unicode_w_t save_unget_value = unget_value;
  unicode_w_t save_cooked_unget_value = cooked_unget_value;
  bool save_was_return = was_return;
  int save_posn = input_filter->get_posn ();

  // Reset the state of the lexer and the input reader.
  line = 1;
  column = 0;
  backslash_count = 0;
  unget_value = UNICODE_W_NONE;
  cooked_unget_value = UNICODE_W_NONE;
  was_return = false;
  input_filter->set_posn (0);

  // Ignore input until we reach the desired line (or end of file).
  unicode_w_t c = UNICODE_W_NONE;
  while (line < line_number && (c = get ()) != UNICODE_EOF)
    ;

  // Read in the characters on the desired line.
  char *ret_val = NULL; 
  if (c != UNICODE_EOF)
    {
      int offset = 0;
      do
        {
          offset = scratch_insert_utf8 (offset, c);
        } while ((c = get ()) != UNICODE_EOF && line == line_number);

      ensure (offset + 1);
      scratch[offset] = '\0';
      ret_val = scratch;
    }

  // Restore the state of the lexer and the input reader.
  line = save_line;
  column = save_column;
  backslash_count = save_backslash_count;
  unget_value = save_unget_value;
  cooked_unget_value = save_cooked_unget_value;
  was_return = save_was_return;
  input_filter->set_posn (save_posn);

  return ret_val;
}



const char *
lexer::token_to_string (token_value val)
{
  // FIXME
  return "???";
}



#ifdef LEX_TEST

int
main ()
{
  ucs2_reader *r = new fallback_ucs2_reader (stdin);
  lexer *l = new lexer (r);

  for (token t = l->get_token (); t != TOKEN_EOF; t = l->get_token ())
    {
      printf ("%d\n", t.get_type ());
    }

  delete l;
  return 0;
}

#endif // LEX_TEST
