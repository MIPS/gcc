

TARGETS=libmudflap.a
#TARGETS=libmudflap.a libmudflap_s.so
#CFLAGS=-g -O2 -pg
CFLAGS=-g -O2 -Wall
CONFIG=mf-config.h

.SUFFIXES: .o .lo .c

SHOBJS=mf-runtime.lo mf-hooks.lo
OBJS=mf-runtime.o mf-hooks.o


all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJS) $(SHOBJS)

$(CONFIG):
	@echo Boo - create $(CONFIG) first.
	@exit 1

libmudflap.a: $(OBJS)
	ar rv $@ $(OBJS)
	ranlib $@

libmudflap_s.so: $(SHOBJS)
	gcc -shared -o $@ $< -ldl

$(SHOBJS) $(OBJS): $(CONFIG)

.c.o:
	gcc -o $@ -include $(CONFIG) $(CFLAGS) -c $<

.c.lo:
	gcc -fPIC -DPIC -DSHARED -o $@ -include $(CONFIG) $(CFLAGS) -c $<

