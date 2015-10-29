
  var store_classtree=Ext.create('Ext.data.TreeStore', 
				{   
						fields: ['text','code'],
						autoLoad : true,
						proxy: 
						{   
							type: 'ajax',   

						url: '/pronline/Msg?FunName@ncmTreeCorp_comp_v4'  
						}  
				}
			);

Ext.define('TreeCombox', {  
    extend: 'Ext.form.field.Picker',  
    xtype: 'treecombox',  
    triggerCls: Ext.baseCSSPrefix + 'form-arrow-trigger',  
    config: {  
        displayField: null,  
        valueField:null,
        columns: null,  
        rootVisible: false,  
        selectOnTab: true,  
        firstSelected: false,  
        maxPickerWidth: 119,  
        maxPickerHeight: 360,  
        minPickerHeight: 100  
    },  
    editable: false,  
    initComponent: function() { 
    	
        var me = this;  
        me.callParent(arguments);  
        this.addEvents('select'); 
        me.store.on('load', me.onLoad, me);  
    //      alert('ggggggggg');
    },  
    createPicker: function() {  
        var me = this,  
            picker = Ext.create('Ext.tree.Panel', {  
                store: me.store,  
                floating: true,  
                hidden: true,  
                width: me.maxPickerWidth,  
                displayField: me.displayField,
                  valueField:me.valueField,
                columns: me.columns,  
                maxHeight: me.maxTreeHeight,  
                shadow: false, 
                rootVisible:false,  
                manageHeight: false,  
                listeners: {  
                    itemclick: Ext.bind(me.onItemClick, me)  
                },  
                viewConfig: {  
                    listeners: {  
                        render: function(view) {  
                            view.getEl().on('keypress', me.onPickerKeypress, me);  
                        }  
                    }  
                }  
            }),  
            view = picker.getView();  
  
        view.on('render', me.setPickerViewStyles, me);  
        if (Ext.isIE9 && Ext.isStrict) {  
            view.on('highlightitem', me.repaintPickerView, me);  
            view.on('unhighlightitem', me.repaintPickerView, me);  
            view.on('afteritemexpand', me.repaintPickerView, me);  
            view.on('afteritemcollapse', me.repaintPickerView, me);  
        }  
        return picker;  
    },  
    setPickerViewStyles: function(view) {  
        view.getEl().setStyle({  
            'min-height': this.minPickerHeight + 'px',  
            'max-height': this.maxPickerHeight + 'px'  
        });  
    },  
    repaintPickerView: function() {  
        var style = this.picker.getView().getEl().dom.style;  
        style.display = style.display;  
    },  
    alignPicker: function() {  
        var me = this,  
            picker;  
  
        if (me.isExpanded) {  
            picker = me.getPicker();  
            if (me.matchFieldWidth) {  
                picker.setWidth(this.picker.getWidth());  
            }  
            if (picker.isFloating()) {  
                me.doAlign();  
            }  
        }  
    },  
    onItemClick: function(view, record, node, rowIndex, e) {  


//    	alert(record.data.id);
        this.selectItem(record);  
    },  
    onPickerKeypress: function(e, el) {  
        var key = e.getKey();  
  
        if(key === e.ENTER || (key === e.TAB && this.selectOnTab)) {  
            this.selectItem(this.picker.getSelectionModel().getSelection()[0]);  
        }  
    },  
    selectItem: function(record) {      
  // me.context.record.data.username=record.get('code');

        var me = this; 
        me.setValue(record.get('text'));  
        me.picker.hide();  
        me.inputEl.focus();  
        me.fireEvent('select', me, record);

    },  
    onExpand: function() {  
        var me = this,  
            picker = me.picker,  
            store = picker.store,  
            value = me.value;  
        if(value) {  
            var node = store.getNodeById(value);  
            if(node)  
                picker.selectPath(node.getPath());  
        } else {  
            var hasOwnProp = me.store.hasOwnProperty('getRootNode');  
            if(hasOwnProp)  
                picker.getSelectionModel().select(store.getRootNode());  
        }  
  
        Ext.defer(function() {  
            picker.getView().focus();  
        }, 1);  
    },  
    setValue: function(value) {  
        var me = this,record;  
        me.value = value;  
        if (me.store.loading) {  
            return me;  
        }  
        try{  
            //var hasOwnProp = me.store.hasOwnProperty('getRootNode');  
            //record = value ? me.store.getNodeById(value) : (hasOwnProp ? me.store.getRootNode() : null);  
            //me.setRawValue(record ? record.get(this.displayField) : '');  
            var i = value.indexOf(">");
            var j = value.indexOf("</");
            if(i != -1 && j != -1)
            {
            		me.setRawValue(value.substring(i+1,j));
            }
            else 
            {
            		me.setRawValue(value);
            }
        }catch(e){  
            me.setRawValue('');  
        }  
  
        return me;  
    },  
    getValue: function() {  
        return this.value;  
    },  
    onLoad: function(store,node,records) {  
        var value = this.value;  
      
      if (value) {  
          this.setValue(value);  
       }else{  
        if(this.firstSelected){  
                if(records && records.length > 0){  
                    var record = records[0];  

                    this.setValue(record.get(this.valueField));  
                }  
           }  
        }  
    },  
    getSubmitData: function() {  
        var me = this,  
            data = null;  
        if (!me.disabled && me.submitValue) {  
            data = {};  
            data[me.getName()] = '' + me.getValue();  
        }  
        return data;  
    },  
    onTriggerClick: function() {  
        var me = this;  
        //me.store.load();  
        if (!me.readOnly && !me.disabled) {  
            if (me.isExpanded) {  
                me.collapse();  
            } else {  
                me.expand();  
            }  
            me.inputEl.focus();  
        }  
    }  
});

 
 
 
 

/*用户管理*/
Ext.define('users.staffManager',{
    extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
		if(store_users_staffmanager.alreadyload !=1){
				store_users_staffmanager.userid = '';
				store_users_staffmanager.username = '';
				store_users_staffmanager.dispname = '';
				store_users_staffmanager.groupname = '';
				store_users_staffmanager.groupid = '';
				store_users_staffmanager.ssid = '';
				store_users_staffmanager.proname = '';
				store_users_staffmanager.openid = '';
				store_users_staffmanager.usermac = '';
				store_users_staffmanager.password = '';
				store_users_staffmanager.mark = '';
				store_users_staffmanager.expiretime = '';
				store_users_staffmanager.usertype = '';
				store_users_staffmanager.flags = '';
				store_users_staffmanager.maxbindmac = '';
				store_users_usermanager.groupcode = '';
				store_users_staffmanager.areaid = '';
		    store_users_staffmanager.keyword = '';
		    store_users_staffmanager.caExport = ''; //导出
			
				store_users_staffmanager.new_params={userid:store_users_staffmanager.userid,username:store_users_staffmanager.username,dispname:store_users_staffmanager.dispname,groupname:store_users_staffmanager.groupname,groupid:store_users_staffmanager.groupid,ssid:store_users_staffmanager.ssid,proname:store_users_staffmanager.proname,openid:store_users_staffmanager.openid,usermac:store_users_staffmanager.usermac,password:store_users_staffmanager.password,mark:store_users_staffmanager.mark,expiretime:store_users_staffmanager.expiretime,usertype:store_users_staffmanager.usertype,flags:store_users_staffmanager.flags,maxbindmac:store_users_staffmanager.maxbindmac,groupcode:store_users_staffmanager.groupcode,areaid:store_users_staffmanager.areaid,mokeyword:store_users_staffmanager.keyword,caExport:store_users_staffmanager.caExport};
		}
		function reflash(){
				store_users_staffmanager.new_params={userid:store_users_staffmanager.userid,username:store_users_staffmanager.username,dispname:store_users_staffmanager.dispname,groupname:store_users_staffmanager.groupname,groupid:store_users_staffmanager.groupid,ssid:store_users_staffmanager.ssid,proname:store_users_staffmanager.proname,openid:store_users_staffmanager.openid,usermac:store_users_staffmanager.usermac,password:store_users_staffmanager.password,mark:store_users_staffmanager.mark,expiretime:store_users_staffmanager.expiretime,usertype:store_users_staffmanager.usertype,flags:store_users_staffmanager.flags,maxbindmac:store_users_staffmanager.maxbindmac,groupcode:store_users_staffmanager.groupcode,areaid:store_users_staffmanager.areaid,mokeyword:store_users_staffmanager.keyword,caExport:store_users_staffmanager.caExport};
				//store_users_staffmanager.currentPage=1;
				store_users_staffmanager.load();
		}
		
				function gotoedit1(value,id,groupid,title,hgt){
				var groupcode = '';
				var groupid = '';
				if(title == "增加"){
						groupcode = store_users_usermanager.groupcode;
						groupid = store_users_usermanager.groupid;
				}
				var htmlvar='<iframe src="/pronline/Msg?FunName@ncm_webuserman_info&key@'+value+'&userid@'+id+'&groupcode@'+groupcode+'&groupid@'+groupid+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%"></iframe>';
				var windr = new Ext.Window ({
				                       id:"windr",
				                       title:"用户信息 - "+title,
				                       //x:0,
				                       //y:0,
				                       width:400,
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
		
				function gotoedit2(value){
			  store_users_usermacmanager.username = value;
				var windr3 = new Ext.Window ({
					     id:'usermacmanager_windr3',
               title:"MAC信息",
               //x:0,
               y:0,
               width:900,
               height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
               autoScroll:true,
             	 bodyPadding: 0,
               modal:true,
						   //headerPosition: 'bottom',
               resizable:true,
               closable:true,
               draggable:true,
               items:[Ext.create('users.usermacManager')]                    
		    })
		    
			  windr3.on("beforeclose", function() {
			  	 reflash();
			  })
			  windr3.show();
		}
		
		
		
		
		var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {
                Ext.getCmp('staffmanager_removeMac').setDisabled(selections.length == 0);
                Ext.getCmp('moveButton1').setDisabled(selections.length == 0);
               
            }
        }
    });	
    

    
    Ext.apply(this, {
        border:false,
				frame:false,
				id:'gridstaff',
				height: Ext.get("layout_center").getHeight()-63,
				//height:Ext.get("layout_center").getHeight(),
				//autoScroll: true,
				selModel: sm,
				//height: grid_height,
        store: store_users_staffmanager,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            text: "序号",
            dataIndex: 'userid', 
            width:110,
            hidden:true,
            sortable: true
        },
        {
            header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
            iconCls: 'user_edit',
            tooltip: '点击进入修改界面',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                    if(window.navigator.userAgent.indexOf("MSIE")>=1){
		                		gotoedit1('update', record.get('userid'),record.get('groupid'),"修改" , 443);
		                }else if(window.navigator.userAgent.indexOf("Firefox")>=1){
		                		gotoedit1('update', record.get('userid'),record.get('groupid'),"修改" , 443);
		                }else{
		                		gotoedit1('update', record.get('userid'),record.get('groupid'),"修改" , 443);
		                }
                
            }
        },
        
        
        
        {
            text: "登录名",
            dataIndex: 'username', 
            width:110,
            sortable: true
        },{
            text: "显示名",
            dataIndex: 'dispname', 
						width:100,
            sortable: true
        },{
            text: "单位名称",
            dataIndex: 'groupname',
            hidden:false, 
            width:100,
            sortable: true
        },{
            text: "设备型号",
            dataIndex: 'dev', 
            width:150,
            sortable: true
        },{
            text: "Portal",
            dataIndex: 'proname', 
            width:80,
            hidden:true,
            sortable: true
        },{
            text: "会员号",
            dataIndex: 'openid',
            hidden:true, 
            width:80,
            sortable: true
        },{
            text: "绑定MAC",
            dataIndex: 'usermac', 
            width:130,
            sortable: true
        },{
            text: "MAC数量",
            dataIndex: 'macnum', 
            width:60,
            hidden:true,
            align: 'right',
            sortable: true
        },{
            text: "最大MAC绑定数量",
            dataIndex: 'maxbindmac', 
            width:110,
            hidden:true,
            align: 'right',
            sortable: true
        },
        {
		            header: 'MAC信息',
		            xtype: 'actioncolumn',
		            dataIndex: 'userid',
		            iconCls: 'edit',
		            tooltip: '点击进入MAC信息界面',
		            width: 60,
		            align: 'center',
		            sortable: false,
		            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
		                gotoedit2(record.get('username'));
		            }
		    },
        
        
        
        /*{
            text: "密码",
            dataIndex: 'password', 
            hidden:true,
            width:80,
            sortable: true
        },*/{
            text: "标签",
            dataIndex: 'mark', 
            width:80,
            sortable: true
        },{
            text: "用户类别",
            dataIndex: 'usertype', 
            hidden:true,
            width:70,
            sortable: true
        },{
            text: "状态",
            dataIndex: 'flags', 
            width:70,
            sortable: true
        },{
            text: "有效时间",
            dataIndex: 'expiretime', 
            width:135,
            sortable: true
        },{
            text: "最后修改时间",
            dataIndex: 'moditime', 
            width:135,
            sortable: true,
            hidden:true
        },{
            text: "最后访问时间",
            dataIndex: 'lasttime', 
            width:135,
            sortable: true
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
                id:'staffmanager_keyword',
                name:'staffmanager_keyword',
								emptyText:'请输入账号/显示名',
                value:store_users_staffmanager.keyword,
                enableKeyEvents:true,
                listeners:{
		                'focus':function(){
				                if(this.value=='请输入账号/显示名'){                
				                    this.setValue('');
				                }
										},
		                'keydown' : function(i,e){
		                    var aae=e.getKey(); 
		                    if(aae==13){
		                    		var aa=Ext.getCmp('staffmanager_keyword').value;

								       	    store_users_staffmanager.keyword=aa;
								       	    
										    		store_users_staffmanager.currentPage=1;
										    		reflash();
		                    }
		                }
                }
            },
              {
             xtype:'treecombox',
             id: 'mgroupid1', 
						labelWidth: 60,
						fieldLabel:'选择单位',
            	name: 'mgroupname1',  
            	valueField: 'id',  
						  displayField: 'text',  
            	store:store_classtree
 				}, '-', {
                text:'查询',
            		iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('staffmanager_keyword').value;
	                 var bm = Ext.getCmp('mgroupid1').value;
		               
		               store_users_staffmanager.keyword=bb;
		               store_users_staffmanager.groupname = bm;
		               store_users_staffmanager.currentPage=1;         
		               reflash();  
                }
            },'-',{
                text:'解除绑定',
								id:'staffmanager_removeMac',
                iconCls:'remove',
                disabled: true,
                handler:function(){
										if(confirm('您真的要执行解除绑定操作吗？')){                  
											 var selsid;  
		                 	 var rows=sm.getSelection();
		                   selsid='';
		                   for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	 		selsid=rows[i].get('userid');		                  	 		
			                  	}
			                  	else{
			                       	selsid=selsid+","+rows[i].get('userid');
			                    }
		                   }
		                   store_users_staffmanager.load({params: {removemac: "removemac",selsid:selsid}});
                    }
                }
            },
            
           
         {
             xtype:'treecombox',
             id: 'mgroupid', 
						labelWidth: 60,
						fieldLabel:'移到单位',
            	name: 'mgroupname',  
            	valueField: 'id',  
						  displayField: 'text',  
            	store:store_classtree
 				}, {
                text:'确认',
                itemId: 'moveButton',
                id:'moveButton1',
                iconCls:'accept',
                disabled: true,
                handler:function(){
                
										if(confirm('您真的要执行移动定操作吗？')){                  
											 var selsid;  
											 var mgroup=Ext.getCmp('mgroupid').value;
										 
											 
		                 	 var rows=sm.getSelection();
		                   selsid='';
		                   for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	 		selsid=rows[i].get('userid');		                  	 		
			                  	}
			                  	else{
			                       	selsid=selsid+","+rows[i].get('userid');
			                    }
		                   }
		         
		                   store_users_staffmanager.load({params: {move: "move",selsid:selsid,mgroup:mgroup}});
                    } 
                }
 
                
            },
            
            
            
            
            
            
            
            
            
            /*'-', {
                text:'应用',
				        id:'staffmanager_refreshButton',
                iconCls:'accept',
                disabled: false,
                handler:function(){
						       store_users_staffmanager.load({params: {refresh: "refresh"}});
                }
            },*/'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		document.getElementById('form1').caExport.value = 'exp';
                		document.getElementById("form1").action = "/pronline/Msg?FunName@ncmWebShowStaffMan?r="+Math.random();
  									document.getElementById("form1").method = "POST";
	                	document.getElementById("form1").submit();
	                	document.getElementById("form1").caExport.value="";
                }
              },
               {
                 	
                text:'导入',
                iconCls:'edit',
                handler:function(){
                     var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_userman/user_dr_first.htm&groupid@'+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
					            var windr = new Ext.Window ({
                             id:"windr",
                             title:"导入用户",
                             x:200,
                             y:100,
                             width:500,
                             height:460,
                             bodyPadding: 0,
                             modal:true,
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
		           
            }
        ]}
    });
	  store_users_staffmanager.alreadyload = 1;
    this.callParent(arguments);
    }
})
