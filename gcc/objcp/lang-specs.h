/* Definitions for specs for Objective-C++.
   Copyright (C) 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Ziemowit Laski  <zlaski@apple.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* This is the contribution to the `default_compilers' array in gcc.c for
   g++.  */

#ifndef OBJCPLUSPLUS_CPP_SPEC
#define OBJCPLUSPLUS_CPP_SPEC 0
#endif

  {".mm",  "@objective-c++", 0},
  {".M",   "@objective-c++", 0},
  {"@objective-c++-header",
    "%{E|M|MM:cc1objplus -E %(cpp_options) %2 %(cpp_debug_options)}\
     %{!E:%{!M:%{!MM:\
       %{save-temps|no-integrated-cpp:cc1objplus -E\
		%(cpp_options) %2 -o %{save-temps:%b.mii} %{!save-temps:%g.mii} \n}\
      cc1objplus %{save-temps|no-integrated-cpp:-fpreprocessed %{save-temps:%b.mii} %{!save-temps:%g.mii}}\
	      %{!save-temps:%{!no-integrated-cpp:%(cpp_unique_options)}}\
	%(cc1_options) %2 %{+e1*}\
        -o %g.s %{!o*:--output-pch=%i.gch} %W{o*:--output-pch=%*}%V}}}",
     OBJCPLUSPLUS_CPP_SPEC},
  {"@objective-c++",
    "%{E|M|MM:cc1objplus -E %(cpp_options) %2 %(cpp_debug_options)}\
     %{!E:%{!M:%{!MM:\
       %{save-temps|no-integrated-cpp:cc1objplus -E\
		%(cpp_options) %2 -o %{save-temps:%b.mii} %{!save-temps:%g.mii} \n}\
      cc1objplus %{save-temps|no-integrated-cpp:-fpreprocessed %{save-temps:%b.mii} %{!save-temps:%g.mii}}\
	      %{!save-temps:%{!no-integrated-cpp:%(cpp_unique_options)}}\
	%(cc1_options) %2 %{+e1*}\
       %{!fsyntax-only:%(invoke_as)}}}}",
     OBJCPLUSPLUS_CPP_SPEC},
  {".mii", "@objective-c++-cpp-output", 0},
  {"@objective-c++-cpp-output",
   "%{!M:%{!MM:%{!E:\
    cc1objplus -fpreprocessed %i %(cc1_options) %2 %{+e*}\
    %{!fsyntax-only:%(invoke_as)}}}}", 0},
