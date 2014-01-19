function myFunction() {
  return sth; //也可以不返回值
}

//日志, 可以当做一个输出用(没有标准输出),在view->logs 中查看
Logger.log(sheet.getLastRow());

//spreadsheet 打开的时候触发这个函数
function onOpen() {
  var spreadsheet = SpreadsheetApp.getActive();
  var menuItems = [
    {name: 'Send Emails', functionName: 'sendEmails'},
    {name: 'Update Contacts sheet',functionName: 'updateContacts'}
  ];
  spreadsheet.addMenu('Emails', menuItems);
}
//为这个spreadsheet 添加一个Menu, 也就是一个菜单栏

//定义在函数外面的变量是全局变量
