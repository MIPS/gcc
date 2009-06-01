/* Plugin-side interface for high-level plugins in GCC.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef HIGHLEV_PLUGIN_H
#define HIGHLEV_PLUGIN_H

/* Callback type for high-level argument-less event callbacks */
typedef void (*event_callback_t) (void);

/* manipulation of event tables and callback lists */
extern void register_plugin_event (const char *name, event_callback_t func);
extern void unregister_plugin_event (const char *name);
extern int call_plugin_event (const char *event_name);
extern const char **list_plugin_events (void);

/* return codes for call_plugin_event */
#define PLUGEVT_SUCCESS         0
#define PLUGEVT_NO_EVENTS       1
#define PLUGEVT_NO_SUCH_EVENT   2
#define PLUGEVT_NO_CALLBACK     3

/* manipulation of event parameter (callback arg) tables */
extern const char **list_event_parameters (void);
extern void *get_event_parameter (const char *name);

/* pass management */
extern void run_pass (char *pass_name);
extern const char **list_passes (void);

#endif /* HIGHLEV_PLUGIN_H */
