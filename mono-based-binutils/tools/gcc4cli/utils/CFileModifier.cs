
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;

namespace gcc4cli.util {

    public abstract class CFileModifier : CFile, ICFileModifier {


        protected CFileModifier (string name, AssemblyKind kind, AssemblyResolver asm_resolver, string attrname)
            : base (name, kind, asm_resolver, attrname)
        {
        }

        protected CFileModifier (AssemblyDefinition assembly, string attrname)
            : base (assembly, attrname)
        {
        }

        public void AddSymbolPrefix (string prefix)
        {
            if (prefix == null || prefix == "")
                return;

            foreach (MethodDefinition method in MainType.Methods) {
                string newName = prefix + method.Name;
                method.Name = newName;
            }
            foreach (FieldDefinition field in MainType.Fields) {
                string newName = prefix + field.Name;
                field.Name = newName;
            }
            ReloadTables ();
        }

        public void AddTypePrefix (string prefix)
        {
            if (prefix == null || prefix == "")
                return;

            foreach (TypeDefinition type in Module.Types) {
                if (type != MainType && !CFileAttributes.TypeDefIsString(type)) {
                    string newName = prefix + type.Name;
                    type.Name = newName;
                }
            }
            ReloadTables ();
        }

        public void localizeSymbol (string name)
        {
            if (def_symbols.Contains (name)) {
                IMemberDefinition sym = (IMemberDefinition)def_symbols[name];
                if (sym is MethodDefinition)
                    (sym as MethodDefinition).IsPrivate = true;
                else if (sym is FieldDefinition)
                    (sym as FieldDefinition).IsPrivate = true;
                else 
                    throw new NotSupportedException ();
                def_symbols.Remove (name);
                private_symbols[name] = sym;
            }
        }

        public void globalizeSymbol (string name)
        {
            if (private_symbols.Contains (name)) {
                IMemberDefinition sym = (IMemberDefinition)private_symbols[name];
                if (sym is MethodDefinition)
                    (sym as MethodDefinition).IsPublic = true;
                else if (sym is FieldDefinition)
                    (sym as FieldDefinition).IsPublic = true;
                else 
                    throw new NotSupportedException ();
                private_symbols.Remove (name);
                def_symbols[name] = sym;
            }
        }

        public void renameSymbol (string name, string newname)
        {
            if (def_symbols.Contains (name)) {
                MemberReference myref = (MemberReference)def_symbols[name];
                myref.Name = newname;
                def_symbols.Remove (name);
                def_symbols[newname] = myref;
            }
            if (private_symbols.Contains (name)) {
                MemberReference myref = (MemberReference)private_symbols[name];
                myref.Name = newname;
                private_symbols.Remove (name);
                private_symbols[newname] = myref;
            }
        }

        public void renameType (string name, string newname)
        {
            if (def_types.Contains (name)) {
                TypeDefinition mytype = (TypeDefinition)def_types[name];
                mytype.Name = newname;
                def_types.Remove (name);
                def_types[newname] = mytype;
            }
            if (incomplete_types.Contains (name)) {
                TypeDefinition mytype = (TypeDefinition)incomplete_types[name];
                mytype.Name = newname;
                incomplete_types.Remove (name);
                incomplete_types[newname] = mytype;
            }
        }

    }

}
