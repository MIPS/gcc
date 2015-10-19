/* Producing binary form of HSA BRIG from our internal representation.
   Copyright (C) 2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "defaults.h"
#include "hard-reg-set.h"
#include "hash-set.h"
#include "is-a.h"
#include "vec.h"
#include "symtab.h"
#include "input.h"
#include "alias.h"
#include "double-int.h"
#include "inchash.h"
#include "tree.h"
#include "tree-iterator.h"
#include "machmode.h"
#include "stor-layout.h"
#include "tree-cfg.h"
#include "tree-ssa-alias.h"
#include "output.h"
#include "gimple-expr.h"
#include "dominance.h"
#include "cfg.h"
#include "function.h"
#include "predict.h"
#include "basic-block.h"
#include "fold-const.h"
#include "gimple.h"
#include "stringpool.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "hash-map.h"
#include "lto-streamer.h"
#include "cgraph.h"
#include "real.h"
#include "gimple-iterator.h"
#include "bitmap.h"
#include "dumpfile.h"
#include "alloc-pool.h"
#include "tree-ssa-operands.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "tree-ssanames.h"
#include "rtl.h"
#include "expr.h"
#include "tree-dfa.h"
#include "ssa-iterators.h"
#include "ipa-ref.h"
#include "gimplify-me.h"
#include "print-tree.h"
#include "cfghooks.h"
#include "symbol-summary.h"
#include "hsa.h"

#define BRIG_ELF_SECTION_NAME ".brig"
#define BRIG_LABEL_STRING "hsa_brig"
#define BRIG_SECTION_DATA_NAME    "hsa_data"
#define BRIG_SECTION_CODE_NAME    "hsa_code"
#define BRIG_SECTION_OPERAND_NAME "hsa_operand"

#define BRIG_CHUNK_MAX_SIZE (64 * 1024)

/* FIXME: The code below uses endian.h routines to convert numbers to
   little-endian.  I suspect this only works on glibc platforms, so we might
   need an alternative solution later.  */

/* Chunks of BRIG binary data.  */

struct hsa_brig_data_chunk
{
  /* Size of the data already stored into a chunk.  */
  unsigned size;

  /* Pointer to the data.  */
  char *data;
};

/* Structure representing a BRIG section, holding and writing its data.  */

class hsa_brig_section
{
public:
  /* Section name that will be output to the BRIG.  */
  const char *section_name;
  /* Size in bytes of all data stored in the section.  */
  unsigned total_size;
  /* The size of the header of the section including padding. */
  unsigned header_byte_count;
  /* The size of the header of the section without any padding.  */
  unsigned header_byte_delta;

  /* Buffers of binary data, each containing BRIG_CHUNK_MAX_SIZE bytes.  */
  vec <struct hsa_brig_data_chunk> chunks;

  /* More convenient access to the last chunk from the vector above. */
  struct hsa_brig_data_chunk *cur_chunk;

  void allocate_new_chunk ();
  void init (const char *name);
  void release ();
  void output ();
  unsigned add (const void *data, unsigned len);
  void round_size_up (int factor);
  void *get_ptr_by_offset (unsigned int offset);
};

static struct hsa_brig_section brig_data, brig_code, brig_operand;
static uint32_t brig_insn_count;
static bool brig_initialized = false;

/* Mapping between emitted HSA functions and their offset in code segment.  */
static hash_map<tree, BrigCodeOffset32_t> *function_offsets;

/* Set of emitted function declarations.  */
static hash_map <tree, BrigDirectiveExecutable *> *emitted_declarations;

/* List of sbr instructions.  */
static vec <hsa_insn_sbr *> *switch_instructions;

/* List of comment instructions.  */
static vec <hsa_insn_comment *> *comment_instructions;

struct function_linkage_pair
{
  function_linkage_pair (tree decl, unsigned int off):
    function_decl (decl), offset (off) {}

  /* Declaration of called function.  */
  tree function_decl;

  /* Offset in operand section.  */
  unsigned int offset;
};

/* Vector of function calls where we need to resolve function offsets.  */
static auto_vec <function_linkage_pair> function_call_linkage;

/* Add a new chunk, allocate data for it and initialize it.  */

void
hsa_brig_section::allocate_new_chunk ()
{
  struct hsa_brig_data_chunk new_chunk;

  new_chunk.data = XCNEWVEC (char, BRIG_CHUNK_MAX_SIZE);
  new_chunk.size = 0;
  cur_chunk = chunks.safe_push (new_chunk);
}

/* Initialize the brig section.  */

void
hsa_brig_section::init (const char *name)
{
  section_name = name;
  /* While the following computation is basically wrong, because the intent
     certainly wasn't to have the first character of name and padding, which
     are a part of sizeof (BrigSectionHeader), included in the first addend,
     this is what the disassembler expects.  */
  total_size = sizeof (BrigSectionHeader) + strlen(section_name);
  chunks.create (1);
  allocate_new_chunk ();
  header_byte_delta = total_size;
  round_size_up (4);
  header_byte_count = total_size;
}

/* Free all data in the section.  */

void
hsa_brig_section::release ()
{
  for (unsigned i = 0; i < chunks.length (); i++)
    free (chunks[i].data);
  chunks.release ();
  cur_chunk = NULL;
}

/* Write the section to the output file to a section with the name given at
   initialization.  Switches the output section and does not restore it.  */

void
hsa_brig_section::output ()
{
  struct BrigSectionHeader section_header;
  char padding[8];

  section_header.byteCount = htole64 (total_size);
  section_header.headerByteCount = htole32 (header_byte_count);
  section_header.nameLength = htole32 (strlen(section_name));
  assemble_string ((const char*) &section_header, 16);
  assemble_string (section_name, (section_header.nameLength));
  memset (&padding, 0, sizeof (padding));
  /* This is also a consequence of the wrong header size computation described
     in a comment in hsa_brig_section::init.  */
  assemble_string (padding, 8);
  for (unsigned i = 0; i < chunks.length (); i++)
    assemble_string (chunks[i].data, chunks[i].size);
}

/* Add to the stream LEN bytes of opaque binary DATA.  Return the offset at
   which it was stored.  */

unsigned
hsa_brig_section::add (const void *data, unsigned len)
{
  unsigned offset = total_size;

  gcc_assert (len <= BRIG_CHUNK_MAX_SIZE);
  if (cur_chunk->size > (BRIG_CHUNK_MAX_SIZE - len))
    allocate_new_chunk ();

  memcpy (cur_chunk->data + cur_chunk->size, data, len);
  cur_chunk->size += len;
  total_size += len;

  return offset;
}

/* Add padding to section so that its size is divisible by FACTOR.  */

void
hsa_brig_section::round_size_up (int factor)
{
  unsigned padding, res = total_size % factor;

  if (res == 0)
    return;

  padding = factor - res;
  total_size += padding;
  if (cur_chunk->size > (BRIG_CHUNK_MAX_SIZE - padding))
    {
      padding -= BRIG_CHUNK_MAX_SIZE - cur_chunk->size;
      cur_chunk->size = BRIG_CHUNK_MAX_SIZE;
      allocate_new_chunk ();
    }

  cur_chunk->size += padding;
}

/* Return pointer to data by global OFFSET in the section.  */

void*
hsa_brig_section::get_ptr_by_offset (unsigned int offset)
{
  gcc_assert (offset < total_size);

  offset -= header_byte_delta;
  unsigned int i;

  for (i = 0; offset >= chunks[i].size; i++)
    offset -= chunks[i].size;

  return chunks[i].data + offset;
}

/* BRIG string data hashing.  */

struct brig_string_slot
{
  const char *s;
  char prefix;
  int len;
  uint32_t offset;
};

/* Hash table helpers.  */

struct brig_string_slot_hasher : pointer_hash <brig_string_slot>
{
  static inline hashval_t hash (const value_type);
  static inline bool equal (const value_type, const compare_type);
  static inline void remove (value_type);
};

/* Returns a hash code for DS.  Adapted from libiberty's htab_hash_string
   to support strings that may not end in '\0'.  */

inline hashval_t
brig_string_slot_hasher::hash (const value_type ds)
{
  hashval_t r = ds->len;
  int i;

  for (i = 0; i < ds->len; i++)
     r = r * 67 + (unsigned)ds->s[i] - 113;
  r = r * 67 + (unsigned)ds->prefix - 113;
  return r;
}

/* Returns nonzero if DS1 and DS2 are equal.  */

inline bool
brig_string_slot_hasher::equal (const value_type ds1, const compare_type ds2)
{
  if (ds1->len == ds2->len)
    return ds1->prefix == ds2->prefix && memcmp (ds1->s, ds2->s, ds1->len) == 0;

  return 0;
}

/* Deallocate memory for DS upon its removal.  */

inline void
brig_string_slot_hasher::remove (value_type ds)
{
  free (const_cast<char*> (ds->s));
  free (ds);
}

/* Hash for strings we output in order not to duplicate them needlessly.  */

static hash_table<brig_string_slot_hasher> *brig_string_htab;

/* Emit a null terminated string STR to the data section and return its
   offset in it.  If PREFIX is non-zero, output it just before STR too.
   Sanitize the string if SANITIZE option is set to true.  */

static unsigned
brig_emit_string (const char *str, char prefix = 0, bool sanitize = true)
{
  unsigned slen = strlen (str);
  unsigned offset, len = slen + (prefix ? 1 : 0);
  uint32_t hdr_len = htole32 (len);
  brig_string_slot s_slot;
  brig_string_slot **slot;
  char *str2;

  /* XXX Sanitize the names without all the strdup.  */
  str2 = xstrdup (str);

  if (sanitize)
    hsa_sanitize_name (str2);
  s_slot.s = str2;
  s_slot.len = slen;
  s_slot.prefix = prefix;
  s_slot.offset = 0;

  slot = brig_string_htab->find_slot (&s_slot, INSERT);
  if (*slot == NULL)
    {
      brig_string_slot *new_slot = XCNEW (brig_string_slot);

      /* In theory we should fill in BrigData but that would mean copying
         the string to a buffer for no reason, so we just emulate it. */
      offset = brig_data.add (&hdr_len, sizeof (hdr_len));
      if (prefix)
        brig_data.add (&prefix, 1);

      brig_data.add (str2, slen);
      brig_data.round_size_up (4);

      /* XXX could use the string we just copied into brig_string->cur_chunk */
      new_slot->s = str2;
      new_slot->len = slen;
      new_slot->prefix = prefix;
      new_slot->offset = offset;
      *slot = new_slot;
    }
  else
    {
      offset = (*slot)->offset;
      free (str2);
    }

  return offset;
}

/* Linked list of queued operands.  */

static struct operand_queue
{
  /* First from the chain of queued operands.  */
  hsa_op_base *first_op, *last_op;

  /* The offset at which the next operand will be enqueued.  */
  unsigned projected_size;

} op_queue;

/* Unless already initialized, initialize infrastructure to produce BRIG.  */

static void
brig_init (void)
{
  brig_insn_count = 0;

  if (brig_initialized)
    return;

  brig_string_htab = new hash_table<brig_string_slot_hasher> (37);
  brig_data.init (BRIG_SECTION_DATA_NAME);
  brig_code.init (BRIG_SECTION_CODE_NAME);
  brig_operand.init (BRIG_SECTION_OPERAND_NAME);
  brig_initialized = true;

  struct BrigDirectiveModule moddir;
  memset (&moddir, 0, sizeof (moddir));
  moddir.base.byteCount = htole16 (sizeof (moddir));

  char *modname;
  if (main_input_filename && *main_input_filename != '\0')
    {
      const char *part = strrchr (main_input_filename, '/');
      if (!part)
	part = main_input_filename;
      else
	part++;
      asprintf (&modname, "&%s", part);
      char* extension = strchr (modname, '.');
      if (extension)
	*extension = '\0';

      /* As in LTO mode, we have to emit a different module names.  */
      if (flag_ltrans)
	{
	  part = strrchr (asm_file_name, '/');
	  if (!part)
	    part = asm_file_name;
	  else
	    part++;
	  char *modname2;
	  asprintf (&modname2, "%s_%s", modname, part);
	  free (modname);
	  modname = modname2;
	}

      hsa_sanitize_name (modname);
      moddir.name = brig_emit_string (modname);
      free (modname);
    }
  else
    moddir.name = brig_emit_string ("unnamed_brig_module", '&');
  moddir.base.kind = htole16 (BRIG_KIND_DIRECTIVE_MODULE);
  moddir.hsailMajor = htole32 (BRIG_VERSION_HSAIL_MAJOR) ;
  moddir.hsailMinor = htole32 (BRIG_VERSION_HSAIL_MINOR);
  moddir.profile = hsa_full_profile_p () ? BRIG_PROFILE_FULL: BRIG_PROFILE_BASE;
  if (hsa_machine_large_p ())
    moddir.machineModel = BRIG_MACHINE_LARGE;
  else
    moddir.machineModel = BRIG_MACHINE_SMALL;
  moddir.defaultFloatRound = BRIG_ROUND_FLOAT_DEFAULT;
  brig_code.add (&moddir, sizeof (moddir));
}

/* Free all BRIG data.  */

static void
brig_release_data (void)
{
  delete brig_string_htab;
  brig_data.release ();
  brig_code.release ();
  brig_operand.release ();

  brig_initialized = 0;
}

/* Enqueue operation OP.  Return the offset at which it will be stored.  */

static unsigned int
enqueue_op (hsa_op_base *op)
{
  unsigned ret;

  if (op->m_brig_op_offset)
    return op->m_brig_op_offset;

  ret = op_queue.projected_size;
  op->m_brig_op_offset = op_queue.projected_size;

  if (!op_queue.first_op)
    op_queue.first_op = op;
  else
    op_queue.last_op->m_next = op;
  op_queue.last_op = op;

  if (is_a <hsa_op_immed *> (op))
    op_queue.projected_size += sizeof (struct BrigOperandConstantBytes);
  else if (is_a <hsa_op_reg *> (op))
    op_queue.projected_size += sizeof (struct BrigOperandRegister);
  else if (is_a <hsa_op_address *> (op))
    op_queue.projected_size += sizeof (struct BrigOperandAddress);
  else if (is_a <hsa_op_code_ref *> (op))
    op_queue.projected_size += sizeof (struct BrigOperandCodeRef);
  else if (is_a <hsa_op_code_list *> (op))
    op_queue.projected_size += sizeof (struct BrigOperandCodeList);
  else
    gcc_unreachable ();
  return ret;
}


/* Emit directive describing a symbol if it has not been emitted already.
   Return the offset of the directive.  */

static unsigned
emit_directive_variable (struct hsa_symbol *symbol)
{
  struct BrigDirectiveVariable dirvar;
  unsigned name_offset;
  static unsigned res_name_offset;
  char prefix;

  if (symbol->m_directive_offset)
    return symbol->m_directive_offset;

  memset (&dirvar, 0, sizeof (dirvar));
  dirvar.base.byteCount = htole16 (sizeof (dirvar));
  dirvar.base.kind = htole16 (BRIG_KIND_DIRECTIVE_VARIABLE);
  dirvar.allocation = BRIG_ALLOCATION_AUTOMATIC;

  /* Readonly variables must have agent allocation.  */
  if (symbol->m_cst_value)
    dirvar.allocation = BRIG_ALLOCATION_AGENT;

  if (symbol->m_decl && is_global_var (symbol->m_decl))
    {
      prefix = '&';

      if (!symbol->m_cst_value)
	{
	  dirvar.allocation = BRIG_ALLOCATION_PROGRAM;
	  if (TREE_CODE (symbol->m_decl) == VAR_DECL)
	    warning (0, "referring to global symbol %q+D by name from HSA code "
		     "won't work", symbol->m_decl);
	}
    }
  else if (symbol->m_global_scope_p)
    prefix = '&';
  else
    prefix = '%';

  if (symbol->m_decl && TREE_CODE (symbol->m_decl) == RESULT_DECL)
    {
      if (res_name_offset == 0)
	res_name_offset = brig_emit_string (symbol->m_name, '%');
      name_offset = res_name_offset;
    }
  else if (symbol->m_name)
    name_offset = brig_emit_string (symbol->m_name, prefix);
  else
    {
      char buf[64];
      sprintf (buf, "__%s_%i", hsa_seg_name (symbol->m_segment),
	       symbol->m_name_number);
      name_offset = brig_emit_string (buf, prefix);
    }

  dirvar.name = htole32 (name_offset);
  dirvar.init = 0;
  dirvar.type = htole16 (symbol->m_type);
  dirvar.segment = symbol->m_segment;
  /* TODO: Once we are able to access global variables, we must copy their
     alignment.  */
  dirvar.align = MAX (hsa_natural_alignment (dirvar.type),
		      (BrigAlignment8_t) BRIG_ALIGNMENT_4);
  dirvar.linkage = symbol->m_linkage;
  dirvar.dim.lo = (uint32_t) symbol->m_dim;
  dirvar.dim.hi = (uint32_t) ((unsigned long long) symbol->m_dim >> 32);
  dirvar.modifier.allBits |= BRIG_VARIABLE_DEFINITION;
  dirvar.reserved = 0;

  if (symbol->m_cst_value)
    {
      dirvar.modifier.allBits |= BRIG_VARIABLE_CONST;
      dirvar.init = htole32 (enqueue_op (symbol->m_cst_value));
    }

  symbol->m_directive_offset = brig_code.add (&dirvar, sizeof (dirvar));
  return symbol->m_directive_offset;
}

/* Emit directives describing either a function declaration or
   definition F.  */

static BrigDirectiveExecutable *
emit_function_directives (hsa_function_representation *f, bool is_declaration)
{
  struct BrigDirectiveExecutable fndir;
  unsigned name_offset, inarg_off, scoped_off, next_toplev_off;
  int count = 0;
  BrigDirectiveExecutable *ptr_to_fndir;
  hsa_symbol *sym;

  if (!f->declaration_p)
    for (int i = 0; f->readonly_variables.iterate (i, &sym); i++)
      {
	emit_directive_variable (sym);
	brig_insn_count++;
      }

  name_offset = brig_emit_string (f->name, '&');
  inarg_off = brig_code.total_size + sizeof(fndir)
    + (f->output_arg ? sizeof (struct BrigDirectiveVariable) : 0);
  scoped_off = inarg_off
    + f->input_args_count * sizeof (struct BrigDirectiveVariable);

  if (!f->declaration_p)
    {
      for (hash_table <hsa_noop_symbol_hasher>::iterator iter
	     = f->local_symbols->begin ();
	   iter != f->local_symbols->end ();
	   ++iter)
	if (TREE_CODE ((*iter)->m_decl) == VAR_DECL)
	  count++;
      count += f->spill_symbols.length ();
    }

  next_toplev_off = scoped_off + count * sizeof (struct BrigDirectiveVariable);

  memset (&fndir, 0, sizeof (fndir));
  fndir.base.byteCount = htole16 (sizeof (fndir));
  fndir.base.kind = htole16 (f->kern_p ? BRIG_KIND_DIRECTIVE_KERNEL
			     : BRIG_KIND_DIRECTIVE_FUNCTION);
  fndir.name = htole32 (name_offset);
  fndir.inArgCount = htole16 (f->input_args_count);
  fndir.outArgCount = htole16 (f->output_arg ? 1 : 0);
  fndir.firstInArg = htole32 (inarg_off);
  fndir.firstCodeBlockEntry = htole32 (scoped_off);
  fndir.nextModuleEntry = htole32 (next_toplev_off);
  fndir.linkage = f->kern_p || TREE_PUBLIC (f->decl) ? BRIG_LINKAGE_PROGRAM :
    BRIG_LINKAGE_MODULE;

  if (!f->declaration_p)
    fndir.modifier.allBits |= BRIG_EXECUTABLE_DEFINITION;
  memset (&fndir.reserved, 0, sizeof (fndir.reserved));

  /* Once we put a definition of function_offsets, we should not overwrite
     it with a declaration of the function.  */
  if (!function_offsets->get (f->decl) || !is_declaration)
    function_offsets->put (f->decl, brig_code.total_size);

  brig_code.add (&fndir, sizeof (fndir));
  /* XXX terrible hack: we need to set instCount after we emit all
     insns, but we need to emit directive in order, and we emit directives
     during insn emitting.  So we need to emit the FUNCTION directive
     early, then the insns, and then we need to set instCount, so remember
     a pointer to it, in some horrible way.  cur_chunk.data+size points
     directly to after fndir here.  */
  ptr_to_fndir
      = (BrigDirectiveExecutable *)(brig_code.cur_chunk->data
                                    + brig_code.cur_chunk->size
                                    - sizeof (fndir));

  if (f->output_arg)
    emit_directive_variable (f->output_arg);
  for (unsigned i = 0; i < f->input_args_count; i++)
    emit_directive_variable (&f->input_args[i]);

  if (!f->declaration_p)
    {
      for (hash_table <hsa_noop_symbol_hasher>::iterator iter
	     = f->local_symbols->begin ();
	   iter != f->local_symbols->end ();
	   ++iter)
	{
	  if (TREE_CODE ((*iter)->m_decl) == VAR_DECL)
	    brig_insn_count++;
	  emit_directive_variable (*iter);
	}
      for (int i = 0; f->spill_symbols.iterate (i, &sym); i++)
	{
	  emit_directive_variable (sym);
	  brig_insn_count++;
	}

    }

  return ptr_to_fndir;
}

/* Emit a label directive for the given HBB.  We assume it is about to start on
   the current offset in the code section.  */

static void
emit_bb_label_directive (hsa_bb *hbb)
{
  struct BrigDirectiveLabel lbldir;
  char buf[32];

  lbldir.base.byteCount = htole16 (sizeof (lbldir));
  lbldir.base.kind = htole16 (BRIG_KIND_DIRECTIVE_LABEL);
  sprintf (buf, "BB_%u_%i", DECL_UID (current_function_decl), hbb->index);
  lbldir.name = htole32 (brig_emit_string (buf, '@'));

  hbb->label_ref.m_directive_offset = brig_code.add (&lbldir, sizeof (lbldir));
  brig_insn_count++;
}

/* Map a normal HSAIL type to the type of the equivalent BRIG operand
   holding such, for constants and registers.  */

static BrigType16_t
regtype_for_type (BrigType16_t t)
{
  switch (t)
    {
    case BRIG_TYPE_B1:
      return BRIG_TYPE_B1;

    case BRIG_TYPE_U8:
    case BRIG_TYPE_U16:
    case BRIG_TYPE_U32:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_B8:
    case BRIG_TYPE_B16:
    case BRIG_TYPE_B32:
    case BRIG_TYPE_F16:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_U8X4:
    case BRIG_TYPE_U16X2:
    case BRIG_TYPE_S8X4:
    case BRIG_TYPE_S16X2:
    case BRIG_TYPE_F16X2:
      return BRIG_TYPE_B32;

    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
    case BRIG_TYPE_F64:
    case BRIG_TYPE_B64:
    case BRIG_TYPE_U8X8:
    case BRIG_TYPE_U16X4:
    case BRIG_TYPE_U32X2:
    case BRIG_TYPE_S8X8:
    case BRIG_TYPE_S16X4:
    case BRIG_TYPE_S32X2:
    case BRIG_TYPE_F16X4:
    case BRIG_TYPE_F32X2:

      return BRIG_TYPE_B64;

    case BRIG_TYPE_B128:
    case BRIG_TYPE_U8X16:
    case BRIG_TYPE_U16X8:
    case BRIG_TYPE_U32X4:
    case BRIG_TYPE_U64X2:
    case BRIG_TYPE_S8X16:
    case BRIG_TYPE_S16X8:
    case BRIG_TYPE_S32X4:
    case BRIG_TYPE_S64X2:
    case BRIG_TYPE_F16X8:
    case BRIG_TYPE_F32X4:
    case BRIG_TYPE_F64X2:
      return BRIG_TYPE_B128;

    default:
      gcc_unreachable ();
    }
}

/* Return the length of the BRIG type TYPE that is going to be streamed out as
   an immediate constant (so it must not be B1).  */

unsigned
hsa_get_imm_brig_type_len (BrigType16_t type)
{
  BrigType16_t base_type = type & BRIG_TYPE_BASE_MASK;
  BrigType16_t pack_type = type & BRIG_TYPE_PACK_MASK;

  switch (pack_type)
    {
    case BRIG_TYPE_PACK_NONE:
      break;
    case BRIG_TYPE_PACK_32:
      return 4;
    case BRIG_TYPE_PACK_64:
      return 8;
    case BRIG_TYPE_PACK_128:
      return 16;
    default:
      gcc_unreachable ();
    }

  switch (base_type)
    {
    case BRIG_TYPE_U8:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_B8:
      return 1;
    case BRIG_TYPE_U16:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_F16:
    case BRIG_TYPE_B16:
      return 2;
    case BRIG_TYPE_U32:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_B32:
      return 4;
    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
    case BRIG_TYPE_F64:
    case BRIG_TYPE_B64:
      return 8;
    case BRIG_TYPE_B128:
      return 16;
    default:
      gcc_unreachable ();
    }
}

/* Emit one scalar VALUE to the buffer DATA intended for BRIG emission.
   If NEED_LEN is not equal to zero, shrink or extend the value
   to NEED_LEN bytes.  Return how many bytes were written.  */

static int
emit_immediate_scalar_to_buffer (tree value, char *data, unsigned need_len)
{
  union hsa_bytes bytes;

  memset (&bytes, 0, sizeof (bytes));
  tree type = TREE_TYPE (value);
  gcc_checking_assert (TREE_CODE (type) != VECTOR_TYPE);

  unsigned data_len = tree_to_uhwi (TYPE_SIZE (type))/BITS_PER_UNIT;
  if (INTEGRAL_TYPE_P (type)
      || (POINTER_TYPE_P (type) && TREE_CODE (value) == INTEGER_CST))
    switch (data_len)
      {
      case 1:
	bytes.b8 = (uint8_t) TREE_INT_CST_LOW (value);
	break;
      case 2:
	bytes.b16 = (uint16_t) TREE_INT_CST_LOW (value);
	break;
      case 4:
	bytes.b32 = (uint32_t) TREE_INT_CST_LOW (value);
	break;
      case 8:
	bytes.b64 = (uint64_t) int_cst_value (value);
	break;
      default:
	gcc_unreachable ();
      }
  else if (SCALAR_FLOAT_TYPE_P (type))
    {
      if (data_len == 2)
	{
	  sorry ("Support for HSA does not implement immediate 16 bit FPU "
		 "operands");
	  return 2;
	}
      unsigned int_len = GET_MODE_SIZE (TYPE_MODE (type));
      /* There are always 32 bits in each long, no matter the size of
	 the hosts long.  */
      long tmp[6];

      real_to_target (tmp, TREE_REAL_CST_PTR (value), TYPE_MODE (type));

      if (int_len == 4)
	bytes.b32 = (uint32_t) tmp[0];
      else
	{
	  bytes.b64 = (uint64_t)(uint32_t) tmp[1];
	  bytes.b64 <<= 32;
	  bytes.b64 |= (uint32_t) tmp[0];
	}
    }
  else
    gcc_unreachable ();

  int len;
  if (need_len == 0)
    len = data_len;
  else
    len = need_len;

  memcpy (data, &bytes, len);
  return len;
}

void
hsa_op_immed::emit_to_buffer (tree value)
{
  unsigned total_len = m_brig_repr_size;

  /* As we can have a constructor with fewer elements, fill the memory
     with zeros.  */
  m_brig_repr = XCNEWVEC (char, total_len);
  char *p = m_brig_repr;

  if (TREE_CODE (value) == VECTOR_CST)
    {
      int i, num = VECTOR_CST_NELTS (value);
      for (i = 0; i < num; i++)
	{
	  unsigned actual;
	  actual = emit_immediate_scalar_to_buffer
	    (VECTOR_CST_ELT (value, i), p, 0);
	  total_len -= actual;
	  p += actual;
	}
      /* Vectors should have the exact size.  */
      gcc_assert (total_len == 0);
    }
  else if (TREE_CODE (value) == STRING_CST)
    memcpy (m_brig_repr, TREE_STRING_POINTER (value),
	    TREE_STRING_LENGTH (value));
  else if (TREE_CODE (value) == COMPLEX_CST)
    {
      gcc_assert (total_len % 2 == 0);
      unsigned actual;
      actual = emit_immediate_scalar_to_buffer
	(TREE_REALPART (value), p, total_len / 2);

      gcc_assert (actual == total_len / 2);
      p += actual;

      actual = emit_immediate_scalar_to_buffer
	(TREE_IMAGPART (value), p, total_len / 2);
      gcc_assert (actual == total_len / 2);
    }
  else if (TREE_CODE (value) == CONSTRUCTOR)
    {
      unsigned len = vec_safe_length (CONSTRUCTOR_ELTS (value));
      for (unsigned i = 0; i < len; i++)
	{
	  unsigned actual = emit_immediate_scalar_to_buffer
	    (CONSTRUCTOR_ELT (value, i)->value, p, 0);
	  total_len -= actual;
	  p += actual;
	}
    }
  else
    emit_immediate_scalar_to_buffer (value, p, total_len);
}

/* Emit an immediate BRIG operand IMM.  The BRIG type of the immediate might
   have been massaged to comply with various HSA/BRIG type requirements, so the
   only important aspect of that is the length (because HSAIL might expect
   smaller constants or become bit-data).  The data should be represented
   according to what is in the tree representation.  */

static void
emit_immediate_operand (hsa_op_immed *imm)
{
  struct BrigOperandConstantBytes out;

  memset (&out, 0, sizeof (out));
  out.base.byteCount = htole16 (sizeof (out));
  out.base.kind = htole16 (BRIG_KIND_OPERAND_CONSTANT_BYTES);
  uint32_t byteCount = htole32 (imm->m_brig_repr_size);
  out.type = htole16 (imm->m_type);
  out.bytes = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_operand.add (&out, sizeof(out));
  brig_data.add (imm->m_brig_repr, imm->m_brig_repr_size);
  brig_data.round_size_up (4);
}

/* Emit a register BRIG operand REG.  */

static void
emit_register_operand (hsa_op_reg *reg)
{
  struct BrigOperandRegister out;

  out.base.byteCount = htole16 (sizeof (out));
  out.base.kind = htole16 (BRIG_KIND_OPERAND_REGISTER);
  out.regNum = htole32 (reg->m_hard_num);

  switch (regtype_for_type (reg->m_type))
    {
    case BRIG_TYPE_B32:
      out.regKind = BRIG_REGISTER_KIND_SINGLE;
      break;
    case BRIG_TYPE_B64:
      out.regKind = BRIG_REGISTER_KIND_DOUBLE;
      break;
    case BRIG_TYPE_B128:
      out.regKind = BRIG_REGISTER_KIND_QUAD;
      break;
    case BRIG_TYPE_B1:
      out.regKind = BRIG_REGISTER_KIND_CONTROL;
      break;
    default:
      gcc_unreachable ();
    }

  brig_operand.add (&out, sizeof (out));
}

/* Emit an address BRIG operand ADDR.  */

static void
emit_address_operand (hsa_op_address *addr)
{
  struct BrigOperandAddress out;

  out.base.byteCount = htole16 (sizeof (out));
  out.base.kind = htole16 (BRIG_KIND_OPERAND_ADDRESS);
  out.symbol = addr->m_symbol
    ? htole32 (emit_directive_variable (addr->m_symbol)) : 0;
  out.reg = addr->m_reg ? htole32 (enqueue_op (addr->m_reg)) : 0;

  /* FIXME: This is very clumsy.  */
  if (sizeof (addr->m_imm_offset) == 8)
    {
      out.offset.lo = htole32 ((uint32_t)addr->m_imm_offset);
      out.offset.hi = htole32 (((long long) addr->m_imm_offset) >> 32);
    }
  else
    {
      out.offset.lo = htole32 (addr->m_imm_offset);
      out.offset.hi = 0;
    }

  brig_operand.add (&out, sizeof (out));
}

/* Emit a code reference operand REF.  */

static void
emit_code_ref_operand (hsa_op_code_ref *ref)
{
  struct BrigOperandCodeRef out;

  out.base.byteCount = htole16 (sizeof (out));
  out.base.kind = htole16 (BRIG_KIND_OPERAND_CODE_REF);
  out.ref = htole32 (ref->m_directive_offset);
  brig_operand.add (&out, sizeof (out));
}

/* Emit a code list operand CODE_LIST.  */

static void
emit_code_list_operand (hsa_op_code_list *code_list)
{
  struct BrigOperandCodeList out;
  unsigned args = code_list->m_offsets.length ();

  for (unsigned i = 0; i < args; i++)
    gcc_assert (code_list->m_offsets[i]);

  out.base.byteCount = htole16 (sizeof (out));
  out.base.kind = htole16 (BRIG_KIND_OPERAND_CODE_LIST);

  uint32_t byteCount = htole32 (4 * args);

  out.elements = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (code_list->m_offsets.address (), args * sizeof (uint32_t));
  brig_data.round_size_up (4);
  brig_operand.add (&out, sizeof (out));
}

/* Emit all operands queued for writing.  */

static void
emit_queued_operands (void)
{
  for (hsa_op_base *op = op_queue.first_op; op; op = op->m_next)
    {
      gcc_assert (op->m_brig_op_offset == brig_operand.total_size);
      if (hsa_op_immed *imm = dyn_cast <hsa_op_immed *> (op))
	emit_immediate_operand (imm);
      else if (hsa_op_reg *reg = dyn_cast <hsa_op_reg *> (op))
	emit_register_operand (reg);
      else if (hsa_op_address *addr = dyn_cast <hsa_op_address *> (op))
	emit_address_operand (addr);
      else if (hsa_op_code_ref *ref = dyn_cast <hsa_op_code_ref *> (op))
	emit_code_ref_operand (ref);
      else if (hsa_op_code_list *code_list = dyn_cast <hsa_op_code_list *> (op))
	emit_code_list_operand (code_list);
      else
	gcc_unreachable ();
    }
}

/* Emit directives describing the function that is used for
a function declaration.  */

static BrigDirectiveExecutable *
emit_function_declaration (tree decl)
{
  hsa_function_representation *f = hsa_generate_function_declaration (decl);

  BrigDirectiveExecutable *e = emit_function_directives (f, true);
  emit_queued_operands ();

  delete f;

  return e;
}

/* Emit an HSA memory instruction and all necessary directives, schedule
   necessary operands for writing .  */

static void
emit_memory_insn (hsa_insn_mem *mem)
{
  struct BrigInstMem repr;
  BrigOperandOffset32_t operand_offsets[2];
  uint32_t byteCount;

  hsa_op_address *addr = as_a <hsa_op_address *> (mem->get_op (1));

  /* This is necessary because of the erroneous typedef of
     BrigMemoryModifier8_t which introduces padding which may then contain
     random stuff (which we do not want so that we can test things don't
     change).  */
  memset (&repr, 0, sizeof (repr));
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_MEM);
  repr.base.opcode = htole16 (mem->m_opcode);
  repr.base.type = htole16 (mem->m_type);

  operand_offsets[0] = htole32 (enqueue_op (mem->get_op (0)));
  operand_offsets[1] = htole32 (enqueue_op (mem->get_op (1)));
  /* We have two operands so use 4 * 2 for the byteCount.  */
  byteCount = htole32 (4 * 2);

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);

  if (addr->m_symbol)
    repr.segment = addr->m_symbol->m_segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  repr.modifier.allBits = 0 ;
  repr.equivClass = mem->m_equiv_class;
  repr.align = mem->m_align;
  if (mem->m_opcode == BRIG_OPCODE_LD)
    repr.width = BRIG_WIDTH_1;
  else
    repr.width = BRIG_WIDTH_NONE;
  memset (&repr.reserved, 0, sizeof (repr.reserved));
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA signal memory instruction and all necessary directives, schedule
   necessary operands for writing.  */

static void
emit_signal_insn (hsa_insn_signal *mem)
{
  struct BrigInstSignal repr;
  BrigOperandOffset32_t *operand_offsets = XCNEWVEC (BrigOperandOffset32_t,
						     mem->operand_count ());
  uint32_t byteCount;

  /* This is necessary because of the erroneous typedef of
     BrigMemoryModifier8_t which introduces padding which may then contain
     random stuff (which we do not want so that we can test things don't
     change).  */
  memset (&repr, 0, sizeof (repr));
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_SIGNAL);
  repr.base.opcode = htole16 (mem->m_opcode);
  repr.base.type = htole16 (mem->m_type);

  for (unsigned i = 0; i < mem->operand_count (); i++)
    operand_offsets[i] = htole32 (enqueue_op (mem->get_op (i)));

  /* We have N operands so use 4 * N for the byteCount.  */
  byteCount = htole32 (4 * mem->operand_count ());

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets, sizeof (BrigOperandOffset32_t) *
		 mem->operand_count ());
  brig_data.round_size_up (4);
  free (operand_offsets);

  repr.memoryOrder = mem->m_memoryorder;
  repr.signalOperation = mem->m_atomicop;
  repr.signalType = BRIG_TYPE_SIG64;

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA atomic memory instruction and all necessary directives, schedule
   necessary operands for writing .  */

static void
emit_atomic_insn (hsa_insn_atomic *mem)
{
  struct BrigInstAtomic repr;
  BrigOperandOffset32_t *operand_offsets = XCNEWVEC (BrigOperandOffset32_t,
						     mem->operand_count ());
  uint32_t byteCount;

  /* Either operand[0] or operand[1] must be an address operand.  */
  hsa_op_address *addr = NULL;
  if (is_a <hsa_op_address *> (mem->get_op (0)))
    addr = as_a <hsa_op_address *> (mem->get_op (0));
  else
    addr = as_a <hsa_op_address *> (mem->get_op (1));

  /* This is necessary because of the erroneous typedef of
     BrigMemoryModifier8_t which introduces padding which may then contain
     random stuff (which we do not want so that we can test things don't
     change).  */
  memset (&repr, 0, sizeof (repr));
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_ATOMIC);
  repr.base.opcode = htole16 (mem->m_opcode);
  repr.base.type = htole16 (mem->m_type);

  for (unsigned i = 0; i < mem->operand_count (); i++)
    operand_offsets[i] = htole32 (enqueue_op (mem->get_op (i)));

  /* We have N operands so use 4 * N for the byteCount.  */
  byteCount = htole32 (4 * mem->operand_count ());

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets, sizeof (BrigOperandOffset32_t) *
		 mem->operand_count ());
  brig_data.round_size_up (4);
  free (operand_offsets);

  if (addr->m_symbol)
    repr.segment = addr->m_symbol->m_segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  repr.memoryOrder = mem->m_memoryorder;
  repr.memoryScope = mem->m_memoryscope;
  repr.atomicOperation = mem->m_atomicop;

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA LDA instruction and all necessary directives, schedule
   necessary operands for writing .  */

static void
emit_addr_insn (hsa_insn_basic *insn)
{
  struct BrigInstAddr repr;
  BrigOperandOffset32_t *operand_offsets = XCNEWVEC (BrigOperandOffset32_t,
						     insn->operand_count ());
  uint32_t byteCount;

  hsa_op_address *addr = as_a <hsa_op_address *> (insn->get_op (1));

  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_ADDR);
  repr.base.opcode = htole16 (insn->m_opcode);
  repr.base.type = htole16 (insn->m_type);

  for (unsigned i = 0; i < insn->operand_count (); i++)
    operand_offsets[i] = htole32 (enqueue_op (insn->get_op (i)));

  /* We have N operands so use 4 * N for the byteCount.  */
  byteCount = htole32 (4 * insn->operand_count ());

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);
  free (operand_offsets);

  if (addr->m_symbol)
    repr.segment = addr->m_symbol->m_segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  memset (&repr.reserved, 0, sizeof (repr.reserved));

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA segment conversion instruction and all necessary directives,
   schedule necessary operands for writing .  */

static void
emit_segment_insn (hsa_insn_seg *seg)
{
  struct BrigInstSegCvt repr;
  BrigOperandOffset32_t operand_offsets[2];
  uint32_t byteCount;

  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_SEG_CVT);
  repr.base.opcode = htole16 (seg->m_opcode);
  repr.base.type = htole16 (seg->m_type);

  operand_offsets[0] = htole32 (enqueue_op (seg->get_op (0)));
  operand_offsets[1] = htole32 (enqueue_op (seg->get_op (1)));

  /* We have two operands so use 4 * 2 for the byteCount.  */
  byteCount = htole32 (4 * 2);

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);

  repr.sourceType = htole16 (as_a <hsa_op_reg *> (seg->get_op (1))->m_type);
  repr.segment = seg->m_segment;
  repr.modifier.allBits = 0;

  brig_code.add (&repr, sizeof (repr));

  brig_insn_count++;
}

/* Emit an HSA comparison instruction and all necessary directives,
   schedule necessary operands for writing .  */

static void
emit_cmp_insn (hsa_insn_cmp *cmp)
{
  struct BrigInstCmp repr;
  BrigOperandOffset32_t operand_offsets[3];
  uint32_t byteCount;

  memset (&repr, 0, sizeof (repr));
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_CMP);
  repr.base.opcode = htole16 (cmp->m_opcode);
  repr.base.type = htole16 (cmp->m_type);

  operand_offsets[0] = htole32 (enqueue_op (cmp->get_op (0)));
  operand_offsets[1] = htole32 (enqueue_op (cmp->get_op (1)));
  operand_offsets[2] = htole32 (enqueue_op (cmp->get_op (2)));
  /* We have three operands so use 4 * 3 for the byteCount.  */
  byteCount = htole32 (4 * 3);

  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);

  if (is_a <hsa_op_reg *> (cmp->get_op (1)))
    repr.sourceType = htole16 (as_a <hsa_op_reg *> (cmp->get_op (1))->m_type);
  else
    repr.sourceType = htole16 (as_a <hsa_op_immed *> (cmp->get_op (1))->m_type);
  repr.modifier.allBits = 0;
  repr.compare = cmp->m_compare;
  repr.pack = 0;

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA branching instruction and all necessary directives, schedule
   necessary operands for writing .  */

static void
emit_branch_insn (hsa_insn_br *br)
{
  struct BrigInstBr repr;
  BrigOperandOffset32_t operand_offsets[2];
  uint32_t byteCount;

  basic_block target = NULL;
  edge_iterator ei;
  edge e;

  /* At the moment we only handle direct conditional jumps.  */
  gcc_assert (br->m_opcode == BRIG_OPCODE_CBR);
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_BR);
  repr.base.opcode = htole16 (br->m_opcode);
  repr.width = BRIG_WIDTH_1;
  /* For Conditional jumps the type is always B1.  */
  repr.base.type = htole16 (BRIG_TYPE_B1);

  operand_offsets[0] = htole32 (enqueue_op (br->get_op (0)));

  FOR_EACH_EDGE (e, ei, br->m_bb->succs)
    if (e->flags & EDGE_TRUE_VALUE)
      {
	target = e->dest;
	break;
      }
  gcc_assert (target);
  operand_offsets[1] = htole32 (enqueue_op
				(&hsa_bb_for_bb (target)->label_ref));

  /* We have 2 operands so use 4 * 2 for the byteCount.  */
  byteCount = htole32 (4 * 2);
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);
  memset (&repr.reserved, 0, sizeof (repr.reserved));

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA unconditional jump branching instruction that points to
   a label REFERENCE.  */

static void
emit_unconditional_jump (hsa_op_code_ref *reference)
{
  struct BrigInstBr repr;
  BrigOperandOffset32_t operand_offsets[1];
  uint32_t byteCount;

  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_BR);
  repr.base.opcode = htole16 (BRIG_OPCODE_BR);
  repr.base.type = htole16 (BRIG_TYPE_NONE);
  /* Direct branches to labels must be width(all).  */
  repr.width = BRIG_WIDTH_ALL;

  operand_offsets[0] = htole32 (enqueue_op (reference));
  /* We have 1 operand so use 4 * 1 for the byteCount.  */
  byteCount = htole32 (4 * 1);
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);
  memset (&repr.reserved, 0, sizeof (repr.reserved));
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA switch jump instruction that uses a jump table to
   jump to a destination label.  */

static void
emit_switch_insn (hsa_insn_sbr *sbr)
{
  struct BrigInstBr repr;
  BrigOperandOffset32_t operand_offsets[2];
  uint32_t byteCount;

  gcc_assert (sbr->m_opcode == BRIG_OPCODE_SBR);
  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_BR);
  repr.base.opcode = htole16 (sbr->m_opcode);
  repr.width = BRIG_WIDTH_1;
  /* For Conditional jumps the type is always B1.  */
  hsa_op_reg *index = as_a <hsa_op_reg *> (sbr->get_op (0));
  repr.base.type = htole16 (index->m_type);
  operand_offsets[0] = htole32 (enqueue_op (sbr->get_op (0)));
  operand_offsets[1] = htole32 (enqueue_op (sbr->m_label_code_list));

  /* We have 2 operands so use 4 * 2 for the byteCount.  */
  byteCount = htole32 (4 * 2);
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);
  memset (&repr.reserved, 0, sizeof (repr.reserved));

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;

  /* Emit jump to default label.  */
  hsa_bb *hbb = hsa_bb_for_bb (sbr->m_default_bb);
  emit_unconditional_jump (&hbb->label_ref);
}

/* Emit a HSA convert instruction and all necessary directives, schedule
   necessary operands for writing.  */

static void
emit_cvt_insn (hsa_insn_basic *insn)
{
  struct BrigInstCvt repr;
  BrigType16_t srctype;
  auto_vec<BrigOperandOffset32_t, HSA_BRIG_INT_STORAGE_OPERANDS>
    operand_offsets;
  uint32_t byteCount, operand_count = insn->operand_count ();

  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_CVT);
  repr.base.opcode = htole16 (insn->m_opcode);
  repr.base.type = htole16 (insn->m_type);

  operand_offsets.safe_grow_cleared (operand_count);
  for (unsigned i = 0; i < operand_count; i++)
    {
      gcc_checking_assert (insn->get_op (i));
      operand_offsets[i] = htole32 (enqueue_op (insn->get_op (i)));
    }

  byteCount = htole32 (4 * operand_count) ;
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets.address (),
		 operand_count * sizeof (BrigOperandOffset32_t));

  if (is_a <hsa_op_reg *> (insn->get_op (1)))
    srctype = as_a <hsa_op_reg *> (insn->get_op (1))->m_type;
  else
    srctype = as_a <hsa_op_immed *> (insn->get_op (1))->m_type;
  repr.sourceType = htole16 (srctype);
  repr.modifier.allBits = 0;
  /* float to smaller float requires a rounding setting (we default
     to 'near'.  */
  if (hsa_type_float_p (insn->m_type)
      && (!hsa_type_float_p (srctype)
	  || ((insn->m_type & BRIG_TYPE_BASE_MASK)
	      < (srctype & BRIG_TYPE_BASE_MASK))))
    repr.round = BRIG_ROUND_FLOAT_NEAR_EVEN;
  else if (hsa_type_integer_p (insn->m_type) &&
	   hsa_type_float_p (srctype))
    repr.round = BRIG_ROUND_INTEGER_ZERO;
  else
    repr.round = BRIG_ROUND_NONE;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit call instruction INSN, where this instruction must be closed
   within a call block instruction.  */

static void
emit_call_insn (hsa_insn_basic *insn)
{
  hsa_insn_call *call = dyn_cast <hsa_insn_call *> (insn);
  struct BrigInstBr repr;
  uint32_t byteCount;

  BrigOperandOffset32_t operand_offsets[3];

  repr.base.base.byteCount = htole16 (sizeof (repr));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_BR);
  repr.base.opcode = htole16 (BRIG_OPCODE_CALL);
  repr.base.type = htole16 (BRIG_TYPE_NONE);

  /* Operand 0: out-args.  */
  operand_offsets[0] = htole32 (enqueue_op (call->result_code_list));

  /* Operand 1: func */
  unsigned int offset = enqueue_op (&call->func);
  function_call_linkage.safe_push
    (function_linkage_pair (call->called_function, offset));

  operand_offsets[1] = htole32 (offset);
  /* Operand 2: in-args.  */
  operand_offsets[2] = htole32 (enqueue_op (call->args_code_list));

  /* We have 3 operands so use 3 * 4 for the byteCount.  */
  byteCount = htole32 (3 * 4);
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (&operand_offsets, sizeof (operand_offsets));
  brig_data.round_size_up (4);
  repr.width = BRIG_WIDTH_ALL;
  memset (&repr.reserved, 0, sizeof (repr.reserved));

  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit argument block directive.  */

static void
emit_arg_block_insn (hsa_insn_arg_block *insn)
{
  switch (insn->kind)
    {
    case BRIG_KIND_DIRECTIVE_ARG_BLOCK_START:
      {
	struct BrigDirectiveArgBlockStart repr;
	repr.base.byteCount = htole16 (sizeof (repr));
	repr.base.kind = htole16 (insn->kind);
	brig_code.add (&repr, sizeof (repr));

	for (unsigned i = 0; i < insn->call_insn->input_args.length (); i++)
	  {
	    insn->call_insn->args_code_list->m_offsets[i] = htole32
	      (emit_directive_variable (insn->call_insn->input_args[i]));
	    brig_insn_count++;
	  }

	if (insn->call_insn->result_symbol)
	  {
	    insn->call_insn->result_code_list->m_offsets[0] = htole32
	      (emit_directive_variable (insn->call_insn->output_arg));
	    brig_insn_count++;
	  }

	break;
      }
    case BRIG_KIND_DIRECTIVE_ARG_BLOCK_END:
      {
	struct BrigDirectiveArgBlockEnd repr;
	repr.base.byteCount = htole16 (sizeof (repr));
	repr.base.kind = htole16 (insn->kind);
	brig_code.add (&repr, sizeof (repr));
	break;
      }
    default:
      gcc_unreachable ();
    }

  brig_insn_count++;
}

/* Emit comment directive.  */

static void
emit_comment_insn (hsa_insn_comment *insn)
{
  struct BrigDirectiveComment repr;
  memset (&repr, 0, sizeof (repr));

  repr.base.byteCount = htole16 (sizeof (repr));
  repr.base.kind = htole16 (insn->m_opcode);
  repr.name = brig_emit_string (insn->comment, '\0', false);
  brig_code.add (&repr, sizeof (repr));
}

/* Emit queue instruction INSN.  */

static void
emit_queue_insn (hsa_insn_queue *insn)
{
  BrigInstQueue repr;
  memset (&repr, 0, sizeof (repr));

  auto_vec<BrigOperandOffset32_t, HSA_BRIG_INT_STORAGE_OPERANDS>
    operand_offsets;
  uint32_t byteCount, operand_count = insn->operand_count ();

  repr.base.base.byteCount = htole16 (sizeof (BrigInstQueue));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_QUEUE);
  repr.base.opcode = htole16 (insn->m_opcode);
  repr.base.type = htole16 (insn->m_type);
  repr.segment = BRIG_SEGMENT_GLOBAL;
  repr.memoryOrder = BRIG_MEMORY_ORDER_SC_RELEASE;

  operand_offsets.safe_grow_cleared (operand_count);
  for (unsigned i = 0; i < operand_count; i++)
    {
      gcc_checking_assert (insn->get_op (i));
      operand_offsets[i] = htole32 (enqueue_op (insn->get_op (i)));
    }

  byteCount = htole32 (4 * operand_count) ;
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets.address (),
		 operand_count * sizeof (BrigOperandOffset32_t));
  brig_data.round_size_up (4);
  brig_code.add (&repr, sizeof (repr));

  brig_insn_count++;
}

/* Emit a basic HSA instruction and all necessary directives, schedule
   necessary operands for writing.  */

static void
emit_basic_insn (hsa_insn_basic *insn)
{
  /* We assume that BrigInstMod has a BrigInstBasic prefix.  */
  struct BrigInstMod repr;
  BrigType16_t type;
  auto_vec<BrigOperandOffset32_t, HSA_BRIG_INT_STORAGE_OPERANDS>
    operand_offsets;
  uint32_t byteCount, operand_count = insn->operand_count ();

  if (insn->m_opcode == BRIG_OPCODE_CVT)
    {
      emit_cvt_insn (insn);
      return;
    }

  memset (&repr, 0, sizeof (repr));
  repr.base.base.byteCount = htole16 (sizeof (BrigInstBasic));
  repr.base.base.kind = htole16 (BRIG_KIND_INST_BASIC);
  repr.base.opcode = htole16 (insn->m_opcode);
  switch (insn->m_opcode)
    {
      /* And the bit-logical operations need bit types and whine about
         arithmetic types :-/  */
      case BRIG_OPCODE_AND:
      case BRIG_OPCODE_OR:
      case BRIG_OPCODE_XOR:
      case BRIG_OPCODE_NOT:
	type = regtype_for_type (insn->m_type);
	break;
      default:
	type = insn->m_type;
	break;
    }
  repr.base.type = htole16 (type);

  operand_offsets.safe_grow_cleared (operand_count);
  for (unsigned i = 0; i < operand_count; i++)
    {
      gcc_checking_assert (insn->get_op (i));
      operand_offsets[i] = htole32 (enqueue_op (insn->get_op (i)));
    }

  byteCount = htole32 (4 * operand_count) ;
  repr.base.operands = htole32 (brig_data.add (&byteCount, sizeof (byteCount)));
  brig_data.add (operand_offsets.address (),
		 operand_count * sizeof (BrigOperandOffset32_t));
  brig_data.round_size_up (4);

  if ((type & BRIG_TYPE_PACK_MASK) != BRIG_TYPE_PACK_NONE)
    {
      if (hsa_type_float_p (type)
	  && !hsa_opcode_floating_bit_insn_p (insn->m_opcode))
	repr.round = BRIG_ROUND_FLOAT_NEAR_EVEN;
      else
	repr.round = 0;
      /* We assume that destination and sources agree in packing
         layout.  */
      if (insn->num_used_ops () >= 2)
	repr.pack = BRIG_PACK_PP;
      else
	repr.pack = BRIG_PACK_P;
      repr.reserved = 0;
      repr.base.base.byteCount = htole16 (sizeof (BrigInstMod));
      repr.base.base.kind = htole16 (BRIG_KIND_INST_MOD);
      brig_code.add (&repr, sizeof (struct BrigInstMod));
    }
  else
    brig_code.add (&repr, sizeof (struct BrigInstBasic));
  brig_insn_count++;
}

/* Emit an HSA instruction and all necessary directives, schedule necessary
   operands for writing .  */

static void
emit_insn (hsa_insn_basic *insn)
{
  gcc_assert (!is_a <hsa_insn_phi *> (insn));

  insn->m_brig_offset = brig_code.total_size;

  if (hsa_insn_signal *signal = dyn_cast <hsa_insn_signal *> (insn))
    {
      emit_signal_insn (signal);
      return;
    }
  if (hsa_insn_atomic *atom = dyn_cast <hsa_insn_atomic *> (insn))
    {
      emit_atomic_insn (atom);
      return;
    }
  if (hsa_insn_mem *mem = dyn_cast <hsa_insn_mem *> (insn))
    {
      emit_memory_insn (mem);
      return;
    }
  if (insn->m_opcode == BRIG_OPCODE_LDA)
    {
      emit_addr_insn (insn);
      return;
    }
  if (hsa_insn_seg *seg = dyn_cast <hsa_insn_seg *> (insn))
    {
      emit_segment_insn (seg);
      return;
    }
  if (hsa_insn_cmp *cmp = dyn_cast <hsa_insn_cmp *> (insn))
    {
      emit_cmp_insn (cmp);
      return;
    }
  if (hsa_insn_br *br = dyn_cast <hsa_insn_br *> (insn))
    {
      emit_branch_insn (br);
      return;
    }
  if (hsa_insn_sbr *sbr = dyn_cast <hsa_insn_sbr *> (insn))
    {
      if (switch_instructions == NULL)
	switch_instructions = new vec <hsa_insn_sbr *> ();

      switch_instructions->safe_push (sbr);
      emit_switch_insn (sbr);
      return;
    }

  if (hsa_insn_arg_block *arg_block = dyn_cast <hsa_insn_arg_block *> (insn))
    {
      emit_arg_block_insn (arg_block);
      return;
    }
  if (hsa_insn_call *call = dyn_cast <hsa_insn_call *> (insn))
    {
      emit_call_insn (call);
      return;
    }
  if (hsa_insn_comment *comment = dyn_cast <hsa_insn_comment *> (insn))
    {
      if (comment_instructions == NULL)
	comment_instructions = new vec <hsa_insn_comment *> ();

      comment_instructions->safe_push (comment);

      emit_comment_insn (comment);
      return;
    }
  if (hsa_insn_queue *queue = dyn_cast <hsa_insn_queue *> (insn))
    {
      emit_queue_insn (queue);
      return;
    }
  emit_basic_insn (insn);
}

/* We have just finished emitting BB and are about to emit NEXT_BB if non-NULL,
   or we are about to finish emiting code, if it is NULL.  If the fall through
   edge from BB does not lead to NEXT_BB, emit an unconditional jump.  */

static void
perhaps_emit_branch (basic_block bb, basic_block next_bb)
{
  basic_block t_bb = NULL, ff = NULL;

  edge_iterator ei;
  edge e;

  /* If the last instruction of BB is a switch, ignore emission of all
     edges.  */
  if (hsa_bb_for_bb (bb)->last_insn
      && is_a <hsa_insn_sbr *> (hsa_bb_for_bb (bb)->last_insn))
    return;

  FOR_EACH_EDGE (e, ei, bb->succs)
    if (e->flags & EDGE_TRUE_VALUE)
      {
	gcc_assert (!t_bb);
	t_bb = e->dest;
      }
    else
      {
	gcc_assert (!ff);
	ff = e->dest;
      }

  if (!ff || ff == next_bb || ff == EXIT_BLOCK_PTR_FOR_FN (cfun))
    return;

  emit_unconditional_jump (&hsa_bb_for_bb (ff)->label_ref);
}

/* Emit the a function with name NAME to the various brig sections.  */

void
hsa_brig_emit_function (void)
{
  basic_block bb, prev_bb;
  hsa_insn_basic *insn;
  BrigDirectiveExecutable *ptr_to_fndir;

  brig_init ();

  brig_insn_count = 0;
  memset (&op_queue, 0, sizeof (op_queue));
  op_queue.projected_size = brig_operand.total_size;

  if (!function_offsets)
    function_offsets = new hash_map<tree, BrigCodeOffset32_t> ();

  if (!emitted_declarations)
    emitted_declarations = new hash_map <tree, BrigDirectiveExecutable *> ();

  for (unsigned i = 0; i < hsa_cfun->called_functions.length (); i++)
    {
      tree called = hsa_cfun->called_functions[i];

      /* If the function has no definition, emit a declaration.  */
      if (!emitted_declarations->get (called))
	{
	  BrigDirectiveExecutable *e = emit_function_declaration (called);
	  emitted_declarations->put (called, e);
	}
    }

  ptr_to_fndir = emit_function_directives (hsa_cfun, false);
  for (insn = hsa_bb_for_bb (ENTRY_BLOCK_PTR_FOR_FN (cfun))->first_insn;
       insn;
       insn = insn->m_next)
    emit_insn (insn);
  prev_bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);
  FOR_EACH_BB_FN (bb, cfun)
    {
      perhaps_emit_branch (prev_bb, bb);
      emit_bb_label_directive (hsa_bb_for_bb (bb));
      for (insn = hsa_bb_for_bb (bb)->first_insn; insn; insn = insn->m_next)
	emit_insn (insn);
      prev_bb = bb;
    }
  perhaps_emit_branch (prev_bb, NULL);
  ptr_to_fndir->nextModuleEntry = brig_code.total_size;

  /* Fill up label references for all sbr instructions.  */
  if (switch_instructions)
    {
      for (unsigned i = 0; i < switch_instructions->length (); i++)
	{
	  hsa_insn_sbr *sbr = (*switch_instructions)[i];
	  for (unsigned j = 0; j < sbr->m_jump_table.length (); j++)
	    {
	      hsa_bb *hbb = hsa_bb_for_bb (sbr->m_jump_table[j]);
	      sbr->m_label_code_list->m_offsets[j] =
		hbb->label_ref.m_directive_offset;
	    }
	}
    }

  if (dump_file)
    {
      fprintf (dump_file, "------- After BRIG emission: -------\n");
      dump_hsa_cfun (dump_file);
    }

  if (comment_instructions)
    for (unsigned i = 0; i < comment_instructions->length (); i++)
      (*comment_instructions)[i]->release_string ();

  delete switch_instructions;
  switch_instructions = NULL;
  delete comment_instructions;
  comment_instructions = NULL;

  emit_queued_operands ();
}

/* Emit all OMP symbols related to OMP.  */

void
hsa_brig_emit_omp_symbols (void)
{
  brig_init ();
  emit_directive_variable (hsa_num_threads);
}

/* Unit constructor and destructor statements.  */

static GTY(()) tree hsa_ctor_statements;
static GTY(()) tree hsa_dtor_statements;

/* Create a static constructor that will register out brig stuff with
   libgomp.  */

static void
hsa_output_kernel_mapping (tree brig_decl)
{
  unsigned map_count = hsa_get_number_decl_kernel_mappings ();

  tree int_num_of_kernels;
  int_num_of_kernels = build_int_cst (uint32_type_node, map_count);
  tree kernel_num_index_type = build_index_type (int_num_of_kernels);
  tree host_functions_array_type = build_array_type (ptr_type_node,
						     kernel_num_index_type);

  vec<constructor_elt, va_gc> *host_functions_vec = NULL;
  for (unsigned i = 0; i < map_count; ++i)
    {
      tree decl = hsa_get_decl_kernel_mapping_decl (i);
      CONSTRUCTOR_APPEND_ELT
	(host_functions_vec, NULL_TREE,
	 build_fold_addr_expr (hsa_get_host_function (decl)));
    }
  tree host_functions_ctor = build_constructor (host_functions_array_type,
						host_functions_vec);
  char tmp_name[64];
  ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_host_functions", 1);
  tree hsa_host_func_table = build_decl (UNKNOWN_LOCATION, VAR_DECL,
					 get_identifier (tmp_name),
					 host_functions_array_type);
  TREE_STATIC (hsa_host_func_table) = 1;
  TREE_READONLY (hsa_host_func_table) = 1;
  TREE_PUBLIC (hsa_host_func_table) = 0;
  DECL_ARTIFICIAL (hsa_host_func_table) = 1;
  DECL_IGNORED_P (hsa_host_func_table) = 1;
  DECL_EXTERNAL (hsa_host_func_table) = 0;
  TREE_CONSTANT (hsa_host_func_table) = 1;
  DECL_INITIAL (hsa_host_func_table) = host_functions_ctor;
  varpool_node::finalize_decl (hsa_host_func_table);

  /* Following code emits list of kernel_info structures.  */

  tree kernel_info_type = make_node (RECORD_TYPE);
  tree id_f1 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("name"), ptr_type_node);
  DECL_CHAIN (id_f1) = NULL_TREE;
  tree id_f2 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("omp_data_size"),
			   unsigned_type_node);
  DECL_CHAIN (id_f2) = id_f1;
  tree id_f3 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("kernel_dependencies_count"),
			   unsigned_type_node);
  DECL_CHAIN (id_f3) = id_f2;
  tree id_f4 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("kernel_dependencies"),
			   build_pointer_type (build_pointer_type
					       (char_type_node)));
  DECL_CHAIN (id_f4) = id_f3;
  finish_builtin_struct (kernel_info_type, "__hsa_kernel_info", id_f4,
			 NULL_TREE);

  int_num_of_kernels = build_int_cstu (uint32_type_node, map_count);
  tree kernel_info_vector_type = build_array_type
    (kernel_info_type, build_index_type (int_num_of_kernels));

  vec<constructor_elt, va_gc> *kernel_info_vector_vec = NULL;
  tree kernel_dependencies_vector_type = NULL;

  for (unsigned i = 0; i < map_count; ++i)
    {
      tree kernel = hsa_get_decl_kernel_mapping_decl (i);
      char *name = hsa_get_decl_kernel_mapping_name (i);
      unsigned len = strlen (name);
      char *copy = XNEWVEC (char, len + 2);
      copy[0] = '&';
      memcpy (copy + 1, name, len);
      copy[len + 1] = '\0';
      len++;

      tree kern_name = build_string (len, copy);
      TREE_TYPE (kern_name) = build_array_type
	(char_type_node, build_index_type (size_int (len)));
      free (copy);

      unsigned omp_size = hsa_get_decl_kernel_mapping_omp_size (i);
      tree omp_data_size = build_int_cstu (uint32_type_node, omp_size);
      unsigned count = 0;

      kernel_dependencies_vector_type = build_array_type
	(build_pointer_type (char_type_node),
	 build_index_type (size_int (0)));

      vec<constructor_elt, va_gc> *kernel_dependencies_vec = NULL;
      if (hsa_decl_kernel_dependencies)
	{
	  vec<const char *> **slot;
	  slot = hsa_decl_kernel_dependencies->get (kernel);
	  if (slot)
	    {
	      vec <const char *> *dependencies = *slot;
	      count = dependencies->length ();

	      kernel_dependencies_vector_type = build_array_type
		(build_pointer_type (char_type_node),
		 build_index_type (size_int (count)));

	      for (unsigned j = 0; j < count; j++)
		{
		  const char *d = (*dependencies)[j];
		  len = strlen (d);
		  tree dependency_name = build_string (len, d);
		  TREE_TYPE (dependency_name) = build_array_type
		    (char_type_node, build_index_type (size_int (len)));

		  CONSTRUCTOR_APPEND_ELT
		    (kernel_dependencies_vec, NULL_TREE,
		     build1 (ADDR_EXPR,
			     build_pointer_type (TREE_TYPE (dependency_name)),
			     dependency_name));
		}
	    }
	}

      tree dependencies_count = build_int_cstu (uint32_type_node, count);

      vec<constructor_elt, va_gc> *kernel_info_vec = NULL;
      CONSTRUCTOR_APPEND_ELT (kernel_info_vec, NULL_TREE,
			      build1 (ADDR_EXPR,
				      build_pointer_type (TREE_TYPE
							  (kern_name)),
				      kern_name));
      CONSTRUCTOR_APPEND_ELT (kernel_info_vec, NULL_TREE, omp_data_size);
      CONSTRUCTOR_APPEND_ELT (kernel_info_vec, NULL_TREE, dependencies_count);

      tree kernel_info_ctor = build_constructor (kernel_info_type,
						 kernel_info_vec);

      if (count > 0)
	{
	  ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_dependencies_list", i);
	  tree dependencies_list = build_decl (UNKNOWN_LOCATION, VAR_DECL,
					       get_identifier (tmp_name),
					       kernel_dependencies_vector_type);

	  TREE_STATIC (dependencies_list) = 1;
	  TREE_READONLY (dependencies_list) = 1;
	  TREE_PUBLIC (dependencies_list) = 0;
	  DECL_ARTIFICIAL (dependencies_list) = 1;
	  DECL_IGNORED_P (dependencies_list) = 1;
	  DECL_EXTERNAL (dependencies_list) = 0;
	  TREE_CONSTANT (dependencies_list) = 1;
	  DECL_INITIAL (dependencies_list) = build_constructor
	    (kernel_dependencies_vector_type, kernel_dependencies_vec);
	  varpool_node::finalize_decl (dependencies_list);

	  CONSTRUCTOR_APPEND_ELT (kernel_info_vec, NULL_TREE,
				  build1 (ADDR_EXPR,
					  build_pointer_type
					    (TREE_TYPE (dependencies_list)),
					  dependencies_list));
	}
      else
	CONSTRUCTOR_APPEND_ELT (kernel_info_vec, NULL_TREE, null_pointer_node);

      CONSTRUCTOR_APPEND_ELT (kernel_info_vector_vec, NULL_TREE,
			      kernel_info_ctor);
    }

  ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_kernels", 1);
  tree hsa_kernels = build_decl (UNKNOWN_LOCATION, VAR_DECL,
				 get_identifier (tmp_name),
				 kernel_info_vector_type);

  TREE_STATIC (hsa_kernels) = 1;
  TREE_READONLY (hsa_kernels) = 1;
  TREE_PUBLIC (hsa_kernels) = 0;
  DECL_ARTIFICIAL (hsa_kernels) = 1;
  DECL_IGNORED_P (hsa_kernels) = 1;
  DECL_EXTERNAL (hsa_kernels) = 0;
  TREE_CONSTANT (hsa_kernels) = 1;
  DECL_INITIAL (hsa_kernels) = build_constructor (kernel_info_vector_type,
						  kernel_info_vector_vec);
  varpool_node::finalize_decl (hsa_kernels);

  tree hsa_image_desc_type = make_node (RECORD_TYPE);
  id_f1 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("brig_module"), ptr_type_node);
  DECL_CHAIN (id_f1) = NULL_TREE;
  id_f2 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("kernel_count"),
			   unsigned_type_node);

  DECL_CHAIN (id_f2) = id_f1;
  id_f3 = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier ("hsa_kernel_infos"),
			   ptr_type_node);
  DECL_CHAIN (id_f3) = id_f2;
  finish_builtin_struct (hsa_image_desc_type, "__hsa_image_desc", id_f3,
			 NULL_TREE);

  vec<constructor_elt, va_gc> *img_desc_vec = NULL;
  CONSTRUCTOR_APPEND_ELT (img_desc_vec, NULL_TREE,
			  build_fold_addr_expr (brig_decl));
  CONSTRUCTOR_APPEND_ELT (img_desc_vec, NULL_TREE,
			  build_int_cstu (unsigned_type_node, map_count));
  CONSTRUCTOR_APPEND_ELT (img_desc_vec, NULL_TREE,
			  build1 (ADDR_EXPR,
				  build_pointer_type (TREE_TYPE
						      (hsa_kernels)),
				  hsa_kernels));

  tree img_desc_ctor = build_constructor (hsa_image_desc_type, img_desc_vec);

  ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_img_descriptor", 1);
  tree hsa_img_descriptor = build_decl (UNKNOWN_LOCATION, VAR_DECL,
					get_identifier (tmp_name),
					hsa_image_desc_type);
  TREE_STATIC (hsa_img_descriptor) = 1;
  TREE_READONLY (hsa_img_descriptor) = 1;
  TREE_PUBLIC (hsa_img_descriptor) = 0;
  DECL_ARTIFICIAL (hsa_img_descriptor) = 1;
  DECL_IGNORED_P (hsa_img_descriptor) = 1;
  DECL_EXTERNAL (hsa_img_descriptor) = 0;
  TREE_CONSTANT (hsa_img_descriptor) = 1;
  DECL_INITIAL (hsa_img_descriptor) = img_desc_ctor;
  varpool_node::finalize_decl (hsa_img_descriptor);

  /* Construct the "host_table" libgomp expects. */
  tree libgomp_host_table_type = build_array_type (ptr_type_node,
						   build_index_type
						   (build_int_cst
						    (integer_type_node, 4)));
  vec<constructor_elt, va_gc> *libgomp_host_table_vec = NULL;
  tree host_func_table_addr = build_fold_addr_expr (hsa_host_func_table);
  CONSTRUCTOR_APPEND_ELT (libgomp_host_table_vec, NULL_TREE,
			  host_func_table_addr);
  offset_int func_table_size = wi::to_offset (TYPE_SIZE_UNIT (ptr_type_node))
    * map_count;
  CONSTRUCTOR_APPEND_ELT (libgomp_host_table_vec, NULL_TREE,
			  fold_build2 (POINTER_PLUS_EXPR,
				       TREE_TYPE (host_func_table_addr),
				       host_func_table_addr,
				       build_int_cst (size_type_node,
						      func_table_size.to_uhwi
						      ())));
  CONSTRUCTOR_APPEND_ELT (libgomp_host_table_vec, NULL_TREE, null_pointer_node);
  CONSTRUCTOR_APPEND_ELT (libgomp_host_table_vec, NULL_TREE, null_pointer_node);
  tree libgomp_host_table_ctor = build_constructor (libgomp_host_table_type,
						    libgomp_host_table_vec);
  ASM_GENERATE_INTERNAL_LABEL (tmp_name, "__hsa_libgomp_host_table", 1);
  tree hsa_libgomp_host_table = build_decl (UNKNOWN_LOCATION, VAR_DECL,
					    get_identifier (tmp_name),
					    libgomp_host_table_type);

  TREE_STATIC (hsa_libgomp_host_table) = 1;
  TREE_READONLY (hsa_libgomp_host_table) = 1;
  TREE_PUBLIC (hsa_libgomp_host_table) = 0;
  DECL_ARTIFICIAL (hsa_libgomp_host_table) = 1;
  DECL_IGNORED_P (hsa_libgomp_host_table) = 1;
  DECL_EXTERNAL (hsa_libgomp_host_table) = 0;
  TREE_CONSTANT (hsa_libgomp_host_table) = 1;
  DECL_INITIAL (hsa_libgomp_host_table) = libgomp_host_table_ctor;
  varpool_node::finalize_decl (hsa_libgomp_host_table);

  /* Generate an initializer with a call to the registration routine.  */

  tree offload_register = builtin_decl_explicit
    (BUILT_IN_GOMP_OFFLOAD_REGISTER);
  gcc_checking_assert (offload_register);

  append_to_statement_list
    (build_call_expr (offload_register, 3,
		      build_fold_addr_expr (hsa_libgomp_host_table),
		      /* 7 stands for HSA.  */
		      build_int_cst (integer_type_node, 7),
		      build_fold_addr_expr (hsa_img_descriptor)),
     &hsa_ctor_statements);

  cgraph_build_static_cdtor ('I', hsa_ctor_statements, DEFAULT_INIT_PRIORITY);

  tree offload_unregister = builtin_decl_explicit
    (BUILT_IN_GOMP_OFFLOAD_UNREGISTER);
  gcc_checking_assert (offload_unregister);

  append_to_statement_list
    (build_call_expr (offload_unregister,
		      3, build_fold_addr_expr (hsa_libgomp_host_table),
		      /* 7 stands for HSA.  */
		      build_int_cst (integer_type_node, 7),
		      build_fold_addr_expr (hsa_img_descriptor)),
     &hsa_dtor_statements);
  cgraph_build_static_cdtor ('D', hsa_dtor_statements, DEFAULT_INIT_PRIORITY);
}

/* Required HSA section alignment. */

#define HSA_SECTION_ALIGNMENT 16

/* Emit the brig module we have compiled to a section in the final assembly and
   also create a compile unit static constructor that will register the brig
   module with libgomp.  */

void
hsa_output_brig (void)
{
  section *saved_section;

  if (!brig_initialized)
    return;

  for (unsigned i = 0; i < function_call_linkage.length (); i++)
    {
      function_linkage_pair p = function_call_linkage[i];

      BrigCodeOffset32_t *func_offset = function_offsets->get (p.function_decl);
      gcc_assert (*func_offset);
      BrigOperandCodeRef *code_ref = (BrigOperandCodeRef *)
	(brig_operand.get_ptr_by_offset (p.offset));
      gcc_assert (code_ref->base.kind == BRIG_KIND_OPERAND_CODE_REF);
      code_ref->ref = htole32 (*func_offset);
    }

  /* Iterate all function declarations and if we meet a function that should
     have module linkage and we are unable to emit HSAIL for the function,
     then change the linkage to program linkage.  Doing so, we will emit
     a valid BRIG image.  */
  if (hsa_failed_functions != NULL && emitted_declarations != NULL)
    for (hash_map <tree, BrigDirectiveExecutable *>::iterator it =
	 emitted_declarations->begin (); it != emitted_declarations->end ();
	 ++it)
      {
	if (hsa_failed_functions->contains ((*it).first))
	  (*it).second->linkage = BRIG_LINKAGE_PROGRAM;
      }

  saved_section = in_section;

  switch_to_section (get_section (BRIG_ELF_SECTION_NAME, SECTION_NOTYPE, NULL));
  char tmp_name[64];
  ASM_GENERATE_INTERNAL_LABEL (tmp_name, BRIG_LABEL_STRING, 1);
  ASM_OUTPUT_LABEL (asm_out_file, tmp_name);
  tree brig_id = get_identifier (tmp_name);
  tree brig_decl = build_decl (UNKNOWN_LOCATION, VAR_DECL, brig_id,
			       char_type_node);
  SET_DECL_ASSEMBLER_NAME (brig_decl, brig_id);
  TREE_ADDRESSABLE (brig_decl) = 1;
  TREE_READONLY (brig_decl) = 1;
  DECL_ARTIFICIAL (brig_decl) = 1;
  DECL_IGNORED_P (brig_decl) = 1;
  TREE_STATIC (brig_decl) = 1;
  TREE_PUBLIC (brig_decl) = 0;
  TREE_USED (brig_decl) = 1;
  DECL_INITIAL (brig_decl) = brig_decl;
  TREE_ASM_WRITTEN (brig_decl) = 1;

  BrigModuleHeader module_header;
  memcpy (&module_header.identification, "HSA BRIG",
	  sizeof(module_header.identification));
  module_header.brigMajor = htole32 (BRIG_VERSION_BRIG_MAJOR);
  module_header.brigMinor = htole32 (BRIG_VERSION_BRIG_MINOR);
  uint64_t section_index[3];

  int data_padding, code_padding, operand_padding;
  data_padding = HSA_SECTION_ALIGNMENT
    - brig_data.total_size % HSA_SECTION_ALIGNMENT;
  code_padding = HSA_SECTION_ALIGNMENT
    - brig_code.total_size % HSA_SECTION_ALIGNMENT;
  operand_padding = HSA_SECTION_ALIGNMENT
    - brig_operand.total_size % HSA_SECTION_ALIGNMENT;

  uint64_t module_size = sizeof (module_header) + sizeof (section_index)
    + brig_data.total_size + data_padding
    + brig_code.total_size + code_padding
    + brig_operand.total_size + operand_padding;
  gcc_assert ((module_size % 16) == 0);
  module_header.byteCount = htole64 (module_size);
  memset (&module_header.hash, 0, sizeof (module_header.hash));
  module_header.reserved = 0;
  module_header.sectionCount = htole32 (3);
  module_header.sectionIndex = htole64 (sizeof (module_header));
  assemble_string ((const char *) &module_header, sizeof(module_header));
  uint64_t off = sizeof (module_header) + sizeof (section_index);
  section_index[0] = htole64 (off);
  off += brig_data.total_size + data_padding;
  section_index[1] = htole64 (off);
  off += brig_code.total_size + code_padding;
  section_index[2] = htole64 (off);
  assemble_string ((const char *) &section_index, sizeof (section_index));

  char padding[HSA_SECTION_ALIGNMENT];
  memset (padding, 0, sizeof(padding));

  brig_data.output ();
  assemble_string (padding, data_padding);
  brig_code.output ();
  assemble_string (padding, code_padding);
  brig_operand.output ();
  assemble_string (padding, operand_padding);

  if (saved_section)
    switch_to_section (saved_section);

  hsa_output_kernel_mapping (brig_decl);

  hsa_free_decl_kernel_mapping ();
  brig_release_data ();
  hsa_deinit_compilation_unit_data ();

  delete emitted_declarations;
  delete function_offsets;
}
