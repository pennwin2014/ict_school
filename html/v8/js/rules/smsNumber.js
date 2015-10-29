/*国内外短信数量限制*/
Ext.define('rules.smsNumber',{
    extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
		if(store_rules_smsnumber.alreadyload !=1){
			  store_rules_smsnumber.sid = '';
		    store_rules_smsnumber.internum = '';
		    store_rules_smsnumber.localnum = '';
		    store_rules_smsnumber.stime = '';
		    store_rules_smsnumber.etime = '';
		    store_rules_smsnumber.systime = '';
		    store_rules_smsnumber.flags = '';
		    store_rules_smsnumber.smsflag = '';
		    store_rules_smsnumber.keyword = '';
		    store_rules_smsnumber.caExport = ''; //导出
			
				store_rules_smsnumber.new_params={sid:store_rules_smsnumber.sid,internum:store_rules_smsnumber.internum,localnum:store_rules_smsnumber.localnum,stime:store_rules_smsnumber.stime,etime:store_rules_smsnumber.etime,systime:store_rules_smsnumber.systime,flags:store_rules_smsnumber.flags,smsflag:store_rules_smsnumber.smsflag,keyword:store_rules_smsnumber.keyword,caExport:store_rules_smsnumber.caExport};
   
		}
		function reflash(){
				store_rules_smsnumber.new_params={sid:store_rules_smsnumber.sid,internum:store_rules_smsnumber.internum,localnum:store_rules_smsnumber.localnum,stime:store_rules_smsnumber.stime,etime:store_rules_smsnumber.etime,systime:store_rules_smsnumber.systime,flags:store_rules_smsnumber.flags,smsflag:store_rules_smsnumber.smsflag,keyword:store_rules_smsnumber.keyword,caExport:store_rules_smsnumber.caExport};
				store_rules_smsnumber.currentPage=1;
				store_rules_smsnumber.load();
		
		}

		var flagsStore = Ext.create('Ext.data.Store', {
		    fields:['groupname', 'groupid'],
		    data:[
		    		{groupname:'使用', groupid:'0'},
		    		{groupname:'不使用', groupid:'1'}
		    ]
		});
		var smsflagsStore = Ext.create('Ext.data.Store', {
		    fields:['groupname', 'groupid'],
		    data:[
		    		{groupname:'使用', groupid:'0'},
		    		{groupname:'不使用', groupid:'1'}
		    ]
		});

	 //高级查询窗口
	function gotoedit1(value, id){
		var win3 = null;
		var stime = '';
		var etime = '';
		var localnum = '';
		var internum = '';
		var flags = '';
		var smsflags = '';
		var sid1 = '';
		var title='增加';
		if(value == 'update'){
		title='修改';
		stime = store_rules_smsnumber.getAt(id).get('stime');
		etime = store_rules_smsnumber.getAt(id).get('etime');
		smsflags = store_rules_smsnumber.getAt(id).get('smsflag');
		localnum = store_rules_smsnumber.getAt(id).get('localnum');
		internum = store_rules_smsnumber.getAt(id).get('internum');
		flags = store_rules_smsnumber.getAt(id).get('flags');
		sid1 = store_rules_smsnumber.getAt(id).get('sid');
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'国内外短信数量限制设置窗口 - '+ title,
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
					id:'smsnumber_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							{
	                        	fieldLabel:'国内短信限制数量',
	                        	xtype: 'textfield',
								emptyText:'请输入国内短信限制数量',
								value:localnum,
	                        	name:'localnum'
	                    	},
							{
	                        	fieldLabel:'国外短信限制数量',
	                        	xtype: 'textfield',
								emptyText:'请输入国外短信限制数量',
								value:internum,
	                        	name:'internum'
	                    	},
							{
	                    			fieldLabel:'开始时间',
	                    			xtype:'timefield',
									emptyText:'请选择开始时间',
						            value:stime,
						            name:'smsnumber_stimed',
						            labelSeparator:':', //分隔符
									msgTarget:'side', //在字段的右边显示一个提示信息  
									autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
									maxValue:'23:59:59', //最大时间
									maxText:'时间应小于{0}', //大于最大时间的提示信息
									minValue:'00:00:00', //最小时间  
									minText:'时间应大于{0}', //小于最小时间的提示信息
									pickerMaxHeight:100, //下拉列表的最大高度
									increment:1, //时间间隔为60分钟 
									format: "H:i:s",
									invalidText:'时间格式无效'
	                    	},{
	                    			fieldLabel:'结束时间',
	                    			xtype:'timefield',
									emptyText:'请选择结束时间',
						            value:etime,
						            name:'smsnumber_etimed',
						            labelSeparator:':', //分隔符
									msgTarget:'side', //在字段的右边显示一个提示信息  
									autoFitErrors:true, //展示错误信息时是否自动调整字段组件宽度
									maxValue:'23:59:59', //最大时间
									maxText:'时间应小于{0}', //大于最大时间的提示信息
									minValue:'00:00:00', //最小时间  
									minText:'时间应大于{0}', //小于最小时间的提示信息
									pickerMaxHeight:100, //下拉列表的最大高度
									increment:1, //时间间隔为60分钟 
									format: "H:i:s",
									invalidText:'时间格式无效'
	                      },{
	                        	xtype:'combo',
								name:'smsflags',
								fieldLabel:'短信标记',
								emptyText:'请选择短信标记',
								value:smsflags,
								triggerAction:'all',
								store:smsflagsStore,
								displayField:'groupname',
								valueField:'groupid',
								queryMode:'local',
								forceSelection:false,
								typeAhead:true,
								allowBlank:true
	                    	},{
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
						        			var objectTmp = Ext.getCmp("smsnumber_form2").getForm().getValues();
											var internum1 = objectTmp.internum;
											var localnum1= objectTmp.localnum;
											var stime1 = objectTmp.smsnumber_stimed;
											var etime1 = objectTmp.smsnumber_etimed;
											var flags1 = objectTmp.flags;
											var smsflag1 = objectTmp.smsflags;
											
											store_rules_smsnumber.load({params: {update:value,sid:sid1,localnum:localnum1,internum:internum1,stime:stime1,etime:etime1,flags:flags1,smsflag:smsflag1}});
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
                Ext.getCmp('smsnumber_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_smsnumber,
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
			sortable: false,
            width: 50,
            align: 'center',
			items:[
			{
			iconCls: 'report_edit',
			tooltip: '点击进入修改界面',
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('update', rowIndex);
            }}
			
			]
        },{
            text: "国内短信限制数量",
            dataIndex: 'localnum',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "国际短信限制数量",
            dataIndex: 'internum',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "短信标记",
            dataIndex: 'smsflag',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "限制时间",
            columns:[{
            		text:'开始时间',
            		dataIndex:'stime',
            		width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
            },{
            		text:'结束时间',
            		dataIndex:'etime',
            		width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
            }]
        },{
            text: "加入时间",
            dataIndex: 'systime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "标记",
            dataIndex: 'flags',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: true,
            editor: {
            	  xtype:'combo',
            		id:'flagsbo',
            		triggerAction:'all',
            		store:flagsStore,
            		displayField:'groupname',
            		valueField:'groupid',
            		queryMode:'local',
            		forceSelection:false,
            		typeAhead:true,
            		allowBlank:true
           	}
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
                id:'smsnumber_keyword',
                name:'smsnumber_keyword',
				emptyText:'请输入国内/国际短信限制数量',
                value:store_rules_smsnumber.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入国内/国际短信限制数量'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('smsnumber_keyword').value;
					       	            store_rules_smsnumber.keyword=aa;
							            store_rules_smsnumber.currentPage=1;
							            reflash();
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('smsnumber_keyword').value;
		               store_rules_smsnumber.keyword=bb;
		               store_rules_smsnumber.currentPage=1;         
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
				id:'smsnumber_removeButton',
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
							store_rules_smsnumber.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }
          ]
         }
        }
        
        );
	store_rules_smsnumber.alreadyload = 1;
    this.callParent(arguments);
    }
})