AUTOMAKE_OPTIONS = foreign dejagnu
RUNTEST = "`if [ -f $(top_srcdir)/../dejagnu/runtest ] ; then \
	    else echo runtest; fi` $(AM_RUNTESTFLAGS)"
## Tell dejagnu whether the interpreter is enabled.
# autoconf2.13's target_alias
target_noncanonical = @target_noncanonical@

AM_RUNTESTFLAGS = INTERPRETER=$(INTERPRETER)
