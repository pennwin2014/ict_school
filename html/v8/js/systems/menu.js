/*系统主菜单管理*/
Ext.define('systems.menu',{
		extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
				if(store_systems_menu.alreadyload !=1){
						store_systems_menu.id = '';
				    store_systems_menu.mainmenu = '';
				    store_systems_menu.systime = '';
				    store_systems_menu.flag = '';
				    store_systems_menu.keyword = '';
				    store_systems_menu.caExport = ''; //导出
					
						store_systems_menu.new_params={id:store_systems_menu.id,mainmenu:store_systems_menu.mainmenu,systime:store_systems_menu.systime,flag:store_systems_menu.flag,keyword:store_systems_menu.keyword,caExport:store_systems_menu.caExport};
				}
				
				function reflash(){
						store_systems_menu.new_params={id:store_systems_menu.id,mainmenu:store_systems_menu.mainmenu,systime:store_systems_menu.systime,flag:store_systems_menu.flag,keyword:store_systems_menu.keyword,caExport:store_systems_menu.caExport};
						//store_systems_menu.currentPage=1;
						store_systems_menu.load();
				}
		
		 		var flagStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
		        data:[
		        		{groupname:'使用', groupid:'0'},
		        		{groupname:'不使用', groupid:'1'}
		        ]
				});
				
			  //高级查询窗口
			  function gotoedit1(value, id){
						var win3 = null;
						var mainmenu = '';
						var flag = '0';
						var id1 = '';
						var title='增加';
						
						if(value == 'update'){
								title='修改';
								mainmenu = store_systems_menu.getAt(id).get('mainmenu');
								flag = store_systems_menu.getAt(id).get('flag');
								id1 = store_systems_menu.getAt(id).get('id');
						}
						
				    if (!win3) { //判断如果不存在就创建新的
				        win3 = new Ext.Window({
								    title:'系统主菜单管理窗口 - '+ title,
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
												id:'menu_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
				                  	fieldLabel:'主菜单名称',
				                  	xtype: 'textfield',
														emptyText:'请输入主菜单名称',
														value:mainmenu,
						                name:'mainmenu'
						            },{
						                xtype:'combo',
														name:'flag',
														fieldLabel:'标记',
														emptyText:'请选择标记',
														value:flag,
														triggerAction:'all',
														store:flagStore,
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
							        			var objectTmp = Ext.getCmp("menu_form2").getForm().getValues();
														var mainmenu1 = objectTmp.mainmenu;
														var flag1 = objectTmp.flag;
														
														store_systems_menu.load({params: {update:value,id:id1,mainmenu:mainmenu1,flag:flag1}});
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
		                Ext.getCmp('menu_removeButton').setDisabled(selections.length == 0);
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
		        store: store_systems_menu,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "序号",
		            dataIndex: 'id',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'id',
		            iconCls: 'report_edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            text: "主菜单名称",
		            dataIndex: 'mainmenu',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "操作时间",
		            dataIndex: 'systime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: " 标记",
		            dataIndex: 's_flag',
		            width: 60,
		            //align: 'center',
		            hidden: false,
		            sortable: false
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
		                id:'menu_keyword',
		                name:'menu_keyword',
										emptyText:'请输入主菜单名称',
		                value:store_systems_menu.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入主菜单名称'){                
						                    this.setValue('');
						                }
												},
				                'keydown' : function(i,e){
				                    var aae=e.getKey(); 
				                    if(aae==13){
				                    		var aa=Ext.getCmp('menu_keyword').value;
								       	    		store_systems_menu.keyword=aa;
														    store_systems_menu.currentPage=1;
														    reflash();
				                    }
				                }
		                }
		            }, '-', {
		                text:'查询',
		            	  iconCls:'accept',
		                handler:function(){
			                 var bb=Ext.getCmp('menu_keyword').value;
				               store_systems_menu.keyword=bb;
				               store_systems_menu.currentPage=1;         
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
										id:'menu_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
										if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('id') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('id') + "'";
					                    }
					                }
													store_systems_menu.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }
		        ]}
		    });
			  store_systems_menu.alreadyload = 1;
		    this.callParent(arguments);
    }
})

