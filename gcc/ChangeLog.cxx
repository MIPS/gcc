2009-05-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/rs6000/rs6000.c (legitimate_indirect_address_p):
	Don't define inline.

	* opt-gen.awk: For all variables with an Init clause, set var_seen.

	* config.gcc (spu-*-elf*): Use spu-cxx.o for cxx_target_objs.
	* config/spu/spu-protos.h (struct cpp_reader): Forward declaration.
	(spu_expand_epilogue) Use bool.
	* config/spu/spu-c.c [CC1PLUS]: Include cp/cp-tree.h instead of
	c-tree.h.
	[!CC1PLUS]: Define same_type_p.
	(spu_resolve_overloaded_builtin): Use same_type_p.
	* config/spu/t-spu-elf (spu-c.o): Depend on $(C_TREE_H).
	(spu-cxx.o): New rule.
	* config/spu/spu.c (spu_scalar_mode_supported_p): Declare with bool.
	(spu_vector_mode_supported_p, spu_handle_fndecl_attribute): Likewise.
	(spu_handle_vector_attribute, spu_pass_by_reference): Likewise.
	(spu_rtx_costs, spu_function_ok_for_sibcall): Likewise.

2009-05-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* var-tracking.c: Include tm_p.h .
