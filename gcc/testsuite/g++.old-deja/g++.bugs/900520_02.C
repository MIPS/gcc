void take_b_array_ref (b_array_ref arg) { } // ERROR - passed to here
b_array_ref b_array_ref_gbl_obj0 = u_array_ref_gbl_obj0; // ERROR - invalid declaration
  take_b_array_ref (u_array_ref_gbl_obj0); // ERROR - invalid call
