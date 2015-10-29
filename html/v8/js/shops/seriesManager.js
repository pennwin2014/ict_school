/*促销广告明细*/
Ext.define('shops.seriesManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_seriesmanager.alreadyload != 1){
						store_shops_seriesmanager.id = '';
						store_shops_seriesmanager.servicecode = '';
						store_shops_seriesmanager.aname = '';
						store_shops_seriesmanager.indexname = '';
						store_shops_seriesmanager.odityname = '';
						store_shops_seriesmanager.discount = '';
						store_shops_seriesmanager.address = '';
						store_shops_seriesmanager.decript = '';
						store_shops_seriesmanager.imgpath = '';
						store_shops_seriesmanager.plevel = '';
						store_shops_seriesmanager.type = '';
						store_shops_seriesmanager.xid = '';
						store_shops_seriesmanager.stime = '';
						store_shops_seriesmanager.etime = '';
						store_shops_seriesmanager.flags = '';
						store_shops_seriesmanager.plateid = plateid;
						store_shops_seriesmanager.keyword = '';
						store_shops_seriesmanager.caExport = '';
						store_shops_seriesmanager.new_params = {id:store_shops_seriesmanager.id,servicecode:store_shops_seriesmanager.servicecode,aname:store_shops_seriesmanager.aname,indexname:store_shops_seriesmanager.indexname,odityname:store_shops_seriesmanager.odityname,couponfee:store_shops_seriesmanager.couponfee,discount:store_shops_seriesmanager.discount,address:store_shops_seriesmanager.address,decript:store_shops_seriesmanager.decript,imgpath:store_shops_seriesmanager.imgpath,plevel:store_shops_seriesmanager.plevel,type:store_shops_seriesmanager.type,xid:store_shops_seriesmanager.xid,stime:store_shops_seriesmanager.stime,etime:store_shops_seriesmanager.etime,flags:store_shops_seriesmanager.flags,keyword:store_shops_seriesmanager.keyword,caExport:store_shops_seriesmanager.caExport};
				}
				
				function reflash(){
						store_shops_seriesmanager.new_params = {id:store_shops_seriesmanager.id,servicecode:store_shops_seriesmanager.servicecode,aname:store_shops_seriesmanager.aname,indexname:store_shops_seriesmanager.indexname,odityname:store_shops_seriesmanager.odityname,couponfee:store_shops_seriesmanager.couponfee,discount:store_shops_seriesmanager.discount,address:store_shops_seriesmanager.address,decript:store_shops_seriesmanager.decript,imgpath:store_shops_seriesmanager.imgpath,plevel:store_shops_seriesmanager.plevel,type:store_shops_seriesmanager.type,xid:store_shops_seriesmanager.xid,stime:store_shops_seriesmanager.stime,etime:store_shops_seriesmanager.etime,flags:store_shops_seriesmanager.flags,keyword:store_shops_seriesmanager.keyword,caExport:store_shops_seriesmanager.caExport};
						//store_shops_seriesmanager.currentPage=1;
						store_shops_seriesmanager.load();			
				}
				
				function gotoedit1(value,id,plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomseries_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:550px;"></iframe>';      
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"活动信息",
				                           //x:0,
				                           //y:0,
				                           width:770,
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
		                Ext.getCmp('seriesmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_seriesmanager,
		    		viewConfig:{
		    				loadMask:false	
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
		            dataIndex: 'groupid',
		            icon: '/images/edit_task.png',
		            tooltip: '点击进入修改界面',
		            width: 40,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', record.get('id'), plateid);
		            }
		        },{
		            text: "活动名称",
		            dataIndex: 'aname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "品牌名称",
		            dataIndex: 'indexname',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商品名称",
		            dataIndex: 'odityname',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "场所代码",
		            dataIndex: 'servicecode',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "优惠卷面值",
		            dataIndex: 'couponfee',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "折扣",
		            dataIndex: 'discount',
		            width: 80,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "地址",
		            dataIndex: 'address',
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
		            text: "活动优先级",
		            dataIndex: 'plevel',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "图片层次",
		            dataIndex: 'type',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "系列ID",
		            dataIndex: 'xid',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "开始时间",
		            dataIndex: 'stime',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "结束时间",
		            dataIndex: 'etime',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "描述",
		            dataIndex: 'decript',
		            width: 200,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            editor: {
		            }
		        },{
		            text: "标记",
		            dataIndex: 'flags',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false,
		            editor: {
		            }
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
		                id:'seriesmanager_keyword',
		                name:'seriesmanager_keyword',
		                value:store_shops_seriesmanager.keyword,
		                emptyText:'请输入活动名称',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入活动名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('seriesmanager_keyword').value;
					                    if(aa!='输入活动名称'){
							       	            store_shops_seriesmanager.keyword=aa;
									                store_shops_seriesmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('seriesmanager_keyword').value;
				               if(bb!='输入活动名称'){         
				                  store_shops_seriesmanager.keyword=bb;
				               }
				               else{
				                  store_shops_seriesmanager.keyword='';
				               }
				               store_shops_seriesmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               		gotoedit1('add', '', plateid);
			               }
		            },'-',{
		                text:'删除',
		                id: 'seriesmanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
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
		                      store_shops_seriesmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }	
		    });
		    store_shops_seriesmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});