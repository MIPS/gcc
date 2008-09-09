
CIL_TOOLS = \
        ar \
        as \
        ld \
        nm \
        objcopy \
        objdump \
        ranlib \
        size \
        strip

CIL_VM = \
	ilrun \
	mono

EXTRA_DIST = ${CIL_TOOLS:%=%.in} ${CIL_VM}
CLEANFILES = ${CIL_TOOLS} ${CIL_TOOLS:%=cil32-%} ${CIL_VM:%=cil32-%} sedscript cil32sedscript

bindir=$(prefix)/bin
includedir=$(prefix)/include
libdir=$(prefix)/lib
cil32bindir=$(prefix)/cil32/bin

bin_SCRIPTS = ${CIL_TOOLS:%=cil32-%} ${CIL_VM:%=cil32-%}

cil32bin_SCRIPTS = ${CIL_TOOLS} 


sedscript:
	echo 's?@CILRUN@?$(MONO)?'  >  $@
	echo 's?@DPREFIX@?../..?'   >> $@
	echo 's?@ILASM@?$(CIL_AS)?' >> $@

cil32sedscript:
	echo 's?@CILRUN@?$(MONO)?'  >  $@
	echo 's?@DPREFIX@?..?'      >> $@
	echo 's?@ILASM@?$(CIL_AS)?' >> $@

%: %.in sedscript
	cat $< | sed -f sedscript > $@

cil32-%: %.in cil32sedscript
	cat $< | sed -f cil32sedscript > $@

cil32-%: %
	cat $< | sed 's?@CILRUN@?$(MONO)?' > $@

installdirs:
	@$(mkinstalldirs) $(DESTDIR)$(bindir)
	@$(mkinstalldirs) $(DESTDIR)$(includedir)
	@$(mkinstalldirs) $(DESTDIR)$(libdir)
	@$(mkinstalldirs) $(DESTDIR)$(cil32bindir);
	@rm -f $(DESTDIR)$(prefix)/cil32/include; (cd $(DESTDIR)$(prefix)/cil32 && $(LN_S) ../include include)
	@rm -f $(DESTDIR)$(prefix)/cil32/lib;     (cd $(DESTDIR)$(prefix)/cil32 && $(LN_S) ../lib lib)

install-binSCRIPTS: $(bin_SCRIPTS) installdirs
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(binSCRIPT_INSTALL) '$$p' '$(DESTDIR)$(bindir)/$$p'"; \
	    $(binSCRIPT_INSTALL) "$$p" "$(DESTDIR)$(bindir)/$$p"; \
	  else :; fi; \
	done

uninstall-binSCRIPTS:
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(bindir)/$$p'"; \
	  rm -f "$(DESTDIR)$(bindir)/$$p"; \
	done

install-cil32binSCRIPTS: $(cil32bin_SCRIPTS) installdirs
	@list='$(cil32bin_SCRIPTS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(cil32binSCRIPT_INSTALL) '$$p' '$(DESTDIR)$(cil32bindir)/$$p'"; \
	    $(cil32binSCRIPT_INSTALL) "$$p" "$(DESTDIR)$(cil32bindir)/$$p"; \
	  else :; fi; \
	done

uninstall-cil32binSCRIPTS:
	@list='$(cil32bin_SCRIPTS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(cil32bindir)/$$p'"; \
	  rm -f "$(DESTDIR)$(cil32bindir)/$$p"; \
	done
