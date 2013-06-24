#! /bin/sh -x

# Copyright (C) 2013 Free Software Foundation, Inc.
# This file melt-upgrade.sh is part of GCC.

# GCC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.

# GCC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.  

# invoked from Makefile target upgrade-warmelt as
## $(SHELL) $(srcdir)/melt-upgrade.sh meltbuild-sources $(srcdir)

meltbuild_sourcedir=$1
meltsource_dir=$2


## should be single word programs
melt_unifdef=unifdef
melt_astyle=astyle
melt_tempsuffix=tmp$$

if [ ! -d $meltbuild_sourcedir ]; then
    echo MELT-upgrade bad generated source directory $meltbuild_sourcedir 1>&2
    exit 1
fi

if [ ! -d $meltsource_dir ]; then
    echo MELT-upgrade bad source directory to upgrade $meltsource_dir 1>&2
    exit 1
fi


## check that we have some non-empty files
for f in  $meltbuild_sourcedir/generated/meltrunsup.h \
  $meltbuild_sourcedir/generated/meltrunsup-inc.cc \
  $meltbuild_sourcedir/warmelt-first.cc \
  $meltbuild_sourcedir/warmelt-first+meltdesc.c \
    ; do
  if [ ! -s "$f" ]; then
     ls -l $f
     ls -Ll $f
     echo MELT-upgrade without file $f  1>&2
    exit 1
  fi
done


## check that we have both unidef and astyle
wf=$(which  "$melt_unifdef")
if [ -s "$wf" ] ; then
    echo MELT-upgrade using unifdef from $wf
else
    echo MELT-upgrade need unifdef program $melt_unifdef 1>&2
    exit 1
fi

wf=$(which "$melt_astyle")
if [ -s "$wf" ] ; then
    echo MELT-upgrade using astyle from $wf
else
    echo MELT-upgrade need astyle program $melt_astyle 1>&2
    exit 1
fi

## upgrade the runtime
for f in $meltbuild_sourcedir/generated/meltrunsup.h  $meltbuild_sourcedir/generated/meltrunsup-inc.cc ; do
    bf=$(basename $f)
    ls -lL $f
    $melt_unifdef -DMELTGCC_NOLINENUMBERING < $f > "$bf-unif-$melt_tempsuffix" 
    if [ ! -s "$bf-unif-$melt_tempsuffix" ]; then
	echo MELT-upgrade failed to unifdef runtime $f
	exit 1
    fi
    $melt_astyle --style=gnu < "$bf-unif-$melt_tempsuffix"  > "$meltsource_dir/melt/generated/$bf-$melt_tempsuffix"
    if [ ! -s "$meltsource_dir/$bf-$melt_tempsuffix" ]; then
	echo MELT-upgrade failed to upgrade runtime file $f 1>&2
	exit 1
    else
	echo MELT-upgrade is upgrading "$meltsource_dir/melt/generated/$bf"
	mv "$meltsource_dir/melt/generated/$bf-$melt_tempsuffix"  "$meltsource_dir/melt/generated/$bf"
    fi
done

## upgrade the translator
for df in $meltbuild_sourcedir/warmelt*+meltdesc.c ; do
    if [ ! -s "$df" ]; then
	echo MELT-upgrade descriptor file $df does not exist 1>&2
	exit 1
    fi
    bs=$(basename "$df" +meltdesc.c)
    echo MELT-upgrade processing translator file $bs
    tf=$meltbuild_sourcedir/$bs+melttime.h
    if [ ! -s "$tf" ]; then
	echo MELT-upgrade timestamp file $tf does not exist 1>&2
	exit 1
    fi
    ## copy the generated sources first
    for f in $meltbuild_sourcedir/$bs.cc $meltbuild_sourcedir/$bs+[0-9][0-9].cc ; do
	bf=$(basename $f)
	$melt_unifdef -DMELTGCC_NOLINENUMBERING < $f > "$bf-unif-$melt_tempsuffix"
	if | ! -s  "$bf-unif-$melt_tempsuffix" ]; then
	    echo MELT-upgrade failed to unifdef translator file $f 1>&2
	    exit 1
	fi
	$melt_astyle --style=gnu < "$bf-unif-$melt_tempsuffix" > "$meltsource_dir/melt/generated/$bf-$melt_tempsuffix"
	if [ ! -s "$meltsource_dir/melt/generated/$bf-$melt_tempsuffix" ]; then
	    echo MELT-upgrade failed to upgrade translator file $f 1>&2
	    exit 1
	else
	    echo MELT-upgrade is upgrading "$meltsource_dir/melt/generated/$bf"
	    mv "$meltsource_dir/melt/generated/$bf-$melt_tempsuffix"  "$meltsource_dir/melt/generated/$bf"
	fi	
    done
    ## copy verbatim without indentation the descriptor and timestamp files at last
    cp $df "$meltsource_dir/melt/generated/$bf+meltdesc.c-$melt_tempsuffix"
    if [ ! -s "$meltsource_dir/melt/generated/$bf+meltdesc.c-$melt_tempsuffix" ]; then
	echo MELT-upgrade failed to upgrade descriptor file $meltsource_dir/melt/generated/$bf+meltdesc.c 1>&2
	exit 1
    else
	echo MELT-upgrade is upgrading descriptor $meltsource_dir/melt/generated/$bf+meltdesc.c
	mv  "$meltsource_dir/melt/generated/$bf+meltdesc.c-$melt_tempsuffix" "$meltsource_dir/melt/generated/$bf+meltdesc.c"
    fi
    cp $tf "$meltsource_dir/melt/generated/$bf+melttime.h-$melt_tempsuffix"
    if [ ! -s "$meltsource_dir/melt/generated/$bf+melttime.h-$melt_tempsuffix"  ]; then
	echo MELT-upgrade failed to upgrade timestamp file "$meltsource_dir/melt/generated/$bf+melttime.h" 1>&2
	exit 1
    else
	echo MELT-upgrade is upgrading timestamp file "$meltsource_dir/melt/generated/$bf+melttime.h"
	mv "$meltsource_dir/melt/generated/$bf+melttime.h-$melt_tempsuffix"  "$meltsource_dir/melt/generated/$bf+melttime.h"
    fi
done

date + "MELT-upgrade completed upgrade from $meltbuild_sourcedir to $meltsource_dir/melt/ at %D %T %Z %n"
# eof melt-upgrade.sh
