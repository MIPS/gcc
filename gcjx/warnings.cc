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

#include "typedefs.hh"

struct warning_desc
{
  const char *name;
  warning_flag value;
  const char *desc;
};

static warning_desc warnings[] =
{
  {
    "assert",
    WARN_ASSERT,
    "use of `assert' keyword"
  },
  {
    "enum",
    WARN_ENUM,
    "use of `enum' keyword"
  },
  {
    "canonical-modifier-order",
    WARN_CANONICAL_MODIFIER_ORDER,
    "misplaced modifiers"
  },
  {
    "redundant-modifier",
    WARN_REDUNDANT_MODIFIER,
    "redundant modifiers"
  },
  {
    "deprecated",
    WARN_DEPRECATED,
    "use of deprecated elements"
  },
  {
    "java-lang-import",
    WARN_JAVA_LANG_IMPORT,
    "import of `java.lang'"
  },
  {
    "unused-import",
    WARN_UNUSED_IMPORT,
    "unused imports"
  },
  {
    "non-static-context",
    WARN_NON_STATIC_CONTEXT,
    "non-static qualifier for static member"
  },
  {
    "dollar-identifier",
    WARN_DOLLAR_IDENTIFIER,
    "`$' in identifiers"
  },
  {
    "ascii-escape",
    WARN_ASCII_ESCAPE,
    "\\u representing an ASCII character"
  },
  {
    "constructor-name",
    WARN_CONSTRUCTOR_NAME,
    "method with name of constructor"
  },
  {
    "finalize-spelling",
    WARN_FINALIZE_SPELLING,
    "`finalise' misspelling"
  },
  {
    "assignment-no-effect",
    WARN_ASSIGNMENT_NO_EFFECT,
    "assignment with no effect"
  },
  {
    "string-plus-char-array",
    WARN_STRING_PLUS_CHAR_ARRAY,
    "String `+' with char[]"
  },
  {
    "finally-abnormal-completion",
    WARN_FINALLY_ABNORMAL_COMPLETION,
    "`finally' can't complete normally"
  },
  {
    "unneeded-instanceof",
    WARN_UNNEEDED_INSTANCEOF,
    "unnecessary `instanceof'"
  },
  {
    "unused-private-members",
    WARN_UNUSED_PRIVATE_MEMBERS,
    "unused private member"
  },
  {
    "bad-serialization-field",
    WARN_BAD_SERIALIZATION_FIELD,
    "bad serialization fields"
  },
  {
    "unneeded-else",
    WARN_UNNEEDED_ELSE,
    "unnecessary `else'"
  },
  {
    "bitwise-boolean",
    WARN_BITWISE_BOOLEAN,
    "bitwise boolean operations"
  },
  {
    "unused-label",
    WARN_UNUSED_LABEL,
    "unused labels"
  },
  {
    "unneeded-extends",
    WARN_UNNEEDED_EXTENDS,
    "unnecessary `extends'"
  },
  {
    "array-size",
    WARN_ARRAY_SIZE,
    "bogus array sizes"
  },
  {
    "enclosing-access",
    WARN_ENCLOSING_ACCESS,
    "enclosing access requires accessor method"
  },
  {
    "zero-shift",
    WARN_ZERO_SHIFT,
    "warn about zero shift count"
  },
  {
    "negative-shift",
    WARN_NEGATIVE_SHIFT,
    "warn about negative shift count"
  },
  {
    "large-shift",
    WARN_LARGE_SHIFT,
    "warn about over-large shift count"
  },
  {
    "switch",
    WARN_SWITCH,
    "warn when switch lacks a an enum constant case"
  },
  {
    "switch-default",
    WARN_SWITCH_DEFAULT,
    "warn when switch lacks a 'default' case"
  },
  {
    "switch-fall-through",
    WARN_SWITCH_FALL_THROUGH,
    "warn when case lacks a 'break'"
  },
  {
    "unused-variable",
    WARN_UNUSED_VARIABLE,
    "warn about unused local variables"
  },
  {
    "unused-parameter",
    WARN_UNUSED_PARAMETER,
    "warn about unused method parameters"
  },
  {
    "unchecked",
    WARN_UNCHECKED,
    "warn about declaration of unchecked exception"
  },

  { NULL, WARN_NONE, NULL }
};



warning_scope::warning_scope ()
  : warning_bits (true),
    error_bits (false)
{
}

warning_scope::warning_scope (warning_scope *other)
  : warning_bits (other->warning_bits),
    error_bits (other->error_bits)
{
}

warning_flag
warning_scope::find_warning (const char *name)
{
  for (int i = 0; warnings[i].name != NULL; ++i)
    {
      if (! strcmp (warnings[i].name, name))
	return warnings[i].value;
    }
  return WARN_NONE;
}

void
warning_scope::print_warning_help (std::ostream &out)
{
  // This duplicates some logic from main.cc:print().
  for (int i = 0; warnings[i].name; ++i)
    {
      out << "  -W" << warnings[i].name;
      int w = 2 + strlen (warnings[i].name);
      for (int j = 2; j < 8 * 4 - w; j += 8)
	out << "\t";
      out << warnings[i].desc << std::endl;
    }
}

void
warning_scope::push_warning_context (IAnnotatable *context)
{
  warning_bits |= context->get_warning_flags ();
}

warning_scope::push_warnings::push_warnings (warning_scope *s,
					     IAnnotatable *context)
  : scope (s),
    save (s->warning_bits)
{
  s->warning_bits |= context->get_warning_flags ();
}

warning_scope::push_warnings::~push_warnings ()
{
  scope->warning_bits = save;
}

void
warning_scope::set_warning (warning_flag flag, warning_state val)
{
  warning_bits.set (flag, val == OFF);
  error_bits.set (flag, val == ERROR);
}
