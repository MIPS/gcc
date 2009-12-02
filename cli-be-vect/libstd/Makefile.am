
SUBDIRS = src

if CLI_LIB
SUBDIRS += include
endif

if OPT_NATIVE_LIB
SUBDIRS += src_opt
endif

# No install-html or install-pdf support in automake yet
.PHONY: install-html install-pdf
install-html:

install-pdf:
