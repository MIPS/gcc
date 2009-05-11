
namespace gcc4cli.util {

    public interface ICFileBuilder {
        
        bool Building { get; }

        void StartBuilding ();
        
        void Add (CFile cfile);

        void EndBuilding ();
    }

}
