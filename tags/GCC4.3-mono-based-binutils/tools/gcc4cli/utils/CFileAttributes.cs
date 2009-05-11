
using Mono.Cecil;

namespace gcc4cli.util {

    public class CFileAttributes {

        public static string initializer = "gcc4net.C_Attributes.Initializer";
        public static string finalizer = "gcc4net.C_Attributes.Finalizer";
        public static string arraytype = "gcc4net.C_Attributes.ArrayType";
        public static string conststringtype = "gcc4net.C_Attributes.ConstStringType";
        public static string incompletetype = "gcc4net.C_Attributes.IncompleteType";
        public static string cobj = "gcc4net.C_Attributes.CObjectFile";
        public static string cexe = "gcc4net.C_Attributes.CExecutable";
        public static string cslib = "gcc4net.C_Attributes.CStaticLibrary";
        public static string cdlib = "gcc4net.C_Attributes.CDynamicLibrary";
        public static string mt = "gcc4net.C_Attributes.MultiThreaded";

        public static bool MethodIsInitializer (MethodDefinition method)
        {
            return CheckForAttribute (initializer, method.CustomAttributes);
        }

        public static bool MethodIsFinalizer (MethodDefinition method)
        {
            return CheckForAttribute (finalizer, method.CustomAttributes);
        }

        public static bool TypeDefIsArray (TypeDefinition type)
        {
            return CheckForAttribute (arraytype, type.CustomAttributes);
        }

        public static bool TypeDefIsString (TypeDefinition type)
        {
            return CheckForAttribute (conststringtype, type.CustomAttributes);
        }

        public static bool TypeDefIsIncomplete (TypeDefinition type)
        {
            return CheckForAttribute (incompletetype, type.CustomAttributes);
        }

        public static bool AssemblyIsCExecutable(AssemblyDefinition asm)
        {
            return CheckForAttribute (cexe, asm.CustomAttributes);
        }
        public static bool AssemblyIsCObject(AssemblyDefinition asm)
        {
            return CheckForAttribute (cobj, asm.CustomAttributes);
        }
        public static bool AssemblyIsCStaticLibrary(AssemblyDefinition asm)
        {
            return CheckForAttribute (cslib, asm.CustomAttributes);
        }
        public static bool AssemblyIsCDynamicLibrary(AssemblyDefinition asm)
        {
            return CheckForAttribute (cdlib, asm.CustomAttributes);
        }
        public static bool AssemblyIsMultiThreaded(AssemblyDefinition asm)
        {
            return CheckForAttribute (mt, asm.CustomAttributes);
        }
        public static bool CheckForAttribute (string fname, CustomAttributeCollection coll)
        {
            foreach (CustomAttribute ca in coll) {
                if (ca.Constructor.DeclaringType.FullName == fname)
                    return true;
            }
            return false;
        }

    }
}
