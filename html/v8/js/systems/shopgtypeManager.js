/*单位组管理*/
Ext.define('systems.shopgtypeManager',{
    extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
		if(store_systems_shopgtypemanager.alreadyload !=1){
			store_systems_shopgtypemanager.groupcode = '';
		    store_systems_shopgtypemanager.groupname = '';
		    store_systems_shopgtypemanager.id = '';
		    store_systems_shopgtypemanager.keyword = '';
		    store_systems_shopgtypemanager.type = '';
	
			store_systems_shopgtypemanager.new_params={groupcode:store_systems_shopgtypemanager.groupcode,groupname:store_systems_shopgtypemanager.groupname,id:store_systems_shopgtypemanager.id,keyword:store_systems_shopgtypemanager.keyword};
   
		}
		function reflash(){
			store_systems_shopgtypemanager.new_params={groupcode:store_systems_shopgtypemanager.groupcode,groupname:store_systems_shopgtypemanager.groupname,type:store_systems_shopgtypemanager.type,id:store_systems_shopgtypemanager.id,keyword:store_systems_shopgtypemanager.keyword};
			store_systems_shopgtypemanager.currentPage=1;
			store_systems_shopgtypemanager.load();
		
		}
		Ext.define('CheckData', {
			  extend: 'Ext.data.Model',
			  fields: [
			      'vara', 'checked1', 'rolename','roldispname','userid','dispname'
			  ],
			  idProperty: 'rolename'
		});
		Ext.define('CheckData2', {
			  extend: 'Ext.data.Model',
			  fields: [
			      'id', 'groupcode', 'menuname','submenu','systime','flag'
			  ],
			  idProperty: 'rolename'
		});
		//
		var store_role = Ext.create('Ext.data.Store', {
		    pageSize: 1000,
		    model: 'CheckData2',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url: '/pronline/Msg?FunName@ncmdeptmenu',
		        reader: {
		            type:'json',
		            root: 'eimdata',
		            totalProperty: 'totalCount'
		        },         
		        simpleSortMode: true
		    },
		    sorters: [{
		        property: 'id',
		        direction: 'DESC'
		    }]
		});
		Ext.define('menu', {
			  extend: 'Ext.data.Model',
			  fields: [
			      'id', 'mainmenu'
			  ],
			  idProperty: 'rolename'
		});
		//
		var store_menu = Ext.create('Ext.data.Store', {
		    pageSize: 1000,
		    model: 'menu',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url: '/pronline/Msg?FunName@ncm_initmenu',
		        reader: {
		            type:'json',
		            root: 'eimdata',
		            totalProperty: 'totalCount'
		        },         
		        simpleSortMode: true
		    },
		    sorters: [{
		        property: 'id',
		        direction: 'DESC'
		    }]
		});
		store_menu.load();
		Ext.define('submenu', {
			  extend: 'Ext.data.Model',
			  fields: [
			      'id', 'submenu', 'pid'
			  ],
			  idProperty: 'rolename'
		});
		//
		var store_submenu = Ext.create('Ext.data.Store', {
		    pageSize: 1000,
		    model: 'submenu',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url: '/pronline/Msg?FunName@ncm_initsubmenu',
		        reader: {
		            type:'json',
		            root: 'eimdata',
		            totalProperty: 'totalCount'
		        },         
		        simpleSortMode: true
		    },
		    sorters: [{
		        property: 'id',
		        direction: 'DESC'
		    }]
		});
		store_submenu.load();
	//取权限check数据
		var store_right = Ext.create('Ext.data.Store', {
		    pageSize: 1000,
		    model: 'CheckData',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url: '/pronline/Msg?FunName@ncmDispRoleFunlist_v5',
		        reader: {
		            type:'json',
		            root: 'eimdata',
		            totalProperty: 'totalCount'
		        },         
		        simpleSortMode: true
		    },
		    sorters: [{
		        property: 'userid',
		        direction: 'DESC'
		    }]
		});
		store_right.load();
		//alert(store_right.getAt(0).get('rolename'));
		function getCombData(record) { 
				
				var temp_check='';
				var temp_check_sub='';
			  if(record!=''){
				//alert(store_role.getTotalCount())
				for(i=0; i<store_role.getTotalCount();i++){
				if(i==0){
					temp_check=store_role.getAt(i).get('menuname');
					temp_check_sub=store_role.getAt(i).get('submenu');
				}
				else{
					temp_check=temp_check+","+store_role.getAt(i).get('menuname');
					temp_check_sub=temp_check_sub+","+store_role.getAt(i).get('submenu');
				}
				}
			   // temp_check=store_role.getAt(0).get('menuname');
				//alert(temp_check)
			  }
		
				var dataStr = "["; 
				var i=0;
		    
		    var bCheck;
		    var br;
				//alert(111)
				//alert(store_right.getAt(0).get('roldispname'))
				store_menu.each(function(item2){
				     bCheck="false";
				     if(temp_check.indexOf(item2.get('mainmenu'))!=-1){
				         bCheck="true";
				     }
					 if(i==0){
				       	 dataStr=dataStr+"{	style: 'margin-top:10',xtype:'fieldset',title:'',collapsible: false,colspan: 1,items:[{xtype: 'checkboxgroup',id:'mainmenu"+item2.get('id')+"', width:800,columns: 5,items:[{boxLabel:'【主菜单】: "+item2.get('mainmenu')+"' ,inputValue:'" +item2.get('mainmenu')+"',checked:"+bCheck+",name:'rb'}";
				     }else{
				       	 dataStr=dataStr+",{	xtype:'fieldset',title:'',collapsible: false,colspan: 1,items:[{xtype: 'checkboxgroup',id:'mainmenu"+item2.get('id')+"',width:800,columns: 5,items:[{boxLabel:'【主菜单】: "+item2.get('mainmenu')+"' ,inputValue:'" +item2.get('mainmenu')+"',checked:"+bCheck+",name:'rb'}";
				     }
					 i++;
				store_submenu.each(function(item){
					if(item2.get('id') == item.get('pid')){
				     bCheck="false";
				     if(temp_check_sub.indexOf(item.get('submenu'))!=-1){
				         bCheck="true";
				     }
				  
			       if(i==0){
				       	 dataStr=dataStr+"{boxLabel: '"+item.get('submenu')+"' ,inputValue:'" +item.get('submenu')+"',checked:"+bCheck+",name:'rb'}";
				     }else{
				       	 dataStr=dataStr+",{boxLabel: '"+item.get('submenu')+"' ,inputValue:'" +item.get('submenu')+"',checked:"+bCheck+",name:'rb'}";
				     }
				     i++;
					 }
				});
					dataStr=dataStr+"]}";
					dataStr=dataStr+"]}";
				});
		    dataStr=dataStr+"]";
				return eval(dataStr); 
		};
		function getRolname(record){
		     if(record!=''){
		      	return record.data.groupname; 
		     }else{
		       	return '';
		     }
		}
		//获取系统用户信息
		function getCombAdminuserData(record) { 
				  var temp_check='';
			     if(record!=''){
			     temp_check=record.data.dx;
			     
			     }
		
						var dataStr = "["; 
				    var i=0;
		     
		          var bCheck;
				      store_adminuser.each(function(item){
				       bCheck="false";
				       if(temp_check.indexOf(item.get('dispname'))!=-1){
				       bCheck="true";
				       }
				      
		
			       if(i==0){
				       	 dataStr=dataStr+"{boxLabel: '"+item.get('dispname')+"' ,inputValue:'" +item.get('userid')+"',checked:"+bCheck+"}";
				       }
				       else{
				       	 dataStr=dataStr+",{boxLabel: '"+item.get('dispname')+"' ,inputValue:'" +item.get('userid')+"',checked:"+bCheck+"}";
				       }
		
		
				       i++;
				
				    });
		        dataStr=dataStr+"]";
		       
					return eval(dataStr); 
		     
		};
		
		function gotoedit2(id,record,title){
			store_role.load({params: {groupcode:id},callback:function(records, options, success){					
		     var rightcheckGroup={
		          xtype:'fieldset',
		          title:'功能权限',
		         
				  collapsible: true,
				  colspan: 1,
		        	defaults: {
		            labelStyle: 'padding-left:4px;'
		        	},
		          collapsible: false,
		          items: getCombData(record)
		     }; 
		             
		     var fp2=Ext.create('Ext.FormPanel', {
		        frame: true,
		        fieldDefaults: {
		            labelWidth: 250
		        },
		        width: 800,
		        bodyPadding: 10,
				baseCls:"x-plain",
		        items: [{
		            xtype: 'textfield',
		            name: 'rolname',
		            fieldLabel: '类别/商超名称',
					readOnly:true,
		            value:getRolname(record)
		        },
		        rightcheckGroup
		        ]
		     });
		    
		    
		     var win2 = new Ext.Window ({
			         id:"addwindow",
			         title:"角色权限管理窗口 - "+title,
					  height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
			         y:0,
			         width:850,
					 autoScroll:true,
			         //height:600,
			         bodyPadding: 10,
			         modal:true,
			         resizable:true,
			         //draggable:false,
			         items: fp2,
		        buttons: [{
		            text: '全选',
		            handler: function(){
							store_menu.each(function(item2){
								Ext.getCmp('mainmenu'+item2.get('id')).items.each(function(item){  
		                            item.setValue(true);  
		                        });  
							});
		            }
					},{
		            text: '保存',
					 iconCls:'icon-save',		 
		            handler: function(){
					
						store_role.load({params: {update:"update",groupcode:id},callback:function(records, options, success){
				            store_menu.each(function(item2){
							var groupcheck=fp2.getForm().findField('mainmenu'+item2.get('id'));
							//alert(groupcheck);
				            var checkvalue=groupcheck.getValue();
				            var rolvalue=fp2.getForm().getValues().rolname;
							var menuname = item2.get('mainmenu');
							//alert(checkvalue);
							if(checkvalue.indexOf(item2.get('mainmenu'))!=-1)
							store_role.load({params:{update:"add",groupcode:id,menuname:menuname,submenu:checkvalue,rolvalue:rolvalue}});
							});
				           // store_role.load({params:{update:"update",groupcode:id,menuname:checkvalue,rolvalue:rolvalue}});
				            Ext.getCmp('addwindow').close();
						}});
						
		            }
		        },{
		            text: '关闭',
		            handler: function(){
		            		Ext.getCmp('addwindow').close();
		            }
		        }]               
		         }
		     )
		     win2.show();
			 }
			});
		};

		var typeStore = Ext.create('Ext.data.Store', {
		    fields:['groupname', 'groupid'],
		    data:[
		    		{groupname:'类别编码', groupid:'1'},
		    		{groupname:'商超编码', groupid:'2'}
		    ]
		});

		
		function showDetail(value, cellmeta, record, rowIndex, colIndex, store){
				//alert(store.name);
				if(value == 1){
						return Ext.String.format('{0}',"类别编码");
				}
				if(value == 2){
						return Ext.String.format('{0}',"商超编码");
				}
				return ;	
		}
		function showDetail2(value, cellmeta, record, rowIndex, colIndex, store){
				//alert(record);
				if(record.get('type') == 2){
						return 'unlock';
				}
				if(record.get('type') == 1){
						return 'lock';
				}
				return ;	
		}
		 //高级查询窗口
		function gotoedit1(value, id){
				var win3 = null;
				var groupname = '';
				var title='增加';
				var type='';
				var id1='';
				var autodef=true;
				var typeflag=false;
			
				if(value == 'update'){
						title='修改';
						groupname = store_systems_shopgtypemanager.getAt(id).get('groupname');
						type = store_systems_shopgtypemanager.getAt(id).get('type');
						id1 = store_systems_shopgtypemanager.getAt(id).get('id');
						autodef = store_systems_shopgtypemanager.getAt(id).get('autodef');
						if(autodef == 1){
								autodef=true;
						}else{
								autodef=false;
						}
						typeflag=true;
				}
			
		    if (!win3) //判断如果不存在就创建新的
		    {
		        win3 = new Ext.Window({
						    title:'商家联盟设置窗口 - '+ title,
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
										id:'shopgtypemanager_form2',
										layout:"form",
										baseCls:"x-plain",
										bodyPadding: 5,
										items:[{
                      	fieldLabel:'类别/商超名称',
                      	xtype: 'textfield',
												emptyText:'请输入类别/商超名称',
												value:groupname,
					              name:'groupname'
						        },{
												xtype:'combo',
												fieldLabel: '编码类别',
												valueField: 'groupid',
												displayField: 'groupname',
											  value:type,
												emptyText:'请选择编码类别',   
												store: typeStore,
												forceSelection: true,
												triggerAction: 'all',
												afterLabelTextTpl: required,
												readOnly:typeflag,
												//allowBlank:false, //是否允许为空
												//blankText:'省份不能为空！',	
												msgTarget:'qtip', //显示一个浮动的提示信息 	
											  // id:'prov',
												name:'type'
										},{
												xtype: 'checkboxgroup',
												name:'autodef',
												columns: 4,
												items: [{
														boxLabel: '自定义' ,
														inputValue:'1',
														checked:autodef
												}]
										}]
		            })],  
				        buttons:[{
									  text:"确定",
									  iconCls:'icon-save',		  
									  handler:function(){
					        			var objectTmp = Ext.getCmp("shopgtypemanager_form2").getForm().getValues();
												var groupname1 = objectTmp.groupname;
												var type1 ="";
												type1 = objectTmp.type;
												var autodef1 = 0;
												autodef1 = Ext.getCmp("shopgtypemanager_form2").getForm().findField('autodef').getValue();
												//alert(autodef1)
												if(type1==""){
														alert("请选择编码类别")
												}else{
														store_systems_shopgtypemanager.load({params: {update:value,groupname:groupname1,id:id1,type:type1,autodef:autodef1},callback:function(records, options, success){
																reflash();
														}});
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

		var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {
              //  Ext.getCmp('shopgtypemanager_removeButton').setDisabled(selections.length == 0);
            }
        }
    });	
		
    Ext.apply(this, {
        border:false,
				frame:false,
				height: Ext.get("layout_center").getHeight()-63,
				//height:Ext.get("layout_center").getHeight(),
				//autoScroll: true,
				selModel: sm,
				//height: grid_height,
        store:store_systems_shopgtypemanager,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'id',
						sortable: false,
            width: 50,
            align: 'center',
						items:[{
								iconCls: 'report_edit',
								tooltip: '点击进入修改界面',
			          handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
			              gotoedit1('update', rowIndex);
			          }
			      }]
        },/*{
			header: '菜单权限编辑',
            xtype: 'actioncolumn',
            dataIndex: 'groupcode',
            icon: '/newver/resources/themes/images/share/computer_edit.png',
            tooltip: '点击进入修改菜单权限编辑界面',
            width: 100,
            align: 'center',
            sortable: false,
			//renderer: showDetail2,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
               if(record.get('type')==2){gotoedit2(record.get('groupcode'),record,'修改');}else{alert("只有商超类别才能编辑权限")}
            }
        },*/{
            text: "类别/商超名称",
            dataIndex: 'groupname',
            width: 250,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "组编码",
            dataIndex: 'groupcode',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "类别",
            dataIndex: 'type',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true,
						renderer: showDetail
        },{
            text: "加入时间",
            dataIndex: 'systime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
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
                id:'shopgtypemanager_keyword',
                name:'shopgtypemanager_keyword',
				emptyText:'请输商家联盟名称',
                value:store_systems_shopgtypemanager.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输商家联盟名称'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('shopgtypemanager_keyword').value;
					       	            store_systems_shopgtypemanager.keyword=aa;
							            store_systems_shopgtypemanager.currentPage=1;
							            reflash();
	                    }
	                }
                }
            }, '-',{
						xtype:'combo',
						valueField: 'groupid',
						displayField: 'groupname',
						emptyText:'请选择编码类别',   
						store: typeStore,
						forceSelection: true,
						triggerAction: 'all',
						afterLabelTextTpl: required,
						//allowBlank:false, //是否允许为空
						//blankText:'省份不能为空！',	
						msgTarget:'qtip', //显示一个浮动的提示信息 	
					  id:'shopgtypemanager_typesearch'
					},'-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('shopgtypemanager_keyword').value;
		               store_systems_shopgtypemanager.keyword=bb;
		               store_systems_shopgtypemanager.currentPage=1; 
						store_systems_shopgtypemanager.type=Ext.getCmp('shopgtypemanager_typesearch').value;		   
		               reflash();  
                }
            },'-',{
                 text:'增加',
                 iconCls:'add',
	             handler : function() {
	               	  gotoedit1('add', '');
	               }
            }
          ]
         }
        }
        
        );
	store_systems_shopgtypemanager.alreadyload = 1;
    this.callParent(arguments);
    }
})