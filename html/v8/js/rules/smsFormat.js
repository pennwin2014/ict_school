	/*短信格式设置*/
	Ext.define('rules.smsFormat',{
    extend: 'Ext.grid.Panel',
	autoScroll: false,
    frame: false,
    initComponent : function(){
	if(store_rules_smsformat.alreadyload !=1){
	store_rules_smsformat.sid = '';
    store_rules_smsformat.intertime = '';
    store_rules_smsformat.freetime = '';
    store_rules_smsformat.idletime = '';
    store_rules_smsformat.timeout = '';
    store_rules_smsformat.systime = '';
    store_rules_smsformat.flags = '';
    store_rules_smsformat.keyword = '';
    store_rules_smsformat.caExport = ''; //导出
	
	
	store_rules_smsformat.new_params={sid:store_rules_smsformat.sid,intertime:store_rules_smsformat.intertime,freetime:store_rules_smsformat.freetime,idletime:store_rules_smsformat.idletime,timeout:store_rules_smsformat.timeout,systime:store_rules_smsformat.systime,flags:store_rules_smsformat.flags,smsflag:store_rules_smsformat.smsflag,keyword:store_rules_smsformat.keyword,caExport:store_rules_smsformat.caExport};
   
	}
 function reflash(){
	store_rules_smsformat.new_params={sid:store_rules_smsformat.sid,intertime:store_rules_smsformat.intertime,freetime:store_rules_smsformat.freetime,idletime:store_rules_smsformat.idletime,timeout:store_rules_smsformat.timeout,systime:store_rules_smsformat.systime,flags:store_rules_smsformat.flags,smsflag:store_rules_smsformat.smsflag,keyword:store_rules_smsformat.keyword,caExport:store_rules_smsformat.caExport};
	store_rules_smsformat.currentPage=1;
	store_rules_smsformat.load();
	
}

 var flagsStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'使用', groupid:'0'},
        		{groupname:'不使用', groupid:'1'}
        ]
		});
  var languageStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'中文', groupid:'0'},
        		{groupname:'英文', groupid:'1'}
        ]
		});
		
	 //高级查询窗口
	function gotoedit1(value, id){
		var win3 = null;
		var content = '';
		var language = '';
		var flags = '';
		var sid1 = '';
		var title='增加';
		if(value == 'update'){
		title='修改';
		content = store_rules_smsformat.getAt(id).get('content');
		language = store_rules_smsformat.getAt(id).get('language');
		flags = store_rules_smsformat.getAt(id).get('flags');
		sid1 = store_rules_smsformat.getAt(id).get('sid');
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'短信格式设置窗口 - '+ title,
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
					id:'smsformat_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							{
	                        	fieldLabel:'短信格式',
	                        	xtype: 'textfield',
								emptyText:'请输入短信格式',
								value:content,
	                        	name:'content'
	                    	},{
	                        	xtype:'combo',
								name:'language',
								fieldLabel:'语言',
								emptyText:'请选择语言',
								value:language,
								triggerAction:'all',
								store:languageStore,
								displayField:'groupname',
								valueField:'groupid',
								queryMode:'local',
								forceSelection:false,
								typeAhead:true,
								allowBlank:true
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
						        			var objectTmp = Ext.getCmp("smsformat_form2").getForm().getValues();
											var content1 = objectTmp.content;
											var language1 = objectTmp.language;
											var flags1 = objectTmp.flags;
											
											store_rules_smsformat.load({params: {update:value,sid:sid1,content:content1,language:language1,flags:flags1}});
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
                Ext.getCmp('smsformat_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_smsformat,
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
            text: "短信格式",
            dataIndex: 'content',
            width: 400,
            //align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "语言",
            dataIndex: 'language',
            width: 100,
            //align: 'right',
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
            text: " 标记",
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
                id:'smsformat_keyword',
                name:'smsformat_keyword',
				emptyText:'请输入短信格式',
                value:store_rules_smsformat.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入短信格式'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('smsformat_keyword').value;
					       	    store_rules_smsformat.keyword=aa;
							    store_rules_smsformat.currentPage=1;
							    reflash();
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('smsformat_keyword').value;
		               store_rules_smsformat.keyword=bb;
		               store_rules_smsformat.currentPage=1;         
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
				id:'smsformat_removeButton',
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
							store_rules_smsformat.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }
          ]
         }
        }
        
        );
	store_rules_smsformat.alreadyload = 1;
    this.callParent(arguments);
    }
})