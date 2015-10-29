/*用户收藏管理*/
Ext.define('shops.shopcollectManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_shopcollectmanager.alreadyload != 1){
						store_shops_shopcollectmanager.id = '';
						store_shops_shopcollectmanager.servicecode= '';
						store_shops_shopcollectmanager.username = '';
						store_shops_shopcollectmanager.nickname = '';
						store_shops_shopcollectmanager.membernum = '';
						store_shops_shopcollectmanager.uid = '';
						store_shops_shopcollectmanager.activename = '';
						store_shops_shopcollectmanager.odityname = '';
						store_shops_shopcollectmanager.imgpath = '';
						store_shops_shopcollectmanager.decript = '';
						store_shops_shopcollectmanager.addtime = '';
						store_shops_shopcollectmanager.lasttime = '';
						store_shops_shopcollectmanager.flags = '';
						store_shops_shopcollectmanager.plateid = plateid;
						store_shops_shopcollectmanager.keyword = '';
						store_shops_shopcollectmanager.caExport = '';
						store_shops_shopcollectmanager.new_params = {id:store_shops_shopcollectmanager.id,servicecode:store_shops_shopcollectmanager.servicecode,username:store_shops_shopcollectmanager.username,nickname:store_shops_shopcollectmanager.nickname,membernum:store_shops_shopcollectmanager.membernum,uid:store_shops_shopcollectmanager.uid,activename:store_shops_shopcollectmanager.activename,odityname:store_shops_shopcollectmanager.odityname,imgpath:store_shops_shopcollectmanager.imgpath,decript:store_shops_shopcollectmanager.decript,addtime:store_shops_shopcollectmanager.addtime,lasttime:store_shops_shopcollectmanager.lasttime,flags:store_shops_shopcollectmanager.flags,keyword:store_shops_shopcollectmanager.keyword,caExport:store_shops_shopcollectmanager.caExport};
				}	
				
				function reflash(){
						store_shops_shopcollectmanager.new_params = {id:store_shops_shopcollectmanager.id,servicecode:store_shops_shopcollectmanager.servicecode,username:store_shops_shopcollectmanager.username,nickname:store_shops_shopcollectmanager.nickname,membernum:store_shops_shopcollectmanager.membernum,uid:store_shops_shopcollectmanager.uid,activename:store_shops_shopcollectmanager.activename,odityname:store_shops_shopcollectmanager.odityname,imgpath:store_shops_shopcollectmanager.imgpath,decript:store_shops_shopcollectmanager.decript,addtime:store_shops_shopcollectmanager.addtime,lasttime:store_shops_shopcollectmanager.lasttime,flags:store_shops_shopcollectmanager.flags,keyword:store_shops_shopcollectmanager.keyword,caExport:store_shops_shopcollectmanager.caExport};
						//store_shops_shopcollectmanager.currentPage=1;
						store_shops_shopcollectmanager.load();		
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('shopcollectmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_shopcollectmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "商城用户收藏ID",
		            dataIndex: 'id',
		            width: 100,
		            //align: 'center',
		            hidden: true ,
		            sortable: true
		        },{
		            text: "用户ID",
		            dataIndex: 'uid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "用户登录名称",
		            dataIndex: 'username',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "昵称",
		            dataIndex: 'nickname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "会员号",
		            dataIndex: 'membernum',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "活动名称",
		            dataIndex: 'activename',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商品名称",
		            dataIndex: 'odityname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
		            editor: {
		            }
		        },{
		            text: "场所代码",
		            dataIndex: 'servicecode',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "收藏时间",
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
		                id:'shopcollectmanager_keyword',
		                name:'shopcollectmanager_keyword',
		                value:store_shops_shopcollectmanager.keyword,
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
			                    		var aa=Ext.getCmp('shopcollectmanager_keyword').value;
					                    if(aa!='输入用户登录名称'){
							       	            store_shops_shopcollectmanager.keyword=aa;
									                store_shops_shopcollectmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('shopcollectmanager_keyword').value;
				               if(bb!='输入用户登录名称'){         
				                  store_shops_shopcollectmanager.keyword=bb;
				               }
				               else{
				                  store_shops_shopcollectmanager.keyword='';
				               }
				               store_shops_shopcollectmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                text:'删除',
		                id: 'shopcollectmanager_removeButton',
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
												  store_shops_shopcollectmanager.load({params: {del: "del",selsid:selsid}});
											 }
		                }
		            }]
		        }
		    });
		    store_shops_shopcollectmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});