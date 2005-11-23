// http://bugzilla.redhat.com/bugzilla/show_bug.cgi?id=129350
// { dg-do compile }

template<class T> struct A {
        struct B { };
};

class C {
        template<class T> friend struct A<T>::B;
};
