VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True'
END

'在最后添加一个sheet'
Sub AddWorksheetAfterLast()
      On Error Resume Next
      Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = "new"
      With Worksheets(Worksheets.Count)
      .Cells(1, 1) = 1
      .Cells(1, 2) = 2
      End With
End Sub

