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

extern const char *lto_kind_str[5] __attribute__ ((visibility ("hidden")));

extern const char *
lto_visibility_str[4] __attribute__ ((visibility ("hidden")));

extern const char *
lto_resolution_str[9] __attribute__ ((visibility ("hidden")));
