
using System;
using System.Text;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.objdump {

    class MethodBodyDisassembler {

        CilWriter Writer;

        MethodBodyDisassembler (CilWriter writer)
        {
            Writer = writer;
        }

        static public void EmitMethodBody (CilWriter writer, MethodDefinition method)
        {
            MethodBodyDisassembler mbd = new MethodBodyDisassembler (writer);
            mbd.EmitMethodBody (method);
        }

        void EmitMethodBody (MethodDefinition method)
        {
            // TODO: Improve generation of ExceptionHandlers
            Writer.WriteLine ("// Method begins at RVA 0x{0}", method.RVA.Value.ToString ("x4"));
            Writer.WriteLine ("// Code size {0} (0x{0:x})", method.Body.CodeSize);
            Writer.WriteLine (".maxstack {0}", method.Body.MaxStack);
            if (IsEntryPoint(method))
                Writer.WriteLine (".entrypoint");

            EmitVariablesDefinition (method.Body);
            // TODO: Scopes
            EmitInstructions (method.Body);
            EmitExceptionHandlers (method.Body.ExceptionHandlers);
        }

        static bool IsEntryPoint (MethodDefinition method) {
            return method == method.DeclaringType.Module.Assembly.EntryPoint;
        }

        void EmitVariablesDefinition (MethodBody body)
        {
            VariableDefinitionCollection variables = body.Variables;

            if (variables.Count == 0)
                return;
            int ivalue = 5;
            Writer.Write (".locals");
            if (body.InitLocals) {
                Writer.Write ("init");
                ivalue = 8;
            }
            Writer.Write ("(");
            Writer.Indent (ivalue);
            for (int i = 0; i < variables.Count; i++) {
                VariableDefinition var = variables [i];
                if (i > 0) {
                    Writer.WriteUntokenized (",");
                    Writer.WriteLine ();
                }
                Writer.WriteTypeReference (var.VariableType, false, true);
                if (var.Name != null && var.Name.Length > 0)
                    Writer.WriteSymbolName (var.Name);
            }
            Writer.Unindent (ivalue);
            Writer.WriteLine (")");
        }

        void EmitInstructions (MethodBody body)
        {
            foreach (Instruction instr in body.Instructions) {

                Writer.WriteLabel (instr);
                Writer.WriteUntokenized (":  ");
                Writer.Write (instr.OpCode.Name);

                switch (instr.OpCode.OperandType) {
                case OperandType.InlineNone:
                    break;
                case OperandType.InlineSwitch:
                    {
                        Instruction [] targets = (Instruction []) instr.Operand;
                        Writer.Write ("( ");
                        Writer.Indent (10);
                        for (int i = 0; i < targets.Length; i++) {
                            if (i > 0) {
                                Writer.WriteUntokenized (",");
                                if (i % 8 == 0)
                                    Writer.WriteLine ();
                            }
                            Writer.WriteLabel (targets [i]);
                        }
                        Writer.Unindent (10);
                        Writer.WriteUntokenized (")");
                    }
                    break;
                case OperandType.ShortInlineBrTarget:
                case OperandType.InlineBrTarget:
                    Writer.WriteLabel (instr.Operand as Instruction);
                    break;
                case OperandType.ShortInlineVar:
                case OperandType.InlineVar:
                    {
                        VariableDefinition var = instr.Operand as VariableDefinition;
                        if (var.Name != null && var.Name.Length > 0)
                            Writer.Write (var.Name);
                        else
                            Writer.Write (var.Index);
                    }
                    break;
                case OperandType.ShortInlineParam:
                case OperandType.InlineParam:
                    {
                        ParameterDefinition param = instr.Operand as ParameterDefinition;
                        if (param.Name != null && param.Name.Length > 0)
                            Writer.WriteSymbolName (param.Name);
                        else
                            Writer.Write (param.Method.Parameters.IndexOf (param));
                    }
                    break;
                case OperandType.InlineI:
                case OperandType.ShortInlineI:
                    Writer.Write (instr.Operand.ToString ());
                    break;
                case OperandType.InlineI8:
                    Writer.Write (instr.Operand.ToString ());
                    break;
                case OperandType.InlineR:
                case OperandType.ShortInlineR:
                    Writer.Write (instr.Operand.ToString ());
                    break;
                case OperandType.InlineString:
                    {
                        string input = instr.Operand as string;
                        byte[] bytes = Encoding.UTF8.GetBytes(input);
                        if (bytes.Length == input.Length
                            && input.IndexOfAny(new char[]{'\0', '"'}) == -1) {
                            StringBuilder sb = new StringBuilder (input);
                            sb.Replace ("\t", "\\t");
                            sb.Replace ("\r", "\\r");
                            sb.Replace ("\n", "\\n");
                            Writer.Write ("\"{0}\"", sb.ToString());
                        } else {
                            bytes = Encoding.Unicode.GetBytes(input);
                            
                            Writer.Write ("bytearray ( ");
                            Writer.Indent(14);
                            Writer.WriteByteArray(bytes);
                            Writer.Write ("00)");
                            Writer.Unindent(14);
                        }
                    }
                    break;
                case OperandType.InlineType:
                    Writer.WriteTypeReference (instr.Operand as TypeReference, false, true);
                    break;
                case OperandType.InlineMethod:
                    Writer.WriteMethodReference (instr.Operand as MethodReference);
                    break;
                case OperandType.InlineField:
                    Writer.WriteFieldReference (instr.Operand as FieldReference);
                    break;
                case OperandType.InlineTok:
                    if (instr.Operand is TypeReference) {
                        Writer.WriteTypeReference (instr.Operand as TypeReference, true, true);
                    } else if (instr.Operand is FieldReference) {
                        Writer.Write ("field");
                        Writer.WriteFieldReference (instr.Operand as FieldReference);
                    } else if (instr.Operand is MethodReference) {
                        Writer.WriteMethodReference (instr.Operand as MethodReference);
                    } else
                        throw new Exception();
                    break;
		case OperandType.InlineSig:
                    Writer.WriteCallSite (instr.Operand as CallSite);
                    break;
                default:
                    throw new NotSupportedException(instr.OpCode.OperandType.ToString());
                }
                Writer.WriteLine ();

                switch (instr.OpCode.FlowControl) {
		case FlowControl.Branch:
		case FlowControl.Cond_Branch:
                    Writer.WriteLine ();
                    break;
                default:
                    break;
                }
            }
        }

        void EmitExceptionHandlers (ExceptionHandlerCollection seh)
        {
            foreach (ExceptionHandler eh in seh) {
                Writer.Write (".try");
                Writer.WriteLabel (eh.TryStart);
                Writer.Write ("to");
                Writer.WriteLabel (eh.TryEnd);
                switch (eh.Type) {
                case ExceptionHandlerType.Catch:
                    Writer.Write ("catch");
                    Writer.WriteTypeReference (eh.CatchType, true, false);
                    break;
                case ExceptionHandlerType.Filter:
                    Writer.Write ("filter");
                    Writer.WriteLabel (eh.FilterStart);
                    break;
                case ExceptionHandlerType.Finally:
                    Writer.Write ("finally");
                    break;
                case ExceptionHandlerType.Fault:
                    Writer.Write ("fault");
                    break;
                }
                Writer.Write ("handler");
                Writer.WriteLabel (eh.HandlerStart);
                Writer.Write ("to");
                Writer.WriteLabel (eh.HandlerEnd);
                Writer.WriteLine ();
            }
        }
    }
}
