/* Run-time support required by CIL binaries.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Roberto Costa
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

using System;
using System.Collections;
using System.Reflection;
using System.Runtime.InteropServices;

namespace gcc4net {
    public unsafe sealed class StartupHelper {

        public unsafe static IntPtr GetArgv(out int argc) {
            String[] argv = Environment.GetCommandLineArgs();
            argc = argv.Length;
            int ptr_size = (int)(sizeof(void*));
            int size = (argc + 1) * ptr_size;
            IntPtr res = Marshal.AllocHGlobal(size);

            for (int i=0; i < argc; ++i) {
                Marshal.WriteIntPtr(res, ptr_size * i,
                                    Marshal.StringToHGlobalAnsi(argv[i]));
            }
            Marshal.WriteIntPtr(res, ptr_size * argc, IntPtr.Zero);

            return res;
        }

        public unsafe static IntPtr GetEnvVars() {
            IDictionary env = Environment.GetEnvironmentVariables();
            int num_elems = env.Keys.Count;
            int ptr_size = (int)(sizeof(void*));
            int size = (num_elems + 1) * ptr_size;
            IntPtr res = Marshal.AllocHGlobal(size);

            int i = 0;
            foreach (DictionaryEntry de in env) {
                String str = de.Key + "=" + de.Value;
                Marshal.WriteIntPtr(res, ptr_size * i,
                                    Marshal.StringToHGlobalAnsi(str));
                ++i;
            }
            Marshal.WriteIntPtr(res, ptr_size * num_elems, IntPtr.Zero);

            return res;
        }

        public unsafe static void Startup() {
            Assembly assembly;
            MethodInfo initMethod = null;

            // Find the module that contains the "main" function.
            assembly = Assembly.GetEntryAssembly();

            Type type = assembly.GetType("<Module>");
            if (type != null)
                initMethod = type.GetMethod(".init");
            else {
                Module module = assembly.GetModules()[0];
                initMethod = module.GetMethod(".init");
            }

            // Invoke the application's ".init" function, if present.
            if(initMethod != null)
                initMethod.Invoke(null, null);
        }

        public unsafe static void Shutdown(int status) {
            try {
                Assembly assembly = Assembly.Load("libstd");
                object[] userParameters = new object[1];
                userParameters[0] = status;

                Type theType = assembly.GetType("libstd");
                MethodInfo theMethod = theType.GetMethod("exit");
                theMethod.Invoke(null, userParameters);
            } catch (Exception) { }

            Environment.Exit(status);
        }

    }

}
