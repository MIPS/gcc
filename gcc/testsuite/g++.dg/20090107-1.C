/* { dg-do compile } */
/* { dg-options "-fwhopr -Wuninitialized -O1" }  */

template <typename T> struct Q1 { typedef int x; };
template <typename T> struct Q2 {
  typename Q1<T>::x f() {
    int k;
    return k; /* { dg-warning "'k' is used uninitialized in this function" }  */
  }
};
int foo() { return Q2<int>().f(); }
