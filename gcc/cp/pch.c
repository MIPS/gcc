/* Precompiled header implementation for C++
   Copyright (C) 2000 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "intl.h"
#include "tree.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "toplev.h"
#include "cpplib.h"
#include "defaults.h"
#include "ggc.h"
#include "tm_p.h"

void
lang_write_pch (name)
     const char *name;
{
}

int
lang_valid_pch (pfile, name, fd)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
     const char *name;
     int fd;
{
  return 2;
}

void
lang_read_pch (pfile, fd)
     cpp_reader *pfile;
     int fd;
{
  abort ();
}
