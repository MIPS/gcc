/* C++ Parser.
   Copyright (C) 2000, 2001 Free Software Foundation, Inc.
   Written by Mark Mitchell <mark@codesourcery.com>.

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "dyn-string.h"
#include "varray.h"
#include "cpplib.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-lex.h"
#include "decl.h"
#include "flags.h"
#include "diagnostic.h"
#include "ggc.h"
/* FIXME: Should not be required.  */
#include "lex.h"

/* FIXME: Remove!  */
cp_tag_kind current_aggr;

/* FIXME: Do we use OFFSET_TYPEs anywhere in the compiler any more?
   It would be neat if we didn't.  */

/* FIXME: Remove parse.y.  */

/* FIXME: When we see `extern "C" { void f(); }' we have to make
   sure that the `extern' gets applied.  See the old have_extern_spec
   stuff.  */

/* FIXME: Issue a helpful error message for:

   class C {} ... (i.e., where the `;' is missing).  */


/* The lexer.  */

/* Overview
   --------

   A cp_lexer represents a stream of cp_tokens.  It allows arbitrary
   look-ahead.

   Methodology
   -----------

   We use a circular buffer to store incoming tokens.

   Some artifacts of the C++ language (such as the
   expression/declaration ambiguity) require arbitrary look-ahead.
   The strategy we adopt for dealing with these problems is to attempt
   to parse one construct (e.g., the declaration) and fall back to the
   other (e.g., the expression) if that attempt does not succeed.
   Therefore, we must sometimes store an arbitrary number of tokens.

   The parser routinely peeks at the next token, and then consumes it
   later.  That also requires a buffer in which to store the tokens.
     
   In order to easily permit adding tokens to the end of the buffer,
   while removing them from the beginning of the buffer, we use a
   circular buffer.  */

/* A C++ token.  */

typedef struct cp_token
{
  /* The kind of token.  */
  enum cpp_ttype type;
  /* The value associated with this token, if any.  */
  tree value;
  /* If this token is a keyword, this value indicates which keyword.
     Otherwise, this value is RID_MAX.  */
  enum rid keyword;
  /* The line-number at which this token was found.  */
  int line_number;
} cp_token;

/* The cp_lexer structure repesents the C++ lexer.  It is responsible
   for managing the token stream from the preprocessor and supplying
   it to the parser.  */

typedef struct cp_lexer
{
  /* The memory allocated for the buffer.  Never NULL.  */
  cp_token *buffer;
  /* A pointer just past the end of the memory allocated for the buffer.  */
  cp_token *buffer_end;
  /* The first valid token in the buffer, or NULL if none.  */
  cp_token *first_token;
  /* The next available token.  If NEXT_TOKEN is NULL, then there are
     no more available tokens.  */
  cp_token *next_token;
  /* A pointer just past the last available token.  If FIRST_TOKEN is
     NULL, however, there are no available tokens, and then this
     location is simply the place in which the next token read will be
     placed.  If LAST_TOKEN == FIRST_TOKEN, then the buffer is full.
     When the LAST_TOKEN == BUFFER, then the last token is at the
     highest memory address in the BUFFER.  */
  cp_token *last_token;

  /* The input stream from which we should read tokens, or NULL if we
     are not reading from a real input stream.  */
  /* FIXME: This value is not actually passed to c_lex.  */
  struct cpp_reader *input_stream;

  /* A stack indicating positions at which cp_lexer_save_tokens were
     called.  The top entry is the most recent position at which we
     began saving tokens.  

     If the stack is non-empty, we are saving tokens.  When a token is
     consumed, the NEXT_TOKEN pointer will move, but the FIRST_TOKEN
     pointer will not.  The token stream will be preserved so that it
     can be reexamined later.

     If the stack is empty, then we are not saving tokens.  Whenever a
     token is consumed, the FIRST_TOKEN pointer will be moved, and the
     consumed token will be gone forever.  */
  varray_type saved_tokens;

  /* Non-zero if we should output debugging information.  */
  int debugging_p;
  /* The stream to which debugging information should be written.  */
  FILE *debug_stream;
} cp_lexer;

/* Prototypes.  */

static cp_lexer *cp_lexer_new
  PARAMS ((cpp_reader *));
static cp_lexer *cp_lexer_new_from_tokens
  PARAMS ((cp_token *, cp_token *));
static void cp_lexer_delete
  PARAMS ((cp_lexer *));
static int cp_lexer_saving_tokens
  PARAMS ((const cp_lexer *));
static cp_token *cp_lexer_next_token
  PARAMS ((cp_lexer *, cp_token *));
static ptrdiff_t cp_lexer_token_difference
  PARAMS ((cp_lexer *, cp_token *, cp_token *));
static cp_token *cp_lexer_read_token
  PARAMS ((cp_lexer *));
static void cp_lexer_maybe_grow_buffer
  PARAMS ((cp_lexer *));
static void cp_lexer_get_preprocessor_token
  PARAMS ((cp_lexer *, cp_token *));
/* FIXME: Make the token const.  */
static cp_token *cp_lexer_peek_token
  PARAMS ((cp_lexer *));
static cp_token *cp_lexer_peek_nth_token
  PARAMS ((cp_lexer *, size_t));
static bool cp_lexer_next_token_is
  PARAMS ((cp_lexer *, enum cpp_ttype));
static bool cp_lexer_next_token_is_not
  PARAMS ((cp_lexer *, enum cpp_ttype));
static bool cp_lexer_next_token_is_keyword
  PARAMS ((cp_lexer *, enum rid));
static cp_token *cp_lexer_consume_token
  PARAMS ((cp_lexer *));
static void cp_lexer_save_tokens
  PARAMS ((cp_lexer *));
static void cp_lexer_commit_tokens
  PARAMS ((cp_lexer *));
static void cp_lexer_rollback_tokens
  PARAMS ((cp_lexer *));
static void cp_lexer_set_source_position_from_token 
  PARAMS ((const cp_token *));
static void cp_lexer_print_token
  PARAMS ((FILE *, cp_token *));
static bool cp_lexer_debugging_p 
  PARAMS ((cp_lexer *));
static void cp_lexer_start_debugging
  PARAMS ((cp_lexer *));
static void cp_lexer_stop_debugging
  PARAMS ((cp_lexer *));

/* Manifest constants.  */

#define CP_TOKEN_BUFFER_SIZE 5
#define CP_SAVED_TOKENS_SIZE 5

/* A token type for keywords, as opposed to ordinary identifiers.  */

/* FIXME: Put this in cpplib.  */
#define CPP_KEYWORD ((enum cpp_ttype) (N_TTYPES + 1))

/* Create a new C++ lexer, reading from the INPUT_STREAM.  */

static cp_lexer *
cp_lexer_new (input_stream)
     cpp_reader *input_stream;
{
  cp_lexer *lexer;

  /* Allocate the memory.  */
  lexer = (cp_lexer *) xcalloc (1, sizeof (cp_lexer));

  /* Create the circular buffer.  */
  lexer->buffer = ((cp_token *) 
		   xmalloc (CP_TOKEN_BUFFER_SIZE * sizeof (cp_token)));
  lexer->buffer_end = lexer->buffer + CP_TOKEN_BUFFER_SIZE;

  /* There are no tokens in the buffer.  */
  lexer->last_token = lexer->buffer;

  /* Record the INPUT_STREAM.  */
  lexer->input_stream = input_stream;

  /* Create the SAVED_TOKENS stack.  */
  VARRAY_GENERIC_PTR_INIT (lexer->saved_tokens,
			   CP_SAVED_TOKENS_SIZE,
			   "saved_tokens");

  /* Assume we are not debugging.  */
  lexer->debugging_p = 0;
  /* But, if we are, use STDERR as the debugging output stream.  */
  lexer->debug_stream = stderr;

  return lexer;
}

/* Create a new lexer whose token stream is primed with the tokens
   from FIRST_TOKEN up to, but not including, LAST_TOKEN.  When these
   tokens are exhausted, no new tokens will be read.  */

static cp_lexer *
cp_lexer_new_from_tokens (first_token, last_token)
     cp_token *first_token;
     cp_token *last_token;
{
  cp_lexer *lexer;
  ptrdiff_t num_tokens;

  /* Create the lexer.  */
  lexer = cp_lexer_new (/*input_stream=*/NULL);

  /* Destroy the old buffer.  */
  free (lexer->buffer);

  /* Create a new one, appropriately sized.  */
  num_tokens = last_token - first_token;
  lexer->buffer = ((cp_token *) 
		   xmalloc (num_tokens * sizeof (cp_token)));
  lexer->buffer_end = lexer->buffer + num_tokens;
  
  /* Install the tokens.  */
  memcpy (lexer->buffer, first_token, num_tokens * sizeof (cp_token));
  /* The FIRST_TOKEN is the beginning of the buffer.  */
  lexer->first_token = lexer->buffer;
  /* The next available token is also at the beginning of the buffer.  */
  lexer->next_token = lexer->buffer;
  /* The buffer is full.  */
  lexer->last_token = lexer->first_token;

  return lexer;
}

/* Destroy the LEXER.  */

static void
cp_lexer_delete (lexer)
     cp_lexer *lexer;
{
  free (lexer->buffer);
  VARRAY_FREE (lexer->saved_tokens);
  free (lexer);
}

/* Non-zero if we are presently saving tokens.  */

static int
cp_lexer_saving_tokens (lexer)
     const cp_lexer *lexer;
{
  return VARRAY_ACTIVE_SIZE (lexer->saved_tokens) != 0;
}

/* TOKEN points into the circular token buffer.  Return a pointer to
   the next token in the buffer.  */

static cp_token *
cp_lexer_next_token (lexer, token)
     cp_lexer *lexer;
     cp_token *token;
{
  token++;
  if (token == lexer->buffer_end)
    token = lexer->buffer;
  return token;
}

/* Returns the number of times that START would have to be incremented
   to reach FINISH.  If START and FINISH are the same, returns zero.  */

static ptrdiff_t
cp_lexer_token_difference (lexer, start, finish)
     cp_lexer *lexer;
     cp_token *start;
     cp_token *finish;
{
  if (finish >= start)
    return finish - start;
  else
    return ((lexer->buffer_end - lexer->buffer)
	    - (start - finish));
}

/* Obtain another token from the C preprocessor and add it to the
   token buffer.  Returns the newly read token.  */

static cp_token *
cp_lexer_read_token (lexer)
     cp_lexer *lexer;
{
  cp_token *token;

  /* Make sure there is room in the buffer.  */
  cp_lexer_maybe_grow_buffer (lexer);

  /* If there weren't any tokens, then this one will be the first.  */
  if (!lexer->first_token)
    lexer->first_token = lexer->last_token;
  /* Similarly, if there were no available tokens, there is one now.  */
  if (!lexer->next_token)
    lexer->next_token = lexer->last_token;

  /* Figure out where we're going to store the new token.  */
  token = lexer->last_token;

  /* Get a new token from the preprocessor.  */
  cp_lexer_get_preprocessor_token (lexer, token);

  /* Increment LAST_TOKEN.  */
  lexer->last_token = cp_lexer_next_token (lexer, token);

  /* The preprocessor does not yet do translation phase six, i.e., the
     combination of adjacent string literals.  Therefore, we do it
     here.  */
  if (token->type == CPP_STRING || token->type == CPP_WSTRING)
    {
      ptrdiff_t delta;
      bool multiple_strings_p = false;
      int i;

      /* When we grow the buffer, we may invalidate TOKEN.  So, save
	 the distance from the beginning of the BUFFER so that we can
	 recaulate it.  */
      delta = cp_lexer_token_difference (lexer, lexer->buffer, token);
      /* Make sure there is room in the buffer for another token.  */
      cp_lexer_maybe_grow_buffer (lexer);
      /* Restore TOKEN.  */
      token = lexer->buffer;
      for (i = 0; i < delta; ++i)
	token = cp_lexer_next_token (lexer, token);

      while (true)
	{
	  /* Read the token after TOKEN.  */
	  cp_lexer_get_preprocessor_token (lexer, lexer->last_token);
	  /* See whether it's another string constant.  */
	  if (lexer->last_token->type != token->type)
	    {
	      /* If not, then it will be the next real token.  */
	      lexer->last_token = cp_lexer_next_token (lexer, 
						       lexer->last_token);
	      break;
	    }

	  /* Chain the strings together.  */
	  TREE_CHAIN (lexer->last_token->value) = token->value;
	  token->value = lexer->last_token->value;

	  /* Remember that we have seen multiple string constants.  */
	  multiple_strings_p = true;
	}

      /* Create a single STRING_CST.  */
      if (multiple_strings_p)
	token->value = combine_strings (nreverse (token->value));
    }

  return token;
}

/* If the circular buffer is full, make it bigger.  */

static void
cp_lexer_maybe_grow_buffer (lexer)
     cp_lexer *lexer;
{
  /* If the buffer is full, enlarge it.  */
  if (lexer->last_token == lexer->first_token)
    {
      cp_token *new_buffer;
      cp_token *old_buffer;
      cp_token *new_first_token;
      ptrdiff_t buffer_length;
      size_t i;

      /* Remember the current buffer pointer.  It will become invalid,
	 but we will need to do pointer arithmetic involving this
	 value.  */
      old_buffer = lexer->buffer;
      /* Compute the current buffer size.  */
      buffer_length = lexer->buffer_end - lexer->buffer;
      /* Allocate a buffer twice as big.  */
      new_buffer = ((cp_token *)
		    xrealloc (lexer->buffer, 
			      2 * buffer_length * sizeof (cp_token)));
      
      /* Because the buffer is circular, logically consecutive tokens
	 are not necessarily placed consecutively in memory.
	 Therefore, we must keep move the tokens that were before
	 FIRST_TOKEN to the second half of the newly allocated
	 buffer.  */
      memcpy (new_buffer + buffer_length,
	      new_buffer,
	      (lexer->first_token - old_buffer) * sizeof (cp_token));

      /* Now recompute all of the buffer pointers.  */
      new_first_token 
	= new_buffer + (lexer->first_token - old_buffer);
      if (lexer->next_token != NULL)
	{
	  ptrdiff_t next_token_delta;

	  if (lexer->next_token > lexer->first_token)
	    next_token_delta = lexer->next_token - lexer->first_token;
	  else
	    next_token_delta = 
	      buffer_length - (lexer->first_token - lexer->next_token);
	  lexer->next_token = new_first_token + next_token_delta;
	}
      lexer->last_token = new_first_token + buffer_length;

      /* Go through all the SAVED_TOKEN pointers.  */
      for (i = 0; 
	   i < VARRAY_ACTIVE_SIZE (lexer->saved_tokens);
	   ++i)
	{
	  size_t delta;
	  cp_token *saved_token;

	  saved_token = ((cp_token *) 
			 VARRAY_GENERIC_PTR (lexer->saved_tokens, i));

	  delta = cp_lexer_token_difference (lexer,
					     lexer->first_token,
					     saved_token);
	  saved_token = new_first_token + delta;

	  VARRAY_GENERIC_PTR (lexer->saved_tokens, i) = saved_token;
	}

      lexer->buffer = new_buffer;
      lexer->buffer_end = new_buffer + buffer_length * 2;
      lexer->first_token = new_first_token;
    }
}

/* Store the next token from the preprocessor in *TOKEN.  */

static void 
cp_lexer_get_preprocessor_token (lexer, token)
     cp_lexer *lexer ATTRIBUTE_UNUSED;
     cp_token *token;
{
  /* If there's no INPUT_STREAM, return a terminating CPP_EOF token.  */
  if (!lexer->input_stream)
    {
      token->type = CPP_EOF;
      token->line_number = 0;
      token->value = NULL_TREE;
      token->keyword = RID_MAX;

      return;
    }

  /* Get a new token from the preprocessor.  */
  while (true)
    {
      /* The call to c_lex should really require us to pass the
	 INPUT_STREAM, but it is hardwired to use a global value.  */
      token->type = c_lex (&token->value);
      /* If we've reached end-of-file, we may simply need to pop
	 buffers to return to the file that #include'd this one.  */
      if (token->type == CPP_EOF && cpp_pop_buffer (parse_in) != 0)
	continue;
      /* Now we've got our token.  */
      token->line_number = lineno;
      break;
    }

  /* Check to see if this token is a keyword.  */
  if (token->type == CPP_NAME 
      && C_IS_RESERVED_WORD (token->value))
    {
      /* Mark this token as a keyword.  */
      token->type = CPP_KEYWORD;
      /* Record which keyword.  */
      token->keyword = C_RID_CODE (token->value);
      /* Update the value.  Some keywords are mapped to particular
	 entities, rather than simply having the value of the
	 corresponding IDENTIFIER_NODE.  For example, `__const' is
	 mapped to `const'.  */
      token->value = ridpointers[token->keyword];
    }
  else
    token->keyword = RID_MAX;
}

/* Return a pointer to the next token in the token stream, but do not
   consume it.  */

static cp_token *
cp_lexer_peek_token (lexer)
     cp_lexer *lexer;
{
  /* If there are no tokens, read one now.  */
  if (!lexer->next_token)
    cp_lexer_read_token (lexer);

  /* Provide debugging output.  */
  if (cp_lexer_debugging_p (lexer))
    {
      fprintf (lexer->debug_stream, "cp_lexer: peeking at token: ");
      cp_lexer_print_token (lexer->debug_stream, lexer->next_token);
      fprintf (lexer->debug_stream, "\n");
    }

  return lexer->next_token;
}

/* Return true if the next token has the indicated TYPE.  */

static bool
cp_lexer_next_token_is (lexer, type)
     cp_lexer *lexer;
     enum cpp_ttype type;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (lexer);
  /* Check to see if it has the indicated TYPE.  */
  return token->type == type;
}

/* Return true if the next token does not have the indicated TYPE.  */

static bool
cp_lexer_next_token_is_not (lexer, type)
     cp_lexer *lexer;
     enum cpp_ttype type;
{
  return !cp_lexer_next_token_is (lexer, type);
}

/* Return true if the next token is the indicated KEYWORD.  */

static bool
cp_lexer_next_token_is_keyword (lexer, keyword)
     cp_lexer *lexer;
     enum rid keyword;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (lexer);
  /* Check to see if it is the indicated keyword.  */
  return token->keyword == keyword;
}

/* Return a pointer to the Nth token in the token stream.  If N is 1,
   then this is precisely equivalent to cp_lexer_peek_token.  */

static cp_token *
cp_lexer_peek_nth_token (lexer, n)
     cp_lexer *lexer;
     size_t n;
{
  cp_token *token;

  /* N is 1-based, not zero-based.  */
  my_friendly_assert (n > 0, 20000224);

  /* Skip ahead from NEXT_TOKEN, reading more tokens as necessary.  */
  token = lexer->next_token;
  /* If there are no tokens in the buffer, get one now.  */
  if (!token)
    {
      cp_lexer_read_token (lexer);
      token = lexer->next_token;
    }

  /* Now, read tokens until we have enough.  */
  while (--n > 0)
    {
      /* Advance to the next token.  */
      token = cp_lexer_next_token (lexer, token);
      /* If that's all the tokens we have, read a new one.  */
      if (token == lexer->last_token)
	token = cp_lexer_read_token (lexer);
    }

  return token;
}

/* Consume the next token.  The pointer returned is valid only until
   another token is read.  Callers should preserve copy the token
   explicitly if they will need its value for a longer period of
   time.  */

static cp_token *
cp_lexer_consume_token (lexer)
     cp_lexer *lexer;
{
  cp_token *token;

  /* If there are no tokens, read one now.  */
  if (!lexer->next_token)
    cp_lexer_read_token (lexer);

  /* Remember the token we'll be returning.  */
  token = lexer->next_token;

  /* Increment NEXT_TOKEN.  */
  lexer->next_token = cp_lexer_next_token (lexer, 
					   lexer->next_token);
  /* Check to see if we're all out of tokens.  */
  if (lexer->next_token == lexer->last_token)
    lexer->next_token = NULL;

  /* If we're not saving tokens, then move FIRST_TOKEN too.  */
  if (!cp_lexer_saving_tokens (lexer))
    {
      /* If there are no tokens available, set FIRST_TOKEN to NULL.  */
      if (!lexer->next_token)
	lexer->first_token = NULL;
      else
	lexer->first_token = lexer->next_token;
    }

  /* Provide debugging output.  */
  if (cp_lexer_debugging_p (lexer))
    {
      fprintf (lexer->debug_stream, "cp_lexer: consuming token: ");
      cp_lexer_print_token (lexer->debug_stream, token);
      fprintf (lexer->debug_stream, "\n");
    }

  /* Now that we've consumed this token, we've moved on to a new
     source position.  */
  if (token->type != CPP_EOF)
    cp_lexer_set_source_position_from_token (token);

  return token;
}

/* Begin saving tokens.  All tokens consumed after this point will be
   preserved.  */

static void
cp_lexer_save_tokens (lexer)
     cp_lexer *lexer;
{
  /* Provide debugging output.  */
  if (cp_lexer_debugging_p (lexer))
    fprintf (lexer->debug_stream, "cp_lexer: saving tokens\n");

  /* Make sure that LEXER->NEXT_TOKEN is non-NULL so that we can
     restore the tokens if required.  */
  if (!lexer->next_token)
    cp_lexer_read_token (lexer);

  VARRAY_PUSH_GENERIC_PTR (lexer->saved_tokens,
			   lexer->next_token);
}

/* Commit to the portion of the token stream most recently saved.  */

static void
cp_lexer_commit_tokens (lexer)
     cp_lexer *lexer;
{
  /* Provide debugging output.  */
  if (cp_lexer_debugging_p (lexer))
    fprintf (lexer->debug_stream, "cp_lexer: committing tokens\n");

  VARRAY_POP (lexer->saved_tokens);
}

/* Return all tokens saved since the last call to cp_lexer_save_tokens
   to the token stream.  Stop saving tokens.  */

static void
cp_lexer_rollback_tokens (lexer)
     cp_lexer *lexer;
{
  cp_token *token;

  /* Provide debugging output.  */
  if (cp_lexer_debugging_p (lexer))
    fprintf (lexer->debug_stream, "cp_lexer: restoring tokens\n");

  /* Find the token that was the NEXT_TOKEN when we started saving
     tokens.  */
  token = VARRAY_TOP_GENERIC_PTR (lexer->saved_tokens);
  /* Make it the next token again now.  */
  lexer->next_token = token;
  /* It might be the case that there wer no tokens when we started
     saving tokens, but that there are some tokens now.  */
  if (!lexer->next_token && lexer->first_token)
    lexer->next_token = lexer->first_token;

  /* Stop saving tokens.  */
  VARRAY_POP (lexer->saved_tokens);

  /* Set the source position back to the location where we started
     storing up tokens.  */
  token = cp_lexer_peek_token (lexer);
  if (token->type != CPP_EOF)
    cp_lexer_set_source_position_from_token (token);
}

/* Set the current source position from the information stored in
   TOKEN.  */

static void
cp_lexer_set_source_position_from_token (token)
     const cp_token *token;
{
  /* Update the line number.  */
  lineno = token->line_number;
  /* FIXME: We should update the filename, too.  */
}

/* Print a representation of the TOKEN on the STREAM.  */

static void
cp_lexer_print_token (stream, token)
     FILE *stream;
     cp_token *token;
{
  const char *token_type = NULL;

  /* Figure out what kind of token this is.  */
  switch (token->type)
    {
    case CPP_EQ:
      token_type = "EQ";
      break;

    case CPP_COMMA:
      token_type = "COMMA";
      break;

    case CPP_OPEN_PAREN:
      token_type = "OPEN_PAREN";
      break;

    case CPP_CLOSE_PAREN:
      token_type = "CLOSE_PAREN";
      break;

    case CPP_OPEN_BRACE:
      token_type = "OPEN_BRACE";
      break;

    case CPP_CLOSE_BRACE:
      token_type = "CLOSE_BRACE";
      break;

    case CPP_SEMICOLON:
      token_type = "SEMICOLON";
      break;

    case CPP_NAME:
      token_type = "NAME";
      break;

    case CPP_EOF:
      token_type = "EOF";
      break;

    case CPP_KEYWORD:
      token_type = "keyword";
      break;

      /* This is not a token that we know how to handle yet.  */
    default:
      break;
    }

  /* If we have a name for the token, print it out.  Otherwise, we
     simply give the numeric code.  */
  if (token_type)
    fprintf (stream, "%s", token_type);
  else
    fprintf (stream, "%d", token->type);
  /* And, for an identifier, print the identifier name.  */
  if (token->type == CPP_NAME 
      /* Some keywords have a value that is not an IDENTIFIER_NODE.
	 For example, `struct' is mapped to an INTEGER_CST.  */
      || (token->type == CPP_KEYWORD 
	  && TREE_CODE (token->value) == IDENTIFIER_NODE))
    fprintf (stream, " %s", IDENTIFIER_POINTER (token->value));
}

/* Returns non-zero if debugging information should be output.  */

static bool
cp_lexer_debugging_p (lexer)
     cp_lexer *lexer;
{
  return lexer->debugging_p;
}

/* Start emitting debugging information.  */

static void
cp_lexer_start_debugging (lexer)
     cp_lexer *lexer;
{
  ++lexer->debugging_p;
}
  
/* Stop emitting debugging information.  */

static void
cp_lexer_stop_debugging (lexer)
     cp_lexer *lexer;
{
  --lexer->debugging_p;
}


/* The parser.  */

/* Overview
   --------

   A cp_parser parses the token stream as specified by the C++
   grammar.  Its job is purely parsing, not semantic analysis.  For
   example, the parser breaks the token stream into declarators,
   expressions, statements, and other similar syntactic constructs.
   It does not check that the types of the expressions on either side
   of an assignment-statement are compatible, or that a function is
   not declared with a parameter of type `void'.

   The parser invokes routines elsewhere in the compiler to perform
   semantic analysis and to build up the abstract syntax tree for the
   code processed.  

   The parser (and the template instantiation code, which is, in a
   way, a close relative of parsing) are the only parts of the
   compiler that should be calling push_scope and pop_scope, or
   related functions.  The parser (and template instantiation code)
   keep track of what scope is presently active; everything else
   should simply honor that.  (The code that generates static
   initializers may also need to set the scope in order to check
   access control correctly when emitting the initializers.)

   Methodology
   -----------
   
   The parser is of the standard recursive-descent variety.  Upcoming
   tokens in the token stream are examined in order to determine which
   production to use when parsing a non-terminal.  Some C++ constructs
   require arbitrary look ahead to disambiguate.  For example, it is
   impossible, in the general case, to tell whether a statement is an
   expression or declaration without scanning the entire statement.
   Therefore, the parser is capable of "parsing tentatively."  When the
   parser is not sure what construct comes next, it enters this mode.
   Then, while we attempt to parse the construct, the parser queues up
   error messages, rather than issuing them immediately, and saves the
   tokens it consumes.  If the construct is parsed successfully, the
   parser "commits", i.e., it issues any queued error messages and
   the tokens that were being preserved are permanently discarded.
   If, however, the construct is not parsed successfully, the parser
   rolls back its state completely so that it can resume parsing using
   a different alternative.

   Future Improvements
   -------------------
   
   The performance of the parser could probably be improved
   substantially.  Some possible improvements include:

     - The expression parser recurses through the various levels of
       precedence as specified in the grammar, rather than using an
       operator-precedence technique.  Therefore, parsing a simple
       identifier requires multiple recursive calls.

     - We could often eliminate the need to parse tentatively by
       looking ahead a little bit.  In some places, this appraoch
       might not entirely eliminate the need to parse tentatively, but
       it might still speed up the average case.

   FIXME: Discuss error-handling (i.e., when to use cp_parser_error),
   and error-recovery.  */

/* FIXME: Replace calls to cp_lexer_peek_token with
   cp_lexer_next_token_is.  */

/* FIXME: Check for all the places new types are not allowed to be 
   defined.  */

/* FIXME: Get rid of record_type_node and its ilk.  */

/* FIXME: Factor identifier-or-template-id into a single function.  */

/* Flags that are pased to some parsing functions.  These values can
   be bitwise-ored together.  */

/* FIXME: Do we still use these flags?  */

typedef enum cp_parser_flags
{
  /* No flags.  */
  CP_PARSER_FLAGS_NONE = 0x0,
  /* The construct is optional.  If it is not present, then no error
     should be issued.  */
  CP_PARSER_FLAGS_OPTIONAL = 0x1,
  /* When parsing a type-specifier, do not allow user-defined types.  */
  CP_PARSER_FLAGS_NO_USER_DEFINED_TYPES = 0x2
} cp_parser_flags;

/* The different kinds of ids that we ecounter.  */

typedef enum cp_parser_id_kind
{
  /* Not an id at all.  */
  CP_PARSER_ID_KIND_NONE,
  /* An unqualified-id.  */
  CP_PARSER_ID_KIND_UNQUALIFIED,
  /* A qualified-id.  */
  CP_PARSER_ID_KIND_QUALIFIED
} cp_parser_id_kind;

/* A mapping from a token type to a corresponding tree node type.  */

typedef struct cp_parser_token_tree_map_node
{
  /* The token type.  */
  enum cpp_ttype token_type;
  /* The corresponding tree code.  */
  enum tree_code tree_type;
} cp_parser_token_tree_map_node;

/* A complete map consists of several ordinary entries, followed by a
   terminator.  The terminating entry has a token-type of CPP_EOF.  */

typedef cp_parser_token_tree_map_node cp_parser_token_tree_map[];

/* When a function definition is encountered in a class scope, the
   body of the function is saved away and parsed later, after the
   other members in the class have been declared.  This structure
   contains data about the saved function definition.  */

typedef struct cp_parser_inline_definition
{
  /* The obstack on which the tokens are saved.  */
  struct obstack token_obstack;
} cp_parser_inline_definition;

/* The cp_parser structure represents the C++ parser.  */

typedef struct cp_parser
{
  /* The lexer from which we are obtaining tokens.  */
  cp_lexer *lexer;

  /* The scope in which names should be looked up.  If NULL_TREE, then
     we look up names in the scope that is currently open in the
     source program.  If non-NULL, this is either a TYPE or
     NAMESPACE_DECL for the scope in which we should look.  

     This value is not cleared automatically after a name is looked
     up, so we must be careful to clear it before starting a new look
     up sequence.  (If it is not cleared, then `X::Y' followed by `Z'
     will look up `Z' in the scope of `X', rather than the current
     scope.)  Unfortunately, it is difficult to tell when name lookup
     is complete, because we sometimes peek at a token, look it up,
     and then decide not to consume it.  */
  tree scope;

  /* If the stack is non-empty, we are parsing tentatively.

     We parse tentatively in order to determine which construct is in
     use in some situations.  For example, in order to determine
     whether a statement is an expression-statement or a
     declaration-statement we parse it tentatively as a
     declaration-statement.  If that fails, we then reparse the same
     token stream as an expression-statement.  

     Each entry in the stack is an integer.  If it is zero, then no
     error has occurred during the most recent tentative parse.  If it
     is one, an error has occurred during the most recent tentative
     parse.  If it is two, then we have decided to take this
     alternative regardless of whether or not an error has occurred.  */
  varray_type parsing_tentatively;

  /* True if we are parsing GNU C++.  If this flag is not set, then
     GNU extensions are not recognized.  */
  bool allow_gnu_extensions_p;

  /* TRUE if the `>' token should be interpreted as the greater-than
     operator.  FALSE if it is the end of a template-id or
     template-parameter-list.  */
  bool greater_than_is_operator_p;

  /* TRUE if we are parsing an integral constant-expression.  See
     [expr.const] for a precise definition.  */
  /* FIXME: Need to implement code that checks this flag.  */
  bool constant_expression_p;

  /* A TREE_LIST representing name-lookups for which we have deferred
     checking access controls.  We cannot check the accessibility of
     names used in a decl-specifier-seq until we know what is being
     declared because code like:

       class A { 
         class B {};
         B* f();
       }

       A::B* A::f() { return 0; }

     is valid, even though `A::B' is not generally accessible.  

     The TREE_PURPOSE of each node is the scope used to qualify the
     name being looked up; the TREE_VALUE is the DECL to which the
     name was resolved.  */
  /* FIXME: Does this need to be a GC root?  */
  /* FIXME: Nothing actually uses this, meaning that we don't do
     access control on types.  */
  tree deferred_access_control;
} cp_parser;

/* The type of a function that parses some kind of expression  */
typedef tree (*cp_parser_expression_fn) PARAMS ((cp_parser *));

/* Prototypes.  */

/* Constructors and destructors.  */

static cp_parser *cp_parser_new
  PARAMS ((void));
static void cp_parser_delete
  PARAMS ((cp_parser *));

/* Routines to parse various constructs.  

   Those that return `tree' will return the error_mark_node (rather
   than NULL_TREE) if a parse error occurs, unless otherwise noted.
   Sometimes, they will return an ordinary node if error-recovery was
   attempted, even though a parse error occurrred.  So, to check
   whether or not a parse error ocurrred, you should always use
   cp_parser_error_ocurred.  If the construct is optional (indicated
   either by an `_opt' in the anem of the function that does the
   parsing or via a FLAGS parameter), then NULL_TREE is returned if
   the construct is not present.  */

/* Lexical conventions [gram.lex]  */

static tree cp_parser_identifier
  PARAMS ((cp_parser *));

/* Basic concepts [gram.basic]  */

static bool cp_parser_translation_unit
  PARAMS ((cp_parser *));

/* Expressions [gram.expr]  */

static tree cp_parser_primary_expression
  PARAMS ((cp_parser *));
static tree cp_parser_id_expression
  PARAMS ((cp_parser *, bool));
static tree cp_parser_unqualified_id
  PARAMS ((cp_parser *));
static tree cp_parser_nested_name_specifier_opt
  PARAMS ((cp_parser *, bool));
static tree cp_parser_nested_name_specifier
  PARAMS ((cp_parser *, bool));
static tree cp_parser_class_or_namespace_name
  PARAMS ((cp_parser *, bool, bool));
static tree cp_parser_postfix_expression
  PARAMS ((cp_parser *));
static tree cp_parser_expression_list
  PARAMS ((cp_parser *));
static void cp_parser_pseudo_destructor_name
  PARAMS ((cp_parser *, tree *, tree *));
static tree cp_parser_unary_expression
  PARAMS ((cp_parser *));
static enum tree_code cp_parser_unary_operator
  PARAMS ((cp_token *));
static tree cp_parser_new_expression
  PARAMS ((cp_parser *));
static tree cp_parser_new_placement
  PARAMS ((cp_parser *));
static tree cp_parser_new_type_id
  PARAMS ((cp_parser *));
static tree cp_parser_new_declarator_opt
  PARAMS ((cp_parser *));
static tree cp_parser_direct_new_declarator
  PARAMS ((cp_parser *));
static tree cp_parser_new_initializer
  PARAMS ((cp_parser *));
static tree cp_parser_delete_expression
  PARAMS ((cp_parser *));
static tree cp_parser_cast_expression 
  PARAMS ((cp_parser *));
static tree cp_parser_pm_expression
  PARAMS ((cp_parser *));
static tree cp_parser_multiplicative_expression
  PARAMS ((cp_parser *));
static tree cp_parser_additive_expression
  PARAMS ((cp_parser *));
static tree cp_parser_shift_expression
  PARAMS ((cp_parser *));
static tree cp_parser_relational_expression
  PARAMS ((cp_parser *));
static tree cp_parser_equality_expression
  PARAMS ((cp_parser *));
static tree cp_parser_and_expression
  PARAMS ((cp_parser *));
static tree cp_parser_exclusive_or_expression
  PARAMS ((cp_parser *));
static tree cp_parser_inclusive_or_expression
  PARAMS ((cp_parser *));
static tree cp_parser_logical_and_expression
  PARAMS ((cp_parser *));
static tree cp_parser_logical_or_expression 
  PARAMS ((cp_parser *));
static tree cp_parser_conditional_expression
  PARAMS ((cp_parser *));
static tree cp_parser_question_colon_clause
  PARAMS ((cp_parser *, tree));
static tree cp_parser_assignment_expression
  PARAMS ((cp_parser *));
static enum tree_code cp_parser_assignment_operator_opt
  PARAMS ((cp_parser *));
static tree cp_parser_expression
  PARAMS ((cp_parser *));
static tree cp_parser_constant_expression
  PARAMS ((cp_parser *));

/* Statements [gram.stmt.stmt]  */

static tree cp_parser_statement
  PARAMS ((cp_parser *));
static tree cp_parser_labeled_statement
  PARAMS ((cp_parser *));
static tree cp_parser_expression_statement
  PARAMS ((cp_parser *));
static tree cp_parser_compound_statement
  PARAMS ((cp_parser *));
static void cp_parser_statement_seq_opt
  PARAMS ((cp_parser *));
static tree cp_parser_selection_statement
  PARAMS ((cp_parser *));
static tree cp_parser_condition
  PARAMS ((cp_parser *));
static tree cp_parser_iteration_statement
  PARAMS ((cp_parser *));
static void cp_parser_for_init_statement
  PARAMS ((cp_parser *));
static tree cp_parser_jump_statement
  PARAMS ((cp_parser *));
static tree cp_parser_declaration_statement
  PARAMS ((cp_parser *));

static tree cp_parser_implicitly_scoped_statement
  PARAMS ((cp_parser *));
static tree cp_parser_already_scoped_statement
  PARAMS ((cp_parser *));

/* Declarations [gram.dcl.dcl] */

static void cp_parser_declaration_seq_opt
  PARAMS ((cp_parser *));
static void cp_parser_declaration
  PARAMS ((cp_parser *));
static void cp_parser_block_declaration
  PARAMS ((cp_parser *));
static void cp_parser_simple_declaration
  PARAMS ((cp_parser *));
static tree cp_parser_decl_specifier_seq 
  PARAMS ((cp_parser *, cp_parser_flags, tree *, bool *));
static tree cp_parser_type_specifier
  PARAMS ((cp_parser *, cp_parser_flags, bool, bool *, bool *));
static tree cp_parser_simple_type_specifier
  PARAMS ((cp_parser *, cp_parser_flags));
static tree cp_parser_type_name
  PARAMS ((cp_parser *));
static tree cp_parser_elaborated_type_specifier
  PARAMS ((cp_parser *, bool));
static tree cp_parser_enum_specifier
  PARAMS ((cp_parser *));
static void cp_parser_enumerator_list
  PARAMS ((cp_parser *, tree));
static void cp_parser_enumerator_definition 
  PARAMS ((cp_parser *, tree));
static tree cp_parser_namespace_name
  PARAMS ((cp_parser *));
static void cp_parser_namespace_definition
  PARAMS ((cp_parser *));
static void cp_parser_namespace_body
  PARAMS ((cp_parser *));
static tree cp_parser_qualified_namespace_specifier
  PARAMS ((cp_parser *));
static void cp_parser_namespace_alias_definition
  PARAMS ((cp_parser *));
static void cp_parser_using_declaration
  PARAMS ((cp_parser *));
static void cp_parser_using_directive
  PARAMS ((cp_parser *));
static void cp_parser_asm_definition
  PARAMS ((cp_parser *));
static void cp_parser_linkage_specification
  PARAMS ((cp_parser *));

/* Declarators [gram.dcl.decl] */

static tree cp_parser_init_declarator
  PARAMS ((cp_parser *, tree, tree, unsigned));
static tree cp_parser_declarator
  PARAMS ((cp_parser *, bool, bool *));
static tree cp_parser_direct_declarator
  PARAMS ((cp_parser *, bool, bool *));
static enum tree_code cp_parser_ptr_operator
  PARAMS ((cp_parser *, tree *, tree *));
static tree cp_parser_cv_qualifier_seq_opt
  PARAMS ((cp_parser *));
static tree cp_parser_cv_qualifier_opt
  PARAMS ((cp_parser *));
static tree cp_parser_declarator_id
  PARAMS ((cp_parser *));
static tree cp_parser_type_id
  PARAMS ((cp_parser *));
static tree cp_parser_type_specifier_seq
  PARAMS ((cp_parser *));
static tree cp_parser_parameter_declaration_clause
  PARAMS ((cp_parser *));
static tree cp_parser_parameter_declaration_list
  PARAMS ((cp_parser *));
static tree cp_parser_parameter_declaration
  PARAMS ((cp_parser *, bool));
static tree cp_parser_function_definition
  PARAMS ((cp_parser *, unsigned));
static void cp_parser_function_body
  PARAMS ((cp_parser *));
static tree cp_parser_initializer
  PARAMS ((cp_parser *, bool *));
static tree cp_parser_initializer_clause
  PARAMS ((cp_parser *));
static tree cp_parser_initializer_list
  PARAMS ((cp_parser *));

/* Classes [gram.class] */

static tree cp_parser_class_name
  PARAMS ((cp_parser *, bool, bool));
static tree cp_parser_class_specifier
  PARAMS ((cp_parser *));
static tree cp_parser_class_head
  PARAMS ((cp_parser *, bool *));
static cp_tag_kind cp_parser_class_key
  PARAMS ((cp_parser *));
static void cp_parser_member_specification_opt
  PARAMS ((cp_parser *));
static void cp_parser_member_declaration
  PARAMS ((cp_parser *));
static tree cp_parser_pure_specifier
  PARAMS ((cp_parser *));
static tree cp_parser_constant_initializer
  PARAMS ((cp_parser *));

/* Derived classes [gram.class.derived] */

static tree cp_parser_base_clause
  PARAMS ((cp_parser *));
static tree cp_parser_base_specifier
  PARAMS ((cp_parser *));

/* Special member functions [gram.special] */

static tree cp_parser_conversion_function_id
  PARAMS ((cp_parser *));
static tree cp_parser_conversion_type_id
  PARAMS ((cp_parser *));
static tree cp_parser_conversion_declarator_opt
  PARAMS ((cp_parser *));
static void cp_parser_ctor_initializer
  PARAMS ((cp_parser *));
static void cp_parser_mem_initializer_list
  PARAMS ((cp_parser *));
static tree cp_parser_mem_initializer
  PARAMS ((cp_parser *));
static tree cp_parser_mem_initializer_id
  PARAMS ((cp_parser *));

/* Overloading [gram.over] */

static tree cp_parser_operator_function_id
  PARAMS ((cp_parser *));
static tree cp_parser_operator
  PARAMS ((cp_parser *));

/* Templates [gram.temp] */

static void cp_parser_template_declaration
  PARAMS ((cp_parser *, bool));
static tree cp_parser_template_parameter_list
  PARAMS ((cp_parser *));
static tree cp_parser_template_parameter
  PARAMS ((cp_parser *));
static tree cp_parser_type_parameter
  PARAMS ((cp_parser *));
static tree cp_parser_template_id
  PARAMS ((cp_parser *, bool));
static tree cp_parser_template_name
  PARAMS ((cp_parser *, bool));
static tree cp_parser_template_argument_list
  PARAMS ((cp_parser *));
static tree cp_parser_template_argument
  PARAMS ((cp_parser *));
static void cp_parser_explicit_instantiation
  PARAMS ((cp_parser *));
static void cp_parser_explicit_specialization
  PARAMS ((cp_parser *));

/* Exception handling [gram.exception] */

static tree cp_parser_try_block 
  PARAMS ((cp_parser *));
static void cp_parser_function_try_block
  PARAMS ((cp_parser *));
static void cp_parser_handler_seq
  PARAMS ((cp_parser *));
static void cp_parser_handler
  PARAMS ((cp_parser *));
static tree cp_parser_exception_declaration
  PARAMS ((cp_parser *));
static tree cp_parser_throw_expression
  PARAMS ((cp_parser *));
static tree cp_parser_exception_specification_opt
  PARAMS ((cp_parser *));
static tree cp_parser_type_id_list
  PARAMS ((cp_parser *));

/* GNU Extensions */

static tree cp_parser_asm_specification_opt
  PARAMS ((cp_parser *));
static tree cp_parser_asm_operand_list
  PARAMS ((cp_parser *));
static tree cp_parser_asm_clobber_list
  PARAMS ((cp_parser *));
static tree cp_parser_attributes_opt
  PARAMS ((cp_parser *));
static tree cp_parser_attribute_list
  PARAMS ((cp_parser *));
static bool cp_parser_extension_opt
  PARAMS ((cp_parser *, int *));

/* Utility Routines */

static tree cp_parser_lookup_name
  PARAMS ((cp_parser *, tree));
static void cp_parser_maybe_lookup_name
  PARAMS ((cp_parser *, tree *, cp_parser_id_kind *));
static tree cp_parser_resolve_typename_type
  PARAMS ((tree));
static tree cp_parser_get_scope_of_declarator 
  PARAMS ((tree));
static bool cp_parser_check_template_parameters
  PARAMS ((tree, unsigned));
static tree cp_parser_binary_expression
  PARAMS ((cp_parser *, 
	   cp_parser_token_tree_map,
	   cp_parser_expression_fn));
static tree cp_parser_global_scope_opt
  PARAMS ((cp_parser *, bool));
static tree cp_parser_function_definition_after_declarator
  PARAMS ((cp_parser *, bool));
static void cp_parser_template_declaration_after_export
  PARAMS ((cp_parser *, bool, unsigned));
static tree cp_parser_single_declaration
  PARAMS ((cp_parser *, unsigned, tree *));
static tree cp_parser_functional_cast
  PARAMS ((cp_parser *, tree));
static tree cp_parser_declares_only_class_p
  PARAMS ((cp_parser *, tree, bool *));
static cp_token *cp_parser_require
  PARAMS ((cp_parser *, enum cpp_ttype, const char *));
static cp_token *cp_parser_require_keyword
  PARAMS ((cp_parser *, enum rid, const char *));
static void cp_parser_parse_tentatively 
  PARAMS ((cp_parser *));
static void cp_parser_commit_to_tentative_parse
  PARAMS ((cp_parser *));
static void cp_parser_abort_tentative_parse
  PARAMS ((cp_parser *));
static bool cp_parser_parse_definitely
  PARAMS ((cp_parser *));
static bool cp_parser_parsing_tentatively
  PARAMS ((cp_parser *));
static bool cp_parser_committed_to_tentative_parse
  PARAMS ((cp_parser *));
static void cp_parser_error
  PARAMS ((cp_parser *, const char *));
static void cp_parser_simulate_error
  PARAMS ((cp_parser *));
static bool cp_parser_skip_to_closing_parenthesis
  PARAMS ((cp_parser *));
static void cp_parser_skip_tokens
  PARAMS ((cp_parser *, enum cpp_ttype));
static bool cp_parser_error_occurred
  PARAMS ((cp_parser *));
static bool cp_parser_allow_gnu_extensions_p
  PARAMS ((cp_parser *));
static bool cp_parser_is_string_literal
  PARAMS ((cp_token *));
static bool cp_parser_is_keyword 
  PARAMS ((cp_token *, enum rid));
static bool cp_parser_dependent_type_p
  PARAMS ((tree));
static bool cp_parser_type_dependent_expression_p
  PARAMS ((tree));
static void cp_parser_unimplemented
  PARAMS ((void));

/* Manifest constants.  */

#define CP_PARSER_PARSING_TENATIVELY_STACK_SIZE 16

/* Returns non-zero if TOKEN is a string literal.  */

static bool
cp_parser_is_string_literal (token)
     cp_token *token;
{
  return (token->type == CPP_STRING || token->type == CPP_WSTRING);
}

/* Returns non-zero if TOKEN is the indicated KEYWORD.  */

static bool
cp_parser_is_keyword (token, keyword)
     cp_token *token;
     enum rid keyword;
{
  return token->keyword == keyword;
}

/* Returns TRUE if TYPE is dependent, in the sense of
   [temp.dep.type].  */

static bool
cp_parser_dependent_type_p (type)
     tree type;
{
  /* The checks below are relatively expensive, so we avoid them when
     we can.  If we no template parameters are in scope, there cannot
     be any dependent types.  */
  if (!current_template_parms)
    return false;

  /* FIXME: uses_template_parms is not 100% correct; consider, for
     example, a variable with typen `S<sizeof (sizeof (T))>' where
     `T' is a template parameter.  In particular, `sizeof (T)' is
     not type-dependent, so `sizeof (sizeof (T))' is not
     type-dependent or value-dependent.  We should keep a bit on
     expressions and types to note whether or not they are
     dependent.  */
  return uses_template_parms (type);
}

/* Returns TRUE if the EXPRESSION is type-dependent, in the sense of
   [temp.dep.expr].  */

static bool
cp_parser_type_dependent_expression_p (expression)
     tree expression;
{
  /* FIXME: This is probably not a completely correct implementation.
     We may need a bit per expression to keep track of this.  */
  /* If the EXPRESSION does not have a type it must be type-dependent;
     otherwise, we would have assigned it a type.  */
  return (TREE_TYPE (expression)
	  ? cp_parser_dependent_type_p (TREE_TYPE (expression))
	  : true);
}

/* This function is called when some feature of the parser has not
   been implemented.  */

static void
cp_parser_unimplemented ()
{
  abort ();
}

/* Issue the indicated error MESSAGE.  */

static void
cp_parser_error (parser, message)
     cp_parser *parser;
     const char *message;
{
  /* Remember that we have issued an error.  */
  cp_parser_simulate_error (parser);
  /* Output the MESSAGE.  */
  cp_error (message);
}

/* If we are parsing tentatively, remember that an error has occurred
   during this tentative parse.  */

static void
cp_parser_simulate_error (parser)
     cp_parser *parser;
{
  if (cp_parser_parsing_tentatively (parser)
      && !cp_parser_committed_to_tentative_parse (parser))
    VARRAY_TOP_UINT (parser->parsing_tentatively) = 1;
}

/* Consume tokens until the next token has the indicated
   TOKEN_TYPE, or until the next token has type CPP_EOF.  */

static void
cp_parser_skip_tokens (parser, token_type)
     cp_parser *parser;
     enum cpp_ttype token_type;
{
  cp_token *token;

  while (true)
    {
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's what we want, stop.  */
      if (token->type == token_type
	  || token->type == CPP_EOF)
	break;
      /* Otherwise, consume the token.  */
      cp_lexer_consume_token (parser->lexer);
    }
}

/* Consume tokens up to, and including, the next non-nested closing `)'. 
   Returns TRUE iff we found a closing `)'.  */

static bool
cp_parser_skip_to_closing_parenthesis (parser)
     cp_parser *parser;
{
  unsigned nesting_depth = 0;

  while (true)
    {
      cp_token *token;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If we've run out of tokens, then there is no closing `)'.  */
      if (token->type == CPP_EOF)
	return false;
      /* Consume the token.  */
      token = cp_lexer_consume_token (parser->lexer);
      /* If it is an `(', we have entered another level of nesting.  */
      if (token->type == CPP_OPEN_PAREN)
	++nesting_depth;
      /* If it is a `)', then we might be done.  */
      else if (token->type == CPP_CLOSE_PAREN
	       && nesting_depth-- == 0)
	return true;
    }
}
					    
/* Create a new C++ paser.  */

static cp_parser *
cp_parser_new ()
{
  cp_parser *parser;

  parser = (cp_parser *) xcalloc (1, sizeof (cp_parser));
  parser->lexer = cp_lexer_new (parse_in);
  VARRAY_UINT_INIT (parser->parsing_tentatively,
		    CP_PARSER_PARSING_TENATIVELY_STACK_SIZE,
		    "parsing_tentatively");
		
  /* For now, we always accept GNU extensions.  */
  parser->allow_gnu_extensions_p = 1;

  /* The `>' token is a greater-than operator, not the end of a
     template-id.  */
  parser->greater_than_is_operator_p = true;
  
  /* We are not parsing a constant-expression.  */
  parser->constant_expression_p = false;

  return parser;
}

/* Destroy the PARSER.  */

static void
cp_parser_delete (parser)
     cp_parser *parser;
{
  /* When we're done parsing, we should be in the midst of any
     tentative parsing.  */
  my_friendly_assert (!cp_parser_parsing_tentatively (parser),
		      20010712);

  cp_lexer_delete (parser->lexer);
  VARRAY_FREE (parser->parsing_tentatively);
  free (parser);
}

/* Lexical conventions [gram.lex]  */

/* Parse an identifier.  Returns an IDENTIFIER_NODE representing the
   identifier.  */

static tree 
cp_parser_identifier (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Look for the identifier.  */
  token = cp_parser_require (parser, CPP_NAME, "identifier");
  /* Return the value.  */
  return token ? token->value : error_mark_node;
}

/* Basic concepts [gram.basic]  */

/* Parse a translation-unit.

   translation-unit:
     declaration-seq [opt]  

   Returns TRUE if all went well.  */

static bool
cp_parser_translation_unit (parser)
     cp_parser *parser;
{
  while (true)
    {
      cp_parser_declaration_seq_opt (parser);

      /* If there are no tokens left then all went well.  */
      if (cp_lexer_peek_token (parser->lexer)->type == CPP_EOF)
	break;
      
      /* Otherwise, issue an error message.  */
      cp_parser_error (parser, "expected declaration");
      return false;
    }

  /* Consume the EOF token.  */
  cp_parser_require (parser, CPP_EOF, "end-of-file");
  
  /* Finish up.  */
  finish_translation_unit ();

  /* All went well.  */
  return true;
}

/* Expressions [gram.expr] */

/* Parse a primary-expression.

   primary-expression:
     literal
     this
     ( expression )
     id-expression

   GNU Extension:

     literal:
       __null

   Returns a representation of the expression.  For an id-expression
   which is an unqualified-id, returns an IDENTIFIER_NODE; name lookup
   is not done.  For a qualified-id, we return an SCOPE_REF.  */

static tree
cp_parser_primary_expression (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  switch (token->type)
    {
      /* literal:
	   integer-literal
	   character-literal
	   floating-literal
	   string-literal
	   boolean-literal  */
    case CPP_INT:
    case CPP_CHAR:
    case CPP_WCHAR:
    case CPP_STRING:
    case CPP_WSTRING:
    case CPP_FLOAT:
    case CPP_NUMBER:
      cp_lexer_consume_token (parser->lexer);
      return token->value;

    case CPP_OPEN_PAREN:
      {
	tree expr;
	bool saved_greater_than_is_operator_p;

	/* Consume the `('.  */
	cp_lexer_consume_token (parser->lexer);
	/* Within a parenthesized expression, a `>' token is always
	   the greater-than operator.  */
	saved_greater_than_is_operator_p 
	  = parser->greater_than_is_operator_p;
	parser->greater_than_is_operator_p = true;
	/* Parse the parenthesized expression.  */
	expr = cp_parser_expression (parser);
	/* The `>' token might be the end of a template-id or
	   template-parameter-list now.  */
	parser->greater_than_is_operator_p 
	  = saved_greater_than_is_operator_p;
	/* Consume the `)'.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

	return expr;
      }

    case CPP_KEYWORD:
      switch (token->keyword)
	{
	  /* These two are the boolean literals.  */
	case RID_TRUE:
	  cp_lexer_consume_token (parser->lexer);
	  return boolean_true_node;
	case RID_FALSE:
	  cp_lexer_consume_token (parser->lexer);
	  return boolean_false_node;
	  
	  /* The `__null' literal.  */
	  /* FIXME: Document this extension.  */
	case RID_NULL:
	  cp_lexer_consume_token (parser->lexer);
	  return null_node;

	  /* Recognize the `this' keyword.  */
	case RID_THIS:
	  cp_lexer_consume_token (parser->lexer);
	  return finish_this_expr ();

	  /* The `operator' keyword can be the beginning of an
	     id-expression.  */
	case RID_OPERATOR:
	  break;

	case RID_FUNCTION_NAME:
	case RID_PRETTY_FUNCTION_NAME:
	case RID_C99_FUNCTION_NAME:
	  /* The symbols __FUNCTION__, __PRETTY_FUNCTION__, and
	     __func__ are the names of variables -- but they are
	     treated specially.  Therefore, they are handled here,
	     rather than relying on the generic id-expression logic
	     below.  Gramatically, these names are id-expressions.  

	     Consume the token.  */
	  token = cp_lexer_consume_token (parser->lexer);
	  /* Look up the name.  */
	  return finish_fname (token->value);

	default:
	  cp_parser_error (parser, "expected primary-expression");
	  return error_mark_node;
	}
      /* Fall through. */

      /* An id-expression can start with either an identifier, or a
	 `::' as the beginning of a qualified-id.  */
    case CPP_NAME:
    case CPP_SCOPE:
      {
	tree id_expression;
	tree decl;

	/* Do name lookups in the current scope.  */
	parser->scope = NULL_TREE;
	/* Parse the id-expression.  */
	id_expression = cp_parser_id_expression (parser, 
						 /*assume_typename_p=*/false);
	/* If the name is an unqualified name, we might have to do
	   Koenig lookup if the identifier is used as the
	   postfix-expression in a function call.  So, in that case,
	   we do not look up the name until later.  */
	if (!parser->scope)
	  decl = id_expression;
	else
	  /* FIXME: Instead of building a SCOPE_REF, we should just
	     resolve this to a declaration immediately.  The old code
	     built an OFFSET_REF (with build_offset_ref) in this
	     situation, so we have to rework some other parts of the
	     compiler.  */
	  decl = build_nt (SCOPE_REF, parser->scope, id_expression);

	return decl;
      }

      /* Anything else is an error.  */
    default:
      cp_parser_error (parser, "expected primary-expression");
      return error_mark_node;
    }
}

/* Parse an id-expression.

   id-expression:
     unqualified-id
     qualified-id

   qualified-id:
     :: [opt] nested-name-specifier template [opt] unqualified-id
     :: identifier
     :: operator-function-id
     :: template-id

   Return a representation of the unqualified portion of the
   identifier.  Sets PARSER->SCOPE to the qualifying scope if there is
   a `::' or nested-name-specifier.  This routine assumes that the
   current PARER->SCOPE is the correct place to begin name-lookup.
   Therefore, if you explicitly need lookup to start from the current
   scope, you must clear PARSER->SCOPE yourself.

   Often, if the id-expression was a qualified-id, the caller will
   want to make a SCOPE_REF to represent the qualified-id.  This
   function does not do this in order to avoid wastefully creating
   SCOPE_REFs when they are not required.

   If ASSUME_TYPENAME_P is true then we assume that qualified names
   are typenames.  This flag is set when parsing a declarator-id;
   for something like:

     template <class T>
     int S<T>::R::i = 3;

   we are supposed to assume that `S<T>::R' is a class.

   FIXME: Perhaps the way we handle ASSUME_TYPENAME_P is wrong.
   Perhaps what we really want to do is look things up in the 
   unspecialized template.  Probably so.

   FIXME: Document the representation returned.  */

static tree
cp_parser_id_expression (parser, assume_typename_p)
     cp_parser *parser;
     bool assume_typename_p;
{
  bool global_scope_p;
  bool nested_name_specifier_p;

  /* Look for the optional `::' operator.  */
  global_scope_p 
    = (cp_parser_global_scope_opt (parser, /*current_scope_valid_p=*/true) 
       != NULL_TREE);
  /* Look for the optional nested-name-specifier.  */
  nested_name_specifier_p 
    = (cp_parser_nested_name_specifier_opt (parser,
					    assume_typename_p)
       != NULL_TREE);
  /* If there is a nested-name-specifier, then we are looking at
     the first qualified-id production.  */
  if (nested_name_specifier_p)
    {
      cp_token *token;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* See if it is the optional `template' keyword.  */
      if (token->keyword == RID_TEMPLATE)
	cp_lexer_consume_token (parser->lexer);
      /* FIXME: We should take the `template' keyword into account.  */

      /* Process the final unqualified-id.  */
      return cp_parser_unqualified_id (parser);
    }
  /* Otherwise, if we are in global scope, then we are looking at one
     of the other qualified-id productions.  */
  else if (global_scope_p)
    {
      cp_token *token;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);

      switch (token->type)
	{
	case CPP_NAME:
	  /* FIXME: Handle template-ids.  */
	  return cp_parser_identifier (parser);

	case CPP_KEYWORD:
	  if (token->keyword == RID_OPERATOR)
	    return cp_parser_operator_function_id (parser);
	  /* Fall through.  */
	  
	default:
	  cp_parser_error (parser, "expected id-expression");
	  return error_mark_node;
	}
    }
  else
    return cp_parser_unqualified_id (parser);
}

/* Parse an unqualified-id.

   unqualified-id:
     identifier
     operator-function-id
     conversion-function-id
     ~ class-name
     template-id

   Returns a representation of unqualified-id.  For the `identifier'
   production, an IDENTIFIER_NODE is returned.  For the `~ class-name'
   production a BIT_NOT_EXPR is returned; the argument to the
   BIT_NOT_EXPR is a TYPE_DECL indicating the class-name.  For the
   other productions, see the documentation accompanying the
   corresponding parsing functions.  */

static tree
cp_parser_unqualified_id (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  
  switch (token->type)
    {
    case CPP_NAME:
      {
	tree id;

	/* We don't know yet whether or not this will be a
	   template-id.  */
	cp_parser_parse_tentatively (parser);
	/* Try a template-id.  */
	/* FIXME: Are there ways to get here after just having seen
	   `template'?  */
	id = cp_parser_template_id (parser, 
				    /*template_keyword_p=*/false);
	/* If it worked, we're done.  */
	if (cp_parser_parse_definitely (parser))
	  return id;
	/* Otherwise, it's an ordinary identifier.  */
	return cp_parser_identifier (parser);
      }

    case CPP_COMPL:
      {
	tree type_decl;

	/* Consume the `~' token.  */
	cp_lexer_consume_token (parser->lexer);
	/* Parse the class-name.  The standard, as written, seems to
	   say that:

	     template <typename T> struct S { ~S (); };
	     template <typename T> S<T>::~S() {}

           is invalid, since `~' must be followed by a class-name, but
	   `S<T>' is dependent, and so not known to be a class.  This
	   is wrong, so we explicitly treat the qualified name as a
	   typename.  */
	type_decl = cp_parser_class_name (parser, 
					  /*typename_keyword_p=*/true,
					  /*template_keyword_p=*/false);
	if (type_decl == error_mark_node)
	  return error_mark_node;

	return build1 (BIT_NOT_EXPR, 
		       NULL_TREE, 
		       type_decl);
      }

    case CPP_KEYWORD:
      if (token->keyword == RID_OPERATOR)
	{
	  tree id;

	  /* We don't know whether we're looking at an
	     operator-function-id or a conversion-function-id.  */
	  cp_parser_parse_tentatively (parser);
	  /* Try an operator-function-id.  */
	  id = cp_parser_operator_function_id (parser);
	  /* If that didn't work, try a conversion-function-id.  */
	  if (!cp_parser_parse_definitely (parser))
	    id = cp_parser_conversion_function_id (parser);

	  return id;
	}
      /* Fall through.  */

    default:
      cp_parser_error (parser, "expected unqualified-id");
      return error_mark_node;
    }
}

/* Parse an (optional) nested-name-specifier.

   nested-name-specifier:
     class-or-namespace-name :: nested-name-specifier [opt]
     class-or-namespace-name :: template nested-name-specifier [opt]

   PARSER->SCOPE should be set appropriately before this function is
   called.  TYPENAME_KEYWORD_P is true if the `typename' keyword is in
   effect.

   Sets PARSER->SCOPE to the class (TYPE) or namespace
   (NAMESPACE_DECL) specified by the nested-name-specifier, or leaves
   it unchanged if there is no nested-name-specifier.  Returns the new
   scope iff there is a nested-name-specifier, or NULL_TREE otherwise.  */

static tree
cp_parser_nested_name_specifier_opt (parser, typename_keyword_p)
     cp_parser *parser;
     bool typename_keyword_p;
{
  bool success = false;
  bool template_keyword_p = false;

  while (true)
    {
      tree new_scope;
      tree old_scope;

      /* If the next token is not an identifier, then it is definitely
	 not a class-or-namespace-name.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_NAME))
	break;

      /* The nested-name-specifier is optional, so we parse
	 tentatively.  */
      cp_parser_parse_tentatively (parser);

      /* Save the old scope since the name lookup we are about to do
	 might destroy it.  */
      old_scope = parser->scope;
      /* Parse the qualifying entity.  */
      new_scope 
	= cp_parser_class_or_namespace_name (parser,
					     typename_keyword_p,
					     template_keyword_p);
      /* Look for the `::' token.  */
      cp_parser_require (parser, CPP_SCOPE, "`::'");

      /* If we found what we wanted, we keep going; otherwise, we're
	 done.  */
      if (!cp_parser_parse_definitely (parser))
	{
	  /* Restore the OLD_SCOPE since it was valid before the
	     failed attempt at finding the last
	     class-or-namespace-name.  */
	  parser->scope = old_scope;
	  break;
	}

      /* We've found one valid nested-name-specifier.  */
      success = true;
      /* Make sure we look in the right scope the next time through
	 the loop.  */
      parser->scope = (TREE_CODE (new_scope) == TYPE_DECL 
		       ? TREE_TYPE (new_scope)
		       : new_scope);

      /* Look for the optional `template' keyword.  */
      if (cp_lexer_next_token_is_keyword (parser->lexer, 
					  RID_TEMPLATE))
	{
	  /* Consume the `template' token.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Remember that we saw it.  */
	  template_keyword_p = true;
	}
      else
	template_keyword_p = false;
    }

  return success ? parser->scope : NULL_TREE;
}

/* Parse a nested-name-specifier.  See
   cp_parser_nested_name_specifier_opt for details.  This function
   behaves identically, except that it will an issue an error if no
   nested-name-specifier is present, and it will return
   ERROR_MARK_NODE, rather than NULL_TREE, if no nested-name-specifier
   is present.  */

static tree
cp_parser_nested_name_specifier (parser, typename_keyword_p)
     cp_parser *parser;
     bool typename_keyword_p;
{
  tree scope;

  /* Look for the nested-name-specifier.  */
  scope = cp_parser_nested_name_specifier_opt (parser,
					       typename_keyword_p);
  /* If it was not present, issue an error message.  */
  if (!scope)
    {
      cp_parser_error (parser, "expected nested-name-specifier");
      return error_mark_node;
    }

  return scope;
}

/* Parse a class-or-namespace-name.

   class-or-namespace-name:
     class-name
     namespace-name

   TYPENAME_KEYWORD_P is TRUE iff the `typename' keyword is in 
   effect.  TEMPLATE_KEYWORD_P iff the `template' keyword is in
   effect.

   Returns the class (TYPE_DECL) or namespace (NAMESPACE_DECL)
   specified by the class-or-namespace-name.  */

static tree
cp_parser_class_or_namespace_name (parser, 
				   typename_keyword_p,
				   template_keyword_p)
     cp_parser *parser;
     bool typename_keyword_p;
     bool template_keyword_p;
{
  tree saved_scope;
  tree scope;

  /* Before we try to parse the class-name, we must save away the
     current PARSER->SCOPE since cp_parser_class_name will destroy
     it.  */
  saved_scope = parser->scope;
  /* Try for a class-name first.  */
  cp_parser_parse_tentatively (parser);
  scope = cp_parser_class_name (parser, 
				typename_keyword_p,
				template_keyword_p);
  /* If that didn't work, try for a namespace-name.  */
  if (!cp_parser_parse_definitely (parser))
    {
      /* Restore the saved scope.  */
      parser->scope = saved_scope;
      /* Now look for a namespace-name.  */
      scope = cp_parser_namespace_name (parser);
    }

  return scope;
}

/* Parse a postfix-expression.

   postfix-expression:
     primary-expression
     postfix-expression [ expression ]
     postfix-expression ( expression-list [opt] )
     simple-type-specifier ( expression-list [opt] )
     typename :: [opt] nested-name-specifier identifier 
       ( expression-list [opt] )
     typename :: [opt] nested-name-specifier template [opt] template-id
       ( expression-list [opt] )
     postfix-expression . template [opt] id-expression
     postfix-expression -> template [opt] id-expression
     postfix-expression . pseudo-destructor-name
     postfix-expression -> pseudo-destructor-name
     postfix-expression ++
     postfix-expression --
     dynamic_cast < type-id > ( expression )
     static_cast < type-id > ( expression )
     reinterpret_cast < type-id > ( expression )
     const_cast < type-id > ( expression )
     typeid ( expression )
     typeid ( type-id )

   GNU Extension:
     
   postfix-expression:
     ( type-id ) { initializer-list , [opt] }

   This extension is a GNU version of the C99 compound-literal
   construct.  (The C99 grammar uses `type-name' instead of `type-id',
   but they are essentially the same concept.)

   Returns a representation of the expression.  */

static tree
cp_parser_postfix_expression (parser)
     cp_parser *parser;
{
  cp_token *token;
  enum rid keyword;
  cp_parser_id_kind idk = CP_PARSER_ID_KIND_NONE;
  tree postfix_expression = NULL_TREE;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Some of the productions are determined by keywords.  */
  keyword = token->keyword;
  switch (keyword)
    {
    case RID_DYNCAST:
    case RID_STATCAST:
    case RID_REINTCAST:
    case RID_CONSTCAST:
      {
	tree type;
	tree expression;

	/* All of these can be handled in the same way from the point
	   of view of parsing.  Begin by consuming the token
	   identifying the cast.  */
	cp_lexer_consume_token (parser->lexer);
	
	/* Parse the type to which we are casting.  */
	cp_parser_require (parser, CPP_LESS, "`<'");
	type = cp_parser_type_id (parser);
	cp_parser_require (parser, CPP_GREATER, "`>'");

	/* And the expression which is being cast.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	expression = cp_parser_expression (parser);
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

	switch (keyword)
	  {
	  case RID_DYNCAST:
	    postfix_expression
	      = build_dynamic_cast (type, expression);
	    break;
	  case RID_STATCAST:
	    postfix_expression
	      = build_static_cast (type, expression);
	    break;
	  case RID_REINTCAST:
	    postfix_expression
	      = build_reinterpret_cast (type, expression);
	    break;
	  case RID_CONSTCAST:
	    postfix_expression
	      = build_const_cast (type, expression);
	    break;
	  default:
	    my_friendly_abort (20000622);
	  }
      }
      break;

    case RID_TYPEID:
      {
	tree type;

	/* Consume the `typeid' token.  */
	cp_lexer_consume_token (parser->lexer);
	/* Look for the `(' token.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	/* We can't be sure yet whether we're looking at a type-id or an
	   expression.  */
	cp_parser_parse_tentatively (parser);
	/* Try a type-id first.  */
	type = cp_parser_type_id (parser);
	/* Look for the `)' token.  Otherwise, we can't be sure that
	   we're not looking at an expression: consider `typeid (int
	   (3))', for example.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	/* If all went well, simply lookup the type-id.  */
	if (cp_parser_parse_definitely (parser))
	  postfix_expression = get_typeid (TYPE_MAIN_VARIANT (type));
	/* Otherwise, fall back to the expression variant.  */
	else
	  {
	    tree expression;

	    /* Look for an expression.  */
	    expression = cp_parser_expression (parser);
	    /* Compute its typeid.  */
	    postfix_expression = build_typeid (expression);
	    /* Look for the `)' token.  */
	    cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	  }
      }
      break;
      
    case RID_TYPENAME:
      {
	bool template_p = false;
	tree id;
	tree type;

	/* Consume the `typename' token.  */
	cp_lexer_consume_token (parser->lexer);
	/* Look for the optional `::' operator.  */
	cp_parser_global_scope_opt (parser, 
				    /*current_scope_valid_p=*/false);
	/* Look for the nested-name-specifier.  */
	cp_parser_nested_name_specifier (parser,
					 /*typename_keyword_p=*/true);
	/* Look for the optional `template' keyword.  */
	if (cp_lexer_next_token_is_keyword (parser->lexer, 
					    RID_TEMPLATE))
	  {
	    /* Consume it.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Remember that we saw `template'.  */
	    template_p = true;
	  }
	/* We don't know whether we're looking at a template-id or an
	   identifier.  */
	cp_parser_parse_tentatively (parser);
	/* Try a template-id.  */
	id = cp_parser_template_id (parser, template_p);
	/* If that didn't work, try an identifier.  */
	if (!cp_parser_parse_definitely (parser))
	  id = cp_parser_identifier (parser);
	/* Create a TYPENAME_TYPE to represent the type to which the
	   functional cast is being performed.  */
	type = make_typename_type (parser->scope, id, 
				   /*complain=*/1);

	return cp_parser_functional_cast (parser, type);
      }

    default:
      {
	tree type;

	/* If the next thing is a simple-type-specifier, we may be
	   looking at a functional cast.  We could also be looking at
	   an id-expression.  So, we try the functional cast, and if
	   that doesn't work we fall back to the primary-expression.  */
	cp_parser_parse_tentatively (parser);
	/* Look for the simple-type-specifier.  */
	type = cp_parser_simple_type_specifier (parser, 
						CP_PARSER_FLAGS_NONE);
	/* Parse the cast itself.  */
	if (!cp_parser_error_occurred (parser))
	  postfix_expression 
	    = cp_parser_functional_cast (parser, type);
	/* If that worked, we're done.  */
	if (cp_parser_parse_definitely (parser))
	  break;

	/* If the functional-cast didn't work out, try a
	   compound-literal.  */
	if (cp_parser_allow_gnu_extensions_p (parser))
	  {
	    tree initializer_list = NULL_TREE;

	    cp_parser_parse_tentatively (parser);
	    /* Look for the `('.  */
	    if (cp_parser_require (parser, CPP_OPEN_PAREN, "`('"))
	      {
		type = cp_parser_type_id (parser);
		/* Look for the `)'.  */
		cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
		/* Look for the `{'.  */
		cp_parser_require (parser, CPP_OPEN_BRACE, "`{'");
		/* If things aren't going well, there's no need to
		   keep going.  */
		if (!cp_parser_error_occurred (parser))
		  {
		    /* Parse the initializer-list.  */
		    initializer_list 
		      = cp_parser_initializer_list (parser);
		    /* Allow a trailing `,'.  */
		    if (cp_lexer_next_token_is (parser->lexer, CPP_COMMA))
		      cp_lexer_consume_token (parser->lexer);
		    /* Look for the final `}'.  */
		    cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");
		  }
	      }
	    /* If that worked, we're definitely looking at a
	       compound-literal expression.  */
	    if (cp_parser_parse_definitely (parser))
	      {
		/* Warn the user that a compound literal is not
		   allowed in standard C++.  */
		if (pedantic)
		  cp_pedwarn ("ISO C++ forbids compound-literals");
		/* Form the representation of the compound-literal.  */
		postfix_expression = 
		  finish_compound_literal (type, initializer_list);
		break;
	      }
	  }

	/* It must be a primary-expression.  */
	postfix_expression 
	  = cp_parser_primary_expression (parser);
	/* If the primary-expression was an id-expression, the name
	   will not have been looked up at this point.  */
	if (TREE_CODE (postfix_expression) == IDENTIFIER_NODE)
	  {
	    tree decl;
	    
	    /* If this turns out to be the postfix-expression for a
	       function call, we will need to do Koenig lookup.  On
	       the other hand, in order to determine whether or not
	       this might be a function call, we must look up the
	       name:
	       
		 [basic.lookup.unqual]

		 For purposes of determining (during parsing) whether an
		 expression is a postfix-pression for a function call,
		 the usual name lookup rules apply.
	       
	       So, we look up the name here.  */
	    /* FIXME: Should cp_parser_id_expression be rejecting
	       identifiers that name types, namespaces, and such so
	       that we do not have to do it here?  */
	    decl = cp_parser_lookup_name (parser, postfix_expression);
	    
	    /* When name lookup fails inside a template, that just
	       means that we need to look up the name at
	       instantiation-time.  By resetting DECL to the
	       IDENTIFIER_NODE, we'll get a LOOKUP_EXPR when
	       cp_parser_maybe_lookup_name is called below.  */
	    /* FIXME: Will this work when we get an error for some
	       other reason, like an ambiguity in a class with
	       multiple inheritance?  What about:
	       
	       struct A { int i; }; 
	       struct B { int i; };
	       struct C : public A, public B {
	       template <typename T> 
	       void f() { i = 3; }
	       };
	       
	       ?  Perhaps we need to distinguish an ambiguous lookup
	       from no result at all.  */
	    if (decl == error_mark_node 
		&& current_template_parms)
	      decl = postfix_expression;
	    
	    /* If we didn't find anything, or what we found was a
	       type, then this wasn't really an id-expression after
	       all.  */
	    if (TREE_CODE (decl) == TYPE_DECL
		|| TREE_CODE (decl) == NAMESPACE_DECL
		|| (TREE_CODE (decl) == TEMPLATE_DECL
		    && !DECL_FUNCTION_TEMPLATE_P (decl))
		|| decl == error_mark_node)
	      {
		if (decl != error_mark_node)
		  cp_parser_error (parser, 
				   "expected primary-expression");
		return error_mark_node;
	      }
	    
	    /* Remember that we still have to look up this name for
	       real.  */
	    idk = CP_PARSER_ID_KIND_UNQUALIFIED;
	  }
	/* Remember that this was a qualified-name so that we will
	   know to look it up later.  */
	/* FIXME: Should we be checking for TYPE_DECLs and such as
	   above?  */
	else if (TREE_CODE (postfix_expression) == SCOPE_REF)
	  idk = CP_PARSER_ID_KIND_QUALIFIED;
      }
      break;
    }

  /* Keep looping until the postfix-expression is complete.  */
  while (true)
    {
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      
      switch (token->type)
	{
	case CPP_OPEN_SQUARE:
	  /* postfix-expression [ expression ] */
	  {
	    tree index;

	    /* Resolve the postfix-expression, if we have not
	       already.  */
	    cp_parser_maybe_lookup_name (parser,
					 &postfix_expression,
					 &idk);

	    /* Consume the `[' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Parse the index expression.  */
	    index = cp_parser_expression (parser);
	    /* Look for the closing `]'.  */
	    cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");

	    /* Build the ARRAY_REF.  */
	    postfix_expression 
	      = grok_array_decl (postfix_expression, index);
	  }
	  break;

	case CPP_OPEN_PAREN:
	  /* postfix-expression ( expression-list [opt] ) */
	  {
	    tree args;

	    /* Consume the `(' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Peek at the next token.  */
	    token = cp_lexer_peek_token (parser->lexer);
	    /* If it's not a `)', then there are some arguments.  */
	    if (token->type != CPP_CLOSE_PAREN)
	      args = cp_parser_expression_list (parser);
	    else
	      args = NULL_TREE;
	    /* Look for the closing `)'.  */
	    cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

	    /* If this is a call to a function that was specified as a
	       qualified-id, handle it specially.  The old parser had
	       a number of bizarre hacks for dealing with function
	       calls.  Until we rework build_x_function_call, we are
	       stuck with the old approach.  

	       The right thing to do is to resolve the name above (see
	       the FIXME after the handling of primary-expression
	       above), and then have finish_call_expr handle all the
	       semantic analysis required.  The parser's job should
	       only be to determine that this is a call.  

	       FIXME: Do as suggested.  */
	    if (TREE_CODE (postfix_expression) == SCOPE_REF)
	      postfix_expression 
		= finish_qualified_call_expr (postfix_expression, args);
	    else
	      postfix_expression 
		= finish_call_expr (postfix_expression, args, 
				    idk == CP_PARSER_ID_KIND_UNQUALIFIED);

	    /* The POSTFIX_EXPRESSION is certainly no longer an id.  */
	    idk = CP_PARSER_ID_KIND_NONE;
	  }
	  break;
	  
	case CPP_DOT:
	case CPP_DEREF:
	  /* postfix-expression . template [opt] id-expression  
	     postfix-expression . pseudo-destructor-name 
	     postfix-expression -> template [opt] id-expression
	     postfix-expression -> pseudo-destructor-name */
	  {
	    tree id_expression;
	    tree scope;

	    /* Resolve the postfix-expression, if we have not
	       already.  */
	    cp_parser_maybe_lookup_name (parser,
					 &postfix_expression,
					 &idk);

	    /* If this is a `->' operator, dereference the pointer.  */
	    if (token->type == CPP_DEREF)
	      postfix_expression = build_x_arrow (postfix_expression);
	    /* The identifier following the `->' or `.' is not
	       qualified.  */
	    parser->scope = NULL_TREE;
	    /* Enter the scope corresponding to the type of the object
	       given by the POSTFIX_EXPRESSION.  */
	    if (TREE_TYPE (postfix_expression) != error_mark_node)
	      {
		scope = TREE_TYPE (postfix_expression);
		/* According to the standard, no expression should
		   ever have reference type.  Unfortunately, we do not
		   currently match the standard in this respect in
		   that our internal representation of an expression
		   may have reference type even when the standard says
		   it does not.  Therefore, we have to manually obtain
		   the underlying type here.  */
		if (scope && TREE_CODE (scope) == REFERENCE_TYPE)
		  scope = TREE_TYPE (scope);
		/* If it's a class scope, enter the class so that
                   names are looked up in the class.  */
		if (scope && CLASS_TYPE_P (scope))
		  push_scope (scope);
	      }

	    /* Consume the `.' or `->' operator.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Try the id-expression production first.  */
	    cp_parser_parse_tentatively (parser);
	    /* Peek at the next token.  */
	    token = cp_lexer_peek_token (parser->lexer);
	    /* If it is the optional `template' keyword, consume
	       it.  */
	    /* FIXME: We should handle the `template' keyword
	       specially.  */
	    if (token->keyword == RID_TEMPLATE)
	      cp_lexer_consume_token (parser->lexer);
	    /* Parse the id-expression.  */
	    id_expression 
	      = cp_parser_id_expression (parser,
					 /*assume_typename_p=*/false);
	    /* Resolve the name, unless the postfix-expression is
	       type-dependent.  */
	    if (!cp_parser_type_dependent_expression_p 
		(postfix_expression))
	      {
		/* Destructor names must be handled specially.  */
		if (TREE_CODE (id_expression) == BIT_NOT_EXPR)
		  /* The call to build_x_component_ref will get us
		     the destructor.  */
		  ;
		else
		  id_expression = cp_parser_lookup_name (parser,
							 id_expression);
	      }

	    /* If that worked, we don't have to try the
	       pseudo-destructor-name production.  */
	    if (cp_parser_parse_definitely (parser))
	      /* Build a representation of the member.  */
	      postfix_expression
		= build_x_component_ref (postfix_expression,
					 id_expression,
					 /*basetype_path=*/NULL_TREE);
	    /* But, if something went wrong, try the
	       pseudo-destructor-name production.  */
	    else
	      {
		tree s;
		tree type;

		/* Parse the pseudo-destructor-name.  */
		cp_parser_pseudo_destructor_name (parser, &s, &type);
		/* Form the call.  */
		postfix_expression 
		  = finish_pseudo_destructor_expr (postfix_expression,
						   s, type);
	      }

	    /* Leave the SCOPE.  */
	    if (scope && CLASS_TYPE_P (scope))
	      pop_scope (scope);
	  }
	  break;

	case CPP_PLUS_PLUS:
	  /* postfix-expression ++  */
	  /* Resolve the postfix-expression, if we have not
	     already.  */
	  cp_parser_maybe_lookup_name (parser,
				       &postfix_expression,
				       &idk);
	  /* Consume the `++' token.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Generate a reprsentation for the complete expression.  */
	  postfix_expression 
	    = finish_increment_expr (postfix_expression, 
				     POSTINCREMENT_EXPR);
	  break;

	case CPP_MINUS_MINUS:
	  /* postfix-expression -- */
	  /* Resolve the postfix-expression, if we have not
	     already.  */
	  cp_parser_maybe_lookup_name (parser,
				       &postfix_expression,
				       &idk);
	  /* Consume the `--' token.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Generate a reprsentation for the complete expression.  */
	  postfix_expression 
	    = finish_increment_expr (postfix_expression, 
				     POSTDECREMENT_EXPR);
	  break;

	default:
	  /* If it's not one of these, then the postfix-expression is
	     complete.  Resolve the postfix-expression, if we have not
	     already.  */
	  cp_parser_maybe_lookup_name (parser,
				       &postfix_expression,
				       &idk);
	  return postfix_expression;
	}
    }

  /* We should never get here.  */
  my_friendly_abort (20010624);
  return error_mark_node;
}

/* Parse an expression-list.

   expression-list:
     assignment-expression
     expression-list, assignment-expression

   Returns a TREE_LIST.  The TREE_VALUE of each node is a
   representation of an assignment-expression.  Note that a TREE_LIST
   is returned even if there is only a single expression in the list.  */

static tree
cp_parser_expression_list (parser)
     cp_parser *parser;
{
  tree expression_list = NULL_TREE;

  /* Consume expressions until there are no more.  */
  while (true)
    {
      cp_token *token;
      tree expr;

      /* Parse the next assignment-expression.  */
      expr = cp_parser_assignment_expression (parser);
      /* Add it to the list.  */
      expression_list = tree_cons (NULL_TREE, expr, expression_list);

      /* If the next token isn't a `,', then we are done.  */
      token = cp_lexer_peek_token (parser->lexer);
      if (token->type != CPP_COMMA)
	break;

      /* Otherwise, consume the `,' and keep going.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* We built up the list in reverse order so we must reverse it now.  */
  return nreverse (expression_list);
}

/* Parse a pseudo-destructor-name.

   pseudo-destructor-name:
     :: [opt] nested-name-specifier [opt] type-name :: ~ type-name
     :: [opt] nested-name-specifier template temlate-id :: ~ type-name
     :: [opt] nested-name-specifier [opt] ~ type-name

   If either of the first two productions is used, sets *SCOPE to the
   TYPE specified before the final `::'.  Otherwise, *SCOPE is set to
   NULL_TREE.  *TYPE is set to the TYPE_DECL for the final type-name.  */

static void
cp_parser_pseudo_destructor_name (parser, scope, type)
     cp_parser *parser;
     tree *scope;
     tree *type;
{
  cp_token *token;
  bool nested_name_specifier_p;

  /* Look for the optional `::' operator.  */
  cp_parser_global_scope_opt (parser, /*current_scope_valid_p=*/true);
  /* Look for the optional nested-name-specifier.  */
  nested_name_specifier_p 
    = (cp_parser_nested_name_specifier_opt (parser,
					    /*typename_keyword_p=*/false) 
       != NULL_TREE);
  /* Now, if we saw a nested-name-specifier, we might be doing the
     second production.  */
  if (nested_name_specifier_p)
    {
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* Check to see if it is `template'.  */
      if (token->type == RID_TEMPLATE)
	{
	  /* Consume the `template' keyword.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Parse the template-id.  */
	  /* FIXME: Implement this.  */
	  cp_parser_unimplemented ();
	}
    }

  /* If the next token is not a `~', then there might be some
     additional qualification. */
  if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMPL))
    {
      /* Look for the type-name.  */
      *scope = TREE_TYPE (cp_parser_type_name (parser));
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* Look for the `::' token.  */
      cp_parser_require (parser, CPP_SCOPE, "`::'");
    }
  else
    *scope = NULL_TREE;

  /* Look for the `~'.  */
  cp_parser_require (parser, CPP_COMPL, "`~'");
  /* Look for the type-name again.  We are not responsible for
     checking that it matches the first type-name.  */
  *type = cp_parser_type_name (parser);
}

/* Parse a unary-expression.

   unary-expression:
     postfix-expression
     ++ cast-expression
     -- cast-expression
     unary-operator cast-expression
     sizeof unary-expression
     sizeof ( type-id )
     new-expression
     delete-expression

   GNU Extensions:

     __extension__ cast-expression
     __alignof__ unary-expression
     __alignof__ ( type-id )
     __real__ cast-expression
     __imag__ cast-expression

   Returns a representation of the expresion.  */

static tree
cp_parser_unary_expression (parser)
     cp_parser *parser;
{
  cp_token *token;
  enum tree_code unary_operator;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Some keywords give away the kind of expression.  */
  if (token->type == CPP_KEYWORD)
    {
      enum rid keyword = token->keyword;

      switch (keyword)
	{
	case RID_SIZEOF:
	case RID_ALIGNOF:
	  {
	    tree expr;
	    
	    /* Consume the `sizeof' or `alignof' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Peek at the next token.  */
	    token = cp_lexer_peek_token (parser->lexer);
	    /* If it's a `(', then we might be looking at the
	       type-specifier construction.  */
	    if (token->type == CPP_OPEN_PAREN)
	      {
		tree type;

		/* We can't be sure yet whether we're looking at a
		   type-specifier or an expression.  */
		cp_parser_parse_tentatively (parser);
		/* Consume the `('.  */
		cp_lexer_consume_token (parser->lexer);
		/* Parse the type-specifier.  */
		type = cp_parser_type_id (parser);
		/* Now, look for the trailing `)'.  */
		cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
		/* If all went well, then we're done.  */
		if (cp_parser_parse_definitely (parser))
		  {
		    /* Build a list of decl-specifiers; right now, we
		       have only a single type-specifier.  */
		    type = build_tree_list (NULL_TREE,
					    type);

		    /* Call grokdeclarator to figure out what type
		       this is.  */
		    type = grokdeclarator (NULL_TREE,
					   type,
					   TYPENAME,
					   /*initialized=*/0,
					   /*attrlist=*/NULL_TREE);
		    return (keyword == RID_SIZEOF
			    ? c_sizeof (type)
			    : c_alignof (type));
		  }
	      }

	    /* Parse the unary-expression.  */
	    expr = cp_parser_unary_expression (parser);
	    return (keyword == RID_SIZEOF
		    ? expr_sizeof (expr)
		    : grok_alignof (expr));
	  }

	  /* FIXME: Handle new-expressions and delete-expressions that
	     begin with a `::'.  */
	case RID_NEW:
	  return cp_parser_new_expression (parser);

	case RID_DELETE:
	  return cp_parser_delete_expression (parser);
	  
	case RID_EXTENSION:
	  {
	    /* The saved value of the PEDANTIC flag.  */
	    int saved_pedantic;
	    tree expr;

	    /* Save away the PEDANTIC flag.  */
	    cp_parser_extension_opt (parser, &saved_pedantic);
	    /* Parse the cast-expression.  */
	    expr = cp_parser_cast_expression (parser);
	    /* Restore the PEDANTIC flag.  */
	    pedantic = saved_pedantic;

	    return expr;
	  }

	case RID_REALPART:
	case RID_IMAGPART:
	  {
	    tree expression;

	    /* Consume the `__real__' or `__imag__' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Parse the cast-expression.  */
	    expression = cp_parser_cast_expression (parser);
	    /* Create the complete representation.  */
	    return build_x_unary_op ((token == RID_REALPART
				      ? REALPART_EXPR : IMAGPART_EXPR),
				     expression);
	  }
	  break;

	default:
	  break;
	}
    }

  /* Look for a unary operator.  */
  unary_operator = cp_parser_unary_operator (token);
  /* The `++' and `--' operators can be handled similarly, even though
     they are not technically unary-operators in the grammar.  */
  if (unary_operator == ERROR_MARK)
    {
      if (token->type == CPP_PLUS_PLUS)
	unary_operator = PREINCREMENT_EXPR;
      else if (token->type == CPP_MINUS_MINUS)
	unary_operator = PREDECREMENT_EXPR;
    }
  if (unary_operator != ERROR_MARK)
    {
      tree cast_expression;

      /* Consume the operator token.  */
      token = cp_lexer_consume_token (parser->lexer);
      /* Parse the cast-expression.  */
      cast_expression = cp_parser_cast_expression (parser);
      /* Now, build an appropriate representation.  */
      switch (unary_operator)
	{
	case INDIRECT_REF:
	  return build_x_indirect_ref (cast_expression, "unary *");
	  
	case ADDR_EXPR:
	  return build_x_unary_op (ADDR_EXPR, cast_expression);
	  
	case CONVERT_EXPR:
	case NEGATE_EXPR:
	case TRUTH_NOT_EXPR:
	case PREINCREMENT_EXPR:
	case PREDECREMENT_EXPR:
	  return finish_unary_op_expr (unary_operator, cast_expression);

	case BIT_NOT_EXPR:
	  return build_x_unary_op (BIT_NOT_EXPR, cast_expression);

	default:
	  my_friendly_abort (20010417);
	  return error_mark_node;
	}
    }

  return cp_parser_postfix_expression (parser);
}

/* Returns ERROR_MARK if TOKEN is not a unary-operator.  If TOKEN is a
   unary-operator, the corresponding tree code is returned.  */

static enum tree_code
cp_parser_unary_operator (token)
     cp_token *token;
{
  switch (token->type)
    {
    case CPP_MULT:
      return INDIRECT_REF;

    case CPP_AND:
      return ADDR_EXPR;

    case CPP_PLUS:
      return CONVERT_EXPR;

    case CPP_MINUS:
      return NEGATE_EXPR;

    case CPP_NOT:
      return TRUTH_NOT_EXPR;
      
    case CPP_COMPL:
      return BIT_NOT_EXPR;

    default:
      return ERROR_MARK;
    }
}

/* Parse a new-expression.

     :: [opt] new new-placement [opt] new-type-id new-initializer [opt]
     :: [opt] new new-placement [opt] ( type-id ) new-initializer [opt]

   Returns a representation of the expression.  */

static tree
cp_parser_new_expression (parser)
     cp_parser *parser;
{
  bool global_scope_p;
  tree placement;
  tree type;
  tree initializer;

  /* Look for the optional `::' operator.  */
  global_scope_p 
    = (cp_parser_global_scope_opt (parser,
				   /*current_scope_valid_p=*/false)
       != NULL_TREE);
  /* Look for the `new' operator.  */
  cp_parser_require_keyword (parser, RID_NEW, "`new'");
  /* There's no easy way to tell a new-placement from the
     `( type-id )' construct.  */
  cp_parser_parse_tentatively (parser);
  /* Look for a new-placement.  */
  placement = cp_parser_new_placement (parser);
  /* If that didn't work out, there's no new-placement.  */
  if (!cp_parser_parse_definitely (parser))
    placement = NULL_TREE;

  /* If the next token is a `(', then we have a parenthesized
     type-id.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_OPEN_PAREN))
    {
      /* Consume the `('.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the type-id.  */
      type = cp_parser_type_id (parser);
      /* Look for the closing `)'.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
    }
  /* Otherwise, there must be a new-type-id.  */
  else
    type = cp_parser_new_type_id (parser);

  /* If the next token is a `(', then we have a new-initializer.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_OPEN_PAREN))
    initializer = cp_parser_new_initializer (parser);
  else
    initializer = NULL_TREE;

  /* Create a representation of the new-expression.  */
  return build_new (placement, type, initializer, global_scope_p);
}

/* Parse a new-placement.

   new-placement:
     ( expression-list )

   Returns the same representation as for an expression-list.  */

static tree
cp_parser_new_placement (parser)
     cp_parser *parser;
{
  tree expression_list;

  /* Look for the opening `('.  */
  if (!cp_parser_require (parser, CPP_OPEN_PAREN, "`('"))
    return error_mark_node;
  /* Parse the expression-list.  */
  expression_list = cp_parser_expression_list (parser);
  /* Look for the closing `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  return expression_list;
}

/* Parse a new-type-id.

   new-type-id:
     type-specifier-seq new-declarator [opt]

   Returns the TYPE specified.  */

static tree
cp_parser_new_type_id (parser)
     cp_parser *parser;
{
  tree type_specifier_seq;
  tree declarator;

  /* Parse the type-specifier-seq.  */
  type_specifier_seq = cp_parser_type_specifier_seq (parser);
  /* Parse the new-declarator.  */
  declarator = cp_parser_new_declarator_opt (parser);

  return grokdeclarator (declarator, type_specifier_seq, TYPENAME,
			 /*initialized=*/0, /*attrlist=*/NULL_TREE);
}

/* Parse an (optional) new-declarator.

   new-declarator:
     ptr-operator new-declarator [opt]
     direct-new-declarator

   Returns a representation of the declarator.  See
   cp_parser_declarator for the representations used.  */

static tree
cp_parser_new_declarator_opt (parser)
     cp_parser *parser;
{
  enum tree_code code;
  tree type;
  tree cv_qualifier_seq;

  /* We don't know if there's a ptr-operator next, or not.  */
  cp_parser_parse_tentatively (parser);
  /* Look for a ptr-operator.  */
  code = cp_parser_ptr_operator (parser, &type, &cv_qualifier_seq);
  /* If that worked, look for more new-declarators.  */
  if (cp_parser_parse_definitely (parser))
    {
      tree declarator;

      /* FIXME: Handle pointers-to-members.  */
      if (type)
	cp_parser_unimplemented ();

      /* Parse another optional declarator.  */
      declarator = cp_parser_new_declarator_opt (parser);

      /* FIXME: Factor this code.  */
      /* Create the representation of the declarator.  */
      if (code == INDIRECT_REF)
	return make_pointer_declarator (declarator, 
					cv_qualifier_seq);
      else
	return make_reference_declarator (declarator, 
					  cv_qualifier_seq);
    }

  /* If the next token is a `[', there is a direct-new-declarator.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_OPEN_SQUARE))
    return cp_parser_direct_new_declarator (parser);

  return NULL_TREE;
}

/* Parse a direct-new-declarator.

   direct-new-declarator:
     [ expression ]
     direct-new-declarator [constant-expression]  

   Returns an ARRAY_REF, following the same conventions as are
   documented for cp_parser_direct_declarator.  */

static tree
cp_parser_direct_new_declarator (parser)
     cp_parser *parser;
{
  tree declarator = NULL_TREE;

  while (true)
    {
      tree expression;

      /* Look for the opening `['.  */
      cp_parser_require (parser, CPP_OPEN_SQUARE, "`['");
      /* The first expression is not required to be constant.  */
      if (!declarator)
	expression = cp_parser_expression (parser);
      /* But all the other expressions must be.  */
      else
	expression = cp_parser_constant_expression (parser);
      /* Look for the closing `]'.  */
      cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");

      /* Add this bound to the declarator.  */
      declarator = build_nt (ARRAY_REF, declarator, expression);

      /* If the next token is not a `[', then there are no more
	 bounds.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_OPEN_SQUARE))
	break;
    }

  return declarator;
}

/* Parse a new-initializer.

   new-initializer:
     ( expression-list [opt] )

   Returns a reprsentation of the expression-list.  If there is no
   expression-list, VOID_ZERO_NODE is returned.  */

static tree
cp_parser_new_initializer (parser)
     cp_parser *parser;
{
  tree expression_list;

  /* Look for the opening parenthesis.  */
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
  /* If the next token is not a `)', then there is an
     expression-list.  */
  if (cp_lexer_next_token_is_not (parser->lexer, CPP_CLOSE_PAREN))
    expression_list = cp_parser_expression_list (parser);
  else
    expression_list = void_zero_node;
  /* Look for the closing parenthesis.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  return expression_list;
}

/* Parse a delete-expression.

   delete-expression:
     :: [opt] delete cast-expression
     :: [opt] delete [ ] cast-expression

   Returns a representation of the expression.  */

static tree
cp_parser_delete_expression (parser)
     cp_parser *parser;
{
  bool global_scope_p;
  bool array_p;
  tree expression;

  /* Look for the optional `::' operator.  */
  global_scope_p
    = (cp_parser_global_scope_opt (parser,
				   /*current_scope_valid_p=*/false)
       != NULL_TREE);
  /* Look for the `delete' keyword.  */
  cp_parser_require_keyword (parser, RID_DELETE, "`delete'");
  /* See if the array syntax is in use.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_OPEN_SQUARE))
    {
      /* Consume the `[' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Look for the `]' token.  */
      cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");
      /* Remember that this is the `[]' construct.  */
      array_p = true;
    }
  else
    array_p = false;

  /* Parse the cast-expression.  */
  expression = cp_parser_cast_expression (parser);

  return delete_sanity (expression, NULL_TREE, array_p, global_scope_p);
}

/* Parse a cast-expression.

   cast-expression:
     unary-expression
     ( type-id ) cast-expression

   Returns a representation of the expression.  */

static tree
cp_parser_cast_expression (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's a `(', then we might be looking at a cast.  */
  if (token->type == CPP_OPEN_PAREN)
    {
      tree type;
      bool compound_literal_p;

      /* There's no way to know yet whether or not this is a cast.
	 For example, `(int (3))' is a unary-expression, while `(int)
	 3' is a cast.  So, we resort to parsing tentatively.  */
      cp_parser_parse_tentatively (parser);
      /* Consume the `('.  */
      cp_lexer_consume_token (parser->lexer);
      /* A very tricky bit is that `(struct S) { 3 }' is a
	 compound-literal (which we permit in C++ as an extension).
	 But, that construct is not a cast-expression -- it is a
	 postfix-expression.  (The reason is that `(struct S) { 3 }.i'
	 is legal; if the compound-literal were a cast-expression,
	 you'd need an extra set of parentheses.)  But, if we parse
	 the type-id, and it happens to be a class-specifier, then we
	 will commit to the parse at that point, because we cannot
	 undo the action that is done when creating a new class.  So,
	 then we cannot back up and do a postfix-expression.  

	 Therefore, we scan ahead to the closing `)', and check to see
	 if the token after the `)' is a `{'.  If so, we are not
	 looking at a cast-expression.  

	 Save tokens so that we can put them back.  */
      cp_lexer_save_tokens (parser->lexer);
      /* Skip tokens until the next token is a closing parenthesis.
	 If we find the closing `)', and the next token is a `{', then
	 we are looking at a compound-literal.  */
      compound_literal_p 
	= (cp_parser_skip_to_closing_parenthesis (parser)
	   && cp_lexer_next_token_is (parser->lexer, CPP_OPEN_BRACE));
      /* Roll back the tokens we skipped.  */
      cp_lexer_rollback_tokens (parser->lexer);
      /* If we were looking at a compound-literal, simulate an error
	 so that the call to cp_parser_parse_definitely below will
	 fail.  */
      if (compound_literal_p)
	cp_parser_simulate_error (parser);
      else
	{
	  /* Look for the type-id.  */
	  type = cp_parser_type_id (parser);
	  /* Look for the closing `)'.  */
	  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	}

      /* If all went well, this is a cast.  */
      if (cp_parser_parse_definitely (parser))
	{
	  tree expr;
	  
	  /* Parse the dependent expression.  */
	  expr = cp_parser_cast_expression (parser);
	  /* Perform the cast.  */
	  return build_c_cast (type, expr);
	}
    }
  
  /* If we get here, then it's not a cast, so it must be a
     unary-expression.  */
  return cp_parser_unary_expression (parser);
}

/* Parse a pm-expression.

   pm-expression:
     cast-expression
     pm-expression .* cast-expression
     pm-expression ->* cast-expression

     Returns a representation of the expression.  */

static tree
cp_parser_pm_expression (parser)
     cp_parser *parser;
{
  tree cast_expr;
  tree pm_expr;

  /* Parse the cast-expresion.  */
  cast_expr = cp_parser_cast_expression (parser);
  pm_expr = cast_expr;
  /* Now look for pointer-to-member operators.  */
  while (true)
    {
      cp_token *token;
      enum cpp_ttype token_type;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      token_type = token->type;
      /* If it's not `.*' or `->*' there's no pointer-to-member
	 operation.  */
      if (token_type != CPP_DOT_STAR 
	  && token_type != CPP_DEREF_STAR)
	break;

      /* Consume the token.  */
      cp_lexer_consume_token (parser->lexer);

      /* Parse another cast-expression.  */
      cast_expr = cp_parser_cast_expression (parser);
      
      /* Build the representation of the pointer-to-member 
	 operation.  */
      if (token_type == CPP_DEREF_STAR)
	pm_expr = build_x_binary_op (MEMBER_REF, pm_expr, cast_expr);
      else
	pm_expr = build_m_component_ref (pm_expr, cast_expr);
    }

  return pm_expr;
}

/* Parse a multiplicative-expression.

   mulitplicative-expression:
     pm-expression
     multiplicative-expression * pm-expression
     multiplicative-expression / pm-expression
     multiplicative-expression % pm-expression

   Returns a representation of the expression.  */

static tree
cp_parser_multiplicative_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_MULT, MULT_EXPR },
    { CPP_DIV, TRUNC_DIV_EXPR },
    { CPP_MOD, TRUNC_MOD_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_pm_expression);
}

/* Parse an additive-expression.

   additive-expression:
     multiplicative-expression
     additive-expression + multiplicative-expression
     additive-expression - multiplicative-expression

   Returns a representation of the expression.  */

static tree
cp_parser_additive_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_PLUS, PLUS_EXPR },
    { CPP_MINUS, MINUS_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_multiplicative_expression);
}

/* Parse a shift-expression.

   shift-expression:
     additive-expression
     shift-expression << additive-expression
     shift-expression >> additive-expression

   Returns a representation of the expression.  */

static tree
cp_parser_shift_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_LSHIFT, LSHIFT_EXPR },
    { CPP_RSHIFT, RSHIFT_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_additive_expression);
}

/* Parse a relational-expression.

   relational-expression:
     shift-expression
     relational-expression < shift-expression
     relational-expression > shift-expression
     relational-expression <= shift-expression
     relational-expression >= shift-expression

   Returns a representation of the expression.  */

static tree
cp_parser_relational_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_LESS, LT_EXPR },
    { CPP_GREATER, GT_EXPR },
    { CPP_LESS_EQ, LE_EXPR },
    { CPP_GREATER_EQ, GE_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_shift_expression);
}

/* Parse an equality-expression.

   equality-expression:
     relational-expression
     equality-expression == relational-expression
     equality-expression != relational-expression

   Returns a representation of the expression.  */

static tree
cp_parser_equality_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_EQ_EQ, EQ_EXPR },
    { CPP_NOT_EQ, NE_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_relational_expression);
}

/* Parse an and-expression.

   and-expression:
     equality-expression
     and-expression & equality-expression

   Returns a representation of the expression.  */

static tree
cp_parser_and_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_AND, BIT_AND_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_equality_expression);
}

/* Parse an exclusive-or-expression.

   exclusive-or-expression:
     and-expression
     exclusive-or-expression ^ and-expression

   Returns a representation of the expression.  */

static tree
cp_parser_exclusive_or_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_XOR, BIT_XOR_EXPR },
    { CPP_EOF, ERROR_MARK }
  };
  
  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_and_expression);
}


/* Parse an inclusive-or-expression.

   inclusive-or-expression:
     exclusive-or-expression
     inclusive-or-expression | exclusive-or-expression

   Returns a representation of the expression.  */

static tree
cp_parser_inclusive_or_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_OR, BIT_IOR_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_exclusive_or_expression);
}

/* Parse a logical-and-expression.

   logical-and-expression:
     inclusive-or-expression
     logical-and-expression && inclusive-or-expression

   Returns a representation of the expression.  */

static tree
cp_parser_logical_and_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_AND_AND, TRUTH_ANDIF_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_inclusive_or_expression);
}

/* Parse a logical-or-expression.

   logical-or-expression:
     logical-and-expresion
     logical-or-expression || logical-and-expression

   Returns a representation of the expression.  */

static tree
cp_parser_logical_or_expression (parser)
     cp_parser *parser;
{
  static cp_parser_token_tree_map map = {
    { CPP_OR_OR, TRUTH_ORIF_EXPR },
    { CPP_EOF, ERROR_MARK }
  };

  return cp_parser_binary_expression (parser,
				      map,
				      cp_parser_logical_and_expression);
}

/* Parse a conditional-expression.

   conditional-expression:
     logical-or-expression
     logical-or-expression ? expression : assignment-expression  

   Returns a representation of the expression.  */

static tree
cp_parser_conditional_expression (parser)
     cp_parser *parser;
{
  tree logical_or_expr;
  cp_token *token;

  /* Parse the logical-or-expression.  */
  logical_or_expr = cp_parser_logical_or_expression (parser);
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the next token is a `?', then we have a real conditional
     expression.  */
  if (token->type == CPP_QUERY)
    return cp_parser_question_colon_clause (parser, logical_or_expr);
  /* Otherwise, the value is simply the logical-or-expression.  */
  else
    return logical_or_expr;
}

/* Parse the `? expression : assignment-expression' part of a
   conditional-expression.  The LOGICAL_OR_EXPR is the
   logical-or-expression that started the conditional-expression.
   Returns a representation of the entire conditional-expression.

   This routine exists only so that it can be shared between
   cp_parser_conditional_expression and
   cp_parser_assignment_expression.  */

static tree
cp_parser_question_colon_clause (parser, logical_or_expr)
     cp_parser *parser;
     tree logical_or_expr;
{
  tree expr;
  tree assignment_expr;

  /* Consume the `?' token.  */
  cp_lexer_consume_token (parser->lexer);
  /* Parse the expression.  */
  expr = cp_parser_expression (parser);
  /* The next token should be a `:'.  */
  cp_parser_require (parser, CPP_COLON, "`:'");
  /* Parse the assignment-expression.  */
  assignment_expr = cp_parser_assignment_expression (parser);

  /* Build the conditional-expression.  */
  return build_x_conditional_expr (logical_or_expr,
				   expr,
				   assignment_expr);
}

/* Parse an assignment-expression.

   assignment-expression:
     conditional-expression
     logical-or-expression assignment-operator assignment_expression
     throw-expression

   Returns a representation for the expression.  */

static tree
cp_parser_assignment_expression (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree expr;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the next token is the `throw' keyword, then we're looking at
     a throw-expression.  */
  if (cp_parser_is_keyword (token, RID_THROW))
    expr = cp_parser_throw_expression (parser);
  /* Otherwise, it must be that we are looking at a
     logical-or-expression.  */
  else
    {
      /* Parse the logical-or-expression.  */
      expr = cp_parser_logical_or_expression (parser);
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's a `?' then we're actually looking at a
         conditional-expression.  */
      if (token->type == CPP_QUERY)
	return cp_parser_question_colon_clause (parser, expr);
      else 
	{
	  enum tree_code assignment_operator;

	  /* If it's an assignment-operator, we're using the second
	     production.  */
	  assignment_operator 
	    = cp_parser_assignment_operator_opt (parser);
	  if (assignment_operator != ERROR_MARK)
	    {
	      tree rhs;
	      
	      /* Parse the right-hand side of the assignment.  */
	      rhs = cp_parser_assignment_expression (parser);
	      /* Build the asignment expression.  */
	      expr = build_x_modify_expr (expr, 
					  assignment_operator, 
					  rhs);
	    }
	}
    }

  return expr;
}

/* Parse an (optional) assignment-operator.

   assignment-operator: one of 
     = *= /= %= += -= >>= <<= &= ^= |=  

   If the next token is an assignment operator, the corresponding tree
   code is returned, and the token is consumed.  For example, for
   `+=', PLUS_EXPR is returned.  For `=' itself, the code returned is
   NOP_EXPR.  For `/', TRUNC_DIV_EXPR is returned; for `%',
   TRUNC_MOD_EXPR is returned.  If TOKEN is not an assignment
   operator, ERROR_MARK is returned.  */

static enum tree_code
cp_parser_assignment_operator_opt (parser)
     cp_parser *parser;
{
  enum tree_code op;
  cp_token *token;

  /* Peek at the next toen.  */
  token = cp_lexer_peek_token (parser->lexer);

  switch (token->type)
    {
    case CPP_EQ:
      op = NOP_EXPR;
      break;

    case CPP_MULT_EQ:
      op = MULT_EXPR;
      break;

    case CPP_DIV_EQ:
      op = TRUNC_DIV_EXPR;
      break;

    case CPP_MOD_EQ:
      op = TRUNC_MOD_EXPR;
      break;

    case CPP_PLUS_EQ:
      op = PLUS_EXPR;
      break;

    case CPP_MINUS_EQ:
      op = MINUS_EXPR;
      break;

    case CPP_RSHIFT_EQ:
      op = RSHIFT_EXPR;
      break;

    case CPP_LSHIFT_EQ:
      op = LSHIFT_EXPR;
      break;

    case CPP_AND_EQ:
      op = BIT_AND_EXPR;
      break;

    case CPP_XOR_EQ:
      op = BIT_XOR_EXPR;
      break;

    case CPP_OR_EQ:
      op = BIT_IOR_EXPR;
      break;

    default: 
      /* Nothing else is an assignment operator.  */
      op = ERROR_MARK;
    }

  /* If it was an assignment operator, consume it.  */
  if (op != ERROR_MARK)
    cp_lexer_consume_token (parser->lexer);

  return op;
}

/* Parse an expression.

   expression:
     assignment-expression
     expression , assignment-expression

   Returns a representation of the expression.  */

static tree
cp_parser_expression (parser)
     cp_parser *parser;
{
  tree expression = NULL_TREE;
  bool saw_comma_p = false;

  while (true)
    {
      tree assignment_expression;
      
      /* Parse the next assignment-expression.  */
      assignment_expression 
	= cp_parser_assignment_expression (parser);
      /* If this is the first assignment-expression, we can just
	 save it away.  */
      if (!expression)
	expression = assignment_expression;
      /* Otherwise, chain the expressions together.  It is unclear why
	 we do not simply build COMPOUND_EXPRs as we go.  */
      else
	expression = tree_cons (NULL_TREE, 
				assignment_expression,
				expression);
      /* If the next token is not a comma, then we are done with the
	 expression.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA))
	break;
      /* Consume the `,'.  */
      cp_lexer_consume_token (parser->lexer);
      /* The first time we see a `,', we must take special action
	 because the representation used for a single expression is
	 different from that used for a list containing the single
	 expression.  */
      if (!saw_comma_p)
	{
	  /* Remember that this expression has a `,' in it.  */
	  saw_comma_p = true;
	  /* Turn the EXPRESSION into a TREE_LIST so that we can link
	     additional expressions to it.  */
	  expression = build_tree_list (NULL_TREE, expression);
	}
    }

  /* Build a COMPOUND_EXPR to represent the entire expression, if
     necessary.  We built up the list in reverse order, so we must
     straighten it out here.  */
  if (saw_comma_p)
    expression = build_x_compound_expr (nreverse (expression));

  return expression;
}

/* Parse a constant-expression. 

   constant-expression:
     conditional-expression  */

static tree
cp_parser_constant_expression (parser)
     cp_parser *parser;
{
  bool saved_constant_expression_p;
  tree expression;

  /* It might seem that we could simply parse the
     conditional-expression, and then check to see if it were
     TREE_CONSTANT.  However, an expression that is TREE_CONSTANT is
     one that the compiler can figure out is constant, possibly after
     doing some simplifications or optimizations.  The standard has a
     precise definition of constant-expression, and we must honor
     that, even though it is somewhat more restrictive.

     For example:

       int i[(2, 3)];

     is not a legal declaration, because `(2, 3)' is not a
     constant-expression.  The `,' operator is forbidden in a
     constant-expression.  However, GCC's constant-folding machinery
     will fold this operation to an INTEGER_CST for `3'.  */

  /* Save the old setting of CONSTANT_EXPRESSION_P.  */
  saved_constant_expression_p = parser->constant_expression_p;
  /* We are now parsing a constant-expression.  */
  parser->constant_expression_p = true;
  /* Parse the conditional-expression.  */
  expression = cp_parser_conditional_expression (parser);
  /* Restore the old setting of CONSTANT_EXPRESSION_P.  */
  parser->constant_expression_p = saved_constant_expression_p;

  return expression;
}

/* Statements [gram.stmt.stmt]  */

/* Parse a statement.  

   statement:
     labeled-statement
     expression-statement
     compound-statement
     selection-statement
     iteration-statement
     jump-statement
     declaration-statement
     try-block  */

static tree
cp_parser_statement (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If this is a keyword, then that will often determine what kind of
     statement we have.  */
  if (token->type == CPP_KEYWORD)
    {
      enum rid keyword = token->keyword;

      switch (keyword)
	{
	case RID_CASE:
	case RID_DEFAULT:
	  return cp_parser_labeled_statement (parser);

	case RID_IF:
	case RID_SWITCH:
	  return cp_parser_selection_statement (parser);

	case RID_WHILE:
	case RID_DO:
	case RID_FOR:
	  return cp_parser_iteration_statement (parser);

	case RID_BREAK:
	case RID_CONTINUE:
	case RID_RETURN:
	case RID_GOTO:
	  return cp_parser_jump_statement (parser);

	case RID_TRY:
	  return cp_parser_try_block (parser);

	default:
	  /* It might be a keyword like `int' that can start a
	     declaration-statement.  */
	  break;
	}
    }
  
  if (token->type == CPP_NAME)
    {
      /* If the next token is a `:', then we are looking at a
	 labeled-statement.  */
      token = cp_lexer_peek_nth_token (parser->lexer, 2);
      if (token->type == CPP_COLON)
	return cp_parser_labeled_statement (parser);
    }
  /* Anything that starts with a `{' must be a compound-statement.  */
  else if (token->type == CPP_OPEN_BRACE)
    return cp_parser_compound_statement (parser);

  /* Everything else must be a declaration-statement or an
     expression-statement.  Try for the declaration-statement 
     first.  */
  cp_parser_parse_tentatively (parser);
  /* Try to parse the declaration-statement.  */
  cp_parser_declaration_statement (parser);
  /* If that worked, we're done.  */
  if (cp_parser_parse_definitely (parser))
    return NULL_TREE;
  /* Otherwise, look for an expression-statement instead.  */
  else
    return cp_parser_expression_statement (parser);
}

/* Parse a labeled-statement.

   labeled-statement:
     identifier : statement
     case constant-expression : statement
     default : statement  

   Returns the new CASE_LABEL, for a `case' or `default' label.  For
   an ordinary label, returns NULL_TREE.  */

static tree
cp_parser_labeled_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree statement = NULL_TREE;

  /* The next token should be an identifier.  */
  token = cp_lexer_peek_token (parser->lexer);
  if (token->type != CPP_NAME
      && token->type != CPP_KEYWORD)
    {
      cp_parser_error (parser, "expected labeled-statement");
      return error_mark_node;
    }

  switch (token->keyword)
    {
    case RID_CASE:
      {
	tree expr;

	/* Consume the `case' token.  */
	cp_lexer_consume_token (parser->lexer);
	/* Parse the constant-expression.  */
	expr = cp_parser_constant_expression (parser);
	/* Create the label.  */
	statement = finish_case_label (expr, NULL_TREE);
      }
      break;

    case RID_DEFAULT:
      /* Consume the `default' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Create the label.  */
      statement = finish_case_label (NULL_TREE, NULL_TREE);
      break;

    default:
      /* Anything else must be an ordinary label.  */
      finish_label_stmt (cp_parser_identifier (parser));
      break;
    }

  /* Require the `:' token.  */
  cp_parser_require (parser, CPP_COLON, "`:'");
  /* Parse the labeled statement.  */
  cp_parser_statement (parser);

  /* Return the label, in the case of a `case' or `default' label.  */
  return statement;
}

/* Parse an expression-statement.

   expression-statement:
     expression [opt] ;

   Returns the new EXPR_STMT.  */

static tree
cp_parser_expression_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree statement;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If is not a `;', then there is an expression to parse.  */
  if (token->type != CPP_SEMICOLON)
    statement = finish_expr_stmt (cp_parser_expression (parser));
  /* Otherwise, we do not even bother to build an EXPR_STMT.  */
  else
    {
      finish_stmt ();
      statement = NULL_TREE;
    }
  /* Consume the final `;'.  */
  cp_parser_require (parser, CPP_SEMICOLON, "`;'");

  return statement;
}

/* Parse a compound-statement.

   compound-statement:
     { statement-seq [opt] }

   Returns a COMPOUND_STMT representing the statement.  */

static tree
cp_parser_compound_statement (parser)
     cp_parser *parser;
{
  tree compound_stmt;

  /* Consume the `{'.  */
  if (!cp_parser_require (parser, CPP_OPEN_BRACE, "`{'"))
    return error_mark_node;
  /* Begin the compound-statement.  */
  compound_stmt = begin_compound_stmt (/*has_no_scope=*/0);
  /* Parse an (optional) statement-seq.  */
  cp_parser_statement_seq_opt (parser);
  /* Finish the compound-statement.  */
  finish_compound_stmt (/*has_no_scope=*/0, compound_stmt);
  /* Consume the `}'.  */
  cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");

  return compound_stmt;
}

/* Parse an (optional) statement-seq.

   statement-seq:
     statement
     statement-seq [opt] statement  */

static void
cp_parser_statement_seq_opt (parser)
     cp_parser *parser;
{
  /* Scan statements until there aren't any more.  */
  while (true)
    {
      cp_token *token;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If we're looking at a `}', then we've run out of statements.  */
      if (token->type == CPP_CLOSE_BRACE)
	break;

      /* Parse the statement.  */
      cp_parser_statement (parser);
    }
}

/* Parse a selection-statement.

   selection-statement:
     if ( condition ) statement
     if ( condition ) statement else statement
     switch ( condition ) statement  

   Returns the new IF_STMT or SWITCH_STMT.  */

static tree
cp_parser_selection_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  enum rid keyword;

  /* Peek at the next token.  */
  token = cp_parser_require (parser, CPP_KEYWORD, "selection-statement");

  /* See what kind of keyword it is.  */
  keyword = token->keyword;
  switch (keyword)
    {
    case RID_IF:
    case RID_SWITCH:
      {
	tree statement;
	tree condition;

	/* Begin the selection-statement.  */
	if (keyword == RID_IF)
	  statement = begin_if_stmt ();
	else
	  statement = begin_switch_stmt ();

	/* Look for the `('.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	/* Parse the condition.  */
	condition = cp_parser_condition (parser);
	/* Look for the `)'.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	
	if (keyword == RID_IF)
	  {
	    tree then_stmt;

	    /* Add the condition.  */
	    finish_if_stmt_cond (condition, statement);

	    /* Parse the then-clause.  */
	    then_stmt = cp_parser_implicitly_scoped_statement (parser);
	    finish_then_clause (statement);

	    /* Peek at the next token.  */
	    token = cp_lexer_peek_token (parser->lexer);
	    /* If the next token is `else', parse the else-clause.  */
	    if (token->keyword == RID_ELSE)
	      {
		tree else_stmt;

		/* Consume the `else' keyword.  */
		cp_lexer_consume_token (parser->lexer);
		/* Parse the else-clause.  */
		else_stmt 
		  = cp_parser_implicitly_scoped_statement (parser);
		finish_else_clause (statement);
	      }

	    /* Now we're all done with the if-statement.  */
	    finish_if_stmt ();
	  }
	else
	  {
	    tree body;

	    /* Add the condition.  */
	    finish_switch_cond (condition, statement);

	    /* Parse the body of the switch-statement.  */
	    body = cp_parser_implicitly_scoped_statement (parser);

	    /* Now we're all done with the switch-statement.  */
	    finish_switch_stmt (statement);
	  }

	return statement;
      }
      break;

    default:
      cp_parser_error (parser, "expected selection-statement");
      return error_mark_node;
    }
}

/* Parse a condition. 

   condition:
     expression
     type-specifier-seq declarator = assignment-expression  

   Returns the expression, if the first production is used.
   Otherwise, returns the DECL for the new declaration.  */

static tree
cp_parser_condition (parser)
     cp_parser *parser;
{
  /* FIXME: Do not assume that the condition is an expression.  */
  return cp_parser_expression (parser);
}

/* Parse an iteration-statement.

   iteration-statement:
     while ( condition ) statement
     do statement while ( expression ) ;
     for ( for-init-statement condition [opt] ; expression [opt] )
       statement

   Returns the new WHILE_STMT, DO_STMT, or FOR_STMT.  */

static tree
cp_parser_iteration_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  enum rid keyword;
  tree statement;

  /* Peek at the next token.  */
  token = cp_parser_require (parser, CPP_KEYWORD, "iteration-statement");
  if (!token)
    return error_mark_node;

  /* See what kind of keyword it is.  */
  keyword = token->keyword;
  switch (keyword)
    {
    case RID_WHILE:
      {
	tree condition;

	/* Begin the while-statement.  */
	statement = begin_while_stmt ();
	/* Look for the `('.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	/* Parse the condition.  */
	condition = cp_parser_condition (parser);
	finish_while_stmt_cond (condition, statement);
	/* Look for the `)'.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	/* Parse the dependent statement.  */
	cp_parser_already_scoped_statement (parser);
	/* We're done with the while-statement.  */
	finish_while_stmt (statement);
      }
      break;

    case RID_DO:
      {
	tree expression;

	/* Begin the do-statement.  */
	statement = begin_do_stmt ();
	/* Parse the body of the do-statement.  */
	cp_parser_implicitly_scoped_statement (parser);
	finish_do_body (statement);
	/* Look for the `while' keyword.  */
	cp_parser_require_keyword (parser, RID_WHILE, "`while'");
	/* Look for the `('.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	/* Parse the expression.  */
	expression = cp_parser_expression (parser);
	/* We're done with the do-statement.  */
	finish_do_stmt (expression, statement);
	/* Look for the `)'.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	/* Look for the `;'.  */
	cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      }
      break;

    case RID_FOR:
      {
	tree condition = NULL_TREE;
	tree expression = NULL_TREE;

	/* Begin the for-statement.  */
	statement = begin_for_stmt ();
	/* Look for the `('.  */
	cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	/* Parse the initialization.  */
	cp_parser_for_init_statement (parser);
	finish_for_init_stmt (statement);

	/* If there's a condition, process it.  */
	if (cp_lexer_next_token_is_not (parser->lexer, CPP_SEMICOLON))
	  condition = cp_parser_condition (parser);
	finish_for_cond (condition, statement);
	/* Look for the `;'.  */
	cp_parser_require (parser, CPP_SEMICOLON, "`;'");

	/* If there's an expression, process it.  */
	if (cp_lexer_next_token_is_not (parser->lexer, CPP_CLOSE_PAREN))
	  expression = cp_parser_expression (parser);
	finish_for_expr (expression, statement);
	/* Look for the `)'.  */
	cp_parser_require (parser, CPP_CLOSE_PAREN, "`;'");

	/* Parse the body of the for-statement.  */
	cp_parser_already_scoped_statement (parser);

	/* We're done with the for-statement.  */
	finish_for_stmt (statement);
      }
      break;

    default:
      cp_parser_error (parser, "expected iteration-statement");
      statement = error_mark_node;
      break;
    }

  return statement;
}

/* Parse a for-init-statement.

   for-init-statement:
     expression-statement
     simple-declaration  */

static void
cp_parser_for_init_statement (parser)
     cp_parser *parser;
{
  /* We're going to speculatively look for a declaration, falling back
     to an expression, if necessary.  */
  cp_parser_parse_tentatively (parser);
  /* Parse the declaration.  */
  cp_parser_simple_declaration (parser);
  /* If the tentative parse failed, then we shall need to look for an
     expression-statement.  */
  if (!cp_parser_parse_definitely (parser))
    cp_parser_expression_statement (parser);
}

/* Parse a jump-statement.

   jump-statement:
     break ;
     continue ;
     return expression [opt] ;
     goto identifier ;  

   Returns the new BREAK_STMT, CONTINUE_STMT, RETURN_STMT, or
   GOTO_STMT.  */

static tree
cp_parser_jump_statement (parser)
     cp_parser *parser;
{
  tree statement = error_mark_node;
  cp_token *token;
  enum rid keyword;

  /* Peek at the next token.  */
  token = cp_parser_require (parser, CPP_KEYWORD, "jump-statement");
  if (!token)
    return error_mark_node;

  /* See what kind of keyword it is.  */
  keyword = token->keyword;
  switch (keyword)
    {
    case RID_BREAK:
      statement = finish_break_stmt ();
      cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      break;

    case RID_CONTINUE:
      statement = finish_continue_stmt ();
      cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      break;

    case RID_RETURN:
      {
	tree expr;

	/* Peek at the next token.  */
	token = cp_lexer_peek_token (parser->lexer);
	/* If the next token is a `;', then there is no 
	   expression.  */
	if (token->type != CPP_SEMICOLON)
	  expr = cp_parser_expression (parser);
	else
	  expr = NULL_TREE;
	/* Build the return-statement.  */
	statement = finish_return_stmt (expr);
	/* Look for the final `;'.  */
	cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      }
      break;

    case RID_GOTO:
      /* Create the goto-statement.  */
      finish_goto_stmt (cp_parser_identifier (parser));
      /* Look for the final `;'.  */
      cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      break;

    default:
      cp_parser_error (parser, "expected jump-statement");
      break;
    }

  return statement;
}

/* Parse a declaration-statement.

   declaration-statement:
     block-declaration  */

static tree
cp_parser_declaration_statement (parser)
     cp_parser *parser;
{
  /* Parse the block-declaration.  */
  cp_parser_block_declaration (parser);

  /* Finish off the statement.  */
  finish_stmt ();

  return NULL_TREE;
}

/* Some dependent statements (like `if (cond) statement'), are
   implicitly in their own scope.  In other words, if the statement is
   a single statement (as opposed to a compound-statement), it is
   none-the-less treated as if it were enclosed in braces.  Any
   declarations appearing in the dependent statement are out of scope
   after control passes that point.  This function parses a statement,
   but ensures that is in its own scope, even if it is not a
   compound-statement.  

   Returns the new statement.  */

static tree
cp_parser_implicitly_scoped_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree statement;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the token is not a `{', then we must take special action.  */
  if (token->type != CPP_OPEN_BRACE)
    {
      /* Create a compound-statement.  */
      statement = begin_compound_stmt (/*has_no_scope=*/0);
      /* Parse the dependent-statement.  */
      cp_parser_statement (parser);
      /* Finish the dummy compound-statement.  */
      finish_compound_stmt (/*has_no_scope=*/0, statement);
    }
  /* Otherwise, we simply parse the statement directly.  */
  else
    statement = cp_parser_compound_statement (parser);
  
  /* Return the statement.  */
  return statement;
}

/* For some dependent statements (like `while (cond) statement'), we
   have already created a scope.  Therefore, even if the dependent
   statement is a compound-statement, we do not want to create another
   scope.  

   Returns the new statement.  */

static tree
cp_parser_already_scoped_statement (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree statement;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the token is not a `{', then we must take special action.  */
  if (token->type != CPP_OPEN_BRACE)
    {
      /* Create a compound-statement.  */
      statement = begin_compound_stmt (/*has_no_scope=*/1);
      /* Parse the dependent-statement.  */
      cp_parser_statement (parser);
      /* Finish the dummy compound-statement.  */
      finish_compound_stmt (/*has_no_scope=*/1, statement);
    }
  /* Otherwise, we simply parse the statement directly.  */
  else
    statement = cp_parser_statement (parser);
  
  /* Return the statement.  */
  return statement;
}

/* Declarations [gram.dcl.dcl] */

/* Parse an optional declaration-sequence.

   declaration-seq:
     declaration
     declaration-seq declaration  */

static void
cp_parser_declaration_seq_opt (parser)
     cp_parser *parser;
{
  while (true)
    {
      cp_token *token;

      token = cp_lexer_peek_token (parser->lexer);
      if (token->type == CPP_CLOSE_BRACE
	  || token->type == CPP_EOF)
	break;
      cp_parser_declaration (parser);
    }
}

/* Parse a declaration.

   declaration:
     block-declaration
     function-definition
     template-declaration
     explicit-instantiation
     explicit-specialization
     linkage-specification
     namespace-definition    */

static void
cp_parser_declaration (parser)
     cp_parser *parser;
{
  cp_token token1;
  cp_token token2;

  /* Try to figure out what kind of declaration is present.  */
  token1 = *cp_lexer_peek_token (parser->lexer);
  if (token1.type != CPP_EOF)
    token2 = *cp_lexer_peek_nth_token (parser->lexer, 2);

  /* If the next token is `extern' and the following token is a string
     literal, then we have a linkage specification.  */
  if (cp_parser_is_keyword (&token1, RID_EXTERN) 
      && cp_parser_is_string_literal (&token2))
    cp_parser_linkage_specification (parser);
  /* If the next token is `template', then we have either a template
     declaration, an explicit instantiation, or an explicit
     specialization.  */
  else if (cp_parser_is_keyword (&token1, RID_TEMPLATE))
    {
      /* `template <>' indicates a template specialization.  */
      if (token2.type == CPP_LESS
	  && cp_lexer_peek_nth_token (parser->lexer, 3)->type == CPP_GREATER)
	cp_parser_explicit_specialization (parser);
      /* `template <' indicates a template declaration.  */
      else if (token2.type == CPP_LESS)
	cp_parser_template_declaration (parser, /*member_p=*/false);
      /* Anything else must be an explicit instantiation.  */
      else
	cp_parser_explicit_instantiation (parser);
    }
  /* If the next token is `export', then we have a template
     declaration.  */
  else if (cp_parser_is_keyword (&token1, RID_EXPORT))
    cp_parser_template_declaration (parser, /*member_p=*/false);
  /* If the next token is `namespace', check for a named or unnamed
     namespace definition.  */
  else if (cp_parser_is_keyword (&token1, RID_NAMESPACE)
	   && (/* A named namespace definition.  */
	       (token2.type == CPP_NAME
		&& (cp_lexer_peek_nth_token (parser->lexer, 3)->type 
		    == CPP_OPEN_BRACE))
	       /* An unnamed namespace definition.  */
	       || token2.type == CPP_OPEN_BRACE))
    cp_parser_namespace_definition (parser);
  /* We must have either a block declaration or a function
     definition.  */
  else
    {
      /* Begin parsing tentatively.  */
      cp_parser_parse_tentatively (parser);
      /* Try to parse a block-declaration.  */
      cp_parser_block_declaration (parser);
      /* If that fails, try a function-definition.  */
      if (!cp_parser_parse_definitely (parser))
	cp_parser_function_definition (parser,
				       /*num_template_parameter_lists=*/0);
    }
}

/* Parse a block-declaration.  

   block-declaration:
     simple-declaration
     asm-definition
     namespace-alias-definition
     using-declaration
     using-directive  

   GNU Extension:

   block-declaration:
     __extension__ block-declaration */

static void
cp_parser_block_declaration (parser)
     cp_parser *parser;
{
  cp_token *token1;
  int saved_pedantic;

  /* Check for the `__extension__' keyword.  */
  if (cp_parser_extension_opt (parser, &saved_pedantic))
    {
      /* Parse the qualified declaration.  */
      cp_parser_block_declaration (parser);
      /* Restore the PEDANTIC flag.  */
      pedantic = saved_pedantic;

      return;
    }

  /* Peek at the next token to figure out which kind of declaration is
     present.  */
  token1 = cp_lexer_peek_token (parser->lexer);

  /* If the next keyword is `asm', we have an asm-definition.  */
  if (token1->keyword == RID_ASM)
    cp_parser_asm_definition (parser);
  /* If the next keyword is `namespace', we have a
     namespace-alias-definition.  */
  else if (token1->keyword == RID_NAMESPACE)
    cp_parser_namespace_alias_definition (parser);
  /* If the next keyword is `using', we have either a
     using-declaration or a using-directive.  */
  else if (token1->keyword == RID_USING)
    {
      cp_token *token2;

      /* If the token after `using' is `namespace', then we have a
	 using-directive.  */
      token2 = cp_lexer_peek_nth_token (parser->lexer, 2);
      if (token2->keyword == RID_NAMESPACE)
	cp_parser_using_directive (parser);
      /* Otherwise, it's a using-declaration.  */
      else
	cp_parser_using_declaration (parser);
    }
  /* Anything else must be a simple-declaration.  */
  else
    cp_parser_simple_declaration (parser);
}

/* Parse a simple-declaration.

   simple-declaration:
     decl-specifier-seq [opt] init-declarator-list [opt] ;  

   init-declarator-list:
     init-declarator
     init-declarator-list , init-declarator */

static void
cp_parser_simple_declaration (parser)
     cp_parser *parser;
{
  tree decl_specifiers;
  tree attributes;
  bool declares_class_or_enum;
  bool saw_declarator;

  /* Parse the decl-specifier-seq.  We have to keep track of whether
     or not the decl-specifier-seq declares a named class or
     enumeration type, since that is the only case in which the
     init-declarator-list is allowed to be empty.  

     [dcl.dcl]

     In a simple-declaration, the optional init-declarator-list can be
     omitted only when declaring a class or enumeration, that is when
     the decl-specifier-seq contains either a class-specifier, an
     elaborated-type-specifier, or an enum-specifier.  */
  decl_specifiers
    = cp_parser_decl_specifier_seq (parser, 
				    CP_PARSER_FLAGS_OPTIONAL,
				    &attributes,
				    &declares_class_or_enum);

  /* Keep going until we hit the `;' at the end of the simple
     declaration.  */
  saw_declarator = false;
  while (cp_lexer_peek_token (parser->lexer)->type != CPP_SEMICOLON)
    {
      cp_token *token;

      saw_declarator = true;
      /* Parse the init-declarator.  */
      cp_parser_init_declarator (parser, decl_specifiers, attributes,
				 /*num_template_parameter_lists=*/0);
      /* The next token should be either a `,' or a `;'.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's a `,', there are more declarators to come.  */
      if (token->type == CPP_COMMA)
	cp_lexer_consume_token (parser->lexer);
      /* If it's a `;', we are done.  */
      else if (token->type == CPP_SEMICOLON)
	continue;
      /* Anything else is an error.  */
      else
	{
	  cp_parser_error (parser, "expected `,' or `;'");
	  return;
	}
    }

  /* Issue an error message if no declarators are present, and the
     decl-specifier-seq does not itself declare a class or
     enumeration.  */
  if (decl_specifiers && !declares_class_or_enum && !saw_declarator)
    cp_parser_unimplemented ();

  /* Consume the `;'.  */
  cp_parser_require (parser, CPP_SEMICOLON, "`;'");
}

/* Parse a decl-specifier-seq.

   decl-specifier-seq:
     decl-specifier-seq [opt] decl-specifier

   decl-specifier:
     storage-class-specifier
     type-specifier
     function-specifier
     friend
     typedef  

   GNU Extension:

   decl-specifier-seq:
     decl-specifier-seq [opt] attributes

   Returns a TREE_LIST, giving the decl-specifiers in the order they
   appear in the source code.  The TREE_VALUE of each node is the
   decl-specifier.  For a keyword (such as `auto' or `friend'), the
   TREE_VALUE is simply the correspoding TREE_IDENTIFIER.  For the
   representation of a type-specifier, see cp_parser_type_specifier.  

   If there are attributes, they will be stored in *ATTRIBUTES,
   represented as described above cp_parser_attributes.  */
   
static tree
cp_parser_decl_specifier_seq (parser, flags, attributes,
			      declares_class_or_enum)
     cp_parser *parser;
     cp_parser_flags flags;
     tree *attributes;
     bool *declares_class_or_enum;
{
  tree decl_specs = NULL_TREE;
  bool friend_p = false;

  /* Assume no class or enumeration type is declared.  */
  *declares_class_or_enum = false;

  /* Assume there are no attributes.  */
  *attributes = NULL_TREE;

  /* Keep reading specifiers until there are no more to read.  */
  while (true)
    {
      tree decl_spec = NULL_TREE;
      bool constructor_p;
      cp_token *token;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* Handle attributes.  */
      if (token->keyword == RID_ATTRIBUTE)
	{
	  /* Parse the attributes.  */
	  decl_spec = cp_parser_attributes_opt (parser);
	  /* Add them to the list.  */
	  *attributes = chainon (*attributes, decl_spec);
	  continue;
	}
      /* If the next token is an appropriate keyword, we can simply
	 add it to the list.  */
      switch (token->keyword)
	{
	case RID_FRIEND:
	  /* decl-specifier:
	       friend  */
	  friend_p = true;

	  /* Fall through.  */

	  /* storage-class-specifier:
	       auto
	       register
	       static
	       extern
	       mutable  */
	case RID_AUTO:
	case RID_REGISTER:
	case RID_STATIC:
	case RID_EXTERN:
	case RID_MUTABLE:
	  /* function-specifier:
	       inline
	       virtual
	       explicit  */
	case RID_INLINE:
	case RID_VIRTUAL:
	case RID_EXPLICIT:
	  /* decl-specifier:
	       typedef  */
	case RID_TYPEDEF:
	  /* The representation of the specifier is simply the
	     appropriate TREE_IDENTIFIER node.  */
	  decl_spec = token->value;
	  /* Consume the token.  */
	  cp_lexer_consume_token (parser->lexer);
	  break;

	default:
	  break;
	}

      /* Constructors are a special case.  The `S' in `S()' is not a
	 decl-specifier; it is the beginning of the declarator.
	 Assume that we are not looking at a constructor.  */
      constructor_p = false;
      /* But, if we haven't already found a decl-specifier, and we are
	 in the process of defining a class, then we might be looking
	 at a constructor.  */
      if (!decl_spec 
	  && at_class_scope_p ()
	  && TYPE_BEING_DEFINED (current_class_type))
	{
	  tree class_name;

	  /* If the next thing is a class-name naming the class, then
	     it's a constructor.  */
	  cp_parser_parse_tentatively (parser);
	  /* When we look up the class type, there is no
	     qualification.  */
	  parser->scope = NULL_TREE;
	  /* Look for the class-name.  */
	  class_name = cp_parser_class_name (parser,
					     /*typename_keyword_p=*/false,
					     /*template_keyword_p=*/false);
	  /* Look for a `('.  If there isn't one, then this is just 
	     something like `S f()' -- it isn't a constructor.  */
	  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	  /* If that worked, see if it's the right type.  */
	  if (!cp_parser_error_occurred (parser)
	      && TREE_TYPE (class_name) == current_class_type)
	    constructor_p = true;
	  /* We did not really want to consume any tokens.  */
	  cp_parser_abort_tentative_parse (parser);
	}
      /* Outside a class-scope we are looking at a constructor
	 declarator, and not a decl-specifier, if we see a
	 nested-name-specifier followed by the class-name again.  */
      /* FIXME: Document that prefix-attributes cannot easily be used
	 with constructors?  */
      else if (!decl_spec)
	{
	  tree scope;
	  tree class_name;

	  /* We have no way of knowing whether we will really be
	     looking at a constructor declarator or not.  */
	  cp_parser_parse_tentatively (parser);
	  /* Look for the optional `::' operator.  */
	  cp_parser_global_scope_opt (parser,
				      /*current_scope_valid_p=*/false);
	  /* Look for the nested-name-specifier.  */
	  scope 
	    = cp_parser_nested_name_specifier (parser,
					       /*typename_keyword_p=*/true);
	  /* Look for the class-name again.  */
	  class_name = cp_parser_class_name (parser,
					     /*typename_keyword_p=*/true,
					     /*template_keyword_p=*/false);
	  /* Look for a `('.  If there isn't one, then this is just 
	     something like `S::S f()' -- it isn't a constructor.  */
	  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
	  /* If that worked, see if it's the right type.  */
	  if (!cp_parser_error_occurred (parser)
	      && constructor_name_p (DECL_NAME (class_name),
				     scope))
	    constructor_p = true;
	  /* We did not really want to consume any tokens.  */
	  cp_parser_abort_tentative_parse (parser);
	}

      /* If we don't have a DECL_SPEC yet, then we must be looking at
	 a type-specifier.  */
      if (!decl_spec && !constructor_p)
	{
	  bool decl_spec_declares_class_or_enum;
	  bool is_cv_qualifier;

	  decl_spec
	    = cp_parser_type_specifier (parser, flags,
					friend_p,
					&decl_spec_declares_class_or_enum,
					&is_cv_qualifier);

	  *declares_class_or_enum |= decl_spec_declares_class_or_enum;

	  /* If this type-specifier referenced a user-defined type
	     (a typedef, class-name, etc.), then we can't allow any
	     more such type-specifiers henceforth.

	     [dcl.spec]

	     The longest sequence of decl-specifiers that could
	     possibly be a type name is taken as the
	     decl-specifier-seq of a declaration.  The sequence shall
	     be self-consistent as described below.

	     [dcl.type]

	     As a general rule, at most one type-specifier is allowed
	     in the complete decl-specifier-seq of a declaration.  The
	     only exceptions are the following:

	     -- const or volatile can be combined with any other
	        type-specifier. 
		
             -- signed or unsigned can be combined with char, long,
        	short, or int.
 
             -- ..

	     Example:
	     
	       typedef char* Pc;
	       void g (const int Pc);

	     Here, Pc is *not* part of the decl-specifier seq; it's
	     the declarator.  Therefore, once we see a type-specifier
	     (other than a cv-qualifier), we forbid any additional
	     user-defined types.  We *do* still allow things like `int
	     int' to be considered a decl-specifier-seq, and issue the
	     error message later.  */
	  if (decl_spec && !is_cv_qualifier)
	    flags |= CP_PARSER_FLAGS_NO_USER_DEFINED_TYPES;
	}

      /* If we still do not have a DECL_SPEC, then there are no more
	 decl-specifiers.  */
      if (!decl_spec)
	{
	  /* If we never saw any, then we must issue an error message,
	     unless the entire construct was optional.  */
	  if (!decl_specs && !(flags & CP_PARSER_FLAGS_OPTIONAL))
	    {
	      cp_parser_error (parser, "expected decl specifier");
	      return error_mark_node;
	    }

	  break;
	}

      /* Add the DECL_SPEC to the list of specifiers.  */
      decl_specs = tree_cons (NULL_TREE, decl_spec, decl_specs);

      /* After we see one decl-specifier, further decl-specifiers are
	 always optional.  */
      flags |= CP_PARSER_FLAGS_OPTIONAL;
    }

  /* We have built up the DECL_SPECS in reverse order.  Return them in
     the correct order.  */
  return nreverse (decl_specs);
}

/* Parse a linkage-specification.

   linkage-specification:
     extern string-literal { declaration-seq [opt] }
     extern string-literal declaration  */

static void
cp_parser_linkage_specification (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree linkage;

  /* Look for the `extern' keyword.  */
  cp_parser_require_keyword (parser, RID_EXTERN, "`extern'");

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's not a string-literal, then there's a problem.  */
  if (!cp_parser_is_string_literal (token))
    {
      cp_parser_error (parser, "expected language-name");
      return;
    }
  /* Consume the token.  */
  cp_lexer_consume_token (parser->lexer);

  /* Transform the literal into an identifier.  If the literal is a
     wide-character string, or contains embedded NULs, then we're
     don't handle it correctly.  */
  if (token->type == CPP_WSTRING
      || (strlen (TREE_STRING_POINTER (token->value))
	  != (size_t) (TREE_STRING_LENGTH (token->value) - 1)))
    {
      cp_parser_error (parser, "invalid linkage-specification");
      /* Assume C++ linkage.  */
      linkage = get_identifier ("c++");
    }
  /* If it's a simple string constant, things are easier.  */
  else
    linkage = get_identifier (TREE_STRING_POINTER (token->value));

  /* We're now using the new linkage.  */
  push_lang_context (linkage);

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's a `{', then we're using the first production.  */
  if (token->type == CPP_OPEN_BRACE)
    {
      /* Consume the `{' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the declarations.  */
      cp_parser_declaration_seq_opt (parser);
      /* Look for the closing `}'.  */
      cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");
    }
  /* Otherwise, there's just one declaration.  */
  else
    cp_parser_declaration (parser);

  /* We're done with the linkage-specification.  */
  pop_lang_context ();
}

/* Special member functions [gram.special] */

/* Parse a conversion-function-id.

   conversion-function-id:
     operator conversion-type-id  

   Returns an IDENTIFIER_NODE representing the operator.  */

static tree 
cp_parser_conversion_function_id (parser)
     cp_parser *parser;
{
  tree type;

  /* Look for the `operator' token.  */
  cp_parser_require_keyword (parser, RID_OPERATOR, "`operator'");
  /* Parse the conversion-type-id.  */
  type = cp_parser_conversion_type_id (parser);

  return mangle_conv_op_name_for_type (type);
}

/* Parse a conversion-type-id:
    
   conversion-type-id:
     type-specifier-seq conversion-declarator [opt]

   Returns the TYPE specified.  */

static tree
cp_parser_conversion_type_id (parser)
     cp_parser *parser;
{
  tree type_specifiers;
  tree declarator;

  /* Parse the type-specifiers.  */
  type_specifiers = cp_parser_type_specifier_seq (parser);
  /* Parse the conversion-declarator.  */
  declarator = cp_parser_conversion_declarator_opt (parser);

  return grokdeclarator (declarator, type_specifiers, TYPENAME,
			 /*initialized=*/0, /*attrlist=*/NULL_TREE);
}

/* Parse an (optional) conversion-declarator.

   conversion-declarator:
     ptr-operator conversion-declarator [opt]  

   Returns a representation of the declarator.  See
   cp_parser_declarator for details.  */

static tree
cp_parser_conversion_declarator_opt (parser)
     cp_parser *parser;
{
  enum tree_code code;
  tree type;
  tree cv_qualifier_seq;

  /* We don't know if there's a ptr-operator next, or not.  */
  cp_parser_parse_tentatively (parser);
  /* Try the ptr-operator.  */
  code = cp_parser_ptr_operator (parser, &type, &cv_qualifier_seq);
  /* If it worked, look for more conversion-declarators.  */
  if (cp_parser_parse_definitely (parser))
    {
      tree declarator;

      /* FIXME: Handle pointers-to-members.  */
      if (type)
	cp_parser_unimplemented ();

      /* Parse another optional declarator.  */
      declarator = cp_parser_conversion_declarator_opt (parser);

      /* FIXME: Factor this code.  */
      /* Create the representation of the declarator.  */
      if (code == INDIRECT_REF)
	return make_pointer_declarator (declarator, 
					cv_qualifier_seq);
      else
	return make_reference_declarator (declarator, 
					  cv_qualifier_seq);
    }

  return NULL_TREE;
}

/* Parse a ctor-initializer.

   ctor-initializer:
     : mem-initializer-list  */

static void
cp_parser_ctor_initializer (parser)
     cp_parser *parser;
{
  /* Look for the `:' token.  */
  cp_parser_require (parser, CPP_COLON, "`:'");
  /* And the mem-initializer-list.  */
  cp_parser_mem_initializer_list (parser);
}

/* Parse a mem-initializer-list.

   mem-initializer-list:
     mem-initializer
     mem-initializer , mem-initializer-list  */

static void
cp_parser_mem_initializer_list (parser)
     cp_parser *parser;
{
  tree mem_initializer_list = NULL_TREE;

  /* Let the semantic analysis code know that we are starting the
     mem-initializer-list.  */
  begin_mem_initializer_list ();

  /* Loop through the list.  */
  while (true)
    {
      cp_token *token;
      tree mem_initializer;

      /* Parse the mem-initializer.  */
      mem_initializer = cp_parser_mem_initializer (parser);
      /* Add it to the list.  */
      TREE_CHAIN (mem_initializer) = mem_initializer_list;
      mem_initializer_list = mem_initializer;
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not a `,', we're done.  */
      if (token->type != CPP_COMMA)
	break;
      /* Consume the `,' token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* We built up the list in reverse order.  */
  mem_initializer_list = nreverse (mem_initializer_list);
  /* Perform semantic analysis.  */
  finish_mem_initializers (mem_initializer_list);
}

/* Parse a mem-initializer.

   mem-initializer:
     mem-initializer-id ( expression-list [opt] )  

   Returns a TREE_LIST.  The TREE_PURPOSE is the TYPE or 
   FIELD_DECL to initialize; the TREE_VALUE is expression-list.  */

static tree
cp_parser_mem_initializer (parser)
     cp_parser *parser;
{
  tree mem_initializer_id;
  tree expression_list;

  /* Find out what is being initialized.  */
  mem_initializer_id = cp_parser_mem_initializer_id (parser);
  /* Look for the opening `('.  */
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
  /* Parse the expression-list.  */
  if (cp_lexer_next_token_is_not (parser->lexer,
				  CPP_CLOSE_PAREN))
    expression_list = cp_parser_expression_list (parser);
  else
    expression_list = NULL_TREE;
  /* Look for the closing `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  return expand_member_init (current_class_ref, 
			     mem_initializer_id,
			     expression_list);
}

/* Parse a mem-initializer-id.

   mem-initializer-id:
     :: [opt] nested-name-specifier [opt] class-name
     identifier  

   Returns a TYPE indicating the class to be initializer for the first
   production.  Returns an IDENTIFIER_NODE indicating the data member
   to be initialized for the second production.  */

static tree
cp_parser_mem_initializer_id (parser)
     cp_parser *parser;
{
  bool global_scope_p;
  bool nested_name_specifier_p;
  tree id;

  /* Look for the optional `::' operator.  */
  global_scope_p 
    = (cp_parser_global_scope_opt (parser, 
				   /*current_scope_valid_p=*/false) 
       != NULL_TREE);
  /* Look for the optional nested-name-specifier.  The simplest way to
     implement:

       [temp.res]

       The keyword `typename' is not permitted in a base-specifier or
       mem-initializer; in these contexts a qualified name that
       depends on a template-parameter is implicitly assumed to be a
       type name.

     is to assume that we have seen the `typename' keyword at this
     point.  */
  nested_name_specifier_p 
    = (cp_parser_nested_name_specifier_opt (parser,
					    /*typename_keyword_p=*/true)
       != NULL_TREE);
  /* If there is a `::' operator or a nested-name-specifier, then we
     are definitely looking for a class-name.  */
  if (global_scope_p || nested_name_specifier_p)
    return cp_parser_class_name (parser,
				 /*typename_keyword_p=*/true,
				 /*template_keyword_p=*/false);
  /* Otherwise, we could also be looking for an ordinary identifier.  */
  cp_parser_parse_tentatively (parser);
  /* Try a class-name.  */
  id = cp_parser_class_name (parser, 
			     /*typename_keyword_p=*/true,
			     /*template_keyword_p=*/false);
  /* If we found one, we're done.  */
  if (cp_parser_parse_definitely (parser))
    return id;
  /* Otherwise, look for an ordinary identifier.  */
  return cp_parser_identifier (parser);
}

/* Overloading [gram.over] */

/* Parse an operator-function-id.

   operator-function-id:
     operator operator  

   Returns an IDENTIFIER_NODE for the operator which is a
   human-readable spelling of the identifier, e.g., `operator +'.  */

static tree 
cp_parser_operator_function_id (parser)
     cp_parser *parser;
{
  /* Look for the `operator' keyword.  */
  cp_parser_require_keyword (parser, RID_OPERATOR, "`operator'");
  /* And then the name of the operator itself.  */
  return cp_parser_operator (parser);
}

/* Parse an operator.

   operator:
     new delete new[] delete[] + - * / % ^ & | ~ ! = < >
     += -= *= /= %= ^= &= |= << >> >>= <<= == != <= >= &&
     || ++ -- , ->* -> () []

   Returns an IDENTIFIER_NODE for the operator which is a
   human-readable spelling of the identifier, e.g., `operator +'.  */
   
static tree
cp_parser_operator (parser)
     cp_parser *parser;
{
  tree id = NULL_TREE;
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Figure out which operator we have.  */
  switch (token->type)
    {
    case CPP_KEYWORD:
      {
	enum tree_code op;

	/* The keyword should be either `new' or `delete'.  */
	if (token->keyword == RID_NEW)
	  op = NEW_EXPR;
	else if (token->keyword == RID_DELETE)
	  op = DELETE_EXPR;
	else
	  break;

	/* Consume the `new' or `delete' token.  */
	cp_lexer_consume_token (parser->lexer);

	/* Peek at the next token.  */
	token = cp_lexer_peek_token (parser->lexer);
	/* If it's a `[' token then this is the array variant of the
	   operator.  */
	if (token->type == CPP_OPEN_SQUARE)
	  {
	    /* Consume the `[' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Look for the `]' token.  */
	    cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");
	    id = ansi_opname (op == NEW_EXPR 
			      ? VEC_NEW_EXPR : VEC_DELETE_EXPR);
	  }
	/* Otherwise, we have the non-array variant.  */
	else
	  id = ansi_opname (op);

	return id;
      }

    case CPP_PLUS:
      id = ansi_opname (PLUS_EXPR);
      break;

    case CPP_MINUS:
      id = ansi_opname (MINUS_EXPR);
      break;

    case CPP_MULT:
      id = ansi_opname (MULT_EXPR);
      break;

    case CPP_DIV:
      id = ansi_opname (TRUNC_DIV_EXPR);
      break;

    case CPP_MOD:
      id = ansi_opname (TRUNC_MOD_EXPR);
      break;

    case CPP_XOR:
      id = ansi_opname (BIT_XOR_EXPR);
      break;

    case CPP_AND:
      id = ansi_opname (BIT_AND_EXPR);
      break;

    case CPP_OR:
      id = ansi_opname (BIT_IOR_EXPR);
      break;

    case CPP_COMPL:
      id = ansi_opname (BIT_NOT_EXPR);
      break;
      
    case CPP_NOT:
      id = ansi_opname (TRUTH_NOT_EXPR);
      break;

    case CPP_EQ:
      id = ansi_assopname (NOP_EXPR);
      break;

    case CPP_LESS:
      id = ansi_opname (LT_EXPR);
      break;

    case CPP_GREATER:
      id = ansi_opname (GT_EXPR);
      break;

    case CPP_PLUS_EQ:
      id = ansi_assopname (PLUS_EXPR);
      break;

    case CPP_MINUS_EQ:
      id = ansi_assopname (MINUS_EXPR);
      break;

    case CPP_MULT_EQ:
      id = ansi_assopname (MULT_EXPR);
      break;

    case CPP_DIV_EQ:
      id = ansi_assopname (TRUNC_DIV_EXPR);
      break;

    case CPP_MOD_EQ:
      id = ansi_assopname (TRUNC_MOD_EXPR);
      break;

    case CPP_XOR_EQ:
      id = ansi_assopname (BIT_XOR_EXPR);
      break;

    case CPP_AND_EQ:
      id = ansi_assopname (BIT_AND_EXPR);
      break;

    case CPP_OR_EQ:
      id = ansi_assopname (BIT_IOR_EXPR);
      break;

    case CPP_LSHIFT:
      id = ansi_opname (LSHIFT_EXPR);
      break;

    case CPP_RSHIFT:
      id = ansi_opname (RSHIFT_EXPR);
      break;

    case CPP_LSHIFT_EQ:
      id = ansi_assopname (LSHIFT_EXPR);
      break;

    case CPP_RSHIFT_EQ:
      id = ansi_assopname (RSHIFT_EXPR);
      break;

    case CPP_EQ_EQ:
      id = ansi_opname (EQ_EXPR);
      break;

    case CPP_NOT_EQ:
      id = ansi_opname (NE_EXPR);
      break;

    case CPP_LESS_EQ:
      id = ansi_opname (LE_EXPR);
      break;

    case CPP_GREATER_EQ:
      id = ansi_opname (GE_EXPR);
      break;

    case CPP_AND_AND:
      id = ansi_opname (TRUTH_ANDIF_EXPR);
      break;

    case CPP_OR_OR:
      id = ansi_opname (TRUTH_ORIF_EXPR);
      break;
      
    case CPP_PLUS_PLUS:
      /* FIXME: It is bizarre that we use post-increment for `++', but
	 pre-decrement for `--'.  Is there a reason for this?  */
      id = ansi_opname (POSTINCREMENT_EXPR);
      break;

    case CPP_MINUS_MINUS:
      id = ansi_opname (PREDECREMENT_EXPR);
      break;

    case CPP_COMMA:
      id = ansi_opname (COMPOUND_EXPR);
      break;

    case CPP_DEREF_STAR:
      id = ansi_opname (MEMBER_REF);
      break;

    case CPP_DEREF:
      id = ansi_opname (COMPONENT_REF);
      break;

    case CPP_OPEN_PAREN:
      /* Consume the `('.  */
      cp_lexer_consume_token (parser->lexer);
      /* Look for the matching `)'.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
      return ansi_opname (CALL_EXPR);

    case CPP_OPEN_SQUARE:
      /* Consume the `['.  */
      cp_lexer_consume_token (parser->lexer);
      /* Look for the matching `]'.  */
      cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");
      return ansi_opname (ARRAY_REF);

    default:
      /* Anything else is an error.  */
      break;
    }

  /* If we have selected an identifier, we need to consume the
     operator token.  */
  if (id)
    cp_lexer_consume_token (parser->lexer);
  /* Otherwise, no valid operator name was present.  */
  else
    {
      cp_parser_error (parser, "expected operator");
      id = error_mark_node;
    }

  return id;
}

/* Parse a template-declaration.

   template-declaration:
     export [opt] template < template-parameter-list > declaration  

   If MEMBER_P is TRUE, this template-declaration occurs within a
   class-specifier.  

   The grammar rule given by the standard isn't correct.  What
   is really meant is:

   template-declaration:
     export [opt] template-parameter-list-seq 
       decl-specifier-seq [opt] init-declarator [opt] ;
     export [opt] template-parameter-list-seq 
       function-definition

   template-parameter-list-seq:
     template-parameter-list-seq [opt]
       template < template-parameter-list >  */

static void
cp_parser_template_declaration (parser, member_p)
     cp_parser *parser;
     bool member_p;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's the `export' token, issue a warning that we don't support
     it.  */
  if (token->keyword == RID_EXPORT)
    {
      /* Consume the `export' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Issue the warning.  */
      cp_warning ("keyword `export' not implemented, and will be ignored");
    }

  cp_parser_template_declaration_after_export (parser, 
					       member_p,
					       /*num_parameter_lists=*/0);
}

/* Parse a template-parameter-list.

   template-parameter-list:
     template-parameter
     template-parameter-list , template-parameter

   Returns a TREE_LIST.  Each node represents a template parameter.
   The nodes are connected via their TREE_CHAINs.  */

static tree
cp_parser_template_parameter_list (parser)
     cp_parser *parser;
{
  tree parameter_list = NULL_TREE;

  while (true)
    {
      tree parameter;
      cp_token *token;

      /* Parse the template-parameter.  */
      parameter = cp_parser_template_parameter (parser);
      /* Add it to the list.  */
      parameter_list = process_template_parm (parameter_list,
					      parameter);

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not a `,', we're done.  */
      if (token->type != CPP_COMMA)
	break;
      /* Otherwise, consume the `,' token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  return parameter_list;
}

/* Parse a template-parameter.

   template-parameter:
     type-parameter
     parameter-declaration

   Returns a TREE_LIST.  The TREE_VALUE represents the parameter.  The
   TREE_PURPOSE is the default value, if any.  */

static tree
cp_parser_template_parameter (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it is `class', `typename', or `template', we have a 
     type-parameter.  */
  if (token->keyword == RID_CLASS 
      || token->keyword == RID_TYPENAME
      || token->keyword == RID_TEMPLATE)
    return cp_parser_type_parameter (parser);

  /* Otherwise, it is a non-type parameter.  

     [temp.param]

     When parsing a default template-argument for a non-type
     template-parameter, the first non-nested `>' is taken as the end
     of the template parameter-list rather than a greater-than
     operator.  */
  return 
    cp_parser_parameter_declaration (parser,
				     /*greater_than_is_operator_p=*/false);
}

/* Parse a type-parameter.

   type-parameter:
     class identifier [opt]
     class identifier [opt] = type-id
     typename identifier [opt]
     typename identifier [opt] = type-id
     template < template-parameter-list > class identifier [opt]
     template < tmeplate-parameter-list > class identifier [opt] 
       = id-expression  

   Returns a TREE_LIST.  The TREE_VALUE is itself a TREE_LIST.  The
   TREE_PURPOSE is the default-argument, if any.  The TREE_VALUE is
   the declaration of the parameter.  */

static tree
cp_parser_type_parameter (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree parameter;

  /* Look for a keyword to tell us what kind of parameter this is.  */
  token = cp_parser_require (parser, CPP_KEYWORD, 
			     "expected `class', `typename', or `template'");
  if (!token)
    return error_mark_node;

  switch (token->keyword)
    {
    case RID_CLASS:
    case RID_TYPENAME:
      {
	tree identifier;
	tree default_argument;

	/* If the next token is an identifier, then it names the
           parameter.  */
	if (cp_lexer_next_token_is (parser->lexer, CPP_NAME))
	  identifier = cp_parser_identifier (parser);
	else
	  identifier = NULL_TREE;

	/* Create the parameter.  */
	parameter = finish_template_type_parm (ctk_class, identifier);

	/* If the next token is an `=', we have a default argument.  */
	if (cp_lexer_next_token_is (parser->lexer, CPP_EQ))
	  {
	    /* Consume the `=' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Parse the default-argumen.  */
	    default_argument = cp_parser_type_id (parser);
	  }
	else
	  default_argument = NULL_TREE;

	/* Create the combined representation of the parameter and the
	   default argument.  */
	parameter = build_tree_list (default_argument, 
				     parameter);
      }
      break;

    case RID_TEMPLATE:
      {
	tree parameter_list;
	tree identifier;
	tree default_argument;

	/* Look for the `<'.  */
	cp_parser_require (parser, CPP_LESS, "`<'");
	/* Parse the template-parameter-list.  */
	begin_template_parm_list ();
	parameter_list 
	  = cp_parser_template_parameter_list (parser);
	parameter_list = end_template_parm_list (parameter_list);
	/* Look for the `>'.  */
	cp_parser_require (parser, CPP_GREATER, "`>'");
	/* Look for the `class' keyword.  */
	cp_parser_require_keyword (parser, RID_CLASS, "`class'");
	/* If the next token is an `=', then there is a
	   default-argument.  If the next token is a `>', we are at
	   the end of the parameter-list.  If the next token is a `,',
	   then we are at the end of this parameter.  */
	if (cp_lexer_next_token_is_not (parser->lexer, CPP_EQ)
	    && cp_lexer_next_token_is_not (parser->lexer, CPP_GREATER)
	    && cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA))
	  identifier = cp_parser_identifier (parser);
	else
	  identifier = NULL_TREE;
	/* Create the template parameter.  */
	parameter = finish_template_template_parm (ctk_class,
						   identifier);
						   
	/* If the next token is an `=', then there is a
	   default-argument.  */
	if (cp_lexer_next_token_is (parser->lexer, CPP_EQ))
	  {
	    /* Consume the `='.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Parse the id-expression.  */
	    default_argument 
	      = cp_parser_id_expression (parser,
					 /*assume_typename_p=*/false);
	    /* See if the default argument is valid.  */
	    default_argument
	      = check_template_template_default_arg (default_argument);
	  }
	else
	  default_argument = NULL_TREE;

	/* Create the combined representation of the parameter and the
	   default argument.  */
	parameter =  build_tree_list (default_argument, 
				      parameter);
      }
      break;

    default:
      /* Anything else is an error.  */
      cp_parser_error (parser,
		       "expected `class', `typename', or `template'");
      parameter = error_mark_node;
    }
  
  return parameter;
}

/* Parse a template-id.

   template-id:
     template-name < template-argument-list [opt] >

   If TEMPLATE_KEYWORD_P is TRUE, then we have just seen the
   `template' keyword.  In this case, a TEMPLATE_ID_EXPR will be
   returned.  Otherwise, if the template-name names a function, or set
   of functions, returns a TEMPLATE_ID_EXPR.  If the template-name
   names a class, returns a TYPE_DECL for the specialization.  */

/* FIXME: Document representation for template-templates.  */

static tree
cp_parser_template_id (parser, template_keyword_p)
     cp_parser *parser;
     bool template_keyword_p;
{
  tree template;
  tree arguments;
  bool saved_greater_than_is_operator_p;

  /* Parse the template-name.  */
  template = cp_parser_template_name (parser, template_keyword_p);
  if (template == error_mark_node)
    return error_mark_node;

  /* Look for the `<' that starts the template-argument-list.  */
  if (!cp_parser_require (parser, CPP_LESS, "`<'"))
    return error_mark_node;

  /* [temp.names]

     When parsing a template-id, the first non-nested `>' is taken as
     the end of the template-argument-list rather than a greater-than
     operator.  */
  saved_greater_than_is_operator_p 
    = parser->greater_than_is_operator_p;
  parser->greater_than_is_operator_p = false;
  /* Parse the template-argument-list itself.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_GREATER))
    arguments = NULL_TREE;
  else
    arguments = cp_parser_template_argument_list (parser);
  /* Look for the `>' that ends the template-argument-list.  */
  cp_parser_require (parser, CPP_GREATER, "`>'");
  /* The `>' token might be a greater-than operator again now.  */
  parser->greater_than_is_operator_p 
    = saved_greater_than_is_operator_p;

  /* Build a representation of the specialization.  */
  if (TREE_CODE (template) == IDENTIFIER_NODE)
    return build_min_nt (TEMPLATE_ID_EXPR, template, arguments);
  if (DECL_CLASS_TEMPLATE_P (template)
      || DECL_TEMPLATE_TEMPLATE_PARM_P (template))
    return finish_template_type (template, arguments, 
				 cp_lexer_next_token_is (parser->lexer, 
							 CPP_SCOPE));
  else if (DECL_FUNCTION_TEMPLATE_P (template)
	   || TREE_CODE (template) == OVERLOAD)
    return lookup_template_function (template, arguments);
}

/* Parse a template-name.

   template-name:
     identifier
 
   If TEMPLATE_KEYWORD_P is true, then we have just seen the
   `template' keyword, in a construction like:

     T::template f<3>()

   In that case `f' is taken to be a template-name, even though there
   is no way of knowing for sure.

   Returns the TEMPLATE_DECL for the template, or an OVERLOAD if the
   name refers to a set of overloaded functions, at least one of which
   is a template, or an IDENTIFIER_NODE with the name of the template,
   if TEMPLATE_KEYWORD_P is true.  */

static tree
cp_parser_template_name (parser, template_keyword_p)
     cp_parser *parser;
     bool template_keyword_p;
{
  tree identifier;
  tree decl;

  /* Look for the identifier.  */
  identifier = cp_parser_identifier (parser);
  if (identifier == error_mark_node)
    return error_mark_node;

  /* If the name immediately followed the `template' keyword, then it
     is a template-name.  */
  /* FIXME: This only matters if the qualifying scope depends on a
     template argument.  If it does not, we should ignore
     TEMPLATE_KEYWORD_P.  */
  if (template_keyword_p)
    return identifier;

  /* Look up the name.  */
  decl = cp_parser_lookup_name (parser, identifier);
  /* FIXME: We should not be inserting a TYPE_DECL for the class being
     declared in a template-class.  */
  decl = maybe_get_template_decl_from_type_decl (decl);
  /* If DECL is a template, then the name was a template-name.  */
  if (TREE_CODE (decl) == TEMPLATE_DECL)
    return decl;

  /* The standard does not explicitly indicate whether a name that
     names a set of overloaded declarations, some of which are
     templates, is a template-name.  However, such a name should be a
     template-name; otherwise, there is no way to form a template-id
     for the overloaded templates.  */
  if (TREE_CODE (decl) == OVERLOAD)
    {
      tree fn;

      for (fn = decl; fn; fn = OVL_NEXT (fn))
	if (TREE_CODE (OVL_CURRENT (fn)) == TEMPLATE_DECL)
	  return decl;
    }

  /* Otherwise, the name does not name a template.  */
  cp_parser_error (parser, "expected template-name");
  return error_mark_node;
}

/* Parse a template-argument-list.

   template-argument-list:
     template-argument
     template-argument-list , template-argument

   Returns a TREE_LIST representing the arguments, in the order they
   appeared.  The TREE_VALUE of each node is a representation of the
   argument.  */

static tree
cp_parser_template_argument_list (parser)
     cp_parser *parser;
{
  tree arguments = NULL_TREE;

  while (true)
    {
      cp_token *token;
      tree argument;

      /* Parse the template-argument.  */
      argument = cp_parser_template_argument (parser);
      /* Add it to the list.  */
      arguments = tree_cons (NULL_TREE, argument, arguments);
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it is not a `,', then there are no more arguments.  */
      if (token->type != CPP_COMMA)
	break;
      /* Otherwise, consume the token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* We built up the arguments in reverse order.  */
  return nreverse (arguments);
}

/* Parse a template-argument.

   template-argument:
     assignment-expression
     type-id
     id-expression

   The representation is that of an assignment-expression, type-id, or
   id-expression -- except that the qualified id-expression is
   evaluated, so that the value returned is either a DECL or an
   OVERLOAD.  */

static tree
cp_parser_template_argument (parser)
     cp_parser *parser;
{
  tree argument;

  /* There's really no way to know what we're looking at, so we just
     try each alternative in order.  

       [temp.arg]

       In a template-argument, an ambiguity between a type-id and an
       expression is resolved to a type-id, regardless of the form of
       the corresponding template-parameter.  

     Therefore, we try a type-id first.  */
  cp_parser_parse_tentatively (parser);
  /* Otherwise, try a type-id.  */
  argument = cp_parser_type_id (parser);
  /* If the next token isn't a `,' or a `>', then this argument wasn't
     really finished.  It might be an expression instead.  */
  if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA)
      && cp_lexer_next_token_is_not (parser->lexer, CPP_GREATER))
    cp_parser_error (parser, "expected template-argument");
  /* If that worked, we're done.  */
  if (cp_parser_parse_definitely (parser))
    return argument;
  /* We're still not sure what the argument will be.  */
  cp_parser_parse_tentatively (parser);
  /* Try an assignment-expression.  */
  argument = cp_parser_assignment_expression (parser);
  /* If that worked, we're done.  */
  if (cp_parser_parse_definitely (parser))
    return argument;
  /* It must be an id-expression.  */
  argument = cp_parser_id_expression (parser, 
				      /*assume_typename_p=*/false);
  /* Figure out what is being referred to.  */
  return cp_parser_lookup_name (parser, argument);
}

/* Parse an explicit-instantiation.

   explicit-instantiation:
     template declaration  

   Although the standard says `declaration', what it really means is:

   explicit-instantiation:
     template decl-specifier-seq [opt] declarator [opt] ; 

   Things like `template int S<int>::i = 5, int S<double>::j;' are not
   supposed to be allowed.  A Defect Report has been filed about this
   issue.  */

static void
cp_parser_explicit_instantiation (parser)
     cp_parser *parser;
{
  bool declares_class_or_enum;
  tree decl_specifiers;
  tree attributes;
  tree type_decl = NULL_TREE;

  /* FIXME: Handle the `extern template' extension.  */

  /* Look for the `template' keyword.  */
  cp_parser_require_keyword (parser, RID_TEMPLATE, "`template'");
  /* Let the front end know that we are processing an explicit
     instantiation.  */
  begin_explicit_instantiation ();
  /* Parse a decl-specifier-seq.  */
  decl_specifiers 
    = cp_parser_decl_specifier_seq (parser,
				    CP_PARSER_FLAGS_OPTIONAL,
				    &attributes,
				    &declares_class_or_enum);
  /* FIXME: Issue an error message if attributes are present here?  */
  /* If there was exactly one decl-specifier, and it declared a class,
     and there's no declarator, then we have an explicit type
     instantiation.  */
  /* FIXME: I bet we're accepting a `friend' keyword here when we
     shouldn't.  */
  if (declares_class_or_enum)
    type_decl 
      = cp_parser_declares_only_class_p (parser,
					 decl_specifiers,
					 NULL);
  else
    type_decl = NULL_TREE;

  /* Instantiate the declaration.  */
  if (type_decl)
    do_type_instantiation (type_decl, NULL_TREE, /*complain=*/1);
  else 
    {
      tree declarator;

      /* Parse the declarator.  */
      declarator 
	= cp_parser_declarator (parser, 
				/*abstract_p=*/false, 
				/*ctor_dtor_or_conv_p=*/NULL);
      /* FIXME: We should form the declaration here, and pass that to
	 do_decl_instantiation.  There is no reason for anything but
	 the parser to be calling grokdeclarator.  */
      /* Do the explicit instantiation.  */
      do_decl_instantiation (decl_specifiers, declarator, NULL_TREE);
    }
  /* We're done with the instantiation.  */
  end_explicit_instantiation ();
}

/* Parse an explicit-specialization.

   explicit-specialization:
     template < > declaration  

   Although the standard says `declaration', what it really means is:

   explicit-specialization:
     template < > decl-specifier [opt] init-declarator [opt] 
     template < > function-definition */

/* FIXME: What about multiple levels of specialization?  */

static void
cp_parser_explicit_specialization (parser)
     cp_parser *parser;
{
  /* Look for the `template' keyword.  */
  cp_parser_require_keyword (parser, RID_TEMPLATE, "`template'");
  /* Look for the `<'.  */
  cp_parser_require (parser, CPP_LESS, "`<'");
  /* Look for the `>'.  */
  cp_parser_require (parser, CPP_GREATER, "`>'");
  /* Let the front end know that we are beginning a 
     specialization.  */
  begin_specialization ();
  /* Parse the dependent declaration.  */
  cp_parser_single_declaration (parser, 
				/*num_parameter_lists=*/1,
				NULL);
  /* We're done with the specialization.  */
  end_specialization ();
}

/* Parse a type-specifier.

   type-specifier:
     simple-type-specifier
     class-specifier
     enum-specifier
     elaborated-type-specifier
     cv-qualifier

   GNU Extension:

   type-specifier:
     __complex__

   Returns a representation of the type-specifier.  If the
   type-specifier is a keyword (like `int' or `const', or
   `__complex__') then the correspoding IDENTIFIER_NODE is returned.
   Otherwise, the type itself is returned.

   If IS_FRIEND is TRUE then this type-specifier is being declared a
   `friend'.

   If DECLARES_CLASS_OR_ENUM is non-NULL, and the type-specifier is a
   class-specifier, enum-specifier, or elaborated-type-specifier, then
   *DECLARES_CLASS_OR_ENUM is set to TRUE.  Otherwise, it is set to
   FALSE.

   If IS_CV_QUALIFIER is non-NULL, and the type-specifier is a
   cv-qualifier, then IS_CV_QUALIFIER is set to TRUE.  Otherwise, it
   is set to FALSE.  */

static tree
cp_parser_type_specifier (parser, 
			  flags, 
			  is_friend,
			  declares_class_or_enum,
			  is_cv_qualifier)
     cp_parser *parser;
     cp_parser_flags flags;
     bool is_friend;
     bool *declares_class_or_enum;
     bool *is_cv_qualifier;
{
  tree type_spec = NULL_TREE;
  cp_token *token;
  enum rid keyword;

  /* Assume this type-specifier does not declare a new type.  */
  if (declares_class_or_enum)
    *declares_class_or_enum = false;
  /* And that it does not specify a cv-qualifier.  */
  if (is_cv_qualifier)
    *is_cv_qualifier = false;
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);

  /* If we're looking at a keyword, we can use that to guide the
     production we choose.  */
  keyword = token->keyword;
  switch (keyword)
    {
      /* Any of these indicate either a class-specifier, or an
	 elaborated-type-specifier.  */
    case RID_CLASS:
    case RID_STRUCT:
    case RID_UNION:
    case RID_ENUM:
      /* Parse tentatively so that we can back up if we don't find a
	 class-specifier or enum-specifier.  */
      cp_parser_parse_tentatively (parser);
      /* Look for the class-specifier or enum-specifier.  */
      if (keyword == RID_ENUM)
	type_spec = cp_parser_enum_specifier (parser);
      else
	type_spec = cp_parser_class_specifier (parser);

      /* If that worked, we're done.  */
      if (cp_parser_parse_definitely (parser))
	{
	  if (declares_class_or_enum)
	    *declares_class_or_enum = 1;
	  return type_spec;
	}

      /* Fall through.  */

    case RID_TYPENAME:
      /* Look for an elaborated-type-specifier.  */
      type_spec = cp_parser_elaborated_type_specifier (parser,
						       is_friend);
      /* We're declaring a class or enum -- unless we're using
	 `typename'.  */
      if (declares_class_or_enum && keyword != RID_TYPENAME)
	*declares_class_or_enum = 1;
      return type_spec;

    case RID_CONST:
    case RID_VOLATILE:
      type_spec = cp_parser_cv_qualifier_opt (parser);
      /* Even though we call a routine that looks for an optional
	 qualifier, we know that there should be one.  */
      my_friendly_assert (type_spec != NULL, 20000328);
      /* This type-specifier was a cv-qualified.  */
      if (is_cv_qualifier)
	*is_cv_qualifier = true;

      return type_spec;

    case RID_COMPLEX:
      /* The `__complex__' keyword is a GNU extension.  */
      return cp_lexer_consume_token (parser->lexer)->value;

    default:
      break;
    }

  /* If we do not already have a type-specifier, assume we are looking
     at a simple-type-specifier.  */
  type_spec = cp_parser_simple_type_specifier (parser, flags);

  /* If we didn't find a type-specifier, and a type-specifier was not
     optional in this context, issue an error message.  */
  if (!type_spec && !(flags & CP_PARSER_FLAGS_OPTIONAL))
    {
      cp_parser_error (parser, "expected type specifier");
      return error_mark_node;
    }

  return type_spec;
}

/* Parse a simple-type-specifier.

   simple-type-specifier:
     :: [opt] nested-name-specifier [opt] type-name
     :: [opt] nested-name-specifier template template-id
     char
     wchar_t
     bool
     short
     int
     long
     signed
     unsigned
     float
     double
     void  

   For the various keywords, the value returned is simply the
   TREE_IDENTIFIER representing the keyword.  For the first two
   productions, the value returned is the indicated type.  */

static tree
cp_parser_simple_type_specifier (parser, flags)
     cp_parser *parser;
     cp_parser_flags flags;
{
  tree type = NULL_TREE;
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);

  /* If we're looking at a keyword, things are easy.  */
  switch (token->keyword)
    {
    case RID_CHAR:
    case RID_WCHAR:
    case RID_BOOL:
    case RID_SHORT:
    case RID_INT:
    case RID_LONG:
    case RID_SIGNED:
    case RID_UNSIGNED:
    case RID_FLOAT:
    case RID_DOUBLE:
    case RID_VOID:
      /* Consume the token.  */
      return cp_lexer_consume_token (parser->lexer)->value;

    default:
      break;
    }

  /* The type-specifier must be a user-defined type.  */
  if (!(flags & CP_PARSER_FLAGS_NO_USER_DEFINED_TYPES)) 
    {
      /* Don't gobble tokens or issue error messages if this iss an
	 optional type-specifier.  */
      if (flags & CP_PARSER_FLAGS_OPTIONAL)
	cp_parser_parse_tentatively (parser);

      /* Look for the optional `::' operator.  */
      cp_parser_global_scope_opt (parser,
				  /*current_scope_valid_p=*/false);
      /* Look for the nested-name specifier.  */
      cp_parser_nested_name_specifier_opt (parser,
					   /*typename_keyword_p=*/false);
      /* If we have asen a nested-name-specifier, and the next token
	 is `template', then we are using the template-id production.  */
      if (parser->scope 
	  && cp_lexer_next_token_is_keyword (parser->lexer, RID_TEMPLATE))
	{
	  /* FIXME: Implement this.  */
	  cp_parser_unimplemented ();
	  type = NULL_TREE;
	}
      /* Otherwise, look for a type-name.  */
      else
	{
	  type = cp_parser_type_name (parser);
	  if (type == error_mark_node)
	    type = NULL_TREE;
	  else
	    type = TREE_TYPE (type);
	}

      /* If it didn't work out, we don't have a TYPE.  */
      if ((flags & CP_PARSER_FLAGS_OPTIONAL) 
	  && !cp_parser_parse_definitely (parser))
	type = NULL_TREE;
    }

  /* If we didn't get a type-name, issue an error message.  */
  if (!type && !(flags & CP_PARSER_FLAGS_OPTIONAL))
    {
      cp_parser_error (parser, "expected type-name");
      return error_mark_node;
    }

  return type;
}

/* Parse a type-name.

   type-name:
     class-name
     enum-name
     typedef-name  

   enum-name:
     identifier

   typedef-name:
     identifier 

   Returns a TYPE_DECL for the the type.  */

static tree
cp_parser_type_name (parser)
     cp_parser *parser;
{
  tree type_decl;
  tree identifier;

  /* FIXME: We probably have to save parser->scope around the call to
     cp_parser_class_name, since a template-id could contain a
     nested-name-specifier.  */

  /* We can't know yet whether it is a class-name or not.  */
  cp_parser_parse_tentatively (parser);
  /* Try a class-name.  */
  type_decl = cp_parser_class_name (parser, 
				    /*typename_keyword_p=*/false,
				    /*template_keyword_p=*/false);
  /* If it's a class-name, we're done.  */
  if (cp_parser_parse_definitely (parser))
    return type_decl;

  /* Otherwise, it must be a typedef-name or an enum-name.  */
  identifier = cp_parser_identifier (parser);
  if (identifier == error_mark_node)
    return error_mark_node;

  /* Look up the type-name.  */
  type_decl = cp_parser_lookup_name (parser, identifier);
  /* Check to see what kind of entity it is.  */
  if (type_decl != error_mark_node
      && TREE_CODE (type_decl) == TYPE_DECL
      && (TREE_CODE (TREE_TYPE (type_decl)) == ENUMERAL_TYPE 
	  /* FIXME: How do we figure out if we're looking a
	     typedef, as opposed to a "stub decl", etc.?  */
	  || true))
    return type_decl;
  
  /* We did not find a type-name.  */
  cp_parser_error (parser, "expected type-name");
  return error_mark_node;
}


/* Parse an elaborated-type-specifier.  Note that the grammar given
   here incorporates the resolution to DR68.

   elaborated-type-specifier:
     class-key :: [opt] nested-name-specifier [opt] identifier
     class-key :: [opt] nested-name-specifier [opt] template [opt] template-id
     enum :: [opt] nested-name-specifier [opt] identifier
     typename :: [opt] nested-name-specifier identifier
     typename :: [opt] nested-name-specifier template [opt] 
       template-id 

   If IS_FRIEND is TRUE, then this elaborated-type-specifier is being
   declared `friend'.

   Returns the TYPE specified.  */

static tree
cp_parser_elaborated_type_specifier (parser, is_friend)
     cp_parser *parser;
     bool is_friend;
{
  cp_tag_kind ctk;
  tree identifier;

  /* See if we're looking at the `enum' keyword.  */
  if (cp_lexer_next_token_is_keyword (parser->lexer, RID_ENUM))
    {
      /* Consume the `enum' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Remember that it's an enumeration type.  */
      ctk = ctk_enum;
    }
  /* Or, it might be `typename'.  */
  else if (cp_lexer_next_token_is_keyword (parser->lexer,
					   RID_TYPENAME))
    {
      /* Consume the `typename' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Remember that it's a `typename' type.  */
      ctk = ctk_typename;
    }
  /* Otherwise it must be a class-key.  */
  else
    {
      ctk = cp_parser_class_key (parser);
      if (ctk == ctk_none)
	return error_mark_node;
    }

  /* Look for the `::' operator.  */
  cp_parser_global_scope_opt (parser, 
			      /*current_scope_valid_p=*/false);
  /* Look for the nested-name-specifier.  */
  if (ctk == ctk_typename)
    cp_parser_nested_name_specifier (parser,
				     /*typename_keyword_p=*/true);
  else
    /* Even though `typename' is not present, the prposed resolution
       to Core Issue 180 says that in `class A::B', `B' should be
       considered a type-name.  */
    cp_parser_nested_name_specifier_opt (parser,
					 /*typename_keyword_p=*/true);
  /* For everything but enumeration types, consider a template-id.  */
  if (ctk != ctk_enum)
    {
      bool template_p = false;
      tree decl;

      /* Allow the `template' keyword.  */
      if (cp_lexer_next_token_is_keyword (parser->lexer, 
					  RID_TEMPLATE))
	{
	  /* Remember that we saw the `template' keyword.  */
	  template_p = true;
	  /* Consume it.  */
	  cp_lexer_consume_token (parser->lexer);
	}

      /* If we didn't see `template', we don't know if there's a
         template-id or not.  */
      if (!template_p)
	cp_parser_parse_tentatively (parser);
      /* Parse the template-id.  */
      decl = cp_parser_template_id (parser, template_p);
      /* If we didn't find a template-id, look for an ordinary
         identifier.  */
      if (!template_p && !cp_parser_parse_definitely (parser))
	identifier = cp_parser_identifier (parser);
      else
	return TREE_TYPE (decl);
    }
  /* For an enumeration type, consider only a plain identifier.  */
  else
    identifier = cp_parser_identifier (parser);

  if (identifier == error_mark_node)
    return error_mark_node;

  /* For a `typename', we needn't call xref_tag.  */
  if (ctk == ctk_typename)
    return make_typename_type (parser->scope, identifier, 
			       /*complain=*/1);

  /* An elaborated-type-specifier sometimes introduces a new type and
     sometimes names an existing type.  Normally, the rule is that it
     introduces a new type only if there is not an existing type of
     the same name already in scope.  For example, given:

       struct S {};
       void f() { struct S s; }

     the `struct S' in the body of `f' is the same `struct S' as in
     the global scope; the existing definition is used.  However, if
     there were no global declaration, this would introduce a new 
     local class named `S'.

     An exception to this rule applies to the following code:

       namespace N { struct S; }

     Here, the elaborated-type-specifier names a new type
     unconditionally; even if there is already an `S' in the
     containing scope this declaration names a new type.
     This exception only applies if the elaborated-type-specifier
     forms the complete declaration:

       [class.name] 

       A declaration consisting solely of `class-key identifier ;' is
       either a redeclaration of the name in the current scope or a
       forward declaration of the identifier as a class name.  It
       introduces the name into the current scope.

     We are in this situation precisely when the next token is a `;'.
     
     An exception to the exception is that a `friend' declaration does
     *not* name a new type; i.e., given:

       struct S { friend struct T; };

     `T' is not a new type in the scope of `S'.  */

  return xref_tag (ctk, identifier, 
		   (is_friend 
		    || cp_lexer_next_token_is_not (parser->lexer, 
						   CPP_SEMICOLON)));
}

/* Parse an enum-specifier.

   enum-specifier:
     enum identifier [opt] { enumerator-list [opt] }

   Returns an ENUM_TYPE representing the enumeration.  */

static tree
cp_parser_enum_specifier (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree identifier;
  tree type;

  /* Look for the `enum' keyword.  */
  if (!cp_parser_require_keyword (parser, RID_ENUM, "`enum'"))
    return error_mark_node;
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);

  /* See if it is an identifier.  */
  if (token->type == CPP_NAME)
    identifier = cp_parser_identifier (parser);
  /* Otherwise, make up a name for the type.  */
  else
    identifier = make_anon_name ();

  /* Look for the `{'.  */
  if (!cp_parser_require (parser, CPP_OPEN_BRACE, "`{'"))
    return error_mark_node;

  /* Create the new type.  */
  type = start_enum (identifier);

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's not a `}', then there are some enumerators.  */
  if (token->type != CPP_CLOSE_BRACE)
    cp_parser_enumerator_list (parser, type);
  /* Look for the `}'.  */
  cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");

  /* Finish up the enumeration.  */
  finish_enum (type);

  return type;
}

/* Parse an enumerator-list.  The enumerators all have the indicated
   TYPE.  

   enumerator-list:
     enumerator-definition
     enumerator-list , enumerator-definition  */

static void
cp_parser_enumerator_list (parser, type)
     cp_parser *parser;
     tree type;
{
  while (true)
    {
      cp_token *token;

      /* Parse an enumerator-definition.  */
      cp_parser_enumerator_definition (parser, type);
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not a `,', then we've reached the end of the 
	 list.  */
      if (token->type != CPP_COMMA)
	break;
      /* Otherwise, consume the `,' and keep going.  */
      cp_lexer_consume_token (parser->lexer);
    }
}

/* Parse an enumerator-definition.  The enumerator has the indicated
   TYPE.

   enumerator-definition:
     enumerator
     enumerator = constant-expression
    
   enumerator:
     identifier  */

static void
cp_parser_enumerator_definition (parser, type)
     cp_parser *parser;
     tree type;
{
  cp_token *token;
  tree identifier;
  tree value;

  /* Look for the identifier.  */
  identifier = cp_parser_identifier (parser);
  if (identifier == error_mark_node)
    return;
  
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's an `=', then there's an explicit value.  */
  if (token->type == CPP_EQ)
    {
      /* Consume the `=' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the value.  */
      value = cp_parser_constant_expression (parser);
    }
  else
    value = NULL_TREE;

  /* Create the enumerator.  */
  build_enumerator (identifier, value, type);
}

/* Parse a namespace-name.

   namespace-name:
     original-namespace-name
     namespace-alias

   Returns the NAMESPACE_DECL for the namespace.  */

static tree
cp_parser_namespace_name (parser)
     cp_parser *parser;
{
  tree identifier;
  tree namespace_decl;

  /* Get the name of the namespace.  */
  identifier = cp_parser_identifier (parser);
  if (identifier == error_mark_node)
    return error_mark_node;

  /* Look up the identifier in the currently active scope.  */
  namespace_decl = cp_parser_lookup_name (parser, identifier);
  /* If it's not a namespace, issue an error.  */
  if (namespace_decl == error_mark_node
      || TREE_CODE (namespace_decl) != NAMESPACE_DECL)
    {
      cp_parser_error (parser, "expected namespace-name");
      namespace_decl = error_mark_node;
    }
  
  return namespace_decl;
}

/* Parse a namespace-definition.

   namespace-definition:
     named-namespace-definition
     unnamed-namespace-definition  

   named-namespace-definition:
     original-namespace-definition
     extension-namespace-definition

   original-namespace-definition:
     namespace identifier { namespace-body }
   
   extension-namespace-definition:
     namespace original-namespace-name { namespace-body }
 
   unnamed-namespace-definition:
     namespace { namespace-body } */

static void
cp_parser_namespace_definition (parser)
     cp_parser *parser;
{
  tree identifier;

  /* Look for the `namespace' keyword.  */
  cp_parser_require_keyword (parser, RID_NAMESPACE, "`namespace'");

  /* Get the name of the namespace.  We do not attempt to distinguish
     between an original-namespace-definition and an
     extension-namespace-definition at this point.  The semantic
     analysis routines are responsible for that.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_NAME))
    identifier = cp_parser_identifier (parser);
  else
    identifier = NULL_TREE;

  /* Look for the `{' to start the namespace.  */
  cp_parser_require (parser, CPP_OPEN_BRACE, "`{'");
  /* Start the namespace.  */
  push_namespace (identifier);
  /* Parse the body of the namespace.  */
  cp_parser_namespace_body (parser);
  /* Finish the namespace.  */
  pop_namespace ();
  /* Look for the final `}'.  */
  cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");
}

/* Parse a namespace-body.

   namespace-body:
     declaration-seq [opt]  */

static void
cp_parser_namespace_body (parser)
     cp_parser *parser;
{
  cp_parser_declaration_seq_opt (parser);
}

/* Parse a namespace-alias-definition.

   namespace-alias-definition:
     namespace identifier = qualified-namespace-specifier ;  */

static void
cp_parser_namespace_alias_definition (parser)
     cp_parser *parser;
{
  tree identifier;
  tree namespace_specifier;

  /* Look for the `namespace' keyword.  */
  cp_parser_require_keyword (parser, RID_NAMESPACE, "`namespace'");
  /* Look for the identifier.  */
  identifier = cp_parser_identifier (parser);
  if (identifier == error_mark_node)
    return;
  /* Look for the `=' token.  */
  cp_parser_require (parser, CPP_EQ, "`='");
  /* Look for the qualified-namespace-specifier.  */
  namespace_specifier 
    = cp_parser_qualified_namespace_specifier (parser);
  /* Look for the `;' token.  */
  cp_parser_require (parser, CPP_SEMICOLON, "`;'");

  /* Register the alias in the symbol table.  */
  do_namespace_alias (identifier, namespace_specifier);
}

/* Parse a qualified-namespace-specifier.

   qualified-namespace-specifier:
     :: [opt] nested-name-specifier [opt] namespace-name

   Returns a NAMESPACE_DECL corresponding to the specified
   namespace.  */

static tree
cp_parser_qualified_namespace_specifier (parser)
     cp_parser *parser;
{
  /* Look for the optional `::'.  */
  cp_parser_global_scope_opt (parser, 
			      /*current_scope_valid_p=*/false);

  /* Look for the optional nested-name-specifier.  */
  cp_parser_nested_name_specifier_opt (parser,
				       /*typename_keyword_p=*/false);

  return cp_parser_namespace_name (parser);
}

/* Parse a using-declaration.

   using-declaration:
     using typename [opt] :: [opt] nested-name-specifier unqualified-id ;
     using :: unqualified-id ;  */

static void
cp_parser_using_declaration (parser)
     cp_parser *parser;
{
  cp_token *token;
  bool typename_p = false;
  bool global_scope_p;
  tree identifier;
  tree scope;

  /* Look for the `using' keyword.  */
  cp_parser_require_keyword (parser, RID_USING, "`using'");
  
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* See if it's `typename'.  */
  if (token->keyword == RID_TYPENAME)
    {
      /* Remember that we've seen it.  */
      typename_p = true;
      /* Consume the `typename' token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* Look for the optional global scope qualification.  */
  global_scope_p 
    = (cp_parser_global_scope_opt (parser,
				   /*current_scope_valid_p=*/false) 
       != NULL_TREE);

  /* If we saw `typename', or didn't see `::', then there must be a
     nested-name-specifier present.  */
  if (typename_p || !global_scope_p)
    cp_parser_nested_name_specifier (parser, typename_p);
  /* Otherwise, we could be in either of the two productions.  In that
     case, treat the nested-name-specifier as optional.  */
  else
    cp_parser_nested_name_specifier_opt (parser,
					 /*typename_keyword_p=*/false);

  /* Parse the unqualified-id.  */
  identifier = cp_parser_unqualified_id (parser);

  /* If there was a nested-name-specifier, create a SCOPE_REF to
     represent that.  */
  /* FIXME: It would make more sense for the parser to resolve the
     name, and hand the resulting declaration directly to the
     functions that process using-declarations.  */
  if (parser->scope)
    identifier = build_nt (SCOPE_REF, parser->scope, identifier);

  /* FIXME: Write a single function in semantics.c that performs this
     dispatch.  */
  /* The function we call to handle a using-declaration is different
     depending on what scope we are in.  */
  scope = current_scope ();
  if (!scope)
    do_toplevel_using_decl (identifier);
  else if (TYPE_P (scope))
    do_class_using_decl (identifier);
  else
    do_local_using_decl (identifier);

  /* Look for the final `;'.  */
  cp_parser_require (parser, CPP_SEMICOLON, "`;'");
}

/* Parse a using-directive.  
 
   using-directive:
     using namespace :: [opt] nested-name-specifier [opt]
       namespace-name ;  */

static void
cp_parser_using_directive (parser)
     cp_parser *parser;
{
  tree namespace_decl;

  /* Look for the `using' keyword.  */
  cp_parser_require_keyword (parser, RID_USING, "`using'");
  /* And the `namespace' keyword.  */
  cp_parser_require_keyword (parser, RID_NAMESPACE, "`namespace'");
  /* Look for the optional `::' operator.  */
  cp_parser_global_scope_opt (parser, /*current_scope_valid_p=*/false);
  /* And the optional nested-name-sepcifier.  */
  cp_parser_nested_name_specifier_opt (parser,
				       /*typename_keyword_p=*/false);
  /* Get the namespace being used.  */
  namespace_decl = cp_parser_namespace_name (parser);
  /* Update the symbol table.  */
  do_using_directive (namespace_decl);
  /* Look for the final `;'.  */
  cp_parser_require (parser, CPP_SEMICOLON, "`;'");
}

/* Parse an asm-definition.

   asm-definition:
     asm ( string-literal ) ;  

   GNU Extension:

     asm volatile [opt] ( string-literal ) ;
     asm volatile [opt] ( string-literal : asm-operand-list [opt] ) ;
     asm volatile [opt] ( string-literal : asm-operand-list [opt]
                          : asm-operand-list [opt] ) ;
     asm volatile [opt] ( string-literal : asm-operand-list [opt] 
                          : asm-operand-list [opt] 
                          : asm-operand-list [opt] ) ;  */

static void
cp_parser_asm_definition (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree string;
  tree outputs = NULL_TREE;
  tree inputs = NULL_TREE;
  tree clobbers = NULL_TREE;
  tree asm_stmt;
  bool volatile_p = false;
  bool extended_p = false;

  /* Look for the `asm' keyword.  */
  cp_parser_require_keyword (parser, RID_ASM, "`asm'");
  /* See if the next token is `volatile'.  */
  if (cp_parser_allow_gnu_extensions_p (parser)
      && cp_lexer_next_token_is_keyword (parser->lexer, RID_VOLATILE))
    {
      /* Remember that we saw the `volatile' keyword.  */
      volatile_p = true;
      /* Consume the token.  */
      cp_lexer_consume_token (parser->lexer);
    }
  /* Look for the opening `('.  */
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
  /* Look for the string.  */
  token = cp_parser_require (parser, CPP_STRING, "asm body");
  if (!token)
    return;
  string = token->value;
  /* If we're allowing GNU extensions, check for the extended assembly
     syntax.  Unfortunately, the `:' tokens need not be separated by 
     a space in C, and so, for compatibility, we tolerate that here
     too.  Doing that means that we have to treat the `::' operator as
     two `:' tokens.  */
  if (cp_parser_allow_gnu_extensions_p (parser))
    {
      bool inputs_p = false;
      bool clobbers_p = false;

      /* Look for outputs.  */
      if (cp_lexer_next_token_is (parser->lexer, CPP_COLON))
	{
	  /* Consume the `:'.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Parse the output-operands.  */
	  if (cp_lexer_next_token_is_not (parser->lexer, 
					  CPP_COLON)
	      && cp_lexer_next_token_is_not (parser->lexer,
					     CPP_SCOPE))
	    outputs = cp_parser_asm_operand_list (parser);
	}
      /* If the next token is `::', there are no outputs, and the
	 next token is the beginning of the inputs.  */
      else if (cp_lexer_next_token_is (parser->lexer, CPP_SCOPE))
	{
	  /* Consume the `::' token.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* The inputs are coming next.  */
	  inputs_p = true;
	}

      /* Look for inputs.  */
      if (inputs_p
	  || cp_lexer_next_token_is (parser->lexer, CPP_COLON))
	{
	  /* Consume the `:'.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Parse the output-operands.  */
	  if (cp_lexer_next_token_is_not (parser->lexer, 
					  CPP_COLON)
	      && cp_lexer_next_token_is_not (parser->lexer,
					     CPP_SCOPE))
	    inputs = cp_parser_asm_operand_list (parser);
	}
      else if (cp_lexer_next_token_is (parser->lexer, CPP_SCOPE))
	{
	  /* Consume the `::' token.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* The clobbers are coming next.  */
	  clobbers_p = true;
	}

      /* Look for clobbers.  */
      if (clobbers_p 
	  || cp_lexer_next_token_is (parser->lexer, CPP_COLON))
	{
	  /* Consume the `:'.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Parse the clobbers.  */
	  clobbers = cp_parser_asm_clobber_list (parser);
	}

      /* The extended syntax was used.  */
      extended_p = true;
    }
  /* Look for the closing `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  /* Create the ASM_STMT.  */
  asm_stmt = 
    finish_asm_stmt (volatile_p 
		     ? ridpointers[(int) RID_VOLATILE] : NULL_TREE,
		     string, outputs, inputs, clobbers);
  /* If the extended syntax was not used, mark the ASM_STMT.  */
  if (!extended_p)
    ASM_INPUT_P (asm_stmt) = 1;
}

/* Declarators [gram.dcl.decl] */

/* Parse an init-declarator.

   init-declarator:
     declarator initializer [opt]

   GNU Extension:

   init-declarator:
     declarator asm-specification [opt] attributes [opt] initializer [opt]

   The DECL_SPECIFIERS and PREFIX_ATTRIBUTES apply to this declarator.
   Returns a reprsentation of the entity declared.
   NUM_TEMPLATE_PARAMETER_LISTS is the number of template parameter
   lists that apply to the declaration.  */

static tree
cp_parser_init_declarator (parser, 
			   decl_specifiers, 
			   prefix_attributes,
			   num_template_parameter_lists)
     cp_parser *parser;
     tree decl_specifiers;
     tree prefix_attributes;
     unsigned num_template_parameter_lists;
{
  cp_token *token;
  tree declarator;
  tree attributes;
  tree asm_specification;
  tree initializer;
  tree decl;
  tree scope;
  bool is_initialized;
  bool is_parenthesized_init;
  bool ctor_dtor_or_conv_p;
  bool member_p;

  /* Parse the declarator.  */
  declarator 
    = cp_parser_declarator (parser,
			    /*abstract_p=*/false,
			    &ctor_dtor_or_conv_p);
  if (declarator == error_mark_node)
    return error_mark_node;

  /* [dcl.dcl]

     Only in function declarations for constructors, destructors, and
     type conversions can the decl-specifier-seq be omitted.  */
  if (!decl_specifiers && !ctor_dtor_or_conv_p)
    {
      cp_parser_error (parser, 
		       "expected constructor, destructor, or type conversion");
      return error_mark_node;
    }

  /* If we're allowing GNU extensions, look for an asm-specification
     and attributes.  */
  if (cp_parser_allow_gnu_extensions_p (parser))
    {
      /* Look for an asm-specification.  */
      asm_specification = cp_parser_asm_specification_opt (parser);
      /* And attributes.  */
      attributes = cp_parser_attributes_opt (parser);
    }
  else
    {
      asm_specification = NULL_TREE;
      attributes = NULL_TREE;
    }

  /* If the next token is an `=' or a `(', then there is an
     initializer for this declaration.  */
  token = cp_lexer_peek_token (parser->lexer);
  is_initialized = (token->type == CPP_EQ 
		     || token->type == CPP_OPEN_PAREN);
  /* If the init-declarator isn't initialized and isn't followed by a
     `,' or `;', it's not a valid init-declarator.  */
  if (!is_initialized 
      && token->type != CPP_COMMA
      && token->type != CPP_SEMICOLON)
    {
      cp_parser_error (parser, "expected init-declarator");
      return error_mark_node;
    }

  /* Because start_decl has side-effects, we should only call it if we
     know we're going ahead.  By this point, we know that we cannot
     possibly be looking at any other construct.  */
  cp_parser_commit_to_tentative_parse (parser);

  /* Check that the number of template-parameter-lists is OK.  */
  if (!cp_parser_check_template_parameters 
      (declarator,
       num_template_parameter_lists))
    /* FIXME: Skip tokens?  */
    cp_parser_unimplemented ();
  /* Figure out what scope the entity declared by the DECLARATOR is
     located in.  */
  scope = cp_parser_get_scope_of_declarator (declarator);
  /* Enter the SCOPE.  That way unqualified names appearing in the
     initializer will be looked up in SCOPE.  */
  if (scope)
    push_scope (scope);
  /* Enter the newly declared entry in the symbol table.  If we're
     processing a declaration in a class-specifier, we wait until
     after processing the initializer.  */
  /* FIXME: Does that really work?  Will names from the declaration
     actually be in scope?  I bet this program:

       struct S {
       static const int i = sizeof (i);

     fails, and did with old versions of GCC, too.  */
  member_p = (current_class_type 
	      && TYPE_BEING_DEFINED (current_class_type));
  if (!member_p)
    decl = start_decl (declarator,
		       decl_specifiers,
		       is_initialized,
		       attributes,
		       prefix_attributes);

  /* Parse the initializer.  */
  if (is_initialized)
    initializer = cp_parser_initializer (parser, 
					 &is_parenthesized_init);
  else
    {
      initializer = NULL_TREE;
      is_parenthesized_init = false;
    }

  /* For an in-class declaration, use `grokfield' to create the
     declaration.  */
  /* FIXME: Correct the handling of attributes here.  */
  /* FIXME: Really, there should be only one function to call here:
     start_decl.  It should be able to figure out that it is looking
     at a member declaration from context.  */
  if (member_p)
    decl = grokfield (declarator, decl_specifiers,
		      initializer, /*asmspec=*/NULL_TREE,
		      /*attributes=*/NULL_TREE);

  /* Finish processing the declaration.  But, skip friend
     declarations.  */
  if (!(member_p && !decl))
    cp_finish_decl (decl, 
		    initializer, 
		    asm_specification,
		    /* If the initializer is in parentheses, then this is
		       a direct-initialization, which means that an
		       `explicit' constructor is OK.  Otherwise, an
		       `explicit' constructor cannot be used.  */
		    ((is_parenthesized_init || !is_initialized)
		     ? LOOKUP_NONE : LOOKUP_ONLYCONVERTING));

  /* Leave the SCOPE.  */
  if (scope)
    pop_scope (scope);

  return decl;
}

/* Parse a declarator.
   
   declarator:
     direct-declarator
     ptr-operator declarator  

   abstract-declarator:
     ptr-operator abstract-declarator [opt]
     direct-abstract-declarator

   Returns a representation of the declarator.  If the declarator has
   the form `* declarator', then an INDIRECT_REF is returned, whose
   only operand is the sub-declarator.  Analagously, `& declarator' is
   represented as an ADDR_EXPR.  For `X::* declarator', a SCOPE_REF is
   used.  The first operand is the TYPE for `X'.  The second operand
   is an INDIRECT_REF whose operand is the sub-declarator.

   Otherwise, the reprsentation is as for a direct-declarator.

   (It would be better to define a structure type to represent
   declarators, rather than abusing `tree' nodes to represent
   declarators.  That would be much clearer and save some memory.
   There is no reason for declarators to be garbage-collected, for
   example; they are created during parser and no longer needed after
   `grokdeclarator' has been called.)

   For a ptr-operatorthat has the optional cv-qualifier-seq,
   cv-qualifiers will be stored in the TREE_TYPE of the INDIRECT_REF
   node.

   If CTOR_DTOR_OR_CONV_P is not NULL, *CTOR_DTOR_OR_CONV_P is set to
   true if this declarator represents a constructor, destructor, or
   type conversion.  Otherwise, it is set to false.  

   (The reason for CTOR_DTOR_OR_CONV_P is that a declaration must have
   a decl-specifier-seq unless it declares a constructor, destructor,
   or conversion.  It might seem that we could check this condition in
   semantic analysis, rather than parsing, but that makes it difficult
   to handle something like `f()'.  We want to notice that there are
   no decl-specifiers, and therefore realize that this is an
   expression, not a declaration.)  */

/* FIXME: Name this function something else, and create
   cp_parser_declarator and cp_parser_abstract_declarator stubs.  */

static tree
cp_parser_declarator (parser, abstract_p, ctor_dtor_or_conv_p)
     cp_parser *parser;
     bool abstract_p;
     bool *ctor_dtor_or_conv_p;
{
  cp_token *token;
  tree declarator;
  enum tree_code code;
  tree cv_qualifier_seq;
  tree class_type;
  

  /* Assume this is not a constructor, destructor, or type-conversion
     operator.  */
  if (ctor_dtor_or_conv_p)
    *ctor_dtor_or_conv_p = false;

  /* FIXME: Handle the complex ptr-operator case involving a
     nested-name-specifier.  */
 
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Check for the ptr-operator production.  */
  cp_parser_parse_tentatively (parser);
  /* Parse the ptr-operator.  */
  code = cp_parser_ptr_operator (parser, 
				 &class_type, 
				 &cv_qualifier_seq);
  /* If that worked, then we have a ptr-operator.  */
  if (cp_parser_parse_definitely (parser))
    {
      /* The dependent declarator is optional if we are parsing an
	 abstract-declarator.  */
      if (abstract_p)
	cp_parser_parse_tentatively (parser);

      /* Parse the dependent declarator.  */
      declarator = cp_parser_declarator (parser, abstract_p,
					 /*ctor_dtor_or_conv_p=*/NULL);

      /* If we are parsing an abstract-declarator, we must handle the
	 case where the dependent declarator is absent.  */
      if (abstract_p && !cp_parser_parse_definitely (parser))
	declarator = NULL_TREE;
	
      /* Build the representation of the ptr-operator.  */
      if (code == INDIRECT_REF)
	declarator = make_pointer_declarator (cv_qualifier_seq, 
					      declarator);
      else
	declarator = make_reference_declarator (cv_qualifier_seq,
						declarator);
      /* Handle the pointer-to-member case.  */
      if (class_type)
	declarator = build_nt (SCOPE_REF, class_type, declarator);
    }
  /* Everything else is a direct-declarator.  */
  else
    declarator = cp_parser_direct_declarator (parser, 
					      abstract_p,
					      ctor_dtor_or_conv_p);

  return declarator;
}

/* Parse a direct-declarator or direct-abstract-declarator.

   direct-declarator:
     declarator-id
     direct-declarator ( parameter-declaration-clause )
       cv-qualifier-seq [opt] 
       exception-specification [opt]
     direct-declarator [ constant-expression [opt] ]
     ( declarator )  

   direct-abstract-delcarator:
     direct-abstract-declarator [opt]
       ( parameter-declaration-clause ) 
       cv-qualifier-seq [opt]
       exception-specification [opt]
     direct-abstract-declarator [opt] [ constant-expression [opt] ]
     ( abstract-declarator )

   Returns a representation of the declarator.  ABSTRACT_P is TRUE if
   we are parsing a direct-abstract-declarator; FALSE if we are
   parsing a direct-declarator.  CTOR_DTOR_OR_CONV_P is as for 
   cp_parser_declarator.

   For the declarator-id production, the representation is as for an
   id-expression, except that a qualified name is represented as a
   SCOPE_REF.  A function-declarator is represented as a CALL_EXPR;
   see the documentation of the FUNCTION_DECLARATOR_* macros for
   information about how to find the various declarator components.
   An array-declarator is represented as an ARRAY_REF.  The
   direct-declarator is the first operand; the constant-expression
   indicating the size of the array is the second operand.  */

static tree
cp_parser_direct_declarator (parser, abstract_p, ctor_dtor_or_conv_p)
     cp_parser *parser;
     bool abstract_p;
     bool *ctor_dtor_or_conv_p;
{
  cp_token *token;
  tree declarator;
  tree scope = NULL_TREE;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Find the initial direct-declarator.  It might be a parenthesized
     declarator.  */
  if (token->type == CPP_OPEN_PAREN)
    {
      /* Consume the `('.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the nested declarator.  */
      declarator 
	= cp_parser_declarator (parser, abstract_p, ctor_dtor_or_conv_p);
      /* Expect a `)'.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
    }
  /* Otherwise, for a non-abstract declarator, there should be a
     declarator-id.  */
  else if (!abstract_p)
    {
      declarator = cp_parser_declarator_id (parser);
      /* FIXME: We should check for, and reject, qualified-ids in
	 places where they are not permitted.  For example,

	   void f (int S::i);

	 should result in an error here, and we should not try to
	 enter the scope of S below.  We need some information about
	 whether this is a top-level declarator, or part of the
	 parameter-declaration-clause.  */
      /* Any names that appear after the declarator-id for a member
	 are looked up in the containing scope.  */
      if (TREE_CODE (declarator) == SCOPE_REF)
	{
	  /* Remember what scope we entered so that we can leave it
	     when we are done with the declarator.  */
	  scope = TREE_OPERAND (declarator, 0);
	  /* In the declaration of a member of a template class
	     outside of the class itself, the SCOPE will sometimes be
	     a TYPENAME_TYPE.  For example, given:
	     
               template <typename T>
	       int S<T>::R::i = 3;

             the SCOPE will be a TYPENAME_TYPE for `S<T>::R'.  In this
	     context, we must resolve S<T>::R to an ordinary type,
	     rather than a typename type.

	     The reason we normally avoid resolving TYPENAME_TYPEs is
	     that a specialization of `S' might render `S<T>::R' not a
	     type.  However, if `S' is specialized, then this `i' will
	     not be used, so there is no harm in resolving the types
	     here.  */
	  if (TREE_CODE (scope) == TYPENAME_TYPE)
	    {
	      /* Resolve the TYPENAME_TYPE.  */
	      scope = cp_parser_resolve_typename_type (scope);
	      /* If that failed, the declarator is invalid.  */
	      if (scope == error_mark_node)
		return error_mark_node;
	      /* Build a new DECLARATOR.  */
	      declarator = build_nt (SCOPE_REF, 
				     scope, 
				     TREE_OPERAND (declarator, 1));
	    }
	  /* Enter the SCOPE.  Unqualified names in the
	     parameter-declaration-clause, for example, should be
	     looked up in SCOPE.  */
	  push_scope (scope);
	}

      /* Check to see whether the declarator-id names a constructor, 
	 destructor, or conversion.  */
      if (at_class_scope_p ())
	{
	  tree unqualified_name;
	  
	  /* Get the unqualified part of the name.  */
	  if (TREE_CODE (declarator) == SCOPE_REF)
	    unqualified_name = TREE_OPERAND (declarator, 1);
	  else
	    unqualified_name = declarator;

	  /* See if it names the constructor.  */
	  if (ctor_dtor_or_conv_p
	      && constructor_name_p (unqualified_name,
				     current_class_type))
	    *ctor_dtor_or_conv_p = true;

	  /* FIXME: Check for conversions.  */
	}
    }
  /* But for an abstract declarator, the initial direct-declarator can
     be omitted.  */
  else
    declarator = NULL_TREE;

  /* Now, parse function-declarators and array-declarators until there
     are no more.  */
  while (true)
    {
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's a `[', we're looking at an array-declarator.  */
      if (token->type == CPP_OPEN_SQUARE)
	{
	  tree bounds;

	  /* Consume the `['.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Peek at the next token.  */
	  token = cp_lexer_peek_token (parser->lexer);
	  /* If the next token is `]', then there is no
	     constant-expression.  */
	  if (token->type != CPP_CLOSE_SQUARE)
	    bounds = cp_parser_constant_expression (parser);
	  else
	    bounds = NULL_TREE;
	  /* Look for the closing `]'.  */
	  cp_parser_require (parser, CPP_CLOSE_SQUARE, "`]'");

	  declarator = build_nt (ARRAY_REF, declarator, bounds);
	}
      /* If it's a `(', we're looking at a function-declarator.  */
      else if (token->type == CPP_OPEN_PAREN)
	{
	  /* A function-declarator.  Or maybe not.  Consider, for
	     example:

	       int i (int);
	       int i (3);

	     The first is the declaration of a function while the
	     second is a the definition of a variable, including its
	     initializer.

	     Having seen only the parenthesis, we cannot know which of
	     these two alternatives should be selected.  Even more
	     complex are examples like:

               int i (int (a));
	       int i (int (3));

	     The former is a function-declaration; the latter is a
	     variable initialization.  

	     First, we attempt to parse a parameter-declaration
	     clause.  If this works, then we continue; otherwise, we
	     replace the tokens consumed in the process and continue.  */
	  tree params;

	  /* We are now parsing tentatively.  */
	  cp_parser_parse_tentatively (parser);
	  
	  /* Consume the `('.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Parse the parameter-declaration-clause.  */
	  params = cp_parser_parameter_declaration_clause (parser);
	  
	  /* If all went well, parse the cv-qualifier-seq and the
	     exception-specification.  */
	  if (cp_parser_parse_definitely (parser))
	    {
	      tree cv_qualifiers;
	      tree exception_specification;

	      /* Consume the `)'.  */
	      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

	      /* Parse the cv-qualifier-seq.  */
	      cv_qualifiers = cp_parser_cv_qualifier_seq_opt (parser);
	      /* And the exception-specification.  */
	      exception_specification 
		= cp_parser_exception_specification_opt (parser);

	      /* Create the function-declarator.  */
	      declarator 
		= make_function_declarator (declarator,
					    params,
					    cv_qualifiers,
					    exception_specification);
	    }
	  /* Otherwise, we must be done with the declarator.  */
	  else
	    break;
	}
      /* Otherwise, we're done with the declarator.  */
      else
	break;
    }
  
  /* If we entered a scope, we must exit it now.  */
  if (scope)
    pop_scope (scope);

  return declarator;
}

/* Parse a ptr-operator.  

   ptr-operator:
     * cv-qualifier-seq [opt]
     &
     :: [opt] nested-name-specifier * cv-qualifier-seq [opt]

   GNU Extension:
     & restrict

   Returns INDIRECT_REF if a pointer, or pointer-to-member, was
   used.  Returns ADDR_EXPR if a reference was used.  In the
   case of a pointer-to-member, *TYPE is filled in with the 
   TYPE containing the member.  *CV_QUALIFIER_SEQ is filled in
   with the cv-qualifier-seq, or NULL_TREE, if there are no
   cv-qualifiers.  Returns ERROR_MARK if an error occurred.  */
   
static enum tree_code
cp_parser_ptr_operator (parser, type, cv_qualifier_seq)
     cp_parser *parser;
     tree *type;
     tree *cv_qualifier_seq;
{
  enum tree_code code = ERROR_MARK;
  cp_token *token;

  /* Assume that it's not a pointer-to-member.  */
  *type = NULL_TREE;
  /* And that there are no cv-qualifiers.  */
  *cv_qualifier_seq = NULL_TREE;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's a `*' or `&' we have a pointer or reference.  */
  if (token->type == CPP_MULT || token->type == CPP_AND)
    {
      /* Remember which ptr-operator we were processing.  */
      code = (token->type == CPP_AND ? ADDR_EXPR : INDIRECT_REF);

      /* Consume the `*' or `&'.  */
      cp_lexer_consume_token (parser->lexer);

      /* A `*' can be followed by a cv-qualifier-seq.  */
      if (code == INDIRECT_REF)
	*cv_qualifier_seq = cp_parser_cv_qualifier_seq_opt (parser);
      /* As a GNU extension, `restrict' is allowed after a `&'.  */
      else if (cp_parser_allow_gnu_extensions_p (parser))
	{
	  /* Peek at the next token.  */
	  token = cp_lexer_peek_token (parser->lexer);
	  /* Handle `restrict'.  */
	  if (token->keyword == RID_RESTRICT)
	    {
	      /* Consume the token.  */
	      cp_lexer_consume_token (parser->lexer);
	      /* And remember that we saw it.  */
	      *cv_qualifier_seq = build_tree_list (NULL_TREE,
						   token->value);
	    }
	}
    }
  else
    {
      /* Try the pointer-to-member case.  */
      cp_parser_parse_tentatively (parser);
      /* Look for the optional `::' operator.  */
      cp_parser_global_scope_opt (parser,
				  /*current_scope_valid_p=*/false);
      /* Look for the nested-name specifier.  */
      cp_parser_nested_name_specifier (parser,
				       /*typename_keyword_p=*/false);
      /* If we found it, and the next token is a `*', then we are
	 indeed looking at a pointer-to-member operator.  */
      if (!cp_parser_error_occurred (parser)
	  && cp_parser_require (parser, CPP_MULT, "`*'"))
	{
	  /* The type of which the member is a member is given by the
	     current SCOPE.  */
	  *type = parser->scope;
	  /* The next name will not be qualified.  */
	  parser->scope = NULL_TREE;
	  /* Indicate that the `*' operator was used.  */
	  code = INDIRECT_REF;
	  /* Look for the optional cv-qualifier-seq.  */
	  *cv_qualifier_seq = cp_parser_cv_qualifier_seq_opt (parser);
	}
      /* If that didn't work we don't have a ptr-operator.  */
      if (!cp_parser_parse_definitely (parser))
	cp_parser_error (parser, "expected ptr-operator");
    }

  return code;
}

/* Parse an (optional) cv-qualifier-seq.

   cv-qualifier-seq:
     cv-qualifier cv-qualifier-seq [opt]  

   Returns a TREE_LIST.  The TREE_VALUE of each node is the
   representation of a cv-qualifier.  */

static tree
cp_parser_cv_qualifier_seq_opt (parser)
     cp_parser *parser;
{
  tree cv_qualifiers = NULL_TREE;
  
  while (true)
    {
      tree cv_qualifier;

      /* Look for the next cv-qualifier.  */
      cv_qualifier = cp_parser_cv_qualifier_opt (parser);
      /* If we didn't find one, we're done.  */
      if (!cv_qualifier)
	break;

      /* Add this cv-qualifier to the list.  */
      cv_qualifiers 
	= tree_cons (NULL_TREE, cv_qualifier, cv_qualifiers);
    }

  /* We built up the list in reverse order.  */
  return nreverse (cv_qualifiers);
}

/* Parse an (optional) cv-qualifier.

   cv-qualifier:
     const
     volatile  

   GNU Extension:
     __restrict__ */

static tree
cp_parser_cv_qualifier_opt (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree cv_qualifier = NULL_TREE;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* See if it's a cv-qualifier.  */
  switch (token->keyword)
    {
    case RID_RESTRICT:
      /* Because `restrict' is not part of ANSI/ISO C++, `restrict'
	 will only be a keyword if we are accepting GNU extensions.  */
      my_friendly_assert (cp_parser_allow_gnu_extensions_p (parser),
			  20010416);
      /* Fall through.  */
    case RID_CONST:
    case RID_VOLATILE:
      /* Save the value of the token.  */
      cv_qualifier = token->value;
      /* Consume the token.  */
      cp_lexer_consume_token (parser->lexer);
      break;

    default:
      break;
    }

  return cv_qualifier;
}

/* Parse a declarator-id.

   declarator-id:
     id-expression
     :: [opt] nested-name-specifier [opt] type-name  

   In the `id-expression' case, the value returned is as for
   cp_parser_id_expression if the id-expression was an unqualified-id.
   If the id-expression was a qualified-id, then a SCOPE_REF is
   returned.  The first operand is the scope (either a NAMESPACE_DECL
   or TREE_TYPE), but the second is still just a representation of an
   unqualified-id.

   FIXME: Document the representation returned for the second
   production.  */

static tree
cp_parser_declarator_id (parser)
     cp_parser *parser;
{
  tree id_expression;

  /* FIXME: Enable this, if necessary.  */
#if 0
  tree type_decl;

  /* Try the second alternative (used to declare a constructor)
     first.  */
  cp_parser_parse_tentatively (parser);
  /* Look for the optional `::' operator.  */
  cp_parser_global_scope_opt (parser,
			      /*current_scope_valid_p=*/false);
  /* Look for the optional nested-name-specifier.  */
  cp_parser_nested_name_specifier_opt (parser);
  /* Look for the type-name indicating the type constructed.  */
  type_decl = cp_parser_type_name (parser);
  /* If all went well, we have a constructor name.  */
  if (cp_parser_parse_definitely (parser))
    /* FIXME: Implement this.  */
    cp_parser_unimplemented ();
#endif

  /* Start from the current scope.  */
  parser->scope = NULL_TREE;
  /* In this case, it must be an id-expression.  Assume that qualified
     names are the names of types so that:

       template <class T>
       int S<T>::R::i = 3;

     will work; we must treat `S<T>::R' as the name of a type.  */
  id_expression = cp_parser_id_expression (parser,
					   /*assume_typename_p=*/true);
  /* If the name was qualified, create a SCOPE_REF to represent 
     that.  */
  if (parser->scope)
    id_expression = build_nt (SCOPE_REF, parser->scope, id_expression);

  return id_expression;
}

/* Parse a type-id.

   type-id:
     type-specifier-seq abstract-declarator [opt]

   Returns the TYPE specified.  */

static tree
cp_parser_type_id (parser)
     cp_parser *parser;
{
  tree type_specifier_seq;
  tree abstract_declarator;

  /* Parse the type-specifier-seq.  */
  type_specifier_seq 
    = cp_parser_type_specifier_seq (parser);

  /* There might or might not be an abstract declarator.  */
  cp_parser_parse_tentatively (parser);
  /* Look for the declarator.  */
  abstract_declarator 
    = cp_parser_declarator (parser, /*abstract_p=*/true, NULL);
  /* Check to see if there really was a declarator.  */
  if (!cp_parser_parse_definitely (parser))
    abstract_declarator = NULL_TREE;

  return groktypename (build_tree_list (type_specifier_seq,
					abstract_declarator));
}

/* Parse a type-specifier-seq.

   type-specifier-seq:
     type-specifier type-specifier-seq [opt]

   Returns a TREE_LIST.  The TREE_VALUE of each node is a
   type-specifier.  */

static tree
cp_parser_type_specifier_seq (parser)
     cp_parser *parser;
{
  tree type_specifier_seq = NULL_TREE;
  tree type_specifier;

  /* Parse the first type-specifier.  It's not optional.  */
  type_specifier = cp_parser_type_specifier (parser, 
					     CP_PARSER_FLAGS_NONE,
					     /*is_friend=*/false,
					     NULL,
					     NULL);
  /* Add the new type-specifier to the list.  */
  type_specifier_seq 
    = tree_cons (NULL_TREE, type_specifier, type_specifier_seq);

  /* Parse more type-specifiers.  */
  while (true)
    {
      /* This type-specifier is optional.  */
      cp_parser_parse_tentatively (parser);
      /* Look for the type-specifier.  */
      type_specifier = cp_parser_type_specifier (parser, 
						 CP_PARSER_FLAGS_NONE,
						 /*is_friend=*/false,
						 NULL,
						 NULL);
      /* If that didn't work, stop.  */
      if (!cp_parser_parse_definitely (parser))
	break;

      /* Add the new type-specifier to the list.  */
      type_specifier_seq 
	= tree_cons (NULL_TREE, type_specifier, type_specifier_seq);
    }

  /* We built up the list in reverse order.  */
  return nreverse (type_specifier_seq);
}

/* Parse a parameter-declaration-clause.

   parameter-declaration-clause:
     parameter-declaration-list [opt] ... [opt]
     parameter-declaration-list , ...

   Returns a representation for the parameter declarations.  Each node
   is a TREE_LIST.  (See cp_parser_parameter_declaration for the exact
   representation.)  If the parameter-declaration-clause ends with an
   ellipsis, PARMLIST_ELLIPSIS_P will hold of the first node in the
   list.  A return value of NULL_TREE indicates a
   parameter-declaration-clause consisting only of an ellipsis.  */

static tree
cp_parser_parameter_declaration_clause (parser)
     cp_parser *parser;
{
  tree parameters;
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* Check for trivial parameter-declaration-clauses.  */
  if (token->type == CPP_ELLIPSIS)
    {
      /* Consume the `...' token.  */
      cp_lexer_consume_token (parser->lexer);
      return NULL_TREE;
    }
  else if (token->type == CPP_CLOSE_PAREN)
    /* There are no parameters.  */
    return void_list_node;
  /* Check for `(void)', too, which is a special case.  */
  else if (token->keyword == RID_VOID
	   && (cp_lexer_peek_nth_token (parser->lexer, 2)->type 
	       == CPP_CLOSE_PAREN))
    {
      /* Consume the `void' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* There are no parameters.  */
      return void_list_node;
    }
  
  /* Parse the parameter-declaration-list.  */
  parameters = cp_parser_parameter_declaration_list (parser);
  /* If a parse error occurred while parsing the
     parameter-declaration-list, then the entire
     parameter-declaration-clause is erroneous.  */
  if (parameters == error_mark_node)
    return error_mark_node;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's a `,', the clause should terminate with an ellipsis.  */
  if (token->type == CPP_COMMA)
    {
      /* Consume the `,'.  */
      cp_lexer_consume_token (parser->lexer);
      /* Expect an ellipsis.  */
      cp_parser_require (parser, CPP_ELLIPSIS, "`...'");
      /* Finish the parameter list.  */
      parameters = finish_parmlist (parameters, /*ellipsis=*/1);
    }
  else
    /* Finish the parameter list.  */
    parameters = finish_parmlist (parameters, /*ellipsis=*/0);

  return parameters;
}

/* Parse a parameter-declaration-list.

   parameter-declaration-list:
     parameter-declaration
     parameter-declaration-list , parameter-declaration

   Returns a representation of the parameter-declaration-list, as for
   cp_parser_parameter_declaration_clause.  However, the
   `void_list_node' is never appended to the list.  */

static tree
cp_parser_parameter_declaration_list (parser)
     cp_parser *parser;
{
  tree parameters = NULL_TREE;

  /* Look for more parameters.  */
  while (true)
    {
      cp_token *token;
      tree parameter;

      /* Parse the parameter.  */
      parameter 
	= cp_parser_parameter_declaration (parser,
					   /*greater_than_is_operator_p=*/true);
      /* If a parse error ocurred parsing the parameter declaration,
	 then the entire parameter-declaration-list is erroneous.  */
      if (parameter == error_mark_node)
	{
	  parameters = error_mark_node;
	  break;
	}
      /* Add the new parameter to the list.  */
      TREE_CHAIN (parameter) = parameters;
      parameters = parameter;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      if (token->type == CPP_CLOSE_PAREN)
	/* The parameter-declaration-list is complete.  */
	break;
      else if (token->type == CPP_COMMA)
	{
	  /* Peek at the next token.  */
	  token = cp_lexer_peek_nth_token (parser->lexer, 2);
	  /* If it's an ellipsis, then the list is complete.  */
	  if (token->type == CPP_ELLIPSIS)
	    break;
	  /* Otherwise, there must be more parameters.  Consume the
	     `,'.  */
	  cp_lexer_consume_token (parser->lexer);
	}
      else
	cp_parser_unimplemented ();
    }

  /* We built up the list in reverse order; straighten it out now.  */
  return nreverse (parameters);
}

/* Parse a parameter declaration.

   parameter-declaration:
     decl-specifier-seq declarator
     decl-specifier-seq declarator = assignment-expression
     decl-specifier-seq abstract-declarator [opt]
     decl-specifier-seq abstract-declarator [opt] = assignment-expression

   If GREATER_THAN_IS_OPERATOR_P is FALSE, then a non-nested `>' token
   encountered during the parsing of the assignment-expression is not
   interpreted as a greater-than operator.

   Returns a TREE_LIST representing the parameter-declaration.  The
   TREE_VALUE is a representation of the decl-specifier-seq and
   declarator.  In particular, the TREE_VALUE will be a TREE_LIST
   whose TREE_PURPOSE represents the decl-specifier-seq and whose
   TREE_VALUE represents the declarator.  */

static tree
cp_parser_parameter_declaration (parser, greater_than_is_operator_p)
     cp_parser *parser;
     bool greater_than_is_operator_p;
{
  bool declares_class_or_enum;
  tree decl_specifiers;
  tree attributes;
  tree declarator;
  tree default_argument;
  tree parameter;
  cp_token *token;

  /* Parse the declaration-specifiers.  */
  decl_specifiers 
    = cp_parser_decl_specifier_seq (parser,
				    CP_PARSER_FLAGS_NONE,
				    &attributes,
				    &declares_class_or_enum);
  /* If an error occurred, there's no reason to attempt to parse the
     rest of the declaration.  */
  if (cp_parser_error_occurred (parser))
    return error_mark_node;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the next token is a `)', `,', `=', or `...', then there is no
     declarator.  */
  if (token->type == CPP_CLOSE_PAREN 
      || token->type == CPP_COMMA
      || token->type == CPP_EQ
      || token->type == CPP_ELLIPSIS)
    declarator = NULL_TREE;
  /* Otherwise, there should be a declarator.  */
  else
    {
      /* We don't know whether the declarator will be abstract or
	 not.  So, first we try an ordinary declarator.  */
      cp_parser_parse_tentatively (parser);
      declarator = cp_parser_declarator (parser,
					 /*abstract_p=*/false,
					 /*ctor_dtor_or_conv_p=*/NULL);
      /* If that didn't work, look for an abstract declarator.  */
      if (!cp_parser_parse_definitely (parser))
	declarator = cp_parser_declarator (parser,
					   /*abstract_p=*/true,
					   /*ctor_dtor_or_conv_p=*/NULL);
    }

  /* If the next token is `=', then process a default argument.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_EQ))
    {
      bool saved_greater_than_is_operator_p;
      /* Consume the `='.  */
      cp_lexer_consume_token (parser->lexer);
      /* Make sure that PARSER->GREATER_THAN_IS_OPERATOR_P is
	 set correctly.  */
      saved_greater_than_is_operator_p 
	= parser->greater_than_is_operator_p;
      parser->greater_than_is_operator_p = greater_than_is_operator_p;
      /* FIXME: We need to handle this specially in a
	 class-definition.  */
      /* FIXME: Forbid `this' and local variable names here, as per
	 the standard.  Then we don't have to check later!  */
      /* Consume the assignment-expression.  */
      default_argument = cp_parser_assignment_expression (parser);
      /* Restore the handling of `>'.  */
      parser->greater_than_is_operator_p 
	= saved_greater_than_is_operator_p;
    }
  else
    default_argument = NULL_TREE;

  /* FIXME: Issue an error message about attributes if they are
     present?  */

  /* Create the representation of the parameter.  */
  parameter = build_tree_list (default_argument, 
			       build_tree_list (decl_specifiers,
						declarator));

  return parameter;
}

/* Parse a function-definition.  

   function-definition:
     decl-specifier-seq [opt] declarator ctor-initializer [opt]
       function-body 
     decl-specifier-seq [opt] declarator function-try-block  

   GNU Extension:

   function-definition:
     __extension__ function-definition 

   NUM_TEMPLATE_PARAMETER_LISTS is the number of template parameter
   lists that apply to the function definition.  Returns the
   FUNCTION_DECL for the function.  */

static tree
cp_parser_function_definition (parser, num_template_parameter_lists)
     cp_parser *parser;
     unsigned num_template_parameter_lists;
{
  tree decl_specifiers;
  tree attributes;
  tree declarator;
  tree fn;
  tree scope;
  cp_token *token;
  bool declares_class_or_enum;
  /* The saved value of the PEDANTIC flag.  */
  int saved_pedantic;

  /* Handle `__extension__'.  */
  if (cp_parser_extension_opt (parser, &saved_pedantic))
    {
      /* Parse the function-definition.  */
      fn = cp_parser_function_definition (parser,
					  num_template_parameter_lists);
      /* Restore the PEDANTIC flag.  */
      pedantic = saved_pedantic;

      return fn;
    }

  /* Parse the decl-specifier-seq.  */
  decl_specifiers 
    = cp_parser_decl_specifier_seq (parser,
				    CP_PARSER_FLAGS_OPTIONAL,
				    &attributes,
				    &declares_class_or_enum);

  /* Parse the declarator.  */
  declarator = cp_parser_declarator (parser, 
				     /*abstract_p=*/false,
				     /*ctor_dtor_or_conv_p=*/NULL);

  /* The next character should be a `{' (for a simple function
     definition), a `:' (for a ctor-initializer), or `try' (for a
     function-try block).  */
  token = cp_lexer_peek_token (parser->lexer);
  if (token->type != CPP_OPEN_BRACE
      && token->type != CPP_COLON
      && !cp_parser_is_keyword (token, RID_TRY))
    {
      /* Issue the error-message.  */
      cp_parser_error (parser, "expected function-definition");
      /* Skip to the next `;'.  */
      cp_parser_skip_tokens (parser, CPP_SEMICOLON);

      return error_mark_node;
    }

  /* If we are in a class scope, then we must handle
     function-definitions specially.  In particular, we save away the
     tokens that make up the function body, and parse them again
     later, in order to handle code like:

       struct S {
         int f () { return i; }
	 int i;
       }; 
 
     Here, we cannot parse the body of `f' until after we have seen
     the declaration of `i'.  */
  if (at_class_scope_p () 
      && TYPE_BEING_DEFINED (current_class_type))
    {
      int depth;
      cp_parser_inline_definition *inline_definition;

      /* Create the function-declaration.  */
      fn = start_method (decl_specifiers, declarator, NULL_TREE);
      /* Create the structure that will contain information about the
	 inline definition.  It does not make sense to allocate this
	 with ggc_alloc, since if it is ever collected we will leak
	 the TOKEN_OBSTACK.  (We do not have finalization functions in
	 the garbage collector.)  */
      inline_definition
	= xcalloc (1, sizeof (cp_parser_inline_definition));
      gcc_obstack_init (&inline_definition->token_obstack);

      /* Save away the tokens that make up the body of the 
	 function.  */
      /* FIXME: Do not assume that there is no function-try-block or
	 ctor-initializer.  */
      depth = 0;
      while (true)
	{
	  /* Consume the next token.  */
	  token = cp_lexer_consume_token (parser->lexer);
	  /* If it is a `{', then we are entering a new scope, so we
	     increase the nesting depth.  */
	  if (token->type == CPP_OPEN_BRACE)
	    ++depth;
	  /* On the other hand, if it is a `}', then we decrease the
	     nesting depth.  We are guaranteed never to get here
	     without having seen the opening `{', so we need not worry
	     about underflow.  */
	  else if (token->type == CPP_CLOSE_BRACE)
	    --depth;
	  else if (token->type == CPP_EOF)
	    {
	      cp_parser_error (parser,
			       "file ends inside function definition");
	      return error_mark_node;
	    }

	  /* Add this token to the tokens we are saving.  */
	  obstack_grow (&inline_definition->token_obstack, 
			token, sizeof (cp_token));

	  /* If this token was the closing `}' of the function
	     definition, then we are done.  */
	  if (!depth && token->type == CPP_CLOSE_BRACE)
	    break;
	}

      /* Save away the inline definition; we will process it when the
	 class is complete.  */
      DECL_PENDING_INLINE_INFO (fn) = inline_definition;
      DECL_PENDING_INLINE_P (fn) = 1;

      /* We're done with the inline definition.  */
      finish_method (fn);

      return fn;
    }

  /* Check that the number of template-parameter-lists is OK.  */
  if (!cp_parser_check_template_parameters 
      (declarator, num_template_parameter_lists))
    /* FIXME: Skip tokens?  */
    cp_parser_unimplemented ();
  /* Figure out what scope the entity declared by the DECLARATOR is
     located in.  */
  scope = cp_parser_get_scope_of_declarator (declarator);
  /* Enter the SCOPE.  That way unqualified names appearing in the
     initializer will be looked up in SCOPE.  */
  if (scope)
    push_scope (scope);
  /* Begin the function-definition.  */
  begin_function_definition (decl_specifiers, attributes, declarator);
  /* Parse the body of the function itself.  */
  fn = cp_parser_function_definition_after_declarator (parser,
						       /*inline_p=*/false);
  /* Leave the SCOPE.  */
  if (scope)
    pop_scope (scope);

  return fn;
}

/* Parse a function-body.

   function-body:
     compound_statement  */

static void
cp_parser_function_body (parser)
     cp_parser *parser;
{
  cp_parser_compound_statement (parser);
}

/* Parse an initializer.

   intializer:
     = initializer-clause
     ( expression-list )  

   Returns a expression representing the initializer.  If no
   initializer is present, NULL_TREE is returned.  

   *IS_PARENTHESIZED_INIT is set to TRUE if the `( expression-list )'
   production is used, and zero otherwise.  *IS_PARENTHESIZED_INIT is
   set to FALSE if there is no initializer present.  */

static tree
cp_parser_initializer (parser, is_parenthesized_init)
     cp_parser *parser;
     bool *is_parenthesized_init;
{
  cp_token *token;
  tree init;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);

  /* Let our caller know whether or not this initializer was
     parenthesized.  */
  *is_parenthesized_init = (token->type == CPP_OPEN_PAREN);

  if (token->type == CPP_EQ)
    {
      /* Consume the `='.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the initializer-clause.  */
      init = cp_parser_initializer_clause (parser);
    }
  else if (token->type == CPP_OPEN_PAREN)
    {
      /* Consume the `('.  */
      cp_lexer_consume_token (parser->lexer);
      /* Parse the expression-list.  */
      init = cp_parser_expression_list (parser);
      /* Consume the `)' token.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
    }
  else
    {
      /* Anything else is an error.  */
      cp_parser_error (parser, "expected initializer");
      init = error_mark_node;
    }

  return init;
}

/* Parse an initializer-clause.  

   initializer-clause:
     assignment-expression
     { initializer-list , [opt] }
     { }

   Returns an expression representing the initializer.  

   If the `assignment-expression' production is used the value
   returned is simply a reprsentation for the expression.  

   Otherwise, a CONSTRUCTOR is returned.  The CONSTRUCTOR_ELTS will be
   the elements of the initializer-list (or NULL_TREE, if the last
   production is used).  The TREE_TYPE for the CONSTRUCTOR will be
   NULL_TREE.  There is no way to detect whether or not the optional
   trailing `,' was provided.  */

static tree
cp_parser_initializer_clause (parser)
     cp_parser *parser;
{
  tree initializer;

  /* If it is not a `{', then we are looking at an
     assignment-expression.  */
  if (cp_lexer_next_token_is_not (parser->lexer, CPP_OPEN_BRACE))
    initializer = cp_parser_assignment_expression (parser);
  else
    {
      /* Consume the `{' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Create a CONSTRUCTOR to represent the braced-initializer.  */
      initializer = make_node (CONSTRUCTOR);
      /* Mark it with TREE_HAS_CONSTRUCTOR.  This should not be
	 necessary, but check_initializer depends upon it, for 
	 now.  */
      TREE_HAS_CONSTRUCTOR (initializer) = 1;
      /* If it's not a `}', then there is a non-trivial initializer.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_CLOSE_BRACE))
	{
	  /* Parse the initializer list.  */
	  CONSTRUCTOR_ELTS (initializer)
	    = cp_parser_initializer_list (parser);
	  /* A trailing `,' token is allowed.  */
	  if (cp_lexer_next_token_is (parser->lexer, CPP_COMMA))
	    cp_lexer_consume_token (parser->lexer);
	}

      /* Now, there should be a trailing `}'.  */
      cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");
    }

  return initializer;
}

/* Parse an initializer-list.

   initializer-list:
     initializer-clause
     initializer-list , initializer-clause

   GNU Extension:
   
   initializer-list:
     identifier : initializer-clause
     initializer-list, identifier : initializer-clause

   Returns a TREE_LIST.  The TREE_VALUE of each node is an expression
   for the initializer.  If the TREE_PURPOSE is non-NULL, it is the
   IDENTIFIER_NODE naming the field to initialize.   */

static tree
cp_parser_initializer_list (parser)
     cp_parser *parser;
{
  tree initializers = NULL_TREE;

  /* Parse the rest of the list.  */
  while (true)
    {
      cp_token *token;
      tree identifier;
      tree initializer;
      
      /* If the next token is an identifier and the following one is a
	 colon, we are looking at the GNU designated-initializer
	 syntax.  */
      if (cp_parser_allow_gnu_extensions_p (parser)
	  && cp_lexer_next_token_is (parser->lexer, CPP_NAME)
	  && cp_lexer_peek_nth_token (parser->lexer, 2)->type == CPP_COLON)
	{
	  /* Consume the identifier.  */
	  identifier = cp_lexer_consume_token (parser->lexer)->value;
	  /* Consume the `:'.  */
	  cp_lexer_consume_token (parser->lexer);
	}
      else
	identifier = NULL_TREE;

      /* Parse the initializer.  */
      initializer = cp_parser_initializer_clause (parser);

      /* Add it to the list.  */
      initializers = tree_cons (identifier, initializer, initializers);

      /* If the next token is not a comma, we have reached the end of
	 the list.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA))
	break;

      /* Peek at the next token.  */
      token = cp_lexer_peek_nth_token (parser->lexer, 2);
      /* If the next token is a `}', then we're still done.  An
	 initializer-clause can have a trailing `,' after the
	 initializer-list and before the closing `}'.  */
      if (token->type == CPP_CLOSE_BRACE)
	break;

      /* Consume the `,' token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* The initializers were built up in reverse order, so we need to
     reverse them now.  */
  return nreverse (initializers);
}

/* Classes [gram.class] */

/* Parse a class-name.

   class-name:
     identifier
     template-id

   TYPENAME_KEYWORD_P is true iff the `typename' keyword has been used
   to indicate that names looked up in dependent types should be
   assumed to be types.  TEMPLATE_KEYWORD_P is true iff the `template'
   keyword has been used to indicate that the name that appears next
   is a template.

   Returns the TYPE_DECL representing the class.  */

static tree
cp_parser_class_name (parser, typename_keyword_p, template_keyword_p)
     cp_parser *parser;
     bool typename_keyword_p;
     bool template_keyword_p;
{
  tree decl;
  tree scope;
  bool typename_p;
  
  /* PARSER->SCOPE can be cleared when parsing the template-arguments
     to a template-id, so we save it here.  */
  scope = parser->scope;
  /* Any name names a type if we're following the `typename' keyword
     in a qualified name where the enclosing scope is type-dependent.  */
  typename_p = (typename_keyword_p && scope && TYPE_P (scope)
		&& cp_parser_dependent_type_p (scope));

  /* We don't know whether what comes next is a template-id or 
     not.  */
  cp_parser_parse_tentatively (parser);
  /* Try a template-id.  */
  /* FIXME: Do we ever get here after having just seen `template'?  */
  decl = cp_parser_template_id (parser, template_keyword_p);
  /* If it wasn't a template-id, try a simple identifier.  */
  if (!cp_parser_parse_definitely (parser))
    {
      tree identifier;

      /* Look for the identifier.  */
      identifier = cp_parser_identifier (parser);
      /* If we know this is a type-name, there's no need to look it
	 up.  */
      if (typename_p)
	decl = identifier;
      else
	decl = cp_parser_lookup_name (parser, identifier);
    }

  /* If this is typename, create a TYPENAME_TYPE.  */
  if (typename_p && decl != error_mark_node)
    decl = TYPE_NAME (make_typename_type (scope, decl,
					  /*complain=*/1));


  /* Check to see that it is really the name of a class.  */
  /* FIXME: This accepts typedefs for a class.  Is that OK?  */
  if (decl == error_mark_node
      || TREE_CODE (decl) != TYPE_DECL
      || !IS_AGGR_TYPE (TREE_TYPE (decl)))
    {
      cp_parser_error (parser, "expected class-name");
      return error_mark_node;
    }

  return decl;
}

/* Parse a class-specifier.

   class-specifier:
     class-head { member-specification [opt] }

   Returns the TREE_TYPE representing the class.  */

static tree
cp_parser_class_specifier (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree member_function;
  tree type;
  tree attributes = NULL_TREE;
  int has_trailing_semicolon;
  bool nested_name_specifier_p;

  /* Parse the class-head.  */
  type = cp_parser_class_head (parser, &nested_name_specifier_p);
  /* Look for the `{'.  */
  if (!cp_parser_require (parser, CPP_OPEN_BRACE, "`{'"))
    return error_mark_node;
  /* At this point, we're going ahead with the class-specifier, even
     if some other problem occurs.  */
  cp_parser_commit_to_tentative_parse (parser);
  /* Start the class.  */
  type = begin_class_definition (type);
  /* FIXME: What's this about?  */
  current_aggr = ctk_none;
  /* Parse the member-specification.  */
  cp_parser_member_specification_opt (parser);
  /* Look for the trailing `}'.  */
  cp_parser_require (parser, CPP_CLOSE_BRACE, "`}'");
  /* We get better error messages by noticing a common problem: a
     missing trailing `;'.  */
  token = cp_lexer_peek_token (parser->lexer);
  has_trailing_semicolon = (token->type == CPP_SEMICOLON);
  /* Look for attributes to apply to this class.  */
  if (cp_parser_allow_gnu_extensions_p (parser))
    attributes = cp_parser_attributes_opt (parser);
  /* Finish the class definition.  */
  type = finish_class_definition (type, 
				  attributes,
				  has_trailing_semicolon,
				  nested_name_specifier_p);
  /* Reenter the class scope, while we parse the bodies of inline
     member functions.  */
  push_scope (type);
  /* Now parse the bodies of any inline member functions.  */
  for (member_function = TYPE_METHODS (type); 
       member_function; 
       member_function = TREE_CHAIN (member_function))
    {
      tree function_scope;
      cp_lexer *saved_lexer;
      cp_token *first_token;
      cp_token *last_token;
      cp_parser_inline_definition *definition;

      /* Skip functions that don't have bodies to parse.  */
      if (!DECL_PENDING_INLINE_P (member_function))
	continue;
      /* The function is no longer pending; we are processing it.  */
      definition = DECL_PENDING_INLINE_INFO (member_function);
      DECL_PENDING_INLINE_INFO (member_function) = NULL;
      DECL_PENDING_INLINE_P (member_function) = 0;
      /* If this was an inline function in a local class, enter the
	 scope of the containing function.  */
      function_scope = decl_function_context (member_function);
      if (function_scope)
	push_function_context_to (function_scope);

      /* Save away the current lexer.  */
      saved_lexer = parser->lexer;
      /* Figure out what the first and last tokens are.  */
      first_token 
	= (cp_token *) obstack_base (&definition->token_obstack);
      last_token 
	= (cp_token *) obstack_next_free (&definition->token_obstack);
      /* Make a new lexer to feed us the tokens saved for this
	 function.  */
      parser->lexer 
	= cp_lexer_new_from_tokens (first_token, last_token);
      /* We do not need the obstack any more.  We free it before any
	 thing that might cause garbage-collection can occur, since
	 DEFINITION is no longer pointed to from the MEMBER_FUNCTION.  */
      obstack_free (&definition->token_obstack, NULL);
      /* We are all done with the DEFINITION.  */
      free (definition);

      /* Let the front end know that we going to be defining this
	 function.  */
      start_function (NULL_TREE, member_function, NULL_TREE,
		      SF_PRE_PARSED | SF_INCLASS_INLINE);

      /* Now, parse the body of the function.  */
      cp_parser_function_definition_after_declarator (parser,
						      /*inline_p=*/true);

      /* Leave the scope of the containing function.  */
      if (function_scope)
	pop_function_context_from (function_scope);
      /* Destroy the lexer we created to handle the body of the
	 function.  */
      cp_lexer_delete (parser->lexer);
      /* Restore the lexer.  */
      parser->lexer = saved_lexer;
    }
  /* Leave the class scope, now that we are done with inline member
     functions.  */
  pop_scope (type);

  return type;
}

/* Parse a class-head.

   class-head:
     class-key identifier [opt] base-clause [opt]
     class-key nested-name-specifier identifier base-clause [opt]
     class-key nested-name-specifier [opt] template-id 
       base-clause [opt]  

   Returns the TYPE of the indicated class.  Sets
   *NESTED_NAME_SPECIFIER_P to TRUE iff one of the productions
   involving a nested-name-specifier was used, and FALSE otherwise.  */

static tree
cp_parser_class_head (parser, nested_name_specifier_p)
     cp_parser *parser;
     bool *nested_name_specifier_p;
{
  cp_token *token;
  tree nested_name_specifier;
  cp_tag_kind class_key;
  tree id = NULL_TREE;
  tree type = NULL_TREE;
  bool template_id_p = false;

  /* Assume no nested-name-specifier will be present.  */
  *nested_name_specifier_p = false;

  /* Look for the class-key.  */
  class_key = cp_parser_class_key (parser);
  if (class_key == ctk_none)
    return error_mark_node;

  /* There is no explicit qualification at this point.  */
  parser->scope = NULL_TREE;
  /* Determine the name of the class.  Begin by looking for an
     optional nested-name-specifier.  */
  nested_name_specifier 
    = cp_parser_nested_name_specifier_opt (parser,
					   /*typename_keyword_p=*/false);
  /* If there was a nested-name-specifier, then there *must* be an
     identifier.  */
  if (nested_name_specifier)
    /* Although the grammar says `identifier', it really means
       `class-name'.  You are only allowed to define a class that has
       already been declared with this syntax.  The proposed
       resolution for Core Issue 180 says that whever you see `class
       T::X' you should treat `X' as a type-name.  */
    /* FIXME: In fact, it means a class-name that is not a
       typedef-name.  */
    type = cp_parser_class_name (parser,
				 /*typename_keyword_p=*/true,
				 /*template_keyword_p=*/false);
  /* Otherwise, the identifier is optional.  */
  else
    {
      /* We don't know whether what comes next is a template-id,
	 an identifier, or nothing at all.  */
      cp_parser_parse_tentatively (parser);
      /* Check for a template-id.  */
      id = cp_parser_template_id (parser, 
				  /*template_keyword_p=*/false);
      /* If that didn't work, it could still be an identifier.  */
      if (!cp_parser_parse_definitely (parser))
	{
	  if (cp_lexer_next_token_is (parser->lexer, CPP_NAME))
	    id = cp_parser_identifier (parser);
	  else
	    id = NULL_TREE;
	}
      else
	template_id_p = true;
    }

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's not a `:' or a `{' then we can't really be looking at a
     class-head, since a class-head only appears as part of a
     class-specifier.  We have to detect this situation before calling
     xref_tag, since that has irreversible side-effects.  */
  if (token->type != CPP_OPEN_BRACE && token->type != CPP_COLON)
    {
      cp_parser_error (parser, "expected `{' or `:'");
      return error_mark_node;
    }

  /* Look up the type.  */
  if (template_id_p)
    type = TREE_TYPE (id);
  else if (!nested_name_specifier)
    {
      /* If the class was unnamed, create a dummy name.  */
      if (!id)
	id = make_anon_name ();
      type = xref_tag (class_key, id, /*globalize=*/0);
    }
  else
    {
      /* Given:

	    template <typename T> struct S { struct T };
	    template <typename T> struct S::T { };

	 we will get a TYPENAME_TYPE when processing the definition of
	 `S::T'.  We need to resolve it to the actual type before we
	 try to define it.  */
      if (TREE_CODE (TREE_TYPE (type)) == TYPENAME_TYPE)
	{
	  type = cp_parser_resolve_typename_type (TREE_TYPE (type));
	  if (type != error_mark_node)
	    type = TYPE_NAME (type);
	}

      type = TREE_TYPE (handle_class_head (class_key, 
					   nested_name_specifier,
					   type));
      *nested_name_specifier_p = true;
    }

  /* Now, look for a base-clause.  */
  token = cp_lexer_peek_token (parser->lexer);
  if (token->type == CPP_COLON)
    {
      tree bases;

      /* Get the list of base-classes.  */
      bases = cp_parser_base_clause (parser);
      /* Process them.  */
      xref_basetypes (type, bases);
    }

  return type;
}

/* Parse a class-key.

   class-key:
     class
     struct
     union

   Returns the kind of class-key specified, or ctk_none to indicate
   error.  */

static cp_tag_kind
cp_parser_class_key (parser)
     cp_parser *parser;
{
  cp_token *token;
  enum rid keyword;

  /* Look for the class-key.  */
  token = cp_parser_require (parser, CPP_KEYWORD, "class-key");
  if (!token)
    return ctk_none;

  /* Keep track of whether this is a class, struct, or union.  */
  keyword = token->keyword;
  switch (keyword)
    {
    case RID_CLASS:
      return ctk_class;
    case RID_STRUCT:
      return ctk_struct;
    case RID_UNION:
      return ctk_union;
      
    default:
      cp_parser_error (parser, "expected class-key");
      return ctk_none;
    }
}

/* Parse an (optional) member-specification.

   member-specification:
     member-declaration member-specification [opt]
     access-specifier : member-specification [opt]  */

static void
cp_parser_member_specification_opt (parser)
     cp_parser *parser;
{
  while (true)
    {
      cp_token *token;
      enum rid keyword;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's a `}', then we've seen all the members.  */
      if (token->type == CPP_CLOSE_BRACE)
	break;

      /* See if this token is a keyword.  */
      keyword = token->keyword;
      switch (keyword)
	{
	case RID_PUBLIC:
	case RID_PROTECTED:
	case RID_PRIVATE:
	  /* Consume the access-specifier.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Remember which access-specifier is active.  */
	  current_access_specifier = token->value;
	  /* Look for the `:'.  */
	  cp_parser_require (parser, CPP_COLON, "`:'");
	  break;

	default:
	  /* Otherwise, the next construction must be a
	     member-declaration.  */
	  cp_parser_member_declaration (parser);
	}
    }
}

/* Parse a member-declaration.  

   member-declaration:
     decl-specifier-seq [opt] member-declarator-list [opt] ;
     function-definition ; [opt]
     :: [opt] nested-name-specifier template [opt] unqualified-id ;
     using-declaration
     template-declaration 

   member-declarator-list:
     member-declarator
     member-declarator-list , member-declarator

   member-declarator:
     declarator pure-specifier [opt] 
     declarator constant-initializer [opt]
     identifier [opt] : constant-expression 

   GNU Extensions:

   member-declaration:
     __extension__ member-declaration

   member-declarator:
     declarator attributes [opt] pure-specifier [opt]
     declarator attributes [opt] constant-initializer [opt]
     identifier [opt] attributes [opt] : constant-expression  */

static void
cp_parser_member_declaration (parser)
     cp_parser *parser;
{
  tree decl_specifiers;
  tree prefix_attributes;
  tree decl;
  bool declares_class_or_enum;
  cp_token *token;
  int saved_pedantic;

  /* Check for the `__extension__' keyword.  */
  if (cp_parser_extension_opt (parser, &saved_pedantic))
    {
      /* Recurse.  */
      cp_parser_member_declaration (parser);
      /* Restore the old value of the PEDANTIC flag.  */
      pedantic = saved_pedantic;

      return;
    }

  /* Check for a template-declaration.  */
  if (cp_lexer_next_token_is_keyword (parser->lexer, RID_TEMPLATE))
    {
      /* Parse the template-declaration.  */
      cp_parser_template_declaration (parser, /*member_p=*/true);

      return;
    }

  /* Check for a using-declaration.  */
  if (cp_lexer_next_token_is_keyword (parser->lexer, RID_USING))
    {
      /* Parse the using-declaration.  */
      cp_parser_using_declaration (parser);

      return;
    }
  
  /* FIXME: Handle the old-style access-declarations.  */

  /* We can't tell whether we're looking at a declaration or a
     function-definition.  */
  cp_parser_parse_tentatively (parser);

  /* Parse the decl-specifier-seq.  */
  decl_specifiers 
    = cp_parser_decl_specifier_seq (parser,
				    CP_PARSER_FLAGS_OPTIONAL,
				    &prefix_attributes,
				    &declares_class_or_enum);
  /* If there is no declarator, then the decl-specifier-seq should
     specify a type.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_SEMICOLON))
    {
      /* If there was no decl-specifier-seq, and the next token is a
	 `;', then we have something like:

	   struct S { ; };

	 [class.mem]

	 Each member-declaration shall declare at least one member
	 name of the class.  */
      if (!decl_specifiers)
	{
	  if (pedantic)
	    cp_pedwarn ("member declaration does not declare anything");
	}
      else 
	{
	  tree type;
	  bool friend_p;
	  
	  /* If there were decl-specifiers, check to see if there was
	     a class-declaration.  */
	  type = cp_parser_declares_only_class_p (parser, 
						  decl_specifiers,
						  &friend_p);
	  /* If not, the user should have had a declarator.  */
	  if (!type)
	    cp_error ("expected declarator");
	  /* Nested classes have already been added to the class, but
	     a `friend' needs to be explicitly registered.  */
	  else if (friend_p)
	    make_friend_class (current_class_type, type);
	}
    }
  else
    /* Keep going until we hit the `;' at the end of the declaration.  */
    while (cp_lexer_next_token_is_not (parser->lexer, CPP_SEMICOLON))
      {
	tree attributes = NULL_TREE;
	tree first_attribute;

	/* Peek at the next token.  */
	token = cp_lexer_peek_token (parser->lexer);

	/* Check for a bitfield declaration.  */
	if (token->type == CPP_COLON
	    || (token->type == CPP_NAME
		&& cp_lexer_peek_nth_token (parser->lexer, 2)->type 
		== CPP_COLON))
	  {
	    tree identifier;
	    tree width;

	    /* Get the name of the bitfield.  Note that we cannot just
	       check TOKEN here because it may have been invalidated by
	       the call to cp_lexer_peek_nth_token above.  */
	    if (cp_lexer_peek_token (parser->lexer)->type != CPP_COLON)
	      identifier = cp_parser_identifier (parser);
	    else
	      identifier = NULL_TREE;

	    /* Consume the `:' token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Get the width of the bitfield.  */
	    width = cp_parser_constant_expression (parser);

	    /* Look for attributes that apply to the bitfield.  */
	    if (cp_parser_allow_gnu_extensions_p (parser))
	      attributes = cp_parser_attributes_opt (parser);

	    /* Great the bitfield declaration.  */
	    decl = grokbitfield (identifier, 
				 decl_specifiers,
				 width);
	  }
	else
	  {
	    cp_token *token;
	    tree declarator;
	    tree initializer;
	    bool ctor_dtor_or_conv_p;

	    /* Parse the declarator.  */
	    declarator 
	      = cp_parser_declarator (parser,
				      /*abstract_p=*/false,
				      &ctor_dtor_or_conv_p);

	    /* FIXME: Complain about absent decl-specifiers.  */
	    /* If something went wrong parsing the declarator, make sure
	       that we at least consume some tokens.  */
	    if (declarator == error_mark_node)
	      {
		/* Skip to the next `;'.  */
		cp_parser_skip_tokens (parser, CPP_SEMICOLON);
		break;
	      }

	    /* Look for attributes that apply to the declaration.  */
	    if (cp_parser_allow_gnu_extensions_p (parser))
	      attributes = cp_parser_attributes_opt (parser);

	    /* Peek at the next token.  */
	    token = cp_lexer_peek_token (parser->lexer);
	    /* If it's an `=', then we have a constant-initializer or a
	       pure-specifier.  */
	    if (token->type == CPP_EQ)
	      {
		/* In [class.mem]:

		   A pure-specifier shall be used only in the declaration of
		   a virtual function.  

		   A member-declarator can contain a constant-initializer
		   only if it declares a static member of integral or
		   enumeration type.  

		   Therefore, if the DECLARATOR is for a function, we look
		   for a pure-specifier; otherwise, we look for a
		   constant-initializer.  When we call `grokfield', it will
		   perform more stringent semantics checks.  */
		if (TREE_CODE (declarator) == CALL_EXPR)
		  initializer = cp_parser_pure_specifier (parser);
		else
		  initializer = cp_parser_constant_initializer (parser);
	      }
	    /* Otherwise, there is no initializer.  */
	    else
	      initializer = NULL_TREE;

	    /* Remember which attributes are prefix attributes and which
	       are not.  */
	    first_attribute = attributes;
	    /* Add the PREFIX_ATTRIBUTES.  */
	    attributes = chainon (prefix_attributes, attributes);

	    /* Create the declaration.  */
	    decl = grokfield (declarator, decl_specifiers, 
			      initializer,
			      /*asmspec=*/NULL,
			      build_tree_list (attributes,
					       NULL_TREE));

	    /* Reset PREFIX_ATTRIBUTES.  */
	    while (attributes 
		   && TREE_CHAIN (attributes) != first_attribute)
	      attributes = TREE_CHAIN (attributes);
	    if (attributes)
	      TREE_CHAIN (attributes) = NULL_TREE;
	  }

	/* Peek at the next token.  */
	token = cp_lexer_peek_token (parser->lexer);
	/* If it's a `,', then there are more declarators.  */
	if (token->type == CPP_COMMA)
	  cp_lexer_consume_token (parser->lexer);
	/* If the next token isn't a `;', then we have a parse error.  */
	else if (token->type != CPP_SEMICOLON)
	  {
	    cp_parser_error (parser, "expected `;'");
	    /* Skip tokens until we find a `;'  */
	    cp_parser_skip_tokens (parser, CPP_SEMICOLON);

	    break;
	  }

	/* Add DECL to the list of members.  */
	finish_member_declaration (decl);
      }

  /* If everything went well, look for the `;'.  */
  if (cp_parser_parse_definitely (parser))
    {
      cp_parser_require (parser, CPP_SEMICOLON, "`;'");
      return;
    }

  /* Parse the function-definition.  */
  decl = cp_parser_function_definition (parser,
					/*num_template_parameter_lists=*/0);
  /* Add the function to the list of members.  */
  finish_member_declaration (decl);
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the next token is a semicolon, consume it.  */
  if (token->type == CPP_SEMICOLON)
    cp_lexer_consume_token (parser->lexer);
}

/* Parse a pure-specifier.

   pure-specifier:
     = 0

   Returns INTEGER_ZERO_NODE if a pure specifier is found.
   Otherwiser, ERROR_MARK_NODE is returned.  */

static tree
cp_parser_pure_specifier (parser)
     cp_parser *parser;
{
  cp_token *token;

  /* Look for the `=' token.  */
  if (!cp_parser_require (parser, CPP_EQ, "`='"))
    return error_mark_node;
  /* Look for the `0' token.  */
  token = cp_parser_require (parser, CPP_NUMBER, "`0'");
  /* FIXME: This will probably accept `0L' and `0x00' as well.  */
  if (!token || !integer_zerop (token->value))
    return error_mark_node;

  return integer_zero_node;
}

/* Parse a constant-initializer.

   constant-initializer:
     = constant-expression

   Returns a representation of the constant-expression.  */

static tree
cp_parser_constant_initializer (parser)
     cp_parser *parser;
{
  /* Look for the `=' token.  */
  if (!cp_parser_require (parser, CPP_EQ, "`='"))
    return error_mark_node;
  
  return cp_parser_constant_expression (parser);
}

/* Derived classes [gram.class.derived] */

/* Parse a base-clause.

   base-clause:
     : base-specifier-list  

   base-specifier-list:
     base-specifier
     base-specifier-list , base-specifier

   Returns a TREE_LIST representing the base-classes, in the order in
   which they were declared.  The representation of each node is as
   described by cp_parser_base_specifier.  

   In the case that no bases are specified, this function will return
   NULL_TREE, not ERROR_MARK_NODE.  */

static tree
cp_parser_base_clause (parser)
     cp_parser *parser;
{
  tree bases = NULL_TREE;

  /* Look for the `:' that begins the list.  */
  cp_parser_require (parser, CPP_COLON, "`:'");

  /* Scan the base-specifier-list.  */
  while (true)
    {
      cp_token *token;
      tree base;

      /* Look for the base-specifier.  */
      base = cp_parser_base_specifier (parser);
      /* Add BASE to the front of the list.  */
      TREE_CHAIN (base) = bases;
      bases = base;
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not a comma, then the list is complete.  */
      if (token->type != CPP_COMMA)
	break;
      /* Consume the `,'.  */
      cp_lexer_consume_token (parser->lexer);
    }

  return nreverse (bases);
}

/* Parse a base-specifier.

   base-specifier:
     :: [opt] nested-name-specifier [opt] class-name
     virtual access-specifier [opt] :: [opt] nested-name-specifier
       [opt] class-name
     access-specifier virtual [opt] :: [opt] nested-name-specifier
       [opt] class-name

   Returns a TREE_LIST.  The TREE_PURPOSE will be one of
   ACCESS_{DEFAULT,PUBLIC,PROTECTED,PRIVATE}_[VIRTUAL]_NODE to
   indicate the specifiers provided.  The TREE_VALUE will be a TYPE
   (or the ERROR_MARK_NODE) indicating the type that was specified.  */
       
static tree
cp_parser_base_specifier (parser)
     cp_parser *parser;
{
  cp_token *token;
  bool done = false;
  bool virtual_p = false;
  bool duplicate_virtual_error_issued_p = false;
  bool duplicate_access_error_issued_p = false;
  access_kind access = ak_none;
  tree access_node;
  tree type;

  /* Process the optional `virtual' and `access-specifier'.  */
  while (!done)
    {
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* Process `virtual'.  */
      switch (token->keyword)
	{
	case RID_VIRTUAL:
	  /* If `virtual' appears more than once, issue an error.  */
	  if (virtual_p && !duplicate_virtual_error_issued_p)
	    {
	      cp_parser_error (parser,
			       "`virtual' specified more than once in base-specified");
	      duplicate_virtual_error_issued_p = true;
	    }

	  virtual_p = true;

	  /* Consume the `virtual' token.  */
	  cp_lexer_consume_token (parser->lexer);

	  break;

	case RID_PUBLIC:
	case RID_PROTECTED:
	case RID_PRIVATE:
	  /* If more than one access specifier appears, issue an
	     error.  */
	  if (access != ak_none && !duplicate_access_error_issued_p)
	    {
	      cp_parser_error (parser,
			       "`virtual' specified more than once in base-specified");
	      duplicate_access_error_issued_p = true;
	    }

	  access = ((access_kind) 
		    tree_low_cst (ridpointers[(int) token->keyword],
				  /*pos=*/1));

	  /* Consume the access-specifier.  */
	  cp_lexer_consume_token (parser->lexer);

	  break;

	default:
	  done = true;
	  break;
	}
    }

  /* Map `virtual_p' and `access' onto one of the access 
     tree-nodes.  */
  /* FIXME: This is a frightful way to express what should be a struct
     with two fields.  */
  if (!virtual_p)
    switch (access)
      {
      case ak_none:
	access_node = access_default_node;
	break;
      case ak_public:
	access_node = access_public_node;
	break;
      case ak_protected:
	access_node = access_protected_node;
	break;
      case ak_private:
	access_node = access_private_node;
	break;
      default:
	my_friendly_abort (20010619);
      }
  else
    switch (access)
      {
      case ak_none:
	access_node = access_default_virtual_node;
	break;
      case ak_public:
	access_node = access_public_virtual_node;
	break;
      case ak_protected:
	access_node = access_protected_virtual_node;
	break;
      case ak_private:
	access_node = access_private_virtual_node;
	break;
      default:
	my_friendly_abort (20010619);
      }

  /* Look for the optional `::' operator.  */
  cp_parser_global_scope_opt (parser, /*current_scope_valid_p=*/false);
  /* Look for the nested-name-specifier.  The simplest way to
     implement:

       [temp.res]

       The keyword `typename' is not permitted in a base-specifier or
       mem-initializer; in these contexts a qualified name that
       depends on a template-parameter is implicitly assumed to be a
       type name.

     is to pretend that we have seen the `typename' keyword at this
     point.  */ 
  cp_parser_nested_name_specifier_opt (parser,
				       /*typename_keyword_p=*/true);
  /* Finally, look for the class-name.  */
  type = cp_parser_class_name (parser, 
			       /*typename_keyword_p=*/true,
			       /*template_keyword_p=*/false);
  
  return build_tree_list (access_node, TREE_TYPE (type));
}

/* Exception handling [gram.exception] */

/* Parse an (optional) exception-specification.

   exception-specification:
     throw ( type-id-list [opt] )

   Returns a TREE_LIST representing the exception-specification.  The
   TREE_VALUE of each node is a type.  */

static tree
cp_parser_exception_specification_opt (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree type_id_list;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's not `throw', then there's no exception-specification.  */
  if (!cp_parser_is_keyword (token, RID_THROW))
    return NULL_TREE;

  /* Consume the `throw'.  */
  cp_lexer_consume_token (parser->lexer);

  /* Look for the `('.  */
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's not a `)', then there is a type-id-list.  */
  if (token->type != CPP_CLOSE_PAREN)
    {
      /* Parse the type-id-list.  */
      type_id_list = cp_parser_type_id_list (parser);
    }
  else
    type_id_list = empty_except_spec;

  /* Look for the `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  return type_id_list;
}

/* Parse an (optional) type-id-list.

   type-id-list:
     type-id
     type-id-list , type-id

   Returns a TREE_LIST.  The TREE_VALUE of each node is a TYPE,
   in the order that the types were presented.  */

static tree
cp_parser_type_id_list (parser)
     cp_parser *parser;
{
  tree types = NULL_TREE;

  while (true)
    {
      cp_token *token;
      tree type;

      /* Get the next type-id.  */
      type = cp_parser_type_id (parser);
      /* Add it to the list.  */
      types = add_exception_specifier (types, type, /*complain=*/1);
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it is not a `,', we are done.  */
      if (token->type != CPP_COMMA)
	break;
      /* Consume the `,'.  */
      cp_lexer_consume_token (parser->lexer);
    }

  return nreverse (types);
}

/* Parse a try-block.

   try-block:
     try compound-statement handler-seq  */

static tree
cp_parser_try_block (parser)
     cp_parser *parser;
{
  tree try_block;

  cp_parser_require_keyword (parser, RID_TRY, "`try'");
  try_block = begin_try_block ();
  cp_parser_compound_statement (parser);
  finish_try_block (try_block);
  cp_parser_handler_seq (parser);
  finish_handler_sequence (try_block);

  return try_block;
}

/* Parse a function-try-block.

   function-try-block:
     try ctor-initializer [opt] function-body handler-seq  */

static void
cp_parser_function_try_block (parser)
     cp_parser *parser;
{
  cp_parser_unimplemented ();
}

/* Parse a handler-seq.

   handler-seq:
     handler handler-seq [opt]  */

static void
cp_parser_handler_seq (parser)
     cp_parser *parser;
{
  while (true)
    {
      cp_token *token;

      /* Parse the handler.  */
      cp_parser_handler (parser);
      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not `catch' then there are no more handlers.  */
      if (!cp_parser_is_keyword (token, RID_CATCH))
	break;
    }
}

/* Parse a handler.

   handler:
     catch ( exception-declaration ) compound-statement  */

static void
cp_parser_handler (parser)
     cp_parser *parser;
{
  tree handler;
  tree declaration;

  cp_parser_require_keyword (parser, RID_CATCH, "`catch'");
  handler = begin_handler ();
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
  declaration = cp_parser_exception_declaration (parser);
  finish_handler_parms (declaration, handler);
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
  cp_parser_compound_statement (parser);
  finish_handler (handler);
}

/* Parse an exception-declaration.

   exception-declaration:
     type-specifier-seq declarator
     type-specifier-seq abstract-declarator
     type-specifier-seq
     ...  

   Returns a VAR_DECL for the declaration, or NULL_TREE if the
   ellipsis variant is used.  */

static tree
cp_parser_exception_declaration (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree type_specifiers;
  tree declarator;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's an ellipsis, it's easy to handle.  */
  if (token->type == CPP_ELLIPSIS)
    {
      /* Consume the `...' token.  */
      cp_lexer_consume_token (parser->lexer);
      return NULL_TREE;
    }

  /* Parse the type-specifier-seq.  */
  type_specifiers = cp_parser_type_specifier_seq (parser);
  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If it's a `)', then there is no declarator.  */
  if (token->type == CPP_CLOSE_PAREN)
    declarator = NULL_TREE;
  else
    {
      /* Otherwise, we can't be sure whether we are looking at a
	 direct, or an abstract, declarator.  */
      cp_parser_parse_tentatively (parser);
      /* Try an ordinary declarator.  */
      declarator = cp_parser_declarator (parser,
					 /*abstract_p=*/false,
					 /*ctor_dtor_or_conv_p=*/NULL);
      /* If that didn't work, try an abstract declarator.  */
      if (!cp_parser_parse_definitely (parser))
	declarator = cp_parser_declarator (parser,
					   /*abstract_p=*/true,
					   /*ctor_dtor_or_conv_p=*/NULL);
    }

  return start_handler_parms (type_specifiers, declarator);
}

/* Parse a throw-expression. 

   throw-expression:
     throw assignment-expresion [opt]

   Returns a THROW_EXPR representing the throw-expression.  */

static tree
cp_parser_throw_expression (parser)
     cp_parser *parser;
{
  tree expression;

  cp_parser_require_keyword (parser, RID_THROW, "`throw'");
  /* We can't be sure if there is an assignment-expression or not.  */
  cp_parser_parse_tentatively (parser);
  /* Try it.  */
  expression = cp_parser_assignment_expression (parser);
  /* If it didn't work, this is just a rethrow.  */
  if (!cp_parser_parse_definitely (parser))
    expression = NULL_TREE;

  return build_throw (expression);
}

/* GNU Extensions */

/* Parse an (optional) asm-specification.

   asm-specification:
     asm ( string-literal )

   If the asm-specification is present, returns a STRING_CST
   corresponding to the string-literal.  Otherwise, returns
   NULL_TREE.  */

static tree
cp_parser_asm_specification_opt (parser)
     cp_parser *parser;
{
  cp_token *token;
  tree asm_specification;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If the next token isn't the `asm' keyword, then there's no 
     asm-specification.  */
  if (!cp_parser_is_keyword (token, RID_ASM))
    return NULL_TREE;

  /* Consume the `asm' token.  */
  cp_lexer_consume_token (parser->lexer);
  /* Look for the `('.  */
  cp_parser_require (parser, CPP_OPEN_PAREN, "`('");

  /* Look for the string-literal.  */
  token = cp_parser_require (parser, CPP_STRING, "string-literal");
  if (token)
    asm_specification = token->value;
  else
    asm_specification = NULL_TREE;

  /* Look for the `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`('");

  return asm_specification;
}

/* Parse an asm-operand-list.  

   asm-operand-list:
     asm-operand
     asm-operand-list , asm-operand
     
   asm-operand:
     string-literal ( expression )  

   Returns a TREE_LIST representing the operands.  The TREE_PURPOSE of
   each node is a STRING_CST representing the string-literal; the
   TREE_VALUE of each node represents the expression.  */

static tree
cp_parser_asm_operand_list (parser)
     cp_parser *parser;
{
  tree asm_operands = NULL_TREE;

  while (true)
    {
      tree string_literal;
      tree expression;
      cp_token *token;

      /* Look for the string-literal.  */
      token = cp_parser_require (parser, CPP_STRING, "string-literal");
      string_literal = token ? token->value : error_mark_node;
      /* Look for the `('.  */
      cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
      /* Parse the expression.  */
      expression = cp_parser_expression (parser);
      /* Look for the `)'.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
      /* Add this operand to the list.  */
      asm_operands = tree_cons (string_literal,
				expression, 
				asm_operands);
      /* If the next token is not a `,', there are no more 
	 operands.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA))
	break;
      /* Consume the `,'.  */
      cp_lexer_consume_token (parser->lexer);
    }

  return nreverse (asm_operands);
}

/* Parse an asm-clobber-list.  

   asm-clobber-list:
     string-literal
     asm-clobber-list , string-literal  

   Returns a TREE_LIST, indicating the clobbers in the order that they
   appeared.  The TREE_VALUE of each node is a STRING_CST.  */

static tree
cp_parser_asm_clobber_list (parser)
     cp_parser *parser;
{
  tree clobbers = NULL_TREE;

  while (true)
    {
      cp_token *token;
      tree string_literal;

      /* Look for the string literal.  */
      token = cp_parser_require (parser, CPP_STRING, "string-literal");
      string_literal = token ? token->value : error_mark_node;
      /* Add it to the list.  */
      clobbers = tree_cons (NULL_TREE, string_literal, clobbers);
      /* If the next token is not a `,', then the list is 
	 complete.  */
      if (cp_lexer_next_token_is_not (parser->lexer, CPP_COMMA))
	break;
      /* Consume the `,' token.  */
      cp_lexer_consume_token (parser->lexer);
    }

  return clobbers;
}

/* Parse an (optional) series of attributes.

   attributes:
     attributes attribute

   attribute:
     __attribute__ (( attribute-list [opt] ))  

   The return value is as for cp_parser_attribute_list.  */
     
static tree
cp_parser_attributes_opt (parser)
     cp_parser *parser;
{
  tree attributes = NULL_TREE;

  while (true)
    {
      cp_token *token;
      tree attribute_list;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's not `__attribute__', then we're done.  */
      if (token->keyword != RID_ATTRIBUTE)
	break;

      /* Consume the `__attribute__' keyword.  */
      cp_lexer_consume_token (parser->lexer);
      /* Look for the two `(' tokens.  */
      cp_parser_require (parser, CPP_OPEN_PAREN, "`('");
      cp_parser_require (parser, CPP_OPEN_PAREN, "`('");

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      if (token->type != CPP_CLOSE_PAREN)
	/* Parse the attribute-list.  */
	attribute_list = cp_parser_attribute_list (parser);
      else
	/* If the next token is a `)', then there is no attribute
	   list.  */
	attribute_list = NULL;

      /* Look for the two `)' tokens.  */
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
      cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

      /* Add these new attributes to the list.  */
      attributes = chainon (attributes, attribute_list);
    }

  return attributes;
}

/* Parse an attribute-list.  

   attribute-list:  
     attribute 
     attribute-list , attribute

   attribute:
     identifier     
     identifier ( identifier )
     identifier ( identifier , expression_list )
     identifier ( expression_list ) 

   Returns a TREE_LIST.  Each node corresponds to an attribute.  THe
   TREE_PURPOSE of each node is the identifier indicating which
   attribute is in use.  The TREE_VALUE represents the arguments, if
   any.  */

static tree
cp_parser_attribute_list (parser)
     cp_parser *parser;
{
  tree attribute_list = NULL_TREE;

  while (true)
    {
      cp_token *token;
      tree identifier;
      tree attribute;

      /* Look for the identifier.  We also allow keywords here; for
	 example `__attribute__ ((const))' is legal.  */
      token = cp_lexer_peek_token (parser->lexer);
      if (token->type != CPP_NAME 
	  && token->type != CPP_KEYWORD)
	return error_mark_node;
      /* Consume the token.  */
      token = cp_lexer_consume_token (parser->lexer);
      
      /* Save away the identifier that indicates which attribute this is.  */
      identifier = token->value;
      attribute = build_tree_list (identifier, NULL_TREE);

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If it's an `(', then parse the attribute arguments.  */
      if (token->type == CPP_OPEN_PAREN)
	{
	  tree arguments;
	  int arguments_allowed_p = 1;

	  /* Consume the `('.  */
	  cp_lexer_consume_token (parser->lexer);
	  /* Peek at the next token.  */
	  token = cp_lexer_peek_token (parser->lexer);
	  /* Check to see if the next token is an identifier.  */
	  if (token->type == CPP_NAME)
	    {
	      /* Save the identifier.  */
	      identifier = token->value;
	      /* Consume the identifier.  */
	      cp_lexer_consume_token (parser->lexer);
	      /* Peek at the next token.  */
	      token = cp_lexer_peek_token (parser->lexer);
	      /* If the next token is a `,', then there are some other
		 expressions as well.  */
	      if (token->type == CPP_COMMA)
		/* Consume the comma.  */
		cp_lexer_consume_token (parser->lexer);
	      else
		arguments_allowed_p = 0;
	    }
	  else
	    identifier = NULL_TREE;

	  /* If there are arguments, parse them too.  */
	  if (arguments_allowed_p)
	    arguments = cp_parser_expression_list (parser);
	  else
	    arguments = NULL_TREE;

	  /* Combine the identifier and the arguments.  */
	  if (identifier)
	    arguments = tree_cons (NULL_TREE, identifier, arguments);

	  /* Save the identifier and arguments away.  */
	  TREE_VALUE (attribute) = arguments;

	  /* Look for the closing `)'.  */
	  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");
	}

      /* Add this attribute to the list.  */
      TREE_CHAIN (attribute) = attribute_list;
      attribute_list = attribute;

      /* Now, look for more attributes.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If the next token isn't a `,', we're done.  */
      if (token->type != CPP_COMMA)
	break;

      /* Consume the commma and keep going.  */
      cp_lexer_consume_token (parser->lexer);
    }

  /* We built up the list in reverse order.  */
  return nreverse (attribute_list);
}

/* Parse an optional `__extension__' keyword.  Returns TRUE if it is
   present, and FALSE otherwise.  *SAVED_PEDANTIC is set to the
   current value of the PEDANTIC flag, regardless of whether or not
   the `__extension__' keyword is present.  The caller is responsible
   for restoring the value of the PEDANTIC flag.  */

static bool
cp_parser_extension_opt (parser, saved_pedantic)
     cp_parser *parser;
     int *saved_pedantic;
{
  /* Save the old value of the PEDANTIC flag.  */
  *saved_pedantic = pedantic;

  if (cp_lexer_next_token_is_keyword (parser->lexer, RID_EXTENSION))
    {
      /* Consume the `__extension__' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* We're not being pedantic while the `__extension__' keyword is
	 in effect.  */
      pedantic = 0;

      return true;
    }

  return false;
}
   
/* Support Functions */

/* Looks up NAME in the current scope, as given by PARSER->SCOPE.
   NAME should have one of the representations used for an
   id-expression.  If NAME is the ERROR_MARK_NODE, the ERROR_MARK_NODE
   is returned.  If PARSER->SCOPE is a dependent type, then a
   SCOPE_REF is returned.

   If NAME is a TEMPLATE_ID_EXPR, then it will be immediately
   returned; the name was already resolved when the TEMPLATE_ID_EXPR
   was formed.  Abstractly, such entities should not be passed to this
   function, because they do not need to be looked up, but it is
   simpler to check for this special case here, rather than at the
   call-sites.

   In cases not explicitly covered above, this function returns a DECL
   or OVERLOAD representing the result of the lookup.  If there was no
   entity with the indicated NAME, the ERROR_MARK_NODE.  */

static tree
cp_parser_lookup_name (parser, name)
     cp_parser *parser;
     tree name;
{
  tree decl;

  if (name == error_mark_node)
    return error_mark_node;

  /* FIXME: What if PARSER->SCOPE is set?  Do we need to represent the
     qualified version of the TEMPLATE_ID_EXPR?  Think about the case
     where PARSER->SCOPE is a dependent type.  */
  /* A template-id has already been resolved; there is no lookup to
     do.  */
  if (TREE_CODE (name) == TEMPLATE_ID_EXPR)
    return name;

  /* By this point, the NAME should be an ordinary identifier.  If
     the id-expression was a qualified name, the qualifying scope is
     stored in PARSER->SCOPE at this point.  */
  my_friendly_assert (TREE_CODE (name) == IDENTIFIER_NODE,
		      20000619);
  
  /* Perform the lookup.  */
  if (parser->scope)
    {
      /* If the SCOPE is dependent, the lookup must be deferred until
	 the template is instantiated.  */
      if (cp_parser_dependent_type_p (parser->scope))
	decl = build_nt (SCOPE_REF, parser->scope, name);
      else
	decl = lookup_qualified_name (parser->scope, name);
    }
  else
    decl = lookup_name (name, /*prefer_type=*/0);

  /* If the lookup failed, let our caller know.  */
  if (!decl)
    return error_mark_node;

  /* If it's a baselink, get the underlying declaration.  */
  if (BASELINK_P (decl))
    decl = TREE_VALUE (decl);
  /* If it's a TREE_LIST, the result of the lookup was ambiguous.  */
  else if (TREE_CODE (decl) == TREE_LIST)
    return error_mark_node;

  my_friendly_assert (DECL_P (decl) 
		      || TREE_CODE (decl) == OVERLOAD
		      || TREE_CODE (decl) == SCOPE_REF,
		      20000619);

  return decl;
}

/* If *IDK is not CP_PARSER_ID_KIND_NONE,look up *ID, which must be
   the representation of an id.  For an unqualified name, replace *ID
   with the DECL, or a LOOKUP_EXPR if *ID is a dependent name.  For a
   qualified name, replaced *ID with an OFFSET_REF.  *IDK is always
   CP_PARSER_ID_KIND_NONE when this function returns.  */

static void
cp_parser_maybe_lookup_name (parser, id, idk)
     cp_parser *parser;
     tree *id;
     cp_parser_id_kind *idk;
{
  tree identifier = *id;

  switch (*idk)
    {
    case CP_PARSER_ID_KIND_NONE:
      /* Do nothing if this is not an unqualified-id.  */
      return;

    case CP_PARSER_ID_KIND_UNQUALIFIED:
      {
	bool dependent_p = false;

	/* Make sure that we look for names in the current scope.  */
	parser->scope = NULL_TREE;
	/* Look up the name.  */
	*id = cp_parser_lookup_name (parser, identifier);
	/* [temp.dep.expr]
	 
	   An id-expression is type-dependent if it contains an
	   identifier that was declared with a dependent type.
	   
	   As an optimization, we could choose not to create a
	   LOOKUP_EXPR for a name that resolved to a local variable in
	   the template function that we are currently declaring; such
	   a name cannot ever resolve to anything else.  If we did
	   that we would not have to look up these names at
	   instantiation time.
	   
	   The standard is not very specific about an id-expression
	   that names a set of overloaded functions.  What if some of
	   them have dependent types and some of them do not?
	   Presumably, such a name should be treated as a dependent
	   name.  For now, we just treat all OVERLOADs as dependent.  */
	if (*id)
	  {
	    /* For anything except an OVERLOAD, just check its type.  */
	    if (TREE_CODE (*id) != OVERLOAD)
	      dependent_p = cp_parser_dependent_type_p (TREE_TYPE (*id));
	    /* For a set of overloaded functions, check each of the
	       functions.  */
	    else
	      {
		tree fn;

		/* Look through the functions.  */
		for (fn = *id; fn; fn = OVL_NEXT (fn))
		  /* If this one has a dependent type, then the name
		     is dependent.  */
		  if (cp_parser_dependent_type_p 
		      (TREE_TYPE (OVL_CURRENT (fn))))
		    {
		      dependent_p = true;
		      break;
		    }
	      }
	  }
	/* If the name was dependent on a template parameter, create a
	   LOOKUP_EXPR for resolution at template-instantiation time.  */
	if (dependent_p)
	  *id = build_min_nt (LOOKUP_EXPR, identifier);
	/* Convert non-static data members and non-static data
	   member-functions to references relative to `this'.  */
	else
	  *id = hack_identifier (*id, identifier);
      }
      break;

    case CP_PARSER_ID_KIND_QUALIFIED:
      /* The *ID should be a SCOPE_REF representing the qualified
	 name.  */
      my_friendly_assert (TREE_CODE (*id) == SCOPE_REF, 
			 20010624);
      /* Build an OFFSET_REF to represent the qualified-id.  */
      *id = build_offset_ref (TREE_OPERAND (*id, 0),
			      TREE_OPERAND (*id, 1));
      break;

    default:
      my_friendly_abort (20010624);
      break;
    }

  /* We have resolved the id.  */
  *idk = CP_PARSER_ID_KIND_NONE;
}

/* TYPE is a TYPENAME_TYPE.  Returns the ordinary TYPE to which the
   TYPENAME_TYPE corresponds.  Note that this function peers inside
   uninstantiated templates and therefore should be used only in
   extremely limited situations.  */

static tree
cp_parser_resolve_typename_type (type)
     tree type;
{
  tree scope;
  tree name;
  tree decl;

  my_friendly_assert (TREE_CODE (type) == TYPENAME_TYPE,
		      20010702);

  scope = TYPE_CONTEXT (type);
  name = DECL_NAME (TYPE_NAME (type));

  /* If the SCOPE is itself a TYPENAME_TYPE, then we need to resolve
     it first before we can figure out what NAME refers to.  */
  if (TREE_CODE (scope) == TYPENAME_TYPE)
    scope = cp_parser_resolve_typename_type (scope);
  /* FIXME: Should we check that SCOPE is a class type?  */
  /* If we don't know what SCOPE refers to, then we cannot resolve the
     TYPENAME_TYPE.  */
  if (scope == error_mark_node)
    return error_mark_node;
  /* Enter the SCOPE so that name lookup will be resolved as if we
     were in the class definition.  In particular, SCOPE will no
     longer be considered a dependent type.  */
  push_scope (scope);
  /* Look up the declaration.  */
  decl = lookup_member (scope, name, /*protect=*/0, /*want_type=*/1);
  /* If all went well, we got a TYPE_DECL for a non-typename.  */
  if (!decl 
      || TREE_CODE (decl) != TYPE_DECL 
      || TREE_CODE (TREE_TYPE (decl)) == TYPENAME_TYPE)
    {
      /* FIXME: This isn't a great error message.  Also, are these
	 checks semantic enough that we should be doing them here?
	 Also, should we be doing this in
	 cp_parser_nested_name_specifier rather than here, after the
	 fact?  */
      cp_error ("invalid type `%T::%D'", scope, name);
      type = error_mark_node;
    }
  else
    type = TREE_TYPE (decl);
  /* Leave the SCOPE.  */
  pop_scope (scope);

  /* FIXME: Should we check that TYPE is a class type?  */
  return type;
}

/* Returns the SCOPE (if any) in which the entity declared by
   DECLARATOR will be located.  If the entity is declared in the
   current scope, NULL_TREE is returned.  */

static tree
cp_parser_get_scope_of_declarator (declarator)
     tree declarator;
{
  switch (TREE_CODE (declarator))
    {
    case CALL_EXPR:
    case ARRAY_REF:
    case INDIRECT_REF:
    case ADDR_EXPR:
      /* For any of these, the main declarator is the first operand.  */
      return cp_parser_get_scope_of_declarator (TREE_OPERAND
						(declarator, 0));

    case SCOPE_REF:
      /* If the declarator-id is a SCOPE_REF, the scope in which the
	 declaration occurs is the first operand.  */
      return TREE_OPERAND (declarator, 0);

    default:
      /* Otherwise, we have a declarator-id which is not a qualified
	 name; the entity will be declared in the current scope.  */
      return NULL_TREE;
    }
}

/* If too many, or too few, template-parameter lists apply to the
   declarator, issue an error message.  Returns TRUE if all went well,
   and FALSE otherwise.  */

/* FIXME: This is probably semantic analysis, and should go
   elsewhere.  */

static bool
cp_parser_check_template_parameters (declarator, 
				     num_parameter_lists)
     tree declarator;
     unsigned num_parameter_lists;
{
  unsigned num_template_classes;

  /* We haven't seen any classes that involve template parameters yet.  */
  num_template_classes = 0;

  switch (TREE_CODE (declarator))
    {
    case CALL_EXPR:
    case ARRAY_REF:
    case INDIRECT_REF:
    case ADDR_EXPR:
      {
	tree main_declarator = TREE_OPERAND (declarator, 0);
	return
	  cp_parser_check_template_parameters (main_declarator,
					       num_parameter_lists);
      }

    case SCOPE_REF:
      {
	tree scope;
	
	scope = TREE_OPERAND (declarator, 0);

	while (scope && CLASS_TYPE_P (scope))
	  {
	    /* You're supposed to have one `template <...>'
	       for every template class, but you don't need one
	       for a full specialization.  For example:
	       
	       template <class T> struct S{};
	       template <> struct S<int> { void f(); };
	       void S<int>::f () {}
	       
	       is correct; there shouldn't be a `template <>' for
	       the definition of `S<int>::f'.  */
	    if (CLASSTYPE_TEMPLATE_INFO (scope)
		&& (CLASSTYPE_TEMPLATE_INSTANTIATION (scope)
		    || uses_template_parms (CLASSTYPE_TI_ARGS (scope)))
		&& PRIMARY_TEMPLATE_P (CLASSTYPE_TI_TEMPLATE (scope)))
	      ++num_template_classes;

	    scope = TYPE_CONTEXT (scope);
	  }
      }

      /* Fall through.  */

    default:
      /* If there are more template classes than parameter lists, we 
	 have something like:
	   
	 template <class T> void S<T>::R<T>::f ();  */
      if (num_parameter_lists < num_template_classes)
	{
	  cp_error ("too few template-parameter-lists");
	  return false;
	}
      /* If there are the same number of template classes and
	 parameter lists, that's OK.  */
      if (num_parameter_lists == num_template_classes)
	return true;
      /* If there are more, but only one more, then we are referring
	 to a member template.  That's OK too.  */
      if (num_parameter_lists == num_template_classes + 1)
	return true;
      /* Otherwise, there are too many template parameter lists.  We
	 have something like:

	 template <class T> template <class U> void S<T>::f();  */
      cp_error ("too many template-parameter-lists");
      return false;
    }
}

/* Parse a binary-expression of the general form:

   binary-expression:
     <expr>
     binary-expression <token> <expr>

   The TOKEN_TREE_MAP maps <token> types to <expr> codes.  FN is used
   to parser the <expr>s.  If the first production is used, then the
   value returned by FN is returned directly.  Otherwise, a node with
   the indicated EXPR_TYPE is returned, with operands corresponding to
   the two sub-expressions.  */

static tree
cp_parser_binary_expression (parser, token_tree_map, fn)
     cp_parser *parser;
     cp_parser_token_tree_map token_tree_map;
     cp_parser_expression_fn fn;
{
  tree lhs;

  /* Parse the first expression.  */
  lhs = (*fn) (parser);
  /* Now, look for more expressions.  */
  while (true)
    {
      cp_token *token;
      cp_parser_token_tree_map_node *map_node;
      tree rhs;

      /* Peek at the next token.  */
      token = cp_lexer_peek_token (parser->lexer);
      /* If the token is `>', and that's not an operator at the
	 moment, then we're done.  */
      if (token->type == CPP_GREATER
	  && !parser->greater_than_is_operator_p)
	break;
      /* If we find one of the tokens we want, build the correspoding
	 tree representation.  */
      for (map_node = token_tree_map; 
	   map_node->token_type != CPP_EOF;
	   ++map_node)
	if (map_node->token_type == token->type)
	  {
	    /* Consume the operator token.  */
	    cp_lexer_consume_token (parser->lexer);
	    /* Parse the right-hand side of the expression.  */
	    rhs = (*fn) (parser);
	    /* Build the binary tree node.  */
	    lhs = build_x_binary_op (map_node->tree_type, lhs, rhs);
	    break;
	  }

      /* If the token wasn't one of the ones we want, we're done.  */
      if (map_node->token_type == CPP_EOF)
	break;
    }

  return lhs;
}

/* Parse an optional `::' token indicating that the following name is
   from the global namespace.  If so, PARSER->SCOPE is set to the
   GLOBAL_NAMESPACE. Otherwise, PARSER->SCOPE is set to NULL_TREE,
   unless CURRENT_SCOPE_VALID_P is TRUE, in which case it is left alone.
   Returns the new value of PARSER->SCOPE, if the `::' token is
   present, and NULL_TREE otherwise.  */

static tree
cp_parser_global_scope_opt (parser, current_scope_valid_p)
     cp_parser *parser;
     bool current_scope_valid_p;
{
  cp_token *token;

  /* Peek at the next token.  */
  token = cp_lexer_peek_token (parser->lexer);
  /* If we're looking at a `::' token then we're starting from the
     global namespace, not our current location.  */
  if (token->type == CPP_SCOPE)
    {
      /* Consume the `::' token.  */
      cp_lexer_consume_token (parser->lexer);
      /* Set the SCOPE so that we know where to start the lookup.  */
      parser->scope = global_namespace;

      return parser->scope;
    }
  else if (!current_scope_valid_p)
    parser->scope = NULL_TREE;
  
  return NULL_TREE;
}

/* Parse the part of a function-definition that follows the
   declarator.  INLINE_P is TRUE iff this function is an inline
   function defined with a class-specifier.

   Returns the function defined.  */

static tree 
cp_parser_function_definition_after_declarator (parser, 
						inline_p)
     cp_parser *parser;
     bool inline_p;
{
  tree fn;
  bool ctor_initializer_p = false;

  /* If it's `try', then we are looking at a function-try-block.  */
  if (cp_lexer_next_token_is_keyword (parser->lexer, RID_TRY))
    cp_parser_function_try_block (parser);
  /* A function-try-block includes the function-body, so we only do
     this next part if we're not processing a function-try-block.  */
  else
    {
      /* If it a `:', then we're looking at a ctor-initializer.  */
      if (cp_lexer_next_token_is (parser->lexer, CPP_COLON))
	{
	  cp_parser_ctor_initializer (parser);
	  ctor_initializer_p = true;
	}
      /* Parse the function-body.  */
      cp_parser_function_body (parser);
    }

  /* Finish the function.  */
  fn = finish_function ((ctor_initializer_p ? 1 : 0) | 
			(inline_p ? 2 : 0));
  /* Generate code for it, if necessary.  */
  expand_body (fn);

  return fn;
}

/* Parse a template-declaration, assuming that the `export' keyword,
   if present, has already been scanned.  MEMBER_P is as for
   cp_parser_template_declaration.  NUM_PARAMETER_LISTS is the number
   of parameter lists that have been processed thus far.  */

static void
cp_parser_template_declaration_after_export (parser,
					     member_p, 
					     num_parameter_lists)
     cp_parser *parser;
     bool member_p;
     unsigned num_parameter_lists;
{
  tree decl = NULL_TREE;
  tree parameter_list;

  /* Look for the `template' keyword.  */
  if (!cp_parser_require_keyword (parser, RID_TEMPLATE, "`template'"))
    return;
      
      /* And the `<'.  */
  if (!cp_parser_require (parser, CPP_LESS, "`<'"))
    return;
      
  /* FIXME: Specializations need multiple lists too.  */
  /* Parse the template parameters.  */
  begin_template_parm_list ();
  parameter_list = cp_parser_template_parameter_list (parser);
  parameter_list = end_template_parm_list (parameter_list);
      
  /* Look for the `>'.  */
  if (cp_parser_require (parser, CPP_GREATER, "`>'"))
    {
      /* We just processed one more parameter list.  */
      ++num_parameter_lists;
      /* If the next token is `template', there are more template
	 parameters.  */
      if (cp_lexer_next_token_is_keyword (parser->lexer, 
					  RID_TEMPLATE))
	cp_parser_template_declaration_after_export (parser,
						     member_p,
						     num_parameter_lists);
      else
	{
	  tree decl_specifiers;

	  decl = cp_parser_single_declaration (parser,
					       num_parameter_lists,
					       &decl_specifiers);

	  /* If this is a member template declaration, let the front
	     end know.  */
	  if (member_p)
	    {
	      /* If it was a friend, there will be no DECL.  */
	      if (!decl)
		;
	      else if (TYPE_P (decl))
		finish_member_class_template (decl);
	      else
		decl = finish_member_template_decl (decl);
	    }
	}
    }

  /* Finish up.  */
  finish_template_decl (parameter_list);

  /* Register member declarations.  */
  if (member_p && decl && !TYPE_P (decl))
    finish_member_declaration (decl);
}

/* Parse a `decl-specifier-seq [opt] init-declarator [opt] ;' or
   `function-definition' sequence.  NUM_PARAMETER_LISTS is the number
   of template-parameter lists that apply to this declaration.  If
   DECL_SPECIFIERS_P is non-NULL, and the declaration is not a
   function-definition, it is filled in with the decl-specifier-seq,
   represented as documented by cp_parser_decl_specifier_seq.

   Returns the DECL for the declared entity.  */

static tree
cp_parser_single_declaration (parser, 
			      num_parameter_lists, 
			      decl_specifiers_p)
     cp_parser *parser;
     unsigned num_parameter_lists;
     tree *decl_specifiers_p;
{
  bool declares_class_or_enum;
  tree decl;
  tree decl_specifiers;
  tree attributes;

  /* Parse the dependent declaration.  We don't know yet
     whether it will be a function-definition.  */
  cp_parser_parse_tentatively (parser);
  /* Try the `decl-specifier-seq [opt] init-declarator [opt]'
     alternative.  */
  decl_specifiers 
    = cp_parser_decl_specifier_seq (parser,
				    CP_PARSER_FLAGS_OPTIONAL,
				    &attributes,
				    &declares_class_or_enum);
  /* Check for the declaration of a template class.  */
  if (declares_class_or_enum)
    decl = cp_parser_declares_only_class_p (parser, 
					    decl_specifiers,
					    NULL);
  else
    decl = NULL_TREE;
  /* If it's not a template class, try for a template function.  */
  if (!decl)
    decl = cp_parser_init_declarator (parser, 
				      decl_specifiers,
				      attributes,
				      num_parameter_lists);
  /* Look for a trailing `;' after the declaration.  */
  cp_parser_require (parser, CPP_SEMICOLON, "expected `;'");
  /* If that didn't work, try a function-definition.  */
  if (!cp_parser_parse_definitely (parser))
    {
      decl_specifiers = NULL_TREE;
      decl = cp_parser_function_definition (parser,
					    num_parameter_lists);
    }

  /* Make the decl-specifiers available to our caller.  */
  if (decl_specifiers_p)
    *decl_specifiers_p = decl_specifiers;

  return decl;
}

/* Parse a functional cast to TYPE.  Returns an expression
   representing the cast.  */

static tree
cp_parser_functional_cast (parser, type)
     cp_parser *parser;
     tree type;
{
  tree expression_list;

  /* Look for the opening `('.  */
  if (!cp_parser_require (parser, CPP_OPEN_PAREN, "`('"))
    return error_mark_node;
  /* If the next token is not an `)', there are arguments to the
     cast.  */
  if (cp_lexer_next_token_is_not (parser->lexer, CPP_CLOSE_PAREN))
    expression_list = cp_parser_expression_list (parser);
  else
    expression_list = NULL_TREE;
  /* Look for the closing `)'.  */
  cp_parser_require (parser, CPP_CLOSE_PAREN, "`)'");

  return build_functional_cast (type, expression_list);
}

/* If the DECL_SPECIFIER_SEQ declares a class, and constitutes the
   entire declaration, return the TYPE for the class.  Otherwise,
   return NULL_TREE.  Sets *FRIEND_P to TRUE iff the `friend'
   specifier appears in the decl-specifier-seq.  */

static tree
cp_parser_declares_only_class_p (parser, 
				 decl_specifier_seq,
				 friend_p)
     cp_parser *parser;
     tree decl_specifier_seq;
     bool *friend_p;
{
  tree decl_specifier;
  tree type = NULL_TREE;
  unsigned count = 0;

  /* Assume that there is no `friend' specifier.  */
  if (friend_p)
    *friend_p = false;

  for (decl_specifier = decl_specifier_seq;
       decl_specifier;
       decl_specifier = TREE_CHAIN (decl_specifier))
    {
      /* The `friend' keyword is OK.  */
      if (TREE_CODE (TREE_VALUE (decl_specifier)) == IDENTIFIER_NODE
	  && C_IS_RESERVED_WORD (TREE_VALUE (decl_specifier))
	  && C_RID_CODE (TREE_VALUE (decl_specifier)) == RID_FRIEND)
	{
	  if (friend_p)
	    *friend_p = true;
	  continue;
	}

      /* If there's more than one decl-specifier, it declares
	 something more than a single class.  */
      if (++count > 1)
	return NULL_TREE;

      type = TREE_VALUE (decl_specifier);
    }

  /* If there's only one decl-specifier, and the next token is a `;',
     then there is no declarator.  */
  if (cp_lexer_next_token_is (parser->lexer, CPP_SEMICOLON)
      && count == 1)
    {
      my_friendly_assert (type == error_mark_node || TYPE_P (type),
			  20010712);
      return type;
    }

  return NULL_TREE;
}

/* If the next token is of the indicated TYPE, consume it.  Otherwise,
   issue an error message indicating that TOKEN_DESC was expected.
   
   Returns the token consumed, if the token had the appropriate type.
   Otherwise, returns NULL.  */

static cp_token *
cp_parser_require (parser, type, token_desc)
     cp_parser *parser;
     enum cpp_ttype type;
     const char *token_desc;
{
  cp_token *token;

  token = cp_lexer_peek_token (parser->lexer);
  if (token->type == type)
    {
      cp_lexer_consume_token (parser->lexer);
      return token;
    }
  else
    {
      dyn_string_t error_msg;

      /* Format the error message.  */
      error_msg = dyn_string_new (0);
      dyn_string_append_cstr (error_msg, "expected ");
      dyn_string_append_cstr (error_msg, token_desc);
      cp_parser_error (parser, error_msg->s);
      dyn_string_delete (error_msg);
      return NULL;
    }
}

/* If the next token is the indicated keyword, consume it.  Otherwise,
   issue an error message indicating that TOKEN_DESC was expected.
   
   Returns the token consumed, if the token had the appropriate type.
   Otherwise, returns NULL.  */

static cp_token *
cp_parser_require_keyword (parser, keyword, token_desc)
     cp_parser *parser;
     enum rid keyword;
     const char *token_desc;
{
  cp_token *token = cp_parser_require (parser, CPP_KEYWORD, token_desc);

  if (token && token->keyword != keyword)
    {
      dyn_string_t error_msg;

      /* Format the error message.  */
      error_msg = dyn_string_new (0);
      dyn_string_append_cstr (error_msg, "expected ");
      dyn_string_append_cstr (error_msg, token_desc);
      cp_parser_error (parser, error_msg->s);
      dyn_string_delete (error_msg);
      return NULL;
    }

  return token;
}

/* Begin parsing tentatively.  We always save tokens while parsing
   tentatively so that if the tentative parsing fails we can restore the
   tokens.  */

static void
cp_parser_parse_tentatively (parser)
     cp_parser *parser;
{
  /* Indicate that we are parsing tentatively, and that no error has
     ocurred yet.  */
  VARRAY_PUSH_UINT (parser->parsing_tentatively, 0);
  /* Begin saving tokens.  */
  cp_lexer_save_tokens (parser->lexer);
  /* Any error messages that we issue must be queued up until we know
     whether or not we want to commit to this tentative parse.  */
  diagnostic_issue_tentatively (diagnostic_buffer);
}

/* Commit to the currently active tentative parse.  */

static void
cp_parser_commit_to_tentative_parse (parser)
     cp_parser *parser;
{
  size_t i;

  /* Commit to the tentative parse.  */
  for (i = 0; 
       i < VARRAY_ACTIVE_SIZE (parser->parsing_tentatively);
       ++i)
    {
      if (VARRAY_UINT (parser->parsing_tentatively, i) != 2)
	{
	  /* Mark this level as having been committed.  */
	  VARRAY_UINT (parser->parsing_tentatively, i) = 2;
	  /* Issue any diagnostics queud up until this point, and begin
	     issuing diagnostics immediately.  */
	  diagnostic_commit (diagnostic_buffer);
	}
    }
}

/* Abort the currently active tentative parse.  All consumed tokens
   will be rolled back, and no diagnostics will be issued.  */

static void
cp_parser_abort_tentative_parse (parser)
     cp_parser *parser;
{
  cp_parser_simulate_error (parser);
  /* Now, pretend that we want to see if the construct was
     successfully parsed.  */
  cp_parser_parse_definitely (parser);
}

/* Stop parsing tentatively.  If a parse error has ocurred, restore the
   token stream.  Otherwise, commit to the tokens we have consumed.
   Returns true if no error occurred; false otherwise.  */

static bool
cp_parser_parse_definitely (parser)
     cp_parser *parser;
{
  bool error_occurred;
  bool committed;

  /* Remember whether or not an error ocurred, since we are about to
     destroy that information.  */
  error_occurred = cp_parser_error_occurred (parser);
  /* Similary, for whether or not we committed to this tentative
     parse.  */
  committed = cp_parser_committed_to_tentative_parse (parser);
  /* We are no longer parsing tentatively.  */
  VARRAY_POP (parser->parsing_tentatively);
  /* Restore the tokens, or commit to consuming them.  */
  if (!error_occurred)
    {
      cp_lexer_commit_tokens (parser->lexer);
      /* If we already committed to this tentative parse then we
	 committed the diagnostics at that point.  */
      if (!committed)
	diagnostic_commit (diagnostic_buffer);
      return true;
    }
  else
    {
      cp_lexer_rollback_tokens (parser->lexer);
      diagnostic_rollback (diagnostic_buffer);
      return false;
    }
}

/* Returns non-zero if we are parsing tentatively.  */

static bool
cp_parser_parsing_tentatively (parser)
     cp_parser *parser;
{
  return VARRAY_ACTIVE_SIZE (parser->parsing_tentatively) != 0;
}

/* Returns true if we are parsing tentatively -- but have decided that
   we will stick with this tentative parse, even if errors occur.  */

static bool
cp_parser_committed_to_tentative_parse (parser)
     cp_parser *parser;
{
  return (cp_parser_parsing_tentatively (parser)
	  && VARRAY_TOP_UINT (parser->parsing_tentatively) == 2);
}

/* Returns non-zero iff an error has occurred during the most recent
   tentative parse.  */
   
static bool
cp_parser_error_occurred (parser)
     cp_parser *parser;
{
  return (cp_parser_parsing_tentatively (parser)
	  && VARRAY_TOP_UINT (parser->parsing_tentatively) == 1);
}

/* Returns non-zero if GNU extensions are allowed.  */

static bool
cp_parser_allow_gnu_extensions_p (parser)
     cp_parser *parser;
{
  return parser->allow_gnu_extensions_p;
}


/* External interface.  */

/* Parse the entire translation unit.  Return a non-zero value if an
   error occurs.  */

int
cp_parse_translation_unit ()
{
  int error_ocurred;
  cp_parser *parser;

  parser = cp_parser_new ();
  error_ocurred = !cp_parser_translation_unit (parser);
  cp_parser_delete (parser);

  return error_ocurred;
}

/* Mark the DECL_PENDING_INLINE_INFO for FN.  */

void
ggc_mark_inline_definition (fn)
     tree fn;
{
  cp_parser_inline_definition *definition;
  cp_token *first_token;
  cp_token *last_token;

  definition = DECL_PENDING_INLINE_INFO (fn);

  /* Figure out what the first and last tokens are.  */
  first_token 
    = (cp_token *) obstack_base (&definition->token_obstack);
  last_token 
    = (cp_token *) obstack_next_free (&definition->token_obstack);

  /* Loop over all the tokens, marking any trees pointed to by them.  */
  while (first_token != last_token)
    {
      /* Mark the VALUE field.  */
      ggc_mark_tree (first_token->value);
      /* Move to the next token.  */
      ++first_token;
    }
}

/* This function must be provided by every front-end.  */

void
set_yydebug (debug_p)
     int debug_p ATTRIBUTE_UNUSED;
{
  cp_warning ("no parser debugging available in C++");
}

/* FIXME: Implement these.  */

void 
add_defarg_fn (decl)
     tree decl ATTRIBUTE_UNUSED;
{
}


void
unprocessed_defarg_fn (fn)
     tree fn ATTRIBUTE_UNUSED;
{
}


void
do_pending_defargs ()
{
}

