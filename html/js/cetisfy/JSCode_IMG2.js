<!--         //   Edit by zms
//纯粹客户端验证码<JS脚本>
var CheckCode; //需要验证的码

function makeCode(objID,N)
{
 //objID为验证区的id
 //N为验证码的长度
 var picURL;   //数字图片的相对路径
 var radCode;
 var CodeHTML;
 var i;
 CheckCode = "";
 picURL = "/images/appceti/";
 CodeHTML = "&nbsp;&nbsp;";

 for(i=0;i<N;i++)
 {
  radCode=Math.floor(Math.random()*10);
  if(radCode==9) radCode=6;
  CheckCode = CheckCode + radCode;
  CodeHTML = CodeHTML + '<img src="' + picURL + radCode +'.png"/>';
 }
 
 //将图片插入验证区
 document.getElementById(objID).innerHTML=CodeHTML;
}
//-->

