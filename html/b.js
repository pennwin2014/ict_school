function a(murl,mtitle,tsid){
    document.getElementById("iframe1").src = murl+"/pronline/Msg?FunName@ncmWebReplace&plate@zhengzhoudianxin/shop_menus2.htm&tsid@"+tsid;
    document.getElementById("logo").innerHTML = mtitle;
}

function b (rurl)
{  
//	alert(document.getElementById("hp").value);
//	alert(frames.document.getElementById("form1").hp.value);
//	alert(document.getElementById("iframe1").document.getElementById("form1").hp.value);
//	alert(document.getElementById("iframe1").hp.value);
//	alert(document.getElementById("iframe1").document.getElementById("hp").value);
//	document.getElementById("iframe1").hp.value = document.getElementById("hp").value;
//     alert(rurl);
	if(parent.window != window)
	{  
             var stateObject = {};
             // alert("none"+rurl);
             history.pushState(stateObject,"tttt","?pro@d");
	           document.getElementById("sn").style.display = "none";
             document.getElementById("iframe1").src = rurl;

	}
	else {
              var stateObject = {};
              history.pushState(stateObject,"tttt","?pro@d");
              parent.document.getElementById("iframe1").src = rurl;
        }
}

function showTitle1(title)
{
//  document.getElementById('logo').innerHTML = '<marquee  width="70%" scrollamount=4> <a href=/shop.htm><FONT color=#ff0000 ><STRONG>双12优惠大放送，苏宁电器全场买1000送500！</FONT></a></marquee>';
  document.getElementById('logo').innerHTML = '<marquee  width="70%" scrollamount=4><span ">'+title+'</span></marquee>';
 setTimeout("showTitle2('"+title+"')",15000);
}
function showTitle2(title)
{
	
  document.getElementById('logo').innerHTML = title;
   setTimeout("showTitle1('"+title+"')",8000);
}
//setTimeout(showTitle1,5000);
function d (rurl,ip,port,args,astatus,scode)
{  
//	alert("astatus = "+astatus+"  scode = "+scode);
	  if (scode == "")
	   scode = 99;
    if (scode == 0)
    {//alert(2);
    	if(port != '')
	      	document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/logout_ok.htm&"+rurl;
	    else
	      	document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/logout_ok.htm&"+rurl;
    	document.getElementById("logo").innerHTML = "开始上网"; 
    	document.getElementById("i").src = "/e.png"; 
    }
    else
    {
	    if (astatus == 1)
	    {
	    	if(port != '')
	      	document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
	    	else
	      	document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
	   
	    }
	    else if (astatus == 2)
	    {
		    if(port != '')
		      document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main_auto.htm&"+rurl;
		    else
		      document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main_auto.htm&"+rurl;
      }else{}
      document.getElementById("logo").innerHTML = "我要上网"; 
    }
  
}
//function k('[#posturl#]')
//{
//	document.getElementById("iframe1").src = "http://192.168.21.192:9080/pronline/Msg?FunName@ncSrvPlate&html@"+"/alogin_main.htm&"+'[#getarg#]';
//}

function e(ip,port,getarg,tsid,statusCode)
{//  alert(tsid);
//alert(221);alert(document.getElementById("rg").value);alert(223);
    // alert(parent.document.getElementById("iframe1").src);
    // alert("ppp"+parent.document.getElementById("sn").height+"tttt"+document.getElementById("sn").height);
//    alert("   getarg = "+getarg);
	//alert('statusCode = '+statusCode);
	
	if(parent.window != window)
	{  
//	   	document.getElementById("sn").style.display = "none";
	   	if(port != '')
	      	self.location.href = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+getarg+"&statusCode@"+statusCode;
			
	    	else
	      	self.location.href = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+getarg+"&statusCode@"+statusCode;
//      parent.document.getElementById("iframe1").src = rurl;
//alert(self.location.href);
	}
	else 
	{		
      document.getElementById("iframe1").src = "/pronline/Msg?FunName@ncmWebReplace&plate@zhengzhoudianxin/shop_menus2.htm&tsid@"+tsid;
  }

}
function f(tsid){
    document.getElementById("iframe1").src = "/pronline/Msg?FunName@ncmWebReplace&plate@zhengzhoudianxin/shop_menus2.htm&tsid@"+tsid;
    document.getElementById("logo").innerHTML = "丹尼斯欢迎您";
}

function h (rurl,ip,port,args,astatus,scode,up)
{ // alert(1);
//	alert("astatus = "+astatus+"  scode = "+scode+"  port= "+port);
//alert("up = "+up);
	  if (scode == "")
	   scode = 99;
    if (scode == 0)
    {
    	if(port != '')
	      	document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/logout_ok.htm&"+rurl;
	    else
	      	document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/logout_ok.htm&"+rurl;
    	
    	document.getElementById("logo").innerHTML = "开始上网"; 
    	document.getElementById("i").src = "/e.png"; 
    }
    else
    {
	    if (astatus == 1)
	    {
	    	if(port != '')
	    	
	    	
	      	document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
	    	else
	      	document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
	    }
	    else if (astatus == 2)
	    {
		    if(port != '')
		      document.getElementById("iframe1").src = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main_auto.htm&"+rurl+"&up@"+up;
		    else
		      document.getElementById("iframe1").src = "http://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main_auto.htm&"+rurl+"&up@"+up;
      }else{}
      document.getElementById("logo").innerHTML = "我要上网"; 
    }
    
}
//function z (rurl,httphost,astatus,scode)
function z (httphost,astatus,scode)
{  
//	alert("homeplate astatus = "+astatus+"  scode = "+scode +" rurl = "+ rurl + "httphost = "+httphost);
//	  alert(scode);
//	  if (scode == "")
//	   scode = 99;
//	   alert(scode);
	   
    if (scode == 0)
    {
	    document.getElementById("iframe1").src = httphost+"/pronline/Msg?FunName@ncSrvLogOut&html@"+"/logout_ok.htm";
//	    alert(document.getElementById("iframe1").src);
	    document.getElementById("logo").innerHTML = "开始上网"; 
    	document.getElementById("i").src = httphost+"/e.png"; 
    }
    else
    {var rurl;
	    if (astatus == 1)
	    {
	        document.getElementById("iframe1").src = httphost+"/pronline/Msg?FunName@ncSrvPlate&html@"+"/alogin_main.htm&"+rurl;
	    }
	    else if (astatus == 2)
	    {
		      document.getElementById("iframe1").src = httphost+"/pronline/Msg?FunName@ncSrvPlate&html@"+"/alogin_main_auto.htm&"+rurl;
      }else{}
      document.getElementById("logo").innerHTML = "我要上网"; 
    }
    
}
function j()
{
   	
}
$(document).ready(function() {
	//只有第一次加载，才提示“点击此处连接上网”
	var loc = window.location.href;
	if(loc){
		if(loc.indexOf("from=runproxy") > 0){
			$("#cover").show();
		}
	}
	//五秒后自动隐藏
	setTimeout(function(){
		$("#cover").hide();
	}, 5000);
	
	//点击隐藏
	$("#cover").click(function(){
		$("#cover").hide();
	})
});

var accessToken = window.location.hash.substring(1);
//alert(accessToken);	
if(accessToken != '')
   callServer();
function createXMLHttpRequest() {
	var C=null;
		if(window.XMLHttpRequest) { 
			C = new XMLHttpRequest();
		}
		else if (window.ActiveXObject) { 
			try {
				C = new ActiveXObject("Msxml2.XMLHTTP");
			} 
			catch (e) {
				try {
					C = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {}
			}
		}
		return C;
}
	
function getxmlbyname(ins,names,namee){
	var start;
	var end;
	var ret='';
	start=ins.indexOf(names);
	end=ins.indexOf(namee);
	if(start!=-1&&end!=-1){
		ret=ins.substr(start+names.length,end-start-names.length);
	}
	return ret;	
}
var statcode= null;
var errmessage= null;  	
var xmlHttp = false;
function callServer(){ 
   if( xmlHttp && xmlHttp .readyState != 0 ){
    	xmlHttp.abort();
   }
   xmlHttp = createXMLHttpRequest();
   if(xmlHttp){
//   	 var web_url = "http://"+ip+":"+port+"/pronline/Msg?FunName@ncmcomqqGetOpenid"+accessToken+"&xx@"+Math.random()*999; 
//   	 var web_url = "http://192.168.20.168:9080/pronline/Msg?FunName@ncmcomqqGetOpenid&"+accessToken+"&xx@"+Math.random()*999; 
   	 var web_url = "/pronline/Msg?FunName@ncmcomqqGetOpenid&"+accessToken+"&xx@"+Math.random()*999; 
//     var web_url="https://graph.qq.com/oauth2.0/me?"+accessToken+"&xx@"+Math.random()*999; 
//     alert(web_url);
     xmlHttp.open("GET",web_url, true);
//     xmlHttp.onreadystatechange = updatePage;
     xmlHttp.send(null);
   }
}


function updatePage(){
//alert(xmlHttp.readyState);
   if (xmlHttp.readyState == 4){
		  var response = xmlHttp.responseText;
		//  alert(response);
      var temp_mesg = getxmlbyname(response,"<openid>","</openid>");
    	var openid = temp_mesg;
    //	alert(openid);
//        var htmltest = errmessage;
//	    if(statcode == 5){
//			  if (errmessage == 1)           
//			  {
//				document.getElementById('gpss').src='/images/getpasswdpczh_2x.png';    
//				document.getElementById("numberErr").style.display="block";           
//			    document.getElementById("gpss").disabled="";
//			  }
//      }
//      if(statcode == 0){             
//      	        
//      } 
   }
 }
var backURL = new Array();
var backnumber = 0;
backURL[backnumber] = "/pronline/Msg?FunName@ncm_yuyuan_home&plateName@zhengzhoudianxin";
function back(tsid){
//alert(tsid);
//alert(backURL[backnumber]);
	if(backnumber !=0){
		backnumber--;//alert(backURL[backnumber]);
		document.getElementById('iframe1').src=backURL[backnumber]+"&tsid@"+tsid;
	}
}

function addURL(url){
	backnumber++;
	backURL[backnumber] = url;
//alert(backnumber)
}

