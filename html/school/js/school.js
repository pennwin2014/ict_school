var mapPage = {
	'id_frontPage_main':['itm_frontPage_main','首页','frontPage/ict_frontPage_main.html'],
	'id_ictShop_main':['itm_ictShop_main','ICT商城','system/ict_system_register.html'],
	//'id_support_help':['itm_support_help','客服与帮助','support/ict_support_help.html'],
	'id_support_help':['itm_support_help','客服与帮助','system/ict_system_login.html'],
	'id_ictShop_products':['itm_ictShop_products','更换套餐','ictShop/ict_ictShop_products.html'],
	'id_ictShop_charge':['itm_ictShop_charge','充值','ictShop/ict_ictShop_charge.html'],
	'id_bill_check':['itm_bill_check','账单查询','bill/ict_bill_check.html'],
	'id_pay_record':['itm_pay_record','充值记录','pay/ict_pay_record.html'],
	'id_system_passwd':['itm_system_passwd','修改密码','system/ict_system_update_password.html']	
};

function f_IctGlobalContext(){
//私有变量
	this.userInfo = {
		'userName':'小白',
		'vName':'124',
		'bType':'1',
		'bName':'100元基础套餐',
		'money':'23'
	};
//公共接口
	this.jumpToPage = function(pid){
		var title = mapPage[pid][1];
		var url = '/school/'+mapPage[pid][2];
		var pnlCenter = Ext.getCmp("center_panel");
		pnlCenter.setTitle("您当前的位置: "+title);
		pnlCenter.body.update('<iframe src="'+url+'" frameborder="no" style="width:100%;height:1000px;"></iframe>');		
	}
	this.cutLan = function(){
		alert("点击断线");
	}
	this.userQuit = function(){
		alert("点击安全退出");
	}
	this.setUserInfo = function(info){
		this.userInfo = info;
	}
	this.getUserInfo = function(){
		return this.userInfo;
	}
	this.doOrderPackage = function(funId){
		alert("点击订购套餐"+funId);
	}
//工具方法
/**
* 作用：判断远程文件是否存在
* 参数：文件的地址
* 返回值： true--存在，false--不存在
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
//校验的方法
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
			alert('您的电子邮件格式不正确');
			return false;
		}
	}
	this.CheckPhone = function(Phone) {
		var filter=/^1\d{10}$|^(0\d{2,3}-?|\(0\d{2,3}\))?[1-9]\d{4,7}(-\d{1,8})?$/;
		if (filter.test(Phone)){
			return true;
		}else {
			alert('您的电话格式不正确');
			return false;
		}
	}
}
var ictGlobalCtx = new f_IctGlobalContext(); 
















