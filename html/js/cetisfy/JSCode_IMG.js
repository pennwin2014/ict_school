<!--         //   Edit by zms
//纯粹客户端验证码<JS脚本>
//首先制作10个数字图片（0～9）

var CheckCode; //需要验证的码

function makeCode(objID,N)
{
 //objID为验证区的id
 //N为验证码的长度
 var picURL;   //数字图片的相对路径
 var radCode;
 var CodeHTML;
 var picURL_big;   //数字图片的相对路径
 var i;
 CheckCode = "";
 picURL = "/images/appceti/";
 CodeHTML = "&nbsp;&nbsp;";
 picURL_big = "/images/appceti/big/";
 // 26个英文字母扩展
 var let = new Array('a','b','c','d','e','f','g','h','i','g','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z');
 
 var rit = new Array('A','B','C','D','E','F','G','H','I','G','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z');
 CodeHTML ='<img src="' + picURL + 'start.png"/>';
 
 for(i = 0;i < N;i++)
 {
	 	//偶数显示数字
	 	if (i%2 == 0)       //大小写
	 	{
	 		radCode = Math.round(Math.random()*25) + 0;
		  CheckCode = CheckCode + let[radCode];
		  if(radCode>10)
		  {
		  	CodeHTML = CodeHTML + '<img src="' + picURL + let[radCode] + '.png"/>';
	 		}
	 		else
	 		{
	 			CodeHTML = CodeHTML + '<img src="' + picURL_big + rit[radCode] + '.png"/>';
	 		}
	 	}
	 	else
	 	{
		  radCode = Math.floor(Math.random()*10);
		  //左歪0~9  右歪10~19     10=0
		  CodeHTML = CodeHTML + '<img src="' + picURL + radCode + '.png"/>';
		  if (radCode >= 10)
		  { 
		  	radCode = radCode%10;
		  }
		  CheckCode = CheckCode + radCode;
		}
 }
 CodeHTML = CodeHTML + '<img src="' + picURL +  'end.png"/>';
 CheckCode = CheckCode.toLowerCase();     //全部小写
 //将图片插入验证区
 document.getElementById(objID).innerHTML = CodeHTML;
}
//-->

