#ifndef __SERVERWEB_H
#define __SERVERWEB_H
#include "stm32f4xx.h"  
#include "types.h"


#if 0
#define INDEX_HTML "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta charset = \"utf-8\">"\
"<title>������ҳCJC</title>"\
"</head>"\
"<body>"\
"<h1>�ҵĵ�һ������</h1>"\
"<p>�ҵĵ�һ������</p>"\
"</body>"\
"</html>" 

#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>Ұ������������W5500����ҳ����</title>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
"<style type='text/css'>"\
"body {text-align:left; background-color:#c0deed;font-family:Verdana;}"\
"#main {margin-right:auto;margin-left:auto;margin-top:30px;}"\
"label{display:inline-block;width:150px;}"\
"#main h3{color:#66b3ff; text-decoration:underline;}"\
"</style>"\
"<script>"\
"function $(id) { return document.getElementById(id); };"\
"function settingsCallback(o) {"\
"if ($('txtVer')) $('txtVer').value = o.ver;"\
"if ($('txtMac')) $('txtMac').value = o.mac;"\
"if ($('txtIp')) $('txtIp').value = o.ip;"\
"if ($('txtSub')) $('txtSub').value = o.sub;"\
"if ($('txtGw')) $('txtGw').value = o.gw;"\
"};"\
"</script>"\
"</head>"\
"<body>"\
"<div id='main'>"\
"<div style='background:snow; display:block;padding:10px 20px;'>"\
"<h3>�����������</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>�̼��汾��:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC��ַ:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP��ַ:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>��������:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>Ĭ������:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"<p><input type='submit' value='���沢����' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<div style='margin:5px 5px;'>"\
"&copy;Copyright 2014 by Ұ��"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"


/* �����HTTP�µ���Ƕ��ҳ���� */
char postQ[] = { "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta charset = \"utf-8\">"\
"<title>������ҳCJC</title>"\
"</head>"\
"<body>"\
"<h1>�ҵĵ�һ������</h1>"\
"<p>�ҵĵ�һ������</p>"\
"</body>"\
"</html>" };

#endif
// ���õľ�̬��ҳ��  --- ���ڼ򵥲��֣�ֻ������ǰ�˵���ʾ���ܣ�
/*
     ע��һ�¿�ͷ��д��  ----<meta>��ǩ��д��
	   ���ܹ���������  --- ������ʾ�Ļ�������  --- Ŀǰ��������д����
*/
#if 0
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>CJC��������ҳ��ʾ</title>"\
"<meta http - equiv = 'Content-Type' content = 'text/html; charset=GB2312'>"\
"<style type = 'text/css'>"\
"body{ text - align:left; background - color:#c0deed; font - family:Verdana; }"\
"#main{ margin - right:auto; margin - left:auto; margin - top:30px; }"\
"#main h1{ color:purple; text - decoration:underline; text - align:center; }"\
"#main p{ color:green; text - decoration:underline; }"\
"</style>"\
"</head>"\
"<body>"\
"<div id = 'main'>"\
"<div style = 'background:snow; display:block;padding:10px 20px;'>"\
"<h1>�ҵĵ�һ����������Ƕ��ҳ</h1>"\
"<p>1.���ʵ�IP 192.168.31.172</p>"\
"<p>2.���ʵ�����CJCW5500</p>"\
"</div>"\
"</div>"\
"</body>"\
"</html>"
#endif
#if 1
/*
     ������Ҫ���ڶ�̬ҳ��Ĳ���
	    JavaScript��ʹ��  
   --- �����еײ�����ã����Ƕ������ݵĽ��պͷ��͵Ĵ�������������
*/
// ���õ��Ƕ�̬��ҳ��  --- ���ڸ��Ӳ��֣��ɽ���ǰ��˵�����ͨ�ţ�
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>CJC����������W5500����ҳ����</title>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
"<style type='text/css'>"\
"body {text-align:left; background-color:#c0deed;font-family:Verdana;}"\
"#main {margin-right:auto;margin-left:auto;margin-top:30px;}"\
"label{display:inline-block;width:150px;}"\
"#main h3{color:#66b3ff; text-decoration:underline;}"\
"</style>"\
"<script>"\
"function $(id) { return document.getElementById(id); };"\
"function settingsCallback(o) {"\
"if ($('txtVer')) $('txtVer').value = o.ver;"\
"if ($('txtMac')) $('txtMac').value = o.mac;"\
"if ($('txtIp')) $('txtIp').value = o.ip;"\
"if ($('txtSub')) $('txtSub').value = o.sub;"\
"if ($('txtGw')) $('txtGw').value = o.gw;"\
"};"\
"</script>"\
"</head>"\
"<body>"\
"<div id='main'>"\
"<div style='background:snow; display:block;padding:10px 20px;'>"\
"<h3>�����������</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>�̼��汾��:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC��ַ:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP��ַ:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>��������:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>Ĭ������:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"<p><input type='submit' value='���沢����' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"
#endif


#endif 


