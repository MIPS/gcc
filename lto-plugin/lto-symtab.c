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



#include "plugin-api.h"

ld_plugin_claim_file_handler claim_file_handler;
void *current_file_handle;

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
  const char *kind_str[] = {"DEF", "WEAKDEF", "UNDEF",
			    "WEAKUNDEF", "COMMON"};
  const char *visibility_str[] = {"DEFAULT", "PROTECTED",
				  "INTERNAL", "HIDDEN"};
  const char *resolution_str[] = {"UNKNOWN", "UNDEF",
				  "PREVAILING_DEF",
				  "PREVAILING_DEF_IRONLY",
				  "PREEMPTED_REG",
				  "PREEMPTED_IR",
				  "RESOLVED_IR",
				  "RESOLVED_EXEC",
				  "RESOLVED_DYN"};

  int i;
  assert (handle == current_file_handle);

  for (i = 0; i < nsyms; i++)
    {
      printf("name: %s; ", syms[i].name);
      if (syms[i].version)
	printf("version: %s;", syms[i].version);
      else
	printf("not versioned; ");
      printf("kind: %s; ", kind_str[syms[i].def]);
      printf("visibility: %s; ", visibility_str[syms[i].visibility]);
      printf("size: %" PRId64 "; ", syms[i].size);
      if (syms[i].comdat_key)
	printf("comdat_key: %s; ", syms[i].comdat_key);
      else
	printf("no comdat_key; ");
      printf ("resolution: %s\n", resolution_str[syms[i].resolution]);
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

  {0, {0}}
};

/* Load a plugin from a file named NAME. */

static void
load_plugin (const char *name)
{
  ld_plugin_onload onload;
  void *lib = dlopen (name, RTLD_LAZY);

  assert (lib != NULL);
  onload = dlsym (lib, "onload");
  assert (onload);
  onload (tv);
  assert (claim_file_handler);
}

/* Send file named NAME to the plugin. */

static void
register_file (const char *name)
{
 int claimed;
 struct ld_plugin_input_file file;
 int fd = open (name, O_RDONLY);
 assert (fd >= 0);

 file.name = (char *) name;
 file.fd = fd;
 file.offset = 0; /* Used only in archives. */
 file.filesize = 0; /* Used only in archives. */

 /* Just to check that the plugin keeps it. */
 file.handle = &file;
 current_file_handle = file.handle;

 claim_file_handler (&file, &claimed);
}

int
main(int argc, char *argv[])
{
  const char *plugin;
  const char *obj;
  assert (argc == 3);
  plugin = argv[1];
  obj = argv[2];

  load_plugin (plugin);
  register_file (obj);

  return 0;
}
