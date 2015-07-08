Program lab1;

Var
  i, j, count : Integer;

Begin
count := 0;
For i := 7 downto 1 Do
  Begin
    For j := 7 downto 1 Do
    Begin
      Write(i + j);
    End;
    Writeln;
  End;
End.