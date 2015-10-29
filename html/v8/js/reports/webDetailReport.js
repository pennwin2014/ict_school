﻿
		
	Ext.define('reports.webDetailReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_reports_webdetailreport.alreadyload !=1){
		store_reports_webdetailreport.slogdate = today;
		store_reports_webdetailreport.elogdate = today;
		store_reports_webdetailreport.slogtime = '00:00:00';
		store_reports_webdetailreport.elogtime = '23:59:59';
		store_reports_webdetailreport.contrycode='';
		store_reports_webdetailreport.username='';
		store_reports_webdetailreport.mac='';
		store_reports_webdetailreport.ip='';
		store_reports_webdetailreport.dev='';
		store_reports_webdetailreport.os='';
		store_reports_webdetailreport.areaname='';
		store_reports_webdetailreport.keyword='';
		store_reports_webdetailreport.caExport='';
		store_reports_webdetailreport.new_params ={slogdate:store_reports_webdetailreport.slogdate,elogdate:store_reports_webdetailreport.elogdate,slogtime:store_reports_webdetailreport.slogtime,elogtime:store_reports_webdetailreport.elogtime,contrycode:store_reports_webdetailreport.contrycode,username:store_reports_webdetailreport.username,mac:store_reports_webdetailreport.mac,ip:store_reports_webdetailreport.ip,areaname:store_reports_webdetailreport.areaname,dev:store_reports_webdetailreport.dev,os:store_reports_webdetailreport.os,keyword:store_reports_webdetailreport.keyword,caExport:store_reports_webdetailreport.caExport};
		}
	
 function reflash(){
	store_reports_webdetailreport.new_params ={slogdate:store_reports_webdetailreport.slogdate,elogdate:store_reports_webdetailreport.elogdate,slogtime:store_reports_webdetailreport.slogtime,elogtime:store_reports_webdetailreport.elogtime,contrycode:store_reports_webdetailreport.contrycode,username:store_reports_webdetailreport.username,mac:store_reports_webdetailreport.mac,ip:store_reports_webdetailreport.ip,areaname:store_reports_webdetailreport.areaname,dev:store_reports_webdetailreport.dev,os:store_reports_webdetailreport.os,keyword:store_reports_webdetailreport.keyword,caExport:store_reports_webdetailreport.caExport};
	store_reports_webdetailreport.currentPage=1;
	store_reports_webdetailreport.load();
 
}
//高级查询窗口
		var win3 = null;
		function showCxForm() {
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'上网详情高级查询窗口',
					    closeAction: 'hide',
				      //plain:false,  
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
	            	id:'webdetailreport_form2', 
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[{
	                        	fieldLabel:'用户账号',
	                        	xtype: 'textfield',
								emptyText:'请输入用户账号',
								value:store_reports_webdetailreport.username,
	                        	name:'username'
	                    	},{
	                        	fieldLabel:'mac地址',
	                        	xtype: 'textfield',
								emptyText:'请输入MAC地址',
								value:store_reports_webdetailreport.mac,
	                        	name:'mac'
	                    	},{
	                        	fieldLabel:'IP',
	                        	xtype: 'textfield',
								emptyText:'请输入IP地址',
								value:store_reports_webdetailreport.ip,
	                        	name:'ip'
	                    	},{
	                        	fieldLabel:'终端品牌',
	                        	xtype: 'textfield',
								emptyText:'请输入终端类型',
								value:store_reports_webdetailreport.dev,
	                        	name:'dev'
	                    	},{
	                        	fieldLabel:'操作系统',
	                        	xtype: 'textfield',
								emptyText:'请输入操作类型',
								value:store_reports_webdetailreport.os,
	                        	name:'os'
	                    	}]  
	                })],  
					        buttons:[{
										text:"确定",
										handler:function(){
						        			var objectTmp = Ext.getCmp("webdetailreport_form2").getForm().getValues();
						        			store_reports_webdetailreport.username = objectTmp.username;
						        			store_reports_webdetailreport.mac = objectTmp.mac;
						        			store_reports_webdetailreport.ip = objectTmp.ip;
						        			store_reports_webdetailreport.dev = objectTmp.dev;
						        			store_reports_webdetailreport.os = objectTmp.os;
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

    Ext.apply(this,       
        {   
		//height:Ext.get("layout_center").getHeight(),
        border:false,
        frame:false,
		autoScroll: true,
        store: store_reports_webdetailreport,
        viewConfig:{
	        	loadMask : false
	        },
          columns:[
		{
            text: "用户账号",
            dataIndex: 'username',
            width: 120,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "MAC地址",
            dataIndex: 'mac',
            width: 120,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "IP",
            dataIndex: 'ip',
            width: 120,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "终端品牌",
            dataIndex: 'dev',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "操作系统",
            dataIndex: 'os',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "登录时间",
            dataIndex: 'starttime',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },{
        	  text:'退机时间',
        	  dataIndex:'endtime',
        	  align: 'right',
        	  width:150,
        	  hidden:false,
        	  sortable:false
        },{
            text: "上网流量(Mb)",
            dataIndex: 'traffic',
            width: 100,
            align: 'right',
            hidden: false,
            sortable: false
        },{
        	 flex: 1	
        }
          ],
          
           columnLines: true,
           tbar:{items:[
            '-',{
            	fieldLabel:'开始时间',
              	xtype: 'datefield',
              	format: "Y/m/d",
              	value:store_reports_webdetailreport.slogdate,
              	id:'webdetailreport_slogdated',
              	name:'webdetailreport_slogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_webdetailreport.slogtime,
              	id:'webdetailreport_slogtimed',
              	name:'webdetailreport_slogtimed',
              	width:100,
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
            }, '-',{
            	fieldLabel:'结束时间',
              	xtype: 'datefield',
              	format: "Y/m/d",
              	value:store_reports_webdetailreport.elogdate,
              	id:'webdetailreport_elogdated',
              	name:'webdetailreport_elogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_webdetailreport.elogtime,
              	id:'webdetailreport_elogtimed',
              	name:'webdetailreport_elogtimed',
              	width:100,
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
            }, '-',{
                text:'查询',
                //itemId: 'moveButton',
            	iconCls:'accept',
                //disabled: true,
                handler:function(){
					
                	store_reports_webdetailreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('webdetailreport_slogdated').value);
		            store_reports_webdetailreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('webdetailreport_elogdated').value);
		            store_reports_webdetailreport.slogtime=Ext.util.Format.substr(Ext.getCmp('webdetailreport_slogtimed').value, 16, 8);
		            store_reports_webdetailreport.elogtime=Ext.util.Format.substr(Ext.getCmp('webdetailreport_elogtimed').value, 16, 8);
		            store_reports_webdetailreport.currentPage=1;         
		            reflash();  
                }
            },'-',{
                text:'高级',
                anchor: 'right',
                handler: showCxForm, //高级查询函数
                iconCls:'option'
            },'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		store_reports_webdetailreport.caExport='exp';
						document.getElementById("form1").FunName.value ="ncm_web_App_intDetailReport";
						document.getElementById("form1").caExport.value =store_reports_webdetailreport.caExport;
						document.getElementById("form1").slogdate.value =store_reports_webdetailreport.slogdate;
						document.getElementById("form1").elogdate.value =store_reports_webdetailreport.elogdate;
						document.getElementById("form1").slogtime.value =store_reports_webdetailreport.slogtime;
						document.getElementById("form1").elogtime.value =store_reports_webdetailreport.elogtime;
                		document.getElementById("form1").submit();
               			store_reports_webdetailreport.caExport="";
	              	// 	reflash();
                }
            }
          ]
         }
        }
        
        );
	store_reports_webdetailreport.alreadyload = 1;
    this.callParent(arguments);
    }
    
})