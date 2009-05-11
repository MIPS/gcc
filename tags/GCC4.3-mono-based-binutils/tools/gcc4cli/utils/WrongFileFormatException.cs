
using System;

using Mono.Cecil;

namespace gcc4cli.util {

    public sealed class WrongFileFormatException : CFileException {

        private string fileName;

        public string FileName {
            get { return fileName; }
        }

        public WrongFileFormatException (string message, string filename) :
            base (message)
        {
            fileName = filename;
        }

    }

}
