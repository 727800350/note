VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True'
END
Attribute VB_Name = "Sheet1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Sub SetLink()
    Dim x As Integer
    Dim n As String
    
    Cells(3, 1) = "序号"
    Cells(3, 2) = "表名"
    Cells(3, 3) = "用途"
    Cells(3, 4) = "备注"
    Cells(3, 5) = "创建日期"
    Cells(3, 6) = "修改日期"
    Cells(3, 7) = "修改记录"
    ' 第一个sheet 是目录,第二个是DataDic, 所以跳过'
    For x = 3 To Worksheets.Count
        If Worksheets(x).Name <> "目录" Then
            
            n = Worksheets(x).Name
            Cells(x + 1, 1) = x - 2
            Cells(x + 1, 2).Select
              ActiveSheet.Hyperlinks.Add Anchor:=Selection, Address:="", SubAddress:=n & "!B1", TextToDisplay:=n
        End If
		'添加返回目录的链接'
       Sheets(x).Hyperlinks.Add Anchor:=Sheets(x).Range("C1"), Address:="", SubAddress:="目录" & "!B" & (x + 1), TextToDisplay:="返回目录"
    Next
End Sub
