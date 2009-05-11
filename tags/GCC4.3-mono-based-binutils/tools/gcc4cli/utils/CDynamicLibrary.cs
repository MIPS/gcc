
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;

namespace gcc4cli.util {

    public sealed class CDynamicLibrary : CFileBuilderWithRuntime {

        internal CDynamicLibrary (string name, AssemblyResolver asm_resolver, bool multithreaded)
            : base (name, AssemblyKind.Dll, asm_resolver, CFileAttributes.cdlib, multithreaded, false, false)
        {
            MainType = new TypeDefinition (name,
                                           string.Empty,
                                           TypeAttributes.Public | TypeAttributes.BeforeFieldInit,
                                           Module.Import (typeof(Object)));
            Module.Types.Add (MainType);
        }
        internal CDynamicLibrary (AssemblyDefinition assembly)
            : base (assembly, CFileAttributes.cdlib, false, false)
        {
            MainType = Module.Types[Assembly.Name.Name];
            FillTables ();
            CheckInitFini ();
        }

        public override void StartBuilding ()
        {
            if (!NeedsInitFini ())
                throw new Exception ("Cannot add new objects to file, runtime already linked in");
            base.StartBuilding ();
            AddInitFini ();
        }

        public override void EndBuilding ()
        {
            if (Building) {
                fixReferences ();
                BuildInitFini ();
                base.EndBuilding ();
            }
        }
    }
}
