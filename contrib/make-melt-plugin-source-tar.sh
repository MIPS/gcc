#! /bin/bash
## file contrib/make-melt-source-tar.sh of the MELT branch of GCC
## building the plugin source distribution
## first argument is GCC MELT source tree
## second argument is the basename of the tar ball
## following optional arguments are gengtype -r gengtype.state ...
##
##    Middle End Lisp Translator = MELT
##
##    Copyright (C)  2010 - 2016 Free Software Foundation, Inc.
##    Contributed by Basile Starynkevitch <basile@starynkevitch.net>
## 
## This file is part of GCC.
## 
## GCC is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3, or (at your option)
## any later version.
## 
## GCC is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with GCC; see the file COPYING3.   If not see
## <http://www.gnu.org/licenses/>.


## if the first argument is -l, we are using symlinks instead of copying

if [ "$1" = "-l" ]; then
    gccmelt_copy="ln -svf"
    shift
else
    gccmelt_copy="cp -av"
fi


## if the first argument is -s we are making a snapshot
if [ "$1" = "-s" ]; then
    gccmelt_snapshot=snapshot
    shift
fi

## the first argument of this script is the source tree of the GCC
## MELT branch from which is extracted the MELT plugin source, for
## instance /usr/src/Lang/gcc-melt
gccmelt_source_tree=$1

## the second argument of this script is a temporary directory
## basename for the plugin source, for instance /tmp/gcc-melt-plugin
## and this script will then make a directory /tmp/gcc-melt-plugin and
## a bzipped tar archive /tmp/gcc-melt-plugin.tar.bz2
gccmelt_tarbase=$2

if [ -z "$gccmelt_tarbase" ]; then
    echo usage: $0 "[-l|-s]" "<meltbranch-sourcetree> <output-tarbase>" >&2
    echo " " use " -l" for symlinks or " -s" for snapshots or nothing for release  >&2
    echo " " "<meltbranch-sourcetree> could be e.g. /usr/src/gcc-melt-branch" >&2
    echo " " "<output-tarbase> could be e.g. /tmp/melt-plugin" >&2
    exit 1
fi

shift 2
## the optional other arguments are used to invoke gengtype, for instance
## $(gcc-4.6 -print-file-name=gengtype) -v -r $(gcc-4.6 -print-file-name=gtype.state)
gengtype_prog=$1

if [ -n "$gengtype_prog" ]; then
    case $gengtype_prog in
	*gengtype*) 
	    shift 1; 
	    gengtype_args="$@";;
	*) 
	    echo $0: Bad optional gengtype $gengtype_prog >&2
	    exit 1
    esac
fi

if [ ! -f $gccmelt_source_tree/gcc/melt-runtime.h ]; then
    echo $0: Bad first argument for GCC MELT source tree $gccmelt_source_tree >&2
    exit 1
fi

if [ -z $gccmelt_tarbase ]; then
    echo $0 Bad second argument for GCC MELT tar ball base $gccmelt_tarbase >&2
    exit 1
fi

gccmelt_svnrev=$(svn info $gccmelt_source_tree | awk '/^Revision:/{print $2}')

if [ -n "$gccmelt_snapshot" ]; then
    case $gccmelt_svnrev in
	[1-9][0-9]*) gccmelt_snapshot="snap-svnrev-$gccmelt_svnrev";;
	*);;
    esac
fi

rm -rf $gccmelt_tarbase

mkdir -p $gccmelt_tarbase/melt/generated

date +"source tar timestamp %c %Z" > $gccmelt_tarbase/GCCMELT-SOURCE-DATE
if [ -n "$gccmelt_svnrev" ]; then
    echo "from $gccmelt_svnrev" >> $gccmelt_tarbase/GCCMELT-SOURCE-DATE
fi

copymelt() {
    if [ -f $gccmelt_source_tree/$1 ]; then
	$gccmelt_copy $gccmelt_source_tree/$1 $gccmelt_tarbase/$2
    fi
}

## just a reminder
echo $0: You should have recently run in gcc/ of build tree: make upgrade-warmelt 

mkdir $gccmelt_tarbase/testmelt

mkdir $gccmelt_tarbase/obsolete-melt

copymelt COPYING3
copymelt move-if-change
copymelt gcc/DATESTAMP GCCMELT-DATESTAMP
copymelt gcc/REVISION GCCMELT-REVISION

copymelt gcc/doc/include/fdl.texi 
copymelt gcc/doc/include/funding.texi 
copymelt gcc/doc/gnu.texi 
copymelt gcc/doc/include/gpl.texi 
copymelt gcc/doc/include/gpl_v3.texi 
copymelt gcc/doc/include/texinfo.tex
copymelt gcc/emit-libmelt-treecode.inc

for f in gcc/testsuite/melt/* ; do
   copymelt $f testmelt/
done

for f in gcc/obsolete-melt/* ; do
   copymelt $f obsolete-melt/
done

copymelt gcc/doc/melt.texi

copymelt contrib/melt-mv-if-changed.c 
copymelt contrib/meltplugin.texi 
copymelt contrib/meltpluginapi.texi

copymelt contrib/MELT-Plugin-Makefile
(cd $gccmelt_tarbase/; ln -s MELT-Plugin-Makefile Makefile)


copymelt gcc/make-melt-predefh.awk make-melt-predefh.awk
copymelt gcc/make-warmelt-predef.awk make-warmelt-predef.awk
copymelt gcc/melt-build-script.tpl
copymelt gcc/melt-build-script.def 
copymelt gcc/melt-build-script.sh
copymelt gcc/melt-runtime.cc
copymelt gcc/melt-runtime.h
copymelt gcc/melt-runtypes-?.h

for mf in $gccmelt_source_tree/gcc/melt/*.melt ; do 
    $gccmelt_copy $mf  $gccmelt_tarbase/melt/
done

for mf in $gccmelt_source_tree/gcc/melt/generated/*.{c,cc,h} ; do 
    $gccmelt_copy $mf  $gccmelt_tarbase/melt/generated/
done


for mf in $gccmelt_source_tree/gcc/*melt*  ; do
    case $mf in
	*~) ;;
	*) copymelt gcc/$(basename $mf) ;;
    esac
done


for cf in  $gccmelt_source_tree/contrib/*melt*.sh $gccmelt_source_tree/contrib/pygmentize-melt ; do
    copymelt contrib/$(basename $cf) 
    chmod a+x $gccmelt_tarbase/$(basename $cf)
done


copymelt INSTALL/README-MELT-PLUGIN

if [ -n "$gccmelt_snapshot" ]; then
    gccmelt_revision=$(tr -c -d '0-9'< $gccmelt_tarbase/GCCMELT-REVISION)
    gccmelt_origpat='#define *MELT_VERSION_STRING *"\([a-zA-Z0-9.-]*\)" *'
    gccmelt_replac="#define MELT_VERSION_STRING \"\\1-snap-r$gccmelt_revision\""
    echo gccmelt_origpat= $gccmelt_origpat
    echo gccmelt_replac= $gccmelt_replac
    echo gccmelt_revision= $gccmelt_revision
    sed  "s/$gccmelt_origpat/$gccmelt_replac/" < $gccmelt_tarbase/melt-runtime.h > $gccmelt_tarbase/melt-runtime.h-tmp
    mv  $gccmelt_tarbase/melt-runtime.h-tmp  $gccmelt_tarbase/melt-runtime.h
    grep "define *MELT_VERSION_STRING"  $gccmelt_tarbase/melt-runtime.h
fi

grep 'MELT_VERSION_STRING'  $gccmelt_tarbase/melt-runtime.h

eval $(awk '/MELT_VERSION_STRING/{printf "gccmelt_version_string=%s\n", $3;}'  $gccmelt_tarbase/melt-runtime.h)

echo gccmelt_version_string= $gccmelt_version_string

if [ -n "$gengtype_prog" ]; then
    gengtype_version=$($gengtype_prog -V | head -1 | awk '{print $NF}' 2>/dev/null)
    $gengtype_prog $gengtype_args -P $gccmelt_tarbase/gt-melt-runtime-$gengtype_version-plugin.h $gccmelt_tarbase/melt-runtime.h  $gccmelt_tarbase/melt/generated/meltrunsup.h
fi

tar -cjf $gccmelt_tarbase.tar.bz2 \
    --format=pax --owner=melt --group=gcc \
    --label="GCC-MELT-$gccmelt_version_string" \
   --exclude-backups --exclude='*~' --exclude='*%'  --exclude-vcs  \
   -C $(dirname $gccmelt_tarbase) $(basename $gccmelt_tarbase)

ls -l $gccmelt_tarbase.tar.bz2 
#eof make-melt-plugin-source-tar.sh
