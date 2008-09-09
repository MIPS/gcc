
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public abstract class CFileBuilder : CFile, ICFileBuilder {

        bool building = false;
        bool filesAdded = false;
        bool referencesFixed = true;

        internal bool allowExternalAssembly;
        bool keepIncompleteTypes;

        protected CFileBuilder (string name, AssemblyKind kind, AssemblyResolver asm_resolver, string attrname,
                                bool _allowExternalAssembly, bool _keepIncompleteTypes)
            : base (name, kind, asm_resolver, attrname)
        {
            allowExternalAssembly = _allowExternalAssembly;
            keepIncompleteTypes = _keepIncompleteTypes;
        }
        protected CFileBuilder (AssemblyDefinition assembly, string attrname,
                                bool _allowExternalAssembly, bool _keepIncompleteTypes)
            : base (assembly, attrname)
        {
            allowExternalAssembly = _allowExternalAssembly;
            keepIncompleteTypes = _keepIncompleteTypes;
        }

        public bool Building {
            get { return building; }
        }

        public virtual void StartBuilding ()
        {
            if (!building) {
                building = true;
                filesAdded = false;
            }
        }

        public void Add (CFile cfile)
        {
            if (!building)
                throw new Exception ("StartBuilding not called");
            Tracer.Trace ("Adding '" + cfile.Name + "'  to '" + Name + "'");
            if (cfile is CObject)
                Add(cfile as CObject);
            else if (cfile is CStaticLibrary)
                Add(cfile as CStaticLibrary);
            else if (cfile is CDynamicLibrary)
                Add(cfile as CDynamicLibrary);
            else
                throw new WrongFileFormatException ("Wrong C File Assembly", cfile.Name);

            filesAdded = true;
            referencesFixed = false;
        }

        public virtual void EndBuilding ()
        {
            if (!referencesFixed)
                throw new Exception ("Linking Error: references not fixed");
            building = false;
        }

        void check_TypeEquality (TypeDefinition type1, TypeDefinition type2)
        {
            if (type1.Fields.Count != type2.Fields.Count)
                throw new TypeMismatchException ("Different # of fields", type1.ToString(), type2.ToString());
            for (int i = 0; i < type1.Fields.Count; ++i) {
                FieldDefinition f1 = type1.Fields[i];
                FieldDefinition f2 = type2.Fields[i];
                if (   f1.HasLayoutInfo != f2.HasLayoutInfo
                    || f1.Offset != f2.Offset
                    || f1.Name != f2.Name
                    || f1.FieldType.ToString () !=  f2.FieldType.ToString ())
                    throw new TypeMismatchException("Different field "+ i +" '"+f1.ToString()+"'!='"+f2.ToString()+"'",
                                                    type1.ToString(), type2.ToString());
            }
        }

        protected virtual void Add (CObject obj)
        {
            obj.RenamePrivates();
            foreach (TypeDefinition type in obj.AllDefTypes ()) {
                if (type != obj.MainType) {
                    if (def_types.Contains (type.Name)) {
                        if (!CFileAttributes.TypeDefIsIncomplete (type))
                            check_TypeEquality ((TypeDefinition)def_types[type.Name], type);
                    } else {
                        incomplete_types.Remove (type.Name);
                        undef_types.Remove (type.Name);
                        if (CFileAttributes.TypeDefIsIncomplete (type))
                            incomplete_types [type.Name] = type;
                        else {
                            TypeDefinition mtype = type.Clone ();
                            Module.Types.Add (mtype);
                            def_types [mtype.Name] = mtype;
                        }
                    }
                }
            }
            foreach (MethodDefinition method in obj.AllDefMethods ()) {
                if (def_symbols.Contains (method.Name) || private_symbols.Contains (method.Name))
                    throw new RedefinitionException (method);
                undef_symbols.Remove (method.Name);
                MethodDefinition mmethod = method.Clone ();
                MainType.Methods.Add (mmethod);
                if (mmethod.IsPublic)
                    def_symbols [mmethod.Name] = mmethod;
                else
                    private_symbols [mmethod.Name] = mmethod;
            }

            foreach (FieldDefinition field in obj.AllDefFields ()) {
                if (def_symbols.Contains (field.Name) || private_symbols.Contains (field.Name))
                    throw new RedefinitionException (field);
                undef_symbols.Remove (field.Name);
                FieldDefinition mfield = field.Clone ();
                MainType.Fields.Add (mfield);
                if (mfield.IsPublic)
                    def_symbols [mfield.Name] = mfield;
                else
                    private_symbols [mfield.Name] = mfield;
            }

            foreach (TypeReference type in obj.AllRefTypes ()) {
                if (type.Module.Assembly.Name.Name == CFile.ExternalAssemblyName) {
                    if (!def_types.Contains (type.Name) && !incomplete_types.Contains (type.Name))
                        undef_types [type.Name] = type;
                } else if (type.Name != CFile.ExternalTypeName) {
                    TypeReference mtype = Module.Import (type);
                    ref_types [mtype.FullName] = mtype;
                    ref_types [type.FullName] = mtype;
                }
            }

            foreach (MemberReference member in obj.AllRefMembers ()) {
                if (member.DeclaringType.Name == CFile.ExternalTypeName) {
                    if (!def_symbols.Contains (member.Name))
                        undef_symbols [member.Name] = member;
                } else {
                    MemberReference mr;
                    if (member is MethodReference) {
                        mr = Module.Import (member as MethodReference);
                    } else if (member is FieldReference) {
                        mr = Module.Import (member as FieldReference);
                    } else
                        throw new NotSupportedException ();
                    ref_symbols [mr.ToString ()] = mr;
                }
            }

            if (Tracer.TraceAtLevel (10))
                DumpTables ();
        }

        protected virtual void Add (CStaticLibrary lib)
        {
            Queue objs = new Queue (lib.Objects);
            Queue new_objs = new Queue ();
            Set _undef_types = new Set(UndefinedTypes ());
            Set _undef_symbols = new Set(UndefinedSymbols ());

            bool changed = true;
            while (changed) {
                changed = false;
                foreach (CObject obj in objs) {
                    if (!new_objs.Contains(obj)) {
                        Tracer.Trace (2, "Analizing '" + obj.Name + "'");
                        if (   _undef_types.ContainsAny (obj.DefinedTypes ())
                               || _undef_symbols.ContainsAny (obj.DefinedSymbols ())) {
                            Tracer.Trace (2, "Adding '" + obj.Name + "'");
                            new_objs.Enqueue (obj);
                            _undef_types.AddRange (obj.UndefinedTypes ());
                            _undef_types.RemoveRange (obj.DefinedTypes ());
                            _undef_symbols.AddRange (obj.UndefinedSymbols ());
                            _undef_symbols.RemoveRange (obj.DefinedSymbols ());
                            changed = true;
                        }
                    }
                }
            }

            foreach (CObject obj in new_objs) {
                Tracer.Trace ("    Adding '" + obj.Name + "'  to '" + Name + "'");
                Add (obj);
            }
        }

        protected virtual bool Add (CDynamicLibrary lib)
        {
            Set pending_types;
            Set pending_symbols;
            Set new_types;
            Set new_symbols;

            pending_types = new Set (DefinedTypes ());
            pending_types.AddRange (UndefinedTypes ());
            pending_types.AddRange (IncompleteTypes ());
            pending_symbols = new Set (UndefinedSymbols ());

            new_types = new Set (lib.DefinedTypes ());
            new_types.Intersect (pending_types);
            new_symbols = new Set (lib.DefinedSymbols ());
            new_symbols.Intersect (pending_symbols);

            if (!new_types.IsEmpty || !new_symbols.IsEmpty) {
                foreach (string typename in new_types) {
                    TypeDefinition type = lib.GetDefType (typename);
                    undef_types.Remove (typename);
                    incomplete_types.Remove (typename);
                    if (def_types.Contains (typename)) {
                        check_TypeEquality ((TypeDefinition)def_types[typename], type);
                        TypeDefinition old_type = (TypeDefinition)def_types[typename];
                        if (!CFileAttributes.TypeDefIsString (old_type)) {
                            Module.Types.Remove (old_type);
                            def_types.Remove (typename);
                            TypeReference tr = Module.Import ((TypeReference)type);
                            def_types [typename] = tr;
                            if (!CFileAttributes.TypeDefIsArray (old_type)) {
                                foreach (FieldDefinition field in type.Fields) {
                                    MemberReference mr = Module.Import (field as FieldReference);
                                    ref_symbols [mr.ToString ()] = mr;
                                }
                            }
                        }
                    } else {
                        TypeReference tr = Module.Import ((TypeReference)type);
                        def_types [typename] = tr;
                    }
                }
                foreach (string membername in new_symbols) {
                    MemberReference mr;
                    IMemberDefinition member;
                    member =  lib.GetDefMember (membername);
                    if (member is MethodDefinition) {
                        mr = Module.Import (member as MethodReference);
                    } else if (member is FieldDefinition) {
                        mr = Module.Import (member as FieldReference);
                    } else
                        throw new NotSupportedException ();
                    undef_symbols.Remove (mr.Name);
                    def_symbols [mr.Name] = mr;
                }

                if (Tracer.TraceAtLevel (10))
                    DumpTables ();

                return true;
            }

            return false;
        }

        protected void fixReferences ()
        {
            if (filesAdded) {
                ReferenceGettersCached reffixer = new ReferenceGettersCached (this);

                if (keepIncompleteTypes) {
                    foreach (TypeDefinition inc_type in incomplete_types.Values) {
                        TypeDefinition mtype = inc_type.Clone ();
                        Module.Types.Add (mtype);
                    }
                } else {
                    TypeReference newtype = new PointerType (Module.Import (typeof (void)));
                    foreach (TypeDefinition inc_type in incomplete_types.Values) {
                            reffixer.Add (inc_type.ToString (), newtype);
                    }
                }

                ReferencesFixer rf = new ReferencesFixer (reffixer);
                rf.FixReferences (Module);
            }
            referencesFixed = true;
        }
    }

}
