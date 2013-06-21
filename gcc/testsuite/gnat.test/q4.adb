with Init4; use Init4;
with Ada.Numerics; use Ada.Numerics;
with Text_IO; use Text_IO;
with Dump;

procedure Q4 is

  A1 : R1 := My_R1;
  B1 : R1 := My_R1;

  A2 : R2 := My_R2;
  B2 : R2 := My_R2;

begin
  Put ("A1 :");
  Dump (A1'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("B1 :");
  Dump (B1'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("A2 :");
  Dump (A2'Address, R1'Max_Size_In_Storage_Elements);
  New_Line;

  Put ("B2 :");
  Dump (B2'Address, R2'Max_Size_In_Storage_Elements);
  New_Line;

  if A1.F /= B1.F then
    raise Program_Error;
  end if;

  if A1.F /= Pi then
    raise Program_Error;
  end if;

  if A2.F /= B2.F then
    raise Program_Error;
  end if;

  if A2.F /= Pi then
    raise Program_Error;
  end if;

end;
