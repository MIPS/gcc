
EXEEXT=
libcilopt_PROGRAMS = MSCorelibWrapper.dll

libnative_PROGRAMS = MSCorelibWrapper_support.so

MSCorelibWrapper_support.so: MSCorelibWrapper_support.c
	$(COMPILE) -std=c99 -fpic -shared -I$(top_srcdir) -o $@ $< 

MSCorelibWrapper.dll: MSCorelibWrapper.cs MSCorelibWrapper_support.so
	$(MCS) $< -unsafe -target:library -out:$@

install-libciloptPROGRAMS: $(libcilopt_PROGRAMS)
	test -z "$(libciloptdir)" || $(mkdir_p) "$(DESTDIR)$(libciloptdir)"
	@list='$(libcilopt_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_PROGRAM_ENV) $(libciloptPROGRAMS_INSTALL) '$$p' '$(DESTDIR)$(libciloptdir)/$$p'"; \
	    $(INSTALL_PROGRAM_ENV) $(libciloptPROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(libciloptdir)/$$p" || exit 1; \
	  else :; fi; \
	done

uninstall-libciloptPROGRAMS:
	@list='$(libcilopt_PROGRAMS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(libciloptdir)/$$p'"; \
	  rm -f "$(DESTDIR)$(libciloptdir)/$$p"; \
	done

install-libnativePROGRAMS: $(libnative_PROGRAMS)
	test -z "$(libnativedir)" || $(mkdir_p) "$(DESTDIR)$(libnativedir)"
	@list='$(libnative_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	   echo " $(INSTALL_PROGRAM_ENV) $(libnativePROGRAMS_INSTALL) '$$p' '$(DESTDIR)$(libnativedir)/$$p'"; \
	   $(INSTALL_PROGRAM_ENV) $(libnativePROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(libnativedir)/$$p" || exit 1; \
	  else :; fi; \
	done

uninstall-libnativePROGRAMS:
	@list='$(libnative_PROGRAMS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(libnativedir)/$$p'"; \
	  rm -f "$(DESTDIR)$(libnativedir)/$$p"; \
	done
