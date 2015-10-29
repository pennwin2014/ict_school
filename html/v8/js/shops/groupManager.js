/*商品大类管理*/
Ext.define('shops.groupManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_groupmanager.alreadyload != 1){
						store_shops_groupmanager.groupid = '';
						store_shops_groupmanager.name = '';
						store_shops_groupmanager.descr = '';
						store_shops_groupmanager.systime = '';
						store_shops_groupmanager.imgpath = '';
						store_shops_groupmanager.flags = '';
						store_shops_groupmanager.plateid = plateid;
						store_shops_groupmanager.keyword = '';
						store_shops_groupmanager.caExport = '';
						store_shops_groupmanager.new_params = {groupid:store_shops_groupmanager.groupid,name:store_shops_groupmanager.name,descr:store_shops_groupmanager.descr,systime:store_shops_groupmanager.systime,imgpath:store_shops_groupmanager.imgpath,flags:store_shops_groupmanager.flags,keyword:store_shops_groupmanager.keyword,caExport:store_shops_groupmanager.caExport};
				}
				
				function reflash(){
						store_shops_groupmanager.new_params = {groupid:store_shops_groupmanager.groupid,name:store_shops_groupmanager.name,descr:store_shops_groupmanager.descr,systime:store_shops_groupmanager.systime,imgpath:store_shops_groupmanager.imgpath,flags:store_shops_groupmanager.flags,keyword:store_shops_groupmanager.keyword,caExport:store_shops_groupmanager.caExport};
						//store_shops_groupmanager.currentPage=1;
						store_shops_groupmanager.load();		
				}
				
				function gotoedit1(value,id,plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomgroup_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:550px;"></iframe>';      
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"商品大类信息",
				                           //x:0,
				                           //y:0,
				                           width:400,
				                           height:550,
				                           bodyPadding: 0,
				                           modal:true,
																	 //headerPosition: 'bottom',
				                           resizable:true,
				                           closable:true,
				                           draggable:true,
				                           html:htmlvar
				                                          
				    })       
				
					  windr.on("beforeclose", function() {
					  	reflash();
					  })
					  windr.show();
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('groupmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_groupmanager,
		    		viewConfig:{
		    				loadMask:false	
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
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1("update",record.get('groupid'),plateid);
		            }
		        },{
		            text: "商品大类名称",
		            dataIndex: 'name',
		            width: 200,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 300,
		            //align: 'center',
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
		            text: "描述",
		            dataIndex: 'descr',
		            width: 700,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "标记",
		            dataIndex: 'flags',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        
		        // paging bar on the bottom
		        tbar:{items:[
				        	  '-', {
		            	labelWidth: 40,
		            	width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'groupmanager_keyword',
		                name:'groupmanager_keyword',
		                value:store_shops_groupmanager.keyword,
		                emptyText:'请输入商品大类名称',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入商品大类名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('groupmanager_keyword').value;
					                    if(aa!='输入商品大类名称'){
							       	            store_shops_groupmanager.keyword=aa;
									                store_shops_groupmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('groupmanager_keyword').value;
				               if(bb!='输入商品大类名称'){         
				                  store_shops_groupmanager.keyword=bb;
				               }
				               else{
				                  store_shops_groupmanager.keyword='';
				               }
				               store_shops_groupmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               		gotoedit1("add", '', plateid);
			               }
		            },'-',{
		                text:'删除',
		                id: 'groupmanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
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
		                      store_shops_groupmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
            }
		    });
		    store_shops_groupmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});