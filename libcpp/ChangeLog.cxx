2009-01-30  Ian Lance Taylor  <iant@google.com>

	* include/symtab.h: Use extern "C" for C++.
	* include/line-map.h: Likewise.
	* include/internal.h: Likewise.

2008-07-05  Tom Tromey  <tromey@redhat.com>

	* include/mkdeps.h: Use 'extern "C"'.

2008-07-03  Tom Tromey  <tromey@redhat.com>

	* include/symtab.h (cpp_hash_table): Rename from hash_table.
	* symtab.c, init.c, identifiers.c, internal.h: Update all users.

2008-06-21  Tom Tromey  <tromey@redhat.com>

	* include/cpplib.h (enum cpp_builtin_type): Rename from
	builtin_type.
	(union _cpp_hashnode_value) <builtin>: Update.
	* init.c (cpp_init_special_builtins): Update.

Local Variables:
mode: change-log
change-log-default-name: "ChangeLog.cxx"
End:
