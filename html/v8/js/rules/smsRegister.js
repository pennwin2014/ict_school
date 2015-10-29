	/*短信注册数量限制*/
	Ext.define('rules.smsRegister',{
    extend: 'Ext.grid.Panel',
	autoScroll: false,
    frame: false,
    initComponent : function(){
	if(store_rules_smsregister.alreadyload !=1){
	store_rules_smsregister.sid = '';
    store_rules_smsregister.regnum = '';
    store_rules_smsregister.stime = '';
    store_rules_smsregister.etime = '';
    store_rules_smsregister.systime = '';
    store_rules_smsregister.flags = '';
    store_rules_smsregister.keyword = '';
    store_rules_smsregister.caExport = ''; //导出
	
	store_rules_smsregister.new_params={sid:store_rules_smsregister.sid,regnum:store_rules_smsregister.regnum,stime:store_rules_smsregister.stime,etime:store_rules_smsregister.etime,systime:store_rules_smsregister.systime,flags:store_rules_smsregister.flags,keyword:store_rules_smsregister.keyword,caExport:store_rules_smsregister.caExport};
   
	}
 function reflash(){
	store_rules_smsregister.new_params={sid:store_rules_smsregister.sid,regnum:store_rules_smsregister.regnum,stime:store_rules_smsregister.stime,etime:store_rules_smsregister.etime,systime:store_rules_smsregister.systime,flags:store_rules_smsregister.flags,keyword:store_rules_smsregister.keyword,caExport:store_rules_smsregister.caExport};
	store_rules_smsregister.currentPage=1;
	store_rules_smsregister.load();
	
}

 var flagsStore = Ext.create('Ext.data.Store', {
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
		var regnum = '';
		var flags = '';
		var sid1 = '';
		var title='增加';
		if(value == 'update'){
		title='修改';
		stime = store_rules_smsregister.getAt(id).get('stime');
		etime = store_rules_smsregister.getAt(id).get('etime');
		regnum = store_rules_smsregister.getAt(id).get('regnum');
		flags = store_rules_smsregister.getAt(id).get('flags');
		sid1 = store_rules_smsregister.getAt(id).get('sid');
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'短信注册数量限制设置窗口 - '+ title,
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
					id:'smsregister_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
							{
	                        	fieldLabel:'短信注册限制数量',
	                        	xtype: 'textfield',
								emptyText:'请请输入短信注册限制数量',
								value:regnum,
	                        	name:'regnum'
	                    	},
							{
	                    			fieldLabel:'开始时间',
	                    			xtype:'timefield',
									emptyText:'请选择开始时间',
						            value:stime,
						            name:'smsregister_stimed',
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
						            name:'smsregister_etimed',
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
						        			var objectTmp = Ext.getCmp("smsregister_form2").getForm().getValues();
											var regnum1 = objectTmp.regnum;
											var stime1 = objectTmp.smsregister_stimed;
											var etime1 = objectTmp.smsregister_etimed;
											var flags1 = objectTmp.flags;
											
											store_rules_smsregister.load({params: {update:value,sid:sid1,regnum:regnum1,stime:stime1,etime:etime1,flags:flags1}});
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
                Ext.getCmp('smsregister_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_smsregister,
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
        },
		{
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
            text: "短信注册限制数量",
            dataIndex: 'regnum',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "限制时间",
            columns:[
			{
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
            }
			]
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
            sortable: true
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
                id:'smsregister_keyword',
                name:'smsregister_keyword',
				emptyText:'请输入短信注册限制数量',
                value:store_rules_smsregister.keyword,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入短信注册限制数量'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('smsregister_keyword').value;
			                    if(aa!='请输入短信注册限制数量'){
					       	            store_rules_smsregister.keyword=aa;
							            store_rules_smsregister.currentPage=1;
							            reflash();
					                }
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('smsregister_keyword').value;
		               if(bb!='输入用户名/广告代码'){         
		                 store_rules_smsregister.keyword=bb;
						  
		               }
		               else{
		                  store_rules_smsregister.keyword='';
		               }
		               store_rules_smsregister.currentPage=1;         
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
				id:'smsregister_removeButton',
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
							store_rules_smsregister.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }
          ]
         }
        }
        
        );
	store_rules_smsregister.alreadyload = 1;
    this.callParent(arguments);
    }
})