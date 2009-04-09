/* Header file for internal GCC plugin mechanism.
   Copyright (C) 2009 Free Software Foundation, Inc.

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

#ifndef PLUGIN_H
#define PLUGIN_H

#include "gcc-plugin.h"

/* Called when -fplugin=/path/to/NAME.so option is parsed.  */
extern void add_new_plugin (const char* plugin_name);
/* Parse the -fplugin-arg-<name>-<key>[=<value>] option.  */
extern void parse_plugin_arg_opt (const char *arg);
/* Invoke all plug-in callbacks registered with the specified event.  */
extern void invoke_plugin_callbacks (enum plugin_event event, void *gcc_data);
/* Main plugin initialization function.  */
extern void initialize_plugins (void);
/* Print the version of each plugin. */
extern void print_plugins_versions (FILE *file, const char *indent);
/* Print help for each plugin. */
extern void print_plugins_help (FILE *file, const char *indent);
/* Free memory allocated by the plugin system. */
extern void finalize_plugins (void);


#endif /* PLUGIN_H */
