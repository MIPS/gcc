// Represent a binary operator.

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

#ifndef GCJX_MODEL_BINARY_HH
#define GCJX_MODEL_BINARY_HH

/// This is the base class for binary expressions.
class model_binary_base : public model_expression
{
protected:

  // Left hand side.
  ref_expression lhs;

  // Right hand side.
  ref_expression rhs;

  bool compute_constant_p ()
  {
    return lhs->constant_p () && rhs->constant_p ();
  }

  model_binary_base (const location &w)
    : model_expression (w)
  {
  }

public:

  void set_lhs (const ref_expression &n)
  {
    lhs = n;
  }

  ref_expression get_lhs () const
  {
    return lhs;
  }

  void set_rhs (const ref_expression &n)
  {
    rhs = n;
  }

  ref_expression get_rhs () const
  {
    return rhs;
  }

  void resolve (resolution_scope *scope)
  {
    lhs->resolve (scope);
    rhs->resolve (scope);
    // The subclass must call set_type().
  }
};

// This enum is used solely for mapping a binary operator to its name.
typedef enum
  {
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_LEFT_SHIFT,
    OP_RIGHT_SHIFT,
    OP_UNSIGNED_RIGHT_SHIFT,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS_THAN,
    OP_GREATER_THAN,
    OP_LESS_EQUAL,
    OP_GREATER_EQUAL
  }
  operator_name;

// Return the name of an operator.
extern const char *get_operator_name (operator_name);

template<binary_function OP, operator_name NAME>
class model_arith_binary : public model_binary_base
{
protected:

  bool compute_constant_p ();

public:

  model_arith_binary (const location &w)
    : model_binary_base (w)
  {
  }

  void resolve (resolution_scope *);

  jvalue value ();

  void visit (visitor *);

  static const char *get_name ()
  {
    return get_operator_name (NAME);
  }
};

class model_plus : public model_binary_base
{
public:

  model_plus (const location &w)
    : model_binary_base (w)
  {
  }

  void resolve (resolution_scope *);

  std::string string_value ();

  jvalue value ();

  void visit (visitor *);

  static const char *get_name ()
  {
    return get_operator_name (OP_PLUS);
  }
};

typedef model_arith_binary<&model_primitive_base::subtract, OP_MINUS>
  model_minus;
typedef model_arith_binary<&model_primitive_base::multiply, OP_MULTIPLY>
  model_mult;
typedef model_arith_binary<&model_primitive_base::divide, OP_DIVIDE>
  model_div;
typedef model_arith_binary<&model_primitive_base::mod, OP_MOD>
  model_mod;

template<binary_function OP, operator_name NAME>
class model_bitwise_binary : public model_binary_base
{
public:

  model_bitwise_binary (const location &w)
    : model_binary_base (w)
  {
  }

  void resolve (resolution_scope *);

  jvalue value ();

  void visit (visitor *);

  static const char *get_name ()
  {
    return get_operator_name (NAME);
  }
};

typedef model_bitwise_binary<&model_primitive_base::band, OP_AND> model_and;
typedef model_bitwise_binary<&model_primitive_base::bor, OP_OR> model_or;
typedef model_bitwise_binary<&model_primitive_base::bxor, OP_XOR> model_xor;

template<binary_function OP, operator_name NAME>
class model_arith_shift : public model_binary_base
{
public:

  model_arith_shift (const location &w)
    : model_binary_base (w)
  {
  }

  void resolve (resolution_scope *);

  jvalue value ();

  void visit (visitor *);

  static const char *get_name ()
  {
    return get_operator_name (NAME);
  }
};

typedef model_arith_shift<&model_primitive_base::left_shift, OP_LEFT_SHIFT>
  model_left_shift;
typedef model_arith_shift<&model_primitive_base::right_shift, OP_RIGHT_SHIFT>
  model_right_shift;
typedef model_arith_shift<&model_primitive_base::unsigned_right_shift,
			  OP_UNSIGNED_RIGHT_SHIFT>
  model_unsigned_right_shift;


template<comparator OP, operator_name NAME>
class model_equality_comparison : public model_binary_base
{
protected:

  // The promoted type.  For comparison operators this different from
  // the result type, which is always boolean.
  model_type *promoted_type;

public:

  model_equality_comparison (const location &w)
    : model_binary_base (w),
      promoted_type (NULL)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  jvalue value ();

  static const char *get_name ()
  {
    return get_operator_name (NAME);
  }
};

typedef model_equality_comparison<&model_primitive_base::equal, OP_EQUAL>
  model_equal;
typedef model_equality_comparison<&model_primitive_base::not_equal,
				  OP_NOT_EQUAL>
  model_notequal;

template<comparator OP, operator_name NAME>
class model_numeric_comparison : public model_binary_base
{
protected:

  // The promoted type.  For comparison operators this different from
  // the result type, which is always boolean.
  model_type *promoted_type;

public:

  model_numeric_comparison (const location &w)
    : model_binary_base (w),
      promoted_type (NULL)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  jvalue value ();

  static const char *get_name ()
  {
    return get_operator_name (NAME);
  }
};

typedef model_numeric_comparison<&model_primitive_base::less_than,
				 OP_LESS_THAN>
  model_lessthan;
typedef model_numeric_comparison<&model_primitive_base::greater_than,
				 OP_GREATER_THAN>
  model_greaterthan;
typedef model_numeric_comparison<&model_primitive_base::less_than_equal,
				 OP_LESS_EQUAL>
  model_lessthanequal;
typedef model_numeric_comparison<&model_primitive_base::greater_than_equal,
				 OP_GREATER_EQUAL>
  model_greaterthanequal;


class model_logical_binary : public model_binary_base
{
protected:
  // Overloaded in subclasses to return name of operator.
  virtual const char *get_name () = 0;

  model_logical_binary (const location &w)
    : model_binary_base (w)
  {
  }

public:

  void resolve (resolution_scope *);
};

class model_lor : public model_logical_binary
{
protected:

  const char *get_name ()
  {
    return "||";
  }

public:

  model_lor (const location &w)
    : model_logical_binary (w)
  {
  }

  jvalue value ()
  {
    return (jboolean) lhs->value () || (jboolean) rhs->value ();
  }

  void visit (visitor *);
};

class model_land : public model_logical_binary
{
protected:

  const char *get_name ()
  {
    return "&&";
  }

public:

  model_land (const location &w)
    : model_logical_binary (w)
  {
  }

  jvalue value ()
  {
    return (jboolean) lhs->value () && (jboolean) rhs->value ();
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_BINARY_HH
