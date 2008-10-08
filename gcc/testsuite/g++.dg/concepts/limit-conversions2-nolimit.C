// { dg-options "-fno-limit-concept-conversions" }

concept HasPlus<typename T> {
  T operator+(T const&, T const&);
}

struct X { };
X operator+(X, X);

struct Y { 
  Y(X);
  operator X() const;
};

concept_map HasPlus<X> { }

concept_map HasPlus<Y> { }

concept PlusShort<typename T> {
  T operator+(T const&, short);
}

struct W { };
struct Z { };

X operator+(X, short);
W operator+(W, int);
Z operator+(Z, char);

concept_map PlusShort<X> { }
concept_map PlusShort<W> { }
concept_map PlusShort<Z> { }
