/*优惠劵管理*/
Ext.define('shops.couponManager', {
		extend:'Ext.grid.Panel',
		autoScroll:false,
		frame:false,
		initComponent:function(){
				if(store_shops_couponmanager.alreadyload != 1){
						store_shops_couponmanager.cid ='';
						store_shops_couponmanager.fee = '';
						store_shops_couponmanager.counum = '';
						store_shops_couponmanager.addtime = '';
						store_shops_couponmanager.imgpath = '';
						store_shops_couponmanager.effetime = '';
						store_shops_couponmanager.plateid = plateid;
						store_shops_couponmanager.keyword = '';
						store_shops_couponmanager.caExport = '';
						store_shops_couponmanager.new_params = {cid:store_shops_couponmanager.cid,fee:store_shops_couponmanager.fee,counum:store_shops_couponmanager.counum,addtime:store_shops_couponmanager.addtime,imgpath:store_shops_couponmanager.imgpath,effetime:store_shops_couponmanager.effetime,keyword:store_shops_couponmanager.keyword,caExport:store_shops_couponmanager.caExport};
				}
				
				function reflash(){
						store_shops_couponmanager.new_params = {cid:store_shops_couponmanager.cid,fee:store_shops_couponmanager.fee,counum:store_shops_couponmanager.counum,addtime:store_shops_couponmanager.addtime,imgpath:store_shops_couponmanager.imgpath,effetime:store_shops_couponmanager.effetime,keyword:store_shops_couponmanager.keyword,caExport:store_shops_couponmanager.caExport};
						//store_shops_couponmanager.currentPage=1;
						store_shops_couponmanager.load();		
				}
				
				function gotoedit1(value, id, plateid){
						var windr;
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webcomcoupon_info&key@'+value+'&sid@'+id+'&plateid@'+plateid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:550px;"></iframe>';      
				    var windr = new Ext.Window ({
				                           id:"windr",
				                           title:"优惠劵信息",
				                           //x:0,
				                           //y:0,
				                           width:400,
				                           bodyPadding: 0,
				                           modal:true,
																	 //headerPosition: 'bottom',
				                           resizable:true,
				                           closable:true,
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
		                Ext.getCmp('couponmanager_removeButton').setDisabled(selections.length == 0); 
		            }
		        }
		    });
		    
		    Ext.apply(this, {
		    		border:false,
		    		frame:false,
		    		autoScroll:true,
		    		selModel:sm,
		    		store:store_shops_couponmanager,
		    		viewConfig:{
		    				loadMask:false	
		    		},
		    		columns:[{
		            text: "序号",
		            dataIndex: 'cid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            header: '编辑',
		            xtype: 'actioncolumn',
		            dataIndex: 'cid',
		            iconCls: 'edit',
		            tooltip: '点击进入修改界面',
		            width: 50,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit1('update', record.get('cid'), plateid);
		            }
		        },{
		            text: "优惠劵面值",
		            dataIndex: 'fee',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "发行数量",
		            dataIndex: 'counum',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "发行日期",
		            dataIndex: 'addtime',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "图片路径",
		            dataIndex: 'imgpath',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "有效时间",
		            dataIndex: 'effetime',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		        
		        columnLines: true,
		        
		        // paging bar on the bottom	
		        tbar:{items:[
		        	  '-', {
		            	  labelWidth: 40,
		            	  width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'couponmanager_keyword',
		                name:'couponmanager_keyword',
		                value:store_shops_couponmanager.keyword,
		                emptyText:'请输入优惠劵面值',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='输入优惠劵面值'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('couponmanager_keyword').value;
					                    if(aa!='输入优惠劵面值'){
							       	            store_shops_couponmanager.keyword=aa;
									                store_shops_couponmanager.currentPage=1;
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
			                 var bb=Ext.getCmp('couponmanager_keyword').value;
				               if(bb!='输入优惠劵面值'){         
				                  store_shops_couponmanager.keyword=bb;
				               }
				               else{
				                  store_shops_couponmanager.keyword='';
				               }
				               store_shops_couponmanager.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                 text:'增加',
		                 iconCls:'add',
			               handler : function() {
			               		gotoedit1('add', '', plateid);
			               }
		            },'-',{
		                text:'删除',
		                id: 'couponmanager_removeButton',
		                iconCls:'remove',
		                //disabled: true,
		                handler:function(){
										   if(confirm('您真的要执行删除操作吗？')){                  
					                var selsid;  
					                var rows=sm.getSelection();
					                selsid='';
					                for(var i=0;i<rows.length;i++){
					                  	if(i==0){
					                  	   selsid="'" + rows[i].get('cid') + "'";
					                  	}
					                  	else{
					                       selsid=selsid + "," + "'" + rows[i].get('cid') + "'";
					                    }
					                }
												  store_shops_couponmanager.load({params: {del: "del",selsid:selsid}});
		                   }
		                }
		            }]
		        }
		    });
		    store_shops_couponmanager.alreadyload = 1;
		    this.callParent(arguments);
		}	
});