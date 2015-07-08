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
i := 1;
While i < n + 1 Do
Begin 
j := 1;
While j < m + 1 Do
Begin 
Write(Map[i,j]); 
inc(j);
End; 
Writeln; 
inc(i);
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
i := 1;
While i < n + 1 Do
Begin 
j := 1;
While j < m + 1 Do
Begin 
Read(A, symbol); 
Map[i,j] := symbol; 
inc(j);
End; 
Readln(A); 
inc(i);
End; 
End; 
Close(A); 
i := 1;
While i < n + 1 Do
Begin 
j := 1;
While j < m + 1 Do
Begin 
If (Map[i,j] = icon) Then 
Begin 
Paint_Map(i,j); 
Inc(count); 
End; 
inc(j);
End; 
inc(i);
End; 
Writeln(count); 
End. 
