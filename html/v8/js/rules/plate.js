/*模板管理*/
Ext.define('rules.plate',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
	  if(store_rules_plate.alreadyload !=1){
		    store_rules_plate.plateid = '';
		    store_rules_plate.shopplateid = '';
		    store_rules_plate.groupcode = '';
		    store_rules_plate.platename = '';
		    store_rules_plate.platepath = '';
		    store_rules_plate.addtime = '';
		    store_rules_plate.moditime = '';
		    store_rules_plate.keyword = '';
		    store_rules_plate.caExport = ''; //导出
		
			  store_rules_plate.new_params={plateid:store_rules_plate.plateid,shopplateid:store_rules_plate.shopplateid,groupcode:store_rules_plate.groupcode,platename:store_rules_plate.platename,platepath:store_rules_plate.platepath,addtime:store_rules_plate.addtime,moditime:store_rules_plate.moditime,keyword:store_rules_plate.keyword,caExport:store_rules_plate.caExport};
   
	  }
	  
    function reflash(){
				store_rules_plate.new_params={plateid:store_rules_plate.plateid,shopplateid:store_rules_plate.shopplateid,groupcode:store_rules_plate.groupcode,platename:store_rules_plate.platename,platepath:store_rules_plate.platepath,addtime:store_rules_plate.addtime,moditime:store_rules_plate.moditime,keyword:store_rules_plate.keyword,caExport:store_rules_plate.caExport};
				//store_rules_plate.currentPage=1;
				store_rules_plate.load();
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

	  //高级查询窗口
	  function gotoedit3(id){
			  store_rules_prule.plateid = id;
				var windr = new Ext.Window ({
					     id:'prule_windr',
               title:"模板投放规则",
               //x:0,
               y:0,
               width:1100,
               height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
               autoScroll:true,
             	 bodyPadding: 0,
               modal:true,
						   //headerPosition: 'bottom',
               resizable:true,
               closable:true,
               draggable:true,
               items:[Ext.create('rules.prule')]                    
		    })
		    
			  windr.on("beforeclose", function() {
			  	 reflash();
			  })
			  windr.show();
		}
	  
	  function gotoedit2(id){
			  store_rules_plist.plateid = id;
				var windr = new Ext.Window ({
					     id:'plist_windr',
               title:"模板类型",
               //x:0,
               y:150,
               width:800,
               //height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
               height:310,
               autoScroll:true,
             	 bodyPadding: 0,
               modal:true,
						   //headerPosition: 'bottom',
               resizable:true,
               closable:true,
               draggable:true,
               items:[Ext.create('rules.plist')]                    
		    })
		    
			  windr.on("beforeclose", function() {
			  	 reflash();
			  })
			  windr.show();
		}
    
    function gotoedit1(value, id){
				var win3 = null;
				var plateid = '';
				var shopplateid1 = '';
				var gcode = '';
				var platename = '';
				var platepath = '';
				var addtime1 = '';
				var title='增加';
				
				if(value == 'update'){
						title='修改';
						shopplateid1 = store_rules_plate.getAt(id).get('shopplateid');
						plateid = store_rules_plate.getAt(id).get('plateid');
						gcode = store_rules_plate.getAt(id).get('groupcode');
						platename = store_rules_plate.getAt(id).get('platename');
						platepath = store_rules_plate.getAt(id).get('platepath');
						addtime1 = store_rules_plate.getAt(id).get('addtime');
				}
		
	      if (!win3){ //判断如果不存在就创建新的
	        win3 = new Ext.Window({
					    title:'模板管理窗口 - '+ title,
					    closeAction: 'hide',
					    layout:'form',  
					    width:400,
					    draggable:true, //可拖动的
						  maximizable:true, //可最大化的
				      //resizable: true, //可改变大小
						  modal: false,//后面的内容可以操作
				      //plain：true,//则主体背景透明
				      //items: fp2
					    constrain:true, //限制窗口只能在其容器内移动
				      //minimizable:true, //可最小化
				      
	            items:[new Ext.FormPanel({
									id:'plate_form2',
									layout:"form",
									baseCls:"x-plain",
									bodyPadding: 5,
									items:[{
                    	fieldLabel:'模板名称',
                    	xtype: 'textfield',
											emptyText:'请输入模板名称',
											value:plateid,
                    	name:'plateid',
                    	afterLabelTextTpl: required,
							        allowBlank:false, //是否允许为空
							        blankText:'模板名称不能为空！',	
									    msgTarget:'qtip' //显示一个浮动的提示信息 	
	                },{
                    	fieldLabel:'显示名称',
                    	xtype: 'textfield',
											emptyText:'请输入显示名称',
											value:platename,
                    	name:'platename'
	                },{
                    	fieldLabel:'模板路径',
                    	xtype: 'textfield',
											emptyText:'请输入模板路径',
											value:platepath,
                    	name:'platepath'
	                },{
                      xtype:'combo',
											name:'groupcode',
											fieldLabel:'单位',
											emptyText:'请选择单位',
											value:gcode,
											triggerAction:'all',
											store:store_group,
											displayField:'dispname',
											valueField:'groupname',
											queryMode:'local',
											//forceSelection:false,
											typeAhead:true,
											allowBlank:true
                  }]  
	            })],  
					    buttons:[{
								  text:"确定",
								  iconCls:'icon-save',		  
								  handler:function(){
				        			var objectTmp = Ext.getCmp("plate_form2").getForm().getValues();
				        			var plateid1 = objectTmp.plateid;
											var platename1 = objectTmp.platename;
											var platepath1 = objectTmp.platepath;
											var gcode1 = objectTmp.groupcode;
											
									    if(plateid1 != ''){
													store_rules_plate.load({params: {update:value,plateid:plateid1,shopplateid:shopplateid1,platename:platename1,platepath:platepath1,gcode:gcode1,addtime:addtime1}});
										  		win3.close();
										  }
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
                Ext.getCmp('plate_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_plate,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            text: "序号",
            dataIndex: 'shopplateid',
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
            header: '模板设置',
            xtype: 'actioncolumn',
            iconCls: 'report_edit',
            tooltip: '点击进入模板设置界面',
            width: 80,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit2(record.get('plateid'));
            }
        },{
            header: '模板投放规则',
            xtype: 'actioncolumn',
            iconCls: 'information',
            tooltip: '点击进入应用模板投放规则',
            width: 80,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit3(record.get('plateid'));
            }
        },{
            text: "模板名称",
            dataIndex: 'plateid',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "显示名称",
            dataIndex: 'platename',
            width: 100,
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
            text: "模板路径",
            dataIndex: 'platepath',
            width: 300,
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
            hidden: false,
            sortable: false
        },{
        	 flex: 1	
        }],
         
        columnLines: true,
        tbar:{items:[
            '-',{
            		labelWidth: 45,
            		width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'plate_keyword',
                name:'plate_keyword',
				        emptyText:'请输入模板名称/显示名称',
                value:store_rules_plate.keyword,
                enableKeyEvents:true,
                listeners:{
		                'focus':function(){
				                 if(this.value=='请输入模板名称/显示名称'){                
				                    this.setValue('');
				                }
										},
		                'keydown' : function(i,e){
		                    var aae=e.getKey(); 
		                    if(aae==13){
		                    		var aa=Ext.getCmp('plate_keyword').value;
						       	    		store_rules_plate.keyword=aa;
								    				store_rules_plate.currentPage=1;
								    				reflash();
		                    }
		                }
                }
            }, '-', {
                text:'查询',
            		iconCls:'search2',
                handler:function(){
	                 var bb=Ext.getCmp('plate_keyword').value;
		               store_rules_plate.keyword=bb;
		               store_rules_plate.currentPage=1;
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
								id:'plate_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
								if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('plateid') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('plateid') + "'";
			                    }
			                }  
											store_rules_plate.load({params: {del: "del",selsid:selsid}});
                   }
                }
            }
        ]}
    });
		store_rules_plate.alreadyload = 1;
    this.callParent(arguments);
    }
})
