# Check for ELF header.  Prefer <libelf.h>, but fall back to
# <libelf/libelf.h> if <libelf.h> is not available.
AC_DEFUN([AC_CHECK_LIBELF],
[
AC_CHECK_HEADERS(libelf.h libelf/libelf.h, [break])
])

# Check for GELF header.  Prefer <gelf.h>, but fall back to
# <libelf/gelf.h> if <gelf.h> is not available.
AC_DEFUN([AC_CHECK_GELF],
[
AC_CHECK_HEADERS(gelf.h libelf/gelf.h, [break])
])
