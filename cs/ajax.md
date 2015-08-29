�첽javascript �� xml
Asynchronous JavaScript and XML

```
WebClient wc = new WebClient();
string s = wc.DownloadString("getServerTime.ashx");
$("#textbox").val(s);
```
����һ��,��webҳ�淢��һ������,�õ����ݺ�,�͸���

XMLHTTPRequest���ǳ䵱��webclient,����ҳ�治��ˢ�¾Ϳ��Ի�÷�����������

XMLHTTPRequest����ajax�ĺ��Ķ���

IE������汾��ajax ʵ��
```
<html>
<head>
	<title></title>
<script type="text/javascript">
	function btnclick(){
		var xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		if(!xmlhttp){
			alert("xmlhttp ����ʧ��");
			return false;
		}
		xmlhttp.open("POST", "GetDate.ashx", false); // ׼�����������GetDate.ashx ҳ�淢��post����
		// xmlhttp Ĭ�ϲ���ͬ�������, Ҳ����open ����������web client ��downloadstring �����ѷ��������ص������õ��ŷ���, ���첽��, �����Ҫ����onreadystatechange�¼�
		xmlhttp.onreadystatechange = function(){
			if(xmlhttp.readystate == 4){
				if(xmlhttp.status == 200){ // 200 ��ʾ�ɹ�
					document.getElementById("Text").value = xmlhttp.responseText;
				}
				else{
					alert("xmlhttp return error");
				}
			}
		}
		xmlhttp.send(); // ��ʱ�ŷ�������
	}
</head>
<body>
	<input id="Text1" type="text" /><input id="Button1" type="button" value="button" onclick="btnclick()" />
</body>
</html>
```
��Ϊ���첽��,�����޷�ͨ��send�ķ���ֵ�õ�����,��Ҫͨ���¼�������

readystate

- 0:����δ��ʼ��(��û�е��� open()).
- 1:�����Ѿ�����,���ǻ�û�з���(��û�е��� send()).
- 2:�����ѷ���,���ڴ�����(ͨ�����ڿ��Դ���Ӧ�л�ȡ����ͷ).
- 3:�����ڴ�����,ͨ����Ӧ�����в������ݿ�����,���Ƿ�������û�������Ӧ������.
- 4:��Ӧ�����,�����Ի�ȡ��ʹ�÷���������Ӧ��.

ͬʱ����Ҳ���Լ������
```
xmlhttp.open("POST", "GetDate.ashx?id=1715", false)
```
ͨ��context.Request���ǿ��Եõ�����ֵ
```
context.Response.Write( DateTime.Now.ToShortTimeString() +"---"+context.Request["id"]);
```

## Jquery ���ajax
`$.post`: Load data from the server using a HTTP POST request.

```
functionButton1_onclick(){
	$.post("GetDate.ashx", function (data, Status) {
		if (Status == "success") {//dataΪ���ص�����
			$("#Text1").val(data);
		}
		else {
			alert('fail');
		}
	})
}
```
Ҳ���Դ������
```
jQuery.post = function( url, data, callback, type );
url: String A string containing the URL to which the request is sent.
data: String A map or string that is sent to the server with the request.//���Ϊ�ֵ�
callback: Function A callback function that is executed if the request succeeds.
type: String The type of data expected from the server. Default: Intelligent Guess (xml, json, script, or html).
```

## ����̻���̵߳ľ�������
Ajax�����첽�ķ�ʽ��������ύ����.�Է���������,���봫ͳ���ύ���������޲�ͬ,�������������첽֮��ʽ�ύ,���ͬʱ�ж��Ajax���󼰴����ύ����,���޷���֤��һ�������Ȼ�÷���������Ӧ.
������Ӧ�ó�����͵Ķ����(process)����߳�(thread)�ľ���(racing)����.
����Ա��˱������д������JavaScript���涯�ֽ��Ա������ྺ������ķ���(��Ajax����δ��Ӧ֮ǰ,��disable�ͳ���ť),���ֲ���Ҫ�������˳���Ա�ĸ���.
Ŀǰ��֪���Զ����������֮��������ƺ�ֻ��ZK.

