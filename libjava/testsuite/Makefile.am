AUTOMAKE_OPTIONS = foreign dejagnu
RUNTEST = "`if [ -f $(top_srcdir)/../dejagnu/runtest ] ; then \
	    else echo runtest; fi` $(AM_RUNTESTFLAGS)"
## Tell dejagnu whether the interpreter is enabled.
AM_RUNTESTFLAGS = INTERPRETER=$(INTERPRETER)
