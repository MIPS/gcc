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
#include "libelf.h"
#include "lto-tags.h"

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
  /* Offset of string table used for section names.  */
  size_t string_table_section_index;
};
typedef struct lto_elf_file lto_elf_file;

/* Forward Declarations */

static const void *
lto_elf_map_fn_body (lto_file *file, const char *fn);

static void
lto_elf_unmap_fn_body (lto_file *file, const char *fn, const void *data);

/* The vtable for ELF input files.  */
static const lto_file_vtable lto_elf_file_vtable = {
  lto_elf_map_fn_body,
  lto_elf_unmap_fn_body
};

/* A helper function to find the section named SECTION_NAME in ELF_FILE, and
   return its data.  Emits an appropriate error message and returns NULL
   if a unique section with that name is not found.  */

static Elf_Data *
lto_elf_find_section_data (lto_elf_file *elf_file, const char *section_name)
{
  Elf_Scn *section, *result;
  Elf_Data *data;
  size_t bits = elf_file->bits;

  result = NULL;
  for (section = elf_getscn (elf_file->elf, 0);
       section;
       section = elf_nextscn (elf_file->elf, section)) 
    {
      size_t offset;
      const char *name;

      if (!section)
	{
	  error ("could not read section information: %s", elf_errmsg (0));
	  return NULL;
	}

#define ELF_GET_SECTION_HEADER_NAME(N)					 \
      do {								 \
	Elf##N##_Shdr *section_header;					 \
	section_header = elf##N##_getshdr (section);			 \
	if (!section_header)						 \
	  {								 \
	    error ("could not read section header: %s", elf_errmsg (0)); \
	    return NULL;						 \
	  }								 \
	offset = section_header->sh_name;				 \
      } while (false)

      switch (bits)
	{
	case 32:
	  ELF_GET_SECTION_HEADER_NAME(32); 
	  break;
	case 64:
	  ELF_GET_SECTION_HEADER_NAME(64);
	  break;
	default:
	  gcc_unreachable ();
	}

#undef ELF_GET_SECTION_HEADER_NAME

      /* Get the name of this section.  */
      name = elf_strptr (elf_file->elf, elf_file->string_table_section_index, 
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
    {
      error ("missing %qs section", section_name);
      return NULL;
    }

  data = elf_getdata (result, NULL);
  if (!data)
    {
      error ("could not read data: %s", elf_errmsg (0));
      return NULL;
    }
  return data;
}

lto_file *
lto_elf_file_open (const char *filename)
{
  lto_elf_file *elf_file;
  size_t bits;
  const char *elf_ident;
  Elf_Scn *string_table_section;
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
  if (elf_getshstrndx (elf_file->elf, &elf_file->string_table_section_index) == -1)
    {
      error ("could not locate ELF string table: %s", elf_errmsg (0));
      goto fail;
    }
  string_table_section = elf_getscn (elf_file->elf, 
				     elf_file->string_table_section_index);

  /* Find the .debug_info and .debug_abbrev sections.  */
  data = lto_elf_find_section_data (elf_file, ".debug_info");
  if (!data)
    goto fail;
  fd = (lto_fd *) &result->debug_info;
  fd->start = (const char *) data->d_buf;
  fd->end = fd->start + data->d_size;

  data = lto_elf_find_section_data (elf_file, ".debug_abbrev");
  if (!data)
    goto fail;
  fd = (lto_fd *) &result->debug_abbrev;
  fd->start = (const char *) data->d_buf;
  fd->end = fd->start + data->d_size;

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

const void *
lto_elf_map_fn_body (lto_file *file,
		     const char *fn)
{
  /* Look in the ELF file to find the actual data, which should be
     in the section named LTO_SECTION_NAME_PREFIX || "the function name".  */
  const char *name = concat (LTO_SECTION_NAME_PREFIX, fn, NULL);
  Elf_Data *data = lto_elf_find_section_data ((lto_elf_file *)file, name);

  free ((void *)name);

  if (! data)
    return NULL;
  else
    return (const void *)(data->d_buf);
}

void
lto_elf_unmap_fn_body (lto_file *file ATTRIBUTE_UNUSED, 
		       const char *fn ATTRIBUTE_UNUSED, 
		       const void *data ATTRIBUTE_UNUSED)
{
  return;
}
