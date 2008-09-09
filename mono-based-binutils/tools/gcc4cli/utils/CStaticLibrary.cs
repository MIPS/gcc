
using System;
using System.IO;
using System.Text;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Binary;

namespace gcc4cli.util {

    public sealed class CStaticLibrary : CFile, ICFileBuilder {

        Hashtable objects = new Hashtable (); //Dictionary<string,CObject>
        Hashtable types = new Hashtable ();   //Dictionary<string,string>
        Hashtable symbols = new Hashtable (); //Dictionary<string,string>

        static readonly object marker = new object ();

        public ICollection Objects {
            get { return objects.Values; }
        }

        public ICollection ObjectsName {
            get { return objects.Keys; }
        }

        bool building = false;

        public bool Building {
            get { return building; }
        }

        internal CStaticLibrary (string name, AssemblyResolver asm_resolver)
            : base (name, AssemblyKind.Dll, asm_resolver, CFileAttributes.cslib)
        {
        }

        internal CStaticLibrary (AssemblyDefinition assembly)
            : base (assembly, CFileAttributes.cslib)
        {
            foreach (EmbeddedResource res in Module.Resources) {
                AssemblyDefinition asm = AssemblyFactory.GetAssembly (res.Data);
                CObject obj = new CObject (asm);
//                 if (objects.Contains (obj))
//                     throw new Exception ("An object file can be added only once");
                objects[obj.Name] = obj;
            }
            LoadMaps (false);
        }

        public void StartBuilding ()
        {
            if (!building) {
                building = true;
                Module.Resources.Clear ();
            }
        }

        public void EndBuilding ()
        {
            building = false;
            foreach (CObject obj in objects.Values) {
                Tracer.Trace ("Creating EmbeddedResource for " + obj.Name);
                EmbeddedResource res = new EmbeddedResource (obj.Name, ManifestResourceAttributes.Private);
                MemoryStream memStream = new MemoryStream ();
                AssemblyFactory.SaveAssembly (obj.Assembly, memStream);
                res.Data = memStream.ToArray ();
                Module.Resources.Add (res);
            }
        }

        protected void ReloadMaps (bool warn)
        {
            ClearMaps ();
            LoadMaps (warn);
        }

        protected void ClearMaps ()
        {
            ClearTables ();
            types.Clear ();
            symbols.Clear ();
        }

        protected void UpdateMaps (CObject obj, bool warn)
        {
            string name = obj.Name;
            foreach (string tname in obj.DefinedTypes ()) {
                types[tname] = name;
//                 def_types[tname] = obj.GetDefType (tname);
                def_types[tname] = marker;
            }
            foreach (string sname in obj.DefinedSymbols ()) {
                if (warn && symbols.Contains (sname)) {
                    Console.WriteLine ("Warning: Symbol " + sname + " redefined in " + name);
                    Console.WriteLine ("         First definition in " + symbols[sname]);
                }
                symbols[sname] = name;
//                 def_symbols[sname] = obj.GetDefMember (sname);
                def_symbols[sname] = marker;
            }
        }

        protected void LoadMaps (bool warn)
        {
            foreach (CObject obj in objects.Values) {
                UpdateMaps (obj, warn);
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
            else
                throw new Exception ("Only CObjects can be added to Static Libraries for the moment");
        }

        void Add (CObject obj)
        {
            if (objects.Contains (obj))
                throw new Exception ("An object file can be added only once");
            objects[obj.Name] = obj;
            UpdateMaps (obj, true);
            // Delay creation of EmbeddedResource till save point
        }


        void Add (CStaticLibrary lib)
        {
            foreach (CObject obj in lib.Objects) {
                Tracer.Trace ("    Adding '" + obj.Name + "'  to '" + Name + "'");
                Add (obj);
            }
        }

        public void Remove (CObject obj)
        {
            Remove (obj.Name);
        }

        public void Remove (string name)
        {
            if (!building)
                throw new Exception ("StartBuilding not called");
            if (objects.Contains (name)) {
                Tracer.Trace ("Removing " + name + " from " + Name);
                objects.Remove(name);
                ReloadMaps (false);
            }
        }

        public CObject Get (string name)
        {
            if (!objects.Contains (name))
                throw new Exception ("Object " + name + " not found in library");
            return (CObject)objects[name];
        }

        public bool Contains (CObject obj)
        {
            return objects.Contains (obj.Name);
        }

        public bool Contains (string name)
        {
            return objects.Contains (name);
        }

    }
}
