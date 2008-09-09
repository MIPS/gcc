
using System;
using System.Collections;

using Mono.Cecil;

namespace gcc4cli.objdump {

    class Disassembler {

//         CilWriter m_writer;
//
//         CilWriter Writer {
//             get { return m_writer; }
//             set { m_writer = value; }
//         }

        CilWriter Writer;

        public Disassembler (CilWriter writer)
        {
            Writer = writer;
        }

        public void DisassembleAssembly (AssemblyDefinition asm)
        {
            // DisassembleManifest
            EmitModuleReferences (asm.MainModule.ModuleReferences);
            Writer.WriteLine ();

            EmitAssemblyNameReferences (asm.MainModule.AssemblyReferences);

            EmitAssemblyDefinition (asm);
            Writer.WriteLine ();

            EmitModuleDefinition (asm.MainModule);
            Writer.WriteLine ();

            EmitTypeDefinitions (asm.MainModule.Types);

            // TODO: EmitResources
        }

        void EmitModuleReferences (ModuleReferenceCollection modules)
        {
            foreach (ModuleReference module in modules) {
                Writer.Write (".module extern");
                Writer.WriteSymbolName (module.Name);
                Writer.WriteLine ();
            }
        }

        void EmitAssemblyDetails (AssemblyNameReference name)
        {
            Writer.Write (".ver");
            if (name.Version == null)
                Writer.WriteLine ("0:0:0:0");
            else {
                Version ver = name.Version;
                Writer.WriteLine ("{0}:{1}:{2}:{3}", ver.Major, ver.Minor, ver.Build, ver.Revision);
            }

            if (!(name is AssemblyNameDefinition) && name.PublicKeyToken != null && name.PublicKeyToken.Length > 0) {
                Writer.Write (".publickeytoken = (");
                Writer.WriteByteArray(name.PublicKeyToken);
                Writer.WriteUntokenized (")");
                Writer.WriteLine ();
            }

            if (name.PublicKey != null && name.PublicKey.Length > 0) {
                Writer.Write (".publickey =");
                Writer.WriteLine (name.PublicKey);
            }

            if (name.HashAlgorithm != AssemblyHashAlgorithm.None) {
                Writer.WriteLine (".hash algorithm 0x" + name.HashAlgorithm.ToString ("x"));
            }
        }

        void EmitAssemblyNameReferences (AssemblyNameReferenceCollection references)
        {
            foreach (AssemblyNameReference reference in references) {
                Writer.Write (".assembly extern");
                Writer.WriteSymbolName (reference.Name);
                Writer.WriteLine ();

                Writer.WriteLine ("{");
                Writer.Indent ();
                EmitAssemblyDetails (reference);
                Writer.Unindent ();
                Writer.WriteLine ("}");

                Writer.WriteLine ();
            }
        }

        void EmitAssemblyDefinition (AssemblyDefinition asm)
        {
            Writer.Write (".assembly");
            Writer.WriteSymbolName (asm.Name.Name);
            Writer.WriteLine ();

            Writer.WriteLine ("{");
            Writer.Indent ();
            EmitAssemblyDetails (asm.Name);

            EmitCustomAttributes (asm.CustomAttributes);
            EmitSecurityDeclarations (asm.SecurityDeclarations);

            Writer.Unindent ();
            Writer.WriteLine ("}");
        }

        void EmitModuleDefinition (ModuleDefinition module)
        {
            Writer.Write (".module");
            Writer.WriteSymbolName (module.Name);
            Writer.WriteLine ();
            Writer.WriteLine ("// MVID: {0:b}", module.Mvid);
        }

        void EmitCustomAttributes (CustomAttributeCollection customAttrs)
        {
            foreach (CustomAttribute attr in customAttrs) {
                Writer.Write (".custom");
                Writer.WriteMethodReference (attr.Constructor);
                Writer.Write ("= (");
                Writer.Indent (8);
                if (attr.Resolve ())
                    Writer.WriteByteArray(CilHelper.GetCustomAttributeByteArray(attr));
                else if (attr.Blob != null)
                    Writer.WriteByteArray(attr.Blob);
                else
                    Writer.WriteUntokenized ("01 00 00 00");
                Writer.WriteUntokenized (")");
                Writer.Unindent (8);
                Writer.WriteLine ();

                Writer.WriteLine ();
            }
        }

        void EmitSecurityDeclarations  (SecurityDeclarationCollection secDecls)
        {
            // TODO:
//             foreach (SecurityDeclaration sec_decl in secDecls) {
//             }
        }

        void EmitTypeDefinitions (TypeDefinitionCollection types)
        {
            TypeDefinition m_type = types["<Module>"];
            // Emit <Module>
            if (m_type != null)
                EmitTypeDefinition (m_type, true, true);

            foreach (TypeDefinition type in types) {
                // Skip Nested Types and <Module>
                if (type.DeclaringType == null && type != m_type)
                    EmitTypeDefinition (type, true, false);
            }
        }

        void EmitTypeDefinition (TypeDefinition type, bool writeNamespace, bool isModule)
        {
            if (writeNamespace && type.Namespace.Length > 0) {
                Writer.Write (".namespace");
                Writer.WriteLine (type.Namespace);
                Writer.WriteLine ("{");
                Writer.Indent ();
            }

            if (!isModule) {
                EmitTypeHeader (type);
                Writer.WriteLine ("{");
                Writer.Indent ();
            } else {
                Writer.WriteLine ("// <Module> {");
                Writer.WriteLine ();
            }

            EmitTypeBody (type);

            if (!isModule) {
                Writer.Unindent ();
                Writer.Write ("} // end of class");
                Writer.WriteTypeReference (type, true, false);
                Writer.WriteLine ();
            } else
                Writer.WriteLine ("// } end of <Module>");

            if (writeNamespace && type.Namespace.Length > 0) {
                Writer.Unindent ();
                Writer.Write ("} // end of namespace");
                Writer.WriteLine (type.Namespace);
            }

            Writer.WriteLine ();
        }

        void EmitTypeHeader (TypeDefinition type)
        {
            Writer.Write (".class");

            if (type.IsInterface)
                Writer.Write ("interface");

            Writer.WriteVisibility (type.Attributes);

            if (type.IsAutoLayout)            Writer.Write ("auto");
            else if (type.IsSequentialLayout) Writer.Write ("sequential");
            else if (type.IsExplicitLayout)   Writer.Write ("explicit");

            if (type.IsAnsiClass)         Writer.Write ("ansi");
            else if (type.IsUnicodeClass) Writer.Write ("unicode");
            else if (type.IsAutoClass)    Writer.Write ("auto");

            if (type.IsAbstract)
                Writer.Write ("abstract");
            if (type.IsSealed)
                Writer.Write ("sealed");
            if (type.IsSpecialName)
                Writer.Write ("special-name");
            if (type.IsImport)
                Writer.Write ("import");
            if (type.IsSerializable)
                Writer.Write ("serializable");
            if (type.IsBeforeFieldInit)
                Writer.Write ("beforefieldinit");

            Writer.WriteSymbolName (type.Name);
            Writer.WriteLine ();

            if (type.BaseType != null) {
                Writer.Indent ();
                Writer.Write ("extends");
                Writer.WriteTypeReference (type.BaseType, true, false);
                Writer.WriteLine ();
                Writer.Unindent ();
            }

            if (type.Interfaces.Count > 0) {
                Writer.Indent ();
                Writer.Write ("implements");
                for (int i = 0; i < type.Interfaces.Count; i++) {
                    if (i > 0)
                        Writer.WriteUntokenized (",");
                    Writer.WriteTypeReference (type.Interfaces [i], true, false);
                }
                Writer.WriteLine ();
                Writer.Unindent ();
            }
        }

        void EmitNestedTypes (NestedTypeCollection nesteds)
        {
            if (nesteds.Count == 0)
                return;

            Writer.WriteLine ();
            foreach (TypeDefinition type in nesteds)
                EmitTypeDefinition (type, false, false);
        }

        void EmitTypeBody (TypeDefinition type)
        {
            EmitCustomAttributes (type.CustomAttributes);
            EmitSecurityDeclarations (type.SecurityDeclarations);

            if (type.HasLayoutInfo) {
                Writer.Write (".pack");
                Writer.WriteLine (type.PackingSize);
                Writer.Write (".size");
                Writer.WriteLine (type.ClassSize);
            }

            foreach (FieldDefinition field in type.Fields)
                EmitFieldDefinition (field);
            foreach (MethodDefinition ctor in type.Constructors)
                EmitMethodDefinition (ctor);
            foreach (MethodDefinition meth in type.Methods)
                EmitMethodDefinition (meth);
            foreach (PropertyDefinition property in type.Properties)
                EmitPropertyDefinition (property);
            foreach (EventDefinition evt in type.Events)
                EmitEventDefinition (evt);

            // skip <Module>
            if (type.MetadataToken.RID != 1)
                EmitNestedTypes (type.NestedTypes);
        }

        void EmitFieldDefinition (FieldDefinition field)
        {
            Writer.Write (".field");

            if (field.HasLayoutInfo)
                Writer.Write("[{0}]", field.Offset);

            Writer.WriteVisibility (field.Attributes);

            if (field.IsStatic)
                Writer.Write ("static");
            if (field.IsSpecialName)
                Writer.Write ("specialname");
            if (field.IsPInvokeImpl) {
                Writer.Write ("pinvokeimpl ()");
                throw new NotImplementedException();
            }
            if (field.IsRuntimeSpecialName)
                Writer.Write ("rtspecialname");
            if (field.IsInitOnly)
                Writer.Write ("initonly");
            if (field.IsLiteral)
                Writer.Write ("literal");
            if (field.IsNotSerialized)
                Writer.Write ("notserialized");

            Writer.WriteTypeReference (field.FieldType, false, true);

            Writer.WriteSymbolName (field.Name);

            if (field.RVA != Mono.Cecil.Binary.RVA.Zero)
                EmitFieldAt (field);

            if (field.HasDefault)
                EmitFieldDefault (field);

            Writer.WriteLine ();
        }

        void EmitFieldAt (FieldDefinition field)
        {
            int old_ilevel = Writer.IndentLevel;
            Writer.WriteLine ("at D_{0:X8}", field.RVA.Value);
            Writer.IndentLevel = 0;
            Writer.WriteLine (".data D_{0:X8} = bytearray (", field.RVA.Value);
            Writer.IndentLevel = 5;
            Writer.WriteByteArray(field.InitialValue);
            Writer.WriteUntokenized (")");
            Writer.IndentLevel = old_ilevel;
        }

        void EmitFieldDefault (FieldDefinition field)
        {
            TypeReference type = field.FieldType;
            if (type is TypeDefinition && (type as TypeDefinition).IsEnum) {
                type = (type as TypeDefinition).Fields.GetField("value__").FieldType;
            }

            Writer.Write ("=");

            switch (type.FullName) {
            case Constants.Boolean:
                if ((bool)field.Constant)
                    Writer.Write ("bool(true)");
                else
                    Writer.Write ("bool(false)");
                break;
            case Constants.Char:
                Writer.Write ("char(0x{0:x8})", field.Constant);
                break;
            case Constants.SByte:
                Writer.Write ("int8(0x{0:x2})", field.Constant);
                break;
            case Constants.Int16:
                Writer.Write ("int16(0x{0:x4})", field.Constant);
                break;
            case Constants.Int32:
                Writer.Write ("int32(0x{0:x8})", field.Constant);
                break;
            case Constants.Int64:
                Writer.Write ("int64(0x{0:x16})", field.Constant);
                break;
            case Constants.Byte:
                Writer.Write ("int8(0x{0:x2})", field.Constant);
                break;
            case Constants.UInt16:
                Writer.Write ("int16(0x{0:x4})", field.Constant);
                break;
            case Constants.UInt32:
                Writer.Write ("int32(0x{0:x8})", field.Constant);
                break;
            case Constants.UInt64:
                Writer.Write ("int64(0x{0:x16})", field.Constant);
                break;
            case Constants.Single:
                Writer.Write ("float32(0x{0:x8})", field.Constant);
                break;
            case Constants.Double:
                Writer.Write ("float64(0x{0:x16})", field.Constant);
                break;
            case Constants.String:
                Writer.Write ("\"{0}\"", field.Constant);
                break;
//             case Constants.Type:
//                 throw new NotSupportedException();
//                 break;
//             case Constants.Object:
//                 throw new NotSupportedException();
//                 break;
            default:
                throw new NotSupportedException();
            }
        }

        void EmitMethodDefinition (MethodDefinition method)
        {
            EmitMethodHeader (method);

            Writer.WriteLine ("{");
            Writer.Indent ();

            EmitMethodBody (method);

            Writer.Unindent ();
            Writer.Write ("} // end of method");
            Writer.WriteMemberReference (method);
            Writer.WriteLine ();

            Writer.WriteLine ();
        }

        void EmitParameterDefinitions (ParameterDefinitionCollection parameters)
        {
            Writer.Write ("(");
            for (int i = 0; i < parameters.Count; i++) {
                if (i > 0)
                    Writer.WriteUntokenized (", ");
                Writer.WriteUntokenized (CilSymbols.GetTypeReferenceString (parameters [i].ParameterType, false, true));
                Writer.WriteSymbolName (parameters [i].Name);
            }
            Writer.WriteUntokenized (")");
        }

        void EmitMethodHeader (MethodDefinition method)
        {
            Writer.Write (".method");

            Writer.WriteVisibility (method.Attributes);

            if (method.IsStatic)
                Writer.Write ("static");
            if (method.IsFinal)
                Writer.Write ("final");
            if (method.IsVirtual)
                Writer.Write ("virtual");
            if (method.IsHideBySig)
                Writer.Write ("hidebysig");
            if (method.IsAbstract)
                Writer.Write ("abstract");
            if (method.IsSpecialName)
                Writer.Write ("specialname");
            if (method.IsUnmanagedExport)
                Writer.Write ("export");
            if (method.IsRuntimeSpecialName)
                Writer.Write ("rtspecialname");
            if (method.IsNewSlot)
                Writer.Write ("newslot");

            Writer.WriteLine ();
            Writer.Indent ();

            if (method.HasThis)
                Writer.Write ("instance");

            Writer.WriteCallingConvention (method.CallingConvention);

            Writer.WriteTypeReference (method.ReturnType.ReturnType, false, true);
            Writer.WriteSymbolName (method.Name);

            EmitParameterDefinitions (method.Parameters);
            Writer.WriteLine ();

            Writer.WriteImplementationAttributes (method.ImplAttributes);
            Writer.WriteLine ();

            if (method.IsPInvokeImpl) {
                Writer.Write ("pinvokeimpl (");
                if (method.PInvokeInfo != null) {
                    if (method.PInvokeInfo.Module != null)
                        Writer.WriteSymbolName (method.PInvokeInfo.Module.Name);
                    if (method.PInvokeInfo.EntryPoint != null) {
                        Writer.Write ("as");
                        Writer.Write (method.PInvokeInfo.EntryPoint);
                    }
                    Writer.Write (method.PInvokeInfo.Attributes);
                }
                Writer.Write (")");
                Writer.WriteLine ();
            }

            Writer.Unindent ();
        }

        void EmitMethodBody (MethodDefinition method)
        {
            EmitCustomAttributes (method.CustomAttributes);
            EmitSecurityDeclarations (method.SecurityDeclarations);

            if (method.HasBody)
                MethodBodyDisassembler.EmitMethodBody (Writer, method);
        }

        void EmitPropertyDefinition (PropertyDefinition property)
        {
            Writer.Write (".property");

            if (property.IsSpecialName)
                Writer.Write ("specialname");
            else if  (property.IsRuntimeSpecialName)
                Writer.Write ("rtspecialname");

            Writer.WriteTypeReference (property.PropertyType, false, true);

            Writer.WriteSymbolName (property.Name);

            EmitParameterDefinitions (property.Parameters);
            Writer.WriteLine ();

            Writer.WriteLine ("{");
            Writer.Indent ();

            EmitCustomAttributes (property.CustomAttributes);

            if (property.SetMethod != null) {
                Writer.Write (".set");
                Writer.WriteMethodReference (property.SetMethod);
                Writer.WriteLine ();
            }
            if (property.GetMethod != null) {
                Writer.Write (".get");
                Writer.WriteMethodReference (property.GetMethod);
                Writer.WriteLine ();
            }

            Writer.Unindent ();
            Writer.WriteLine ("}");
        }

        void EmitEventDefinition (EventDefinition evt)
        {
            Writer.Write (".event");

            if (evt.IsSpecialName)
                Writer.Write ("specialname");
            else if  (evt.IsRuntimeSpecialName)
                Writer.Write ("rtspecialname");

            if (evt.EventType != null)
                Writer.WriteTypeReference (evt.EventType, false, true);

            Writer.WriteSymbolName (evt.Name);
            Writer.WriteLine ();

            Writer.WriteLine ("{");
            Writer.Indent ();

            EmitCustomAttributes (evt.CustomAttributes);

            if (evt.AddMethod != null) {
                Writer.Write (".addon");
                Writer.WriteMethodReference (evt.AddMethod);
                Writer.WriteLine ();
            }
            if (evt.RemoveMethod != null) {
                Writer.Write (".removeon");
                Writer.WriteMethodReference (evt.RemoveMethod);
                Writer.WriteLine ();
            }
            if (evt.InvokeMethod != null) {
                Writer.Write (".fire");
                Writer.WriteMethodReference (evt.InvokeMethod);
                Writer.WriteLine ();
            }

            Writer.Unindent ();
            Writer.WriteLine ("}");
        }

    }
}
