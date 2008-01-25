// { dg-options "-std=c++0x" }
// Check parsing of concepts
concept C1<typename T> { }
concept C2<typename T, typename U> { };
auto concept C3<int N> { }
