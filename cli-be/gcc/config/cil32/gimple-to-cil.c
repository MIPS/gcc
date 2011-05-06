/* GIMPLE to CIL conversion pass.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto
   Thierry Lafage

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "flags.h"
#include "timevar.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "pointer-set.h"
#include "c-common.h"

#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-stmt.h"
#include "cil-types.h"
#include "cil-stack.h"
#include "emit-cil.h"
#include "source-location.h"
#include "missing-protos.h"

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

/* Return variable for pre-C99 functions which contain VOID return statements
   even though they are declared to return a non-VOID value.  */
static tree res_var;

/* Defined in cfgexpand.c, made non-local so that we can use it here */
extern tree gimple_to_tree (gimple);
extern void release_stmt_tree (gimple, tree);

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static void gen_integer_cst (cil_stmt_iterator *, tree, source_location);
static void gen_real_cst (cil_stmt_iterator *, tree, source_location);
static void gen_vector_cst (cil_stmt_iterator *, tree, source_location);
static void gen_addr_expr (cil_stmt_iterator *, tree, source_location);
static void gen_array_ref_addr_expr (cil_stmt_iterator *, tree,
                                     source_location);
static void gen_initblk (cil_stmt_iterator *, tree, tree, tree,
                         source_location);
static void gen_cpblk (cil_stmt_iterator *, tree, tree, tree, source_location);
static void gen_scalar_ld_st_ind (cil_stmt_iterator *, tree, bool, bool,
                                  source_location);
static inline void gen_scalar_stind (cil_stmt_iterator *, tree, bool,
                                     source_location);
static inline void gen_scalar_ldind (cil_stmt_iterator *, tree, bool,
                                     source_location);
static void gen_ldind (cil_stmt_iterator *, tree, bool, source_location);
static void gen_stind (cil_stmt_iterator *, tree, bool, source_location);
static bool mostly_zeros_p (tree);
static bool all_zeros_p (tree);
static void write_cst_image (char *, char *, tree, unsigned HOST_WIDE_INT);
static void expand_init_to_cil_seq1 (tree, tree, cil_seq, bool, cil_seq,
				     char *, char *, source_location);
static size_t cil_seq_num_instr (cil_seq);
static void gen_bit_field_modify_expr (cil_stmt_iterator *, tree, tree,
                                       source_location);
static void gen_target_mem_ref_modify_expr (cil_stmt_iterator *, tree, tree,
                                            source_location);
static void gen_bitfield_ref_modify_expr (cil_stmt_iterator *, tree, tree,
                                          source_location);
static void gen_vector_bitfield_ref_modify_expr (cil_stmt_iterator *, tree,
                                                 tree, source_location);
static void gen_modify_expr (cil_stmt_iterator *, tree, tree, source_location);
static void gen_goto_expr (cil_stmt_iterator *, tree);
static void gen_cond_expr (cil_stmt_iterator *, tree);
static void gen_switch_expr (cil_stmt_iterator *, tree);
static void gen_builtin_va_start (cil_stmt_iterator *, tree, source_location);
static void gen_builtin_va_end (cil_stmt_iterator *, tree, source_location);
static void gen_builtin_va_copy (cil_stmt_iterator *, tree, source_location);
static bool gen_call_builtin (cil_stmt_iterator *, tree, tree);
static void gen_call_expr (cil_stmt_iterator *, tree);
static tree gen_expr_copy (cil_stmt_iterator *, tree, source_location);
static void gen_bit_and_expr (cil_stmt_iterator *, tree, source_location);
static void gen_compare_expr (cil_stmt_iterator *, tree, source_location);
static void gen_minmax_expr (cil_stmt_iterator *, tree, source_location);
static void gen_abs_expr (cil_stmt_iterator *, tree, source_location);
static void gen_var_decl (cil_stmt_iterator *, tree, source_location);
static void gen_bit_field_comp_ref (cil_stmt_iterator *, tree, source_location);
static void gen_comp_ref (cil_stmt_iterator *, tree, source_location);
static void gen_vector_bitfield_ref (cil_stmt_iterator *, tree,
                                     source_location);
static void gen_bit_field_ref (cil_stmt_iterator *, tree, source_location);
static void gen_truth_expr (cil_stmt_iterator *, tree, source_location);
static void gen_target_mem_ref (cil_stmt_iterator *, tree, source_location);
static void gen_vector_ctor (cil_stmt_iterator *, tree, source_location);
static void gen_vector_view_convert_expr (cil_stmt_iterator *, tree, tree,
                                          source_location);
static void gen_complex_part_expr (cil_stmt_iterator *, tree, source_location);
static void gen_complex (cil_stmt_iterator *, tree, tree, tree,
                         source_location);
static enum cil_opcode conv_opcode_from_type (tree);
static void gen_integral_conv (cil_stmt_iterator *, tree, tree,
                               source_location);
static void gen_conv (cil_stmt_iterator *, bool, tree, tree, source_location);
static void gen_rotate (cil_stmt_iterator *, tree, source_location);
static void gimple_to_cil_node (cil_stmt_iterator *, tree, source_location);
static void process_labels (void);
static void process_initializers (void);

/******************************************************************************
 * GIMPLE/generic to CIL conversion functions                                 *
 ******************************************************************************/

/* Load the value of the integer constant CST on the stack.  The constant will
   be 32-bits or 64-bits wide depending on the type of CST.  The generated
   statements will be appended to the current function's CIL code using the CSI
   iterator.  */

static void
gen_integer_cst (cil_stmt_iterator *csi, tree cst, source_location sloc)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (TREE_TYPE (cst)), 1);
  enum cil_opcode opcode;
  cil_stmt stmt;

  opcode = (size <= 32) ? CIL_LDC_I4 : CIL_LDC_I8;
  stmt = cil_build_stmt_arg (opcode, cst);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Load the value of the real constant CST on the stack.  The constant will
   be 32-bits or 64-bits wide depending on the type of CST.  The generated
   statements will be appended to the current function CIL code using the CSI
   iterator.  */

static void
gen_real_cst (cil_stmt_iterator *csi, tree cst, source_location sloc)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (TREE_TYPE (cst)), 1);
  enum cil_opcode opcode;
  cil_stmt stmt;

  opcode = (size == 32) ? CIL_LDC_R4 : CIL_LDC_R8;
  stmt = cil_build_stmt_arg (opcode, cst);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}


/* If all elements of a constant vector are equal, return that
   element. Otherwise return NULL.  */

static tree
is_vec_uniform (tree vec)
{
  tree elt;
  tree prev_val = NULL;

  for (elt = TREE_VECTOR_CST_ELTS (vec); elt; elt = TREE_CHAIN (vec))
    {
      tree elt_val = TREE_VALUE (elt);
      if (!prev_val)
        prev_val = elt_val;  /* First value seen, register it. */
      else
        {
          /* Stop at the first value that differs. */
          if (prev_val != elt_val)
              return NULL;
        }
    }
  return prev_val;
}


/* Load the value of a vector constant CST on the stack.  The generated
   statements will be appended to the current function CIL code using the CSI
   iterator.  */

static void
gen_vector_cst (cil_stmt_iterator *csi, tree cst, source_location sloc)
{
  unsigned HOST_WIDE_INT num_elt = 0;
  tree vector_type = TREE_TYPE (cst);
  tree elt_type = TREE_TYPE (vector_type);
  tree zero, elt;
  cil_type_t cil_type = scalar_to_cil (elt_type);
  tree     uniform_val = NULL;
  cil_stmt stmt;

  if (cil_float_p (cil_type))
    zero = build_real_from_int_cst (elt_type, integer_zero_node);
  else if (cil_int_or_smaller_p (cil_type))
    zero = integer_zero_node;
  else /* 64-bit integers */
    zero = build_int_cst (intDI_type_node, 0);

  /* If all elements are equal, use the shorter form of the constructor.  */
  uniform_val = is_vec_uniform (cst);
  if (uniform_val)
    gimple_to_cil_node (csi, uniform_val, sloc);
  else
    {
      for (elt = TREE_VECTOR_CST_ELTS (cst); elt; elt = TREE_CHAIN (elt))
        {
          tree elt_val = TREE_VALUE (elt);
          gimple_to_cil_node (csi, elt_val, sloc);
          ++num_elt;
        }

      /* Fill in the missing initializers, if any */
      for ( ; num_elt < TYPE_VECTOR_SUBPARTS (vector_type); ++num_elt)
        gimple_to_cil_node (csi, zero, sloc);
    }

  stmt = cil_build_stmt_arg (CIL_VEC_CTOR, vector_type);
  cil_set_locus(stmt, sloc);
  if (uniform_val)
    cil_set_short_ctor (stmt, 1);

  csi_insert_after (csi,  stmt, CSI_CONTINUE_LINKING);

  if (cfun)
    cfun->machine->has_vec = true;
}

/* Generates a sequence which computes the address of the object described by
   OBJ and pushes it on top of the stack. The generated statements are
   appended to the current function's CIL code using the CSI iterator.  */

static void
gen_addr_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;

  switch (TREE_CODE (node))
    {
    case STRING_CST:
      node = mark_referenced_string (node);
      stmt = cil_build_stmt_arg (CIL_LDSFLDA, node);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case VAR_DECL:
    case RESULT_DECL:
      /* Function local static variables are promoted to global variables.  */
      if (!DECL_FILE_SCOPE_P (node) && !TREE_STATIC (node))
	stmt = cil_build_stmt_arg (CIL_LDLOCA, node);
      else
	stmt = cil_build_stmt_arg (CIL_LDSFLDA, node);

      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (TREE_CODE (TREE_TYPE (node)) == ARRAY_TYPE)
	{
	  stmt = cil_build_stmt (CIL_CONV_I);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      break;

    case PARM_DECL:
      stmt = cil_build_stmt_arg (CIL_LDARGA, node);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (TREE_CODE (TREE_TYPE (node)) == ARRAY_TYPE)
        {
          stmt = cil_build_stmt (CIL_CONV_I);
          cil_set_locus(stmt, sloc);
          csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
        }
      else if (TREE_CODE (TREE_TYPE (node)) == VECTOR_TYPE)
        cfun->machine->has_vec = true;

      break;

    case FUNCTION_DECL:
      stmt = cil_build_stmt_arg (CIL_LDFTN, node);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case LABEL_DECL:
      {
	/* We cannot emit the address of the label in CIL, so we map each
	  label to an ID and emit the ID.  The GOTO will then be implemented
	  with a switch based on that ID.  The ID is simply the position in
	  the list of all address taken labels.  */

	tree id = get_addr_taken_label_id (node);
	gen_integer_cst (csi, id, sloc);
      }
      break;

    case INDIRECT_REF:
    case MISALIGNED_INDIRECT_REF:
      gimple_to_cil_node (csi, TREE_OPERAND (node, 0), sloc);
      break;

    case ARRAY_REF:
      gen_array_ref_addr_expr (csi, node, sloc);
      break;

    case COMPONENT_REF:
      {
	tree obj = TREE_OPERAND (node, 0);
	tree fld = TREE_OPERAND (node, 1);
	tree obj_type = TYPE_MAIN_VARIANT (TREE_TYPE (obj));

	gcc_assert (!DECL_BIT_FIELD (fld));

	gen_addr_expr (csi, obj, sloc);
	stmt = cil_build_stmt_arg (CIL_LDFLDA, fld);
	cil_set_locus(stmt, source_location_from_tree (fld));
	mark_referenced_type (obj_type);
	/* Some statements might have been added */
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      }
      break;

    case VIEW_CONVERT_EXPR:
      gen_addr_expr (csi, TREE_OPERAND (node, 0), sloc);
      stmt = cil_build_stmt (CIL_CONV_I);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      {
	tree obj = TREE_OPERAND (node, 0);
	tree type = TREE_TYPE (obj);

	gen_addr_expr (csi, obj, sloc);

	if (TREE_CODE (node) == REALPART_EXPR)
	  {
	    stmt = cil_build_stmt_arg (CIL_LDFLDA,
				       cil_get_builtin_complex_real_fld (type));
	  }
	else
	  {
	    stmt = cil_build_stmt_arg (CIL_LDFLDA,
				       cil_get_builtin_complex_imag_fld (type));
	  }

	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      }
      break;

    case COMPOUND_LITERAL_EXPR:
      /* HACK: We should find a way to avoid front-end nodes */
      gen_addr_expr (csi, COMPOUND_LITERAL_EXPR_DECL (node), sloc);
      break;

    default:
      {
	/* A generic expression, we first evaluate it, then store it inside a
	   temporary and finally get the address of said temporary.  */
	tree tmp_var;

	tmp_var = create_tmp_var (TREE_TYPE (node), "addr_expr");
	gimple_to_cil_node (csi, node, sloc);
	stmt = cil_build_stmt_arg (CIL_STLOC, tmp_var);
	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	stmt = cil_build_stmt_arg (CIL_LDLOCA, tmp_var);
	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	stmt = cil_build_stmt (CIL_CONV_I);
	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	break;
      }
    }
}

/* Generates the address of an ARRAY_REF expression.  The generated statements
   are appended to the current funcion's CIL code using the CSI iterator.  */

static void
gen_array_ref_addr_expr (cil_stmt_iterator *csi, tree node,
                         source_location sloc)
{
  HOST_WIDE_INT bitsize = 0;
  HOST_WIDE_INT bitpos = 0;
  tree offset = NULL_TREE;
  enum machine_mode mode;
  int unsignedp = 0;
  int volatilep = 0;
  cil_stmt stmt;
  tree ref;

  ref = get_inner_reference (node, &bitsize, &bitpos, &offset, &mode,
			     &unsignedp, &volatilep, false);
  gen_addr_expr (csi, ref, sloc);

  if (TREE_CODE (TREE_TYPE (ref)) != ARRAY_TYPE)
    {
      stmt = cil_build_stmt (CIL_CONV_I);
      cil_set_locus (stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (bitpos != 0)
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node, bitpos / BITS_PER_UNIT),
		       sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (offset != NULL_TREE)
    {
      gimple_to_cil_node (csi, offset, sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates a CIL INITBLK instruction with the destination address specified
   by PTR, the value specified by VALUE and the size specified by SIZE.  */

static void
gen_initblk (cil_stmt_iterator *csi, tree ptr, tree value, tree size,
             source_location sloc)
{
  cil_stmt stmt;

  gimple_to_cil_node (csi, ptr, sloc);
  gimple_to_cil_node (csi, value, sloc);
  gimple_to_cil_node (csi, size, sloc);
  stmt = cil_build_stmt (CIL_INITBLK);
  cil_set_locus(stmt, sloc);
  cil_set_prefix_unaligned (stmt, 1);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generates a CIL CPBLK instruction with the destination address specified
   by DST, the source address specified by SRC and the size specified by SIZE.
 */

static void
gen_cpblk (cil_stmt_iterator *csi, tree dst, tree src, tree size,
           source_location sloc)
{
  cil_stmt stmt;

  gimple_to_cil_node (csi, dst, sloc);
  gimple_to_cil_node (csi, src, sloc);
  gimple_to_cil_node (csi, size, sloc);
  stmt = cil_build_stmt (CIL_CPBLK);
  cil_set_locus(stmt, sloc);
  cil_set_prefix_unaligned (stmt, 1);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generates a load/store indirect statement for the scalar type specified by
   TYPE. If STORE is true then a store is generated, otherwise a load.
   The statement is made volatile if VOLAT is true. The generated statements
   are appended to the current function's CIL code using the CSI iterator.  */

static void
gen_scalar_ld_st_ind (cil_stmt_iterator *csi, tree type, bool store,
                      bool volat, source_location sloc)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (type), 1);
  enum cil_opcode opcode;
  cil_stmt stmt;

  if (INTEGRAL_TYPE_P (type))
    {
      if (TYPE_UNSIGNED (type))
	{
	  switch (size)
	    {
	    case 8:  opcode = store ? CIL_STIND_I1 : CIL_LDIND_U1; break;
	    case 16: opcode = store ? CIL_STIND_I2 : CIL_LDIND_U2; break;
	    case 32: opcode = store ? CIL_STIND_I4 : CIL_LDIND_U4; break;
	    case 64: opcode = store ? CIL_STIND_I8 : CIL_LDIND_U8; break;
	    default:
	      internal_error ("Unsupported integer size "
			      HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	    }
	}
      else
	{
	  switch (size)
	    {
	    case 8:  opcode = store ? CIL_STIND_I1 : CIL_LDIND_I1; break;
	    case 16: opcode = store ? CIL_STIND_I2 : CIL_LDIND_I2; break;
	    case 32: opcode = store ? CIL_STIND_I4 : CIL_LDIND_I4; break;
	    case 64: opcode = store ? CIL_STIND_I8 : CIL_LDIND_I8; break;
	    default:
	      internal_error ("Unsupported integer size "
			      HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	    }
	}
    }
  else if (POINTER_TYPE_P (type))
    opcode = store ? CIL_STIND_I : CIL_LDIND_I;
  else if (SCALAR_FLOAT_TYPE_P (type))
    {
      switch (size)
	{
	case 32: opcode = store ? CIL_STIND_R4 : CIL_LDIND_R4; break;
	case 64: opcode = store ? CIL_STIND_R8 : CIL_LDIND_R8; break;
	default:
	  internal_error ("Unsupported floating point size "
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
    }
  else
    gcc_unreachable ();

  stmt = cil_build_stmt (opcode);
  cil_set_locus(stmt, sloc);
  cil_set_prefix_volatile (stmt, volat);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generates a load indirect statement for the scalar type specified by TYPE.
   The statement is made volatile if VOLAT is true. The generated statements
   are appended to the current function's CIL code using the CSI iterator.  */

static inline void
gen_scalar_ldind (cil_stmt_iterator *csi, tree type, bool volat,
                  source_location sloc)
{
  gen_scalar_ld_st_ind (csi, type, false, volat, sloc);
}

/* Generates a store indirect statement for the scalar type specified by TYPE.
   The statement is made volatile if VOLAT is true. The generated statements
   are appended to the current function's CIL code using the CSI iterator.  */

static inline void
gen_scalar_stind (cil_stmt_iterator *csi, tree type, bool volat,
                  source_location sloc)
{
  gen_scalar_ld_st_ind (csi, type, true, volat, sloc);
}

/* Generate a load indirect statement for the type specified by TYPE. The
   load is made volatile if VOLAT is true. The generated statements are
   appended to the current function's CIL code using the CSI iterator.  */

static void
gen_ldind (cil_stmt_iterator *csi, tree type, bool volat, source_location sloc)
{
  cil_stmt stmt;

  if (AGGREGATE_TYPE_P (type) || TREE_CODE (type) == COMPLEX_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_LDOBJ, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else if (TREE_CODE (type) == VECTOR_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_LDVEC, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      cfun->machine->has_vec = true;
    }
  else
    gen_scalar_ldind (csi, type, volat, sloc);
}

/* Generate a misaligned indirect load statement for the type specified by
   TYPE. The load is made volatile if VOLAT is true. The generated statements
   are appended to the current function's CIL code using the CSI iterator.  */

static void
gen_misaligned_ldvec (cil_stmt_iterator *csi, tree type, bool volat,
                      source_location sloc)
{
  cil_stmt stmt;

  gcc_assert (TREE_CODE (type) == VECTOR_TYPE);

  if (AGGREGATE_TYPE_P (type) || TREE_CODE (type) == COMPLEX_TYPE)
    {
      gcc_assert (0);  /* FIXME: is this reachable? */
      stmt = cil_build_stmt_arg (CIL_LDOBJ, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else if (TREE_CODE (type) == VECTOR_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_LDVEC, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      cil_set_prefix_unaligned (stmt, 1);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (cfun)
	cfun->machine->has_vec = true;
    }
  else
    gen_scalar_ldind (csi, type, volat, sloc);
}

/* Generate a store indirect statement for the type specified by TYPE. The
   store is made volatile if VOLAT is true. The generated statements are
   appended to the current function's CIL code using the CSI iterator.  */

static void
gen_stind (cil_stmt_iterator *csi, tree type, bool volat, source_location sloc)
{
  cil_stmt stmt;

  if (AGGREGATE_TYPE_P (type) || TREE_CODE (type) == COMPLEX_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_STOBJ, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else if (TREE_CODE (type) == VECTOR_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_STVEC, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (cfun)
	cfun->machine->has_vec = true;
    }
  else
    gen_scalar_stind (csi, type, volat, sloc);
}

/* Generate a misaligned indirect store statement for the type specified by
   TYPE. The store is made volatile if VOLAT is true. The generated statements
   are appended to the current function's CIL code using the CSI iterator.  */

static void
gen_misaligned_stvec (cil_stmt_iterator *csi, tree type, bool volat,
                      source_location sloc)
{
  cil_stmt stmt;

  gcc_assert (TREE_CODE (type) == VECTOR_TYPE);

  if (AGGREGATE_TYPE_P (type) || TREE_CODE (type) == COMPLEX_TYPE)
    {
      gcc_assert (0);  /* FIXME: is this reachable? */
      stmt = cil_build_stmt_arg (CIL_STOBJ, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else if (TREE_CODE (type) == VECTOR_TYPE)
    {
      stmt = cil_build_stmt_arg (CIL_STVEC, type);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, volat);
      cil_set_prefix_unaligned (stmt, 1);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      cfun->machine->has_vec = true;
    }
  else
    gen_scalar_stind (csi, type, volat, sloc);
}

/* Return TRUE if EXP contains mostly (3/4)  zeros.  */

static bool
mostly_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count, elts;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  if (must_clear)
    return TRUE;

  elts = count_type_elements (TREE_TYPE (exp), false);

  return (nz_elts < elts / 4);
}

/* Return TRUE if EXP contains all zeros. */

static bool
all_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  return (nz_elts == 0);
}

/* Write the value of size SIZE bits held by the tree CST into the
   little-endian image LE_IMAGE and big-endian image BE_IMAGE.  */

static void
write_cst_image (char *le_image, char *be_image, tree value,
		 unsigned HOST_WIDE_INT size)
{
  char b0 = TREE_INT_CST_LOW (value);
  char b1 = TREE_INT_CST_LOW (value) >> 8;
  char b2 = TREE_INT_CST_LOW (value) >> 16;
  char b3 = TREE_INT_CST_LOW (value) >> 24;

  switch (size)
    {
    case 8:
      le_image[0] |= b0;
      be_image[0] |= b0;
      break;

    case 16:
      le_image[0] |= b0;
      le_image[1] |= b1;
      be_image[0] |= b1;
      be_image[1] |= b0;
      break;

    case 32:
      le_image[0] |= b0;
      le_image[1] |= b1;
      le_image[2] |= b2;
      le_image[3] |= b3;
      be_image[0] |= b3;
      be_image[1] |= b2;
      be_image[2] |= b1;
      be_image[3] |= b0;
      break;

    default:
      gcc_unreachable ();
    }
}

/* Expand the initializer INIT for the declaration DECL into two CIL sequences
   held by SEQ1 and SEQ2, also write the constant values encountered in
   little-endian format in LE_IMAGE and in big-endian format in BE_IMAGE.  */

static void
expand_init_to_cil_seq1 (tree decl, tree init, cil_seq seq1, bool cleared,
			 cil_seq seq2, char *le_image, char *be_image,
			 source_location sloc)
{
  tree decl_size = TYPE_SIZE_UNIT (TREE_TYPE (decl));
  unsigned HOST_WIDE_INT size = tree_low_cst (decl_size, 1);
  bool need_to_clear = FALSE;
  cil_stmt_iterator csi;

  if (TREE_CODE (init) == CONST_DECL)
    {
      init = DECL_INITIAL (init);
      gcc_assert (init && init != error_mark_node);
    }

  if (!cleared && TREE_CODE (init) == CONSTRUCTOR && all_zeros_p (init))
    {
      csi = csi_last (seq1);
      gen_initblk (&csi, build_fold_addr_expr (decl), integer_zero_node,
		   decl_size, sloc);
      return;
    }

  switch (TREE_CODE (init))
    {
    case STRING_CST:
      {
	csi = csi_last (seq1);

	if ((unsigned HOST_WIDE_INT) TREE_STRING_LENGTH (init) < size)
	  {
	    size = TREE_STRING_LENGTH (init);
	    decl_size = build_int_cst (TREE_TYPE (decl_size), size);

	    if (! cleared)
	      {
		gen_initblk (&csi, build_fold_addr_expr (decl),
			     integer_zero_node, decl_size, sloc);
	      }
	  }

	gen_cpblk (&csi, build_fold_addr_expr (decl),
		   build_fold_addr_expr (init), decl_size,
		   sloc);
	memcpy(le_image, TREE_STRING_POINTER (init), size);
	memcpy(be_image, TREE_STRING_POINTER (init), size);
      }
    break;

    case CONSTRUCTOR:
      switch (TREE_CODE (TREE_TYPE (init)))
	{
	case RECORD_TYPE:
	case UNION_TYPE:
	case QUAL_UNION_TYPE:
	  {
	    unsigned HOST_WIDE_INT idx;
	    tree init_type = TREE_TYPE (init);
	    tree field, value;

	    /* If size is zero or the target is already cleared, do nothing */
	    if (size == 0 || cleared)
	      {
		need_to_clear = FALSE;
		cleared = TRUE;
	      }
	    /* We either clear the aggregate or indicate the value is dead.  */
	    else if ((TREE_CODE (init_type) == UNION_TYPE
		      || TREE_CODE (init_type) == QUAL_UNION_TYPE)
		     && !CONSTRUCTOR_ELTS (init))
	      {
		/* If the constructor is empty, clear the union.  */
		need_to_clear = TRUE;
	      }
	    /* If the constructor has fewer fields than the structure or
	       if we are initializing the structure to mostly zeros, clear
	       the whole structure first. */
	    else if (size > 0
		     && (((int) VEC_length (constructor_elt,
					    CONSTRUCTOR_ELTS (init))
			  != fields_length (init_type))
			 || mostly_zeros_p (init)))
	      {
		need_to_clear = TRUE;
	      }

	    if (need_to_clear && size > 0)
	      {
		csi = csi_last (seq1);
		gen_initblk (&csi, build_fold_addr_expr (decl),
			     integer_zero_node, decl_size, sloc);
		cleared = TRUE;
	      }

	    /* Store each element of the constructor into the
	       corresponding field of TARGET.  */
	    FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
				      field, value)
	      {
		tree ltarget;

		/* Just ignore missing fields.  We cleared the whole
		   structure, above, if any fields are missing.  */
		if (field == 0)
		  continue;

		if (cleared && initializer_zerop (value))
		  continue;

		ltarget = build3 (COMPONENT_REF, TREE_TYPE (field), decl,
				  field, NULL);

		if (le_image != NULL && !DECL_BIT_FIELD (field))
		  {
		    unsigned HOST_WIDE_INT offset = tree_low_cst (DECL_FIELD_OFFSET (field), 1);
		    unsigned HOST_WIDE_INT bit_offset = tree_low_cst (DECL_FIELD_BIT_OFFSET (field), 1);
		    gcc_assert (bit_offset % BITS_PER_UNIT == 0);
		    offset += (bit_offset / BITS_PER_UNIT);

		    expand_init_to_cil_seq1 (ltarget, value, seq1, cleared,
					     seq2, le_image + offset,
					     be_image + offset, sloc);
		  }
		else if (le_image != NULL && DECL_BIT_FIELD (field)
			 && (TARGET_LITTLE_ENDIAN || TARGET_BIG_ENDIAN))
		  {
		    unsigned HOST_WIDE_INT offset = 0;
		    HOST_WIDE_INT bit_size = 0;
		    HOST_WIDE_INT bit_pos = 0;
		    HOST_WIDE_INT cont_off;
		    HOST_WIDE_INT cont_size = 8;
		    enum machine_mode mode;
		    int unsignedp = 0;
		    int volatilep = 0;
		    tree cont_type;
		    tree shift_cst;
		    tree tmp;

		    get_inner_reference (ltarget, &bit_size, &bit_pos,
					 &tmp, &mode, &unsignedp,
					 &volatilep, false);

		    /* Calculate the container size.  */
		    while ((bit_pos % cont_size + bit_size) > cont_size)
		      cont_size *= 2;

		    if (cont_size > 32)
		      {
			expand_init_to_cil_seq1 (ltarget, value,
						 seq1, cleared,
						 seq2, NULL, NULL, sloc);
		      }
		    else
		      {
			csi = csi_last (seq1);
			gen_modify_expr (&csi, ltarget, value, sloc);

			cont_type = get_integer_type (cont_size, true);
			cont_off = bit_pos % cont_size;

			/* Calculate the container offset.  */
			if ((bit_pos - cont_off) / BITS_PER_UNIT != 0)
			  {
			    tmp = build_int_cst (intSI_type_node,
						 (bit_pos - cont_off)
						 / BITS_PER_UNIT);
			    offset = tree_low_cst (tmp, 1);
			  }

			shift_cst = build_int_cst (intSI_type_node, cont_off);
			tmp = fold_binary_to_constant (LSHIFT_EXPR, cont_type,
						       fold_convert (cont_type,
								     value),
						       shift_cst);
			write_cst_image (le_image, be_image, tmp, cont_size);
		      }
		  }
		else
		  {
		    expand_init_to_cil_seq1 (ltarget, value, seq1, cleared,
					     seq2, NULL, NULL, sloc);
		  }
	      }
	  }
	  break;

	case ARRAY_TYPE:
	  {
	    tree value, index;
	    unsigned HOST_WIDE_INT i;
	    tree domain;
	    tree elttype = TREE_TYPE (TREE_TYPE (init));
	    int const_bounds_p;
	    HOST_WIDE_INT minelt = 0;
	    HOST_WIDE_INT maxelt = 0;

	    domain = TYPE_DOMAIN (TREE_TYPE (init));
	    const_bounds_p = (TYPE_MIN_VALUE (domain)
			      && TYPE_MAX_VALUE (domain)
			      && host_integerp (TYPE_MIN_VALUE (domain), 0)
			      && host_integerp (TYPE_MAX_VALUE (domain), 0));

	    /* If we have constant bounds for the range
	       of the type, get them.  */
	    if (const_bounds_p)
	      {
		minelt = tree_low_cst (TYPE_MIN_VALUE (domain), 0);
		maxelt = tree_low_cst (TYPE_MAX_VALUE (domain), 0);
	      }

	    /* If the constructor has fewer elements than the array, clear
	       the whole array first. */
	    if (cleared)
	      need_to_clear = FALSE;
	    else
	      {
		unsigned HOST_WIDE_INT idx;
		tree index, value;
		HOST_WIDE_INT count = 0;
		HOST_WIDE_INT zero_count = 0;
		need_to_clear = !const_bounds_p;

		/* This loop is a more accurate version of the loop in
		   mostly_zeros_p (it handles RANGE_EXPR in an index).  It
		   is also needed to check for missing elements.  */
		FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
					  index, value)
		  {
		    HOST_WIDE_INT this_node_count;

		    if (need_to_clear)
		      break;

		    if (index != NULL_TREE && TREE_CODE (index) == RANGE_EXPR)
		      {
			tree lo_index = TREE_OPERAND (index, 0);
			tree hi_index = TREE_OPERAND (index, 1);

			if (!host_integerp (lo_index, 1)
			    || !host_integerp (hi_index, 1))
			  {
			    need_to_clear = TRUE;
			    break;
			  }

			this_node_count = tree_low_cst (hi_index, 1)
					  - tree_low_cst (lo_index, 1) + 1;
		      }
		    else
		      this_node_count = 1;

		    count += this_node_count;
		    if (TREE_CODE (value) == CONSTRUCTOR
			&& mostly_zeros_p (value))
		      {
			zero_count += this_node_count;
		      }
		  }

		/* Clear the entire array first if there are any missing
		   elements, or if the incidence of zero elements is >= 75%.  */
		if (!need_to_clear
		    && (count < maxelt - minelt + 1
			|| 4 * zero_count >= 3 * count))
		  {
		    need_to_clear = TRUE;
		  }
	      }

	    if (need_to_clear && size > 0)
	      {
		csi = csi_last (seq1);
		gen_initblk (&csi, build_fold_addr_expr (decl),
			     integer_zero_node, decl_size, sloc);
		cleared = TRUE;
	      }

	    /* Store each element of the constructor into the
	       corresponding element of TARGET, determined by counting the
	       elements.  */
	    FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), i, index, value)
	      {
		tree t;
		tree elsize;

		if (initializer_zerop (value))
		  continue;

		gcc_assert (index == NULL_TREE
			    || TREE_CODE (index) != RANGE_EXPR);

		if (minelt)
		  index = fold_convert (ssizetype,
					fold_build2 (MINUS_EXPR,
						     TREE_TYPE (index),
						     index,
						     TYPE_MIN_VALUE (domain)));

		t = build4 (ARRAY_REF, elttype, decl, index, NULL, NULL);

		elsize = array_ref_element_size (t);

		if (le_image != NULL
		    && TREE_CODE (index)  == INTEGER_CST
		    && TREE_CODE (elsize) == INTEGER_CST)
		  {
		    unsigned HOST_WIDE_INT offset;

		    offset = tree_low_cst (index, 1) * tree_low_cst (elsize, 1);
		    expand_init_to_cil_seq1 (t, value, seq1, cleared, seq2,
					     le_image + offset,
					     be_image + offset, sloc);
		  }
		else
		  {
		    expand_init_to_cil_seq1 (t, value, seq1, cleared, seq2,
					     NULL, NULL, sloc);
		  }
	      }
	  }
	  break;

	case VECTOR_TYPE:
	  csi = csi_last (seq1);
	  gen_modify_expr (&csi, decl, init, sloc);
	  csi = csi_last (seq2);
	  gen_modify_expr (&csi, decl, init, sloc);
	  break;

	default:
	  gcc_unreachable ();
	  break;
	}
      break;

    case INTEGER_CST:
      {
	unsigned HOST_WIDE_INT type_size = tree_low_cst (decl_size, 1);
	tree t = fold_build2 (MODIFY_EXPR, TREE_TYPE (decl), decl, init);

	csi = csi_last (seq1);
	gimple_to_cil_node (&csi, t, sloc);

	if (le_image != NULL && type_size <= 4)
	  write_cst_image (le_image, be_image, init, type_size * BITS_PER_UNIT);
	else
	  {
	    csi = csi_last (seq2);
	    gimple_to_cil_node (&csi, t, sloc);
	  }
      }
      break;

    case REAL_CST:
      /* Missing optimization, fall through for now */
    default:
      csi = csi_last (seq1);
      gen_modify_expr (&csi, decl, init, sloc);
      csi = csi_last (seq2);
      gen_modify_expr (&csi, decl, init, sloc);
    }
}

static size_t
cil_seq_num_instr (cil_seq seq)
{
  cil_stmt_iterator csi;
  size_t i = 0;

  for (csi = csi_start (seq); !csi_end_p (csi); csi_next (&csi))
    i++;

  return i;
}

static bool
cil_choose_init_method (size_t n1, size_t n2, size_t size, bool same)
{
  size_t c1, c2;

  if (same)
    {
      c1 = n1 * 2;
      c2 = (n2 + 5) * 2 + size;
    }
  else
    {
      c1 = n1 * 2;
      c2 = (n2 + 8) * 2 + size * 2;
    }

  if (size < 100)
    {
      if (c1 < c2 / 2)
	return false;
      else
	return true;
    }
  else
    {
      if (c1 < c2 / 10)
	return false;
      else
	return true;
    }
}

cil_seq
expand_init_to_cil_seq (tree decl, tree init, source_location sloc)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE_UNIT (TREE_TYPE (decl)), 1);
  char *le_image = (char *) alloca (size);
  char *be_image = (char *) alloca (size);
  cil_stmt_iterator csi;
  cil_seq seq, seq1, seq2;
  cil_stmt stmt;
  unsigned int num_seq1, num_seq2;
  bool same;

  seq = cil_seq_alloc ();
  csi = csi_start (seq);

  if (size == 0)
    return seq;

  seq1 = cil_seq_alloc ();
  seq2 = cil_seq_alloc ();
  memset (le_image, 0, size);
  memset (be_image, 0, size);

  expand_init_to_cil_seq1 (decl, init, seq1, FALSE, seq2, le_image, be_image, sloc);

  num_seq1 = cil_seq_num_instr (seq1);
  num_seq2 = cil_seq_num_instr (seq2);
  same = (memcmp (le_image, be_image, size) == 0)
	 || TARGET_BIG_ENDIAN || TARGET_LITTLE_ENDIAN;

  /* Decide what to do */
  if (cil_choose_init_method (num_seq1, num_seq2, size, same))
    {
      tree sconst_le, sconst_be;

      gimple_to_cil_node (&csi, build_fold_addr_expr (decl), sloc);

      if (!same)
	{
	  sconst_le = build_string_literal (size, le_image);
	  gimple_to_cil_node (&csi, sconst_le, sloc);

	  sconst_be = build_string_literal (size, be_image);
	  gimple_to_cil_node (&csi, sconst_be, sloc);

	  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_ENDIAN_SELECT]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
	}
      else if (TARGET_BIG_ENDIAN)
	{
	  sconst_be = build_string_literal (size, be_image);
	  gimple_to_cil_node (&csi, sconst_be, sloc);
	}
      else /* TARGET_LITTLE_ENDIAN */
	{
	  sconst_le = build_string_literal (size, le_image);
	  gimple_to_cil_node (&csi, sconst_le, sloc);
	}

      gimple_to_cil_node (&csi, size_int (size), sloc);
      stmt = cil_build_stmt (CIL_CPBLK);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_unaligned (stmt, 1);
      csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
      csi_insert_seq_after (&csi, seq2, CSI_CONTINUE_LINKING);
    }
  else
    csi_insert_seq_after (&csi, seq1, CSI_CONTINUE_LINKING);

  cil_lower_init (seq);

  return seq;
}

/* Generates a GIMPLE_MODIFY_STMT, MODIFY_EXPR or INIT_EXPR with a bit field as
   its left hand side operand.  The left hand side operand is pointed by LHS
   abd the right hand side one by RHS.  The generated statements are appended
   to the current function's CIL code using the CSI iterator.  */

static void
gen_bit_field_modify_expr (cil_stmt_iterator *csi, tree lhs, tree rhs,
                           source_location sloc)
{
  HOST_WIDE_INT bit_size = 0;
  HOST_WIDE_INT bit_pos = 0;
  HOST_WIDE_INT cont_off;
  HOST_WIDE_INT cont_size = 8;
  tree offset = NULL_TREE;
  enum machine_mode mode;
  int unsignedp = 0;
  int volatilep = 0;
  cil_stmt stmt;
  tree cont_type;
  tree mask_cst, shift_cst;
  tree ref;
  tree folded_rhs, tmp;

  /* TODO: Add support for packed bit-fields crossing 64-bit boundaries.
     TODO: Add support for big-endian targets.  */

  /* Get the object base address and emit it.  */
  ref = get_inner_reference (lhs, &bit_size, &bit_pos, &offset, &mode,
			     &unsignedp, &volatilep, false);
  gen_addr_expr (csi, ref, sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Calculate the container size.  */
  while ((bit_pos % cont_size + bit_size) > cont_size)
    cont_size *= 2;

  cont_type = get_integer_type (cont_size, true);
  cont_off = bit_pos % cont_size;

  /* Calculate the container address if needed.  */
  if ((bit_pos - cont_off) / BITS_PER_UNIT != 0)
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node,
				      (bit_pos - cont_off) / BITS_PER_UNIT),
		       sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (offset != NULL_TREE)
    {
      gimple_to_cil_node (csi, offset, sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Duplicate the container address, we will need it later.  */
  stmt = cil_build_stmt (CIL_DUP);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Load the container.  */
  gen_scalar_ldind (csi, cont_type, volatilep, sloc);

  /* Compute the mask to be applied to the container.  */
  shift_cst = build_int_cst (intSI_type_node, cont_off);
  mask_cst = size_binop (LSHIFT_EXPR, build_int_cst (cont_type, 1),
			 build_int_cst (intSI_type_node, bit_size));
  mask_cst = size_binop (MINUS_EXPR, mask_cst, build_int_cst (cont_type, 1));
  mask_cst = size_binop (LSHIFT_EXPR, mask_cst, shift_cst);

  /* Apply the mask to the container.  */
  gen_integer_cst (csi, size_binop (BIT_XOR_EXPR, mask_cst,
				    build_int_cst (cont_type, -1)),
		   sloc);
  stmt = cil_build_stmt (CIL_AND);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Put the new value on the stack. If the rhs is a constant fold the
     shift & mask operations, if it is not copy it and convert it in the
     container type */
  folded_rhs = fold_binary_to_constant (LSHIFT_EXPR, cont_type,
					fold_convert (cont_type, rhs),
					shift_cst);

  if (folded_rhs != NULL_TREE)
    {
      folded_rhs = fold_binary_to_constant (BIT_AND_EXPR, cont_type,
					    fold_convert (cont_type,
							  folded_rhs),
					    mask_cst);
    }

  if (folded_rhs != NULL_TREE)
    {
      if (!integer_zerop (folded_rhs))
	gimple_to_cil_node (csi, folded_rhs, sloc);
    }
  else
    {
      tmp = rhs;

      /* Strip redundant conversions.  */
      while (TREE_CODE (tmp) == NOP_EXPR && INTEGRAL_TYPE_P (TREE_TYPE (tmp)))
	tmp = TREE_OPERAND (tmp, 0);

      gimple_to_cil_node (csi, tmp, sloc);

      if (TYPE_PRECISION (TREE_TYPE (tmp)) > 32 && cont_size <= 32)
	{
	  stmt = cil_build_stmt (CIL_CONV_U4);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      else if (TYPE_PRECISION (TREE_TYPE (tmp)) <= 32 && cont_size > 32)
	{
	  stmt = cil_build_stmt (CIL_CONV_U8);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      if (!integer_zerop (shift_cst))
	{
	  gen_integer_cst (csi, shift_cst, sloc);
	  stmt = cil_build_stmt (CIL_SHL);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      if (cont_off + bit_size != cont_size)
	{
	  gen_integer_cst (csi, mask_cst, sloc);
	  stmt = cil_build_stmt (CIL_AND);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }

  if (folded_rhs == NULL_TREE || !integer_zerop (folded_rhs))
    {
      /* Insert the new value inside the container.  */
      stmt = cil_build_stmt (CIL_OR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Store the container in memory.  */
  gen_scalar_stind (csi, cont_type, volatilep, sloc);
}

/* Generates a MODIFY_EXPR using a TARGET_MEM_REF node as its LHS operand.  */

static void
gen_target_mem_ref_modify_expr (cil_stmt_iterator *csi, tree lhs, tree rhs,
                                source_location sloc)
{
  tree type = TREE_TYPE (lhs);
  tree ptr_type = build_pointer_type (type);

  gimple_to_cil_node (csi, tree_mem_ref_addr (ptr_type, lhs), sloc);
  gimple_to_cil_node (csi, rhs, sloc);
  gen_stind (csi, type, TREE_THIS_VOLATILE (lhs), sloc);
}

/* Generates a MODIFY_EXPR using a BIT_FIELD_REF scalar access as its LHS
   operand.  */

static void
gen_bitfield_ref_modify_expr (cil_stmt_iterator *csi, tree lhs, tree rhs,
                              source_location sloc)
{
  HOST_WIDE_INT bit_size = 0;
  HOST_WIDE_INT bit_pos = 0;
  tree offset = NULL_TREE;
  enum machine_mode mode;
  int unsignedp = 0;
  int volatilep = 0;
  cil_stmt stmt;
  tree ref;

  /* Get the object base address and emit it.  */
  ref = get_inner_reference (lhs, &bit_size, &bit_pos, &offset, &mode,
			     &unsignedp, &volatilep, false);
  gen_addr_expr (csi, ref, sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Calculate the container address if needed.  */
  if (bit_pos / BITS_PER_UNIT != 0)
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node,
				      bit_pos / BITS_PER_UNIT),
		       sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (offset != NULL_TREE)
    {
      gimple_to_cil_node (csi, offset, sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Put the new value on the stack and store the field.  */
  gimple_to_cil_node (csi, rhs, sloc);
  gen_scalar_stind (csi, TREE_TYPE (rhs), volatilep, sloc);
}

/* Generates a MODIFY_EXPR using a BIT_FIELD_REF scalar-element vector access
   as its LHS operand.  */

static void
gen_vector_bitfield_ref_modify_expr (cil_stmt_iterator *csi, tree lhs,
                                     tree rhs, source_location sloc)
{
  cil_stmt stmt;
  tree cst;

  gen_addr_expr (csi, TREE_OPERAND (lhs, 0), sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  cst = size_binop (TRUNC_DIV_EXPR, TREE_OPERAND (lhs, 2), bitsize_unit_node);

  if (!integer_zerop (cst))
    {
      gen_integer_cst (csi, fold_convert (intSI_type_node, cst), sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  gimple_to_cil_node (csi, rhs, sloc);
  gen_stind (csi, TREE_TYPE (rhs), TREE_THIS_VOLATILE (lhs), sloc);
}

/* Converts a GIMPLE_MODIFY_STMT, MODIFY_EXPR or INIT_EXPR into the CIL form
   eventually expanding the arguments if they cannot be converted directly. The
   left hand side operand is pointed by LHS and the right hand side one by RHS.
   The generated statements are appended to the current function's CIL code
   using the CSI iterator.  */

static void
gen_modify_expr (cil_stmt_iterator *csi, tree lhs, tree rhs, source_location sloc)
{
  cil_stmt stmt;

  switch (TREE_CODE (lhs))
    {
    case VAR_DECL:
    case RESULT_DECL:
      mark_referenced_type (TREE_TYPE (lhs));

      if (!DECL_FILE_SCOPE_P (lhs) && !TREE_STATIC (lhs))
	{
	  if (TREE_THIS_VOLATILE (lhs))
	    {
	      /* put the address of the loc on the stack */
	      stmt = cil_build_stmt_arg (CIL_LDLOCA, lhs);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	      /* put the value on the stack */
	      gimple_to_cil_node (csi, rhs, sloc);
	      /* and emit a volatile stind or stobj */
	      gen_stind (csi, TREE_TYPE (lhs), true, sloc);
	    }
	  else
	    {
	      /* put the value on the stack */
	      gimple_to_cil_node (csi, rhs, sloc);
	      stmt = cil_build_stmt_arg (CIL_STLOC, lhs);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    }
	}
      else
	{
	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt_arg (CIL_STSFLD, lhs);
	  cil_set_locus(stmt, sloc);
	  cil_set_prefix_volatile (stmt, TREE_THIS_VOLATILE (lhs));
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      break;

    case PARM_DECL:
      gimple_to_cil_node (csi, rhs, sloc);
      stmt = cil_build_stmt_arg (CIL_STARG, lhs);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case ARRAY_REF:
    case INDIRECT_REF:
      gen_addr_expr (csi, lhs, sloc);
      gimple_to_cil_node (csi, rhs, sloc);
      gen_stind (csi, TREE_TYPE (rhs), TREE_THIS_VOLATILE (lhs), sloc);
      break;

    case MISALIGNED_INDIRECT_REF:
      gen_addr_expr (csi, lhs, sloc);
      gimple_to_cil_node (csi, rhs, sloc);
      gen_misaligned_stvec (csi, TREE_TYPE (rhs), TREE_THIS_VOLATILE (lhs), sloc);
      break;

    case COMPONENT_REF:
      {
	tree obj = TREE_OPERAND (lhs, 0);
	tree fld = TREE_OPERAND (lhs, 1);

	mark_referenced_type (TYPE_MAIN_VARIANT (TREE_TYPE (obj)));

	if (DECL_BIT_FIELD (fld))
	  gen_bit_field_modify_expr (csi, lhs, rhs, sloc);
	else
	  {
	    /* put the value on the stack */
	    gen_addr_expr (csi, obj, sloc);
	    gimple_to_cil_node (csi, rhs, sloc);
	    stmt = cil_build_stmt_arg (CIL_STFLD, fld);

	    cil_set_locus(stmt, sloc);

	    if (contains_packed_reference (lhs))
	      cil_set_prefix_unaligned (stmt, 1);

	    cil_set_prefix_volatile (stmt, TREE_THIS_VOLATILE (lhs));
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  }
      }
      break;

    case TARGET_MEM_REF:
      gen_target_mem_ref_modify_expr (csi, lhs, rhs, sloc);
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      gen_addr_expr (csi, lhs, sloc);
      gimple_to_cil_node (csi, rhs, sloc);
      gen_scalar_stind (csi, TREE_TYPE (lhs), TREE_THIS_VOLATILE (lhs), sloc);
      break;

    case BIT_FIELD_REF:
      if (TREE_CODE (TREE_TYPE (TREE_OPERAND (lhs, 0))) == VECTOR_TYPE)
	gen_vector_bitfield_ref_modify_expr (csi, lhs, rhs, sloc);
      else
	gen_bitfield_ref_modify_expr (csi, lhs, rhs, sloc);

      break;

    default:
      gcc_unreachable ();
    }
}

/* Generates a GOTO_EXPR including the emulation needed for computed GOTOs.  */

static void
gen_goto_expr (cil_stmt_iterator *csi, tree node)
{
  tree label_decl = GOTO_DESTINATION (node);
  cil_stmt stmt;
  source_location sloc = source_location_from_tree (node);

  if (TREE_CODE (label_decl) != LABEL_DECL)
    {
      /* This is a goto to the address of a label. Labels have
	 been numbered, and we emit a switch based on that ID.  */
      gimple_to_cil_node (csi, label_decl, sloc);
      stmt = cil_build_switch (get_label_addrs());
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else
    {
      basic_block dest_bb = label_to_block (label_decl);

      if (csi_bb (*csi)->next_bb != dest_bb)
	{
	  stmt = cil_build_stmt_arg (CIL_BR, label_decl);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
}

/* Generates a conditional expression.  */

static void
gen_cond_expr (cil_stmt_iterator *csi, tree node)
{
  edge true_edge, false_edge;
  tree label_then, label_else, cond, lhs, rhs, type;
  basic_block dest_bb;
  cil_stmt stmt;
  enum cil_opcode opcode;
  bool uns;
  source_location sloc = source_location_from_tree (node);

  extract_true_false_edges_from_block (csi_bb (*csi), &true_edge, &false_edge);
  label_then = gimple_block_label (true_edge->dest);
  label_else = gimple_block_label (false_edge->dest);

  cond = COND_EXPR_COND (node);

  if (DECL_P (cond))
    {
      gimple_to_cil_node (csi, cond, sloc);
      gimple_to_cil_node (csi,
			  fold_convert (TREE_TYPE (cond), integer_zero_node),
			  sloc);
      stmt = cil_build_stmt_arg (CIL_BNE_UN, label_then);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else
    {
      lhs = TREE_OPERAND (cond, 0);
      rhs = TREE_OPERAND (cond, 1);
      type = TREE_TYPE (lhs);

      switch (TREE_CODE (cond))
	{
	case EQ_EXPR:
	case NE_EXPR:
	  if (tree_int_cst_equal (lhs, integer_zero_node)
	      && (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type)))
	    {
	      opcode = TREE_CODE (cond) == EQ_EXPR ? CIL_BRFALSE : CIL_BRTRUE;
	      gimple_to_cil_node (csi, rhs, sloc);
	      stmt = cil_build_stmt_arg (opcode, label_then);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    }
	  else if (tree_int_cst_equal (rhs, integer_zero_node)
		   && (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type)))
	    {
	      opcode = TREE_CODE (cond) == EQ_EXPR ? CIL_BRFALSE : CIL_BRTRUE;
	      gimple_to_cil_node (csi, lhs, sloc);
	      stmt = cil_build_stmt_arg (opcode, label_then);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    }
	  else
	    {
	      opcode = TREE_CODE (cond) == EQ_EXPR ? CIL_BEQ : CIL_BNE_UN;
	      gimple_to_cil_node (csi, lhs, sloc);
	      gimple_to_cil_node (csi, rhs, sloc);
	      stmt = cil_build_stmt_arg (opcode, label_then);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    }

	  break;

	case LE_EXPR:
	case LT_EXPR:
	case GE_EXPR:
	case GT_EXPR:
	case UNLT_EXPR:
	case UNLE_EXPR:
	case UNGT_EXPR:
	case UNGE_EXPR:
	  uns = TYPE_UNSIGNED (type);
	  gimple_to_cil_node (csi, lhs, sloc);
	  gimple_to_cil_node (csi, rhs, sloc);

	  switch (TREE_CODE (cond))
	    {
	    case LE_EXPR:   opcode = uns ? CIL_BLE_UN : CIL_BLE; break;
	    case LT_EXPR:   opcode = uns ? CIL_BLT_UN : CIL_BLT; break;
	    case GE_EXPR:   opcode = uns ? CIL_BGE_UN : CIL_BGE; break;
	    case GT_EXPR:   opcode = uns ? CIL_BGT_UN : CIL_BGT; break;
	    case UNLT_EXPR: opcode = CIL_BLT_UN;                 break;
	    case UNLE_EXPR: opcode = CIL_BLE_UN;                 break;
	    case UNGT_EXPR: opcode = CIL_BGT_UN;                 break;
	    case UNGE_EXPR: opcode = CIL_BGE_UN;                 break;
	    default:
	      gcc_unreachable ();
	    }

	  stmt = cil_build_stmt_arg (opcode, label_then);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  break;

	case UNORDERED_EXPR:
	case ORDERED_EXPR:
	  gimple_to_cil_node (csi, lhs, sloc);
	  stmt = cil_build_stmt (CIL_DUP);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt (CIL_DUP);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  stmt = cil_build_stmt (CIL_AND);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  opcode = (TREE_CODE (cond) == ORDERED_EXPR) ? CIL_BRTRUE : CIL_BRFALSE;
	  stmt = cil_build_stmt_arg (opcode, label_then);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  break;

	case UNEQ_EXPR:
	  lhs = gen_expr_copy (csi, lhs, sloc);
	  rhs = gen_expr_copy (csi, rhs, sloc);

	  /* Emit the equivalent of an UNORDERED_EXPR ...  */
	  gimple_to_cil_node (csi, lhs, sloc);
	  stmt = cil_build_stmt (CIL_DUP);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt (CIL_DUP);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_AND);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  gen_integer_cst (csi, integer_one_node, sloc);
	  stmt = cil_build_stmt (CIL_XOR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  /* ... plus an equal comparison.  */
	  gimple_to_cil_node (csi, lhs, sloc);
	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_OR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt_arg (CIL_BRTRUE, label_then);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  break;

	case LTGT_EXPR:
	  lhs = gen_expr_copy (csi, lhs, sloc);
	  rhs = gen_expr_copy (csi, rhs, sloc);

	  gimple_to_cil_node (csi, lhs, sloc);
	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt (CIL_CGT);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  gimple_to_cil_node (csi, lhs, sloc);
	  gimple_to_cil_node (csi, rhs, sloc);
	  stmt = cil_build_stmt (CIL_CLT);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_OR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt_arg (CIL_BRTRUE, label_then);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  break;

	default:
	  gimple_to_cil_node (csi, cond, sloc);
	  stmt = cil_build_stmt_arg (CIL_BRTRUE, label_then);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }

  /* TODO: Emit JIT compilation hints
  if (TARGET_EMIT_JIT_COMPILATION_HINTS)
    branch_probability_add (file, node); */

  dest_bb = label_to_block (label_else);

  /* Emit else block only if it is not a fallthrough */
  if (csi_bb (*csi)->next_bb != dest_bb)
    {
      stmt = cil_build_stmt_arg (CIL_BR, label_else);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Emit a switch expression.  */

static void
gen_switch_expr (cil_stmt_iterator *csi, tree node)
{
  tree labels = SWITCH_LABELS (node);
  tree min = TREE_VEC_ELT (labels, 0);
  unsigned int length = TREE_VEC_LENGTH (labels);
  tree default_label = CASE_LABEL (TREE_VEC_ELT (labels, length - 1));
  basic_block dest_bb;
  cil_stmt stmt;
  source_location sloc = source_location_from_tree(node);

  /* Generate the switch condition.  */
  gimple_to_cil_node (csi, SWITCH_COND (node), sloc);

  /* 'Normalize' the condition.  */
  if (!tree_int_cst_equal (CASE_LOW (min), integer_zero_node))
    {
      gen_integer_cst (csi, CASE_LOW (min), sloc);
      stmt = cil_build_stmt (CIL_SUB);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Generate the switch and the default label fall thru.  */
  stmt = cil_build_switch (labels);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  dest_bb = label_to_block (default_label);

  if (csi_bb (*csi)->next_bb != dest_bb)
    {
      stmt = cil_build_stmt_arg (CIL_BR, default_label);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}


/* Generate the CIL code associated with the __builtin_va_start() call
   specified by NODE. The generated CIL statements will be appended to CSI.  */

static void
gen_builtin_va_start (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree argiter = create_tmp_var (cil32_arg_iterator_type, "arg_iterator");
  cil_stmt stmt;

  stmt = cil_build_stmt_arg (CIL_LDLOCA, argiter);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_stmt (CIL_DUP);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_VA_INIT]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_VA_START]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* FIXME: The extra indirection step may be optimized out in the common case
     or removed later using a peephole optimization.  */
  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
  stmt = cil_build_stmt_arg (CIL_LDLOCA, argiter);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_stmt (CIL_STIND_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generate the CIL code associated with the __builtin_va_end() call
   specified by NODE. The generated CIL statements will be appended to CSI.  */

static void
gen_builtin_va_end (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;

  /* FIXME: The extra indirection step may be optimized out in the common case
     or removed later using a peephole optimization.  */
  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
  stmt = cil_build_stmt (CIL_LDIND_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_VA_END]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generate the CIL code associated with the __builtin_va_copy() call
   specified by NODE. The generated CIL statements will be appended to CSI.  */

static void
gen_builtin_va_copy (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree argiter = create_tmp_var (cil32_arg_iterator_type, "arg_iterator");
  cil_stmt stmt;

  stmt = cil_build_stmt_arg (CIL_LDLOCA, argiter);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_stmt (CIL_DUP);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_VA_INIT]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* FIXME: The extra indirection step may be optimized out in the common case
     or removed later using a peephole optimization.  */
  /* Load the source argument iterator.  */
  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
  stmt = cil_build_stmt_arg (CIL_LDLOCA, argiter);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  stmt = cil_build_stmt (CIL_STIND_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Load the destination argument iterator.*/
  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 1), sloc);

  /* Make the copy.  */
  stmt = cil_build_call (cil32_builtins[CIL32_BUILT_IN_VA_COPY]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Inspired from 'expand_builtin_object_size' in builtins.c. We return -1
   for types 0 and 1, and 0 for types 2 and 3.  */

static void
gen_builtin_object_size (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree arg2 = CALL_EXPR_ARG (node, 1);
  int obj_type;

  STRIP_NOPS (arg2);
  gcc_assert (TREE_CODE (arg2) == INTEGER_CST);
  obj_type = tree_low_cst (arg2, 0);

  switch (obj_type)
    {
    case 0:
    case 1:
      gen_integer_cst (csi, integer_zero_node, sloc);
      break;

    case 2:
    case 3:
      gen_integer_cst (csi, integer_minus_one_node, sloc);
      break;

    default:
      gcc_unreachable ();
    }
}

/* Try to handle a builtin call. In some cases this function will expand the
   builtin and return true, this indicates that the call has been effectively
   removed and no other action is required, otherwise false will be
   returned. The current CALL_EXPR is passed in NODE and the function
   declaration in FDECL. If the builtin is expanded the generated CIL
   statements will be appended to CSI.  */

static bool
gen_call_builtin (cil_stmt_iterator *csi, tree node, tree fdecl)
{
  cil_stmt stmt;
  source_location sloc = source_location_from_tree(node);

  if (DECL_BUILT_IN_CLASS (fdecl) != BUILT_IN_MD)
    {
      switch (DECL_FUNCTION_CODE (fdecl))
	{
	case BUILT_IN_VA_START:
	  gen_builtin_va_start (csi, node, sloc);
	  return true;

	case BUILT_IN_VA_END:
	  gen_builtin_va_end (csi, node, sloc);
	  return true;

	case BUILT_IN_VA_COPY:
	  gen_builtin_va_copy (csi, node, sloc);
	  return true;

	case BUILT_IN_CLZ:
	case BUILT_IN_CLZL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_CLZSI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_CLZLL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_CLZDI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_CTZ:
	case BUILT_IN_CTZL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_CTZSI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_CTZLL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_CTZDI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_FFS:
	case BUILT_IN_FFSL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_FFSSI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_FFSLL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_FFSDI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_PARITY:
	case BUILT_IN_PARITYL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_PARITYSI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_PARITYLL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_PARITYDI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_POPCOUNT:
	case BUILT_IN_POPCOUNTL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_POPCOUNTSI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_POPCOUNTLL:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (cil32_builtins[CIL32_POPCOUNTDI2]);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_OBJECT_SIZE:
	  gen_builtin_object_size (csi, node, sloc);
	  return true;

	case BUILT_IN_INIT_TRAMPOLINE:
	case BUILT_IN_ADJUST_TRAMPOLINE:
	case BUILT_IN_NONLOCAL_GOTO:
	  internal_error ("Builtins to support Trampolines not implemented\n");

	case BUILT_IN_PROFILE_FUNC_ENTER:
	case BUILT_IN_PROFILE_FUNC_EXIT:
	  internal_error ("Builtins to support Profiling not implemented\n");

	case BUILT_IN_SETJMP_SETUP:
	case BUILT_IN_SETJMP_DISPATCHER:
	case BUILT_IN_SETJMP_RECEIVER:
	  internal_error ("Builtins to support Setjump not implemented\n");

	case BUILT_IN_MEMSET:
	  {
	    tree ptr   = CALL_EXPR_ARG (node, 0);
	    tree value = CALL_EXPR_ARG (node, 1);
	    tree size  = CALL_EXPR_ARG (node, 2);

	    gimple_to_cil_node (csi, ptr, sloc);
	    stmt = cil_build_stmt (CIL_DUP);
	    cil_set_locus(stmt, sloc);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    gimple_to_cil_node (csi, value, sloc);
	    gimple_to_cil_node (csi, size, sloc);
	    stmt = cil_build_stmt (CIL_INITBLK);
	    cil_set_locus(stmt, sloc);
	    cil_set_prefix_unaligned (stmt, 1);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    return true;
	  }

	case BUILT_IN_MEMCPY:
	  {
	    tree ptr_dst = CALL_EXPR_ARG (node, 0);
	    tree ptr_src = CALL_EXPR_ARG (node, 1);
	    tree size    = CALL_EXPR_ARG (node, 2);

	    gimple_to_cil_node (csi, ptr_dst, sloc);
	    stmt = cil_build_stmt (CIL_DUP);
	    cil_set_locus(stmt, sloc);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    gimple_to_cil_node (csi, ptr_src, sloc);
	    gimple_to_cil_node (csi, size, sloc);
	    stmt = cil_build_stmt (CIL_CPBLK);
	    cil_set_locus(stmt, sloc);
	    cil_set_prefix_unaligned (stmt, 1);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    return true;
	  }

	case BUILT_IN_ALLOCA:
	  {
	    tree size = CALL_EXPR_ARG (node, 0);

	    gimple_to_cil_node (csi, size, sloc);
	    stmt = cil_build_stmt (CIL_LOCALLOC);
	    cil_set_locus(stmt, sloc);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    return true;
	  }

	case BUILT_IN_STACK_SAVE:
	  /* FIXME: This built-in is only used for the implementation
	     of variable-length arrays.  It is not needed in CIL.  */
	  gen_integer_cst (csi, integer_zero_node, sloc);
	  stmt = cil_build_stmt (CIL_CONV_I);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case BUILT_IN_STACK_RESTORE:
	  /* FIXME: This built-in is only used for the implementation
	     of variable-length arrays.  It is not needed in CIL.  */
	  return true;

	case BUILT_IN_EXPECT:
	  /* TODO: __builtin_expect(exp,val) evalutes exp and tells the
	     compiler that it most likely gives val.  We just evaluate exp
	     but we could flag it for JIT hints emission.  */
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  return true;

	case BUILT_IN_PREFETCH:
	  if (TREE_SIDE_EFFECTS (CALL_EXPR_ARG (node, 0)))
	    gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);

	  return true;

	case BUILT_IN_FRAME_ADDRESS:
	case BUILT_IN_RETURN_ADDRESS:
	  {
	    /* Supported (sort of) only for non-zero parameter, when it is ok
	       to return NULL.  */
	    tree arg = CALL_EXPR_ARG (node, 0);
	    int  int_arg = tree_low_cst (arg, 0);

	    if (int_arg == 0)
	      internal_error ("__builtin_{return,frame}_address not implemented\n");
	    else
	      gen_integer_cst (csi, integer_zero_node, sloc);
	  }
	  return true;

	case BUILT_IN_BZERO:
	  {
	    tree ptr   = CALL_EXPR_ARG (node, 0);
	    tree size  = CALL_EXPR_ARG (node, 1);

	    gen_initblk (csi, ptr, integer_zero_node, size, sloc);
	    return true;
	  }

	case BUILT_IN_BCOPY:
	  {
	    tree ptr_src = CALL_EXPR_ARG (node, 0);
	    tree ptr_dst = CALL_EXPR_ARG (node, 1);
	    tree size    = CALL_EXPR_ARG (node, 2);

	    gen_cpblk (csi, ptr_dst, ptr_src, size, sloc);
	    return true;
	  }

	default:
	  if (DECL_ASSEMBLER_NAME_SET_P (node))
	    {
	       /* Go ahead as a normal function call */
	    }
	}
    }
  else
    {
      switch (DECL_FUNCTION_CODE (fdecl))
	{
	case CIL32_BUILT_IN_VA_ARG:
	  gimple_to_cil_node (csi, CALL_EXPR_ARG (node, 0), sloc);
	  stmt = cil_build_call (fdecl);
	  cil_set_locus(stmt, sloc);
	  cil_call_set_dummy_arg (stmt, CALL_EXPR_ARG (node, 1));
	  /* We 'patch' the generated call statement so as to make it behave as
	     if it had been passed a single argument.  */
	  stmt->arg.fcall->nargs = 1;
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  return true;

	case CIL32_BUILT_IN_CPBLK:
	  {
	    tree ptr_dst = CALL_EXPR_ARG (node, 0);
	    tree ptr_src = CALL_EXPR_ARG (node, 1);
	    tree size    = CALL_EXPR_ARG (node, 2);

	    gen_cpblk (csi, ptr_dst, ptr_src, size, sloc);
	    return true;
	  }

	case CIL32_BUILT_IN_INITBLK:
	  {
	    tree ptr   = CALL_EXPR_ARG (node, 0);
	    tree value = CALL_EXPR_ARG (node, 1);
	    tree size  = CALL_EXPR_ARG (node, 2);

	    gen_initblk (csi, ptr, value, size, sloc);
	    return true;
	  }

	default:
	  ;
	}
    }

  return false;
}

/* Generates a function call from a CALL_EXPR gimple node NODE.  The generated
   statements are appended to the current function's CIL code using the CSI
   iterator.  */

static void
gen_call_expr (cil_stmt_iterator *csi, tree node)
{
  tree fdecl;
  tree ftype;
  tree arg_types, arg_type;
  VEC(tree, heap) *arglist;
  tree static_chain;
  bool direct = true;
  bool varargs = false;
  bool missing = false;
  size_t nargs_base;
  size_t nargs;
  size_t max_nargs;
  size_t i;
  cil_stmt stmt;
  source_location sloc;

  gcc_assert (TREE_CODE (node) == CALL_EXPR);
  fdecl = get_callee_fndecl (node);
  nargs = call_expr_nargs (node);
  max_nargs = nargs;
  sloc = source_location_from_tree(node);

  if (fdecl != NULL_TREE)
    ftype = TREE_TYPE (fdecl);
  else
    {
      ftype = TREE_TYPE (TREE_TYPE (CALL_EXPR_FN (node)));
      direct = false;
    }

  /* Built-in functions must be handled in a special way */
  if (direct && DECL_BUILT_IN (fdecl))
    {
      if (gen_call_builtin (csi, node, fdecl))
	return;
    }

  arg_types = TYPE_ARG_TYPES (ftype);

  /* Before guessing the prototype, try to fix it */
  if (arg_types == NULL && fdecl)
    {
      fix_missing_prototype (fdecl);
      /* fix_missing_prototype may update function type */
      ftype = TREE_TYPE (fdecl);
      arg_types = TYPE_ARG_TYPES (ftype);
    }

  if (arg_types == NULL)
    {
      if (direct)
	{
	  if (TREE_ASM_WRITTEN (fdecl))
	    {
	      max_nargs = list_length (DECL_ARGUMENTS (fdecl));
	      if (max_nargs < nargs)
		{
		  warning (OPT_Wcil_missing_prototypes,
			   "Too many arguments calling %s, ignoring extra ones",
                           IDENTIFIER_POINTER (DECL_NAME (fdecl)));
		}
	      else if (max_nargs > nargs)
                {
                  internal_error ("Less arguments than needed in call to %s, "
                                  "fix your code",
                                  IDENTIFIER_POINTER (DECL_NAME (fdecl)));
                }
	    }
          else
            {
              warning (OPT_Wcil_missing_prototypes,
                       "Missing function %s prototype, guessing it, "
                       "you should fix the code",
                       IDENTIFIER_POINTER (DECL_NAME (fdecl)));
            }
	}
      else
	{
	  warning (OPT_Wcil_missing_prototypes,
		   "Missing indirect function prototype, guessing it, you "
		   "should fix the code");
	}

      /* Guess types using the type of the arguments.  */
      nargs_base = 0;
      missing = true;
    }
  else
    {
      tree last_arg_type = tree_last (arg_types);
      nargs_base = list_length (arg_types);

      if (TREE_VALUE (last_arg_type) != void_type_node)
	varargs = true;
      else
	nargs_base--;
    }

  arglist = VEC_alloc (tree, heap, max_nargs - nargs_base);

  /* If static chain present, it will be the first argument.  */
  static_chain = CALL_EXPR_STATIC_CHAIN (node);

  if (static_chain)
    gimple_to_cil_node (csi, static_chain, sloc);

  /* Arguments used in functions with automatically generated prototypes
     may lack the casting (since the type was unknown during gimplification)
     We provide it just before emiting the argument */
  arg_type = arg_types;
  for (i = 0; i < nargs_base; i++)
    {
      tree arg = CALL_EXPR_ARG (node, i);
      
      if (arg_type)
        {
          /* FIXME: This is not gimple! Anyway, we are about to remove the gimple tree */
          if (!useless_type_conversion_p (TREE_VALUE (arg_type), TREE_TYPE (arg)))
            arg = CALL_EXPR_ARG (node, i) = fold_convert (TREE_VALUE (arg_type), CALL_EXPR_ARG (node, i));
          arg_type =  TREE_CHAIN (arg_type);
        }
    gimple_to_cil_node (csi, arg, sloc);
  }

  /* Vararg parameters, this will be added only if they are present.  */
  for (; i < nargs; i++)
    {
      tree arg = CALL_EXPR_ARG (node, i);
      tree arg_type = TREE_TYPE (arg);

      gimple_to_cil_node (csi, arg, sloc);
      if (i < max_nargs)
	{
	  VEC_quick_insert (tree, arglist, i - nargs_base, arg_type);
	  if (TREE_CODE (arg_type) == POINTER_TYPE
	      || (TREE_CODE (arg_type) == ARRAY_TYPE
		  && (!TYPE_DOMAIN (arg_type)
		      || ARRAY_TYPE_VARLENGTH (arg_type))))
	    {
	      stmt = cil_build_stmt (CIL_CONV_I);
	      cil_set_locus(stmt, sloc);
	      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	    }
	}
      else
	{
	  stmt = cil_build_stmt (CIL_POP);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }

  /* TODO: We could do return slot optimizations, or insertion of the tail
     call prefix here? */
  if (!direct)
    {
      /* Generate the function pointer, in case of an indirect call.  */
      gimple_to_cil_node (csi, CALL_EXPR_FN (node), sloc);

      if (varargs)
	stmt = cil_build_calli_va (ftype, arglist);
      else if (missing)
	stmt = cil_build_calli_mp (ftype, arglist);
      else
	stmt = cil_build_calli (ftype);

      cil_set_locus(stmt, sloc);
    }
  else
    {
      if (varargs)
	stmt = cil_build_call_va (fdecl, arglist);
      else if (missing)
	stmt = cil_build_call_mp (fdecl, arglist);
      else
	stmt = cil_build_call (fdecl);

      cil_set_locus(stmt, sloc);
    }

  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  if (static_chain)
    cil_call_set_static_chain (stmt, TREE_TYPE (static_chain));

  if (arglist != NULL)
    VEC_free (tree, heap, arglist);
}

/* Generates a copy inside a temporary variable of the expression NODE if it is
   necessary or beneficial. Returns the new variable holding the copy or the
   original expression if it wasn't copied.  */

static tree
gen_expr_copy (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  enum tree_code code = TREE_CODE (node);
  cil_stmt stmt;
  tree tmp;

  if (!TREE_SIDE_EFFECTS (node)
      && ((code == INTEGER_CST) || (code == REAL_CST)
	  || (code == VAR_DECL) || (code == PARM_DECL)))
    {
      return node;
    }

  tmp = create_tmp_var (TREE_TYPE (node), "gimple2cil");
  gimple_to_cil_node (csi, node, sloc);
  stmt = cil_build_stmt_arg (CIL_STLOC, tmp);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  return tmp;
}

/* Generates a BIT_AND_EXPR potentially folding it into a conversion in order
   to minimize code size.  */

static void
gen_bit_and_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  enum cil_opcode opcode;
  cil_stmt stmt;
  tree op0 = TREE_OPERAND (node, 0);
  tree op1 = TREE_OPERAND (node, 1);

  if (TREE_CODE (op0) == INTEGER_CST
      && (TREE_INT_CST_LOW (op0) == 255U
	  || TREE_INT_CST_LOW (op0) == 65535U
	  || TREE_INT_CST_LOW (op0) == 4294967295U)
      && TREE_INT_CST_HIGH (op0) == 0)
    {
      gimple_to_cil_node (csi, op1, sloc);

      switch (TREE_INT_CST_LOW (op0))
	{
	case 255U:        opcode = CIL_CONV_U1; break;
	case 65535U:      opcode = CIL_CONV_U2; break;
	case 4294967295U: opcode = CIL_CONV_U4; break;
	default:
	  gcc_unreachable ();
	}

      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
	{
	  stmt = cil_build_stmt (CIL_CONV_U8);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
  else if (TREE_CODE (op1) == INTEGER_CST
	   && (TREE_INT_CST_LOW (op1) == 255U
	       || TREE_INT_CST_LOW (op1) == 65535U
	       || TREE_INT_CST_LOW (op1) == 4294967295U)
	   && TREE_INT_CST_HIGH (op1) == 0)
    {
      gimple_to_cil_node (csi, op0, sloc);

      switch (TREE_INT_CST_LOW (op1))
	{
	case 255U:        opcode = CIL_CONV_U1; break;
	case 65535U:      opcode = CIL_CONV_U2; break;
	case 4294967295U: opcode = CIL_CONV_U4; break;
	default:
	  gcc_unreachable ();
	}

    stmt = cil_build_stmt (opcode);
    cil_set_locus(stmt, sloc);
    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

    if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
      {
	stmt = cil_build_stmt (CIL_CONV_U8);
	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      }
    }
  else
    {
      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);
      stmt = cil_build_stmt (CIL_AND);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* No need for conversions even in case of values with precision
     smaller than the one used on the evaluation stack, since for these
     operations the output is always less or equal than both operands.  */
}

/* Generates a LT_EXPR, LE_EXPR, GT_EXPR, GE_EXPR, EQ_EXPR, NE_EXPR,
   UNORDERED_EXPR, ORDERED_EXPR, UNLT_EXPR, UNLE_EXPR, UNGT_EXPR, UNGE_EXPR,
   UNEQ_EXPR or LTGT_EXPR expression used outside of a COND_EXPR.  */

static void
gen_compare_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  enum tree_code code = TREE_CODE (node);
  tree op0 = TREE_OPERAND (node, 0);
  tree op1 = TREE_OPERAND (node, 1);
  cil_stmt stmt;
  enum cil_opcode opcode;

  switch (code)
    {
    case LT_EXPR:
    case GT_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case UNLT_EXPR:
    case UNGT_EXPR:
      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);

      if (TREE_CODE (node) == NE_EXPR)
	{
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  gen_integer_cst (csi, integer_one_node, sloc);
	  stmt = cil_build_stmt (CIL_XOR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      else
	{
	  switch (TREE_CODE (node))
	    {
	    case LT_EXPR:
	      opcode = TYPE_UNSIGNED (TREE_TYPE (op0)) ? CIL_CLT_UN : CIL_CLT;
	      break;

	    case GT_EXPR:
	      opcode = TYPE_UNSIGNED (TREE_TYPE (op0)) ? CIL_CGT_UN : CIL_CGT;
	      break;

	    case EQ_EXPR:
	      opcode = CIL_CEQ;
	      break;

	    case UNLT_EXPR:
	      opcode = CIL_CLT_UN;
	      break;

	    case UNGT_EXPR:
	      opcode = CIL_CGT_UN;
	      break;

	    default:
	      gcc_unreachable ();
	  }

	  stmt = cil_build_stmt (opcode);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      break;

    case LE_EXPR:
    case GE_EXPR:
      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);

      if (TREE_CODE (node) == LE_EXPR)
	opcode = TYPE_UNSIGNED (TREE_TYPE (op0)) ? CIL_CGT_UN : CIL_CGT;
      else
	opcode = TYPE_UNSIGNED (TREE_TYPE (op0)) ? CIL_CLT_UN : CIL_CLT;

      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      gen_integer_cst (csi, integer_one_node, sloc);
      stmt = cil_build_stmt (CIL_XOR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case UNORDERED_EXPR:
    case ORDERED_EXPR:
      gimple_to_cil_node (csi, op0, sloc);
      stmt = cil_build_stmt (CIL_DUP);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      gimple_to_cil_node (csi, op1, sloc);
      stmt = cil_build_stmt (CIL_DUP);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_AND);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (TREE_CODE (node) == UNORDERED_EXPR)
	{
	  gen_integer_cst (csi, integer_one_node, sloc);
	  stmt = cil_build_stmt (CIL_XOR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      break;

    case UNEQ_EXPR:
    case UNLE_EXPR:
    case UNGE_EXPR:
      op0 = gen_expr_copy (csi, op0, sloc);
      op1 = gen_expr_copy (csi, op1, sloc);

      /* Emit the equivalent of an ORDERED_EXPR ...  */
      gimple_to_cil_node (csi, op0, sloc);
      stmt = cil_build_stmt (CIL_DUP);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      gimple_to_cil_node (csi, op1, sloc);
      stmt = cil_build_stmt (CIL_DUP);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_AND);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      /* ... plus the relevant comparison.  */
      if (code == UNEQ_EXPR)
	{
	  /* !ORDERED_EXPR || EQ_EXPR */
	  gen_integer_cst (csi, integer_one_node, sloc);
	  stmt = cil_build_stmt (CIL_XOR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  gimple_to_cil_node (csi, op0, sloc);
	  gimple_to_cil_node (csi, op1, sloc);
	  stmt = cil_build_stmt (CIL_CEQ);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_OR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      else
	{
	  /* !(ORDERED_EXPR && GT_EXPR) or !(ORDERED_EXPR && LT_EXPR) */
	  gimple_to_cil_node (csi, op0, sloc);
	  gimple_to_cil_node (csi, op1, sloc);
	  stmt = cil_build_stmt (code == UNLE_EXPR ? CIL_CGT : CIL_CLT);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt (CIL_AND);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  gen_integer_cst (csi, integer_one_node, sloc);
	  stmt = cil_build_stmt (CIL_XOR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      break;

    case LTGT_EXPR:
      op0 = gen_expr_copy (csi, op0, sloc);
      op1 = gen_expr_copy (csi, op1, sloc);

      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);
      stmt = cil_build_stmt (CIL_CGT);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);
      stmt = cil_build_stmt (CIL_CLT);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      stmt = cil_build_stmt (CIL_OR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    default:
      gcc_unreachable ();
  }

  if (tree_low_cst (TYPE_SIZE (TREE_TYPE (node)), 1) > 32)
    {
      stmt = cil_build_stmt (CIL_CONV_I8);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates CIL code for a (MIN|MAX)_EXPR held in the tree NODE.  The node is
   either expanded or replaced with a builtin call, depending on the options
   and the optimization level.  The generated statements are appended to the
   current function's CIL code using the CSI iterator.  */

static void
gen_minmax_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;
  tree type = TREE_TYPE (node);
  bool max = (TREE_CODE (node) == MAX_EXPR);
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (type), 1);
  bool unsignedp;
  enum cil32_builtin builtin = 0;

  gimple_to_cil_node (csi, TREE_OPERAND (node, 0), sloc);

  if (POINTER_TYPE_P (type))
    {
      stmt = cil_build_stmt (CIL_CONV_I);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  gimple_to_cil_node (csi, TREE_OPERAND (node, 1), sloc);

  if (POINTER_TYPE_P (type))
    {
      stmt = cil_build_stmt (CIL_CONV_I);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type))
    {
      unsignedp = TYPE_UNSIGNED (type) || POINTER_TYPE_P (type);

      if (size <= 32)
	{
	  if (max)
	    builtin = unsignedp ? CIL32_UMAXSI3 : CIL32_MAXSI3;
	  else
	    builtin = unsignedp ? CIL32_UMINSI3 : CIL32_MINSI3;
	}
      else
	{
	  gcc_assert (size <= 64);

	  if (max)
	    builtin = unsignedp ? CIL32_UMAXDI3 : CIL32_MAXDI3;
	  else
	    builtin = unsignedp ? CIL32_UMINDI3 : CIL32_MINDI3;
	}
    }
  else if (SCALAR_FLOAT_TYPE_P (type))
    {
      if (size == 32)
	builtin = max ? CIL32_MAXSF3 : CIL32_MINSF3;
      else
	{
	  gcc_assert (size == 64);
	  builtin = max ? CIL32_MAXDF3 : CIL32_MINDF3;
	}
    }
  else
    gcc_unreachable ();

  stmt = cil_build_call (cil32_builtins[builtin]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generates CIL code for an ABS_EXPR held in the tree NODE.  The node is
   either expanded or replaced with a builtin call, depending on the options
   and the optimization level.  The generated statements are appended to the
   current function's CIL code using the CSI iterator.  */

static void
gen_abs_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;
  tree type = TREE_TYPE (node);
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (type), 1);
  enum cil32_builtin builtin = 0;

  gcc_assert (!TARGET_EXPAND_ABS);

  gimple_to_cil_node (csi, TREE_OPERAND (node, 0), sloc);

  if (INTEGRAL_TYPE_P (type))
    {
      if (size == 32)
	builtin = CIL32_ABSSI2;
      else
	{
	  gcc_assert (size == 64);
	  builtin = CIL32_ABSDI2;
	}
    }
  else if (SCALAR_FLOAT_TYPE_P (type))
    {
      if (size == 32)
	builtin = CIL32_ABSSF2;
      else
	{
	  gcc_assert (size == 64);
	  builtin = CIL32_ABSDF2;
	}
    }
  else
    gcc_unreachable ();

  stmt = cil_build_call (cil32_builtins[builtin]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Generates CIL code for a VAR_DECL expression held in the tree NODE.  The
   generated statements are appended to the current function's CIL code using
   the CSI iterator.  */

static void
gen_var_decl (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;
  tree type = TREE_TYPE (node);

  mark_referenced_type (type);

  /* Function local static variables are promoted to global variables.  */
  if (!DECL_FILE_SCOPE_P (node) && !TREE_STATIC (node))
    {
      if (TREE_THIS_VOLATILE (node))
	{
	  /* put the address of the loc on the stack */
	  stmt = cil_build_stmt_arg (CIL_LDLOCA, node);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  /* and emit a volatile ldind or ldobj */
	  gen_ldind (csi, type, true, sloc);
	}
      else
	{
	  stmt = cil_build_stmt_arg (CIL_LDLOC, node);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
  else
    {
      stmt = cil_build_stmt_arg (CIL_LDSFLD, node);
      cil_set_locus(stmt, sloc);
      cil_set_prefix_volatile (stmt, TREE_THIS_VOLATILE (node));
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates CIL code for a COMPONENT_REF working on a bit field held in the
   tree NODE.  The generated statements are appended to the current function's
   CIL code using the CSI iterator.  */

static void
gen_bit_field_comp_ref (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  HOST_WIDE_INT bit_size = 0;
  HOST_WIDE_INT bit_pos = 0;
  HOST_WIDE_INT cont_off;
  HOST_WIDE_INT cont_size = 8;
  tree offset = NULL_TREE;
  enum machine_mode mode;
  int unsignedp = 0;
  int volatilep = 0;
  cil_stmt stmt;
  tree cont_type;
  tree ref;

  /* TODO: Add support for packed bit-fields crossing 64-bit boundaries.
     TODO: Add support for big-endian targets.  */

  /* Get the object base address and emit it.  */
  ref = get_inner_reference (node, &bit_size, &bit_pos, &offset, &mode,
			     &unsignedp, &volatilep, false);

  gen_addr_expr (csi, ref, sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  /* Calculate the container size.  */
  while ((bit_pos % cont_size + bit_size) > cont_size)
    cont_size *= 2;

  cont_type = get_integer_type (cont_size, unsignedp);
  cont_off = bit_pos % cont_size;

  /* Calculate the container address if needed.  */
  if ((bit_pos - cont_off) / BITS_PER_UNIT != 0)
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node,
				      (bit_pos - cont_off) / BITS_PER_UNIT),
		       sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (offset != NULL_TREE)
    {
      gimple_to_cil_node (csi, offset, sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Load the container.  */
  gen_scalar_ldind (csi, cont_type, volatilep, sloc);

  /* Shift the resulting value into the correct position, zero/sign extending
     it as appropriate. Since the value is now on the stack the container size
     is either 32 or 64.  */
  if (cont_size <= 32)
    cont_size = 32;
  else
    cont_size = 64;

  if (cont_size - (cont_off + bit_size))
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node,
				      cont_size - (cont_off + bit_size)),
		       sloc);
      stmt = cil_build_stmt (CIL_SHL);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (cont_size - bit_size)
    {
      gen_integer_cst (csi,
		       build_int_cst (intSI_type_node, cont_size - bit_size),
		       sloc);
      stmt = cil_build_stmt (unsignedp ? CIL_SHR_UN : CIL_SHR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  if (TYPE_PRECISION (TREE_TYPE (node)) <= 32)
    {
      if (cont_size > 32)
	{
	  stmt = cil_build_stmt (CIL_CONV_I4);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
  else
    {
      if (cont_size <= 32)
	{
	  stmt = cil_build_stmt (unsignedp ? CIL_CONV_U8 : CIL_CONV_I8);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
}

/* Generates CIL code for a COMPONENT_REF expression held in the tree NODE.
   The generated statements are appended to the current function's CIL code
   using the CSI iterator.  */

static void
gen_comp_ref (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree obj = TREE_OPERAND (node, 0);
  tree fld = TREE_OPERAND (node, 1);
  cil_stmt stmt;

  gcc_assert (TREE_CODE (fld) == FIELD_DECL);

  mark_referenced_type (TYPE_MAIN_VARIANT (TREE_TYPE (obj)));

  if (DECL_BIT_FIELD (fld))
    gen_bit_field_comp_ref (csi, node, sloc);
  else
    {
      gen_addr_expr (csi, obj, sloc);
      stmt = cil_build_stmt_arg (CIL_LDFLD, fld);
      cil_set_locus(stmt, sloc);

      if (contains_packed_reference (node))
	cil_set_prefix_unaligned (stmt, 1);

      cil_set_prefix_volatile (stmt, TREE_THIS_VOLATILE (node));
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates CIL code for a BIT_FIELD_REF expression used to access a vector
   element. The generated statements are appended to the current function's CIL
   code using the CSI iterator.  */

static void
gen_vector_bitfield_ref (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;
  tree cst;

  gen_addr_expr (csi, TREE_OPERAND (node, 0), sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  cst = size_binop (TRUNC_DIV_EXPR, TREE_OPERAND (node, 2), bitsize_unit_node);

  if (!integer_zerop (cst))
    {
      gen_integer_cst (csi, fold_convert (intSI_type_node, cst), sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  gen_ldind (csi, TREE_TYPE (node), TREE_THIS_VOLATILE (node), sloc);
}

/* Generates CIL code for a BIT_FIELD_REF expression held in the tree NODE.
   The generated statements are appended to the current function's CIL code
   using the CSI iterator.  Hopefully this function will go away with
   BIT_FIELD_REFs sooner than later.  */

static void
gen_bit_field_ref (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  unsigned HOST_WIDE_INT bit_size = 0;
  unsigned HOST_WIDE_INT bit_pos = 0;
  unsigned HOST_WIDE_INT cont_off;
  unsigned HOST_WIDE_INT cont_size = 8;
  cil_stmt stmt;
  enum cil_opcode opcode;
  tree cont_type;
  tree cst;
  tree offset = TREE_OPERAND (node, 2);
  bool unsignedp = TYPE_UNSIGNED (TREE_TYPE (node));

  /* TODO: Add support for big-endian targets.  */
  gen_addr_expr (csi, TREE_OPERAND (node, 0), sloc);
  stmt = cil_build_stmt (CIL_CONV_I);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

  cst = size_binop (TRUNC_DIV_EXPR, offset, bitsize_unit_node);

  if (!integer_zerop (cst))
    {
      gen_integer_cst (csi, fold_convert (intSI_type_node, cst), sloc);
      stmt = cil_build_stmt (CIL_ADD);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* Calculate the container size.  */
  cst = size_binop (TRUNC_MOD_EXPR, offset, bitsize_unit_node);
  bit_pos = tree_low_cst (cst, 1);
  bit_size = tree_low_cst (TREE_OPERAND (node, 1), 1);

  while ((bit_pos % cont_size + bit_size) > cont_size)
    cont_size *= 2;

  cont_type = get_integer_type (cont_size, unsignedp);
  cont_off = bit_pos % cont_size;

  /* Load the container.  */
  gen_scalar_ldind (csi, cont_type, TREE_THIS_VOLATILE (node), sloc);

  /* Shift the resulting value into the correct position, zero extending it.
     Since the value is now on the stack the container size is either 32
     or 64.  */
  if (bit_size != cont_size)
    {
      if (cont_size <= 32)
	cont_size = 32;
      else
	cont_size = 64;

      if (cont_size - (cont_off + bit_size))
	{
	  gen_integer_cst (csi,
			   build_int_cst (intSI_type_node,
					  cont_size - (cont_off + bit_size)),
			   sloc);
	  stmt = cil_build_stmt (CIL_SHL);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      if (cont_size - bit_size)
	{
	  gen_integer_cst (csi,
			   build_int_cst (intSI_type_node,
					  cont_size - bit_size),
                           sloc);
	  opcode = unsignedp ? CIL_SHR_UN : CIL_SHR;
	  stmt = cil_build_stmt (opcode);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
}

/* Generates CIL code for a TRUTH_(AND|OR|XOR)_EXPR expression held in the tree
   NODE.  The generated statements are appended to the current function's CIL
   code using the CSI iterator.  */

static void
gen_truth_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree op0 = TREE_OPERAND (node, 0);
  tree op1 = TREE_OPERAND (node, 1);
  cil_stmt stmt;

  gimple_to_cil_node (csi, op0, sloc);

  if (TREE_CODE (TREE_TYPE (op0)) == INTEGER_TYPE)
    {
      gen_integer_cst (csi, integer_zero_node, sloc);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      gen_integer_cst (csi, integer_one_node, sloc);
      stmt = cil_build_stmt (CIL_XOR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else
    gcc_assert (TREE_CODE (TREE_TYPE (op0)) == BOOLEAN_TYPE);

  gimple_to_cil_node (csi, op1, sloc);

  if (TREE_CODE (TREE_TYPE (op1)) == INTEGER_TYPE)
    {
      gen_integer_cst (csi, integer_zero_node, sloc);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      gen_integer_cst (csi, integer_one_node, sloc);
      stmt = cil_build_stmt (CIL_XOR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else
    gcc_assert (TREE_CODE (TREE_TYPE (op1)) == BOOLEAN_TYPE);

  if (TREE_CODE (node) == TRUTH_AND_EXPR)
    {
      stmt = cil_build_stmt (CIL_AND);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else if (TREE_CODE (node) == TRUTH_OR_EXPR)
    {
      stmt = cil_build_stmt (CIL_OR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
  else
    {
      stmt = cil_build_stmt (CIL_XOR);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      gen_integer_cst (csi, integer_one_node, sloc);
      stmt = cil_build_stmt (CIL_AND);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates the address of a TARGET_MEM_REF node specified by NODE and push it
   on the stack. The generated statements are appended to the current
   function's CIL code using the CSI iterator.  */

static void
gen_target_mem_ref (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  tree type = TREE_TYPE (node);
  tree ptr_type = build_pointer_type (type);

  gimple_to_cil_node (csi, tree_mem_ref_addr (ptr_type, node), sloc);
  gen_ldind (csi, type, TREE_THIS_VOLATILE (node), sloc);
}

/* Generates CIL code for a VIEW_CONVERT_EXPR working on the operand held by
   NODE to the destination type DEST_TYPE with either the type of NODE or
   DEST_TYPE or both being vector types.  The generated statements are appended
   to the current function's CIL code using the CSI iterator.  */

static void
gen_vector_view_convert_expr (cil_stmt_iterator *csi, tree dest_type, tree node, source_location sloc)
{
  tree src_type = TREE_TYPE (node);
  tree elem_type;
  unsigned HOST_WIDE_INT dest_size = tree_low_cst (TYPE_SIZE (dest_type), 1);
  unsigned HOST_WIDE_INT src_size = tree_low_cst (TYPE_SIZE (src_type), 1);
  unsigned HOST_WIDE_INT elem_size, n_elem;
  bool unsignedp;
  enum cil32_builtin builtin = CIL32_MAX_BUILT_IN; /* Placeholder */
  cil_stmt stmt;

  gimple_to_cil_node (csi, node, sloc);

  if (TREE_CODE (src_type) == VECTOR_TYPE)
    {
      /* Convert a vector type to something.  */
      elem_type = TREE_TYPE (src_type);
      elem_size = tree_low_cst (TYPE_SIZE (elem_type), 1);
      n_elem = TYPE_VECTOR_SUBPARTS (src_type);
      unsignedp = TYPE_UNSIGNED (dest_type);

      if (INTEGRAL_TYPE_P (dest_type))
	{
	  if (dest_size == 32 && INTEGRAL_TYPE_P (elem_type))
	    {
	      if (elem_size == 8 && n_elem == 4)
		builtin = unsignedp ? CIL32_GCC_V4QI_TO_USI
				    : CIL32_GCC_V4QI_TO_SI;
	      else if (elem_size == 16 && n_elem == 2)
		builtin = unsignedp ? CIL32_GCC_V2HI_TO_USI
				    : CIL32_GCC_V2HI_TO_SI;
	    }
	  else if (dest_size == 64 && INTEGRAL_TYPE_P (elem_type))
	    {
	      if (elem_size == 8 && n_elem == 8)
		builtin = unsignedp ? CIL32_GCC_V8QI_TO_UDI
				    : CIL32_GCC_V8QI_TO_DI;
	      else if (elem_size == 16 && n_elem == 4)
		builtin = unsignedp ? CIL32_GCC_V4HI_TO_UDI
				    : CIL32_GCC_V4HI_TO_DI;
	      else if (elem_size == 32 && n_elem == 2)
		builtin = unsignedp ? CIL32_GCC_V2SI_TO_UDI
				    : CIL32_GCC_V2SI_TO_DI;
	    }
	  else if (dest_size == 64 && SCALAR_FLOAT_TYPE_P (elem_type))
	    {
	      if (elem_size == 32)
		builtin = CIL32_GCC_V2SF_TO_DI;
	    }
	}
      else if (TREE_CODE (dest_type) == VECTOR_TYPE)
	{
	  if ((INTEGRAL_TYPE_P (elem_type) && elem_size == 32)
	      && (SCALAR_FLOAT_TYPE_P (TREE_TYPE (dest_type))
		  && tree_low_cst (TYPE_SIZE (TREE_TYPE (dest_type)), 1) == 32)
	      && dest_size == src_size)
	    {
	      builtin = CIL32_GCC_V4SI_TO_V4SF;
	    }
	}
    }
  else
    {
      /* Convert something to a vector type */
      elem_type = TREE_TYPE (dest_type);
      elem_size = tree_low_cst (TYPE_SIZE (elem_type), 1);
      n_elem = TYPE_VECTOR_SUBPARTS (dest_type);

      if (INTEGRAL_TYPE_P (src_type))
	{
	  if (src_size == 32)
	    {
	      if (elem_size == 8 && n_elem == 4)
		builtin = CIL32_GCC_V4QI_CTOR2;
	      else if (elem_size == 16 && n_elem == 2)
		builtin = CIL32_GCC_V2HI_CTOR2;
	    }
	  else if (src_size == 64)
	    {
	      if (elem_size == 8 && n_elem == 8)
		builtin = CIL32_GCC_V8QI_CTOR2;
	      else if (elem_size == 16 && n_elem == 4)
		builtin = CIL32_GCC_V4HI_CTOR2;
	      else if (elem_size == 32 && n_elem == 2)
		builtin = CIL32_GCC_V2SI_CTOR2;
	    }
	}
    }

  if (builtin == CIL32_MAX_BUILT_IN)
    {
      internal_error ("Unsupported VIEW_CONVERT_EXPR\n");
    }

  stmt = cil_build_call (cil32_builtins[builtin]);
  cil_set_locus(stmt, source_location_from_tree (node));
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
  cfun->machine->has_vec = true;
}

/* If all elements of a constant vector are equal, return that
   element. Otherwise return NULL.  */

static tree
is_constructor_uniform (tree ctor)
{
  unsigned HOST_WIDE_INT i;
  tree purpose, elt_val;
  tree prev_val = NULL;
  VEC (constructor_elt, gc) *elts = CONSTRUCTOR_ELTS (ctor);

  FOR_EACH_CONSTRUCTOR_ELT (elts, i, purpose, elt_val)
    {
      if (!prev_val)
        prev_val = elt_val;
      else
        {
          if (prev_val != elt_val)
            return NULL;
        }
    }
  return prev_val;
}


/* Generates the code for a vector CONSTRUCTOR node CTOR.  The generated
   statements are appended to the current function's CIL code using the CSI
   iterator.  */

static void
gen_vector_ctor (cil_stmt_iterator *csi, tree ctor, source_location sloc)
{
  VEC (constructor_elt, gc) *elts = CONSTRUCTOR_ELTS (ctor);
  unsigned HOST_WIDE_INT i;
  tree purpose, value;
  tree vector_type = TREE_TYPE (ctor);
  tree uniform_val = is_constructor_uniform (ctor);
  cil_stmt stmt;

  if (uniform_val)
    gimple_to_cil_node (csi, uniform_val, sloc);
  else
    {
      FOR_EACH_CONSTRUCTOR_ELT (elts, i, purpose, value)
        gimple_to_cil_node (csi, value, sloc);
    }

  stmt = cil_build_stmt_arg (CIL_VEC_CTOR, vector_type);
  cil_set_locus(stmt, sloc);
  if (uniform_val)
    cil_set_short_ctor (stmt, 1);

  csi_insert_after (csi,  stmt, CSI_CONTINUE_LINKING);

  if (cfun)
    cfun->machine->has_vec = true;
}

/* Emit the code needed to generate a REALPART_ or IMAGPART_EXPR expression.  */

static void
gen_complex_part_expr (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  cil_stmt stmt;
  tree op0 = TREE_OPERAND (node, 0);
  tree type = TREE_TYPE (op0);

  if (TREE_CODE (op0) == COMPLEX_EXPR)
    {
      /* Get the relevant part immediately */
      if (TREE_CODE (node) == REALPART_EXPR)
	gimple_to_cil_node (csi, TREE_OPERAND (op0, 0), sloc);
      else
	gimple_to_cil_node (csi, TREE_OPERAND (op0, 1), sloc);
    }
  else
    {
      if (DECL_P (op0)
	  || TREE_CODE (op0) == INDIRECT_REF
	  || TREE_CODE (op0) == ARRAY_REF
	  || TREE_CODE (op0) == COMPONENT_REF)
	{
	  /* Generate the object's address */
	  gen_addr_expr (csi, op0, sloc);
	}
      else
	gimple_to_cil_node (csi, op0, sloc);

      if (TREE_CODE (node) == REALPART_EXPR)
	{
	  stmt = cil_build_stmt_arg (CIL_LDFLD,
				     cil_get_builtin_complex_real_fld (type));
	}
      else
	{
	  stmt = cil_build_stmt_arg (CIL_LDFLD,
				     cil_get_builtin_complex_imag_fld (type));
	}

      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Emit the code needed for COMPLEX_CST/COMPLEX_EXPR expressions.  */

static void
gen_complex (cil_stmt_iterator *csi, tree type, tree real, tree imag, source_location sloc)
{
  tree elem_type = TREE_TYPE (type);
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (elem_type), 1);
  enum cil32_builtin builtin = 0;
  cil_stmt stmt;
  bool unsignedp;

  gimple_to_cil_node (csi, real, sloc);
  gimple_to_cil_node (csi, imag, sloc);

  if (INTEGRAL_TYPE_P (elem_type))
    {
      unsignedp = TYPE_UNSIGNED (elem_type);

      switch (size)
	{
	case 8:
	  builtin = unsignedp ? CIL32_CPLX_UCHAR_CTOR : CIL32_CPLX_CHAR_CTOR;
	  break;

	case 16:
	  builtin = unsignedp ? CIL32_CPLX_USHORT_CTOR : CIL32_CPLX_SHORT_CTOR;
	  break;

	case 32:
	  builtin = unsignedp ? CIL32_CPLX_UINT_CTOR : CIL32_CPLX_INT_CTOR;
	  break;

	case 64:
	  builtin = unsignedp ? CIL32_CPLX_ULONG_CTOR : CIL32_CPLX_LONG_CTOR;
	  break;

	default:
	  gcc_unreachable ();
	}
    }
  else
    {
      gcc_assert (SCALAR_FLOAT_TYPE_P (elem_type)
		  && ((size == 32) || (size == 64)));

      if (size == 32)
	builtin = CIL32_CPLX_FLOAT_CTOR;
      else
	builtin = CIL32_CPLX_DOUBLE_CTOR;
    }

  stmt = cil_build_call (cil32_builtins[builtin]);
  cil_set_locus(stmt, sloc);
  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
}

/* Return the opcode associated with the conversion to the TYPE type.  */

static enum cil_opcode
conv_opcode_from_type (tree type)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (type), 1);
  bool unsignedp;

  if (INTEGRAL_TYPE_P (type))
    {
      unsignedp = TYPE_UNSIGNED (type);

      switch (size)
	{
	case 8:  return (unsignedp ? CIL_CONV_U1 : CIL_CONV_I1);
	case 16: return (unsignedp ? CIL_CONV_U2 : CIL_CONV_I2);
	case 32: return (unsignedp ? CIL_CONV_U4 : CIL_CONV_I4);
	case 64: return (unsignedp ? CIL_CONV_U8 : CIL_CONV_I8);
	default:
	  gcc_unreachable ();
	}
    }
  else if (POINTER_TYPE_P (type))
    return CIL_CONV_I;
  else if (SCALAR_FLOAT_TYPE_P (type))
    {
      if (size == 32)
	return CIL_CONV_R4;
      else
	{
	  gcc_assert (size == 64);
	  return CIL_CONV_R8;
	}
    }
  else
    gcc_unreachable ();
}

/* Emit a conversion from integral or pointer type SRC to integral type DST.
   If the precision of DST is bigger than that of SRC, then SRC and DST have
   to have the same signedness.   */

static void
gen_integral_conv (cil_stmt_iterator *csi, tree dst, tree src, source_location sloc)
{
  unsigned int dst_bits, cont_size, src_bits;
  enum cil_opcode opcode;
  cil_stmt stmt;
  tree type;

  gcc_assert (INTEGRAL_TYPE_P (dst));
  gcc_assert (INTEGRAL_TYPE_P (src) || POINTER_TYPE_P (src));
  gcc_assert (TYPE_PRECISION (dst) <= 64);
  gcc_assert (TYPE_PRECISION (dst) <= TYPE_PRECISION (src)
	      || TYPE_UNSIGNED (dst) == TYPE_UNSIGNED (src));

  /* Get the precision of the output and input types and the size
     of the output type container */
  src_bits = TYPE_PRECISION (src);
  dst_bits = TYPE_PRECISION (dst);
  cont_size = GET_MODE_BITSIZE (TYPE_MODE (dst));
  gcc_assert (cont_size >= dst_bits);

  /* Dump a conv with for the container size, if not superfluous */
  if ((cont_size == dst_bits && (dst_bits != src_bits || dst_bits < 32))
      || ((dst_bits > 32) ^ (src_bits > 32)))
    {
      opcode = conv_opcode_from_type (get_integer_type (cont_size,
							TYPE_UNSIGNED (dst)));
      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }

  /* If the container is bigger than the output type precision,
     force the output to be of the desired precision.   */
  if (cont_size > dst_bits)
    {
      type = (dst_bits <= 32) ? intSI_type_node : intDI_type_node;

      if (TYPE_UNSIGNED (dst))
	{
	  tree mask;

	  mask = size_binop (LSHIFT_EXPR, build_int_cst (type, 1),
			     build_int_cst (type, dst_bits));
	  mask = size_binop (MINUS_EXPR, mask, build_int_cst (type, 1));
	  gen_integer_cst (csi, mask, sloc);
	  stmt = cil_build_stmt (CIL_AND);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      else
	{
	  tree shift;

	  if (dst_bits <= 32)
	    shift = build_int_cst (intSI_type_node, 32 - dst_bits);
	  else
	    shift = build_int_cst (intSI_type_node, 64 - dst_bits);

	  /* Do a pair of shift to perform the sign extension */
	  gen_integer_cst (csi, shift, sloc);
	  stmt = cil_build_stmt (CIL_SHL);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  gen_integer_cst (csi, shift, sloc);
	  stmt = cil_build_stmt (CIL_SHR);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }
}

/* Emit a conversion from type IN_TYPE to type OUT_TYPE to file FILE.
   IS_NOP says whether the conversion comes from a NOP_EXPR.   */

static void
gen_conv (cil_stmt_iterator *csi, bool is_nop, tree dst, tree src, source_location sloc)
{
  cil_stmt stmt;

  if (is_nop && INTEGRAL_TYPE_P (dst) && INTEGRAL_TYPE_P (src))
    {
      if (TYPE_PRECISION (dst) > TYPE_PRECISION (src))
	{
	  tree tmp = TYPE_UNSIGNED (src)
		     ? unsigned_type_for (dst)
		     : signed_type_for (dst);

	  gen_integral_conv (csi, tmp, src, sloc);
	  gen_integral_conv (csi, dst, tmp, sloc);
	}
      else
	gen_integral_conv (csi, dst, src, sloc);
    }

  /* Special case for conversion to float type are not orthogonal
     in CIL opcode set.   */
  else if (SCALAR_FLOAT_TYPE_P (dst)
	   && INTEGRAL_TYPE_P (src)
	   && TYPE_UNSIGNED (src))
    {
      stmt = cil_build_stmt (CIL_CONV_R_UN);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      if (TYPE_PRECISION (dst) <= 32)
	{
	  stmt = cil_build_stmt (CIL_CONV_R4);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
    }

  /* Do nothing for a conversion from two REAL_TYPEs with the
     same precision or two pointers.   */
  else if (!SCALAR_FLOAT_TYPE_P (dst)
	   || !SCALAR_FLOAT_TYPE_P (src)
	   || TYPE_PRECISION (dst) != TYPE_PRECISION (src))
    {
      stmt = cil_build_stmt (conv_opcode_from_type (dst));
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
    }
}

/* Generates the equivalent CIL code for rotate expressions. Since rotations
   are not available in CIL they are emulated using shifts.  */

static void
gen_rotate (cil_stmt_iterator *csi, tree node, source_location sloc)
{
  bool left = (TREE_CODE (node) == LROTATE_EXPR);
  tree op0, op1;
  tree t1, t2;
  tree uns_type;

  /* Rotation is replaced by shifts on unsigned values:
     generate the unsigned version of first operand type.  */
  op0 = TREE_OPERAND (node, 0);
  uns_type = unsigned_type_for (TREE_TYPE (op0));
  op0 = fold_convert (uns_type, op0);

  /* Convert the second operand to 32-bit.  */
  op1 = fold_convert (intSI_type_node, TREE_OPERAND (node, 1));

  /* Build first shift.  */
  t1 = fold_build2 (left ? LSHIFT_EXPR : RSHIFT_EXPR, uns_type, op0, op1);

  /* Build second shift.  */
  t2 = fold_build2 (left ? RSHIFT_EXPR : LSHIFT_EXPR, uns_type,
		    op0,
		    fold_build2 (MINUS_EXPR, unsigned_intSI_type_node,
				 fold_convert (unsigned_intSI_type_node,
					       TYPE_SIZE (TREE_TYPE (op0))),
				 op1));

  /* Build the rotate result.  We do not use fold_build2() as it would
     recreate the *ROTATE_EXPR.  */
  t1 = build2 (BIT_IOR_EXPR, uns_type, t1, t2);
  t1 = fold_convert (TREE_TYPE (TREE_OPERAND (node, 0)), t1);

  /* Generate the code */
  gimple_to_cil_node (csi, t1, sloc);
}

/* Converts a GIMPLE/generic node into its CIL form. The generated statements
 * are appended to the current function's CIL code using the CSI iterator.  */

static void
gimple_to_cil_node (cil_stmt_iterator *csi, tree node, source_location parent_sloc)
{
  tree op0, op1;
  cil_stmt stmt = NULL;
  enum cil_opcode opcode;
  bool uns;
  source_location sloc;

  if (node == NULL_TREE || node == error_mark_node)
    return;

  sloc = source_location_from_tree (node);
  /* TL - If parent_sloc is not known, then do use current sloc:
   * it is better than nothing at all, and mandatory in certain cases.
   * Also, if sloc is unknown, do use parent_sloc */
  if (parent_sloc == UNKNOWN_LOCATION)
    parent_sloc = sloc;
  else if (sloc == UNKNOWN_LOCATION)
    sloc = parent_sloc;

  switch (TREE_CODE (node))
    {
    case INTEGER_CST:
      gen_integer_cst (csi, node, parent_sloc);

      if (POINTER_TYPE_P (TREE_TYPE (node)))
	{
	  stmt = cil_build_stmt (CIL_CONV_I);
	  cil_set_locus(stmt, parent_sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      break;

    case REAL_CST:
      gen_real_cst (csi, node, parent_sloc);
      break;

    case COMPLEX_CST:
      gen_complex (csi, TREE_TYPE (node),
		   TREE_REALPART (node),
		   TREE_IMAGPART (node),
		   parent_sloc);
      break;

    case CONSTRUCTOR:
      if (TREE_CODE (TREE_TYPE (node)) == VECTOR_TYPE)
	gen_vector_ctor (csi, node, sloc);
      else
	internal_error ("Unsupported type in CONSTRUCTOR\n");

      break;

    case VECTOR_CST:
      gen_vector_cst (csi, node, sloc);
      break;

    case LABEL_DECL:
      gcc_unreachable ();
      break;

    case COMPOUND_LITERAL_EXPR:
      /* HACK: We should find a way to avoid front-end nodes */
      gimple_to_cil_node (csi, COMPOUND_LITERAL_EXPR_DECL (node), sloc);
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      if ((TREE_CODE (op1) == CONSTRUCTOR &&
	   (TREE_CODE (TREE_TYPE (op1)) != VECTOR_TYPE)) ||
	  TREE_CODE (op1) == STRING_CST)
	{
	  csi_insert_seq_after (csi, expand_init_to_cil_seq (op0, op1, sloc),
				CSI_CONTINUE_LINKING);
	}
      else
	gen_modify_expr (csi, op0, op1, sloc);

      break;

    case GOTO_EXPR:
      internal_error ("GOTO_EXPRs shouldn't appear inside other trees or "
		      "before the end of a basic block\n");
      break;

    case COND_EXPR:
      {
	/* HACK: COND_EXPRs shouldn't appear here without proper vector
	   support, we should either implement proper vector support in
	   builtin-calls form or remove it alltogether.  */
	tree type = TREE_TYPE (node);
	unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (type), 1);
	enum cil32_builtin builtin;

	if (INTEGRAL_TYPE_P (type))
	  {
	    if (size <= 32)
	      builtin = CIL32_SELECTSI4;
	    else
	      builtin = CIL32_SELECTDI4;
	  }
	else if (SCALAR_FLOAT_TYPE_P (type))
	  {
	    if (size <= 32)
	      builtin = CIL32_SELECTSF4;
	    else
	      builtin = CIL32_SELECTDF4;
	  }
	else
	  gcc_unreachable ();

	gimple_to_cil_node (csi, COND_EXPR_COND (node), sloc);
	gimple_to_cil_node (csi, COND_EXPR_THEN (node), sloc);
	gimple_to_cil_node (csi, COND_EXPR_ELSE (node), sloc);
	stmt = cil_build_call (cil32_builtins[builtin]);
	cil_set_locus(stmt, sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      }

      break;

    case SWITCH_EXPR:
      internal_error ("SWITCH_EXPRs shouldn't appear inside other trees or "
		      "before the end of a basic block\n");
      break;

    case CALL_EXPR:
      gen_call_expr (csi, node);
      break;

    case MULT_EXPR:
    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
    case MINUS_EXPR:
    case RDIV_EXPR:
    case LSHIFT_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gimple_to_cil_node (csi, op0, sloc);

      if (TREE_CODE (node) == LSHIFT_EXPR)
	gimple_to_cil_node (csi, fold_convert (intSI_type_node, op1), sloc);
      else
	gimple_to_cil_node (csi, op1, sloc);

      switch (TREE_CODE (node))
	{
	case MULT_EXPR:         opcode = CIL_MUL; break;
	case POINTER_PLUS_EXPR:
	case PLUS_EXPR:         opcode = CIL_ADD; break;
	case MINUS_EXPR:        opcode = CIL_SUB; break;
	case RDIV_EXPR:         opcode = CIL_DIV; break;
	case LSHIFT_EXPR:       opcode = CIL_SHL; break;
	default:
	  gcc_unreachable ();
	}

      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      /* Values with precision smaller than the one used
	 on the evaluation stack require an explicit conversion.   */
      if (INTEGRAL_TYPE_P (TREE_TYPE (node)))
	gen_integral_conv (csi, TREE_TYPE (node), TREE_TYPE (node), sloc);
      break;

    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gimple_to_cil_node (csi, op0, sloc);
      gimple_to_cil_node (csi, op1, sloc);

      switch (TREE_CODE (node))
	{
	case BIT_IOR_EXPR: opcode = CIL_OR;  break;
	case BIT_XOR_EXPR: opcode = CIL_XOR; break;
	default:
	  gcc_unreachable ();
	}

      /* No need for conversions even in case of values with precision
	 smaller than the one used on the evaluation stack,
	 since for these operations the output is
	 always less or equal than both operands.   */

      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case BIT_AND_EXPR:
      gen_bit_and_expr (csi, node, sloc);
      break;

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
    case LTGT_EXPR:
      gen_compare_expr (csi, node, sloc);
      break;

    case EXACT_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case RSHIFT_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);
      uns = TYPE_UNSIGNED (TREE_TYPE (node));

      gimple_to_cil_node (csi, op0, sloc);

      if (TREE_CODE (node) == RSHIFT_EXPR)
	gimple_to_cil_node (csi, fold_convert (intSI_type_node, op1), sloc);
      else
	gimple_to_cil_node (csi, op1, sloc);

      switch (TREE_CODE (node))
	{
	case EXACT_DIV_EXPR:
	case TRUNC_DIV_EXPR: opcode = uns ? CIL_DIV_UN : CIL_DIV; break;
	case TRUNC_MOD_EXPR: opcode = uns ? CIL_REM_UN : CIL_REM; break;
	case RSHIFT_EXPR:    opcode = uns ? CIL_SHR_UN : CIL_SHR; break;
	default:
	  gcc_unreachable ();
	}

      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      /* No need for conversions even in case of values with precision
	 smaller than the one used on the evaluation stack,
	 since for these operations the output is
	 always less or equal than both operands.   */
      break;

    case LROTATE_EXPR:
    case RROTATE_EXPR:
      gen_rotate (csi, node, sloc);
      break;

    case FLOOR_DIV_EXPR:
      {
	bool is_signed0, is_signed1;

	op0 = TREE_OPERAND (node, 0);
	op1 = TREE_OPERAND (node, 1);

	gimple_to_cil_node (csi, op0, sloc);
	gimple_to_cil_node (csi, op1, sloc);

	is_signed0 = TYPE_UNSIGNED (TREE_TYPE (op0));
	is_signed1 = TYPE_UNSIGNED (TREE_TYPE (op1));
	/* If both operands are unsigned, the result is positive and thus
	   rounding towards zero is identical to towards -infinity.  */
	if (is_signed0 && is_signed1)
	  {
	    stmt = cil_build_stmt (CIL_DIV_UN);
	    cil_set_locus(stmt, sloc);
	    csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	  }
	else
	  internal_error ("\n\nFLOOR_DIV_EXPR is not completely supported\n");

	/* No need for conversions even in case of values with precision
	   smaller than the one used on the evaluation stack,
	   since for these operations the output is
	   always less or equal than both operands.   */
	break;
      }

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
      gimple_to_cil_node (csi, TREE_OPERAND (node, 0), sloc);

      if (POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (node, 0))))
	{
	  stmt = cil_build_stmt (CIL_CONV_I);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}

      opcode = (TREE_CODE (node) == NEGATE_EXPR) ? CIL_NEG : CIL_NOT;
      stmt = cil_build_stmt (opcode);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

      /* Values with precision smaller than the one used
	 on the evaluation stack require an explicit conversion.
	 Unfortunately this is true for the negation as well just
	 for the case in which the operand is the smallest negative value.
	 Example: 8-bit negation of -128 gives 0 and not 128.   */
      if (INTEGRAL_TYPE_P (TREE_TYPE (node)))
	gen_integral_conv (csi, TREE_TYPE (node), TREE_TYPE (node), sloc);
      break;

    case ARRAY_REF:
    case INDIRECT_REF:
      gen_addr_expr (csi, node, sloc);
      gen_ldind (csi, TREE_TYPE (node), TREE_THIS_VOLATILE (node), sloc);
      break;

    case MISALIGNED_INDIRECT_REF:
      gen_addr_expr (csi, node, sloc);
      gen_misaligned_ldvec (csi, TREE_TYPE (node), TREE_THIS_VOLATILE (node), sloc);
      break;

    case TARGET_MEM_REF:
      gen_target_mem_ref (csi, node, sloc);
      break;

    case CONVERT_EXPR:
    case FLOAT_EXPR:
      /* TODO: if flag_trapv is set, we could generate the .ovf version? */
    case FIX_TRUNC_EXPR:
    case NOP_EXPR:
      {
	tree type;

	op0 = TREE_OPERAND (node, 0);
	gimple_to_cil_node (csi, op0, sloc);

	/* Temporaries with weird types are handled correctly without need
	   for an explicit conversion as they have already been promoted.  */
	if ((TREE_CODE (node) == NOP_EXPR) && (TREE_CODE (op0) == VAR_DECL))
	  type = promote_local_var_type (op0);
	else
	  type = TREE_TYPE (op0);

	gen_conv (csi, TREE_CODE (node) == NOP_EXPR, TREE_TYPE (node), type, sloc);
      }
      break;

    case LABEL_EXPR:
      /* Skip this expression, labels are emitted later. TODO: Check that the
	 labels appear only at the beginning of a basic-block?  */
      break;

    case RETURN_EXPR:
      op0 = TREE_OPERAND (node, 0);

      if (op0 != NULL_TREE)
	{
	  if (TREE_CODE (op0) == MODIFY_EXPR)
	    op0 = TREE_OPERAND (op0, 1);

	  gimple_to_cil_node (csi, op0, sloc);

          stmt = cil_build_stmt (CIL_RET_VAL);
          cil_set_locus(stmt, sloc);
          csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
	}
      else if (!VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
	{
	  /* Pre-C99 code may contain void-returns for non-void functions.
	     In this case, return an artificially generated result variable.  */
	  tree res_type = TREE_TYPE (TREE_TYPE (current_function_decl));

	  if (TYPE_SIZE (res_type) != NULL_TREE
	      && TREE_CODE (TYPE_SIZE (res_type)) != INTEGER_CST)
	    {
	      internal_error ("Returned type cannot be a variable size array or"
			      " struct\n");
	    }

	  if (res_var == NULL_TREE)
	    res_var = create_tmp_var (res_type, "gimple2cil");

	  stmt = cil_build_stmt_arg (CIL_LDLOC, res_var);
	  cil_set_locus(stmt, sloc);
	  csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

          stmt = cil_build_stmt (CIL_RET_VAL);
          cil_set_locus(stmt, sloc);
          csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	  /* Flag the function so that the emission phase will emit an 'init'
	     directive in the local variables declaration.  */
	  cfun->machine->locals_init = true;
	}
      else
        {
          stmt = cil_build_stmt (CIL_RET);
          cil_set_locus(stmt, sloc);
          csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
        }
      break;

    case ASM_EXPR:
      /* TODO: support just a simple string, no input/output/clober */
      stmt = cil_build_stmt_arg (CIL_ASM, ASM_STRING (node));
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case MAX_EXPR:
    case MIN_EXPR:
      gen_minmax_expr (csi, node, sloc);
      break;

    case ABS_EXPR:
      gen_abs_expr (csi, node, sloc);
      break;

    case SSA_NAME:
      gcc_unreachable ();

    case VAR_DECL:
    case RESULT_DECL:
      gen_var_decl (csi, node, parent_sloc);
      break;

    case PARM_DECL:
      {
	tree type = TREE_TYPE (node);
	mark_referenced_type (type);
	stmt = cil_build_stmt_arg (CIL_LDARG, node);
        cil_set_locus(stmt, parent_sloc);
	csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);

	if (TREE_CODE (type) == VECTOR_TYPE)
	  cfun->machine->has_vec = true;
      }
      break;

    case FIELD_DECL:
    case NAMESPACE_DECL:
      internal_error ("CIL: Cannot handle FIELD_DECL or NAMESPACE_DECL");
      break;

    case TREE_LIST:
      gcc_unreachable ();
      break;

    case FUNCTION_DECL:
    case CONST_DECL:
      gcc_unreachable ();
      break;

    case ADDR_EXPR:
      gen_addr_expr (csi, TREE_OPERAND (node, 0), sloc);
      break;

    case COMPONENT_REF:
      gen_comp_ref (csi, node, sloc);
      break;

    case TRUTH_NOT_EXPR:
      gimple_to_cil_node (csi, TREE_OPERAND (node, 0), sloc);
      gen_integer_cst (csi, integer_zero_node, sloc);
      stmt = cil_build_stmt (CIL_CEQ);
      cil_set_locus(stmt, sloc);
      csi_insert_after (csi, stmt, CSI_CONTINUE_LINKING);
      break;

    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
      gen_truth_expr (csi, node, sloc);
      break;

    case VIEW_CONVERT_EXPR:
      {
	/* VIEW_CONVERT_EXPRs may be redundant, check the type of the innermost
	   object and emit code only if it differs from the topmost conversion.
	 */
	tree op0 = TREE_OPERAND (node, 0);
	tree dest_type = TREE_TYPE (node);
	tree src_type;

	while (TREE_CODE (op0) == VIEW_CONVERT_EXPR)
	  op0 = TREE_OPERAND (op0, 0);

	src_type = TREE_TYPE (op0);

	if (src_type == dest_type)
	  gimple_to_cil_node (csi, op0, sloc);
	else if ((TREE_CODE (src_type) == VECTOR_TYPE)
		 || (TREE_CODE (dest_type) == VECTOR_TYPE))
	  {
	    gen_vector_view_convert_expr (csi, dest_type, op0, sloc);
	  }
	else
	  {
	    gen_addr_expr (csi, node, sloc);
	    gen_ldind (csi, dest_type, TREE_THIS_VOLATILE (node), sloc);
	  }
      }
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      gen_complex_part_expr (csi, node, sloc);
      break;

    case COMPLEX_EXPR:
      gen_complex (csi, TREE_TYPE (node),
		   TREE_OPERAND (node, 0),
		   TREE_OPERAND (node, 1),
		   sloc);
      break;

    case BIT_FIELD_REF:
      if (TREE_CODE (TREE_TYPE (TREE_OPERAND (node, 0))) == VECTOR_TYPE)
	gen_vector_bitfield_ref (csi, node, sloc);
      else
	gen_bit_field_ref (csi, node, sloc);

      break;

    case ENUMERAL_TYPE:
    case ARRAY_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case COMPLEX_TYPE:
    case VECTOR_TYPE:
    case BOOLEAN_TYPE:
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      internal_error ("gen_cil_node does not support TYPE nodes, "
		      "to dump Type name use dump_type.\n");
      break;

    default:
      internal_error ("\n\nUnsupported tree in CIL generation: '%s'\n",
		      tree_code_name[TREE_CODE (node)]);
      break;
    }
}

/* Records the addresses whose labels have been taken and generate the
   appropriate switch labels to emulate computed GOTOs.  Also ensure that all
   basic blocks are properly labeled.  */

static void
process_labels (void)
{
  basic_block bb;
  gimple_stmt_iterator gsi;
  gimple stmt;

  /* Record all the labels whose address has been taken */
  FOR_EACH_BB (bb)
    {
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
        {
          stmt = gsi_stmt (gsi);

          /* Record the address taken labels.  */
          if (gimple_code (stmt) == GIMPLE_LABEL)
            {
              tree label = gimple_label_label (stmt);
              /* Check if the label has its address taken.  */
              if (FORCED_LABEL (label))
		record_addr_taken_label (label);
            }
        }
    }

  /* Make sure that every bb has a label */
  FOR_EACH_BB (bb)
    {
      gimple_block_label (bb);
    }
}

/* Looks for non structured types initializers specified as DECL_INIT
   expressions attached to the declarations and turn them into a list of CIL
   statements. The generated list is prepended to the instructions in the first
   basic block of the current function.  */

static void
process_initializers (void)
{
  cil_seq seq = cil_seq_alloc ();
  cil_stmt_iterator csi = csi_start (seq);
  cil_stmt_iterator prev_csi;
  cil_stmt_iterator bb_csi = csi_start_bb (single_succ (ENTRY_BLOCK_PTR));
  tree cell;

  for (cell = cfun->local_decls; cell; cell = TREE_CHAIN (cell))
    {
      tree var = TREE_VALUE (cell);
      tree init = DECL_INITIAL (var);

      if (!TREE_STATIC (var) && init && init != error_mark_node)
	{
	  prev_csi = csi;
	  csi_insert_seq_after (&csi,
	                        expand_init_to_cil_seq (var, init, cfun->function_start_locus),
				CSI_CONTINUE_LINKING);
	}
    }

  csi_insert_seq_before (&bb_csi, seq, CSI_SAME_STMT);
}

/* Converts the GIMPLE/generic code of the current function in the CIL
   intermediate representation */

static unsigned int
gimple_to_cil (void)
{
  basic_block bb;
  gimple_stmt_iterator gsi;
  cil_stmt stmt;
  cil_seq seq;
  cil_stmt_iterator csi, prev_csi;
  tree node = NULL_TREE;
  gimple gimple_node = NULL;
  source_location gimple_loc = UNKNOWN_LOCATION;


  /* First try to fix this function prototype (if missing) */
  if (!TYPE_ARG_TYPES (TREE_TYPE (current_function_decl)))
    fix_missing_prototype (current_function_decl);

  /* Initialization */
  refs_init ();
  res_var = NULL_TREE;

  /* Preprocessing */
  process_labels ();

  FOR_EACH_BB (bb)
    {
      seq = cil_seq_alloc ();
      cil_set_bb_seq (bb, seq);
      csi = csi_start_bb (bb);

      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	{
	  prev_csi = csi;
	  gimple_node = gsi_stmt (gsi);
	  node = gimple_to_tree (gimple_node);

	  switch (TREE_CODE (node))
	    {
	    case CALL_EXPR:
	      {
		tree fun_expr = CALL_EXPR_FN (node);
		tree fun_type = TREE_TYPE (TREE_TYPE (fun_expr));

		gen_call_expr (&csi, node);

		if (!VOID_TYPE_P (TREE_TYPE (fun_type)))
		  {
		    stmt = cil_build_stmt (CIL_POP);
		    cil_set_locus(stmt, source_location_from_tree (node));
		    csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
		  }
	      }
	      break;

	    case GOTO_EXPR:
	      gcc_assert (gsi_stmt (gsi_last_bb (bb)) == gimple_node);
	      gen_goto_expr (&csi, node);
	      break;

	    case COND_EXPR:
	      gcc_assert (gsi_stmt (gsi_last_bb (bb)) == gimple_node);
	      gen_cond_expr (&csi, node);
	      break;

	    case SWITCH_EXPR:
	      gcc_assert (gsi_stmt (gsi_last_bb (bb)) == gimple_node);
	      gen_switch_expr (&csi, node);
	      break;

	    default:
	      if (TREE_CODE (node) != NOP_EXPR
		  || TREE_CODE (TREE_OPERAND (node, 0)) != INTEGER_CST)
		{
		  gimple_to_cil_node (&csi, node, UNKNOWN_LOCATION);
		}
	    }

	  release_stmt_tree (gimple_node, node);
	}

      gimple_node = gsi_stmt (gsi_last_bb (bb));
      gimple_loc = gimple_location (gimple_node);

      if ((!gimple_node || (gimple_code (gimple_node) != GIMPLE_COND))
	  && single_succ_p (bb))
	{
	  basic_block succ = single_succ (bb);

	  /* The last part of the test (succ != bb->next_bb) is a HACK.  It
	     avoids generating a branch to the successor in case of a
	     fallthrough. To be fixed when we have a proper layout of basic
	     blocks.   */
	  if ((succ->index != EXIT_BLOCK) && (succ != bb->next_bb))
	    {
	      tree label = gimple_block_label (succ);

	      stmt = cil_build_stmt_arg (CIL_BR, label);
	      cil_set_locus(stmt, gimple_loc);
	      csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
	    }
	}
      else if (EDGE_COUNT (bb->succs) == 0)
	{
	  if (gimple_code (gimple_node) != GIMPLE_RETURN)
	    {
	      tree ret_type = TREE_TYPE (TREE_TYPE (current_function_decl));

	      if (!VOID_TYPE_P (ret_type) && res_var == NULL_TREE)
		res_var = create_tmp_var (ret_type, "gimple2cil");

	      if (res_var != NULL_TREE)
		{
		  stmt = cil_build_stmt_arg (CIL_LDLOC, res_var);
		  cil_set_locus(stmt, gimple_loc);
		  csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
                  stmt = cil_build_stmt (CIL_RET_VAL);
		}
              else
                stmt = cil_build_stmt (CIL_RET);

	      cil_set_locus(stmt, gimple_loc);
              csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);

	      /* Flag the function so that the emission phase will emit an
		 'init' directive in the local variables declaration.  */
	      cfun->machine->locals_init = true;

	      /* FIXME: Is this really needed? */
	      make_single_succ_edge (bb, EXIT_BLOCK_PTR, EDGE_FALLTHRU);
	    }
	}
    }

  /* Add the initializers to the entry block */
  process_initializers ();

  return 0;
}

/* Gate function of GIMPLE/generic-to-CIL conversion.  */

static bool
gimple_to_cil_gate (void)
{
  return current_function_decl != NULL;
}

/* Define the parameters of the tree-final-simp-CIL pass.  */

struct gimple_opt_pass pass_gimple_to_cil =
{
 {
  GIMPLE_PASS,                          /* type */
  "gimple2cil",                         /* name */
  gimple_to_cil_gate,                   /* gate */
  gimple_to_cil,                        /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_GIMPLE_TO_CIL,                     /* tv_id */
  PROP_cfg,                             /* properties_required */
  PROP_cil,                             /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_ggc_collect|TODO_dump_func       /* todo_flags_finish */
 }
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
