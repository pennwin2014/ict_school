	/*黑名单设置*/
	Ext.define('users.tagManager',{
    extend: 'Ext.grid.Panel',
	autoScroll: false,
    frame: false,
    initComponent : function(){
	if(store_users_tagmanager.alreadyload !=1){
    store_users_tagmanager.name = '';
    store_users_tagmanager.memo = '';
    store_users_tagmanager.validtime = '';
    store_users_tagmanager.utype = '';
    store_users_tagmanager.systime = '';
    store_users_tagmanager.keyword = '';
    store_users_tagmanager.caExport = ''; //导出

	store_users_tagmanager.new_params={keyword:store_users_tagmanager.keyword,caExport:store_users_tagmanager.caExport};
   
	}
 function reflash(){
	store_users_tagmanager.new_params={keyword:store_users_tagmanager.keyword,caExport:store_users_tagmanager.caExport};
	store_users_tagmanager.currentPage=1;
	store_users_tagmanager.load();
	
}

 var flagsStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'使用', groupid:'0'},
        		{groupname:'不使用', groupid:'1'}
        ]
		});

var typeStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'Username', groupid:'1'},
        		{groupname:'Mac', groupid:'2'}
        ]
		});
		
	 //高级查询窗口
	function gotoedit1(value, id){
		var win3 = null;
		var groupname = '';
		var dispname = '';
		var macbindtime = '';
		var idletime = '';
		var sessiontime = '';
		var title='增加';
		if(value == 'update'){
		title='修改';
		groupname = store_users_tagmanager.getAt(id).get('groupname');
		pregroupname = store_users_tagmanager.getAt(id).get('groupname');
		macbindtime = store_users_tagmanager.getAt(id).get('macbindtime');
		idletime = store_users_tagmanager.getAt(id).get('idletime');
		sessiontime = store_users_tagmanager.getAt(id).get('sessiontime');
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'黑名单设置窗口 - '+ title,
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
					id:'tagmanager_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							
							{
	                        	fieldLabel:'标签名称',
	                        	xtype: 'textfield',
								emptyText:'请输入用户登录名称',
								value:groupname,
	                        	name:'groupname'
	                    	},
							{
	                        	fieldLabel:'空闲超时时间(秒)',
	                        	xtype: 'textfield',
								emptyText:'请输入空闲超时时间',
								value:idletime,
	                        	name:'idletime'
	                    	},
							{
	                        	fieldLabel:'Mac地址绑定时间(秒)',
	                        	xtype: 'textfield',
								emptyText:'请输入Mac地址绑定时间',
								value:macbindtime,
	                        	name:'macbindtime'
	                    	},{
	                        	fieldLabel:'登录超时时间(秒)',
	                        	xtype: 'textfield',
								emptyText:'请输入登录超时时间',
								value:sessiontime,
	                        	name:'sessiontime'
	                    	}
							]  
	                })],  
					        buttons:[{
										  text:"确定",
										  iconCls:'icon-save',		  
										  handler:function(){
						        			var objectTmp = Ext.getCmp("tagmanager_form2").getForm().getValues();
											var groupname = objectTmp.groupname;
											var macbindtime = objectTmp.macbindtime;
											var idletime = objectTmp.idletime;
											var sessiontime = objectTmp.sessiontime;
											
											store_users_tagmanager.load({params: {update:value,pregroupname:pregroupname,groupname:groupname,macbindtime:macbindtime,idletime:idletime,sessiontime:sessiontime}});
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
                Ext.getCmp('tagmanager_removeButton').setDisabled(selections.length == 0);
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
        store: store_users_tagmanager,
        viewConfig:{
	        	loadMask : false
	        },
        columns:[
		{
            header: '编辑',
            xtype: 'actioncolumn',
            iconCls: 'tag_edit',
            tooltip: '点击进入修改界面',
            width: 50,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('update', rowIndex);
            }
        },
			{
            text: "标签名称",
            dataIndex: 'groupname',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "空闲超时时间（秒）",
            dataIndex: 'idletime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "Mac地址绑定时间（秒）",
            dataIndex: 'macbindtime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "登录超时时间(秒)",
            dataIndex: 'sessiontime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false,
            allowBlank:true
        },{
            text: "更新时间",
            dataIndex: 'moditime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false,
            allowBlank:true
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
                id:'tagmanager_keyword',
                name:'tagmanager_keyword',
				emptyText:'请输入用户登录名称/显示名称',
                value:store_users_tagmanager.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入用户登录名称/显示名称'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('tagmanager_keyword').value;
					       	    store_users_tagmanager.keyword=aa;
							    store_users_tagmanager.currentPage=1;
							    reflash();
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('tagmanager_keyword').value;
		               store_users_tagmanager.keyword=bb;
		               store_users_tagmanager.currentPage=1;         
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
				id:'tagmanager_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
							if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('groupname') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('groupname') + "'";
			                    }
			                }  
						store_users_tagmanager.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }
          ]
         }
        }
        
        );
	store_users_tagmanager.alreadyload = 1;
    this.callParent(arguments);
    }
})