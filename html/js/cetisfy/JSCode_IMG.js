<!--         //   Edit by zms
//����ͻ�����֤��<JS�ű�>
//��������10������ͼƬ��0��9��

var CheckCode; //��Ҫ��֤����

function makeCode(objID,N)
{
 //objIDΪ��֤����id
 //NΪ��֤��ĳ���
 var picURL;   //����ͼƬ�����·��
 var radCode;
 var CodeHTML;
 var picURL_big;   //����ͼƬ�����·��
 var i;
 CheckCode = "";
 picURL = "/images/appceti/";
 CodeHTML = "&nbsp;&nbsp;";
 picURL_big = "/images/appceti/big/";
 // 26��Ӣ����ĸ��չ
 var let = new Array('a','b','c','d','e','f','g','h','i','g','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z');
 
 var rit = new Array('A','B','C','D','E','F','G','H','I','G','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z');
 CodeHTML ='<img src="' + picURL + 'start.png"/>';
 
 for(i = 0;i < N;i++)
 {
	 	//ż����ʾ����
	 	if (i%2 == 0)       //��Сд
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
		  //����0~9  ����10~19     10=0
		  CodeHTML = CodeHTML + '<img src="' + picURL + radCode + '.png"/>';
		  if (radCode >= 10)
		  { 
		  	radCode = radCode%10;
		  }
		  CheckCode = CheckCode + radCode;
		}
 }
 CodeHTML = CodeHTML + '<img src="' + picURL +  'end.png"/>';
 CheckCode = CheckCode.toLowerCase();     //ȫ��Сд
 //��ͼƬ������֤��
 document.getElementById(objID).innerHTML = CodeHTML;
}
//-->

