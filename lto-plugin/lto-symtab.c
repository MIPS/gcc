/* Program to read the IL symbol table.
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

#include <fcntl.h>
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "plugin-api.h"
#include "common.h"

ld_plugin_claim_file_handler claim_file_handler;
ld_plugin_all_symbols_read_handler all_symbols_read_handler;
static void *plugin_handle;

struct file_handle {
  unsigned nsyms;
  struct ld_plugin_symbol *syms;
};

struct file_handle *all_file_handles = NULL;
unsigned int num_file_handles;

/* Write NSYMS symbols from file HANDLE in SYMS. */

static enum ld_plugin_status
get_symbols (void *handle, int nsyms, struct ld_plugin_symbol *syms)
{
  unsigned i;
  struct file_handle *h = (struct file_handle *) handle;
  assert (h->nsyms == nsyms);

  for (i = 0; i < nsyms; i++)
    syms[i] = h->syms[i];

  return LDPS_OK;
}

/* Register HANDLER as the callback for notifying the plugin that all symbols
   have been read. */

static enum ld_plugin_status
register_all_symbols_read (ld_plugin_all_symbols_read_handler handler)
{
  all_symbols_read_handler = handler;
  return LDPS_OK;
}

/* Register HANDLER as the callback for claiming a file. */

enum ld_plugin_status
register_claim_file(ld_plugin_claim_file_handler handler)
{
  claim_file_handler = handler;
  return LDPS_OK;
}

/* For a file identified by HANDLE, add NSYMS symbols from SYMS. */

static enum ld_plugin_status
add_symbols (const void *handle, int nsyms,
	     const struct ld_plugin_symbol *syms)
{
  int i;
  struct file_handle *h = (struct file_handle *) handle;
  h->nsyms = nsyms;
  h->syms = calloc (nsyms, sizeof (struct ld_plugin_symbol));
  assert (h->syms);

  for (i = 0; i < nsyms; i++)
    {
      h->syms[i] = syms[i];
      h->syms[i].name = strdup (h->syms[i].name);
      if (h->syms[i].version)
	h->syms[i].version = strdup (h->syms[i].version);
      if (h->syms[i].comdat_key)
	h->syms[i].comdat_key = strdup (h->syms[i].comdat_key);
    }

  return LDPS_OK;
}

struct ld_plugin_tv tv[] = {
  {LDPT_REGISTER_CLAIM_FILE_HOOK,
   {.tv_register_claim_file = register_claim_file}
  },
  {LDPT_ADD_SYMBOLS,
   {.tv_add_symbols = add_symbols}
  },

  {LDPT_REGISTER_ALL_SYMBOLS_READ_HOOK,
   {.tv_register_all_symbols_read = register_all_symbols_read}
  },
  {LDPT_GET_SYMBOLS,
   {.tv_get_symbols = get_symbols}
  },

  {0, {0}}
};

/* Load a plugin from a file named NAME. */

static void
load_plugin (const char *name)
{
  ld_plugin_onload onload;
  plugin_handle = dlopen (name, RTLD_LAZY);

  assert (plugin_handle != NULL);
  onload = dlsym (plugin_handle, "onload");
  assert (onload);
  onload (tv);
  assert (claim_file_handler);
}

/* Send file named NAME to the plugin. */

static void
register_file (const char *name, struct file_handle *handle)
{
 int claimed;
 struct ld_plugin_input_file file;
 int fd = open (name, O_RDONLY);
 assert (fd >= 0);

 file.name = (char *) name;
 file.fd = fd;
 file.offset = 0; /* Used only in archives. */
 file.filesize = 0; /* Used only in archives. */

 file.handle = handle;

 claim_file_handler (&file, &claimed);
}

/* Fake symbol resolution for testing. */

static void
resolve (void)
{
  unsigned j;
  for (j = 0; j < num_file_handles; j++)
    {
      unsigned int nsyms = all_file_handles[j].nsyms;
      struct ld_plugin_symbol *syms = all_file_handles[j].syms;
      unsigned i;
      for (i = 0; i < nsyms; i++)
	{
	  switch (syms[i].def)
	    {
	    case LDPK_DEF:
	    case LDPK_WEAKDEF:
	    case LDPK_COMMON:
	      syms[i].resolution =  LDPR_PREVAILING_DEF;
	      break;
	    case LDPK_UNDEF:
	    case LDPK_WEAKUNDEF:
	      syms[i].resolution =  LDPR_RESOLVED_IR;
	      break;
	    }
	}
    }
}

/* Print all symbol information. */

static void
print (void)
{
  unsigned j;
  for (j = 0; j < num_file_handles; j++)
    {
      unsigned int nsyms = all_file_handles[j].nsyms;
      struct ld_plugin_symbol *syms = all_file_handles[j].syms;
      unsigned i;
      for (i = 0; i < nsyms; i++)
	{
	  printf("name: %s; ", syms[i].name);
	  if (syms[i].version)
	     printf("version: %s;", syms[i].version);
	  else
	    printf("not versioned; ");
	  printf("kind: %s; ", lto_kind_str[syms[i].def]);
	  printf("visibility: %s; ", lto_visibility_str[syms[i].visibility]);
	  printf("size: %" PRId64 "; ", syms[i].size);
	  if (syms[i].comdat_key)
	    printf("comdat_key: %s; ", syms[i].comdat_key);
	  else
	    printf("no comdat_key; ");
	  printf ("resolution: %s\n", lto_resolution_str[syms[i].resolution]);
	}
    }
}

/* Unload the plugin. */

static void
unload_plugin (void)
{
  unsigned err = dlclose (plugin_handle);
  assert (err == 0);
  claim_file_handler = 0;
  all_symbols_read_handler = 0;
}

/* Free all memory allocated by us that hasn't been freed yet. */

static void
free_all (void)
{
  unsigned j;
  for (j = 0; j < num_file_handles; j++)
    {
      unsigned int nsyms = all_file_handles[j].nsyms;
      struct ld_plugin_symbol *syms = all_file_handles[j].syms;
      unsigned i;
      for (i = 0; i < nsyms; i++)
	{
	  free (syms[i].name);
	  syms[i].name = 0;
	  if (syms[i].version)
	    {
	      free (syms[i].version);
	      syms[i].version = 0;
	    }
	  if (syms[i].comdat_key)
	    {
	      free (syms[i].comdat_key);
	      syms[i].comdat_key = 0;
	    }
	}
      free (syms);
      all_file_handles[j].syms = NULL;
      all_file_handles[j].nsyms = 0;
    }

  free (all_file_handles);
  all_file_handles = NULL;
  num_file_handles = 0;
}

int
main(int argc, char *argv[])
{
  const char *plugin;
  unsigned int i;
  assert (argc >= 3);
  plugin = argv[1];

  load_plugin (plugin);

  num_file_handles = argc - 2;
  all_file_handles = calloc (num_file_handles, sizeof (struct file_handle));
  assert (all_file_handles);
  for (i = 2; i < argc; i++)
    register_file (argv[i], &all_file_handles [i - 2]);

  resolve ();

  print ();

  all_symbols_read_handler ();

  free_all ();

  unload_plugin ();

  return 0;
}
