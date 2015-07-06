Program Forest;

Const
  tree = 1;
  car = 2;
  man = 3;
  train = 54;

Var
  Map : Array [1..10,1..10] Of Integer;
  n, m : Integer;

Function Build_Fence : Integer;
Var
  i, j, fence_items_count : Integer;
Begin
  fence_items_count := 0;
  For i := 2 To n - 1 Do
  Begin
    For j := 2 To m - 1 Do
    Begin
      If Map[i,j] <> 0 Then
      Begin
        If Map[i,j+1] = 0 Then Inc(fence_items_count);
        If Map[i+1,j] = 0 Then Inc(fence_items_count);
        If Map[i,j-1] = 0 Then Inc(fence_items_count);
        If Map[i-1,j] = 0 Then Inc(fence_items_count);
      End;
    End;
  End;
  Build_Fence := fence_items_count;
End;

Procedure Check_Around_Recurs(x, y : Integer);
//Var
Begin
  If (x >= 2 And x < n And y in [2..m-1]) Then
  Begin
    If (Map[x,y+1] <> 1) And (Map[x,y+1] <> -1) And (Map[x,y+1] <> -2) Then
    Begin
      If Map[
    End;
    If (Map[x+1,y] <> 1) And (Map[x+1,y] <> -1) And (Map[x+1,y] <> -2) Then
    Begin

    End;
    If (Map[x-1,y] <> 1)  And (Map[x-1,y] <> -1) And (Map[x-1,y] <> -2) Then
    Begin

    End;
    If (Map[x,y-1] <> 1) And (Map[x,y-1] <> -1) And (Map[x,y-1] <> -2)  Then
    Begin

    End;
  End;
End;

Procedure Check_Around;
Var
  i, j, temp, temp1 : Integer;
Begin
  temp := 0;
  temp1 := 0;
  For i := 2 To n - 1 Do
  Begin
    For j := 2 To m - 1 Do
    Begin
      If Map[i,j] <> 1 Then
      Begin
        If Map[i,j+1] = 1 Then
          Inc(temp)
        Else
          Inc(temp1);
        If Map[i+1,j] = 1 Then
          Inc(temp)
        Else
          Inc(temp1);
        If Map[i,j-1] = 1 Then
          Inc(temp)
        Else
          Inc(temp1);
        If Map[i-1,j] = 1 Then
          Inc(temp)
        Else
          Inc(temp1);
      End;
      If temp = 4 Then
        Map[i,j] := tree;
      If temp1 = 4 Then
        Map[i,j] := -1
      Else
        Map[i,j] := -2;
      temp := 0;
      temp1 := 0;
      Check_Around_Recurs(i,j);
    End;
  End;
End;

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

Procedure Fill_Map(k, p : Integer);
Var
  i, j : Integer;
Begin
  For i := 1 To k Do
  Begin
    For j := 1 To p Do
    Begin
      Map[i,j] := 0;
    End;
  End;
End;

Procedure Read_Map;
Var
  i, j, k, prom : Integer;
  temp : String;
  A : Text;
Begin
  k := 1;
  Assign(A, 'input.txt');
  Reset(A);
  Read(A, n);
  Readln(A, m);
  Inc(n);Inc(n);
  Inc(m);Inc(m);
  Fill_Map(n,m);
  While not EOF(A) Do
  Begin
    For i := 2 to n - 1 Do
    Begin
      Readln(A, temp);
      For j := 2 to m - 1 Do
      Begin
        prom := Ord(temp[k]) - 48;
        Map[i,j] := prom;
        Inc(k);
      End;
    k := 1;
    End;
  End;
  Close(A);
End;
  
Begin
  Read_Map;
  Check_Around;
  Writeln(Build_Fence);
End.
