// { dg-options "-fno-limit-concept-conversions" }

concept HasPlusEquals<typename T> {
  T& operator+=(T&, T const&);
}

struct X { };
X& operator+=(X&, X);

struct Y { 
  Y(X);
  operator X() const;
};
Y& operator+=(Y&, X); 

concept_map HasPlusEquals<X> { }

concept_map HasPlusEquals<Y> { } 

concept PlusEqualsShort<typename T> {
  T& operator+=(T&, short);
}

struct W { };
struct Z { };

X& operator+=(X&, short);
W& operator+=(W&, int);
Z& operator+=(Z&, char);

concept_map PlusEqualsShort<X> { }
concept_map PlusEqualsShort<W> { }
concept_map PlusEqualsShort<Z> { }
