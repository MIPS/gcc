enum enum0 { enum0_value_0 };	
  int enum0;			
  void member_function (enum0 e); // ERROR - invalid use of struct-local member
void class0::member_function (enum0 e) {	// ERROR - syntax error
