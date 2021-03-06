var mapPage = {
	'id_frontPage_main':['itm_frontPage_main','首页','frontPage/ict_frontPage_main.html'],
	'id_ictShop_main':['itm_ictShop_main','ICT商城','system/ict_system_register.html'],
	'id_support_help':['itm_support_help','客服与帮助','support/ict_support_help.html'],
	'id_ictShop_products':['itm_ictShop_products','更换套餐','ictShop/ict_ictShop_products.html'],
	'id_ictShop_charge':['itm_ictShop_charge','充值','ictShop/ict_ictShop_charge.html'],
	'id_bill_check':['itm_bill_check','账单查询','bill/ict_bill_check.html'],
	'id_pay_record':['itm_pay_record','充值记录','pay/ict_pay_record.html'],
	'id_system_passwd':['itm_system_passwd','修改密码','system/ict_system_update_password.html']	
};

function f_IctGlobalContext(){
//私有变量
	this.lastErrorMsg = "";
	this.userInfo = {
		'userName':'小白',
		'vName':'124',
		'bType':'1',
		'bName':'100元基础套餐',
		'money':'23',
		'loginStatus':true
	};
	
	
	this.openGrant = function(id, name, namedes, money){
		var ictMe = this;
		//套餐生效时间
		var timeval= "";
		
		var timeFlagStore = Ext.create('Ext.data.Store', {
	  		fields:['id','text'],
	  		data:[
					{id:'1',text:'立即生效'},
					{id:'2',text:'次日生效'}
				]
	  	});
		
		var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
			fieldLabel: '生效时间',
			id : 'timeFlag',
			labelWidth: 40,
			width: 130,
			valueField:'id',
			value:'1',
			displayField: 'text',
			allowBlank: true,
			store: timeFlagStore,
			triggerAction: 'all',
			queryMode: 'local',
			typeAhead: true,
			listeners: {
				'change':function(thisField,newValue,oldValue,eOpts){
						var sdateTmp = new Date();
						var nowDate = new Date();
						alert(newValue);			
						switch(newValue){
							case '1':
								timeval = newValue;
								break;
							case '2':
								timeval = newValue;
								break;
							default:
									break;		
						}
				}
			},
			listConfig:{
				loadingText: 'Searching...',
				emptyText: 'No matching found.'
			}
		});
		
		var mNamedes = namedes;
		var rightcheckGroup = {
			xtype: 'fieldset',
			title: '更换套餐详细',
			layout: 'form',
			defaults: {
				labelStyle: 'padding-left:4px;'
			},
			collapsible: false
		};
		
		var fp2 = Ext.create('Ext.FormPanel', {
			frame: true,
			fieldDefaults: {
				labelWidth: 150
			},
			width: 400,
			//height : 300,
			bodyPadding: 10,
			items: [
				{
					xtype: 'tbtext',
					text:'套餐名称:'+name+'<br>套餐内容:'+namedes+'<br>套餐费用:'+money+'元<br>'
				},
				timeFlagCombo
			],			
			buttons: [{
				text: '确认更换',
				defaults: {},
				handler: function () {

					//前面有处理更换套餐操作的处理，在后台，并且通过record来传递要修改的套餐信息，并且将record的信息传到后台，进行处理
					//还涉及到登录用户的信息。获取登录用户的信息，将登录的用户的套餐信息修改

					var mVname = ictMe.getUserInfo()['vName'];
					alert("更换");
					//向后台传输数据
					Ext.Ajax.request({
						url: '/pronline/Msg?FunName@ict_change_package&vname=' + mVname + '&tsid=' + tsid + '&id=' + id+ '&timeval=' + timeval,
						method: 'GET',
						success: function (resp, opts) {
							var respText = resp.responseText;
							var respRecord = eval("(" + respText + ")");
							if (respRecord.result == 0) {
								alert(respRecord.mesg);

							} else if (respRecord.result == 1) {
								alert(respRecord.mesg);
							}
							//alert(respRecord.result+"_"+respRecord.mesg);
						},
						failure: function (resp, opts) {
							var respText = eval("(" + respText + ")");
							alert('错误', respText.error);
						}
					});

				}
			}, {
				text: '取消',
				handler: function () {
					Ext.getCmp('addwindow').close();
					//                fp.getForm().reset();
				}
			}

			]
		});
		
		
		var win2 = new Ext.Window({
			id: "addwindow",
			title: "更换套餐详细",
			x: 600,
			y: 300,
			width: 430,
			height: 220,
			bodyPadding: 10,
			modal: true,
			resizable: true,
			items: fp2

		});
		win2.show();
		
	}
	
	
//公共接口
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
		pnlCenter.setTitle("您当前的位置: "+title);
		pnlCenter.body.update('<iframe src="'+url+'" frameborder="no" style="width:100%;height:1000px;"></iframe>');		
	}
	this.cutLan = function(){
		alert("点击断线");
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
			this.setLastError('您的电子邮件格式不正确');
			return false;
		}
	}
	this.CheckPhone = function(Phone) {
		var filter=/^1\d{10}$|^(0\d{2,3}-?|\(0\d{2,3}\))?[1-9]\d{4,7}(-\d{1,8})?$/;
		if (filter.test(Phone)){
			return true;
		}else {
			this.setLastError('您的电话格式不正确');
			return false;
		}
	}
	this.checkLoginData = function(vname, passwd){
		if(this.CheckPhone(vname)==false){
			return false;
		}
		if(passwd == ""){
			this.setLastError("密码不能为空");
			return false;
		}
		return true;
	}
}
var ictGlobalCtx = new f_IctGlobalContext(); 
















