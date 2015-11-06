var mapPage = {
	'id_frontPage_main':['itm_frontPage_main','��ҳ','frontPage/ict_frontPage_main.html'],
	'id_ictShop_main':['itm_ictShop_main','ICT�̳�','system/ict_system_register.html'],
	'id_support_help':['itm_support_help','�ͷ������','support/ict_support_help.html'],
	'id_ictShop_products':['itm_ictShop_products','�����ײ�','ictShop/ict_ictShop_products.html'],
	'id_ictShop_charge':['itm_ictShop_charge','��ֵ','ictShop/ict_ictShop_charge.html'],
	'id_bill_check':['itm_bill_check','�˵���ѯ','bill/ict_bill_check.html'],
	'id_pay_record':['itm_pay_record','��ֵ��¼','pay/ict_pay_record.html'],
	'id_system_passwd':['itm_system_passwd','�޸�����','system/ict_system_update_password.html']	
};

function f_IctGlobalContext(){
//˽�б���
	this.lastErrorMsg = "";
	this.userInfo = {
		'userName':'С��',
		'vName':'124',
		'bType':'1',
		'bName':'100Ԫ�����ײ�',
		'money':'23',
		'loginStatus':true
	};
//�����ӿ�
	this.setLastError = function(msg){
		this.lastErrorMsg = msg;
	}
	this.getLastError = function(){
		return this.lastErrorMsg;
	}
	this.jumpToPage = function(pid){
		var title = mapPage[pid][1];
		var url = '/school/'+mapPage[pid][2];
		var pnlCenter = Ext.getCmp("center_panel");
		pnlCenter.setTitle("����ǰ��λ��: "+title);
		pnlCenter.body.update('<iframe src="'+url+'" frameborder="no" style="width:100%;height:1000px;"></iframe>');		
	}
	this.cutLan = function(){
		alert("�������");
	}
	this.userQuit = function(){
		this.clearUserInfo();
		self.location='/school/login.html';
	}
	this.setUserInfo = function(info){
		this.userInfo = info;
	}
	this.getUserInfo = function(){
		return this.userInfo;
	}
	this.clearUserInfo = function(){
		this.userInfo['loginStatus'] = false;
		this.userInfo['userName'] = "";
		this.userInfo['vName'] = "";
		this.userInfo['bType'] = "";
		this.userInfo['bName'] = "";
		this.userInfo['money'] = "";
	}
	this.doOrderPackage = function(funId){
		alert("��������ײ�"+funId);
	}
//���߷���
/**
* ���ã��ж�Զ���ļ��Ƿ����
* �������ļ��ĵ�ַ
* ����ֵ�� true--���ڣ�false--������
*/
	this.isRemoteFileExist = function(fileUrl){
		Ext.Ajax.request({
			url:""+fileUrl,
			async: false,
			type:'HEAD',
			error: function() {
			   return false;
			},
			success: function() {
			  return true;
			}
		});
	}
//У��ķ���
	this.isNull = function(value){
		if (!value || typeof(value)=="undefined" || value==0){
			return true;
		}else{
			return false;
		}
	}
	this.CheckMail = function (mail){
		var filter  = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
		if (filter.test(mail)) 
			return true;
		else {
			this.setLastError('���ĵ����ʼ���ʽ����ȷ');
			return false;
		}
	}
	this.CheckPhone = function(Phone) {
		var filter=/^1\d{10}$|^(0\d{2,3}-?|\(0\d{2,3}\))?[1-9]\d{4,7}(-\d{1,8})?$/;
		if (filter.test(Phone)){
			return true;
		}else {
			this.setLastError('���ĵ绰��ʽ����ȷ');
			return false;
		}
	}
	this.checkLoginData = function(vname, passwd){
		if(this.CheckPhone(vname)==false){
			return false;
		}
		if(passwd == ""){
			this.setLastError("���벻��Ϊ��");
			return false;
		}
		return true;
	}
}
var ictGlobalCtx = new f_IctGlobalContext(); 
















