template <class A> class B {   
  A a;                          
  B(A&aa);			// ERROR - near match
};  // ERROR - candidates
static B<int> b_int (3);	// ERROR - no matching function
