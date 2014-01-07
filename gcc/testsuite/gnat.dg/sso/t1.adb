package body T1 is

  function Compare1 (X, Y : R1) return Boolean is
  begin
    return X.I = Y.I;
  end;

  function Is_Special1 (X : R1) return Boolean is
  begin
    return X.I = 16#12345678#;
  end;

  function Compare2 (X, Y : R2) return Boolean is
  begin
    return X.I = Y.I;
  end;

  function Is_Special2 (X : R2) return Boolean is
  begin
    return X.I = 16#12345678#;
  end;

end T1;
