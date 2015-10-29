function a(){
    document.getElementById("iframe1").src = "/suning/shop_menus.htm";
    document.getElementById("logo").innerHTML = "苏宁电器";
}

function b (rurl)
{  
//     callServer();
    //  alert(rurl);
    // alert(parent.document.getElementById("iframe1").src);
    //alert("ppp"+parent.document.getElementById("sn").height+"tttt"+document.getElementById("sn").height);
	if(parent.window != window)
	{  
	   	document.getElementById("sn").style.display = "none";
        parent.document.getElementById("iframe1").src = rurl;
	}
	else {
        document.getElementById("iframe1").src = "/suning/shop_menus.htm";
    }
}

function c(rurl,ip,port){//alert(rurl);
//    document.getElementById("iframe1").src = "/pronline/Msg?FunName@ncSrvPlate&html@"+"/alogin_main.htm&[#getarg#]";
//    document.getElementById("iframe1").src = "https://192.168.21.192/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
    if(port != '')
      document.getElementById("iframe1").src = "https://"+ip+":"+port+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
    else
      document.getElementById("iframe1").src = "https://"+ip+"/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&"+rurl;
//alert(document.getElementById("iframe1").src);
    document.getElementById("logo").innerHTML = "我要上网";
}

function createXMLHttpRequest() 
{
	  var C=null;
		if(window.XMLHttpRequest) { 
			C = new XMLHttpRequest();
		}
		else if (window.ActiveXObject) { 
			try {
				C = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
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
	
	if(start != -1 && end != -1)
	{
		ret=ins.substr(start+names.length,end-start-names.length);
	}
	return ret;	
}
var myurl= null;

var xmlHttp = false;

 function callServer(){ 
   if( xmlHttp && xmlHttp .readyState != 0 ){
    	xmlHttp.abort();
   }
   xmlHttp = createXMLHttpRequest();
   if(xmlHttp){
     var web_url="https://192.168.21.192/pronline/Msg?FunName@ncSrvPlate&lang@"+""+"&code@"+""+"&usernametmp@"+""+"&html@"+"/alogin_main.htm&";
     xmlHttp.open("GET",web_url, true);
//	   if(xmlHttp.readyState == 1)
//	 	 {
//	     xmlHttp.setRequestHeader("requestHeaders", {'Referer':'http://127.0.0.1/'});
//	   }
     xmlHttp.onreadystatechange = updatePage;
     xmlHttp.send(null);
   }
 }
 function updatePage(){
// 	xmlHttp.setRequestHeader("requestHeaders", {'Referer':'http://127.0.0.1/'});
//         alert(" Update Page  Status="+xmlHttp.readyState);
// var stateObject = {};  
// var title = "Wow Title";  
// var newUrl = "/my/awesome/url"; 
// history.pushState(stateObject,title,newUrl); 
 	 
   if (xmlHttp.readyState == 4)
   {
      var response = xmlHttp.responseText;
    	var headurl = getxmlbyname(response,"<headurl>","</headurl>");
    	myurl=headurl;
//   		alert(response);
    //  document.getElementById('passmesg').innerHTML = "";
			
   }
 }  


function showTitle1(){
document.getElementById('logo').innerHTML = '<marquee  width="80%" scrollamount=4> <a href=/shop.htm><FONT color=#ff0000 ><STRONG>双12优惠大放送，苏宁电器全场买1000送500！</FONT></a></marquee>';
setTimeout(showTitle2,15000);
}
function showTitle2(){
document.getElementById('logo').innerHTML = '苏宁电器';
setTimeout(showTitle1,7000);
}
setTimeout(showTitle1,8000);


