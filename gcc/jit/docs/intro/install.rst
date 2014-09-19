.. Copyright (C) 2014 Free Software Foundation, Inc.
   Originally contributed by David Malcolm <dmalcolm@redhat.com>

   This is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see
   <http://www.gnu.org/licenses/>.

.. default-domain:: c

Getting Started
---------------

Installation via packages
=========================

It's easiest to use pre-built packages of the library.

Fedora and RHEL
***************
RPM packages of libgccjit (and its Python 2 and 3 bindings) are
available for Fedora and RHEL, for i386 and x86_64.

These currently should be treated as experimental.

See https://copr.fedoraproject.org/coprs/dmalcolm/libgccjit/
for information on subscribing to the appropriate repository for
your system.  Having done this,

.. code-block:: bash

  sudo yum install libgccjit-devel

should give you both the JIT library (`libgccjit`) and the header files
needed to develop against it (`libgccjit-devel`):

.. code-block:: console

  $ rpm -qlv libgccjit
  lrwxrwxrwx    1 root    root                       18 Aug 12 07:56 /usr/lib64/libgccjit.so.0 -> libgccjit.so.0.0.1
  -rwxr-xr-x    1 root    root                 14463448 Aug 12 07:57 /usr/lib64/libgccjit.so.0.0.1
  $ rpm -qlv libgccjit-devel
  -rwxr-xr-x    1 root    root                    37654 Aug 12 07:56 /usr/include/libgccjit++.h
  -rwxr-xr-x    1 root    root                    28967 Aug 12 07:56 /usr/include/libgccjit.h
  lrwxrwxrwx    1 root    root                       14 Aug 12 07:56 /usr/lib64/libgccjit.so -> libgccjit.so.0


Other distributions
*******************

Prebuilt packages for other distributions would be most welcome; please
contact the `jit mailing list`_.


Installation from source
========================
If pre-built packages are not available, you can built the library from
source.  Doing so currently requires about 4.2G of drive space (for
the combination of the source tree, the build directory, and the
installation path).

The code can currently be seen within the git branch "dmalcolm/jit" here:
  http://gcc.gnu.org/git/?p=gcc.git;a=shortlog;h=refs/heads/dmalcolm/jit

The following will check out a copy of the appropriate branch, to the
"jit/src" subdirectory:

.. code-block:: bash

  mkdir jit
  cd jit
  git clone \
      -b dmalcolm/jit \
       git://gcc.gnu.org/git/gcc.git \
       src

The source tree currently occupies about 2.8G of disk space.

To build it (within the "jit/build" subdirectory, installing to
"jit/install"):

.. code-block:: bash

  mkdir build
  mkdir install
  PREFIX=$(pwd)/install
  cd build
  ../src/configure \
     --enable-host-shared \
     --enable-languages=jit \
     --disable-bootstrap \
     --enable-checking=release \
     --prefix=$PREFIX
  nice make -j4 # altering the "4" to however many cores you have

On my 4-core laptop this takes 17 minutes and 1.1G of disk space
(it's much faster with many cores and a corresponding -j setting).

This should build a libgccjit.so within jit/build/gcc:

.. code-block:: console

 [build] $ file gcc/libgccjit.so*
 gcc/libgccjit.so:       symbolic link to `libgccjit.so.0'
 gcc/libgccjit.so.0:     symbolic link to `libgccjit.so.0.0.1'
 gcc/libgccjit.so.0.0.1: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, not stripped

Note that this is a branch of GCC, so if it fails to build, you might want
to consult the `general GCC FAQ <https://gcc.gnu.org/wiki/FAQ>`_  for some
common issues, before checking on the `jit mailing list`_.

.. _jit mailing list: https://gcc.gnu.org/ml/jit/

You should then be able to install it (to the `--prefix` specified
earlier) via:

.. code-block:: bash

  make install

On my laptop this uses a further 0.4G of disk space.

You should be able to see the header files within the `include`
subdirectory of the installation prefix:

.. code-block:: console

  $ find $PREFIX/include
  /home/david/gcc-jit/install/include
  /home/david/gcc-jit/install/include/libgccjit.h
  /home/david/gcc-jit/install/include/libgccjit++.h

and the library within the `lib` subdirectory:

.. code-block:: console

  $ find $PREFIX/lib/libgccjit.*
  /home/david/gcc-jit/install/lib/libgccjit.so
  /home/david/gcc-jit/install/lib/libgccjit.so.0
  /home/david/gcc-jit/install/lib/libgccjit.so.0.0.1


"Hello world"
=============

Let's look at how to build and run programs that use the library.

Here's a toy "hello world" program that uses the library to synthesize
a call to `printf` and use it to write a message to stdout.

   .. literalinclude:: ../examples/install-hello-world.c
    :language: c

Copy it to `jit-hello-world.c`.

To build it with prebuilt packages, use:

.. code-block:: console

  $ gcc \
      jit-hello-world.c \
      -o jit-hello-world \
      -lgccjit

  # Run the built program:
  $ ./jit-hello-world
  hello world


If building against an locally-built install (to `$PREFIX`), you can use
`pkg-config <http://www.freedesktop.org/wiki/Software/pkg-config/>`_ to
specify the compilation and linkage flags:

.. code-block:: console

  $ export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
  $ gcc \
      jit-hello-world.c \
      -o jit-hello-world \
      $(pkg-config libgccjit --cflags --libs)

and ensure that `LD_LIBRARY_PATH` is set appropriate when running the
built program, so that it can locate and dynamically link against
`libgccjit.so`:

.. code-block:: console

  # Run the built program:
  $ export LD_LIBRARY_PATH=$PREFIX/lib
  $ ./jit-hello-world
  hello world

This is equivalent to handcoding the include and library paths with `-I`
and `-L` and specifying `-lgccjit` (i.e. linkage against libgccjit):

.. code-block:: console

  $ gcc \
      jit-hello-world.c \
      -o jit-hello-world \
      -lgccjit \
      -I$PREFIX/include -L$PREFIX/lib

  $ LD_LIBRARY_PATH=$PREFIX/lib ./jit-hello-world
  hello world
