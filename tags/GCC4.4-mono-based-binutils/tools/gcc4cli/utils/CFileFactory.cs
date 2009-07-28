
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;

namespace gcc4cli.util {

    public class CFileFactory {

        public static AssemblyDefinition LoadAssembly (string asm_name)
        {
            return AssemblyResolver.DefaultResolver.Resolve (asm_name);
        }

        public static bool CheckIsCLibrary (string asm_name)
        {
            AssemblyDefinition asm = AssemblyResolver.DefaultResolver.Resolve (asm_name);
            if (CFileAttributes.AssemblyIsCStaticLibrary (asm)
                || CFileAttributes.AssemblyIsCDynamicLibrary (asm))
                return true;
            return false;
        }

        public static CFile LoadCLibrary (string asm_name)
        {
            CFile result = null;
            AssemblyDefinition asm = AssemblyResolver.DefaultResolver.Resolve (asm_name);
            result = LoadCFile (asm);
            if (!(result is CStaticLibrary || result is CDynamicLibrary))
                throw new WrongFileFormatException ("Wrong Assembly Format, '" + asm_name + "' is not a C Library",
                                                    asm_name);
            return result;
        }

        public static bool CheckIsCFile (string asm_name)
        {
            AssemblyDefinition asm = AssemblyResolver.DefaultResolver.Resolve (asm_name);
            if (   CFileAttributes.AssemblyIsCObject (asm)
                || CFileAttributes.AssemblyIsCStaticLibrary (asm)
                || CFileAttributes.AssemblyIsCDynamicLibrary (asm)
                || CFileAttributes.AssemblyIsCExecutable (asm))
                return true;
            return false;
        }

        public static CFile LoadCFile (string asm_name)
        {
            AssemblyDefinition asm = AssemblyResolver.DefaultResolver.ResolveWithoutLibPath (asm_name);
            return LoadCFile (asm);
        }

        static CFile LoadCFile (AssemblyDefinition asm)
        {
            if (CFileAttributes.AssemblyIsCExecutable (asm)) {
                Tracer.Trace ("Loading a C Executable: " + asm.Name.Name);
                return new CExecutable (asm);
            } else if (CFileAttributes.AssemblyIsCObject (asm)) {
                Tracer.Trace ("Loading a C Object: " + asm.Name.Name);
                return new CObject (asm);
            } else if (CFileAttributes.AssemblyIsCDynamicLibrary (asm)) {
                Tracer.Trace ("Loading a C Dynamic Library: " + asm.Name.Name);
                return new CDynamicLibrary (asm);
            } else if (CFileAttributes.AssemblyIsCStaticLibrary (asm)) {
                Tracer.Trace ("Loading a C Static Library: " + asm.Name.Name);
                return new CStaticLibrary (asm);
            } else
                throw new WrongFileFormatException ("Assembly Format of '" + asm.Name.Name + "' is not a valid C file one",
                                                    asm.Name.Name);
        }

        public static CObject CreateCObject (string name)
        {
            return new CObject (name, AssemblyResolver.DefaultResolver);
        }
        public static CStaticLibrary CreateCStaticLibrary (string name)
        {
            return new CStaticLibrary (name, AssemblyResolver.DefaultResolver);
        }
        public static CDynamicLibrary CreateCDynamicLibrary (string name, bool multithreaded)
        {
            return new CDynamicLibrary (name, AssemblyResolver.DefaultResolver, multithreaded);
        }
        public static CExecutable CreateCExecutable (string name, bool multithreaded)
        {
            return new CExecutable (name, AssemblyResolver.DefaultResolver, multithreaded);
        }
    }

}
