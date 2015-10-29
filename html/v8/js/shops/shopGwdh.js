/*商店购物导航管理*/
Ext.define('shops.shopGwdh', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_shopGwdh.alreadyload != 1){
						store_shops_shopGwdh.id = '';
						store_shops_shopGwdh.name = '';	
						store_shops_shopGwdh.service_code = '';
						store_shops_shopGwdh.sgroupname = '';
						store_shops_shopGwdh.imgpath = '';
						store_shops_shopGwdh.floor = '';
						store_shops_shopGwdh.url = '';
						store_shops_shopGwdh.code = '';
						store_shops_shopGwdh.tel = '';
						store_shops_shopGwdh.address = '';
						store_shops_shopGwdh.level = '';
						store_shops_shopGwdh.plevel = '';
						store_shops_shopGwdh.type = '';
						store_shops_shopGwdh.stype = '';
						store_shops_shopGwdh.descr = '';
						store_shops_shopGwdh.detail = '';
						store_shops_shopGwdh.systime = '';
						store_shops_shopGwdh.flags = '';
						store_shops_shopGwdh.plateid = plateid;
						store_shops_shopGwdh.areaid = '';
						store_shops_shopGwdh.title = '';
						store_shops_shopGwdh.keyword = '';
						store_shops_shopGwdh.caExport = '';
						store_shops_shopGwdh.new_params = {id:store_shops_shopGwdh.id,name:store_shops_shopGwdh.name,service_code:store_shops_shopGwdh.service_code,sgroupname:store_shops_shopGwdh.sgroupname,imgpath:store_shops_shopGwdh.imgpath,floor:store_shops_shopGwdh.floor,url:store_shops_shopGwdh.url,code:store_shops_shopGwdh.code,tel:store_shops_shopGwdh.tel,address:store_shops_shopGwdh.address,level:store_shops_shopGwdh.level,plevel:store_shops_shopGwdh.plevel,type:store_shops_shopGwdh.type,stype:store_shops_shopGwdh.stype,descr:store_shops_shopGwdh.descr,detail:store_shops_shopGwdh.detail,systime:store_shops_shopGwdh.systime,flags:store_shops_shopGwdh.flags,title:store_shops_shopGwdh.title,keyword:store_shops_shopGwdh.keyword,caExport:store_shops_shopGwdh.caExport};
				}
				
				function reflash(){
						store_shops_shopGwdh.new_params = {id:store_shops_shopGwdh.id,name:store_shops_shopGwdh.name,service_code:store_shops_shopGwdh.service_code,sgroupname:store_shops_shopGwdh.sgroupname,imgpath:store_shops_shopGwdh.imgpath,floor:store_shops_shopGwdh.floor,url:store_shops_shopGwdh.url,code:store_shops_shopGwdh.code,tel:store_shops_shopGwdh.tel,address:store_shops_shopGwdh.address,level:store_shops_shopGwdh.level,plevel:store_shops_shopGwdh.plevel,type:store_shops_shopGwdh.type,stype:store_shops_shopGwdh.stype,descr:store_shops_shopGwdh.descr,detail:store_shops_shopGwdh.detail,systime:store_shops_shopGwdh.systime,flags:store_shops_shopGwdh.flags,title:store_shops_shopGwdh.title,keyword:store_shops_shopGwdh.keyword,caExport:store_shops_shopGwdh.caExport};
						//store_shops_shopGwdh.currentPage=1;
						store_shops_shopGwdh.load();	
				}
				
				function gotoedit1(value,id,plateid,hgt){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomshop_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%"></iframe>';
						var windr = new Ext.Window ({
						                       id:"windr",
						                       title:"商店信息",
						                       //x:0,
						                       //y:0,
						                       width:770,
						                       height:hgt,
						                       bodyPadding: 0,
						                       autoScroll:true,
						                       modal:true,
																	 //headerPosition: 'bottom',
						                       resizable:true,
						                       closable : true,
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
		                Ext.getCmp('shopGwdh_removeButton').setDisabled(selections.length == 0);
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_shopGwdh,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "商店ID",
		            dataIndex: 'id',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'id',
		            iconCls: 'edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		            	  if(window.navigator.userAgent.indexOf("MSIE")>=1){
		                		gotoedit1('update', record.get('id'), plateid, 544);
		                }else if(window.navigator.userAgent.indexOf("Firefox")>=1){
		                		gotoedit1('update', record.get('id'), plateid, 588);	
		                }else{
		                		gotoedit1('update', record.get('id'), plateid, 538);	
		                }
		            }
		        },{
		            text: "商店名称",
		            dataIndex: 'name',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "导航栏标题",
		            dataIndex: 'title',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "商店类型",
		            dataIndex: 'stype',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "场所代码",
		            dataIndex: 'service_code',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "商品小类名称",
		            dataIndex: 'sgroupname',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 300,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "楼层",
		            dataIndex: 'floor',
		            width: 80,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "网址",
		            dataIndex: 'url',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "国家代码/国内区号",
		            dataIndex: 'code',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "电话",
		            dataIndex: 'tel',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "地址",
		            dataIndex: 'address',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "广告优先级",
		            dataIndex: 'level',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "促销优先级",
		            dataIndex: 'plevel',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "活动类型",
		            dataIndex: 'type',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "加入时间",
		            dataIndex: 'systime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "简述",
		            dataIndex: 'descr',
		            width: 120,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "描述",
		            dataIndex: 'detail',
		            width: 200,
		            //align: 'center',
		            hidden: false,
		            sortable: false,
					      renderer: function(value, meta, record) {
                    meta.style = 'white-space:nowrap;';
									  while(value.indexOf("<br>")>0){
												value=value.replace("<br>","");
										}
                    return value;   
                }
		        },{
		            text: "标记",
		            dataIndex: 'flags',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        
		        // paging bar on the bottom
            tbar: {items: [
		            '-', {
		            	  labelWidth: 45,
		            	  width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'shopGwdh_keyword',
		                name:'shopGwdh_keyword',
		                emptyText:'请输入商店名称',
		                value:store_shops_shopGwdh.keyword,
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入商店名称'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('shopGwdh_keyword').value;
					                    if(aa!='输入商店名称'){
							       	            store_shops_shopGwdh.keyword=aa;
									                store_shops_shopGwdh.currentPage=1;
									                reflash();
							                }
			                    }
			                }
		                }
		            }, {
		                text:'查询',
		                //itemId: 'moveButton',
		            		iconCls:'accept',
		                //disabled: true,
		                handler:function(){
			                 var bb=Ext.getCmp('shopGwdh_keyword').value;
				               if(bb!='输入商店名称'){         
				                  store_shops_shopGwdh.keyword=bb;
				               }
				               else{
				                  store_shops_shopGwdh.keyword='';
				               }
				               store_shops_shopGwdh.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 id:'shopGwdh_addButton',
		                 iconCls:'add',
		                 disabled: false,
			               handler : function() {
    			              if(window.navigator.userAgent.indexOf("MSIE")>=1){
    		                		gotoedit1('add', '', plateid, 544);
    		                }else if(window.navigator.userAgent.indexOf("Firefox")>=1){
    		                		gotoedit1('add', '', plateid, 588);	
    		                }else{
    		                		gotoedit1('add', '', plateid, 538);	
    		                }
			               }
		            },'-',{
		                text:'删除',
		                id: 'shopGwdh_removeButton',
		                iconCls:'remove',
		                disabled: true,
		                handler:function(){
										   if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('id') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('id') + "'";
					                    }
					                }
												  store_shops_shopGwdh.load({params: {del: "del",selsid:selsid}});
											 }
		                }
		            }]
            }
		    });
		    store_shops_shopGwdh.alreadyload = 1;
		    this.callParent(arguments);	
		}	
})
