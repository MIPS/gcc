/* Data types used in the IL symbol table.
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

enum gcc_plugin_symbol_kind
  {
    GCCPK_DEF,
    GCCPK_WEAKDEF,
    GCCPK_UNDEF,
    GCCPK_WEAKUNDEF,
    GCCPK_COMMON
  };

enum gcc_plugin_symbol_visibility
  {
    GCCPV_DEFAULT,
    GCCPV_PROTECTED,
    GCCPV_INTERNAL,
    GCCPV_HIDDEN
  };
