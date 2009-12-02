/* Implementation of the stack information functionality.

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

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "tree.h"
#include "vec.h"

#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-stack.h"
#include "cil-stmt.h"
#include "cil-types.h"

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static cil_type_t cil_binary_op_type (enum cil_opcode, cil_type_t, cil_type_t);

/******************************************************************************
 * Stack information functionality                                            *
 ******************************************************************************/

/* Create a new empty stack.  */

cil_stack
cil_stack_create (void)
{
  cil_stack stack = XNEW (struct cil_stack_d);

  stack->vec = VEC_alloc (cil_type_t, heap, 1);

  return stack;
}

/* Create a new stack by copying the contents of stack SRC.  */

cil_stack
cil_stack_copy(const_cil_stack src)
{
  cil_stack stack = XNEW (struct cil_stack_d);

  if (VEC_length (cil_type_t, src->vec) != 0)
    stack->vec = VEC_copy (cil_type_t, heap, src->vec);
  else
    stack->vec = VEC_alloc (cil_type_t, heap, 1);

  return stack;
}

/* Release the stack object pointed by STACK.  */

void
cil_stack_free (cil_stack stack)
{
  VEC_free (cil_type_t, heap, stack->vec);
  XDELETE (stack);
}

/* Update the stack STACK as if its state was altered by executing the
   statement STMT.  */

void
cil_stack_after_stmt (cil_stack stack, cil_stmt stmt)
{
  VEC (cil_type_t, heap) *vstack = stack->vec;
  enum cil_opcode opcode = cil_opcode (stmt);
  cil_type_t op1, op2, res;
  tree type;
  size_t i;

  switch (opcode)
    {
    case CIL_CPBLK:
    case CIL_INITBLK:
      VEC_pop (cil_type_t, vstack);
      VEC_pop (cil_type_t, vstack);
      VEC_pop (cil_type_t, vstack);
      break;

    case CIL_BEQ:
    case CIL_BGE:
    case CIL_BGE_UN:
    case CIL_BGT:
    case CIL_BGT_UN:
    case CIL_BLE:
    case CIL_BLE_UN:
    case CIL_BLT:
    case CIL_BLT_UN:
    case CIL_BNE_UN:
    case CIL_STFLD:
    case CIL_STIND_I1:
    case CIL_STIND_I2:
    case CIL_STIND_I4:
    case CIL_STIND_I8:
    case CIL_STIND_R4:
    case CIL_STIND_R8:
    case CIL_STIND_I:
    case CIL_STOBJ:
    case CIL_STVEC:
      VEC_pop (cil_type_t, vstack);
      VEC_pop (cil_type_t, vstack);
      break;

    case CIL_ADD:
    case CIL_AND:
    case CIL_CEQ:
    case CIL_CGT:
    case CIL_CGT_UN:
    case CIL_CLT:
    case CIL_CLT_UN:
    case CIL_DIV:
    case CIL_DIV_UN:
    case CIL_MUL:
    case CIL_OR:
    case CIL_REM:
    case CIL_REM_UN:
    case CIL_SHL:
    case CIL_SHR:
    case CIL_SHR_UN:
    case CIL_SUB:
    case CIL_XOR:
      op2 = VEC_pop (cil_type_t, vstack);
      op1 = VEC_pop (cil_type_t, vstack);
      res = cil_binary_op_type (opcode, op1, op2);
      VEC_quick_push (cil_type_t, vstack, res);
      break;

    case CIL_BRFALSE:
    case CIL_BRTRUE:
    case CIL_INITOBJ:
    case CIL_POP:
    case CIL_STARG:
    case CIL_STLOC:
    case CIL_STSFLD:
    case CIL_SWITCH:
      VEC_pop (cil_type_t, vstack);
      break;

    case CIL_CONV_I1:
    case CIL_CONV_I2:
    case CIL_CONV_I4:
    case CIL_CONV_I8:
    case CIL_CONV_R4:
    case CIL_CONV_R8:
    case CIL_CONV_U1:
    case CIL_CONV_U2:
    case CIL_CONV_U4:
    case CIL_CONV_U8:
    case CIL_CONV_I:
    case CIL_CONV_U:
    case CIL_CONV_R_UN:
      VEC_pop (cil_type_t, vstack);

      switch (opcode)
	{
	case CIL_CONV_I1:   res = CIL_INT8;                break;
	case CIL_CONV_I2:   res = CIL_INT16;               break;
	case CIL_CONV_I4:   res = CIL_INT32;               break;
	case CIL_CONV_I8:   res = CIL_INT64;               break;
	case CIL_CONV_R4:   res = CIL_FLOAT32;             break;
	case CIL_CONV_R8:   res = CIL_FLOAT64;             break;
	case CIL_CONV_U1:   res = CIL_UNSIGNED_INT8;       break;
	case CIL_CONV_U2:   res = CIL_UNSIGNED_INT16;      break;
	case CIL_CONV_U4:   res = CIL_UNSIGNED_INT32;      break;
	case CIL_CONV_U8:   res = CIL_UNSIGNED_INT64;      break;
	case CIL_CONV_I:    res = CIL_NATIVE_INT;          break;
	case CIL_CONV_U:    res = CIL_NATIVE_UNSIGNED_INT; break;
	case CIL_CONV_R_UN: res = CIL_FLOAT;               break;
	default:
	  gcc_unreachable ();
	}

      VEC_quick_push (cil_type_t, vstack, res);
      break;

    case CIL_BR:
    case CIL_BREAK:
    case CIL_CKFINITE:
      break;

    case CIL_LDFLD:
      VEC_pop (cil_type_t, vstack);
      /* FALLTHROUGH */

    case CIL_LDSFLD:
      type = TREE_TYPE (cil_field (stmt));
      VEC_safe_push (cil_type_t, heap, vstack, type_to_cil_on_stack (type));
      break;

    case CIL_LDIND_I1:
    case CIL_LDIND_I2:
    case CIL_LDIND_I4:
    case CIL_LDIND_I8:
    case CIL_LDIND_U1:
    case CIL_LDIND_U2:
    case CIL_LDIND_U4:
    case CIL_LDIND_U8:
    case CIL_LDIND_R4:
    case CIL_LDIND_R8:
    case CIL_LDIND_I:
      VEC_pop (cil_type_t, vstack);

      switch (opcode)
	{
	case CIL_LDIND_I1: res = CIL_INT8;           break;
	case CIL_LDIND_I2: res = CIL_INT16;          break;
	case CIL_LDIND_I4: res = CIL_INT32;          break;
	case CIL_LDIND_I8: res = CIL_INT64;          break;
	case CIL_LDIND_U1: res = CIL_UNSIGNED_INT8;  break;
	case CIL_LDIND_U2: res = CIL_UNSIGNED_INT16; break;
	case CIL_LDIND_U4: res = CIL_UNSIGNED_INT32; break;
	case CIL_LDIND_U8: res = CIL_UNSIGNED_INT64; break;
	case CIL_LDIND_R4: res = CIL_FLOAT;          break;
	case CIL_LDIND_R8: res = CIL_FLOAT;          break;
	case CIL_LDIND_I:  res = CIL_NATIVE_INT;     break;
	default:
	  gcc_unreachable ();
	}

      VEC_quick_push (cil_type_t, vstack, res);
      break;

    case CIL_LDOBJ:
    case CIL_ARGLIST:
      VEC_pop (cil_type_t, vstack);
      VEC_quick_push (cil_type_t, vstack, CIL_VALUE_TYPE);
      break;

    case CIL_LDVEC:
      {
	tree type = cil_type (stmt);
	cil_type_t cil_type = vector_to_cil (type);
	VEC_pop (cil_type_t, vstack);
	VEC_quick_push (cil_type_t, vstack, cil_type);
      }
      break;

    case CIL_LDFLDA:
    case CIL_LOCALLOC:
      VEC_pop (cil_type_t, vstack);
      VEC_quick_push (cil_type_t, vstack, CIL_POINTER);
      break;

    case CIL_NEG:
    case CIL_NOT:
      op1 = VEC_pop (cil_type_t, vstack);

      if (cil_int_or_smaller_p (op1))
	res = CIL_INT32;
      else if (cil_long_p (op1))
	res = CIL_INT64;
      else if (cil_float_p (op1))
	res = CIL_FLOAT;
      else if (cil_native_integer_p (op1))
	res = CIL_NATIVE_INT;
      else
	gcc_unreachable ();

      VEC_quick_push (cil_type_t, vstack, res);
      break;

    case CIL_DUP:
      op1 = VEC_last (cil_type_t, vstack);
      VEC_safe_push (cil_type_t, heap, vstack, op1);
      break;

    case CIL_LDARG:
      type = DECL_ARG_TYPE (cil_var (stmt));
      VEC_safe_push (cil_type_t, heap, vstack, type_to_cil_on_stack (type));
      break;

    case CIL_LDLOC:
      type = TREE_TYPE (cil_var (stmt));
      VEC_safe_push (cil_type_t, heap, vstack, type_to_cil_on_stack (type));
      break;

    case CIL_LDARGA:
    case CIL_LDLOCA:
    case CIL_LDSFLDA:
      VEC_safe_push (cil_type_t, heap, vstack, CIL_POINTER);
      break;

    case CIL_LDC_I4:
      VEC_safe_push (cil_type_t, heap, vstack, CIL_INT32);
      break;

    case CIL_LDC_I8:
      VEC_safe_push (cil_type_t, heap, vstack, CIL_INT64);
      break;

    case CIL_LDC_R4:
    case CIL_LDC_R8:
      VEC_safe_push (cil_type_t, heap, vstack, CIL_FLOAT);
      break;

    case CIL_LDFTN:
      VEC_safe_push (cil_type_t, heap, vstack, CIL_NATIVE_INT);
      break;

    case CIL_RET:
      break;

    case CIL_RET_VAL:
	VEC_pop (cil_type_t, vstack);
      break;

    case CIL_CALL:
    case CIL_JMP:
    case CIL_NEWOBJ:
    case CIL_CALLI:
      i = cil_call_nargs (stmt);

      while (i-- != 0)
	VEC_pop (cil_type_t, vstack);

      type = TREE_TYPE (cil_call_ftype (stmt));

      if (!VOID_TYPE_P (type))
	VEC_safe_push (cil_type_t, heap, vstack, type_to_cil_on_stack (type));

      break;

    case CIL_VEC_CTOR:
      {
	tree type = cil_type (stmt);
	cil_type_t cil_type = vector_to_cil (type);
	unsigned int num_elts;

        if (cil_short_ctor (stmt))
          num_elts = 1;
        else
          num_elts = TYPE_VECTOR_SUBPARTS (type);

	while (num_elts-- != 0)
	  VEC_pop (cil_type_t, vstack);

	VEC_safe_push (cil_type_t, heap, vstack, cil_type);
      }
      break;

    case CIL_ASM:
      /* TODO: Specify a way for asm statements to tell the compiler how
	 many stack slots they need.  */
      break;

    default:
      gcc_unreachable ();
    }

  /* Update the STACK field as VEC_* operations may alter the source.  */
  stack->vec = vstack;
}

/* Return the depth of the stack STACK.  */

size_t
cil_stack_depth (const_cil_stack stack)
{
  return VEC_length (cil_type_t, stack->vec);
}

/* Return the type of the element on top of the stack.  */

cil_type_t
cil_stack_top (const_cil_stack stack)
{
  return VEC_last (cil_type_t, stack->vec);
}

/* Peek the value I-th location of the stack STACK.  The location is considered
   in reverse order, i.e. location 0 is the top of the stack, 1 the location
   just below it and so on.  */

cil_type_t
cil_stack_peek (const_cil_stack stack, size_t i)
{
  size_t depth = VEC_length (cil_type_t, stack->vec);

  return VEC_index (cil_type_t, stack->vec, depth - 1 - i);
}

/******************************************************************************
 * CIL types functions                                                        *
 ******************************************************************************/

/* Return TRUE if the type TYPE is a non-native integer type, FALSE
   otherwise.  */

bool
cil_integer_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_INT8:
    case CIL_INT16:
    case CIL_INT32:
    case CIL_INT64:
    case CIL_UNSIGNED_INT8:
    case CIL_UNSIGNED_INT16:
    case CIL_UNSIGNED_INT32:
    case CIL_UNSIGNED_INT64:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a native integer type, FALSE otherwise.  */

bool
cil_native_integer_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_NATIVE_INT:
    case CIL_NATIVE_UNSIGNED_INT:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a floating-point type, FALSE otherwise.  */

bool
cil_float_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_FLOAT:
    case CIL_FLOAT32:
    case CIL_FLOAT64:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a complex type, FALSE otherwise.  */

bool
cil_complex_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_CMPLXI8:
    case CIL_CMPLXI16:
    case CIL_CMPLXI32:
    case CIL_CMPLXI64:
    case CIL_CMPLXU8:
    case CIL_CMPLXU16:
    case CIL_CMPLXU32:
    case CIL_CMPLXU64:
    case CIL_CMPLXF32:
    case CIL_CMPLXF64:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a vector type, FALSE otherwise.  */

bool
cil_vector_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_V2SI:
    case CIL_V4HI:
    case CIL_V8QI:
    case CIL_V2SF:
    case CIL_V2DI:
    case CIL_V4SI:
    case CIL_V8HI:
    case CIL_V16QI:
    case CIL_V4SF:
    case CIL_V2DF:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a pointer type (or native integer),
   FALSE otherwise.  */

bool
cil_pointer_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_NATIVE_INT:
    case CIL_NATIVE_UNSIGNED_INT:
    case CIL_POINTER:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a 32-bit or smaller integer, FALSE
   otherwise.  */

bool
cil_int_or_smaller_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_INT8:
    case CIL_INT16:
    case CIL_INT32:
    case CIL_UNSIGNED_INT8:
    case CIL_UNSIGNED_INT16:
    case CIL_UNSIGNED_INT32:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type TYPE is a 64-bit integer, FALSE otherwise.  */

bool
cil_long_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_INT64:
    case CIL_UNSIGNED_INT64:
      return true;

    default:
      return false;
    }
}

/* Returns TRUE if the type TYPE is unsigned, FALSE otherwise.  */

bool
cil_unsigned_int_p (cil_type_t type)
{
  switch (type)
    {
    case CIL_UNSIGNED_INT8:
    case CIL_UNSIGNED_INT16:
    case CIL_UNSIGNED_INT32:
    case CIL_UNSIGNED_INT64:
    case CIL_NATIVE_UNSIGNED_INT:
      return true;

    default:
      return true;
    }
}

/* Return the CIL stack representation for scalar type TYPE.  */

cil_type_t
scalar_to_cil (const_tree type)
{
  unsigned HOST_WIDE_INT size;

  if (cil_pointer_type_p (type))
      return CIL_POINTER;
  else
    switch (TREE_CODE (type))
      {
      case ENUMERAL_TYPE:
        return CIL_INT32;

      case INTEGER_TYPE:
        size = tree_low_cst (TYPE_SIZE (type), 1);

        switch (size)
          {
	  case 8:  return TYPE_UNSIGNED (type) ? CIL_UNSIGNED_INT8 : CIL_INT8;
	  case 16: return TYPE_UNSIGNED (type) ? CIL_UNSIGNED_INT16 : CIL_INT16;
	  case 32: return TYPE_UNSIGNED (type) ? CIL_UNSIGNED_INT32 : CIL_INT32;
	  case 64: return TYPE_UNSIGNED (type) ? CIL_UNSIGNED_INT64 : CIL_INT64;
	  default:
	    internal_error ("Unsupported integer size "
			    HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
          }

      case REAL_TYPE:
        size = tree_low_cst (TYPE_SIZE (type), 1);

        switch (size)
          {
	  case 32: return CIL_FLOAT32;
	  case 64: return CIL_FLOAT64;
	  default:
	    internal_error ("Unsupported float size "
			    HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
          }

      case BOOLEAN_TYPE:
        return CIL_INT8;

      default:
        gcc_unreachable ();
      }
}

/* Return the CIL stack representation for scalar type TYPE.  */

cil_type_t
complex_to_cil (const_tree type)
{
  tree elem_type;

  gcc_assert (TREE_CODE (type) == COMPLEX_TYPE);

  elem_type = TYPE_MAIN_VARIANT (TREE_TYPE (type));

  switch (scalar_to_cil (elem_type))
    {
    case CIL_INT8:  return CIL_CMPLXI8;
    case CIL_INT16: return CIL_CMPLXI16;
    case CIL_INT32: return CIL_CMPLXI32;
    case CIL_INT64: return CIL_CMPLXI64;

    case CIL_UNSIGNED_INT8:  return CIL_CMPLXU8;
    case CIL_UNSIGNED_INT16: return CIL_CMPLXU16;
    case CIL_UNSIGNED_INT32: return CIL_CMPLXU32;
    case CIL_UNSIGNED_INT64: return CIL_CMPLXU64;

    case CIL_FLOAT32: return CIL_CMPLXF32;
    case CIL_FLOAT64: return CIL_CMPLXF64;

    default:
      gcc_unreachable ();
    }
}

/* Return the CIL stack representation for vector type TYPE.  */

cil_type_t
vector_to_cil (const_tree type)
{
  cil_type_t ret = CIL_NO_TYPE;
  tree innertype;
  HOST_WIDE_INT innersize;
  HOST_WIDE_INT vec_size;

  gcc_assert (TREE_CODE (type) == VECTOR_TYPE);

  innertype = TYPE_MAIN_VARIANT (TREE_TYPE (type));
  innersize = tree_low_cst (TYPE_SIZE (innertype), 1);
  vec_size  = tree_low_cst (TYPE_SIZE (type), 1);

  if ((TREE_CODE (innertype) == INTEGER_TYPE) ||
      (TREE_CODE (innertype) == POINTER_TYPE))
    {
      if (vec_size == 128)  /* 16-byte vectors */
	{
	  switch (innersize)
	    {
	    case  8: ret = CIL_V16QI; break;
	    case 16: ret = CIL_V8HI;  break;
	    case 32: ret = CIL_V4SI;  break;
	    case 64: ret = CIL_V2DI;  break;
	    default: break;
	    }
	}
      else if (vec_size == 64)  /* 8-byte vectors */
	{
	  switch (innersize)
	    {
	    case  8: ret = CIL_V8QI; break;
	    case 16: ret = CIL_V4HI;  break;
	    case 32: ret = CIL_V2SI;  break;
	    default: break;
	    }
	}
      else if (vec_size == 32)  /* 4-byte vectors */
	{
	  switch (innersize)
	    {
	    case  8: ret = CIL_V4QI; break;
	    case 16: ret = CIL_V2HI;  break;
	    default: break;
	    }
	}
    }
  else if (TREE_CODE (innertype) == REAL_TYPE)
    {
      if (vec_size == 128)
	{
	  if (innersize == 32)
	    ret = CIL_V4SF;
	  else if (innersize == 64)
	    ret = CIL_V2DF;
	}
      else if (vec_size == 64)
	if (innersize == 32)
	  ret = CIL_V2SF;
    }
  else
    {
      fprintf (stderr, "Vector of '%s'\n",
	       tree_code_name[TREE_CODE (innertype)]);
    }
  if (ret == CIL_NO_TYPE)
    internal_error ("Unsupported vector type %ld/%ld", vec_size, innersize);

  return ret;
}

/* Return the CIL stack representation for type TYPE.  */

cil_type_t
type_to_cil_on_stack (const_tree type)
{
  cil_type_t cil_type;

  if (cil_value_type_p (type))
    cil_type = CIL_VALUE_TYPE;
  else if (cil_vector_type_p  (type))
    cil_type = vector_to_cil(type);
  else
    {
      cil_type = scalar_to_cil(type);

      if (cil_type == CIL_FLOAT32 || cil_type == CIL_FLOAT64)
	cil_type = CIL_FLOAT;
    }

  return cil_type;
}

/* Return the CIL representation for type TYPE.  */

cil_type_t
type_to_cil (const_tree type)
{
  if (TYPE_MAIN_VARIANT (type) == cil32_arg_iterator_type)
    return CIL_ARGITER;
  else if (TREE_CODE (type) == VOID_TYPE)
    return CIL_VOID;
  else if (TREE_CODE (type) == COMPLEX_TYPE)
    return complex_to_cil(type);
  else if (cil_value_type_p (type))
    return CIL_VALUE_TYPE;
  else if (cil_vector_type_p  (type))
    return vector_to_cil(type);
  else
    return scalar_to_cil(type);
}

/* Return TRUE if the type specified by TYPE is loaded on the stack as a
   value type, FALSE otherwise.  */

bool
cil_value_type_p (const_tree type)
{
  switch (TREE_CODE (type))
    {
    case ARRAY_TYPE:
      if (!TYPE_DOMAIN (type) || ARRAY_TYPE_VARLENGTH (type))
	return false; /* Incomplete array types are treated life pointers.  */

      /* FALLTHROUGH */
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case COMPLEX_TYPE:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the type specified by TYPE is loaded on the stack as a
   vector type, FALSE otherwise.  */

bool
cil_vector_type_p (const_tree type)
{
  if (TREE_CODE (type) == VECTOR_TYPE)
    return true;
  else
    return false;
}

/* Return true if we treat the type TYPE as a pointer, false otherwise.  */

bool
cil_pointer_type_p (const_tree type)
{
  if (TREE_CODE (type) == ARRAY_TYPE)
    {
      if (!TYPE_DOMAIN (type) || ARRAY_TYPE_VARLENGTH (type))
	return true;
    }
  else if (POINTER_TYPE_P (type))
    return true;

  return false;
}

/* Return the result type of a binary operation OP with operands of types
  TYPE1 and TYPE2.  This function doesn't do any validation of the passed
  operation as it assumes that it is fed valid CIL code.  */

static cil_type_t
cil_binary_op_type (enum cil_opcode op, cil_type_t type1, cil_type_t type2)
{
  /* TODO: When pointers will be specialized this function should be
     altered to deal with them properly.  */

  switch (op)
    {
    case CIL_ADD:
    case CIL_AND: /* TODO: Could be improved */
    case CIL_DIV:
    case CIL_DIV_UN:
    case CIL_MUL:
    case CIL_OR: /* TODO: Could be improved */
    case CIL_REM:
    case CIL_REM_UN:
    case CIL_SUB:
    case CIL_XOR: /* TODO: Could be improved */
      if (cil_int_or_smaller_p (type1))
	{
	  if (cil_int_or_smaller_p (type2))
	    return CIL_INT32;
	  else if (cil_native_integer_p (type2))
	    return CIL_NATIVE_INT;
	  else /* CIL_POINTER */
	    return type2;
	}
      else if (cil_long_p (type1))
	return CIL_INT64;
      else if (cil_native_integer_p (type1))
	{
	  if (type2 == CIL_POINTER)
	    return type2;
	  else
	    return CIL_NATIVE_INT;
	}
      else if (cil_float_p (type1))
	return CIL_FLOAT;
      else if (type1 == CIL_POINTER)
	{
	  if (type2 == CIL_POINTER)
	    return CIL_NATIVE_INT;
	  else
	    return type1;
	}
      else if (cil_vector_p (type1) && (type1 == type2))
	return type1;
      else
	gcc_unreachable ();

      break;

    case CIL_CEQ:
    case CIL_CGT:
    case CIL_CGT_UN:
    case CIL_CLT:
    case CIL_CLT_UN:
      return CIL_INT32;

    case CIL_SHL:
    case CIL_SHR:
    case CIL_SHR_UN:
      if (cil_int_or_smaller_p (type1))
	return CIL_INT32;
      else if (cil_long_p (type1))
	return CIL_INT64;
      else
	return CIL_NATIVE_INT;

      break;

    default:
      internal_error ("Unsupported operation for cil_binary_op_type()");
    }
}

/* Create a new stacks-to-basic-blocks mapping structure.  */

cil_bb_stacks
cil_bb_stacks_create (void)
{
  cil_bb_stacks bbs = XNEW (struct cil_bb_stacks_d);

  bbs->stacks = XCNEWVEC (cil_stack, last_basic_block);

  return bbs;
}

/* Destroy a stacks-to-basic-blocks mapping structure, also free all the stacks
   it holds.  */

void
cil_bb_stacks_destroy (cil_bb_stacks bbs)
{
  size_t i;

  for (i = 0; i < (size_t) last_basic_block; i++)
    {
      if (bbs->stacks[i] != NULL)
	cil_stack_free (bbs->stacks[i]);
    }

  XDELETEVEC (bbs->stacks);
  XDELETE (bbs);
}

/* Returns the stack associated with the basic block BB if present, otherwise
   a new empty stack will be associated with the basic block BB and
   returned.  */

cil_stack
cil_stack_for_bb (const_cil_bb_stacks bbs, basic_block bb)
{
  if (bbs->stacks[bb->index] == NULL)
    bbs->stacks[bb->index] = cil_stack_create ();

  return bbs->stacks[bb->index];
}

/* Sets to STACK the stack associated with the basic block BB.  The stack is
   copied so that the copy can be manipulated later without impacting the
   original.  If a stack was already associated with the basic block then
   it is fred and replaced.  */

void
cil_set_stack_for_bb (cil_bb_stacks bbs, basic_block bb, cil_stack stack)
{
  if (bbs->stacks[bb->index] == stack)
    return;
  else if (bbs->stacks[bb->index] != NULL)
    cil_stack_free (bbs->stacks[bb->index]);

  bbs->stacks[bb->index] = cil_stack_copy (stack);
}

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
