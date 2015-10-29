﻿/*广告日志*/
Ext.define('logs.adlog',{
    extend: 'Ext.grid.Panel',
		autoScroll: false,
    frame: false,
    initComponent : function(){
	
		if(store_logs_adlog.alreadyload !=1){
		store_logs_adlog.time_flag='1';
		store_logs_adlog.slogdate = today;
		store_logs_adlog.elogdate = today;
		store_logs_adlog.slogtime = '00:00:00';
		store_logs_adlog.elogtime = '23:59:59';
		store_logs_adlog.sdate='';
		store_logs_adlog.edate='';
		store_logs_adlog.sid='';
		store_logs_adlog.refer='';
		store_logs_adlog.url='';
		store_logs_adlog.ncname='';
		store_logs_adlog.username='';
		store_logs_adlog.sip='';
		store_logs_adlog.mac='';
		store_logs_adlog.adcod='';
		store_logs_adlog.apname='';
		store_logs_adlog.apssid='';
		store_logs_adlog.phtype='';
		store_logs_adlog.os='';
		store_logs_adlog.sptype='';
		store_logs_adlog.devtype='';
		store_logs_adlog.browser='';
		store_logs_adlog.areacode='';
		store_logs_adlog.areaname='';
		store_logs_adlog.countrycode='';
		store_logs_adlog.lasttime='';
		store_logs_adlog.lcount='';
		store_logs_adlog.keyword='';
		store_logs_adlog.caExport='';
		store_logs_adlog.new_params= {time_flag:store_logs_adlog.time_flag,sdate:store_logs_adlog.sdate,edate:store_logs_adlog.edate,sid:store_logs_adlog.sid,refer:store_logs_adlog.refer,url:store_logs_adlog.url,ncname:store_logs_adlog.ncname,username:store_logs_adlog.username,sip:store_logs_adlog.sip,mac:store_logs_adlog.mac,adcod:store_logs_adlog.adcod,apname:store_logs_adlog.apname,apssid:store_logs_adlog.apssid,phtype:store_logs_adlog.phtype,os:store_logs_adlog.os,sptype:store_logs_adlog.sptype,devtype:store_logs_adlog.devtype,browser:store_logs_adlog.browser,areacode:store_logs_adlog.areacode,areaname:store_logs_adlog.areaname,countrycode:store_logs_adlog.countrycode,lasttime:store_logs_adlog.lasttime,lcount:store_logs_adlog.lcount,keyword:store_logs_adlog.keyword,caExport:store_logs_adlog.caExport};
		}
		function reflash(){
				store_logs_adlog.new_params= {time_flag:store_logs_adlog.time_flag,sdate:store_logs_adlog.sdate,edate:store_logs_adlog.edate,sid:store_logs_adlog.sid,refer:store_logs_adlog.refer,url:store_logs_adlog.url,ncname:store_logs_adlog.ncname,username:store_logs_adlog.username,sip:store_logs_adlog.sip,mac:store_logs_adlog.mac,adcod:store_logs_adlog.adcod,apname:store_logs_adlog.apname,apssid:store_logs_adlog.apssid,phtype:store_logs_adlog.phtype,os:store_logs_adlog.os,sptype:store_logs_adlog.sptype,devtype:store_logs_adlog.devtype,browser:store_logs_adlog.browser,areacode:store_logs_adlog.areacode,areaname:store_logs_adlog.areaname,countrycode:store_logs_adlog.countrycode,lasttime:store_logs_adlog.lasttime,lcount:store_logs_adlog.lcount,keyword:store_logs_adlog.keyword,caExport:store_logs_adlog.caExport};
				store_logs_adlog.currentPage=1;
				store_logs_adlog.load();
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
				id:'adlog_timeFlagCombo',
				fieldLabel: '时间',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
				emptyText:'请选择',
        value:store_logs_adlog.time_flag,
        displayField: 'groupname',
        name : 'time_flag',      
        allowBlank: true,
        store: timeFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
					store_logs_adlog.time_flag=this.value;
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
					    title:'广告日志高级查询窗口',
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
	            	id:'adlog_form2', 
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[{   						
	                        	fieldLabel:'开始日期',
	                        	xtype: 'datefield',
	                        	format: "Y/m/d",
	                        	value:store_logs_adlog.slogdate,
	                        	name:'slogdate'
	                    	},{
	                    		fieldLabel:'开始时间',
	                    		xtype:'timefield',
						        value:store_logs_adlog.slogtime,
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
	                        	value:store_logs_adlog.elogdate,
	                        	name:'elogdate'
	                    	},{
	                    		fieldLabel:'结束时间',
	                    		xtype:'timefield',
						        value:store_logs_adlog.elogtime,
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
								value:store_logs_adlog.username,
	                        	name:'username'
	                    	},{
	                        	fieldLabel:'广告代码',
	                        	xtype: 'textfield',
								emptyText:'请输入广告代码',
								value:store_logs_adlog.adcod,
	                        	name:'adcod'
	                    	},{
	                        	fieldLabel:'MAC',
	                        	xtype: 'textfield',
								emptyText:'请输入MAC地址',
								value:store_logs_adlog.mac,
	                        	name:'mac'
	                    	},{
	                        	fieldLabel:'IP',
	                        	xtype: 'textfield',
								emptyText:'请输入IP地址',
								value:store_logs_adlog.sip,
	                        	name:'sip'
	                    	}
							]  
	                })],  
					        buttons:[{
										text:"确定",
										handler:function(){
						        			var objectTmp = Ext.getCmp("adlog_form2").getForm().getValues();
											store_logs_adlog.slogdate = objectTmp.slogdate;
											store_logs_adlog.elogdate = objectTmp.elogdate;
											store_logs_adlog.slogtime = objectTmp.slogtime;
											store_logs_adlog.elogtime = objectTmp.elogtime;
						        			store_logs_adlog.sdate = objectTmp.slogdate+" "+objectTmp.slogtime;
						        			store_logs_adlog.edate = objectTmp.elogdate+" "+objectTmp.elogtime;
						        			store_logs_adlog.username = objectTmp.username;
						        			store_logs_adlog.adcod = objectTmp.adcod;
						        			store_logs_adlog.mac = objectTmp.mac;
						        			store_logs_adlog.sip = objectTmp.sip;
											Ext.getCmp('adlog_timeFlagCombo').setValue('0');
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
        //scrollOffset: 0,
        border:false,
				frame:false,
				//width: Ext.get("layout_center").getWidth()-5, 
				height:Ext.get("layout_center").getHeight()-60,
				autoScroll: true,
				//height: grid_height,
        store: store_logs_adlog,
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
            text: "请求网址",
            dataIndex: 'refer',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "广告网址",
            dataIndex: 'url',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "场所名称",
            dataIndex: 'ncname',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "用户名",
            dataIndex: 'username',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "IP",
            dataIndex: 'sip',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "Mac",
            dataIndex: 'mac',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "广告代码",
            dataIndex: 'adcod',
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
            text: "apssid",
            dataIndex: 'apssid',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "上网终端",
            dataIndex: 'phtype',
            width: 80,
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
            text: "运营商类别",
            dataIndex: 'sptype',
            width: 80,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "终端类型",
            dataIndex: 'devtype',
            width: 80,
            //align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "浏览器",
            dataIndex: 'browser',
            width: 80,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "区号",
            dataIndex: 'areacode',
            width: 80,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "区域名称",
            dataIndex: 'areaname',
            width: 80,
            //align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "国家代码",
            dataIndex: 'countrycode',
            width: 80,
            //align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "统计时间",
            dataIndex: 'lasttime',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "计数",
            dataIndex: 'lcount',
            width: 80,
            //align: 'center',
            hidden: true,
            sortable: false
        },{
        	 flex: 1	
        }],
          
        columnLines: true,
        tbar:{items:[
            '-',timeFlagCombo,//时间下拉菜单
            '-',{
	            	labelWidth: 45,
	            	width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'adlog_keyword',
                name:'adlog_keyword',
								emptyText:'请输入用户名/广告代码',
                value:store_logs_adlog.keyword,
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
		                    		var aa=Ext.getCmp('adlog_keyword').value;
				                    if(aa!='输入用户名/广告代码'){
						       	            store_logs_adlog.keyword=aa;
								            store_logs_adlog.currentPage=1;
								            reflash();
						                }
		                    }
		                }
                }
            },'-',{
                text:'查询',
                //itemId: 'moveButton',
            		iconCls:'accept',
                //disabled: true,
                handler:function(){
	                 var bb=Ext.getCmp('adlog_keyword').value;
		               if(bb!='输入用户名/广告代码'){         
		                 store_logs_adlog.keyword=bb;
						  
		               }
		               else{
		                  store_logs_adlog.keyword='';
		               }
		               store_logs_adlog.currentPage=1;         
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
		store_logs_adlog.alreadyload = 1;
    this.callParent(arguments);
    }
})