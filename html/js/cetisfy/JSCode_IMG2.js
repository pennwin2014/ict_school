<!--         //   Edit by zms
//����ͻ�����֤��<JS�ű�>
var CheckCode; //��Ҫ��֤����

function makeCode(objID,N)
{
 //objIDΪ��֤����id
 //NΪ��֤��ĳ���
 var picURL;   //����ͼƬ�����·��
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
 
 //��ͼƬ������֤��
 document.getElementById(objID).innerHTML=CodeHTML;
}
//-->

