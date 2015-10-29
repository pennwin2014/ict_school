/*微信设置*/
Ext.define('settings.weixin',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
			  if(store_settings_weixin.alreadyload !=1){
					  store_settings_weixin.wid = '';
				    store_settings_weixin.name = '';
				    store_settings_weixin.token = '';
				    store_settings_weixin.account = '';
				    store_settings_weixin.appid = '';
				    store_settings_weixin.appsecret = '';
				    store_settings_weixin.sendcont = '';
				    store_settings_weixin.redurl = '';
				    store_settings_weixin.usinfo = '';
				    store_settings_weixin.access_token = '';
				    store_settings_weixin.systime = '';
				    store_settings_weixin.status = '';
				    store_settings_weixin.keyword = '';
				    store_settings_weixin.caExport = ''; //导出
					
					  store_settings_weixin.new_params={wid:store_settings_weixin.wid,name:store_settings_weixin.name,token:store_settings_weixin.token,account:store_settings_weixin.account,appid:store_settings_weixin.appid,appsecret:store_settings_weixin.appsecret,sendcont:store_settings_weixin.sendcont,redurl:store_settings_weixin.redurl,usinfo:store_settings_weixin.usinfo,access_token:store_settings_weixin.access_token,systime:store_settings_weixin.systime,status:store_settings_weixin.status,keyword:store_rules_smsformat.keyword,caExport:store_rules_smsformat.caExport};
				}
		    function reflash(){
						store_settings_weixin.new_params={wid:store_settings_weixin.wid,name:store_settings_weixin.name,token:store_settings_weixin.token,account:store_settings_weixin.account,appid:store_settings_weixin.appid,appsecret:store_settings_weixin.appsecret,sendcont:store_settings_weixin.sendcont,redurl:store_settings_weixin.redurl,usinfo:store_settings_weixin.usinfo,access_token:store_settings_weixin.access_token,systime:store_settings_weixin.systime,status:store_settings_weixin.status,keyword:store_rules_smsformat.keyword,caExport:store_rules_smsformat.caExport};
						//store_settings_weixin.currentPage=1;
						store_settings_weixin.load();
		    }
		
		    var statusStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'启用', groupid:'0'},
		        		{groupname:'停用', groupid:'1'}
		        ]
				});
				
				var usinfoStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'No', groupid:'0'},
		        		{groupname:'Yes', groupid:'1'}
		        ]
				});
				
				//高级查询窗口
				function gotoedit1(value, id){
						var win3 = null;
						var name = '';
						var token = '';
						var account = '';
						var appid = '';
						var appsecret = '';
						var sendcont = '';
						var redurl = '';
						var usinfo = 'Yes';
						var status = '0';
						var wid1 = '';
						var title='增加';
						if(value == 'update'){
								title='修改';
								name = store_settings_weixin.getAt(id).get('name');
								token = store_settings_weixin.getAt(id).get('token');
								account = store_settings_weixin.getAt(id).get('account');
								appid = store_settings_weixin.getAt(id).get('appid');
								appsecret = store_settings_weixin.getAt(id).get('appsecret');
								sendcont = store_settings_weixin.getAt(id).get('sendcont');
								redurl = store_settings_weixin.getAt(id).get('redurl');
								usinfo = store_settings_weixin.getAt(id).get('usinfo');
								status = store_settings_weixin.getAt(id).get('status');
								wid1 = store_settings_weixin.getAt(id).get('wid');
						}
						
				    if (!win3) //判断如果不存在就创建新的
				    {
				        win3 = new Ext.Window({
								    title:'微信设置窗口 - '+ title,
								    closeAction: 'hide',
								    layout:'form',  
								    width:360,
								    draggable:true, //可拖动的
										maximizable:true, //可最大化的
							      //resizable: true, //可改变大小
										modal: false,//后面的内容可以操作
							      //plain：true,//则主体背景透明
							      //items: fp2
								    constrain:true, //限制窗口只能在其容器内移动
							      //minimizable:true, //可最小化
							      
				            items:[new Ext.FormPanel({
												id:'weixin_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
		                      	fieldLabel:'商家名称',
		                      	xtype: 'textfield',
														emptyText:'请输入商家名称',
														allowBlank:false,
														value:name,
						                name:'name'
						            },{
		                      	fieldLabel:'微信账号',
		                      	xtype: 'textfield',
														emptyText:'请输入微信账号',
														allowBlank:false,
														value:account,
						                name:'account'
						            },{
		                      	fieldLabel:'Token',
		                      	xtype: 'textfield',
														emptyText:'请输入接入Token',
														allowBlank:false,
														value:token,
						                name:'token'
						            },{
		                      	fieldLabel:'发送信息',
		                      	xtype: 'textfield',
														emptyText:'请输入发送信息',
														allowBlank:false,
														value:sendcont,
						                name:'sendcont'
						            },{
		                      	fieldLabel:'重定向URL',
		                      	xtype: 'textfield',
														emptyText:'请输入重定向URL',
														allowBlank:false,
														value:redurl,
						                name:'redurl'
						            },{
		                      	fieldLabel:'appid',
		                      	xtype: 'textfield',
														emptyText:'请输入appid',
														//allowBlank:false,
														value:appid,
						                name:'appid'
						            },{
		                      	fieldLabel:'appsecret',
		                      	xtype: 'textfield',
														emptyText:'请输入appsecret',
														//allowBlank:false,
														value:appsecret,
						                name:'appsecret'
						            },{
						                xtype:'combo',
														name:'usinfo',
														fieldLabel:'获取用户信息',
														emptyText:'请选择获取用户信息',
														value:usinfo,
														triggerAction:'all',
														store:usinfoStore,
														displayField:'groupname',
														valueField:'groupname',
														queryMode:'local',
														forceSelection:false,
														typeAhead:true,
														allowBlank:true
						            },{
						                xtype:'combo',
														name:'status',
														fieldLabel:'状态',
														emptyText:'请选择状态',
														value:status,
														triggerAction:'all',
														store:statusStore,
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
							        	var objectTmp = Ext.getCmp("weixin_form2").getForm().getValues();
												var name1 = objectTmp.name;
                        if(name1==''){
                        		alert('商家名称不能为空！');
                        }
                        var account1 = objectTmp.account;
												if(account1==''){
														alert('微信账号不能为空！');
												}
												var token1 = objectTmp.token;
												if(token1==''){
														alert('Token不能为空！');
												}
												var sendcont1 = objectTmp.sendcont;
												if(sendcont1==''){
														sendcont1="我要上网";
												}
												var redurl1 = objectTmp.redurl;
												if(redurl1==''){
														alert('重定向URL不能为空！');
												}
												var appid1 = objectTmp.appid;
												var appsecret1 = objectTmp.appsecret;
												var usinfo1 = objectTmp.usinfo;
												var status1 = objectTmp.status;
												
												store_settings_weixin.load({params: {update:value,wid:wid1,name:name1,token:token1,account:account1,appid:appid1,appsecret:appsecret1,sendcont:sendcont1,redurl:redurl1,usinfo:usinfo1,status:status1}});
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
		
			  var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('weixin_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });	
				
		    Ext.apply(this, {
		        border:false,
						frame:false,
						//height: Ext.get("layout_center").getHeight()-58,
						//height:Ext.get("layout_center").getHeight(),
						autoScroll: true,
						selModel: sm,
						//height: grid_height,
		        store: store_settings_weixin,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "序号",
		            dataIndex: 'wid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'wid',
		            iconCls: 'report_edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            text: "商家名称",
		            dataIndex: 'name',
		            width: 80,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "微信账号",
		            dataIndex: 'account',
		            width: 120,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "Token",
		            dataIndex: 'token',
		            width: 80,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "发送信息",
		            dataIndex: 'sendcont',
		            width: 80,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "重定向URL",
		            dataIndex: 'redurl',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "appid",
		            dataIndex: 'appid',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "appsecret",
		            dataIndex: 'appsecret',
		            width: 240,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "用户信息",
		            dataIndex: 'usinfo',
		            width: 60,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "通行证",
		            dataIndex: 'access_token',
		            width: 100,
		            //align: 'right',
		            hidden: true,
		            sortable: false
		        },{
		            text: "加入时间",
		            dataIndex: 'systime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "状态",
		            dataIndex: 'status',
		            width: 60,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            renderer:function(value, metaData, record){
		            		return record.get('s_status'); 
		            }
		        },{
		        	 flex: 1	
		        }],
		        listeners:{
								'itemdblclick':function(grid,record,item,index,e){
										gotoedit1('update', index);
								}
						},  
		        columnLines: true,
		        tbar:{items:[
		            '-',{
			            	labelWidth: 45,
			            	width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'weixin_keyword',
		                name:'weixin_keyword',
						        emptyText:'请输入微信账号',
		                value:store_settings_weixin.keyword,
		                enableKeyEvents:true,
		                listeners:{
			                'focus':function(){
					                 if(this.value=='请输入微信账号'){                
					                    this.setValue('');
					                }
											},
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('weixin_keyword').value;
							       	    		store_settings_weixin.keyword=aa;
													    store_settings_weixin.currentPage=1;
													    reflash();
			                    }
			                }
		                }
		            }, '-', {
		                text:'查询',
		            		iconCls:'accept',
		                handler:function(){
			                 var bb=Ext.getCmp('weixin_keyword').value;
				               store_settings_weixin.keyword=bb;
				               store_settings_weixin.currentPage=1;         
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
						        id:'weixin_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('wid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('wid') + "'";
					                    }
					                }
									        store_settings_weixin.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            },'-', {
		                text:'刷新',
						        id:'weixin_refreshButton',
		                iconCls:'accept',
		                disabled: false,
		                handler:function(){
								       store_settings_weixin.load({params: {refresh: "refresh"}});
		                   
		                }
		            }
		        ]}
		    });
			  store_settings_weixin.alreadyload = 1;
		    this.callParent(arguments);
    }
});