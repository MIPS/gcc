// { dg-options "-std=c++0x" }
namespace std {
 concept CopyConstructible<typename T> {
   T::T(const T&);
   T::~T();
 }
}

concept Vector<typename V> {
  typename ComponentType = V::ComponentType;
  ComponentType getComponent(const V&, int ind);
};

template <Vector Left, Vector Right>
class Add {
  typedef typename Left::ComponentType LeftComponentType;
  typedef typename Right::ComponentType RightComponentType;
  typedef Add<LeftComponentType,RightComponentType> ComponentType;
  const Left& l;
  const Right& r;
  Add(const Left& left, const Right& right) : l(left), r(right) {}
  ComponentType getComponent(int ind) const {
    return Vector<Left>::getComponent(l, ind) + Vector<Right>::getComponent(r, ind); // { dg-error "no match for" }
  }
};
