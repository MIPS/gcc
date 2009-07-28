
using System;
using System.IO;
using System.Text;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.objdump {

    class CilWriter : TextWriter {

        TextWriter m_writer;

        int m_indentLevel = 0;

        bool new_line = true;

        public override Encoding Encoding {
            get { return m_writer.Encoding; }
        }

        public CilWriter (TextWriter writer) {
            m_writer = writer;
        }

        public int IndentLevel {
            get { return m_indentLevel; }
            set { m_indentLevel = value; }
        }

        public void Indent () {
            m_indentLevel++;
        }

        public void Unindent () {
            m_indentLevel--;
        }

        public void Indent (int value) {
            m_indentLevel += value;
        }

        public void Unindent (int value) {
            m_indentLevel -= value;
            if (m_indentLevel < 0)
                m_indentLevel =0;  
        }

        void WriteIndent ()
        {
            if (new_line) {
                for (int i = 0; i < m_indentLevel; i++)
                    m_writer.Write ("  ");
                new_line = false;
            } else
                m_writer.Write (" ");
        }

        public override void Write (string value)
        {
            WriteIndent ();
            m_writer.Write (value);
        }

        public override void Write (char [] buffer, int index, int count)
        {
            WriteIndent ();
            m_writer.Write (buffer, index, count);
        }

        public override void Write (char value)
        {
            WriteIndent ();
            m_writer.Write (value);
        }

        public override void WriteLine ()
        {
            m_writer.WriteLine ();
            new_line = true;
        }

        public void WriteUntokenized (string value)
        {
            m_writer.Write (value);
        }

        public void WriteSymbolName (string name)
        {
            Write (CilSymbols.GetSymbolName (name));
        }

        public void WriteTypeReference (TypeReference type, bool only_name, bool convert_primitive)
        {
            Write (CilSymbols.GetTypeReferenceString (type, only_name, convert_primitive));
        }

        public void WriteMemberReference (MemberReference member)
        {
            Write (CilSymbols.GetMemberString (member));
        }

        public void WriteFieldReference (FieldReference field)
        {
            Write (CilSymbols.GetFieldReferenceString (field));
        }

        public void WriteCallingConvention (MethodCallingConvention callconv)
        {
            Write (CilSymbols.GetCallingConventionString (callconv));
        }

        public void WriteVisibility (TypeAttributes attributes)
        {
            string temp = "";
            switch (attributes & TypeAttributes.VisibilityMask) {
            case TypeAttributes.NotPublic:         temp = "private";            break;
            case TypeAttributes.Public:            temp = "public";             break;
            case TypeAttributes.NestedPrivate:     temp = "nested private";     break;
            case TypeAttributes.NestedPublic:      temp = "nested public";      break;
            case TypeAttributes.NestedFamily:      temp = "nested family";      break;
            case TypeAttributes.NestedAssembly:    temp = "nested assembly";    break;
            case TypeAttributes.NestedFamANDAssem: temp = "nested famandassem"; break;
            case TypeAttributes.NestedFamORAssem:  temp = "nested famorassem";  break;
            }
            Write (temp);
        }

        public void WriteVisibility (MethodAttributes attributes)
        {
            string temp = "";
            switch (attributes & MethodAttributes.MemberAccessMask) {
//             case MethodAttributes.Compilercontrolled:
//                 break;
            case MethodAttributes.Private:     temp = "private";     break;
            case MethodAttributes.Public:      temp = "public";      break;
            case MethodAttributes.Family:      temp = "family";      break;
            case MethodAttributes.Assem:       temp = "assembly";    break;
            case MethodAttributes.FamANDAssem: temp = "famandassem"; break;
            case MethodAttributes.FamORAssem:  temp = "famorassem";  break;
            }
            Write (temp);
        }

        public void WriteVisibility (FieldAttributes attributes)
        {
            string temp = "";
            switch (attributes & FieldAttributes.FieldAccessMask) {
//             case FieldAttributes.Compilercontrolled:
//                 break;
            case FieldAttributes.Private:     temp = "private";     break;
            case FieldAttributes.Public:      temp = "public";      break;
            case FieldAttributes.Family:      temp = "family";      break;
            case FieldAttributes.Assembly:    temp = "assembly";    break;
            case FieldAttributes.FamANDAssem: temp = "famandassem"; break;
            case FieldAttributes.FamORAssem:  temp = "famorassem";  break;
            }
            Write (temp);
        }

        public void WriteImplementationAttributes (MethodImplAttributes attributes)
        {
            // PInvokeInfo handled separately

            string temp = "";
            switch (attributes & MethodImplAttributes.CodeTypeMask) {
            case MethodImplAttributes.IL:      temp = "cil";     break;
            case MethodImplAttributes.Native:  temp = "native";  break;
            case MethodImplAttributes.Runtime: temp = "runtime"; break;
            case MethodImplAttributes.OPTIL:   temp = "optil";   break;
            }
            Write (temp);

            switch (attributes & MethodImplAttributes.ManagedMask) {
            case MethodImplAttributes.Managed:   temp = "managed";   break;
            case MethodImplAttributes.Unmanaged: temp = "unmanaged"; break;
            }
            Write (temp);

            // TODO: Write forwardref, preservesig, internalcall, synchronized, noinlining
        }

        public void Write (PInvokeAttributes attributes)
        {
            if ((attributes & PInvokeAttributes.NoMangle) != 0)
                Write ("nomangle");

            string temp = "";
            switch (attributes & PInvokeAttributes.CharSetMask) {
            case PInvokeAttributes.CharSetAnsi:       temp = "ansi";     break;
            case PInvokeAttributes.CharSetUnicode:    temp = "unicode";  break;
            case PInvokeAttributes.CharSetAuto:       temp = "autochar"; break;
            case PInvokeAttributes.SupportsLastError: temp = "lasterr";  break;
            }
            Write (temp);

            switch (attributes & PInvokeAttributes.CallConvMask) {
            case PInvokeAttributes.CallConvWinapi:   temp = "winapi";   break;
            case PInvokeAttributes.CallConvCdecl:    temp = "cdecl";    break;
            case PInvokeAttributes.CallConvStdCall:  temp = "stdcall";  break;
            case PInvokeAttributes.CallConvThiscall: temp = "thiscall"; break;
            case PInvokeAttributes.CallConvFastcall: temp = "fastcall"; break;
            }
            Write (temp);
        }

        public void WriteMethodReference (MethodReference method)
        {
            Write (CilSymbols.GetMethodReferenceString (method));
        }

        public void WriteCallSite (CallSite method)
        {
            Write (CilSymbols.GetCallsiteString (method));
        }

        public void WriteLabel (Instruction instr)
        {
            Write ("IL_{0:X4}", instr.Offset);
        }

        public void WriteByteArray (byte[] arg)
        {
            if (new_line)
                WriteIndent ();
            for (int i = 0; i < arg.Length; ++i) {
                if (i > 0) {
                    if (i % 16 == 0) {
                        WriteLine ();
                        WriteIndent ();
                    } else
                        m_writer.Write (" ");
                }
                m_writer.Write ("{0:X2}", arg [i]);
            }
        }

        public override void Close ()
        {
            m_writer.Close ();
        }

        public override string ToString ()
        {
            return m_writer.ToString ();
        }
    }
}

