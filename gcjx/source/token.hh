// A token.

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

#ifndef GCJX_SOURCE_TOKEN_HH
#define GCJX_SOURCE_TOKEN_HH

enum token_value
{
  TOKEN_ABSTRACT,
  TOKEN_ASSERT,
  TOKEN_ASSIGN,
  TOKEN_AT,
  TOKEN_BITWISE_AND,
  TOKEN_BITWISE_AND_ASSIGN,
  TOKEN_BITWISE_NOT,
  TOKEN_BITWISE_OR,
  TOKEN_BITWISE_OR_ASSIGN,
  TOKEN_BITWISE_XOR,
  TOKEN_BITWISE_XOR_ASSIGN,
  TOKEN_BOOLEAN,
  TOKEN_BREAK,
  TOKEN_BYTE,
  TOKEN_CASE,
  TOKEN_CATCH,
  TOKEN_CHAR,
  TOKEN_CHAR_LIT,
  TOKEN_CLASS,
  TOKEN_CLOSE_BRACE,
  TOKEN_CLOSE_BRACKET,
  TOKEN_CLOSE_PAREN,
  TOKEN_COLON,
  TOKEN_COMMA,
  TOKEN_CONST,
  TOKEN_CONTINUE,
  TOKEN_DECREMENT,
  TOKEN_DEFAULT,
  TOKEN_DIVIDE,
  TOKEN_DIVIDE_ASSIGN,
  TOKEN_DO,
  TOKEN_DOT,
  TOKEN_DOTDOTDOT,
  TOKEN_DOUBLE,
  TOKEN_DOUBLE_LIT,
  TOKEN_ELSE,
  TOKEN_ENUM,
  TOKEN_EQUALS,
  TOKEN_EXTENDS,
  TOKEN_FALSE,
  TOKEN_FINAL,
  TOKEN_FINALLY,
  TOKEN_FLOAT,
  TOKEN_FLOAT_LIT,
  TOKEN_FOR,
  TOKEN_GOTO,
  TOKEN_GREATER_EQUALS,
  TOKEN_GREATER_THAN,
  TOKEN_IDENTIFIER,
  TOKEN_IF,
  TOKEN_IMPLEMENTS,
  TOKEN_IMPORT,
  TOKEN_INCREMENT,
  TOKEN_INSTANCEOF,
  TOKEN_INT,
  TOKEN_INTERFACE,
  TOKEN_DECIMAL_INT_LIT,
  TOKEN_INT_LIT,
  TOKEN_JAVADOC,
  TOKEN_LEFT_SHIFT,
  TOKEN_LEFT_SHIFT_ASSIGN,
  TOKEN_LESS_EQUALS,
  TOKEN_LESS_THAN,
  TOKEN_LOGICAL_AND,
  TOKEN_LOGICAL_NOT,
  TOKEN_LOGICAL_OR,
  TOKEN_LONG,
  TOKEN_DECIMAL_LONG_LIT,
  TOKEN_LONG_LIT,
  TOKEN_MINUS,
  TOKEN_MINUS_ASSIGN,
  TOKEN_MOD,
  TOKEN_MOD_ASSIGN,
  TOKEN_MULTIPLY,
  TOKEN_MULTIPLY_ASSIGN,
  TOKEN_NATIVE,
  TOKEN_NEW,
  TOKEN_NOT_EQUALS,
  TOKEN_NULL,
  TOKEN_OPEN_BRACE,
  TOKEN_OPEN_BRACKET,
  TOKEN_OPEN_PAREN,
  TOKEN_PACKAGE,
  TOKEN_PLUS,
  TOKEN_PLUS_ASSIGN,
  TOKEN_PRIVATE,
  TOKEN_PROTECTED,
  TOKEN_PUBLIC,
  TOKEN_QUERY,
  TOKEN_RETURN,
  TOKEN_RIGHT_SHIFT,
  TOKEN_RIGHT_SHIFT_ASSIGN,
  TOKEN_SEMI,
  TOKEN_SHORT,
  TOKEN_STATIC,
  TOKEN_STRICT,
  TOKEN_STRING_LIT,
  TOKEN_SUPER,
  TOKEN_SWITCH,
  TOKEN_SYNCHRONIZED,
  TOKEN_THIS,
  TOKEN_THROW,
  TOKEN_THROWS,
  TOKEN_TRANSIENT,
  TOKEN_TRUE,
  TOKEN_TRY,
  TOKEN_UNSIGNED_RIGHT_SHIFT,
  TOKEN_UNSIGNED_RIGHT_SHIFT_ASSIGN,
  TOKEN_VOID,
  TOKEN_VOLATILE,
  TOKEN_WHILE,

  TOKEN_REPEAT,

  TOKEN_INVALID,
  TOKEN_EOF
};

class token
{
  // The type of token.
  token_value type;

  // The location of the token.
  location where;

  // The value of the token.  Ordinarily this is NULL.
  ref_element value;

public:

  token (token_value t = TOKEN_INVALID, location l = LOCATION_UNKNOWN,
	 model_element *v = NULL)
    : type (t),
      where (l),
      value (v)
  {
  }

  // You can construct with any owner<> that wraps an element.
  template<typename T>
  token (token_value t, const owner<T> &v)
    : type (t),
      where (v->get_location ()),
      value (v)
  {
  }

  token (const token &other)
    : type (other.type),
      where (other.where),
      value (other.value)
  {
  }

  token &operator= (const token &other)
  {
    type = other.type;
    where = other.where;
    value = other.value;
    return *this;
  }

  ~token ()
  {
  }

  token_value get_type () const
  {
    return type;
  }

  operator location () const
  {
    return where;
  }

  void set_location (location w)
  {
    where = w;
  }

  // This only checks the types, so it could be a bit confusing.
  bool operator== (const token &other) const
  {
    return type == other.type;
  }

  // This only checks the types, so it could be a bit confusing.
  bool operator!= (const token &other) const
  {
    return type != other.type;
  }

  bool operator== (token_value v) const
  {
    return type == v;
  }

  bool operator!= (token_value v) const
  {
    return type != v;
  }

  // Let the user try to extract what they need.
  template<typename T>
  operator owner<T> () const
  {
    return owner<T> (value);
  }
};

#endif // GCJX_SOURCE_TOKEN_HH
