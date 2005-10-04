// Header glue for interfacing with gcc.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#ifndef GCC_JAVA_GLUE_HH
#define GCC_JAVA_GLUE_HH

// Note that this header must be included before any header specific
// to this front end.  See the '#undef's below to understand why.
// Also, all gcc includes must be included by this file, and before we
// start undefining preprocessor macros.

#include <config.h>

// A few things redefined in the gcjx config header.
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

// Include this early to get the same libstc++ flags as gcjx.
#include <gcjx-config.h>

// We include this before we include other GCC headers, so that we can
// avoid bad interactions between GCC's identifier poisoning and
// libstdc++'s use of those same identifiers.
#include <iostream>

#define IN_GCC

extern "C"
{
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-iterator.h"
#include "real.h"
#include "cgraph.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "options.h"
#include "output.h"
#include "toplev.h"
#include "except.h"
#include "flags.h"
#include "tree-inline.h"
#include "opts.h"
#include "libfuncs.h"
#include "convert.h"
#include "ggc.h"
#include "debug.h"
#include "version.h"
#include "target.h"
#include "rtl.h"
#include "mkdeps.h"

// This gets us alloc_stmt_list().  Shouldn't that be in
// tree-iterator.h?
#include "tree-gimple.h"
#include "tree-dump.h"
#include "function.h"
}

// gcc's system.h defines these unconditionally, but they make life
// difficult for us.
#undef bool
#undef true
#undef false

// This is defined in tree.h.  It is ok for us to undef it since the
// define is just an optimization.  But FIXME anyway.
#undef get_identifier

// libiberty lameness.
#undef hex_value

// Now include things from gcjx.
#include "typedefs.hh"
#include "aot/aotclass.hh"
#include "aot/aotfactory.hh"
#include "java/builtins.hh"
#include "java/hooks.hh"
#include "java/classobj.hh"

#endif // GCC_JAVA_GLUE_HH
