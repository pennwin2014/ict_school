
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
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;��������ȷ���ֻ����롣' 	
  		return;
  }
  
	if(document.getElementById("usernametmp").value!=""&&document.getElementById("usernametmp").value!='�ֻ�����'&&document.getElementById('acceptme').checked)
	{
		document.getElementById("nclogin").submit();
	}
	
	if (document.getElementById("usernametmp").value==""||document.getElementById("usernametmp").value=='�ֻ�����'){
		document.getElementById('username_err').className = 'error1';
		document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;��������ȷ���ֻ����롣';
		return ;
	}
	if(!document.getElementById('acceptme').checked){ 
		document.getElementById('chaccept_err').className = 'error3';
		document.getElementById('chaccept_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;��ȷ�Ͻ������ϸ��'
		return ;
	}
	
}


function getpasswd(){	
	document.getElementById('username_err').innerHTML = "";
	document.getElementById('chaccept_err').innerHTML = "";	
   
	  	document.getElementById('err5').className = 'error';
      document.getElementById("err5").innerHTML = "";

	  if(document.getElementById("nclogin").usernametmp.value==''||document.getElementById("usernametmp").value=='�ֻ�����'){
			document.getElementById('username_err').className = 'error1';
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;��������ȷ���ֻ����롣';
			return false;
	  }
	  
//		var telno=document.getElementById("nclogin").username.value;
		var telno=  ncPortalCheckMobileNumBer(document.getElementById("code").value,document.getElementById("nclogin").usernametmp.value);
		if(telno.length<6 || isNaN(telno)){
			document.getElementById('username_err').className = 'error1';
			document.getElementById('username_err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;�ֻ��Ÿ�ʽ����';
			return false;
		} 
		
		
		var temp_value;
		temp_value = document.getElementById('usernametmp').value;
		

		 if(document.getElementById('code').value!='' && document.getElementById('code').value != "���Ҵ���"){
		 	 		temp_value = '0'+document.getElementById('code').value+'-'+document.getElementById('usernametmp').value;
		 }
		 document.getElementById("nclogin").username.value = temp_value;
//		 alert(document.getElementById("nclogin").username.value);
//		 if(chk(telno)!='0'){
//		    alert('��������ȷ���ֻ���!');
//		  	return false;
//		  }
	  
	  document.getElementById('gpss').className = 'getpasscode_no';
	  document.getElementById('passmesg').innerHTML = "�����û���յ����룬���Ե�һ���ӣ�Ȼ������һ�Ρ�";
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
		  document.getElementById('err5').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;�����뷢���������ֻ��������ȷ���롣';
	  }	
	  if('[#statusCode#]' == '3')
	  {
	  	document.getElementById('err').className = 'error3';
		  document.getElementById('err').innerHTML = '<img src = "/images/ap/mark.png" style = "margin-bottom:-2px;" >&nbsp;�ܱ�Ǹ��ע��ʱ���ִ������Ժ����ԡ�';
	  }	
	  	if('[#usernametmp#]' != '' && '[#usernametmp#]' != '�ֻ�����')
	{
		document.getElementById("usernametmp").value = '[#usernametmp#]';
		document.getElementById('usernametmp').className = 'username2';
	}
	if('[#usernametmp#]' == '�ֻ�����')
	{
		document.getElementById("usernametmp").value = '�ֻ�����';
		document.getElementById('usernametmp').className = 'username';
	}
//	alert("[#code#]"+ " username =" +"[#usernametmp#]" );
	if('[#code#]' != '' && '[#code#]' != '���Ҵ���')
	{
		document.getElementById("code").value = '[#code#]';
		document.getElementById('code').className = 'code2';
	}
	if('[#code#]' == '���Ҵ���')
	{
		document.getElementById("code").value = '���Ҵ���';
		document.getElementById('code').className = 'code';
	}
	
}
	