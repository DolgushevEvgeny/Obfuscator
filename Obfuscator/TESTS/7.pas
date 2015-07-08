Program lab1;

Var
  i, j, count : Integer;

Begin
count := 0;
For i := 1 to 3 Do
  Begin
    For j := 1 to 3 Do
    Begin
      Write(i + j);
    End;
    Writeln;
  End;
End.