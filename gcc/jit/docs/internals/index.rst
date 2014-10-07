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

Internals
=========

Using a working copy without installing every time
--------------------------------------------------
When directly working on the library you can avoid needing to install to
test every change.

You need to do a ``make install`` of the ``gcc`` subdirectory to install
the driver binary (the top-level ``gcc`` binary).  This is used internally
by the library for converting from .s assembler files to .so shared
libraries.  Specifically, it looks for an executable on the ``$PATH`` with
a name expanded by the ``configure`` script from
``${target_noncanonical}-gcc-${gcc_BASEVER}${exeext}``,
such as ``x86_64-unknown-linux-gnu-gcc-5.0.0``.

For example, if you configured with a prefix of ``$PREFIX`` like this:

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

then you can install (once) to ensure that ``$PREFIX/bin/`` is populated:

.. code-block:: console

  [build]$ ll ../install/bin/*gcc*
  -rwxr-xr-x. 3 david david 2733458 Oct  6 14:25 ../install/bin/gcc
  -rwxr-xr-x. 2 david david  136921 Oct  6 14:25 ../install/bin/gcc-ar
  -rwxr-xr-x. 2 david david  136857 Oct  6 14:25 ../install/bin/gcc-nm
  -rwxr-xr-x. 2 david david  136869 Oct  6 14:25 ../install/bin/gcc-ranlib
  -rwxr-xr-x. 3 david david 2733458 Oct  6 14:25 ../install/bin/x86_64-unknown-linux-gnu-gcc
  -rwxr-xr-x. 3 david david 2733458 Oct  6 14:25 ../install/bin/x86_64-unknown-linux-gnu-gcc-5.0.0
  -rwxr-xr-x. 2 david david  136921 Oct  6 14:25 ../install/bin/x86_64-unknown-linux-gnu-gcc-ar
  -rwxr-xr-x. 2 david david  136857 Oct  6 14:25 ../install/bin/x86_64-unknown-linux-gnu-gcc-nm
  -rwxr-xr-x. 2 david david  136869 Oct  6 14:25 ../install/bin/x86_64-unknown-linux-gnu-gcc-ranlib

Note the presence above of ``../install/bin/x86_64-unknown-linux-gnu-gcc``.

When building code using the API you need to ensure that ``-I`` points to
the directory containing ``libgccjit.h`` and ``-L`` points to the
directory containing the built library.

When running binaries (or debugging them), you'll need to manually set
``LD_LIBRARY_PATH`` to the directory containing ``libgccjit.so``, and
``PATH`` needs to contain the path to the installed binaries.

and then you can run from a built (but not installed) copy:

.. code-block:: console

  [gcc]$ PATH=../../install/bin:$PATH LD_LIBRARY_PATH=. ./testsuite/jit/test-factorial.exe

without needing to reinstall everything for every tweak to the library.

Running the test suite
----------------------

.. code-block:: console

  [build] $ cd gcc
  [gcc] $ make check-jit RUNTESTFLAGS="-v -v -v"

A summary of the tests can then be seen in:

.. code-block:: console

  jit/build/gcc/testsuite/jit/jit.sum

and detailed logs in:

.. code-block:: console

  jit/build/gcc/testsuite/jit/jit.log

The test executables can be seen as:

.. code-block:: console

  jit/build/gcc/testsuite/jit/*.exe

which can be run independently.

You can compile and run individual tests by passing "jit.exp=TESTNAME" to RUNTESTFLAGS e.g.:

.. code-block:: console

   [gcc] $ make check-jit RUNTESTFLAGS="-v -v -v jit.exp=test-factorial.c"

and once a test has been compiled, you can debug it directly:

.. code-block:: console

   [gcc] $ PATH=../../install/bin:$PATH \
           LD_LIBRARY_PATH=. \
             gdb --args \
               testsuite/jit/test-factorial.exe


Overview of code structure
--------------------------

* ``libgccjit.c`` implements the API entrypoints.  It performs error
  checking, then calls into classes of the gcc::jit::recording namespace
  within ``jit-recording.c`` and ``jit-recording.h``.

* The gcc::jit::recording classes (within ``jit-recording.c`` and
  ``jit-recording.h``) record the API calls that are made:

   .. literalinclude:: ../../jit-common.h
    :start-after: /* Recording types.  */
    :end-before: /* End of recording types. */
    :language: c++

* When the context is compiled, the gcc::jit::playback classes (within
  ``jit-playback.c`` and ``jit-playback.h``) replay the API calls
  within langhook:parse_file:

   .. literalinclude:: ../../jit-common.h
    :start-after: /* Playback types.  */
    :end-before: /* End of playback types. */
    :language: c++

   .. literalinclude:: ../../notes.txt
    :lines: 1-

Here is a high-level summary from ``jit-common.h``:

   .. literalinclude:: ../../jit-common.h
    :start-after: /* Summary.  */
    :end-before: namespace gcc {
    :language: c++
