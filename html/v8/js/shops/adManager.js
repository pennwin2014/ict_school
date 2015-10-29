﻿/*广告管理*/
Ext.define('shops.adManager',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
				if(store_shops_admanager.alreadyload !=1){
					  store_shops_admanager.sid = '';
				    store_shops_admanager.name = '';
				    store_shops_admanager.sgroupname = '';
				    store_shops_admanager.descr = '';
				    store_shops_admanager.imgpath = '';
				    store_shops_admanager.serno = '';
				    store_shops_admanager.floor ='';
				    store_shops_admanager.level = '';
				    store_shops_admanager.plevel = '';
				    store_shops_admanager.type = '';
				    store_shops_admanager.systime = '';
				    store_shops_admanager.shopname = '';
				    store_shops_admanager.plateid = 1;
				    store_shops_admanager.flags = '';
				    store_shops_admanager.keyword = '';
				    store_shops_admanager.caExport = ''; //导出
					  store_shops_admanager.new_params={sid:store_shops_admanager.sid,name:store_shops_admanager.name,sgroupname:store_shops_admanager.sgroupname,descr:store_shops_admanager.descr,imgpath:store_shops_admanager.imgpath,serno:store_shops_admanager.serno,floor:store_shops_admanager.floor,level:store_shops_admanager.level,plevel:store_shops_admanager.plevel,type:store_shops_admanager.type,systime:store_shops_admanager.systime,shopname:store_shops_admanager.shopname,plateid:store_shops_admanager.plateid,flags:store_shops_admanager.flags,keyword:store_shops_admanager.keyword,caExport:store_shops_admanager.caExport};
			  }
			  
				function reflash(){
						store_shops_admanager.new_params={sid:store_shops_admanager.sid,name:store_shops_admanager.name,sgroupname:store_shops_admanager.sgroupname,descr:store_shops_admanager.descr,imgpath:store_shops_admanager.imgpath,serno:store_shops_admanager.serno,floor:store_shops_admanager.floor,level:store_shops_admanager.level,plevel:store_shops_admanager.plevel,type:store_shops_admanager.type,systime:store_shops_admanager.systime,shopname:store_shops_admanager.shopname,plateid:store_shops_admanager.plateid,flags:store_shops_admanager.flags,keyword:store_shops_admanager.keyword,caExport:store_shops_admanager.caExport};
						//store_shops_admanager.currentPage=1;
						store_shops_admanager.load();
				}
				
				function gotoedit1(value, id, plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomindex_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%; height:800px;"></iframe>';    
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"广告管理窗口",
				                           //x:0,
				                           y:0,
				                           width:800,
				                           height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
								                   autoScroll:true, 
				                           bodyPadding:0,
				                           modal:true,
				                           resizable:true,
				                           closable :true,
				                           draggable:true,
				                           html:htmlvar        
		    		})       
		
					  windr.on("beforeclose", function() {
					  	reflash();
					  })
					  windr.show();
		    }
		    
			  var sm = Ext.create('Ext.selection.CheckboxModel',{
		        listeners: {
		            selectionchange: function(sm, selections) {
		                Ext.getCmp('admanager_removeButton').setDisabled(selections.length == 0);
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
		        store: store_shops_admanager,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "商标ID",
		            dataIndex: 'sid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'sid',
		            iconCls: 'edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', record.get('sid'), record.get('plateid'));
		            }
		        },{
		            text: "商标名称",
		            dataIndex: 'name',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商标序列号",
		            dataIndex: 'serno',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 200,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "商品小类名称",
		            dataIndex: 'sgroupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "商店名称",
		            dataIndex: 'shopname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "楼层",
		            dataIndex: 'floor',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "活动类型",
		            dataIndex: 'type',
		            width: 80,
		            //align: 'center',
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
		            text: "描述",
		            dataIndex: 'descr',
		            width: 200,
		            //align: 'center',
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
		                id:'admanager_keyword',
		                name:'admanager_keyword',
										emptyText:'请输入商标名称',
		                value:store_shops_admanager.keyword,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                 if(this.value=='输入用户名/广告代码'){                
						                    this.setValue('');
						                }
												},
												
				                'keydown' : function(i,e){
				                    var aae=e.getKey(); 
				                    if(aae==13){
				                    		var aa=Ext.getCmp('admanager_keyword').value;
						                    if(aa!='输入用户名/广告代码'){
								       	            store_shops_admanager.keyword=aa;
										            store_shops_admanager.currentPage=1;
										            reflash();
								                }
				                    }
				                }
		                }
		            }, '-', {
		                text:'查询',
		            		iconCls:'accept',
		                handler:function(){
			                 var bb=Ext.getCmp('admanager_keyword').value;
				               if(bb!='输入用户名/广告代码'){         
				                 store_shops_admanager.keyword=bb;
				               }
				               else{
				                  store_shops_admanager.keyword='';
				               }
				               store_shops_admanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               	  var pp = store_shops_admanager.plateid;
			               	  gotoedit1('add', '', pp);
			               }
		            },'-', {
		                text:'删除',
										id:'admanager_removeButton',
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
							
														store_shops_admanager.load({params: {del: "del",selsid:selsid}});
									
			                  }
		                }
		            }]
		         }
		    });
			  store_shops_admanager.alreadyload = 1;
		    this.callParent(arguments);
    }
});