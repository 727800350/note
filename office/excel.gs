综合了前面几个函数, 用户选择一个目录, 然后将这个目录中的每个文件导入到这个workbook 的新的sheet 中, 这个sheet的名字为这个文件的名字</B>
Sub ImportData()
    On Error Resume Next
    Dim shell As Variant
    Set shell = CreateObject("Shell.Application")
    Set filePath = shell.BrowseForFolder(&O0, "选择文件夹", &H1 + &H10, "")   '获取文件夹路径地址 手动选择
    Set shell = Nothing
  	If filePath Is Nothing Then                 '检测是否获得有效路径，如取消直接跳出程序
       Exit Sub
    Else
       gg = filePath.Items.Item.Path
    End If
    Set obj = CreateObject("Scripting.FileSystemObject")   '定义变量
    Set fld = obj.getfolder(gg)                           '获取路径
   
    For Each ff In fld.Files                   '遍历文件夹里文件
    
        '如果这个文件存在,将这个文件删除, 然后在添加这个名字的文件
        '清空这个sheet 也可以, Worksheets("sheet2").Cells.Clear
        If Worksheets(ff.Name) Is Nothing Then '判断以ff.Name 为名字的sheet 是否存在
            Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = ff.Name
        Else
            Application.DisplayAlerts = False '删除时不用确认
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
