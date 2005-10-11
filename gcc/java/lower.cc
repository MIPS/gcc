// Lower a block of bytecode to GENERIC.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

// This include must come first.
#include "java/glue.hh"

#include "java/tree.hh"
#include "bytecode/insns.hh"
#include "verify.h"



static inline jint
get1s (const uint8 *bytes, int &pc)
{
  int8 *sbytes = (int8 *) bytes;
  return sbytes[pc++];
}

static inline jint
get1u (const uint8 *bytes, int &pc)
{
  return bytes[pc++];
}

static inline jint
get2s (const uint8 *bytes, int &pc)
{
  int8 *sbytes = (int8 *) bytes;
  jint result = (sbytes[pc] << 8) | (sbytes[pc + 1] & 0xff);
  pc += 2;
  return result;
}

static inline jint
get2u (const uint8 *bytes, int &pc)
{
  jint result = (bytes[pc] << 8) | bytes[pc + 1];
  pc += 2;
  return result;
}

static inline jint
get4 (const uint8 *bytes, int &pc)
{
  jint result = (bytes[pc] << 24
		 | bytes[pc + 1] << 16
		 | bytes[pc + 2] << 8
		 | bytes[pc + 3]);
  pc += 4;
  return result;
}



// This function is very large, so we put it into its own file for
// clarity.
void
tree_generator::visit_bytecode_block (model_bytecode_block *block,
				      int max_stack, int max_locals,
				      int length, const uint8 *bytes)
{
  block->verify (method);

  assert (! labels);
  labels = new tree[length];
  stack_top = 0;
  assert (! stack_types);
  stack_types = new tree[max_stack];

  // Set up DECLs representing each stack and local variable slot.
  stack_slots = new tree[max_stack];
  for (int i = 0; i < max_stack; ++i)
    {
      char name[20];
      sprintf (name, "stack%d", i);
      stack_slots[i] = add_temporary (get_identifier (name), type_slot_union);
    }
  local_slots = new tree[max_locals];
  for (int i = 0; i < max_locals; ++i)
    {
      char name[20];
      sprintf (name, "local%d", i);
      local_slots[i] = add_temporary (get_identifier (name), type_slot_union);
    }

  model_unit_class *unit
    = assert_cast<model_unit_class *> (method->get_declaring_class ()->get_compilation_unit ());
  constant_pool *cpool = unit->get_constant_pool ();

  tree body = alloc_stmt_list ();
  tree_stmt_iterator statements = tsi_start (body);

  unsigned char *flags = block->get_flags ();

  int pc = 0;
  while (pc < length)
    {
      // Don't bother compiling dead code.
      if ((flags[pc] & VERIFY_SEEN) == 0)
	continue;

      // If this location is a branch target, link its corresponding
      // label into the instruction stream.
      if ((flags[pc] & VERIFY_BRANCH_TARGET) != 0)
	{
	  tree label_decl = find_label (pc);
	  tsi_link_after (&statements, build1 (LABEL_EXPR, void_type_node,
					       label_decl),
			  TSI_CONTINUE_LINKING);
	}

      tree insn = NULL_TREE;
      java_opcode op = (java_opcode) bytes[pc++];
      switch (op)
	{
	case op_nop:
	  insn = build_empty_stmt ();
	  break;

	case op_aconst_null:
	  insn = push (null_pointer_node);
	  break;

	case op_iconst_m1:
	case op_iconst_0:
	case op_iconst_1:
	case op_iconst_2:
	case op_iconst_3:
	case op_iconst_4:
	case op_iconst_5:
	  insn = push (build_int (op - op_iconst_0));
	  break;

	case op_lconst_0:
	case op_lconst_1:
	  insn = push (build_long (op - op_lconst_0));
	  break;

	case op_fconst_0:
	case op_fconst_1:
	case op_fconst_2:
	  insn = push (handle_float (jfloat (op - op_fconst_0)));
	  break;

	case op_dconst_0:
	case op_dconst_1:
	  insn = push (handle_double (jdouble (op - op_dconst_0)));
	  break;

	case op_bipush:
	  insn = push (build_int (get1s (bytes, pc)));
	  break;

	case op_sipush:
	  insn = push (build_int (get2s (bytes, pc)));
	  break;

	case op_ldc:
	  insn = handle_ldc (cpool, get1u (bytes, pc));
	  break;

	case op_ldc_w:
	case op_ldc2_w:
	  insn = handle_ldc (cpool, get2u (bytes, pc));
	  break;

	case op_iload:
	  insn = push (load (get1u (bytes, pc), type_jint));
	  break;

	case op_lload:
	  insn = push (load (get1u (bytes, pc), type_jlong));
	  break;

	case op_fload:
	  insn = push (load (get1u (bytes, pc), type_jfloat));
	  break;

	case op_dload:
	  insn = push (load (get1u (bytes, pc), type_jdouble));
	  break;

	case op_aload:
	  insn = push (load (get1u (bytes, pc), type_object_ptr));
	  break;

	case op_iload_0:
	case op_iload_1:
	case op_iload_2:
	case op_iload_3:
	  insn = push (load (op - op_iload_0, type_jint));
	  break;

	case op_lload_0:
	case op_lload_1:
	case op_lload_2:
	case op_lload_3:
	  insn = push (load (op - op_lload_0, type_jlong));
	  break;

	case op_fload_0:
	case op_fload_1:
	case op_fload_2:
	case op_fload_3:
	  insn = push (load (op - op_fload_0, type_jfloat));
	  break;

	case op_dload_0:
	case op_dload_1:
	case op_dload_2:
	case op_dload_3:
	  insn = push (load (op - op_dload_0, type_jdouble));
	  break;

	case op_aload_0:
	case op_aload_1:
	case op_aload_2:
	case op_aload_3:
	  insn = push (load (op - op_aload_0, type_object_ptr));
	  break;

	case op_iaload:
	  insn = array_access (primitive_int_type, type_jint);
	  break;

	case op_laload:
	  insn = array_access (primitive_long_type, type_jlong);
	  break;

	case op_faload:
	  insn = array_access (primitive_float_type, type_jfloat);
	  break;

	case op_daload:
	  insn = array_access (primitive_double_type, type_jdouble);
	  break;

	case op_aaload:
	  // FIXME: we shouldn't even require Object with the BC ABI.
	  insn = array_access (global->get_compiler ()->java_lang_Object (),
			       type_object_ptr);
	  break;

	case op_baload:
	  insn = array_access (primitive_byte_type, type_jbyte);
	  break;

	case op_caload:
	  insn = array_access (primitive_char_type, type_jchar);
	  break;

	case op_saload:
	  insn = array_access (primitive_short_type, type_jshort);
	  break;

	case op_istore:
	  insn = store (get1u (bytes, pc), type_jint);
	  break;

	case op_lstore:
	  insn = store (get1u (bytes, pc), type_jlong);
	  break;

	case op_fstore:
	  insn = store (get1u (bytes, pc), type_jfloat);
	  break;

	case op_dstore:
	  insn = store (get1u (bytes, pc), type_jdouble);
	  break;

	case op_astore:
	  insn = store (get1u (bytes, pc), type_object_ptr);
	  break;

	case op_istore_0:
	case op_istore_1:
	case op_istore_2:
	case op_istore_3:
	  insn = store (op - op_istore_0, type_jint);
	  break;

	case op_lstore_0:
	case op_lstore_1:
	case op_lstore_2:
	case op_lstore_3:
	  insn = store (op - op_lstore_0, type_jlong);
	  break;

	case op_fstore_0:
	case op_fstore_1:
	case op_fstore_2:
	case op_fstore_3:
	  insn = store (op - op_fstore_0, type_jfloat);
	  break;

	case op_dstore_0:
	case op_dstore_1:
	case op_dstore_2:
	case op_dstore_3:
	  insn = store (op - op_dstore_0, type_jdouble);
	  break;

	case op_astore_0:
	case op_astore_1:
	case op_astore_2:
	case op_astore_3:
	  insn = store (op - op_astore_0, type_object_ptr);
	  break;

	case op_iastore:
	  insn = array_store (primitive_int_type, type_jint);
	  break;

	case op_lastore:
	  insn = array_store (primitive_long_type, type_jlong);
	  break;

	case op_fastore:
	  insn = array_store (primitive_float_type, type_jfloat);
	  break;

	case op_dastore:
	  insn = array_store (primitive_double_type, type_jdouble);
	  break;

	case op_aastore:
	  // FIXME: we shouldn't even require Object with the BC ABI.
	  insn = array_store (global->get_compiler ()->java_lang_Object (),
			      type_object_ptr);
	  break;

	case op_bastore:
	  insn = array_store (primitive_byte_type, type_jbyte);
	  break;

	case op_castore:
	  insn = array_store (primitive_char_type, type_jchar);
	  break;

	case op_sastore:
	  insn = array_store (primitive_short_type, type_jshort);
	  break;

	case op_pop:
	  // We don't have to generate code here.
	  --stack_top;
	  break;

	case op_pop2:
	  // We don't have to generate code here.
	  if (type_wide_p (stack_types[stack_top - 1]))
	    --stack_top;
	  else
	    stack_top -= 2;
	  break;

	case op_dup:
	  {
	    tree type = stack_types[stack_top - 1];
	    tree value = find_slot (stack_top - 1, type);
	    insn = push (value);
	  }
	  break;

	case op_dup_x1:
	  {
	    tree type1 = stack_types[stack_top - 1];
	    tree type2 = stack_types[stack_top - 2];
	    tree val1 = pop (type1);
	    tree val2 = pop (type2);
	    tsi_link_after (&statements, push (val1),
			    TSI_CONTINUE_LINKING);
	    tsi_link_after (&statements, push (val2),
			    TSI_CONTINUE_LINKING);
	    insn = push (val1);
	  }
	  break;

	case op_dup_x2:
	  {
	    tree type1 = stack_types[stack_top - 1];
	    tree type2 = stack_types[stack_top - 2];

	    if (type_wide_p (type2))
	      {
		// Wide Narrow -> Narrow Wide Narrow
		tree val1 = pop (type1);
		tree val2 = pop (type2);
		tsi_link_after (&statements, push (val1),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		insn = push (val1);
	      }
	    else
	      {
		// N3 N2 N1 -> N1 N3 N2 N1
		tree type3 = stack_types[stack_top - 3];

		tree val1 = pop (type1);
		tree val2 = pop (type2);
		tree val3 = pop (type3);
		tsi_link_after (&statements, push (val1),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val3),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		insn = push (val1);
	      }
	  }
	  break;

	case op_dup2:
	  {
	    tree type1 = stack_types[stack_top - 1];
	    if (type_wide_p (type1))
	      {
		// W1 -> W1 W1
		tree val1 = find_slot (stack_top - 1, type1);
		insn = push (val1);
	      }
	    else
	      {
		// N2 N1 -> N2 N1 N2 N1
		tree type2 = stack_types[stack_top - 2];
		tree val1 = find_slot (stack_top - 1, type1);
		tree val2 = find_slot (stack_top - 2, type2);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		insn = push (val1);
	      }
	  }
	  break;

	case op_dup2_x1:
	  {
	    tree type1 = stack_types[stack_top - 1];
	    tree type2 = stack_types[stack_top - 2];
	    if (type_wide_p (type1))
	      {
		// N W -> W N W
		tree val1 = pop (type1);
		tree val2 = pop (type2);
		
		tsi_link_after (&statements, push (val1),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		insn = push (val1);
	      }
	    else
	      {
		// N3 N2 N1 -> N2 N1 N3 N2 N1
		tree type3 = stack_types[stack_top - 3];

		tree val1 = pop (type1);
		tree val2 = pop (type2);
		tree val3 = pop (type3);

		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val1),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val3),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
		insn = push (val1);
	      }
	  }
	  break;

	case op_dup2_x2:
	  {
	    tree type1 = stack_types[stack_top - 1];
	    tree type2 = stack_types[stack_top - 2];

	    tree val1;

	    if (type_wide_p (type1) && type_wide_p (type2))
	      {
		// W2 W1 -> W1 W2 W1
		val1 = pop (type1);
		tree val2 = pop (type2);

		tsi_link_after (&statements, push (val1),
				TSI_CONTINUE_LINKING);
		tsi_link_after (&statements, push (val2),
				TSI_CONTINUE_LINKING);
	      }
	    else
	      {
		tree type3 = stack_types[stack_top - 3];

		if (type_wide_p (type1))
		  {
		    // N3 N2 W1 -> W1 N3 N2 W1
		    val1 = pop (type1);
		    tree val2 = pop (type2);
		    tree val3 = pop (type3);

		    tsi_link_after (&statements, push (val1),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val3),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val2),
				    TSI_CONTINUE_LINKING);
		  }
		else if (type_wide_p (type3))
		  {
		    // W3 N2 N1 -> N2 N1 W3 N2 N1
		    val1 = pop (type1);
		    tree val2 = pop (type2);
		    tree val3 = pop (type3);

		    tsi_link_after (&statements, push (val2),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val1),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val3),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val2),
				    TSI_CONTINUE_LINKING);
		  }
		else
		  {
		    // N4 N3 N2 N1 -> N2 N1 N4 N3 N2 N1
		    tree type4 = stack_types[stack_top - 4];

		    val1 = pop (type1);
		    tree val2 = pop (type2);
		    tree val3 = pop (type3);
		    tree val4 = pop (type4);

		    tsi_link_after (&statements, push (val2),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val1),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val4),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val3),
				    TSI_CONTINUE_LINKING);
		    tsi_link_after (&statements, push (val2),
				    TSI_CONTINUE_LINKING);
		  }
	      }

	    insn = push (val1);
	  }
	  break;

	case op_swap:
	  {
	    // See the comments in find_slot to understand this code.
	    tree expr1 = find_slot (stack_top - 1, stack_types[stack_top - 1]);
	    tree expr2 = find_slot (stack_top - 2, stack_types[stack_top - 2]);

	    tree temp = add_temporary (gcc_builtins->get_symbol (),
				       TREE_TYPE (expr1));

	    tree decl1 = find_slot (stack_top - 2, TREE_TYPE (expr1));
	    tree decl2 = find_slot (stack_top - 1, TREE_TYPE (expr2));

	    tsi_link_after (&statements,
			    build2 (MODIFY_EXPR, TREE_TYPE (expr1),
				    temp, expr1),
			    TSI_CONTINUE_LINKING);
	    tsi_link_after (&statements,
			    build2 (MODIFY_EXPR, TREE_TYPE (expr2),
				    decl2, expr2),
			    TSI_CONTINUE_LINKING);
	    insn = build2 (MODIFY_EXPR, TREE_TYPE (expr1),
			   decl1, expr1);
	  }
	  break;

	case op_iadd:
	  insn = binop (PLUS_EXPR, type_jint);
	  break;

	case op_ladd:
	  insn = binop (PLUS_EXPR, type_jlong);
	  break;

	case op_fadd:
	  insn = binop (PLUS_EXPR, type_jfloat);
	  break;

	case op_dadd:
	  insn = binop (PLUS_EXPR, type_jdouble);
	  break;

	case op_isub:
	  insn = binop (MINUS_EXPR, type_jint);
	  break;

	case op_lsub:
	  insn = binop (MINUS_EXPR, type_jlong);
	  break;

	case op_fsub:
	  insn = binop (MINUS_EXPR, type_jfloat);
	  break;

	case op_dsub:
	  insn = binop (MINUS_EXPR, type_jdouble);
	  break;

	case op_imul:
	  insn = binop (MULT_EXPR, type_jint);
	  break;

	case op_lmul:
	  insn = binop (MULT_EXPR, type_jlong);
	  break;

	case op_fmul:
	  insn = binop (MULT_EXPR, type_jfloat);
	  break;

	case op_dmul:
	  insn = binop (MULT_EXPR, type_jdouble);
	  break;

	case op_idiv:
	  {
	    tree val2 = pop (type_jint);
	    tree val1 = pop (type_jint);
	    insn = push (build_divide (type_jint, val1, val2));
	  }
	  break;

	case op_ldiv:
	  {
	    tree val2 = pop (type_jlong);
	    tree val1 = pop (type_jlong);
	    insn = push (build_divide (type_jlong, val1, val2));
	  }
	  break;

	case op_fdiv:
	  // No need to use build_divide here, as we know the result
	  // is float.
	  insn = binop (RDIV_EXPR, type_jfloat);
	  break;

	case op_ddiv:
	  // No need to use build_divide here, as we know the result
	  // is double.
	  insn = binop (RDIV_EXPR, type_jdouble);
	  break;

	case op_irem:
	  {
	    tree val2 = pop (type_jint);
	    tree val1 = pop (type_jint);
	    insn = push (build_mod (type_jint, val1, val2));
	  }
	  break;

	case op_lrem:
	  {
	    tree val2 = pop (type_jlong);
	    tree val1 = pop (type_jlong);
	    insn = push (build_mod (type_jlong, val1, val2));
	  }
	  break;

	case op_frem:
	  {
	    tree val2 = pop (type_jfloat);
	    tree val1 = pop (type_jfloat);
	    tree call = build3 (CALL_EXPR,
				type_jfloat,
				build_address_of (built_in_decls[BUILT_IN_FMODF]),
				tree_cons (NULL_TREE, val1,
					   build_tree_list (NULL_TREE,
							    val2)),
				NULL_TREE);
	    TREE_SIDE_EFFECTS (call) = 1;
	    insn = push (call);
	  }
	  break;

	case op_drem:
	  {
	    tree val2 = pop (type_jdouble);
	    tree val1 = pop (type_jdouble);
	    tree call = build3 (CALL_EXPR,
				type_jdouble,
				build_address_of (built_in_decls[BUILT_IN_FMOD]),
				tree_cons (NULL_TREE, val1,
					   build_tree_list (NULL_TREE,
							    val2)),
				NULL_TREE);
	    TREE_SIDE_EFFECTS (call) = 1;
	    insn = push (call);
	  }
	  break;

	case op_ineg:
	  {
	    tree expr = pop (type_jint);
	    insn = push (build1 (NEGATE_EXPR, type_jint, expr));
	  }
	  break;

	case op_lneg:
	  {
	    tree expr = pop (type_jlong);
	    insn = push (build1 (NEGATE_EXPR, type_jlong, expr));
	  }
	  break;

	case op_fneg:
	  {
	    tree expr = pop (type_jfloat);
	    insn = push (build1 (NEGATE_EXPR, type_jfloat, expr));
	  }
	  break;

	case op_dneg:
	  {
	    tree expr = pop (type_jdouble);
	    insn = push (build1 (NEGATE_EXPR, type_jdouble, expr));
	  }
	  break;

	case op_ishl:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jint);
	    insn = push (build2 (LSHIFT_EXPR, type_jint, val,
				 build2 (BIT_AND_EXPR,
					 type_jint,
					 shift, build_int (0x1f))));
	  }
	  break;

	case op_lshl:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jlong);
	    insn = push (build2 (LSHIFT_EXPR, type_jlong, val,
				 build2 (BIT_AND_EXPR,
					 type_jint,
					 shift, build_int (0x3f))));
	  }
	  break;

	case op_ishr:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jint);
	    insn = push (build2 (RSHIFT_EXPR, type_jint, val,
				 build2 (BIT_AND_EXPR,
					 type_jint,
					 shift, build_int (0x1f))));
	  }
	  break;

	case op_lshr:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jlong);
	    insn = push (build2 (RSHIFT_EXPR, type_jlong, val,
				 build2 (BIT_AND_EXPR,
					 type_jint,
					 shift, build_int (0x3f))));
	  }
	  break;

	case op_iushr:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jint);
	    val = convert (type_juint, val);

	    tree expr = build2 (RSHIFT_EXPR, type_juint, val,
				build2 (BIT_AND_EXPR,
					type_jint,
					shift, build_int (0x1f)));
	    insn = push (convert (type_jint, expr));
	  }
	  break;

	case op_lushr:
	  {
	    tree shift = pop (type_jint);
	    tree val = pop (type_jlong);
	    val = convert (type_julong, val);

	    tree expr = build2 (RSHIFT_EXPR, type_julong, val,
				build2 (BIT_AND_EXPR,
					type_jint,
					shift, build_int (0x3f)));
	    insn = push (convert (type_jlong, expr));
	  }
	  break;

	case op_iand:
	  insn = binop (BIT_AND_EXPR, type_jint);
	  break;

	case op_land:
	  insn = binop (BIT_AND_EXPR, type_jlong);
	  break;

	case op_ior:
	  insn = binop (BIT_IOR_EXPR, type_jint);
	  break;

	case op_lor:
	  insn = binop (BIT_IOR_EXPR, type_jlong);
	  break;

	case op_ixor:
	  insn = binop (BIT_XOR_EXPR, type_jint);
	  break;

	case op_lxor:
	  insn = binop (BIT_XOR_EXPR, type_jlong);
	  break;

	case op_iinc:
	  {
	    int index = get1u (bytes, pc);
	    int amount = get1s (bytes, pc);
	    tree decl = find_generic_slot (index, type_jint, local_slots);
	    insn = build2 (MODIFY_EXPR, type_jint, decl,
			   build2 (PLUS_EXPR, type_jint,
				   decl, build_int (amount)));
	  }
	  break;

	case op_i2l:
	  insn = cast (type_jint, type_jlong);
	  break;

	case op_i2f:
	  insn = cast (type_jint, type_jfloat);
	  break;

	case op_i2d:
	  insn = cast (type_jint, type_jdouble);
	  break;

	case op_l2i:
	  insn = cast (type_jlong, type_jint);
	  break;

	case op_l2f:
	  insn = cast (type_jlong, type_jfloat);
	  break;

	case op_l2d:
	  insn = cast (type_jlong, type_jdouble);
	  break;

	case op_f2i:
	  {
	    tree expr = pop (type_jfloat);
	    insn = handle_convert (primitive_int_type, expr);
	  }
	  break;

	case op_f2l:
	  {
	    tree expr = pop (type_jfloat);
	    insn = handle_convert (primitive_long_type, expr);
	  }
	  break;

	case op_f2d:
	  insn = cast (type_jfloat, type_jdouble);
	  break;

	case op_d2i:
	  {
	    tree expr = pop (type_jdouble);
	    insn = handle_convert (primitive_int_type, expr);
	  }
	  break;

	case op_d2l:
	  {
	    tree expr = pop (type_jdouble);
	    insn = handle_convert (primitive_long_type, expr);
	  }
	  break;

	case op_d2f:
	  insn = cast (type_jdouble, type_jfloat);
	  break;

	case op_i2b:
	  {
	    tree value = pop (type_jint);
	    insn = push (convert (type_jbyte, value));
	  }
	  break;

	case op_i2c:
	  {
	    tree value = pop (type_jint);
	    insn = push (convert (type_jchar, value));
	  }
	  break;

	case op_i2s:
	  {
	    tree value = pop (type_jint);
	    insn = push (convert (type_jshort, value));
	  }
	  break;

	case op_lcmp:
	  insn = compare (type_jlong);
	  break;

	case op_fcmpl:
	  // Result is -1 for unordered.
	  insn = compare (type_jfloat, GT_EXPR);
	  break;

	case op_fcmpg:
	  // Result is 1 for unordered.
	  insn = compare (type_jfloat, UNGT_EXPR);
	  break;

	case op_dcmpl:
	  // Result is -1 for unordered.
	  insn = compare (type_jdouble, GT_EXPR);
	  break;

	case op_dcmpg:
	  // Result is 1 for unordered.
	  insn = compare (type_jdouble, UNGT_EXPR);
	  break;

	case op_ifeq:
	  insn = int_condition (EQ_EXPR, bytes, pc);
	  break;

	case op_ifne:
	  insn = int_condition (NE_EXPR, bytes, pc);
	  break;

	case op_iflt:
	  insn = int_condition (LT_EXPR, bytes, pc);
	  break;

	case op_ifge:
	  insn = int_condition (GE_EXPR, bytes, pc);
	  break;

	case op_ifgt:
	  insn = int_condition (GT_EXPR, bytes, pc);
	  break;

	case op_ifle:
	  insn = int_condition (LE_EXPR, bytes, pc);
	  break;

	case op_if_icmpeq:
	  insn = full_condition (EQ_EXPR, bytes, pc);
	  break;

	case op_if_icmpne:
	  insn = full_condition (NE_EXPR, bytes, pc);
	  break;

	case op_if_icmplt:
	  insn = full_condition (LT_EXPR, bytes, pc);
	  break;

	case op_if_icmpge:
	  insn = full_condition (GE_EXPR, bytes, pc);
	  break;

	case op_if_icmpgt:
	  insn = full_condition (GT_EXPR, bytes, pc);
	  break;

	case op_if_icmple:
	  insn = full_condition (LE_EXPR, bytes, pc);
	  break;

	case op_if_acmpeq:
	  {
	    tree value2 = pop (type_object_ptr);
	    tree value1 = pop (type_object_ptr);
	    insn = full_condition (EQ_EXPR, value1, value2, bytes, pc);
	  }
	  break;

	case op_if_acmpne:
	  {
	    tree value2 = pop (type_object_ptr);
	    tree value1 = pop (type_object_ptr);
	    insn = full_condition (NE_EXPR, value1, value2, bytes, pc);
	  }
	  break;

	case op_goto_w:
	  {
	    int here = pc - 1;
	    tree dest = find_label (here + get4 (bytes, pc));
	    insn = build1 (GOTO_EXPR, void_type_node, dest);
	  }
	  break;

	case op_goto:
	  {
	    int here = pc - 1;
	    tree dest = find_label (here + get2s (bytes, pc));
	    insn = build1 (GOTO_EXPR, void_type_node, dest);
	  }
	  break;

	case op_jsr_w:
	  {
	    int here = pc - 1;
	    tree dest = find_label (here + get4 (bytes, pc));
	    tree ret = find_label (pc);
	    tree ret_label = build_address_of (ret);
	    tsi_link_after (&statements, push (ret_label),
			    TSI_CONTINUE_LINKING);
	    insn = build1 (GOTO_EXPR, void_type_node, dest);
	  }
	  break;

	case op_jsr:
	  {
	    int here = pc - 1;
	    tree dest = find_label (here + get2s (bytes, pc));
	    tree ret = find_label (pc);
	    tree ret_label = build_address_of (ret);
	    tsi_link_after (&statements, push (ret_label),
			    TSI_CONTINUE_LINKING);
	    insn = build1 (GOTO_EXPR, void_type_node, dest);
	  }
	  break;

	case op_ret:
	  {
	    jint index = get1u (bytes, pc);
	    tree where = load (index, type_nativecode_ptr);
	    insn = build1 (GOTO_EXPR, void_type_node, where);
	  }
	  break;

	case op_tableswitch:
	  {
	    int base_pc = pc - 1;
	    tree expr = pop (type_jint);

	    while (pc % 4 != 0)
	      ++pc;

	    int def = get4 (bytes, pc);
	    int low = get4 (bytes, pc);
	    int high = get4 (bytes, pc);

	    tree body_statements = alloc_stmt_list ();
	    tree_stmt_iterator out = tsi_start (body_statements);

	    for (int i = low; i < high; ++i)
	      {
		tree label = build_label ();

		int where = base_pc + get4 (bytes, pc);
		tree case_label = build3 (CASE_LABEL_EXPR, NULL_TREE,
					  build_int (i), NULL_TREE, label);
		tsi_link_after (&out, case_label, TSI_CONTINUE_LINKING);
		tsi_link_after (&out, build1 (GOTO_EXPR, void_type_node,
					      find_label (where)),
				TSI_CONTINUE_LINKING);
	      }

	    tree label = build_label ();
	    tree def_label = build3 (CASE_LABEL_EXPR, void_type_node,
				     NULL_TREE, NULL_TREE, label);
	    tsi_link_after (&out, def_label, TSI_CONTINUE_LINKING);

	    tsi_link_after (&out, build1 (GOTO_EXPR, void_type_node,
					  find_label (base_pc + def)),
			    TSI_CONTINUE_LINKING);

	    insn = build3 (SWITCH_EXPR, type_jint, expr, body_statements,
			   NULL_TREE);
	  }
	  break;

	case op_lookupswitch:
	  {
	    int base_pc = pc - 1;
	    tree expr = pop (type_jint);

	    while (pc % 4 != 0)
	      ++pc;

	    int def = get4 (bytes, pc);
	    int npairs  = get4 (bytes, pc);

	    tree body_statements = alloc_stmt_list ();
	    tree_stmt_iterator out = tsi_start (body_statements);

	    for (int i = 0; i < npairs; ++i)
	      {
		int match = get4 (bytes, pc);
		int dest = base_pc + get4 (bytes, pc);

		tree label = build_label ();

		tree case_label = build3 (CASE_LABEL_EXPR, void_type_node,
					  build_int (match), NULL_TREE,
					  label);
		tsi_link_after (&out, case_label, TSI_CONTINUE_LINKING);
		tsi_link_after (&out, build1 (GOTO_EXPR, void_type_node,
					      find_label (dest)),
				TSI_CONTINUE_LINKING);
	      }

	    tree label = build_label ();

	    tree def_label = build3 (CASE_LABEL_EXPR, void_type_node,
				     NULL_TREE, NULL_TREE, label);
	    tsi_link_after (&out, def_label, TSI_CONTINUE_LINKING);

	    tsi_link_after (&out, build1 (GOTO_EXPR, void_type_node,
					  find_label (base_pc + def)),
			    TSI_CONTINUE_LINKING);

	    insn = build3 (SWITCH_EXPR, type_jint, expr, body_statements,
			   NULL_TREE);
	  }
	  break;

	case op_areturn:
	  {
	    // We pop an object but then cast it to the actual return
	    // type.  The verifier ensures this is always valid.
	    tree return_type
	      = gcc_builtins->map_type (method->get_return_type ());
	    insn = handle_return (type_object_ptr, return_type);
	  }
	  break;

	case op_lreturn:
	  insn = handle_return (type_jlong);
	  break;

	case op_freturn:
	  insn = handle_return (type_jfloat);
	  break;

	case op_dreturn:
	  insn = handle_return (type_jdouble);
	  break;

	case op_ireturn:
	  insn = handle_return (type_jint);
	  break;

	case op_return:
	  insn = build1 (RETURN_EXPR, void_type_node, NULL_TREE);
	  break;

	case op_getstatic:
	  {
	    jint fieldref_index = get2u (bytes, pc);
	    std::string classname, fieldname, signature;
	    cpool->get_fieldref (fieldref_index, classname, fieldname,
				 signature);
	    tree ref = gcc_builtins->map_field_ref (class_wrapper,
						    NULL_TREE,
						    classname, fieldname,
						    signature);
	    insn = push (ref);
	  }
	  break;

	case op_getfield:
	  {
	    jint fieldref_index = get2u (bytes, pc);
	    std::string classname, fieldname, signature;
	    cpool->get_fieldref (fieldref_index, classname, fieldname,
				 signature);
	    tree expr = pop (type_object_ptr);
	    tree ref = gcc_builtins->map_field_ref (class_wrapper,
						    expr,
						    classname, fieldname,
						    signature);
	    insn = push (ref);
	  }
	  break;

	case op_putstatic:
	  {
	    jint fieldref_index = get2u (bytes, pc);
	    std::string classname, fieldname, signature;
	    cpool->get_fieldref (fieldref_index, classname, fieldname,
				 signature);
	    tree ref = gcc_builtins->map_field_ref (class_wrapper,
						    NULL_TREE,
						    classname, fieldname,
						    signature);
	    tree expr = pop (TREE_TYPE (ref));
	    insn = build2 (MODIFY_EXPR, TREE_TYPE (ref), ref, expr);
	  }
	  break;

	case op_putfield:
	  {
	    jint fieldref_index = get2u (bytes, pc);
	    std::string classname, fieldname, signature;
	    cpool->get_fieldref (fieldref_index, classname, fieldname,
				 signature);
	    tree field_type = find_class (signature);
	    tree value = pop (field_type);
	    tree obj = pop (type_object_ptr);
	    tree ref = gcc_builtins->map_field_ref (class_wrapper,
						    obj,
						    classname, fieldname,
						    signature);
	    insn = build2 (MODIFY_EXPR, TREE_TYPE (ref), ref, value);
	  }
	  break;

	case op_invokeinterface:
	case op_invokespecial:
	case op_invokestatic:
	case op_invokevirtual:
	  {
	    jint meth_index = get2u (bytes, pc);
	    std::string classname, methname, descriptor;
	    cpool->get_methodref (meth_index, classname, methname, descriptor);
	    if (op == op_invokeinterface)
	      {
		// Skip dummy bytes.
		get2u (bytes, pc);
	      }

	    // FIXME: this method signature parsing code appears in at
	    // least 3 places.
	    tree arg_types = NULL_TREE;
	    for (int i = 0; i < descriptor.length (); ++i)
	      {
		int start = i;
		while (descriptor[i] == '[')
		  ++i;
		if (descriptor[i] == 'L')
		  {
		    while (descriptor[i] != ';')
		      ++i;
		  }
		// Skip the ';' or, for a primitive type, the sole
		// character.
		++i;
		std::string tname (&descriptor[start], i - start);
		// FIXME: BC?
		tree type = find_class (tname);
		arg_types = tree_cons (NULL_TREE, type, arg_types);
	      }

	    // We want to pop the arguments in reverse order, so we
	    // leave the list of types as we constructed it.  After
	    // this loop, we'll have built up the actual arguments in
	    // the correct order.
	    tree arguments = NULL_TREE;
	    for (; arg_types != NULL_TREE; arg_types = TREE_CHAIN (arg_types))
	      {
		tree one_type = TREE_VALUE (arg_types);
		tree one_arg = pop (one_type);
		arguments = tree_cons (NULL_TREE, one_arg, arguments);
	      }

	    tree obj = NULL_TREE;
	    if (op != op_invokestatic)
	      obj = pop (type_object_ptr);

	    insn = gcc_builtins->map_method_call (class_wrapper, obj,
						  arguments, classname,
						  methname, descriptor);
	  }
	  break;

	case op_new:
	  {
	    tree klass_ref
	      = build_class_ref (cpool->get_class (get2u (bytes, pc)));
	    // Note that we just use "Object *" here as the result
	    // type, even though this looks incorrect.  The verifier
	    // ensures that everything is ok type-wise; and we
	    // downcast this in any place that we need a specific
	    // type.
	    // FIXME: should be ABI call here, as we might want a
	    // finalizer-free allocator or the like.
	    tree basetype = TREE_TYPE (TREE_TYPE (builtin_Jv_AllocObject));
	    tree call = build3 (CALL_EXPR, basetype, builtin_Jv_AllocObject,
				build_tree_list (NULL_TREE, klass_ref),
				NULL_TREE);
	    TREE_SIDE_EFFECTS (call) = 1;
	    insn = push (convert (type_object_ptr, call));
	  }
	  break;

	case op_newarray:
	  {
	    model_type *elt_type = vfy_get_primitive_type (get1u (bytes, pc));
	    tree size = pop (type_jint);
	    insn = build_new_array (elt_type, size, block);
	  }
	  break;

	case op_anewarray:
	  {
	    model_type *elt_type
	      = find_model_class (cpool->get_class (get2u (bytes, pc)));
	    tree size = pop (type_jint);
	    insn = build_new_object_array (elt_type, size);
	  }
	  break;

	case op_arraylength:
	  {
	    tree value = pop (type_object_ptr);

	    model_class *objclass
	      = global->get_compiler ()->java_lang_Object ();
	    // Any array type will do.
	    model_class *objarray = objclass->array ();
	    tree field_tree
	      = gcc_builtins->find_decl (gcc_builtins->map_type (objarray),
					 "length");

	    insn
	      = push (build3 (COMPONENT_REF, TREE_TYPE (field_tree),
			      build1 (INDIRECT_REF,
				      TREE_TYPE (TREE_TYPE (value)),
				      gcc_builtins->check_reference (value)),
			      field_tree,
			      NULL_TREE));
	  }
	  break;

	case op_athrow:
	  {
	    tree value = pop (type_object_ptr);
	    insn = build3 (CALL_EXPR, void_type_node,
			   builtin_Jv_Throw,
			   build_tree_list (NULL_TREE, value),
			   NULL_TREE);
	    TREE_SIDE_EFFECTS (insn) = 1;
	  }
	  break;

	case op_checkcast:
	  {
	    tree value = pop (type_object_ptr);
	    jint index = get2u (bytes, pc);
	    tree klass = build_class_ref (cpool->get_class (index));

	    insn = build3 (CALL_EXPR, klass,
			   builtin_Jv_CheckCast,
			   tree_cons (NULL_TREE, klass,
				      build_tree_list (NULL_TREE, value)),
			   NULL_TREE);
	    TREE_SIDE_EFFECTS (insn) = 1;
	  }
	  break;

	case op_instanceof:
	  {
	    tree value = pop (type_object_ptr);
	    tree klass = find_class (cpool->get_class (get2u (bytes, pc)));
	    insn = push (convert (type_jint,
				  handle_instanceof (value, klass)));
	  }
	  break;

	case op_monitorenter:
	  {
	    tree value = pop (type_object_ptr);
	    insn = build3 (CALL_EXPR, void_type_node,
			   builtin_Jv_MonitorEnter,
			   build_tree_list (NULL_TREE, value),
			   NULL_TREE);
	    TREE_SIDE_EFFECTS (insn) = 1;
	  }
	  break;

	case op_monitorexit:
	  {
	    tree value = pop (type_object_ptr);
	    insn = build3 (CALL_EXPR, void_type_node,
			   builtin_Jv_MonitorExit,
			   build_tree_list (NULL_TREE, value),
			   NULL_TREE);
	    TREE_SIDE_EFFECTS (insn) = 1;
	  }
	  break;

	case op_ifnull:
	  {	  
	    tree value = pop (type_object_ptr);
	    insn = full_condition (EQ_EXPR, value,
				   null_pointer_node, bytes, pc);
	  }
	  break;

	case op_ifnonnull:
	  {
	    tree value = pop (type_object_ptr);
	    insn = full_condition (NE_EXPR, value,
				   null_pointer_node, bytes, pc);
	  }
	  break;

	case op_multianewarray:
	  {
	    int kind_index = get2u (bytes, pc);
	    int dim = get1u (bytes, pc);

	    // Construct the arguments.  We don't null-terminate the
	    // list, this indicates a varargs call.
	    tree args = pop (type_jint);
	    for (int i = 1; i < dim; ++i)
	      {
		tree next = pop (type_jint);
		args = tree_cons (NULL_TREE, next, args);
	      }

	    tree klassref = build_class_ref (cpool->get_class (kind_index));
	    args = tree_cons (NULL_TREE, klassref, args);

	    tree call = build3 (CALL_EXPR, type_object_ptr,
				builtin_Jv_NewMultiArray, args, NULL_TREE);
	    TREE_SIDE_EFFECTS (call) = 1;
	    insn = push (call);
	  }
	  break;

	case op_wide:
	  {
	    jint the_mod_op = get1u (bytes, pc);
	    jint wide = get2u (bytes, pc);

	    switch (the_mod_op)
	      {
	      case op_istore:
		insn = store (wide, type_jint);
		break;

	      case op_fstore:
		insn = store (wide, type_jfloat);
		break;

	      case op_astore:
		insn = store (wide, type_object_ptr);
		break;

	      case op_lload:
		insn = push (load (wide, type_jlong));
		break;

	      case op_dload:
		insn = push (load (wide, type_jdouble));
		break;

	      case op_iload:
		insn = push (load (wide, type_jint));
		break;

	      case op_fload:
		insn = push (load (wide, type_jfloat));
		break;

	      case op_aload:
		insn = push (load (wide, type_object_ptr));
		break;

	      case op_lstore:
		insn = store (wide, type_jlong);
		break;

	      case op_dstore:
		insn = store (wide, type_jdouble);
		break;

	      case op_ret:
		{
		  jint index = get2u (bytes, pc);
		  tree where = load (index, type_nativecode_ptr);
		  insn = build1 (GOTO_EXPR, void_type_node, where);
		}
		break;

	      case op_iinc:
		{
		  int amount = get2s (bytes, pc);
		  tree decl = find_generic_slot (wide, type_jint, local_slots);
		  insn = build2 (MODIFY_EXPR, type_jint, decl,
				 build2 (PLUS_EXPR,
					 type_jint,
					 decl, build_int (amount)));
		}
		break;

	      default:
		// Caught by verifier.
		abort ();
	      }
	  }

	default:
	  // Caught by verifier.
	  abort ();
	}

      assert (insn != NULL_TREE);
      tsi_link_after (&statements, insn, TSI_CONTINUE_LINKING);
    }

  // Report back to our caller.
  current = body;
}

tree
tree_generator::int_condition (tree_code op, const uint8 *bytes, int &pc)
{
  tree expr = pop (type_jint);
  return full_condition (op, expr, build_int_cst (type_jint, 0), bytes, pc);
}

tree
tree_generator::full_condition (tree_code op, const uint8 *bytes, int &pc)
{
  tree expr2 = pop (type_jint);
  tree expr1 = pop (type_jint);
  return full_condition (op, expr1, expr2, bytes, pc);
}

tree
tree_generator::full_condition (tree_code op, tree expr1, tree expr2,
				const uint8 *bytes, int &pc)
{
  int here = pc - 1;
  tree dest = find_label (here + get2s (bytes, pc));
  return build3 (COND_EXPR, void_type_node,
		 build2 (op, type_jboolean,
			 expr1, expr2),
		 build1 (GOTO_EXPR, void_type_node,
			 dest),
		 NULL_TREE);
}

tree
tree_generator::binop (tree_code op, tree type)
{
  tree expr2 = pop (type);
  tree expr1 = pop (type);
  return push (build2 (op, type, expr1, expr2));
}

tree
tree_generator::compare (tree type, tree_code gt_op)
{
  tree value2 = save_expr (pop (type));
  tree value1 = save_expr (pop (type));

  tree result = build3 (COND_EXPR, type_jint,
			build2 (gt_op,
				type_jboolean,
				value1, value2),
			build_int_cst (type_jint, 1),
			build3 (COND_EXPR, type_jint,
				build2 (EQ_EXPR,
					type_jboolean,
					value1, value2),
				build_int_cst (type_jint, 0),
				build_int_cst (type_jint, -1)));

  return push (result);
}

tree
tree_generator::handle_return (tree type, tree override)
{
  tree value = pop (type);
  if (override != NULL_TREE)
    value = convert (override, value);
  else
    override = type;

  tree assign = build2 (MODIFY_EXPR, override, DECL_RESULT (method_tree),
			value);
  TREE_SIDE_EFFECTS (assign) = 1;
  return build1 (RETURN_EXPR, void_type_node, assign);
}

tree
tree_generator::cast (tree from, tree to)
{
  tree val = pop (from);
  return push (convert (to, val));
}

tree
tree_generator::handle_convert (model_type *dest_type, tree expr)
{
  assert (dest_type == primitive_int_type || dest_type == primitive_long_type);

  tree min_tree, max_tree;
  tree dest_tree = gcc_builtins->map_type (dest_type);

  if (dest_type == primitive_int_type)
    {
      min_tree = build_int (MIN_INT);
      max_tree = build_int (MAX_INT);
    }
  else
    {
      min_tree = build_long (MIN_LONG);
      max_tree = build_long (MAX_LONG);
    }
  assert (TREE_TYPE (min_tree) == dest_tree);

  tree flt_min_tree = convert (TREE_TYPE (expr), min_tree);
  tree flt_max_tree = convert (TREE_TYPE (expr), max_tree);

  expr = save_expr (expr);

  // We start at the end of the expression and work toward the
  // front.  At the end we see:
  //   if (val != val)
  //     return 0;
  //   else
  //     return (TO) val;
  tree ne = build2 (NE_EXPR, type_jboolean, expr, expr);
  tree cond = build3 (COND_EXPR, dest_tree, ne,
		      build_int_cst (dest_tree, 0),
		      convert_to_integer (dest_tree, expr));

  // Now build:
  //   if (val <= (FROM) min) return min; else [ the above ]
  cond = build3 (COND_EXPR, dest_tree,
		 build2 (LE_EXPR, type_jboolean, expr, flt_min_tree),
		 min_tree, cond);

  // Finally:
  //   if (val >= (FROM) max) return max; else [ the above ]
  return build3 (COND_EXPR, dest_tree,
		 build2 (GE_EXPR, type_jboolean, expr, flt_max_tree),
		 max_tree, cond);
}

tree
tree_generator::find_label (int pc)
{
  if (labels[pc] == NULL_TREE)
    {
      char buf[32];
      sprintf (buf, "LJpc=%d", pc); // FIXME internal label?
      labels[pc] = build_decl (LABEL_DECL, get_identifier (buf), NULL_TREE);
      DECL_CONTEXT (labels[pc]) = method_tree;
      DECL_IGNORED_P (labels[pc]) = 1;
      DECL_ARTIFICIAL (labels[pc]) = 1;
    }
  return labels[pc];
}

tree
tree_generator::push (tree value)
{
  tree type = TREE_TYPE (value);
  if (value == null_pointer_node)
    type = type_object_ptr;

  tree decl = find_slot (stack_top, type);
  stack_types[stack_top] = type;
  ++stack_top;
  tree result = build2 (MODIFY_EXPR, type, decl, value);
  TREE_SIDE_EFFECTS (result) = 1;
  return result;
}

tree
tree_generator::pop (tree type)
{
  tree slot = find_slot (stack_top - 1, type);
  --stack_top;
  return slot;
}

tree
tree_generator::find_slot (int slot_num, tree type)
{
  // This finds the VAR_DECL corresponding to a given stack slot and
  // type.  For each stack slot, we have one VAR_DECL per "basic
  // type", i.e. those corresponding to the VMs various "store"
  // instructions.  At any given moment, only one such decl per slot
  // is live.  Note that this implementation is much less
  // sophisticated than the old gcj implementation was.  With the
  // advent of tree-ssa, we don't need to worry as much about
  // generating dead stores to local variables.  These are efficiently
  // deleted by the various optimization passes.
  return find_generic_slot (slot_num, type, stack_slots);
}

// Load a value from a local variable slot.
tree
tree_generator::load (int slot, tree type)
{
  return find_generic_slot (slot, type, local_slots);
}

// Store a value into a local variable slot.
tree
tree_generator::store (int slot, tree type)
{
  tree value = pop (type);
  tree decl = find_generic_slot (slot, type, local_slots);
  tree result = build2 (MODIFY_EXPR, type, decl, value);
  TREE_SIDE_EFFECTS (result) = 1;
  return result;
}

tree
tree_generator::find_generic_slot (int slot, tree type,
				   tree *slots)
{
  tree base_decl = slots[slot];

  tree field;
  if (TREE_CODE (type) == POINTER_TYPE)
    {
      field = field_slot_o;
      type = type_object_ptr;
    }
  else if (type == type_jdouble)
    field = field_slot_d;
  else if (type == type_jfloat)
    field = field_slot_f;
  else if (type == type_jlong)
    field = field_slot_j;
  else
    {
      assert (type == type_jint || type == type_jbyte
	      || type == type_jboolean || type == type_jchar
	      || type == type_jshort);
      field = field_slot_i;
      type = type_jint;
    }

  return build3 (COMPONENT_REF, type, base_decl, field, NULL_TREE);
}

// An array store operation.
tree
tree_generator::array_store (model_type *mtype, tree type)
{
  tree val = pop (type);
  tree ary = array_access (mtype, type);
  tree result = build2 (MODIFY_EXPR, type, ary, val);
  TREE_SIDE_EFFECTS (result) = 1;
  return result;
}

// An array fetch operation.
tree
tree_generator::array_access (model_type *mtype, tree type)
{
  tree array_type = gcc_builtins->map_type (mtype->array ());

  tree index = pop (type_jint);
  tree array = pop (array_type);
  return build_array_reference (array, index, array_type);
}

tree
tree_generator::find_class (const std::string &name)
{
  return gcc_builtins->map_type (find_model_class (name));
}

model_type *
tree_generator::find_model_class (const std::string &name)
{
  // FIXME: the places we use this don't work with the new ABI.
  model_class *klass = method->get_declaring_class ();
  model_unit_class *unit
    = assert_cast<model_unit_class *> (klass->get_compilation_unit ());
  return unit->find_class_from_descriptor (NULL, method, name);
}

tree
tree_generator::handle_ldc (constant_pool *cpool, uint16 index)
{
  assert (cpool->valid_p (index));
  tree result = NULL_TREE;
  switch (cpool->tag (index))
    {
    case CONSTANT_String:
      result = handle_string_literal (cpool->get_string (index));
      break;

    case CONSTANT_Integer:
      result = build_int (jint (cpool->value (index)));
      break;

    case CONSTANT_Float:
      {
	long w = cpool->value (index);
	REAL_VALUE_TYPE f;
	real_from_target_fmt (&f, &w, &ieee_single_format);
	result = build_real (type_jfloat, f);
      }
      break;

    case CONSTANT_Long:
      {
	jlong l = words_to_long (cpool->value (index),
				 cpool->value (index + 1));
	result = build_long (l);
      }
      break;

    case CONSTANT_Double:
      {
	long wl[2];
	wl[0] = cpool->value (index);
	wl[1] = cpool->value (index + 1);
	REAL_VALUE_TYPE d;
	real_from_target_fmt (&d, wl, &ieee_double_format);
	current = build_real (type_jdouble, d);
      }

    case CONSTANT_Class:
      // 1.5 only.  The verifier will reject this for pre-1.5 class
      // 1.files.
      result = build_class_ref (cpool->get_class (index));
      break;

    default:
      // Caught by verifier.
      abort ();
    }
  return push (result);
}

bool
tree_generator::type_wide_p (tree type) const
{
  return type == type_jdouble || type == type_jlong;
}
