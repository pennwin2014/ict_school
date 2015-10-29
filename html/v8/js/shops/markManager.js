/*SSID管理*/
Ext.define('shops.markManager', {
		extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
    		if(store_shops_markmanager.alreadyload !=1){
    				store_shops_markmanager.sid = '';
    				store_shops_markmanager.mark = '';
    				store_shops_markmanager.mapid = '';
    				store_shops_markmanager.sx = '';
    				store_shops_markmanager.sy = '';
    				store_shops_markmanager.ex = '';
    				store_shops_markmanager.ey = '';
    				store_shops_markmanager.sname = '';
    				store_shops_markmanager.keyword = '';
						store_shops_markmanager.caExport = '';
    				store_shops_markmanager.new_params = {sid:store_shops_markmanager.sid,mark:store_shops_markmanager.mark,mapid:store_shops_markmanager.mapid,sx:store_shops_markmanager.sx,sy:store_shops_markmanager.sy,ex:store_shops_markmanager.ex,ey:store_shops_markmanager.ey,sname:store_shops_markmanager.sname,keyword:store_shops_markmanager.keyword,caExport:store_shops_markmanager.caExport};
    		}
    
		    function reflash(){
		    		store_shops_markmanager.new_params = {sid:store_shops_markmanager.sid,mark:store_shops_markmanager.mark,mapid:store_shops_markmanager.mapid,sx:store_shops_markmanager.sx,sy:store_shops_markmanager.sy,ex:store_shops_markmanager.ex,ey:store_shops_markmanager.ey,sname:store_shops_markmanager.sname,keyword:store_shops_markmanager.keyword,caExport:store_shops_markmanager.caExport};
		    		//store_shops_markmanager.currentPage=1;
			      store_shops_markmanager.load();	
		    }
		    
		    var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('markmanager_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });	
		    
		    // 高级查询窗口
		    function gotoedit1(value, id){
		    		var win3 = null;
		    		var sid1 = '';
		    		var mark = '';
		    		var mapid = '';
		    		var sx = '';
		    		var sy = '';
		    		var ex = '';
		    		var ey = '';
		    		var sname = '';
		    			
		    		if(value == 'update'){
		    				mark = store_shops_markmanager.getAt(id).get('mark');
		    				mapid = store_shops_markmanager.getAt(id).get('mapid');
		    				sx = store_shops_markmanager.getAt(id).get('sx');
		    				sy = store_shops_markmanager.getAt(id).get('sy');
		    				ex = store_shops_markmanager.getAt(id).get('ex');
		    				ey = store_shops_markmanager.getAt(id).get('ey');
		    				sname = store_shops_markmanager.getAt(id).get('sname');
		    				sid1 = store_shops_markmanager.getAt(id).get('sid');
		    		}
		    		
		    		if(!win3){ //判断如果不存在就创建新的
		    			  win3 = new Ext.Window({
		    			  		title:'地图标签管理',
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
						      			id:'markmanager_form2',
												layout:"form",
												baseCls:"x-plain",
												bodyPadding: 5,
												items:[{
			                      fieldLabel:'标签',
			                      xtype: 'textfield',
														emptyText:'请输入标签',
														value:mark,
			                      name:'mark'
			                  },{
			                      fieldLabel:'地图',
			                      xtype: 'textfield',
														emptyText:'请输入地图',
														value:mapid,
			                      name:'mapid'
			                  },{
			                      fieldLabel:'SX',
			                      xtype: 'textfield',
														emptyText:'请输入SX',
														value:sx,
			                      name:'sx'
			                  },{
			                      fieldLabel:'SY',
			                      xtype: 'textfield',
														emptyText:'请输入SY',
														value:sy,
			                      name:'sy'
			                  },{
			                      fieldLabel:'EX',
			                      xtype: 'textfield',
														emptyText:'请输入EX',
														value:ex,
			                      name:'ex'
			                  },{
			                      fieldLabel:'EY',
			                      xtype: 'textfield',
														emptyText:'请输入EY',
														value:ey,
			                      name:'ey'
			                  },{
			                      fieldLabel:'Sname',
			                      xtype: 'textfield',
														emptyText:'请输入Sname',
														value:sname,
			                      name:'sname'
			                  }]	
						      	})],
						      	buttons:[{
												  text:"确定",
												  iconCls:'icon-save',		  
												  handler:function(){
								        			var objectTmp = Ext.getCmp("markmanager_form2").getForm().getValues();
															var mark1 = objectTmp.mark;
															var mapid1 = objectTmp.mapid;
															var sx1 = objectTmp.sx;
															var sy1 = objectTmp.sy;
															var ex1 = objectTmp.ex;
															var ey1 = objectTmp.ey;
															var sname1 = objectTmp.sname;
															store_shops_markmanager.load({params: {update:value,sid:sid1,mark:mark1,mapid:mapid1,sx:sx1,sy:sy1,ex:ex1,ey:ey1,sname:sname1}});
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
		        store: store_shops_markmanager,
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
		            iconCls: 'report_edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', rowIndex);
		            }
		        },{
		            text: "标签",
		            dataIndex: 'mark',
		            width: 100,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "地图",
		            dataIndex: 'mapid',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "SX",
		            dataIndex: 'sx',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "SY",
		            dataIndex: 'sy',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "EX",
		            dataIndex: 'ex',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "EY",
		            dataIndex: 'ey',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "Sname",
		            dataIndex: 'sname',
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
		            		labelWidth: 40,
		            		width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'markmanager_keyword',
		                name:'markmanager_keyword',
										emptyText:'请输入标签',
		                value:store_shops_markmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                if(this.value=='请输入标签'){                
						                   this.setValue('');
						                }
										    },
			                  'keydown' : function(i,e){
				                  	var aae=e.getKey(); 
				                    if(aae==13){
						                  	var aa=Ext.getCmp('markmanager_keyword').value;
									       	    	store_shops_markmanager.keyword=aa;
											    			store_shops_markmanager.currentPage=1;
											    			reflash();
				                    }
			                  }
		                }
		            }, '-', {
		                text:'查询',
		            	  iconCls:'accept',
		                handler:function(){
			                 var bb=Ext.getCmp('markmanager_keyword').value;
				               store_shops_markmanager.keyword=bb;
				               store_shops_markmanager.currentPage=1;         
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
										id:'markmanager_removeButton',
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
													store_shops_markmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }	
		    });
		    store_shops_markmanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});