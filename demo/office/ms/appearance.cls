VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True'
END

'为每个sheet 设置字体, 也就是统一字体'
Sub setFont()
On Error Resume Next
For Each s In Worksheets
    s.Cells.Font.Name = "Times New Roman"
Next
End Sub

