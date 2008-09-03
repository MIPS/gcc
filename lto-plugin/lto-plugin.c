/* LTO plugin for gold.
   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Rafael Avila de Espindola (espindola@google.com).

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

/* The plugin has only one external function: onload. Gold passes it an array of
   function that the plugin uses to communicate back to gold.

   With the functions provided by gold, the plugin can be notified when
   gold first analyzes a file and pass a symbol table back to gold. The plugin
   is also notified when all symbols have been read and it is time to generate
   machine code for the necessary symbols.

   More information at http://gcc.gnu.org/wiki/whopr/driver.
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_GELF_H
# include <gelf.h>
#else
# if defined(HAVE_LIBELF_GELF_H)
#   include <libelf/gelf.h>
# else
#  error "gelf.h not available"
# endif
#endif

#include "plugin-api.h"

static ld_plugin_register_claim_file register_claim_file;
static ld_plugin_add_symbols add_symbols;

/* Parse an entry of the IL symbol table. The data to be parsed is pointed
   by P and the result is written in ENTRY. The slot number is stored in SLOT.
   Returns the address of the next entry. */

static char *
parse_table_entry (char *p, struct ld_plugin_symbol *entry, uint32_t *slot)
{
  unsigned char t;
  enum ld_plugin_symbol_kind translate_kind[] =
    {
      LDPK_DEF,
      LDPK_WEAKDEF,
      LDPK_UNDEF,
      LDPK_WEAKUNDEF,
      LDPK_COMMON
    };

  enum ld_plugin_symbol_visibility translate_visibility[] =
    {
      LDPV_DEFAULT,
      LDPV_PROTECTED,
      LDPV_INTERNAL,
      LDPV_HIDDEN
    };

  entry->name = p;
  while (*p)
    p++;
  p++;

  entry->version = NULL;

  entry->comdat_key = p;
  while (*p)
    p++;
  p++;

  if (strcmp (entry->comdat_key, "") == 0)
    entry->comdat_key = NULL;

  t = *p;
  assert (t <= 4);
  entry->def = translate_kind[t];
  p++;

  t = *p;
  assert (t <= 3);
  entry->visibility = translate_visibility[t];
  p++;

  entry->size = *(uint64_t *) p;
  p += 8;

  *slot = *(uint32_t *) p;
  p += 4;

  entry->resolution = LDPR_UNKNOWN;

  return p;
}

/* Returns the string table of file ELF. */

static const char *
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

static Elf_Scn *
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

/* Returns the IL symbol table of file ELF. */

static Elf_Data *
get_symtab (Elf *elf)
{
  Elf_Data *data = 0;
  Elf_Scn *section = get_section (elf, ".gnu.lto_.symtab");
  if (!section)
    return NULL;

  data = elf_getdata (section, data);
  assert (data);
  return data;
}

/* Translate the IL symbol tabel SYMTAB into an array of ld_plugin_symbol
   and return it. Write the array size in SIZE. */

static struct ld_plugin_symbol *
translate (Elf_Data *symtab, int *size)
{
  char *data = symtab->d_buf;
  char *end = data + symtab->d_size;
  struct ld_plugin_symbol *ret = NULL;
  int n = 0;
  while (data < end)
    {
      uint32_t slot;
      n++;
      ret = realloc (ret, n * sizeof (struct ld_plugin_symbol));
      assert (ret);
      data = parse_table_entry (data, &ret[n - 1], &slot);
    }
  *size = n;
  return ret;
}

/* Callback used by gold to check if the plugin will claim FILE. Writes
   the result in CLAIMED. */

static enum ld_plugin_status
claim_file_handler (const struct ld_plugin_input_file *file, int *claimed)
{
  enum ld_plugin_status status;
  Elf *elf = elf_begin (file->fd, ELF_C_READ, NULL);
  int num;
  Elf_Data *symtab;
  struct ld_plugin_symbol *syms;

  *claimed = 0;

  if (!elf)
    return LDPS_OK;

  symtab = get_symtab (elf);
  if (!symtab)
    return LDPS_OK;

  syms = translate (symtab, &num);

  status = add_symbols (file->handle, num, syms);
  assert (status == LDPS_OK);

  *claimed = 1;
  return LDPS_OK;
}

/* Called by gold after loading the plugin. TV is the transfer vector. */

void
onload (struct ld_plugin_tv *tv)
{
  struct ld_plugin_tv *p;
  enum ld_plugin_status status;

  unsigned version = elf_version (EV_CURRENT);
  assert (version != EV_NONE);

  p = tv;
  while (p->tv_tag)
    {
      switch (p->tv_tag)
	{
	case LDPT_REGISTER_CLAIM_FILE_HOOK:
	  register_claim_file = p->tv_u.tv_register_claim_file;
	  break;
	case LDPT_ADD_SYMBOLS:
	  add_symbols = p->tv_u.tv_add_symbols;
	  break;
	default:
	  break;
	}
      p++;
    }

  assert (register_claim_file);
  assert (add_symbols);
  status = register_claim_file (claim_file_handler);
  assert (status == LDPS_OK);
}
