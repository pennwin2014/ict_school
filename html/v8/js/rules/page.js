/*模板页面管理*/
Ext.define('rules.page',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
	  if(store_rules_page.alreadyload !=1){
		    store_rules_page.pageid = '';
		    store_rules_page.pagename = '';
		    store_rules_page.pagepath = '';
		    store_rules_page.addtime = '';
		    store_rules_page.moditime = '';
		    store_rules_page.keyword = '';
		    store_rules_page.caExport = ''; //导出
		
			  store_rules_page.new_params={pageid:store_rules_page.pageid,pagename:store_rules_page.pagename,pagepath:store_rules_page.pagepath,addtime:store_rules_page.addtime,moditime:store_rules_page.moditime,keyword:store_rules_page.keyword,caExport:store_rules_page.caExport};
   
	  }
	  
    function reflash(){
				store_rules_page.new_params={pageid:store_rules_page.pageid,pagename:store_rules_page.pagename,pagepath:store_rules_page.pagepath,addtime:store_rules_page.addtime,moditime:store_rules_page.moditime,keyword:store_rules_page.keyword,caExport:store_rules_page.caExport};
				//store_rules_page.currentPage=1;
				store_rules_page.load();
    }

	  //高级查询窗口
    function gotoedit1(value, id){
				var win3 = null;
				var pagename = '';
				var pagepath = '';
				var pageid1 = '';
				var addtime1 = '';
				var title='增加';
				
				if(value == 'update'){
						title='修改';
						pagename = store_rules_page.getAt(id).get('pagename');
						pagepath = store_rules_page.getAt(id).get('pagepath');
						pageid1 = store_rules_page.getAt(id).get('pageid');
						addtime1 = store_rules_page.getAt(id).get('addtime');
				}
		
	      if (!win3){ //判断如果不存在就创建新的
	        win3 = new Ext.Window({
					    title:'模板页面管理窗口 - '+ title,
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
									id:'page_form2',
									layout:"form",
									baseCls:"x-plain",
									bodyPadding: 5,
									items:[{
                    	fieldLabel:'页面名称',
                    	xtype: 'textfield',
											emptyText:'请输入页面名称',
											value:pagename,
                    	name:'pagename'
	                },{
                    	fieldLabel:'页面路径',
                    	xtype: 'textfield',
											emptyText:'请输入页面路径',
											value:pagepath,
                    	name:'pagepath'
	                }]  
	            })],  
					    buttons:[{
								  text:"确定",
								  iconCls:'icon-save',		  
								  handler:function(){
				        			var objectTmp = Ext.getCmp("page_form2").getForm().getValues();
											var pagename1 = objectTmp.pagename;
											var pagepath1 = objectTmp.pagepath;
									
											store_rules_page.load({params: {update:value,pageid:pageid1,pagename:pagename1,pagepath:pagepath1,addtime:addtime1}});
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
                Ext.getCmp('page_removeButton').setDisabled(selections.length == 0);
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
        store: store_rules_page,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            text: "序号",
            dataIndex: 'pageid',
            width: 100,
            //align: 'center',
            hidden: true,
            sortable: true
        },{
            header: '编辑',
            xtype: 'actioncolumn',
            iconCls: 'report_edit',
            tooltip: '点击进入修改界面',
            width: 50,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                gotoedit1('update', rowIndex);
            }
        },{
            text: "页面名称",
            dataIndex: 'pagename',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "页面路径",
            dataIndex: 'pagepath',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "加入时间",
            dataIndex: 'addtime',
            width: 150,
            //align: 'center',
            hidden: false,
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
            		labelWidth: 40,
            		width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'page_keyword',
                name:'page_keyword',
				        emptyText:'请输入模板名称',
                value:store_rules_page.keyword,
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
		                    		var aa=Ext.getCmp('page_keyword').value;
						       	    		store_rules_page.keyword=aa;
								    				store_rules_page.currentPage=1;
								    				reflash();
		                    }
		                }
                }
            }, '-', {
                text:'查询',
            		iconCls:'search2',
                handler:function(){
	                 var bb=Ext.getCmp('page_keyword').value;
		               store_rules_page.keyword=bb;
		               store_rules_page.currentPage=1;
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
								id:'page_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
								if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="'" + rows[i].get('pageid') + "'";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "'" + rows[i].get('pageid') + "'";
			                    }
			                }  
											store_rules_page.load({params: {del: "del",selsid:selsid}});
							
                   }
                }
            }
        ]}
    });
		store_rules_page.alreadyload = 1;
    this.callParent(arguments);
    }
})
