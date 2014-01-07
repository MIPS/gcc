with Init5; use Init5;
with Text_IO; use Text_IO;
with Dump;

procedure P5 is

  Local_R1 : R1;
  Local_R2 : R2;

begin
  Put ("My_R1    :");
  Dump (My_R1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("My_R2    :");
  Dump (My_R2'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R1 := My_R1;
  Put ("Local_R1 :");
  Dump (Local_R1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R2 := My_R2;
  Put ("Local_R2 :");
  Dump (Local_R2'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R1 := (I => 16#12345678#,
               A => (16#AB0012#, 16#CD0034#, 16#EF0056#));
  Put ("Local_R1 :");
  Dump (Local_R1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R2 := (I => 16#12345678#,
               A => (16#AB0012#, 16#CD0034#, 16#EF0056#));
  Put ("Local_R2 :");
  Dump (Local_R2'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R1.I    := Local_R2.I;
  Local_R1.A(1) := Local_R2.A(1);
  Local_R1.A(2) := Local_R2.A(2);
  Local_R1.A(3) := Local_R2.A(3);

  Put ("Local_R1 :");
  Dump (Local_R1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Local_R2.I    := Local_R1.I;
  Local_R2.A(1) := Local_R1.A(1);
  Local_R2.A(2) := Local_R1.A(2);
  Local_R2.A(3) := Local_R1.A(3);

  Put ("Local_R2 :");
  Dump (Local_R2'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;
end;
