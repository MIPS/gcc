#!/bin/sh


INSTALL_GCC=yes

TARGET=cil32

BUILD_DIR=${BUILD_DIR-`pwd`}
SOURCE_DIR=${SOURCE_DIR-`dirname $0`}
SOURCE_DIR=`cd $SOURCE_DIR; pwd`
PREFIX=${PREFIX-`pwd`/newbuild}
SYSROOT=${PREFIX}/${TARGET}
LANGUAGES=c

_help()
{
    echo "Options:"
    echo "         -nogcc : build only binutils (no compiler)"
    echo "         -cpp   : build also C++ compiler"
    echo "         -help  : this message"
}

while [ $# -gt 0 ] ; do

if    [ "x$1" == x-help ] ; then
    _help ;
    exit 0
elif  [ "x$1" == x-cpp ] ; then
    LANGUAGES=c,c++
elif  [ "x$1" == x-nogcc ] ; then
    INSTALL_GCC=no
else
    echo "Unrecognized option: " $1
    _help ;
    exit 1
fi

shift 1
done

echo "DIRECTORIES:"
echo "  BUILD_DIR:  " ${BUILD_DIR}
echo "  SOURCE_DIR: " ${SOURCE_DIR}
echo "  SYSROOT:    " ${SYSROOT}
echo "  PREFIX:     " ${PREFIX}


PATH="${PREFIX}/bin:${PATH}"
export PATH

# Create directories

mkdir -p ${PREFIX}
mkdir -p ${PREFIX}/bin
mkdir -p ${PREFIX}/include
mkdir -p ${PREFIX}/lib
# SYSROOT must be = ${PREFIX}/${TARGET}
mkdir -p ${SYSROOT}
mkdir -p ${SYSROOT}/bin
( cd ${SYSROOT} ; ln -fs ../include . ) 
( cd ${SYSROOT} ; ln -fs ../lib . )  

# Install Binutils

CIL_AS=${CIL_AS-`which ilasm`}
CIL_LD=${CIL_LD-`which ilalink`}

echo "Tools:"
echo "  CIL ASSEMBLER: " ${CIL_AS}
echo "  CIL LINKER:    " ${CIL_LD}

echo
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

echo
echo "Installing vm wrappers"

cat > ${PREFIX}/bin/${TARGET}-ilrun <<_EOF_
#!/bin/sh

ilrun -L ${PREFIX}/lib "\$@"
_EOF_
chmod 755 ${PREFIX}/bin/${TARGET}-ilrun

cat > ${PREFIX}/bin/${TARGET}-mono <<_EOF_
#!/bin/sh

export MONO_PATH=${PREFIX}/lib

mono "\$@" 
_EOF_
chmod 755 ${PREFIX}/bin/${TARGET}-mono

# ... To Be Completed with needed binutil tools
 
# ... To Be Completed with libraries

if test "x${INSTALL_GCC}" == "xno"
then
    exit 0
fi

# Configure gcc

echo
echo "Configuring GCC"

mkdir -p ${BUILD_DIR}/build-gcc
cd ${BUILD_DIR}/build-gcc

${SOURCE_DIR}/configure --target=${TARGET} \
                        --enable-languages=${LANGUAGES} \
                        --disable-bootstrap \
                        --prefix=${PREFIX} \
                        --with-local-prefix=${SYSROOT}

# Build and install gcc

echo
echo "Building GCC"
make all

echo
echo "Installing GCC"
make install
