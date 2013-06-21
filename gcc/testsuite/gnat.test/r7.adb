with Init7; use Init7;

procedure R7 is

  function Get_Elem (R : R1) return Integer is
    Tmp : R1 := R;
  begin
    return Tmp.N.C1;
  end;

  procedure Set_Elem (R : access R1; I : Integer) is
    Tmp : R1 := R.all;
  begin
    Tmp.N.C1 := I;
    R.all := Tmp;
  end;

  function Get_Elem (R : R2) return Integer is
    Tmp : R2 := R;
  begin
    return Tmp.N.C1;
  end;

  procedure Set_Elem (R : access R2; I : Integer) is
    Tmp : R2 := R.all;
  begin
    Tmp.N.C1 := I;
    R.all := Tmp;
  end;

  A1 : aliased R1 := My_R1;
  A2 : aliased R2 := My_R2;

begin

  if Get_Elem (A1) /= 16#AB0012# then
    raise Program_Error;
  end if;

  Set_Elem (A1'Access, 16#CD0034#);
  if Get_Elem (A1) /= 16#CD0034# then
    raise Program_Error;
  end if;

  if Get_Elem (A2) /= 16#AB0012# then
    raise Program_Error;
  end if;

  Set_Elem (A2'Access, 16#CD0034#);
  if Get_Elem (A2) /= 16#CD0034# then
    raise Program_Error;
  end if;

end;
