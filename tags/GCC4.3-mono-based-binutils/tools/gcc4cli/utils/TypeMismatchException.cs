
using System;

using Mono.Cecil;

namespace gcc4cli.util {

    public sealed class TypeMismatchException : CFileException {

        private string type1;
        private string type2;

        public string FirstType {
            get { return type1; }
        }

        public string SecondType {
            get { return type2; }
        }

        internal TypeMismatchException (string message, string type_1, string type_2) :
            base (message)
        {
            type1 = type_1;
            type2 = type_2;
        }

        public override string Message {
            get {
                if (FirstType!=SecondType)
                    return String.Format ("Type Missmatch ({0},{1}): {2}", FirstType, SecondType, base.Message);
                else
                    return String.Format ("Type Missmatch ({0}): {1}", FirstType, base.Message);
            }
        }
    }

}
