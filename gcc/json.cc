/* JSON parsing
   Copyright (C) 2017 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include <dirent.h> // for selftests
#include "json.h"
#include "pretty-print.h"
#include "math.h"
#include "selftest.h"

using namespace json;

/* class json::value.  */

/* Generate a char * for this json::value tree.
   The returned value must be freed by the caller.  */

char *
value::to_str () const
{
  pretty_printer pp;
  print (&pp);
  return xstrdup (pp_formatted_text (&pp));
}

/* Dump this json::value tree to OUTF.
   No formatting is done.  There are no guarantees about the order
   in which the key/value pairs of json::objects are printed.  */

void
value::dump (FILE *outf) const
{
  pretty_printer pp;
  pp_buffer (&pp)->stream = outf;
  print (&pp);
  pp_flush (&pp);
}

/* If this json::value is a json::object, return it,
   otherwise return NULL.  */

const object *
value::as_object () const
{
  if (get_kind () != JSON_OBJECT)
    return NULL;
  return static_cast <const object *> (this);
}

/* If this json::value is a json::array, return it,
   otherwise return NULL.  */

const array *
value::as_array () const
{
  if (get_kind () != JSON_ARRAY)
    return NULL;
  return static_cast <const array *> (this);
}

/* If this json::value is a json::number, return it,
   otherwise return NULL.  */

const number *
value::as_number () const
{
  if (get_kind () != JSON_NUMBER)
    return NULL;
  return static_cast <const number *> (this);
}

/* If this json::value is a json::string, return it,
   otherwise return NULL.  */

const string *
value::as_string () const
{
  if (get_kind () != JSON_STRING)
    return NULL;
  return static_cast <const string *> (this);
}

/* Attempt to get the value of a key/value pair from this value
   as if THIS value were an object.

   If THIS is not a json::object, return write an error message to OUT_ERR
   (which must be freed by the caller) and return false.

   Otherwise write the value ptr (possibly NULL) to OUT_VALUE and
   return true.  */

bool
value::get_optional_value_by_key (const char *name, const value *&out_value,
				  char *&out_err) const
{
  const json::object *obj = as_object ();
  if (!obj)
    {
      out_err = xstrdup ("not an object");
      return false;
    }
  out_value = obj->get (name);
  return true;
}

/* Attempt to get a string value of a key/value pair from this value
   as if THIS value were an object.

   If THIS is a json::object, and KEY is either not present, is a string,
   or is the "null" JSON literal, then return true, and write to OUT_VALUE.
   If a string, then the ptr is written to OUT_VALUE, otherwise NULL
   is written to OUT_VALUE.

   If THIS is not a json::object, or KEY is not a string/"null",
   return false and write an error message to OUT_ERR
   (which must be freed by the caller).  */

bool
value::get_optional_string_by_key (const char *name, const char *&out_value,
				   char *&out_err) const
{
  const json::value *v;
  if (!get_optional_value_by_key (name, v, out_err))
    return false;
  if (v && v->get_kind () != JSON_NULL)
    {
      const json::string *s = v->as_string ();
      if (!s)
	{
	  out_err = xasprintf ("not a string: \"%s\"", name);
	  return false;
	}
      out_value = s->get_string ();
      return true;
    }
  else
    {
      out_value = NULL;
      return true;
    }
}

/* Attempt to get lookup the value of a key/value pair from this value
   as if this value were an object.

   To succeed, THIS must be a json::object, and it must have a key named
   NAME.

   On success, return true and write the value to OUT_VALUE.
   On failure, return false and write an error message to OUT_ERR
   (which must be freed by the caller).  */

bool
value::get_value_by_key (const char *name, const value *&out_value,
			 char *&out_err) const
{
  const json::object *obj = as_object ();
  if (!obj)
    {
      out_err = xstrdup ("not an object");
      return false;
    }
  const json::value *v = obj->get (name);
  if (!v)
    {
      out_err = xasprintf ("missing attribute: \"%s\"", name);
      return false;
    }
  out_value = v;
  return true;
}

/* As value::get_value_by_key, but the value must be a number;
   if successful, write it as an int to OUT_VALUE.  */

bool
value::get_int_by_key (const char *name, int &out_value, char *&out_err) const
{
  const json::value *v;
  if (!get_value_by_key (name, v, out_err))
    return false;
  const json::number *n = v->as_number ();
  if (!n)
    {
      out_err = xasprintf ("not a number: \"%s\"", name);
      return false;
    }
  out_value = n->get ();
  return true;
}

/* As value::get_value_by_key, but the value must be a string;
   if successful, write it as const char * to OUT_VALUE.  */

bool
value::get_string_by_key (const char *name, const char *&out_value,
			  char *&out_err) const
{
  const json::value *v;
  if (!get_value_by_key (name, v, out_err))
    return false;
  const json::string *s = v->as_string ();
  if (!s)
    {
      out_err = xasprintf ("not a string: \"%s\"", name);
      return false;
    }
  out_value = s->get_string ();
  return true;
}

/* As value::get_value_by_key, but the value must be an array;
   if successful, write it as a json::array * to OUT_VALUE.  */

bool
value::get_array_by_key (const char *name, const array *&out_value,
			 char *&out_err) const
{
  const json::value *v;
  if (!get_value_by_key (name, v, out_err))
    return false;
  const json::array *arr = v->as_array ();
  if (!arr)
    {
      out_err = xasprintf ("not an array: \"%s\"", name);
      return false;
    }
  out_value = arr;
  return true;
}

/* class json::object, a subclass of json::value, representing
   an unordered collection of key/value pairs.  */

/* json:object's dtor.  */

object::~object ()
{
  for (map_t::iterator it = m_map.begin (); it != m_map.end (); ++it)
    {
      free (const_cast <char *>((*it).first));
      delete ((*it).second);
    }
}

/* Implementation of json::value::print for json::object.  */

void
object::print (pretty_printer *pp) const
{
  /* Note that the order is not guaranteed.  */
  pp_character (pp, '{');
  for (map_t::iterator it = m_map.begin (); it != m_map.end (); ++it)
    {
      if (it != m_map.begin ())
	pp_string (pp, ", ");
      const char *key = const_cast <char *>((*it).first);
      value *value = (*it).second;
      pp_printf (pp, "\"%s\": ", key); // FIXME: escaping?
      value->print (pp);
    }
  pp_character (pp, '}');
}

/* Implementation of json::value::clone for json::object.  */

value *
object::clone () const
{
  object *other = new object ();
  for (map_t::iterator it = m_map.begin (); it != m_map.end (); ++it)
    {
      const char *key = const_cast <char *>((*it).first);
      value *value = (*it).second;
      other->set (key, value->clone ());
    }
  return other;
}

/* Get the json::value * for KEY, or NULL if the key is not present.  */

value *
object::get (const char *key) const
{
  value **slot = const_cast <object*> (this)->m_map.get (key);
  if (slot)
    return *slot;
  return NULL;
}

/* As object::get (KEY), but return NULL if the value of the key
   is the "null" JSON literal.  */

value *
object::get_if_nonnull (const char *key) const
{
  value *result = get (key);
  if (!result)
    return NULL;
  if (result->get_kind () == JSON_NULL)
    return NULL;
  return result;
}

/* Set the json::value * for KEY, taking ownership of VALUE
   (and taking a copy of KEY if necessary).  */

void
object::set (const char *key, value *v)
{
  value **ptr = m_map.get (key);
  if (ptr)
    {
      /* If the key is already present, delete the existing value
	 and overwrite it.  */
      delete *ptr;
      *ptr = v;
    }
  else
    /* If the key wasn't already present, take a copy of the key,
       and store the value.  */
    m_map.put (xstrdup (key), v);
}

/* class json::array, a subclass of json::value, representing
   an ordered collection of values.  */

/* json::array's dtor.  */

array::~array ()
{
  unsigned i;
  value *v;
  FOR_EACH_VEC_ELT (m_elements, i, v)
    delete v;
}

/* Implementation of json::value::print for json::array.  */

void
array::print (pretty_printer *pp) const
{
  pp_character (pp, '[');
  unsigned i;
  value *v;
  FOR_EACH_VEC_ELT (m_elements, i, v)
    {
      if (i)
	pp_string (pp, ", ");
      v->print (pp);
    }
  pp_character (pp, ']');
}

/* Implementation of json::value::clone for json::array.  */

value *
array::clone () const
{
  array *other = new array ();
  unsigned i;
  value *v;
  FOR_EACH_VEC_ELT (m_elements, i, v)
    other->append (v->clone ());
  return other;
}

/* class json::number, a subclass of json::value, wrapping a double.  */

/* Implementation of json::value::print for json::number.  */

void
number::print (pretty_printer *pp) const
{
  char tmp[1024];
  snprintf (tmp, sizeof (tmp), "%g", m_value);
  pp_string (pp, tmp);
}

/* Implementation of json::value::clone for json::number.  */

value *
number::clone () const
{
  return new number (m_value);
}

/* class json::string, a subclass of json::value.  */

void
string::print (pretty_printer *pp) const
{
  pp_character (pp, '"');
  for (const char *ptr = m_utf8; *ptr; ptr++)
    {
      char ch = *ptr;
      switch (ch)
	{
	case '"':
	  pp_string (pp, "\\\"");
	  break;
	case '\\':
	  pp_string (pp, "\\n");
	  break;
	case '\b':
	  pp_string (pp, "\\b");
	  break;
	case '\f':
	  pp_string (pp, "\\f");
	  break;
	case '\n':
	  pp_string (pp, "\\n");
	  break;
	case '\r':
	  pp_string (pp, "\\r");
	  break;
	case '\t':
	  pp_string (pp, "\\t");
	  break;

	default:
	  pp_character (pp, ch);
	}
    }
  pp_character (pp, '"');
}

/* Implementation of json::value::clone for json::string.  */

value *
string::clone () const
{
  return new string (m_utf8);
}

/* class json::literal, a subclass of json::value.  */

/* Implementation of json::value::print for json::literal.  */

void
literal::print (pretty_printer *pp) const
{
  switch (m_kind)
    {
    case JSON_TRUE:
      pp_string (pp, "true");
      break;
    case JSON_FALSE:
      pp_string (pp, "false");
      break;
    case JSON_NULL:
      pp_string (pp, "null");
      break;
    default:
      gcc_unreachable ();
    }
}

/* Implementation of json::value::clone for json::literal.  */

value *
literal::clone () const
{
  return new literal (m_kind);
}


/* Declarations relating to parsing JSON, all within an
   anonymous namespace.  */

namespace {

/* A typedef representing a single unicode character.  */

typedef unsigned unichar;

/* An enum for discriminating different kinds of JSON token.  */

enum token_id
{
  TOK_ERROR,

  TOK_EOF,

  /* Punctuation.  */
  TOK_OPEN_SQUARE,
  TOK_OPEN_CURLY,
  TOK_CLOSE_SQUARE,
  TOK_CLOSE_CURLY,
  TOK_COLON,
  TOK_COMMA,

  /* Literal names.  */
  TOK_TRUE,
  TOK_FALSE,
  TOK_NULL,

  TOK_STRING,
  TOK_NUMBER
};

/* Human-readable descriptions of enum token_id.  */

static const char *token_id_name[] = {
  "error",
  "EOF",
  "'['",
  "'{'",
  "']'",
  "'}'",
  "':'",
  "','",
  "'true'",
  "'false'",
  "'null'",
  "string",
  "number"
};

/* Tokens within the JSON lexer.  */

struct token
{
  /* The kind of token.  */
  enum token_id id;

  /* The location of this token within the unicode
     character stream.  */
  int index;

  union
  {
    /* Value for TOK_ERROR and TOK_STRING.  */
    char *string;

    /* Value for TOK_NUMBER.  */
    double number;
  } u;
};

/* A class for lexing JSON.  */

class lexer
{
 public:
  lexer ();
  ~lexer ();
  bool add_utf8 (size_t length, const char *utf8_buf, char **err_out);

  const token *peek ();
  void consume ();

 private:
  bool get_char (unichar &out);
  void unget_char ();
  static void dump_token (FILE *outf, const token *tok);
  void lex_token (token *out);
  void lex_string (token *out);
  void lex_number (token *out, unichar first_char);
  bool rest_of_literal (const char *suffix);

 private:
  auto_vec<unichar> m_buffer;
  int m_next_char_idx;

  static const int MAX_TOKENS = 1;
  token m_next_tokens[MAX_TOKENS];
  int m_num_next_tokens;
};

/* A class for parsing JSON.  */

class parser
{
 public:
  parser (char **err_out);
  bool add_utf8 (size_t length, const char *utf8_buf, char **err_out);
  value *parse_value (int depth);
  object *parse_object (int depth);
  array *parse_array (int depth);

  bool seen_error_p () const { return *m_err_out; }
  void require_eof ();

 private:
  void require (enum token_id tok_id);
  void error_at (int, const char *, ...) ATTRIBUTE_PRINTF_3;

 private:
  lexer m_lexer;
  char **m_err_out;
};

} // anonymous namespace for parsing implementation

/* Parser implementation.  */

/* lexer's ctor.  */

lexer::lexer ()
: m_buffer (), m_next_char_idx (0), m_num_next_tokens (0)
{
}

/* lexer's dtor.  */

lexer::~lexer ()
{
  while (m_num_next_tokens > 0)
    consume ();
}

/* Peek the next token.  */

const token *
lexer::peek ()
{
  if (m_num_next_tokens == 0)
    {
      lex_token (&m_next_tokens[0]);
      m_num_next_tokens++;
    }
  return &m_next_tokens[0];
}

/* Consume the next token.  */

void
lexer::consume ()
{
  if (m_num_next_tokens == 0)
    peek ();

  gcc_assert (m_num_next_tokens > 0);
  gcc_assert (m_num_next_tokens <= MAX_TOKENS);

  if (0)
    {
      fprintf (stderr, "consuming token: ");
      dump_token (stderr, &m_next_tokens[0]);
      fprintf (stderr, "\n");
    }

  if (m_next_tokens[0].id == TOK_ERROR
      || m_next_tokens[0].id == TOK_STRING)
    free (m_next_tokens[0].u.string);

  m_num_next_tokens--;
  memmove (&m_next_tokens[0], &m_next_tokens[1],
	   sizeof (token) * m_num_next_tokens);
}

/* Add LENGTH bytes of UTF-8 encoded text from UTF8_BUF to this lexer's
   buffer.  */

bool
lexer::add_utf8 (size_t length, const char *utf8_buf, char **err_out)
{
  /* FIXME: adapted from charset.c:one_utf8_to_cppchar.  */
  static const uchar masks[6] = { 0x7F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
  static const uchar patns[6] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

  const uchar *inbuf = (const unsigned char *) (utf8_buf);
  const uchar **inbufp = &inbuf;
  size_t *inbytesleftp = &length;

  while (length > 0)
    {
      unichar c;
      const uchar *inbuf = *inbufp;
      size_t nbytes, i;

      c = *inbuf;
      if (c < 0x80)
	{
	  m_buffer.safe_push (c);
	  *inbytesleftp -= 1;
	  *inbufp += 1;
	  continue;
	}

      /* The number of leading 1-bits in the first byte indicates how many
	 bytes follow.  */
      for (nbytes = 2; nbytes < 7; nbytes++)
	if ((c & ~masks[nbytes-1]) == patns[nbytes-1])
	  goto found;
      *err_out = xstrdup ("ill-formed UTF-8 sequence");
      return false;
    found:

      if (*inbytesleftp < nbytes)
	{
	  *err_out = xstrdup ("ill-formed UTF-8 sequence");
	  return false;
	}

      c = (c & masks[nbytes-1]);
      inbuf++;
      for (i = 1; i < nbytes; i++)
	{
	  unichar n = *inbuf++;
	  if ((n & 0xC0) != 0x80)
	    {
	      *err_out = xstrdup ("ill-formed UTF-8 sequence");
	      return false;
	    }
	  c = ((c << 6) + (n & 0x3F));
	}

      /* Make sure the shortest possible encoding was used.  */
      if ((   c <=      0x7F && nbytes > 1)
	  || (c <=     0x7FF && nbytes > 2)
	  || (c <=    0xFFFF && nbytes > 3)
	  || (c <=  0x1FFFFF && nbytes > 4)
	  || (c <= 0x3FFFFFF && nbytes > 5))
	{
	  *err_out = xstrdup ("ill-formed UTF-8:"
			      " shortest possible encoding not used");
	  return false;
	}

      /* Make sure the character is valid.  */
      if (c > 0x7FFFFFFF || (c >= 0xD800 && c <= 0xDFFF))
	{
	  *err_out = xstrdup ("ill-formed UTF-8: invalid character");
	  return false;
	}

      m_buffer.safe_push (c);
      *inbufp = inbuf;
      *inbytesleftp -= nbytes;
    }
  return true;
}

/* Attempt to get the next unicode character from this lexer's buffer.
   If successful, write it to OUT and return true.
   Otherwise, return false.  */

bool
lexer::get_char (unichar &out)
{
  if (m_next_char_idx >= (int)m_buffer.length ())
    return false;

  out = m_buffer[m_next_char_idx++];
  return true;
}

/* FIXME.  */

void
lexer::unget_char ()
{
  --m_next_char_idx;
}

/* Print a textual representation of TOK to OUTF.
   This is intended for debugging the lexer and parser,
   rather than for user-facing output.  */

void
lexer::dump_token (FILE *outf, const token *tok)
{
  switch (tok->id)
    {
    case TOK_ERROR:
      fprintf (outf, "TOK_ERROR (\"%s\")", tok->u.string);
      break;

    case TOK_EOF:
      fprintf (outf, "TOK_EOF");
      break;

    case TOK_OPEN_SQUARE:
      fprintf (outf, "TOK_OPEN_SQUARE");
      break;

    case TOK_OPEN_CURLY:
      fprintf (outf, "TOK_OPEN_CURLY");
      break;

    case TOK_CLOSE_SQUARE:
      fprintf (outf, "TOK_CLOSE_SQUARE");
      break;

    case TOK_CLOSE_CURLY:
      fprintf (outf, "TOK_CLOSE_CURLY");
      break;

    case TOK_COLON:
      fprintf (outf, "TOK_COLON");
      break;

    case TOK_COMMA:
      fprintf (outf, "TOK_COMMA");
      break;

    case TOK_TRUE:
      fprintf (outf, "TOK_TRUE");
      break;

    case TOK_FALSE:
      fprintf (outf, "TOK_FALSE");
      break;

    case TOK_NULL:
      fprintf (outf, "TOK_NULL");
      break;

    case TOK_STRING:
      fprintf (outf, "TOK_STRING (\"%s\")", tok->u.string);
      break;

    case TOK_NUMBER:
      fprintf (outf, "TOK_NUMBER (%f)", tok->u.number);
      break;

    default:
      gcc_unreachable ();
      break;
    }
}

/* Attempt to lex the input buffer, writing the next token to OUT.
   On errors, TOK_ERROR (or TOK_EOF) is written to OUT.  */

void
lexer::lex_token (token *out)
{
  /* Skip to next non-whitespace char.  */
  unichar next_char;
  while (1)
    {
      out->index = m_next_char_idx;
      if (!get_char (next_char))
	{
	  out->id = TOK_EOF;
	  return;
	}
      if (next_char != ' '
	  && next_char != '\t'
	  && next_char != '\n'
	  && next_char != '\r')
	break;
    }

  switch (next_char)
    {
    case '[':
      out->id = TOK_OPEN_SQUARE;
      break;

    case '{':
      out->id = TOK_OPEN_CURLY;
      break;

    case ']':
      out->id = TOK_CLOSE_SQUARE;
      break;

    case '}':
      out->id = TOK_CLOSE_CURLY;
      break;

    case ':':
      out->id = TOK_COLON;
      break;

    case ',':
      out->id = TOK_COMMA;
      break;

    case '"':
      lex_string (out);
      break;

    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      lex_number (out, next_char);
      break;

    case 't':
      /* Handle literal "true".  */
      if (rest_of_literal ("rue"))
	{
	  out->id = TOK_TRUE;
	  break;
	}
      else
	goto err;

    case 'f':
      /* Handle literal "false".  */
      if (rest_of_literal ("alse"))
	{
	  out->id = TOK_FALSE;
	  break;
	}
      else
	goto err;

    case 'n':
      /* Handle literal "null".  */
      if (rest_of_literal ("ull"))
	{
	  out->id = TOK_NULL;
	  break;
	}
      else
	goto err;

    err:
    default:
      out->id = TOK_ERROR;
      out->u.string = xasprintf ("unexpected character: %c", next_char);
      break;
    }
}

/* Having consumed an open-quote character from the lexer's buffer, attempt
   to lex the rest of a JSON string, writing the result to OUT (or TOK_ERROR)
   if an error occurred.
   (ECMA-404 section 9; RFC 7159 section 7).  */

void
lexer::lex_string (token *out)
{
  auto_vec<unichar> content;
  bool still_going = true;
  while (still_going)
    {
      unichar uc;
      if (!get_char (uc))
	{
	  out->id = TOK_ERROR;
	  out->u.string = xstrdup ("EOF within string");
	  return;
	}
      switch (uc)
	{
	case '"':
	  still_going = false;
	  break;
	case '\\':
	  {
	    unichar next_char;
	    if (!get_char (next_char))
	      {
		out->id = TOK_ERROR;
		out->u.string = xstrdup ("EOF within string");;
		return;
	      }
	    switch (next_char)
	      {
	      case '"':
	      case '\\':
	      case '/':
		content.safe_push (next_char);
		break;

	      case 'b':
		content.safe_push ('\b');
		break;

	      case 'f':
		content.safe_push ('\f');
		break;

	      case 'n':
		content.safe_push ('\n');
		break;

	      case 'r':
		content.safe_push ('\r');
		break;

	      case 't':
		content.safe_push ('\t');
		break;

	      case 'u':
		{
		  unichar result = 0;
		  for (int i = 0; i < 4; i++)
		    {
		      unichar hexdigit;
		      if (!get_char (hexdigit))
			{
			  out->id = TOK_ERROR;
			  out->u.string = xstrdup ("EOF within string");
			  return;
			}
		      result <<= 4;
		      if (hexdigit >= '0' && hexdigit <= '9')
			result += hexdigit - '0';
		      else if (hexdigit >= 'a' && hexdigit <= 'f')
			result += (hexdigit - 'a') + 10;
		      else if (hexdigit >= 'A' && hexdigit <= 'F')
			result += (hexdigit - 'A') + 10;
		      else
			{
			  out->id = TOK_ERROR;
			  out->u.string = xstrdup ("bogus hex char");
			  return;
			}
		    }
		  content.safe_push (result);
		}
		break;

	      default:
		out->id = TOK_ERROR;
		out->u.string = xstrdup ("unrecognized escape char");
		return;
	      }
	  }
	  break;

	default:
	  /* Reject unescaped control characters U+0000 through U+001F
	     (ECMA-404 section 9 para 1; RFC 7159 section 7 para 1).  */
	  if (uc <= 0x1f)
	    {
		out->id = TOK_ERROR;
		out->u.string = xstrdup ("unescaped control char");
		return;
	    }

	  /* Otherwise, add regular unicode code point.  */
	  content.safe_push (uc);
	  break;
	}
    }

  out->id = TOK_STRING;

  auto_vec<char> utf8_buf;
  // FIXME: adapted from libcpp/charset.c:one_cppchar_to_utf8
  for (unsigned i = 0; i < content.length (); i++)
    {
      static const uchar masks[6] =  { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
      static const uchar limits[6] = { 0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
      size_t nbytes;
      uchar buf[6], *p = &buf[6];
      unichar c = content[i];

      nbytes = 1;
      if (c < 0x80)
	*--p = c;
      else
	{
	  do
	    {
	      *--p = ((c & 0x3F) | 0x80);
	      c >>= 6;
	      nbytes++;
	    }
	  while (c >= 0x3F || (c & limits[nbytes-1]));
	  *--p = (c | masks[nbytes-1]);
	}

      while (p < &buf[6])
	utf8_buf.safe_push (*p++);
    }

  out->u.string = XNEWVEC (char, utf8_buf.length () + 1);
  for (unsigned i = 0; i < utf8_buf.length (); i++)
    out->u.string[i] = utf8_buf[i];
  out->u.string[utf8_buf.length ()] = '\0';

  // FIXME: leaks?  have a json_context do the allocation
}

/* Having consumed FIRST_CHAR, an initial digit or '-' character from
   the lexer's buffer attempt to lex the rest of a JSON number, writing
   the result to OUT (or TOK_ERROR) if an error occurred.
   (ECMA-404 section 8; RFC 7159 section 6).  */

void
lexer::lex_number (token *out, unichar first_char)
{
  bool negate = false;
  double value = 0.0;
  if (first_char == '-')
    {
      negate = true;
      if (!get_char (first_char))
	{
	  out->id = TOK_ERROR;
	  out->u.string = xstrdup ("expected digit");
	  return;
	}
    }

  if (first_char == '0')
    value = 0.0;
  else if (!ISDIGIT (first_char))
    {
      out->id = TOK_ERROR;
      out->u.string = xstrdup ("expected digit");
      return;
    }
  else
    {
      /* Got a nonzero digit; expect zero or more digits.  */
      value = first_char - '0';
      while (1)
	{
	  unichar uc;
	  if (!get_char (uc))
	    break;
	  if (ISDIGIT (uc))
	    {
	      value *= 10;
	      value += uc -'0';
	      continue;
	    }
	  else
	    {
	      unget_char ();
	      break;
	    }
	}
    }

  /* Optional '.', followed by one or more decimals.  */
  unichar next_char;
  if (get_char (next_char))
    {
      if (next_char == '.')
	{
	  /* Parse decimal digits.  */
	  bool had_digit = false;
	  // FIXME: does this lose too much precision?
	  double digit_factor = 0.1;
	  while (get_char (next_char))
	    {
	      if (!ISDIGIT (next_char))
		{
		  unget_char ();
		  break;
		}
	      value += (next_char - '0') * digit_factor;
	      digit_factor *= 0.1;
	      had_digit = true;
	    }
	  if (!had_digit)
	    {
	      out->id = TOK_ERROR;
	      out->u.string = xstrdup ("expected digit");
	      return;
	    }
	}
      else
	unget_char ();
    }

  /* Parse 'e' and 'E'.  */
  unichar exponent_char;
  if (get_char (exponent_char))
    {
      if (exponent_char == 'e' || exponent_char == 'E')
	{
	  /* Optional +/-.  */
	  unichar sign_char;
	  int exponent = 0;
	  bool negate_exponent = false;
	  bool had_exponent_digit = false;
	  if (!get_char (sign_char))
	    {
	      out->id = TOK_ERROR;
	      out->u.string = xstrdup ("EOF within exponent");
	      return;
	    }
	  if (sign_char == '-')
	    negate_exponent = true;
	  else if (sign_char == '+')
	    ;
	  else if (ISDIGIT (sign_char))
	    {
	      exponent = sign_char - '0';
	      had_exponent_digit = true;
	    }
	  else
	    {
	      out->id = TOK_ERROR;
	      out->u.string
		= xstrdup ("expected '-','+' or digit within exponent");
	      return;
	    }

	  /* One or more digits (we might have seen the digit above,
	     though).  */
	  while (1)
	    {
	      unichar uc;
	      if (!get_char (uc))
		break;
	      if (ISDIGIT (uc))
		{
		  exponent *= 10;
		  exponent += uc -'0';
		  had_exponent_digit = true;
		  continue;
		}
	      else
		{
		  unget_char ();
		  break;
		}
	    }
	  if (!had_exponent_digit)
	    {
	      out->id = TOK_ERROR;
	      out->u.string = xstrdup ("expected digit within exponent");
	      return;
	    }
	  if (negate_exponent)
	    exponent = -exponent;
	  /* FIXME: better way to do this?  */
	  value = value * pow (10, exponent);
	}
      else
	unget_char ();
    }

  if (negate)
    value = -value;

  out->id = TOK_NUMBER;
  out->u.number = value;
}

/* Determine if the next characters to be lexed match SUFFIX.
   SUFFIX must be pure ASCII.
   If so, consume the characters and return true.
   Otherwise, return false.  */

bool
lexer::rest_of_literal (const char *suffix)
{
  int suffix_idx = 0;
  int buf_idx = m_next_char_idx;
  while (1)
    {
      if (suffix[suffix_idx] == '\0')
	{
	  m_next_char_idx += suffix_idx;
	  return true;
	}
      if (buf_idx >= (int)m_buffer.length ())
	return false;
      /* This assumes that suffix is ASCII.  */
      if (m_buffer[buf_idx] != (unichar)suffix[suffix_idx])
	return false;
      buf_idx++;
      suffix_idx++;
    }
}

/* parser's ctor.  */

parser::parser (char **err_out)
: m_lexer (), m_err_out (err_out)
{
  gcc_assert (err_out);
  gcc_assert (*err_out == NULL);
  *err_out = NULL;
}

/* Add LENGTH bytes of UTF-8 encoded text from UTF8_BUF to this parser's
   lexer's buffer.  */

bool
parser::add_utf8 (size_t length, const char *utf8_buf, char **err_out)
{
  return m_lexer.add_utf8 (length, utf8_buf, err_out);
}

/* Parse a JSON value (object, array, number, string, or literal).
   (ECMA-404 section 5; RFC 7159 section 3).  */

value *
parser::parse_value (int depth)
{
  const token *tok = m_lexer.peek ();

  /* Avoid stack overflow with deeply-nested inputs; RFC 7159 section 9
     states: "An implementation may set limits on the maximum depth
     of nesting.".

     Ideally we'd avoid this limit (e.g. by rewriting parse_value,
     parse_object, and parse_array into a single function with a vec of
     state).  */
  const int MAX_DEPTH = 100;
  if (depth >= MAX_DEPTH)
    {
      error_at (tok->index, "maximum nesting depth exceeded: %i", MAX_DEPTH);
      return NULL;
    }

  switch (tok->id)
    {
    case TOK_OPEN_CURLY:
      return parse_object (depth);

    case TOK_STRING:
      {
	string *result = new string (tok->u.string);
	m_lexer.consume ();
	return result;
      }

    case TOK_OPEN_SQUARE:
      return parse_array (depth);

    case TOK_NUMBER:
      {
	number *result = new number (tok->u.number);
	m_lexer.consume ();
	return result;
      }

    case TOK_TRUE:
      {
	literal *result = new literal (JSON_TRUE);
	m_lexer.consume ();
	return result;
      }

    case TOK_FALSE:
      {
	literal *result = new literal (JSON_FALSE);
	m_lexer.consume ();
	return result;
      }

    case TOK_NULL:
      {
	literal *result = new literal (JSON_NULL);
	m_lexer.consume ();
	return result;
      }

    default:
      error_at (tok->index, "unexpected token: %s", token_id_name[tok->id]);
      return NULL;
    }
}

/* Parse a JSON object.
   (ECMA-404 section 6; RFC 7159 section 4).  */

object *
parser::parse_object (int depth)
{
  require (TOK_OPEN_CURLY);

  object *result = new object ();

  const token *tok = m_lexer.peek ();
  if (tok->id == TOK_CLOSE_CURLY)
    {
      require (TOK_CLOSE_CURLY);
      return result;
    }
  if (tok->id != TOK_STRING)
    {
      error_at (tok->index, "expected string for object key");
      return result;
    }
  while (!seen_error_p ())
    {
      tok = m_lexer.peek ();
      if (tok->id != TOK_STRING)
	{
	  error_at (tok->index, "expected string for object key");
	  return result;
	}
      char *key = xstrdup (tok->u.string);
      m_lexer.consume ();

      require (TOK_COLON);

      value *v = parse_value (depth + 1);
      if (!v)
	{
	  free (key);
	  return result;
	}
      /* We don't enforce uniqueness for keys.  */
      result->set (key, v);
      free (key);

      tok = m_lexer.peek ();
      if (tok->id == TOK_COMMA)
	{
	  m_lexer.consume ();
	  continue;
	}
      else
	{
	  require (TOK_CLOSE_CURLY);
	  break;
	}
    }
  return result;
}

/* Parse a JSON array.
   (ECMA-404 section 7; RFC 7159 section 5).  */

array *
parser::parse_array (int depth)
{
  require (TOK_OPEN_SQUARE);

  array *result = new array ();

  const token *tok = m_lexer.peek ();
  if (tok->id == TOK_CLOSE_SQUARE)
    {
      m_lexer.consume ();
      return result;
    }

  while (!seen_error_p ())
    {
      value *v = parse_value (depth + 1);
      if (!v)
	return result;

      result->append (v);

      tok = m_lexer.peek ();
      if (tok->id == TOK_COMMA)
	{
	  m_lexer.consume ();
	  continue;
	}
      else
	{
	  require (TOK_CLOSE_SQUARE);
	  break;
	}
    }

  return result;
}

/* Require an EOF, or fail if there is surplus input.  */

void
parser::require_eof ()
{
  require (TOK_EOF);
}

/* Consume the next token, issuing an error if it is not of kind TOK_ID.  */

void
parser::require (enum token_id tok_id)
{
  const token *tok = m_lexer.peek ();
  if (tok->id != tok_id)
    {
      if (tok->id == TOK_ERROR)
	error_at (tok->index, "expected %s; got bad token: %s",
		  token_id_name[tok_id], tok->u.string);
      else
	error_at (tok->index, "expected %s; got %s", token_id_name[tok_id],
		  token_id_name[tok->id]);
    }
  m_lexer.consume ();
}

/* Issue a parsing error.  If this is the first error that has occurred on
   the parser, store it within the parser's m_err_out (the buffer will
   eventually need to be free by the caller of the parser).
   Otherwise the error is discarded.

   TODO: maybe provide a callback so that client code can print all errors?  */

void
parser::error_at (int index, const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  char *formatted = xvasprintf (fmt, ap);
  va_end (ap);

  char *msg_with_index = xasprintf ("error at index %i: %s",
				    index, formatted);
  free (formatted);

  if (0)
    fprintf (stderr, "%s\n", msg_with_index);
  if (*m_err_out == NULL)
    *m_err_out = msg_with_index;
  else
    free (msg_with_index);
}

/* Attempt to parse the UTF-8 encoded buffer at UTF8_BUF
   of the given LENGTH.
   If successful, return a non-NULL json::value *.
   if there was a problem, return NULL and write an error
   message to err_out, which must be freed by the caller.  */

value *
json::parse_utf8_string (size_t length, const char *utf8_buf,
			 char **err_out)
{
  gcc_assert (err_out);
  gcc_assert (*err_out == NULL);

  parser p (err_out);
  if (!p.add_utf8 (length, utf8_buf, err_out))
    return NULL;
  value *result = p.parse_value (0);
  if (!p.seen_error_p ())
    p.require_eof ();
  if (p.seen_error_p ())
    {
      gcc_assert (*err_out);
      delete result;
      return NULL;
    }
  return result;
}

/* Attempt to parse the nil-terminated UTF-8 encoded buffer at
   UTF8_BUF.
   If successful, return a non-NULL json::value *.
   if there was a problem, return NULL and write an error
   message to err_out, which must be freed by the caller.  */

value *
json::parse_utf8_string (const char *utf8, char **err_out)
{
  return parse_utf8_string (strlen (utf8), utf8, err_out);
}


#if CHECKING_P

namespace selftest {

/* Selftests.  */

/* Verify that JV->to_str () equals EXPECTED_JSON.  */

static void
assert_to_str_eq (const char *expected_json, json::value *jv)
{
  char *json = jv->to_str ();
  ASSERT_STREQ (expected_json, json);
  free (json);
}

/* FIXME.  */

static void
test_parse_string ()
{
  char *err = NULL;
  json::value *jv = parse_utf8_string ("\"foo\"", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_STRING, jv->get_kind ());
  ASSERT_STREQ ("foo", ((json::string *)jv)->get_string ());
  assert_to_str_eq ("\"foo\"", jv);

  json::value *clone = jv->clone ();
  ASSERT_EQ (JSON_STRING, clone->get_kind ());
  ASSERT_STREQ ("foo", ((json::string *)clone)->get_string ());
  assert_to_str_eq ("\"foo\"", clone);
  delete clone;
  delete jv;

  const char *contains_quotes = "\"before \\\"quoted\\\" after\"";
  jv = parse_utf8_string (contains_quotes, &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_STRING, jv->get_kind ());
  ASSERT_STREQ ("before \"quoted\" after", ((json::string *)jv)->get_string ());
  assert_to_str_eq (contains_quotes, jv);
  delete jv;

  /* Test of non-ASCII input.  This string is the Japanese word "mojibake",
     written as C octal-escaped UTF-8.  */
  const char *mojibake = (/* Opening quote.  */
			  "\""
			  /* U+6587 CJK UNIFIED IDEOGRAPH-6587
			     UTF-8: 0xE6 0x96 0x87
			     C octal escaped UTF-8: \346\226\207.  */
			  "\346\226\207"
			  /* U+5B57 CJK UNIFIED IDEOGRAPH-5B57
			     UTF-8: 0xE5 0xAD 0x97
			     C octal escaped UTF-8: \345\255\227.  */
			  "\345\255\227"
			 /* U+5316 CJK UNIFIED IDEOGRAPH-5316
			      UTF-8: 0xE5 0x8C 0x96
			      C octal escaped UTF-8: \345\214\226.  */
			  "\345\214\226"
			 /* U+3051 HIRAGANA LETTER KE
			      UTF-8: 0xE3 0x81 0x91
			      C octal escaped UTF-8: \343\201\221.  */
			  "\343\201\221"
			  /* Closing quote.  */
			  "\"");
  jv = parse_utf8_string (mojibake, &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_STRING, jv->get_kind ());
  /* Result of get_string should be UTF-8 encoded, without quotes.  */
  ASSERT_STREQ ("\346\226\207" "\345\255\227" "\345\214\226" "\343\201\221",
		((json::string *)jv)->get_string ());
  /* Result of dump should be UTF-8 encoded, with quotes.  */
  assert_to_str_eq (mojibake, jv);
  delete jv;

  /* Test of \u-escaped unicode.  This is "mojibake" again, as above.  */
  const char *escaped_unicode = "\"\\u6587\\u5b57\\u5316\\u3051\"";
  jv = parse_utf8_string (escaped_unicode, &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_STRING, jv->get_kind ());
  /* Result of get_string should be UTF-8 encoded, without quotes.  */
  ASSERT_STREQ ("\346\226\207" "\345\255\227" "\345\214\226" "\343\201\221",
		((json::string *)jv)->get_string ());
  /* Result of dump should be UTF-8 encoded, with quotes.  */
  assert_to_str_eq (mojibake, jv);
  delete jv;
}

/* FIXME.  */

static void
test_parse_number ()
{
  json::value *jv, *clone;

  char *err = NULL;
  jv = parse_utf8_string ("42", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (42.0, ((json::number *)jv)->get ());
  assert_to_str_eq ("42", jv);
  clone = jv->clone ();
  ASSERT_EQ (JSON_NUMBER, clone->get_kind ());
  delete clone;
  delete jv;

  /* Negative number.  */
  jv = parse_utf8_string ("-17", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (-17.0, ((json::number *)jv)->get ());
  assert_to_str_eq ("-17", jv);
  delete jv;

  /* Decimal.  */
  jv = parse_utf8_string ("3.141", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (3.141, ((json::number *)jv)->get ());
  assert_to_str_eq ("3.141", jv);
  delete jv;

  /* Exponents.  */
  jv = parse_utf8_string ("3.141e+0", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (3.141, ((json::number *)jv)->get ());
  assert_to_str_eq ("3.141", jv);
  delete jv;

  jv = parse_utf8_string ("42e2", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (4200, ((json::number *)jv)->get ());
  assert_to_str_eq ("4200", jv);
  delete jv;

  jv = parse_utf8_string ("42e-1", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_NUMBER, jv->get_kind ());
  ASSERT_EQ (4.2, ((json::number *)jv)->get ());
  assert_to_str_eq ("4.2", jv);
  delete jv;

}

/* FIXME.  */

static void
test_parse_array ()
{
  json::value *jv, *clone;

  char *err = NULL;
  jv = parse_utf8_string ("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_EQ (JSON_ARRAY, jv->get_kind ());
  json::array *arr = static_cast <json::array *> (jv);
  ASSERT_EQ (10, arr->get_length ());
  for (int i = 0; i < 10; i++)
    {
      json::value *element = arr->get (i);
      ASSERT_EQ (JSON_NUMBER, element->get_kind ());
      ASSERT_EQ (i, ((json::number *)element)->get ());
    }
  assert_to_str_eq ("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]", jv);

  clone = jv->clone ();
  ASSERT_EQ (JSON_ARRAY, clone->get_kind ());
  arr = static_cast <json::array *> (clone);
  ASSERT_EQ (10, arr->get_length ());
  for (int i = 0; i < 10; i++)
    {
      json::value *element = arr->get (i);
      ASSERT_EQ (JSON_NUMBER, element->get_kind ());
      ASSERT_EQ (i, ((json::number *)element)->get ());
    }
  assert_to_str_eq ("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]", clone);
  delete clone;

  delete jv;
}

/* FIXME.  */

static void
test_parse_object ()
{
  char *err = NULL;
  json::value *jv
    = parse_utf8_string ("{\"foo\": \"bar\", \"baz\": [42, null]}", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  ASSERT_EQ (JSON_OBJECT, jv->get_kind ());
  json::object *jo = static_cast <json::object *> (jv);

  json::value *foo_value = jo->get ("foo");
  ASSERT_TRUE (foo_value != NULL);
  ASSERT_EQ (JSON_STRING, foo_value->get_kind ());
  ASSERT_STREQ ("bar", ((json::string *)foo_value)->get_string ());

  json::value *baz_value = jo->get ("baz");
  ASSERT_TRUE (baz_value != NULL);
  ASSERT_EQ (JSON_ARRAY, baz_value->get_kind ());

  json::array *baz_array = (json::array *)baz_value;
  ASSERT_EQ (2, baz_array->get_length ());
  ASSERT_EQ (42, baz_array->get (0)->as_number ()->get ());
  ASSERT_EQ (JSON_NULL, baz_array->get (1)->get_kind ());

  // TODO: error-handling
  // TODO: partial document

  /* We can't use assert_to_str_eq since ordering is not guaranteed.  */

  json::value *clone = jv->clone ();
  ASSERT_EQ (JSON_OBJECT, clone->get_kind ());
  ASSERT_EQ (JSON_STRING, clone->as_object ()->get ("foo")->get_kind ());
  delete clone;

  delete jv;
}

/* Verify that the literals "true", "false" and "null" are parsed,
   dumped, and are clonable.  */

static void
test_parse_literals ()
{
  json::value *jv, *clone;
  char *err = NULL;
  jv = parse_utf8_string ("true", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  ASSERT_EQ (JSON_TRUE, jv->get_kind ());
  assert_to_str_eq ("true", jv);
  clone = jv->clone ();
  ASSERT_EQ (JSON_TRUE, clone->get_kind ());
  delete clone;
  delete jv;

  jv = parse_utf8_string ("false", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  ASSERT_EQ (JSON_FALSE, jv->get_kind ());
  assert_to_str_eq ("false", jv);
  clone = jv->clone ();
  ASSERT_EQ (JSON_FALSE, clone->get_kind ());
  delete clone;
  delete jv;

  jv = parse_utf8_string ("null", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  ASSERT_EQ (JSON_NULL, jv->get_kind ());
  assert_to_str_eq ("null", jv);
  clone = jv->clone ();
  ASSERT_EQ (JSON_NULL, clone->get_kind ());
  delete clone;
  delete jv;
}

/* FIXME.  */

static void
test_parse_jsonrpc ()
{
  char *err = NULL;
  const char *request
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"subtract\","
       " \"params\": [42, 23], \"id\": 1}");
  json::value *jv = parse_utf8_string (request, &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  delete jv;
}

/* FIXME.  */

static void
test_parse_empty_object ()
{
  char *err = NULL;
  json::value *jv = parse_utf8_string ("{}", &err);
  ASSERT_EQ (NULL, err);
  ASSERT_TRUE (jv != NULL);
  ASSERT_EQ (JSON_OBJECT, jv->get_kind ());
  assert_to_str_eq ("{}", jv);
  delete jv;
}

/* FIXME.  */

static void
test_error_empty_string ()
{
  char *err = NULL;
  json::value *jv = parse_utf8_string ("", &err);
  ASSERT_STREQ ("error at index 0: unexpected token: EOF", err);
  ASSERT_TRUE (jv == NULL);
  free (err);
}

/* FIXME.  */

static void
test_error_missing_comma ()
{
  char *err = NULL;
  /*                  01234567.  */
  const char *json = "[0, 1 2]";
  json::value *jv = parse_utf8_string (json, &err);
  ASSERT_STREQ ("error at index 6: expected ']'; got number",
		err);
  // FIXME: unittest the lexer?
  ASSERT_TRUE (jv == NULL);
  free (err);
}

/* FIXME.  */

static int
is_json_file (const struct dirent *entry)
{
  // FIXME: should be a regular file, ending in ".json"
  if (!strstr (entry->d_name, ".json"))
    return 0;
  return 1;
}

/* FIXME.  */

static void
test_file (const char *path, const char *filename, bool verbose)
{
  if (verbose)
    fprintf (stderr, "%s\n", filename);

  /* This selftest routine can't yet cope with embedded NIL characters,
     since it calls selftest::read_file and then parses up to the first
     embedded NIL.   Skip such tests for now.  */
  if (0 == strcmp (filename, "n_multidigit_number_then_00.json"))
    return;

  const char *full_path = ACONCAT ((path, "/", filename, NULL)); // FIXME

  char *content = read_file (SELFTEST_LOCATION, full_path);
  if (verbose)
    fprintf (stderr, "  content: %s\n", content);

  char *err = NULL;
  json::value *jv = parse_utf8_string (content, &err);

  if (verbose && err)
    fprintf (stderr, "  err: %s\n", err);

  if (filename[0] == 'y')
    {
      /* We expect filenames starting with "y_" to be successfully parsed.  */
      ASSERT_NE (NULL, jv);
      ASSERT_EQ (NULL, err);

      /* Verify that we can dump the value.  */
      char *json = jv->to_str ();
      free (json);

      delete jv;
    }
  else if (filename[0] == 'n')
    {
      /* We expect filenames starting with "n_" to be rejected, with an
	 error message.  */
      ASSERT_EQ (NULL, jv);
      ASSERT_NE (NULL, err);
    }
  else if (filename[0] == 'i')
    {
      /* Filenames that start with "i_" have implementation-defined
	 results; we may or may not cope with them.  */
      delete jv;
    }
  free (err);

  free (content);
}

/* FIXME.  */

static void
test_seriot_testsuite (bool verbose)
{
  struct dirent **json_files;
  const char *path = "../../JSONTestSuite/test_parsing"; // FIXME
  int num_entries = scandir (path, &json_files, is_json_file, alphasort);
  if (num_entries < 0)
    {
      perror ("scandir");
      fail (SELFTEST_LOCATION, "scandir failed");
    }

  for (int i = 0; i < num_entries; i++)
    {
      test_file (path, json_files[i]->d_name, verbose);
      free (json_files[i]);
    }
  free (json_files);
}

/* Run all of the selftests within this file.  */

void
json_cc_tests ()
{
  test_parse_string ();
  test_parse_number ();
  test_parse_array ();
  test_parse_object ();
  test_parse_literals ();
  test_parse_jsonrpc ();
  test_parse_empty_object ();
  test_error_empty_string ();
  test_error_missing_comma ();

  /* FIXME: tests for roundtripping (noting that we don't preserve
     object key ordering).  */

  /* FIXME: cloning.  */
  test_seriot_testsuite (false);
}

} // namespace selftest

#endif /* #if CHECKING_P */
