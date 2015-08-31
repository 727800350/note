# intro
Document Object Model�ĵ�����ģ��

CSS+JavaScript+DOM=DHTML

# ��ʱ����
`var id = setTimeout(fn, delay);`
Initiates a single timer which will call the specified function after the delay(����)
The function returns a unique ID with which the timer can be canceled at a later time.

`var id = setInterval(fn, delay);` 
Similar to setTimeout but continually calls the function (with a delay every time) until it is canceled.

`clearInterval(id);`, `clearTimeout(id);`
Accepts a timer ID (returned by either of the aforementioned functions) and stops the timer callback from occurring.

`setTimeout(myTimeoutFunction, 1000);` vs `setInterval(myTimeoutFunction, 1000);`

- setInterval code executes every 1000ms exactly, while...
- setTimeout waits 1000ms, runs the function (which takes few ms), then sets another timeout. So the wait period is actually a bit more than 1000ms.

# �¼�
- srcElement: ����¼�Դ����
- keyCode: �����¼�ʱ�İ���ֵ
- button: �����¼�ʱ����갴��, 1Ϊ���, 2Ϊ�Ҽ�, 3Ϊ���Ҽ�ͬʱ��

## body, document������¼�
- onload:��ҳ������Ϻ󴥷�
- onunload:��ҳ�ر�(�����뿪)�󴥷�
- onbeforeunload:����ҳ׼���ر�(�����뿪)ʱ����
- onbeforeunload="window.event.returnValue='�ı��ᶪʧ'"
- onclick ����
- ondblclick˫��
- onkeydown��������
- onkeypress�������
- onkeyup�����ͷ�
- onmousedown��갴��
- onmousemove����ƶ�
- onmouseout����뿪Ԫ�ط�Χ
- onmouseover����ƶ���Ԫ�ط�Χ
- onmouseup��갴���ͷ�
- onmousedown :���������ʱ�������¼�
- ��ַlocation

- onchangeֻ���ڹ���뿪ʱ��ִ��
�����Ǻ����,�������һ��text�ı���,�������ݵ�ʱ��,���������һֱ�ڸı�,���һ�ı�ʹ���,��ôonchange����¼��ͻ�һֱ���ڴ���״̬.

- onfocus: �������ؼ����¼���
- onblur: �����뿪�ؼ����¼���

navigate: `<input type="button" onclick="navigate('HTMLPageForm.htm')" value="navigate demo" />`

`window.location.href`
�ȿ���ͨ����ȡ�õ�ǰ��ַ,Ҳ���Ը�����ֵʵ��navigate��Ч��

`window.location.reload()`ˢ��

`window.event` ����¼�����ʱ����Ϣ,ʱ�䲻������window������¼�.������winForm�е�e(EventArg)

`<a href="HTMLPageTable.htm" onclick="alert('��ֹ����');window.event.returnValue=false;">hi</a>`
�����û���ע��ʱ,������Ϣ����,���ʱ��Ͳ����ύ��������,�Ϳ���ͨ�������ʽ
 
# ����Ĳ���
setAttribute���������һЩhtml����û�е�����, ����:
```
label.setAttribute("studentId", "39241715");//�����������Ժ�ʹ��
var id=label.getAttribute(("studentId");//�����Ϳ���ȡ�����ֵ
```

innerText, innerHTML: 
�������е�DOMԪ�ض���innerText��innerHTML����Ԫ��,������Ԫ���Ǵ�Сд���е�,�ҿ��Զ���д

## ��ȡ����
- getElementById();
- getElementsByName();

һ��ҳ����name���Բ�Ψһ,���Է��صĽ����һ������
```
function btnclick(){
	var radios = document.getElementsByName("gender");
	for(var i = 0; i < radios.length; i++){
		alert(radios[i].value);
	}
}
```

## createElement
��̬��ӵ�����ͨ���鿴��ҳԴ����ķ�ʽ�ǿ�������,����ͨ��debugbar�鿴 

��ӵ� div ��
```
var div = document.getElementById("divMain");
var input = document.createElement("input");
input.type = "button";
input.value = "je suis dynamique";
div.appendChild(input);
```

��ӵ� document��body ��
```
var input = document.createElement("<input type='button' value='hello'/>");
document.body.appendChild(input);
```

����label
```
var label = document.createElement("label");
label.innerText = "user";
label.setAttribute("for", "username");
// username��һ���ı���,��Ϊfor��js����һ���ؼ���,����ֱ��ʹ��label.for="username");�ǲ��е�
document.body.appendChild(label);
```

## �������
### clipboardData����
��ճ����Ĳ���
�����κ�Ԫ�ض���oncopy��onpaste��������

- `clearData("Text")`: ���ճ����,����ֻ��ʱText(ע:������Ϊ���Ժ����չ������Ҫ��)
- `getData("Text")`: ��ȡճ�����ֵ,����ֵΪճ�����е�����
- `setData("Text",val)`: ����ճ�����е�ֵ
 
```
<body oncopy="alert('��ֹ����');window.event.returnValue=false;"></body>
<body oncopy="alert('��ֹ����'); return false;"></body>
 
mobile:<input type="text" /><br />
mobile again:<input type="text" onpaste="alert('Ϊ��֤����ʽ��ֵ����ȷ���ֻ���,������');return false;" />

function modifyClipboard(){
	clipboardData.setData('Text', clipboardData.getData('Text') +"����ת��:"+ window.location.href);
}
<body oncopy="setTimeout('modifyClipboard()', 100)"></body>
```

### window.history
- window.history
- window.history.back();  window.history.go(-1);����һ��ҳ��
- window.history.forward();  window.history.g0(1);
- window.history.go(2);
```
<a href="javascript:window.history.back()">back</a>

Ҳ���Էŵ�input button��onclick�¼���
<input type="button" onclick="window.history.back()" value="back"  />
```

### window.document
���ĵ���д������ 
```
window.document.write("this is a document<br/>");
window.document.write("<a href='http://www.buaa.edu.cn'>buaa</a><br/>");
```

# style
style.display����, ���Ʋ���ʾ���.
ֻ�е�ֵΪnoneʱ,�Ų���ʾ,����ֵ(������ֵ)����ʾ

css��background-color,��js����backgroundcolor,��Ϊ��js��,-������ĺ���,����

# form
form submit
ͨ��һ����ͨ�İ�ťҲ���ύ��
```
<form action="default.aspx" method="post" id="form1">
	<input type="button" name="name" value="same as submit" onclick="form1.submit()" />
	<input type="submit" />
</form>
```
����ύ֮ǰҪ�������,������form��`onsubmit`(�����return false;�Ϳ�����ֹ�ύ)�¼���д����������ݼ��,
�������ͨ��`form1.submit()`�����ύ,������ִ��onsubmit��Ĵ���.

����,���onsubmit�¼�Ҫ���ú���,Ҫ�ر�ע�ⷵ��ֵ�Ĵ���
```
function beforeSubmit(){
   var name = document.getElementById("name");
   if (name.value.length <= 0) {
   		alert('name can not be empty');
		return false;
   }
}
onsubmit="return beforeSubmit()"
```

ͨ��һ����ť�ĵ����������һ����ť�������
```
<input type="button" id="btn1" value="btn" onclick="alert('btn')" />
<input type="button" name="name" value="clickonme"  onclick="btn1.click()" />
//����btn1.onclick();
```

