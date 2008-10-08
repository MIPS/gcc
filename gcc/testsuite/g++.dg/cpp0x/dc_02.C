// { dg-do compile }
// { dg-options --std=gnu++0x }

struct A {

	int i, j;

	A () : A(0), j(0) { } // { dg-error "" "only initializer" }

	A (int _i) : i(_i) { }

};

struct B {

	int i, j;

	B () : i(0), B(0) { } // { dg-error "" "only initializer" }

	B (int _j) : j(_j) { }

};

struct C { };

struct D : public C {

	D () : C() { }

	D (float) : D(), C() { } // { dg-error "" "only initializer" }

	D (float, float): C(), D() { } // { dg-error "" "only initializer" }

};

