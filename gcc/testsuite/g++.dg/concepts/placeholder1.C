// { dg-options "-std=c++1z" }

template<typename T>
concept bool C1 = true;

template<typename T, typename U>
concept bool C2 = true;

template<typename T>
concept bool C3() { return true; }

template<typename T, typename U>
concept bool C4() { return true; }


C1      c1 = 0;
C2<int> c2 = 0;
C3      c3 = 0;
C4<int> c4 = 0;
