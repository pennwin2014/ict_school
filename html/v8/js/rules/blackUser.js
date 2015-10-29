﻿/*黑名单设置*/
Ext.define('rules.blackUser',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
	  if(store_rules_blackuser.alreadyload !=1){
	  	  store_rules_blackuser.utype = ''; 
	  	  store_rules_blackuser.groupid = '';
		    store_rules_blackuser.name = '';
		    store_rules_blackuser.memo = '';
		    store_rules_blackuser.validtime = '';
		    store_rules_blackuser.keyword = '';
		    store_rules_blackuser.caExport = ''; //导出
		
			  store_rules_blackuser.new_params={groupid:store_rules_blackuser.groupid,name:store_rules_blackuser.name,memo:store_rules_blackuser.memo,validtime:store_rules_blackuser.validtime,utype:store_rules_blackuser.utype,keyword:store_rules_blackuser.keyword,caExport:store_rules_blackuser.caExport};
   
	  }
		function reflash(){
			store_rules_blackuser.new_params={groupid:store_rules_blackuser.groupid,name:store_rules_blackuser.name,memo:store_rules_blackuser.memo,validtime:store_rules_blackuser.validtime,utype:store_rules_blackuser.utype,keyword:store_rules_blackuser.keyword,caExport:store_rules_blackuser.caExport};
			//store_rules_blackuser.currentPage=1;
			store_rules_blackuser.load();
		}

    var typeStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'Mac', groupid:'1'},
        		{groupname:'Username', groupid:'2'}
        ]
		});
		
		//高级查询窗口
		function gotoedit1(value, id){
				var win3 = null;
				var name = '';
				var prename = '';
				var utype = '';
				var memo = '';
				var validtime = '';
				var title='增加';
				if(value == 'update'){
						title='修改';
						name = store_rules_blackuser.getAt(id).get('name');
						prename = store_rules_blackuser.getAt(id).get('name');
						utype = store_rules_blackuser.getAt(id).get('utype');
						memo = store_rules_blackuser.getAt(id).get('memo');
						validtime = store_rules_blackuser.getAt(id).get('validtime');
				}
			
		    if (!win3) //判断如果不存在就创建新的
		    {
		        win3 = new Ext.Window({
						    title:'黑名单设置窗口 - '+ title,
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
										id:'blackuser_form2',
										layout:"form",
										baseCls:"x-plain",
										bodyPadding: 5,
										items:[{
						              xtype:'combo',
													name:'utype',
													fieldLabel:'类别',
													emptyText:'请选择类别',
													value:utype,
													triggerAction:'all',
													store:typeStore,
													displayField:'groupname',
													valueField:'groupid',
													queryMode:'local',
													forceSelection:false,
													typeAhead:true,
													allowBlank:true
						        },{
				                	fieldLabel:'名称',
				                	xtype: 'textfield',
													emptyText:'请输入名称',
													value:name,
						              name:'name'
						        },{
													fieldLabel:'失效时间',
						              xtype:'datefield',
													emptyText:'请选择失效时间',
													name:'validtime',
													value:validtime,
													format:'Y/m/d'
						        },{
				                	fieldLabel:'说明',
				                	xtype: 'textfield',
													emptyText:'请输入说明',
													value:memo,
						              name:'memo'
						        }]  
		            })],  
				        buttons:[{
									  text:"确定",
									  iconCls:'icon-save',		  
									  handler:function(){
					        			var objectTmp = Ext.getCmp("blackuser_form2").getForm().getValues();
												var name1 = objectTmp.name;
												var utype1 = objectTmp.utype;
												var memo1 = objectTmp.memo;
												var validtime1 = objectTmp.validtime;
												
												store_rules_blackuser.load({params: {update:value,preid:prename,utype:utype1,name:name1,memo:memo1,validtime:validtime1}});
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
                Ext.getCmp('blackuser_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_blackuser,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            header: '编辑',
            xtype: 'actioncolumn',
            iconCls: 'report_edit',
            tooltip: '点击进入修改界面',
            width: 50,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('update', rowIndex);
            }
        },{
            text: "类别",
            dataIndex: 's_utype',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "名称",
            dataIndex: 'name',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
        	  text:'单位名称',
        	  dataIndex:'groupid',
        	  width:100,
        	  hidden:true,
        	  sortable:true
        },{
            text: "失效时间",
            dataIndex: 'validtime',
            width: 150,
            //align: 'left',
            hidden: false,
            sortable: false
        },{
            text: "说明",
            dataIndex: 'memo',
            width: 250,
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
                id:'blackuser_keyword',
                name:'blackuser_keyword',
				        emptyText:'请输入名称',
                value:store_rules_blackuser.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入名称'){                
			                    this.setValue('');
			                }
					        },
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('blackuser_keyword').value;
					       	    store_rules_blackuser.keyword=aa;
							    store_rules_blackuser.currentPage=1;
							    reflash();
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('blackuser_keyword').value;
		               store_rules_blackuser.keyword=bb;
		               store_rules_blackuser.currentPage=1;         
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
								id:'blackuser_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
							  if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('name') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('name') + "'";
			                    }
			                }  
											store_rules_blackuser.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            },'-',{
		            		text:'导入',
				           	id:'imp',
				           	iconCls:'exp',
				           	handler: function(){
							            var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@nc/black_dr_first.htm'+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
							            var windr = new Ext.Window ({
		                             id:"windr",
		                             title:"导入黑白名单",
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
		            }]
        }
    });
		store_rules_blackuser.alreadyload = 1;
    this.callParent(arguments);
    }
})