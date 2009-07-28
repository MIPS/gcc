
using System;
using System.Text;
using System.Collections;

using Mono.Cecil;

using gcc4cli.util;


namespace gcc4cli.objdump {

    sealed class CilSymbols {

        static readonly Hashtable m_aliases;
        static readonly Set m_keywords;

        static CilSymbols ()
        {
            m_aliases = new Hashtable ();

            m_aliases["System.Void"]    = "void";
            m_aliases["System.Object"]  = "object";
            m_aliases["System.String"]  = "string";
            m_aliases["System.Boolean"] = "bool";
            m_aliases["System.Char"]    = "char";
            m_aliases["System.Single"]  = "float32";
            m_aliases["System.Double"]  = "float64";
            m_aliases["System.SByte"]   = "int8";
            m_aliases["System.Byte"]    = "unsigned int8";
            m_aliases["System.Int16"]   = "int16";
            m_aliases["System.UInt16"]  = "unsigned int16";
            m_aliases["System.Int32"]   = "int32";
            m_aliases["System.UInt32"]  = "unsigned int32";
            m_aliases["System.Int64"]   = "int64";
            m_aliases["System.UInt64"]  = "unsigned int64";
            m_aliases["System.IntPtr"]  = "native int";
            m_aliases["Systen.UIntPtr"] = "unsigned native int";
            m_aliases["System.TypedReference"] = "typedref";
			
            m_keywords = new Set ();

            m_keywords.AddRange (new object[] {
                    "Compilercontrolled",
                    "abstract",
                    "add",
                    "algorithm",
                    "alignment",
                    "and",
                    "ansi",
                    "any",
                    "arglist",
                    "array",
                    "as",
                    "assembly",
                    "assert",
                    "at",
                    "auto",
                    "autochar",
                    "beforefieldinit",
                    "beq",
                    "bestfit",
                    "bge",
                    "bgt",
                    "ble",
                    "blob",
                    "blob_object",
                    "blt",
                    "bne",
                    "bool",
                    "box",
                    "br",
                    "break",
                    "brfalse",
                    "brinst",
                    "brnull",
                    "brtrue",
                    "brzero",
                    "bstr",
                    "bytearray",
                    "byvalstr",
                    "call",
                    "callconv",
                    "calli",
                    "callmostderived",
                    "callvirt",
                    "carray",
                    "castclass",
                    "catch",
                    "cdecl",
                    "ceq",
                    "cf",
                    "cgt",
                    "char",
                    "charmaperror",
                    "cil",
                    "ckfinite",
                    "class",
                    "clsid",
                    "clt",
                    "const",
                    "cpblk",
                    "cpobj",
                    "currency",
                    "custom",
                    "date",
                    "decimal",
                    "default",
                    "demand",
                    "deny",
                    "div",
                    "dup",
                    "endfault",
                    "endfilter",
                    "endfinally",
                    "endmac",
                    "enum",
                    "error",
                    "explicit",
                    "extends",
                    "extern",
                    "false",
                    "famandassem",
                    "family",
                    "famorassem",
                    "fastcall",
                    "fault",
                    "field",
                    "filetime",
                    "filter",
                    "final",
                    "finally",
                    "fixed",
                    "flags",
                    "float",
                    "float32",
                    "float64",
                    "forwardref",
                    "fromunmanaged",
                    "handler",
                    "hidebysig",
                    "hresult",
                    "idispatch",
                    "il",
                    "illegal",
                    "implements",
                    "implicitcom",
                    "implicitres",
                    "import",
                    "in",
                    "inheritcheck",
                    "init",
                    "initblk",
                    "initobj",
                    "initonly",
                    "instance",
                    "int",
                    "int16",
                    "int32",
                    "int64",
                    "int8",
                    "interface",
                    "internalcall",
                    "isinst",
                    "iunknown",
                    "jmp",
                    "lasterr",
                    "lcid",
                    "ldarg",
                    "ldarga",
                    "ldelem",
                    "ldelema",
                    "ldfld",
                    "ldflda",
                    "ldftn",
                    "ldlen",
                    "ldloc",
                    "ldloca",
                    "ldnull",
                    "ldobj",
                    "ldsfld",
                    "ldsflda",
                    "ldstr",
                    "ldtoken",
                    "ldvirtftn",
                    "leave",
                    "linkcheck",
                    "literal",
                    "localloc",
                    "lpstr",
                    "lpstruct",
                    "lptstr",
                    "lpvoid",
                    "lpwstr",
                    "managed",
                    "marshal",
                    "method",
                    "mkrefany",
                    "modopt",
                    "modreq",
                    "mul",
                    "native",
                    "neg",
                    "nested",
                    "newarr",
                    "newobj",
                    "newslot",
                    "noappdomain",
                    "noinlining",
                    "nomachine",
                    "nomangle",
                    "nometadata",
                    "noncasdemand",
                    "noncasinheritance",
                    "noncaslinkdemand",
                    "nop",
                    "noprocess",
                    "not",
                    "not_in_gc_heap",
                    "notremotable",
                    "notserialized",
                    "null",
                    "nullref",
                    "object",
                    "objectref",
                    "off",
                    "on",
                    "opt",
                    "optil",
                    "or",
                    "out",
                    "permitonly",
                    "pinned",
                    "pinvokeimpl",
                    "pop",
                    "prefix1",
                    "prefix2",
                    "prefix3",
                    "prefix4",
                    "prefix5",
                    "prefix6",
                    "prefix7",
                    "prefixref",
                    "prejitdeny",
                    "prejitgrant",
                    "preservesig",
                    "private",
                    "privatescope",
                    "property",
                    "protected",
                    "public",
                    "readonly",
                    "record",
                    "refany",
                    "refanytype",
                    "refanyval",
                    "rem",
                    "reqmin",
                    "reqopt",
                    "reqrefuse",
                    "reqsecobj",
                    "request",
                    "ret",
                    "rethrow",
                    "retval",
                    "rtspecialname",
                    "runtime",
                    "safearray",
                    "sealed",
                    "sequential",
                    "serializable",
                    "shl",
                    "shr",
                    "sizeof",
                    "special",
                    "specialname",
                    "starg",
                    "static",
                    "stdcall",
                    "stelem",
                    "stfld",
                    "stloc",
                    "stobj",
                    "storage",
                    "stored_object",
                    "stream",
                    "streamed_object",
                    "strict",
                    "string",
                    "struct",
                    "stsfld",
                    "sub",
                    "switch",
                    "synchronized",
                    "syschar",
                    "sysstring",
                    "tbstr",
                    "thiscall",
                    "tls",
                    "to",
                    "true",
                    "type",
                    "typedref",
                    "uint",
                    "uint16",
                    "uint32",
                    "uint64",
                    "uint8",
                    "unbox",
                    "unicode",
                    "unmanaged",
                    "unmanagedexp",
                    "unsigned",
                    "userdefined",
                    "value",
                    "valuetype",
                    "vararg",
                    "variant",
                    "vector",
                    "virtual",
                    "void",
                    "wchar",
                    "winapi",
                    "with",
                    "xor"
                });
        }

        static bool IsPrimitive (TypeReference type)
        {
            return m_aliases.ContainsKey (type.FullName);
        }

        static string GetPrimitiveAlias (TypeReference type)
        {
            return (string)m_aliases [type.FullName];
        }

        static public string GetSymbolName (string name)
        {
            if (m_keywords.Contains (name)
                || Char.IsDigit(name[0])
                || (name.IndexOfAny(new char[]{'.', '[', ']', '<', '>', '-'}) != -1
                    && name != ".ctor"
                    && name != ".cctor"))
                return "'" + name + "'";
            else
                return name;
        }

        static void GetTypeName (StringBuilder sb, TypeReference type)
        {
            if (type is TypeDefinition) {
                // Nothing to do
            } else {
                sb.Append ("[");
                sb.Append (GetSymbolName (type.Scope.Name));
                sb.Append ("]");
            }

            if (type.DeclaringType != null) {
                if (type.DeclaringType.Namespace.Length != 0) {
                    sb.Append (GetSymbolName (type.DeclaringType.Namespace));
                    sb.Append (".");
                }
                sb.Append (GetSymbolName (type.DeclaringType.Name));
                sb.Append ("/");
                sb.Append (GetSymbolName (type.Name));
            } else {
                if (type.Namespace.Length != 0) {
                    sb.Append (GetSymbolName (type.Namespace));
                    sb.Append (".");
                }
                sb.Append (GetSymbolName (type.Name));
            }
        }

        static void GetTypeReferenceString (StringBuilder sb, TypeReference type, bool only_name, bool convert_primitive)
        {
            if (type is PinnedType) {
                TypeSpecification ts = type as TypeSpecification;
                sb.Append ("pinned ");
                GetTypeReferenceString (sb, ts.ElementType, only_name, convert_primitive);
            } else if (type is ArrayType) {
                TypeSpecification ts = type as TypeSpecification;
                GetTypeReferenceString (sb, ts.ElementType, only_name, convert_primitive);
                sb.Append ("[]");
            } else if (type is PointerType) {
                TypeSpecification ts = type as TypeSpecification;
                GetTypeReferenceString (sb, ts.ElementType, only_name, convert_primitive);
                sb.Append ("*");
            } else if (type is FunctionPointerType) {
                FunctionPointerType fp = type as FunctionPointerType;
                sb.Append ("method ");
                GetTypeReferenceString (sb, fp.ReturnType.ReturnType, false, true);
                sb.Append (" *(");
                for (int i = 0; i < fp.Parameters.Count; i++) {
                    if (i > 0)
                        sb.Append (", ");
                    GetTypeReferenceString (sb, fp.Parameters [i].ParameterType, false, true);
                }
                sb.Append (")");
            } else if (type is ReferenceType) {
                TypeSpecification ts = type as TypeSpecification;
                GetTypeReferenceString (sb, ts.ElementType, only_name, convert_primitive);
                sb.Append ("&");
            } else if (type is ModifierOptional) {
                ModType mt = type as ModifierOptional;
                GetTypeReferenceString (sb, mt.ElementType, only_name, convert_primitive);
                sb.Append (" modopt(");
                GetTypeReferenceString (sb, mt.ModifierType, true, convert_primitive);
                sb.Append (")");
            } else if (type is ModifierRequired) {
                ModType mt = type as ModifierRequired;
                GetTypeReferenceString (sb, mt.ElementType, only_name, convert_primitive);
                sb.Append (" modreq(");
                GetTypeReferenceString (sb, mt.ModifierType, true, convert_primitive);
                sb.Append (")");
            } else if (type is SentinelType) {
                TypeSpecification ts = type as TypeSpecification;
                sb.Append (" ...,");
                GetTypeReferenceString (sb, ts.ElementType, only_name, convert_primitive);
            } else if (type is TypeSpecification) {
                throw new NotSupportedException(type.FullName);
            } else if (convert_primitive && IsPrimitive (type)) {
                sb.Append (GetPrimitiveAlias (type));
            } else {
                if (!only_name) {
                    sb.Append (type.IsValueType ? "valuetype " : "class ");
                }
                GetTypeName (sb, type);
            }
        }

        static public string GetTypeReferenceString (TypeReference type, bool only_name, bool convert_primitive)
        {
            StringBuilder sb = new StringBuilder ();
            GetTypeReferenceString (sb, type, only_name, convert_primitive);
            return sb.ToString ();
        }

        static public string GetMemberString (MemberReference member)
        {
            StringBuilder sb = new StringBuilder ();

            GetTypeReferenceString (sb, member.DeclaringType, true, true);
            sb.Append ("::");
            sb.Append (GetSymbolName (member.Name));

            return sb.ToString ();
        }

        static public string GetFieldReferenceString (FieldReference field)
        {
            StringBuilder sb = new StringBuilder ();

            GetTypeReferenceString (sb, field.FieldType, false, true);
            sb.Append (" ");
            sb.Append (GetMemberString (field));

            return sb.ToString ();
        }

        static public string GetCallingConventionString (MethodCallingConvention callconv)
        {
            switch (callconv) {
            case MethodCallingConvention.Default:  return "default";
            case MethodCallingConvention.C:        return "unmanaged cdecl";
            case MethodCallingConvention.StdCall:  return "unmanaged stdcall";
            case MethodCallingConvention.ThisCall: return "unmanaged thiscall";
            case MethodCallingConvention.FastCall: return "unmanaged fastcall";
            case MethodCallingConvention.VarArg:   return "vararg";
//             case MethodCallingConvention.Generic:  return "";
            default:
                throw new NotSupportedException();
            }
        }

        static string GetMethodSignatureString (IMethodSignature method, string name)
        {
            StringBuilder sb = new StringBuilder ();

            if (method.HasThis)
                sb.Append ("instance ");

            if (method.CallingConvention != MethodCallingConvention.Default) {
                sb.Append (GetCallingConventionString (method.CallingConvention));
                sb.Append (" ");
            }

            GetTypeReferenceString (sb, method.ReturnType.ReturnType, false, true);
            sb.Append (" ");
            if (name != null)
                sb.Append (name);
            sb.Append ("(");
            for (int i = 0; i < method.Parameters.Count; i++) {
                if (i > 0)
                    sb.Append (", ");
                GetTypeReferenceString (sb, method.Parameters [i].ParameterType, false, true);
            }
            sb.Append  (")");
            return sb.ToString ();
        }

        static public string GetMethodReferenceString (MethodReference method)
        {
            return GetMethodSignatureString (method, GetMemberString (method));
        }

        static public string GetCallsiteString (CallSite method)
        {
            return GetMethodSignatureString (method, null);
        }

    }
}
