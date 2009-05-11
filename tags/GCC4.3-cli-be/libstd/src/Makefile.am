
SRC = _Exit.c \
      __io_device.c \
      __io_fin.c \
      __io_flush.c \
      __io_fopen.c \
      __io_fout.c \
      __io_fread.c \
      __io_ftable.c \
      __io_perm.c \
      __libinfo.c \
      __libstd_initfini.c \
      abort.c \
      abs.c \
      asctime.c \
      assert.c \
      basename.c \
      bsearch.c \
      calloc.c \
      clearerr.c \
      close.c \
      creat.c \
      ctime.c \
      ctype.c \
      dirname.c \
      div.c \
      errno.c \
      exit.c \
      fclose.c \
      fenv.c \
      feof.c \
      ferror.c \
      fflush.c \
      fgetc.c \
      fgetpos.c \
      fgets.c \
      float.c \
      fopen.c \
      fprintf.c \
      fputc.c \
      fputs.c \
      fread.c \
      free.c \
      freopen.c \
      fscanf.c \
      fseek.c \
      fsetpos.c \
      ftell.c \
      fwrite.c \
      getenv.c \
      imaxabs.c \
      imaxdiv.c \
      isatty.c \
      labs.c \
      ldiv.c \
      llabs.c \
      lldiv.c \
      localeconv.c \
      lseek.c \
      malloc.c \
      math.c \
      math_ld.c \
      mblen.c \
      mbstowcs.c \
      mbtowc.c \
      memchr.c \
      memcmp.c \
      memcpy.c \
      memmove.c \
      memset.c \
      open.c \
      perror.c \
      printf.c \
      qsort.c \
      rand.c \
      read.c \
      realloc.c \
      remove.c \
      rename.c \
      rewind.c \
      scanf.c \
      setbuf.c \
      setlocale.c \
      setvbuf.c \
      signal.c \
      snprintf.c \
      sprintf.c \
      sscanf.c \
      stat.c \
      strcat.c \
      strchr.c \
      strcmp.c \
      strcoll.c \
      strcpy.c \
      strcspn.c \
      strerror.c \
      strftime.c \
      strlen.c \
      strncat.c \
      strncmp.c \
      strncpy.c \
      strpbrk.c \
      strrchr.c \
      strspn.c \
      strstr.c \
      strtod.c \
      strtok.c \
      strtolul.c \
      strtollull.c \
      strxfrm.c \
      system.c \
      systime.c \
      time.c \
      tmpfile.c \
      tmpnam.c \
      ungetc.c \
      vfprintf.c \
      vfscanf.c \
      vprintf.c \
      vsnprintf.c \
      vsprintf.c \
      vscanf.c \
      vsscanf.c \
      wcstombs.c \
      wctomb.c \
      write.c

EXTRA_DIST = __host.c

EXEEXT=
if CLI_LIB
libcil_PROGRAMS = libstd.dll MSCorelibWrapper.dll
endif

libstd_dll_SOURCES = ${SRC}
libstd_dll_CFLAGS = -nostdinc -iwithprefix include -I$(top_srcdir)/include
#libstd_dll_LDFLAGS = -nostdlib -lgcc4net.dll
#libstd_dll_LDADD = -l ./MSCorelibWrapper.dll
libstd_dll_LDFLAGS = -L. -nostdlib -lgcc4net.dll -lMSCorelibWrapper.dll
libstd_dll_DEPENDENCIES = MSCorelibWrapper.dll


MSCorelibWrapper_dll_SOURCES= MSCorelibWrapper.cs

MSCorelibWrapper.dll: MSCorelibWrapper.cs
	$(MCS) $< -unsafe -target:library -out:$@

install-libcilPROGRAMS: $(libcil_PROGRAMS)
	test -z "$(libcildir)" || $(mkdir_p) "$(DESTDIR)$(libcildir)"
	@list='$(libcil_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_PROGRAM_ENV) $(libcilPROGRAMS_INSTALL) '$$p' '$(DESTDIR)$(libcildir)/$$p'"; \
	    $(INSTALL_PROGRAM_ENV) $(libcilPROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(libcildir)/$$p" || exit 1; \
	  else :; fi; \
	done

uninstall-libcilPROGRAMS:
	@list='$(libcil_PROGRAMS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(libcildir)/$$p'"; \
	  rm -f "$(DESTDIR)$(libcildir)/$$p"; \
	done


if NATIVE_LIB
libnative_LTLIBRARIES = libstd.la
install-exec-local: myinstall_libgcc
endif

libstd_la_SOURCES = ${SRC}
libstd_la_CFLAGS = -nostdinc -iwithprefix include -I$(top_srcdir)/include
libstd_la_LIBADD = libstd_la-__host.lo

libstd_la-__host.lo: $(srcdir)/__host.c
	$(LIBTOOL) --tag=CC --mode=compile $(CC) $(DEFAULT_INCLUDES) $(INCLUDES) -I$(top_srcdir) -std=c99 $(CFLAGS) -c -o $@ $<

LIBGCC_CC  = $(shell ${CC} -print-file-name=libgcc.a)
myinstall_libgcc: installdirs
	$(INSTALL_SCRIPT) $(LIBGCC_CC) $(DESTDIR)$(libnativedir)/libgcc.a
