﻿	/*短信日志*/
	Ext.define('logs.smslog',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_logs_smslog.alreadyload !=1){
				store_logs_smslog.time_flag='1';
				store_logs_smslog.slogdate = today;
				store_logs_smslog.elogdate = today;
				store_logs_smslog.slogtime = '00:00:00';
				store_logs_smslog.elogtime = '23:59:59';
				store_logs_smslog.sdate='';
				store_logs_smslog.edate='';
				store_logs_smslog.sid='';
				store_logs_smslog.servicecode='';
				store_logs_smslog.userid='';
				store_logs_smslog.username='';
				store_logs_smslog.usermac='';
				store_logs_smslog.ip='';
				store_logs_smslog.logtime='';
				store_logs_smslog.smsname='';
				store_logs_smslog.sptype='';
				store_logs_smslog.areacode='';
				store_logs_smslog.areaname='';
				store_logs_smslog.contrycode='';
				store_logs_smslog.status='';
				store_logs_smslog.apmac='';
				store_logs_smslog.keyword='';
				store_logs_smslog.caExport='';
				store_logs_smslog.new_params ={time_flag:store_logs_smslog.time_flag,sdate:store_logs_smslog.sdate,edate:store_logs_smslog.edate,sid:store_logs_smslog.sid,servicecode:store_logs_smslog.servicecode,userid:store_logs_smslog.userid,username:store_logs_smslog.username,usermac:store_logs_smslog.usermac,ip:store_logs_smslog.ip,logtime:store_logs_smslog.logtime,smsname:store_logs_smslog.smsname,sptype:store_logs_smslog.sptype,areacode:store_logs_smslog.areacode,areaname:store_logs_smslog.areaname,contrycode:store_logs_smslog.contrycode,status:store_logs_smslog.status,apmac:store_logs_smslog.apmac,keyword:store_logs_smslog.keyword,caExport:store_logs_smslog.caExport};
		}
		function reflash(){
			store_logs_smslog.new_params ={time_flag:store_logs_smslog.time_flag,sdate:store_logs_smslog.sdate,edate:store_logs_smslog.edate,sid:store_logs_smslog.sid,servicecode:store_logs_smslog.servicecode,userid:store_logs_smslog.userid,username:store_logs_smslog.username,usermac:store_logs_smslog.usermac,ip:store_logs_smslog.ip,logtime:store_logs_smslog.logtime,smsname:store_logs_smslog.smsname,sptype:store_logs_smslog.sptype,areacode:store_logs_smslog.areacode,areaname:store_logs_smslog.areaname,contrycode:store_logs_smslog.contrycode,status:store_logs_smslog.status,apmac:store_logs_smslog.apmac,keyword:store_logs_smslog.keyword,caExport:store_logs_smslog.caExport};
			
			store_logs_smslog.currentPage=1;
			store_logs_smslog.load();
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
		id:'smslog_timeFlagCombo',
		fieldLabel: '时间',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
		emptyText:'请选择',
        value:store_logs_smslog.time_flag,
        displayField: 'groupname',
        name : 'time_flag',      
        allowBlank: true,
        store: timeFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
	               store_logs_smslog.time_flag=this.value;
					reflash();
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		});
	  // 运营商类别
	  var sptypeStore = Ext.create('Ext.data.Store', {
	  		fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'不合法', groupid:'0'},
	  				{groupname:'移动', groupid:'1'},
	  				{groupname:'联通', groupid:'2'},
	  				{groupname:'电信', groupid:'3'},
	  				{groupname:'其他', groupid:'8'},
	  				{groupname:'国际', groupid:'9'}
	  		]	
	  });
	  var sptypeCombo = Ext.create('Ext.form.field.ComboBox', {
		id:'smslog_sptypeCombo',
		fieldLabel: '类别',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
        emptyText:'请选择',
		value:store_logs_smslog.sptype,
        displayField: 'groupname',
        name : 'sptype',      
        allowBlank: true,
        store: sptypeStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
	          		
						store_logs_smslog.sptype=this.value;
						reflash();
	                
	          }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		})		
	  //高级查询窗口
		var win3 = null;
	  function showCxForm() {
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'短信日志高级查询窗口',
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
	            	 id:'smslog_form2', 
	               layout:"form",
						     baseCls:"x-plain",
	               bodyPadding: 5,
	               items:[{   						
	                        	fieldLabel:'开始日期',
	                        	xtype: 'datefield',
	                        	format: "Y/m/d",
	                        	value:store_logs_smslog.slogdate,
	                        	name:'slogdate'
	                    	},{
	                    			fieldLabel:'开始时间',
	                    			xtype:'timefield',
						              	value:store_logs_smslog.slogtime,
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
	                        	value:store_logs_smslog.elogdate,
	                        	name:'elogdate'
	                    	},{
	                    			fieldLabel:'结束时间',
	                    			xtype:'timefield',
						              	value:store_logs_smslog.elogtime,
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
	                        	fieldLabel:'用户名',
	                        	xtype: 'textfield',
								emptyText:'请输入用户名',
								value:store_logs_smslog.username,
	                        	name:'username'
	                    	},{
	                        	fieldLabel:'MAC',
	                        	xtype: 'textfield',
								emptyText:'请输入MAC地址',
								value:store_logs_smslog.mac,
	                        	name:'mac'
	                    	},{
	                        	fieldLabel:'IP',
	                        	xtype: 'textfield',
								emptyText:'请输入IP地址',
								value:store_logs_smslog.sip,
	                        	name:'sip'
	                    	},{
	                        	fieldLabel:'区号',
	                        	xtype: 'textfield',
								emptyText:'请输入区号',
								value:store_logs_smslog.areacode,
	                        	name:'areacode'
	                    	},{
	                        	fieldLabel:'区域名称',
	                        	xtype: 'textfield',
								emptyText:'请输入区域名称',
								value:store_logs_smslog.areaname,
	                        	name:'areaname'
	                    	}]  
	                })],  
					        buttons:[{
										  text:"确定",
										  handler:function(){
						        			var objectTmp = Ext.getCmp("smslog_form2").getForm().getValues();
											store_logs_smslog.slogdate = objectTmp.slogdate;
											store_logs_smslog.elogdate = objectTmp.elogdate;
											store_logs_smslog.slogtime = objectTmp.slogtime;
											store_logs_smslog.elogtime = objectTmp.elogtime;
						        			store_logs_smslog.sdate = objectTmp.slogdate+" "+objectTmp.slogtime;
						        			store_logs_smslog.edate = objectTmp.elogdate+" "+objectTmp.elogtime;
						        			store_logs_smslog.username = objectTmp.username;
						        			store_logs_smslog.adcod = objectTmp.adcod;
						        			store_logs_smslog.mac = objectTmp.mac;
						        			store_logs_smslog.sip = objectTmp.sip;
						        			Ext.getCmp('smslog_timeFlagCombo').setValue('0');
											reflash();
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
        scrollOffset: 0,
        border:false,
        frame:false,
        store: store_logs_smslog,
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
            sortable: true
        },{
            text: "单位名称",
            dataIndex: 'groupname',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "场所代码",
            dataIndex: 'Servicecode',
            width: 100,
            //align: 'center',
            hidden: true,
            sortable: true
        },{
            text: "AP位置",
            dataIndex: 'apmac',
            width: 120,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "MAC",
            dataIndex: 'usermac',
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
            text: "短信平台",
            dataIndex: 'smsname',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "运营商编码",
            dataIndex: 'sptype',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "区号",
            dataIndex: 'areacode',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "区域名称",
            dataIndex: 'areaname',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "国家代码",
            dataIndex: 'contrycode',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "注册时间",
            dataIndex: 'logtime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "发送状态",
            dataIndex: 'status',
            width: 60,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
        	 flex: 1	
        }],
		
        columnLines: true,
        tbar:{items:[
            '-',timeFlagCombo,//时间下拉菜单
            '-',sptypeCombo, //运营商类别下拉菜单
            '-',{
            	  labelWidth: 45,
            	  width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'smslog_keyword',
                name:'smslog_keyword',
								emptyText:'请输入用户名/MAC',
                value:store_logs_smslog.keyword,
                enableKeyEvents:true,
                listeners:{
	                 'focus':function(){
			                 if(this.value=='输入用户名/MAC'){                
			                    this.setValue('');
			                 }
                   },
         
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('smslog_keyword').value;
			                    if(aa!='输入用户名/MAC'){
					       	            store_logs_smslog.keyword=aa;
							            store_logs_smslog.currentPage=1;
							            reflash();
					                }
	                    }
	                }
                }
            },'-',{
                text:'查询',
            		iconCls:'accept',
                //disabled: true,
                handler:function(){
	                 var bb=Ext.getCmp('smslog_keyword').value;
		               if(bb!='输入用户名/MAC'){         
		                 store_logs_smslog.keyword=bb;
		               }
		               else{
		                 store_logs_smslog.keyword='';
		               }
		               store_logs_smslog.currentPage=1;         
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
		store_logs_smslog.alreadyload = 1;
    this.callParent(arguments);
    }
})
