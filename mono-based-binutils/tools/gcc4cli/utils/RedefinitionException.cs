
using System;

using Mono.Cecil;

namespace gcc4cli.util {

    public sealed class RedefinitionException : CFileException {

        private string redefined;

        public string RedefinedSymbol {
            get { return redefined; }
        }

        internal RedefinitionException (string message, string symbol) :
            base (message)
        {
            redefined = symbol;
        }

        internal RedefinitionException (FieldDefinition field) :
            this ("Global variable " + field.ToString () + " redefined", field.Name)
        {
        }

        internal RedefinitionException (MethodDefinition method) :
            this ("Function " + method.ToString () + " redefined", method.Name)
        {
        }

    }

}
