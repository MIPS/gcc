// { dg-do compile { target c++2a } }

template<class T>
concept Addable =
 requires(T x){
  {x + x} -> T;
 };

int main(){
 Addable auto t = 0;
}
