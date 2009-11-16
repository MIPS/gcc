/* Interface for high-level plugins in GCC - Parts common between GCC,
   ICI and high-level plugins.

   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

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

#ifndef HIGHLEV_PLUGIN_COMMON_H
#define HIGHLEV_PLUGIN_COMMON_H

/* Return codes for invoke_plugin_{,va_}callbacks / call_plugin_event .  */
#define PLUGEVT_SUCCESS         0
#define PLUGEVT_NO_EVENTS       1
#define PLUGEVT_NO_SUCH_EVENT   2
#define PLUGEVT_NO_CALLBACK     3

/* Datatype of event parameter structure.  */
typedef enum
{
  EP_SILENT, /* Used to pass infomation between ICI and plugin,
		parameter of this type will not be recorded.
		The data type is int.  */
  EP_VOID,
  EP_CHAR,
  EP_UNSIGNED_CHAR,
  EP_INT,
  EP_UNSIGNED,
  EP_LONG,
  EP_UNSIGNED_LONG
} event_parameter_type;

#endif /* HIGHLEV_PLUGIN_COMMON_H */
