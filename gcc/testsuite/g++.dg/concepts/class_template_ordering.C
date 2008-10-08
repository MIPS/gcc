concept Superior<typename T> { };
concept Refined<typename T> : Superior<T> { };
concept UeberRefined<typename T> : Refined<T> { };
concept AlsoRefined<typename T> : Superior<T> { };

template<typename T>
struct Picky
{
  static const int value = 0;
};

template<typename T> requires Superior<T>
struct Picky<T>
{
  static const int value = 1;
};

template<typename T> requires Refined<T>
struct Picky<T>
{
  static const int value = 2;
};

template<typename T> requires UeberRefined<T>
struct Picky<T>
{
  static const int value = 3;
};

template<typename T> requires AlsoRefined<T>
struct Picky<T>
{
  static const int value = 4;
};

class S { };
class R { };
class UR { };
class AR { };
class R_AR { };

concept_map Superior<S> { };
concept_map Refined<R> { };
concept_map UeberRefined<UR> { };
concept_map AlsoRefined<AR> { };
concept_map Refined<R_AR> { };
concept_map AlsoRefined<R_AR> { };

int array0[Picky<int>::value == 0? 1 : -1];
int array1[Picky<S>::value == 1? 1 : -1];
int array2[Picky<R>::value == 2? 1 : -1];
int array3[Picky<UR>::value == 3? 1 : -1];
int array4[Picky<AR>::value == 4? 1 : -1];
int array5[Picky<R_AR>::value]; // { dg-error "ambiguous class template instantiation" }
// { dg-error "struct Picky" "" { target *-*-* } 32 }
// { dg-error "struct Picky" "" { target *-*-* } 20 }
// { dg-error "struct Picky" "" { target *-*-* } 14 }
// { dg-error "incomplete type" "" { target *-*-* } 54 }
