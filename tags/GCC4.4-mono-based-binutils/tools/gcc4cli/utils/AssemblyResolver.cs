
using System;
using System.Collections;
using System.IO;
using Mono.Cecil;

namespace gcc4cli.util {

    public class AssemblyResolver : IAssemblyResolver {

        ArrayList m_std_directories = new ArrayList ();
        ArrayList m_env_directories = new ArrayList ();
        ArrayList m_directories = new ArrayList ();
        Hashtable asmdefs = new Hashtable();
        char DirSep = Path.DirectorySeparatorChar;

        AssemblyDefinition gcc4net = null;

        public AssemblyDefinition GCC4NET {
            get {
                if (gcc4net == null)
                    gcc4net = Resolve ("gcc4net.dll");
                return gcc4net;
            }
        }

        AssemblyDefinition mscorlib = null;

        AssemblyDefinition MSCORLIB {
            get {
                if (mscorlib == null) {
                    mscorlib = AssemblyFactory.GetAssembly (typeof (object).Module.FullyQualifiedName);
                }
                return mscorlib;
            }
        }

        static AssemblyResolver defResolver = null;
        static public AssemblyResolver DefaultResolver {
            get {
                if (defResolver == null)
                    defResolver = new AssemblyResolver();
                return defResolver;
            }
        }

        public AssemblyResolver()
        {
            string bindir = Path.GetDirectoryName (typeof (AssemblyResolver).Module.FullyQualifiedName);
            string toolchaindir = bindir  + DirSep + "..";
            m_std_directories.Add (toolchaindir  + DirSep + "lib");
            string ld_lpath = Environment.GetEnvironmentVariable ("LD_LIBRARY_PATH");
            if (ld_lpath != null) {
                string[] directories = ld_lpath.Split(':');
                m_env_directories.AddRange (directories);
            }

            m_directories.Add (".");
            m_directories.AddRange (m_std_directories);
            m_directories.AddRange (m_env_directories);
        }

        public void RemoveDefaultDirs()
        {
            foreach (string directory in m_std_directories) {
                m_directories.Remove (directory);
            }
        }

        public void AddSearchDirectories (string libpath)
        {
            string[] directories = libpath.Split(':');
            foreach (string directory in directories) {
                AddSearchDirectory (directory);
            }
        }

        public void AddSearchDirectories (string[] directories)
        {
            foreach (string directory in directories) {
                AddSearchDirectory (directory);
            }
        }

        public void AddSearchDirectory (string directory)
        {
            string ldir = directory.Trim();
            if (ldir != "")  {
                m_directories.Add (ldir);
            }
        }

        public void RemoveSearchDirectory (string directory)
        {
            string ldir = directory.Trim();
            if (ldir != "")  {
                m_directories.Remove (ldir);
            }
        }

        AssemblyDefinition addAssemblyDef (string name, string filename)
        {
            AssemblyDefinition asm = AssemblyFactory.GetAssembly(filename);
            asmdefs[name] = asm;
            asm.Resolver = this;
            return asm;
        }

        public AssemblyDefinition Resolve (string fullName)
        {
            return Resolve (AssemblyNameReference.Parse (fullName));
        }

        public AssemblyDefinition Resolve (AssemblyNameReference asmName)
        {
            string name = asmName.Name.Trim();

            if (asmdefs.ContainsKey(name))
                return (AssemblyDefinition) asmdefs[name];

            if (name[0] == DirSep) {
                if (File.Exists(name + ".dll"))
                    return addAssemblyDef (name, name + ".dll");
                else if (File.Exists(name + ".exe"))
                    return addAssemblyDef (name, name + ".exe");
                else if (File.Exists(name))
                    return addAssemblyDef (name, name);
            }

            foreach (string dir in m_directories) {
                string filename = dir + DirSep + name;
                if (File.Exists(filename + ".dll"))
                    return addAssemblyDef (name, filename + ".dll");
                else if (File.Exists(filename + ".exe"))
                    return addAssemblyDef (name, filename + ".exe");
                else if (File.Exists(filename))
                    return addAssemblyDef (name, filename);
            }

            if (name == "mscorlib")
                return MSCORLIB;

            throw new FileNotFoundException(null, name);
        }

        public AssemblyDefinition ResolveWithoutLibPath (string fullName)
        {
            string name = fullName.Trim();

            if (asmdefs.ContainsKey(name))
                return (AssemblyDefinition) asmdefs[name];


            if (File.Exists(name))
                return addAssemblyDef (name, name);

            throw new FileNotFoundException(null, name);
        }

    }
}
