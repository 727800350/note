// A spreadsheet consists of many sheets
var spreadsheet = SpreadsheetApp.getActive(); 
var sheet = spreadsheet.getSheetByName("contacts"); 


var url = "https://docs.google.com/spreadsheet/ccc?key=0Ao-_w-u7UzjWdFdnVjI4TTZsTnI4UWduMnNpNm5ac1E#gid=0";
var sheets = SpreadsheetApp.openByUrl(url); //这个返回的是一个Spreadsheet 对象, 是这个spreadsheet中的所有sheets 构成的一个集合
var sheet = sheets.getSheetByName("mail");

//key后面的 0Ao-_w-u7UzjWdFdnVjI4TTZsTnI4UWduMnNpNm5ac1E 实际上是 SpreadsheetApp.openById() 的参数
SpreadsheetApp.getActiveSheet(); //得到当前处于激活状态的sheet


function mark() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var range = sheet.getDataRange();

  var position = 'P'.charCodeAt(0) - 'A'.charCodeAt(0)+1; //返回的是一个浮点数, 但是也可以用作getCell的参数
  for (var i = 6; i <= 99; i++) {   
    var cell = range.getCell(i,position);
    var score = cell.getValue();
    
    if(cell.isBlank()) 
      continue;
    if(score >= 5)      cell.setBackground("green");
    else if (score >=3) cell.setBackground("white");
    else if(score <= 2) cell.setBackground("red");       
    else Logger.log("error");
  }
};


var realkey  = String.fromCharCode(48);     //Ascii码转化成字符的方法, 把48转化成单个字符
var code = 'A'.charCodeAt(0) //把字符转换成Ascii的方法


var numRows = sheet.getLastRow();//得到这个sheet 已经使用了多少rows
sheet.getMaxRows();// 这个是得到这个sheet 一共有多少rows, 包括未经使用的
//相应的,这两个函数有column 的版本

//Range
//当一个cell 是空的时候, sheet.getRange(row, column).getValue() == "";

var dataRange = sheet.getDataRange();
//得到所有数据的范围

var dataRange = sheet.getRange(startRow, startColumn, numRows, numColumns);
//从startRow, startColumn 开始得到numRows 行与numColumns 列的数据范围
//sheet 的行 与列 都是从1 开始

var data = dataRange.getValues();
//得到这个范围内的数据, 是一个二维数组, 下标都是从0 开始

var subject = sheet.getRange(1,2).getValue(); 
//只提供两个参数可以得到具体的cell
sheet.getRange(startRow + i, 1).setBackground("red")
sheet.getRange(startRow + i, numColumns - startColumn + 1).setValue(EMAIL_SENT);


SpreadsheetApp.flush();
//刷新这个spreadsheet, 这样可以立刻看到结果
