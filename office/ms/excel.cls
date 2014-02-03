# encoding: utf-8

'在最后添加一个新的sheet'
Sub AddWorksheetAfterLast()
On Error Resume Next
Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = "newsheet"  
    With Worksheets(Worksheets.Count)
     .Cells(1, 1) = 1
     .Cells(1, 2) = 2
    End With
End Sub
Sheets.Add(Before, After, Count, Type)
'其中参数Before,After指定新建工作表的位置,Coun指定工作表的数量,Type指定新建工作表的类型'

'循环'
For Each sht In Worksheets
    If sht.Name = "Sheet1" Then
        MsgBox "存在"
        Exit Sub
    End If
Next

Dim x As Integer
For x=1 To Sheets.Count
    If Sheets(x).Name = "Sheet4" Then
	'当"Sheet4"工作表存在时,运行该程序会弹出显示"已存在Sheet4工作表"'
    MsgBox "已存在Sheet4工作表"
    End if
Next X

'判断sheet 存在与否'
Sub pd()
On Error Resume Next
If Sheets("张三") Is Nothing Then
MsgBox "工作表不存在"
Else
MsgBox "工作表存在"
End If
End Sub

Sub testWorksheetExists1()
  Dim ws As Worksheet
  If Not WorksheetExists(ThisWorkbook, "sheet1") Then
    MsgBox "不能够找到该工作表", vbOKOnly
    Exit Sub
  End If
  MsgBox "已经找到工作表"
  Set ws = ThisWorkbook.Worksheets("sheet1")
End Sub

Sub a()
Dim sht As Worksheet
For Each sht In Worksheets
    If sht.Name = "Sheet1" Then
        MsgBox "存在"
        Exit Sub
    End If
Next
MsgBox "不存在"
End Sub
