/*商城用户管理*/
Ext.define('shops.shopuserManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_shopusermanager.alreadyload != 1){
						store_shops_shopusermanager.uid = '';
						store_shops_shopusermanager.username = '';
						store_shops_shopusermanager.dispname = '';
						store_shops_shopusermanager.groupname = '';
						store_shops_shopusermanager.proname = '';
						store_shops_shopusermanager.usermac = '';
						store_shops_shopusermanager.password = '';
						store_shops_shopusermanager.nickname = '';
						store_shops_shopusermanager.membernum = '';
						store_shops_shopusermanager.intergral = '';
						store_shops_shopusermanager.coupon = '';
						store_shops_shopusermanager.address = '';
						store_shops_shopusermanager.decript = '';
						store_shops_shopusermanager.lcount = '';
						store_shops_shopusermanager.imgpath = '';
						store_shops_shopusermanager.addtime = '';
						store_shops_shopusermanager.lasttime = '';
						store_shops_shopusermanager.flags = '';
						store_shops_shopusermanager.plateid = plateid;
						store_shops_shopusermanager.keyword = '';
						store_shops_shopusermanager.caExport = '';
						store_shops_shopusermanager.new_params = {uid:store_shops_shopusermanager.uid,username:store_shops_shopusermanager.username,dispname:store_shops_shopusermanager.dispname,groupname:store_shops_shopusermanager.groupname,proname:store_shops_shopusermanager.proname,usermac:store_shops_shopusermanager.usermac,password:store_shops_shopusermanager.password,nickname:store_shops_shopusermanager.nickname,membernum:store_shops_shopusermanager.membernum,integral:store_shops_shopusermanager.integral,coupon:store_shops_shopusermanager.coupon,address:store_shops_shopusermanager.address,decript:store_shops_shopusermanager.decript,lcount:store_shops_shopusermanager.lcount,imgpath:store_shops_shopusermanager.imgpath,addtime:store_shops_shopusermanager.addtime,lasttime:store_shops_shopusermanager.lasttime,flags:store_shops_shopusermanager.flags,keyword:store_shops_shopusermanager.keyword,caExport:store_shops_shopusermanager.caExport};
				}	
				
				function reflash(){
						store_shops_shopusermanager.new_params = {uid:store_shops_shopusermanager.uid,username:store_shops_shopusermanager.username,dispname:store_shops_shopusermanager.dispname,groupname:store_shops_shopusermanager.groupname,proname:store_shops_shopusermanager.proname,usermac:store_shops_shopusermanager.usermac,password:store_shops_shopusermanager.password,nickname:store_shops_shopusermanager.nickname,membernum:store_shops_shopusermanager.membernum,integral:store_shops_shopusermanager.integral,coupon:store_shops_shopusermanager.coupon,address:store_shops_shopusermanager.address,decript:store_shops_shopusermanager.decript,lcount:store_shops_shopusermanager.lcount,imgpath:store_shops_shopusermanager.imgpath,addtime:store_shops_shopusermanager.addtime,lasttime:store_shops_shopusermanager.lasttime,flags:store_shops_shopusermanager.flags,keyword:store_shops_shopusermanager.keyword,caExport:store_shops_shopusermanager.caExport};
						//store_shops_shopusermanager.currentPage=1;
						store_shops_shopusermanager.load();		
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('shopusermanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_shopusermanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "商城用户ID",
		            dataIndex: 'uid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            text: "用户登录名称",
		            dataIndex: 'username',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "显示名称",
		            dataIndex: 'dispname',
		            width: 130,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "密码",
		            dataIndex: 'password',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "手机号",
		            dataIndex: 'mobile',
		            width: 130,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "用户mac",
		            dataIndex: 'usermac',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "单位名称",
		            dataIndex: 'groupname',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "性别",
		            dataIndex: 'sex',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "证件号",
		            dataIndex: 'idno',
		            width: 180,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "邮箱地址",
		            dataIndex: 'email',
		            width: 180,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "注册时间",
		            dataIndex: 'stime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "优惠劵",
		            dataIndex: 'coupon',
		            width: 50,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "地址",
		            dataIndex: 'address',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "访问次数",
		            dataIndex: 'lcount',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 150,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "加入时间",
		            dataIndex: 'addtime',
		            width: 150,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "最后访问时间",
		            dataIndex: 'lasttime',
		            width: 150,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "个人关注",
		            dataIndex: 'decript',
		            width: 100,
		            //align: 'center',
		            hidden: true,
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
		            	  labelWidth:40,
		            	  width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'shopuser_keyword',
		                name:'shopuser_keyword',
		                value:store_shops_shopusermanager.keyword,
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
			                    		var aa=Ext.getCmp('shopuser_keyword').value;
					                    if(aa!='输入用户登录名称'){
							       	            store_shops_shopusermanager.keyword=aa;
									                store_shops_shopusermanager.currentPage=1;
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
			                 var bb=Ext.getCmp('shopuser_keyword').value;
				               if(bb!='输入用户登录名称'){         
				                  store_shops_shopusermanager.keyword=bb;
				               }
				               else{
				                  store_shops_shopusermanager.keyword='';
				               }
				               store_shops_shopusermanager.currentPage=1;         
				               reflash();  
		                }
		            },'-'
		    /*        
		            ,{
		                text:'删除',
		                itemId: 'shopusermanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
		                handler:function(){
										   if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('uid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('uid') + "'";
					                    }
					                }
												  store_shops_shopusermanager.load({params: {del: "del",selsid:selsid}});
											 }
		                }
		            }
		        */    
		            ]
		        }
		    });
		    store_shops_shopusermanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});