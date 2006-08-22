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
#include <inttypes.h>

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
  DW_cl_block = 0x2 ,
  DW_cl_constant = 0x4 ,
  DW_cl_flag = 0x8,
  DW_cl_lineptr = 0x10,
  DW_cl_loclistptr = 0x20,
  DW_cl_macptr = 0x40,
  DW_cl_rangelistptr = 0x80,
  DW_cl_reference = 0x100,
  DW_cl_string = 0x200
} DWARF2_class;

/* The data corresponding to a single attribute form in a DIE.  */
typedef struct DWARF2_form_data
{
  /* The class of data.  */
  DWARF2_class cl;
  union {
    /* If CL is DW_address, DW_lineptr, DW_loclistptr, DW_macptr, 
       or DW_rangelistptr, there is no additional data.  These forms
       are not of interest to us for link-time optimization.  */
    /* Used when CL is DW_cl_constant.  */
    uint64_t constant;
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
    const char *reference;
  } u;
} DWARF2_form_data;

/* Information passed from parent DIEs to child DIEs to give context
   about the current location in the scope tree.  */
typedef struct lto_context
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
  /* If the last DIE read (with lto_read_DIE) was a type, then this
     field is the type.  NULL otherwise.  */
  tree type;
} lto_context;

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

/* Forward Declarations */

static hashval_t
lto_cache_hash (const void *data);

static int
lto_cache_eq (const void *data, const void *key);

static bool
lto_read_DIE (lto_info_fd *fd,
	      lto_context *context);

static void
lto_read_child_DIEs (lto_info_fd *fd, 
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
lto_file_init (lto_file *file, const char *filename)
{
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
static void
lto_file_corrupt_error (const lto_fd *fd)
{
  fatal_error ("corrupt link-time optimization information in "
	       "%qs section", fd->name);
}

/* Issue an error indicating that the ABI used to compile the object
   file does not match that currently in use by the LTO front end.  */
static void
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

  result = 0;
  do
    {
      byte = lto_read_ubyte (fd);
      more = byte & 0x80;
      result = (result << 7) | (byte & ~0x80);
    }
  while (more);

  return result;
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
    DW_cl_error, /* ordering */
    DW_cl_error, /* subscr_data */
    DW_cl_block | DW_cl_constant | DW_cl_reference, /* byte_size */
    DW_cl_error, /* bit_offset */
    DW_cl_error, /* bit_size */
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
    DW_cl_error, /* const_value */
    DW_cl_error, /* containing_type */
    DW_cl_error, /* default_value */
    DW_cl_error, /* padding */
    DW_cl_error, /* inline */
    DW_cl_error, /* is_optional */
    DW_cl_error, /* lower_bound */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_string, /* producer */
    DW_cl_error, /* padding */
    DW_cl_error, /* prototyped */
    DW_cl_error, /* padding */
    DW_cl_error, /* padding */
    DW_cl_error, /* return_addr */
    DW_cl_error, /* padding */
    DW_cl_error, /* start_scope */
    DW_cl_error, /* padding */
    DW_cl_error, /* stride_size */
    DW_cl_error, /* upper_bound */
    DW_cl_error, /* padding */
    DW_cl_error, /* abstract_origin */
    DW_cl_error, /* accessibility */
    DW_cl_error, /* address_class */
    DW_cl_error, /* artificial */
    DW_cl_error, /* base_types */
    DW_cl_error, /* calling_convention */
    DW_cl_error, /* count */
    DW_cl_error, /* data_member_location */
    DW_cl_error, /* decl_column */
    DW_cl_constant, /* decl_file */
    DW_cl_constant, /* decl_line */
    DW_cl_flag, /* declaration */
    DW_cl_error, /* discr_list */
    DW_cl_constant, /* encoding */
    DW_cl_flag, /* external */
    DW_cl_error, /* frame_base */
    DW_cl_error, /* friend */
    DW_cl_error, /* identifier_case */
    DW_cl_error, /* macro_info */
    DW_cl_error, /* namelist_items */
    DW_cl_error, /* priority */
    DW_cl_error, /* segment */
    DW_cl_error, /* specification */
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
      out->cl = DW_cl_address;
      fd->dwarf64 ? lto_read_uword (fd) : lto_read_udword (fd);
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

    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
      {
	uint64_t data;
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
	    out->cl = DW_cl_constant;
	    out->u.constant = data;
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
	DWARF2_CompUnit *cu;

	/* The standard says 
	   "In the 32-bit DWARF format, this offset is a 4-byte unsigned
	   value; in the 64-bit DWARF format, it is an 8-byte unsigned
	   value"
	*/
	if (!fd->dwarf64)
	  offset = lto_read_uword (fd);
	else
	  offset = lto_read_udword (fd);
	
	cu = find_cu_for_offset (info_fd, offset);

	/* Swap context if necessary.  */
	if (cu != context->cu)
	  {
	    lto_context *new_context = XCNEW (lto_context);
	    
	    *new_context = *context;
	    
	    lto_set_cu_context (new_context, info_fd, cu);
	    *form_context = new_context;
	  }

	out->cl = DW_cl_reference;
	out->u.reference 
	  = ((*form_context)->cu_start
	     + lto_check_size_t_val (offset, "offset too large"));
	if (out->u.reference >= (*form_context)->cu_end)
	  lto_file_corrupt_error (fd);
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
	  = (context->cu_start 
	     + lto_check_size_t_val (offset, "offset too large"));
	if (out->u.reference >= context->cu_end)
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

/* DIE Cache

   Some DIEs (like those for types and declarations) may be referred
   to explicitly, rather than just included in the DWARF tree.  So as
   to avoid having to repeatedly process the same subtrees, we cache
   the trees assocaited with DIEs.  The DIE cache is implemented as a
   hash table, mapping in-memory DIE addresses to {TYPE,DECL}
   nodes.  */

typedef struct lto_die_cache_entry {
  /* The DIE address.  */
  const char *die;
  /* The tree corresponding to this DIE.  */
  tree val;
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
		     const char *die,
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
  *slot = entry;
}

/* If FD points to a DIE that has already been processed, returned the
   cached value.  */
static tree
lto_cache_lookup_DIE (lto_info_fd *fd, const char *die)
{
  lto_die_cache_entry *entry;

  entry = htab_find_with_hash (fd->die_cache, die, 
			       htab_hash_pointer (die));
  return entry ? entry->val : NULL_TREE;
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

      static void
      lto_read_tag_DIE (lto_info_fd *fd, 
                        const DWARF2_abbrev *abbrev,
		        lto_context *context)
      {
         gcc_assert (abbrev->tag == DW_TAG_<tag>);

         LTO_BEGIN_READ_ATTRS (fd, abbrev)
	   {
	   case DW_AT_attr1:
	     ... handle this attribute ...
	     ;

	   case DW_AT_attr2:
	     ... handle this attribute ...
	     ;
	   }   
	 LTO_END_READ_ATTRS ();  
	 
	 lto_read_child_DIES (fd, abbrev, context);
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

/* Return an IDENTIFIER_NODE for the string constant indicatec by
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
   node corresponding to the DIE.  */
static tree
lto_read_referenced_type_DIE (lto_info_fd *fd,
			      lto_context *context,
			      const char *reference)
{
  const char *saved_cur;
  tree type;

  /* Check that the reference is in range.  We use an assert, rather
     than calling lto_file_corrupt_error, because REFERENCE should be
     checked for validity when it is read from the file.  */
  gcc_assert (reference >= context->cu_start
	      && reference < context->cu_end);
  type = lto_cache_lookup_DIE (fd, reference);
  if (!type)
    {
      /* Move the file pointer to the referenced location.  */
      saved_cur = fd->base.cur;
      fd->base.cur = reference;
      /* Read the DIE, which we insist must be a type.  */
      lto_read_DIE (fd, context);
      /* Restore the file pointer.  */
      fd->base.cur = saved_cur;
      type = context->type;
      /* Clear CONTEXT->TYPE since we are just reading a reference to
	 a type, not the DWARF subtree for a type.  */
      context->type = NULL_TREE;
    }
  /* The DIE read should have been a type.  */
  if (!type || !TYPE_P (type))
    lto_file_corrupt_error ((lto_fd *)fd);

  return type;
}

static void
lto_read_compile_unit_DIE (lto_info_fd *fd, 
			   const DWARF2_abbrev *abbrev,
			   lto_context *context)
{
  gcc_assert (abbrev->tag == DW_TAG_compile_unit);

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_language:
      switch (attr_data.u.constant)
	{
	case DW_LANG_C:
	case DW_LANG_C89:
	case DW_LANG_C99:
	  break;

	default:
	  error ("unsupported language " HOST_WIDEST_INT_PRINT_UNSIGNED, 
		 attr_data.u.constant);
	  break;
	}
      break;

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
}

static void
lto_read_variable_formal_parameter_constant_DIE (lto_info_fd *fd,
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
      code = CONST_DECL;
      break;
    default:
      gcc_unreachable ();
      break;
    }
  decl = build_decl (code, name, type);
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
  /* Let the middle end know about the new entity.  */
  if (decl != error_mark_node)
    rest_of_decl_compilation (decl,
			      /*top_level=*/1,
			      /*at_end=*/0);

  lto_read_child_DIEs (fd, abbrev, context);
}

static void
lto_read_pointer_type_DIE (lto_info_fd *fd,
			   const DWARF2_abbrev *abbrev,
			   lto_context *context)
{
  tree pointed_to;
  tree type;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_type:
      pointed_to = lto_read_referenced_type_DIE (fd, 
						 context, 
						 attr_data.u.reference);
      break;

    case DW_AT_byte_size:
      if (attr_data.cl != DW_cl_constant
	  || attr_data.u.constant * BITS_PER_UNIT != POINTER_SIZE)
	lto_abi_mismatch_error ();
      break;
    }
  LTO_END_READ_ATTRS ();

  /* The DW_AT_type attribute is required.  */
  if (!pointed_to)
    lto_file_corrupt_error ((lto_fd *)fd);
  /* Build the pointer type.  */
  type = build_pointer_type (pointed_to);
  /* Record the type for our caller.  */
  context->type = type;

  lto_read_child_DIEs (fd, abbrev, context);
}

static void
lto_read_base_type_DIE (lto_info_fd *fd, 
			const DWARF2_abbrev *abbrev,
			lto_context *context)
{
  tree name;
  bool have_encoding;
  enum dwarf_type encoding;
  bool have_size;
  uint64_t size;
  tree type;

  name = NULL_TREE;
  have_encoding = false;
  encoding = DW_ATE_void;
  have_size = false;
  size = 0;
  type = NULL_TREE;

  LTO_BEGIN_READ_ATTRS ()
    {
    case DW_AT_name:
      name = lto_get_identifier (&attr_data);
      break;

    case DW_AT_encoding:
      have_encoding = true;
      encoding = attr_data.u.constant;
      break;

    case DW_AT_byte_size:
      have_size = true;
      switch (attr_data.cl)
	{
	case DW_cl_constant:
	  size = attr_data.u.constant;
	  break;
	default:
	  sorry ("dynamically sized types are not supported");
	  break;
	}
      break;

    case DW_AT_bit_size:
    case DW_AT_bit_offset:
      lto_unsupported_attr_error (abbrev, attr);
      break;
    }
  LTO_END_READ_ATTRS ();

  if (!name || !have_encoding || !have_size)
    lto_file_corrupt_error ((lto_fd *)fd);

  lto_read_child_DIEs (fd, abbrev, context);

  /* Build the type.  */
  switch (encoding)
    {
    case DW_ATE_signed:
    case DW_ATE_unsigned:
      {
	int bits;
	bits = (BITS_PER_UNIT 
		* lto_check_int_val (size,
				     "size of base type too large"));
	type = build_nonstandard_integer_type (bits,
					       encoding == DW_ATE_unsigned);
      }
      break;
    default:
      sorry ("unsupported base type encoding");
      break;
    }
  /* If this is a new type, declare it.  */
  if (!TYPE_NAME (type))
    {
      tree decl;
      decl = build_decl (TYPE_DECL, name, type);
      TYPE_NAME (type) = decl;
      /* Let the middle end know about the type declaration.  */
      rest_of_decl_compilation (decl, 
				/*top_level=*/1,
				/*at_end=*/0);
    }
 
  /* Record the type for our caller.  */
  context->type = type;
}

/* Read the next DIE from FD.  CONTEXT provides information about the
   current state of the compilation unit.  Returns true iff there was
   a real DIE present; false if the DIE was a null entry indicating
   the end of a list of sibling DIEs.  */
static bool
lto_read_DIE (lto_info_fd *fd, lto_context *context)
{
  typedef void (*DIE_reader_fnptr)(lto_info_fd * fd,
				   const DWARF2_abbrev *abbrev,
				   lto_context *context);
  /* Reader functions for the tags defined by DWARF 3.  */
  static const DIE_reader_fnptr readers[DW_TAG_shared_type + 1] = 
    {
      NULL, /* padding */
      NULL, /* array_type */
      NULL, /* class_type */
      NULL, /* entry_point */
      NULL, /* enumeration_type */
      lto_read_variable_formal_parameter_constant_DIE,
      NULL, /* padding */
      NULL, /* padding */
      NULL, /* imported_declaration */
      NULL, /* padding */
      NULL, /* label */
      NULL, /* lexical_block */
      NULL, /* padding */
      NULL, /* member */
      NULL, /* padding */
      lto_read_pointer_type_DIE,
      NULL, /* reference_type */
      lto_read_compile_unit_DIE,
      NULL, /* string_type */
      NULL, /* structure_type */
      NULL, /* padding */
      NULL, /* subroutine_type */
      NULL, /* typedef */
      NULL, /* union_type */
      NULL, /* unspecified_parameters */
      NULL, /* variant */
      NULL, /* common_block */
      NULL, /* common_inclusion */
      NULL, /* inheritance */
      NULL, /* inlined_subroutine */
      NULL, /* module */
      NULL, /* ptr_to_member_type */
      NULL, /* set_type */
      NULL, /* subrange_type */
      NULL, /* with_stmt */
      NULL, /* access_declaration */
      lto_read_base_type_DIE,
      NULL, /* catch_block */
      NULL, /* const_type */
      lto_read_variable_formal_parameter_constant_DIE,
      NULL, /* enumerator */
      NULL, /* file_type */
      NULL, /* friend */
      NULL, /* namelist */
      NULL, /* namelist_item */
      NULL, /* packed_type */
      NULL, /* subprogram */
      NULL, /* template_type_param */
      NULL, /* template_value_param */
      NULL, /* thrown_type */
      NULL, /* try_block */
      NULL, /* variant_part */
      lto_read_variable_formal_parameter_constant_DIE,
      NULL, /* volatile_type */
      NULL, /* dwarf_procedure */
      NULL, /* restrict_type */
      NULL, /* interface_type */
      NULL, /* namespace */
      NULL, /* imported_module */
      NULL, /* unspecified_type */
      NULL, /* partial_unit */
      NULL, /* imported_unit */
      NULL, /* padding */
      NULL, /* condition */
      NULL /* shared_type */
    };

  uint64_t index;
  const DWARF2_abbrev *abbrev;
  DIE_reader_fnptr reader;
  const char *die;
  tree val;
  bool skip;

  /* Record the location of the current DIE -- before we change the
     file pointer.  */
  die = ((lto_fd *)fd)->cur;
  /* This DIE is not yet known to be a type.  */
  context->type = NULL_TREE;
  /* Read the abbreviation index.  */
  index = lto_read_uleb128 ((lto_fd *)fd);
  /* Zero indicates a null entry.  */
  if (!index)
    return false; 
  /* Get the actual abbreviation entry.  */
  abbrev = lto_abbrev_lookup (&fd->base.file->debug_abbrev, index);
  /* Assume that we will need to skip over this DIE.  */
  skip = true;
  /* Check to see if this DIE has already been processed.  */
  val = lto_cache_lookup_DIE (fd, die);
  if (val)
    {
      if (TYPE_P (val))
	context->type = val;
    }
  else
    {
      /* Determine the DIE reader function.  */
      reader = NULL;
      if (abbrev->tag < sizeof (readers) / sizeof (DIE_reader_fnptr))
	reader = readers[abbrev->tag];
      if (reader)
	{
	  /* If there is a reader, use it.  */
	  reader (fd, abbrev, context);
	  /* If this DIE refers to a type, cache the value so that future
	     references to the type can be processed quickly.  */
	  if (context->type)
	    lto_cache_store_DIE (fd, die, context->type);
	  skip = false;
	}
      else
	/* Assume that all other tags matter, as we are otherwise at
	   risk of silently generating wrong code.  If a tag can be
	   safely ignored, it should be explicitly ignored above.  */
	error ("DWARF tag " HOST_WIDEST_INT_PRINT_UNSIGNED " not "
	       "supported by link-time optimization", abbrev->tag);
    }
  /* Skip over this DIE if it has not already been processed.  */
  if (skip)
    {
      /* Read and ignore all attributes.  */
      LTO_BEGIN_READ_ATTRS_UNCHECKED ()
	{
	}
      LTO_END_READ_ATTRS ();
      /* Read children.  */ 
      lto_read_child_DIEs (fd, abbrev, context);
    }

  return true;
}

/* Read the children of a DIE from FD, passing CONTEXT to the DIE readers
   for the children.  The attributes of the DIE have already been
   read.  ABBREV indicates whether or not the DIE actually has
   children; if it does not, then this function returns without
   reading anything.  Therefore, it is always safe to call this
   function from a DIE reader, after reading the DIE's attributes.  */
static void
lto_read_child_DIEs (lto_info_fd *fd, 
		     const DWARF2_abbrev *abbrev,
		     lto_context *context)
{
  if (abbrev->has_children)
    {
      bool more;
      do 
	more = lto_read_DIE (fd, context);
      while (more);
    }
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
      context.type = NULL_TREE;

      /* Read DIEs.  */
      while (fd->cur < context.cu_end)
	lto_read_DIE (&file->debug_info, &context);
    }

  return true;
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
