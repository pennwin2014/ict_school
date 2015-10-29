	/*用户日志*/
	Ext.define('logs.userlog',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_logs_userlog.alreadyload !=1){
				store_logs_userlog.authenticat_flag = '1';
				store_logs_userlog.time_flag='1';
				store_logs_userlog.sort_flag='2';
				store_logs_userlog.slogdate = today;
				store_logs_userlog.elogdate = today;
				store_logs_userlog.slogtime = '00:00:00';
				store_logs_userlog.elogtime = '23:59:59';
				store_logs_userlog.sdate='';
				store_logs_userlog.edate='';
				//store_logs_userlog.sid='';
				store_logs_userlog.portalname='';
				store_logs_userlog.userid='';
				store_logs_userlog.groupid='';
				//store_logs_userlog.ssid='';
				store_logs_userlog.username='';
				//store_logs_userlog.dispname='';
				store_logs_userlog.mac='';
				store_logs_userlog.ip='';
				//store_logs_userlog.apname='';
				store_logs_userlog.os='';
				store_logs_userlog.bro='';
				store_logs_userlog.dev='';
				store_logs_userlog.devtype='';
				store_logs_userlog.ubytes='';
				store_logs_userlog.dbytes='';
				//store_logs_userlog.starttime='';
				//store_logs_userlog.endtime='';
				//store_logs_userlog.conntime='';
				//store_logs_userlog.cause='';
				store_logs_userlog.keyword='';
				store_logs_userlog.caExport='';
				//store_logs_userlog.new_params = {time_flag:store_logs_userlog.time_flag,sdate:store_logs_userlog.sdate,edate:store_logs_userlog.edate,sid:store_logs_userlog.sid,portalname:store_logs_userlog.portalname,userid:store_logs_userlog.userid,groupid:store_logs_userlog.groupid,username:store_logs_userlog.username,dispname:store_logs_userlog.dispname,ssid:store_logs_userlog.ssid,apname:store_logs_userlog.apname,mac:store_logs_userlog.mac,ip:store_logs_userlog.ip,dev:store_logs_userlog.dev,os:store_logs_userlog.os,bro:store_logs_userlog.bro,devtype:store_logs_userlog.devtype,ubytes:store_logs_userlog.ubytes,dbytes:store_logs_userlog.dbytes,starttime:store_logs_userlog.starttime,endtime:store_logs_userlog.endtime,conntime:store_logs_userlog.conntime,cause:store_logs_userlog.cause,keyword:store_logs_userlog.keyword,caExport:store_logs_userlog.caExport};
				store_logs_userlog.new_params = {authenticat_flag:store_logs_userlog.authenticat_flag,time_flag:store_logs_userlog.time_flag,sort_flag:store_logs_userlog.sort_flag,sdate:store_logs_userlog.sdate,edate:store_logs_userlog.edate,portalname:store_logs_userlog.portalname,userid:store_logs_userlog.userid,groupid:store_logs_userlog.groupid,username:store_logs_userlog.username,mac:store_logs_userlog.mac,ip:store_logs_userlog.ip,dev:store_logs_userlog.dev,os:store_logs_userlog.os,bro:store_logs_userlog.bro,devtype:store_logs_userlog.devtype,ubytes:store_logs_userlog.ubytes,dbytes:store_logs_userlog.dbytes,keyword:store_logs_userlog.keyword,caExport:store_logs_userlog.caExport};
		}
 
		function reflash(){
				store_logs_userlog.new_params = {authenticat_flag:store_logs_userlog.authenticat_flag,time_flag:store_logs_userlog.time_flag,sort_flag:store_logs_userlog.sort_flag,sdate:store_logs_userlog.sdate,edate:store_logs_userlog.edate,portalname:store_logs_userlog.portalname,userid:store_logs_userlog.userid,groupid:store_logs_userlog.groupid,username:store_logs_userlog.username,mac:store_logs_userlog.mac,ip:store_logs_userlog.ip,dev:store_logs_userlog.dev,os:store_logs_userlog.os,bro:store_logs_userlog.bro,devtype:store_logs_userlog.devtype,ubytes:store_logs_userlog.ubytes,dbytes:store_logs_userlog.dbytes,keyword:store_logs_userlog.keyword,caExport:store_logs_userlog.caExport};
				//store_logs_userlog.currentPage=1;
				store_logs_userlog.load();
		}
		
		//加载认证
	  var authenticatFlagStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'全部', groupid:'0'}，
	  				{groupname:'认证', groupid:'1'},
	  				{groupname:'未认证', groupid:'2'}
	  		]
		});
    var authenticatFlagCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'userlog_authenticatFlagCombo',
				fieldLabel: '状态',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
        emptyText:'请选择',
				value:store_logs_userlog.authenticat_flag,
        displayField: 'groupname',
        name : 'authenticat_flag',      
        allowBlank: true,
        store: authenticatFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
		            store_logs_userlog.authenticat_flag = this.value;
								reflash();
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		}); 
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
				id:'userlog_timeFlagCombo',
				fieldLabel: '时间',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
        emptyText:'请选择',
				value:store_logs_userlog.time_flag,
        displayField: 'groupname',
        name : 'time_flag',      
        allowBlank: true,
        store: timeFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
		            store_logs_userlog.time_flag=this.value;
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
				id:'userlog_sortFlagCombo',
				fieldLabel: '排序',
        labelWidth: 30,
        width: 140,
        valueField:'groupid',
        emptyText:'请选择',
				value:store_logs_userlog.sort_flag,
        displayField: 'groupname',
        name : 'sort_flag',      
        allowBlank: true,
        store: sortFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
		            store_logs_userlog.sort_flag=this.value;
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
	            	 id:'userlog_form2', 
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[{   						
	                        	fieldLabel:'开始日期',
	                        	xtype: 'datefield',
	                        	format: "Y/m/d",
	                        	value:store_logs_userlog.slogdate,
	                        	name:'slogdate'
	                    	},{
	                    			fieldLabel:'开始时间',
	                    			xtype:'timefield',
						            value:store_logs_userlog.slogtime,
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
	                        	value:store_logs_userlog.elogdate,
	                        	name:'elogdate'
	                    	},{
	                    			fieldLabel:'结束时间',
	                    			xtype:'timefield',
						            value:store_logs_userlog.elogtime,
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
								value:store_logs_userlog.portalname,
	                        	name:'portalname'
	                    	},{
	                        	fieldLabel:'用户名',
	                        	xtype: 'textfield',
								emptyText:'请输入用户名',
								value:store_logs_userlog.username,
	                        	name:'username'
	                    	},{
	                        	fieldLabel:'MAC',
	                        	xtype: 'textfield',
								emptyText:'请输入MAC地址',
								value:store_logs_userlog.mac,
	                        	name:'mac'
	                    	},{
	                        	fieldLabel:'IP',
	                        	xtype: 'textfield',
								emptyText:'请输入IP地址',
								value:store_logs_userlog.ip,
	                        	name:'ip'
	                    	},{
	                        	fieldLabel:'终端类型',
	                        	xtype: 'textfield',
								emptyText:'请输入终端类型',
								value:store_logs_userlog.devtype,
	                        	name:'devtype'
	                    	}]  
	                })],  
					        buttons:[{
										  text:"确定",
										  handler:function(){
						        			var objectTmp = Ext.getCmp("userlog_form2").getForm().getValues();
											store_logs_userlog.slogdate = objectTmp.slogdate;
											store_logs_userlog.elogdate = objectTmp.elogdate;
											store_logs_userlog.slogtime = objectTmp.slogtime;
											store_logs_userlog.elogtime = objectTmp.elogtime;
						        			store_logs_userlog.sdate = objectTmp.slogdate+" "+objectTmp.slogtime;
						        			store_logs_userlog.edate = objectTmp.elogdate+" "+objectTmp.elogtime;
						        			store_logs_userlog.portalname= objectTmp.portalname;
						        			store_logs_userlog.username = objectTmp.username;
						        			store_logs_userlog.mac = objectTmp.mac;
						        			store_logs_userlog.ip = objectTmp.ip;
						        			store_logs_userlog.devtype = objectTmp.devtype;
										  	Ext.getCmp('userlog_timeFlagCombo').setValue('0');	
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
		
    Ext.apply(this, {   
				height: Ext.get("layout_center").getHeight()-60,
        border:false,
        frame:false,
				autoScroll: false,
        store: store_logs_userlog,
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
            hidden: false,
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
            text: "显示名",
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
            hidden: false,
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
            text: "操作系统",
            dataIndex: 'os',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "浏览器",
            dataIndex: 'bro',
            width: 80,
            //align: 'center',
            hidden: false,
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
            text: "连接时间",
            dataIndex: 'conntime',
            width: 100,
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
        		'-',authenticatFlagCombo,//认证下拉菜单
        	  '-',sortFlagCombo,//排序下拉菜单
	          '-',timeFlagCombo,//时间下拉菜单
	          '-',{
	          		labelWidth: 40,
	          		width:200,
	              fieldLabel:'关键字',
	              xtype: 'textfield',
	              emptyText:'请输入Portal/用户名称',
	              value:store_logs_userlog.keyword,
								id:'userlog_keyword',
	              name:'userlog_keyword',
	              enableKeyEvents:true,
	              listeners:{
	                 'focus':function(){
			                 if(this.value=='请输入Portal/用户名称'){                
			                    this.setValue('');
			                 }
	                 },
	       
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('userlog_keyword').value;
			                    if(aa!='请输入Portal/用户名称'){
					       	            store_logs_userlog.keyword=aa;
							            store_logs_userlog.currentPage=1;
							                reflash();
					                }
	                    }
	                }
	              }
	          },'-',{
	              text:'查询',
	          		iconCls:'accept',
	              handler:function(){
	                 	var bb=Ext.getCmp('userlog_keyword').value;       
		                store_logs_userlog.keyword=bb;
										store_logs_userlog.currentPage=1;   
										reflash();  
	              }
	          },'-',{
	              text:'高级',
	              anchor: 'right',
	              handler: showCxForm, //高级查询函数
	              iconCls:'option'
	          }]
       	}
    });
		store_logs_userlog.alreadyload = 1;
    this.callParent(arguments);
    }
})