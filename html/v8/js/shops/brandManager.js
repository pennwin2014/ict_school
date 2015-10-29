/*品牌管理*/
Ext.define('shops.brandManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_brandmanager.alreadyload != 1){
						store_shops_brandmanager.sid = '';
						store_shops_brandmanager.name = '';
						store_shops_brandmanager.sgroupname = '';
						store_shops_brandmanager.descr = '';
						store_shops_brandmanager.imgpath = '';
						store_shops_brandmanager.serno = '';
						store_shops_brandmanager.floor = '';
						store_shops_brandmanager.level = '';
						store_shops_brandmanager.plevel = '';
						store_shops_brandmanager.type = '';
						store_shops_brandmanager.systime = '';
						store_shops_brandmanager.shopid = '';
						store_shops_brandmanager.shopname = '';
						store_shops_brandmanager.plateid = plateid;
						store_shops_brandmanager.flags = '';
						store_shops_brandmanager.keyword = '';
						store_shops_brandmanager.caExport = '';
						store_shops_brandmanager.new_params = {sid:store_shops_brandmanager.sid,name:store_shops_brandmanager.name,sgroupname:store_shops_brandmanager.sgroupname,descr:store_shops_brandmanager.descr,imgpath:store_shops_brandmanager.imgpath,serno:store_shops_brandmanager.serno,floor:store_shops_brandmanager.floor,level:store_shops_brandmanager.level,plevel:store_shops_brandmanager.plevel,type:store_shops_brandmanager.type,systime:store_shops_brandmanager.systime,shopid:store_shops_brandmanager.shopid,shopname:store_shops_brandmanager.shopname,flags:store_shops_brandmanager.flags,keyword:store_shops_brandmanager.keyword,caExport:store_shops_brandmanager.caExport};
				}
				
				function reflash(){
					  store_shops_brandmanager.new_params = {sid:store_shops_brandmanager.sid,name:store_shops_brandmanager.name,sgroupname:store_shops_brandmanager.sgroupname,descr:store_shops_brandmanager.descr,imgpath:store_shops_brandmanager.imgpath,serno:store_shops_brandmanager.serno,floor:store_shops_brandmanager.floor,level:store_shops_brandmanager.level,plevel:store_shops_brandmanager.plevel,type:store_shops_brandmanager.type,systime:store_shops_brandmanager.systime,shopid:store_shops_brandmanager.shopid,shopname:store_shops_brandmanager.shopname,flags:store_shops_brandmanager.flags,keyword:store_shops_brandmanager.keyword,caExport:store_shops_brandmanager.caExport};
					  //store_shops_brandmanager.currentPage=1;
						store_shops_brandmanager.load();	
				}
				
				function gotoedit1(value,id,plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcombrand_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%; height:373px;"></iframe>';      
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"品牌信息",
				                           //x:0,
				                           //y:0,
				                           width:770,
				                           bodyPadding:0,
				                           modal:true,
																	 //headerPosition: 'bottom',
				                           resizable:true,
				                           closable :true,
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
		                Ext.getCmp('brandmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_brandmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "商标ID",
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
		            text: "商标名称",
		            dataIndex: 'name',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商标序列号",
		            dataIndex: 'serno',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 200,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "商品小类名称",
		            dataIndex: 'sgroupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商店名称",
		            dataIndex: 'shopname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "楼层",
		            dataIndex: 'floor',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "广告优先级",
		            dataIndex: 'level',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "促销优先级",
		            dataIndex: 'plevel',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "活动类型",
		            dataIndex: 'type',
		            width: 80,
		            //align: 'center',
		            hidden: false,
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
		            width: 300,
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
		                id:'brandmanager_keyword',
		                name:'brandmanager_keyword',
		                value:store_shops_brandmanager.keyword,
		                emptyText:'请输入商标名称',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入商标名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('brandmanager_keyword').value;
					                    if(aa!='输入商标名称'){
							       	            store_shops_brandmanager.keyword=aa;
									                store_shops_brandmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('brandmanager_keyword').value;
				               if(bb!='输入商标名称'){         
				                  store_shops_brandmanager.keyword=bb;
				               }
				               else{
				                  store_shops_brandmanager.keyword='';
				               }
				               store_shops_brandmanager.currentPage=1;         
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
		                id: 'brandmanager_removeButton',
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
		                      store_shops_brandmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }
		    });
		    store_shops_brandmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});