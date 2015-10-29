/*消费管理*/
Ext.define('shops.consumpManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_consumpmanager.alreadyload != 1){
						store_shops_consumpmanager.cid = '';
						store_shops_consumpmanager.username = '';
						store_shops_consumpmanager.dispname = '';
						store_shops_consumpmanager.usermac = '';
						store_shops_consumpmanager.sname = '';
						store_shops_consumpmanager.sid = '';
						store_shops_consumpmanager.fee = '';
						store_shops_consumpmanager.date = '';
						store_shops_consumpmanager.uid = '';
						store_shops_consumpmanager.lasttime = '';
						store_shops_consumpmanager.plateid = plateid;
						store_shops_consumpmanager.keyword = '';
						store_shops_consumpmanager.caExport = '';
						store_shops_consumpmanager.new_params = {cid:store_shops_consumpmanager.cid,username:store_shops_consumpmanager.username,dispname:store_shops_consumpmanager.dispname,usermac:store_shops_consumpmanager.usermac,sname:store_shops_consumpmanager.sname,sid:store_shops_consumpmanager.sid,fee:store_shops_consumpmanager.fee,date:store_shops_consumpmanager.date,uid:store_shops_consumpmanager.uid,lasttime:store_shops_consumpmanager.lasttime,keyword:store_shops_consumpmanager.keyword,caExport:store_shops_consumpmanager.caExport};
				}	
				
				function reflash(){
						store_shops_consumpmanager.new_params = {cid:store_shops_consumpmanager.cid,username:store_shops_consumpmanager.username,dispname:store_shops_consumpmanager.dispname,usermac:store_shops_consumpmanager.usermac,sname:store_shops_consumpmanager.sname,sid:store_shops_consumpmanager.sid,fee:store_shops_consumpmanager.fee,date:store_shops_consumpmanager.date,uid:store_shops_consumpmanager.uid,lasttime:store_shops_consumpmanager.lasttime,keyword:store_shops_consumpmanager.keyword,caExport:store_shops_consumpmanager.caExport};
						//store_shops_consumpmanager.currentPage=1;
						store_shops_consumpmanager.load();		
				}
				
				var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('consumpmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_consumpmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "序号",
		            dataIndex: 'cid',
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
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "用户MAC",
		            dataIndex: 'usermac',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商品名称",
		            dataIndex: 'sname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商品ID",
		            dataIndex: 'sid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "售价",
		            dataIndex: 'fee',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "消费日期",
		            dataIndex: 'date',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "用户ID",
		            dataIndex: 'uid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "最后访问时间",
		            dataIndex: 'lasttime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
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
		                id:'consumpmanager_keyword',
		                name:'consumpmanager_keyword',
		                value:store_shops_consumpmanager.keyword,
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
			                    		var aa=Ext.getCmp('consumpmanager_keyword').value;
					                    if(aa!='输入用户登录名称'){
							       	            store_shops_consumpmanager.keyword=aa;
									                store_shops_consumpmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('consumpmanager_keyword').value;
				               if(bb!='输入用户登录名称'){         
				                  store_shops_consumpmanager.keyword=bb;
				               }
				               else{
				                  store_shops_consumpmanager.keyword='';
				               }
				               store_shops_consumpmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                text:'删除',
		                id: 'consumpmanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
		                handler:function(){
										   if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('cid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('cid') + "'";
					                    }
					                }
												  store_shops_consumpmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]	
		        }	
		    });
		    store_shops_consumpmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});