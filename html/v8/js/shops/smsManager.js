	
	/*短信支付管理*/
	Ext.define('shops.smsManager',{
    extend: 'Ext.grid.Panel',
	autoScroll: false,
    frame: false,
    initComponent : function(){
	if(store_shops_smsmanager.alreadyload !=1){
	store_shops_smsmanager.sid = '';
    store_shops_smsmanager.intertime = '';
    store_shops_smsmanager.freetime = '';
    store_shops_smsmanager.idletime = '';
    store_shops_smsmanager.timeout = '';
    store_shops_smsmanager.systime = '';
    store_shops_smsmanager.flags = '';
    store_shops_smsmanager.keyword = '';
    store_shops_smsmanager.caExport = ''; //导出
	
	
	store_shops_smsmanager.new_params={sid:store_shops_smsmanager.sid,intertime:store_shops_smsmanager.intertime,freetime:store_shops_smsmanager.freetime,idletime:store_shops_smsmanager.idletime,timeout:store_shops_smsmanager.timeout,systime:store_shops_smsmanager.systime,flags:store_shops_smsmanager.flags,smsflag:store_shops_smsmanager.smsflag,keyword:store_shops_smsmanager.keyword,caExport:store_shops_smsmanager.caExport};
   
	}
 function reflash(){
	store_shops_smsmanager.new_params={sid:store_shops_smsmanager.sid,intertime:store_shops_smsmanager.intertime,freetime:store_shops_smsmanager.freetime,idletime:store_shops_smsmanager.idletime,timeout:store_shops_smsmanager.timeout,systime:store_shops_smsmanager.systime,flags:store_shops_smsmanager.flags,smsflag:store_shops_smsmanager.smsflag,keyword:store_shops_smsmanager.keyword,caExport:store_shops_smsmanager.caExport};
	store_shops_smsmanager.currentPage=1;
	store_shops_smsmanager.load();
	
}

 var flagsStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'未充值', groupid:'0'},
        		{groupname:'已充值', groupid:'1'}
        ]
		});
	function columnBackground(value){
	
	if(value == 0){
	return "未充值" ;
	}
	else{
	return "已充值" ;
	}
	}
	Ext.define('EimData', {
        extend: 'Ext.data.Model',
        fields: [
           'userid','username'
        ],
        idProperty: 'userid'
    });
		var storeStore = Ext.create('Ext.data.Store', {
		        model: 'EimData',
				//pageSize: 5000,
		        autoLoad:true,
						proxy:{
							type:'ajax',
							url:'/pronline/Msg?FunName@ncm_groupInfo',
							reader:{
								type:'json',
								root:'eimdata'
							}
						}
		    });
		storeStore.load();

			
		    
	 //高级查询窗口
	function gotoedit1(value, index){
		var win3 = null;
		var id='';
		var desc = '';
		var count = '';
		var storeValue = '';
		var moneyflag = '';
		var title='增加';
		if(value == 'modi'){
		title='修改';
		desc = store_shops_smsmanager.getAt(index).get('desc');
		storeValue = store_shops_smsmanager.getAt(index).get('sid');
		moneyflag = store_shops_smsmanager.getAt(index).get('moneyflag');
		desc = store_shops_smsmanager.getAt(index).get('desc');
		count = store_shops_smsmanager.getAt(index).get('count');
		id = store_shops_smsmanager.getAt(index).get('id');
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'短信管理窗口 - '+ title,
					    closeAction: 'hide',
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
					id:'buysms_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							{
								id:'smsmanager_name',
								xtype:'combobox',
								fieldLabel: '商家名称',
								labelWidth: 120,
								valueField:'userid',
								emptyText:'All',
								value:storeValue,
								displayField: 'username',
								name : 'name',      
								allowBlank: true,
								store: storeStore,
								triggerAction: 'all',
								queryMode: 'local',
								typeAhead: true
							},
							{
	                        	fieldLabel:'购买短信数量(条)',
	                        	xtype: 'numberfield',
								emptyText:'请输入想要购买短信数量',
								value:count,
	                        	name:'count'
	                    	},
							{
	                        	fieldLabel:'描述',
	                        	xtype: 'textfield',
								emptyText:'请输入描述内容',
								value:desc,
	                        	name:'desc'
	                    	},
							{
	                        	fieldLabel:'标记',
	                        	xtype: 'combobox',
								emptyText:'请输入描述内容',
								store:flagsStore,
								valueField:'groupid',
								displayField: 'groupname',
								value:moneyflag,
								
	                        	name:'moneyflag'
	                    	}
							]  
	                })],  
					        buttons:[{
										  text:"确定",
										  iconCls:'icon-save',		  
										  handler:function(){
						        			var objectTmp = Ext.getCmp("buysms_form2").getForm().getValues();
											var intertime1 = objectTmp.intertime;
											var moneyflag1= objectTmp.moneyflag;
											var desc1 = objectTmp.desc;
											var count1= objectTmp.count;
											var name1 = Ext.getCmp("smsmanager_name").getRawValue();
											var sid1 = Ext.getCmp("smsmanager_name").getValue();
											store_shops_smsmanager.load({params: {update:value,id:id,moneyflag:moneyflag1,count:count1,desc:desc1,sid:sid1,name:name1},
											callback:function(records, options, success){ 
											reflash();
											win3.close();
											}
											});
											
											
											
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

	var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {
                Ext.getCmp('timelimit_removeButton').setDisabled(selections.length == 0);
            }
        }
    });	
		
    Ext.apply(this,       
        {
        border:false,
		frame:false,
		//height: Ext.get("layout_center").getHeight()-58,
		//height:Ext.get("layout_center").getHeight(),
		autoScroll: true,
		selModel: sm,
		//height: grid_height,
        store: store_shops_smsmanager,
        viewConfig:{
	        	loadMask : false
	        },
        columns:[
		{
            text: "序号",
            dataIndex: 'id',
            width: 100,
            //align: 'center',
            hidden: true,
            sortable: true
        },{
            header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'sid',
            iconCls: 'report_edit',
            tooltip: '点击进入修改界面',
            width: 50,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('modi', rowIndex);
            }
        },{
            text: "商店名称",
            dataIndex: 'name',
            width: 150,
            align: 'left',
            hidden: false,
            sortable: false
        },{
            text: "购买短信数量",
            dataIndex: 'count',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "描述",
            dataIndex: 'desc',
            width: 150,
            align: 'left',
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
            text: "标记",
            dataIndex: 'moneyflag',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false,
			renderer:columnBackground
        },{
        	 flex: 1	
        }
          ],
            listeners:{
		'itemdblclick':function(grid,record,item,index,e){
			gotoedit1('modi', index);
		}
		},  
           columnLines: true,
           tbar:{items:[
            '-',{
            	labelWidth: 40,
            	width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'timelimit_keyword',
                name:'timelimit_keyword',
				emptyText:'请输入商店名称/描述',
                value:store_shops_smsmanager.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入商店名称/描述'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('timelimit_keyword').value;
					       	    store_shops_smsmanager.keyword=aa;
							    store_shops_smsmanager.currentPage=1;
							    reflash();
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('timelimit_keyword').value;
		               store_shops_smsmanager.keyword=bb;
		               store_shops_smsmanager.currentPage=1;         
		               reflash();  
                }
            },'-',{
                 text:'增加',
                 iconCls:'add',
	               handler : function() {
	               	  gotoedit1('add', '');
	               }
            },'-', {
                text:'删除',
				id:'timelimit_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
							if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;
							var selmoneyflag;
			                var rows=sm.getSelection();
			                selsid='';
			                selmoneyflag='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('id') + "'";
			                  	   selmoneyflag="'" + rows[i].get('moneyflag') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('id') + "'";
			                       selmoneyflag=selmoneyflag + "," + "'" + rows[i].get('moneyflag') + "'";
			                    }
			                }
							//alert(selmoneyflag);
							if(selmoneyflag.indexOf("1")>0){alert("已经充值，不能删除改记录！")}else{
							store_shops_smsmanager.load({params: {update: "del",id:selsid},
							callback:function(records, options, success){ 
									reflash();
								}
							});
							}
                   }
                }
            }
          ]
         }
        }
        
        );
	store_shops_smsmanager.alreadyload = 1;
    this.callParent(arguments);
    }
})