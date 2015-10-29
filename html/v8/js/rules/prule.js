/*模板参数设置*/
Ext.define('rules.prule',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
	  if(store_rules_prule.alreadyload !=1){
		    store_rules_prule.ruleid = '';
		    store_rules_prule.priority = '';
		    store_rules_prule.groupid = '';
		    store_rules_prule.ssid = '';
		    store_rules_prule.nasid = '';
		    store_rules_prule.mark = '';
		    store_rules_prule.dotime = '';
		    store_rules_prule.dateid = '';
		    store_rules_prule.starttime = '';
		    store_rules_prule.endtime = '';
		    store_rules_prule.addtime = '';
		    store_rules_prule.moditime = '';
		    store_rules_prule.status = '';
		    store_rules_prule.groupcode = '';
		    store_rules_prule.keyword = '';
		    store_rules_prule.caExport = ''; //导出
		
			  store_rules_prule.new_params={ruleid:store_rules_prule.ruleid,priority:store_rules_prule.priority,groupid:store_rules_prule.groupid,ssid:store_rules_prule.ssid,nasid:store_rules_prule.nasid,mark:store_rules_prule.mark,dotime:store_rules_prule.dotime,dateid:store_rules_prule.dateid,starttime:store_rules_prule.starttime,endtime:store_rules_prule.endtime,addtime:store_rules_prule.addtime,moditime:store_rules_prule.moditime,status:store_rules_prule.status,groupcode:store_rules_prule.groupcode,keyword:store_rules_prule.keyword,caExport:store_rules_prule.caExport};
   
	  }
	  
    function reflash(){
				store_rules_prule.new_params={ruleid:store_rules_prule.ruleid,priority:store_rules_prule.priority,plateid:store_rules_prule.plateid,groupid:store_rules_prule.groupid,ssid:store_rules_prule.ssid,nasid:store_rules_prule.nasid,mark:store_rules_prule.mark,dotime:store_rules_prule.dotime,dateid:store_rules_prule.dateid,starttime:store_rules_prule.starttime,endtime:store_rules_prule.endtime,addtime:store_rules_prule.addtime,moditime:store_rules_prule.moditime,status:store_rules_prule.status,groupcode:store_rules_prule.groupcode,keyword:store_rules_prule.keyword,caExport:store_rules_prule.caExport};
				//store_rules_prule.currentPage=1;
				store_rules_prule.load();
    }
    
    Ext.define('groupData', {
        extend: 'Ext.data.Model',
        fields: ['groupname', 'dispname']
    });
	  var store_group = Ext.create('Ext.data.Store', {
	  	 model:'groupData',
	  	 pageSize: 10000,
	  	 autoLoad:true,
	  	 proxy:{
	  	 		type:'ajax',
	  	 		url:'/pronline/Msg?FunName@ncm_websrvgroup_list',
	  	 		reader:{
	  	 				type:'json',
	  	 				root:'eimdata'	
	  	 		},
	  	 		simplleSortMode:true	
	  	 }
	  });
	  
	  var dateStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'每天', groupid:'0'},
        		{groupname:'其他', groupid:'1'}
        ]
		});
    
    var statusStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'停用', groupid:'0'},
        		{groupname:'启用', groupid:'1'}
        ]
		});

	  //高级查询窗口
    function gotoedit1(value, id){
				var win3 = null;
				var ruleid1 = '';
				var priority = '';
				var plateid1 = store_rules_prule.plateid;
				var groupid = '';
				var ssid = '';
				var nasid = '';
				var mark = '';
				var dotime = '';
				var dateid = '0';
				var starttime = '';
				var endtime = '';
				var addtime1 = '';
				var statusd = '1';
				var groupcode = '';
				var title='增加';
				
				if(value == 'update'){
						title='修改';
						ruleid1 = store_rules_prule.getAt(id).get('ruleid');
						priority = store_rules_prule.getAt(id).get('priority');
						groupid = store_rules_prule.getAt(id).get('groupid');
						ssid = store_rules_prule.getAt(id).get('ssid');
						nasid = store_rules_prule.getAt(id).get('nasid');
						mark = store_rules_prule.getAt(id).get('mark');
						dotime = store_rules_prule.getAt(id).get('dotime');
						dateid = store_rules_prule.getAt(id).get('dateid');
						starttime = store_rules_prule.getAt(id).get('starttime');
						endtime = store_rules_prule.getAt(id).get('endtime');
						statusd = store_rules_prule.getAt(id).get('status');
						plateid1 = store_rules_prule.getAt(id).get('plateid');
						addtime1 = store_rules_prule.getAt(id).get('addtime');
						groupcode = store_rules_prule.getAt(id).get('groupcode');
				}
		
	      if (!win3){ //判断如果不存在就创建新的
	        win3 = new Ext.Window({
					    title:'模板参数设置窗口 - '+ title,
					    closeAction: 'hide',
					    layout:'form',  
					    width:450,
					    draggable:true, //可拖动的
						  maximizable:true, //可最大化的
				      //resizable: true, //可改变大小
						  modal: false,//后面的内容可以操作
				      //plain：true,//则主体背景透明
				      //items: fp2
					    constrain:true, //限制窗口只能在其容器内移动
				      //minimizable:true, //可最小化
				      defaultType:'textfield', //设置表单字段的默认类型  
							fieldDefaults:{ //统一设置表单字段默认属性
									labelSeparator:':', //分隔符
									labelWidth:100, //标签宽度
									width:350, //字段宽度
									msgTarget:'side'
							},
	            items:[new Ext.FormPanel({
									id:'prule_form2',
									layout:"form",
									baseCls:"x-plain",
									bodyPadding: 5,
									items:[{
											xtype:'numberfield',
											fieldLabel:'优先级',
											name:'priority',
											value:priority,
											emptyText:'请选择优先级',
											hideTrigger:false, //隐藏微调按钮
					        		allowDecimals:false, //不允许输入小数
					        		maxValue:100, //最大值
					        		minvalue:0, //最小值
					        		nanText:'请输入有效范围内的整数' //无效数字提示	
									},{
                      xtype:'combo',
											name:'groupcode',
											fieldLabel:'单位',
											emptyText:'请选择单位',
											value:groupcode,
											triggerAction:'all',
											store:store_group,
											displayField:'dispname',
											valueField:'groupname',
											queryMode:'local',
											//forceSelection:false,
											typeAhead:true,
											allowBlank:true
                  },{
                    	fieldLabel:'SSID',
                    	xtype: 'textfield',
											emptyText:'请输入SSID',
											value:ssid,
                    	name:'ssid'
	                },{
                    	fieldLabel:'接入设备',
                    	xtype: 'textfield',
											emptyText:'请输入接入设备',
											value:nasid,
                    	name:'nasid'
	                },{
					            fieldLabel:'接入设备说明',
					            xtype:'displayfield',
					            name:'na',
					            value:'可以为多个，可以用通配符表示，如：*aa*,??aa*'
					        },{
                    	fieldLabel:'标签',
                    	xtype: 'textfield',
											emptyText:'请输入标签',
											value:mark,
                    	name:'mark'
	                },{
					            fieldLabel:'标签加载说明',
					            xtype:'displayfield',
					            name:'mk',
					            value:'可以多个，每个之间用逗号分隔，如：景点,外卖'
					        },{
                      xtype:'combo',
											name:'dateid',
											fieldLabel:'投放日期',
											emptyText:'请选择投放日期',
											value:dateid,
											triggerAction:'all',
											store:dateStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											//forceSelection:false,
											typeAhead:true,
											allowBlank:true,
											listeners:{
													afterrender:function(){
															if(this.value == '0'){
																  Ext.getCmp("prule_form2").getForm().findField('dotime').setVisible(true);
																  Ext.getCmp("prule_form2").getForm().findField('dt').setVisible(true);
															}else{
																	Ext.getCmp("prule_form2").getForm().findField('dotime').setVisible(false);
																  Ext.getCmp("prule_form2").getForm().findField('dt').setVisible(false);	
															}	
													},
													
													select : function(combo,record,index){ 
															if(this.value == '0'){
																  Ext.getCmp("prule_form2").getForm().findField('dotime').setVisible(true);
																  Ext.getCmp("prule_form2").getForm().findField('dt').setVisible(true);
															}else{
																	Ext.getCmp("prule_form2").getForm().findField('dotime').setVisible(false);
																  Ext.getCmp("prule_form2").getForm().findField('dt').setVisible(false);	
															}		
													}
											}
                  },{
                    	fieldLabel:'投放时间段',
                    	xtype: 'textfield',
											emptyText:'请输入投放时间段',
											//id:'prule_dotime',
											value:dotime,
                    	name:'dotime'
	                },{
					            fieldLabel:'投放时间段说明',
					            xtype:'displayfield',
					            //id:'prule_dt',
					            name:'dt',
					            value:'投放时间段如：08:00:00-11:30:00,13:00:00-16:00:00'
					        },{
					        	  fieldLabel:'开始时间',
					          	xtype: 'datefield',
					          	format: "Y/m/d",
					          	value:starttime,
					          	name:'starttime',
							        allowBlank:true //是否允许为空
					        },{
					        	  fieldLabel:'结束时间',
					          	xtype: 'datefield',
					          	format: "Y/m/d",
					          	value:endtime,
					          	name:'endtime',
							        allowBlank:true //是否允许为空
					        },{
                      xtype:'combo',
											name:'status',
											fieldLabel:'状态',
											emptyText:'请选择状态',
											value:statusd,
											triggerAction:'all',
											store:statusStore,
											displayField:'groupname',
											valueField:'groupid',
											queryMode:'local',
											forceSelection:false,
											typeAhead:true,
											allowBlank:true
                  }]  
	            })],  
					    buttons:[{
								  text:"确定",
								  iconCls:'icon-save',		  
								  handler:function(){
				        			var objectTmp = Ext.getCmp("prule_form2").getForm().getValues();
											var priority1 = objectTmp.priority;
											var groupcode1 = objectTmp.groupcode;
											var ssid1 = objectTmp.ssid;
											if(objectTmp.nasid != ''){
													var nasid1 = encodeURIComponent(objectTmp.nasid);
										  }else{
										  		var nasid1 = '';	
										  }
											var mark1 = objectTmp.mark;
											var dotime1 = objectTmp.dotime;
											var dateid1 = objectTmp.dateid;
											var starttime1 = objectTmp.starttime;
											var endtime1 = objectTmp.endtime;
											var status1 = objectTmp.status;
									
											store_rules_prule.load({params: {update:value,ruleid:ruleid1,priority:priority1,plateid:plateid1,groupcode:groupcode1,nasid:nasid1,ssid:ssid1,mark:mark1,dateid:dateid1,dotime:dotime1,starttime:starttime1,endtime:endtime1,addtime:addtime1,status:status1}});
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
                Ext.getCmp('prule_removeButton').setDisabled(selections.length == 0);
            }
        }
    });	
		
    Ext.apply(this, {
        border:false,
				frame:false,
				//height: Ext.get("layout_center").getHeight()-58,
				//height:Ext.get("layout_center").getHeight(),
				autoScroll: true,
				selModel: sm,
				//height: grid_height,
        store: store_rules_prule,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            text: "序号",
            dataIndex: 'ruleid',
            width: 100,
            //align: 'center',
            hidden: true,
            sortable: true
        },{
            header: '编辑',
            xtype: 'actioncolumn',
            iconCls: 'edit',
            tooltip: '点击进入修改界面',
            width: 50,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('update', rowIndex);
            }
        },{
            text: "模板名称",
            dataIndex: 'platename',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "优先级",
            dataIndex: 'priority',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "单位名称",
            dataIndex: 'dispname',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "SSID",
            dataIndex: 'ssid',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "接入设备ID",
            dataIndex: 'nasid',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "标签",
            dataIndex: 'mark',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "投放时间段",
            dataIndex: 'dotime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "投放日期",
            dataIndex: 'datename',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "开始时间",
            dataIndex: 'starttime',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "结束时间",
            dataIndex: 'endtime',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "加入时间",
            dataIndex: 'addtime',
            width: 150,
            //align: 'center',
            hidden: true,
            sortable: true
        },{
            text: "修改时间",
            dataIndex: 'moditime',
            width: 150,
            //align: 'left',
            hidden: true,
            sortable: false
        },{
            text: "状态",
            dataIndex: 's_status',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
        	 flex: 1	
        }],
         
        columnLines: true,
        tbar: Ext.create('Ext.PagingToolbar', {
            store: store_rules_prule,
            displayInfo: true,
            displayMsg: '显示 <font color=red>{0}</font> - <font color=red>{1}</font> 条，  共 <font color=red>{2}</font> 条',
            emptyMsg: "共 0 条",
            plugins: Ext.create('Ext.ux.ProgressBarPager', {}),
            
        		items:[
            '-',{
            		//labelWidth: 45,
            		width:160,
                //fieldLabel:'关键字',
                xtype: 'textfield',
                id:'prule_keyword',
                name:'prule_keyword',
				        emptyText:'请输入模板名称',
                value:store_rules_prule.keyword,
                enableKeyEvents:true,
                listeners:{
		                'focus':function(){
				                 if(this.value=='请输入模板名称'){                
				                    this.setValue('');
				                }
										},
		                'keydown' : function(i,e){
		                    var aae=e.getKey(); 
		                    if(aae==13){
		                    		var aa=Ext.getCmp('prule_keyword').value;
						       	    		store_rules_prule.keyword=aa;
								    				store_rules_prule.currentPage=1;
								    				reflash();
		                    }
		                }
                }
            },'-',{
                text:'查询',
            		iconCls:'search2',
                handler:function(){
	                 var bb=Ext.getCmp('prule_keyword').value;
		               store_rules_prule.keyword=bb;
		               store_rules_prule.currentPage=1;
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
								id:'prule_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
								if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('ruleid') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('ruleid') + "'";
			                    }
			                }  
											store_rules_prule.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }]
        })
    });
    reflash();
		store_rules_prule.alreadyload = 1;
    this.callParent(arguments);
    }
})
