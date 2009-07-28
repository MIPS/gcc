
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public class ReferenceGettersCached : ReferenceGetters
    {

        Hashtable types_cache = new Hashtable (); //Dictionary<string,TypeReference>

        Hashtable members_cache = new Hashtable (); //Dictionary<string,IMemberReference>


        public ReferenceGettersCached (CFile cfile, bool allowExternalAssembly)
            : base (cfile, allowExternalAssembly)
        {
        }

        public ReferenceGettersCached (CFileBuilder cfile)
            : base (cfile)
        {
        }

        public ReferenceGettersCached (ModuleDefinition module)
            : base (module)
        {
        }

        public void Add (string name, TypeReference type)
        {
            Tracer.Trace (6,"Add type to Cache: " + name + " as " + type.ToString ());
            types_cache[name] = type;
        }

        public void Add (TypeReference type)
        {
            Add (type.ToString (), type);
        }

        public void Add (string name, IMemberReference member)
        {
            Tracer.Trace (6,"Add member to Cache: " + name + " as " + member.ToString ());
            members_cache[name] = member;
        }

        public void Add (IMemberReference member)
        {
            Add (member.ToString (),member);
        }

        public TypeReference CacheGetType (string name)
        {
            
            if (types_cache.Contains (name))
                return (TypeReference) types_cache[name];
            return null;
        }

        public IMemberReference CacheGetSymbol (string name)
        {
            if (members_cache.Contains (name))
                return (IMemberReference) members_cache[name];
            return null;
        }

        public override TypeReference GetTypeReference (TypeReference type)
        {
            if (type == null)
                return null;

            if (type is SentinelType) {
                type = (type as SentinelType).ElementType;
            }

            TypeReference result = null;
            string fullname = type.ToString ();

            if (types_cache.Contains (fullname)) {
                result = (TypeReference)types_cache[fullname];
                Tracer.Trace (4,"Found in Cache: " + fullname + " as " + result.FullName);
                return result;
            }

            result = base.GetTypeReference (type);

            Add (fullname, result);
            Add (result);

            return result;
        }

        public override MethodReference GetMethodReference (MethodReference method)
        {
            if (method == null)
                return null;

            MethodReference result = null;
            string fullname = method.ToString ();

            if (members_cache.Contains (fullname)) {
                result = (MethodReference)members_cache[fullname];
                Tracer.Trace (4,"Found in Cache: " + fullname + " as " + result.ToString ());
                return result;
            }

            result = base.GetMethodReference (method);

            Add (fullname, result);
            Add (result);

            return result;
        }

        public override FieldReference GetFieldReference (FieldReference field)
        {
            FieldReference result = null;
            string fullname = field.ToString ();

            if (members_cache.Contains (fullname)) {
                result = (FieldReference)members_cache[fullname];
                Tracer.Trace (4,"Found in Cache: " + fullname + " as " + result.ToString ());
                return result;
            }

            result = base.GetFieldReference (field);

            Add (fullname, result);
            Add (result);

            return result;
        }

    }

}
