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
#include "tm.h"
#include "libiberty.h"
#include "ggc.h"


/* ### Moved lto_file_init and lto_file_close here from lto.c.
   The lto_file vs. lto_elf_file distinction now appears superfluous.  */

/* Initialize FILE, an LTO file object for FILENAME.  */
static void
lto_file_init (lto_file *file, const char *filename)
{
  file->filename = filename;
  /* ### We no longer use the debug_info and debug_abbrev fields. */
}

/* Close FILE.  */
static void
lto_file_close (lto_file *file ATTRIBUTE_UNUSED)
{
  /* ### We no longer use the debug_info and debug_abbrev fields. */
  ggc_free (file);
}

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
  /* Section number of string table used for section names.  */
  size_t sec_strtab;
};
typedef struct lto_elf_file lto_elf_file;


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


/* Returns a hash code for P.  */

static hashval_t
hash_name (const void *p)
{
  const struct lto_section_slot *ds = (const struct lto_section_slot *) p;
  return (hashval_t) htab_hash_string (ds->name);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_name (const void *p1, const void *p2)
{
  const struct lto_section_slot *s1 =
    (const struct lto_section_slot *) p1;
  const struct lto_section_slot *s2 =
    (const struct lto_section_slot *) p2;

  return strcmp (s1->name, s2->name) == 0;
}


/* Build a hash table whose key is the section names and whose data is
   the start and size of each section in the .o file.  

   FIXME!  This code will most likely require an upgrade when it comes
   time to try to read archive files.  This means that when we start
   randomly reading functions out of archives, we will need to load
   the member of the archive manually rather than just opening the
   archive as a file and lseeking to the start of the function.  */

htab_t
lto_elf_build_section_table (lto_file *lto_file) 
{
  lto_elf_file *elf_file = (lto_elf_file *)lto_file;
  htab_t section_hash_table;
  Elf_Scn *section;

  section_hash_table
    = htab_create (37, hash_name, eq_name, free);

  for (section = elf_getscn (elf_file->elf, 0);
       section;
       section = elf_nextscn (elf_file->elf, section)) 
    {
      Elf64_Shdr *shdr;
      const char *name;
      size_t offset;
      char *new_name;
      void **slot;
      struct lto_section_slot s_slot;

      /* Get the name of this section.  */
      shdr = lto_elf_get_shdr (elf_file, section);
      offset = shdr->sh_name;
      name = elf_strptr (elf_file->elf, 
			 elf_file->sec_strtab,
			 offset);

      /* Only put lto stuff into the symtab.  */
      if (strncmp (name, LTO_SECTION_NAME_PREFIX, 
		   strlen (LTO_SECTION_NAME_PREFIX)) != 0)
	{
	  lto_elf_free_shdr (elf_file, shdr);
	  continue;
	}

      new_name = xmalloc (strlen (name) + 1);
      strcpy (new_name, name);
      s_slot.name = new_name;
      slot = htab_find_slot (section_hash_table, &s_slot, INSERT);
      if (*slot == NULL)
	{
	  struct lto_section_slot *new_slot
	    = xmalloc (sizeof (struct lto_section_slot));

	  new_slot->name = new_name;
	  /* The offset into the file for this section.  */
	  new_slot->start = shdr->sh_offset;
	  new_slot->len = shdr->sh_size;
	  *slot = new_slot;
	}
      else
	{
	  error ("two or more sections for %s:", new_name);
	  return NULL;
	}
      lto_elf_free_shdr (elf_file, shdr);
    }
  return section_hash_table;
}


lto_file *
lto_elf_file_open (const char *filename)
{
  lto_elf_file *elf_file;
  size_t bits;
  const char *elf_ident;
  lto_file *result;

  /* Set up.  */
  elf_file = GGC_NEW (lto_elf_file);
  result = (lto_file *)elf_file;
  lto_file_init (result, filename);
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
