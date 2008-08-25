/* Program to read the IL symbol table.
   Copyright (C) 1999, 2000, 2002, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Vladimir Makarov (vmakarov@cygnus.com).

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#include <fcntl.h>
#include <assert.h>
#ifdef HAVE_GELF_H
# include <gelf.h>
#else
# if defined(HAVE_LIBELF_GELF_H)
#   include <libelf/gelf.h>
# else
#  error "gelf.h not available"
# endif
#endif
#include <stdio.h>
#include <string.h>
#include "lto-symtab.h"

/* Return the string table of ELF. */

const char *
get_string_table (Elf *elf)
{
  GElf_Ehdr header;
  GElf_Ehdr *t = gelf_getehdr (elf, &header);
  assert (t == &header);
  Elf_Scn *section = elf_getscn (elf, header.e_shstrndx);
  assert (section);
  Elf_Data *data = 0;
  data = elf_getdata (section, data);
  assert (data);
  assert (data->d_buf);
  return (const char *) data->d_buf;
}

/* Return the section in ELF that is named NAME. */

Elf_Scn *
get_section (Elf *elf, const char *name)
{
  const char *string_table = get_string_table (elf);
  Elf_Scn *section = 0;
  while ((section = elf_nextscn(elf, section)) != 0)
    {
      GElf_Shdr shdr;
      GElf_Shdr *tshdr = gelf_getshdr (section, &shdr);
      const char *t;
      assert (tshdr == &shdr);
      t = string_table + shdr.sh_name;
      if (strcmp (t, name) == 0)
	return section;
    }
  return NULL;
}

/* Return the IL symbol table of ELF. */

Elf_Data *
get_symtab (Elf *elf)
{
  Elf_Scn *section = get_section (elf, ".gnu.lto_.symtab");
  assert (section);
  Elf_Data *data = 0;
  data = elf_getdata (section, data);
  assert (data);
  return data;
}

struct table_entry
{
  char *name;
  char *comdat;
  enum gcc_plugin_symbol_kind kind;
  enum gcc_plugin_symbol_visibility visibility;
  uint64_t size;
  uint32_t slot_num;
};

/* Parse an entry of the IL symbol table. The data to be parsed is pointed
   by P and the result is written in ENTRY. Returs the address of the next
   entry. */

char *parse_table_entry (char *p, struct table_entry *entry)
{
  entry->name = p;
  while (*p)
    p++;
  p++;

  entry->comdat = p;
  while (*p)
    p++;
  p++;

  entry->kind = *p;
  assert (entry->kind <= 4);
  p++;

  entry->visibility = *p;
  assert (entry->visibility <= 3);
  p++;

  entry->size = *(uint64_t *) p;
  p += 8;

  entry->slot_num = *(uint32_t *) p;
  p += 4;

  return p;
}

/* Print the symbol table SYMTAB to stdout. */

void printTable (Elf_Data *symtab)
{
  char *data = symtab->d_buf;
  char *end = data + symtab->d_size;
  while (data < end)
    {
      struct table_entry entry;
      const char *kind_str[] = {"GCCPK_DEF", "GCCPK_WEAKDEF", "GCCPK_UNDEF",
				"GCCPK_WEAKUNDEF", "GCCPK_COMMON"};
      const char *visibility_str[] = {"GCCPV_DEFAULT", "GCCPV_PROTECTED",
				      "GCCPV_INTERNAL", "GCCPV_HIDDEN"};
      data = parse_table_entry (data, &entry);

      printf("%s %s %s %s %lld %d\n", entry.name, entry.comdat,
	     kind_str[entry.kind], visibility_str[entry.visibility],
	     entry.size, entry.slot_num);
    }
}

int main (int argc,  char *argv[])
{
  unsigned version = elf_version(EV_CURRENT);
  assert (version != EV_NONE);
  assert (argc == 2);
  int fd = open (argv[1], O_RDONLY);
  assert (fd >= 0);
  Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
  assert (elf);

  Elf_Data *symtab = get_symtab (elf);
  printTable (symtab);

  elf_end(elf);
  return 0;
}
