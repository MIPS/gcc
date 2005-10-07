# Process this file with automake to create Makefile.in.

bin_PROGRAMS = gcjx
lib_LTLIBRARIES = libgcjx.la

AUTOMAKE_OPTIONS = subdir-objects foreign

################################################################

gcjx_SOURCES = main.cc
gcjx_LDADD = libgcjx.la -lpthread

libgcjx_la_SOURCES = $(dot_sources) $(model_sources) $(reader_sources) \
$(source_sources) $(format_sources) $(bytecode_sources)	\
$(header_sources) $(fdlibm_c_sources) $(fdlibm_cc_sources) \
$(aot_sources)

BUILT_SOURCES = source/keyword.h source/chartables.h typedefs.hh.gch

EXTRA_DIST = source/keyword.gperf source/gen-table.pl

## FIXME: gcc specific
WARNINGS = -Wall -Winvalid-pch -Wreturn-type
AM_CXXFLAGS = $(WARNINGS)
AM_CPPFLAGS = -I$(srcdir)/fdlibm

source/keyword.h: source/keyword.gperf
	cd $(srcdir) && gperf -L ANSI-C -C -F ', TOKEN_INVALID' -t -o -N find_keyword -k1,2,\$$ source/keyword.gperf > tmp.kwd && mv tmp.kwd source/keyword.h

source/chartables.h: source/gen-table.pl
	cd $(srcdir)/source && perl gen-table.pl UnicodeData-3.0.1.txt

## FIXME: need a better way to convince libtool to let us do this.
typedefs.hh.gch: $(headers)
	-rm -f typedefs.hh.gch
	$(CXXCOMPILE) -fPIC -DPIC -x c++-header -o typedefs.hh.gch $(srcdir)/typedefs.hh

mostlyclean-local:
	-rm -f typedefs.hh.gch

################################################################

headers = access.hh buffer.hh directory.hh bytecode/block.hh \
bytecode/bytegen.hh bytecode/byteutil.hh bytecode/classreader.hh \
bytecode/classwriter.hh bytecode/cpool.hh bytecode/generate.hh \
bytecode/insns.hh bytecode/locals.hh bytecode/outpool.hh \
bytecode/outstream.hh bytecode/relocation.hh classcache.hh codegen.hh \
compiler.hh conversions.hh defassign.hh exception.hh factory.hh	\
fold.hh format/format.hh global.hh header/cni.hh header/jni.hh \
header/jnistub.hh location.hh model/annotation.hh model/annomember.hh \
model/annotype.hh model/annovalue.hh model/arrayinit.hh	\
model/arrayref.hh model/arraytype.hh model/assert.hh model/assign.hh \
model/binary.hh model/block.hh model/blockscope.hh model/break.hh \
model/bytecode.hh model/cast.hh model/catch.hh model/class.hh \
model/classinst.hh model/classref.hh model/cond.hh \
model/constructor.hh model/continue.hh model/declstmt.hh model/do.hh \
model/element.hh model/empty.hh model/enumconst.hh model/enum.hh \
model/expr.hh model/exprstmt.hh model/field.hh model/fieldinit.hh \
model/fieldref.hh model/forenhanced.hh model/for.hh model/fwdtype.hh \
model/iannotatable.hh model/icatcher.hh model/icontext.hh \
model/identifier.hh model/ideprecatable.hh model/if.hh model/ilabel.hh \
model/imember.hh model/imodifiable.hh model/import.hh model/iname.hh \
model/init.hh model/instanceof.hh model/invoke.hh model/iscope.hh \
model/javadoc.hh model/label.hh model/literal.hh model/loop.hh \
model/memberref.hh model/method.hh model/modifier.hh model/newarray.hh \
model/new.hh model/null.hh model/package.hh model/parameters.hh	\
model/primitive.hh model/return.hh model/stmt.hh model/switch.hh \
model/synchronized.hh model/this.hh model/throw.hh \
model/throwsclause.hh model/try.hh model/type.hh model/typemap.hh \
model/typevar.hh model/unary.hh model/unit.hh model/value.hh \
model/variable.hh model/varref.hh model/varstmt.hh model/void.hh \
model/while.hh model/wildcard.hh name.hh owner.hh reader/classbytes.hh \
reader/classinst.hh reader/reader.hh reader/fdreader.hh	\
reader/zereader.hh reader/mmapbuffer.hh reader/readbuffer.hh \
reader/zebuffer.hh reader/source.hh resolve.hh scope.hh source/lex.hh \
source/machine.hh source/parse.hh source/token.hh source/tstream.hh \
source/ucs2.hh typedefs.hh util.hh visitor.hh warnings.hh watch.hh

dot_sources = access.cc classcache.cc compiler.cc conversions.cc \
defassign.cc directory.cc dump.cc factory.cc fold.cc init.cc \
location.cc name.cc owner.cc scope.cc util.cc warnings.cc

model_sources = model/annotation.cc model/annomember.cc	\
model/annotype.cc model/arrayinit.cc model/arrayref.cc \
model/arraytype.cc model/assert.cc model/assign.cc model/binary.cc \
model/block.cc model/blockscope.cc model/break.cc model/bytecode.cc \
model/cast.cc model/catch.cc model/class.cc model/classinst.cc \
model/classref.cc model/cond.cc model/constructor.cc model/continue.cc \
model/declstmt.cc model/do.cc model/element.cc model/empty.cc \
model/enum.cc model/expr.cc model/exprstmt.cc model/field.cc \
model/fieldinit.cc model/fieldref.cc model/for.cc model/forenhanced.cc \
model/fwdtype.cc model/iannotatable.cc model/identifier.cc model/if.cc \
model/import.cc model/imodifiable.cc model/init.cc model/instanceof.cc \
model/invoke.cc model/label.cc model/literal.cc model/memberref.cc \
model/method.cc model/modifier.cc model/newarray.cc model/new.cc \
model/null.cc model/package.cc model/parameters.cc model/primitive.cc \
model/return.cc model/stmt.cc model/switch.cc model/synchronized.cc \
model/this.cc model/throw.cc model/throwsclause.cc model/try.cc \
model/type.cc model/typemap.cc model/typevar.cc model/unary.cc \
model/unit.cc model/value.cc model/variable.cc model/varref.cc \
model/varstmt.cc model/while.cc model/wildcard.cc

bytecode_sources = bytecode/attribute.cc bytecode/block.cc \
bytecode/bytegen.cc bytecode/classreader.cc bytecode/classwriter.cc \
bytecode/cpool.cc bytecode/generate.cc bytecode/glue.cc	\
bytecode/locals.cc bytecode/outpool.cc bytecode/outstream.cc \
bytecode/poolreader.cc bytecode/relocation.cc bytecode/signature.cc \
bytecode/verify.cc

reader_sources = reader/classbytes.cc reader/fdreader.cc \
reader/zereader.cc reader/zebuffer.cc reader/mmapbuffer.cc \
reader/readbuffer.cc reader/source.cc

format_sources = format/format.cc

header_sources = header/cni.cc header/jni.cc header/jnistub.cc

source_sources = source/iconv.cc source/lex.cc source/parse.cc \
source/tstream.cc source/ucs2.cc

fdlibm_c_sources = fdlibm/dtoa.c fdlibm/e_acos.c fdlibm/e_asin.c \
fdlibm/e_atan2.c fdlibm/e_exp.c fdlibm/e_fmod.c fdlibm/e_log.c \
fdlibm/e_pow.c fdlibm/e_remainder.c fdlibm/e_rem_pio2.c	\
fdlibm/e_scalb.c fdlibm/e_sqrt.c fdlibm/k_cos.c fdlibm/k_rem_pio2.c \
fdlibm/k_sin.c fdlibm/k_tan.c fdlibm/mprec.c fdlibm/s_atan.c \
fdlibm/s_ceil.c fdlibm/s_copysign.c fdlibm/s_cos.c fdlibm/s_fabs.c \
fdlibm/sf_fabs.c fdlibm/s_floor.c fdlibm/sf_rint.c fdlibm/s_rint.c \
fdlibm/s_scalbn.c fdlibm/s_sin.c fdlibm/s_tan.c fdlibm/strtod.c	\
fdlibm/w_acos.c fdlibm/w_asin.c fdlibm/w_atan2.c fdlibm/w_exp.c	\
fdlibm/w_fmod.c fdlibm/w_log.c fdlibm/w_pow.c fdlibm/w_remainder.c \
fdlibm/w_sqrt.c

fdlibm_cc_sources = fdlibm/classpath.cc

aot_sources = aot/aotclass.cc aot/aotfactory.cc aot/mangle.cc
