function a(murl,mtitle){
    document.getElementById("iframe1").src = murl+"/suning/shop_menus.htm";
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

function showTitle1()
{
//  document.getElementById('logo').innerHTML = '<marquee  width="70%" scrollamount=4> <a href=/shop.htm><FONT color=#ff0000 ><STRONG>双12优惠大放送，苏宁电器全场买1000送500！</FONT></a></marquee>';
  document.getElementById('logo').innerHTML = '<marquee  width="70%" scrollamount=4><span style="color:red;font-family:Arial;font-weight:Bold; text-shadow: 1px 1px 1px #000000">双12优惠大放送，商城全场买1000送500！</span></marquee>';
  setTimeout(showTitle2,15000);
}
function showTitle2()
{
  document.getElementById('logo').innerHTML = '商城';
  setTimeout(showTitle1,8000);
}
setTimeout(showTitle1,5000);
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
    	document.getElementById("i").src = "/c.png"; 
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
function e(rurl,posturl)
{  
//     callServer();
     alert(rurl);
	 alert(posturl);
    // alert(parent.document.getElementById("iframe1").src);
    // alert("ppp"+parent.document.getElementById("sn").height+"tttt"+document.getElementById("sn").height);
//    alert("posturl = "+posturl+"   postarg = "+postarg);
	if(parent.window != window)
	{  
	   	document.getElementById("sn").style.display = "none";
      parent.document.getElementById("iframe1").src = rurl;
	}
	else 
	{
      document.getElementById("iframe1").src = "/suning/shop_menus.htm";
  }
  alert(document.getElementById("iframe1").src);
}
function f(){
    document.getElementById("iframe1").src = "/suning/shop_menus.htm";
    document.getElementById("logo").innerHTML = "商城";
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
    	document.getElementById("i").src = "/c.png"; 
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
    	document.getElementById("i").src = httphost+"/b.png"; 
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
