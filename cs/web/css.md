Cascading Style Sheets �����ʽ��

����һ������õ�css �ļ�: `<link href="StyleSheet1.css" rel="stylesheet" type="text/css" />`

������λ:px����;30%;em��Ե�λ.width:20px

background-color������ɫ;color�ı���ɫ

border

border-style;border-color;border-width(Ĭ��ֵ��0)

`style="border-style:dotted;border-color:Red;border-width:2px"`

display������ʾ���none(����ʾ);block(��ʾΪ�鼶Ԫ��,Ԫ��ǰ����л��з�);inline(Ԫ��ǰ��û�л��з�)

CSS float ����

float ���Զ���Ԫ�����ĸ����򸡶�.�������������Ӧ����ͼ��,ʹ�ı�Χ����ͼ����Χ,������CSS ��,�κ�Ԫ�ض����Ը���.����Ԫ�ػ�����һ���鼶��,�������������Ǻ���Ԫ��.
����������滻Ԫ��,��Ҫָ��һ����ȷ�Ŀ��,����,���ǻᾡ���ܵ�խ.
ע��:������һ��֮��ֻ�м��ٵĿռ�ɹ�����Ԫ��,��ô���Ԫ�ػ�������һ��,������̻������ĳһ��ӵ���㹻�Ŀռ�Ϊֹ.
JavaScript �﷨:object.style.cssFloat="left"

cursor:���ָ��ŵ�Ԫ������ʱ��ʾ������  
`td.style.cursor = "pointer";`//td��һ����table�е�һ����Ԫ��,���ŵ������Ԫ������ʱ,������ֵ���״

list-style-type:����<ul></ul><li></li>�б�ʱ���Ǹ�Բ��Ŀ���
```
onfocus="this.style.textAlign='left';" onblur="this.style.textAlign='right';"
```

## ��ʽѡ����
����:��ǩѡ����;classѡ����;idѡ����

��ǩ
```
input{background-color:Green;border-color:Black;}
p{color:Blue;}
```

class
```
.Class_warning{font-style:italic;font-size:xx-large;}//ǰ��ĵ㲻�ܶ�
<p class=" Class_warning ">aaaaa</p>
```
���ж����ʽһ�������໥��ͻʱ,����styleֱ��ָ����Ϊ׼

 
��ǩ+class
```
input.class1{background-color:Green;border-color:Black;}
p.class1{color:Blue;}
```

id
#id_username
```
{ background-color:Red; }
```

����ѡ����

ʹ�ÿո�ָ�
```
p strong{color:Yellow;}
```
ͬʱ����p��strong��ǩ

