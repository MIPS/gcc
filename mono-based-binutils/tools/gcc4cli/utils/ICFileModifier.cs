
using System;

namespace gcc4cli.util {

    public interface ICFileModifier {


        void AddSymbolPrefix (string prefix);

        void AddTypePrefix (string prefix);

        void localizeSymbol (string name);

        void globalizeSymbol (string name);

        void renameSymbol (string name, string newname);

        void renameType (string name, string newname);

    }

}
