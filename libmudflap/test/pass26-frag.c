int *p;

__mf_set_options ("-wipe-stack");

{
  int array [10];
  p = & array[0];

  array[0] = 2;
  array[9] = 5;

  /* Array[] should be wiped clean at this point.  */
}

__mf_set_options ("-no-wipe-stack");

{ 
  int array2[10];

 /* hope that this is allocated on top of old array[] */  
  if (p != & array2[0])
    abort ();

  array2[5] = 6;

  /* Old values shouldn't still be around; the new one should.  */
  if (p[0] == 2 || p[9] == 5 || p[5] != 6)
    abort() ;

  /* array2[] should not be wiped at this point! */
}

{ 
  int array3[10];

 /* hope that this is allocated on top of old array[] and array2[]*/  
  if (p != & array3[0])
    abort ();

  array3[1] = 2;

  /* Check that old assignment is still around.  */
  if (p[5] != 6 || p[1] != 2)
    abort() ;
}
