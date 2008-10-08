// { dg-options "-std=c++0x" }
// { dg-do "link" }
// Contributed by Malin Ljungberg

concept GridSpec<typename G> {
 const int size();
}; 

template <int N> 
class Geometry {
 public:
  static const int SIZE = N;
};

template <int N>
concept_map GridSpec<Geometry<N> > { 
  const int size(); 
};

template <int N>
const int GridSpec<Geometry<N> >::size() { return Geometry<N>::SIZE; }

template<GridSpec G> int grid_size(const G& g) { return GridSpec<G>::size(); }

int main()
{
  grid_size(Geometry<17>());
}
