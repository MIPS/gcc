// { dg-options "-std=c++2a" }

template<class T>
concept Addable =
 requires(T x){
  {x + x} -> T;
 };

int main(){
 Addable t = 0;
}
