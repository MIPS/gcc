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

Using a working copy without installing
---------------------------------------
When directly working on the library you can avoid needing to install to
test changes.

When building code using the API you need to ensure that ``-I`` points to
the directory containing ``libgccjit.h`` and ``-L`` points to the
directory containing the built library.

You'll need to manually set ``LD_LIBRARY_PATH`` to the directory containing
``libgccjit.so`` when running binaries (or debugging them).

Running the test suite
----------------------

.. code-block:: console

  [build] $ cd gcc
  [gcc] $ make check-parallel-jit RUNTESTFLAGS="-v -v -v"

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

   [gcc] $ make check-parallel-jit RUNTESTFLAGS="-v -v -v jit.exp=test-factorial.c"

and once a test has been compiled, you can debug it directly:

.. code-block:: console

   [gcc] $ LD_LIBRARY_PATH=. gdb testsuite/jit/test-factorial.exe


Overview of code structure
--------------------------

* ``libgccjit.c`` implements the API entrypoints.  It performs error
  checking, then calls into classes of the gcc::jit::recording namespace
  within internal-api.c and internal-api.h.

* The gcc::jit::recording classes (within ``internal-api.c`` and
  ``internal-api.h``) record the API calls that are made:

   .. literalinclude:: ../../internal-api.h
    :start-after: /* Recording types.  */
    :end-before: /* End of recording types. */
    :language: c++

* When the context is compiled, the gcc::jit::playback classes (also
  within ``internal-api.c`` and ``internal-api.h``) replay the API calls
  within langhook:parse_file:

   .. literalinclude:: ../../internal-api.h
    :start-after: /* Playback types.  */
    :end-before: /* End of playback types. */
    :language: c++

   .. literalinclude:: ../../notes.txt
    :lines: 1-

Here is a high-level summary from ``internal-api.h``:

   .. literalinclude:: ../../internal-api.h
    :start-after: /* Summary.  */
    :end-before: namespace gcc {
    :language: c++
