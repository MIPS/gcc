#!/usr/bin/perl

# Filters out some of the #defines used thourghout the GCC sources:
# - GTY(()) marks declarations for gengtype.c
# - PARAMS(()) is used for K&R compatibility. See ansidecl.h.

undef $/;
$a = <>;
$a =~ s/GTY[ \t]*\(\(.*\)\)//g;
$a =~ s/PARAMS[ \t]*\(\((.*?)\)\);/\($1\);/sg;
print $a;
