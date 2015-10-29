/*用户关注管理*/
Ext.define('shops.shopattentManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_shopattentmanager.alreadyload != 1){
						store_shops_shopattentmanager.id = '';
						store_shops_shopattentmanager.servicecode = '';
						store_shops_shopattentmanager.username = '';
						store_shops_shopattentmanager.uid = '';
						store_shops_shopattentmanager.activename = '';
						store_shops_shopattentmanager.indexname = '';
						store_shops_shopattentmanager.shopname = '';
						store_shops_shopattentmanager.integral = ''; 
						store_shops_shopattentmanager.decript = '';
						store_shops_shopattentmanager.imgpath = '';
						store_shops_shopattentmanager.addtime = '';
						store_shops_shopattentmanager.lasttime = '';
						store_shops_shopattentmanager.flags = '';
						store_shops_shopattentmanager.plateid = plateid;
						store_shops_shopattentmanager.keyword = '';
						store_shops_shopattentmanager.caExport = '';
						store_shops_shopattentmanager.new_params = {id:store_shops_shopattentmanager.id,servicecode:store_shops_shopattentmanager.servicecode,username:store_shops_shopattentmanager.username,uid:store_shops_shopattentmanager.uid,activename:store_shops_shopattentmanager.activename,indexname:store_shops_shopattentmanager.indexname,shopname:store_shops_shopattentmanager.shopname,integral:store_shops_shopattentmanager.integral,decript:store_shops_shopattentmanager.decript,imgpath:store_shops_shopattentmanager.imgpath,addtime:store_shops_shopattentmanager.addtime,lasttime:store_shops_shopattentmanager.lasttime,flags:store_shops_shopattentmanager.flags,keyword:store_shops_shopattentmanager.keyword,caExport:store_shops_shopattentmanager.caExport};
				}	
				
				function reflash(){
						store_shops_shopattentmanager.new_params = {id:store_shops_shopattentmanager.id,servicecode:store_shops_shopattentmanager.servicecode,username:store_shops_shopattentmanager.username,uid:store_shops_shopattentmanager.uid,activename:store_shops_shopattentmanager.activename,indexname:store_shops_shopattentmanager.indexname,shopname:store_shops_shopattentmanager.shopname,integral:store_shops_shopattentmanager.integral,decript:store_shops_shopattentmanager.decript,imgpath:store_shops_shopattentmanager.imgpath,addtime:store_shops_shopattentmanager.addtime,lasttime:store_shops_shopattentmanager.lasttime,flags:store_shops_shopattentmanager.flags,keyword:store_shops_shopattentmanager.keyword,caExport:store_shops_shopattentmanager.caExport};
						//store_shops_shopattentmanager.currentPage=1;
						store_shops_shopattentmanager.load();		
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('shopattentmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_shopattentmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "商城用户关注ID",
		            dataIndex: 'id',
		            width: 100,
		            //align: 'center',
		            hidden: true ,
		            sortable: true
		        },{
		            text: "用户登录名称",
		            dataIndex: 'username',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "用户ID",
		            dataIndex: 'uid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "活动名称",
		            dataIndex: 'activename',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "品牌名称",
		            dataIndex: 'indexname',
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
		            text: "场所代码",
		            dataIndex: 'servicecode',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "积分",
		            dataIndex: 'integral',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "关注时间",
		            dataIndex: 'addtime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "最后访问时间",
		            dataIndex: 'lasttime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "描述",
		            dataIndex: 'decript',
		            width: 200,
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
		                id:'shopattentmanager_keyword',
		                name:'shopattentmanager_keyword',
		                value:store_shops_shopattentmanager.keyword,
		                emptyText:'请输入用户登录名称',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入用户登录名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('shopattentmanager_keyword').value;
					                    if(aa!='输入用户登录名称'){
							       	            store_shops_shopattentmanager.keyword=aa;
									                store_shops_shopattentmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('shopattentmanager_keyword').value;
				               if(bb!='输入用户登录名称'){         
				                  store_shops_shopattentmanager.keyword=bb;
				               }
				               else{
				                  store_shops_shopattentmanager.keyword='';
				               }
				               store_shops_shopattentmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                text:'删除',
		                id: 'shopattentmanager_removeButton',
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
												  store_shops_shopattentmanager.load({params: {del: "del",selsid:selsid}});
											 }
		                }
		            }]
		        }
		    });
		    store_shops_shopattentmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});