# Run tests covering all config.gcc cases.
host_options='--with-mpc=/opt/cfarm/mpc' # gcc10
# v850e1-elf is rejected by config.sub
LIST = alpha-linux-gnu alpha-gnu alpha-freebsd6 alpha-netbsd alpha-openbsd \
  alpha-dec-osf5.1 alpha64-dec-vms alpha-dec-vms am33_2.0-linux arc-elf \
  arm-wrs-vxworks arm-freebsd6 arm-netbsdelf arm-netbsd arm-linux \
  arm-linux-androideabi arm-uclinux_eabi arm-ecos-elf arm-eabi \
  arm-symbianelf arm-rtems arm-elf arm-wince-pe arm-pe avr-rtems avr-elf \
  bfin-elf bfin-uclinux bfin-linux-uclibc bfin-rtems bfin-openbsd \
  cris-elf cris-linux crisv32-elf crisv32-linux crx-elf fido-elf \
  fr30-elf frv-elf frv-linux h8300-elf h8300-rtems hppa-linux-gnu \
  hppa-linux-gnu@--enable-sjlj-exceptions=yes hppa64-linux-gnu \
  hppa2.0-hpux10.1 hppa64-hpux11.3 \
  hppa64-hpux11.0@--enable-sjlj-exceptions=yes hppa2.0-hpux11.9 \
  i686-pc-linux-gnu i686-apple-darwin i686-apple-darwin9 i686-apple-darwin10 \
  i386-freebsd6.0_aout i486-freebsd4 i686-freebsd6 i686-kfreebsd-gnu i386-netbsd \
  i686-netbsdelf9 i686-knetbsd-gnu i686-openbsd i686-openbsd3.0 i686-elf \
  i686-kopensolaris-gnu i686-symbolics-gnu i686-pc-msdosdjgpp i686-lynxos \
  ix86-netware@--with-ld=nwld i686-nto-qnx i686-rtems \
  i686-solaris2.10@--enable-threads=solaris i686-wrs-vxworks \
  i686-wrs-vxworksae i686-pe@--enable-sjlj-exceptions=yes \
  i686-cygwin@--enable-threads=yes i686-mingw32crt i686-interix3 ia64-elf \
  ia64-freebsd6 ia64-linux ia64-hpux ia64-hp-vms iq2000-elf lm32-elf \
  lm32-rtems lm32-uclinux m32c-rtems m32c-elf m32r-elf m32rle-elf m32r-rtems \
  m32r-linux m32rle-linux m68hc11-elf m68hc12-elf m68k-elf m68k-netbsdef \
  m68k-openbsd m68k-uclinuxoldabi m68k-uclinux m68k-linux m68k-rtems \
  mcore-elf mcore-pe mep-elf microblaze-linux microblaze-elf \
  mips-sgi-irix6.5@--with-stabs@--enable-threads=posix mips-netbsd \
  mips64el-st-linux-gnu mips64octeon-linux mipsisa64r2-linux \
  mipsisa32r2-linux-gnu mips-openbsd mipsisa64r2-sde-elf mipsisa32-elfoabi \
  mipsisa64-elfoabi mipsisa64r2el-elf mipsisa64sr71k-elf mipsisa64sb1-elf \
  mipsel-elf mips64-elf mips64vr-elf mips64orion-elf mips-rtems \
  mips-wrs-vxworks mipstx39-elf mmix-knuth-mmixware mn10300-elf moxie-elf \
  moxie-uclinux moxie-rtems pdp11-aout picochip-elf powerpc-darwin8 \
  powerpc-darwin7 powerpc64-darwin powerpc-freebsd6 powerpc-netbsd \
  powerpc-eabispe powerpc-eabisimaltivec powerpc-eabisim ppc-elf \
  powerpc-eabialtivec powerpc-xilinx-eabi powerpc-eabi \
  powerpc-rtems4.11@--enable-threads=yes powerpc-linux_spe \
  powerpc-linux_paired powerpc64-linux_altivec powerpc64-darl-gnu \
  powerpc-gnu-gnualtivec powerpc-ungulate-gnu@--enable-threads=yes \
  powerpc-wrs-vxworks powerpc-wrs-vxworksae powerpc-lynxos powerpcle-elf \
  powerpcle-eabisim powerpcle-eabi rs6000-ibm-aix4.3 rs6000-ibm-aix5.1.0 \
  rs6000-ibm-aix5.2.0 rs6000-ibm-aix5.3.0 rs6000-ibm-aix6.0 ppc64-elf \
  rx-elf s390-linux-gnu s390x-linux-gnu s390x-ibm-tpf score-elf sh-elf \
  shle-linux sh-netbsd sh-superh-elf sh5el-netbsd sh64-netbsd sh64-linux \
  sh64-elf@--with-newlib sh-symbianelf sh-rtems sh-wrs-vxworks sparc-elf \
  sparc-leon-elf sparc-rtems sparc-linux-gnu \
  sparc-leon3-linux-gnu@--enable-target=all sparc-netbsdelf \
  sparc-sun-solaris2@--enable-threads=solaris \
  sparc64-sun-solaris2.10@--with-gnu-ld@--with-gnu-as@--enable-threads=posix \
  sparc-wrs-vxworks sparc64-elf sparc64-rtems sparc64-linux sparc64-freebsd6 \
  sparc64-netbsd sparc64-openbsd spu-elf v850e-elf v850-elf vax-linux-gnu \
  vax-netbsdelf vax-netbsd vax-openbsd x86_64-apple-darwin \
  x86_64-pc-linux-gnu@--with-fpmath=avx \
  x86_64-elf@--with-fpmath=sse x86_64-freebsd6 x86_64-netbsd \
  x86_64-knetbsd-gnu x86_64-w64-mingw32 \
  x86_64-mingw64-gnu@--enable-sjlj-exceptions=yes xstormy16-elf xtensa-elf \
  xtensa-linux

all: $(LIST)

.PHONEY: make-log-dir

empty=

#Check for the presence of the MAINTAINERS file to make sure we are in a
#suitable current working direcrory.
make-log-dir: ../gcc/MAINTAINERS
	mkdir log

$(LIST): make-log-dir
	-mkdir $@
	(cd $@ && \
	../../gcc/configure --target=$(subst @,$(empty) ,$@) \
	--enable-werror-always ${host_options} --enable-languages=all,ada,go) \
	> log/$@-config.out 2>&1
	-$(MAKE) -C $@ all-gcc > log/$@-make.out 2>&1 && rm -r $@
