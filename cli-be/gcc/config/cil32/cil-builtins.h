/*

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#ifndef CIL_BUILTINS_H
#define CIL_BUILTINS_H

#include "coretypes.h"

enum cil32_builtin {
#define DEF_CILBUILTIN(bid, name, ret_type, ...) \
    CIL32_##bid,
#  include "cil-builtins.def"
#undef DEF_CILBUILTIN
  CIL32_MAX_BUILT_IN
};

extern GTY(()) tree cil32_builtins[CIL32_MAX_BUILT_IN];

extern GTY(()) tree cil32_va_list_type;
extern GTY(()) tree cil32_arg_iterator_type;

extern void cil_init_builtins (void);
extern tree cil_build_builtin_va_list (void);
extern tree cil_get_builtin_complex_real_fld (tree);
extern tree cil_get_builtin_complex_imag_fld (tree);
extern void cil_record_builtin_type (tree);
extern bool cil_builtin_type_p (tree);

#endif /* !CIL_BUILTINS_H */
