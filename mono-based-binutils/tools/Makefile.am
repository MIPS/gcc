

EXTRA_DIST = Mono.Cecil
CLEANFILES = Mono.Cecil.dll gcc4cli.utils.dll ar.exe ld.exe nm.exe objdump.exe objcopy.exe

lbin_SCRIPTS = Mono.Cecil.dll gcc4cli.utils.dll ar.exe ld.exe nm.exe objdump.exe objcopy.exe

install-lbinSCRIPTS: $(lbin_SCRIPTS)
	test -z "$(lbindir)" || $(mkdir_p) "$(DESTDIR)$(lbindir)"
	@list='$(lbin_SCRIPTS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  if test -f $$d$$p; then \
	    echo " $(lbinSCRIPT_INSTALL) '$$d$$p' '$(DESTDIR)$(lbindir)/$$p'"; \
	    $(lbinSCRIPT_INSTALL) "$$d$$p" "$(DESTDIR)$(lbindir)/$$p"; \
	  else :; fi; \
	done

uninstall-lbinSCRIPTS:
	@list='$(lbin_SCRIPTS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(lbindir)/$$p'"; \
	  rm -f "$(DESTDIR)$(lbindir)/$$p"; \
	done


MONOCECIL = $(shell cat $(srcdir)/Mono.Cecil/Mono.Cecil.dll.sources)
CECIL_SRC = $(MONOCECIL:%=$(srcdir)/Mono.Cecil/%)

Mono.Cecil.dll: $(CECIL_SRC)
	  $(MCS) -t:library -out:Mono.Cecil.dll $(CECIL_SRC)

UTILS_SRC = \
	gcc4cli/utils/AssemblyResolver.cs \
	gcc4cli/utils/CDynamicLibrary.cs \
	gcc4cli/utils/CExecutable.cs \
	gcc4cli/utils/CFile.cs \
	gcc4cli/utils/CFileAttributes.cs \
	gcc4cli/utils/CFileBuilder.cs \
	gcc4cli/utils/CFileBuilderWithRuntime.cs \
	gcc4cli/utils/CFileException.cs \
	gcc4cli/utils/CFileFactory.cs \
	gcc4cli/utils/CObject.cs \
	gcc4cli/utils/CStaticLibrary.cs \
	gcc4cli/utils/CodeFixer.cs \
	gcc4cli/utils/NotFoundException.cs \
	gcc4cli/utils/RedefinitionException.cs \
	gcc4cli/utils/TypeMismatchException.cs \
	gcc4cli/utils/WrongFileFormatException.cs \
	gcc4cli/utils/ICFileBuilder.cs \
	gcc4cli/utils/IReferenceGetters.cs \
	gcc4cli/utils/ReferenceGetters.cs \
	gcc4cli/utils/ReferenceGettersCached.cs \
	gcc4cli/utils/ReferencesFixer.cs \
	gcc4cli/utils/Set.cs \
	gcc4cli/utils/Tracer.cs

LD_SRC = \
	gcc4cli/ld/Driver.cs

NM_SRC = \
	gcc4cli/nm/Driver.cs

AR_SRC = \
	gcc4cli/ar/Driver.cs

OBJDUMP_SRC = \
	gcc4cli/objdump/CilHelper.cs \
	gcc4cli/objdump/CilSymbols.cs \
	gcc4cli/objdump/CilWriter.cs \
	gcc4cli/objdump/Driver.cs \
	gcc4cli/objdump/Disassembler.cs \
	gcc4cli/objdump/MethodBodyDisassembler.cs

OBJCOPY_SRC = \
	gcc4cli/objcopy/Driver.cs

EXTRA_DIST += $(UTILS_SRC) $(LD_SRC) $(NM_SRC) $(AR_SRC) $(OBJDUMP_SRC) $(OBJCOPY_SRC)

UTILS_FULL_SRC = $(UTILS_SRC:%=$(srcdir)/%)
LD_FULL_SRC = $(LD_SRC:%=$(srcdir)/%)
NM_FULL_SRC = $(NM_SRC:%=$(srcdir)/%)
AR_FULL_SRC = $(AR_SRC:%=$(srcdir)/%)
OBJDUMP_FULL_SRC = $(OBJDUMP_SRC:%=$(srcdir)/%)
OBJCOPY_FULL_SRC = $(OBJCOPY_SRC:%=$(srcdir)/%)

gcc4cli.utils.dll: $(UTILS_FULL_SRC) Mono.Cecil.dll
	  $(MCS) -t:library -r:Mono.Cecil.dll -out:gcc4cli.utils.dll $(UTILS_FULL_SRC)

ld.exe: $(LD_FULL_SRC) Mono.Cecil.dll gcc4cli.utils.dll
	$(MCS) -r:Mono.Cecil.dll -r:gcc4cli.utils.dll -out:ld.exe $(LD_FULL_SRC)

nm.exe: $(NM_FULL_SRC) Mono.Cecil.dll gcc4cli.utils.dll
	$(MCS) -r:Mono.Cecil.dll -r:gcc4cli.utils.dll -out:nm.exe $(NM_FULL_SRC)

ar.exe: $(AR_FULL_SRC) Mono.Cecil.dll gcc4cli.utils.dll
	$(MCS) -r:Mono.Cecil.dll -r:gcc4cli.utils.dll -out:ar.exe $(AR_FULL_SRC)

objdump.exe: $(OBJDUMP_FULL_SRC) Mono.Cecil.dll gcc4cli.utils.dll
	$(MCS) -r:Mono.Cecil.dll -r:gcc4cli.utils.dll -out:objdump.exe $(OBJDUMP_FULL_SRC)

objcopy.exe: $(OBJCOPY_FULL_SRC) Mono.Cecil.dll gcc4cli.utils.dll
	$(MCS) -r:Mono.Cecil.dll -r:gcc4cli.utils.dll -out:objcopy.exe $(OBJCOPY_FULL_SRC)
