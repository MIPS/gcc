
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;

namespace gcc4cli.util {

    public abstract class CFile {

        public static readonly string ExternalAssemblyName = "ExternalAssembly";
        public static readonly string ExternalTypeName = "ExternalAssembly";

        string name;
        AssemblyDefinition assembly;
        ModuleDefinition module;
        TypeDefinition mainType;

        public string Name {
            get { return name; }
            protected set { name = value; }
        }

        public AssemblyDefinition Assembly {
            get { return assembly; }
            protected set { assembly = value; }
        }

        public ModuleDefinition Module {
            get { return module; }
            protected set { module = value; }
        }

        public TypeDefinition MainType {
            get { return mainType; }
            protected set { mainType = value; }
        }

        protected Hashtable def_symbols = new Hashtable (); //Dictionary<string,IMemberDefinition>
        protected Hashtable undef_symbols = new Hashtable (); //Dictionary<string,IMemberReference>
        protected Hashtable private_symbols = new Hashtable (); //Dictionary<string,IMemberDefinition>

        protected Hashtable def_types = new Hashtable (); //Dictionary<string,TypeDefinition>
        protected Hashtable undef_types = new Hashtable (); //Dictionary<string,TypeReference>
        protected Hashtable incomplete_types = new Hashtable (); //Dictionary<string,TypeDefinition>

        protected Hashtable ref_types = new Hashtable (); //Dictionary<string,TypeReference>
        protected Hashtable ref_symbols = new Hashtable (); //Dictionary<string,IMemberReference>

        internal Hashtable DefinedSymbolsTable {
            get { return def_symbols; }
        }

        internal Hashtable PrivateSymbolsTable {
            get { return private_symbols; }
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

        void DumpHashtable (string title, string prefix, Hashtable ht)
        {
            Console.WriteLine (title);
            foreach (string name in ht.Keys)  {
                Console.WriteLine ("{0}{1} \t{2}", prefix, name, ht[name]);
            }
        }

        protected void DumpTables ()
        {
            DumpHashtable ("++++ def_types", "  ", def_types);
            DumpHashtable ("++++ ref_types", "  ", ref_types);
            DumpHashtable ("++++ incomplete_types", "  ", incomplete_types);
            DumpHashtable ("++++ undef_types", "  ", undef_types);
            DumpHashtable ("++++ def_symbols", "  ", def_symbols);
            DumpHashtable ("++++ undef_symbols", "  ", undef_symbols);
            DumpHashtable ("++++ private_symbols", "  ", private_symbols);
            DumpHashtable ("++++ ref_symbols", "  ", ref_symbols);
        }

        public ICollection DefinedSymbols ()
        {
            return def_symbols.Keys;
        }

        public ICollection DefinedTypes ()
        {
            return def_types.Keys;
        }

        public ICollection UndefinedSymbols ()
        {
            return undef_symbols.Keys;
        }

        public ICollection UndefinedTypes ()
        {
            return undef_types.Keys;
        }

        public ICollection IncompleteTypes ()
        {
            return incomplete_types.Keys;
        }

        public ICollection PrivateSymbols ()
        {
            return private_symbols.Keys;
        }

        public ICollection RefSymbols ()
        {
            return ref_symbols.Keys;
        }

        public TypeDefinition GetDefType (string name)
        {
            if (!def_types.Contains (name))
                throw new NotFoundException ("Can't find type '" + name + "' in assembly " + Name, name);
            return (TypeDefinition)def_types[name];
        }

        public IMemberDefinition GetDefMember (string name)
        {
            if (def_symbols.Contains (name))
                return (IMemberDefinition)def_symbols[name];
            if (private_symbols.Contains (name))
                return (IMemberDefinition)private_symbols[name];
            throw new NotFoundException ("Can't find member '" + name + "' in assembly " + Name, name);
        }

        public ICollection AllDefTypes ()
        {
            return Module.Types;
        }

        public ICollection AllDefMethods ()
        {
            return MainType.Methods;
        }

        public ICollection AllDefFields ()
        {
            return MainType.Fields;
        }

        public ICollection AllRefTypes ()
        {
            return Module.TypeReferences;
        }

        public ICollection AllRefMembers ()
        {
            return Module.MemberReferences;
        }

        protected void FillTables ()
        {
            Module.FullLoad ();
            LoadTables ();
        }

        public void ReloadTables ()
        {
            ClearTables ();
            LoadTables ();
            if (Tracer.TraceAtLevel (10))
                DumpTables ();
        }

        protected void ClearTables ()
        {
            def_symbols.Clear ();
            undef_symbols.Clear ();
            private_symbols.Clear ();

            def_types.Clear ();
            undef_types.Clear ();
            incomplete_types.Clear ();

            ref_types.Clear ();
            ref_symbols.Clear ();
        }

        protected void LoadTables ()
        {
            foreach (TypeDefinition type in Module.Types) {
                if (type != MainType) {
                    if (type.DeclaringType != null)
                        throw new NotSupportedException("Error Nested Types not supported");
                    if (CFileAttributes.TypeDefIsIncomplete(type))
                        incomplete_types [type.Name] = type;
                    else
                        def_types [type.Name] = type;
                }
            }

            foreach (MethodDefinition method in MainType.Methods) {
                if (method.IsPublic)
                    def_symbols [method.Name] = method;
                else
                    private_symbols [method.Name] = method;
            }

            foreach (FieldDefinition field in MainType.Fields) {
                if (field.IsPublic)
                    def_symbols [field.Name] = field;
                else
                    private_symbols [field.Name] = field;
            }

            foreach (TypeReference type in Module.TypeReferences) {
                if (type.Name != CFile.ExternalTypeName)
                    ref_types [type.FullName] = type;
                else if (type.Module.Assembly.Name.Name == CFile.ExternalAssemblyName)
                    undef_types [type.Name] = type;
            }

            foreach (MemberReference member in Module.MemberReferences) {
                if (member.DeclaringType.Name == CFile.ExternalTypeName)
                    undef_symbols [member.Name] = member;
                else
                    ref_symbols [member.ToString ()] = member;
            }

        }

        protected CFile (string name, AssemblyKind kind, AssemblyResolver asm_resolver, string attrname)
        {
            Name = name;
            Assembly = AssemblyFactory.DefineAssembly (name, name,
                                                       TargetRuntime.NET_1_0,
                                                       kind);
            Assembly.Resolver = asm_resolver;

            Module = Assembly.MainModule;

            TypeDefinition tdef = asm_resolver.GCC4NET.MainModule.Types[attrname];
            CustomAttribute cattr =
                new CustomAttribute (Module.Import (tdef.Constructors.GetConstructor (false, new Type [] {})));
            Assembly.CustomAttributes.Add (cattr);
        }

        protected CFile (AssemblyDefinition assembly, string attrname)
        {
            Assembly = assembly;
            Name = Assembly.Name.Name;
            if (Assembly.Modules.Count != 1)
                throw new WrongFileFormatException ("Wrong C File Assembly", Name);
            Module = Assembly.Modules[0];
        }

        public void Save(string filename) {
            Tracer.Trace ("Saving Output to " + filename);
            AssemblyFactory.SaveAssembly (Assembly, filename);
        }

    }

}
