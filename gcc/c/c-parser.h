/* Declarations for the parser for C and Objective-C.
   Copyright (C) 1987-2016 Free Software Foundation, Inc.

   Parser actions based on the old Bison parser; structure somewhat
   influenced by and fragments based on the C++ parser.

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

#ifndef GCC_C_PARSER_H
#define GCC_C_PARSER_H

/* The C lexer intermediates between the lexer in cpplib and c-lex.c
   and the C parser.  Unlike the C++ lexer, the parser structure
   stores the lexer information instead of using a separate structure.
   Identifiers are separated into ordinary identifiers, type names,
   keywords and some other Objective-C types of identifiers, and some
   look-ahead is maintained.

   ??? It might be a good idea to lex the whole file up front (as for
   C++).  It would then be possible to share more of the C and C++
   lexer code, if desired.  */

/* More information about the type of a CPP_NAME token.  */
enum c_id_kind {
  /* An ordinary identifier.  */
  C_ID_ID,
  /* An identifier declared as a typedef name.  */
  C_ID_TYPENAME,
  /* An identifier declared as an Objective-C class name.  */
  C_ID_CLASSNAME,
  /* An address space identifier.  */
  C_ID_ADDRSPACE,
  /* Not an identifier.  */
  C_ID_NONE
};

/* A single C token after string literal concatenation and conversion
   of preprocessing tokens to tokens.  */
typedef struct GTY (()) c_token {
  /* The kind of token.  */
  ENUM_BITFIELD (cpp_ttype) type : 8;
  /* If this token is a CPP_NAME, this value indicates whether also
     declared as some kind of type.  Otherwise, it is C_ID_NONE.  */
  ENUM_BITFIELD (c_id_kind) id_kind : 8;
  /* If this token is a keyword, this value indicates which keyword.
     Otherwise, this value is RID_MAX.  */
  ENUM_BITFIELD (rid) keyword : 8;
  /* If this token is a CPP_PRAGMA, this indicates the pragma that
     was seen.  Otherwise it is PRAGMA_NONE.  */
  ENUM_BITFIELD (pragma_kind) pragma_kind : 8;
  /* The location at which this token was found.  */
  location_t location;
  /* The value associated with this token, if any.  */
  tree value;
  /* Token flags.  */
  unsigned char flags;

  source_range get_range () const
  {
    return get_range_from_loc (line_table, location);
  }

  location_t get_finish () const
  {
    return get_range ().m_finish;
  }
} c_token_;

/* A parser structure recording information about the state and
   context of parsing.  Includes lexer information with up to two
   tokens of look-ahead; more are not needed for C.  */
struct GTY(()) c_parser {
  /* The look-ahead tokens.  */
  struct c_token * GTY((skip)) tokens;
  /* Buffer for look-ahead tokens.  */
  struct c_token GTY(()) tokens_buf[4];
  /* How many look-ahead tokens are available (0 - 4, or
     more if parsing from pre-lexed tokens).  */
  unsigned int tokens_avail;
  /* True if a syntax error is being recovered from; false otherwise.
     c_parser_error sets this flag.  It should clear this flag when
     enough tokens have been consumed to recover from the error.  */
  BOOL_BITFIELD error : 1;
  /* True if we're processing a pragma, and shouldn't automatically
     consume CPP_PRAGMA_EOL.  */
  BOOL_BITFIELD in_pragma : 1;
  /* True if we're parsing the outermost block of an if statement.  */
  BOOL_BITFIELD in_if_block : 1;
  /* True if we want to lex an untranslated string.  */
  BOOL_BITFIELD lex_untranslated_string : 1;

  /* Objective-C specific parser/lexer information.  */

  /* True if we are in a context where the Objective-C "PQ" keywords
     are considered keywords.  */
  BOOL_BITFIELD objc_pq_context : 1;
  /* True if we are parsing a (potential) Objective-C foreach
     statement.  This is set to true after we parsed 'for (' and while
     we wait for 'in' or ';' to decide if it's a standard C for loop or an
     Objective-C foreach loop.  */
  BOOL_BITFIELD objc_could_be_foreach_context : 1;
  /* The following flag is needed to contextualize Objective-C lexical
     analysis.  In some cases (e.g., 'int NSObject;'), it is
     undesirable to bind an identifier to an Objective-C class, even
     if a class with that name exists.  */
  BOOL_BITFIELD objc_need_raw_identifier : 1;
  /* Nonzero if we're processing a __transaction statement.  The value
     is 1 | TM_STMT_ATTR_*.  */
  unsigned int in_transaction : 4;
  /* True if we are in a context where the Objective-C "Property attribute"
     keywords are valid.  */
  BOOL_BITFIELD objc_property_attr_context : 1;

  /* Cilk Plus specific parser/lexer information.  */

  /* Buffer to hold all the tokens from parsing the vector attribute for the
     SIMD-enabled functions (formerly known as elemental functions).  */
  vec <c_token_, va_gc> *cilk_simd_fn_tokens;
};

/* Possibly kinds of declarator to parse.  */
enum c_dtr_syn {
  /* A normal declarator with an identifier.  */
  C_DTR_NORMAL,
  /* An abstract declarator (maybe empty).  */
  C_DTR_ABSTRACT,
  /* A parameter declarator: may be either, but after a type name does
     not redeclare a typedef name as an identifier if it can
     alternatively be interpreted as a typedef name; see DR#009,
     applied in C90 TC1, omitted from C99 and reapplied in C99 TC2
     following DR#249.  For example, given a typedef T, "int T" and
     "int *T" are valid parameter declarations redeclaring T, while
     "int (T)" and "int * (T)" and "int (T[])" and "int (T (int))" are
     abstract declarators rather than involving redundant parentheses;
     the same applies with attributes inside the parentheses before
     "T".  */
  C_DTR_PARM
};

/* The binary operation precedence levels, where 0 is a dummy lowest level
   used for the bottom of the stack.  */
enum c_parser_prec {
  PREC_NONE,
  PREC_LOGOR,
  PREC_LOGAND,
  PREC_BITOR,
  PREC_BITXOR,
  PREC_BITAND,
  PREC_EQ,
  PREC_REL,
  PREC_SHIFT,
  PREC_ADD,
  PREC_MULT,
  NUM_PRECS
};

enum c_lookahead_kind {
  /* Always treat unknown identifiers as typenames.  */
  cla_prefer_type,

  /* Could be parsing a nonabstract declarator.  Only treat an identifier
     as a typename if followed by another identifier or a star.  */
  cla_nonabstract_decl,

  /* Never treat identifiers as typenames.  */
  cla_prefer_id
};


extern c_token * c_parser_peek_token (c_parser *parser);
extern bool c_parser_require (c_parser *parser, enum cpp_ttype type,
			      const char *msgid);
extern void c_parser_error (c_parser *parser, const char *gmsgid);
extern void c_parser_consume_token (c_parser *parser);
extern void c_parser_skip_until_found (c_parser *parser, enum cpp_ttype type,
				       const char *msgid);
extern bool c_parser_next_token_starts_declspecs (c_parser *parser);
extern c_token * c_parser_peek_2nd_token (c_parser *parser);
bool c_parser_next_tokens_start_declaration (c_parser *parser);
bool c_token_starts_typename (c_token *token);

/* Return true if the next token from PARSER has the indicated
   TYPE.  */

static inline bool
c_parser_next_token_is (c_parser *parser, enum cpp_ttype type)
{
  return c_parser_peek_token (parser)->type == type;
}

/* Return true if the next token from PARSER does not have the
   indicated TYPE.  */

static inline bool
c_parser_next_token_is_not (c_parser *parser, enum cpp_ttype type)
{
  return !c_parser_next_token_is (parser, type);
}

/* Return true if the next token from PARSER is the indicated
   KEYWORD.  */

static inline bool
c_parser_next_token_is_keyword (c_parser *parser, enum rid keyword)
{
  return c_parser_peek_token (parser)->keyword == keyword;
}

extern struct c_declarator *
c_parser_declarator (c_parser *parser, bool type_seen_p, c_dtr_syn kind,
		     bool *seen_id);
extern void c_parser_declspecs (c_parser *, struct c_declspecs *, bool, bool,
				bool, bool, bool, enum c_lookahead_kind);
extern struct c_expr c_parser_cast_expression (c_parser *, struct c_expr *);

#endif
