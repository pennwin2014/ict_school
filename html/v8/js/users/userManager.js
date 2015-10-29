/*员工管理*/
var bool = '';
if(isexp == 'false'){
		bool = false;	
}else{
		bool = true;	
}

Ext.define('users.userManager',{
    extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
		if(store_users_usermanager.alreadyload !=1){
				store_users_usermanager.userid = '';
				store_users_usermanager.username = '';
				store_users_usermanager.dispname = '';
				store_users_usermanager.groupname = '';
				store_users_usermanager.groupid = '';
				store_users_usermanager.ssid = '';
				store_users_usermanager.proname = '';
				store_users_usermanager.openid = '';
				store_users_usermanager.usermac = '';
				store_users_usermanager.password = '';
				store_users_usermanager.mark = '';
				store_users_usermanager.expiretime = '';
				store_users_usermanager.usertype = '';
				store_users_usermanager.flags = '0';
				store_users_usermanager.groupcode = '';
				store_users_usermanager.maxbindmac = '';
				store_users_usermanager.areaid = '';
		    store_users_usermanager.keyword = '';
		    store_users_usermanager.caExport = ''; //导出
			
				store_users_usermanager.new_params={userid:store_users_usermanager.userid,username:store_users_usermanager.username,dispname:store_users_usermanager.dispname,groupname:store_users_usermanager.groupname,groupid:store_users_usermanager.groupid,ssid:store_users_usermanager.ssid,proname:store_users_usermanager.proname,openid:store_users_usermanager.openid,usermac:store_users_usermanager.usermac,password:store_users_usermanager.password,mark:store_users_usermanager.mark,expiretime:store_users_usermanager.expiretime,usertype:store_users_usermanager.usertype,flags:store_users_usermanager.flags,maxbindmac:store_users_usermanager.maxbindmac,groupcode:store_users_usermanager.groupcode,areaid:store_users_usermanager.areaid,mokeyword:store_users_usermanager.keyword,caExport:store_users_usermanager.caExport};
		}
		function reflash(){
				store_users_usermanager.new_params={userid:store_users_usermanager.userid,username:store_users_usermanager.username,dispname:store_users_usermanager.dispname,groupname:store_users_usermanager.groupname,groupid:store_users_usermanager.groupid,ssid:store_users_usermanager.ssid,proname:store_users_usermanager.proname,openid:store_users_usermanager.openid,usermac:store_users_usermanager.usermac,password:store_users_usermanager.password,mark:store_users_usermanager.mark,expiretime:store_users_usermanager.expiretime,usertype:store_users_usermanager.usertype,flags:store_users_usermanager.flags,maxbindmac:store_users_usermanager.maxbindmac,groupcode:store_users_usermanager.groupcode,areaid:store_users_usermanager.areaid,mokeyword:store_users_usermanager.keyword,caExport:store_users_usermanager.caExport};
				//store_users_usermanager.currentPage=1;
				store_users_usermanager.load();
		}
		
		var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {
                Ext.getCmp('usermanager_removeButton').setDisabled(selections.length == 0);
                Ext.getCmp('usermanager_removeMac').setDisabled(selections.length == 0);
            }
        }
    });
    
    //加载SSID
	  var flagsStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  		    {groupname:'在职', groupid:'0'},
	  				{groupname:'离司', groupid:'9'},
	  				{groupname:'待处理', groupid:'2'}
	  		]
		});
    var flagsCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'usermanager_flagsCombo',
				fieldLabel: '状态',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
        emptyText:'请选择',
				value:store_users_usermanager.flags,
        displayField: 'groupname',
        name : 'usermanager_flagsCombo',      
        allowBlank: true,
        store: flagsStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
	          	  if(this.value != 0){
	          	  		Ext.getCmp('usermanager_addButton').setVisible(false);
	          	  		Ext.getCmp('usermanager_removeButton').setVisible(false);	
	          	  		Ext.getCmp('usermanager_removeMac').setVisible(false);
	          	  		Ext.getCmp('usermanager_refreshButton').setVisible(false);
	          	  		Ext.getCmp('usermanager_expButton').setVisible(false);
	          	  }else{
	          	  		Ext.getCmp('usermanager_addButton').setVisible(true);	
	          	  		Ext.getCmp('usermanager_removeButton').setVisible(true);	
	          	  		Ext.getCmp('usermanager_removeMac').setVisible(true);	
	          	  		Ext.getCmp('usermanager_refreshButton').setVisible(true);
	          	  		if(bool){
	          	  				Ext.getCmp('usermanager_expButton').setVisible(false);	
	          	  	  }else{
	          	  	  		Ext.getCmp('usermanager_expButton').setVisible(true);	
	          	  	  }
	          	  }
	            	store_users_usermanager.flags=this.value;
								reflash();
            },
            
            render:function(){
            	  if(this.value != 0){
            	  		Ext.getCmp('usermanager_addButton').setVisible(false);
	          	  		Ext.getCmp('usermanager_removeButton').setVisible(false);	
	          	  		Ext.getCmp('usermanager_removeMac').setVisible(false);
	          	  		Ext.getCmp('usermanager_refreshButton').setVisible(false);
	          	  		Ext.getCmp('usermanager_expButton').setVisible(false);	
            	  }else{
            	  		Ext.getCmp('usermanager_addButton').setVisible(true);	
	          	  		Ext.getCmp('usermanager_removeButton').setVisible(true);		
	          	  		Ext.getCmp('usermanager_removeMac').setVisible(true);	
	          	  		Ext.getCmp('usermanager_refreshButton').setVisible(true);
	          	  		if(bool){
	          	  				Ext.getCmp('usermanager_expButton').setVisible(false);	
	          	  	  }else{
	          	  	  		Ext.getCmp('usermanager_expButton').setVisible(true);	
	          	  	  }		
            	  }
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		}); 	
		
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
		
    Ext.apply(this, {
        border:false,
				frame:false,
				id:'griduser',
				height: Ext.get("layout_center").getHeight()-63,
				//height:Ext.get("layout_center").getHeight(),
				//autoScroll: true,
				selModel: sm,
				//height: grid_height,
        store: store_users_usermanager,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            text: "序号",
            dataIndex: 'userid', 
            width:110,
            hidden:true,
            sortable: true
        },{
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
        },{
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
            width:100,
            sortable: true
        },{
            text: "SSID",
            dataIndex: 'ssid', 
            width:120,
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
            width:80,
            sortable: true
        },/*{
            text: "MAC信息",
            dataIndex: 'usermac', 
            width:130,
            sortable: true
        },*/{
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
		    },{
            text: "MAC数量",
            dataIndex: 'macnum', 
            width:60,
            align: 'right',
            sortable: true
        },{
            text: "最大MAC绑定数量",
            dataIndex: 'maxbindmac', 
            width:110,
            align: 'right',
            sortable: true
        },/*{
            text: "密码",
            dataIndex: 'password', 
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
        listeners:{
						'itemdblclick':function(grid,record,item,index,e){
								gotoedit1('update', record.get('userid'), record.get('groupid'),"修改");
						}
				},  
        columnLines: true,
        tbar:{items:[
            '-',{
            		labelWidth: 45,
            		width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'usermanager_keyword',
                name:'usermanager_keyword',
								emptyText:'请输入账号/显示名',
                value:store_users_usermanager.keyword,
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
		                    		var aa=Ext.getCmp('usermanager_keyword').value;
								       	    store_users_usermanager.keyword=aa;
										    		store_users_usermanager.currentPage=1;
										    		reflash();
		                    }
		                }
                }
            },'-',{
                text:'查询',
            		iconCls:'accept',
                handler:function(){
	                 var bb=Ext.getCmp('usermanager_keyword').value;
		               store_users_usermanager.keyword=bb;
		               store_users_usermanager.currentPage=1;         
		               reflash();  
                }
            },'-',{
                 text:'增加',
                 id:'usermanager_addButton',
                 iconCls:'add',
	               handler : function() {
                  	if(window.navigator.userAgent.indexOf("MSIE")>=1){
		                		gotoedit1('add', '', '','增加', 413);
		                }else if(window.navigator.userAgent.indexOf("Firefox")>=1){
		                		gotoedit1('add', '', '','增加', 413);	
		                }else{
		                		gotoedit1('add', '', '','增加', 413);	
		                }
	               }
            },'-',{
                text:'删除',
								id:'usermanager_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
										if(confirm('您真的要执行删除操作吗？')){                  
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
		                   var gid=store_users_usermanager.groupid;
		                   store_users_usermanager.load({params: {del: "del",selsid:selsid,groupid: gid }});
		                   //store_users_usermanager.load({params: {del: "del",selsid:selsid}});
                    }
                }
            },/*'-',{
		           	text:'导入',
		           	id:'imp',
		           	iconCls:'exp',
		           	handler: function(){
					            var htmlvar='<iframe src="/v8/cust_dr_first.htm'+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
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
            },*/'-',{
                text:'解除绑定',
								id:'usermanager_removeMac',
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
		                   store_users_usermanager.load({params: {removemac: "removemac",selsid:selsid}});
                    }
                }
            },'-', {
                text:'应用',
				        id:'usermanager_refreshButton',
                iconCls:'accept',
                disabled: false,
                handler:function(){
						       store_users_usermanager.load({params: {refresh: "refresh"}});
                }
            },'-',flagsCombo,'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		document.getElementById('form1').caExport.value = 'exp';
                		document.getElementById("form1").flags.value = Ext.getCmp('usermanager_flagsCombo').value;
			            	document.getElementById("form1").action = "/pronline/Msg?FunName@ncmWebShowUserMan?r="+Math.random();
  									document.getElementById("form1").method = "POST";
	                	document.getElementById("form1").submit();
	                	document.getElementById("form1").caExport.value="";
                }
            },'-',{
            		text:'批量导入',
            		iconCls:'exp',
            		id:'usermanager_expButton',
            		hidden:bool,
            		handler:function(){
	               	  var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncmsysman/ncm_webusermanager_dr.htm" scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
								    var windr2 = new Ext.Window ({
								                           id:"usermanager_windr2",
								                           title:"上传文件",
								                           x:0,
								                           y:0,
								                           width:400,
								                           height:150,
								                           bodyPadding: 0,
								                           modal:true,
																					 //headerPosition: 'bottom',
								                           resizable:true,
								                           closable :true,
								                           draggable:true,
								                           html:htmlvar
								    })
									  windr2.show();
	              }
            }
        ]}
    });
	  store_users_usermanager.alreadyload = 1;
    this.callParent(arguments);
    }
})