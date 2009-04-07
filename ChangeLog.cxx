2009-03-04  Ian Lance Taylor  <iant@google.com>

	* Makefile.tpl (POSTSTAGE1_HOST_EXPORTS): Add -L option to CXX and
	CXX_FOR_BUILD to pick up previous libstdc++.  Set RPATH_ENVVAR to
	point to previous libstdc++.
	* Makefile.in: Rebuild.

2009-02-04  Ian Lance Taylor  <iant@google.com>

	* Makefile.def: Bootstrap the libstdc++-v3 target module.
	* Makefile.tpl (POSTSTAGE1_HOST_EXPORTS): Add CXX and
	CXX_FOR_BUILD.
	(POSTSTAGE1_FLAGS_TO_PASS): Likewise.
	* Makefile.in: Rebuild.

2009-01-30  Ian Lance Taylor  <iant@google.com>

	* configure.ac: Don't add c++ to missing_languages.  Don't use
	-fkeep-inline-functions.
	* configure: Rebuild.

2008-06-18  Ian Lance Taylor  <iant@google.com>

	* configure.ac: Build C++ in stage 1.
	* configure: Rebuild.

