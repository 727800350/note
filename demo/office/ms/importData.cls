VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True'
END

'导入一个text 文件到excel中'
'熟悉导入文本文件'
Sub AddQuery()
    Sheet1.UsedRange.ClearContents
    With Sheet1.QueryTables.Add( _
        Connection:="TEXT;" & ThisWorkbook.Path & "\acknowledges.txt", _
        Destination:=Range("A1"))
        .TextFilePlatform = 936
        .TextFileCommaDelimiter = True
        .Refresh
    End With
End Sub

'遍历一个用户选择的目录, 将遍历到的文件信息写入到excel 中'
'熟悉目录的操作'
Sub getpath()
	'清空A2：C1000列'
    Range("A2:C1000").ClearContents              
    On Error Resume Next
    Dim shell As Variant
    Set shell = CreateObject("Shell.Application")
	'获取文件夹路径地址 手动选择'
    Set filePath = shell.BrowseForFolder(&O0, "选择文件夹", &H1 + &H10, "")   
    Set shell = Nothing
  '检测是否获得有效路径，如取消直接跳出程序'
  If filePath Is Nothing Then                 
       Exit Sub
    Else
       gg = filePath.Items.Item.Path
  End If
    Set obj = CreateObject("Scripting.FileSystemObject")
	'获取路径'
    Set fld = obj.getfolder(gg)                         
    m = 0
    Cells(1, 1) = "Name"
    Cells(1, 2) = "Path"
    Cells(1, 3) = "Size"
    Cells(1, 4) = "DateCreated"
	 '遍历文件夹里文件'
     For Each ff In fld.Files                   
        m = m + 1
        Cells(m + 1, 1) = ff.Name
        Cells(m + 1, 2) = ff.Path
        Cells(m + 1, 3) = ff.Size
        Cells(m + 1, 4) = ff.DateCreated
      Next
End Sub

Sub ImportData()
    On Error Resume Next
    Dim shell As Variant
    Set shell = CreateObject("Shell.Application")
	'获取文件夹路径地址 手动选择'
    Set filePath = shell.BrowseForFolder(&O0, "选择文件夹", &H1 + &H10, "")   
    Set shell = Nothing
  '检测是否获得有效路径，如取消直接跳出程序'
  If filePath Is Nothing Then                 
       Exit Sub
  Else
       gg = filePath.Items.Item.Path
  End If
	'定义变量'
    Set obj = CreateObject("Scripting.FileSystemObject")   
	'获取路径'
    Set fld = obj.getfolder(gg)                         
   
	'遍历文件夹里文件'
    For Each ff In fld.Files                   
        If Worksheets(ff.Name) Is Nothing Then
            Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = ff.Name
        Else
			'删除时不用确认'
            Application.DisplayAlerts = False 
            Worksheets(ff.Name).Delete
            Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = ff.Name
        End If
      
        With Worksheets(ff.Name).QueryTables.Add(Connection:="TEXT;" & ff.Path, Destination:=Range("A1"))
            .TextFilePlatform = 936
            .TextFileCommaDelimiter = True
            .Refresh
        End With
    Next
End Sub

