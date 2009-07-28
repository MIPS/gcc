
using Mono.Cecil;

namespace gcc4cli.util {

    public interface IReferenceGetters {

        ModuleReference GetModuleReference (ModuleReference module);

        TypeReference GetTypeReference (TypeReference type);

        MethodReference GetMethodReference (MethodReference method);

        FieldReference GetFieldReference (FieldReference field);

        CallSite GetCallSite (CallSite sig);
    }

}
