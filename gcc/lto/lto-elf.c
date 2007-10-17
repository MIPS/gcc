/* LTO routines for ELF object files.
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
#include "toplev.h"
#include "lto.h"
#ifdef HAVE_LIBELF_H
# include <libelf.h>
#else
# if defined(HAVE_LIBELF_LIBELF_H)
#   include <libelf/libelf.h>
# else
#  error "libelf.h not available"
# endif
#endif
#include "lto-tags.h"
#include "tm.h"
#include "libiberty.h"

/* An ELF input file.  */
struct lto_elf_file 
{
  /* The base information.  */
  lto_file base;
  /* The system file descriptor for the file.  */
  int fd;
  /* The libelf descriptor for the file.  */
  Elf *elf;
  /* 32 or 64 bits? */
  size_t bits;
  /* Section number of section table.  */
  size_t strtab;
  /* Section number of string table used for section names.  */
  size_t sec_strtab;
  /* The ELF symbol table.  */
  Elf_Data *symtab;
};
typedef struct lto_elf_file lto_elf_file;

/* Forward Declarations */

static const void *
lto_elf_map_optional_lto_section (lto_file *file, const char *id);

static void
lto_elf_unmap_fn_body (lto_file *file, const char *fn, const void *data);

/* The vtable for ELF input files.  */
static const lto_file_vtable lto_elf_file_vtable = {
  lto_elf_map_optional_lto_section,
  lto_elf_unmap_fn_body,
  lto_elf_map_optional_lto_section,
  lto_elf_unmap_fn_body
};

/* Return the section header for SECTION.  The return value is never
   NULL.  Call lto_elf_free_shdr to release the memory allocated.  */
static Elf64_Shdr *
lto_elf_get_shdr (lto_elf_file *elf_file,
		  Elf_Scn *section)
{
  Elf64_Shdr *shdr;

  switch (elf_file->bits)
    {
    case 32:
      {
	Elf32_Shdr *shdr32;
	/* Read the 32-bit section header.  */
	shdr32 = elf32_getshdr (section);
	if (!shdr32)
	  fatal_error ("could not read section header: %s",
		       elf_errmsg (0));
	/* Transform it into a 64-bit section header.  */
	shdr = XNEW (Elf64_Shdr);
	shdr->sh_name = shdr32->sh_name;
	shdr->sh_type = shdr32->sh_type;
	shdr->sh_flags = shdr32->sh_flags;
	shdr->sh_addr = shdr32->sh_addr;
	shdr->sh_offset = shdr32->sh_offset;
	shdr->sh_size = shdr32->sh_size;
	shdr->sh_link = shdr32->sh_link;
	shdr->sh_info = shdr32->sh_info;
	shdr->sh_addralign = shdr32->sh_addralign;
	shdr->sh_entsize  = shdr32->sh_entsize;
	break;
      }
      break;
    case 64:
      shdr = elf64_getshdr (section);
      if (!shdr)
	fatal_error ("could not read section header: %s",
		     elf_errmsg (0));
      break;
    default:
      gcc_unreachable();
    }

  return shdr;
}

/* Free SHDR, previously allocated by lto_elf_get_shdr.  */
static void
lto_elf_free_shdr (lto_elf_file *elf_file, Elf64_Shdr *shdr)
{
  if (elf_file->bits != 64)
    free (shdr);
}

/* A helper function to find the section named SECTION_NAME in
   ELF_FILE, and return its data.  If we can't find a section by that
   name, return NULL, but don't report an error.  If anything else
   goes wrong, report an error and return NULL.  */

static Elf_Data *
lto_elf_find_section_data (lto_elf_file *elf_file, const char *section_name)
{
  Elf_Scn *section, *result;
  Elf_Data *data;

  result = NULL;
  for (section = elf_getscn (elf_file->elf, 0);
       section;
       section = elf_nextscn (elf_file->elf, section)) 
    {
      Elf64_Shdr *shdr;
      size_t offset;
      const char *name;

      /* Get the name of this section.  */
      shdr = lto_elf_get_shdr (elf_file, section);
      offset = shdr->sh_name;
      lto_elf_free_shdr (elf_file, shdr);
      name = elf_strptr (elf_file->elf, 
			 elf_file->sec_strtab,
			 offset);
      if (!name)
	{
	  error ("could not read section name: %s", elf_errmsg (0));
	  return NULL;
	}
      
      /* Check to see if this is the section of interest.  */
      if (strcmp (name, section_name) == 0)
	{
	  /* There should not be two debugging sections with the same
	     name.  */
	  if (result)
	    {
	      error ("duplicate %qs section", section_name);
	      return NULL;
	    }
	  result = section;
	}
    }
  if (! result)
    return NULL;

  data = elf_getdata (result, NULL);
  if (!data)
    {
      error ("could not read data: %s", elf_errmsg (0));
      return NULL;
    }
  return data;
}

/* Read the ELF symbol table from ELF_FILE.  */
static void
lto_elf_read_symtab (lto_elf_file *elf_file)
{
  Elf_Scn *section;
  Elf64_Shdr *shdr;

  /* Iterate over the section table until we find one with type
     SHT_SYMTAB.  */
  for (section = elf_getscn (elf_file->elf, 0);
       section;
       section = elf_nextscn (elf_file->elf, section))
    {
      shdr = lto_elf_get_shdr (elf_file, section);
      if (shdr->sh_type == SHT_SYMTAB)
	{
	  /* We have found the symbol table.  */
	  elf_file->symtab = elf_getdata (section, NULL);
	  elf_file->strtab = shdr->sh_link;
	}
      lto_elf_free_shdr (elf_file, shdr);
    }
}

/* Return the ELF symbol entry for NAME, or NULL if none.  The caller
   must call lto_elf_free_sym when done with the value returned.  */
static Elf64_Sym *
lto_elf_lookup_sym (lto_elf_file *elf_file,
		    const char *name)
{
  const char *first_sym;
  const char *last_sym;
  const char *ptr;
  size_t sym_size;
  Elf64_Sym *sym;

  gcc_assert (name);

  first_sym = (const char *) elf_file->symtab->d_buf;
  last_sym = first_sym + elf_file->symtab->d_size;
  sym_size = ((elf_file->bits == 32) 
	      ? sizeof (Elf32_Sym)
	      : sizeof (Elf64_Sym));
  for (ptr = first_sym; ptr < last_sym; ptr += sym_size)
    {
      Elf64_Word sym_name_offset;
      Elf32_Sym *sym32;
      const char *sym_name;

      /* Get the ELF symbol entry.  */
      if (elf_file->bits == 32)
	{
	  sym = NULL;
	  sym32 = (Elf32_Sym *) ptr;
	  sym_name_offset = sym32->st_name;
	}
      else
	{
	  sym = (Elf64_Sym *) ptr;
	  sym32 = NULL;
	  sym_name_offset = sym->st_name;
	}
      /* If the symbol has no name, it cannot be the one we want.  */
      if (!sym_name_offset)
	continue;
      /* Look up the symbol name.  */
      sym_name = elf_strptr (elf_file->elf,
			     elf_file->strtab,
			     sym_name_offset);
      /* Check to see if it is the one we want.  */
      if (strcmp (sym_name, name) != 0)
	continue;
      /* We have a match.  */
      if (elf_file->bits == 32)
	{
	  sym = XNEW (Elf64_Sym);
	  sym->st_name = sym32->st_name;
	  sym->st_value = sym32->st_value;
	  sym->st_size = sym32->st_size;
	  sym->st_info = sym32->st_info;
	  sym->st_other = sym32->st_other;
	  sym->st_shndx = sym32->st_shndx;
	}
      return sym;
    }

  return NULL;
}

/* Free SYM, previously allocated by lto_elf_lookup_sym.  */
static void
lto_elf_free_sym (lto_elf_file *elf_file, 
		  Elf64_Sym *sym)
{
  if (elf_file->bits == 32)
    free (sym);
}

lto_file *
lto_elf_file_open (const char *filename)
{
  lto_elf_file *elf_file;
  size_t bits;
  const char *elf_ident;
  Elf_Data *data;
  lto_file *result;
  lto_fd *fd;

  /* Set up.  */
  elf_file = XNEW (lto_elf_file);
  result = (lto_file *)elf_file;
  lto_file_init (result, &lto_elf_file_vtable, filename);
  elf_file->fd = -1;
  elf_file->elf = NULL;

  /* Open the file.  */
  elf_file->fd = open (filename, O_RDONLY);
  if (elf_file->fd == -1)
    {
      error ("could not open");
      goto fail;
    }

  /* Initialize the ELF library.  */
  if (elf_version (EV_CURRENT) == EV_NONE)
    {
      error ("ELF library is older than that used when building GCC");
      goto fail;
    }

  /* Open the ELF file descriptor.  */
  elf_file->elf = elf_begin (elf_file->fd, ELF_C_READ, NULL);
  if (!elf_file->elf)
    {
      error ("could not open ELF file: %s", elf_errmsg (0));
      goto fail;
    }

  /* Some aspects of the libelf API are dependent on whether the
     object file is a 32-bit or 64-bit file.  Determine which kind of
     file this is now.  */
  elf_ident = elf_getident (elf_file->elf, NULL);
  if (!elf_ident)
    {
      error ("could not read ELF identification information: %s",
	      elf_errmsg (0));
      goto fail;
	     
    }
  switch (elf_ident[EI_CLASS])
    {
    case ELFCLASS32:
      bits = 32;
      break;
    case ELFCLASS64:
      bits = 64;
      break;
    default:
      error ("unsupported ELF file class");
      goto fail;
    }
  elf_file->bits = bits;

  /* Check that the input file is a relocatable object file.  */
#define ELF_CHECK_FILE_TYPE(N)						\
  do {									\
    Elf##N##_Ehdr *elf_header;						\
    elf_header = elf##N##_getehdr (elf_file->elf);			\
    if (!elf_header)							\
      {									\
	error ("could not read ELF header: %s", elf_errmsg (0));	\
        goto fail;							\
      }									\
   if (elf_header->e_type != ET_REL)					\
     {									\
        error ("not a relocatable ELF object file");			\
        goto fail;							\
     }									\
  } while (false)

  switch (bits)
    {
    case 32:
      ELF_CHECK_FILE_TYPE (32);
      break;
    case 64:
      ELF_CHECK_FILE_TYPE (64);
      break;
    default:
      gcc_unreachable ();
    }

#undef ELF_CHECK_FILE_TYPE

  /* Read the string table used for section header names.  */
  if (elf_getshstrndx (elf_file->elf, &elf_file->sec_strtab) == -1)
    {
      error ("could not locate ELF string table: %s", elf_errmsg (0));
      goto fail;
    }
  /* Find the .debug_info and .debug_abbrev sections.  */
  data = lto_elf_find_section_data (elf_file, ".debug_info");
  if (!data)
    {
      error ("could not read %qs section: %s",
             ".debug_info", elf_errmsg (0));
      goto fail;
    }
  fd = (lto_fd *) &result->debug_info;
  fd->start = (const char *) data->d_buf;
  fd->end = fd->start + data->d_size;

  data = lto_elf_find_section_data (elf_file, ".debug_abbrev");
  if (!data)
    {
      error ("could not read %qs section: %s", 
             ".debug_abbrev", elf_errmsg (0));
      goto fail;
    }
  fd = (lto_fd *) &result->debug_abbrev;
  fd->start = (const char *) data->d_buf;
  fd->end = fd->start + data->d_size;

  /* Read the ELF symbol table.  */
  lto_elf_read_symtab (elf_file);

  return result;

 fail:
  lto_elf_file_close (result);
  return NULL;
}

void
lto_elf_file_close (lto_file *file)
{
  lto_elf_file *elf_file = (lto_elf_file *) file;
  if (elf_file->elf)
    elf_end (elf_file->elf);
  if (elf_file->fd != -1)
    close (elf_file->fd);
  lto_file_close (file);
}

static const void *
lto_elf_map_optional_lto_section (lto_file *file,
                                  const char *id)
{
  /* Look in the ELF file to find the actual data, which should be
     in the section named LTO_SECTION_NAME_PREFIX || "the function name".  */
  const char *name = concat (LTO_SECTION_NAME_PREFIX, id, NULL);
  Elf_Data *data = lto_elf_find_section_data ((lto_elf_file *)file, name);

  free ((void *)name);

  if (! data)
    return NULL;
  else
    return (const void *)(data->d_buf);
}

static void
lto_elf_unmap_fn_body (lto_file *file ATTRIBUTE_UNUSED, 
		       const char *fn ATTRIBUTE_UNUSED, 
		       const void *data ATTRIBUTE_UNUSED)
{
  return;
}

/* Build an initializer of the indicated TYPE from the DATA stored in
   the object file.  */
static tree
lto_elf_build_init (lto_elf_file *elf_file ATTRIBUTE_UNUSED,
		    tree type,
		    const uint8_t *data)
{
  tree init;

  init = error_mark_node;
  if (AGGREGATE_TYPE_P (type))
    sorry ("initializers for aggregate types");
  else if (TREE_CODE (type) == COMPLEX_TYPE)
    sorry ("initializers for complex types");
  else if (INTEGRAL_TYPE_P (type))
    {
      unsigned HOST_WIDE_INT low;
      unsigned HOST_WIDE_INT high;
      HOST_WIDE_INT size;
      int i;

      low = 0;
      high = 0;
      size = int_size_in_bytes (type);
      gcc_assert (size != -1);
      if ((size_t) size > sizeof (low))
	sorry ("multi-word integral initializers");
      /* Read the initializer, swapping bytes if necessary.  */
      for (i = 0; i < size; ++i)
	{
	  uint8_t byte;
	  byte = *data++;
	  if (!BYTES_BIG_ENDIAN)
	    low |= byte << (HOST_BITS_PER_CHAR * i);
	  else
	    low |= byte << (HOST_BITS_PER_CHAR * (size - 1 - i));
	}
      /* Create the INTEGER_CST.  */
      init = build_int_cst_wide_type (type, low, high);
    }
  else
    sorry ("initializers for other types");

  return init;
}

/* Read the initializer for VAR from FILE, reconsitituing it from the
   initializer and relocation information in the object file.  This
   function is not presently used; instead we are reading in
   serialized trees.  If this function remains unused, it should be
   removed before mering LTO into mainline.  */
static void
lto_elf_read_var_init (lto_file *file,
		       tree var)
{
  lto_elf_file *elf_file;
  const char *name;
  Elf64_Sym *sym;

  elf_file = (lto_elf_file *) file;
  /* Find the ELF symbol table entry for VAR.  */
  name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (var));
  sym = lto_elf_lookup_sym (elf_file, name);
  /* This is an initialized variable so it must be present in the ELF
     file.  */
  if (!sym)
    fatal_error ("ELF symbol table does not contain %qs", name);
  /* A VAR_DECL should always be an STT_OBJECT symbol.  */
  if (ELF64_ST_TYPE (sym->st_info) != STT_OBJECT)
    fatal_error ("ELF symbol for variable %qs is not %qs",
		 name, "STT_OBJECT");
  switch (sym->st_shndx)
    {
    case SHN_ABS:
      /* The input is a relocatable object so the symbol should not
	 have an absolute address.  */
      fatal_error ("ELF symbol for defined variable %qs is %qs",
		   name, "SHN_ABS");
      break;
    case SHN_UNDEF:
      /* This function is only called for variables with initializers
	 which must, therefore, be defined.  */
      fatal_error ("ELF symbol for defined variable %qs is %qs",
		   name, "SHN_UNDEF");
      break;
    case SHN_COMMON:
      /* The symbol is zero-initialized.  We do not need to explicitly
	 represent the initializer.  */
      break;
    case SHN_XINDEX:
      sorry ("support for SHN_XINDEX");
      break;
    default:
      {
	Elf_Scn *section;
	uint64_t offset;
	Elf_Data *data;
	tree init;

	/* The location of the data is given as a section-relative
	   offset.  */
	section = elf_getscn (elf_file->elf, sym->st_shndx);
	offset = (uint64_t) sym->st_value;
	data = elf_rawdata (section, NULL);
	init = lto_elf_build_init (elf_file,
				   TREE_TYPE (var),
				   (uint8_t *) data->d_buf + offset);
	DECL_INITIAL (var) = init;
      }
      break;
    }
  /* We're now done with the symbol.  */
  lto_elf_free_sym (elf_file, sym);

  return;
}
