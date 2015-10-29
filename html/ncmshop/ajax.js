//创建XMLHttpRequest对象       
function createXMLHttpRequest() {
	  var C=null;
		if(window.XMLHttpRequest) { //Mozilla 浏览器
			C = new XMLHttpRequest();
		}
		else if (window.ActiveXObject) { //IE浏览器
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

//解析字符串  ins-输入字符串 names-开始字符串 namee -结束字符串 返回值 （names 和namee 之间的字符串 可用于解析简单的XML
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

var xmlHttp = false;

function callServer(){ 
   if( xmlHttp && xmlHttp .readyState != 0 ){
    	xmlHttp.abort();
   }
   xmlHttp = createXMLHttpRequest();
   if(xmlHttp){
     var web_url="/pronline/Msg?FunName@ncSrvGetPass&username@"+document.getElementById("nclogin").username.value+"&xx@"+Math.random()*999;
   
    xmlHttp.open("GET",web_url, true);
    xmlHttp.onreadystatechange = updatePage;
    xmlHttp.send(null);
  }
}


function updatePage(){
   if (xmlHttp.readyState == 4){
   	
      var response = xmlHttp.responseText;
   
//		  var temp_num = getxmlbyname(response,"<statusId>","</statusCode>");
//    	statcode=parseInt(temp_num);
//      var temp_mesg = getxmlbyname(response,"<message>","</message>");
//    	errmessage = temp_mesg;
    	
    	var temp_num = getxmlbyname(response,"<statusCode>","</statusCode>");
    	statcode=parseInt(temp_num);

      var temp_mesg = getxmlbyname(response,"<statusId>","</statusId>");
    	errmessage = temp_mesg;
//    		alert(response);
    	if(statcode == 5){
			  if (errmessage == 1)            //错误不灰
			  {
			  	document.getElementById('username_err').className = 'error1';
					document.getElementById("username_err").innerHTML = "<span id='errormark'><img src = '/images/ap/mark.png' style = 'margin-bottom:-2px;' ></span>&nbsp;请输入正确的手机号码。";  
			    document.getElementById('gpss').className = 'getpasscode';
			    document.getElementById("gpss").disabled="";
			    document.getElementById('passmesg').innerHTML = "";
			  }
      }
      if(statcode == 0)                //正确灰掉
      {                 
//      	   document.getElementById('gpss').className = 'getpasscode_no';
//      	   document.getElementById("gpss").disabled="true";
      	   document.getElementById('username_err').className = 'error';
//    
//      if(p_flag=='0') return false;
//      	   setTimeout('restore()',60000);
     	     
      }
   }
}















var xmlHttp = false;

 function callServer(){ 
   if( xmlHttp && xmlHttp .readyState != 0 ){
    	xmlHttp.abort();
   }
   xmlHttp = createXMLHttpRequest();
   if(xmlHttp){
     var web_url="http://[#ip#]:[#port#]/pronline/Msg?FunName@ncSrvGetPass&lang@[#lang#]&username@"+document.getElementById("nclogin").username.value+"&xx@"+Math.random()*999;
   
    xmlHttp.open("GET",web_url, true);
    xmlHttp.onreadystatechange = updatePage;
    xmlHttp.send(null);
  }
 }


 function updatePage(){
   if (xmlHttp.readyState == 4){
   	
      var response = xmlHttp.responseText;
   
//		  var temp_num = getxmlbyname(response,"<statusId>","</statusCode>");
//    	statcode=parseInt(temp_num);
//      var temp_mesg = getxmlbyname(response,"<message>","</message>");
//    	errmessage = temp_mesg;
    	
    	var temp_num = getxmlbyname(response,"<statusCode>","</statusCode>");
    	statcode=parseInt(temp_num);

      var temp_mesg = getxmlbyname(response,"<statusId>","</statusId>");
    	errmessage = temp_mesg;
//    		alert(response);
    	if(statcode == 5){
			  if (errmessage == 1)            //错误不灰
			  {
			  	document.getElementById('username_err').className = 'error1';
					document.getElementById("username_err").innerHTML = "<span id='errormark'><img src = '/images/ap/mark.png' style = 'margin-bottom:-2px;' ></span>&nbsp;请输入正确的手机号码。";  
			    document.getElementById('gpss').className = 'getpasscode';
			    document.getElementById("gpss").disabled="";
			    document.getElementById('passmesg').innerHTML = "";
			  }
      }
      if(statcode == 0)                //正确灰掉
      {                 
//      	   document.getElementById('gpss').className = 'getpasscode_no';
//      	   document.getElementById("gpss").disabled="true";
      	   document.getElementById('username_err').className = 'error';
//    
//      if(p_flag=='0') return false;
//      	   setTimeout('restore()',60000);
     	     
      }
   }
 }	
