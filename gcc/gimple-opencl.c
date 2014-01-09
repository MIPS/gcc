/*
   Generation of OpenCL kernels from GIMPLE.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "gimple-pretty-print.h"
#include "dumpfile.h"
#include "tree-cfg.h"
#include "tree-ssa.h"
#include "plugin.h"
#include "function.h"
#include "stringpool.h"
#include "tree-ssa-alias.h"   
#include "tree-ssanames.h"
#include "gimple.h"
#include "gimple-ssa.h"
#include "bitmap.h"
#include "tree-ssa-live.h"
#include "tree-outof-ssa.h"
#include "gimple-opencl.h"
#include "gimple-iterator.h"

struct opencl_attribute_spec
{
  struct attribute_spec spec;
  tree attr_node;
};

struct opencl_attribute_spec opencl_attributes[] =
{
    {{ "*kernel", 0, 0, true, false, false, NULL, false}, NULL_TREE},
    {{ "*global", 0, 0, true, false, false, NULL, false}, NULL_TREE},
    {{ "*local", 0, 0, true, false, false, NULL, false}, NULL_TREE},
    {{ "*constant", 0, 0, true, false, false, NULL, false}, NULL_TREE},
    {{ "*private", 0, 0, true, false, false, NULL, false }, NULL_TREE}
};

static struct opencl_attribute_spec*
find_attribute(const char* name)
{
  size_t i;
  struct opencl_attribute_spec* spec = NULL;
  
  for(i = 0; i < sizeof(opencl_attributes) / sizeof(opencl_attributes[0]); ++i)
    {
      if(strcmp(name, opencl_attributes[i].spec.name) == 0)
        {
          spec = &opencl_attributes[i];
          break;
        }
    }
  return spec;
}

tree
get_opencl_attribute(const char* name)
{
  struct opencl_attribute_spec* spec = NULL; 
  
  spec =  find_attribute(name);
  if(spec != NULL)
    {
      if(spec->attr_node == NULL_TREE)
        {
            register_attribute(&spec->spec);
            spec->attr_node =
                    tree_cons(get_identifier(spec->spec.name), NULL, NULL);
        }
      return spec->attr_node;
    }
  
  return NULL_TREE;
}

static void
escape_name(FILE* fp, const char* name)
{
  /* may be convert to locale?
     name = identifier_to_locale(name);
   */
  for(; *name; ++name)
    {
      if(*name == '.' || *name == '$')
        {
          fputc('_', fp);
        }
      else
        {
          fputc(*name, fp);
        }
    }
}

static void
generate_name(FILE *fp, tree decl, bool get_asm = false)
{
  const char* id;
  tree name = NULL_TREE;
  bool is_ssa_name = TREE_CODE(decl) == SSA_NAME;
  int num = 0;

  if(is_ssa_name)
    {
      num = SSA_NAME_VERSION(decl);
      decl = SSA_NAME_VAR(decl);
    }

  if(decl)
    {
      if(get_asm)
        {
          name = DECL_ASSEMBLER_NAME(decl);
        }
      if(!name)
        {
          name = DECL_NAME(decl);
        }
    }

  if(name)
    {
      id = IDENTIFIER_POINTER(name);
    }
  else
    {
      int uid = 0;
      if(decl)
        uid = DECL_UID(decl);
      id = (const char*)XALLOCAVEC(char, 64);
      sprintf((char*)id, "_openacc_tmp_%d_%d", uid, num);
    }

  escape_name(fp, id);
}

static void
generate_opencl_attributes(FILE *fp, tree decl)
{
  tree attrs = DECL_ATTRIBUTES(decl);
  
  if(TREE_CODE(decl) == FUNCTION_DECL)
    {
    if(lookup_attribute("*kernel", attrs) != NULL_TREE)
        {
          fprintf(fp, "__kernel ");
        }
    }
  else
    {
    if(lookup_attribute("*global", attrs) != NULL_TREE)
      {
        fprintf(fp, "__global ");
      }
    if(lookup_attribute("*local", attrs) != NULL_TREE)
      {
        fprintf(fp, "__local ");
      }
    if(lookup_attribute("*private", attrs) != NULL_TREE)
      {
        fprintf(fp, "__private ");
      }
  }
}

static void
generate_integer_type(FILE *fp, tree type)
{
  unsigned prec = TYPE_PRECISION(type);

  if(prec <= 32)
    {
      fprintf(fp, "int ");
    }
  else
    {
      fprintf(fp, "long int ");
    }
}

static void
generate_real_type(FILE *fp, tree type)
{
  unsigned prec = TYPE_PRECISION(type);

  if(prec <= 32)
    {
      fprintf(fp, "float ");
    }
  else
    {
      fprintf(fp, "double ");
    }
}

static bool
generate_type_1(FILE* fp, tree type, tree decl)
{
  bool retval = false;

  switch(TREE_CODE(type))
    {
    case VOID_TYPE:
      fprintf(fp, "void ");
      break;
    case INTEGER_TYPE:
      generate_integer_type(fp, type);
      break;
    case REAL_TYPE:
      generate_real_type(fp, type);
      break;
    case BOOLEAN_TYPE:
      fprintf(fp, "int ");
      break;
    case ARRAY_TYPE:
      generate_type_1(fp, TREE_TYPE(type), decl);
      fprintf(fp, "* ");
      break;
    case POINTER_TYPE:
      generate_type_1(fp, TREE_TYPE(type), decl);
      fprintf(fp, "* ");
      break;
    default:
      gcc_unreachable();
    }
  return retval;
}

static bool
generate_type(FILE* fp, tree type, tree decl)
{
  bool retval = false;

  if(decl != NULL_TREE && TREE_CODE(decl) != SSA_NAME)
    {
        generate_opencl_attributes(fp, decl);
    }
  generate_type_1(fp, type, decl);
  return retval;
}

static void
generate_var_decl(FILE* fp, tree decl)
{
  bool all_done = generate_type(fp, TREE_TYPE(decl), decl);
  if(!all_done)
    {
      fputc(' ', fp);
      generate_name(fp, decl);
    }
}

static void
generate_kernel_header(FILE* fp, tree kernel_fn)
{
  tree param;

  generate_type(fp, TREE_TYPE(DECL_RESULT(kernel_fn)), kernel_fn);
  fprintf(fp, IDENTIFIER_POINTER(DECL_NAME(kernel_fn)));
  fputc('(', fp);
  for(param = DECL_ARGUMENTS(kernel_fn); param; param = DECL_CHAIN(param))
    {
      generate_var_decl(fp, param);
      if(DECL_CHAIN(param) != NULL_TREE)
        fputc(',', fp);
    }
  fprintf(fp, ")\n");
}

static void
generate_locals(FILE* fp, tree kernel_fn)
{
  size_t i;
  tree var;

  FOR_EACH_LOCAL_DECL(DECL_STRUCT_FUNCTION(kernel_fn), i, var)
  {
    generate_var_decl(fp, var);
    fprintf(fp, ";\n");
  }

  if(gimple_in_ssa_p(cfun))
    {
      for(i = 1; i < num_ssa_names; ++i)
        {
          var = ssa_name(i);
          if(var && !SSA_NAME_VAR(var))
            {
              generate_var_decl(fp, var);
              fprintf(fp, ";\n");
            }
        }
    }

}

static void
generate_constant(FILE* fp, tree cnst)
{
  switch(TREE_CODE(cnst))
    {
    case INTEGER_CST:
      if(tree_fits_shwi_p(cnst))
        {
          fprintf(fp, HOST_WIDE_INT_PRINT_DEC,
            (HOST_WIDE_INT)TREE_INT_CST_LOW(cnst));
        }
      else if(tree_fits_uhwi_p(cnst))
        {
          fprintf(fp, HOST_WIDE_INT_PRINT_UNSIGNED,
            (unsigned HOST_WIDE_INT)TREE_INT_CST_LOW(cnst));
        }
      break;
    case REAL_CST:
    {
      REAL_VALUE_TYPE x;
      char buf[128];

      x = TREE_REAL_CST(cnst);
      real_to_decimal(buf, &x, sizeof(buf), 0, 1);
      fprintf(fp, buf);
    }
    break;
    case STRING_CST:
      fprintf(fp, "\"%s\"", TREE_STRING_POINTER(cnst));
      break;
    default:
      gcc_unreachable();
    }
}

static void
generate_expr(FILE* fp, tree expr)
{
  if(is_gimple_variable(expr) || DECL_P(expr))
    {
      generate_name(fp, expr);
    }
  else if(CONSTANT_CLASS_P(expr))
    {
      generate_constant(fp, expr);
    }
  else
    {
      switch(TREE_CODE(expr))
        {
        case ARRAY_REF:
        case MEM_REF:
          generate_expr(fp, TREE_OPERAND(expr, 0));
          fprintf(fp, "[");
          generate_expr(fp, TREE_OPERAND(expr, 1));
          fprintf(fp, "]");
          break;
        case ADDR_EXPR:
          fprintf(fp, "&");
          generate_expr(fp, TREE_OPERAND(expr, 0));
          break;
        case INDIRECT_REF:
          fprintf(fp, "*");
          generate_expr(fp, TREE_OPERAND(expr, 0));
          break;
        case COMPONENT_REF:
          generate_expr(fp, TREE_OPERAND(expr, 0));
          fprintf(fp, ".");
          generate_expr(fp, TREE_OPERAND(expr, 1));
          break;
        default:
          gcc_unreachable();
        }
    }
}

static void
generate_abs_expr(FILE *fp, tree expr)
{
  tree type = TREE_TYPE(expr);

  switch(TREE_CODE(type))
    {
    case INTEGER_TYPE:
      fprintf(fp, "abs(");
      break;
    case REAL_TYPE:
      fprintf(fp, "fabs(");
      break;
    default:
      gcc_unreachable();
    }
  generate_expr(fp, expr);
  fprintf(fp, ")");
}

static void
generate_truncate_expr(FILE *fp, tree expr)
{
  fprintf(fp, "trunc(");
  generate_expr(fp, expr);
  fprintf(fp, ")");
}

static void
generate_convert_expr(FILE *fp, tree to_type, tree expr)
{
  fprintf(fp, "(");
  generate_type(fp, to_type, NULL_TREE);
  fprintf(fp, ")(");
  generate_expr(fp, expr);
  fprintf(fp, ")");
}

static void
generate_unary_rhs(FILE* fp, enum tree_code code, gimple stmt)
{
  tree rhs = gimple_assign_rhs1(stmt);

  switch(code)
    {
    case NEGATE_EXPR:
      fprintf(fp, "-");
      break;
    case ABS_EXPR:
      generate_abs_expr(fp, rhs);
      return;
    case BIT_NOT_EXPR:
      fprintf(fp, "~");
      break;
    case TRUTH_NOT_EXPR:
      fprintf(fp, "!");
      break;
    case PREDECREMENT_EXPR:
      fprintf(fp, "--");
      break;
    case PREINCREMENT_EXPR:
      fprintf(fp, "++");
      break;
    case POSTDECREMENT_EXPR:
      generate_expr(fp, rhs);
      fprintf(fp, "--");
      return;
    case POSTINCREMENT_EXPR:
      generate_expr(fp, rhs);
      fprintf(fp, "++");
      return;
    case FIX_TRUNC_EXPR:
      generate_truncate_expr(fp, rhs);
      return;
    case NON_LVALUE_EXPR:
    case NOP_EXPR:
      break;
    case FLOAT_EXPR:
    case CONVERT_EXPR:
    case FIXED_CONVERT_EXPR:
      generate_convert_expr(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs);
      return;
    default:
      gcc_unreachable();
    }

  generate_expr(fp, rhs);
}

static void
generate_truth_xor(FILE *fp, tree expr1, tree expr2)
{
  fprintf(fp, "(((");
  generate_expr(fp, expr1);
  fprintf(fp, ") && !(");
  generate_expr(fp, expr2);
  fprintf(fp, ")) || (!(");
  generate_expr(fp, expr1);
  fprintf(fp, ") && (");
  generate_expr(fp, expr2);
  fprintf(fp, ")))");
}

static void
generate_floor_div(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") floor((");
  generate_expr(fp, expr1);
  fprintf(fp, ")/(");
  generate_expr(fp, expr2);
  fprintf(fp, "))");

}

static void
generate_ceil_div(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") ceil((");
  generate_expr(fp, expr1);
  fprintf(fp, ")/(");
  generate_expr(fp, expr2);
  fprintf(fp, "))");
}

static void
generate_round_div(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") round((");
  generate_expr(fp, expr1);
  fprintf(fp, ")/(");
  generate_expr(fp, expr2);
  fprintf(fp, "))");
}

static void
generate_floor_mod(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") floor( remainder(");
  generate_expr(fp, expr1);
  fprintf(fp, ", ");
  generate_expr(fp, expr2);
  fprintf(fp, "))");
}

static void
generate_ceil_mod(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") ceil( remainder(");
  generate_expr(fp, expr1);
  fprintf(fp, ", ");
  generate_expr(fp, expr2);
  fprintf(fp, "))");
}

static void
generate_round_mod(FILE *fp, tree type, tree expr1, tree expr2)
{
  fprintf(fp, "(");
  generate_type(fp, type, NULL_TREE);
  fprintf(fp, ") round( remainder(");
  generate_expr(fp, expr1);
  fprintf(fp, ", ");
  generate_expr(fp, expr2);
  fprintf(fp, "))");
}

static void
generate_binary_rhs(FILE* fp, enum tree_code code, gimple stmt)
{
  tree rhs1 = gimple_assign_rhs1(stmt);
  tree rhs2 = gimple_assign_rhs2(stmt);

  switch(code)
    {
    case TRUTH_XOR_EXPR:
      generate_truth_xor(fp, rhs1, rhs2);
      return;
    case FLOOR_DIV_EXPR:
      generate_floor_div(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    case CEIL_DIV_EXPR:
      generate_ceil_div(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    case ROUND_DIV_EXPR:
      generate_round_div(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    case FLOOR_MOD_EXPR:
      generate_floor_mod(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    case CEIL_MOD_EXPR:
      generate_ceil_mod(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    case ROUND_MOD_EXPR:
      generate_round_mod(fp, TREE_TYPE(gimple_assign_lhs(stmt)), rhs1, rhs2);
      return;
    default:
      break;
    }

  generate_expr(fp, rhs1);
  switch(code)
    {
    case LSHIFT_EXPR:
      fprintf(fp, " << ");
      break;
    case RSHIFT_EXPR:
      fprintf(fp, " >> ");
      break;
    case BIT_IOR_EXPR:
      fprintf(fp, " | ");
      break;
    case BIT_XOR_EXPR:
      fprintf(fp, " ^ ");
      break;
    case BIT_AND_EXPR:
      fprintf(fp, " & ");
      break;
    case TRUTH_ANDIF_EXPR:
      fprintf(fp, " && ");
      break;
    case TRUTH_ORIF_EXPR:
      fprintf(fp, " || ");
      break;
    case TRUTH_AND_EXPR:
      fprintf(fp, " && ");
      break;
    case TRUTH_OR_EXPR:
      fprintf(fp, " || ");
      break;
    case POINTER_PLUS_EXPR:
      fprintf(fp, " + ");
      break;
    case PLUS_EXPR:
      fprintf(fp, " + ");
      break;
    case MINUS_EXPR:
      fprintf(fp, " - ");
      break;
    case MULT_EXPR:
      fprintf(fp, " * ");
      break;
    case TRUNC_DIV_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
      fprintf(fp, " / ");
      break;
    case TRUNC_MOD_EXPR:
      fprintf(fp, " %% ");
      break;

    case LT_EXPR:
      fprintf(fp, " < ");
      break;
    case LE_EXPR:
      fprintf(fp, " <= ");
      break;
    case GT_EXPR:
      fprintf(fp, " > ");
      break;
    case GE_EXPR:
      fprintf(fp, " >= ");
      break;
    case EQ_EXPR:
      fprintf(fp, " == ");
      break;
    case NE_EXPR:
      fprintf(fp, " != ");
      break;

    case UNLT_EXPR:
      fprintf(fp, " < ");
      break;
    case UNLE_EXPR:
      fprintf(fp, " <= ");
      break;
    case UNGT_EXPR:
      fprintf(fp, " > ");
      break;
    case UNGE_EXPR:
      fprintf(fp, " >= ");
      break;
    case UNEQ_EXPR:
      fprintf(fp, " == ");
      break;
    case LTGT_EXPR:
      fprintf(fp, " != ");
      break;

    default:
      gcc_unreachable();
    }
  generate_expr(fp, rhs2);
}

static void
generate_ternary_rhs(FILE* fp, enum tree_code code, gimple stmt)
{
  tree rhs1 = gimple_assign_rhs1(stmt);
  tree rhs2 = gimple_assign_rhs2(stmt);
  tree rhs3 = gimple_assign_rhs3(stmt);

  switch(code)
    {
    case COND_EXPR:
      fprintf(fp, "(");
      generate_expr(fp, rhs1);
      fprintf(fp, ") ? (");
      generate_expr(fp, rhs2);
      fprintf(fp, ") : (");
      generate_expr(fp, rhs3);
      fprintf(fp, ")");
      break;
    default:
      gcc_unreachable();
    }

}

static void
generate_gimple_assign(FILE* fp, gimple stmt)
{
  enum gimple_rhs_class rhs_class = gimple_assign_rhs_class(stmt);
  enum tree_code code = gimple_assign_rhs_code(stmt);

  generate_expr(fp, gimple_assign_lhs(stmt));
  fprintf(fp, " = ");

  switch(rhs_class)
    {
    case GIMPLE_SINGLE_RHS:
      generate_expr(fp, gimple_assign_rhs1(stmt));
      break;
    case GIMPLE_UNARY_RHS:
      generate_unary_rhs(fp, code, stmt);
      break;
    case GIMPLE_BINARY_RHS:
      generate_binary_rhs(fp, code, stmt);
      break;
    case GIMPLE_TERNARY_RHS:
      generate_ternary_rhs(fp, code, stmt);
      break;
    }
  fprintf(fp, ";\n");
}

static void
generate_gimple_cond(FILE* fp, gimple stmt)
{
  enum tree_code code = gimple_cond_code(stmt);
  tree lhs = gimple_cond_lhs(stmt);
  tree rhs = gimple_cond_rhs(stmt);

  generate_expr(fp, lhs);
  switch(code)
    {
    case LT_EXPR:
      fprintf(fp, " < ");
      break;
    case LE_EXPR:
      fprintf(fp, " <= ");
      break;
    case GT_EXPR:
      fprintf(fp, " > ");
      break;
    case GE_EXPR:
      fprintf(fp, " >= ");
      break;
    case EQ_EXPR:
      fprintf(fp, " == ");
      break;
    case NE_EXPR:
      fprintf(fp, " != ");
      break;

    case UNLT_EXPR:
      fprintf(fp, " < ");
      break;
    case UNLE_EXPR:
      fprintf(fp, " <= ");
      break;
    case UNGT_EXPR:
      fprintf(fp, " > ");
      break;
    case UNGE_EXPR:
      fprintf(fp, " >= ");
      break;
    case UNEQ_EXPR:
      fprintf(fp, " == ");
      break;
    case LTGT_EXPR:
      fprintf(fp, " != ");
      break;

    default:
      gcc_unreachable();
    }
  generate_expr(fp, rhs);
}

static void
generate_gimple_call(FILE* fp, gimple stmt)
{
  tree lhs = gimple_call_lhs(stmt);
  unsigned i;

  if(TREE_CODE(gimple_call_return_type(stmt)) != VOID_TYPE)
    {
      generate_expr(fp, lhs);
      fprintf(fp, " = ");
    }
  generate_name(fp, gimple_call_fndecl(stmt), true);
  fprintf(fp, "(");
  for(i = 0; i < gimple_call_num_args(stmt); ++i)
    {
      generate_expr(fp, gimple_call_arg(stmt, i));
      if(i < gimple_call_num_args(stmt) - 1)
        {
          fprintf(fp, ", ");
        }
    }
  fprintf(fp, ");\n");
}

static void
generate_gimple_return(FILE *fp, gimple stmt)
{
  tree ret_val = gimple_return_retval(stmt);

  fprintf(fp, "return ");
  if(ret_val != NULL_TREE)
    {
      generate_expr(fp, ret_val);
    }
  fprintf(fp, ";\n");
}

static void
generate_gimple_goto(FILE *fp, gimple stmt)
{
  tree label = gimple_goto_dest(stmt);
  if(label != NULL_TREE)
    {
      fprintf(fp, "goto ");
      generate_expr(fp, label);
      fprintf(fp, ";\n");
    }
}

static void
generate_gimple_label(FILE *fp, gimple stmt)
{
  tree label = gimple_label_label(stmt);

  generate_expr(fp, label);
  fprintf(fp, ":\n");
}

static void
generate_gimple_switch(FILE *fp, gimple stmt)
{
  // TODO
  gcc_unreachable();
}

static void
generate_stmt(FILE* fp, gimple stmt)
{
  fprintf(fp, "// ");
  print_gimple_stmt(fp, stmt, 0, TDF_RAW);

  switch(gimple_code(stmt))
    {
    case GIMPLE_ASSIGN:
      generate_gimple_assign(fp, stmt);
      break;
    case GIMPLE_COND:
      fprintf(fp, "if(");
      generate_gimple_cond(fp, stmt);
      fprintf(fp, ")\n");
      break;
    case GIMPLE_RETURN:
      generate_gimple_return(fp, stmt);
      break;
    case GIMPLE_CALL:
      generate_gimple_call(fp, stmt);
      break;
    case GIMPLE_NOP:
      fprintf(fp, ";\n");
      break;
    case GIMPLE_GOTO:
      generate_gimple_goto(fp, stmt);
      break;
    case GIMPLE_LABEL:
      generate_gimple_label(fp, stmt);
      break;
    case GIMPLE_SWITCH:
      generate_gimple_switch(fp, stmt);
      break;
    case GIMPLE_DEBUG:
      /* silently ignore */
      break;
    default:
      gcc_unreachable();
      break;
    }
}

static void
generate_jump(FILE* fp, basic_block bb)
{
  fprintf(fp, "goto L%d;", bb->index);
}

static void
generate_implicit_gotos(FILE* fp, basic_block bb)
{
  edge e;
  gimple stmt;

  stmt = last_stmt (bb);

  if (stmt && gimple_code (stmt) == GIMPLE_COND)
    {
      edge true_edge, false_edge;
      extract_true_false_edges_from_block (bb, &true_edge, &false_edge);

      generate_jump (fp, true_edge->dest);
      fprintf(fp, "\n");
      fprintf(fp, "else\n");
      generate_jump (fp, false_edge->dest);
      fprintf(fp, "\n");
      return;
    }
  e = find_fallthru_edge (bb->succs);
  if (e && e->dest != bb->next_bb)
    {
      generate_jump (fp, e->dest);
      fprintf(fp, "\n");
    }
}

static void
generate_stmts(FILE* fp, tree kernel_fn)
{
  basic_block bb;

  FOR_EACH_BB(bb)
  {
    fprintf(fp, "L%d:\n", bb->index);
    gimple_stmt_iterator gsi;
    gsi = gsi_start_bb(bb);
    if(gsi_end_p(gsi))
      {
        fprintf(fp, "\t;\n");
      }
    else
      {
        for(; !gsi_end_p(gsi); gsi_next(&gsi))
          {
            gimple stmt = gsi_stmt(gsi);
            generate_stmt(fp, stmt);
          }
      }
    generate_implicit_gotos(fp, bb);
  }
}

static void
generate_kernel_body(FILE* fp, tree kernel_fn)
{
  fprintf(fp, "{\n");
  generate_locals(fp, kernel_fn);
  generate_stmts(fp, kernel_fn);
  fprintf(fp, "}\n");
}


static void
generate_kernel(FILE* fp, tree kernel_fn)
{
  generate_kernel_header(fp, kernel_fn);
  generate_kernel_body(fp, kernel_fn);
}

void
generate_opencl_kernel(char* cl_file, tree kernel_fn, struct ssaexpand* ssa)
{
  FILE* fp;

  fp = fopen(cl_file, "a+t");
  generate_kernel(fp, kernel_fn);
  fclose(fp);
}
