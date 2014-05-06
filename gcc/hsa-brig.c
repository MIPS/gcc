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
#include "hsa.h"
#include "tm.h"
#include "tree.h"
#include "stor-layout.h"
#include "tree-cfg.h"
#include "machmode.h"
#include "output.h"
#include "basic-block.h"
#include "vec.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"

#define BRIG_SECTION_STRING_NAME ".brig_strtab"
#define BRIG_SECTION_DIRECTIVE_NAME ".brig_directives"
#define BRIG_SECTION_CODE_NAME ".brig_code"
#define BRIG_SECTION_OPERAND_NAME ".brig_operands"
#define BRIG_SECTION_DEBUG_NAME ".brig_debug"

#define BRIG_CHUNK_MAX_SIZE (64 * 1024)

/* FIXME: The code below uses endian.h routines to convert numbers to
   little-endian.  I suspect this only works on glibc platforms, so we might
   need an alternative solutin later.  */

/* Chunks of BRIG binary data.  */

struct hsa_brig_data_chunk
{
  /* Size of the data already stored into a chunk.  */
  unsigned size;

  /* Pointer to the data.  */
  char *data;
};

/* Structure represeting a BRIC section, holding and writing its data.  */

class hsa_brig_section
{
public:
  /* Size in bytes of all data stored in the section.  */
  unsigned total_size;

  /* Buffers of binary data, each containing BRIG_CHUNK_MAX_SIZE bytes.  */
  vec <struct hsa_brig_data_chunk> chunks;

  /* More convenient access to the last chunk from the vector above. */
  struct hsa_brig_data_chunk *cur_chunk;

  void allocate_new_chunk ();
  void init ();
  void release ();
  void output (const char *section_name);
  unsigned add (const void *data, unsigned len);
  void round_size_up (int factor);
};

static struct hsa_brig_section brig_string, brig_directive, brig_code;
static struct hsa_brig_section brig_operand, brig_debug;
static uint32_t brig_insn_count;

static bool brig_initialized = false;

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
hsa_brig_section::init ()
{
  total_size = sizeof (struct BrigSectionHeader);
  chunks.create (1);
  allocate_new_chunk ();
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

/* Write the section to the output file to a section called SECTION_NAME.
   Switches the output section and does not restore it. */

void
hsa_brig_section::output (const char *section_name)
{
  struct BrigSectionHeader section_header;

  switch_to_section (get_section (section_name, SECTION_NOTYPE, NULL));

  section_header.size = htole32 (total_size);
  assemble_string ((const char *)&section_header, sizeof (section_header));

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

/* Add padding to section so that its size is divisble by FACTOR.  */

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

/* BRIG String hashing.  */

struct brig_string_slot
{
  const char *s;
  char prefix;
  int len;
  uint32_t offset;
};

/* Hashtable helpers.  */

struct brig_string_slot_hasher
{
  typedef brig_string_slot value_type;
  typedef brig_string_slot compare_type;
  static inline hashval_t hash (const value_type *);
  static inline bool equal (const value_type *, const compare_type *);
  static inline void remove (value_type *);
};

/* Returns a hash code for DS.  Adapted from libiberty's htab_hash_string
   to support strings that may not end in '\0'.  */

inline hashval_t
brig_string_slot_hasher::hash (const value_type *ds)
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
brig_string_slot_hasher::equal (const value_type *ds1, const compare_type *ds2)
{
  if (ds1->len == ds2->len)
    return ds1->prefix == ds2->prefix && memcmp (ds1->s, ds2->s, ds1->len) == 0;

  return 0;
}

inline void
brig_string_slot_hasher::remove (value_type *ds)
{
  free (const_cast<char*> (ds->s));
}

static hash_table<brig_string_slot_hasher> brig_string_htab;

static void
sanitize_hsa_name (char *p)
{
  for (; *p; p++)
    if (*p == '.')
      *p = '_';
}

/* Emit a null terminated string STR to the string section and return its
   offset in it.  */

static unsigned
brig_emit_string (const char *str, char prefix = 0)
{
  unsigned slen = strlen (str);
  unsigned offset, len = slen + (prefix ? 1 : 0);
  uint32_t hdr_len = htole32 (len);
  brig_string_slot s_slot;
  brig_string_slot **slot;
  char *str2;

  /* XXX Sanitize the names without all the strdup.  */
  str2 = xstrdup (str);
  sanitize_hsa_name (str2);
  s_slot.s = str2;
  s_slot.len = slen;
  s_slot.prefix = prefix;
  s_slot.offset = 0;

  slot = brig_string_htab.find_slot (&s_slot, INSERT);
  if (*slot == NULL)
    {
      brig_string_slot *new_slot = XCNEW (brig_string_slot);

      /* In theory we should fill in struct BrigString and stream that but that
	 would mean copying the string to a buffer for no good reason, so we
	 just emaulate it.   */
      offset = brig_string.add (&hdr_len, sizeof (hdr_len));
      if (prefix)
	brig_string.add (&prefix, 1);
      brig_string.add (str2, slen);
      brig_string.round_size_up (4);

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

/* Offsets of corresponding register names so that we don't have to generate
   them repeatedly.  */
static unsigned c_reg_names[HSA_C_REGISTER_COUNT];
static unsigned s_reg_names[HSA_S_REGISTER_COUNT];
static unsigned d_reg_names[HSA_D_REGISTER_COUNT];
static unsigned q_reg_names[HSA_Q_REGISTER_COUNT];
/* Offsets of operands describing registers so that we don't generate them
   repeatadly.  */
static unsigned c_regs[HSA_C_REGISTER_COUNT];
static unsigned s_regs[HSA_S_REGISTER_COUNT];
static unsigned d_regs[HSA_D_REGISTER_COUNT];
static unsigned q_regs[HSA_Q_REGISTER_COUNT];


/* unless already initialized, Initialzie infrastructure to produce BRIG.  */

static void
brig_init (void)
{
  struct BrigDirectiveVersion verdir;

  brig_insn_count = 0;

  if (brig_initialized)
    return;

  brig_string_htab.create (37);
  brig_string.init ();
  brig_directive.init ();
  brig_code.init ();
  brig_operand.init ();
  brig_debug.init ();

  verdir.size = htole16 (sizeof (verdir));
  verdir.kind = htole16 (BRIG_DIRECTIVE_VERSION);
  verdir.code = htole32 (brig_code.total_size);
  /*verdir.major = htole16 (BRIG_VERSION_MAJOR);
  verdir.minor = htole16 (BRIG_VERSION_MINOR);*/
  verdir.hsailMajor = htole32 (BRIG_VERSION_HSAIL_MAJOR);
  verdir.hsailMinor = htole32 (BRIG_VERSION_HSAIL_MINOR);
  verdir.brigMajor = htole32 (BRIG_VERSION_BRIG_MAJOR);
  verdir.brigMinor = htole32 (BRIG_VERSION_BRIG_MINOR);
  verdir.profile = hsa_full_profile_p () ? BRIG_PROFILE_FULL: BRIG_PROFILE_BASE;
  if (hsa_machine_large_p ())
    verdir.machineModel = BRIG_MACHINE_LARGE;
  else
    verdir.machineModel = BRIG_MACHINE_SMALL;
  verdir.reserved = 0;
  brig_directive.add (&verdir, sizeof (verdir));

  brig_initialized = true;
}

/* Free all BRIG data.  */

static void
brig_release_data (void)
{
  brig_string_htab.dispose ();
  brig_string.release ();
  brig_directive.release ();
  brig_code.release ();
  brig_operand.release ();
  brig_debug.release ();

  brig_initialized = 0;
}

/* Emit directive describing a symbol if it has not been emitted already.
   Return the offset of the directive.  */

static unsigned
emit_symbol_directive (struct hsa_symbol *symbol)
{
  struct BrigDirectiveSymbol symdir;
  unsigned name_offset;
  static unsigned res_name_offset;
  char prefix;

  if (symbol->offset)
    return symbol->offset;

  symdir.size = htole16 (sizeof (symdir));
  symdir.kind = htole16 (BRIG_DIRECTIVE_VARIABLE);
  symdir.code = htole32 (brig_code.total_size);

  if (symbol->decl && is_global_var (symbol->decl))
    {
      prefix = '&';
      if (TREE_CODE (symbol->decl) == VAR_DECL)
	warning (0, "referring to global symbol %q+D by name from HSA code won't work", symbol->decl);
    }
  else
    prefix = '%';

  if (symbol->decl && TREE_CODE (symbol->decl) == RESULT_DECL)
    {
      if (res_name_offset == 0)
	res_name_offset = brig_emit_string (symbol->name, '%');
      name_offset = res_name_offset;
    }
  else if (symbol->name)
    name_offset = brig_emit_string (symbol->name, prefix);
  else
    {
      char buf[64];
      sprintf (buf, "__%s_%i", hsa_seg_name (symbol->segment),
	       symbol->name_number);
      name_offset = brig_emit_string (buf, prefix);
    }

  symdir.name = htole32 (name_offset);
  symdir.init = 0;
  symdir.type = htole16 (symbol->type);
  symdir.segment = symbol->segment;
  /* FIXME: For now assume natural alignment.  */
  symdir.align = 0;
  symdir.dimLo = htole32 (symbol->dimLo);
  symdir.dimHi = htole32 (symbol->dimHi);
  symdir.modifier = 0;
  if (symbol->segment == BRIG_SEGMENT_KERNARG)
    symdir.modifier = BRIG_SYMBOL_DECLARATION;
  memset (&symdir.reserved, 0, sizeof (symdir.reserved));
  symbol->offset = brig_directive.add (&symdir, sizeof (symdir));
  return symbol->offset;
}

/* Emit directives describing the function, for example its input and output
   arguments, local variables etc.  */

static BrigDirectiveExecutable *
emit_function_directives (void)
{
  struct BrigDirectiveExecutable fndir;
  unsigned name_offset, inarg_off, scoped_off, next_toplev_off;
  int count = 0;
  BrigDirectiveExecutable *ptr_to_fndir;
  hsa_symbol *sym;

  name_offset = brig_emit_string (hsa_cfun.name, '&');
  inarg_off = brig_directive.total_size + sizeof(fndir)
    + (hsa_cfun.output_arg ? sizeof (struct BrigDirectiveSymbol) : 0);
  scoped_off = inarg_off
    + hsa_cfun.input_args_count * sizeof (struct BrigDirectiveSymbol);
  for (hash_table <hsa_noop_symbol_hasher>::iterator iter
	 = hsa_cfun.local_symbols.begin ();
       iter != hsa_cfun.local_symbols.end ();
       ++iter)
    if (TREE_CODE ((*iter).decl) == VAR_DECL)
      count++;
  count += hsa_cfun.spill_symbols.length();

  next_toplev_off = scoped_off + count * sizeof (struct BrigDirectiveSymbol);

  fndir.size = htole16 (sizeof (fndir));
  fndir.kind = htole16 (BRIG_DIRECTIVE_KERNEL);
  fndir.code = htole32 (brig_code.total_size);
  fndir.name = htole32 (name_offset);
  fndir.inArgCount = htole16 (hsa_cfun.input_args_count);
  fndir.outArgCount = htole16 (hsa_cfun.output_arg ? 1 : 0);
  fndir.firstInArg = htole32 (inarg_off);
  fndir.firstScopedDirective = htole32 (scoped_off);
  fndir.nextTopLevelDirective = htole32 (next_toplev_off);
  fndir.instCount = htole32 (0);
  fndir.modifier = 0;
  memset (&fndir.reserved, 0, sizeof (fndir.reserved));
  brig_directive.add (&fndir, sizeof (fndir));
  /* XXX terrible hack: we need to set instCount after we emit all
     insns, but we need to emit directive in order, and we emit directives
     during insn emitting.  So we need to emit the FUNCTION directive 
     early, then the insns, and then we need to set instCount, so remember
     a pointer to it, in some horrible way.  cur_chunk.data+size points
     directly to after fndir here.  */
  ptr_to_fndir
      = (BrigDirectiveExecutable *)(brig_directive.cur_chunk->data
				    + brig_directive.cur_chunk->size
				    - sizeof (fndir));

  if (hsa_cfun.output_arg)
    emit_symbol_directive (hsa_cfun.output_arg);
  for (int i = 0; i < hsa_cfun.input_args_count; i++)
    emit_symbol_directive (&hsa_cfun.input_args[i]);
  for (hash_table <hsa_noop_symbol_hasher>::iterator iter
	 = hsa_cfun.local_symbols.begin ();
       iter != hsa_cfun.local_symbols.end ();
       ++iter)
    emit_symbol_directive (&*iter);
  for (int i = 0; hsa_cfun.spill_symbols.iterate (i, &sym); i++)
    emit_symbol_directive (sym);
  return ptr_to_fndir;
}

/* Emit a label directive for the given HBB.  We assume it is about to start on
   the current offset in the code section.  */

static void
emit_bb_label_directive (hsa_bb *hbb)
{
  struct BrigDirectiveLabel lbldir;
  char buf[32];

  lbldir.size = htole16 (sizeof (lbldir));
  lbldir.kind = htole16 (BRIG_DIRECTIVE_LABEL);
  lbldir.code = htole32 (brig_code.total_size);
  sprintf (buf, "BB_%u_%i", DECL_UID (current_function_decl), hbb->index);
  lbldir.name = htole32 (brig_emit_string (buf, '@'));
  hbb->label_ref.directive_offset = brig_directive.add (&lbldir,
							sizeof (lbldir));
}

BrigType16_t
bittype_for_type (BrigType16_t t)
{
  switch (t)
    {
    case BRIG_TYPE_B1:
      return BRIG_TYPE_B1;

    case BRIG_TYPE_U8:
    case BRIG_TYPE_S8:
    case BRIG_TYPE_B8:
      return BRIG_TYPE_B8;

    case BRIG_TYPE_U16:
    case BRIG_TYPE_S16:
    case BRIG_TYPE_B16:
    case BRIG_TYPE_F16:
      return BRIG_TYPE_B16;

    case BRIG_TYPE_U32:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_B32:
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

/* Enqueue operation OP.  Return the offset at which it will be stored.  */

unsigned int
enqueue_op (hsa_op_base *op)
{
  unsigned ret;

  if (op->offset)
    return op->offset;

  if (hsa_op_reg *reg = dyn_cast <hsa_op_reg> (op))
    {
      unsigned *cache;
      if (reg->reg_class == 'c')
	cache = &c_regs[(int) reg->hard_num];
      else if (reg->reg_class == 's')
	cache = &s_regs[(int) reg->hard_num];
      else if (reg->reg_class == 'd')
	cache = &d_regs[(int) reg->hard_num];
      else if (reg->reg_class == 'q')
	cache = &q_regs[(int) reg->hard_num];
      else
	gcc_unreachable ();

      if (*cache)
	{
	  op->offset = *cache;
	  return *cache;
	}
      *cache = op_queue.projected_size;
    }

  ret = op_queue.projected_size;
  op->offset = op_queue.projected_size;

  if (!op_queue.first_op)
    op_queue.first_op = op;
  else
    op_queue.last_op->next = op;
  op_queue.last_op = op;

  if (hsa_op_immed *imm = dyn_cast <hsa_op_immed> (op))
    {
      unsigned len = 8;
      switch (imm->type)
	{
	case BRIG_TYPE_U8:
	case BRIG_TYPE_S8:
	  len += 1;
	  break;
	case BRIG_TYPE_U16:
	case BRIG_TYPE_S16:
	case BRIG_TYPE_F16:
	  len += 2;
	  break;
	case BRIG_TYPE_U32:
	case BRIG_TYPE_S32:
	case BRIG_TYPE_F32:
	case BRIG_TYPE_U8X4:
	case BRIG_TYPE_S8X4:
	case BRIG_TYPE_U16X2:
	case BRIG_TYPE_S16X2:
	case BRIG_TYPE_F16X2:
	  len += 4;
	  break;
	case BRIG_TYPE_U64:
	case BRIG_TYPE_S64:
	case BRIG_TYPE_F64:
	case BRIG_TYPE_U8X8:
	case BRIG_TYPE_S8X8:
	case BRIG_TYPE_U16X4:
	case BRIG_TYPE_S16X4:
	case BRIG_TYPE_F16X4:
	case BRIG_TYPE_U32X2:
	case BRIG_TYPE_S32X2:
	case BRIG_TYPE_F32X2:
	  len += 8;
	  break;
	default:
	  gcc_unreachable ();
	}
      op_queue.projected_size += ((len + 3) / 4) * 4;
    }
  else if (is_a <hsa_op_reg> (op))
    op_queue.projected_size += sizeof (struct BrigOperandReg);
  else if (is_a <hsa_op_address> (op))
    op_queue.projected_size += sizeof (struct BrigOperandAddress);
  else if (is_a <hsa_op_label> (op))
    op_queue.projected_size += sizeof (struct BrigOperandRef);
  else
    gcc_unreachable ();
  return ret;
}

/* Emit an immediate BRIG operand IMM.  */
static void
emit_immediate_operand (hsa_op_immed *imm)
{
  struct BrigOperandImmed out;
  union
  {
    uint8_t b8;
    uint16_t b16;
    uint32_t b32;
    uint64_t b64;
  } bytes;
  unsigned len;

  switch (imm->type)
    {
      /* FIXME: Implement the unreachables below.  And also the implemented
	 parts seem too hacky.  */

    case BRIG_TYPE_U8:
    case BRIG_TYPE_S8:
      len = 1;
      bytes.b8 = (uint8_t) TREE_INT_CST_LOW (imm->value);
      break;
    case BRIG_TYPE_U16:
    case BRIG_TYPE_S16:
      bytes.b16 = (uint16_t) TREE_INT_CST_LOW (imm->value);
      len = 2;
      break;

    case BRIG_TYPE_F16:
      gcc_unreachable ();

    case BRIG_TYPE_U32:
    case BRIG_TYPE_S32:
      bytes.b32 = (uint32_t) TREE_INT_CST_LOW (imm->value);
      len = 4;
      break;

    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
      bytes.b64 = (uint64_t) int_cst_value (imm->value);
      len = 8;
      break;

    case BRIG_TYPE_F32:
    case BRIG_TYPE_F64:
      {
	tree expr = imm->value;
	tree type = TREE_TYPE (expr);

	len = GET_MODE_SIZE (TYPE_MODE (type));

	/* There are always 32 bits in each long, no matter the size of
	   the hosts long.  */
	long tmp[6];

	gcc_assert (len == 4 || len == 8);

	real_to_target (tmp, TREE_REAL_CST_PTR (expr), TYPE_MODE (type));

	if (len == 4)
	  bytes.b32 = (uint32_t) tmp[0];
	else
	  {
	    bytes.b64 = (uint64_t)(uint32_t) tmp[1];
	    bytes.b64 <<= 32;
	    bytes.b64 |= (uint32_t) tmp[0];
	  }

	break;
      }

    case BRIG_TYPE_U8X4:
    case BRIG_TYPE_S8X4:
    case BRIG_TYPE_U16X2:
    case BRIG_TYPE_S16X2:
    case BRIG_TYPE_F16X2:
      gcc_unreachable ();

    case BRIG_TYPE_U8X8:
    case BRIG_TYPE_S8X8:
    case BRIG_TYPE_U16X4:
    case BRIG_TYPE_S16X4:
    case BRIG_TYPE_F16X4:
    case BRIG_TYPE_U32X2:
    case BRIG_TYPE_S32X2:
    case BRIG_TYPE_F32X2:
    default:
      gcc_unreachable ();
    }

  out.size = htole16 ((8 + len + 3) & ~3);
  out.kind = htole16 (BRIG_OPERAND_IMMED);
  out.type = htole16 (bittype_for_type (imm->type));
  out.byteCount = htole16 (len);
  brig_operand.add (&out, 8);
  brig_operand.add (&bytes, len);
  brig_operand.round_size_up (4);
}

/* Emit a register name into the strings section, unless already emitted.
   Return its offset in the string section.  */

static unsigned
emit_register_name  (hsa_op_reg *reg)
{
  unsigned *cache;
  unsigned offset;
  char buf[32];

  if (reg->reg_class == 'c')
    cache = &c_reg_names[(int) reg->hard_num];
  else if (reg->reg_class == 's')
    cache = &s_reg_names[(int) reg->hard_num];
  else if (reg->reg_class == 'd')
    cache = &d_reg_names[(int) reg->hard_num];
  else if (reg->reg_class == 'q')
    cache = &q_reg_names[(int) reg->hard_num];
  else
    gcc_unreachable ();

  if (*cache)
    return *cache;

  sprintf (buf, "%c%i", reg->reg_class, reg->hard_num);
  offset = brig_emit_string (buf, '$');
  *cache = offset;
  return offset;
}

/* Emit a register BRIG operand REG.  */

static void
emit_register_operand (hsa_op_reg *reg)
{
  struct BrigOperandReg out;

  out.size = htole16 (sizeof (out));
  out.kind = htole16 (BRIG_OPERAND_REG);
  out.reg = htole32 (emit_register_name (reg));
  /* FIXME, hack, real regalloc */
  out.type = htole16 (regtype_for_type (reg->type));
  out.reserved = 0;
  brig_operand.add (&out, sizeof (out));
}

/* Emit an address BRIG opernad ADDR.  */

static void
emit_address_operand (hsa_op_address *addr)
{
  struct BrigOperandAddress out;

  out.size = htole16 (sizeof (out));
  out.kind = htole16 (BRIG_OPERAND_ADDRESS);
  out.symbol = addr->symbol
    ? htole32 (emit_symbol_directive (addr->symbol)) : 0;
  out.reg = addr->reg ? htole32 (emit_register_name (addr->reg)) : 0;

  /* FIXME: This is very clumsy.  */
  if (sizeof (addr->imm_offset) == 8)
    {
      out.offsetLo = htole32 ((uint32_t)addr->imm_offset);
      out.offsetHi = htole32 (((long long) addr->imm_offset) >> 32);
    }
  else
    {
      out.offsetLo = htole32 (addr->imm_offset);
      out.offsetHi = 0;
    }

  BrigType16_t optype;
  if (addr->symbol)
    optype = hsa_get_segment_addr_type (addr->symbol->segment);
  else
    optype = hsa_get_segment_addr_type (BRIG_SEGMENT_FLAT);
  /* The standard (PRM 19.8.3 BrigOperandAddress) says these should be U types
     but the validator insists these are B types, so as a hackish workaround,
     we run the type through regtype_for_type.  */
  out.type = htole16 (regtype_for_type (optype));
  out.reserved = 0;
  brig_operand.add (&out, sizeof (out));
}

/* Emit an address BRIG opernad ADDR.  */

static void
emit_label_operand (hsa_op_label *lbl)
{
  struct BrigOperandRef out;

  gcc_assert (lbl->directive_offset);
  out.size = htole16 (sizeof (out));
  out.kind = htole16 (BRIG_OPERAND_LABEL_REF);
  out.ref = htole32 (lbl->directive_offset);
  brig_operand.add (&out, sizeof (out));
}

/* Emit all operands queued for writing.  */

static void
emit_queued_operands (void)
{
  for (hsa_op_base *op = op_queue.first_op; op; op = op->next)
    {
      gcc_assert (op->offset == brig_operand.total_size);
      if (hsa_op_immed *imm = dyn_cast <hsa_op_immed> (op))
	emit_immediate_operand (imm);
      else if (hsa_op_reg *reg = dyn_cast <hsa_op_reg> (op))
	emit_register_operand (reg);
      else if (hsa_op_address *addr = dyn_cast <hsa_op_address> (op))
	emit_address_operand (addr);
      else if (hsa_op_label *lbl = dyn_cast <hsa_op_label> (op))
	emit_label_operand (lbl);
      else
	gcc_unreachable ();
    }
}

/* Emit an HSA memory instruction and all necessary directives, schedule
   necessary operands for writing .  */

static void
emit_memory_insn (hsa_insn_mem *mem)
{
  struct BrigInstMem repr;
  hsa_op_address *addr = as_a <hsa_op_address> (mem->operands[1]);

  /* This is necessary because of the errorneous typedef of
     BrigMemoryModifier8_t which introduces padding which may then contain
     random stuff (which we do not want so that we can test things don't
     change).  */
  memset (&repr, 0, sizeof (repr));
  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_MEM);
  repr.opcode = htole16 (mem->opcode);
  repr.type = htole16 (mem->type);
  repr.operands[0] = htole32 (enqueue_op (mem->operands[0]));
  repr.operands[1] = htole32 (enqueue_op (mem->operands[1]));
  repr.operands[2] = 0;
  repr.operands[3] = 0;
  repr.operands[4] = 0;
  if (addr->symbol)
    repr.segment = addr->symbol->segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  repr.modifier = mem->semantic ? mem->semantic : BRIG_SEMANTIC_REGULAR;
  repr.equivClass = mem->equiv_class;
  if (mem->opcode == BRIG_OPCODE_LD)
    repr.width = BRIG_WIDTH_1;
  else
    repr.width = BRIG_WIDTH_NONE;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA memory instruction and all nececcary directives, schedule
   necessary operands for writing .  */

static void
emit_atomic_insn (hsa_insn_atomic *mem)
{
  struct BrigInstAtomic repr;
  hsa_op_address *addr = as_a <hsa_op_address> (mem->operands[1]);

  /* This is necessary because of the errorneous typedef of
     BrigMemoryModifier8_t which introduces padding which may then contain
     random stuff (which we do not want so that we can test things don't
     change).  */
  memset (&repr, 0, sizeof (repr));
  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_ATOMIC);
  repr.opcode = htole16 (mem->opcode);
  repr.type = htole16 (mem->type);
  repr.operands[0] = htole32 (enqueue_op (mem->operands[0]));
  repr.operands[1] = htole32 (enqueue_op (mem->operands[1]));
  repr.operands[2] = htole32 (enqueue_op (mem->operands[2]));
  if (mem->atomicop == BRIG_ATOMIC_CAS)
    repr.operands[3] = htole32 (enqueue_op (mem->operands[3]));
  if (addr->symbol)
    repr.segment = addr->symbol->segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  repr.memorySemantic = mem->semantic ? mem->semantic : BRIG_SEMANTIC_REGULAR;
  repr.atomicOperation = mem->atomicop;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA LDA instruction and all nececcary directives, schedule
   necessary operands for writing .  */

static void
emit_addr_insn (hsa_insn_addr *insn)
{
  struct BrigInstAddr repr;
  hsa_op_address *addr = as_a <hsa_op_address> (insn->operands[1]);

  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_ADDR);
  repr.opcode = htole16 (insn->opcode);
  repr.type = htole16 (insn->type);
  repr.operands[0] = htole32 (enqueue_op (insn->operands[0]));
  repr.operands[1] = htole32 (enqueue_op (insn->operands[1]));
  repr.operands[2] = 0;
  repr.operands[3] = 0;
  repr.operands[4] = 0;
  if (addr->symbol)
    repr.segment = addr->symbol->segment;
  else
    repr.segment = BRIG_SEGMENT_FLAT;
  memset (&repr.reserved, 0, sizeof (repr.reserved));
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA segment conversion instruction and all nececcary directives,
   schedule necessary operands for writing .  */

static void
emit_segment_insn (hsa_insn_seg *seg)
{
  struct BrigInstSeg repr;

  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_SEG);
  repr.opcode = htole16 (seg->opcode);
  repr.type = htole16 (seg->type);
  repr.operands[0] = htole32 (enqueue_op (seg->operands[0]));
  repr.operands[1] = htole32 (enqueue_op (seg->operands[1]));
  repr.operands[2] = 0;
  repr.operands[3] = 0;
  repr.operands[4] = 0;
  repr.sourceType = htole16 (seg->src_type);
  repr.segment = seg->segment;
  repr.reserved = 0;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA comparison instruction and all nececcary directives,
   schedule necessary operands for writing .  */

static void
emit_cmp_insn (hsa_insn_cmp *cmp)
{
  struct BrigInstCmp repr;

  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_CMP);
  repr.opcode = htole16 (cmp->opcode);
  repr.type = htole16 (cmp->type);
  repr.operands[0] = htole32 (enqueue_op (cmp->operands[0]));
  repr.operands[1] = htole32 (enqueue_op (cmp->operands[1]));
  repr.operands[2] = htole32 (enqueue_op (cmp->operands[2]));
  repr.operands[3] = 0;
  repr.operands[4] = 0;
  if (is_a <hsa_op_reg> (cmp->operands[1]))
    repr.sourceType = htole16 (as_a <hsa_op_reg> (cmp->operands[1])->type);
  else
    repr.sourceType = htole16 (as_a <hsa_op_immed> (cmp->operands[1])->type);
  repr.modifier = 0;
  repr.compare = cmp->compare;
  repr.pack = 0;
  repr.reserved = 0;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit an HSA branching instruction and all nececcary directives, schedule
   necessary operands for writing .  */

static void
emit_branch_insn (hsa_insn_br *br)
{
  struct BrigInstBr repr;
  basic_block target = NULL;
  edge_iterator ei;
  edge e;

  /* At the moment we only handle direct conditional jumps.  */
  gcc_assert (br->opcode == BRIG_OPCODE_CBR
	      && !br->operands[2]);
  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_BR);
  repr.opcode = htole16 (br->opcode);
  repr.width = BRIG_WIDTH_1;
  repr.type = htole16 (br->type);
  repr.operands[0] = htole32 (enqueue_op (br->operands[0]));

  FOR_EACH_EDGE (e, ei, br->bb->succs)
    if (e->flags & EDGE_TRUE_VALUE)
      {
	target = e->dest;
	break;
      }
  gcc_assert (target);
  repr.operands[1] = htole32 (enqueue_op (&hsa_bb_for_bb (target)->label_ref));
  repr.operands[2] = 0;
  repr.operands[3] = 0;
  repr.operands[4] = 0;

  repr.modifier = 0;
  repr.width = BRIG_WIDTH_1;
  repr.reserved = 0;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

static bool
float_type_p (BrigType16_t t)
{
  switch (t & BRIG_TYPE_BASE_MASK)
    {
    case BRIG_TYPE_F16:
    case BRIG_TYPE_F32:
    case BRIG_TYPE_F64:
      return true;
    default:
      return false;
    }
}

/* Emit a HSA convert instruction and all nececcary directives, schedule
   necessary operands for writing.  */
static void
emit_cvt_insn (hsa_insn_basic *insn)
{
  struct BrigInstCvt repr;
  BrigType16_t srctype;

  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_CVT);
  repr.opcode = htole16 (insn->opcode);
  repr.type = htole16 (insn->type);
  for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
    if (insn->operands[i])
      repr.operands[i] = htole32 (enqueue_op (insn->operands[i]));
    else
      repr.operands[i] = 0;
  if (is_a <hsa_op_reg> (insn->operands[1]))
    srctype = as_a <hsa_op_reg> (insn->operands[1])->type;
  else
    srctype = as_a <hsa_op_immed> (insn->operands[1])->type;
  repr.sourceType = htole16 (srctype);

  /* float to smaller float requires a rounding setting (we default
     to 'near'.  */
  if (float_type_p (insn->type) && float_type_p (srctype)
      && (insn->type & BRIG_TYPE_BASE_MASK) < (srctype & BRIG_TYPE_BASE_MASK))
    repr.modifier = BRIG_ROUND_FLOAT_NEAR_EVEN;
  else
    repr.modifier = 0;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
}

/* Emit a basic HSA instruction and all nececcary directives, schedule
   necessary operands for writing .  */

static void
emit_basic_insn (hsa_insn_basic *insn)
{
  /* We assume that BrigInstMod has a BrigInstBasic prefix.  */
  struct BrigInstMod repr;
  BrigType16_t type;

  if (insn->opcode == BRIG_OPCODE_CVT)
    {
      emit_cvt_insn (insn);
      return;
    }

  repr.size = htole16 (sizeof (BrigInstBasic));
  repr.kind = htole16 (BRIG_INST_BASIC);
  repr.opcode = htole16 (insn->opcode);
  switch (insn->opcode)
    {
      /* XXX The spec says mov can take all types.  But the LLVM based
	 simulator cries about "Mov_s32" not being defined.  */
      case BRIG_OPCODE_MOV:
      /* And the bit-logical operations need bit types and whine about
         arithmetic types :-/  */
      case BRIG_OPCODE_AND:
      case BRIG_OPCODE_OR:
      case BRIG_OPCODE_XOR:
      case BRIG_OPCODE_NOT:
	type = regtype_for_type (insn->type);
	break;
      default:
	type = insn->type;
	break;
    }
  repr.type = htole16 (type);
  for (int i = 0; i < HSA_OPERANDS_PER_INSN; i++)
    if (insn->operands[i])
      repr.operands[i] = htole32 (enqueue_op (insn->operands[i]));
    else
      repr.operands[i] = 0;
  if ((type & BRIG_TYPE_PACK_MASK) != BRIG_TYPE_PACK_NONE)
    {
      if (float_type_p (type))
	repr.modifier = BRIG_ROUND_FLOAT_NEAR_EVEN;
      else
	repr.modifier = 0;
      /* We assume that destination and sources agree in packing
         layout.  */
      if (insn->operands[2])
	repr.pack = BRIG_PACK_PP;
      else
	repr.pack = BRIG_PACK_P;
      repr.reserved = 0;
      repr.size = htole16 (sizeof (BrigInstMod));
      repr.kind = htole16 (BRIG_INST_MOD);
      brig_code.add (&repr, sizeof (struct BrigInstMod));
    }
  else
    brig_code.add (&repr, sizeof (struct BrigInstBasic));
  brig_insn_count++;
}

/* Emit an HSA instruction and all nececcary directives, schedule necessary
   operands for writing .  */

static void
emit_insn (hsa_insn_basic *insn)
{
  gcc_assert (!is_a <hsa_insn_phi> (insn));
  if (hsa_insn_atomic *atom = dyn_cast <hsa_insn_atomic> (insn))
    {
      emit_atomic_insn (atom);
      return;
    }
  if (hsa_insn_mem *mem = dyn_cast <hsa_insn_mem> (insn))
    {
      emit_memory_insn (mem);
      return;
    }
  if (hsa_insn_addr *addr = dyn_cast <hsa_insn_addr> (insn))
    {
      emit_addr_insn (addr);
      return;
    }
  if (hsa_insn_seg *seg = dyn_cast <hsa_insn_seg> (insn))
    {
      emit_segment_insn (seg);
      return;
    }
  if (hsa_insn_cmp *cmp = dyn_cast <hsa_insn_cmp> (insn))
    {
      emit_cmp_insn (cmp);
      return;
    }
  if (hsa_insn_br *br = dyn_cast <hsa_insn_br> (insn))
    {
      emit_branch_insn (br);
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
  struct BrigInstBr repr;
  edge_iterator ei;
  edge e;

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
  gcc_assert (ff);
  if (ff == next_bb
      || ff == EXIT_BLOCK_PTR_FOR_FN (cfun))
    return;

  repr.size = htole16 (sizeof (repr));
  repr.kind = htole16 (BRIG_INST_BR);
  repr.opcode = htole16 (BRIG_OPCODE_BRN);
  repr.type = htole16 (BRIG_TYPE_NONE);
  /* Direct branches to labels must be width(all).  */
  repr.width = BRIG_WIDTH_ALL;
  repr.operands[0] = htole32 (enqueue_op (&hsa_bb_for_bb (ff)->label_ref));
  repr.operands[1] = 0;
  repr.operands[2] = 0;
  repr.operands[3] = 0;
  repr.operands[4] = 0;

  repr.modifier = 0;
  repr.width = BRIG_WIDTH_ALL;
  repr.reserved = 0;
  brig_code.add (&repr, sizeof (repr));
  brig_insn_count++;
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

  ptr_to_fndir = emit_function_directives ();
  for (insn = hsa_bb_for_bb (ENTRY_BLOCK_PTR_FOR_FN (cfun))->first_insn;
       insn;
       insn = insn->next)
    emit_insn (insn);
  prev_bb = ENTRY_BLOCK_PTR_FOR_FN (cfun);
  FOR_EACH_BB_FN (bb, cfun)
    {
      perhaps_emit_branch (prev_bb, bb);
      emit_bb_label_directive (hsa_bb_for_bb (bb));
      for (insn = hsa_bb_for_bb (bb)->first_insn; insn; insn = insn->next)
	emit_insn (insn);
      prev_bb = bb;
    }
  perhaps_emit_branch (prev_bb, NULL);
  ptr_to_fndir->instCount = brig_insn_count;
  ptr_to_fndir->nextTopLevelDirective = brig_directive.total_size;

  emit_queued_operands ();
}

/* Write BRIG sections to the output file.  */

void
hsa_output_brig (void)
{
  section *saved_section;

  if (!brig_initialized)
    return;

  saved_section = in_section;
  brig_string.output (BRIG_SECTION_STRING_NAME);
  brig_directive.output (BRIG_SECTION_DIRECTIVE_NAME);
  brig_code.output (BRIG_SECTION_CODE_NAME);
  brig_operand.output (BRIG_SECTION_OPERAND_NAME);
  brig_debug.output (BRIG_SECTION_DEBUG_NAME);
  if (saved_section)
    switch_to_section (saved_section);

  brig_release_data ();
  hsa_deinit_compilation_unit_data ();
}
