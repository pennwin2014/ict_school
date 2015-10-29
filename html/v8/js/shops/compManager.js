/*商品对照表*/
Ext.define('shops.compManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_compmanager.aireadyload != 1){
						store_shops_compmanager.id = '';	
						store_shops_compmanager.shopname = '';
						store_shops_compmanager.level = '';
						store_shops_compmanager.sgroupid = '';
						store_shops_compmanager.etime = '';
						store_shops_compmanager.plateid = plateid;
						store_shops_compmanager.areaid = '';
						store_shops_compmanager.keyword = '';
						store_shops_compmanager.caExport = '';
						store_shops_compmanager.new_params = {id:store_shops_compmanager.id,shopname:store_shops_compmanager.shopname,level:store_shops_compmanager.level,sgroupid:store_shops_compmanager.sgroupid,etime:store_shops_compmanager.etime,keyword:store_shops_compmanager.keyword,caExport:store_shops_compmanager.caExport};
				}
				
				function reflash(){
						store_shops_compmanager.new_params = {id:store_shops_compmanager.id,shopname:store_shops_compmanager.shopname,level:store_shops_compmanager.level,sgroupid:store_shops_compmanager.sgroupid,etime:store_shops_compmanager.etime,keyword:store_shops_compmanager.keyword,caExport:store_shops_compmanager.caExport};
						//store_shops_compmanager.currentPage=1;
						store_shops_compmanager.load();	
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('compmanager_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });
		    
		    // 高级查询窗口
		    function gotoedit1(value, id){
		    		var win3 = null;
		    		var level = '';
		    		var etime = '';
		    		var shopid1 = '';
		    		var sgroupid1 = '';
		    		var sid1 = '';
		    		
		    		if(value == 'update'){
		    				level = store_shops_compmanager.getAt(id).get('level');
		    				etime = store_shops_compmanager.getAt(id).get('etime');
		    				shopid1 = store_shops_compmanager.getAt(id).get('shopid');
		    				sgroupid1 = store_shops_compmanager.getAt(id).get('sgroupid');
		    				sid1 = store_shops_compmanager.getAt(id).get('id');
		    		}	
		    		
		    		if(!win3){  //判断如果不存在就创建新的
		    				win3 = new Ext.Window({
		    						title:'高级查询窗口',
		    						closeAction:'hide',
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
							      		id:'comp_form2',
							      		layout:"form",
							      		baseCls:"x-plain",
							      		bodyPadding:5,
							      		items:[{
								      			xtype:'numberfield',
														fieldLabel:'优先级',
														name:'level',
														value:level,
														hideTrigger:false, //隐藏微调按钮
								        		allowDecimals:false, //不允许输入小数
								        		maxValue:100, //最大值
		        								minvalue:0, //最小值
								        		nanText:'请输入有效的小数' //无效数字提示
												},{
														xtype:'textfield',
														fieldLabel:'有效时间(秒)',
														emptyText:'请输入商店排名有效时间',
														value:etime,
														name:'etime'
												}]
							      })],
							      buttons:[{
											  text:"确定",
											  iconCls:'icon-save',		  
											  handler:function(){
							        			var objectTmp = Ext.getCmp("comp_form2").getForm().getValues();
														var level1 = objectTmp.level;
														var etime1= objectTmp.etime;
														
														store_shops_compmanager.load({params: {update:value,sid:sid1,shopid:shopid1,sgroupid:sgroupid1,level:level1,etime:etime1}});
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
		    
		    Ext.define('TypeData', {
		        extend: 'Ext.data.Model',
		        fields: ['sid', 'name', 'groupname', 'descr', 'systime', 'imgpath', 'flags']
		        //idProperty: 'uid'
		    });
			  var store_type = Ext.create('Ext.data.Store', {
			  	 model:'TypeData',
			  	 pageSize: 10000,
			  	 autoLoad:true,
			  	 proxy:{
			  	 		type:'ajax',
			  	 		url:'/pronline/Msg?FunName@ncm_webcomtype_list',
			  	 		reader:{
			  	 				type:'json',
			  	 				root:'eimdata',
			  	 				totalProperty:'totalCount'	
			  	 		},
			  	 		simpleSortMode:true	
			  	 }
			  });
			  var typeCombo = Ext.create('Ext.form.field.ComboBox', {
			  	 fieldLabel:'商品小类名称',
			  	 labelWidth:80,
			  	 width:200,
			  	 valueField:'sid',
			  	 displayField:'name',
			  	 id:'compmanager_tyid',
			  	 name:'compmanager_tyid',
			  	 allowBlank:true,
			  	 store:store_type,
			  	 triggerAction:'all',
			  	 queryMode:'local',
			  	 typeAhead:true,
			  	 listeners:{
			  	 		'change':function(){
			  	 				 store_shops_compmanager.sgroupid = this.value;
			  	 				 reflash();
			  	 		}	
			  	 },
			  	 listConfig:{
			  	 		loadingText:'Searching...',
			  	 		emptyText:'No matching found.'	
			  	 }
			  });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_compmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		    				text:"对照ID",
		    				dataIndex:'id',
		    				width:100,
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
		            text: "商店名称",
		            dataIndex: 'shopname',
		            width: 200,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "商品小类名称",
		            dataIndex: 'sgroupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "商店优先级",
		            dataIndex: 'level',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: true
		        },{
		            text: "有效时间(秒)",
		            dataIndex: 'etime',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: true
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        
		        // paging bar on the bottom
		        tbar:{items:[
		        	  '-', typeCombo,
		        		'-', {
		            	  labelWidth: 40,
		            	  width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'compmanager_keyword',
		                name:'compmanager_keyword',
		                emptyText:'请输入商店名称',
		                value:store_shops_compmanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入商店名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('compmanager_keyword').value;
					                    if(aa!='输入商店名称'){
							       	            store_shops_compmanager.keyword=aa;
									                store_shops_compmanager.currentPage=1;
									                reflash();
							                }
			                    }
			                }
		                }
		            }, {
		                text:'查询',
		                //itemId: 'moveButton',
		            		iconCls:'accept',
		                //disabled: true,
		                handler:function(){
			                 var bb=Ext.getCmp('compmanager_keyword').value;
				               if(bb!='输入商店名称'){         
				                  store_shops_compmanager.keyword=bb;
				               }
				               else{
				                  store_shops_compmanager.keyword='';
				               }
				               store_shops_compmanager.currentPage=1;         
				               reflash();  
		                }
		            }]
		        }
		    });
		    store_shops_compmanager.alreadyload = 1;
		    this.callParent(arguments);	
		}	
});