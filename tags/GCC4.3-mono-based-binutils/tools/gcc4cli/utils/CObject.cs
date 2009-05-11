
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public sealed class CObject : CFileBuilder {

        internal CObject (AssemblyDefinition assembly)
            : base (assembly, CFileAttributes.cobj, true, true)
        {
            MainType = Module.Types["<Module>"];
            FillTables ();
            if (ref_types.Contains ("<ModuleExtern>"))
                fix_DOTGNU_Errors ();
        }

        internal CObject (string name, AssemblyResolver asm_resolver)
            : base (name, AssemblyKind.Dll, asm_resolver, CFileAttributes.cobj, true, true)
        { 
            // FIXME: should I use string.Empty for Name?
            MainType = new TypeDefinition ("<Module>",
                                           string.Empty,
                                           TypeAttributes.Public | TypeAttributes.BeforeFieldInit,
                                           Module.Import(typeof(Object)));
            Module.Types.Add (MainType);
        }

        void fix_DOTGNU_Errors ()
        {
            foreach (MethodDefinition method in MainType.Methods) {
                foreach (Instruction instr in method.Body.Instructions) {
                    if (   instr.OpCode.OperandType == OperandType.InlineMethod
                        || (instr.OpCode.OperandType == OperandType.InlineTok && instr.Operand is MethodReference)) {
                        MethodReference mref = instr.Operand as MethodReference;
                        if (mref.DeclaringType.Name == "<ModuleExtern>") {
                            string m_name = mref.Name;
                            if (def_symbols.Contains (m_name)) {
                                mref.DeclaringType = ((MethodReference)def_symbols[m_name]).DeclaringType;
                            }
                            else if (private_symbols.Contains (m_name)) {
                                mref.DeclaringType = ((MethodReference)private_symbols[m_name]).DeclaringType;
                            }
                            else
                                throw new Exception ("Error in hack for DOTGNU");
                        }
                    }
                }
            }
            ref_types.Remove ("<ModuleExtern>");
        }

        static int renamer_id = 0;

        internal void RenamePrivates ()
        {
            Hashtable new_private_symbols = new Hashtable ();
            int id = ++renamer_id;
            foreach (MemberReference member in private_symbols.Values) {
                string old_name = member.Name;
                string new_name = "Obj?" + id + "?" + old_name;
                member.Name = new_name;
                new_private_symbols[new_name] = member;
            }
            private_symbols = new_private_symbols;
        }

        public override void EndBuilding ()
        {
            if (Building) {
                fixReferences ();
                base.EndBuilding ();
            }
        }
    }

}
