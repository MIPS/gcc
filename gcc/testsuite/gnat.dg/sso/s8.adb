with Init8; use Init8;
with Text_IO; use Text_IO;
with Dump;

procedure S8 is

  A1 : R1 := My_R1;
  A2 : R2 := My_R2;

  N1 : Nested1;
  N2 : Nested2;

  C1 : Integer;
  C2 : Integer;
  C3 : Integer;

begin

  Put ("A1 :");
  Dump (A1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("A2 :");
  Dump (A2'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  N1 := A1.N;
  C1 := N1.C1;
  C2 := N1.C2;
  C3 := N1.C3;

  Put_Line("C1 :" & C1'Img);
  Put_Line("C2 :" & C2'Img);
  Put_Line("C3 :" & C3'Img);

  N1.C1 := C1;
  N1.C2 := C2;
  N1.C3 := C3;
  A1.N := N1;

  N2 := A2.N;
  C1 := N2.C1;
  C2 := N2.C2;
  C3 := N2.C3;

  Put_Line("C1 :" & C1'Img);
  Put_Line("C2 :" & C2'Img);
  Put_Line("C3 :" & C3'Img);

  N2.C1 := C1;
  N2.C2 := C2;
  N2.C3 := C3;
  A2.N := N2;

  Put ("A1 :");
  Dump (A1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("A2 :");
  Dump (A2'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

end;
