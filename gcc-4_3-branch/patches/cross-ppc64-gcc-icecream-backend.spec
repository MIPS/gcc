#
# spec file for package cross-ppc64-gcc-icecream-backend (Version 4.3.3_20081022)
#
# Copyright (c) 2008 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

%define build_cp 1
%define build_ada 0
%define build_libjava 0
%define build_java 0
%define build_fortran 0
%define build_objc 0
%define build_objcp 0
%define build_nof 0
%define build_with_mudflap 0
%define gcc_for_opt 0
%define gcc_target_arch ppc64-linux
%define gcc_icecream 1

Name:           cross-ppc64-gcc-icecream-backend
BuildRequires:  cross-ppc64-binutils glibc-devel-64bit
BuildRequires:  gmp-devel mpfr-devel
BuildRequires:  bison
BuildRequires:  flex
ExclusiveArch:  ppc ppc64 %ix86 x86_64 ia64
%define _binary_payload w.ufdio
# COMMON-BEGIN
# COMMON-BEGIN
%define biarch_targets x86_64 s390x powerpc64 powerpc
Url:            http://gcc.gnu.org/
License:        BSD 3-Clause; GPL v2 or later; LGPL v2.1 or later; X11/MIT
Version:        4.3.3_20081022
Release:        9.3
%define gcc_version %(echo %version | sed 's/_.*//')
%define gcc_dir_version %(echo %gcc_version | cut -d '.' -f 1-2)
%define snapshot_date %(echo %version | sed 's/[34]\.[0-4]\.[0-6]//' | sed 's/_/-/')
%define binsuffix -4.3
# COMMON-END
# COMMON-BEGIN
Group:          Development/Languages/C and C++
AutoReqProv:    on
Summary:        The GNU C Compiler and Support Files
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source:         gcc-%{gcc_version}%{snapshot_date}.tar.bz2
Source1:        change_spec
Source2:        libgcj43-rpmlintrc
Source3:        gcc43-rpmlintrc
Source4:        ecj.jar
Source5:        README.First-for.SuSE.packagers
Patch1:         gcc-dir-version.patch
Patch2:         gcc-sles-version.patch
Patch3:         gcc-noalias-warn.diff
Patch4:         gcc-add-defaultsspec.diff
Patch5:         boehm-gc-strict-aliasing.patch
Patch6:         suse-record-gcc-opts.diff
Patch7:         acats-timeout.patch
Patch10:        program-transform-name.diff
Patch11:        program-transform-name-tools.diff
Patch12:        gcc4-ppc64-m32-m64-multilib-only.patch
Patch22:        libjava-no-static.diff
Patch23:        libjava-no-multilib.diff
Patch24:        tls-no-direct.diff
Patch25:        pr27880.diff
Patch28:        pr33763.diff
Patch30:        Wunprototyped-calls.diff
Patch31:        pr27799.diff
Patch40:        pr36765.diff
Patch41:        fpreserve-function-arguments43.patch
Patch42:        pr27975.diff
Patch43:        build-id.diff
Patch45:        nvl423594.patch
Patch46:        pr32277.diff
Patch47:        pr36438.diff
Patch48:        pr37868.diff
Patch49:        pr38051.diff
# A set of patches from the RH srpm
Patch50:        gcc43-c++-builtin-redecl.patch
Patch51:        gcc41-ia64-stack-protector.patch
Patch52:        gcc43-java-debug-iface-type.patch
Patch53:        gcc43-libgomp-omp_h-multilib.patch
Patch54:        gcc43-pr34037.patch
Patch55:        gcc41-java-slow_pthread_self.patch
Patch56:        gcc43-pr36741-revert.patch
Patch57:        gcc41-ppc32-retaddr.patch
Patch58:        gcc43-pr37189.patch
Patch59:        gcc43-x86_64-va_start.patch
# Some patches taken from Debian
Patch60:        gcc43-textdomain.patch
Patch61:        gcc43-rename-info-files.patch
# Patches for AMD features
Patch70:        pr34043-1.diff
Patch71:        pr34043-2.diff
Patch72:        pr34043-3.diff
Patch73:        pr34043-4.diff
Patch74:        pr34043-5.diff
Patch75:        amd-SSE5-shift-ppc-1.diff
Patch76:        amd-SSE5-shift-ppc-2.diff
Patch77:        amd-SSE5-shift.diff
Patch78:        amd-cunroll-1.diff
Patch79:        amd-cunroll-2.diff
Patch81:        amd-cvect-1.diff
Patch82:        amd-cvect-2.diff
# Patches for Intel features
Patch80:        intel303993-aes.diff
# Patches for IBM features
Patch90:        ibm304071-z10-1
Patch91:        ibm304071-z10-2
Patch92:        ibm304071-z10-3
Patch93:        ibm304071-z10-4
Patch94:        ibm304071-z10-5
Patch95:        ibm304071-z10-6
Patch96:        pr36745.diff
Patch97:        pr36822.diff
Patch99:        ibm-vector-keyword-1
Patch100:       ibm-vector-keyword-2
Patch101:       ibm-vector-keyword-3
Patch102:       ibm304071-z10-7
Patch103:       ibm304134-power7-1
Patch104:       ibm-cell-split
Patch105:       ibm-cell-split-fixes
Patch106:       nvl425806.patch
Patch109:       nvl425783.patch
Patch110:       nvl425784.patch
Patch111:       nvl425788.patch
Patch112:       nvl425789.patch
Patch114:       nvl425791.patch
Patch115:       nvl425794.patch
Patch116:       nvl425798-1.patch
Patch117:       nvl425798-2.patch
Patch118:       nvl425799.patch
Patch119:       nvl426087.patch
Patch120:       ibm304071-z10-8
Patch121:       nvl434500.patch
Patch122:       nvl436041.patch
Patch123:       gcc43-no-unwind-tables.diff
Patch124:       nvl441016.patch
Patch125:       common-sysroot-gcc.diff
Patch126:       nvl447669.diff
# Patches for SAP features
Patch130:       sap303956-utf16-1.diff
Patch131:       sap303956-utf16-2.diff
Patch132:       sap303956-utf16-3.diff
Patch133:       sap303956-utf16-4.diff
Patch134:       sap303956-utf16-mangling.diff
# LIBJAVA-DELETE-BEGIN

%description
NOTE: Be sure to install at least the following packages as well or you
will not be able to compile:  binutils and glibc-devel.

Compiled with support for Ada, C, C++, Fortran77, Java, and
Objective-C.



Authors:
--------
    The GCC team.

%if 0%{?gcc_target_arch:1}
%define CONFIGURE_TARGET %{gcc_target_arch}
%define TARGET_ARCH %(echo %{gcc_target_arch} | cut -d - -f 1 | sed -e "s/i.86/i586/;s/ppc/powerpc/")
%if 0%{?gcc_icecream:1} && %{TARGET_ARCH} == "powerpc"
%define CONFIGURE_TARGET powerpc64-linux
%endif
%if 0%{?gcc_icecream:1} && %{TARGET_ARCH} == "powerpc64"
%define CONFIGURE_TARGET powerpc64-linux
%endif
%if 0%{?gcc_icecream:1} && %{TARGET_ARCH} == "i586"
%define CONFIGURE_TARGET i586-linux
%endif
%else
%define TARGET_ARCH %(echo %{_target_cpu} | sed -e "s/i.86/i586/;s/ppc/powerpc/")
%endif
%define HOST_ARCH %(echo %{_host_cpu} | sed -e "s/i.86/i586/;s/ppc/powerpc/")
%define biarch %(case " %{biarch_targets} " in (*" %{TARGET_ARCH} "*) echo 1;; (*) echo 0;; esac)
%ifarch ppc
%define GCCDIST powerpc64-suse-linux
%else
%define GCCDIST %{HOST_ARCH}-suse-linux
%endif
%define libsubdir %{_libdir}/gcc/%{GCCDIST}/%{gcc_dir_version}
%define gxxinclude %{_prefix}/include/c++/%{gcc_dir_version}
# COMMON-END
# COMMON-BEGIN

%prep
%setup -q -n gcc-%{gcc_version}%{snapshot_date}
%patch1
# This patch changes the version string reported by GCC from
# gcc (SUSE Linux) 4.3.2 20080806 (prerelease) [gcc-4_3-branch revision 138797]
# to
# gcc (SUSE Linux) 4.3.1 [gcc-4_3-branch revision 138797]
# as requested by bnc#363219.
%patch2
%patch3
%patch4
%patch5
%if 0%{suse_version} > 1100
%patch6
%endif
%patch7
%patch10
%patch11
%if !%{build_nof}
%patch12
%endif
%patch22
%patch23
%patch24
%patch25
%patch28
%patch30
%patch31
%patch40
%patch41
%patch42
%if 0%{suse_version} > 1100
%patch43
%endif
%patch45
%patch46
%patch47
%patch48 -p1
%patch49
%patch50
%patch51
%patch52
%patch53
%patch54
%patch55
%patch56
%patch57
%patch58
%patch59
%patch60
%patch61
%patch70
%patch71
%patch72
%patch73
%patch74 -p1
%patch75
%patch76
%patch77
%patch78
%patch79
%patch81
%patch82
%patch80
%patch90
%patch91
%patch92
%patch93
%patch94
%patch95
%patch96
%patch97
%patch99
%patch100
%patch101
%patch102
%patch103
%patch104 -p1
%patch105 -p1
%patch106
%patch109
%patch110
%patch111
%patch112
%patch114
%patch115
%patch116
%patch117
%patch118
%patch119
%patch120
%patch121
%patch122
%patch123
%patch124 -p1
%patch125 -p1
%patch126
%patch130
%patch131
%patch132
%patch133
%patch134

%build
# Avoid rebuilding of generated files
contrib/gcc_update --touch
# Split version file into version used for directories (X.Y) and
# version to report with --version (X.Y.Z).
# See also gcc-dir-version.patch.
# Also decrement the patchlevel version by one
if test `cat gcc/DEV-PHASE` == "prerelease"; then
  ( cat gcc/BASE-VER | cut -d '.' -f 1-2 | tr -d '\n'; echo -n .; cat gcc/BASE-VER | cut -d '.' -f 3 | tr '0123456789' '0012345678' ) > gcc/FULL-VER
else
  mv gcc/BASE-VER gcc/FULL-VER
fi
cat gcc/FULL-VER | cut -d '.' -f 1-2 > gcc/BASE-VER
rm -rf obj-%{GCCDIST}
mkdir obj-%{GCCDIST}
cd obj-%{GCCDIST}
RPM_OPT_FLAGS="$RPM_OPT_FLAGS -U_FORTIFY_SOURCE"
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/-fno-rtti//g' -e 's/-fno-exceptions//g' -e 's/-Wmissing-format-attribute//g' -e 's/-fstack-protector//g' -e 's/-ffortify=.//g' -e 's/-Wall//g' -e 's/-m32//g' -e 's/-m64//g'`
%ifarch %ix86
# -mcpu is superceded by -mtune but -mtune is not supported by
# our bootstrap compiler.  -mcpu gives a warning that stops
# the build process, so remove it for now.
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/-mcpu=i.86//g'`
%endif
%ifarch s390 s390x
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/-fsigned-char//g'`
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/-O1/-O2/g'`
%endif
%if 0%{?gcc_target_arch:1} && 0%{!?gcc_icecream:1}
# Kill all -march/tune/cpu because that screws building the target libs
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/-m\(arch\|tune\|cpu\)=[^ ]*//g'`
%endif
# Replace 2 spaces by one finally
RPM_OPT_FLAGS=`echo $RPM_OPT_FLAGS|sed -e 's/  / /g'`
languages=c
%if %{build_cp}
languages=$languages,c++
%endif
%if %{build_objc}
languages=$languages,objc
%endif
%if %{build_fortran}
languages=$languages,fortran
%endif
%if %{build_objcp}
languages=$languages,obj-c++
%endif
%if %{build_java}
languages=$languages,java
%endif
%if %{build_ada}
languages=$languages,ada
%endif
J=%{?jobs:%jobs}
if test -z "$J"; then
  J=$(getconf _NPROCESSORS_CONF)
  JL=$(($J * 2))
else
  test 1 -gt "$J" && J=1
  JL=$(($(getconf _NPROCESSORS_CONF)*2))
fi
if test "$J" == "0"; then
  J=1
fi
if test "$JL" == "0"; then
  JL=1
fi
PARALLEL="-j$J -l$JL"
# we don't want some miscompiles in the testsuite, or some fault in
# the compiler to kill the machine.  Hence we limit the amount of memory
# by the physical RAM plus half of swap
#MEM=$(free -m | awk '/^Mem:/ {print $2}')
#SWAP=$(free -m | awk '/^Swap:/ {print $2}')
#ulimit -v $(((MEM + SWAP/2)*1024))
# In general we want to ship release checking enabled compilers
# and run BETA with checking enabled.
#ENABLE_CHECKING="--enable-checking=yes"
ENABLE_CHECKING="--enable-checking=release"
%if "%{name}" != "gcc43"
export PATH=/opt/cross/bin:$PATH
%endif
# Work around tail/head -1 changes
export _POSIX2_VERSION=199209
# configure tries to parse include/features.h to detect
# if glibc supports SSP.  When cross compiling we would need
# to install a cross glibc, which we don't, or make the whole
# thing point to the native glibc, which is in /, hence the
# --with-sysroot=/
%if %{build_ada}
# Using the host gnatmake like
#   CC="gcc%{hostsuffix}" GNATBIND="gnatbind%{hostsuffix}"
#   GNATMAKE="gnatmake%{hostsuffix}"
# doesn't work due to PR33857, so an un-suffixed gnatmake has to be
# available
mkdir -p host-tools/bin
cp -a /usr/bin/gnatmake%{hostsuffix} host-tools/bin/gnatmake
cp -a /usr/bin/gnatlink%{hostsuffix} host-tools/bin/gnatlink
cp -a /usr/bin/gnatbind%{hostsuffix} host-tools/bin/gnatbind
cp -a /usr/bin/gcc%{hostsuffix} host-tools/bin/gcc
ln -sf /usr/%{_lib} host-tools/%{_lib}
export PATH="`pwd`/host-tools/bin:$PATH"
%endif
#%if 0%{?gcc_target_arch:1} && 0%{!?gcc_icecream:1}
#%else
#	--enable-threads=posix \
#%endif
#	--enable-shared \
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" XCFLAGS="$RPM_OPT_FLAGS" \
TCFLAGS="$RPM_OPT_FLAGS" GCJFLAGS="$RPM_OPT_FLAGS" \
../configure \
	--prefix=%{_prefix} \
	--infodir=%{_infodir} \
	--mandir=%{_mandir} \
	--libdir=%{_libdir} \
	--libexecdir=%{_libdir} \
	--enable-languages=$languages \
	$ENABLE_CHECKING \
	--with-gxx-include-dir=%{_prefix}/include/c++/%{gcc_dir_version} \
	--enable-ssp \
	--disable-libssp \
	--with-bugurl="http://bugs.opensuse.org/" \
	--with-pkgversion="SUSE Linux" \
%if !%{build_libjava}
        --disable-libgcj \
%else
	--with-java-home=%{_libdir}/jvm/java-1.5.0-gcj%{binsuffix}-1.5.0.0/jre \
	--with-ecj-jar=%{libsubdir}/ecj.jar \
        --enable-java-awt=gtk \
	--enable-gtk-cairo \
%if !%{biarch_libjava}
        --disable-libjava-multilib \
%endif
%endif
%if !%{build_with_mudflap}
	--disable-libmudflap \
%endif
	--with-slibdir=/%{_lib} \
	--with-system-zlib \
	--enable-__cxa_atexit \
	--enable-libstdcxx-allocator=new \
	--disable-libstdcxx-pch \
	--enable-version-specific-runtime-libs \
%if 0%{?gcc_target_arch:1}
%if 0%{?gcc_icecream:1}
	--with-as=/opt/cross/%{TARGET_ARCH}-linux/bin/as \
	--with-ld=/opt/cross/%{TARGET_ARCH}-linux/bin/ld \
	--with-sysroot=/ \
	--enable-linux-futex \
%endif
%if 0%{?build_static:1}
	--disable-shared \
%endif
%if "%{TARGET_ARCH}" == "spu"
	--with-sysroot=/usr/spu \
	--with-gxx-include-dir=%{_prefix}/spu/include/c++/%{gcc_dir_version} \
	--with-newlib \
%endif
	--disable-nls \
	--target=%{CONFIGURE_TARGET} \
%else
	--program-suffix=%{binsuffix} \
	--enable-linux-futex \
%ifarch ia64
	--with-system-libunwind \
%else
	--without-system-libunwind \
%endif
%endif
%if "%{TARGET_ARCH}" == "powerpc"
	--with-cpu=default32 \
	--enable-secureplt \
	--with-long-double-128 \
%endif
%if "%{TARGET_ARCH}" == "powerpc64"
	--with-cpu=power4 \
	--enable-secureplt \
	--with-long-double-128 \
%endif
%if "%{TARGET_ARCH}" == "i586"
	--with-cpu=generic \
%endif
%if "%{TARGET_ARCH}" == "x86_64"
	--with-cpu=generic \
%endif
%if "%{TARGET_ARCH}" == "s390"
        --with-tune=z9-109 --with-arch=z900 \
	--with-long-double-128 \
%endif
%if "%{TARGET_ARCH}" == "s390x"
        --with-tune=z9-109 --with-arch=z900 \
	--with-long-double-128 \
%endif
	--build=%{GCCDIST}
# COMMON-END
# COMMON-END
make %{?jobs:-j%jobs} all-gcc TARGET-gcc="cc1 cc1plus gcc-cross g++ native"

%install
cd obj-%{GCCDIST}
rm -rf $RPM_BUILD_ROOT
BUILD_TARGET=%{TARGET_ARCH}
ORIG_TARGET=$BUILD_TARGET
# We play funny tricks with the ppc compiler.  We want to have it biarch,
# which the powerpc compiler itself doesn't support.  So we actually
# configure it as powerpc64 target compiler (i.e. a native cross compiler
# even on ppc), with the default code generation to be powerpc32.
# But this doesn't hide the fact that it's still a cross compiler which
# shows also in various paths
test "$BUILD_TARGET" = powerpc && BUILD_TARGET=powerpc64
make -C gcc DESTDIR=$RPM_BUILD_ROOT/inst install-driver install-common
# But the assembler comes from the cross-binutils, and hence is _not_
# named funnily, not even on ppc, so there we need the original target
install -s -D /opt/cross/bin/${ORIG_TARGET}-linux-as $RPM_BUILD_ROOT/env/usr/bin/as
install -s $RPM_BUILD_ROOT/inst%{_prefix}/bin/${BUILD_TARGET}-linux-g++ $RPM_BUILD_ROOT/env/usr/bin/g++
install -s $RPM_BUILD_ROOT/inst%{_prefix}/bin/${BUILD_TARGET}-linux-gcc $RPM_BUILD_ROOT/env/usr/bin/gcc
for back in cc1 cc1plus; do 
	install -s -D $RPM_BUILD_ROOT/inst%{_libdir}/gcc/${BUILD_TARGET}-linux/%{gcc_dir_version}/$back \
		$RPM_BUILD_ROOT/env%{_libdir}/gcc/${BUILD_TARGET}-linux/%{gcc_dir_version}/$back
done
libs=`for bin in $RPM_BUILD_ROOT/env/usr/bin/* $RPM_BUILD_ROOT/env%{_libdir}/gcc/${BUILD_TARGET}-linux/%{gcc_dir_version}/*; do \
  ldd $bin | sed -n '\,^[^/]*\(/[^ ]*\).*,{ s//\1/; p; }'  ;\
done | sort -u `
for lib in $libs; do
   # Check wether the same library also exists in the parent directory,
   # and prefer that on the assumption that it is a more generic one.
   baselib=`echo "$lib" | sed 's,/[^/]*\(/[^/]*\)$,\1,'`
   test -f "$baselib" && lib=$baselib
   install -s -D $lib $RPM_BUILD_ROOT/env$lib
done
rm -rf $RPM_BUILD_ROOT/inst
cd $RPM_BUILD_ROOT/env
tar cvzf ../%{name}_%{_arch}.tar.gz *
cd ..
mkdir -p usr/share/icecream-envs
mv %{name}_%{_arch}.tar.gz usr/share/icecream-envs
rpm -q --changelog glibc >  usr/share/icecream-envs/%{name}_%{_arch}.glibc
rpm -q --changelog binutils >  usr/share/icecream-envs/%{name}_%{_arch}.binutils
rm -r env

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/share/icecream-envs

%changelog
* Thu Nov 27 2008 rguenther@suse.de
- Add capability to process default_spec from a defaults.spec specs
  file if that exists.
* Fri Nov 21 2008 matz@suse.de
- Fix a segfault.  [bnc #447669]
* Fri Nov 21 2008 rguenther@suse.de
- Back to z900 default architecture and z9-109 tuning for System-z.
* Mon Nov 17 2008 rguenther@suse.de
- Backport patch for PR38051.  [ggo#38169]
- Filter out -m32/-m64 from RPM_OPT_FLAGS.
* Thu Nov 13 2008 matz@suse.de
- Pass build-id to linker also on ppc/ppc64.
* Mon Nov 10 2008 matz@suse.de
- Make libgcc43 also provide/obsolete libgcc33.  [bnc #442802]
* Fri Nov  7 2008 rguenther@suse.de
- Disable profiledbootstrap.
* Fri Nov  7 2008 rguenther@suse.de
- Add patch for PR37868.  [bnc#442276]
- Add uchar.h header.  [fate#303956]
* Thu Nov  6 2008 rguenther@suse.de
- Add patch for PR36438.  [bnc#442192]
* Mon Nov  3 2008 rguenther@suse.de
- Fix Cell ABI problems.  [bnc#441016]
- Move to new Cell cross compiler directory layout.
- Add patch to fix --with-sysroot for Cell.
* Fri Oct 24 2008 schwab@suse.de
- Fix indirect call profiling on ia64 (PR32277).
* Wed Oct 22 2008 matz@suse.de
- Update to gcc-4_3-branch head (141291).
  * Fixes PR37603, PR37731, PR35437, PR37102.
- Build crtend*.o without unwind tables.
* Wed Oct 15 2008 rguenther@suse.de
- Fix version reporting for SLES, now also for openSUSE.  [bnc#363219]
- Fix use of DFP instructions with -msoft-float on System-z.  [bnc#436041]
- Change default architecture to z9-109 for System-z.
- Change default tuning to z10 for System-z.
- Enable final C++ mangling for UTF-16.  [fate#303956]
* Mon Oct 13 2008 rguenther@suse.de
- Add missing SPU intrinsics.  [bnc#434500]
* Thu Oct  2 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (140831).
  * Drop now unneeded patches.
- Adopt patches from Fedora10
  * Add fixes for PR34037, PR37189 and revert the fix for PR36741.
  * Fix omp.h header for the 32/64bit multilib.
  * Enable -mno-sse to optimize varargs stack usage.
- Enable proper java home for libgcj.  [bnc#303904]
* Sat Sep 20 2008 rguenther@suse.de
- Drop patch for bnc#425800 again.
- Disable fortran for static crosses.  [bnc#428329]
* Tue Sep 16 2008 rguenther@suse.de
- Put the compile flags section at the end of the file.  Fixes
  wrong code for glibc startfiles.
- Apply build-id and compile flags recording only for suse_version > 1100.
- Add backport patches for UTF-16 support.  [fate#303956]
- Change .comment.SuSE.OPTs to .comment.SUSE.OPTs.
- Add patch to document -march=z10 option for s390.
* Mon Sep 15 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r140371).
- Add patch for PR35620.  [bnc#425697]
- Add backport patches for powerpc bugfixes from GCC trunk.
  [bnc#425806, bnc#425803, bnc#425800, bnc#425783, bnc#425784, bnc#425788,
  bnc#425789, bnc#425790, bnc#425791, bnc#425794, bnc#425798, bnc#425799,
  bnc#426087]
- Add patch to fix invalid GIMPLE from SRA.  [bnc#423594]
- Fix libgcj43 build.
- Make compile flags comment section non-allocated.
* Fri Sep 12 2008 dmueller@suse.de
- track compile flags in ELF objects (fate #300498)
* Wed Sep 10 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r140201).
  * adds requested Cell bugfix.
- Add patch for Cell optimizations.
- Passs --build-id to linker.
* Thu Sep  4 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r139985).
- Do not build static libjava.
* Mon Sep  1 2008 rguenther@suse.de
- Add patch to adjust GCC version string for SLES.  [bnc#363219]
- Forward port patch to preserve function arguments for debug info
  from SLE10.  [fate#301921]
- Add patch for POWER7 support.  [fate#304134]
- Disable mudflap support by default, do not build libmudflap.
- Add patch for PR27975.  [fate#300955]
* Thu Aug 28 2008 rguenther@suse.de
- Update to GCC 4.3.2 release (r139672).  [bnc#363219]
- Revert -mcpu=mpower4 change for ppc, keep it for ppc64.  [bnc#408808]
* Thu Aug 21 2008 rguenther@suse.de
- Update patch for PR27799.  [bnc#419260]
* Mon Aug 18 2008 rguenther@suse.de
- Add patch for the z10 pipeline description.  [bnc#417534, fate#304071]
* Fri Aug 15 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r139129).
- Make -mcpu=power4 the default.  [bnc#417566]
* Mon Aug 11 2008 rguenther@suse.de
- Add patch for PR36998.  [bnc#416157]
* Tue Aug  5 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r138797).
- Add patches for context sensitive vector keyword support (IBM).
- Add patch for PR36613.
* Fri Jul 25 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r138144).
- Add patches for early complete loop unrolling.
* Mon Jul 21 2008 rguenther@suse.de
- Add patches to support SSE5 vectorized shift instructions.
- Add patch for PR36765.
* Tue Jul 15 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r137837).
- Make it build on SLES9.
- Enable C++ and GFortran for the spu cross.
- Add patch for PR36745.
- Add patch for PR36822.  [bnc#408075]
* Thu Jul  3 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r137416).
- Add patch to support Intel AES intrinsics.  [fate#303993]
- Add patches to support the IBM z10 architecture.  [fate#304071]
* Wed Jun 25 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r137104).  [bnc#402268]
* Fri Jun 13 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r136744).
- Add patch for PR36519.  [ggo#36519]
* Mon Jun  9 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r136588).
- Includes GCC 4.3.1 release.
- Add patch for PR36474.  [ggo#36474]
* Tue May 27 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r136151).
- Add patches for PRs 36262 and 36291.  [ggo#36262, ggo#36291]
- Add patch for PR36343.  [ggo#36343]
- Add patch for better supporting suffixed Ada drivers and tools.
- Always set sysroot for spu target.  [bnc#397825]
* Thu May 15 2008 rguenther@suse.de
- Add patch to fix memtest miscompilation.  [ggo#36227]
- Add patch for PR34330 and PR36244.  [ggo#34330, ggo#36244]
* Fri May  9 2008 rguenther@suse.de
- Add patch for PR36187.  [bnc#388664]
* Wed May  7 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r135036).  [ggo#36077, ggo#36137]
- Adds warning for undefined behavior on pointer overflow.
* Fri Apr 25 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r134659).  [ggo#35773, ggo#35907, ggo#35695,
  ggo#36008, ggo#36015, ggo#36017]
- Add patch to fix latent aliasing bug.
* Thu Apr 10 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r134138).  [ggo#35833, ggo#35900]
* Thu Apr 10 2008 ro@suse.de
- added baselibs.conf file to build xxbit packages
  for multilib support
* Tue Apr  1 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r133790).
- Remove use of %%run_ldconfig.
- Add fix for K&R missed prototype warning.  [bnc#376194]
* Thu Mar 27 2008 schwab@suse.de
- Add fix for PR35695.
* Fri Mar 14 2008 matz@suse.de
- Hide some more gcov functions.
* Tue Mar 11 2008 rguenther@suse.de
- Add patch for PR35469.
- Fix two bugs in the patches for PR34043.
* Thu Mar  6 2008 rguenther@suse.de
- Update to GCC 4.3.0 release.
- Add patches for PR35472 and PR34989.
- Add patches for AMD requested feature.  [ggo#34043]
* Fri Feb 22 2008 rguenther@suse.de
- Deal with fixed wchar.h on old glibc systems.
* Mon Feb 18 2008 rguenther@suse.de
- Update to gcc-4_3-branch head (r132521).  [bnc#355496, bnc#354113]
* Sat Feb 16 2008 schwab@suse.de
- Make sure the icecream env contains all needed shared libs.
* Tue Feb 12 2008 rguenther@suse.de
- Update to SVN trunk head (r132345).
- Add patches to versionize info documentation and locales.
* Fri Feb  1 2008 rguenther@suse.de
- Fix libgcj_bc packaging.
- Add workaround for PR35056.  [bnc#358319]
* Mon Jan 28 2008 rguenther@suse.de
- Update to SVN trunk head (r131976).  [bnc#355254]
* Tue Jan 22 2008 rguenther@suse.de
- Update to SVN trunk head (r131724).  [#343712]
- Add patch for PR34829.
- Add -Wunprototyped-calls and enable it with -Wall.
* Wed Jan 16 2008 rguenther@suse.de
- Update to SVN trunk head (r131592).  [#337901, #354119]
* Wed Jan  2 2008 rguenther@suse.de
- Update to SVN trunk head (r131254).
- Drop no longer required patches.
- Add patch for PR33960.
* Mon Dec 10 2007 rguenther@suse.de
- Add patches for PR34391 and PR34302.
* Wed Dec  5 2007 rguenther@suse.de
- Add patch for PR34334.
* Thu Nov 29 2007 rguenther@suse.de
- Update to SVN trunk head (r130511).  [#340981]
- Add patch for PR33763.  [#344524]
- Add patch for PR34244.  [#342603]
- Add patch for PR34238.
* Fri Nov 23 2007 rguenther@suse.de
- Build gcc43-java from libgcj43.spec.
- Shuffle binaries back from gcc43-gij to gcc43-java retaining
  only classpath tools in gcc43-gij.
- Add libgcj43-jar package required by libgcj43-devel and only
  recommended by libgcj43.
- Fix icecream cross install directory.
* Thu Nov 22 2007 rguenther@suse.de
- Simplify biarch rpm-macros.
- Add patch for PR34176.  [#342236]
- Add another alias fix.
* Tue Nov 20 2007 rguenther@suse.de
- Update to SVN trunk head (r130276).
- Revert fix for PR25950.
- Add patch for PR34148.
* Tue Nov 13 2007 rguenther@suse.de
- Change directories used in GCC install to only contain major and
  minor version, not patchlevel.
* Fri Nov  9 2007 rguenther@suse.de
- Update to SVN trunk head (r130038).
- Enable spu and avr cross targets.
- Fix i386 and ppc64 icecream cross targets.
- Disable alpha and arm icecream cross targets.
- Fix icecream cross target search path issues.
- Add README.First-for.SuSE.packagers as extra source and update it.
- Depend on gcc43-ada.
* Thu Nov  8 2007 rguenther@suse.de
- Enable icecream backends.
* Mon Nov  5 2007 rguenther@suse.de
- Fix ia64 stack-protector patch.
- Add patch for PR32260.
* Tue Oct 30 2007 rguenther@suse.de
- Fix rpmlint override files.
* Mon Oct 29 2007 rguenther@suse.de
- Add patch for PR33871, linker errors with anonymous namespaces and
  typeinfo.
* Wed Oct 24 2007 rguenther@suse.de
- Add patch for PR33870, sqlite miscompile.
- Add patch for PR33868, memory usage regression on glibc testsuite.
* Thu Oct 18 2007 rguenther@suse.de
- Add back patch for PR27880.  Fixes missing _Unwind_GetIPInfo
  symbol in static libgcc on ia64.
- Add patch for PR33168, fixes ppc64 bootstrap failure.
- Add patch for PR33608, fixes Ada build on ia64.  Re-enable Ada for ia64.
- Work around PR33857, allow building Ada with old version alone.
* Tue Oct 16 2007 rguenther@suse.de
- Update to SVN trunk head (r129378).
* Mon Oct 15 2007 rguenther@suse.de
- Update to SVN trunk head (r129351).
* Fri Oct 12 2007 rguenther@suse.de
- Package modified eclipse frontent jar.
- Remove ecj-bootstrap BuildRequires.
* Thu Oct 11 2007 rguenther@suse.de
- Update to SVN trunk head (r129228).
* Tue Oct  9 2007 rguenther@suse.de
- Update to SVN trunk head (r129166).
- Add patch for PR33692.
* Mon Oct  8 2007 rguenther@suse.de
- Update to SVN trunk head (r129130).
* Tue Oct  2 2007 rguenther@suse.de
- Update to SVN trunk head (r128956).
- Obsoletes fixes PR33146, PR33552, PR30375, PR33560 and linker
  version detection.
* Mon Oct  1 2007 rguenther@suse.de
- Disable ada for ia64.
* Thu Sep 27 2007 rguenther@suse.de
- Add patch for PR33552, also fixes reload failure for i586 glibc.
* Wed Sep 26 2007 rguenther@suse.de
- Add patch for PR30375 and PR33560.
* Mon Sep 24 2007 rguenther@suse.de
- Fix binutils version detection.
* Fri Sep 21 2007 rguenther@suse.de
- Update to SVN trunk head (r128649).
- Add patch for PR33146.
* Mon Sep 17 2007 rguenther@suse.de
- Update to SVN trunk head (r128539).
- Drop patch for PR32756.
- Revert r127960.
* Thu Aug 30 2007 rguenther@suse.de
- Add patch for PR32756.
* Fri Aug 24 2007 rguenther@suse.de
- Update to SVN trunk head (r127773).
* Fri Aug 24 2007 rguenther@suse.de
- Allow disabling of C++ and ObjC.
- Add rpmlint exceptions for libraries.
* Mon Aug 20 2007 rguenther@suse.de
- Delete fix-included sound headers.
* Wed Jul 25 2007 rguenther@suse.de
- Update with specfile changes that went into the gcc42 package.
- Update to SVN trunk head (r126913).
* Wed May  9 2007 rguenther@suse.de
- Update to SVN trunk head (r124570).
- Add unzip BuildRequires for libjava.
- No longer package precompiled headers for libstdc++.
* Tue Mar 27 2007 rguenther@suse.de
- Update to SVN trunk head (r123255).
- Remove %%versionspecificlibs stuff.
* Thu Nov 16 2006 rguenther@suse.de
- Update to SVN 4.2 branch head.
- Remove patch for PR26898 again.
* Fri Oct 20 2006 rguenther@suse.de
- Update to SVN 4.2 branch head, fixes some remaining aliasing miscompiles.
- Fix libgcj.pc install.
- Update AMD-0201 patch.
* Tue Oct 17 2006 rguenther@suse.de
- Update to SVN head.
- Add AMD-0201.
- Add other minor improvements.
* Tue Sep 19 2006 rguenther@suse.de
- New gcc42-gij package.
- Adjust libgcj so version.
- Add proper Obsoletes to virtual Provides and gcc41 packages to
  make rpm install work.
- Kill @gcc_suffix@.
* Mon Sep 18 2006 rguenther@suse.de
- Update to SVN head.
* Mon Sep  4 2006 rguenther@suse.de
- Fix libgcj packaging.
- Run the testsuite from the libgcj package and package results
  in gcc42-testresults.
* Tue Aug 29 2006 rguenther@suse.de
- Update to SVN head.
- Remove patch for bogus warning which is fixed upstream already.
- Fix URL for bugreporting, use http://bugs.opensuse.org instead of
  http://www.suse.de/feedback.
- No longer package gccbug script which led to feedback@suse.de which no
  longer exists.
* Tue Aug 22 2006 rguenther@suse.de
- Forward port spec file changes from gcc41 package.
- libobjc changed .so version.
* Tue Aug 15 2006 schwab@suse.de
- Fix infinite loop (PR28672).
* Tue Aug 15 2006 rguenther@suse.de
- New gcc42 package.
