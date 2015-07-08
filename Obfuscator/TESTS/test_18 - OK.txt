Program lab1;

Var
  i, j, count : Integer;

Begin
Readln(count);
For i := count downto 1 Do
  Begin
    For j := count downto 1 Do
    Begin
      Write(i + j);
    End;
    Writeln;
  End;
End.