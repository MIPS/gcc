/* Generic data handling for GNU CC.
   Copyright (C) 2000, 2001 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "intl.h"
#include "tree.h"
#include "rtl.h"
#include "function.h"
#include "expr.h"
#include "c-tree.h"
#include "toplev.h"
#include "hashtab.h"
#include "md5.h"
#include "defaults.h"
#include "hash.h"
#include "ggc.h"
#include "tm_p.h"

/* Definitions of all the types we know.  */
typedef struct pch_writer_context
{
  htab_t pointer_h;
  unsigned n_constant[num_type_defs];
  unsigned n[num_type_defs];
  const void **ptrs[num_type_defs];
  unsigned max_size[num_type_defs];
  FILE *f;
  int *listd; 	/* Record what objects the listed variables point to.  */
  size_t listd_ndx;
  size_t string_size;
  char *string_table;
} *wctx_t;

struct pointer_ent
{
  const void *v;
  unsigned ndx;
  type_definition_p td;
};

static size_t constant_size_fetcher
PARAMS ((const void *, type_definition_p));

static code_type tree_code_fetcher PARAMS ((const void *));
static size_t tree_size_fetcher PARAMS ((const void *, type_definition_p ));
static int tree_more_fields PARAMS ((struct field_definition_s * ,
				     const void *,
				     unsigned , code_type ,
				     type_definition_p ));

static size_t rtx_size_fetcher PARAMS ((const void *, type_definition_p ));
static int rtx_more_fields PARAMS ((struct field_definition_s * ,
				    const void *,
				    unsigned, code_type ,
				    type_definition_p ));

static size_t rtvec_size_fetcher PARAMS ((const void *,
					  type_definition_p ));
static int rtvec_more_fields PARAMS ((struct field_definition_s * ,
				      const void *,
				      unsigned , code_type ,
				      type_definition_p ));

static size_t string_size_fetcher PARAMS ((const void *,
					   type_definition_p ));

static size_t regno_reg_rtx_size_fetcher PARAMS ((const void *,
						  type_definition_p ));
static int regno_reg_rtx_more_fields PARAMS ((struct field_definition_s * ,
					      const void *,
					      unsigned , code_type ,
					      type_definition_p ));
static size_t regno_pointer_flag_size_fetcher PARAMS (( const void *, type_definition_p ));

static size_t varray_size_fetcher PARAMS ((const void *,
					   type_definition_p ));
static int varray_more_fields PARAMS ((struct field_definition_s * ,
				       const void *,
				       unsigned , code_type ,
				       type_definition_p ));

static int typed_addresses_list_more_fields PARAMS ((struct field_definition_s * ,
	 const void *, unsigned , code_type , type_definition_p ));
static void setup_strtab PARAMS ((wctx_t));
static void record_listed_nodes PARAMS ((void **v_p, type_definition_p,
					 void *));
static size_t htab_size_fetcher PARAMS (( const void *, type_definition_p ));
static int htab_more_fields PARAMS (( struct field_definition_s *, const void *,
     				unsigned ,
     				code_type,
			        type_definition_p));

extern void add_hash_tree_addresses PARAMS ((typed_addresses_list_p list,
	 struct htab *p , size_t, const char *));

/* Size fetcher for structures with constant size; just returns SIZE from
   the type definition.  */

static size_t
constant_size_fetcher (v, td)
     const void *v ATTRIBUTE_UNUSED;
     type_definition_p td;
{
  return td->size;
}

static code_type
tree_code_fetcher (t_v)
     const void *t_v;
{
  tree t = (tree) t_v;
  return TREE_CODE (t) << 8 | TREE_CODE_CLASS (TREE_CODE (t));
}

static size_t
tree_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return tree_size ((tree) v);
}

static int
tree_more_fields (fp, t_v, n, c, td)
     struct field_definition_s *fp;
     const void *t_v;
     unsigned n;
     code_type c;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  const tree t = (tree) t_v;
  enum tree_code tc = (c >> 8) & 0xFF;

  if (tc == TREE_VEC && (long) n < TREE_VEC_LENGTH (t))
    {
      fp->offset = (offsetof (union tree_node, vec.a)
		    + n * sizeof (TREE_VEC_ELT (t, 0)));
      fp->type = tree_type_def;
      return 1;
    }
  else if (((c & 0xFF) == 'r' || (c & 0xFF) == '<'
	    || (c & 0xFF) == '1' || (c & 0xFF) == '2'
	    || (c & 0xFF) == 'e' || (c & 0xFF) == 's')
	   && (long) n < TREE_CODE_LENGTH (tc))
    {
      fp->offset = (offsetof (union tree_node, exp.operands)
		    + n * sizeof (TREE_OPERAND (t, 0)));
      fp->type =
	((long) n >= first_rtl_op (tc) ? rtx_type_def : tree_type_def);
      return 1;
    }

  return 0;
}

static struct field_definition_s tree_field_defs[] = {
  {0, 0, offsetof (union tree_node, common.chain), tree_type_def},
  {0, 0, offsetof (union tree_node, common.type), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.filename), string_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.size), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.size_unit), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.name), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.context), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.arguments), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.result), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.initial), tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.abstract_origin),
   tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.assembler_name),
   tree_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.section_name), tree_type_def},
#if 0
  {'d', 0xFF, offsetof (union tree_node, decl.machine_attributes), tree_type_def},
#endif
  {'d', 0xFF, offsetof (union tree_node, decl.rtl), rtx_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.live_range_rtl), rtx_type_def},
  {'d', 0xFF, offsetof (union tree_node, decl.vindex), tree_type_def},
  {FUNCTION_DECL << 8, 0xFF00,
   offsetof (union tree_node, decl.u2.f), function_type_def},
  {PARM_DECL << 8, 0xFF00,
   offsetof (union tree_node, decl.u2.r), rtx_type_def},
  {FIELD_DECL << 8, 0xFF00,
   offsetof (union tree_node, decl.u2.t), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.values), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.size), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.size_unit), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.attributes), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.pointer_to), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.reference_to), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.name), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.minval), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.maxval), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.next_variant), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.main_variant), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.binfo), tree_type_def},
  {'t', 0xFF, offsetof (union tree_node, type.context), tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.vars), tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.subblocks), tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.supercontext), tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.abstract_origin),
   tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.fragment_origin), tree_type_def},
  {'b', 0xFF, offsetof (union tree_node, block.fragment_chain), tree_type_def},

  {'c', 0xFF, offsetof (union tree_node, real_cst.rtl), rtx_type_def},
  {COMPLEX_CST << 8, 0xFF00,
   offsetof (union tree_node, complex.real), tree_type_def},
  {COMPLEX_CST << 8, 0xFF00,
   offsetof (union tree_node, complex.imag), tree_type_def},
  {STRING_CST << 8, 0xFF00,
   offsetof (union tree_node, string.pointer), string_type_def},
  {IDENTIFIER_NODE << 8, 0xFF00,
   offsetof (union tree_node, identifier.id.str), string_type_def},
  {TREE_LIST << 8, 0xFF00,
   offsetof (union tree_node, list.purpose), tree_type_def},
  {TREE_LIST << 8, 0xFF00,
   offsetof (union tree_node, list.value), tree_type_def},
  /* TREE_VEC_ELT is dealt with by MORE_FIELDS.  */
  /* expressions are dealt with by MORE_FIELDS.  */

  /* The remaining unused entries are available to hold such things as
     decl.lang_specific, type.lang_specific, and any extra fields
     introduced by a 'struct lang_identifier'.  Normally, you'd simply
     say

     { 'd', 0xFF, offsetof (union tree_node, decl.lang_specific), 
     lang_decl_type_def },
     { 't', 0xFF, offsetof (union tree_node, type.lang_specific), 
     lang_type_type_def },

     for the first two, but some frontends put bizzare things in these fields.
   */
  NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS,
  NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS,
  NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS, NO_MORE_FIELDS,
  NO_MORE_FIELDS
};


/* This is how languages handle adding their favourite stuff
at the end of a tree... */

void
add_tree_fields (fields)
     const struct field_definition_s *fields;
{
  unsigned i;
  enum
  {
    num_tree_fields = sizeof (tree_field_defs) / sizeof (tree_field_defs[0])
  };

  i = 0;
  while (tree_field_defs[i].type != NULL)
    i++;
  for (; i < num_tree_fields; i++)
    if (fields->type != NULL)
      tree_field_defs[i] = *fields++;
    else
      return;
  abort ();			/* Need to add more NO_MORE_FIELDS to tree_field_defs.  */
}

static size_t
rtx_size_fetcher (r_v, td)
     const void *r_v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return (sizeof (struct rtx_def) +
	  (GET_RTX_LENGTH (GET_CODE ((rtx) r_v)) - 1) * sizeof (rtunion));
}

static int
rtx_more_fields (fp, r_v, n, c, td)
     struct field_definition_s *fp;
     const void *r_v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  const rtx r = (rtx) r_v;
  char fc = GET_RTX_FORMAT (GET_CODE (r))[n];

  fp->offset = (offsetof (struct rtx_def, fld) + n * sizeof (rtunion));
  switch (fc)
    {
    case '\0':
      return 0;

    case 'e':
    case 'u':
      fp->type = rtx_type_def;
      break;
    case 'V':
    case 'E':
      fp->type = rtvec_type_def;
      break;
    case 'S':
    case 's':
    case 'T':
      fp->type = string_type_def;
      break;
    case 't':
      fp->type = tree_type_def;
      break;
    case '0':
      switch (GET_CODE (r))
	{
	case JUMP_INSN:
	case LABEL_REF:
	case CONST_DOUBLE:
	  fp->type = rtx_type_def;
	  break;
	case CODE_LABEL:
	  if (n == 3)
	    fp->type = NULL;
	  else
	    fp->type = rtx_type_def;
	  break;
	case MEM:
	case REG:
	case SCRATCH:
	case ADDR_DIFF_VEC:
	  fp->type = NULL;
	  break;

	case NOTE:
	  switch (NOTE_LINE_NUMBER (r))
	    {
	    case NOTE_INSN_RANGE_BEG:
	    case NOTE_INSN_RANGE_END:
	    case NOTE_INSN_LIVE:
	    case NOTE_INSN_EXPECTED_VALUE:
	      fp->type = rtx_type_def;
	      break;
	    case NOTE_INSN_BLOCK_BEG:
	    case NOTE_INSN_BLOCK_END:
	      fp->type = tree_type_def;
	      break;
	    case NOTE_INSN_DELETED_LABEL:
	      fp->type = string_type_def;
	      break;
	    default:
	      if (NOTE_LINE_NUMBER (r) >= 0)
		fp->type = string_type_def;
	      else
		fp->type = NULL;
	      break;
	    }
	  break;
	default:
	  abort ();
	}
      break;
    default:
      fp->type = NULL;
      break;
    }
  return 1;
}

static size_t
rtvec_size_fetcher (v_v, td)
     const void *v_v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return (sizeof (struct rtvec_def) +
	  (GET_NUM_ELEM ((rtvec) v_v) - 1) * sizeof (rtx));
}

static int
rtvec_more_fields (fp, v_v, n, c, td)
     struct field_definition_s *fp;
     const void *v_v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  if ((long) n < GET_NUM_ELEM ((rtvec) v_v))
    {
      fp->offset = (offsetof (struct rtvec_def, elem) + n * sizeof (rtx));
      fp->type = rtx_type_def;
      return 1;
    }
  return 0;
}

static size_t
string_size_fetcher (v_v, td)
     const void *v_v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return strlen ((char *) v_v) + 1;
}


static const struct field_definition_s function_field_defs[] = {
#if 1
  {0, 0, offsetof (struct function, next_global), function_type_def},
  {0, 0, offsetof (struct function, next), function_type_def},
#endif
  /*  struct eh_status *eh;
     struct stmt_status *stmt;
     struct expr_status *expr; */
  {0, 0, offsetof (struct function, emit), emit_status_type_def},
  /*  struct varasm_status *varasm; */
  {0, 0, offsetof (struct function, name), string_type_def},
  {0, 0, offsetof (struct function, decl), tree_type_def},
#if 0
  {0, 0, offsetof (struct function, outer), function_type_def},
#endif
  {0, 0, offsetof (struct function, arg_offset_rtx), rtx_type_def},
  /* CUMULATIVE_ARGS args_info; */
  {0, 0, offsetof (struct function, return_rtx), rtx_type_def},
  {0, 0, offsetof (struct function, cannot_inline), string_type_def},
  {0, 0, offsetof (struct function, x_nonlocal_labels), tree_type_def},
  {0, 0, offsetof (struct function, x_nonlocal_goto_handler_slots),
   rtx_type_def},
  {0, 0, offsetof (struct function, x_nonlocal_goto_handler_labels),
   rtx_type_def},
  {0, 0, offsetof (struct function, x_nonlocal_goto_stack_level),
   rtx_type_def},
  {0, 0, offsetof (struct function, x_cleanup_label), rtx_type_def},
  {0, 0, offsetof (struct function, x_return_label), rtx_type_def},
  {0, 0, offsetof (struct function, x_save_expr_regs), rtx_type_def},
  {0, 0, offsetof (struct function, x_stack_slot_list), rtx_type_def},
  {0, 0, offsetof (struct function, x_rtl_expr_chain), tree_type_def},
  {0, 0, offsetof (struct function, x_tail_recursion_label), rtx_type_def},
  {0, 0, offsetof (struct function, x_arg_pointer_save_area), rtx_type_def},
  {0, 0, offsetof (struct function, x_clobber_return_insn), rtx_type_def},
  {0, 0, offsetof (struct function, x_context_display), tree_type_def},
  {0, 0, offsetof (struct function, x_trampoline_list), tree_type_def},
  {0, 0, offsetof (struct function, x_parm_birth_insn), rtx_type_def},
  {0, 0, offsetof (struct function, x_last_parm_insn), rtx_type_def},
  /* rtx *x_parm_reg_stack_loc; */
  /* struct temp_slot *x_temp_slots; */
  /* struct var_refs_queue *fixup_var_refs_queue; */
  {0, 0, offsetof (struct function, original_arg_vector), rtvec_type_def},
  {0, 0, offsetof (struct function, original_decl_initial), tree_type_def},
  {0, 0, offsetof (struct function, inl_last_parm_insn), rtx_type_def},
  /* struct machine_function *machine */
  {0, 0, offsetof (struct function, language), lang_function_type_def},
  {0, 0, offsetof (struct function, epilogue_delay_list), rtx_type_def},
  NO_MORE_FIELDS
};

static size_t
regno_pointer_flag_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return ((struct emit_status *)v)->regno_pointer_align_length;
}

static size_t
regno_reg_rtx_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  return ((struct emit_status *) v)->regno_pointer_align_length * sizeof (rtx);
}

static int
regno_reg_rtx_more_fields (fp, v, n, c, td)
     struct field_definition_s *fp;
     const void *v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  struct emit_status *e = (struct emit_status *) v;

  if (n < (unsigned) e->regno_pointer_align_length)
    {
      fp->offset = n * sizeof (rtx);
      fp->type = rtx_type_def;
      return 1;
    }
  return 0;
}

static const struct field_definition_s emit_status_field_defs[] = {
  {0, 0, offsetof (struct emit_status, x_first_insn), rtx_type_def},
  {0, 0, offsetof (struct emit_status, x_last_insn), rtx_type_def},
  /* tree sequence_rtl_expr;  -- only valid while generating a sequence. */
  /* struct sequence_stack *sequence_stack; -- should be NULL.  */
  /* char *x_last_filename;  -- only used to emit NOTEs.  */
  NO_MORE_FIELDS
};

static const struct subobject_definition_s emit_status_subobject_defs[] = {
  { 0, 0, offsetof (struct emit_status, regno_pointer_align),
    regno_pointer_flag_size_fetcher, NULL },
  { 0, 0, offsetof (struct emit_status, x_regno_reg_rtx),
    regno_reg_rtx_size_fetcher, regno_reg_rtx_more_fields },
  NO_MORE_SUBOBJECTS
};

#define VARRAY_HEAD_SIZE \
  (sizeof (struct varray_head_tag) - sizeof (varray_data))

static size_t
varray_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  varray_type va = (varray_type) v;

  return va->num_elements * va->element_size + VARRAY_HEAD_SIZE;
}

static int
varray_more_fields (fp, v, n, c, td)
     struct field_definition_s *fp;
     const void *v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td;
{
  varray_type va = (varray_type) v;

  if (n < VARRAY_SIZE (va))
    {
      int i = type_to_type_index (td) + (tree_type_ndx - varray_tree_type_ndx);

      fp->offset =
	(offsetof (struct varray_head_tag, data) + n * va->element_size);
      fp->type = type_index_to_type (i);
      return 1;
    }
  return 0;
}

#define HTAB_HEAD_SIZE \
  (sizeof ( htab_t))

static size_t
htab_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  struct htab *va = (struct htab *) v;

  return va->size  + HTAB_HEAD_SIZE;
}

static int
htab_more_fields (fp, v, n, c, td)
     struct field_definition_s *fp;
     const void *v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td;
{
  struct htab *va = (struct htab *) v;

#if 0 
  if (n < VARRAY_SIZE (va))
    {
      int i = type_to_type_index (td) + (tree_type_ndx - htab_tree_type_ndx);

      fp->offset =
	(offsetof (struct htab_head_tag, data) + n * va->element_size);
      fp->type = type_index_to_type (i);
      return 1;
    }
#endif
  return 0;
}

static int
typed_addresses_list_more_fields (fp, v, n, c, td)
     struct field_definition_s *fp;
     const void *v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  typed_addresses_list_p l = (typed_addresses_list_p) v;

  if (n < TYPED_ADDRESSES_LIST_CHUNK_SIZE)
    {
      fp->offset = (offsetof (typed_addresses_list, ta)
		    + n * sizeof (struct typed_addresses_s)
		    + offsetof (struct typed_addresses_s, p));
      fp->type = l->ta[n].td;
      return 1;
    }
  else if (n == TYPED_ADDRESSES_LIST_CHUNK_SIZE)
    {
      fp->offset = offsetof (typed_addresses_list, next);
      fp->type = typed_addresses_list_type_def;
      return 1;
    }
  return 0;
}

/* A description of the pointer fields in a type.  */

struct type_definition_s basic_type_defs[num_type_defs] = {
  {				/* tree */
   tree_code_fetcher,
   tree_size_fetcher,
   NULL,
   sizeof (struct tree_common),
   tree_field_defs,
   tree_more_fields,
   NULL,
   1},
  {				/* rtx */
   NULL,
   rtx_size_fetcher,
   NULL,
   sizeof (struct rtx_def) - sizeof (rtunion),
   NULL,
   rtx_more_fields,
   NULL,
   1}
  ,
  {				/* rtvec */
   NULL,
   rtvec_size_fetcher,
   NULL,
   sizeof (struct rtvec_def) - sizeof (rtx),
   NULL,
   rtvec_more_fields,
   NULL,
   1}
  ,
  {				/* string */
   NULL, string_size_fetcher, NULL, 0, NULL, NULL, NULL, 1}
  ,
  /* struct lang_type, initialized by the language-specific frontends.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 1}
  ,
  /* struct lang_decl, initialized by the language-specific frontends.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 1}
  ,
  {				/* struct function */
   NULL,
   constant_size_fetcher,
   NULL,
   sizeof (struct function),
   function_field_defs,
   NULL,
   NULL,
   0},
  /* struct lang_function, initialized by the language-specific frontends.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 1},
  {				/* struct emit_status */
   NULL,
   constant_size_fetcher,
   NULL,
   sizeof (struct emit_status),
   emit_status_field_defs,
   NULL,
   emit_status_subobject_defs,
   0},
  /* The variable `typevec' in dbxout.c, initialized there.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0},
  /* struct dbx_file in dbxout.c, initialized there.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0},
  /* The various varray types, arranged in the same order as
     tree/rtx/rtvec/string above.  They are distinguished by their
     addresses.  */
  {NULL, htab_size_fetcher, NULL, HTAB_HEAD_SIZE, NULL,
   htab_more_fields, NULL, -1},
  {NULL, varray_size_fetcher, NULL, VARRAY_HEAD_SIZE, NULL,
   varray_more_fields, NULL, -1},
  {NULL, varray_size_fetcher, NULL, VARRAY_HEAD_SIZE, NULL,
   varray_more_fields, NULL, -1},
  {NULL, varray_size_fetcher, NULL, VARRAY_HEAD_SIZE, NULL,
   varray_more_fields, NULL, -1},
  {NULL, varray_size_fetcher, NULL, VARRAY_HEAD_SIZE, NULL,
   varray_more_fields, NULL, -1},
  {				/* typed_addresses_list */
   NULL,
   constant_size_fetcher,
   NULL,
   sizeof (typed_addresses_list),
   NULL,
   typed_addresses_list_more_fields,
   NULL,
   0}
  ,
  /* Language-specific types, initialized in the frontend.  */
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, -1}
  ,
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0}
  ,
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0}
  ,
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0}
  ,
  {NULL, constant_size_fetcher, NULL, 0, NULL, NULL, NULL, 0}
  ,
};

void
apply_pointer (p, td, pfn, param)
     void *p;
     type_definition_p td;
     pointerfn_fnp pfn;
     void *param;
{
  code_type c = 0;

  if (td->code_fetcher)
    c = td->code_fetcher (p);

  if (td->field_definitions != NULL)
    {
      int i;

      for (i = 0; td->field_definitions[i].type != NULL; i++)
	if ((c & td->field_definitions[i].mask)
	    == td->field_definitions[i].cond)
	  pfn ((void **) ((char *) p + td->field_definitions[i].offset),
	       td->field_definitions[i].type, param);
    }
  if (td->more_fields)
    {
      unsigned i = 0;
      struct field_definition_s fp;

      fp.cond = fp.mask = 0;
      while (td->more_fields (&fp, p, i++, c, td))
	if (fp.type != NULL)
	  pfn ((void **) ((char *) p + fp.offset), fp.type, param);
    }
  if (td->subobject_definitions)
    {
      subobject_definition_p sd;

      for (sd = td->subobject_definitions; sd->size_fetcher != NULL; sd++)
	if (sd->fields && ((c & sd->mask) == sd->cond))
	  {
	    unsigned j = 0;
	    struct field_definition_s fp;
	    char *sdf = (char *) *(void **) ((char *) p + sd->offset);

	    if (sdf)
	      while (sd->fields (&fp, p, j++, c, td))
		if (fp.type != NULL)
		  pfn ((void **) (sdf + fp.offset), fp.type, param);
	  }
    }
}


/* Apply FN with context CTX to every pointer reachable from V, and
apply SF to every subobject.  V has type TD.  SF may be NULL in which
case it is treated as returning 1 if and only if the pointer to the subobject
is not NULL.  */

void
apply_pointer_recursively (p_p, td_p, pfn, sfn, param)
     void *p_p;
     type_definition_p td_p;
     r_pointerfn_fnp pfn;
     s_pointerfn_fnp sfn;
     void *param;
{
  enum { worklist_size = 384 };
  typed_address worklist[worklist_size];
  unsigned worklist_next;

  worklist[0].p = p_p;
  worklist[0].td = td_p;
  worklist_next = 1;

  do
    {
      code_type c = 0;
      type_definition_p td;
      void *p;

      worklist_next--;
      p = worklist[worklist_next].p;
      td = worklist[worklist_next].td;

      if (td->code_fetcher)
	c = td->code_fetcher (p);

      if (td->field_definitions != NULL)
	{
	  int i;

	  for (i = 0; td->field_definitions[i].type != NULL; i++)
	    if ((c & td->field_definitions[i].mask)
		== td->field_definitions[i].cond)
	      {
		void **np = (void **) ((char *) p
				       + td->field_definitions[i].offset);
		type_definition_p ntd = td->field_definitions[i].type;

		if (pfn (np, ntd, param))
		  {
		    if (worklist_next < worklist_size)
		      {
			worklist[worklist_next].p = *np;
			worklist[worklist_next].td = ntd;
			worklist_next++;
		      }
		    else
		      apply_pointer_recursively (*np, ntd, pfn, sfn, param);
		  }
	      }
	}
      if (td->more_fields)
	{
	  unsigned i = 0;
	  struct field_definition_s fp;

	  fp.cond = fp.mask = 0;
	  while (td->more_fields (&fp, p, i++, c, td))
	    if (fp.type != NULL)
	      if (pfn ((void **) ((char *) p + fp.offset), fp.type, param))
		{
		  void *np = *(void **) ((char *) p + fp.offset);

		  if (worklist_next < worklist_size)
		    {
		      worklist[worklist_next].p = np;
		      worklist[worklist_next].td = fp.type;
		      worklist_next++;
		    }
		  else
		    apply_pointer_recursively (np, fp.type, pfn, sfn, param);
		}
	}
      if (td->subobject_definitions)
	{
	  subobject_definition_p sd;

	  for (sd = td->subobject_definitions; sd->size_fetcher != NULL; sd++)
	    if (sd->fields && ((c & sd->mask) == sd->cond))
	      {
		unsigned j = 0;
		struct field_definition_s fp;

		char *sdf = (char *) *(void **) ((char *) p + sd->offset);

		if ((!sfn && sdf)
		    || (sfn
			&& sfn (p, td, (void **) ((char *) p + sd->offset),
				sd, param)))
		  while (sd->fields (&fp, p, j++, c, td))
		    if (fp.type != NULL)
		      if (pfn ((void **) (sdf + fp.offset), fp.type, param))
			{
			  void *np = *(void **) (sdf + fp.offset);

			  if (worklist_next < worklist_size)
			    {
			      worklist[worklist_next].p = np;
			      worklist[worklist_next].td = fp.type;
			      worklist_next++;
			    }
			  else {
if (fp.offset == 36900 ) haltit();
			    apply_pointer_recursively (np, fp.type, pfn, sfn,
						       param);
			 }
			}
	      }
	}
    }
  while (worklist_next > 0);
}

/* Add a block of typed addresses to a list.  */


void
add_typed_addresses (list, p, td, count, name)
     typed_addresses_list_p list;
     void **p;
     type_definition_p td;
     size_t count;
     const char *name;
{
  int i;

#ifdef GT_DEBUG
  fprintf(stderr,"add_typed_addresses (%s)\n",name);
#endif
  while (list->next)
    list = list->next;
  if (list->ta[TYPED_ADDRESSES_LIST_CHUNK_SIZE - 1].p)
    list = list->next = xcalloc (1, sizeof (*list->next));
  for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
    if (!list->ta[i].p)
      {
	list->ta[i].p = p;
	list->ta[i].td = td;
	list->ta[i].count = count;
	list->ta[i].name = name;
	return;
      }
  abort ();
}

/* Add addresses of various types to a list.  */

void
add_tree_addresses (list, p, count, name)
     typed_addresses_list_p list;
     tree *p;
     size_t count;
     const char *name;
{
#ifdef GT_DEBUG
  fprintf(stderr,"add_tree_addresses (%s)\n",name);
#endif
  add_typed_addresses (list, (void **) p, tree_type_def, count, name);
}

void
add_rtx_addresses (list, p, count, name)
     typed_addresses_list_p list;
     rtx *p;
     size_t count;
     const char *name;
{
#ifdef GT_DEBUG
  fprintf(stderr,"add_rtx_addresses (%s)\n",name);
#endif
  add_typed_addresses (list, (void **) p, rtx_type_def, count, name);
}

void
add_hash_tree_addresses (list, p, count, name)
     typed_addresses_list_p list;
     struct htab *p;
     size_t count;
     const char *name;
{
#ifdef GT_DEBUG
  fprintf(stderr,"add_hash_tree_addresses (%s)\n",name);
#endif
  add_typed_addresses (list, (void **) p, hash_tree_type_def, count, name);
}

void
add_varray_tree_addresses (list, p, count, name)
     typed_addresses_list_p list;
     varray_type *p;
     size_t count;
     const char *name;
{
#ifdef GT_DEBUG
  fprintf(stderr,"add_varray_tree_addresses (%s)\n",name);
#endif
  add_typed_addresses (list, (void **) p, varray_tree_type_def, count, name);
}


/* Do something to each pointer in a list.  */

void
apply_list_pointers (list, fn, ctx)
     typed_addresses_list_p list;
     pointerfn_fnp fn;
     void *ctx;
{
  int i;
  size_t j;

  for (; list; list = list->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (list->ta[i].p && list->ta[i].td)
	for (j = list->ta[i].count; j > 0; j--)
	  fn (list->ta[i].p + (j - 1), list->ta[i].td, ctx);
}

/* Do something to each pointer referenced by a list.  */

void
apply_list_pointers_recursively (list, fn, sfn, ctx)
     typed_addresses_list_p list;
     r_pointerfn_fnp fn;
     s_pointerfn_fnp sfn;
     void *ctx;
{
  int i;
  size_t j;

  for (; list; list = list->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++) {
#ifdef GT_DEBUG
	  fprintf(stderr,"APLPR(%s)\n",list->ta[i].name);
#endif
      if (list->ta[i].p && list->ta[i].td)
	for (j = list->ta[i].count; j > 0; j--) 
	  if (fn (list->ta[i].p + (j - 1), list->ta[i].td, ctx)) 
	    apply_pointer_recursively (list->ta[i].p[j - 1], list->ta[i].td,
				       fn, sfn, ctx);
				       }
}


/* Add a block of untyped memory to a list.  */

void
add_untyped_address (list, p, size, name)
     typed_addresses_list_p list;
     void *p;
     size_t size;
     const char *name;
{
  int i;

#ifdef GT_DEBUG
  fprintf(stderr,"add_untyped_address (%s)\n",name);
#endif
  while (list->next)
    list = list->next;
  if (list->ta[TYPED_ADDRESSES_LIST_CHUNK_SIZE - 1].p)
    list = list->next = xcalloc (1, sizeof (*list->next));
  for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
    if (!list->ta[i].p)
      {
	list->ta[i].p = (void **) p;
	list->ta[i].td = NULL;
	list->ta[i].count = size;
	list->ta[i].name = name;
	return;
      }
  abort ();
}


/* Do something to each untyped memory block in a list.  */

void
apply_list_untyped (list, fn, ctx)
     typed_addresses_list_p list;
     untypedfn_fnp fn;
     void *ctx;
{
  int i;

  for (; list; list = list->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (list->ta[i].p && !list->ta[i].td)
	fn ((void *) list->ta[i].p, list->ta[i].count, ctx);
}

/* Remove an address (typed or untyped) from a list.  */

void
remove_list_address (list, p)
     typed_addresses_list_p list;
     void *p;
{
  int i;
  for (; list; list = list->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (list->ta[i].p == p)
	{
	  typed_addresses_list_p tail = list;
	  int taili;

	  while (tail->next)
	    tail = tail->next;
	  for (taili = 0; taili < TYPED_ADDRESSES_LIST_CHUNK_SIZE; taili++)
	    if (!tail->ta[taili].p)
	      break;

	  list->ta[i] = tail->ta[i];
	  tail->ta[i].p = NULL;
	  return;
	}
  abort ();
}

/* Count items in a list.  */

void
count_list_pointers (list, typed_p, untyped_p)
     typed_addresses_list_p list;
     size_t *typed_p;
     size_t *untyped_p;
{
  size_t typed, untyped;
  int i;

  typed = untyped = 0;
  for (; list; list = list->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (list->ta[i].p)
	{
	  if (list->ta[i].td)
	    typed += list->ta[i].count;
	  else
	    untyped++;
	}

  if (typed_p)
    *typed_p = typed;
  if (untyped_p)
    *untyped_p = untyped;
}


/* Update a checksum of the addresses in LIST.  If LIST refers to the
same list of pointers, the checksum shouldn't vary even between
different runs of the program.  Initially, set CHKSUM to contain
all zeros.  */

void
sum_type_addresses_list (cksum, list)
     unsigned char cksum[CHECKSUM_SIZE];
     typed_addresses_list_p list;
{
  struct md5_ctx ctx;
  unsigned char result[16];
  typed_addresses_list_p l;
  int i;

  md5_init_ctx (&ctx);
  md5_process_bytes (cksum, CHECKSUM_SIZE, &ctx);

  for (l = list; l; l = l->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (l->ta[i].p && !l->ta[i].td)
	{
	  md5_process_bytes (&l->ta[i].count, sizeof (l->ta[i].count), &ctx);
	  md5_process_bytes (l->ta[i].p, l->ta[i].count, &ctx);
	}

  for (l = list; l; l = l->next)
    for (i = 0; i < TYPED_ADDRESSES_LIST_CHUNK_SIZE; i++)
      if (l->ta[i].p && l->ta[i].td)
	{
	  int ndx;
	  type_definition_p td = l->ta[i].td;
	  size_t j;

	  md5_process_bytes (&l->ta[i].count, sizeof (l->ta[i].count), &ctx);
	  ndx = type_to_type_index (td);
	  md5_process_bytes (&ndx, sizeof (ndx), &ctx);
	  if (td->field_definitions == NULL && td->more_fields == NULL)
	    for (j = 0; j < l->ta[i].count; j++)
	      if (l->ta[i].p[j] && 0)
		{
		  size_t sz;
		  sz = td->size_fetcher (l->ta[i].p[j], td);
		  md5_process_bytes (&sz, sizeof (sz), &ctx);
		  md5_process_bytes (l->ta[i].p[j], sz, &ctx);
		}
	}

  md5_finish_ctx (&ctx, result);
  memcpy (cksum, result, CHECKSUM_SIZE);
}

struct pch_header
{
  unsigned n[num_type_defs];
  unsigned max_size;
  unsigned string_size;
};

typedef struct pch_reader_context
{
  struct pch_header h;
  unsigned n_constant[num_type_defs];
  void **r[num_type_defs];
  int *rv;
  size_t rvndx;
  FILE *f;
  int error;
}
 *rctx_t;

static void add_reader_pointer_directly PARAMS ((void **v,
						 type_definition_p td,
						 void *c_v));
static void correct_pointer PARAMS ((void **v,
				     type_definition_p td, void *c_v));
static void restore_variable PARAMS ((void **v,
				      type_definition_p td, void *c_v));
static void read_untyped PARAMS ((void *v, size_t sz, void *ctx));

static void
add_reader_pointer_directly (v, td, c_v)
     void **v;
     type_definition_p td;
     void *c_v;
{
  rctx_t c = (rctx_t) c_v;
  enum type_index_e i = type_to_type_index (td);

  c->r[i][c->n_constant[i]++] = *v;
}

static void
correct_pointer (x, td, c_v)
     void **x;
     type_definition_p td;
     void *c_v;
{
  rctx_t c = (rctx_t) c_v;
  enum type_index_e i = type_to_type_index (td);

#ifdef GT_DEBUG
fprintf(stderr,"correct_pointer ( x = %p )\n",*x);
#endif
  *x = c->r[i][(unsigned) (*x)];
}

static void
restore_variable (x, td, c_v)
     void **x;
     type_definition_p td;
     void *c_v;
{
  rctx_t c = (rctx_t) c_v;
  enum type_index_e i = type_to_type_index (td);
  int xi = c->rv[c->rvndx++];

#ifdef ENABLE_CHECKING
  if ((td->ggc_p <= 0)  && (*x != NULL) && (*x != c->r[i][xi]))
    warning ("PCH: leaking data structure at %x", *x);
#endif

  *x = c->r[i][xi];
}

static void
read_untyped (v, sz, c_v)
     void *v;
     size_t sz;
     void *c_v;
{
  rctx_t c = c_v;
  if (!c->error)
    if (fread (v, sz, 1, c->f) != 1)
      c->error = 1;
}


/* Restore a list.  KNOWN and LIST must point to the same list of pointers
as was originally written.  */

int
read_type_addresses_list (f, known, list)
     FILE *f;
     typed_addresses_list_p known;
     typed_addresses_list_p list;
{
  struct pch_reader_context c;
  char *buf;
  enum type_index_e i;
  unsigned j;
  size_t listsize;
  void *null = 0;

  memset((void *)&c, 0, sizeof(struct pch_reader_context));
  /* Read the header from the file.  */
  c.f = f;
  if (fread (&c.h, sizeof (c.h), 1, c.f) != 1)
    return -1;

  /* Allocate the main static arrays, initialise the context.  */
  for (i = 0; i < num_type_defs; i++)
    {
      c.n_constant[i] = 0;
      c.r[i] = xcalloc (c.h.n[i], sizeof (void *));

      /* Add NULL pointers for each kind of object.  */

      add_reader_pointer_directly (&null, type_index_to_type (i), &c);
    }

  /* Add the known pointers.  */
  apply_list_pointers (known, add_reader_pointer_directly, &c);

  c.error = 0;

  /* Read in the untyped data.  */
  apply_list_untyped (list, read_untyped, &c);

  if (c.error)
    return -1;

  buf = xmalloc (c.h.max_size);

  /* Read in the string table.  */
  if (c.h.n[string_type_ndx] > 0)
    {
      char *string_table;
      char *p;

      string_table = xmalloc (c.h.string_size);
      if (fread (string_table, c.h.string_size, 1, c.f) != 1)
	return -1;

      p = string_table;
      i = string_type_ndx;
      for (j = 1; j < c.h.n[i]; ++j)
	{
	  int len = strlen (p);
	  c.r[i][j] = (void *)ggc_alloc_string (p, len);
#ifdef GT_DEBUG
fprintf(stderr,"Reading (%s)\n",p);
#endif
	  p += len + 1;
	}

      if (p != (string_table + c.h.string_size))
	abort ();
      free (string_table);
    }

  /* Read in all the data... */
  for (i = 0; i < num_type_defs; i++)
    if (i != string_type_ndx)
      for (j = c.n_constant[i]; j < c.h.n[i]; j++)
	{
	  type_definition_p td = type_index_to_type (i);
	  size_t sz, readsize;

	  /* Read in enough to be able to determine the type's size.  */
	  readsize = td->size;
	  if (readsize != 0 && fread (buf, readsize, 1, c.f) != 1)
	    return -1;

	  /* There's a really annoying special case here for
	     TREE_VEC trees.  We need more information to be able
	     to determine their size.  */
	  if (i == tree_type_ndx && TREE_CODE ((tree) buf) == TREE_VEC)
	    {
	      if (fread (buf + readsize, sizeof (struct tree_vec) - readsize,
			 1, c.f) != 1)
		return -1;
	      readsize = sizeof (struct tree_vec);
	    }

	  sz = td->size_fetcher (buf, td);
	  if (sz != readsize)
	    if (fread (buf + readsize, sz - readsize, 1, c.f) != 1)
	      return -1;

	  /* Handle reading any subobjects.  */
	  if (td->subobject_definitions)
	    {
	      subobject_definition_p sd;
	      code_type code = 0;

	      if (td->code_fetcher)
		code = td->code_fetcher (buf);
	      for (sd = td->subobject_definitions;
		   sd->size_fetcher != NULL; sd++)
		if ((code & sd->mask) == sd->cond)
		  {
		    size_t sz;
		    char *sob;

		    if (fgetc (c.f) == 0)
		      {
			*(char **) (buf + sd->offset) = 0;
			continue;
		      }

		    sz = sd->size_fetcher (buf, td);
		    if (td->ggc_p > 0)
		      sob = ggc_alloc (sz);
		    else
		      sob = xmalloc (sz);
		    if (fread (sob, sz, 1, c.f) != 1)
		      return -1;
		    *(char **) (buf + sd->offset) = sob;
		  }
	    }

	  /* IDENTIFIER_NODEs must be canonicalized.  Ask
	     get_identifier what the canonical IDENTIFIER_NODE
	     should be.  */
	  if (i == tree_type_ndx && TREE_CODE ((tree) buf) == IDENTIFIER_NODE)
	    {
	      tree oldt = (tree) buf;
	      tree t;
#if 1
	      correct_pointer ((void **)IDENTIFIER_POINTER (oldt),
			       string_type_def, &c);
#else
	      correct_pointer ((void **) IDENTIFIER_POINTER (oldt),
			       identifier_type_def, &c);
#endif
	      t = get_identifier (IDENTIFIER_POINTER (oldt));

	      TREE_CHAIN (oldt) = TREE_CHAIN (t);
	      c.r[i][j] = t;
	    }
	  else 
	  if (td->ggc_p > 0)
	    c.r[i][j] = ggc_alloc (sz);
	  else
	    c.r[i][j] = xmalloc (sz);
	  memcpy (c.r[i][j], buf, sz);
	}
  free (buf);

  /* Fix up all the pointers in the just-read structures.  */
  for (i = 0; i < num_type_defs; i++)
    if (i != string_type_ndx)
      for (j = c.n_constant[i]; j < c.h.n[i]; j++)
	{
	  if (i == tree_type_ndx
	      && TREE_CODE ((tree) c.r[i][j]) == IDENTIFIER_NODE)
	    {
	      tree old_chain;
	      const char *old_identifier_pointer;

	      /* For IDENTIFIER_NODEs, the IDENTIFIER_POINTER and the
	         TREE_CHAIN are pointers at this point, so save and
	         restore them around the correction.  */
	      old_chain = TREE_CHAIN ((tree) c.r[i][j]);
	      old_identifier_pointer = IDENTIFIER_POINTER ((tree) c.r[i][j]);
	      TREE_CHAIN ((tree) c.r[i][j]) = (tree) 0;
	      IDENTIFIER_POINTER ((tree) c.r[i][j]) = (char *) 0; /* FIXME */

	      apply_pointer (c.r[i][j], type_index_to_type (i),
			     correct_pointer, &c);

	      TREE_CHAIN ((tree) c.r[i][j]) = old_chain;
	      IDENTIFIER_POINTER ((tree) c.r[i][j]) = old_identifier_pointer;/* FIXME */
	    }
	  else
	    apply_pointer (c.r[i][j], type_index_to_type (i), correct_pointer,
			   &c);
	}

  /* Store the values read into the list.  */
  count_list_pointers (list, &listsize, NULL);
  c.rv = xmalloc (listsize * sizeof (int));
  if (fread (c.rv, sizeof (int), listsize, c.f) != listsize)
    return -1;
  c.rvndx = 0;
  apply_list_pointers (list, restore_variable, &c);
  free (c.rv);
  for (i = 0; i < num_type_defs; i++)
    free (c.r[i]);

  return 0;
}


static int pointer_ent_eq PARAMS ((const void *, const void *));
static hashval_t pointer_ent_hash PARAMS ((const void *));
static unsigned add_one_pointer PARAMS ((void *v, type_definition_p td,
					 wctx_t ctx));
static int add_writer_pointer_wrapper PARAMS ((void **v, type_definition_p td,
					       void *ctx));
static void record_writer_pointer PARAMS ((void **v, type_definition_p td,
					   void *ctx));
static void add_writer_pointer_directly PARAMS ((void **v,
						 type_definition_p td,
						 void *ctx));
static int prewrite_pointer PARAMS ((void **, void *));
static void pickle_pointer PARAMS ((void **v, type_definition_p td,
				    void *ctx));
static int write_kind PARAMS ((enum type_index_e, wctx_t));
static void write_untyped PARAMS ((void *v, size_t sz, void *ctx));

static int
pointer_ent_eq (a_p, b_p)
     const void *a_p;
     const void *b_p;
{
  const struct pointer_ent *a = (const struct pointer_ent *) a_p;
  const struct pointer_ent *b = (const struct pointer_ent *) b_p;

  return a->v == b->v && a->td == b->td;
}

static hashval_t
pointer_ent_hash (a_p)
     const void *a_p;
{
  const struct pointer_ent *a = (const struct pointer_ent *) a_p;

  return (hashval_t) a->v;
}

/* Add the object pointer V, of type TD, to the hash table in C, recording
   its index for later use.  */

static unsigned
add_one_pointer (v, td, c)
     void *v;
     type_definition_p td;
     wctx_t c;
{
  struct pointer_ent e;
  int i = type_to_type_index (td);
  void **slot;

  e.v = v;
  e.td = td;
  e.ndx = c->n[i];
  slot = htab_find_slot (c->pointer_h, &e, INSERT);
  if (*slot == NULL)
    {
      *slot = ggc_alloc (sizeof (e));
      memcpy (*slot, &e, sizeof (e));
      if (td == string_type_def)
	c->string_size += (v == NULL ? 1 : td->size_fetcher (v, td));
      c->n[i]++;
      return e.ndx;
    }
  else
    return ((struct pointer_ent *) *slot)->ndx;
}

static int
add_writer_pointer_wrapper (v_p, td, c_v)
     void **v_p;
     type_definition_p td;
     void *c_v;
{
  wctx_t c = (void *) c_v;
  int i = type_to_type_index (td);
  unsigned old_n = c->n[i];
  unsigned ndx;

  ndx = add_one_pointer (*v_p, td, c);
  return ndx == old_n;
}

static void
record_writer_pointer (v_p, td, c_v)
     void **v_p;
     type_definition_p td;
     void *c_v;
{
  wctx_t c = c_v;
  int i = type_to_type_index (td);
  unsigned old_n = c->n[i];
  unsigned ndx;

  ndx = add_one_pointer (*v_p, td, c);
  if (ndx == old_n)
    apply_pointer_recursively (*v_p, td, add_writer_pointer_wrapper, NULL,
			       c_v);
}

/* Record the indices of the objects pointed to by a variable in the
   data_to_save list.  This must happen after the indices in the hash table
   are final; i.e. after any sorting done in setup_strtab.  */

static void
record_listed_nodes (v_p, td, c_v)
     void **v_p;
     type_definition_p td;
     void *c_v;
{
  wctx_t c = c_v;
  struct pointer_ent e;
  struct pointer_ent *p;

  e.v = *v_p;
  e.td = td;
  e.ndx = 0;			/* ndx should never be used by the lookup.  */

  p = (struct pointer_ent *) htab_find (c->pointer_h, &e);
  if (p == NULL)
    abort ();

  c->listd[c->listd_ndx++] = p->ndx;
}

static void
add_writer_pointer_directly (x_p, td, c_v)
     void **x_p;
     type_definition_p td;
     void *c_v;
{
  wctx_t c = (wctx_t) c_v;
  int i = type_to_type_index (td);

  if (c->n[i] != c->n_constant[i])
    abort ();
  add_one_pointer (*x_p, td, c);
  c->n_constant[i]++;
  c->n[i] = c->n_constant[i];
}

static int
prewrite_pointer (slot, c_v)
     void **slot;
     void *c_v;
{
  wctx_t c = c_v;
  const struct pointer_ent *e = (const struct pointer_ent *) *slot;
  int i = type_to_type_index (e->td);
  size_t sz = e->v == NULL ? 0 : e->td->size_fetcher (e->v, e->td);

  c->ptrs[i][e->ndx] = e->v;

  if (c->max_size[i] < sz)
    c->max_size[i] = sz;

  if (e->v && e->td->prewrite_hook)
    e->td->prewrite_hook (e->v);

  return 1;
}

/*
 * Serialize pointer
 */
static void
pickle_pointer (x, td, c_v)
     void **x;
     type_definition_p td;
     void *c_v;
{
  wctx_t c = c_v;
  struct pointer_ent e;
  struct pointer_ent *p;

  e.v = *x;
  e.td = td;
  e.ndx = 0;			/* ndx should never be used by the lookup.  */
  p = (struct pointer_ent *) htab_find (c->pointer_h, &e);
  if (p == NULL)
    abort ();
  *x = (void *) p->ndx;
}

/*
 * Write out actual data
 */
static int
write_kind (i, c)
     enum type_index_e i;
     wctx_t c;
{
  type_definition_p td = type_index_to_type (i);
  unsigned j;
  const unsigned maxj = c->n[i];
  char *d = xmalloc (c->max_size[i]);

  for (j = c->n_constant[i]; j < maxj; j++)
    {
      const void *x = c->ptrs[i][j];
      size_t sz = td->size_fetcher (x, td);
      code_type code = 0;

      memcpy (d, x, sz);

      if (td->code_fetcher)
	code = td->code_fetcher (d);

      /* Copy any subobjects.  */
      if (td->subobject_definitions)
	{
	  subobject_definition_p sd;

	  for (sd = td->subobject_definitions; sd->size_fetcher != NULL; sd++)
	    if ((code & sd->mask) == sd->cond)
	      {
		char **loc = (char **) (d + sd->offset);

		if (*loc)
		  {
		    size_t sz = sd->size_fetcher (d, td);
		    char *sob = xmalloc (sz);
		    memcpy (sob, *loc, sz);
		    *loc = sob;
		  }
	      }
	}

      /* Serialize pointer d */
      apply_pointer (d, td, pickle_pointer, c);

      if (fwrite (d, sz, 1, c->f) != 1)
	return -1;

      /* Write any subobjects.  */
      if (td->subobject_definitions)
	{
	  subobject_definition_p sd;

	  for (sd = td->subobject_definitions; sd->size_fetcher != NULL; sd++)
	    if ((code & sd->mask) == sd->cond)
	      {
		size_t sz = sd->size_fetcher (d, td);
		char *sob = *(char **) (d + sd->offset);
		int r;

		if (sob)
		  {
		    r = putc (1, c->f);
		    if (r >= 0)
		      r = fwrite (sob, sz, 1, c->f);
		    free (sob);
		    if (r != 1)
		      return -1;
		  }
		else
		  {
		    r = putc (0, c->f);
		    if (r < 0)
		      return -1;
		  }
	      }
	}
    }

  free (d);
  return 0;
}

/*
 * Write out untyped data
 */
static void
write_untyped (v, sz, c_v)
     void *v;
     size_t sz;
     void *c_v;
{
  wctx_t c = c_v;
  fwrite (v, sz, 1, c->f);
}

#if 0
/* FIXME Don't bother sorting the strings until we update to Zack's string
   table code; otherwise we have no way of guaranteeing increasing
   addresses when we read the strings back in.  */

/* qsort comparison function for sorting pointers by address.  */

static int
pointer_compare (a_p, b_p)
     const void *a_p;
     const void *b_p;
{
  const void *a = *(const void **) a_p;
  const void *b = *(const void **) b_p;

  if (a == b)
    return 0;
  else if (a < b)
    return -1;
  else
    return 1;
}
#endif

/* We have accumulated the strings in c->ptrs like any other object.
   But we want to treat them specially, in two ways:
     - We want to read and write them as one large block rather than individual
       objects, since the only way to determine their length at read time is
       to search for the null.
     - We want them to be emitted in ascending address order, so that we can
       preserve that order for sorted lists in the C and C++ frontends.

   This processing must happen before pickling.  */

static void
setup_strtab (c)
     wctx_t c;
{
  unsigned int i;
  size_t string_idx;
  struct pointer_ent e;
  struct pointer_ent *p;
  int ndx = string_type_ndx;
  type_definition_p td = string_type_def;

#if 0
  /* Sort the strings by address (not alphabetically).  */
  qsort (c->ptrs[ndx] + c->n_constant[ndx],
	 c->n[ndx] - c->n_constant[ndx], sizeof (void *), pointer_compare);
#endif

  /* Set up the string table.  */
  c->string_table = xcalloc (c->string_size, 1);
  e.td = string_type_def;
  e.ndx = 0;			/* ndx should never be used by the lookup.  */

  /* The NULL pointer will always be at index 0; skip it so we don't need
     to handle it in the loop.  */
  string_idx = 1;
  if (c->n_constant[ndx] != 1)
    abort ();
  for (i = c->n_constant[ndx]; i < c->n[ndx]; ++i)
    {
      const void *ob = c->ptrs[ndx][i];
      size_t sz = td->size_fetcher (ob, td);

      e.v = ob;
      p = (struct pointer_ent *) htab_find (c->pointer_h, &e);
      if (p == NULL)
	abort ();

      p->ndx = i;
      memcpy (c->string_table + string_idx, ob, sz);
      string_idx += sz;
    }
}


/* Write out a list.   
KNOWN is a list of pointers that need not be written.  */

int
write_type_addresses_list (f, known, list)
     FILE *f;
     typed_addresses_list_p known;
     typed_addresses_list_p list;
{
  struct pch_writer_context c;
  struct pch_header h;
  int i;
  size_t listsize;
  void *null = NULL;

  memset((void *)&c, 0, sizeof(struct pch_writer_context));
  c.pointer_h = htab_create (2040, pointer_ent_hash, pointer_ent_eq, NULL);
  c.string_size = 0;
  for (i = 0; i < num_type_defs; i++)
    {
      c.n_constant[i] = 0;
      c.n[i] = 0;
      /* Add NULL pointers for each kind of object.  */
      add_writer_pointer_directly (&null, type_index_to_type (i), &c);
    }

  /* Add the well-known pointers.  */
  apply_list_pointers (known, add_writer_pointer_directly, &c);

  /* Add the data to write; set up the list.  */
  apply_list_pointers (list, record_writer_pointer, &c);

  /* Set up the ptrs[] array.  */
  for (i = 0; i < num_type_defs; i++)
    {
      c.ptrs[i] = xcalloc (c.n[i], sizeof (void *));
      c.max_size[i] = 0;
    }
  c.f = f;
  htab_traverse (c.pointer_h, prewrite_pointer, &c);

  /* Set up the string table.  */
  setup_strtab (&c);

  /* Set up the header.  */
  for (i = 0; i < num_type_defs; i++)
    h.n[i] = c.n[i];

  /* Don't bother with the initial null.  */
  h.string_size = c.string_size - 1;

  /* Compute the maximum size.  */
  h.max_size = 0;
  for (i = 0; i < num_type_defs; i++)
    if (c.max_size[i] > h.max_size)
      h.max_size = c.max_size[i];

  /* Write out the header.  */
  if (fwrite (&h, sizeof (h), 1, f) != 1)
    return -1;

  /* Write out the untyped data.  */
  apply_list_untyped (list, write_untyped, &c);

  /* Write out the strings.  */
  if (h.string_size > 0)
    if (fwrite (c.string_table + 1, h.string_size, 1, f) != 1)
      return -1;

  /* Write out the actual data.  */
  for (i = 0; i < num_type_defs; i++)
    if (i != string_type_ndx)
      write_kind (i, &c);

  for (i = 0; i < num_type_defs; i++)
    free (c.ptrs[i]);

  /* Record what objects the listed variables point to.  */
  count_list_pointers (list, &listsize, NULL);
  c.listd = xmalloc (listsize * sizeof (int));
  c.listd_ndx = 0;
  apply_list_pointers (list, record_listed_nodes, &c);

  /* Write out LIST.  */
  if (fwrite (c.listd, sizeof (int), listsize, c.f) != listsize)
    return -1;
  free (c.listd);

  return 0;
}

/* Various global variables used by the precompiled header machinery.  */
typed_addresses_list data_to_save;
typed_addresses_list known_pointers;
