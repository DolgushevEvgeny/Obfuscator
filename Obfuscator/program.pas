Program lab1;

Const
  icon = '#';
  icon_1 = '.';

Var
  A : Text;
  symbol : Char;
  Map : Array [1..10,1..10] Of Char;
  n, m, i, j, count : Integer;

Procedure Print_Map;
Var
  i, j : Integer;
Begin
For i := 1 to n Do
  Begin
    For j := 1 to m Do
    Begin
      Write(Map[i,j]);
    End;
    Writeln;
  End;
End;

Procedure Paint_Map(k, p : Integer);
Begin
  Map[k,p] := icon_1;
  If (k + 1 <= n) Then
  Begin
    If (Map[k+1,p] = icon) Then
      Paint_Map(k+1,p);
  End;

  If (k - 1 >= 1) Then
  Begin
    If (Map[k-1,p] = icon) Then
      Paint_Map(k-1,p);
  End;

  If (p + 1 <= m) Then
  Begin
    If (Map[k,p+1] = icon) Then
      Paint_Map(k,p+1);
  End;

  If (p - 1 >= 1) Then
  Begin
    If (Map[k,p-1] = icon) Then
      Paint_Map(k,p-1);
  End;

End;

Begin
count := 0;
Assign(A, 'input3.txt');
Reset(A);
Read(A, n);
Readln(A, m);
While not EOF(A) Do
Begin
  For i := 1 to n Do
  Begin
    For j := 1 to m Do
    Begin
      Read(A, symbol);
      Map[i,j] := symbol;
    End;
    Readln(A);
  End;
End;
Close(A);

For i := 1 To n Do
Begin
  For j := 1 To m Do
  Begin
    If (Map[i,j] = icon) Then
    Begin
      Paint_Map(i,j);
      Inc(count);
    End;
  End;
End;
Writeln(count);
End.