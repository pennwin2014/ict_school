Ext.define('dennisShoppingGuiding.view.common.AddWin' ,{
    extend: 'Ext.window.Window',
    alias : 'widget.addwin',
    width: 1200,
    height:500,
	title: null,
	autoScroll: true,	
	modal: true,
	field1: null,
	field2: null,
	value1: null,
//	html:'<html><head><meta content=\"text/html; charset=gbk\" http-equiv=\"content-type\" /></head><body><div id=\"div_brandMnanglist\" style=\"width:100%;height:100%\"></div></body></html>',

    initComponent: function() {
		Ext.define('dennisBrandManage.model.List', {
		    extend: 'Ext.data.Model',
		    fields: [ 'brandId','brandName', 'categoryName', 'shopName', 'floorName', 'flag', 'enterTime', 'brandDesc', 'mallId', 'floorId', 'categoryId', 'shopId', 'imgpath']          
		});

        store_type.load();
        var new_params={cid:this.field1.get('mallId')};
        Ext.apply(store_shop.proxy.extraParams,new_params);
        store_shop.load();
		Ext.define('dennisBrandManage.store.List', {
		    extend: 'Ext.data.Store',
		    pageSize: 40,
		    model: 'dennisBrandManage.model.List',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url : '/pronline/Msg?FunName@ncmBrandManageList',
		        //getMethod: function(){ return 'POST'; },
		        reader: {
		          type: 'json',
		          root: 'eimdata',
		          totalProperty: 'totalCount'
		        },
		        simpleSortMode: true
		    },
		    sorters: {
		        property: 'sid',
		        direction: 'DESC'
		    }
		});

		var	store_dennisBrandManage = Ext.create('dennisBrandManage.store.List', 
    		{listeners:
    		    { 
                    beforeload:function(){ 
//                        msgTip = Ext.MessageBox.show({ 
//                        title:'提示', 
//                        width : 250, 
//                        msg:'页面报表统计信息刷新中,请稍后......' 
//                        }); 
                    } 
                } 
            }
        );
        var mallId = this.field1.get('mallId');
        var floorId = this.field1.get('floorId');
        var new_params={mallId:mallId,floorId:floorId}; 
        Ext.apply(store_dennisBrandManage.proxy.extraParams,new_params);			            
		store_dennisBrandManage.load({ 
            callback: function(records, options, success){ 
//                msgTip.hide(); // 加载完成，关闭提示框 
            } 
        });

		Ext.define('dennisBrandManage.view.list.List' ,{
		    extend: 'Ext.grid.Panel',
		    alias : 'widget.brandManageList',
		    store : store_dennisBrandManage, 
		    height:parent.grid_height - 50,
		    width:parent.grid_width - 50,
		    autoScroll: true,
		    title:'',
		    columnLines: true,
		    mallId:0,
		    floorId:0,
		    shopId:0,
		    
		    initComponent: function() {         
			    //关键字查询
			    function dwglkeycx(thisBtn){
                    SetPage(1);  
			        loadProcyListStore();
			    }
			  
			    //添加品牌
			    function addBrand(thisBtn){
			        var mallId = Ext.ComponentQuery.query('#brandManageList1')[0].mallId;
			        var floorId = Ext.ComponentQuery.query('#brandManageList1')[0].floorId;
			        showAddBrandWin(0, floorId, '', '', mallId, 0, 0, '', null);
			    }// addBrand: function(thisBtn){

			    function deleteBrand(thisBtn){
			        var grid = Ext.ComponentQuery.query('#brandManageList1')[0];
                    var tstore=grid.getStore();
                    var rows = grid.getSelectionModel().getSelection(); 
                    selsid='';
                    for(var i=0;i<rows.length;i++){
                        if(i==0){
                            selsid="'" + rows[i].get('brandId') + "'";
                        } else{
                            selsid=selsid + "," + "'" + rows[i].get('brandId') + "'";
                        }
                    }
                    tstore.load({params:{actionName:"delete",selsid:selsid}});
			    }// addBrand: function(thisBtn){


			  
			    /**************************************
			    * 策略标签 
			    ***************************************/  
			    function showTips( thisTV, eOpts ){
					thisTV.tip = Ext.create('Ext.tip.ToolTip', {
					target: thisTV.el,
					trackMouse: true,
					dismissDelay : 60000,
					html: '<p>例如：</p><p>&nbsp;&nbsp;&nbsp;单个端口：80,21</p><p>&nbsp;&nbsp;&nbsp; 端口段：2000~3000</p><p>&nbsp;&nbsp;&nbsp;组合：80,2000~3000,3005</p>'
					});
			    }
			    /**************************************
			    * 全局函数 
			    ***************************************/
			    inheritableStatics:{
			        function loadProcyListStore(){
			            var qgrid = Ext.ComponentQuery.query('#brandManageList1')[0];
			            var store = qgrid.getStore();
                        var keyword = Ext.getCmp('keywordBrandName').value;
                        var mallId = qgrid.mallId;    			             
                        var new_params={keyword:keyword,mallId:mallId}; 
                        Ext.apply(store.proxy.extraParams,new_params);			            
			            store.load();
			        }
			        function SetPage(curpage){
			            var store = Ext.ComponentQuery.query('#brandManageList1')[0].getStore();
			            store.currentPage = curpage;
			        }
			        function setTitle(title){
			             document.getElementById("titledx").innerHTML = 
			                 '&nbsp'+ title +'&nbsp;&nbsp;';   
			        }
			        function showAddBrandWin(brandId, floorId, brandName, 
			            brandDesc, mallId, categoryId, shopId, imgpath, record){
			            var ensureButtonText = 
			                0 == brandId ? '&nbsp;&nbsp;增&nbsp;&nbsp;加 &nbsp;&nbsp;' : '&nbsp;&nbsp;修&nbsp;&nbsp;改 &nbsp;&nbsp;';                         
			            var actName = 
			                0 == brandId ?  "addBrand" : "modifyBrand" ;
			            var hidImgView = brandId ? false : true;
                        var categoryName = record ? record.get('categoryName') : '';
                        var shopName = record ? record.get('shopName') : '';
                        shopId = 0 == shopId ? '' : shopId;
                        categoryId = 0 == categoryId ? '' : categoryId;
			            var fp2 = Ext.create('Ext.FormPanel', {         
			                frame: true,
			                layout: 'form',
			                //width: 580,//parent.width,
			                //height: 330,//parent.height,
			                fieldDefaults: {
			                   labelWidth: 85,                        
			                   width: 350
			                },
			                collapsible: false,
			                bodyPadding: 10,
                            items: [
			                    {//left fieldset
									xtype:'fieldset',
									width:350,
									title:'基本信息',
									items:[
			                        {
			                            layout: 'hbox',
			                            xtype: 'container',       
			                            items:[
			                                {
			                                    fieldLabel:'商标名称',
			                                    xtype: 'textfield',
			                                    maxLength:32,
			                                    name:'brandName',
			                                    id:'brandName',
			                                    value:brandName
			                                }
			                            ]
			                        },             
			                        {
			                            layout: 'hbox',
			                            xtype: 'container',
			                            items:[
			                                {
			                                    xtype:'combo',
			                                    id: 'smCateCom', 
			                                    fieldLabel:'商标小类名称',
			                                    name: 'smCateId',  
			                                    valueField: 'sid',  
			                                    displayField: 'name', 
			                                    hiddenName:'smCateIdHid', 
			                                    hidden:false,
			                                    allowBlank:false,
			                                    blankText:'商标小类名称不能为空！',
			                                    //forceSelection:true,
			                                    editable:false,
			                                    msgTarget:'qtip', //显示一个浮动的提示信息 
			                                    store:store_type,
			                                    triggerAction:'all',
			                                    queryMode:'local',
			                                    value:categoryId,
			                                    typeAhead:true
			                                }
			                            ]
			                        },   
			                        {
			                            layout: 'hbox',
			                            xtype: 'container',
			                            items:[
			                                {
			                                    xtype:'combo',
			                                    id: 'shopListCom', 
			                                    fieldLabel:'商店名称',
			                                    name: 'shopId',  
			                                    valueField: 'id',  
			                                    hiddenName:'shopIdHid',
			                                    displayField: 'name',  
			                                    hidden:false,
			                                    allowBlank:false,
			                                    blankText:'商店名称不能为空！',
			                                    //forceSelection:true,
			                                    editable:false,
			                                    msgTarget:'qtip', //显示一个浮动的提示信息 
			                                    store:store_shop,
			                                    triggerAction:'all',
			                                    queryMode:'local',
			                                    value:shopId,
			                                    typeAhead:true
			                                }
			                            ]
			                        },
			                        {
			                            xtype: 'hidden',
			                            value:brandId,
			                            name:'brandId',
			                            id:'brandId'
			                        },
			                        {
			                            xtype: 'hidden',
			                            value:mallId,
			                            name:'mallId',
			                            id:'mallId'
			                        },
			                        {
			                            xtype: 'hidden',
			                            value:floorId,
			                            name:'floorId',
			                            id:'floorId'
			                        },
			                        {
			                            xtype: 'hidden',
			                            value:actName,
			                            name:'actName',
			                            id:'actName'
			                        },
			                        {
			                            layout: 'hbox',
			                            xtype: 'container',       
			                            items:[
			                                {
			                                    fieldLabel:'商标描述',
			                                    xtype: 'textarea',
			                                    maxLength:128,
			                                    name:'brandDesc',
			                                    id:'brandDesc',
			                                    value:brandDesc,
			                                    height:50
			                                }
			                            ]
			                        }									
			                        ]									
								},  //left fieldset                             
			                    {
									xtype: 'component',
			                        width: 25	
			       				},
			       				{//image upload fieldset
									xtype:'fieldset',
									title:'图片',
									width:350,
									items:[{
										xtype:'image',
										id:'img',
										name:'img',
										autoWidth:true,
										autoHeight:true,
										hidden:hidImgView,
										width:350,
										height:200,
										src:'[#imgpath#]'
									},{
										xtype: 'component',
								  		height:15	
									},{
										xtype:'filefield',
										fieldLabel:'上传图片',
										id:'fname',
										name:'fname',
										hidden:false,
										buttonText: '选择图片'
									},{
										xtype:'displayfield',
										fieldLabel:'提示',
										id:'imgsize',
										name:'imgsize',
										value:'您要上传的图片大小 (122*122)'	
									}]
							    },//image upload fieldset			       				                                              
			                    {//buttons
			                        layout: 'column',
			                        xtype: 'container',
			                        items:[

			                        
			                            {
			                                xtype: 'container',
			                                columnWidth: .20
			                            },
			                            {                 
			                                xtype: 'button',
			                                anchor: 'left',
			                                text:ensureButtonText,
			                                columnWidth: .25, 
			                                handler: function(){
			                                    var grid = Ext.ComponentQuery.query('#brandManageList1')[0];             
			                                    var store = grid.getStore();
                                                var cnt =  store_type.getTotalCount();
//                                                msgTip = Ext.MessageBox.show({ 
//                                                title:'提示', 
//                                                width : 250, 
//                                                msg:'store_type totalcount' + cnt + '' 
//                                                });   			                                    
//			                                    var rec = store_type.getAt(store_type.find('name', 
//                                                    fp2.getForm().getValues().smCateId));
//                                                if (rec) {
//                                                    Ext.getCmp("smCateCom").setValue(rec.get('sid'));
//                                                }
                                                var cnt =  store_shop.getTotalCount();
//                                                msgTip = Ext.MessageBox.show({ 
//                                                title:'提示', 
//                                                width : 250, 
//                                                msg:'store_shop totalcount' + cnt + '' 
//                                                });   	                                                
			                                    
//			                                    var rec = store_shop.getAt(store_shop.find('name', 
//                                                    fp2.getForm().getValues().shopId));
//                                                if (rec) {
//                                                    Ext.getCmp("shopListCom").setValue(rec.get('id'));
//                                                }
                                    
			                                    fp2.form.doAction('submit',{
			                                        url:'/pronline/upload',
			                                        method:'post',
			                                        enctype:'multipart/form-data',
			                                        params:{FunName:'ncmBrandAddOrModify', UploadPath:'/home/ncmysql/ncsrv/upload'},   
			                                        success:function(form,action){
			                                            alert(action.result.success);
			                                            if ("addBrand" == fp2.getForm().getValues().actName) {
    			                                            Ext.getCmp("brandName").setValue('');
    			                                            Ext.getCmp("brandDesc").setValue('');			                                                
			                                            } else {
			                                                Ext.getCmp('img').setSrc(action.result.imgpath);
			                                            }
			                                            store.load();
			                                        }
			                                    });
			                                }
			                            },
			                            {
			                                xtype: 'container',
			                                columnWidth: .10
			                            },
			                            {               
			                                text: '&nbsp;&nbsp;重&nbsp;&nbsp;置 &nbsp;&nbsp;',
			                                xtype: 'button',
			                                anchor: 'right',
			                                columnWidth: .25, 
			                                handler: function(){
			                                    fp2.getForm().reset();
			                                }
			                            },
			                            {
			                                xtype: 'container',
			                                columnWidth: .20
			                            }
			                        ]
			                    }//buttons                             
			                ]// items: [			                
			            });// var fp2=Ext.create('Ext.FormPanel', {         
			            
			            var win3 = Ext.widget('window', {
			                title: '品牌信息',
			                x:400,
			                y:32,
			                width: 570,
			                resizable: true,
			                draggable:true,
			                modal:false,
			                items: fp2
			            });
			           
//                        if(record && record != ''){
//                           fp2.getForm().loadRecord(record);  
//                        }                        
                        if(imgpath && '' != imgpath){
                            Ext.getCmp('img').setSrc(imgpath);
                        }
                        //fp2.getForm().setValues({smCateId:categoryId,shopList:shopId});
                        //Ext.getCmp("smCateId").setValue(categoryId);                      
                        //Ext.getCmp("shopList").setValue(shopId);  			                               		            
			            win3.show();
			        }//showAddBrandWin: function(floorId, floorName, floorDetail, mallId){
			    }//inheritableStatics:{   

		        this.tbar = Ext.create('Ext.PagingToolbar',{
		            store:store_dennisBrandManage,
		            displayInfo: true,
		            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
		            emptyMsg: "没有记录可显示",
		            items:[
		                {
		                    xtype:'textfield',
		                    id:'keywordBrandName',
		                    width:180,
		                    maxLength:128,
		                    name:'keywordBrandName',
		                    style:'color:#7aa7d5',             
		                    emptyText:'请输入商标名称',
		                    enableKeyEvents:true,
		                    listeners:{
		                        'focus':function(){
		                        },
		              
		                        'keydown' : function(i,e){
		                             var aae=e.getKey() ; 
		                             if(aae==13){
		                             }
		                         }
		                    }
		                },
		                {
		                    text:'查询',
		                    iconCls:'accept',
							handler : function() {
								  dwglkeycx();
							}
		                },
		                '-',
		                {
			                text:'增加',
			                iconCls:'add',
							handler : function() {
								  addBrand();
							}
				        },
        	            '-',
                        {
                            text:'删除',
                            itemId: 'removeButton',
                            iconCls:'remove',
                            disabled: true,
							handler : function() {
								  deleteBrand();
							}
                        },				                       
		                {xtype:'label', html:'<span id="titledx"></span>'}
		            ]// items:[
		        });//this.tbar = Ext.create('Ext.PagingToolbar',{

                var sm = Ext.create('Ext.selection.CheckboxModel',{
                    listeners: {
                        selectionchange: function(sm, selections) {  
                        	var grid=Ext.ComponentQuery.query('#brandManageList1')[0];                  
                            grid.down('#removeButton').setDisabled(selections.length == 0);          
                        }
                    }    
                });
                this.selModel=sm;
                
		        this.columns = [
		            {
		                header: '编辑',
		                xtype: 'actioncolumn',
		                dataIndex: 'brandId',
		                icon: '/images/v8/edit_task.png',
		                tooltip: '点击该图标进入修改商标信息界面。',
		                width: 50,
		                align: 'center',
		                sortable: false,
		                handler: function(grid, rowIndex, colIndex, actionItem, event, 
		                    record, row) {
		                    showAddBrandWin(
		                        record.get('brandId'), record.get('floorId'),
		                        record.get('brandName'), record.get('brandDesc'),
		                        record.get('mallId'), record.get('categoryId'), 
		                        record.get('shopId'), record.get('imgpath'), record);
		                }
		            },
		            {header: '商标名称', dataIndex: 'brandName',width:200,sortable: false},
		            {header: '商标小类名称', dataIndex: 'categoryName', width:110,sortable: false},
		            {header: '商店名称', dataIndex: 'shopName',  width:210},
		            {header: '楼层', dataIndex: 'floorName',   width:110},
		            {header: '状态', dataIndex: 'flag',   width:110},
		            {header: '加入时间', dataIndex: 'enterTime',   width:150},
		            {header: '描述', dataIndex: 'brandDesc',   width:110},
		            {flex:1}	            
		        ];

		        this.callParent(arguments);  
		    }// initComponent: function() {	 
//            ,
//            showRender: function(){		    
//            onRender: function() {	
//		        this.callParent(arguments);                	    
//		        var store = Ext.ComponentQuery.query('#brandManageList1')[0].getStore();
//		        store.on('beforeload', function (store, options) {
//		            //调整视图高度
//		            var qgrid=Ext.ComponentQuery.query('#brandManageList1')[0];
//    	        	parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
//		            qgrid.setHeight(parseInt(parent.grid_height));
//		              	
//		            var keyword = Ext.getCmp('keywordBrandName').value;
//		            var mallId = qgrid.mallId;    			             
//		            var new_params={keyword:keyword,mallId:mallId}; 
//		            Ext.apply(store.proxy.extraParams,new_params);
//		        });//store.on('beforeload', function (store, options) {
//
//		        store.load();
//
//			}// showRender: function(){  
				          
		});//    Ext.define('dennisBrandManage.view.list.List' ,{

        this.items = [
            {
            	xtype: 'brandManageList',
                id:'brandManageList1',
                name:'brandManageList1',            	
				mallId: this.field1.get('mallId'),
				floorId: this.field1.get('floorId'),
//				renderto: 'div_brandMnanglist',
            	width:'100%'            	
            }
        ];

        this.callParent(arguments);
        
    }//initComponent
//    ,
//    onshow: function(thisBtn){
//        var qgrid = Ext.ComponentQuery.query('#brandManageList1')[0];
//        qgrid.loadProcyListStore();
//    }  
});
