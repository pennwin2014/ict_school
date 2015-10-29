
function spylogin(){
	document.getElementById('username_err').innerHTML = "";
	document.getElementById('chaccept_err').innerHTML = "";
	
	document.getElementById('err5').className = 'error';
  document.getElementById("err5").innerHTML = "";
	
	var aa =document.getElementById("code").value;	
  var bb =document.getElementById("usernametmp").value;	
  var cc =  ncPortalCheckMobileNumBer(aa,bb);
  document.getElementById("username").value = cc ;
  
  if (isNaN(cc))
  {
    	document.getElementById('username_err').className = 'error1';
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;请输入正确的手机号码。' 	
  		return;
  }
  
	if(document.getElementById("usernametmp").value!=""&&document.getElementById("usernametmp").value!='手机号码'&&document.getElementById('acceptme').checked)
	{
		document.getElementById("nclogin").submit();
	}
	
	if (document.getElementById("usernametmp").value==""||document.getElementById("usernametmp").value=='手机号码'){
		document.getElementById('username_err').className = 'error1';
		document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;请输入正确的手机号码。';
		return ;
	}
	if(!document.getElementById('acceptme').checked){ 
		document.getElementById('chaccept_err').className = 'error3';
		document.getElementById('chaccept_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;请确认接受条款及细则。'
		return ;
	}
	
}


function getpasswd(){	
	document.getElementById('username_err').innerHTML = "";
	document.getElementById('chaccept_err').innerHTML = "";	
   
	  	document.getElementById('err5').className = 'error';
      document.getElementById("err5").innerHTML = "";

	  if(document.getElementById("nclogin").usernametmp.value==''||document.getElementById("usernametmp").value=='手机号码'){
			document.getElementById('username_err').className = 'error1';
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;请输入正确的手机号码。';
			return false;
	  }
	  
//		var telno=document.getElementById("nclogin").username.value;
		var telno=  ncPortalCheckMobileNumBer(document.getElementById("code").value,document.getElementById("nclogin").usernametmp.value);
		if(telno.length<6 || isNaN(telno)){
			document.getElementById('username_err').className = 'error1';
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;手机号格式错误。';
			return false;
		} 
		
		
		var temp_value;
		temp_value = document.getElementById('usernametmp').value;
		

		 if(document.getElementById('code').value!='' && document.getElementById('code').value != "国家代码"){
		 	 		temp_value = '0'+document.getElementById('code').value+'-'+document.getElementById('usernametmp').value;
		 }
		 document.getElementById("nclogin").username.value = temp_value;
//		 alert(document.getElementById("nclogin").username.value);
//		 if(chk(telno)!='0'){
//		    alert('请输入正确的手机号!');
//		  	return false;
//		  }
	  
	  document.getElementById('gpss').className = 'getpasscode_no';
	  document.getElementById('passmesg').innerHTML = "如果你没有收到密码，请稍等一分钟，然后再试一次。";
    document.getElementById("gpss").disabled="true";
    setTimeout('restore()',60000);
	  
	  callServer();
	    
//	  temp_html="http://[#ip#]:[#port#]/pronline/Msg?FunName@ncSrvGetPass&lang@[#lang#]&username@"+document.getElementById("nclogin").username.value;
//		document.getElementById("ifhide").src=temp_html;

	  //document.getElementById("imgm").src='/images/authimage/donghua.gif';
//      document.getElementById("gpss").disabled="true";
     // document.getElementById("imgm").style.display="";
//      p_flag='0';
	 

      
//		  return false;
		
		
	}
	
	function init()
	{
		document.getElementById('sign').disabled = true;
	  if('[#statusCode#]' == '5')
	  {
	  	document.getElementById('err5').className = 'error3';
		  document.getElementById('err5').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;请输入发送至上述手机号码的正确密码。';
	  }	
	  if('[#statusCode#]' == '3')
	  {
	  	document.getElementById('err').className = 'error3';
		  document.getElementById('err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;很抱歉，注册时出现错误。请稍后再试。';
	  }	
	  	if('[#usernametmp#]' != '' && '[#usernametmp#]' != '手机号码')
	{
		document.getElementById("usernametmp").value = '[#usernametmp#]';
		document.getElementById('usernametmp').className = 'username2';
	}
	if('[#usernametmp#]' == '手机号码')
	{
		document.getElementById("usernametmp").value = '手机号码';
		document.getElementById('usernametmp').className = 'username';
	}
//	alert("[#code#]"+ " username =" +"[#usernametmp#]" );
	if('[#code#]' != '' && '[#code#]' != '国家代码')
	{
		document.getElementById("code").value = '[#code#]';
		document.getElementById('code').className = 'code2';
	}
	if('[#code#]' == '国家代码')
	{
		document.getElementById("code").value = '国家代码';
		document.getElementById('code').className = 'code';
	}
	
}
	