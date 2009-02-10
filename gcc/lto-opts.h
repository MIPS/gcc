/* LTO IL options.

   Copyright 2009 Free Software Foundation, Inc.
   Contributed by Simon Baldwin <simonb@google.com>

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

#include "lto-section-in.h"

extern void lto_register_user_option (size_t, const char *, int, int);
extern void lto_read_file_options (struct lto_file_decl_data *);
extern void lto_write_options (void);
extern void lto_reissue_options (void);
void lto_clear_user_options (void);
void lto_clear_file_options (void);
