
using System;
using Mono.Cecil;
using Mono.Cecil.Cil;
using System.Collections;

using gcc4cli.util;

namespace gcc4cli.util {

    public class CodeFixer : ICodeVisitor {

        IReferenceGetters m_reffixer;

        internal CodeFixer (IReferenceGetters reffixer)
        {
            m_reffixer = reffixer;
        }

        public void FixReferences (MethodBody body)
        {
            if (body == null)
                return;
            body.Accept (this);
        }

        void VisitCollection (ICollection coll)
        {
            foreach (ICodeVisitable visitable in coll)
                visitable.Accept (this);
        }

        public void VisitMethodBody (MethodBody body) { }

        public void VisitInstructionCollection (InstructionCollection instructions)
        {
            VisitCollection (instructions);
        }

        public void VisitInstruction (Instruction instr)
        {
            switch (instr.OpCode.OperandType) {
            case OperandType.InlineField:
                instr.Operand = m_reffixer.GetFieldReference (instr.Operand as FieldReference);
                break;
            case OperandType.InlineMethod:
                instr.Operand = m_reffixer.GetMethodReference (instr.Operand as MethodReference);
                break;
            case OperandType.InlineSig:
                instr.Operand = m_reffixer.GetCallSite (instr.Operand as CallSite);
                break;
            case OperandType.InlineTok:
                if (instr.Operand is TypeReference)
                    instr.Operand = m_reffixer.GetTypeReference (instr.Operand as TypeReference);
                else if (instr.Operand is FieldReference)
                    instr.Operand = m_reffixer.GetFieldReference (instr.Operand as FieldReference);
                else if (instr.Operand is MethodReference)
                    instr.Operand = m_reffixer.GetMethodReference (instr.Operand as MethodReference);
                break;
            case OperandType.InlineType:
                instr.Operand = m_reffixer.GetTypeReference (instr.Operand as TypeReference);
                break;
            default:
            case OperandType.InlineBrTarget:
            case OperandType.InlineI:
            case OperandType.InlineI8:
            case OperandType.InlineNone:
            case OperandType.InlinePhi:
            case OperandType.InlineR:
            case OperandType.InlineString:
            case OperandType.InlineSwitch:
            case OperandType.InlineVar:
            case OperandType.InlineParam:
            case OperandType.ShortInlineBrTarget:
            case OperandType.ShortInlineI:
            case OperandType.ShortInlineR:
            case OperandType.ShortInlineVar:
            case OperandType.ShortInlineParam:
                // Nothing to be done
                break;
            }
        }

        public void VisitExceptionHandlerCollection (ExceptionHandlerCollection seh)
        {
            VisitCollection (seh);
        }

        public void VisitExceptionHandler (ExceptionHandler eh)
        {
            if (eh.Type == ExceptionHandlerType.Catch)
                eh.CatchType = m_reffixer.GetTypeReference (eh.CatchType);
        }

        public void VisitVariableDefinitionCollection (VariableDefinitionCollection variables)
        {
            VisitCollection (variables);
        }

        public void VisitVariableDefinition (VariableDefinition var)
        {
            TypeReference tr = var.VariableType;
            var.VariableType = m_reffixer.GetTypeReference (tr);
        }

        public void VisitScopeCollection (ScopeCollection scopes)
        {
            VisitCollection (scopes);
        }

        public void VisitScope (Scope s) { }

        public void TerminateMethodBody (MethodBody body) { }
    }
}
