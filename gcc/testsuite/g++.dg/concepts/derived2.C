#include "core_concepts.h"

struct Polygon { 
  virtual int num_sides() const = 0;
  virtual ~Polygon() = 0;
};

struct Triangle : Polygon { 
  virtual int num_sides() const { return 3; }
};

struct Rectangle : Polygon { 
  virtual int num_sides() const { return 4; }
};

int sides(Polygon& poly) {
  return poly.num_sides();
}

template<std::DerivedFrom<Polygon> T>
int double_sides(T& poly) {
  return sides(poly) * 2;
}
