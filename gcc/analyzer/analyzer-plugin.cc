/* Top-level interface to the analyzer, as a GCC plugin.
   Copyright (C) 2019 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "gcc-plugin.h"
#include "system.h"
#include "coretypes.h"
#include "analyzer/analyzer.h"
#include "analyzer/analyzer-selftests.h"

int plugin_is_GPL_compatible;

#if CHECKING_P

namespace selftest {

/* Callback for running the analyzer's selftests.  */

static void
analyzer_selftests_cb (void */*gcc_data*/, void */*user_data*/)
{
  run_analyzer_selftests ();
}

} /* end of namespace selftest.  */

#endif /* #if CHECKING_P */

/* Entrypoint to the pass.  */

int
plugin_init (struct plugin_name_args *plugin_info ATTRIBUTE_UNUSED,
	     struct plugin_gcc_version */*version*/)
{
  /* Register our selftests.  */
#if CHECKING_P
  register_callback (plugin_info->base_name,
		     PLUGIN_RUN_SELFTESTS,
		     selftest::analyzer_selftests_cb, NULL);
#endif /* #if CHECKING_P */

  /* Register our pass.  */
  register_analyzer_pass ();

  return 0;
}
