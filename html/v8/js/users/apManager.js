/*AP管理*/
Ext.define('users.apManager', {
		extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
    		if(store_users_apmanager.alreadyload !=1){
    				store_users_apmanager.gwid = '';
    				store_users_apmanager.address = '';
    				store_users_apmanager.longitude = '';
    				store_users_apmanager.latitude = '';
    				store_users_apmanager.addtime = '';
    				store_users_apmanager.flags = '';
    				store_users_apmanager.s_flags = '';
    				store_users_apmanager.keyword = '';
						store_users_apmanager.caExport = '';
    				store_users_apmanager.new_params = {gwid:store_users_apmanager.gwid,address:store_users_apmanager.address,longitude:store_users_apmanager.longitude,latitude:store_users_apmanager.latitude,addtime:store_users_apmanager.addtime,flags:store_users_apmanager.flags,s_flags:store_users_apmanager.s_flags,keyword:store_users_apmanager.keyword,caExport:store_users_apmanager.caExport};
    		}
    		
		    function reflash(){
		    		store_users_apmanager.new_params = {groupid:store_users_apmanager.groupid,gwid:store_users_apmanager.gwid,address:store_users_apmanager.address,longitude:store_users_apmanager.longitude,latitude:store_users_apmanager.latitude,addtime:store_users_apmanager.addtime,flags:store_users_apmanager.flags,s_flags:store_users_apmanager.s_flags,keyword:store_users_apmanager.keyword,caExport:store_users_apmanager.caExport};
		    		//store_users_apmanager.currentPage=1;
			      store_users_apmanager.load();	
		    }
		    
		    var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('apmanager_removeButton').setDisabled(selections.length == 0);
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
		    
		    // 高级查询窗口
		    function gotoedit1(value, id){
		    		var win3 = null;
		    		var groupid1 = store_users_apmanager.groupid;
		    		var gwid = '';
		    		var address = '';
		    		var longitude = '';
		    		var latitude = '';
		    		var flags = '1';
		    			
		    		if(value == 'update'){
		    				gwid = store_users_apmanager.getAt(id).get('gwid');
		    				address = store_users_apmanager.getAt(id).get('address');
		    				longitude = store_users_apmanager.getAt(id).get('longitude');
		    				latitude = store_users_apmanager.getAt(id).get('latitude');
		    				flags = store_users_apmanager.getAt(id).get('flags');
		    				groupid1 = store_users_apmanager.getAt(id).get('groupid');
		    		}
		    		
		    		if(!win3){ //判断如果不存在就创建新的
		    			  win3 = new Ext.Window({
		    			  		title:'AP管理',
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
						      			id:'apmanager_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
			                      fieldLabel:'接入设备ID',
			                      xtype: 'textfield',
														emptyText:'请输入接入设备ID',
														value:gwid,
			                      name:'gwid',
			                      allowBlank:false,
			                      listeners:{
			                      		'focus':function(){
			                      				if(this.value=="请输入接入设备ID"){
			                      						this.setValue('');	
			                      				}	
			                      		}	
			                      }
			                  },{
			                      fieldLabel:'安装地址',
			                      xtype: 'textfield',
														emptyText:'请输入安装地址',
														value:address,
			                      name:'address',
			                      listeners:{
			                      		'focus':function(){
			                      				if(this.value=="请输入安装地址"){
			                      						this.setValue('');	
			                      				}	
			                      		}	
			                      }
			                  },{
			                      fieldLabel:'经度',
			                      xtype: 'textfield',
														emptyText:'请输入经度',
														value:longitude,
			                      name:'longitude',
			                      listeners:{
			                      		'focus':function(){
			                      				if(this.value=="请输入经度"){
			                      						this.setValue('');	
			                      				}	
			                      		}	
			                      }
			                  },{
			                      fieldLabel:'纬度',
			                      xtype: 'textfield',
														emptyText:'请输入纬度',
														value:latitude,
			                      name:'latitude',
			                      listeners:{
			                      		'focus':function(){
			                      				if(this.value=="请输入纬度"){
			                      						this.setValue('');	
			                      				}	
			                      		}	
			                      }
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
								        			var objectTmp = Ext.getCmp("apmanager_form2").getForm().getValues();
															var gwid1 = objectTmp.gwid;
															if(gwid1==''){
																	alert("接入设备不能为空!");
																	return ;	
															}
															var address1 = objectTmp.address;
															var longitude1 = objectTmp.longitude;
															var latitude1 = objectTmp.latitude;
															var flags1 = objectTmp.flags;
															store_users_apmanager.load({params: {update:value,pregwid:gwid,groupid:groupid1,gwid:gwid1,address:address1,longitude:longitude1,latitude:latitude1,flags:flags1}});
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
		        store: store_users_apmanager,
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
		            iconCls: 'report_edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            text: "接入设备ID",
		            dataIndex: 'gwid',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "地址",
		            dataIndex: 'address',
		            width: 100,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "经度",
		            dataIndex: 'longitude',
		            width: 100,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "纬度",
		            dataIndex: 'latitude',
		            width: 100,
		            //align: 'right',
		            hidden: false,
		            sortable: false
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
		            dataIndex: 'addtime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        tbar:Ext.create('Ext.PagingToolbar', {
		            store: store_users_apmanager,
		            displayInfo: true,
		            displayMsg: '显示 <font color=red>{0}</font> - <font color=red>{1}</font> 条，  共 <font color=red>{2}</font> 条',
		            emptyMsg: "共 0 条",
		            plugins: Ext.create('Ext.ux.ProgressBarPager', {}),
		            
		        	  items:[
		        		'-',{
		            		//labelWidth: 45,
		            		width:160,
		                //fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'apmanager_keyword',
		                name:'apmanager_keyword',
										emptyText:'请输入接入设备ID',
		                value:store_users_apmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入接入设备ID'){                
						                   this.setValue('');
						                }
										    },
			                  'keydown' : function(i,e){
				                  	var aae=e.getKey(); 
				                    if(aae==13){
						                  	var aa=Ext.getCmp('apmanager_keyword').value;
									       	    	store_users_apmanager.keyword=aa;
											    			store_users_apmanager.currentPage=1;
											    			reflash();
				                    }
			                  }
		                }
		            },'-',{
		                text:'查询',
		            	  iconCls:'search2',
		                handler:function(){
			                 var bb=Ext.getCmp('apmanager_keyword').value;
				               store_users_apmanager.keyword=bb;
				               store_users_apmanager.currentPage=1;         
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
										id:'apmanager_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('gwid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('gwid') + "'";
					                    }
					                }
													store_users_apmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            },'-',{
		                text:'应用',
						        id:'apmanager_refreshButton',
		                iconCls:'accept',
		                disabled: false,
		                handler:function(){
								       store_users_apmanager.load({params: {refresh: "refresh"}});
		                }
		            }]
		        })	
		    });
		    reflash();
		    store_users_apmanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});