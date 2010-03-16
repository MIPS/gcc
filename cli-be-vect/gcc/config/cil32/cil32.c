/* Definitions for GCC.  Part of the machine description for cil32.

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
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-attr.h"
#include "flags.h"
#include "tree.h"
#include "expr.h"
#include "except.h"
#include "function.h"
#include "toplev.h"
#include "recog.h"
#include "reload.h"
#include "tm_p.h"
#include "debug.h"
#include "output.h"
#include "target.h"
#include "target-def.h"
#include "ggc.h"
#include "optabs.h"
#include "langhooks.h"

#include "cil32-protos.h"
#include "cil-builtins.h"
#include "cil-types.h"
#include "cil-refs.h"
#include "emit-cil.h"

static struct machine_function *cil_init_machine_status (void);
static hashval_t cil_basic_block_hash (const void *);
static int cil_basic_block_eq (const void *, const void *);

static tree cil32_handle_function_attribute (tree *, tree, tree, int, bool *);
static void cil32_file_start (void);
static void cil32_file_end (void);
static void cil32_named_section (const char *, unsigned int, tree);
static void cil32_globalize_label (FILE *, const char *);
static bool cil32_assemble_integer (rtx, unsigned int, int);

static tree cil32_gimplify_va_arg (tree, tree, tree*, tree*);

static bool cil32_vector_mode_supported_p (enum machine_mode);

const struct attribute_spec cil32_attribute_table[];

static tree cil32_builtin_get_vec_stride (tree);
static tree cil32_builtin_build_uniform_vec (tree, tree);
static tree cil32_builtin_build_affine_vec (tree, tree, tree);
static tree cil32_builtin_get_vec_size (tree);
static tree cil32_builtin_get_vec_align (tree);
static tree cil32_builtin_build_reduc_epilogue (enum tree_code, tree);
static bool cil32_builtin_vectorize_independent_drs_only (void);
static bool cil32_builtin_always_realign (void);
static tree cil32_builtin_mask_for_load (void);
static tree cil32_builtin_realign_load (tree);
static bool cil32_builtin_can_force_alignment (void);
static tree cil32_builtin_get_loop_niters (void);
static tree cil32_builtin_pattern (enum tree_code code, tree);
static tree cil32_builtin_realign_offset (tree);

/* Initialize the GCC target structure.  */
#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE cil32_attribute_table

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS cil_init_builtins


#undef TARGET_ASM_FILE_START
#define TARGET_ASM_FILE_START cil32_file_start
#undef TARGET_ASM_FILE_END
#define TARGET_ASM_FILE_END cil32_file_end
#undef	TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION cil32_named_section
#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL cil32_globalize_label
#undef  TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER cil32_assemble_integer

#undef TARGET_BUILD_BUILTIN_VA_LIST
#define TARGET_BUILD_BUILTIN_VA_LIST cil_build_builtin_va_list
#undef TARGET_GIMPLIFY_VA_ARG_EXPR
#define TARGET_GIMPLIFY_VA_ARG_EXPR cil32_gimplify_va_arg

#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P cil32_vector_mode_supported_p

#undef TARGET_VECTORIZE_BUILTIN_GET_VEC_STRIDE
#define TARGET_VECTORIZE_BUILTIN_GET_VEC_STRIDE cil32_builtin_get_vec_stride

#undef TARGET_VECTORIZE_BUILTIN_GET_VEC_SIZE
#define TARGET_VECTORIZE_BUILTIN_GET_VEC_SIZE cil32_builtin_get_vec_size

#undef TARGET_VECTORIZE_BUILTIN_BUILD_UNIFORM_VEC
#define TARGET_VECTORIZE_BUILTIN_BUILD_UNIFORM_VEC \
  cil32_builtin_build_uniform_vec

#undef TARGET_VECTORIZE_BUILTIN_BUILD_AFFINE_VEC
#define TARGET_VECTORIZE_BUILTIN_BUILD_AFFINE_VEC \
  cil32_builtin_build_affine_vec

#undef TARGET_VECTORIZE_BUILTIN_GET_VEC_ALIGN
#define TARGET_VECTORIZE_BUILTIN_GET_VEC_ALIGN cil32_builtin_get_vec_align

#undef TARGET_VECTORIZE_BUILTIN_BUILD_REDUC_EPILOGUE
#define TARGET_VECTORIZE_BUILTIN_BUILD_REDUC_EPILOGUE \
  cil32_builtin_build_reduc_epilogue

#undef TARGET_VECTORIZE_BUILTIN_VECT_INDEPENDENT_DRS
#define TARGET_VECTORIZE_BUILTIN_VECT_INDEPENDENT_DRS \
  cil32_builtin_vectorize_independent_drs_only

#undef TARGET_VECTORIZE_BUILTIN_ALWAYS_REALIGN
#define TARGET_VECTORIZE_BUILTIN_ALWAYS_REALIGN \
  cil32_builtin_always_realign

#undef TARGET_VECTORIZE_BUILTIN_MASK_FOR_LOAD
#define TARGET_VECTORIZE_BUILTIN_MASK_FOR_LOAD \
  cil32_builtin_mask_for_load

#undef TARGET_VECTORIZE_BUILTIN_REALIGN_LOAD
#define TARGET_VECTORIZE_BUILTIN_REALIGN_LOAD \
  cil32_builtin_realign_load

#undef TARGET_VECTORIZE_BUILTIN_CAN_FORCE_ALIGNMENT
#define TARGET_VECTORIZE_BUILTIN_CAN_FORCE_ALIGNMENT \
  cil32_builtin_can_force_alignment

#undef TARGET_VECTORIZE_BUILTIN_GET_LOOP_NITERS
#define TARGET_VECTORIZE_BUILTIN_GET_LOOP_NITERS \
  cil32_builtin_get_loop_niters

#undef TARGET_VECTORIZE_BUILTIN_PATTERN
#define TARGET_VECTORIZE_BUILTIN_PATTERN \
  cil32_builtin_pattern 

#undef TARGET_VECTORIZE_BUILTIN_REALIGN_OFFSET
#define TARGET_VECTORIZE_BUILTIN_REALIGN_OFFSET \
  cil32_builtin_realign_offset

struct gcc_target targetm = TARGET_INITIALIZER;

/* CIL32 Code */

static struct machine_function *
cil_init_machine_status (void)
{
  struct machine_function *machine = GGC_NEW (struct machine_function);

  machine->label_id = 0;
  machine->label_addrs = NULL_TREE;
  machine->locals_init = false;
  machine->has_vec = false;
  machine->bb_seqs = htab_create_ggc (32, cil_basic_block_hash,
				      cil_basic_block_eq, NULL);

  return machine;
}

void
cil_override_options (void)
{
  init_machine_status = cil_init_machine_status;

  if (simd_backend_str)
    {
      if (strcmp (simd_backend_str, "mono") == 0)
        {
	  warning (0, "The Mono SIMD backend is unlikely to work\n");
          simd_type = MONO_SIMD;
        }
      else if (strcmp (simd_backend_str, "gcc") == 0)
        {
	  warning (0, "The Mono GCC backend is unlikely to work\n");
          simd_type = GCC_SIMD;
        }
      else if (strcmp (simd_backend_str, "generic") == 0)
        simd_type = GEN_SIMD;
      else
	{
	  fprintf (stderr, "Unknown SIMD backend '%s', using GENERIC\n",
		   simd_backend_str);
          simd_type = GEN_SIMD;
	}
    }
  else
    simd_type = GEN_SIMD;
}

/* Hash value calculation function for CIL basic blocks.  */

static hashval_t
cil_basic_block_hash (const void *ptr)
{
  const_cil_basic_block cbb = (const_cil_basic_block) ptr;

  return (hashval_t) ((long) cbb->bb >> 3);
}

/* Equality function for CIL basic blocks.  */

static int
cil_basic_block_eq (const void *ptr1, const void *ptr2)
{
  const_cil_basic_block cbb1 = (const_cil_basic_block) ptr1;
  const_cil_basic_block cbb2 = (const_cil_basic_block) ptr2;

  return cbb1->bb == cbb2->bb;
}

static tree
cil32_handle_function_attribute (tree *node, tree name, tree args,
				 int flags ATTRIBUTE_UNUSED,
				 bool *no_add_attrs)
{
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning (OPT_Wattributes, "%qs attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
      return NULL_TREE;
    }

  if (strcmp (IDENTIFIER_POINTER (name), "pinvoke") == 0)
    {
      add_pinvoke (*node);
      add_referenced_assembly (TREE_STRING_POINTER (TREE_VALUE (args)));
    }
  else if (strcmp (IDENTIFIER_POINTER (name), "assembly_name") == 0)
    add_referenced_assembly (TREE_STRING_POINTER (TREE_VALUE (args)));

  return NULL_TREE;
}

/* Table of valid machine attributes.  */
const struct attribute_spec cil32_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "assembly_name", 1,1,false,false,false, cil32_handle_function_attribute },
  { "cil_name",      1,1,false,false,false, cil32_handle_function_attribute },
  { "cil_strattr",   1,1,false,false,false, cil32_handle_function_attribute },
  { "pinvoke",       1,2,false,false,false, cil32_handle_function_attribute },
  { NULL,            0,0,false,false,false, NULL }
};

static void
cil32_file_start (void)
{
  refs_init ();
  emit_vcg_init ();
  emit_cil_init ();
}

static void
cil32_file_end (void)
{
  emit_cil_fini ();
  emit_vcg_fini ();
  refs_fini ();
}

static void
cil32_named_section (const char *name ATTRIBUTE_UNUSED,
                     unsigned int flags ATTRIBUTE_UNUSED,
                     tree decl ATTRIBUTE_UNUSED)
{
}

static void
cil32_globalize_label (FILE *stream ATTRIBUTE_UNUSED,
                       const char *name ATTRIBUTE_UNUSED)
{
}

static bool
cil32_assemble_integer (rtx x ATTRIBUTE_UNUSED,
                        unsigned int size ATTRIBUTE_UNUSED,
                        int aligned_p ATTRIBUTE_UNUSED)
{
  return true;
}



void
cil32_ivdep (struct cpp_reader * x ATTRIBUTE_UNUSED)
{
    warning (OPT_Wcil_pragma_ignored, "pragma ivdep not implemented yet");
}


void
cil32_loopdep_liberal (struct cpp_reader * x ATTRIBUTE_UNUSED)
{
    warning (OPT_Wcil_pragma_ignored, "pragma loopdep not implemented yet");
}

void
cil32_loopdep_parallel (struct cpp_reader * x ATTRIBUTE_UNUSED)
{
    warning (OPT_Wcil_pragma_ignored, "pragma loopdep not implemented yet");
}

void
cil32_loopdep_vector (struct cpp_reader * x ATTRIBUTE_UNUSED)
{
    warning (OPT_Wcil_pragma_ignored, "pragma loopdep not implemented yet");
}

static tree
cil32_gimplify_va_arg (tree valist, tree type, tree *pre_p ATTRIBUTE_UNUSED, tree *post_p ATTRIBUTE_UNUSED)
{
  tree fcall;
  tree ptr_type = build_pointer_type (type);
  tree arglist = build_tree_list (NULL_TREE, build_int_cstu (ptr_type, 0));
  arglist = tree_cons (NULL_TREE, valist, arglist);
  fcall = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_VA_ARG], arglist);
  TREE_TYPE (fcall) = ptr_type;
  return build1 (INDIRECT_REF, type, fcall);
}

/* Target hook for vector_mode_supported_p.  */
static bool
cil32_vector_mode_supported_p (enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return true;
}

static tree
cil32_builtin_get_vec_size (tree type ATTRIBUTE_UNUSED)
{
  return cil32_builtins[CIL32_GCC_GET_STEP];
}

static tree
cil32_builtin_get_vec_stride (tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VDI];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VSI];

        case 2:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VHI];

        case 1:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VQI];

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VDF];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_GET_STRIDE_VSF];

        default:
          return NULL_TREE;
        }
    }
}

static tree
cil32_builtin_get_vec_align (tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VDI];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VSI];

        case 2:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VHI];

        case 1:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VQI];

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VDF];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_GET_ALIGN_VSF];

        default:
          return NULL_TREE;
        }
    }
}

static tree cil32_builtin_build_uniform_vec (tree value ATTRIBUTE_UNUSED, 
                                             tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VDI];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VSI];

        case 2:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VHI];

        case 1:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VQI];

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VDF];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_BUILD_UNIFORM_VEC_VSF];

        default:
          return NULL_TREE;
        }
    }
}

static tree cil32_builtin_build_affine_vec (tree init ATTRIBUTE_UNUSED, 
                                            tree step ATTRIBUTE_UNUSED, 
                                            tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VDI];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VSI];

        case 2:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VHI];

        case 1:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VQI];

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
#if 0
        case 8:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VDF];
#endif
        case 4:
          return cil32_builtins[CIL32_GCC_BUILD_AFFINE_VEC_VSF];

        default:
          return NULL_TREE;
        }
    }
}


static tree cil32_builtin_build_reduc_epilogue (enum tree_code reduc_code, 
                                                tree type) 
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
#if 0
        case 8:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VDI];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VDI];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VDI];

            default:
              return NULL_TREE;
            }
#endif
        case 4:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VSI];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VSI];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VSI];

            default:
              return NULL_TREE;
            }


        case 2:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VHI];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VHI];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VHI];

            default:
              return NULL_TREE;
            }

        case 1:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VQI];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VQI];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VQI];

            default:
              return NULL_TREE;
            }

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
#if 0
        case 8:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VDF];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VDF];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VDF];

            default:
              return NULL_TREE;
            }
#endif

        case 4:
          switch (reduc_code)
            {
            case REDUC_MAX_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MAX_EPILOGUE_VSF];

            case REDUC_MIN_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_MIN_EPILOGUE_VSF];

            case REDUC_PLUS_EXPR:
              return cil32_builtins[CIL32_GCC_BUILD_REDUC_PLUS_EPILOGUE_VSF];

            default:
              return NULL_TREE;
            }

        default:
          return NULL_TREE;
        }
    }
}

static bool cil32_builtin_vectorize_independent_drs_only (void)
{
  return true;
}

static bool cil32_builtin_always_realign (void)
{
  if (TARGET_ALWAYS_REALIGN)
    return true;
  else
    return false;
}

static tree cil32_builtin_mask_for_load (void)
{
  return cil32_builtins[CIL32_GCC_MASK_FOR_LOAD];
}

static tree cil32_builtin_realign_load (tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
          case 1:
            return cil32_builtins[CIL32_GEN_VQI_REALIGN_LOAD];

          case 2:
            return cil32_builtins[CIL32_GEN_VHI_REALIGN_LOAD];

          case 4:
            return cil32_builtins[CIL32_GEN_VSI_REALIGN_LOAD];

          default:
            return NULL_TREE;
        }
    }
  else
    {
      if (element_size == 4)
        return cil32_builtins[CIL32_GEN_VSF_REALIGN_LOAD];
    }  

  return NULL_TREE;
}

static bool cil32_builtin_can_force_alignment (void)
{
  return false;
}


static tree cil32_builtin_get_loop_niters (void)
{
  return cil32_builtins[CIL32_GCC_GET_LOOP_NITERS];
}


static tree cil32_builtin_pattern (enum tree_code code, tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (code == DOT_PROD_EXPR && TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
          case 1:
            return cil32_builtins[CIL32_GEN_DOT_PRODUCT_VHI];

          case 2:
            return cil32_builtins[CIL32_GEN_DOT_PRODUCT_VSI];

          default:
            return NULL_TREE;
        }
    }

  return NULL_TREE;
}

static tree cil32_builtin_realign_offset (tree type)
{
  tree elem_type = TREE_TYPE (type);
  unsigned element_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (elem_type));

  if (TREE_CODE (elem_type) == INTEGER_TYPE)
    {
      switch (element_size)
        {
        case 4:
          return cil32_builtins[CIL32_GCC_REALIGN_OFFSET_VSI];

        case 2:
          return cil32_builtins[CIL32_GCC_REALIGN_OFFSET_VHI];

        case 1:
          return cil32_builtins[CIL32_GCC_REALIGN_OFFSET_VQI];

        default:
          return NULL_TREE;
        }
    }
  else
    {
      switch (element_size)
        {
        case 4:
          return cil32_builtins[CIL32_GCC_REALIGN_OFFSET_VSF];

        default:
          return NULL_TREE;
        }
    }
}


