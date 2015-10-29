
		
	Ext.define('reports.smsDetailReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_reports_smsdetailreport.alreadyload !=1){
		store_reports_smsdetailreport.slogdate = today;
		store_reports_smsdetailreport.elogdate = today;
		store_reports_smsdetailreport.slogtime = '00:00:00';
		store_reports_smsdetailreport.elogtime = '23:59:59';
		store_reports_smsdetailreport.contrycode='';
		store_reports_smsdetailreport.username='';
		store_reports_smsdetailreport.usermac='';
		store_reports_smsdetailreport.ip='';
		store_reports_smsdetailreport.logtime='';
		store_reports_smsdetailreport.sptype='';
		store_reports_smsdetailreport.areaname='';
		store_reports_smsdetailreport.keyword='';
		store_reports_smsdetailreport.caExport='';
		store_reports_smsdetailreport.new_params ={slogdate:store_reports_smsdetailreport.slogdate,elogdate:store_reports_smsdetailreport.elogdate,slogtime:store_reports_smsdetailreport.slogtime,elogtime:store_reports_smsdetailreport.elogtime,contrycode:store_reports_smsdetailreport.contrycode,username:store_reports_smsdetailreport.username,usermac:store_reports_smsdetailreport.usermac,ip:store_reports_smsdetailreport.ip,areaname:store_reports_smsdetailreport.areaname,logtime:store_reports_smsdetailreport.logtime,sptype:store_reports_smsdetailreport.sptype,keyword:store_reports_smsdetailreport.keyword,caExport:store_reports_smsdetailreport.caExport};
		}

 function reflash(){
	store_reports_smsdetailreport.new_params ={slogdate:store_reports_smsdetailreport.slogdate,elogdate:store_reports_smsdetailreport.elogdate,slogtime:store_reports_smsdetailreport.slogtime,elogtime:store_reports_smsdetailreport.elogtime,contrycode:store_reports_smsdetailreport.contrycode,username:store_reports_smsdetailreport.username,usermac:store_reports_smsdetailreport.usermac,ip:store_reports_smsdetailreport.ip,areaname:store_reports_smsdetailreport.areaname,logtime:store_reports_smsdetailreport.logtime,sptype:store_reports_smsdetailreport.sptype,keyword:store_reports_smsdetailreport.keyword,caExport:store_reports_smsdetailreport.caExport};
	store_reports_smsdetailreport.currentPage=1;
	store_reports_smsdetailreport.load();
 
}

    Ext.apply(this,       
        {   
		//height:Ext.get("layout_center").getHeight(),
        border:false,
        frame:false,
		autoScroll: true,
        store: store_reports_smsdetailreport,
        viewConfig:{
	        	loadMask : false
	        },
          columns:[
		  {
            text: "国家区号",
            dataIndex: 'contrycode',
            width: 80,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "用户账号",
            dataIndex: 'username',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },
//        {
//            text: "AP位置",
//            dataIndex: 'apmac',
//            width: 120,
//            //align: 'center',
//            hidden: false,
//            sortable: false
//        },
        {
            text: "MAC地址",
            dataIndex: 'usermac',
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
            text: "发送时间",
            dataIndex: 'logtime',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "运营商类别",
            dataIndex: 'sptype',
            width: 100,
            //align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "区域名称",
            dataIndex: 'areaname',
            width: 120,
            //align: 'center',
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
              	value:store_reports_smsdetailreport.slogdate,
              	id:'smsdetailreport_slogdated',
              	name:'smsdetailreport_slogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_smsdetailreport.slogtime,
              	id:'smsdetailreport_slogtimed',
              	name:'smsdetailreport_slogtimed',
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
              	value:store_reports_smsdetailreport.elogdate,
              	id:'smsdetailreport_elogdated',
              	name:'smsdetailreport_elogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_smsdetailreport.elogtime,
              	id:'smsdetailreport_elogtimed',
              	name:'smsdetailreport_elogtimed',
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
					
                	store_reports_smsdetailreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsdetailreport_slogdated').value);
		            store_reports_smsdetailreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsdetailreport_elogdated').value);
		            store_reports_smsdetailreport.slogtime=Ext.util.Format.substr(Ext.getCmp('smsdetailreport_slogtimed').value, 16, 8);
		            store_reports_smsdetailreport.elogtime=Ext.util.Format.substr(Ext.getCmp('smsdetailreport_elogtimed').value, 16, 8);
		            store_reports_smsdetailreport.currentPage=1;         
		            reflash();  
                }
            },'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		store_reports_smsdetailreport.caExport='exp';
										document.getElementById("form1").FunName.value ="ncm_web_App_smsDetailReport";
										document.getElementById("form1").caExport.value =store_reports_smsdetailreport.caExport;
										document.getElementById("form1").slogdate.value =store_reports_smsdetailreport.slogdate;
										document.getElementById("form1").elogdate.value =store_reports_smsdetailreport.elogdate;
										document.getElementById("form1").slogtime.value =store_reports_smsdetailreport.slogtime;
										document.getElementById("form1").elogtime.value =store_reports_smsdetailreport.elogtime;
                		document.getElementById("form1").submit();
                		
               			store_reports_smsdetailreport.caExport="";
	              	// 	reflash();
                }
            }
          ]
         }
        }
        
        );
	store_reports_smsdetailreport.alreadyload = 1;
    this.callParent(arguments);
    }
    
})