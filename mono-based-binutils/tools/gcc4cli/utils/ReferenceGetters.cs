
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public class ReferenceGetters : IReferenceGetters
    {

        CFile m_cfile;

        ModuleDefinition m_module;

        bool m_aea; // allowExternalAssembly

        readonly string ExternalAssemblyName = CFile.ExternalAssemblyName;
        readonly string ExternalTypeName = CFile.ExternalTypeName;

        ReferenceGetters (CFile cfile, ModuleDefinition module, bool allowExternalAssembly)
        {
            m_cfile = cfile;
            m_module = module;
            m_aea = allowExternalAssembly;
        }

        public ReferenceGetters (CFile cfile, bool allowExternalAssembly)
            : this (cfile, cfile.Module, allowExternalAssembly)
        {
        }

        public ReferenceGetters (CFileBuilder cfile)
            : this (cfile, cfile.Module, cfile.allowExternalAssembly)
        {
        }

        public ReferenceGetters (ModuleDefinition module)
            : this (null, module, false)
        {
        }

        public virtual ModuleReference GetModuleReference (ModuleReference module)
        {
            string name = module.Name.ToLower ();

            foreach (ModuleReference tmr in m_module.ModuleReferences) {
                if (tmr.Name == name)
                    return tmr;
            }
            ModuleReference mr = new ModuleReference (name);
            m_module.ModuleReferences.Add (mr);
            return mr;
        }

        public virtual TypeReference GetTypeReference (TypeReference type)
        {
            if (type == null)
                return null;

            if (type is SentinelType) {
                type = (type as SentinelType).ElementType;
            }

            TypeReference result = null;
            string fullname = type.FullName;

            Tracer.Trace (2,"Searching:      " + fullname);

            if (m_cfile != null
                && (type.Name == "<Module>" || type.Name == ExternalAssemblyName)) {
                result = m_cfile.MainType;
            } else if (type is FunctionPointerType) {
                FunctionPointerType fpt = type as FunctionPointerType;
                fpt.ElementType = GetTypeReference (fpt.ElementType);
                fpt.ReturnType.ReturnType = GetTypeReference (fpt.ReturnType.ReturnType);
                for (int i = 0; i < fpt.Parameters.Count ; ++i) {
                    fpt.Parameters[i].ParameterType = GetTypeReference (fpt.Parameters[i].ParameterType);
                }
                result = fpt;
            } else if (type is ModType) {
                ModType spec = type as ModType;
                spec.ElementType = GetTypeReference (spec.ElementType);
                spec.ModifierType = GetTypeReference (spec.ModifierType);
                result = spec;
            } else if (type is TypeSpecification) {
                TypeSpecification spec = type as TypeSpecification;
                spec.ElementType = GetTypeReference (spec.ElementType);
                result = spec;
            } else {
                if (m_module.Types.Contains (fullname))
                    result = m_module.Types[fullname];
                else if (m_module.TypeReferences.Contains (fullname))
                    result = m_module.TypeReferences[fullname];
            }

            if (result == null)
                throw new NotFoundException (type);

            Tracer.Trace(2,"Returning:      " + result.ToString ());

            return result;
        }

        MemberReference GetMemberReference (MemberReference member)
        {
            foreach (MemberReference mr in m_module.MemberReferences) {
                if (mr.ToString () == member.ToString ())
                    return mr;
            }
            throw new NotFoundException(member);
        }

        bool MethodIsVarArg(MethodReference method)
        {
            return (method.CallingConvention & MethodCallingConvention.VarArg) != 0;
        }

        MethodReference duplicate_VarargMethodReference (MethodReference m_sig, MethodReference base_method)
        {
            MethodReference result = new MethodReference (base_method.Name,
                                                          GetTypeReference (base_method.DeclaringType),
                                                          GetTypeReference (base_method.ReturnType.ReturnType),
                                                          base_method.HasThis,
                                                          base_method.ExplicitThis,
                                                          base_method.CallingConvention);

            int bm_argsNum = base_method.Parameters.Count;
            int m_argsNum = m_sig.Parameters.Count;
            int i = 0;

            for (; i<bm_argsNum ; ++i) {
                TypeReference param_type = GetTypeReference (base_method.Parameters[i].ParameterType);
                ParameterDefinition p = new ParameterDefinition (param_type);
                result.Parameters.Add (p);
            }

            if (i<m_argsNum) {
                TypeReference param_type = GetTypeReference (m_sig.Parameters[i].ParameterType);
                ParameterDefinition p = new ParameterDefinition (new SentinelType (param_type));
                result.Parameters.Add (p);
                ++i;
                for (; i<m_argsNum ; ++i) {
                    param_type = GetTypeReference (m_sig.Parameters[i].ParameterType);
                    p = new ParameterDefinition (param_type);
                    result.Parameters.Add (p);
                }
            }

            return result;
        }

        public virtual MethodReference GetMethodReference (MethodReference method)
        {
            if (method == null)
                return null;

            MethodReference result = null;
            string name = method.Name;
            string fullname = method.ToString ();

            Tracer.Trace(2,"Searching:      " + fullname);
            if (m_cfile != null
                && GetTypeReference (method.DeclaringType) == m_cfile.MainType) {
                MethodReference base_method = null;

                if (m_cfile.DefinedSymbolsTable.Contains (name))
                    base_method = (MethodReference) m_cfile.DefinedSymbolsTable[name];
                else if (m_cfile.PrivateSymbolsTable.Contains (name))
                    base_method = (MethodReference) m_cfile.PrivateSymbolsTable[name];

                if (base_method != null) {
                    if (MethodIsVarArg (base_method)) {
                        result = duplicate_VarargMethodReference (method, base_method);
                    } else {
                        result = base_method;
                    }
                }
            } else {
                TypeReference type = GetTypeReference (method.DeclaringType);

                if (type is TypeDefinition) {
                    TypeDefinition td = type as TypeDefinition;
                    if (method is MethodDefinition) {
                        MethodDefinition md = method as MethodDefinition;
                        if (md.IsConstructor)
                            result = td.Constructors.GetConstructor (md.IsStatic, md.Parameters);
                        else
                            result =  td.Methods.GetMethod (md.Name, md.Parameters);
                    } else {
                        if (method.Name == MethodDefinition.Ctor || method.Name == MethodDefinition.Cctor)
                            result = td.Constructors.GetConstructor (!method.HasThis, method.Parameters);
                        else if (MethodIsVarArg (method)) {
                            MethodReference base_method = null;
                            MethodReference []methods = td.Methods.GetMethod (method.Name);
                            if (methods.Length != 1) {
                                throw new NotFoundException (method);
                            }
                            base_method = methods[0];
                            if (base_method != null) {
                                result = duplicate_VarargMethodReference (method, base_method);
                            }
                        }
                        else
                            result =  td.Methods.GetMethod (method.Name, method.Parameters);
                    }
                } else {
                    result =  (MethodReference) GetMemberReference (method);
                }
            }

            if  (result != null) {
                result.DeclaringType = GetTypeReference (result.DeclaringType);
                result.ReturnType.ReturnType = GetTypeReference (result.ReturnType.ReturnType);
                foreach (ParameterDefinition parameter in result.Parameters) {
                    TypeReference tp = parameter.ParameterType;
                    parameter.ParameterType = GetTypeReference (parameter.ParameterType);
                    if (tp is SentinelType)
                        parameter.ParameterType = new SentinelType (parameter.ParameterType);
                }
            } else if (m_aea && method.DeclaringType.Name == ExternalTypeName) {
                if (m_module.TypeReferences [ExternalTypeName] == null) {
                    AssemblyNameReference asm = AssemblyNameReference.Parse (ExternalAssemblyName);
                    m_module.AssemblyReferences.Add(asm);
                    TypeReference type = new TypeReference (ExternalTypeName, "", asm, false);
                    m_module.TypeReferences.Add (type);
                }
		result = new MethodReference (method.Name,
                                              m_module.TypeReferences [ExternalTypeName],
                                              GetTypeReference (method.ReturnType.ReturnType),
                                              method.HasThis,
                                              method.ExplicitThis,
                                              method.CallingConvention);

                foreach (ParameterDefinition param in method.Parameters) {
                    TypeReference tp = GetTypeReference (param.ParameterType);
                    if (param.ParameterType is SentinelType)
                        tp = new SentinelType (tp);
                    result.Parameters.Add (new ParameterDefinition (tp));
                }
                m_module.MemberReferences.Add (result);
            } else {
                throw new NotFoundException (method);
            }

            Tracer.Trace(2,"Returning:      " + result.ToString());
            return result;
        }

        public virtual FieldReference GetFieldReference (FieldReference field)
        {
            FieldReference result = null;
            string name = field.Name;
            string fullname = field.ToString ();

            Tracer.Trace(2,"Searching:      " + fullname);
            if (m_cfile != null
                && (field.DeclaringType.Name == "<MODULE>"
                    || field.DeclaringType == m_cfile.MainType
                    || field.DeclaringType.Name == ExternalAssemblyName)) {
                if (m_cfile.DefinedSymbolsTable.Contains (name))
                    result = (FieldReference) m_cfile.DefinedSymbolsTable[name];
                else if (m_cfile.PrivateSymbolsTable.Contains (name))
                    result = (FieldReference) m_cfile.PrivateSymbolsTable[name];
            } else {
                TypeReference type = GetTypeReference (field.DeclaringType);
                if (type is TypeDefinition) {
                    TypeDefinition td = type as TypeDefinition;
                    result = td.Fields.GetField (field.Name);
                } else {
                    result = (FieldReference) GetMemberReference (field);
                }
            }

            if (result == null) {
                if (m_aea && field.DeclaringType.Name == ExternalTypeName) {
                    if (m_module.TypeReferences [ExternalTypeName] == null) {
                        AssemblyNameReference asm = AssemblyNameReference.Parse (ExternalAssemblyName);
                        m_module.AssemblyReferences.Add(asm);
                        TypeReference type = new TypeReference (ExternalTypeName, "", asm, false);
                        m_module.TypeReferences.Add (type);
                    }
                    result = new FieldReference (field.Name,
                                                 m_module.TypeReferences [ExternalTypeName],
                                                 GetTypeReference (field.FieldType));
                    m_module.MemberReferences.Add (result);
                } else
                    throw new NotFoundException (field);
            }

            Tracer.Trace(2,"Returning:      " + result.ToString ());
            return result;
        }

        public virtual CallSite GetCallSite (CallSite cs)
        {
            cs.ReturnType.ReturnType = GetTypeReference (cs.ReturnType.ReturnType);
            for (int i = 0; i < cs.Parameters.Count ; ++i) {
                cs.Parameters[i].ParameterType = GetTypeReference (cs.Parameters[i].ParameterType);
            }
            return cs;
        }
    }

}
