/*用户MAC管理*/
Ext.define('users.usermacManager', {
		extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
    		if(store_users_usermacmanager.alreadyload !=1){
    				store_users_usermacmanager.usermac = '';
    				store_users_usermacmanager.groupid = '';
    				store_users_usermacmanager.flags = '';
    				store_users_groupmanager.keyword = '';
    				store_users_usermacmanager.new_params = {usermac:store_users_usermacmanager.usermac,groupid:store_users_usermacmanager.groupid,flags:store_users_usermacmanager.flags,keyword:store_users_usermacmanager.keyword};
    		}
    		
		    function reflash(){
		    		store_users_usermacmanager.new_params = {usermac:store_users_usermacmanager.usermac,username:store_users_usermacmanager.username,groupid:store_users_usermacmanager.groupid,flags:store_users_usermacmanager.flags,keyword:store_users_usermacmanager.keyword};
		    		//store_users_usermacmanager.currentPage=1;
			      store_users_usermacmanager.load();	
		    }
		    
		    var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('usermacmanager_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });	
		    
		    var flagsStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'有效',   groupid:'0'},
		        		{groupname:'无效',   groupid:'9'}
		        ]
				});
		    
		    // 高级查询窗口
		    function gotoedit1(value, id){
		    		var win3 = null;
		    		var preusermac = '';
		    		var username1 = store_users_usermacmanager.username;
		    		var flags = '0';
		    		if(value == 'update'){
		    				preusermac = store_users_usermacmanager.getAt(id).get('usermac');
		    				username1 = store_users_usermacmanager.getAt(id).get('username');
		    				flags = store_users_usermacmanager.getAt(id).get('flags');
		    		}
		    		if(!win3){ //判断如果不存在就创建新的
		    			  win3 = new Ext.Window({
		    			  		title:'用户MAC管理',
							    	closeAction: 'hide',
							    	layout:'form',  
							    	width:300,
							   	  draggable:true, //可拖动的
										maximizable:true, //可最大化的
						      	//resizable: true, //可改变大小
										modal: false,//后面的内容可以操作
						      	//plain：true,//则主体背景透明
						      	//items: fp2,
							    	constrain:true, //限制窗口只能在其容器内移动
						      	//minimizable:true, //可最小化	
						      	
						      	items:[new Ext.FormPanel({
						      			id:'usermacmanager_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
			                      fieldLabel:'接入用户MAC',
			                      xtype: 'textfield',
														emptyText:'接入用户MAC',
														value:preusermac,
			                      name:'usermac'
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
								        			var objectTmp = Ext.getCmp("usermacmanager_form2").getForm().getValues();
															var usermac1 = objectTmp.usermac;
															var flags1 = objectTmp.flags;
															store_users_usermacmanager.load({
																	params: {update:value,preusermac:preusermac,usermac:usermac1,username:username1,flags:flags1}
															});
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
						autoScroll: true,
						selModel: sm,
		        store:store_users_usermacmanager,
		        viewConfig:{
			        	loadMask : false
			      },
			      columns:[{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'groupid',
		            iconCls: 'report_edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            text: "用户MAC",
		            dataIndex: 'usermac',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "开始时间",
		            dataIndex: 'starttime',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: " 标记",
		            dataIndex: 'flags',
		            width: 60,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            renderer:function(value, metaData, record){
		            		return record.get('s_flags'); 
		            }
		        },{
		        	 flex: 1	
		        }],
		        columnLines: true,
		        tbar: Ext.create('Ext.PagingToolbar', {
		            store: store_users_usermacmanager,
		            displayInfo: true,
		            displayMsg: '显示 <font color=red>{0}</font> - <font color=red>{1}</font> 条，  共 <font color=red>{2}</font> 条',
		            emptyMsg: "共 0 条",
		            plugins: Ext.create('Ext.ux.ProgressBarPager', {}),
		            items:[
		        		'-',{
		            		//labelWidth: 40,
		            		width:160,
		                //fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'usermacmanager_keyword',
		                name:'usermacmanager_keyword',
										emptyText:'请输入用户MAC',
		                value:store_users_usermacmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入用户MAC'){                
						                   this.setValue('');
						                }
										    },
			                  'keydown' : function(i,e){
				                  	var aae=e.getKey(); 
				                    if(aae==13){
						                  	var aa=Ext.getCmp('usermacmanager_keyword').value;
									       	    	store_users_usermacmanager.keyword=aa;
											    			store_users_usermacmanager.currentPage=1;
											    			reflash();
				                    }
			                  }
		                }
		            },'-',{
		                text:'查询',
		            	  iconCls:'search2',
		                handler:function(){
			                 var bb=Ext.getCmp('usermacmanager_keyword').value;
				               store_users_usermacmanager.keyword=bb;
				               store_users_usermacmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               	  gotoedit1('add', '');
			               }
		            },'-',{
		                text:'删除',
										id:'usermacmanager_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('usermac') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('usermac') + "'";
					                    }
					                }
													store_users_usermacmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            },'-']
		        })	
		    });
		    reflash();
		    store_users_usermacmanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});
