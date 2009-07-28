
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public sealed class CExecutable : CFileBuilderWithRuntime {

        internal CExecutable (string name, AssemblyResolver asm_resolver, bool multithreaded)
            : base (name, AssemblyKind.Console, asm_resolver, CFileAttributes.cexe, multithreaded, false, true)
        {
            MainType = new TypeDefinition (name,
                                           string.Empty,
                                           TypeAttributes.Public | TypeAttributes.BeforeFieldInit,
                                           Module.Import(typeof(Object)));
            Module.Types.Add (MainType);
        }
        internal CExecutable (AssemblyDefinition assembly)
            : base (assembly, CFileAttributes.cexe, false, true)
        {
            MainType = Module.Types[Assembly.Name.Name];
            FillTables ();
            CheckInitFini ();
            CheckStart ();
        }

        public override void StartBuilding ()
        {
            if (!NeedsInitFini () || !NeedsStart ())
                throw new Exception ("Cannot add new objects to file, runtime already linked in");
            base.StartBuilding ();
            AddInitFini ();
            AddStart ();
        }

        public override void EndBuilding ()
        {
            if (Building) {
                fixReferences ();
                BuildInitFini ();
                BuildStart ();
                base.EndBuilding ();
            }
        }
    }
}
