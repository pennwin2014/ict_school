<script type="text/javascript" src="/newver/weixin/wechatutil.js" ></script>

var appId = 'wx086f838720eb4d73';
var extend = 'pronetway';
var timestamp=''+new Date().getTime();
var shop_id = '7789679';
var secretkey = 'c1881db3420a6525eebae6a3a18b2d5c';
var authUrl = 'http://'+proauthip+':'+proauthport+'/proauth/Msg?FunName@ncLoginAuth_weixin';
var mac = '';//用户手机mac地址，格式冒号分隔，字符长度17个，并且字母小写，例如：00:1f:7a:ad:5c:a8 
var ssid = 'WXPronetway_VIP1';//AP设备的信号名称 
var bssid = '';//无线网络设备的无线mac地址，格式冒号分隔，字符长度17个，并且字母小写，例如：00:1f:7a:ad:5c:a8 
var sign = MD5(appId + extend + timestamp + shop_id + authUrl + mac + ssid + bssid + secretkey);


Wechat_GotoRedirect(
	appId,      
	extend,     
	timestamp, 
	sign,       
	shop_id,   
	authUrl,   
	mac,      
	ssid,      
	bssid); 

Wechat_GotoRedirect(
appId,  
'test',            
timestamp,          
sign,   
'6747662',  
'http://wifi.weixin.qq.com/assistant/wifigw/auth.xhtml?httpCode=200',       
'aa:aa:aa:aa:aa:aa',     
'2099',    
'ff:ff:ff:ff:ff:ff');

SSID:WXPronetway_VIP1
shopId:7789679
appId:wx086f838720eb4d73
secretKey:c1881db3420a6525eebae6a3a18b2d5c

