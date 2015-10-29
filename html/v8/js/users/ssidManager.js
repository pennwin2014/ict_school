/*SSID管理*/
Ext.define('users.ssidManager', {
		extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
    		if(store_users_ssidmanager.alreadyload !=1){
    			  store_users_ssidmanager.sid = '';
    				store_users_ssidmanager.groupid = '';
    				store_users_ssidmanager.ssid = '';
    				store_users_ssidmanager.addtime = '';
    				store_users_ssidmanager.moditime = '';
    				store_users_ssidmanager.flags = '';
    				store_users_ssidmanager.s_flags = '';
    				store_users_ssidmanager.keyword = '';
					store_users_ssidmanager.caExport = '';
    				store_users_ssidmanager.new_params = {sid:store_users_ssidmanager.sid,groupid:store_users_ssidmanager.groupid,ssid:store_users_ssidmanager.ssid,addtime:store_users_ssidmanager.addtime,moditime:store_users_ssidmanager.moditime,flags:store_users_ssidmanager.flags,s_flags:store_users_ssidmanager.s_flags,keyword:store_users_ssidmanager.keyword,caExport:store_users_ssidmanager.caExport};
    		}
    
		    function reflash(){
		    		store_users_ssidmanager.new_params = {sid:store_users_ssidmanager.sid,groupid:store_users_ssidmanager.groupid,ssid:store_users_ssidmanager.ssid,addtime:store_users_ssidmanager.addtime,moditime:store_users_ssidmanager.moditime,flags:store_users_ssidmanager.flags,s_flags:store_users_ssidmanager.s_flags,keyword:store_users_ssidmanager.keyword,caExport:store_users_ssidmanager.caExport};
		    		//store_users_ssidmanager.currentPage=1;
			      store_users_ssidmanager.load();	
		    }
		    
		    var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('ssidmanager_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });	
		    
		    var flagsStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'未开通', groupid:'0'},
		        		{groupname:'正常使用', groupid:'1'}
		        ]
				});
				Ext.define('TypeData', {
						extend:'Ext.data.Model',
						fields:['groupid', 'dispname']
				});
				var store_group = Ext.create('Ext.data.Store',{
						model:'TypeData',
						pageSize: 10000,
						autoLoad: true,
		      	proxy: {
		        		type: 'ajax',
		        		url: '/pronline/Msg?FunName@ncm_srvgroup',
		        		reader: {
			        			type:'json',
			        			root: 'eimdata',
			        			totalProperty: 'totalCount'
		        		},
		        		simpleSortMode: true
		      	}
				});
				Ext.define('TypeData2', {
						extend:'Ext.data.Model',
						fields:['sid', 'ssid']
				});
				var store_ssid = Ext.create('Ext.data.Store',{
						model:'TypeData2',
						pageSize: 10000,
						autoLoad: true,
		      	proxy: {
		        		type: 'ajax',
		        		url: '/pronline/Msg?FunName@ncm_srvgroupssid',
		        		reader: {
			        			type:'json',
			        			root: 'eimdata',
			        			totalProperty: 'totalCount'
		        		},
		        		simpleSortMode: true
		      	}
				});
			
				var flagsStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'使用', groupid:'1'},
				    		{groupname:'不使用', groupid:'0'}
				    ]
				});
				var macauthStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'使用', groupid:'1'},
				    		{groupname:'不使用', groupid:'0'}
				    ]
				});
				var autoauthStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'使用', groupid:'1'},
				    		{groupname:'不使用', groupid:'0'}
				    ]
				});
				var errorlockStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'使用', groupid:'1'},
				    		{groupname:'不使用', groupid:'0'}
				    ]
				});
				var limitunitStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'天', groupid:'0'},
				    		{groupname:'周', groupid:'1'},
				    		{groupname:'月', groupid:'2'},
				    		{groupname:'年', groupid:'3'},
				    		{groupname:'总共', groupid:'9'}
				    ]
				});
				var weixinauthStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'不使用', groupid:'0'},
				    		{groupname:'微信认证', groupid:'1'}
				    ]
				});
				var ptypeStore = Ext.create('Ext.data.Store', {
				    fields:['groupname', 'groupid'],
				    data:[
				    		{groupname:'全局', groupid:'0'},
				    		{groupname:'Group', groupid:'1'},
				    		{groupname:'SSID', groupid:'2'}
				    ]
				});

	 			//高级查询窗口
				function gotoedit4(value, id,ssidvalue){
					var win3 = null;
					var srvpar_form2;
					var ptype = '2';
					var group_display = true;
					var group = '';
					var ssid = ssidvalue;
					var ssid_display = false;
					var sessiontime = '';
					var idletime = '';
					var macbindtime = '';
					var smslocalnum = '';
					var smsinternum = '';
					var smsusetime = '';
					var limittime = '';
					var limitunit = '0';
					var passcodetime = '';
					var servicetime = '';
					var eng_smsmessage = 'Your WLAN passcode is [#password#].【新网程】';
					var zh_smsmessage = '你的 WLAN 网络密码是：[#password#]。【新网程】';
					var autoauth = '';
					var macauth = '';
					var weixinauth = '';
					var maxmac = '';
					var errornum = '';
					var errornum_dispaly=true;
					var errorlock = '';
					var paroption = 'NcmTest = "Yes"';
					var flags = '';
					var sid1 = '';
					var title='增加';
					var selsid='';
					var seltype='';
					var smsusetime_stime='';
					var smsusetime_etime='';
					var servicetime_stime='';
					var servicetime_etime='';
					var ptype_read=true;
					var group_read=true;
					var ssid_read=true;
					if(value == 'update'){
					title='修改';
					ptype_read = true;
					group_read = true;
					ssid_read = true;
					ptype = store_rules_srvpar.getAt(id).get('ptype');
					if(ptype == 1){
					group = store_rules_srvpar.getAt(id).get('pid');
					group_display=false;
					}
					if(ptype == 2){
					ssid = store_rules_srvpar.getAt(id).get('pid');
					ssid_display=false;
					}
					selsid = store_rules_srvpar.getAt(id).get('pid');
					seltype = store_rules_srvpar.getAt(id).get('ptype');
					sessiontime = store_rules_srvpar.getAt(id).get('sessiontime');
					idletime = store_rules_srvpar.getAt(id).get('idletime');
					macbindtime = store_rules_srvpar.getAt(id).get('macbindtime');
					smslocalnum = store_rules_srvpar.getAt(id).get('smslocalnum');
					smsinternum = store_rules_srvpar.getAt(id).get('smsinternum');
					smsusetime = store_rules_srvpar.getAt(id).get('smsusetime');
					//smsusetime_stime = smsusetime.split("-");
					//alert(smsusetime_stime);
					//smsusetime_etime = smsusetime;
					//alert(smsusetime_etime);
					smsusetime_stime=smsusetime.substring(0,8);
					smsusetime_etime=smsusetime.substring(9,18);
			
					limittime = store_rules_srvpar.getAt(id).get('limittime');
					limitunit = store_rules_srvpar.getAt(id).get('limitunit');
					passcodetime = store_rules_srvpar.getAt(id).get('passcodetime');
					servicetime = store_rules_srvpar.getAt(id).get('servicetime');
					servicetime_stime=servicetime.substring(0,8);
					servicetime_etime=servicetime.substring(9,18);
					eng_smsmessage = store_rules_srvpar.getAt(id).get('eng_smsmessage');
					zh_smsmessage = store_rules_srvpar.getAt(id).get('zh_smsmessage');
					autoauth = store_rules_srvpar.getAt(id).get('autoauth');
					macauth = store_rules_srvpar.getAt(id).get('macauth');
					weixinauth = store_rules_srvpar.getAt(id).get('weixinauth');
					maxmac = store_rules_srvpar.getAt(id).get('maxmac');
					
					errorlock = store_rules_srvpar.getAt(id).get('errorlock');
					if(errorlock == 0)
					{
					errornum_dispaly=true;
					}
					if(errorlock == 1){
					errornum_dispaly=false;	
					errornum = store_rules_srvpar.getAt(id).get('errornum');
					}
					paroption = store_rules_srvpar.getAt(id).get('paroption');
					flags = store_rules_srvpar.getAt(id).get('flags');
					sid1 = store_rules_srvpar.getAt(id).get('sid');
					}
					
				    if (!win3) //判断如果不存在就创建新的
				    {
				        win3 = new Ext.Window({
								    title:'设置窗口 - '+ title,
								    closeAction: 'hide',
								    //width:900,
								    draggable:true, //可拖动的
									maximizable:true, //可最大化的
							      //resizable: true, //可改变大小
									  modal: false,//后面的内容可以操作
							      //plain：true,//则主体背景透明
							      //items: fp2
								    constrain:true, //限制窗口只能在其容器内移动
							      //minimizable:true, //可最小化
							      
				            items:[srvpar_form2 = new Ext.FormPanel({
								//id:'srvpar_form2',
								type: 'form',
								baseCls:"x-plain",
								bodyPadding: 10,
								items:[
								{  
										anchor:'100%', 
										baseCls:"x-plain",
										layout:{  
										 type:'hbox',  
										 align:'top'  
										},  
									items:[
										{
				                        	xtype:'combo',
											name:'ptype',
											fieldLabel:'类别',
											style: "margin-left:10",
											emptyText:'请选限类别',
											value:ptype,
											readOnly:ptype_read,
											triggerAction:'all',
											store:ptypeStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true,
											listeners:{
											'change': function(){
											if(this.value ==0){
											srvpar_form2.getForm().findField('group').setVisible(false);
											srvpar_form2.getForm().findField('ssid').setVisible(false);
											}
											if(this.value ==1){
											srvpar_form2.getForm().findField('group').setVisible(true);
											srvpar_form2.getForm().findField('ssid').setVisible(false);
											
											}
											if(this.value ==2){
											srvpar_form2.getForm().findField('group').setVisible(false);
											srvpar_form2.getForm().findField('ssid').setVisible(true);
											}
											}
											}
				                    	},{
				                        	xtype:'combo',
											name:'group',
											style: "margin-left:40",
											fieldLabel:'Group',
											value:group,
											emptyText:'请选择Group',
											triggerAction:'all',
											store:store_group,
											displayField:'dispname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											hidden:group_display,
											readOnly:group_read,
											typeAhead:true,
											allowBlank:true
				                    	},{
				                        	xtype:'combo',
											name:'ssid',
											style: "margin-left:40",
											fieldLabel:'SSID',
											value:ssid,
											emptyText:'请选择SSID',
											triggerAction:'all',
											store:store_ssid,
											displayField:'ssid',
											valueField:'sid',
											queryMode:'local',
											forceSelection:false,
											hidden:ssid_display,
											readOnly:ssid_read,
											typeAhead:true,
											allowBlank:true
				                    	}]},{  
										anchor:'100%', 
										baseCls:"x-plain",
										layout:{  
										 type:'hbox',  
										// padding:'10',  
										// pack:'start',  
										 align:'top'  
										},  
								
									items:[
									{
											xtype:'fieldset',
											//width:350,
							
											title:'认证设置',
											collapsible: true,
											colspan: 1,
											items:[		
										{
				                        	fieldLabel:'Mac绑定时间(秒)',
				                        	xtype: 'textfield',
											emptyText:'请输入ac地址绑定时间',
											value:macbindtime,
				                        	name:'macbindtime'
				                    	},{
				                        	fieldLabel:'密码有效时间(秒)',
				                        	xtype: 'textfield',
											afterLabelTextTpl: required,
											emptyText:'请输入密码的有效时间',
											value:passcodetime,
				                        	name:'passcodetime'
				                    	},
										{
				                        	xtype:'combo',
											name:'autoauth',
											fieldLabel:'第二次自动验证',
											emptyText:'请选择第二次自动验证',
											value:autoauth,
											triggerAction:'all',
											store:autoauthStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
				                    	},
										{
				                        	xtype:'combo',
											name:'macauth',
											fieldLabel:'无感知认证',
											emptyText:'请选择无感知认证',
											value:macauth,
											triggerAction:'all',
											store:macauthStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
				                    	},
										{
				                        	xtype:'combo',
											name:'weixinauth',
											fieldLabel:'第三方认证',
											emptyText:'请选择第三方认证',
											value:weixinauth,
											triggerAction:'all',
											store:weixinauthStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
				                    	},{
				                        	fieldLabel:'一个账号最多绑定Mac地址的数量',
				                        	xtype: 'textfield',
											emptyText:'请输入一个账号最多绑定Mac地址的数量',
											value:maxmac,
				                        	name:'maxmac'
				                    	},{
				                        	fieldLabel:'错误次数',
				                        	xtype: 'textfield',
											emptyText:'请输入错误次数',
											hidden:errornum_dispaly,
											value:errornum,
				                        	name:'errornum'
				                    	},
										{
				                        	xtype:'combo',
											name:'errorlock',
											fieldLabel:'错误开关',
											emptyText:'请选择错误开关',
											value:errorlock,
											triggerAction:'all',
											store:errorlockStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true,
											listeners:{
											'change': function(){
											if(this.value ==0){
											srvpar_form2.getForm().findField('errornum').setVisible(false);
											}
											if(this.value ==1){
											srvpar_form2.getForm().findField('errornum').setVisible(true);
											}
											}
											}
				                    	},{
				                        	fieldLabel:'参数选项',
				                        	xtype: 'textarea',
											//emptyText:'请输入参数选项',
											height:100,
											value:paroption,
				                        	name:'paroption'
				                    	}]},{
											xtype:'fieldset',
											//width:350,
											
											style: "margin-left:10",
							
											title:'短信设置',
											collapsible: true,
											colspan: 1,
											items:[
										{
				                        	fieldLabel:'国内短信限制数量',
				                        	xtype: 'textfield',
											emptyText:'请输入国内短信限制数量',
											value:smslocalnum,
				                        	name:'smslocalnum'
				                    	},
										{
				                        	fieldLabel:'国际短信限制数量',
				                        	xtype: 'textfield',
											emptyText:'请输入国际短信限制数量',
											value:smsinternum,
				                        	name:'smsinternum'
				                    	},
										{
				                    			fieldLabel:'短信发送开始时间',
				                    			xtype:'timefield',
												emptyText:'请选择开始时间',
									            value:smsusetime_stime,
									            name:'smsusetime_stime',
									            labelSeparator:':', //分隔符
												msgTarget:'side', //在字段的右边显示一个提示信息  
												autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
												maxValue:'23:59:59', //最大时间
												maxText:'时间应小于{0}', //大于最大时间的提示信息
												minValue:'00:00:00', //最小时间  
												minText:'时间应大于{0}', //小于最小时间的提示信息
												pickerMaxHeight:100, //下拉列表的最大高度
												increment:1, //时间间隔为60分钟 
												format: "H:i:s",
												invalidText:'时间格式无效'
				                    	},{
				                    			fieldLabel:'短信发送结束时间',
				                    			xtype:'timefield',
												emptyText:'请选择结束时间',
									            value:smsusetime_etime,
									            name:'smsusetime_etime',
									            labelSeparator:':', //分隔符
												msgTarget:'side', //在字段的右边显示一个提示信息  
												autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
												maxValue:'23:59:59', //最大时间
												maxText:'时间应小于{0}', //大于最大时间的提示信息
												minValue:'00:00:00', //最小时间  
												minText:'时间应大于{0}', //小于最小时间的提示信息
												pickerMaxHeight:100, //下拉列表的最大高度
												increment:1, //时间间隔为60分钟 
												format: "H:i:s",
												invalidText:'时间格式无效'
				                      },{
				                        	fieldLabel:'英文短信格式',
				                        	xtype: 'textarea',
											//emptyText:'请输入英文短信格式',
											height:90,
											value:eng_smsmessage,
				                        	name:'eng_smsmessage'
				                    	},{
				                        	fieldLabel:'中文短信格式',
				                        	xtype: 'textarea',
											afterLabelTextTpl: required,
											//emptyText:'请输入中文短信格式',
											height:90,
											value:zh_smsmessage,
				                        	name:'zh_smsmessage'
				                    	}]},{
											xtype:'fieldset',
											//width:350,
											title:'上网设置',
											style: "margin-left:10",
											collapsible: true,
											colspan: 1,
											items:[
										{
				                        	fieldLabel:'登录超时时间(秒)',
				                        	xtype: 'textfield',
											afterLabelTextTpl: required,
											emptyText:'请输入登录超时时间',
											value:sessiontime,
				                        	name:'sessiontime'
				                    	},{
				                        	fieldLabel:'空闲超时时间(秒)',
				                        	xtype: 'textfield',
											afterLabelTextTpl: required,
											emptyText:'请输入空闲超时时间',
											value:idletime,
				                        	name:'idletime'
				                    	},{
				                        	fieldLabel:'上网时间限制(秒)',
				                        	xtype: 'textfield',
											afterLabelTextTpl: required,
											emptyText:'请输入每天上网时间限制',
											value:limittime,
				                        	name:'limittime'
				                    	},{
				                        	xtype:'combo',
											name:'limitunit',
											fieldLabel:'限制单位',
											emptyText:'请选限制单位',
											value:limitunit,
											triggerAction:'all',
											store:limitunitStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
				                    	},{
				                    			fieldLabel:'上网开始时间',
				                    			xtype:'timefield',
												emptyText:'请选择开始时间',
									            value:servicetime_stime,
									            name:'servicetime_stime',
									            labelSeparator:':', //分隔符
												msgTarget:'side', //在字段的右边显示一个提示信息  
												autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
												maxValue:'23:59:59', //最大时间
												maxText:'时间应小于{0}', //大于最大时间的提示信息
												minValue:'00:00:00', //最小时间  
												minText:'时间应大于{0}', //小于最小时间的提示信息
												pickerMaxHeight:100, //下拉列表的最大高度
												increment:1, //时间间隔为60分钟 
												format: "H:i:s",
												invalidText:'时间格式无效'
				                    	},{
				                    			fieldLabel:'上网结束时间',
				                    			xtype:'timefield',
												emptyText:'请选择结束时间',
									            value:servicetime_etime,
									            name:'servicetime_etime',
									            labelSeparator:':', //分隔符
												msgTarget:'side', //在字段的右边显示一个提示信息  
												autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
												maxValue:'23:59:59', //最大时间
												maxText:'时间应小于{0}', //大于最大时间的提示信息
												minValue:'00:00:00', //最小时间  
												minText:'时间应大于{0}', //小于最小时间的提示信息
												pickerMaxHeight:100, //下拉列表的最大高度
												increment:1, //时间间隔为60分钟 
												format: "H:i:s",
												invalidText:'时间格式无效'
				                      }
										]}
								]},
									{  
										anchor:'100%', 
										baseCls:"x-plain",
										layout:{  
										 type:'hbox',  
										 align:'top'  
										},  
									items:[
									]},{
				                        	xtype:'combo',
											name:'flags',
											fieldLabel:'标记',
											emptyText:'请选择标记',
											value:flags,
											triggerAction:'all',
											store:flagsStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
				                    	}
										]  
				                })],  
								        buttons:[{
													  text:"确定",
													  iconCls:'icon-save',		  
													  handler:function(){
									        			var objectTmp = srvpar_form2.getForm().getValues();
														var ptype1 = objectTmp.ptype;
														var pid1="0";
														if(ptype1 == 1){
														pid1 = objectTmp.group;
														}
														if(ptype1 == 2){
														pid1 = objectTmp.ssid;
														}
														
														//var pid1 = objectTmp.pid;
														var sessiontime1 = objectTmp.sessiontime;
														var idletime1 = objectTmp.idletime;
														var macbindtime1 = objectTmp.macbindtime;
														var smslocalnum1 = objectTmp.smslocalnum;
														
														var smsinternum1 = objectTmp.smsinternum;
														var smsusetime_stime1 = objectTmp.smsusetime_stime;
														var smsusetime_etime1 = objectTmp.smsusetime_etime;
														var smsusetime="";
														if(smsusetime_stime1 == 'undefined'){
														smsusetime="";
														}
														else if(smsusetime_etime1 == 'undefined'){
														smsusetime="";
														}else{
														smsusetime = smsusetime_stime1+'-'+smsusetime_etime1;
														}
														var limittime1 = objectTmp.limittime;
														var limitunit1 = objectTmp.limitunit;
														var passcodetime1 = objectTmp.passcodetime;
														var servicetime_stime1 = objectTmp.servicetime_stime;
														var servicetime_etime1 = objectTmp.servicetime_etime;
														var servicetime = servicetime_stime1+'-'+servicetime_etime1;
														var eng_smsmessage1 = objectTmp.eng_smsmessage;
														var zh_smsmessage1 = objectTmp.zh_smsmessage;
														var autoauth1 = objectTmp.autoauth;
														var macauth1 = objectTmp.macauth;
														var weixinauth1 = objectTmp.weixinauth;
														var maxmac1 = objectTmp.maxmac;
														var errornum1 = objectTmp.errornum;
														var errorlock1 = objectTmp.errorlock;
														var paroption1 = objectTmp.paroption;
														var flags1 = objectTmp.flags;
														
														if(passcodetime1 == '' || passcodetime1 == 0){
														alert("密码有效时间不能为零")
														}
														else if(sessiontime1 == '' || sessiontime1 == 0){
														alert("登录超时时间不能为零")
														}
														else if(idletime1 == '' || idletime1 == 0){
														alert("空闲超时时间不能为零")
														}else if(limittime1 == '' || limittime1 == 0){
														alert("上网时间限制不能为零")
														}
														else if(zh_smsmessage1 == ''){
														alert("中文短信格式不能为空")
														}
														else{
										
														store_rules_srvpar.load({params: {update:value,pid:pid1,ptype:ptype1,sessiontime:sessiontime1,idletime:idletime1,macbindtime:macbindtime1,smslocalnum:smslocalnum1,smslocalnum:smslocalnum1,smsinternum:smsinternum1,smsusetime:smsusetime,limittime:limittime1,limitunit:limitunit1,passcodetime:passcodetime1,servicetime:servicetime,eng_smsmessage:eng_smsmessage1,zh_smsmessage:zh_smsmessage1,autoauth:autoauth1,macauth:macauth1,weixinauth:weixinauth1,maxmac:maxmac1,errornum:errornum1,errorlock:errorlock1,paroption:paroption1,flags:flags1,selsid:selsid,seltype:seltype},callback:function(records, options, success){
										
														reflash();
														}
														});
														win3.close();
														}
													  }
												},{
														text:"取消",
														handler: function(){
																win3.close();
														}
												}]
								});
						}
				    win3.show();
				
			  }	
				function gotoedit2(id, value){
					  store_rules_white2.pid = id;
					  store_rules_white2.ptype = value;
					  
						var windr2 = new Ext.Window ({
							             id:'white2_windr2',
                           title:"黑白名单",
                           x:130,
                           y:90,
                           width:1170,
                           height:653,
                           //width:window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth,
                           //height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
                           autoScroll:true,
                           bodyPadding: 0,
                           modal:true,
													 //headerPosition: 'bottom',
                           resizable:true,
                           closable:true,
                           draggable:true,
                           items:[Ext.create('rules.white2')]                 
				    })
				    
					  windr2.on("beforeclose", function() {
					  	 reflash();
					  })
					  windr2.show();
				}
				
				Ext.define('GroupData', {
						extend:'Ext.data.Model',
						fields:['groupid', 'groupname', 'dispname', 'moditime', 'flags', 's_flags', 'count']
				});
				var store_group = Ext.create('Ext.data.Store',{
						model:'GroupData',
						pageSize: 10000,
						autoLoad: true,
	        	proxy: {
		        		type: 'ajax',
		        		url: '/pronline/Msg?FunName@ncm_websrvgroup_list',
		        		reader: {
			        			type:'json',
			        			root: 'eimdata',
			        			totalProperty: 'totalCount'
		        		},
		        		simpleSortMode: true
	        	}
        });
		    
		    // 高级查询窗口
		    function gotoedit1(value, id){
		    		var win3 = null;
		    		var sid1 = '';
		    		var groupid = '';
		    		var ssid = '';
		    		var flags = '1';
		    		var addtime1 = '';
		    			
		    		if(value == 'update'){
		    			  sid1 = store_users_ssidmanager.getAt(id).get('sid');
		    				ssid = store_users_ssidmanager.getAt(id).get('ssid');
		    				flags = store_users_ssidmanager.getAt(id).get('flags');
		    				groupid = store_users_ssidmanager.getAt(id).get('groupid');
		    				addtime1 = store_users_ssidmanager.getAt(id).get('addtime');
		    				//alert(groupid);
		    		}
		    		
		    		if(!win3){ //判断如果不存在就创建新的
		    			  win3 = new Ext.Window({
		    			  		title:'SSID管理',
							    	closeAction: 'hide',
							    	layout:'form',  
							    	width:300,
							   	  draggable:true, //可拖动的
										maximizable:true, //可最大化的
						      	//resizable: true, //可改变大小
										modal: false,//后面的内容可以操作
						      	//plain：true,//则主体背景透明
						      	//items: fp2
							    	constrain:true, //限制窗口只能在其容器内移动
						      	//minimizable:true, //可最小化	
						      	
						      	items:[new Ext.FormPanel({
						      			id:'ssidmanager_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
													  xtype:'combobox',
													  fieldLabel: '单位名称',
										        labelWidth: 100,
										        width: 300,
										        valueField: 'groupid',
										        displayField: 'dispname',
										        value: groupid,
										        name: 'groupid',      
										        allowBlank: true,
										        store: store_group,
										        triggerAction: 'all',
										        queryMode: 'local',
										        typeAhead: true
												},{
			                      fieldLabel:'接入设备SSID',
			                      xtype: 'textfield',
														emptyText:'请输入接入设备SSID',
														value:ssid,
			                      name:'ssid'
			                  },{
			                      xtype:'combo',
														name:'flags',
														fieldLabel:'标记',
														emptyText:'请选择标记',
														value:flags,
														triggerAction:'all',
														store:flagsStore,
														displayField:'groupname',
														valueField:'groupid',
														queryMode:'local',
														forceSelection:false,
														typeAhead:true,
														allowBlank:true
			                  }]	
						      	})],
						      	buttons:[{
												  text:"确定",
												  iconCls:'icon-save',		  
												  handler:function(){
								        			var objectTmp = Ext.getCmp("ssidmanager_form2").getForm().getValues();
															var groupid1 = objectTmp.groupid;
															var ssid1 = objectTmp.ssid;
															var flags1 = objectTmp.flags;
															store_users_ssidmanager.load({params: {update:value,sid:sid1,groupid:groupid1,ssid:ssid1,addtime:addtime1,flags:flags1}});
															//reflash();
															win3.close();
												  }
										},{
													text:"取消",
													handler: function(){
															win3.close();
													}
									  }]
		    			  });
		    		}
		    		win3.show();
		    }
		    
		    Ext.apply(this, {
		    		border:false,
				    frame:false,
						//height: Ext.get("layout_center").getHeight()-58,
						//height:Ext.get("layout_center").getHeight(),
						autoScroll: true,
						selModel: sm,
						//height: grid_height,
		        store: store_users_ssidmanager,
		        viewConfig:{
			        	loadMask : false
			      },
			      columns:[{
		            text: "序号",
		            dataIndex: 'sid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'sid',
		            iconCls: 'edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            header: '参数设置',
		            xtype: 'actioncolumn',
		            dataIndex: 'sid',
		            iconCls: 'report_edit',
		            tooltip: '点击进入参数设置界面',
		            width: 80,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
										//alert(record.get('sid'));
						        store_rules_srvpar.load({params: {update:"",pid:record.get('sid'),ptype:"2"},callback:function(records, options, success){
										if(store_rules_srvpar.getTotalCount()>0)
												gotoedit4('update', "0");
										else
												gotoedit4('add', "0",record.get('sid'));
										}});
		            }
		        },{
		            text: "单位名称",
		            dataIndex: 'groupname',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "接入设备SSID",
		            dataIndex: 'ssid',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            header: '黑白名单',
		            xtype: 'actioncolumn',
		            dataIndex: 'sid',
		            iconCls: 'edit',
		            tooltip: '点击进入黑白名单界面',
		            hidden:true,
		            width: 80,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit2(record.get('sid'), 2);
		            }
		        },{
		            text: " 标记",
		            dataIndex: 'flags',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            renderer:function(value, metaData, record){
		            		return record.get('s_flags'); 
		            }
		        },{
		            text: "加入时间",
		            dataIndex: 's_addtime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "修改时间",
		            dataIndex: 'moditime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        tbar:{items:[
		        		'-',{
		            		labelWidth: 45,
		            		width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'ssidmanager_keyword',
		                name:'ssidmanager_keyword',
										emptyText:'请输入接入设备SSID',
		                value:store_users_ssidmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入接入设备SSID'){                
						                   this.setValue('');
						                }
										    },
			                  'keydown' : function(i,e){
				                  	var aae=e.getKey(); 
				                    if(aae==13){
						                  	var aa=Ext.getCmp('ssidmanager_keyword').value;
									       	    	store_users_ssidmanager.keyword=aa;
											    			store_users_ssidmanager.currentPage=1;
											    			reflash();
				                    }
			                  }
		                }
		            }, '-', {
		                text:'查询',
		            	  iconCls:'search2',
		                handler:function(){
			                 var bb=Ext.getCmp('ssidmanager_keyword').value;
				               store_users_ssidmanager.keyword=bb;
				               store_users_ssidmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               	  gotoedit1('add', '');
			               }
		            },'-', {
		                text:'删除',
										id:'ssidmanager_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('sid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('sid') + "'";
					                    }
					                }
													store_users_ssidmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }	
		    });
		    store_users_ssidmanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});