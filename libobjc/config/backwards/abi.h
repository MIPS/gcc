/* Backwards compatiblity for old targets
   Copyright (C) 1993, 1995, 1996, 1997, 1998, 2000, 2002, 2004
   Free Software Foundation, Inc.
   Contributed by Andrew Pinski

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* As a special exception, if you link this library with files
   compiled with GCC to produce an executable, this does not cause
   the resulting executable to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

/* FIXME: This file has no business including tm.h.  */
#warning This file should not be used at all, you should update
#warning target to use the new style config for libobjc

#include "tconfig.h"
#include "coretypes.h"
#include "tm.h"

/* Various hacks for objc_layout_record. These are used by the target
   macros. */

#define TREE_CODE(TYPE) *(TYPE)
#define TREE_TYPE(TREE) (TREE)

#define RECORD_TYPE     _C_STRUCT_B
#define UNION_TYPE      _C_UNION_B
#define QUAL_UNION_TYPE _C_UNION_B
#define ARRAY_TYPE      _C_ARY_B

#define REAL_TYPE       _C_DBL

#define VECTOR_TYPE     _C_VECTOR

#define TYPE_FIELDS(TYPE)           ({const char *_field = (TYPE)+1; \
    while (*_field != _C_STRUCT_E && *_field != _C_STRUCT_B \
           && *_field != _C_UNION_B && *_field++ != '=') \
    /* do nothing */; \
    _field;})

#define DECL_MODE(TYPE) *(TYPE)
#define TYPE_MODE(TYPE) *(TYPE)

#define DFmode          _C_DBL

#define get_inner_array_type(TYPE)      ({const char *_field = (TYPE); \
  while (*_field == _C_ARY_B)\
    {\
      while (isdigit ((unsigned char)*++_field))\
	;\
    }\
    _field;})

/* Some ports (eg ARM) allow the structure size boundary to be selected
   at compile-time.  We usually use CHAR_BIT, but we have to override
   the normal definition with one that has a constant value for this
   compilation, in case another target macro we use needs BITS_PER_UNIT.
   */
#undef BITS_PER_UNIT
#define BITS_PER_UNIT __CHAR_BIT__

/* Some ROUND_TYPE_ALIGN macros use TARGET_foo, and consequently
   target_flags.  Define a dummy entry here to so we don't die.
   We have to rename it because target_flags may already have been
   declared extern.  */
#define target_flags not_target_flags
static int __attribute__ ((__unused__)) not_target_flags = 0;

/* Some ROUND_TYPE_ALIGN use ALTIVEC_VECTOR_MODE (rs6000 darwin).
 *    Define a dummy ALTIVEC_VECTOR_MODE so it will not die.  */
#undef ALTIVEC_VECTOR_MODE
#define ALTIVEC_VECTOR_MODE(MODE) (0)


/*  FIXME: while this file has no business including tm.h, this
    definitely has no business defining this macro but it
    is only way around without really rewritting this file,
    should look after the branch of 3.4 to fix this.  */
#define rs6000_special_round_type_align(STRUCT, COMPUTED, SPECIFIED)	\
  ({ const char *_fields = TYPE_FIELDS (STRUCT);				\
  ((_fields != 0							\
    && TYPE_MODE (TREE_CODE (TREE_TYPE (_fields)) == ARRAY_TYPE		\
		    ? get_inner_array_type (_fields)			\
		    : TREE_TYPE (_fields)) == DFmode)			\
   ? MAX (MAX (COMPUTED, SPECIFIED), 64)				\
   : MAX (COMPUTED, SPECIFIED));})

