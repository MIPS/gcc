/* Top-level LTO routines.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "opts.h"
#include "toplev.h"
#include "dwarf2.h"
#include "tree.h"
#include "tm.h"
#include "cgraph.h"
#include "ggc.h"
#include "lto.h"

/* References 

   DWARF Debugging Information Format (Version 3)
     http://dwarf.freestandards.org/Dwarf3.pdf

   */  
   
/* Types */

/* A DWARF2 attribute.  */ 
typedef struct DWARF2_attr
{
  /* The name of the attribute, e.g., DW_AT_name.  */
  uint64_t name;
  /* The form of the attribute, e.g., DW_FORM_data2.  */
  uint64_t form;
} DWARF2_attr;

/* A DWARF2 abbreviation-table entry.  */
struct DWARF2_abbrev
{
  /* The tag for a DIE using this abbreviation, e.g.,
     DW_TAG_pointer_type.  */
  uint64_t tag;
  /* True if the next DIE is a child of this one; false, if the next
     DIE is a sibling of this one.  */
  bool has_children;
  /* The number of attributes in the ATTRIBUTES array.  */
  size_t num_attrs;
  /* A dynamically-sized array of attributes.  */
  DWARF2_attr attrs[1];
};

/* The classes of DWARF2 attribute data.  */
typedef enum DWARF2_class
{
  DW_cl_error = 0x0,
  DW_cl_address = 0x1,
  DW_cl_block = 0x2,
  DW_cl_uconstant = 0x4,
  DW_cl_sconstant = 0x8,
  DW_cl_flag = 0x10,
  DW_cl_lineptr = 0x20,
  DW_cl_loclistptr = 0x40,
  DW_cl_macptr = 0x80,
  DW_cl_rangelistptr = 0x100,
  DW_cl_reference = 0x200,
  DW_cl_string = 0x400
} DWARF2_class;

/* Some places we want to specify that both unsigned and signed constant
   forms are allowed, so define a shorthand.  */
#define DW_cl_constant (DW_cl_uconstant | DW_cl_sconstant)

/* A pointer to a DWARF2 DIE, as mapped into memory.  

   The "lto_die" type is intentionally never defined.  This typedef
   exists purely for type safety.  */
typedef struct lto_die *lto_die_ptr;

/* The data corresponding to a single attribute form in a DIE.  */
typedef struct DWARF2_form_data
{
  /* The class of data.  */
  DWARF2_class cl;
  union {
    /* If CL is DW_address, DW_lineptr, DW_loclistptr, DW_macptr, 
       or DW_rangelistptr, there is no additional data.  These forms
       are not of interest to us for link-time optimization.  */
    /* Used when CL is DW_cl_uconstant.  */
    uint64_t uconstant;
    /* Used when CL is DW_cl_sconstant.  */
    int64_t sconstant;
    /* Used when CL is DW_cl_block.  */
    struct DWARF2_form_data_block {
      /* The number of bytes in the block.  */
      size_t length;
      /* The data in the block.  */
      const void *data;
    } block;
    /* Used when CL is DW_cl_flag.  */
    bool flag;
    /* Used when CL is DW_cl_string.  The value is a NUL-terminated
       string.  */
    const char *string;
    /* Used when CL is DW_cl_reference.  The value is a pointer into
       the .debug_info section indicating the DIE referenced.  */
    lto_die_ptr reference;
  } u;
} DWARF2_form_data;

/* Information passed from parent DIEs to child DIEs to give context
   about the current location in the scope tree.  */
struct lto_context
{
  /* The start of the current compilation unit info.  This is right
     *after* the header, at the beginning of the DIE entries, so if
     you want the start of the header, you need to subtract the
     header length.  */
  const char *cu_start;
  /* The byte just past the end of the current compilation unit.  */
  const char *cu_end;
  /* Pointer to the CompUnit structure currently being used.  */
  const DWARF2_CompUnit *cu;
  /* The scope (a NAMESPACE_DECL, FUNCTION_DECL, RECORD_TYPE, etc.)
     containing the DIE about to be read.  NULL_TREE if at the
     outermost level.  */
  tree scope;
  /* Some things (like lower array bounds) have language-specific defaults,
     so keep track of the language here.  */
  int language;
  /* Some DIEs (like enumerated types) need to pass down some information
     to their children as they are parsed.  */
  tree parentdata;
};

/* We can't use DWARF_Internal_CompUnit because it does not track the
   offset from the beginning of debug_info, which is necessary for
   lookups of DW_FORM_ref_addr data.  */
   
struct DWARF2_CompUnit
{
  /* Offset from start of .debug_info for this CU.  */
  uint64_t cu_start_offset;  
  /* Length of the CU header.  */
  uint64_t cu_header_length;
  /* Length of CU *including* the length field.  */
  uint64_t cu_length;
  /* DWARF version number of this CU.  */
  unsigned short cu_version;
  /* Offset of abbrevs for this CU from the start of debug_abbrev.  */
  uint64_t cu_abbrev_offset;
  /* Pointer size of this CU.  */
  unsigned char cu_pointer_size;
};

/* Variables */

/* Cookie for DWARF DW_TAG_unspecified_parameters.  All we need is a uniquely
   identifiable object; it doesn't need to carry any information.  */
tree lto_varargs_cookie;

/* Forward Declarations */

static hashval_t
lto_cache_hash (const void *data);

static int
lto_cache_eq (const void *data, const void *key);

static tree
lto_read_DIE (lto_info_fd *fd,
	      lto_context *context,
	      bool *more);

static void
lto_read_child_DIEs (lto_info_fd *fd, 
		     const DWARF2_abbrev *abbrev, 
		     lto_context *context);

static VEC(tree,heap) *
lto_collect_child_DIEs (lto_info_fd *fd, 
			const DWARF2_abbrev *abbrev, 
			lto_context *context);

static void
lto_set_cu_context (lto_context *context, lto_info_fd *fd,
		    DWARF2_CompUnit *unit);
/* Functions */ 

/* Initialize FD, a newly allocated "file descriptor".  NAME indicates
   the name of the section from which FD is reading.  FILE is the
   file containing that section.  */
static void
lto_fd_init (lto_fd *fd, const char *name, lto_file *file)
{
  fd->name = name;
  fd->start = NULL;
  fd->end = NULL;
  fd->cur = NULL;
  fd->file = file;
  fd->dwarf64 = false;
}

/* Initialize FD, a newly allocated file descriptor for a DWARF2
   info section.  NAME and FILE are as for lto_fd_init.  */
static void
lto_info_fd_init (lto_info_fd *fd, const char *name, lto_file *file)
{
  lto_fd_init ((lto_fd *) fd, name, file);
  fd->num_units = 0;
  fd->units = NULL;
  fd->die_cache = htab_create (37,
			       lto_cache_hash,
			       lto_cache_eq,
			       free);
}

/* Initialize FD, a newly allocated file descriptor for a DWARF2
   abbreviation section.  NAME and FILE are as for lto_fd_init.  */
static void
lto_abbrev_fd_init (lto_abbrev_fd *fd, const char *name, lto_file *file)
{
  lto_fd_init ((lto_fd *) fd, name, file);
  fd->num_abbrevs = 0;
  fd->abbrevs = NULL;
}

/* Close FD.  */
static void
lto_info_fd_close (lto_info_fd *fd)
{
  size_t i;

  htab_delete (fd->die_cache);
  for (i = 0; i < fd->num_units; ++i)
    XDELETE (fd->units[i]);
  XDELETEVEC (fd->units);
}

/* Close FD.  */
static void
lto_abbrev_fd_close (lto_abbrev_fd *fd)
{
  size_t i;

  for (i = 0; i < fd->num_abbrevs; ++i)
    XDELETE (fd->abbrevs[i]);
  XDELETEVEC (fd->abbrevs);
}

/* Initialize FILE, an LTO file object for FILENAME.  */
void
lto_file_init (lto_file *file, 
	       const lto_file_vtable *vtable,
	       const char *filename)
{
  file->vtable = vtable;
  file->filename = filename;
  lto_info_fd_init (&file->debug_info, ".debug_info", file);
  lto_abbrev_fd_init (&file->debug_abbrev, ".debug_abbrev", file);
}

/* Close FILE.  */
void
lto_file_close (lto_file *file)
{
  lto_info_fd_close (&file->debug_info);
  lto_abbrev_fd_close (&file->debug_abbrev);
  free (file);
}

/* Issue an error indicating that the section from which FD is reading
   is corrupt, i.e., fails to conform to the DWARF specification.  */
static void ATTRIBUTE_NORETURN
lto_file_corrupt_error (const lto_fd *fd)
{
  fatal_error ("corrupt link-time optimization information in "
	       "%qs section", fd->name);
}

/* Issue an error indicating that the ABI used to compile the object
   file does not match that currently in use by the LTO front end.  */
static void ATTRIBUTE_NORETURN
lto_abi_mismatch_error (void)
{
  fatal_error ("ABI for object file does not match current "
	       "compilation options");
}

/* Define a function:

     static C_TYPE 
     lto_check_C_TYPE_val (uint64_t val, const char *message);

   VAL is being used in a way that requires that it fit into a host
   C_TYPE.  If the value fits, it is returned unchanged (but cast to
   C_TYPE).  If it doesn't fit, the DWARF information is still valid,
   but we are unable to process it; in that case, a fatal error is
   issued, using the error MESSAGE.  */
#define LTO_CHECK_INT_VAL(C_TYPE)				\
  static C_TYPE							\
  lto_check_##C_TYPE##_val (uint64_t val, const char *message)	\
  {								\
    C_TYPE result;						\
								\
    result = val;						\
    if ((uint64_t) result != val)				\
      fatal_error ("%s", message);				\
    return result;						\
  }

/* Define functions to check conversions from DWARF constant values to
   C datatypes.  */
LTO_CHECK_INT_VAL(size_t);
LTO_CHECK_INT_VAL(int);
LTO_CHECK_INT_VAL(HOST_WIDE_INT);

#undef LTO_CHECK_INT_VAL

/* Define a function:

     static C_TYPE lto_read_DWARF_TYPE (lto_fd *fd);

   Here, DWARF_TYPE is a defined name (like "ubyte") from the DWARF
   specification.  C_TYPE is the equivalent C_TYPE.  This function is
   responsible for performing any byte-swapping required when reading
   the raw data from memory.  

   ??? However, that byte-swapping is not actually performed as of
   yet.  */
#define LTO_READ_TYPE(DWARF_TYPE, C_TYPE)	\
  static C_TYPE					\
  lto_read_##DWARF_TYPE (lto_fd *fd)		\
  {						\
    C_TYPE result;				\
						\
    if (fd->cur + sizeof (C_TYPE) > fd->end)	\
      lto_file_corrupt_error (fd);		\
    memcpy (&result, fd->cur, sizeof (C_TYPE));	\
    fd->cur += sizeof (C_TYPE);			\
						\
    return result;				\
  }

/* Define functions to read the DWARF primitive types.  */
LTO_READ_TYPE (ubyte, uint8_t);
LTO_READ_TYPE (uhalf, uint16_t);
LTO_READ_TYPE (uword, uint32_t);
/* DWARF3 does not specify "udword" by name, but does use 64-bit
   unsigned quantities in some places.  */
LTO_READ_TYPE (udword, uint64_t);

#undef LTO_READ_TYPE

/* Read an unsigned LEB128 value from FD.  */
static uint64_t
lto_read_uleb128 (lto_fd *fd)
{
  uint8_t byte;
  uint64_t result;
  bool more;
  unsigned shift;

  result = 0;
  shift = 0;
  do
    {
      byte = lto_read_ubyte (fd);
      more = byte & 0x80;
      result |= (byte & ~0x80) << shift;
      shift += 7;
    }
  while (more);

  return result;
}

/* Read an signed LEB128 value from FD.  */
static int64_t
lto_read_sleb128 (lto_fd *fd)
{
  uint8_t byte;
  uint64_t result;
  int64_t sresult;
  bool more;
  uint8_t sign;
  unsigned shift;

  result = 0;
  shift = 0;
  do
    {
      byte = lto_read_ubyte (fd);
      more = byte & 0x80;
      result |= (byte & ~0x80) << shift;
      shift += 7;
      sign = (byte & 0x40);
    }
  while (more);

  if (sign && shift < 64)
    sresult = (int64_t) result | ((int64_t)(-1) << shift);
  else
    sresult = (int64_t) result;
  return sresult;
}

/* Read an initial length field from FD.  The length may be a 32-bit
   or 64-bit quantity, depending on whether 32-bit or 64-bit DWARF is
   in use, so the value returned is always a 64-bit value.  Set
   FIELD_SIZE to the size (in bytes) of the initial length field.  */
static uint64_t
lto_read_initial_length (lto_fd *fd, unsigned int *field_size)
{
  /* The length of the debugging information section, as indicated in
     the DWARF information itself.  We use a 64-bit value so that we
     can support 64-bit DWARF in future.  */
  uint64_t length;

  /* Assume that this will be a 32-bit DWARF section.  */
  fd->dwarf64 = false;
  length = lto_read_uword (fd);
  if (length == 0xffffffff) 
    {
      *field_size = sizeof (uint32_t) + sizeof (uint64_t);
      /* 64-bit DWARF.  */
      fd->dwarf64 = true;
      /* The "dwarf2.h" header used by GCC does not declare 64-bit
	 DWARF objects, so we cannot actually use 64-bit DWARF.  */
      fatal_error ("64-bit DWARF not supported by link-time optimization");
    }
  else if (length >= 0xffffff00)
    /* An extension to DWARF that we do not support.  */
    fatal_error ("link-time optimization does not support DWARF extension "
		 HOST_WIDEST_INT_PRINT_UNSIGNED, length);
  else
    *field_size = sizeof (uint32_t);
  
  return length;
}

/* Read the attributes for an abbreviation entry from ABBREV_FD.  If
   ATTRS is non-NULL, fill it in with the attributes read; otherwise,
   the attributes are discarded.  In either case, return the number of
   attributes read.  */
static size_t
lto_abbrev_read_attrs (lto_abbrev_fd *abbrev_fd, DWARF2_attr *attrs)
{
  size_t num_attrs;
  lto_fd *fd;

  num_attrs = 0;
  fd = (lto_fd *)abbrev_fd;

  /* Read all the attributes.  */
  while (true)
    {
      uint64_t name;
      uint64_t form;
      name = lto_read_uleb128 (fd);
      form = lto_read_uleb128 (fd);
      if (!name && !form)
	break;
      /* If requested, fill in the attribute array.  */
      if (attrs)
	{
	  attrs->name = name;
	  attrs->form = form;
	  ++attrs;
	}
      ++num_attrs;
    }

  return num_attrs;
}

/* Read all the DWARF2 abbreviations from ABBREV_FD, placing them in
   ABBREV_FD->ABBREVS.  */
static void
lto_abbrev_read (lto_abbrev_fd *abbrev_fd)
{
  size_t num_abbrevs = 0;
  unsigned pass;
  lto_fd *fd;
  
  /* We should only read the abbreviations once.  */
  gcc_assert (!abbrev_fd->abbrevs);
  fd = (lto_fd *)abbrev_fd;
  /* Make two passes over the data.  On the first pass, count the
     number of abbreviations; on the second, read them in.  */
  for (pass = 0; pass < 2; ++pass)
    {
      /* On the second pass, allocate the array of abbreviations.  */
      if (pass == 1)
	{
	  abbrev_fd->num_abbrevs = num_abbrevs;
	  abbrev_fd->abbrevs = XCNEWVEC (DWARF2_abbrev *, num_abbrevs);
	}
      num_abbrevs = 0;
      /* Read the abbreviations, starting from the beginning of the
	 abbreviations section.  */
      fd->cur = fd->start;
      while (true)
	{
	  uint64_t index;
	  uint64_t tag;
	  uint8_t children;
	  size_t num_attrs;
	  DWARF2_abbrev *abbrev;
	  const char *cur;

	  /* Read the abbreviation index.  */
	  index = lto_read_uleb128 (fd);
	  if (!index)
	    /* Index zero indicates the end of the abbreviation codes.  */
	    break;
	  if (index > num_abbrevs)
	    {
	      lto_check_size_t_val (index,
				    "too many DWARF2 abbreviation entries");
	      num_abbrevs = (size_t) index;
	    }
	  /* The index is followed by the tag for the DIE.  */
	  tag = lto_read_uleb128 (fd);
	  children = lto_read_ubyte (fd);
	  if (children != DW_CHILDREN_yes && children != DW_CHILDREN_no)
	    lto_file_corrupt_error (fd);
	  /* Count the attributes so that we know how much space is
	     required for ABBREV.  Remember where we are, so that we can
	     return to our present location to read the attributes, after
	     ABBREV has been allocated.  */
	  cur = fd->cur;
	  num_attrs = lto_abbrev_read_attrs (abbrev_fd, NULL);
	  if (abbrev_fd->abbrevs)
	    {
	      /* Allocate the abbreviation entry.  */
	      abbrev = ((DWARF2_abbrev *) 
			xmalloc (sizeof (DWARF2_abbrev)
				 + (num_attrs - 1) * sizeof (DWARF2_attr)));
	      abbrev->tag = tag;
	      abbrev->has_children = (children == DW_CHILDREN_yes);
	      abbrev->num_attrs = num_attrs;
	      /* Read the attributes again.  */
	      fd->cur = cur;
	      lto_abbrev_read_attrs (abbrev_fd, abbrev->attrs);
	      abbrev_fd->abbrevs[index - 1] = abbrev;
	    }
	}
    }
}

/* Return the abbreviation entry with index ABBREV.  */
static const DWARF2_abbrev *
lto_abbrev_lookup (lto_abbrev_fd *abbrev_fd, uint64_t abbrev)
{
  const DWARF2_abbrev *result;

  /* An abbreviation of zero indicates the end of a list of sibling
     DIEs.  Therefore, callers should never call this function with
     ABBREV set to zero.  */
  gcc_assert (abbrev != 0);
  /* If the abbreviation is missing, that is an error.  */
  if (abbrev > abbrev_fd->num_abbrevs)
    lto_file_corrupt_error ((lto_fd *)abbrev_fd);
  result = abbrev_fd->abbrevs[abbrev - 1]; 
  if (!result)
    lto_file_corrupt_error ((lto_fd *)abbrev_fd);
  return result;
}

/* Read a section offset from FD.  In 32-bit DWARF, this is a 32-bit
   value; in 64-bit DWARF, it is a 64-bit value.  Therefore, the value
   returned is always a 64-bit value.  */
static uint64_t
lto_read_section_offset (lto_fd *fd)
{
  return fd->dwarf64 ? lto_read_udword (fd) : lto_read_uword (fd);
}

/* Read the compilation-unit header from FD, placing the resulting
   header in *CU.  */
static void
lto_read_comp_unit_header (lto_info_fd *fd, 
			   DWARF2_CompUnit *cu)
{
  unsigned int length_field_size;
  lto_fd *basefd = (lto_fd *)fd;

  /* The cu length is the length of the compilation unit, *not*
     including the length field.  Since the length field can be 4 or
     12 bytes, and *our* cu_length is specified to *include* the
     length field, we have to account for this.  */
  cu->cu_length = lto_read_initial_length (basefd, &length_field_size);
  cu->cu_length += length_field_size;
  cu->cu_version = lto_read_uhalf (basefd);
  cu->cu_abbrev_offset = lto_read_section_offset (basefd);
  cu->cu_pointer_size = lto_read_ubyte (basefd);
  if (cu->cu_pointer_size * BITS_PER_UNIT != POINTER_SIZE)
    lto_abi_mismatch_error ();
}

/* Find the compilation unit in FD that contains the DWARF2 DIE
   at OFFSET from the beginning of .debug_info.  */
static DWARF2_CompUnit *
find_cu_for_offset (const lto_info_fd *fd,
		    uint64_t offset)
{
  unsigned int len = fd->num_units;
  unsigned int half, middle;
  unsigned int first = 0;

  while (len > 0)
    {
      DWARF2_CompUnit *middle_elem;

      half = len >> 1;
      middle = first;
      middle += half;
      middle_elem = fd->units[middle];
      if (middle_elem->cu_start_offset < offset)
	{
	  first = middle;
	  ++first;
	  len = len - half - 1;
	}
      else
	len = half;
    }

  /* The above calculation will give us the first place *after* the cu
     we want.  So just subtract 1.  */
  
  if (first <= 0 || first > fd->num_units)
    lto_file_corrupt_error (&fd->base);
  
  return fd->units[first - 1];
}

/* Resolve a reference to the DIE at offset OFFSET.  Returns a pointer
   to the DIE, as mapped into memory.  Sets *NEW_CONTEXT to CONTEXT,
   or to a newly allocated context corresponding to the DIE referred
   to by OFFSET.  If *NEW_CONTEXT != CONTEXT upon return, the caller
   must free *NEW_CONTEXT when it is no longer needed.  */
static lto_die_ptr
lto_resolve_reference (lto_info_fd *info_fd,
		       uint64_t offset,
		       const lto_context *context,
		       lto_context **new_context)
{
  DWARF2_CompUnit *cu;
  lto_die_ptr reference;

  /* Swap context if necessary.  */
  cu = find_cu_for_offset (info_fd, offset);
  if (cu != context->cu)
    {
      *new_context = XCNEW (lto_context);
      **new_context = *context;
      lto_set_cu_context (*new_context, info_fd, cu);
      context = *new_context;
    }
  /* Resolve reference.  */
  reference = (lto_die_ptr) ((context->cu_start
			      + lto_check_size_t_val (offset,
						      "offset too large")));
  if (reference >= (lto_die_ptr) context->cu_end)
    lto_file_corrupt_error ((lto_fd *)info_fd);

  return reference;
}

/* Read the value of the attribute ATTR from INFO_FD.  CONTEXT is as for
   the DIE readers.  Upon return, *OUT contains the data read,
   and FORM_CONTEXT is the context necessary to do something with the
   form.  */
static void
lto_read_form (lto_info_fd *info_fd, 
	       const DWARF2_attr *attr, 
	       lto_context *context,
	       lto_context **form_context,
	       DWARF2_form_data *out)
{
  lto_fd *fd = (lto_fd *)info_fd;
  /* The Nth element in this array specifies (as a bitmask) the
     permissible classes of data for the attribute with code N.  See
     Figure 20 in DWARF 3 \S 7.5.4.  */
  static const DWARF2_class attr_classes[DW_AT_recursive + 1] = {
    DW_cl_error,
    DW_cl_reference, /* sibling */
    DW_cl_block | DW_cl_loclistptr, /* location */
    DW_cl_string, /* name */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_constant, /* ordering */
    DW_cl_error, /* subscr_data */
    DW_cl_block | DW_cl_constant | DW_cl_reference, /* byte_size */
    DW_cl_constant, /* bit_offset */
    DW_cl_constant, /* bit_size */
    DW_cl_error, /* padding */
    DW_cl_error, /* element_list */
    DW_cl_lineptr, /* stmt_list */
    DW_cl_address, /* low_pc */
    DW_cl_address, /* high_pc */
    DW_cl_constant, /* language */
    DW_cl_error, /* member */
    DW_cl_error, /* discr */
    DW_cl_error, /* discr_value */
    DW_cl_error, /* visibility */
    DW_cl_error, /* import */
    DW_cl_error, /* string_length */
    DW_cl_error, /* common_reference */
    DW_cl_string, /* comp_dir */
    DW_cl_block | DW_cl_constant | DW_cl_string, /* const_value */
    DW_cl_error, /* containing_type */
    DW_cl_error, /* default_value */
    DW_cl_error, /* padding */
    DW_cl_error, /* inline */
    DW_cl_error, /* is_optional */
    DW_cl_block | DW_cl_constant | DW_cl_reference, /* lower_bound */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_string, /* producer */
    DW_cl_error, /* padding */
    DW_cl_flag, /* prototyped */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_error, /* return_addr */
    DW_cl_error, /* padding */
    DW_cl_error, /* start_scope */
    DW_cl_error, /* padding */
    DW_cl_error, /* stride_size */
    DW_cl_block | DW_cl_constant | DW_cl_reference, /* upper_bound */
    DW_cl_error, /* padding */
    DW_cl_error, /* abstract_origin */
    DW_cl_error, /* accessibility */
    DW_cl_error, /* address_class */
    DW_cl_error, /* artificial */
    DW_cl_error, /* base_types */
    DW_cl_error, /* calling_convention */
    DW_cl_block | DW_cl_constant | DW_cl_reference, /* count */
    DW_cl_block | DW_cl_constant, /* data_member_location */
    DW_cl_error, /* decl_column */
    DW_cl_constant, /* decl_file */
    DW_cl_constant, /* decl_line */
    DW_cl_flag, /* declaration */
    DW_cl_error, /* discr_list */
    DW_cl_constant, /* encoding */
    DW_cl_flag, /* external */
    DW_cl_block | DW_cl_loclistptr, /* frame_base */
    DW_cl_error, /* friend */
    DW_cl_error, /* identifier_case */
    DW_cl_error, /* macro_info */
    DW_cl_error, /* namelist_items */
    DW_cl_error, /* priority */
    DW_cl_error, /* segment */
    DW_cl_reference, /* specification */
    DW_cl_error, /* static_link */
    DW_cl_reference, /* type */
    DW_cl_error, /* use_location */
    DW_cl_error, /* variable_parameter */
    DW_cl_error, /* virtuality */
    DW_cl_error, /* vtable_elem_location */
    DW_cl_error, /* allocated */
    DW_cl_error, /* associated */
    DW_cl_error, /* data_location */
    DW_cl_error, /* stride */
    DW_cl_error, /* entry_pc */
    DW_cl_error, /* use_UTF8 */
    DW_cl_error, /* extension */
    DW_cl_error, /* ranges */
    DW_cl_error, /* trampoline */
    DW_cl_error, /* call_column */
    DW_cl_error, /* call_file */
    DW_cl_error, /* call_line */
    DW_cl_error, /* description */
    DW_cl_error, /* binary_scale */
    DW_cl_error, /* decimal_scale */
    DW_cl_error, /* small */
    DW_cl_error, /* decimal_sign */
    DW_cl_error, /* digit_count */
    DW_cl_error, /* picture_string */
    DW_cl_error, /* mutable */
    DW_cl_error, /* threads_scaled */
    DW_cl_error, /* explicit */
    DW_cl_error, /* object_pointer */
    DW_cl_error, /* endianity */
    DW_cl_error, /* elemental */
    DW_cl_error, /* pure */
    DW_cl_error /* recursive */
  };

  /* The name (DW_AT_...) of this attribute.  */
  uint64_t name;
  /* The form (DW_FORM_...) of this attribute.  */
  uint64_t form;
  /* The set of form classes permitted for this attribute.  */ 
  DWARF2_class class_mask;

  name = attr->name;
  form = attr->form;
  /* Make sure this is an attribute we recognize.  */
  if (attr->name >= sizeof (attr_classes) / sizeof (attr_classes[0]))
    lto_file_corrupt_error (fd);
  /* Determine the set of permitted attribute classes.  */
  class_mask = attr_classes[attr->name];

  /* Initialize OUT.  */ 
  out->cl = DW_cl_error;
  /* The data read depends on the form given for the attribute.  */
  switch (form)
    {
    case DW_FORM_addr:
      /* Addresses are not useful without relocation info, so just skip
	 over the appropriate number of bytes.  */
      out->cl = DW_cl_address;
      fd->cur += context->cu->cu_pointer_size;
      if (fd->cur > context->cu_end)
	lto_file_corrupt_error (fd);
      break;

    case DW_FORM_string:
      out->cl = DW_cl_string;
      out->u.string = fd->cur;
      /* Skip over the string.  */
      while (true)
	{
	  if (!*fd->cur++)
	    break;
	  if (fd->cur > context->cu_end)
	    lto_file_corrupt_error (fd);
	}
      break;

    case DW_FORM_strp:
      /* Temporary hack:  we don't need the string data, but we do need
     it not to crash if it sees one.  So just skip past the offset
     and return an empty string.  */
      out->cl = DW_cl_string;
      fd->dwarf64 ? lto_read_uword (fd) : lto_read_udword (fd);
      out->u.string = "";
      break;

    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_sdata:
    case DW_FORM_udata:
      {
	uint64_t data = 0;
	int64_t sdata = 0;
	/* Read the actual data.  */
	switch (form)
	  {
	  case DW_FORM_data1:
	    data = lto_read_ubyte (fd);
	    break;
	  case DW_FORM_data2:
	    data = lto_read_uhalf (fd);
	    break;
	  case DW_FORM_data4:
	    data = lto_read_uword (fd);
	    break;
	  case DW_FORM_data8:
	    data = lto_read_udword (fd);
	    break;
	  case DW_FORM_sdata:
	    sdata = lto_read_sleb128 (fd);
	    break;
	  case DW_FORM_udata:
	    data = lto_read_uleb128 (fd);
	    break;
	  default:
	    gcc_unreachable ();
	    break;
	  }
	switch (name)
	  {
	  case DW_AT_location:
	  case DW_AT_string_length:
	  case DW_AT_return_addr:
	  case DW_AT_data_member_location:
	  case DW_AT_frame_base:
	  case DW_AT_static_link:
	  case DW_AT_use_location:
	  case DW_AT_vtable_elem_location:
	    out->cl = DW_cl_loclistptr;
	    break;

	  case DW_AT_stmt_list:
	    out->cl = DW_cl_lineptr;
	    break;

	  case DW_AT_macro_info:
	    out->cl = DW_cl_macptr;
	    break;

	  case DW_AT_ranges:
	    out->cl = DW_cl_rangelistptr;
	    break;

	  default:
	    /* These forms are constants only if they do not allow one
	       of the loclistptr, lineptr, macptr, or rangelistptr
	       classes.  */
	    gcc_assert (!(class_mask & (DW_cl_loclistptr
					| DW_cl_lineptr
					| DW_cl_macptr
					| DW_cl_rangelistptr)));
	    if (form == DW_FORM_sdata)
	      {
		out->cl = DW_cl_sconstant;
		out->u.sconstant = sdata;
	      }
	    else
	      {
		out->cl = DW_cl_uconstant;
		out->u.uconstant = data;
	      }
	  }
	break;
      }

    case DW_FORM_flag:
      out->cl = DW_cl_flag;
      out->u.flag = (lto_read_ubyte (fd) != 0);
      break;

    case DW_FORM_ref_addr:
      {
	uint64_t offset;

	/* The standard says 
	   "In the 32-bit DWARF format, this offset is a 4-byte unsigned
	   value; in the 64-bit DWARF format, it is an 8-byte unsigned
	   value"
	   FIXME: Dwarf2 is different than Dwarf3 here: Dwarf2 says offet is
	   the native address size and not offset size.
	*/
	if (!fd->dwarf64)
	  offset = lto_read_uword (fd);
	else
	  offset = lto_read_udword (fd);

	out->cl = DW_cl_reference;
	out->u.reference 
	  = lto_resolve_reference (info_fd,
				   offset,
				   context,
				   form_context);
      }
      break;

    case DW_FORM_ref1:
    case DW_FORM_ref2:
    case DW_FORM_ref4:
    case DW_FORM_ref8:
    case DW_FORM_ref_udata:
      {
	uint64_t offset;
	/* Read the offset from the start of the compilation unit to
	   the referenced DIE.  */
	switch (form)
	  {
	  case DW_FORM_ref1:
	    offset = lto_read_ubyte (fd);
	    break;
	  case DW_FORM_ref2:
	    offset = lto_read_uhalf (fd);
	    break;
	  case DW_FORM_ref4:
	    offset = lto_read_uword (fd);
	    break;
	  case DW_FORM_ref8:
	    offset = lto_read_udword (fd);
	    break;
	  case DW_FORM_ref_udata:
	    offset = lto_read_uleb128 (fd);
	    break;
	  default:
	    gcc_unreachable ();
	    break;
	  }
	out->cl = DW_cl_reference;
	out->u.reference 
	  = (lto_die_ptr) ((context->cu_start 
			    + lto_check_size_t_val (offset, 
						    "offset too large")));
	if (out->u.reference >= (lto_die_ptr) context->cu_end)
	  lto_file_corrupt_error (fd);
      }
      break;
      
    case DW_FORM_block:
    case DW_FORM_block1:
    case DW_FORM_block2:
    case DW_FORM_block4:
      {
	uint64_t length;
	switch (form)
	  {
	  case DW_FORM_block:
	    length = lto_read_uleb128 (fd);
	    break;
	  case DW_FORM_block1:
	    length = lto_read_ubyte (fd);
	    break;
	  case DW_FORM_block2:
	    length = lto_read_uhalf (fd);
	    break;
	  case DW_FORM_block4:
	    length = lto_read_uword (fd);
	    break;
	  default:
	    gcc_unreachable ();
	    break;
	  }
	out->cl = DW_cl_block;
	out->u.block.length = lto_check_size_t_val (length,
						    "block size too large");
	out->u.block.data = fd->cur;
	fd->cur += out->u.block.length;
	if (fd->cur > context->cu_end)
	  lto_file_corrupt_error (fd);
      }
      break;

    default:
      /* We cannot handle unrecognized forms.  We do not even know how
	 many bytes we ought to read.  */
      fatal_error ("unsupported DWARF form " HOST_WIDEST_INT_PRINT_UNSIGNED,
		   form);
      break;
    }
  /* Make sure that we actually read something.  */
  gcc_assert (out->cl != DW_cl_error);
  /* Check the data read is of a class appropriate for the attribute.  */
  if (!(out->cl & attr_classes[name]))
    lto_file_corrupt_error (fd);
}

/* Convert the attribute data to an int.  */
static int
attribute_value_as_int (DWARF2_form_data *attr_data)
{
  switch (attr_data->cl)
    {
    case DW_cl_uconstant:
      return lto_check_int_val (attr_data->u.uconstant,
				"unsigned value cannot be converted to int");
    case DW_cl_sconstant:
      if (attr_data->u.sconstant <= INT_MAX
	  && attr_data->u.sconstant >= INT_MIN)
	return (int)attr_data->u.sconstant;
      else
	fatal_error ("signed value cannot be converted to int");
    default:
      fatal_error ("cannot interpret attribute value as integer");
    }
}

/* A helper function: convert an unsigned HOST_WIDE_INT to a signed
   HOST_WIDE_INT.  According to ISO C, the obvious conversion is undefined
   if the unsigned value is larger than the largest positive signed
   HOST_WIDE_INT.  There are many other places in GCC that assume it works
   (and, more specifically, that a two's-complement representation is used
   for signed integers on the host machine), but to be pedantic here,
   we'll split out the conversion into its own function so that it could
   be reimplemented.  */

static HOST_WIDE_INT
make_signed_host_wide_int (unsigned HOST_WIDE_INT u)
{
  return (HOST_WIDE_INT) u;
}

/* Convert the attribute value to an integer constant of type TYPE.  If
   TYPE is null, choose an appropriate signed/unsigned int type, depending
   on the form of the attribute data.  This doesn't do any checking to make
   sure that the constant value can actually be represented in the indicated
   TYPE; we'll assume that the compiler is smart enough not to emit such
   DWARF constants in the first place.  */
static tree
attribute_value_as_constant (DWARF2_form_data *attr_data, tree type)
{
  switch (attr_data->cl)
    {
    case DW_cl_uconstant:
      {
	uint64_t u = attr_data->u.uconstant;
	if (!type)
	  type = unsigned_type_node;
	if (sizeof (HOST_WIDE_INT) >= sizeof (uint64_t))
	  return build_int_cstu (type, u);
	else
	  {
	    int size = sizeof (HOST_WIDE_INT);
	    int nbits = size * CHAR_BIT;
	    gcc_assert (size * 2 >= (int) sizeof (uint64_t));
	    return build_int_cst_wide (type, (unsigned HOST_WIDE_INT)u,
				       make_signed_host_wide_int (u >> nbits));
	  }
      }
    case DW_cl_sconstant:
      {
	int64_t s = attr_data->u.sconstant;
	if (!type)
	  type = integer_type_node;
	if (sizeof (HOST_WIDE_INT) >= (int) sizeof (uint64_t))
	  return build_int_cst (type, s);
	else
	  {
	    int size = sizeof (HOST_WIDE_INT);
	    int nbits = size * BITS_PER_UNIT;
	    gcc_assert (size * 2 >= (int) sizeof (int64_t));
	    return build_int_cst_wide (type, (unsigned HOST_WIDE_INT)s,
				       make_signed_host_wide_int (s >> nbits));
	  }
      }
    default:
      fatal_error ("cannot interpret attribute value as integer");
    }
}

/* DIE Cache

   Some DIEs (like those for types and declarations) may be referred
   to explicitly, rather than just included in the DWARF tree.  So as
   to avoid having to repeatedly process the same subtrees, we cache
   the trees assocaited with DIEs.  The DIE cache is implemented as a
   hash table, mapping in-memory DIE addresses to {TYPE,DECL}
   nodes.  */

typedef struct lto_die_cache_entry {
  /* The DIE address.  */
  lto_die_ptr die;
  /* The tree corresponding to this DIE.  */
  tree val;
  /* The address of the next sibling after the DIE.  */
  const char *sibling;
} lto_die_cache_entry;

static hashval_t
lto_cache_hash (const void *data)
{
  const lto_die_cache_entry *entry = data;

  return htab_hash_pointer (entry->die);
}

static int
lto_cache_eq (const void *data, const void *key)
{
  const lto_die_cache_entry *entry = data;
  
  return entry->die == key;
}

/* Record the fact that DIE refers to VAL.  */
static void
lto_cache_store_DIE (lto_info_fd *fd,
		     lto_die_ptr die,
		     tree val)
{
  void **slot;
  lto_die_cache_entry *entry;

  gcc_assert (TYPE_P (val) || DECL_P (val));
  /* Find a place to put the cached value.  */
  slot = htab_find_slot_with_hash (fd->die_cache, die, 
				   htab_hash_pointer (die),
				   INSERT);
  /* There can only be one entry for a given DIE.  */
  gcc_assert (!*slot);
  /* Store the value.  */
  entry = XNEW (lto_die_cache_entry);
  entry->die = die;
  entry->val = val;
  entry->sibling = fd->base.cur;
  *slot = entry;
}

/* If FD points to a DIE that has already been processed, return the
   cached value.  If SKIP is true and the DIE has already been processed,
   adjust FD to skip over the DIE and its children and point to its next
   sibling.  */
static tree
lto_cache_lookup_DIE (lto_info_fd *fd, lto_die_ptr die, bool skip)
{
  lto_die_cache_entry *entry;

  entry = htab_find_with_hash (fd->die_cache, die, 
			       htab_hash_pointer (die));
  if (entry)
    {
      if (skip)
	fd->base.cur = entry->sibling;
      return entry->val;
    }
  else
    return NULL_TREE;
}  


/* Some DIEs (notably those for DW_TAG_subrange_type and
   DW_TAG_enumeration_type) may include either or both of base type and 
   byte size attributes; the byte size is supposed modify the base
   type.  Put them together and return the resulting type.
   Additionally enforce the restriction that the base type must be
   a complete integral type.
   If neither attribute is specified, then return integer_type as the default.
*/

static tree
lto_find_integral_type (tree base_type, int byte_size, bool got_byte_size)
{
  int nbits = byte_size * BITS_PER_UNIT;

  if (! base_type)
    {
      if (! got_byte_size)
	base_type = integer_type_node;
      else if (nbits == INT_TYPE_SIZE)
	base_type = integer_type_node;
      else if (nbits == CHAR_TYPE_SIZE)
	base_type = char_type_node;
      else if (nbits == SHORT_TYPE_SIZE)
	base_type = short_integer_type_node;
      else if (nbits == LONG_TYPE_SIZE)
	base_type = long_integer_type_node;
      else if (nbits == LONG_LONG_TYPE_SIZE)
	base_type = long_long_integer_type_node;
      else
	lto_abi_mismatch_error ();
    }
  else if (! INTEGRAL_TYPE_P (base_type)
	   || !TYPE_SIZE (base_type)
	   || !host_integerp (TYPE_SIZE (base_type), 1))
    lto_abi_mismatch_error ();
  else if (got_byte_size
	   && nbits != tree_low_cst (TYPE_SIZE (base_type), 1))
    sorry ("size of base type (%d bits) doesn't match specified size (%d bits)",
	   (int) tree_low_cst (TYPE_SIZE (base_type), 1),
	   nbits);
  return base_type;
}
   
/* DIE Readers
 
   There is a DIE reader function for each supported kind of DIE.
   DIEs of type DW_TAG_<tag> are read by lto_read_<tag>_DIE.  If
   convenient, a single reader may handle several related types of
   DIEs.  If a reader handles DW_TAG_<tag1>, DW_TAG_<tag2>, ...,
   DW_TAG_<tagn>, it should be named
   lto_read_<tag1>_<tag2>_..._<tagn>_DIE.

   Each DIE reader is responsible for reading a DIE and its children.
   In general, the form of each DIE reader is:

      static tree
      lto_read_tag_DIE (lto_info_fd *fd, 
                        lto_die_ptr die,
                        const DWARF2_abbrev *abbrev,
		        lto_context *context)
      {
         gcc_assert (abbrev->tag == DW_TAG_<tag>);

         LTO_BEGIN_READ_ATTRS ()
	   {
	   case DW_AT_attr1:
	     ... handle this attribute ...
	     ;

	   case DW_AT_attr2:
	     ... handle this attribute ...
	     ;
	   }   
	 LTO_END_READ_ATTRS ();  
	 
	 lto_read_child_DIEs (fd, abbrev, context);

         return ...tree representing decoded DIE;        
      }  

   The set of attributes handled should include (at least) all the
   attributes actually emitted by GCC.  Uninteresting attributes may
   simply be ignored, but they should be named.  Attributes that you
   know are important, but for which support has not yet been
   implemented, should be handled by calling
   lto_unsupported_attr_error.  As a special case, you do not need to
   handle DW_AT_sibling; this attribute is just a hint, and is ignored
   everywhere.  LTO_{BEGIN,END}_READ_ATTRS add a default case
   that issues about errors for unhandled attributes to help to
   enforce these rules.  
   
   These rules may seem in contrast to the normal rules for a DWARF
   consumer which suggest that consumers should just ignore entries they 
   do not understand.  However, in this situation, we can assume that the
   input files came from GCC itself (so we do not need to worry about
   extensions from other tools).  Furthermore, we must be concerned
   that ignoring entries might result in silent wrong-code generation.

   The CONTEXT argument is passed down to child DIEs from parent DIEs.  

   After writing a new DIE reader function, add function to the
   READERS array in lto_read_DIE.

   There is no need for a comment above each DIE reader as their
   functionality is described by this meta-comment.  */

/* Macro to create the beginning of a loop for reading attributes for
   a DIE.  The variables FD, ABBREV, and CONTEXT must be defined by
   the caller.

   This macro should not be used by DIE readers directly; use
   LTO_BEGIN_READ_ATTRS instead.  */
#define LTO_BEGIN_READ_ATTRS_UNCHECKED()		\
  do {							\
    const DWARF2_attr *attr;				\
    for (attr = abbrev->attrs;				\
	 attr != abbrev->attrs + abbrev->num_attrs;	\
	 ++attr)					\
      {							\
	lto_context *old_context = context;		\
	lto_context *new_context = context;		\
	DWARF2_form_data attr_data;			\
							\
        lto_read_form (fd, attr, context, &new_context, \
		       &attr_data);			\
	if (context != new_context)			\
	  context = new_context;			\
	switch (attr->name)				\
	  {						\
          case DW_AT_sibling:				\
            break; 

/* Like LTO_BEGIN_READ_ATTRS_UNCHECKED, except that unhandled
   attribute are treated as indicating a corrupt object file.  

   NB: The lto_context may change depending on the form, so be careful
   when falling through to another attr name.  */
#define LTO_BEGIN_READ_ATTRS()			\
  LTO_BEGIN_READ_ATTRS_UNCHECKED()		\
    default:					\
      lto_file_corrupt_error ((lto_fd *)fd);	\
      break;  

/* Macro to create the end of a loop for reading attributes from a
   DIE.  */
#define LTO_END_READ_ATTRS()			\
         }					\
  if (context != old_context)			\
    {						\
      XDELETE (context);			\
      context = old_context;			\
    }						\
      }						\
  } while (false)

/* Print an error message indicating that an attribute (as indicated
   by ATTR) has been read from a DIE (as indicated by ABBREV) -- but
   that there is not yet code to handle this attribute.  */
static void
lto_unsupported_attr_error (const DWARF2_abbrev *abbrev,
			    const DWARF2_attr *attr)
{
  sorry ("unsupported attribute " HOST_WIDEST_INT_PRINT_UNSIGNED " for tag "
	 HOST_WIDEST_INT_PRINT_UNSIGNED, attr->name, abbrev->tag);
}

/* Return an IDENTIFIER_NODE for the string constant indicated by
   DATA.  */
static tree
lto_get_identifier (const DWARF2_form_data *data)
{
  gcc_assert (data->cl == DW_cl_string);
  /* The input string is presently in memory mapped in from the DWARF
     section.  Because the input to get_identifier must already be in
     GC-able memory, we must copy the string data.  */  
  return get_identifier (ggc_strdup (data->u.string));
}

/* Read a type DIE (located at REFERENCE) from FD.  CONTEXT is the
   current context within the compilation unit.  Returns the _TYPE
   node corresponding to the DIE.
   FIXME: Skipping around in the DWARF tree is potentially buggy if
   there are references to types thare arbitrarily nested children of
   other DIEs, because this will attempt to read those children without
   the context parentdata that would otherwise be set up by their parents.
   Current uses of parentdata for parsing DW_TAG_ENUMERATOR and
   DW_TAG_MEMBER ought to be OK since those are not types.  */
static tree
lto_read_referenced_type_DIE (lto_info_fd *fd,
			      lto_context *context,
			      lto_die_ptr reference)
{
  tree type;

  /* Check that the reference is in range.  We use an assert, rather
     than calling lto_file_corrupt_error, because REFERENCE should be
     checked for validity when it is read from the file.  */
  gcc_assert (reference >= (lto_die_ptr) context->cu_start
	      && reference < (lto_die_ptr) context->cu_end);
  type = lto_cache_lookup_DIE (fd, reference, false);
  if (!type)
    {
      const char *saved_cur = fd->base.cur;
      tree parentdata = context->parentdata; 

      /* Move the file pointer to the referenced location.  */
      fd->base.cur = (const char *) reference;
      /* Reset parent data in context.  */
      context->parentdata = NULL_TREE;
      /* Read the DIE, which we insist must be a type.  */
      type = lto_read_DIE (fd, context, NULL);
      /* Restore the file pointer and parentdata.  */
      fd->base.cur = saved_cur;
      context->parentdata = parentdata;
    }
  /* The DIE read should have been a type.  */
  if (!type || !TYPE_P (type))
    lto_file_corrupt_error ((lto_fd *)fd);

  return type;
}

static tree
lto_read_compile_unit_DIE (lto_info_fd *fd, 
			   lto_die_ptr die ATTRIBUTE_UNUSED,
			   const DWARF2_abbrev *abbrev,
			   lto_context *context)
{
  gcc_assert (abbrev->tag == DW_TAG_compile_unit);

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_language:
      {
	int lang = attribute_value_as_int (&attr_data);
	switch (lang)
	  {
	  case DW_LANG_C:
	  case DW_LANG_C89:
	  case DW_LANG_C99:
	    break;
	    
	  default:
	    error ("unsupported language %d", lang);
	    break;
	  }
	context->language = lang;
	break;
      }

    case DW_AT_low_pc:
    case DW_AT_high_pc:
    case DW_AT_ranges:
    case DW_AT_name:
    case DW_AT_stmt_list:
    case DW_AT_macro_info:
    case DW_AT_comp_dir:
    case DW_AT_producer:
    case DW_AT_use_UTF8:
    case DW_AT_segment:
      /* Ignore.  */
      break;

    case DW_AT_identifier_case:
    case DW_AT_base_types:
      lto_unsupported_attr_error (abbrev, attr);
      break;
    }
  LTO_END_READ_ATTRS ();

  lto_read_child_DIEs (fd, abbrev, context);
  return NULL_TREE;
}

static tree
lto_read_array_type_DIE (lto_info_fd *fd,
			 lto_die_ptr die ATTRIBUTE_UNUSED,
			 const DWARF2_abbrev *abbrev,
			 lto_context *context)
{
  tree type = NULL_TREE;
  VEC(tree,heap) *dims;
  int ndims;
  int order = -1;
  int i, istart, iend;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_type:
      type = lto_read_referenced_type_DIE (fd, 
					   context, 
					   attr_data.u.reference);
      break;

    case DW_AT_ordering:
      switch (attribute_value_as_int (&attr_data))
	{
	case DW_ORD_col_major:
	  /* Process dimensions left-to-right */
	  order = 1;
	  break;
	case DW_ORD_row_major:
	  /* Process dimensions right-to-left */
	  order = -1;
	  break;
	default:
	  lto_file_corrupt_error ((lto_fd *)fd);
	}
      break;
      
    case DW_AT_byte_size:
    case DW_AT_stride:
      lto_unsupported_attr_error (abbrev, attr);
      break;

    }
  LTO_END_READ_ATTRS ();

  /* The DW_AT_type attribute is required.  */
  if (!type)
    lto_file_corrupt_error ((lto_fd *)fd);

  /* Array dimensions are given as children of the DW_TAG_array_type DIE,
     and are tagged with either DW_TAG_subrange_type or
     DW_TAG_enumeration_type.  */
  dims = lto_collect_child_DIEs (fd, abbrev, context);
  ndims = VEC_length (tree, dims);

  /* Construct and cache the array type object for our caller.  */
  if (ndims == 0)
    lto_file_corrupt_error ((lto_fd *)fd);
  if (order == -1)
    {
      istart = ndims - 1;
      iend = -1;
    }
  else
    {
      gcc_assert (order == 1);
      istart = 0;
      iend = ndims;
    }
  for (i = istart; i != iend; i += order)
    {
      tree dim = VEC_index (tree, dims, i);
      if (!dim || ! INTEGRAL_TYPE_P (dim))
	lto_file_corrupt_error ((lto_fd *)fd);	
      type = build_array_type (type, dim);
    }
  VEC_free (tree, heap, dims);
  return type;
}


static tree
lto_read_structure_union_class_type_DIE (lto_info_fd *fd,
					 lto_die_ptr die ATTRIBUTE_UNUSED,
					 const DWARF2_abbrev *abbrev,
					 lto_context *context)
{
  tree type = NULL_TREE;
  bool declaration = false;
  tree name = NULL_TREE;
  int size = 0;
  unsigned int align = 0;
  tree parentdata;
  VEC(tree,heap) *children;
  int i, n;
  tree *fields_tail;
  tree *methods_tail;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_byte_size:
      if (attr_data.cl == DW_cl_uconstant || attr_data.cl == DW_cl_sconstant)
	size = attribute_value_as_int (&attr_data);
      else
	sorry ("can't handle dynamically-sized struct/union/class types");
      break;

    case DW_AT_declaration:
      declaration = attr_data.u.flag;
      break;

    case DW_AT_specification:
      type = lto_read_referenced_type_DIE (fd, context, attr_data.u.reference);
      if (TREE_CODE (type) != RECORD_TYPE
	  && TREE_CODE (type) != UNION_TYPE
	  && TREE_CODE (type) != QUAL_UNION_TYPE)
	lto_file_corrupt_error ((lto_fd *) fd);
      break;

    }
  LTO_END_READ_ATTRS ();

  /* Create the type, if this isn't a definition of a previously
     forward-declared type.  */
  if (!type)
    {
      switch (abbrev->tag)
	{
	case DW_TAG_structure_type:
	case DW_TAG_class_type:
	  type = make_node (RECORD_TYPE);
	  break;

	case DW_TAG_union_type:
	  type = make_node (UNION_TYPE);
	  break;

	default:
	  gcc_unreachable ();
	}
      switch (context->language)
	{
	case DW_LANG_C_plus_plus:
	  /* In C++, the name slot gets a TYPE_DECL.  */
	  if (name)
	    {
	      tree decl = build_decl (TYPE_DECL, name, type);
	      TYPE_NAME (type) = decl;
	      TYPE_STUB_DECL (type) = decl;
	    }
	  else
	    TYPE_STUB_DECL (type) = build_decl (TYPE_DECL, NULL_TREE, type);
	  break;

	case DW_LANG_C:
	case DW_LANG_C89:
	case DW_LANG_C99:
	default:
	  /* In C, the name slot gets the identifier which represents the
	     only the struct/union tag name, not a type name.  TYPE_STUB_DECL
	     contains an anonymous decl to be used when emitting debug info. */
	  if (name)
	    TYPE_NAME (type) = name;
	  TYPE_STUB_DECL (type) = build_decl (TYPE_DECL, NULL_TREE, type);
	  break;
	}
    }
  if (size)
    {
      TYPE_SIZE (type) = bitsize_int (size * BITS_PER_UNIT);
      TYPE_SIZE_UNIT (type) = size_int (size);
    }

  /* Process the members.  */
  parentdata = context->parentdata;
  context->parentdata = type;
  children = lto_collect_child_DIEs (fd, abbrev, context);
  context->parentdata = parentdata;

  n = VEC_length (tree, children);
  fields_tail = &TYPE_FIELDS (type);
  methods_tail = &TYPE_METHODS (type);
  for (i = 0; i < n; i++)
    {
      tree child = VEC_index (tree, children, i);

      if (!child)
	continue;

      switch (TREE_CODE (child))
	{
	case FIELD_DECL:
	  /* Field declarations.  */
	  *fields_tail = child;
	  fields_tail = &TREE_CHAIN (child);
	  /* FIXME:  This is kind of nasty.  DWARF doesn't encode the overall
	     alignment of the struct/union type, so we'll take a stab at
	     recomputing it and hope it comes out the same as for the
	     original type.  A better solution would be to extend DWARF to
	     add an additional attribute for the alignment of the record
	     type.  */
	  if (DECL_ALIGN (child) > align)
	    align = DECL_ALIGN (child);
	  /* FIXME: mess with propagating mutable/volatile/etc attributes
	     back to parent struct type.  */
	  if (TREE_READONLY (child) || TREE_THIS_VOLATILE (child))
	    sorry ("Don't know what to do with readonly or volatile fields");
	  break;
	  
	case VAR_DECL:
	  /* Static variables in a class.  */
	  *fields_tail = child;
	  fields_tail = &TREE_CHAIN (child);
	  break;

	case FUNCTION_DECL:
	  /* Member functions of a class.
	     FIXME: Extend the DW_TAG_subprogram reader to recognize
	     additional attributes for member functions.  */
	  *methods_tail = child;
	  methods_tail = &TREE_CHAIN (child);
	  break;

	  /* FIXME: Add support for DW_TAG_access_declaration,
	     DW_TAG_inheritance,  DW_TAG_friend, and DW_TAG_variant_part
	     which can appear as children of a class/struct type.  */

	default:
	  if (TYPE_P (child))
	    /* Types declared locally within the scope of a class.  */
	    {
	      tree name = TYPE_NAME (child);
	      if (name && (TREE_CODE (name) == TYPE_DECL))
		{
		  *fields_tail = name;
		  fields_tail = &TREE_CHAIN (name);
		}
	      if (TREE_CODE (child) == ENUMERAL_TYPE)
		{
		/* Add enumerators to TYPE_FIELDS, too.  */
		  tree pair;
		  for (pair = TYPE_VALUES (child); pair;
		       pair = TREE_CHAIN (pair))
		    {
		      tree decl = build_decl (CONST_DECL, TREE_PURPOSE (pair),
					      TREE_VALUE (pair));
		      *fields_tail = decl;
		      fields_tail = &TREE_CHAIN (decl);
		    }
		}
	    }
	}
    }
  VEC_free (tree, heap, children);

  /* The type mode isn't encoded in the DWARF spec, either, so just recompute
     it from scratch.  */
  compute_record_mode (type);
  if (GET_MODE_ALIGNMENT (TYPE_MODE (type)) > align)
    align = GET_MODE_ALIGNMENT (TYPE_MODE (type));
  TYPE_ALIGN (type) = align;

  /* Finish debugging output for this type.  */
  if (!declaration)
    rest_of_type_compilation (type, /*top_level=*/1);
  return type;
}

static tree
lto_read_enumeration_type_DIE (lto_info_fd *fd,
			      lto_die_ptr die ATTRIBUTE_UNUSED,
			       const DWARF2_abbrev *abbrev,
			       lto_context *context)
{
  VEC(tree,heap) *enumerators;
  int i, n;
  tree type;
  tree base = NULL_TREE;
  tree name = NULL_TREE;
  int byte_size = 0;
  bool got_byte_size = false;
  tree enumlist = NULL_TREE;
  tree parentdata;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;
      
    case DW_AT_type:
      base = lto_read_referenced_type_DIE (fd, 
					   context, 
					   attr_data.u.reference);
      break;

    case DW_AT_byte_size:
      byte_size = attribute_value_as_int (&attr_data);
      got_byte_size = true;
      break;

    case DW_AT_stride:
      lto_unsupported_attr_error (abbrev, attr);
    }
  LTO_END_READ_ATTRS ();

  /* Reconcile base type and byte size attributes.  */
  base = lto_find_integral_type (base, byte_size, got_byte_size);

  /* Build the (empty) enumeration type and fill in some attributes
     copied from the base type.  */
  type = make_node (ENUMERAL_TYPE);
  if (name)
    TYPE_NAME (type) = name;
  TYPE_MIN_VALUE (type) = TYPE_MIN_VALUE (base);
  TYPE_MAX_VALUE (type) = TYPE_MAX_VALUE (base);
  TYPE_UNSIGNED (type) = TYPE_UNSIGNED (base);
  TYPE_SIZE (type) = 0;
  TYPE_PRECISION (type) = TYPE_PRECISION (base);
  layout_type (type);

  /* Process the enumerators.  */
  parentdata = context->parentdata;
  context->parentdata = type;
  enumerators = lto_collect_child_DIEs (fd, abbrev, context);
  context->parentdata = parentdata;
  
  n = VEC_length (tree, enumerators);
  for (i = n-1; i >= 0; i--)
    {
      tree pair = VEC_index (tree, enumerators, i);
      /* Make sure the child was actually an enumerator, which parse as
	 (name, value) pairs.  */
      if (! pair
	  || TREE_CODE (pair) != TREE_LIST
	  || ! TREE_VALUE (pair)
	  || TREE_CODE (TREE_VALUE (pair)) != INTEGER_CST
	  || ! TREE_PURPOSE (pair)
	  || TREE_CODE (TREE_PURPOSE (pair)) != IDENTIFIER_NODE)
	lto_abi_mismatch_error ();
      /* Link the enumerators together.  */
      TREE_CHAIN (pair) = enumlist;
      enumlist = pair;
    }
  TYPE_VALUES (type) = enumlist;
  TYPE_STUB_DECL (type) = build_decl (TYPE_DECL, NULL_TREE, type);

  /* Finish debugging output for this type.  */
  rest_of_type_compilation (type, /*top_level=*/1);
  return type;
}

static tree
lto_read_enumerator_DIE (lto_info_fd *fd,
			 lto_die_ptr die ATTRIBUTE_UNUSED,
			 const DWARF2_abbrev *abbrev,
			 lto_context *context)
{
  tree name = NULL_TREE;
  tree value = NULL_TREE;
  tree type = context->parentdata;

  /* Enumerators can only appear as children of an enumeral type DIE.  The
     parent has already created the enum type node and stashed it in the
     context.  */
  if (!type || TREE_CODE (type) != ENUMERAL_TYPE)
    lto_file_corrupt_error ((lto_fd *)fd);

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_const_value:
      value = attribute_value_as_constant (&attr_data, type);
      break;
    }
  LTO_END_READ_ATTRS ();

  lto_read_child_DIEs (fd, abbrev, context);

  /* Return a TREE_LIST with the name as TREE_PURPOSE and the value as
     TREE_VALUE.  */
  return tree_cons (name, value, NULL_TREE);
}

static tree
lto_read_variable_formal_parameter_constant_DIE (lto_info_fd *fd,
						 lto_die_ptr die ATTRIBUTE_UNUSED,
						 const DWARF2_abbrev *abbrev,
						 lto_context *context)
{
  tree name;
  tree type;
  bool external;
  bool declaration;
  enum tree_code code;
  tree decl;

  gcc_assert (abbrev->tag == DW_TAG_variable
	      || abbrev->tag == DW_TAG_formal_parameter
	      || abbrev->tag == DW_TAG_constant);

  name = NULL_TREE;
  type = NULL_TREE;
  external = false;
  declaration = false;
  code = ERROR_MARK;
  decl = NULL_TREE;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_location:
    case DW_AT_segment:
    case DW_AT_default_value:
    case DW_AT_start_scope:
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_external:
      external = attr_data.u.flag;
      break;

    case DW_AT_type:
      type = lto_read_referenced_type_DIE (fd, 
					   context, 
					   attr_data.u.reference);
      break;

    case DW_AT_declaration:
      declaration = attr_data.u.flag;
      break;

    case DW_AT_specification:
    case DW_AT_variable_parameter:
    case DW_AT_is_optional:
    case DW_AT_const_value:
      lto_unsupported_attr_error (abbrev, attr);
      break;
    }
  LTO_END_READ_ATTRS ();

  /* Build the declaration.  */
  switch (abbrev->tag)
    {
    case DW_TAG_variable:
      code = VAR_DECL;
      break;
    case DW_TAG_formal_parameter:
      code = PARM_DECL;
      break;
    case DW_TAG_constant:
      /* GCC doesn't have a distinct tree representation for this, and never
	 generates this DWARF tag.  (CONST_DECL is only for enumerators.)
	 Do something reasonable if we see one.  */
      code = VAR_DECL;
      break;
    default:
      gcc_unreachable ();
      break;
    }
  decl = build_decl (code, name, type);

  /* Parameter decls never have external linkage, never need merging, etc.  */
  if (code == PARM_DECL)
    return decl;

  TREE_PUBLIC (decl) = external;
  DECL_EXTERNAL (decl) = declaration;
  if (!context->scope || TREE_CODE (context->scope) != FUNCTION_DECL)
    TREE_STATIC (decl) = 1;
  else
    /* ???  We should look at DW_TAG_location to figure out whether a
       function-scope variable is static or not.  We should presumably
       use the same method for variables outside of function scopes,
       for consistency.  */
    sorry ("cannot determine storage duration of local variables");
  /* If this variable has already been declared, merge the
     declarations.  */
  decl = lto_symtab_merge_var (decl);
  if (decl != error_mark_node)
    {
      /* Record this variable in the DIE cache so that it can be
	 resolved from the bodies of functions.  */
      lto_cache_store_DIE (fd, die, decl);
      /* Let the middle end know about the new entity.  */
      rest_of_decl_compilation (decl,
				/*top_level=*/1,
				/*at_end=*/0);
    }

  lto_read_child_DIEs (fd, abbrev, context);
  return decl;
}


static tree
lto_read_member_DIE (lto_info_fd *fd,
		     lto_die_ptr die ATTRIBUTE_UNUSED,
		     const DWARF2_abbrev *abbrev,
		     lto_context *context)
{
  tree name = NULL_TREE;
  tree type = NULL_TREE;
  tree byte_offset = NULL_TREE;
  tree bit_offset = NULL_TREE;
  tree bit_size = NULL_TREE;
  tree byte_size = NULL_TREE;
  bool bit_field_p = false;
  tree decl;
  int code;
  tree bitpos;
  unsigned int bitpos_align;

  /* The DWARF spec implies that data member entries can appear outside of a
     structure, union, or class, but doesn't say what that might mean.  */
  code = context->parentdata ? TREE_CODE (context->parentdata) : ERROR_MARK;
  if (code != RECORD_TYPE
      && code != UNION_TYPE
      && code != QUAL_UNION_TYPE)
    sorry ("member declaration not inside structure, class, or union");

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_type:
      type = lto_read_referenced_type_DIE (fd, 
					   context, 
					   attr_data.u.reference);
      break;

    case DW_AT_data_member_location:
      /* This is a constant or location description giving the offset in
	 bytes from the beginning of the data member.  If it's a location
	 description, it's an expression that would be evaluated as if the
	 offset of the beginning of the structure has already been pushed
	 on the stack.  */
      if (attr_data.cl == DW_cl_uconstant || attr_data.cl == DW_cl_sconstant)
	byte_offset = attribute_value_as_constant (&attr_data, bitsizetype);
      else if (attr_data.cl != DW_cl_block)
	lto_file_corrupt_error ((lto_fd *) fd);
      else
	/* Instead of doing a general location evaluation, just look
	   for the patterns generated by dwarf2out.c.  */
	{
	  struct lto_fd datafd;
	  uint64_t operator;

	  lto_fd_init (&datafd, "DW_AT_data_member_location data",
		       ((lto_fd *)fd)->file);
	  datafd.start = (const char *)attr_data.u.block.data;
	  datafd.end = datafd.start + attr_data.u.block.length;
	  datafd.cur = datafd.start;
	  datafd.dwarf64 = ((lto_fd *)fd)->dwarf64;

	  operator = lto_read_uleb128 (&datafd);
	  if (operator == DW_OP_constu || operator == DW_OP_plus_uconst)
	    {
	      uint64_t operand = lto_read_uleb128 (&datafd);
	      HOST_WIDE_INT opval =
		lto_check_HOST_WIDE_INT_val (operand, "offset too large");
	      byte_offset = build_int_cst (bitsizetype, opval);
	      if (datafd.cur != datafd.end)
		lto_unsupported_attr_error (abbrev, attr);
	    }
	  else
	    lto_unsupported_attr_error (abbrev, attr);
	}
      break;

    case DW_AT_byte_size:
      byte_size = attribute_value_as_constant (&attr_data, bitsizetype);
      break;

    case DW_AT_bit_offset:
      bit_offset = attribute_value_as_constant (&attr_data, bitsizetype);
      bit_field_p = true;
      break;

    case DW_AT_bit_size:
      bit_size = attribute_value_as_constant (&attr_data, bitsizetype);
      bit_field_p = true;
      break;

    case DW_AT_accessibility:
    case DW_AT_mutable:
      lto_unsupported_attr_error (abbrev, attr);
      break;

    }
  LTO_END_READ_ATTRS ();

  /* Complain if we didn't get a type.  */
  if (! type)
    lto_file_corrupt_error ((lto_fd *) fd);

  /* Complain if we got a non-zero offset or bit field attribute for a union,
     or a missing offset when we're not processing a union, or incomplete
     bit field information.  */
  if (code == UNION_TYPE)
    {
      if (bit_field_p)
	lto_file_corrupt_error ((lto_fd *) fd);
      if (!byte_offset)
	byte_offset = bitsize_zero_node;
      else if (! integer_zerop (byte_offset))
	lto_file_corrupt_error ((lto_fd *) fd);
    }
  else
    {
      if (! byte_offset)
	lto_file_corrupt_error ((lto_fd *) fd);
      if (bit_field_p && ! (bit_size && bit_offset))
	lto_file_corrupt_error ((lto_fd *) fd);
    }

  /* Make sure we got a byte size if we need one, or that it's not
     incompatible with the specified type.  */
  if (bit_field_p && ! byte_size)
    byte_size = fold_convert (bitsizetype, TYPE_SIZE_UNIT (type));
  else if (!bit_field_p && byte_size &&
	   !tree_int_cst_equal (byte_size, TYPE_SIZE_UNIT (type)))
    sorry ("don't know what to do with DW_AT_byte_size for non-bitfields");

  /* Compute the bit position of the field relative to the beginning of
     its containing structure.  */
  bitpos = size_binop (MULT_EXPR, byte_offset, bitsize_unit_node);
  if (bit_field_p)
    {
      /* DWARF's bit offset is the number of bits from the most significant
	 bit of the anonymous containing object to the most significant
	 bit of the bit field.  So we need to reverse to count from the
	 opposite end of the container in the little-endian case.  */
      if (! BYTES_BIG_ENDIAN)
	{
	  tree container_bit_size =
	    size_binop (MULT_EXPR, byte_size, bitsize_unit_node);
	  bit_offset = size_binop (MINUS_EXPR,
				   size_binop (MINUS_EXPR,
					       container_bit_size, bit_offset),
				   bit_size);
	}
      bitpos = size_binop (PLUS_EXPR, bitpos, bit_offset);
    }

  /* Next we need to find the alignment of the bit position.  This bit of
     code copied from the Ada front end.  */
  if (host_integerp (bitpos, 1))
    bitpos_align = (tree_low_cst (bitpos, 1) & -
		   tree_low_cst (bitpos, 1));
  else
    bitpos_align = BITS_PER_UNIT;

  /* Build the decl and fill in its attributes.  For bit fields, create
     and layout the field with its declared type, then overwrite it with
     a type of the specified precision; this is the way the C front end
     does it, and doing it the same way ensures that all of the various
     types, sizes, and alignments are compatible with those generated by
     C.  */
  decl = build_decl (FIELD_DECL, name, type);
  if (bit_field_p)
    {
      DECL_SIZE (decl) = bit_size;
      DECL_BIT_FIELD (decl) = true;
      DECL_NONADDRESSABLE_P (decl) = true;
      layout_decl (decl, bitpos_align);
      TREE_TYPE (decl) =
	build_nonstandard_integer_type (tree_low_cst (bit_size, 1),
					TYPE_UNSIGNED (type));
      DECL_MODE (decl) = TYPE_MODE (type);
    }
  else
    layout_decl (decl, bitpos_align);
  DECL_CONTEXT (decl) = context->parentdata;

  /* Now set the offset explicitly.  This bit of code was stolen from the
     Ada front end.  */
  SET_DECL_OFFSET_ALIGN (decl,
			 (host_integerp (bitpos, 1)
			  ? BIGGEST_ALIGNMENT : BITS_PER_UNIT));
  pos_from_bit (&DECL_FIELD_OFFSET (decl),
		&DECL_FIELD_BIT_OFFSET (decl),
		DECL_OFFSET_ALIGN (decl),
		bitpos);

  lto_read_child_DIEs (fd, abbrev, context);
  return decl;
}

static tree
lto_read_subroutine_type_subprogram_DIE (lto_info_fd *fd,
					 lto_die_ptr die ATTRIBUTE_UNUSED,
					 const DWARF2_abbrev *abbrev,
					 lto_context *context)
{
  tree ret_type;
  tree arg_types;
  tree type;
  tree name;
  tree asm_name = NULL_TREE;
  bool external;
  VEC(tree,heap) *parms;
  unsigned n_parms;
  unsigned n_children;
  unsigned i, j;
  bool prototyped;
  bool varargs_p;
  tree result;
  int inlined = DW_INL_not_inlined;

  gcc_assert (abbrev->tag == DW_TAG_subroutine_type
	      || abbrev->tag == DW_TAG_subprogram);

  ret_type = NULL_TREE;
  prototyped = false;
  name = NULL_TREE;
  external = false;

  if (abbrev->tag == DW_TAG_subroutine_type)
    {
      LTO_BEGIN_READ_ATTRS ()
	{
	case DW_AT_type:
	  ret_type = lto_read_referenced_type_DIE (fd,
						   context,
						   attr_data.u.reference);
	  break;

	case DW_AT_prototyped:
	  prototyped = attr_data.u.flag;
	  break;
	}
      LTO_END_READ_ATTRS ();
    }
  else
    {
      LTO_BEGIN_READ_ATTRS ()
	{
	case DW_AT_decl_column:
	case DW_AT_decl_file:
	case DW_AT_decl_line:
	  /* Ignore.  */
	  break;

	case DW_AT_low_pc:
	case DW_AT_high_pc:
	case DW_AT_ranges:
	case DW_AT_frame_base:
	  /* Ignore.  */
	  break;

	case DW_AT_name:
	  name = lto_get_identifier (&attr_data);
	  break;

	case DW_AT_external:
	  external = attr_data.u.flag;
	  break;

	case DW_AT_type:
	  ret_type = lto_read_referenced_type_DIE (fd,
						   context,
						   attr_data.u.reference);
	  break;

	case DW_AT_prototyped:
	  prototyped = attr_data.u.flag;
	  break;

	case DW_AT_inline:
	  inlined = attribute_value_as_int (&attr_data);
	  break;

        case DW_AT_MIPS_linkage_name:
          /* Set the DECL_ASSEMBLER_NAME so we don't have to remangle
             names in LTO.  */
          asm_name = lto_get_identifier (&attr_data);
          break;
	}
      LTO_END_READ_ATTRS ();
    }


  /* The DWARF3 specification says that a return type is only
     specified for functions that return a value.  Therefore,
     functions without an explicit return type return "void".  */
  if (!ret_type)
    ret_type = void_type_node;
 
  if (!prototyped)
    sorry ("support for unprototyped functions not yet implemented");

  parms = lto_collect_child_DIEs (fd, abbrev, context);

  /* Per the DWARF spec, the children can include "other entries used
     by formal parameter entries, such as types", as well as parameters
     and a DW_TAG_unspecified_parameters to indicate varargs.  So we need
     one loop over the children to count the number of actual parameters
     and another to assemble the parameter type vector.  */
  n_children = VEC_length (tree, parms);
  n_parms = 0;
  varargs_p = !prototyped;
  for (i = 0; i < n_children; ++i)
    {
      tree parm = VEC_index (tree, parms, i);
      if (parm == lto_varargs_cookie)
	varargs_p = true;
      else if (TREE_CODE (parm) == PARM_DECL)
	n_parms++;
    }
  arg_types = make_tree_vec (n_parms + (varargs_p ? 0 : 1));
  for (i = 0, j = 0; i < n_children; ++i)
    {
      tree parm = VEC_index (tree, parms, i);
      if (TREE_CODE (parm) == PARM_DECL)
	{
	  TREE_VEC_ELT (arg_types, j) = TREE_TYPE (parm);
	  j++;
	}
    }
  if (!varargs_p)
    TREE_VEC_ELT (arg_types, j) = void_type_node;
  VEC_free (tree, heap, parms);

  /* Build the function type.  */
  type = build_function_type (ret_type, arg_types);
  if (abbrev->tag == DW_TAG_subroutine_type)
    result = type;
  else
    {
      const void *body;
      lto_file *file;
      const char *name_str;

      if (!name)
	lto_file_corrupt_error ((lto_fd *)fd);

      result = build_decl (FUNCTION_DECL, name, type);
      TREE_PUBLIC (result) = external;
      /* Load the body of the function.  */
      file = fd->base.file;
      name_str = IDENTIFIER_POINTER (name);
      body = file->vtable->map_fn_body (file, name_str);
      if (body)
	{
	  /* This function has a definition.  */
	  TREE_STATIC (result) = 1;
	  DECL_EXTERNAL (result) = 0;
	}
      else
	DECL_EXTERNAL (result) = 1;
      if (inlined == DW_INL_declared_inlined
	  || inlined == DW_INL_declared_not_inlined)
	DECL_DECLARED_INLINE_P (result) = 1;

      /* Set the DECL_ASSEMBLER_NAME for the function from the
         information in the DIE.  */
      SET_DECL_ASSEMBLER_NAME (result, asm_name ? asm_name : name);

      /* If the function has already been declared, merge the
	 declarations.  */
      result = lto_symtab_merge_fn (result);
      if (result != error_mark_node)
	{
	  /* Record this function in the DIE cache so that it can be
	     resolved from the bodies of functions.  */
	  lto_cache_store_DIE (fd, die, result);
	  /* If the function has a body, read it in.  */ 
	  if (body)
	    {
	      DECL_RESULT (result)
		= build_decl (RESULT_DECL, NULL_TREE,
			      TYPE_MAIN_VARIANT (ret_type));
	      allocate_struct_function (result);
	      lto_read_function_body (fd, context, result, body);
	      file->vtable->unmap_fn_body (file, name_str, body);
	    }
	  /* Let the middle end know about the function.  */
	  rest_of_decl_compilation (result,
				    /*top_level=*/1,
				    /*at_end=*/0);
	  if (body)
	    cgraph_finalize_function (result, /*nested=*/false);
	}
    }

  return result;
}

static tree
lto_read_unspecified_parameters_DIE (lto_info_fd *fd,
				     lto_die_ptr die ATTRIBUTE_UNUSED,
				     const DWARF2_abbrev *abbrev,
				     lto_context *context)
{
  gcc_assert (abbrev->tag == DW_TAG_unspecified_parameters);
  LTO_BEGIN_READ_ATTRS ()
    {
    }
  LTO_END_READ_ATTRS ();
  lto_read_child_DIEs (fd, abbrev, context);
  return lto_varargs_cookie;
}

static tree
lto_read_typedef_DIE (lto_info_fd *fd,
		      lto_die_ptr die ATTRIBUTE_UNUSED,
		      const DWARF2_abbrev *abbrev,
		      lto_context *context)
{
  tree base_type = NULL_TREE;
  tree name = NULL_TREE;
  tree decl;
  tree type;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_type:
      base_type = lto_read_referenced_type_DIE (fd, 
						context, 
						attr_data.u.reference);
      break;

    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    }
  LTO_END_READ_ATTRS ();

  /* The DW_AT_name attribute is required.  */
  if (!name)
    lto_file_corrupt_error ((lto_fd *)fd);
  /* The DW_AT_type attribute is supposed to be required, but since DWARF
     has no representation for the void type, dwarf2out.c omits the type
     attribute in that case.  So, in order to allow GCC's output to be
     read in again, we have to interpret a missing base type attribute as
     the void type, too.  */
  if (!base_type)
    base_type = void_type_node;
  /* Build the typedef.  */
  type = build_variant_type_copy (base_type);
  decl = build_decl (TYPE_DECL, name, type);
  TYPE_NAME (type) = decl;
  DECL_ORIGINAL_TYPE (decl) = base_type;

  lto_read_child_DIEs (fd, abbrev, context);
  return type;
}


static tree
lto_read_pointer_reference_type_DIE (lto_info_fd *fd,
				     lto_die_ptr die ATTRIBUTE_UNUSED,
				     const DWARF2_abbrev *abbrev,
				     lto_context *context)
{
  tree pointed_to = NULL_TREE;
  tree type;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_type:
      pointed_to = lto_read_referenced_type_DIE (fd, 
						 context, 
						 attr_data.u.reference);
      break;

    case DW_AT_byte_size:
      if (attribute_value_as_int (&attr_data) * BITS_PER_UNIT != POINTER_SIZE)
	lto_abi_mismatch_error ();
      break;
    }
  LTO_END_READ_ATTRS ();

  /* The DW_AT_type attribute is supposed to be required, but since DWARF
     has no representation for the void type, dwarf2out.c omits the type
     attribute in that case.  So, in order to allow GCC's output to be
     read in again, we have to interpret a missing base type attribute as
     the void type, too.  */
  if (!pointed_to)
    pointed_to = void_type_node;
  /* Build the pointer or reference type.  */
  switch (abbrev->tag)
    {
    case DW_TAG_pointer_type:
      type = build_pointer_type (pointed_to);
      break;
    case DW_TAG_reference_type:
      type = build_reference_type (pointed_to);
      break;
    default:
      gcc_unreachable ();
    }

  lto_read_child_DIEs (fd, abbrev, context);
  return type;
}

static tree
lto_read_subrange_type_DIE (lto_info_fd *fd,
			    lto_die_ptr die ATTRIBUTE_UNUSED,
			    const DWARF2_abbrev *abbrev,
			    lto_context *context)
{
  tree type;
  tree base = NULL_TREE;
  int byte_size = 0;
  bool got_byte_size = false;
  tree lower = NULL_TREE;
  tree upper = NULL_TREE;
  tree count = NULL_TREE;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_decl_column:
    case DW_AT_decl_file:
    case DW_AT_decl_line:
      /* Ignore.  */
      break;

    case DW_AT_type:
      base = lto_read_referenced_type_DIE (fd, 
					   context, 
					   attr_data.u.reference);
      break;

    case DW_AT_byte_size:
      byte_size = attribute_value_as_int (&attr_data);
      got_byte_size = true;
      break;

    case DW_AT_lower_bound:
      lower = attribute_value_as_constant (&attr_data, NULL_TREE);
      break;

    case DW_AT_upper_bound:
      upper = attribute_value_as_constant (&attr_data, NULL_TREE);
      break;

    case DW_AT_count:
      count = attribute_value_as_constant (&attr_data, NULL_TREE);
      break;

    case DW_AT_threads_scaled:
    case DW_AT_stride:
      lto_unsupported_attr_error (abbrev, attr);
    }
  LTO_END_READ_ATTRS ();

  /* Reconcile base type and byte size attributes.  */
  base = lto_find_integral_type (base, byte_size, got_byte_size);

  /* Lower bound can be omitted if there is a language-specific default.  */
  if (!lower)
    {
      switch (context->language)
	{
	case DW_LANG_C89:
	case DW_LANG_C:
	case DW_LANG_C_plus_plus:
	case DW_LANG_C99:
	  lower = integer_zero_node;
	  break;

	case DW_LANG_Fortran77:
	case DW_LANG_Fortran90:
	case DW_LANG_Fortran95:
	  lower = integer_one_node;
	  break;

	default:
	  /* No other default lower bound values are currently defined.  */
	  lto_file_corrupt_error ((lto_fd *)fd);
	}
    }

  /* At most one of count and upper bound can be specified.  If we got count,
     use it to compute the upper bound.  */
  if (count)
    {
      if (upper)
	lto_file_corrupt_error ((lto_fd *)fd);
      else if (host_integerp (count, 0) && host_integerp (lower, 0))
	upper = build_int_cst (TREE_TYPE (count),
			       TREE_INT_CST_LOW (lower)
			       + TREE_INT_CST_LOW (count) - 1);
      else
	sorry ("can't compute upper array bound");
    }

  /* Build the range type, and record it for our caller.  */
  type = build_range_type (base, lower, upper);

  lto_read_child_DIEs (fd, abbrev, context);
  return type;
}

static tree
lto_read_base_type_DIE (lto_info_fd *fd, 
			lto_die_ptr die ATTRIBUTE_UNUSED,
			const DWARF2_abbrev *abbrev,
			lto_context *context)
{
  tree name;
  bool have_encoding;
  enum dwarf_type encoding;
  bool have_size;
  bool have_offset;
  bool have_bits;
  int size;
  tree type;
  int bits;
  int offset;
  int maxbits;

  name = NULL_TREE;
  have_encoding = false;
  encoding = DW_ATE_void;
  have_size = false;
  size = 0;
  have_bits = false;
  bits = 0;
  have_offset = false;
  offset = 0;
  type = NULL_TREE;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_encoding:
      have_encoding = true;
      encoding = attribute_value_as_int (&attr_data);
      break;

    case DW_AT_byte_size:
      have_size = true;
      size = attribute_value_as_int (&attr_data);
      break;

    case DW_AT_bit_size:
      have_bits = true;
      bits = attribute_value_as_int (&attr_data);
      break;

    case DW_AT_bit_offset:
      have_offset = true;
      offset = attribute_value_as_int (&attr_data);
      break;
    }
  LTO_END_READ_ATTRS ();

  if (!have_encoding || !have_size)
    lto_file_corrupt_error ((lto_fd *)fd);

  /* The DWARF spec implies that DW_AT_bit_size and DW_AT_bit_offset go
     together as a pair.  We only support the case where integral types are
     aligned in the low order bits of its containing word, which is how
     dwarf2out.c encodes types with "weird" precisions.  */
  maxbits = BITS_PER_UNIT * size;
  if (have_offset && have_bits)
    {
      if (bits > maxbits)
	lto_file_corrupt_error ((lto_fd *)fd);
      if (encoding != DW_ATE_unsigned && encoding != DW_ATE_signed)
	sorry ("bit size attribute only supported for signed/unsigned types");
      if (offset != maxbits - bits)
	sorry ("unaligned base type not supported");
    }
  else if (have_offset || have_bits)
    lto_file_corrupt_error ((lto_fd *)fd);
  else
    bits = maxbits;

  lto_read_child_DIEs (fd, abbrev, context);

  /* Build the type.  */
  switch (encoding)
    {
    case DW_ATE_unsigned:
      type = build_nonstandard_integer_type (bits, 1);
      break;

    case DW_ATE_signed:
      type = build_nonstandard_integer_type (bits, 0);
      break;
      
    case DW_ATE_unsigned_char:
      type = build_nonstandard_integer_type (bits, 1);
      TYPE_STRING_FLAG (type) = 1;
      break;

    case DW_ATE_signed_char:
      type = build_nonstandard_integer_type (bits, 0);
      TYPE_STRING_FLAG (type) = 1;
      break;

    case DW_ATE_boolean:
      type = build_nonstandard_integer_type (bits, 1);
      TREE_SET_CODE (type, BOOLEAN_TYPE);
      TYPE_MAX_VALUE (type) = build_int_cst (type, 1);
      TYPE_PRECISION (type) = 1;
      break;

    case DW_ATE_float:
      type = make_node (REAL_TYPE);
      TYPE_PRECISION (type) = bits;
      layout_type (type);
      break;

    case DW_ATE_decimal_float:
    case DW_ATE_complex_float:
    case DW_ATE_lo_user:  
    default:
      sorry ("unsupported base type encoding - 0x%x", encoding);
      break;
    }
  /* If this is a new type, declare it.
     The DWARF spec seems to imply that the name attribute is required,
     but GCC generates base_type DIEs without names in some cases (e.g.,
     for the referenced type of the subrange type used as an array
     subscript).  Accept that without complaining by just skipping making
     the declaration.  */
  if (name && !TYPE_NAME (type))
    {
      tree decl;
      decl = build_decl (TYPE_DECL, name, type);
      TYPE_NAME (type) = decl;
      /* Let the middle end know about the type declaration.  */
      rest_of_decl_compilation (decl, 
				/*top_level=*/1,
				/*at_end=*/0);
    }
 
  return type;
}

static tree
lto_read_const_volatile_restrict_type_DIE (lto_info_fd *fd,
					   lto_die_ptr die ATTRIBUTE_UNUSED,
					   const DWARF2_abbrev *abbrev,
					   lto_context *context)
{
  tree base_type = NULL_TREE;
  tree type;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_type:
      base_type = lto_read_referenced_type_DIE (fd, 
						context, 
						attr_data.u.reference);
      break;
    }
  LTO_END_READ_ATTRS ();

  /* The DW_AT_type attribute is supposed to be required, but since DWARF
     has no representation for the void type, dwarf2out.c omits the type
     attribute in that case.  So, in order to allow GCC's output to be
     read in again, we have to interpret a missing base type attribute as
     the void type, too.  */
  if (!base_type)
    base_type = void_type_node;
  /* Build the modified type.  */
  switch (abbrev->tag)
    {
    case DW_TAG_const_type:
      type = build_qualified_type (base_type, TYPE_QUAL_CONST);
      break;
    case DW_TAG_volatile_type:
      type = build_qualified_type (base_type, TYPE_QUAL_VOLATILE);
      break;
    case DW_TAG_restrict_type:
      type = build_qualified_type (base_type, TYPE_QUAL_RESTRICT);
      break;
    default:
      gcc_unreachable ();
    }

  lto_read_child_DIEs (fd, abbrev, context);
  return type;
}

static tree
lto_read_unspecified_type_DIE (lto_info_fd *fd,
			       lto_die_ptr die ATTRIBUTE_UNUSED,
			       const DWARF2_abbrev *abbrev,
			       lto_context *context)
{
  tree type = NULL_TREE;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_name:
      gcc_assert (attr_data.cl == DW_cl_string);
      if (strcmp (attr_data.u.string, "void") == 0)
	type = void_type_node;
      break;
    }
  LTO_END_READ_ATTRS ();

  if (!type)
    sorry ("unsupported use of DW_TAG_unspecified_type");
  lto_read_child_DIEs (fd, abbrev, context);
  return type;
}

/* Read the next DIE from FD.  CONTEXT provides information about the
   current state of the compilation unit.  Returns a (possibly null) TREE
   representing the DIE read.  If more is non-NULL, *more is set to true
   iff there was a real DIE present; false if the DIE was a null entry
   indicating the end of a list of sibling DIEs.  */
static tree
lto_read_DIE (lto_info_fd *fd, lto_context *context, bool *more)
{
  typedef tree (*DIE_reader_fnptr)(lto_info_fd * fd,
				   lto_die_ptr die,
				   const DWARF2_abbrev *abbrev,
				   lto_context *context);
  /* Reader functions for the tags defined by DWARF 3.  */
  static const DIE_reader_fnptr readers[DW_TAG_shared_type + 1] = 
    {
      NULL, /* padding */
      lto_read_array_type_DIE,
      lto_read_structure_union_class_type_DIE,
      NULL, /* entry_point */
      lto_read_enumeration_type_DIE,
      lto_read_variable_formal_parameter_constant_DIE,
      NULL, /* padding */
      NULL, /* padding */
      NULL, /* imported_declaration */
      NULL, /* padding */
      NULL, /* label */
      NULL, /* lexical_block */
      NULL, /* padding */
      lto_read_member_DIE,
      NULL, /* padding */
      lto_read_pointer_reference_type_DIE,
      lto_read_pointer_reference_type_DIE,
      lto_read_compile_unit_DIE,
      NULL, /* string_type */
      lto_read_structure_union_class_type_DIE,
      NULL, /* padding */
      lto_read_subroutine_type_subprogram_DIE,
      lto_read_typedef_DIE,
      lto_read_structure_union_class_type_DIE,
      lto_read_unspecified_parameters_DIE,
      NULL, /* variant */
      NULL, /* common_block */
      NULL, /* common_inclusion */
      NULL, /* inheritance */
      NULL, /* inlined_subroutine */
      NULL, /* module */
      NULL, /* ptr_to_member_type */
      NULL, /* set_type */
      lto_read_subrange_type_DIE,
      NULL, /* with_stmt */
      NULL, /* access_declaration */
      lto_read_base_type_DIE,
      NULL, /* catch_block */
      lto_read_const_volatile_restrict_type_DIE,
      lto_read_variable_formal_parameter_constant_DIE,
      lto_read_enumerator_DIE,
      NULL, /* file_type */
      NULL, /* friend */
      NULL, /* namelist */
      NULL, /* namelist_item */
      NULL, /* packed_type */
      lto_read_subroutine_type_subprogram_DIE,
      NULL, /* template_type_param */
      NULL, /* template_value_param */
      NULL, /* thrown_type */
      NULL, /* try_block */
      NULL, /* variant_part */
      lto_read_variable_formal_parameter_constant_DIE,
      lto_read_const_volatile_restrict_type_DIE,
      NULL, /* dwarf_procedure */
      lto_read_const_volatile_restrict_type_DIE,
      NULL, /* interface_type */
      NULL, /* namespace */
      NULL, /* imported_module */
      lto_read_unspecified_type_DIE, 
      NULL, /* partial_unit */
      NULL, /* imported_unit */
      NULL, /* padding */
      NULL, /* condition */
      NULL /* shared_type */
    };

  uint64_t index;
  const DWARF2_abbrev *abbrev;
  DIE_reader_fnptr reader;
  lto_die_ptr die;
  tree val;

  /* Record the location of the current DIE -- before we change the
     file pointer.  */
  die = (lto_die_ptr) ((lto_fd *)fd)->cur;
  /* Read the abbreviation index.  */
  index = lto_read_uleb128 ((lto_fd *)fd);
  /* Zero indicates a null entry.  */
  if (!index)
    {
      if (more)
	*more = false;
      return NULL_TREE;
    }
  /* Get the actual abbreviation entry.  */
  abbrev = lto_abbrev_lookup (&fd->base.file->debug_abbrev, index);
  /* Check to see if this DIE has already been processed.  If so, skip
     over it and its children.  */
  val = lto_cache_lookup_DIE (fd, die, /*skip=*/true);
  if (!val)
    {
      /* Determine the DIE reader function.  */
      reader = NULL;
      if (abbrev->tag < sizeof (readers) / sizeof (DIE_reader_fnptr))
	reader = readers[abbrev->tag];
      if (reader)
	{
	  /* If there is a reader, use it.  */
	  val = reader (fd, die, abbrev, context);
	  /* If this DIE refers to a type, cache the value so that future
	     references to the type can be processed quickly.  */
	  if (val && TYPE_P (val))
	    lto_cache_store_DIE (fd, die, val);
	}
      else
	{
	  /* Assume that all other tags matter, as we are otherwise at
	     risk of silently generating wrong code.  If a tag can be
	     safely ignored, it should be explicitly ignored above.  */
	  error ("DWARF tag " HOST_WIDEST_INT_PRINT_UNSIGNED " not "
		 "supported by link-time optimization", abbrev->tag);

	  /* Skip over this DIE, but attempt to read its children.  */
	  LTO_BEGIN_READ_ATTRS_UNCHECKED ()
	    {
	    }
	  LTO_END_READ_ATTRS ();
	  /* Read children.  */ 
	  lto_read_child_DIEs (fd, abbrev, context);
	}
    }

  if (more)
    *more = true;
  return val;
}

/* Read the children of a DIE from FD, passing CONTEXT to the DIE readers
   for the children.  The attributes of the DIE have already been
   read.  ABBREV indicates whether or not the DIE actually has
   children; if it does not, then this function returns without
   reading anything.  Therefore, it is always safe to call this
   function from a DIE reader, after reading the DIE's attributes.
   This function simply discards the tree valued returned by lto_read_DIE
   for each child.
*/
static void
lto_read_child_DIEs (lto_info_fd *fd, 
		     const DWARF2_abbrev *abbrev,
		     lto_context *context)
{
  if (abbrev->has_children)
    {
      bool more;
      do 
	lto_read_DIE (fd, context, &more);
      while (more);
    }
}

/* This function is similar to lto_read_child_DIEs but collects the tree
   values from reading each child into a heap-allocated VEC, which is
   returned.
*/
static VEC(tree,heap) *
lto_collect_child_DIEs (lto_info_fd *fd, 
			const DWARF2_abbrev *abbrev,
			lto_context *context)
{
  VEC(tree,heap) *result = VEC_alloc (tree, heap, 32);
  if (abbrev->has_children)
    {
      bool more;
      do
	{
	  tree val = lto_read_DIE (fd, context, &more);
	  if (more)
	    VEC_safe_push (tree, heap, result, val);
	}
      while (more);
    }
  return result;
}


/* Read all the DWARF2 compile units from INFO_FD, placing them in
   INFO_FD->UNITS.  */
static void
lto_info_read (lto_info_fd *info_fd)
{
  size_t num_units;
  lto_fd *fd;
  unsigned long offset;
  size_t current_unit;

  /* We should only read the compilation units once.  */
  gcc_assert (!info_fd->units);
  fd = (lto_fd *)info_fd;

  /* Start reading from the beginning of the section.  */
  fd->cur = fd->start;

  num_units = 0;

  /* Get the number of compilation units.  */
  while (fd->cur < fd->end)
    {
      /* The compilation-unit header.  */ 
      DWARF2_CompUnit cu;
      const char *before_header = fd->cur;
      
      /* Read the compilation unit header.  */
      lto_read_comp_unit_header (info_fd, &cu);

      fd->cur = before_header + cu.cu_length;
      num_units++;
    }
 
  info_fd->num_units = num_units;
  info_fd->units = XCNEWVEC (DWARF2_CompUnit *, num_units);

  /* Now read the actual compilation units.  */
  fd->cur = fd->start;
  offset = 0;
  current_unit = 0;

  /* Read the compilation units.  */
  while (fd->cur < fd->end)
    {
      const char *before_header = fd->cur;
      unsigned long header_length;
      DWARF2_CompUnit *cu;

      cu = XCNEW (DWARF2_CompUnit);
      info_fd->units[current_unit] = cu;

      /* Read the compilation unit header.  */
      lto_read_comp_unit_header (info_fd, cu);
          
      header_length = fd->cur - before_header;
      cu->cu_start_offset = offset + header_length;
      cu->cu_header_length = header_length;

      fd->cur = before_header + cu->cu_length;
      offset = fd->cur - fd->start;
      current_unit++;
    }
}

/* Setup compile unit fields in CONTEXT from FD and UNIT */
static void
lto_set_cu_context (lto_context *context, lto_info_fd *fd,
		    DWARF2_CompUnit *unit)
{
  context->cu_start = 
    fd->base.start + unit->cu_start_offset - unit->cu_header_length;
  context->cu_end = context->cu_start + unit->cu_length;
  context->cu = unit;
}

bool
lto_file_read (lto_file *file)
{
  size_t i;
  /* The descriptor for the .debug_info section.  */
  lto_fd *fd;

  /* Read the abbreviation entries.  */
  lto_abbrev_read (&file->debug_abbrev);
  /* Read the compilation units.  */
  lto_info_read (&file->debug_info);

  fd = &file->debug_info.base;

  for (i = 0; i < file->debug_info.num_units; i++)
    {
      /* The current compilation unit.  */
      DWARF2_CompUnit *unit = file->debug_info.units[i];
      /* The context information for this compilation unit.  */
      lto_context context;

      /* Set up the context.  */
      lto_set_cu_context (&context, &file->debug_info, unit);
      fd->cur = context.cu_start + unit->cu_header_length;
      context.scope = NULL_TREE;
      context.parentdata = NULL_TREE;

      /* Read DIEs.  */
      while (fd->cur < context.cu_end)
	lto_read_DIE (&file->debug_info, &context, NULL);
    }

  return true;
}

tree 
lto_resolve_type_ref (lto_info_fd *info_fd,
		      lto_context *context,
		      const lto_ref *ref)
{
  lto_die_ptr die;
  lto_context *new_context = context;
  tree type;

  /* At present, we only support a single DWARF section.  */
  if (ref->section != 0)
    lto_abi_mismatch_error ();
  /* Map REF to a DIE.  */
  die = lto_resolve_reference (info_fd, ref->offset, context, &new_context);
  /* Map DIE to a type.  */
  type = lto_read_referenced_type_DIE (info_fd, context, die);
  if (!type || !TYPE_P (type))
    lto_file_corrupt_error ((lto_fd *)info_fd);
  /* Clean up.  */
  if (new_context != context)
    XDELETE (new_context);

  return type;
}

tree 
lto_resolve_var_ref (lto_info_fd *info_fd,
		     lto_context *context,
		     const lto_ref *ref)
{
  lto_die_ptr die;
  lto_context *new_context = context;
  tree var;

  /* At present, we only support a single DWARF section.  */
  if (ref->section != 0)
    lto_abi_mismatch_error ();
  /* Map REF to a DIE.  */
  die = lto_resolve_reference (info_fd, ref->offset, context, &new_context);
  /* Map DIE to a variable.  */
  var = lto_cache_lookup_DIE (info_fd, die, /*skip=*/false);
  if (!var || TREE_CODE (var) != VAR_DECL)
    lto_file_corrupt_error ((lto_fd *)info_fd); 
  /* Clean up.  */
  if (new_context != context)
    XDELETE (new_context);

  return var;
}

tree 
lto_resolve_fn_ref (lto_info_fd *info_fd,
		    lto_context *context,
		    const lto_ref *ref)
{
  lto_die_ptr die;
  lto_context *new_context = context;
  tree fn;

  /* At present, we only support a single DWARF section.  */
  if (ref->section != 0)
    lto_abi_mismatch_error ();
  /* Map REF to a DIE.  */
  die = lto_resolve_reference (info_fd, ref->offset, context, &new_context);
  /* Map DIE to a variable.  */
  fn = lto_cache_lookup_DIE (info_fd, die, /*skip=*/false);
  if (!fn || TREE_CODE (fn) != FUNCTION_DECL)
    lto_file_corrupt_error ((lto_fd *)info_fd); 
  /* Clean up.  */
  if (new_context != context)
    XDELETE (new_context);

  return fn;
}

void
lto_main (int debug_p ATTRIBUTE_UNUSED)
{
  unsigned i;

  /* Read all of the object files specified on the command line.  */
  for (i = 0; i < num_in_fnames; ++i)
    {
      lto_file *file;

      file = lto_elf_file_open (in_fnames[i]);
      if (!file)
	break;
      gcc_assert (file->debug_info.base.start);
      gcc_assert (file->debug_abbrev.base.start);
      if (!lto_file_read (file))
	break;
      lto_elf_file_close (file);
    }

  /* Let the middle end know that we have read and merged all of the
     input files.  */ 
  cgraph_finalize_compilation_unit ();
  cgraph_optimize ();
}
