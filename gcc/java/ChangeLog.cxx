2008-07-04  Tom Tromey  <tromey@redhat.com>

	* mangle.c (emit_compression_string): Insert newline.
	* builtins.c (java_builtins): Cast initializers.
	* Make-lang.in (jvspec.o): Use ALL_CXXFLAGS.
	($(GCJ)$(exeext)): Likewise.
	(jc1$(exeext)): Likewise.
	(jcf-dump$(exeext)): Likewise.
	(jvgenmain$(exeext)): Likewise.
	(java/jcf-io.o): Likewise.
	(java/jcf-path.o): Likewise.

2008-07-03  Tom Tromey  <tromey@redhat.com>

	* verify.h: Don't use 'extern "C"'.
	* jcf-parse.c (handle_constant): Cast to cpool_tag.
	* verify-impl.c (check_class_constant): Initialize with void_type,
	not 0.
	(check_constant): Likewise.
	(check_wide_constant): Likewise.
	(verify_instructions_0) <op_newarray>: Cast 'atype' to type_val.
	* Make-lang.in (jvspec.o): Use $(CXX).
	($(GCJ)$(exeext)): Likewise.
	(jc1$(exeext)): Likewise.
	(jcf-dump$(exeext)): Likewise.
	(jvgenmain$(exeext)): Likewise.
	(java/jcf-io.o): Likewise.
	(java/jcf-path.o): Likewise.

Local Variables:
mode: change-log
change-log-default-name: "ChangeLog.cxx"
End:
