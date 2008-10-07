STAGE2_CFLAGS += -gtoggle
do-compare = $(SHELL) $(srcdir)/contrib/compare-debug $$f1 $$f2
