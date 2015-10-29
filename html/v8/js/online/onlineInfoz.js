/*在线信息*/
Ext.define('online.onlineInfo',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
				if(store_online_onlineinfo.alreadyload !=1){
						store_online_onlineinfo.did1='';
						store_online_onlineinfo.gid='';
						store_online_onlineinfo.ru_name='';
						store_online_onlineinfo.ru_dept='';
						store_online_onlineinfo.ru_ip='';
						store_online_onlineinfo.ru_mac='';
						store_online_onlineinfo.ru_starttime='';
						store_online_onlineinfo.ru_conntime='';
						store_online_onlineinfo.ru_bytes_out='';
						store_online_onlineinfo.ru_bytes_in='';
						store_online_onlineinfo.ru_bytes='';
						store_online_onlineinfo.ru_bcount='';
						store_online_onlineinfo.cond='';
						store_online_onlineinfo.save='';
						store_online_onlineinfo.del='';
						store_online_onlineinfo.tr_gname='';
						store_online_onlineinfo.flow_flag='';
						store_online_onlineinfo.auth_flag='';
						store_online_onlineinfo.new_params={ru_name:store_online_onlineinfo.ru_name,tr_gname:store_online_onlineinfo.tr_gname,ru_dept:store_online_onlineinfo.ru_dept,ru_ip:store_online_onlineinfo.ru_ip,ru_mac:store_online_onlineinfo.ru_mac,ru_starttime:store_online_onlineinfo.ru_starttime,ru_conntime:store_online_onlineinfo.ru_conntime,ru_bytes_out:store_online_onlineinfo.ru_bytes_out,ru_bytes_in:store_online_onlineinfo.ru_bytes_in,ru_bytes:store_online_onlineinfo.ru_bytes,ru_bcount:store_online_onlineinfo.ru_bcount,groupid:store_online_onlineinfo.gid,did:store_online_onlineinfo.did1,cond:store_online_onlineinfo.cond,save:store_online_onlineinfo.save,del:store_online_onlineinfo.del,flow_flag:store_online_onlineinfo.flow_flag,auth_flag:store_online_onlineinfo.auth_flag};  
				}
				function reflash(){
				store_online_onlineinfo.new_params={ru_name:store_online_onlineinfo.ru_name,tr_gname:store_online_onlineinfo.tr_gname,ru_dept:store_online_onlineinfo.ru_dept,ru_ip:store_online_onlineinfo.ru_ip,ru_mac:store_online_onlineinfo.ru_mac,ru_starttime:store_online_onlineinfo.ru_starttime,ru_conntime:store_online_onlineinfo.ru_conntime,ru_bytes_out:store_online_onlineinfo.ru_bytes_out,ru_bytes_in:store_online_onlineinfo.ru_bytes_in,ru_bytes:store_online_onlineinfo.ru_bytes,ru_bcount:store_online_onlineinfo.ru_bcount,groupid:store_online_onlineinfo.gid,did:store_online_onlineinfo.did1,cond:store_online_onlineinfo.cond,save:store_online_onlineinfo.save,del:store_online_onlineinfo.del,flow_flag:store_online_onlineinfo.flow_flag,auth_flag:store_online_onlineinfo.auth_flag};  
				store_online_onlineinfo.currentPage=1;
				store_online_onlineinfo.load();
				}
		
				var sm = Ext.create('Ext.selection.CheckboxModel',{
				    listeners: {
				        selectionchange: function(sm, selections) {
				            Ext.getCmp('onlineInfo_removeButton').setDisabled(selections.length == 0);
				        }
				    }
				});
		
		
				//加载流量
			  var timeFlagStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  				{groupname:'有流量', groupid:'1'},
			  				{groupname:'无流量', groupid:'2'}
			  		]
				});
		    var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'onlineinfo_flow_flag',
				fieldLabel: '流量',
		        labelWidth: 30,
		        width: 130,
		        valueField:'groupid',
				emptyText:'请选择',
		        value:store_online_onlineinfo.flow_flag,
		        displayField: 'groupname',
		        name : 'flow_flag',      
		        allowBlank: true,
		        store: timeFlagStore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
							store_online_onlineinfo.flow_flag=this.value;
							reflash();
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				});  	
		    
		    //是否认证
			  var authFlagStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  				{groupname:'已认证', groupid:'1'},
			  				{groupname:'全部'  , groupid:'0'},
			  				{groupname:'未认证', groupid:'2'},
			  				{groupname:'退出认证', groupid:'9'}
			  		]
				});
		    var authFlagCombo = Ext.create('Ext.form.field.ComboBox', {
				    id:'onlineinfo_auth_flag',
				    fieldLabel: '用户状态',
		        labelWidth: 60,
		        width: 140,
		        valueField:'groupid',
				    emptyText:'已认证',
		        value:store_online_onlineinfo.auth_flag,
		        displayField: 'groupname',
		        name : 'auth_flag',      
		        allowBlank: true,
		        store: authFlagStore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
									store_online_onlineinfo.auth_flag=this.value;
									reflash();
									if(store_online_onlineinfo.auth_flag != 1)
										Ext.getCmp('onlineInfo_removeButton').setVisible(false);
									else
									  Ext.getCmp('onlineInfo_removeButton').setVisible(true);
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				});  		
				
		    Ext.apply(this, {   
		    //scrollOffset: 0,
		    border:false,
		frame:false,
		//width: Ext.get("layout_center").getWidth()-5, 
		//height: Ext.get("layout_center").getHeight()-58,
		height:Ext.get("layout_center").getHeight(),
		autoScroll: true,
		selModel: sm,
		//height: grid_height,
		    store: store_online_onlineinfo,
		    viewConfig:{
		      	loadMask : false
		      },
		    columns:[
		    {
		        id: 'on_name',
		        text: "登录名",
		        dataIndex: 'on_name',
		        width: 90,
		        sortable: true
		    },
		    {
		        id: 'on_dispname',
		        text: "显示名",
		        dataIndex: 'on_dispname',
		        width: 90,
		        sortable: true
		    }
		    ,{
		        text: "SSID",
		        dataIndex: 'on_ssid', 
		       width: 100,
		        sortable: false
		    },{
		        text: "AP名称",
		        dataIndex: 'on_apname',           
		        width: 120,
		//           flex: 1,
		        sortable: false
		//            renderer: ipgj
		    },
		    {
		        text: "MAC",
		        dataIndex: 'on_mac',
		        hidden: false,
		        width: 117,
		        sortable: false
		    }
		    ,
		    {
		        text: "IP",
		        dataIndex: 'on_ip',
		        hidden: false,
		        width: 110,
		        sortable: false
		    },
		    {
		        text: "设备类别",
		        dataIndex: 'on_temtype',
		        hidden: false,
		        width: 70,
		        sortable: false
		    },
		    {
		        text: "设备型号",
		        dataIndex: 'on_caDev',
		        hidden: false,
		        width: 70,
		        sortable: false
		    },
		    {
		        text: "操作系统",
		        dataIndex: 'on_os',
		        hidden: false,
		        width: 80,
		        sortable: false
		    },
		    {
		        text: "语言",
		        dataIndex: 'on_lang',
		        hidden: false,
		        width: 60,
		        hidden:true,
		        align: 'right',
		        sortable: true
		    }
		    ,{
		        text: "状态",
		        dataIndex: 'on_login',
		        width: 50,
		        align: 'right',
		        sortable: false
		    }
		    ,{
		        text: "流量",
		        dataIndex: 'on_byte',
		        width: 80,
		        align: 'right',
		        hidden:true,
		        sortable: false
		    },
		    {
		        text: "上线时间",
		        dataIndex: 'on_sdate',
		        width: 135,
		        align: 'left',
		//            hidden: true,
		        sortable: false
		    },
		    {
		        text: "最后操作时间",
		        dataIndex: 'on_lasttime',
		        width: 135,
		        align: 'right',
		        hidden: true,
		        sortable: false
		    },
		    {
		       flex:1,
		       sortable: false
		    }
		      ],
		      
		       columnLines: true,
		       tbar:{items:[
		        '-', 
		        //timeFlagCombo,
		         authFlagCombo,
		        '-',{
		        	labelWidth: 40,
		        	width:200,
		            fieldLabel:'关键字',
		            xtype: 'textfield',
		            id:'onlineinfo_cond',
		            name:'onlineinfo_cond',
				emptyText:'请输入登录名/SSID/MAC',
		            value:store_online_onlineinfo.cond,
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
		                  		var aa=Ext.getCmp('onlineinfo_cond').value;
			                    if(aa!='输入用户名/广告代码'){
					       	            store_online_onlineinfo.cond=aa;
							            store_online_onlineinfo.currentPage=1;
							            reflash();
					                }
		                  }
		              }
		            }
		        }, '-', {
		            text:'查询',
		        	  iconCls:'search2',
		            handler:function(){
		               var bb=Ext.getCmp('onlineinfo_cond').value;     
		               store_online_onlineinfo.cond=bb;
		               store_online_onlineinfo.currentPage=1;         
		               reflash();  
		            }
		        }, '-', {
		            text:'强制下线',
		            id:'onlineInfo_removeButton',
		            iconCls:'remove',
		            disabled: true,
		            handler:function(){
							  if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var rows=sm.getSelection();
			                selsid='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid = rows[i].get('on_name') ;
			                  	}
			                  	else{
			                       selsid = selsid + "," + rows[i].get('on_name') ;
			                    }
			                }
											store_online_onlineinfo.load({params: {del: "del",selsid:selsid}});
		               }
		            }
		        }
		      ]
		     }
		    });
				store_online_onlineinfo.alreadyload = 1;
alert(9);
//				reflash();
				store_online_onlineinfo.load();alert(4);
				setInterval("store_online_onlineinfo.load()",20000);
				//alert(intervalId);
				//window.clearInterval(intervalId);
		    this.callParent(arguments);
    }
})