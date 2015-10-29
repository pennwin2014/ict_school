﻿
		
	Ext.define('reports.deviceReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_reports_devicereport.alreadyload !=1){
		store_reports_devicereport.slogdate = today;
		store_reports_devicereport.elogdate = today;
		store_reports_devicereport.slogtime = '00:00:00';
		store_reports_devicereport.elogtime = '23:59:59';
		store_reports_devicereport.devtype='';
		store_reports_devicereport.dev='';
		store_reports_devicereport.num='';
		store_reports_devicereport.keyword='';
		store_reports_devicereport.caExport='';
		store_reports_devicereport.new_params ={slogdate:store_reports_devicereport.slogdate,elogdate:store_reports_devicereport.elogdate,slogtime:store_reports_devicereport.slogtime,elogtime:store_reports_devicereport.elogtime,devtype:store_reports_devicereport.devtype,dev:store_reports_devicereport.dev,num:store_reports_devicereport.num,keyword:store_reports_devicereport.keyword,caExport:store_reports_devicereport.caExport};
		}
 function reflash(){
	store_reports_devicereport.new_params ={slogdate:store_reports_devicereport.slogdate,elogdate:store_reports_devicereport.elogdate,slogtime:store_reports_devicereport.slogtime,elogtime:store_reports_devicereport.elogtime,devtype:store_reports_devicereport.devtype,dev:store_reports_devicereport.dev,num:store_reports_devicereport.num,keyword:store_reports_devicereport.keyword,caExport:store_reports_devicereport.caExport};
	store_reports_devicereport.currentPage=1;
	store_reports_devicereport.load();
 
}

    Ext.apply(this,       
        {   
		//height:Ext.get("layout_center").getHeight(),
        border:false,
        frame:false,
		autoScroll: true,
        store: store_reports_devicereport,
        viewConfig:{
	        	loadMask : false
	        },
          columns:[
		{
            text: "设备类型",
            dataIndex: 'devtype',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "设备名称",
            dataIndex: 'dev',
            width: 150,
            //align: 'center',
            hidden: false,
            sortable: true
        },{
            text: "数量",
            dataIndex: 'num',
            width: 150,
            //align: 'right',
            hidden: false,
            sortable: true
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
              	value:store_reports_devicereport.slogdate,
              	id:'devicereport_slogdated',
              	name:'devicereport_slogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_devicereport.slogtime,
              	id:'devicereport_slogtimed',
              	name:'devicereport_slogtimed',
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
              	value:store_reports_devicereport.elogdate,
              	id:'devicereport_elogdated',
              	name:'devicereport_elogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_devicereport.elogtime,
              	id:'devicereport_elogtimed',
              	name:'devicereport_elogtimed',
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
					
                	store_reports_devicereport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicereport_slogdated').value);
		            store_reports_devicereport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicereport_elogdated').value);
		            store_reports_devicereport.slogtime=Ext.util.Format.substr(Ext.getCmp('devicereport_slogtimed').value, 16, 8);
		            store_reports_devicereport.elogtime=Ext.util.Format.substr(Ext.getCmp('devicereport_elogtimed').value, 16, 8);
		            store_reports_devicereport.currentPage=1;         
		            reflash();  
                }
            },'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		store_reports_devicereport.caExport='exp';
						document.getElementById("form1").FunName.value ="ncm_web_App_intDeviceReport";
						document.getElementById("form1").caExport.value =store_reports_devicereport.caExport;
						document.getElementById("form1").slogdate.value =store_reports_devicereport.slogdate;
						document.getElementById("form1").elogdate.value =store_reports_devicereport.elogdate;
						document.getElementById("form1").slogtime.value =store_reports_devicereport.slogtime;
						document.getElementById("form1").elogtime.value =store_reports_devicereport.elogtime;
                		document.getElementById("form1").submit();
               			store_reports_devicereport.caExport="";
	              	// 	reflash();
                }
            }
          ]
         }
        }
        
        );
	store_reports_devicereport.alreadyload = 1;
    this.callParent(arguments);
    }
    
})