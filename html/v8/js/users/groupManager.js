/*单位管理*/
var atd = '';
var isupdate = "";
if(autodef == 'false'){
		atd = false;
}else{
		atd = true;	
}

Ext.define('users.groupManager', {
		extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
    		if(store_users_groupmanager.alreadyload !=1){
    				store_users_groupmanager.groupid = '';
    				store_users_groupmanager.groupname = '';
    				store_users_groupmanager.dispname = '';
    				store_users_groupmanager.moditime = '';
    				store_users_groupmanager.flags = '';
    				store_users_groupmanager.s_flags = '';
    				store_users_groupmanager.keyword = '';
						store_users_groupmanager.caExport = '';
    				store_users_groupmanager.new_params = {groupid:store_users_groupmanager.groupid,groupname:store_users_groupmanager.groupname,dispname:store_users_groupmanager.dispname,moditime:store_users_groupmanager.moditime,flags:store_users_groupmanager.flags,s_flags:store_users_groupmanager.s_flags,keyword:store_users_groupmanager.keyword,caExport:store_users_groupmanager.caExport};
    		}
    
		    function reflash(){
		    		store_users_groupmanager.new_params = {groupid:store_users_groupmanager.groupid,groupname:store_users_groupmanager.groupname,dispname:store_users_groupmanager.dispname,moditime:store_users_groupmanager.moditime,flags:store_users_groupmanager.flags,s_flags:store_users_groupmanager.s_flags,keyword:store_users_groupmanager.keyword,caExport:store_users_groupmanager.caExport};
		    		//store_users_groupmanager.currentPage=1;
			      store_users_groupmanager.load();	
			    
		    }
		    
		    			var store_limit_band = Ext.create('Ext.data.Store', 
			{
						id:'store_limit_band',
				    fields:['name', 'value'],
    				proxy: 
    				{
        			type: 'ajax',
        			url: '/pronline/Msg?FunName@ncm_limitband',
        			reader: 
       		 	{
            	type:'json',
            	root: 'record'
            	
        		}
    				},
    					
    					 listeners: {
            load: function() {
   // 	        var combo = Ext.getCmp('limitband');
   //             combo.setValue(combo.getValue());
            }
          }
    				
			});
			
				store_limit_band.load();	
		    
		    
		    
		    
		    var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('groupmanager_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });	
		    
		    var flagsStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'无效', groupid:'0'},
		        		{groupname:'有效', groupid:'1'}
		        ]
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
	
				var flagsStore4 = Ext.create('Ext.data.Store', {
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
				function gotoedit4(value, id,groupid){
	 
			    isupdate = "";
					var win3 = null;
					var srvpar_form2;
					var ptype = '1';
					var group_display = false;
					var group = groupid;
					var ssid = '';
					var ssid_display = true;
					var sessiontime = '';
					var idletime = '';
					var macbindtime = '';
					var smslocalnum = '';
					var smsinternum = '';
					var smsusetime = '';
					var limittime = '';
					var limitflow = '';
					var limitband = '';
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
					var smsname='';
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
					smsname=store_rules_srvpar.getAt(id).get('smsname');

					//smsusetime_stime = smsusetime.split("-");
					//alert(smsusetime_stime);
					//smsusetime_etime = smsusetime;
					//alert(smsusetime_etime);
					smsusetime_stime=smsusetime.substring(0,8);
					smsusetime_etime=smsusetime.substring(9,18);
			
					limittime = store_rules_srvpar.getAt(id).get('limittime');
					limitflow = store_rules_srvpar.getAt(id).get('limitflow');
					limitband = store_rules_srvpar.getAt(id).get('limitband');
					

	/*					
						if(limitband==1)
						{
  					limitband="16K";
  					}
						if(limitband==2)
						{
  					limitband="32K";
 					  }
						if(limitband==3)
						{
  					limitband="64K";
  					}
  					if(limitband==4)
						{
  					limitband="128K";
  					}
  					if(limitband==5)
						{
  					limitband="256K";
  					}
  					if(limitband==6)
						{
  					limitband="512K";
  					}
  					if(limitband==7)
						{
  					limitband="1M";
  					}
  					if(limitband==8)
						{
  					limitband="2M";
  					}
  					if(limitband==9)
						{
  					limitband="4M";
  					}
				
						if(limitband==10)
						{
							limitband="8M";	
						}
						if(limitband==11)
						{
  					limitband="16M";
  					}
  					if(limitband==12)
						{
  					limitband="32M";
  					}
	*/				
					
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
				                    	}
				                    	,{
				                        	fieldLabel:'短信平台标志',
				                        	xtype: 'textfield',
											emptyText:'用于区分短信发送平台',
											value:smsname,
				                        	name:'smsname'
				                    	},
				                    	
				                    	
				                    	]},{
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
				                        	fieldLabel:'上网流量限制(MB)',
				                        	xtype: 'textfield',
											afterLabelTextTpl: required,
											emptyText:'请输入每天上网流量限制',
											value:limitflow,
				                        	name:'limitflow'
				                    	},{
				                        	xtype:'combo',
											name:'limitband',
											value:limitband,
											fieldLabel:'带宽设置(KB)',
											emptyText:'请设置带宽',
											triggerAction:'all',
											store:store_limit_band,
											displayField:'name',
											valueField:'value',
											editable:false,
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
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
											store:flagsStore4,
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
														
														var limitflow1 = objectTmp.limitflow;
														var limitband1 = objectTmp.limitband;
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
														var smsname=objectTmp.smsname;
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
														
													isupdate = "ys";
													
								//					alert(limitband1);
													srvpar_form2.form.doAction('submit',{
							             url:'/pronline/Msg',
							             method:'post',
							             params:{FunName:'ncm_srvpar',update:value,pid:pid1,ptype:ptype1,sessiontime:sessiontime1,idletime:idletime1,macbindtime:macbindtime1,smslocalnum:smslocalnum1,smsinternum:smsinternum1,smsusetime:smsusetime,limittime:limittime1,limitflow:limitflow1,limitunit:limitunit1,passcodetime:passcodetime1,servicetime:servicetime,eng_smsmessage:eng_smsmessage1,zh_smsmessage:zh_smsmessage1,autoauth:autoauth1,macauth:macauth1,weixinauth:weixinauth1,maxmac:maxmac1,errornum:errornum1,errorlock:errorlock1,paroption:paroption1,flags:flags1,selsid:selsid,seltype:seltype,smsname:smsname,isupdate:isupdate},
							             success:function(form,action){							           
                           //store_oksecord.load();
                           alert('信息已保存');
							             	
							             }  
					             })
//														store_rules_srvpar.load({params: {update:value,pid:pid1,ptype:ptype1,sessiontime:sessiontime1,idletime:idletime1,macbindtime:macbindtime1,smslocalnum:smslocalnum1,smsinternum:smsinternum1,smsusetime:smsusetime,limittime:limittime1,limitflow:limitflow1,limitband:limitband1,limitunit:limitunit1,passcodetime:passcodetime1,servicetime:servicetime,eng_smsmessage:eng_smsmessage1,zh_smsmessage:zh_smsmessage1,autoauth:autoauth1,macauth:macauth1,weixinauth:weixinauth1,maxmac:maxmac1,errornum:errornum1,errorlock:errorlock1,paroption:paroption1,flags:flags1,selsid:selsid,seltype:seltype},callback:function(records, options, success){
//														
//											
//
//														
//														
//														reflash();
//														}
//														});
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
				function gotoedit3(id, value){  // 黑白名单
					  store_rules_white1.pid = id;
					  store_rules_white1.ptype = value;
					  
						var windr2 = new Ext.Window ({
                 id:'white1_windr2',
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
                 items:[Ext.create('rules.white1')]                 
				    })
				    
					  windr2.on("beforeclose", function() {
					  	 reflash();
					  })
					  windr2.show();
				}
				
				function gotoedit2(id){  // ap管理
					  store_users_apmanager.groupid = id;
						var windr3 = new Ext.Window ({
							     id:'apmanager_windr',
                   title:"AP管理",
                   //x:0,
                   y:0,
                   width:900,
                   height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
                   autoScroll:true,
                 	 bodyPadding: 0,
                   modal:true,
								   //headerPosition: 'bottom',
                   resizable:true,
                   closable:true,
                   draggable:true,
                   items:[Ext.create('users.apManager')]                    
				    })
				    
					  windr3.on("beforeclose", function() {
					  	 reflash();
					  })
					  windr3.show();
				}
				
		    function gotoedit1(value,id , hgt){
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_websrvgroup_info&key@'+value+'&groupid@'+id+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%"></iframe>';
						var windr = new Ext.Window ({
                 id:"windr",
                 title:"单位信息",
                 //x:0,
                 //y:0,
                 width:370,
                 height:hgt,
                 bodyPadding: 0,
                 autoScroll:true,
                 modal:true,
								 //headerPosition: 'bottom',
                 resizable:true,
                 closable : true,
                 draggable:true,
                 html:htmlvar                        
						})       
						
						windr.on("beforeclose", function() {
							reflash();
						})
						windr.show();
				}
		    
		    Ext.apply(this, {
		    		border:false,
				    frame:false,
						height: Ext.get("layout_center").getHeight()-63,
						//height:Ext.get("layout_center").getHeight(),
						//autoScroll: true,
						selModel: sm,
						//height: grid_height,
		        store: store_users_groupmanager,
		        viewConfig:{
			        	loadMask : false
			      },
			      columns:[{
		            text: "序号",
		            dataIndex: 'groupid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'groupid',
		            iconCls: 'edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            hidden:atd,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', record.get('groupid') , 482);
		            }
		        },{
		            header: '参数设置',
		            xtype: 'actioncolumn',
		            dataIndex: 'groupid',
		            iconCls: 'report_edit',
		            tooltip: '点击进入参数设置界面',
		            width: 100,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
										//alert(record.get('groupid'));
						        store_rules_srvpar.load({params: {update:"",pid:record.get('groupid'),ptype:"1"},callback:function(records, options, success){
										if(store_rules_srvpar.getTotalCount()>0)
												gotoedit4('update', "0");
										else{
												gotoedit4('add', "0",record.get('groupid'));
										}
										}});
		            }
		        },{
		            text: "单位编码",
		            dataIndex: 'groupname',
		            width: 200,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "显示名称",
		            dataIndex: 'dispname',
		            width: 100,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "区域",
		            dataIndex: 'areaname',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            header: 'AP管理',
		            xtype: 'actioncolumn',
		            dataIndex: 'count',
		            //iconCls: 'report_edit',
		            tooltip: '点击进入AP管理界面',
		            width: 80,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit2(record.get('groupid'));
		            },
		            getClass:function(value){
		            	  var css="information";
		            	  
		            		if(value > 0){
		            			 css = "information";	
		            		}	else {
		            			 css = "exclamation";
		            		}
		            		return css;
		            }
		        },{
		            header: '黑白名单',
		            xtype: 'actioncolumn',
		            dataIndex: 'groupid',
		            iconCls: 'edit',
		            tooltip: '点击进入黑白名单界面',
		            hidden:true,
		            width: 80,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit3(record.get('groupid'), 1);
		            }
		        },{
		            text: " 标记",
		            dataIndex: 'flags',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            renderer:function(value, metaData, record){
		            		return record.get('s_flags'); 
		            }
		        },{
		            text: "最后修改时间",
		            dataIndex: 'moditime',
		            width: 135,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        }
		        ,{
		            header: '导入用户',
		            xtype: 'actioncolumn',
		            dataIndex: 'groupid',
		            iconCls: 'edit',
		            tooltip: '点击进入导入界面',
		            hidden:false,
		            width: 80,
		            align: 'center',
		            sortable: false,

	              handler: function(grid, rowIndex, colIndex, actionItem, event, record, row){
	              	
					            var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_userman/user_dr_first.htm&groupid@'+record.get('groupid')+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
					            var windr = new Ext.Window ({
                             id:"windr",
                             title:"导入用户",
                             x:200,
                             y:100,
                             width:500,
                             height:460,
                             bodyPadding: 0,
                             modal:true,
                             resizable:true,
                             closable : true,
                             draggable:true,
                             html:htmlvar               
					            })       
					
					           windr.on("beforeclose", function() {
					        			reflash();
					           })
					           windr.show();
		           	}


		        }
		        ,{
		        	  flex: 1	
		        }
		        
		        
		        ],
		        
		        columnLines: true,
		        tbar:{items:[
		        		'-',{
		            		labelWidth: 45,
		            		width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'groupmanager_keyword',
		                name:'groupmanager_keyword',
										emptyText:'请输入单位名称',
		                value:store_users_groupmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入单位名称'){                
						                   this.setValue('');
						                }
										    },
			                  'keydown' : function(i,e){
				                  	var aae=e.getKey(); 
				                    if(aae==13){
						                  	var aa=Ext.getCmp('groupmanager_keyword').value;
									       	    	store_users_groupmanager.keyword=aa;
											    			store_users_groupmanager.currentPage=1;
											    			reflash();
				                    }
			                  }
		                }
		            },'-',{
		                text:'查询',
		            	  iconCls:'search2',
		                handler:function(){
			                 var bb=Ext.getCmp('groupmanager_keyword').value;
				               store_users_groupmanager.keyword=bb;
				               store_users_groupmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
		                 hidden:atd,
			               handler : function() {
			               	  gotoedit1('add', '' , 482);
			               }
		            },'-', {
		                text:'删除',
										id:'groupmanager_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                hidden:atd,
		                handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('groupid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('groupid') + "'";
					                    }
					                }
													store_users_groupmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            },'-', {
		                text:'应用',
						        id:'groupmanager_refreshButton',
		                iconCls:'accept',
		                disabled: false,
		                handler:function(){
								       store_users_groupmanager.load({params: {update: "refresh"}});
		                }
		            }]
		        }	
		    });
		    store_users_groupmanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});