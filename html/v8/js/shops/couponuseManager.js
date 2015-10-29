/*优惠劵使用管理*/
Ext.define('shops.couponuseManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_couponusemanager.alreadyload != 1){
						store_shops_couponusemanager.id = '';	
						store_shops_couponusemanager.scode = '';
						store_shops_couponusemanager.username = '';
						store_shops_couponusemanager.aid = '';
						store_shops_couponusemanager.aname = '';
						store_shops_couponusemanager.serno = '';
						store_shops_couponusemanager.usermac = '';
						store_shops_couponusemanager.sname = '';
						store_shops_couponusemanager.feetime = '';
						store_shops_couponusemanager.uid = '';
						store_shops_couponusemanager.sid = '';
						store_shops_couponusemanager.fee = '';
						store_shops_couponusemanager.imgpath = '';
						store_shops_couponusemanager.systime = '';
						store_shops_couponusemanager.flags = '';
						store_shops_couponusemanager.plateid = plateid;
						store_shops_couponusemanager.keyword = '';
						store_shops_couponusemanager.caExport = '';
						store_shops_couponusemanager.new_params = {id:store_shops_couponusemanager.id,scode:store_shops_couponusemanager.scode,username:store_shops_couponusemanager.username,aid:store_shops_couponusemanager.aid,aname:store_shops_couponusemanager.aname,serno:store_shops_couponusemanager.serno,usermac:store_shops_couponusemanager.usermac,sname:store_shops_couponusemanager.sname,feetime:store_shops_couponusemanager.feetime,uid:store_shops_couponusemanager.uid,sid:store_shops_couponusemanager.sid,fee:store_shops_couponusemanager.fee,imgpath:store_shops_couponusemanager.imgpath,systime:store_shops_couponusemanager.systime,flags:store_shops_couponusemanager.flags,keyword:store_shops_couponusemanager.keyword,caExport:store_shops_couponusemanager.caExport};
				}
				
				function reflash(){
						store_shops_couponusemanager.new_params = {id:store_shops_couponusemanager.id,scode:store_shops_couponusemanager.scode,username:store_shops_couponusemanager.username,aid:store_shops_couponusemanager.aid,aname:store_shops_couponusemanager.aname,serno:store_shops_couponusemanager.serno,usermac:store_shops_couponusemanager.usermac,sname:store_shops_couponusemanager.sname,feetime:store_shops_couponusemanager.feetime,uid:store_shops_couponusemanager.uid,sid:store_shops_couponusemanager.sid,fee:store_shops_couponusemanager.fee,imgpath:store_shops_couponusemanager.imgpath,systime:store_shops_couponusemanager.systime,flags:store_shops_couponusemanager.flags,keyword:store_shops_couponusemanager.keyword,caExport:store_shops_couponusemanager.caExport};
						//store_shops_couponusemanager.currentPage=1;
						store_shops_couponusemanager.load();		
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('couponusemanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_couponusemanager,
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
		            text: "场所代码",
		            dataIndex: 'scode',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
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
		            text: "用户MAC",
		            dataIndex: 'usermac',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "活动ID",
		            dataIndex: 'aid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "活动名",
		            dataIndex: 'aname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "优惠劵面值",
		            dataIndex: 'fee',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "优惠劵编码",
		            dataIndex: 'serno',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "商品ID",
		            dataIndex: 'sid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "商品名称",
		            dataIndex: 'sname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "消费日期",
		            dataIndex: 'feetime',
		            width: 100,
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
		            text: "加入时间",
		            dataIndex: 'systime',
		            width: 150,
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
		                id:'couponusemanager_keyword',
		                name:'couponusemanager_keyword',
		                value:store_shops_couponusemanager.keyword,
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
			                    		var aa=Ext.getCmp('couponusemanager_keyword').value;
					                    if(aa!='输入用户登录名称'){
							       	            store_shops_couponusemanager.keyword=aa;
									                store_shops_couponusemanager.currentPage=1;
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
			                 var bb=Ext.getCmp('couponusemanager_keyword').value;
				               if(bb!='输入用户登录名称'){         
				                  store_shops_couponusemanager.keyword=bb;
				               }
				               else{
				                  store_shops_couponusemanager.keyword='';
				               }
				               store_shops_couponusemanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                text:'删除',
		                id: 'couponusemanager_removeButton',
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
												  store_shops_couponusemanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }	
		    });
		    store_shops_couponusemanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});