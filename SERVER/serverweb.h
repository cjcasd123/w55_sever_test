#ifndef __SERVERWEB_H
#define __SERVERWEB_H
#include "stm32f4xx.h"  
#include "types.h"


#if 0
#define INDEX_HTML "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta charset = \"utf-8\">"\
"<title>测试网页CJC</title>"\
"</head>"\
"<body>"\
"<h1>我的第一个标题</h1>"\
"<p>我的第一个段落</p>"\
"</body>"\
"</html>" 

#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>野火网络适配器W5500的网页配置</title>"\
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
"<h3>配置网络参数</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>固件版本号:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC地址:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP地址:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>子网掩码:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>默认网关:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"<p><input type='submit' value='保存并重启' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<div style='margin:5px 5px;'>"\
"&copy;Copyright 2014 by 野火"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"


/* 这个是HTTP下的内嵌网页设置 */
char postQ[] = { "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta charset = \"utf-8\">"\
"<title>测试网页CJC</title>"\
"</head>"\
"<body>"\
"<h1>我的第一个标题</h1>"\
"<p>我的第一个段落</p>"\
"</body>"\
"</html>" };

#endif
// 配置的静态的页面  --- 属于简单部分（只是用于前端的显示功能）
/*
     注意一下开头的写法  ----<meta>标签的写法
	   （能够发送数据  --- 但是显示的会是乱码  --- 目前就是这种写法）
*/
#if 0
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>CJC基础的网页显示</title>"\
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
"<h1>我的第一个基础的内嵌网页</h1>"\
"<p>1.访问的IP 192.168.31.172</p>"\
"<p>2.访问的域名CJCW5500</p>"\
"</div>"\
"</div>"\
"</body>"\
"</html>"
#endif
#if 1
/*
     这里主要对于动态页面的测试
	    JavaScript的使用  
   --- 并进行底层的配置（就是对于数据的接收和发送的处理）！！！！！
*/
// 配置的是动态的页面  --- 属于复杂部分（可建立前后端的数据通信）
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>CJC网络适配器W5500的网页配置</title>"\
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
"<h3>配置网络参数</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>固件版本号:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC地址:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP地址:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>子网掩码:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>默认网关:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"<p><input type='submit' value='保存并重启' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"
#endif


#endif 


