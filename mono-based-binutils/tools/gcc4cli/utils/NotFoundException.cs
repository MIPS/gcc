
using System;

using Mono.Cecil;

namespace gcc4cli.util {

    public sealed class NotFoundException : CFileException {

        private string notFound;

        public string NotFound {
            get { return notFound; }
        }

        internal NotFoundException (string message, string not_found) :
            base (message)
        {
            notFound = not_found;
        }

        internal NotFoundException (TypeReference tr) :
            this ("Can't find type: " + tr.FullName, tr.FullName)
        {
        }

        internal NotFoundException (MemberReference member) :
            this ("Can't find member: " + member.Name, member.Name)
        {
        }

        internal NotFoundException (MethodReference mr) :
            this ("Can't find method: " + mr.Name, mr.Name)
        {
        }

        internal NotFoundException (FieldReference fr) :
            this ("Can't find field: " + fr.Name, fr.Name)
        {
        }

    }

}
