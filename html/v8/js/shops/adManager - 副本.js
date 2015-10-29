	/*上网时长设置*/
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

		
	 //高级查询窗口
	function gotoedit1(value, id){
		var win3 = null;
		var idletime = '';
		var timeout = '';
		var freetime = '';
		var intertime = '';
		var flags = '';
		var sid1 = '';
		var title='增加';
		if(value == 'update'){
		title='修改';
		idletime = store_shops_smsmanager.getAt(id).get('idletime');
		timeout = store_shops_smsmanager.getAt(id).get('timeout');
		freetime = store_shops_smsmanager.getAt(id).get('freetime');
		intertime = store_shops_smsmanager.getAt(id).get('intertime');
		flags = store_shops_smsmanager.getAt(id).get('flags');
		sid1 = store_shops_smsmanager.getAt(id).get('sid');
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
					id:'timelimit_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							{
	                        	fieldLabel:'商店名称',
	                        	xtype: 'textfield',
								emptyText:'请输入商店名称',
								value:intertime,
	                        	name:'intertime'
	                    	},
							{
	                        	fieldLabel:'购买短信数量',
	                        	xtype: 'textfield',
								emptyText:'请输入免认证时间',
								value:freetime,
	                        	name:'freetime'
	                    	},
							{
	                        	fieldLabel:'描述',
	                        	xtype: 'textfield',
								emptyText:'请输入空闲时间',
								value:idletime,
	                        	name:'idletime'
	                    	},
							
							{
	                        	xtype:'combo',
								name:'flags',
								fieldLabel:'标记',
								emptyText:'请选择标记',
								value:flags,
								triggerAction:'all',
								store:flagsStore,
								displayField:'groupname',
								valueField:'groupid',
								queryMode:'local',
								forceSelection:false,
								typeAhead:true,
								allowBlank:true
	                    	}
							]  
	                })],  
					        buttons:[{
										  text:"确定",
										  iconCls:'icon-save',		  
										  handler:function(){
						        			var objectTmp = Ext.getCmp("timelimit_form2").getForm().getValues();
											var intertime1 = objectTmp.intertime;
											var freetime1= objectTmp.freetime;
											var idletime1 = objectTmp.idletime;
											var timeout1 = objectTmp.timeout;
											var flags1 = objectTmp.flags;
											
											store_shops_smsmanager.load({params: {update:value,sid:sid1,intertime:intertime1,freetime:freetime1,idletime:idletime1,timeout:timeout1,flags:flags1}});
											//reflash();
											win3.close();
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
            dataIndex: 'sid',
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
                gotoedit1('update', rowIndex);
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
            dataIndex: 'flags',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
        	 flex: 1	
        }
          ],
            listeners:{
		'itemdblclick':function(grid,record,item,index,e){
			gotoedit1('update', index);
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
				emptyText:'请输入商店名称',
                value:store_shops_smsmanager.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入商店名称'){                
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
							store_shops_smsmanager.load({params: {del: "del",selsid:selsid}});
							
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