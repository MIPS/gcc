/* Definitions for c-common.c.
   Copyright (C) 1987, 1993, 1994, 1995, 1997 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Standard named or nameless data types of the C compiler.  */

extern tree c_global_trees[50];

#define char_array_type_node		c_global_trees[0]
#define const_ptr_type_node		c_global_trees[1]
#define const_string_type_node		c_global_trees[2]
#define double_type_node		c_global_trees[3]
#define float_type_node			c_global_trees[4]
#define intTI_type_node			c_global_trees[5]
#define intDI_type_node			c_global_trees[6]
#define intHI_type_node			c_global_trees[7]
#define intQI_type_node			c_global_trees[8]
#define intSI_type_node			c_global_trees[9]
#define int_array_type_node		c_global_trees[10]
#define long_double_type_node		c_global_trees[11]
#define long_integer_type_node		c_global_trees[12]
#define long_long_integer_type_node	c_global_trees[13]
#define long_long_unsigned_type_node	c_global_trees[14]
#define long_unsigned_type_node		c_global_trees[15]
#define complex_integer_type_node	c_global_trees[16]
#define complex_float_type_node		c_global_trees[17]
#define complex_double_type_node	c_global_trees[18]
#define complex_long_double_type_node	c_global_trees[19]
#define ptr_type_node			c_global_trees[20]
#define ptrdiff_type_node		c_global_trees[21]
#define short_integer_type_node		c_global_trees[22]
#define short_unsigned_type_node	c_global_trees[23]
#define signed_char_type_node		c_global_trees[24]
#define signed_wchar_type_node		c_global_trees[25]
#define string_type_node		c_global_trees[26]
#define unsigned_char_type_node		c_global_trees[27]
#define unsigned_intTI_type_node	c_global_trees[28]
#define unsigned_intDI_type_node	c_global_trees[29]
#define unsigned_intHI_type_node	c_global_trees[30]
#define unsigned_intQI_type_node	c_global_trees[31]
#define unsigned_intSI_type_node	c_global_trees[32]
#define unsigned_wchar_type_node	c_global_trees[33]
#define wchar_array_type_node		c_global_trees[34]
#define wchar_type_node			c_global_trees[35]
#define boolean_type_node		c_global_trees[36]
#define boolean_true_node		c_global_trees[37]
#define boolean_false_node		c_global_trees[38]

#define default_function_type		c_global_trees[39]
#define double_ftype_double		c_global_trees[40]
#define double_ftype_double_double	c_global_trees[41]
#define int_ftype_cptr_cptr_sizet	c_global_trees[42]
#define int_ftype_int			c_global_trees[43]
#define int_ftype_string_string		c_global_trees[44]
#define long_ftype_long			c_global_trees[45]
#define string_ftype_ptr_ptr		c_global_trees[46]
#define void_ftype_ptr_int_int		c_global_trees[47]
#define float_ftype_float		c_global_trees[48]
#define ldouble_ftype_ldouble		c_global_trees[49]

extern void declare_function_name               PROTO((void));
extern void decl_attributes                     PROTO((tree, tree, tree));
extern void init_function_format_info		PROTO((void));
extern void check_function_format		PROTO((tree, tree, tree));
extern int c_get_alias_set                      PROTO((tree));
/* Print an error message for invalid operands to arith operation CODE.
   NOP_EXPR is used as a special case (see truthvalue_conversion).  */
extern void binary_op_error                     PROTO((enum tree_code));
extern void c_expand_expr_stmt                  PROTO((tree));
extern void c_expand_start_cond                 PROTO((tree, int, int));
extern void c_expand_start_else                 PROTO((void));
extern void c_expand_end_cond                   PROTO((void));
/* Validate the expression after `case' and apply default promotions.  */
extern tree check_case_value                    PROTO((tree));
/* Concatenate a list of STRING_CST nodes into one STRING_CST.  */
extern tree combine_strings                     PROTO((tree));
extern void constant_expression_warning         PROTO((tree));
extern tree convert_and_check			PROTO((tree, tree));
extern void overflow_warning			PROTO((tree));
extern void unsigned_conversion_warning		PROTO((tree, tree));
/* Read the rest of the current #-directive line.  */
#if USE_CPPLIB
extern char *get_directive_line                 PROTO((void));
#define GET_DIRECTIVE_LINE() get_directive_line ()
#else
extern char *get_directive_line                 PROTO((FILE *));
#define GET_DIRECTIVE_LINE() get_directive_line (finput)
#endif

/* Subroutine of build_binary_op, used for comparison operations.
   See if the operands have both been converted from subword integer types
   and, if so, perhaps change them both back to their original type.  */
extern tree shorten_compare                     PROTO((tree *, tree *, tree *, enum tree_code *));
/* Prepare expr to be an argument of a TRUTH_NOT_EXPR,
   or validate its data type for an `if' or `while' statement or ?..: exp. */
extern tree truthvalue_conversion               PROTO((tree));
extern tree type_for_mode                       PROTO((enum machine_mode, int));
extern tree type_for_size                       PROTO((unsigned, int));

