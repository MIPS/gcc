// Typedefs for the front end.

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

#ifndef GCJX_TYPEDEFS_HH
#define GCJX_TYPEDEFS_HH

#include <gcjx-config.h>

// Must be included before anything else on Mac OS 10.3 (and probably other
// systems) when _GLIBCPP_USE_C99 is defined, or the compiler can't
// resolve ::_Exit.
#include <unistd.h>
#include <stdlib.h>

#include <set>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <deque>
#include <cmath>

#include <assert.h>

#include "thread/thread.hh"

// This is like dynamic_cast<>, but it ensures the correctness of the
// result.
template<typename TO, typename FROM>
inline TO
assert_cast (FROM val)
{
  TO result = dynamic_cast<TO> (val);
  assert (result);
  return result;
}

/// FIXME: probably should be elsewhere.
/// Really we ought to self-init...
extern void initialize_primitive_types ();

// FIXME: of course, these are bogus.  We can take the code from gcj.
// One thing to note is that we could also have a class to represent
// one or more of these, for instance when needed to ensure proper
// constant folding.
typedef bool jboolean;
typedef char jbyte;
typedef short jshort;
typedef unsigned short jchar;
typedef int jint;
typedef long long jlong;
typedef float jfloat;
typedef double jdouble;

// FIXME
typedef char int8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

// Has to be wider than a jchar, since we want to also be able to hold
// a couple extra values.
typedef unsigned int unicode_w_t;

// The special EOF value.
#define UNICODE_EOF ((unicode_w_t) -1)

// Some integer extrema.
#define MIN_INT  -0x80000000LL
#define MAX_INT   0x7fffffffLL
#define MIN_LONG -0x8000000000000000LL
#define MAX_LONG  0x7fffffffffffffffLL

#include "util.hh"

#include "owner.hh"

// Forward-declaration for use by the lexer headers.
class model_element;
typedef owner<model_element> ref_element;

/// This is a list of possible warning states.
typedef enum
  {
    OFF = 0,
    WARN = 1,
    ERROR = 2
  } warning_state;

#include "exception.hh"

#include "global.hh"
#include "factory.hh"
#include "model/iscope.hh"
#include "warnings.hh"
#include "scope.hh"
#include "access.hh"

#include "location.hh"
#include "format/format.hh"

// Modifiers, once resolved.
typedef unsigned int modifier_t;

class visitor;

#include "resolve.hh"
#include "watch.hh"

#include "model/iname.hh"
#include "model/icontext.hh"
#include "model/icatcher.hh"
#include "model/imember.hh"

// FIXME includes &c are bogus here
// need their own header
#include "model/value.hh"

#include "model/element.hh"

#include "model/modifier.hh"
typedef owner<model_modifier_list> ref_modifier_list;

#include "model/imodifiable.hh"

#include "model/ilabel.hh"

#include "model/javadoc.hh"
typedef owner<model_javadoc> ref_javadoc;

#include "model/ideprecatable.hh"

#include "model/expr.hh"
#include "model/type.hh"
typedef owner<model_type> ref_type;

#include "model/literal.hh"
typedef owner<model_boolean_literal> ref_boolean_literal;
typedef owner<model_byte_literal> ref_byte_literal;
typedef owner<model_char_literal> ref_char_literal;
typedef owner<model_short_literal> ref_short_literal;
typedef owner<model_int_literal> ref_int_literal;
typedef owner<model_long_literal> ref_long_literal;
typedef owner<model_float_literal> ref_float_literal;
typedef owner<model_double_literal> ref_double_literal;
typedef owner<model_string_literal> ref_string_literal;

#include "model/identifier.hh"
typedef owner<model_identifier> ref_identifier;

#include "model/primitive.hh"
#include "model/void.hh"

#include "model/expr.hh"
typedef owner<model_expression> ref_expression;

#include "fold.hh"

#include "model/null.hh"

#include "model/binary.hh"
typedef owner<model_binary_base> ref_binary;

#include "model/assign.hh"
typedef owner<model_assignment> ref_assignment;

#include "model/fwdtype.hh"
typedef owner<model_forwarding_type> ref_forwarding_type;
typedef owner<model_forwarding_parameterized> ref_forwarding_parameterized;

#include "model/stmt.hh"
typedef owner<model_stmt> ref_stmt;

#include "model/typemap.hh"

#include "model/annovalue.hh"
typedef owner<model_annotation_value> ref_annotation_value;

#include "model/annomember.hh"
typedef owner<model_annotation_member> ref_annotation_member;

#include "model/annotation.hh"
typedef owner<model_annotation> ref_annotation;

#include "model/iannotatable.hh"

#include "model/variable.hh"
typedef owner<model_variable_decl> ref_variable_decl;

#include "model/assert.hh"
typedef owner<model_assert> ref_assert;

#include "model/block.hh"
typedef owner<model_block> ref_block;

#include "model/init.hh"

#include "bytecode/cpool.hh"

#include "model/bytecode.hh"
typedef owner<model_bytecode_block> ref_bytecode_block;

#include "model/loop.hh"
#include "model/do.hh"
typedef owner<model_do> ref_do;

#include "model/empty.hh"
typedef owner<model_empty> ref_empty;

#include "model/exprstmt.hh"
typedef owner<model_expression_stmt> ref_expression_stmt;

#include "model/for.hh"
typedef owner<model_for_base> ref_for;

#include "model/if.hh"
typedef owner<model_if> ref_if;

#include "model/label.hh"
typedef owner<model_label> ref_label;

#include "model/return.hh"
#include "model/break.hh"
#include "model/continue.hh"

#include "model/switch.hh"
typedef owner<model_switch> ref_switch;

#include "model/synchronized.hh"
typedef owner<model_synchronized> ref_synchronized;

#include "model/throw.hh"
typedef owner<model_throw> ref_throw;

#include "model/forenhanced.hh"

#include "model/varstmt.hh"
typedef owner<model_variable_stmt> ref_variable_stmt;

#include "model/catch.hh"
typedef owner<model_catch> ref_catch;

#include "model/try.hh"
typedef owner<model_try> ref_try;

#include "model/while.hh"
typedef owner<model_while> ref_while;

// This must be forward-declared so it can be used by method.hh and
// class.hh.
class model_type_variable;
typedef owner<model_type_variable> ref_type_variable;

class model_class_instance;
typedef owner<model_class_instance> ref_class_instance;

#include "model/field.hh"
typedef owner<model_field> ref_field;

#include "model/fieldinit.hh"

#include "model/parameters.hh"
#include "model/throwsclause.hh"
#include "model/method.hh"
typedef owner<model_method> ref_method;
#include "model/constructor.hh"

#include "model/class.hh"
typedef owner<model_class> ref_class;

#include "model/typevar.hh"
#include "model/classinst.hh"
#include "model/intersect.hh"

#include "model/annotype.hh"
typedef owner<model_annotation_type> ref_annotation_type;

#include "model/wildcard.hh"
typedef owner<model_wildcard> ref_wildcard;

#include "model/package.hh"
typedef owner<model_package> ref_package;
typedef owner<model_unnamed_package> ref_unnamed_package;

#include "model/arraytype.hh"

#include "model/newarray.hh"
typedef owner<model_new_array> ref_new_array;

#include "model/arrayref.hh"
typedef owner<model_array_ref> ref_array_ref;

#include "model/arrayinit.hh"

#include "model/unary.hh"
typedef owner<model_unary> ref_unary;

#include "model/cast.hh"

#include "model/memberref.hh"
typedef owner<model_memberref_forward> ref_memberref_forward;

#include "model/cond.hh"
#include "model/classref.hh"

#include "model/varref.hh"
typedef owner<model_simple_variable_ref> ref_simple_variable_ref;

#include "model/this.hh"
#include "model/instanceof.hh"

#include "model/fieldref.hh"
typedef owner<model_field_ref> ref_field_ref;

#include "model/invoke.hh"
typedef owner<model_invocation_base> ref_invocation_base;
typedef owner<model_super_invocation> ref_super_invocation;
typedef owner<model_method_invocation> ref_method_invocation;

// enable if needed
// typedef model_generic_invocation<model_memberref_forward> model_generic_memberref_forward;

#include "model/new.hh"
typedef owner<model_new> ref_new;
typedef owner<model_new_primary> ref_new_primary;

#include "model/import.hh"
typedef owner<model_import> ref_import;

#include "model/unit.hh"
typedef owner<model_unit> ref_unit;

#include "model/declstmt.hh"

#include "model/enumconst.hh"
typedef owner<model_enum_constant> ref_enum_constant;

#include "model/enum.hh"
typedef owner<model_enum> ref_enum;

#include "model/blockscope.hh"


#include "classcache.hh"
#include "directory.hh"
#include "compiler.hh"
#include "conversions.hh"

// Must come after various model_*.
#include "visitor.hh"

#include "name.hh"

// FIXME
#define FILE_SEPARATOR '/'

#endif // GCJX_TYPEDEFS_HH
