// Definitions for compiler warnings.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#ifndef GCJX_WARNINGS_HH
#define GCJX_WARNINGS_HH

#include <string.h>
#include <limits.h>

typedef enum
{
  WARN_NONE = -1,
  WARN_ASSERT,
  WARN_ENUM,
  WARN_CANONICAL_MODIFIER_ORDER,
  WARN_REDUNDANT_MODIFIER,
  WARN_DEPRECATED,
  WARN_JAVA_LANG_IMPORT,
  WARN_UNUSED_IMPORT,
  WARN_NON_STATIC_CONTEXT,
  WARN_DOLLAR_IDENTIFIER,
  WARN_ASCII_ESCAPE,
  WARN_CONSTRUCTOR_NAME,
  WARN_FINALIZE_SPELLING,
  WARN_ASSIGNMENT_NO_EFFECT,
  WARN_STRING_PLUS_CHAR_ARRAY,
  WARN_FINALLY_ABNORMAL_COMPLETION,
  WARN_UNNEEDED_INSTANCEOF,
  WARN_UNUSED_PRIVATE_MEMBERS,
  WARN_BAD_SERIALIZATION_FIELD,
  WARN_UNNEEDED_ELSE,
  WARN_BITWISE_BOOLEAN,
  WARN_UNUSED_LABEL,
  WARN_UNNEEDED_EXTENDS,
  WARN_ARRAY_SIZE,
  WARN_ENCLOSING_ACCESS,
  WARN_ZERO_SHIFT,
  WARN_NEGATIVE_SHIFT,
  WARN_LARGE_SHIFT,
  WARN_SWITCH,
  WARN_SWITCH_DEFAULT,
  WARN_SWITCH_FALL_THROUGH,
  WARN_UNUSED_VARIABLE,
  WARN_UNUSED_PARAMETER,
  WARN_UNCHECKED,

  // Must be last.
  WARN_LAST
} warning_flag;

#define WARN_NBYTES ((WARN_LAST + CHAR_BIT - 1) / CHAR_BIT)

class IAnnotatable;

/// This is a helper class that keeps track of bit sets for warning
/// handling.
class warning_bitset
{
  unsigned char bits[WARN_NBYTES];

public:

  warning_bitset (bool on)
  {
    memset (bits, on ? 0xff : 0, WARN_NBYTES);
  }

  warning_bitset (const warning_bitset &other)
  {
    memcpy (bits, other.bits, WARN_NBYTES);
  }

  warning_bitset &operator= (const warning_bitset &other)
  {
    memcpy (bits, other.bits, WARN_NBYTES);
    return *this;
  }

  warning_bitset &operator|= (const warning_bitset &other)
  {
    for (int i = 0; i < WARN_NBYTES; ++i)
      bits[i] |= other.bits[i];
    return *this;
  }

  void set (int index, bool val)
  {
    int a = index / CHAR_BIT;
    int mask = 1 << (index % CHAR_BIT);
    if (val)
      bits[a] |= mask;
    else
      bits[a] &= ~mask;
  }

  bool get (int index) const
  {
    int a = index / CHAR_BIT;
    int mask = 1 << (index % CHAR_BIT);
    return bits[a] & mask;
  }
};

/// This class keeps track of warning states.  A given warning can be
/// enabled, and, if enabled, can result in an error or a plain
/// warning.
class warning_scope
{
  // Current warning flags.  A set bit here indicates that the warning
  // is suppressed in the current scope.
  warning_bitset warning_bits;

  // Current error flags.  A warning that is enabled and whose
  // corresponding bit is set here, should be emitted as an error.
  warning_bitset error_bits;

  warning_state check_warning (warning_flag flag) const
  {
    if (warning_bits.get (flag))
      return OFF;
    return error_bits.get (flag) ? ERROR : WARN;
  }

public:

  warning_scope ();

  warning_scope (warning_scope *);

  /// An asymmetric API: this sets the warning suppression state from
  /// the IAnnotatable, but there is no way to pop it.
  void push_warning_context (IAnnotatable *context);

  /// More asymmetric APIs, for setting a particular warning's state.
  void set_warning (warning_flag flag, warning_state val);

  /// One method per warning we support.
  warning_state warn_assert () const
  {
    return check_warning (WARN_ASSERT);
  }

  warning_state warn_enum () const
  {
    return check_warning (WARN_ENUM);
  }

  warning_state warn_canonical_modifier_order () const
  {
    return check_warning (WARN_CANONICAL_MODIFIER_ORDER);
  }

  warning_state warn_redundant_modifier () const
  {
    return check_warning (WARN_REDUNDANT_MODIFIER);
  }

  warning_state warn_deprecated () const
  {
    return check_warning (WARN_DEPRECATED);
  }

  warning_state warn_java_lang_import () const
  {
    return check_warning (WARN_JAVA_LANG_IMPORT);
  }

  warning_state warn_unused_import () const
  {
    return check_warning (WARN_UNUSED_IMPORT);
  }

  warning_state warn_non_static_context () const
  {
    return check_warning (WARN_NON_STATIC_CONTEXT);
  }

  warning_state warn_dollar_identifier () const
  {
    return check_warning (WARN_DOLLAR_IDENTIFIER);
  }

  warning_state warn_ascii_escape () const
  {
    return check_warning (WARN_ASCII_ESCAPE);
  }

  warning_state warn_constructor_name () const
  {
    return check_warning (WARN_CONSTRUCTOR_NAME);
  }

  warning_state warn_finalize_spelling () const
  {
    return check_warning (WARN_FINALIZE_SPELLING);
  }

  warning_state warn_assignment_no_effect () const
  {
    return check_warning (WARN_ASSIGNMENT_NO_EFFECT);
  }

  warning_state warn_string_plus_char_array () const
  {
    return check_warning (WARN_STRING_PLUS_CHAR_ARRAY);
  }

  warning_state warn_finally_abnormal_completion () const
  {
    return check_warning (WARN_FINALLY_ABNORMAL_COMPLETION);
  }

  warning_state warn_unneeded_instanceof () const
  {
    return check_warning (WARN_UNNEEDED_INSTANCEOF);
  }

  warning_state warn_unused_private_members () const
  {
    return check_warning (WARN_UNUSED_PRIVATE_MEMBERS);
  }

  warning_state warn_bad_serialization_field () const
  {
    return check_warning (WARN_BAD_SERIALIZATION_FIELD);
  }

  warning_state warn_unneeded_else () const
  {
    return check_warning (WARN_UNNEEDED_ELSE);
  }

  warning_state warn_bitwise_boolean () const
  {
    return check_warning (WARN_BITWISE_BOOLEAN);
  }

  warning_state warn_unused_label () const
  {
    return check_warning (WARN_UNUSED_LABEL);
  }

  warning_state warn_unneeded_extends () const
  {
    return check_warning (WARN_UNNEEDED_EXTENDS);
  }

  warning_state warn_array_size () const
  {
    return check_warning (WARN_ARRAY_SIZE);
  }

  warning_state warn_enclosing_access () const
  {
    return check_warning (WARN_ENCLOSING_ACCESS);
  }

  warning_state warn_zero_shift () const
  {
    return check_warning (WARN_ZERO_SHIFT);
  }

  warning_state warn_negative_shift () const
  {
    return check_warning (WARN_NEGATIVE_SHIFT);
  }

  warning_state warn_large_shift () const
  {
    return check_warning (WARN_LARGE_SHIFT);
  }

  warning_state warn_switch () const
  {
    return check_warning (WARN_SWITCH);
  }

  warning_state warn_switch_default () const
  {
    return check_warning (WARN_SWITCH_DEFAULT);
  }

  warning_state warn_switch_fall_through () const
  {
    return check_warning (WARN_SWITCH_FALL_THROUGH);
  }

  warning_state warn_unused_variable () const
  {
    return check_warning (WARN_UNUSED_VARIABLE);
  }

  warning_state warn_unused_parameter () const
  {
    return check_warning (WARN_UNUSED_PARAMETER);
  }

  warning_state warn_unchecked () const
  {
    return check_warning (WARN_UNCHECKED);
  }


  /// Return the warning flag corresponding to the warning name, or
  /// WARN_NONE if no such named warning exists.
  static warning_flag find_warning (const char *);

  /// Print help for all recognized warnings.
  static void print_warning_help (std::ostream &);

  /// This uses the RAII idiom to push some warning suppressions onto
  /// a warning_scope, and then remove them when the object is
  /// destroyed.
  class push_warnings
  {
    // Scope we're using.
    warning_scope *scope;

    // Saved bits.
    warning_bitset save;

  public:

    push_warnings (warning_scope *, IAnnotatable *);

    ~push_warnings ();
  };
};

#endif // GCJX_WARNINGS_HH
