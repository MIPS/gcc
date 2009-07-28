
CIL_TOOLS = \
        ar \
        as \
        ld \
        nm \
        objdump \
        ranlib \
        size \
        strip

EXTRA_DIST = ${CIL_TOOLS:%=%.in}
CLEANFILES = ${CIL_TOOLS}

bin_SCRIPTS = ${CIL_TOOLS} ilrun mono

as: as.in
	cat $< | sed 's?@ILASM@?$(CIL_AS)?' > $@

ld: ld.in
	cat $< | sed 's?@ILLD@?$(CIL_LD)?' > $@

%: %.in
	cat $< > $@


.PHONY: install-exec-local install-mydirs install-exec-tooldir install-link-lib

install-exec-local: install-mydirs install-exec-tooldir install-link-lib

install-mydirs:
	@$(mkinstalldirs) $(DESTDIR)$(bindir)
	@$(mkinstalldirs) $(DESTDIR)$(includedir)
	@$(mkinstalldirs) $(DESTDIR)$(libdir)
	@if [ "x$(local_prefix)" != "x" ]; then \
	  $(mkinstalldirs) $(DESTDIR)$(local_prefix)/bin; \
	  rm -f $(DESTDIR)$(local_prefix)/include; \
	  rm -f $(DESTDIR)$(local_prefix)/lib; \
	  (cd $(DESTDIR)$(local_prefix) && $(LN_S) ../include include); \
	  (cd $(DESTDIR)$(local_prefix) && $(LN_S) ../lib lib); \
	fi

install-exec-tooldir: $(bin_SCRIPTS)
	@if [ "x$(local_prefix)" != "x" ]; then \
	  $(mkinstalldirs) $(DESTDIR)$(local_prefix)/bin; \
	  list='$(CIL_TOOLS)'; for p in $$list; do \
	    n=`echo $$p | sed '$(transform)'`; \
	    if [ "$(DESTDIR)$(bindir)/$$n$(EXEEXT)" != "$(DESTDIR)$(local_prefix)/bin/$$p$(EXEEXT)" ]; then \
	      rm -f $(DESTDIR)$(local_prefix)/bin/$$p$(EXEEXT); \
	      echo " $(binSCRIPT_INSTALL) '$$p' '$(DESTDIR)$(local_prefix)/bin/$$p$(EXEEXT)'"; \
	      $(binSCRIPT_INSTALL) "$$p" "$(DESTDIR)$(local_prefix)/bin/$$p$(EXEEXT)"; \
	    fi; \
	  done; \
	fi

install-link-lib:
	@$(mkinstalldirs) $(DESTDIR)$(libdir)
	$(INSTALL) $(OPENSYSTEM_C_DLL) $(DESTDIR)$(libdir)/
