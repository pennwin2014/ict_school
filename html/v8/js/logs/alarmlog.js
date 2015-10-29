/*告警日志*/
Ext.define('logs.alarmlog',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
				if(store_logs_alarmlog.alreadyload !=1){
						store_logs_alarmlog.time_flag='1';
						store_logs_alarmlog.sort_flag='2';
						store_logs_alarmlog.slogdate = today;
						store_logs_alarmlog.elogdate = today;
						store_logs_alarmlog.slogtime = '00:00:00';
						store_logs_alarmlog.elogtime = '23:59:59';
						store_logs_alarmlog.sdate='';
						store_logs_alarmlog.edate='';
						//store_logs_alarmlog.sid='';
						store_logs_alarmlog.portalname='';
						store_logs_alarmlog.userid='';
						store_logs_alarmlog.groupid='';
						store_logs_alarmlog.ssid='1';
						store_logs_alarmlog.username='';
						//store_logs_alarmlog.dispname='';
						store_logs_alarmlog.mac='';
						store_logs_alarmlog.ip='';
						//store_logs_alarmlog.apname='';
						store_logs_alarmlog.os='';
						store_logs_alarmlog.bro='';
						store_logs_alarmlog.dev='';
						store_logs_alarmlog.devtype='';
						store_logs_alarmlog.ubytes='';
						store_logs_alarmlog.dbytes='';
						//store_logs_alarmlog.starttime='';
						//store_logs_alarmlog.endtime='';
						//store_logs_alarmlog.conntime='';
						//store_logs_alarmlog.cause='';
						store_logs_alarmlog.keyword='';
						store_logs_alarmlog.caExport='';
						//store_logs_alarmlog.new_params = {time_flag:store_logs_alarmlog.time_flag,sdate:store_logs_alarmlog.sdate,edate:store_logs_alarmlog.edate,sid:store_logs_alarmlog.sid,portalname:store_logs_alarmlog.portalname,userid:store_logs_alarmlog.userid,groupid:store_logs_alarmlog.groupid,username:store_logs_alarmlog.username,dispname:store_logs_alarmlog.dispname,ssid:store_logs_alarmlog.ssid,apname:store_logs_alarmlog.apname,mac:store_logs_alarmlog.mac,ip:store_logs_alarmlog.ip,dev:store_logs_alarmlog.dev,os:store_logs_alarmlog.os,bro:store_logs_alarmlog.bro,devtype:store_logs_alarmlog.devtype,ubytes:store_logs_alarmlog.ubytes,dbytes:store_logs_alarmlog.dbytes,starttime:store_logs_alarmlog.starttime,endtime:store_logs_alarmlog.endtime,conntime:store_logs_alarmlog.conntime,cause:store_logs_alarmlog.cause,keyword:store_logs_alarmlog.keyword,caExport:store_logs_alarmlog.caExport};
						store_logs_alarmlog.new_params = {time_flag:store_logs_alarmlog.time_flag,sort_flag:store_logs_alarmlog.sort_flag,sdate:store_logs_alarmlog.sdate,edate:store_logs_alarmlog.edate,portalname:store_logs_alarmlog.portalname,userid:store_logs_alarmlog.userid,groupid:store_logs_alarmlog.groupid,username:store_logs_alarmlog.username,ssid:store_logs_alarmlog.ssid,mac:store_logs_alarmlog.mac,ip:store_logs_alarmlog.ip,dev:store_logs_alarmlog.dev,os:store_logs_alarmlog.os,bro:store_logs_alarmlog.bro,devtype:store_logs_alarmlog.devtype,ubytes:store_logs_alarmlog.ubytes,dbytes:store_logs_alarmlog.dbytes,keyword:store_logs_alarmlog.keyword,caExport:store_logs_alarmlog.caExport};
				}
		 
				function reflash(){
					  //store_logs_alarmlog.new_params = {time_flag:store_logs_alarmlog.time_flag,sdate:store_logs_alarmlog.sdate,edate:store_logs_alarmlog.edate,sid:store_logs_alarmlog.sid,portalname:store_logs_alarmlog.portalname,userid:store_logs_alarmlog.userid,groupid:store_logs_alarmlog.groupid,username:store_logs_alarmlog.username,dispname:store_logs_alarmlog.dispname,ssid:store_logs_alarmlog.ssid,apname:store_logs_alarmlog.apname,mac:store_logs_alarmlog.mac,ip:store_logs_alarmlog.ip,dev:store_logs_alarmlog.dev,os:store_logs_alarmlog.os,bro:store_logs_alarmlog.bro,devtype:store_logs_alarmlog.devtype,ubytes:store_logs_alarmlog.ubytes,dbytes:store_logs_alarmlog.dbytes,starttime:store_logs_alarmlog.starttime,endtime:store_logs_alarmlog.endtime,conntime:store_logs_alarmlog.conntime,cause:store_logs_alarmlog.cause,keyword:store_logs_alarmlog.keyword,caExport:store_logs_alarmlog.caExport};
						store_logs_alarmlog.new_params = {time_flag:store_logs_alarmlog.time_flag,sort_flag:store_logs_alarmlog.sort_flag,sdate:store_logs_alarmlog.sdate,edate:store_logs_alarmlog.edate,portalname:store_logs_alarmlog.portalname,userid:store_logs_alarmlog.userid,groupid:store_logs_alarmlog.groupid,username:store_logs_alarmlog.username,ssid:store_logs_alarmlog.ssid,mac:store_logs_alarmlog.mac,ip:store_logs_alarmlog.ip,dev:store_logs_alarmlog.dev,os:store_logs_alarmlog.os,bro:store_logs_alarmlog.bro,devtype:store_logs_alarmlog.devtype,ubytes:store_logs_alarmlog.ubytes,dbytes:store_logs_alarmlog.dbytes,keyword:store_logs_alarmlog.keyword,caExport:store_logs_alarmlog.caExport};
						//store_logs_alarmlog.currentPage=1;
						store_logs_alarmlog.load();
				}
				
				//加载时间
			  var timeFlagStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  				{groupname:'今天', groupid:'1'},
			  				{groupname:'最近三天', groupid:'2'},
			  				{groupname:'最近一周', groupid:'3'},
			  				{groupname:'最近一月', groupid:'4'},
			  				{groupname:'最近三月', groupid:'5'},
			  				{groupname:'最近半年', groupid:'6'},
			  				{groupname:'自定义', groupid:'0'}
			  		]
				});
		    var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
						id:'alarmlog_timeFlagCombo',
						fieldLabel: '时间',
		        labelWidth: 30,
		        width: 130,
		        valueField:'groupid',
		        emptyText:'请选择',
						value:store_logs_alarmlog.time_flag,
		        displayField: 'groupname',
		        name : 'time_flag',      
		        allowBlank: true,
		        store: timeFlagStore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
			            	store_logs_alarmlog.time_flag=this.value;
										reflash();
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				});  
				//排序方法
			  var sortFlagStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  				{groupname:'开始时间升序', groupid:'1'},
			  				{groupname:'开始时间倒序', groupid:'2'},
			  				{groupname:'结束时间升序', groupid:'3'},
			  				{groupname:'结束时间倒序', groupid:'4'}
			  		]
				});
		    var sortFlagCombo = Ext.create('Ext.form.field.ComboBox', {
						id:'alarmlog_sortFlagCombo',
						fieldLabel: '排序',
		        labelWidth: 30,
		        width: 140,
		        valueField:'groupid',
		        emptyText:'请选择',
						value:store_logs_alarmlog.sort_flag,
		        displayField: 'groupname',
		        name : 'sort_flag',      
		        allowBlank: true,
		        store: sortFlagStore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
				            store_logs_alarmlog.sort_flag=this.value;
										reflash();
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				}); 
				//加载SSID
			  var ssidStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  		    {groupname:'全部', groupid:'0'},
			  				{groupname:'内部员工', groupid:'1'},
			  				{groupname:'其他用户', groupid:'2'}
			  		]
				});
		    var ssidCombo = Ext.create('Ext.form.field.ComboBox', {
						id:'alarmlog_ssidCombo',
						fieldLabel: '用户类型',
		        labelWidth: 60,
		        width: 160,
		        valueField:'groupid',
		        emptyText:'请选择',
						value:store_logs_alarmlog.ssid,
		        displayField: 'groupname',
		        name : 'alarmlog_ssidCombo',      
		        allowBlank: true,
		        store: ssidStore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
			            	store_logs_alarmlog.ssid=this.value;
										reflash();
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				}); 
					
			  //高级查询窗口
				var win3 = null;
			  function showCxForm() {
			    if (!win3) //判断如果不存在就创建新的
			    {
			        win3 = new Ext.Window({
							    title:'用户日志高级查询窗口',
							    closeAction: 'hide',
							    layout:'form',  
							    width:300,
							    draggable:true, //可拖动的
						      //maximizable:true, //可最大化的
						      //resizable: true, //可改变大小
			            modal: false,//后面的内容可以操作
						      //plain：true,//则主体背景透明
						      //items: fp2
							    constrain:true, //限制窗口只能在其容器内移动
						      //minimizable:true, //可最小化
						      
			            items:[new Ext.FormPanel({
			            	 	id:'alarmlog_form2', 
											layout:"form",
											baseCls:"x-plain",
											bodyPadding: 5,
											items:[{   						
                        	fieldLabel:'开始日期',
                        	xtype: 'datefield',
                        	format: "Y/m/d",
                        	value:store_logs_alarmlog.slogdate,
                        	name:'slogdate'
                    	},{
                    			fieldLabel:'开始时间',
                    			xtype:'timefield',
							            value:store_logs_alarmlog.slogtime,
							            name:'slogtime',
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
                        	fieldLabel:'结束日期',
                        	xtype: 'datefield',
                        	format: "Y/m/d",
                        	value:store_logs_alarmlog.elogdate,
                        	name:'elogdate'
                    	},{
                    			fieldLabel:'结束时间',
                    			xtype:'timefield',
							            value:store_logs_alarmlog.elogtime,
							            name:'elogtime',
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
	                      	fieldLabel:'Portal',
	                      	xtype: 'textfield',
													emptyText:'请输入Portal',
													value:store_logs_alarmlog.portalname,
			                    name:'portalname'
                    	},{
                        	fieldLabel:'用户名',
                        	xtype: 'textfield',
													emptyText:'请输入用户名',
													value:store_logs_alarmlog.username,
                        	name:'username'
                    	},{
                        	fieldLabel:'MAC',
                        	xtype: 'textfield',
													emptyText:'请输入MAC地址',
													value:store_logs_alarmlog.mac,
	                      	name:'mac'
	                  	},{
	                      	fieldLabel:'IP',
	                      	xtype: 'textfield',
													emptyText:'请输入IP地址',
													value:store_logs_alarmlog.ip,
                        	name:'ip'
                    	}]  
			            })],  
					        buttons:[{
										  text:"确定",
										  handler:function(){
						        			var objectTmp = Ext.getCmp("alarmlog_form2").getForm().getValues();
													store_logs_alarmlog.slogdate = objectTmp.slogdate;
													store_logs_alarmlog.elogdate = objectTmp.elogdate;
													store_logs_alarmlog.slogtime = objectTmp.slogtime;
													store_logs_alarmlog.elogtime = objectTmp.elogtime;
						        			store_logs_alarmlog.sdate = objectTmp.slogdate+" "+objectTmp.slogtime;
						        			store_logs_alarmlog.edate = objectTmp.elogdate+" "+objectTmp.elogtime;
						        			store_logs_alarmlog.portalname= objectTmp.portalname;
						        			store_logs_alarmlog.username = objectTmp.username;
						        			store_logs_alarmlog.mac = objectTmp.mac;
						        			store_logs_alarmlog.ip = objectTmp.ip;
										  		Ext.getCmp('alarmlog_timeFlagCombo').setValue('0');	
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
				
		    Ext.apply(this,{   
						height: Ext.get("layout_center").getHeight()-60,
		        border:false,
		        frame:false,
						autoScroll: false,
		        store: store_logs_alarmlog,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "序号",
		            dataIndex: 'sid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		            text: "Portal",
		            dataIndex: 'portalname',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "用户ID",
		            dataIndex: 'userid',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "用户名",
		            dataIndex: 'username',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "显示名称",
		            dataIndex: 'dispname',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "单位名称",
		            dataIndex: 'groupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "终端类型",
		            dataIndex: 'devtype',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "连接时间",
		            dataIndex: 'conntime',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "ssid",
		            dataIndex: 'ssid',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "apname",
		            dataIndex: 'apname',
		            width: 100,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "Mac",
		            dataIndex: 'mac',
		            width: 120,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "IP",
		            dataIndex: 'ip',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "终端品牌",
		            dataIndex: 'dev',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "操作系统",
		            dataIndex: 'os',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "浏览器",
		            dataIndex: 'bro',
		            width: 80,
		            //align: 'center',
		            hidden: true,
		            sortable: false
		        },{
		            text: "上行流量(Mb)",
		            dataIndex: 'ubytes',
		            width: 82,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "下行流量(Mb)",
		            dataIndex: 'dbytes',
		            width: 82,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "开始时间",
		            dataIndex: 'starttime',
		            width: 150,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "结束时间",
		            dataIndex: 'endtime',
		            width: 150,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "终止原因",
		            dataIndex: 'cause',
		            width: 150,
		            align: 'right',
		            hidden: true,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		          
		        columnLines: true,
		        tbar:{items:[
		        	  '-',sortFlagCombo,//排序下拉菜单
		        	  '-',ssidCombo,//用户类型
		            '-',timeFlagCombo,//时间下拉菜单
		            '-',{
		            		labelWidth: 45,
		            		width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                emptyText:'请输入用户名',
		                value:store_logs_alarmlog.keyword,
										id:'alarmlog_keyword',
		                name:'alarmlog_keyword',
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='请输入用户名'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('alarmlog_keyword').value;
					                    if(aa!='请输入用户名'){
							       	            store_logs_alarmlog.keyword=aa;
									                store_logs_alarmlog.currentPage=1;
									                reflash();
							                }
			                    }
			                }
		                }
		            },'-',{
		                text:'查询',
		            		iconCls:'accept',
		                handler:function(){
			                 	var bb=Ext.getCmp('alarmlog_keyword').value;       
				                store_logs_alarmlog.keyword=bb;
												store_logs_alarmlog.currentPage=1;   
												reflash();  
		                }
		            },'-',{
		                text:'高级',
		                anchor: 'right',
		                handler: showCxForm, //高级查询函数
		                iconCls:'option'
		            }
		        ]}
		    });
			  store_logs_alarmlog.alreadyload = 1;
		    this.callParent(arguments);
    }
})