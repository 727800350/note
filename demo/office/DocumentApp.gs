var doc = DocumentApp.getActiveDocument();
var doc = DocumentApp.openById('1w5jWQbYkwn7VnhobPSvclMy0GU90RNiZsMoUoIrR-rU');

doc.saveAndClose();


 // Define a custom paragraph style.
 var style = {};
 style[DocumentApp.Attribute.HORIZONTAL_ALIGNMENT] =
     DocumentApp.HorizontalAlignment.RIGHT;
 style[DocumentApp.Attribute.FONT_FAMILY] =
     DocumentApp.FontFamily.CALIBRI;
 style[DocumentApp.Attribute.FONT_SIZE] = 18;
 style[DocumentApp.Attribute.BOLD] = true;
style[DocumentApp.Attribute.FOREGROUND_COLOR] = '#000000';
//black #000000; yellow #FFFF00; white #FFFFFF, 也就是 255,255,0
//http://www.colorcombos.com/colors
//http://www.w3schools.com/tags/ref_color_tryit.asp?

 // Append a plain paragraph.
 var par = doc.appendParagraph('A paragraph with custom style.');

 // Apply the custom style.
 par.setAttributes(style);


//我用来处理单词文档的格式设置
function toNormalStyle() {
  var normalStyle = {};
  normalStyle[DocumentApp.Attribute.FOREGROUND_COLOR] = '#000000';//文字用黑色
  normalStyle[DocumentApp.Attribute.BACKGROUND_COLOR] = '#FFFFFF';//背景用白色
  normalStyle[DocumentApp.Attribute.BOLD] = false;//黑体
  normalStyle[DocumentApp.Attribute.UNDERLINE] = false; //下划线
  normalStyle[DocumentApp.Attribute.ITALIC] = false; //斜体
  normalStyle[DocumentApp.Attribute.LINK_URL] = ""; //去除超链接
  
  DocumentApp.getActiveDocument().editAsText()
  .setAttributes(normalStyle);
 }
