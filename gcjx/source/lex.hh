// The lexer.

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

#ifndef GCJX_SOURCE_LEX_HH
#define GCJX_SOURCE_LEX_HH

#include "source/token.hh"
#include "source/ucs2.hh"

class lexer
{
  // An internal constant, only used to indicate that the unget value
  // is not valid.
  static const unicode_w_t UNICODE_W_NONE = ((unicode_w_t) -2);

  // The file from which we're reading.
  ucs2_reader *input_filter;

  // Number of consecutive backslashes we've read.
  int backslash_count;

  // If not UNICODE_W_NONE, a character that was pushed back.  This
  // value is only used by escape processing.
  unicode_w_t unget_value;

  // True if we previously saw a \r.  Use by get().
  bool was_return;

  // There's a second layer of unget handling.  This field is used by
  // the more common get()/unget()/peek() set of methods.
  unicode_w_t cooked_unget_value;

  // A scratch buffer used for error display and other stuff.
  char *scratch;
  // Size of the scratch buffer.
  int scratch_size;

  // File name.
  const char *filename;
  // Line number.
  int line;
  // Column number.
  int column;

  // Tab width.  We cache this here so we don't have to keep asking
  // the compiler.
  int tab_width;

  // Buffer of characters from the input reader, its size, and our
  // position in it.
  jchar *chars;
  int num_chars;
  int position;

  // Some methods to characterize characters.
  bool identifier_part_p (unicode_w_t);
  bool identifier_start_p (unicode_w_t);
  bool white_space_p (unicode_w_t);
  bool ascii_digit_p (unicode_w_t);
  bool fp_char_p (unicode_w_t);

  // Return the value of a hexadecimal digit, or -1 if this character
  // isn't a valid hex digit.
  int hex_value (unicode_w_t);

  // Ensure SCRATCH is big enough.
  void ensure (int);

  // Append a character to SCRATCH in UTF-8 format.  Pass in offset
  // at which to place output, returns new offset.
  int scratch_insert_utf8 (int, unicode_w_t);

  // Helper function used to format a single character.
  char *print_unicode (unicode_w_t);

  // Read an identifier from the input stream and put a UTF-8 copy in
  // SCRATCH.  Returns true if all the characters were ASCII.
  bool extract_identifier (unicode_w_t);

  // Get an unprocessed character from the input source.
  unicode_w_t get_raw ();

  // Return a character after processing \u escapes.
  unicode_w_t read_handling_escapes ();

  // The current location.
  location here ();

  // Return a character after processing \u escapes and collapsing
  // line terminators.
  unicode_w_t get ();

  // Like get(), but throw exception if EOF is seen.
  unicode_w_t get_no_eof ();

  // Push back a character.
  void unget (unicode_w_t);

  // Look ahead one character.
  unicode_w_t peek ();

  // Translate a \-escape sequence.
  unicode_w_t escape_sequence ();

  // Scan for the end of a '//' comment.
  void single_line_comment ();

  // Scan for the end of a '/*' comment.
  void end_comment ();

  // Parse a javadoc comment.
  token parse_javadoc (unicode_w_t c);

  // Parse a '/*' comment; either translate to a javadoc comment or
  // an ordinary one.
  token multi_line_comment ();

  // Skip whitespace.  Returns the following character.
  unicode_w_t skip_whitespace (location &);

  // Lexing of literals.
  token character_literal ();
  token string_literal ();
  bool multiply_add (unsigned int[], int, int);
  token create_int_literal (const location &, int, int, bool);
  token parse_float_literal (const location &, int, bool, unicode_w_t);
  token numeric_literal (unicode_w_t);

  // Potentially lex a keyword.  Returns true if a keyword found.
  bool handle_keyword (char *, token &);

  // Handle common separators and some other short punctuation.
  // Returns true and constructs RESULT if character was recognized.
  bool separator_p (unicode_w_t, token &);

  // Handle operators.  Returns true and constructs RESULT if
  // character was recognized.
  bool operator_p (unicode_w_t, token &);

  // Internal interface to the lexer; does most of the work.
  token get_token_internal (location &);

protected:

  // Return a single token.
  token lex_token ();

public:

  lexer (ucs2_reader *source, const char *file);

  virtual ~lexer ();

  // Return the name of a token.
  static const char *token_to_string (token_value);

  /// Return the line at the given position, counting from 1.  The characters
  /// are encoded using UTF-8.  Returns NULL if the line number is out of
  /// range.
  const char *get_line (int line_number);
};

#endif // GCJX_SOURCE_LEX_HH
