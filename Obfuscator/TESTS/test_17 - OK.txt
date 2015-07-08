Program lab1;

Var
  i, j, count : Integer;

Begin
Readln(count);
For i := 1 to count Do
  Begin
    For j := 1 to count Do
    Begin
      Write(i + j);
    End;
    Writeln;
  End;
End.