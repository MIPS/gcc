#!/bin/bash


INSTALL_BINUTILS=yes
INSTALL_GCC=yes

TARGET=cil32

BUILD_DIR=${BUILD_DIR-`pwd`}
SOURCE_DIR=${SOURCE_DIR-`dirname $0`}
PREFIX=${PREFIX-`pwd`/newbuild}
LANGUAGES=c

_help()
{
    echo "Options:"
    echo "         -src        : dir with the sources (default ${SOURCE_DIR})"
    echo "         -build      : dir where to the the build (default ${BUILD_DIR})"
    echo "         -prefix     : dir where to install (default ${PREFIX})"
    echo "         -binutils   : install of binutils (default)"
    echo "         -nobinutils : skip install of binutils"
    echo "         -gcc        : gcc build and installation (default)"
    echo "         -nogcc      : skip gcc build and installation"
    echo "         -cpp        : build also C++ compiler"
    echo "         -help       : this message"
}

# Create directories
_prepare_dirs()
{
    mkdir -p ${PREFIX}
    mkdir -p ${PREFIX}/bin
    mkdir -p ${PREFIX}/include
    mkdir -p ${PREFIX}/lib
    # SYSROOT must be = ${PREFIX}/${TARGET}
    mkdir -p ${SYSROOT}
    mkdir -p ${SYSROOT}/bin
    ( cd ${SYSROOT} ; ln -fs ../include . ) 
    ( cd ${SYSROOT} ; ln -fs ../lib . )  
}

# Install Binutils
_binutils()
{
    CIL_AS=${CIL_AS-`which ilasm.pnet`}
    CIL_LD=${CIL_LD-`which ilalink`}

    echo "Tools:"
    echo "  CIL ASSEMBLER: " ${CIL_AS}
    echo "  CIL LINKER:    " ${CIL_LD}

    if [ "x$CIL_AS" == x ] ; then
        echo "Error I cannot find DotGnu ilasm, you can specify it by defining an env variable called CIL_AS"
        exit 1
    fi

    echo ""
    echo "Installing Bin Utils"

    cat > ${SYSROOT}/bin/tool_not_exist <<_EOF_
#!/bin/sh
echo "Tool " \`basename \$0\` " not provided"
exit 1
_EOF_
    chmod 755 ${SYSROOT}/bin/tool_not_exist

    cat > ${SYSROOT}/bin/as <<_EOF_
#!/bin/sh

${CIL_AS} "\$@" 
_EOF_
    chmod 755 ${SYSROOT}/bin/as

    cat > ${SYSROOT}/bin/ld <<_EOF_
#!/bin/sh

PARAM=

for i in "\$@" ; do
    case "\$i" in
        -lm)
         ;;
        /tmp*)
            PARAM="\${PARAM} /\${i}";
         ;;
        /exchange*)
            PARAM="\${PARAM} /\${i}";
         ;;
        *)
            PARAM="\${PARAM} \${i}";
         ;;
    esac
done

${CIL_LD} \$PARAM

_EOF_
    chmod 755 ${SYSROOT}/bin/ld

    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/addr2line
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/ar
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/nm
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/objcopy
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/objdump
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/ranlib
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/readelf
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/size
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/strings
    cp ${SYSROOT}/bin/tool_not_exist ${SYSROOT}/bin/strip


    cp ${SYSROOT}/bin/addr2line ${PREFIX}/bin/${TARGET}-addr2line
    cp ${SYSROOT}/bin/ar        ${PREFIX}/bin/${TARGET}-ar
    cp ${SYSROOT}/bin/as        ${PREFIX}/bin/${TARGET}-as
    cp ${SYSROOT}/bin/ld        ${PREFIX}/bin/${TARGET}-ld
    cp ${SYSROOT}/bin/nm        ${PREFIX}/bin/${TARGET}-nm
    cp ${SYSROOT}/bin/objcopy   ${PREFIX}/bin/${TARGET}-objcopy
    cp ${SYSROOT}/bin/objdump   ${PREFIX}/bin/${TARGET}-objdump
    cp ${SYSROOT}/bin/ranlib    ${PREFIX}/bin/${TARGET}-ranlib
    cp ${SYSROOT}/bin/readelf   ${PREFIX}/bin/${TARGET}-readelf
    cp ${SYSROOT}/bin/size      ${PREFIX}/bin/${TARGET}-size
    cp ${SYSROOT}/bin/strings   ${PREFIX}/bin/${TARGET}-strings
    cp ${SYSROOT}/bin/strip     ${PREFIX}/bin/${TARGET}-strip
    
    echo ""
    echo "Installing vm wrappers"

    cat > ${PREFIX}/bin/${TARGET}-ilrun <<_EOF_
#!/bin/sh

basedir=\`dirname \$0\`
basedir=\`cd \$basedir/.. ; pwd\`

ilrun -L \$basedir/lib "\$@"
_EOF_
    chmod 755 ${PREFIX}/bin/${TARGET}-ilrun

    cat > ${PREFIX}/bin/${TARGET}-mono <<_EOF_
#!/bin/sh

basedir=\`dirname \$0\`
basedir=\`cd \$basedir/.. ; pwd\`

MONO_PATH=\$basedir/lib mono "\$@" 
_EOF_
    chmod 755 ${PREFIX}/bin/${TARGET}-mono
}

# Configure, build and install gcc

_gcc()
{
    echo ""
    echo "Configuring GCC"

    mkdir -p ${BUILD_DIR}/build-gcc
    cd ${BUILD_DIR}/build-gcc

    ${SOURCE_DIR}/configure --target=${TARGET} \
                            --enable-languages=${LANGUAGES} \
                            --disable-bootstrap \
                            --prefix=${PREFIX} \
                            --with-local-prefix=${SYSROOT}

    echo ""
    echo "Building GCC"
    make all

    echo ""
    echo "Installing GCC"
    make install
}

while [ $# -gt 0 ] ; do

if    [ "x$1" == x-help ] ; then
    _help ;
    exit 0
elif  [ "x$1" == x-cpp ] ; then
    LANGUAGES=c,c++
elif  [ "x$1" == x-nobinutils ] ; then
    INSTALL_BINUTILS=no
elif  [ "x$1" == x-nogcc ] ; then
    INSTALL_GCC=no
elif  [ "x$1" == x-binutils ] ; then
    INSTALL_BINUTILS=yes
elif  [ "x$1" == x-gcc ] ; then
    INSTALL_GCC=yes
elif  [ "x$1" == x-src ] ; then
    SOURCE_DIR=$2
    shift 1
elif  [ "x$1" == x-build ] ; then
    BUILD_DIR=$2
    shift 1
elif  [ "x$1" == x-prefix ] ; then
    PREFIX=$2
    shift 1
else
    echo "Unrecognized option: " $1
    _help ;
    exit 1
fi

shift 1
done

SOURCE_DIR=`cd $SOURCE_DIR; pwd`
SYSROOT=${PREFIX}/${TARGET}

echo "DIRECTORIES:"
echo "  BUILD_DIR:  " ${BUILD_DIR}
echo "  SOURCE_DIR: " ${SOURCE_DIR}
echo "  SYSROOT:    " ${SYSROOT}
echo "  PREFIX:     " ${PREFIX}

PATH="${PREFIX}/bin:${PATH}"
export PATH

_prepare_dirs

if test "x${INSTALL_BINUTILS}" == "xyes"
then
    _binutils
fi


if test "x${INSTALL_GCC}" == "xyes"
then
    _gcc
fi
