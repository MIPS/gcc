
using System;
using Mono.Cecil;
using Mono.Cecil.Cil;
using System.Collections;

using gcc4cli.util;

namespace gcc4cli.util {

    public class ReferencesFixer : IReflectionVisitor {

        IReferenceGetters m_reffixer;

        CodeFixer m_codefixer;

        public ReferencesFixer (IReferenceGetters reffixer)
        {
            m_reffixer  = reffixer;
            m_codefixer = new CodeFixer (m_reffixer);
        }

        public void FixReferences (ModuleDefinition MainModule)
        {
            Tracer.Trace ("Fix References of " + MainModule.Assembly.Name.Name);
            VisitTypeDefinitionCollection (MainModule.Types);
            VisitMemberReferenceCollection (MainModule.MemberReferences);
        }

        void VisitCollection (ICollection coll)
        {
            foreach (IReflectionVisitable visitable in coll)
                visitable.Accept (this);
        }

        public void VisitModuleDefinition (ModuleDefinition module) { }

        public void VisitTypeDefinitionCollection (TypeDefinitionCollection types)
        {
            VisitCollection (types);
        }

        public void VisitTypeDefinition (TypeDefinition type)
        {
            type.BaseType = m_reffixer.GetTypeReference (type.BaseType);
        }

        public void VisitTypeReferenceCollection (TypeReferenceCollection refs) { }

        public void VisitTypeReference (TypeReference type) { }

        public void VisitMemberReferenceCollection (MemberReferenceCollection members)
        {
            VisitCollection (members);
        }

        void VisitMethodReference (MethodReference method)
        {
            method.DeclaringType = m_reffixer.GetTypeReference (method.DeclaringType);
            method.GenericParameters.Accept (this);
            method.Parameters.Accept (this);
            method.ReturnType.ReturnType = m_reffixer.GetTypeReference (method.ReturnType.ReturnType);
        }

        void VisitFieldReference (FieldReference field)
        {
            field.DeclaringType = m_reffixer.GetTypeReference (field.DeclaringType);
            field.FieldType = m_reffixer.GetTypeReference (field.FieldType);
        }

        public void VisitMemberReference (MemberReference member)
        {
            if (member is MethodReference) {
                VisitMethodReference (member as MethodReference);
            } else if (member is FieldReference) {
                VisitFieldReference (member as FieldReference);
            } else
                throw new NotFoundException (member);
        }

        public void VisitInterfaceCollection (InterfaceCollection interfaces)
        {
            for (int i = 0; i < interfaces.Count; i++)
                interfaces [i] = m_reffixer.GetTypeReference (interfaces [i]);

            foreach (TypeReference interf in interfaces)
                VisitInterface (interf);
        }

        public void VisitInterface (TypeReference interf)
        {
            interf.DeclaringType = m_reffixer.GetTypeReference (interf.DeclaringType);
        }

        public void VisitExternTypeCollection (ExternTypeCollection externs)
        {
            VisitCollection (externs);
        }

        public void VisitExternType (TypeReference externType)
        {
            throw new NotSupportedException ("Extern Types not supported");
        }

        public void VisitOverrideCollection (OverrideCollection meth)
        {
            VisitCollection (meth);
        }

        public void VisitOverride (MethodReference ov)
        {
            throw new NotSupportedException ("Override not supported yet");
        }

        public void VisitNestedTypeCollection (NestedTypeCollection nestedTypes)
        {
            VisitCollection (nestedTypes);
        }

        public void VisitNestedType (TypeDefinition nestedType)
        {
            throw new NotSupportedException ("Nested Types Not Supported yet");
        }

        public void VisitParameterDefinitionCollection (ParameterDefinitionCollection parameters)
        {
            VisitCollection (parameters);
        }

        public void VisitParameterDefinition (ParameterDefinition parameter)
        {
            parameter.ParameterType = m_reffixer.GetTypeReference (parameter.ParameterType);
        }

        public void VisitMethodDefinitionCollection (MethodDefinitionCollection methods)
        {
            VisitCollection (methods);
        }

        public void VisitMethodDefinition (MethodDefinition method)
        {
            m_codefixer.FixReferences (method.Body);

            if (method.PInvokeInfo != null)
                method.PInvokeInfo = new PInvokeInfo (method,
                                                      method.PInvokeInfo.Attributes,
                                                      method.PInvokeInfo.EntryPoint,
                                                      m_reffixer.GetModuleReference (method.PInvokeInfo.Module));

            method.ReturnType.ReturnType = m_reffixer.GetTypeReference (method.ReturnType.ReturnType);
        }

        public void VisitConstructorCollection (ConstructorCollection ctors)
        {
            VisitCollection (ctors);
        }

        public void VisitConstructor (MethodDefinition ctor)
        {
            m_codefixer.FixReferences (ctor.Body);
        }

        public void VisitPInvokeInfo (PInvokeInfo pinvk) { }

        public void VisitEventDefinitionCollection (EventDefinitionCollection events)
        {
            VisitCollection (events);
        }

        public void VisitEventDefinition (EventDefinition evt)
        {
            throw new NotSupportedException ("Events not supported yet");
//             evt.DeclaringType = m_reffixer.GetTypeReference (evt.DeclaringType);

//             evt.EventType = m_reffixer.GetTypeReference (evt.EventType);

//             evt.AddMethod = (MethodDefinition) m_reffixer.GetMethodReference (evt.AddMethod);
//             evt.InvokeMethod = (MethodDefinition) m_reffixer.GetMethodReference (evt.InvokeMethod);
//             evt.RemoveMethod = (MethodDefinition) m_reffixer.GetMethodReference (evt.RemoveMethod);
        }

        public void VisitFieldDefinitionCollection (FieldDefinitionCollection fields)
        {
            VisitCollection (fields);
        }

        public void VisitFieldDefinition (FieldDefinition field)
        {
            field.DeclaringType = m_reffixer.GetTypeReference (field.DeclaringType);
            field.FieldType = m_reffixer.GetTypeReference (field.FieldType);
        }

        public void VisitPropertyDefinitionCollection (PropertyDefinitionCollection properties)
        {
            VisitCollection (properties);
        }

        public void VisitPropertyDefinition (PropertyDefinition property)
        {
            property.DeclaringType = m_reffixer.GetTypeReference (property.DeclaringType);
            property.PropertyType = m_reffixer.GetTypeReference (property.PropertyType);
            VisitCollection (property.Parameters);
            property.GetMethod = (MethodDefinition) m_reffixer.GetMethodReference (property.GetMethod);
            property.SetMethod = (MethodDefinition) m_reffixer.GetMethodReference (property.SetMethod);
        }

        public void VisitSecurityDeclarationCollection (SecurityDeclarationCollection secDecls) { }

        public void VisitSecurityDeclaration (SecurityDeclaration secDecl) { }

        public void VisitCustomAttributeCollection (CustomAttributeCollection customAttrs)
        {
            VisitCollection (customAttrs);
        }

        public void VisitCustomAttribute (CustomAttribute customAttr)
        {
            customAttr.Constructor = m_reffixer.GetMethodReference (customAttr.Constructor);

            Hashtable dict = new Hashtable (customAttr.Fields);
            foreach (DictionaryEntry entry in dict)
                customAttr.SetFieldType ((string) entry.Key,
                                         m_reffixer.GetTypeReference (customAttr.GetFieldType ((string) entry.Key)));

            dict = new Hashtable (customAttr.Properties);
            foreach (DictionaryEntry entry in dict)
                customAttr.SetPropertyType ((string) entry.Key,
                                            m_reffixer.GetTypeReference (customAttr.GetPropertyType ((string) entry.Key)));
        }

        public void VisitGenericParameterCollection (GenericParameterCollection genparams)
        {
            VisitCollection (genparams);
        }


        public void VisitGenericParameter (GenericParameter genparam)
        {
            throw new NotSupportedException ("Generics Not Supported yet");
        }

        public void VisitMarshalSpec (MarshalSpec marshalSpec)
        {
            if (marshalSpec.Container is FieldDefinition) {
                FieldReference fr = (FieldReference) marshalSpec.Container;
                TypeReference tr = m_reffixer.GetTypeReference (fr.DeclaringType);
                if (tr is TypeDefinition)
                    marshalSpec.Container = (tr as TypeDefinition).Fields.GetField (fr.Name);
                else
                    throw new NotFoundException (fr);
            }
        }

        public void TerminateModuleDefinition (ModuleDefinition module) { }

    }
}
