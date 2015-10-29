/*商品小类管理*/
Ext.define('shops.typeManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_typemanager.alreadyload != 1){
						store_shops_typemanager.sid = '';	
						store_shops_typemanager.name = '';
						store_shops_typemanager.groupname = '';
						store_shops_typemanager.descr = '';
						store_shops_typemanager.systime = '';
						store_shops_typemanager.imgpath = '';
						store_shops_typemanager.flags = '';
						store_shops_typemanager.plateid = plateid;
						store_shops_typemanager.keyword = '';
						store_shops_typemanager.caExport = '';
						store_shops_typemanager.new_params = {sid:store_shops_typemanager.sid,name:store_shops_typemanager.name,groupname:store_shops_typemanager.groupname,descr:store_shops_typemanager.descr,systime:store_shops_typemanager.systime,imgpath:store_shops_typemanager.imgpath,flags:store_shops_typemanager.flags,keyword:store_shops_typemanager.keyword,caExport:store_shops_typemanager.caExport};
				}	
				
				function reflash(){
						store_shops_typemanager.new_params = {sid:store_shops_typemanager.sid,name:store_shops_typemanager.name,groupname:store_shops_typemanager.groupname,descr:store_shops_typemanager.descr,systime:store_shops_typemanager.systime,imgpath:store_shops_typemanager.imgpath,flags:store_shops_typemanager.flags,keyword:store_shops_typemanager.keyword,caExport:store_shops_typemanager.caExport};
						//store_shops_typemanager.currentPage=1;
						store_shops_typemanager.load();		
				}
				
				function gotoedit1(value, id, plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomtype_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:518px;"></iframe>';      
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"商品小类信息",
				                           //x:0,
				                           y:0,
				                           width:400,
				                           bodyPadding: 0,
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
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('typemanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_typemanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "类别编码",
		            dataIndex: 'sid',
		            width: 100,
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
		                gotoedit1('update',record.get('sid'),plateid);
		            }
		        },{
		            text: "商品小类名称",
		            dataIndex: 'name',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商品大类名称",
		            dataIndex: 'groupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
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
		                id:'typemanager_keyword',
		                name:'typemanager_keyword',
		                value:store_shops_typemanager.keyword,
		                emptyText:'请输入商品小类名称',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入商品小类名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('typemanager_keyword').value;
					                    if(aa!='输入商品小类名称'){
							       	            store_shops_typemanager.keyword=aa;
									                store_shops_typemanager.currentPage=1;
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
			                 var bb=Ext.getCmp('typemanager_keyword').value;
				               if(bb!='输入商品小类名称'){         
				                  store_shops_typemanager.keyword=bb;
				               }
				               else{
				                  store_shops_typemanager.keyword='';
				               }
				               store_shops_typemanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               	  gotoedit1('add', '', plateid);
			               }
		            }, '-', {
		                text:'删除',
		                id: 'typemanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
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
		                      store_shops_typemanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }
		    });
		    store_shops_typemanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});