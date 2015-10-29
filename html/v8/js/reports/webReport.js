
		
	Ext.define('reports.webReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_reports_webreport.alreadyload !=1){
		store_reports_webreport.slogdate = today;
		store_reports_webreport.elogdate = today;
		store_reports_webreport.slogtime = '00:00:00';
		store_reports_webreport.elogtime = '23:59:59';
		store_reports_webreport.time='';
		store_reports_webreport.number='';
		store_reports_webreport.traffic='';
		store_reports_webreport.keyword='';
		store_reports_webreport.caExport='';
		store_reports_webreport.new_params ={slogdate:store_reports_webreport.slogdate,elogdate:store_reports_webreport.elogdate,slogtime:store_reports_webreport.slogtime,elogtime:store_reports_webreport.elogtime,time:store_reports_webreport.time,number:store_reports_webreport.number,traffic:store_reports_webreport.traffic,keyword:store_reports_webreport.keyword,caExport:store_reports_webreport.caExport};
		}
 
 function reflash(){
	store_reports_webreport.new_params ={slogdate:store_reports_webreport.slogdate,elogdate:store_reports_webreport.elogdate,slogtime:store_reports_webreport.slogtime,elogtime:store_reports_webreport.elogtime,time:store_reports_webreport.time,number:store_reports_webreport.number,traffic:store_reports_webreport.traffic,keyword:store_reports_webreport.keyword,caExport:store_reports_webreport.caExport};
	store_reports_webreport.currentPage=1;
	store_reports_webreport.load();
   
}
    Ext.apply(this,       
        {   
		//height:Ext.get("layout_center").getHeight(),
        border:false,
        frame:false,
		autoScroll: true,
        store: store_reports_webreport,
        viewConfig:{
	        	loadMask : false
	        },
          columns:[
		  {
            text: "时间段",
            dataIndex: 'time',
            width: 150,
            align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "数量",
            dataIndex: 'number',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "流量(Mb)",
            dataIndex: 'traffic',
            width: 150,
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
              	value:store_reports_webreport.slogdate,
              	id:'webreport_slogdated',
              	name:'webreport_slogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_webreport.slogtime,
              	id:'webreport_slogtimed',
              	name:'webreport_slogtimed',
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
              	value:store_reports_webreport.elogdate,
              	id:'webreport_elogdated',
              	name:'webreport_elogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_webreport.elogtime,
              	id:'webreport_elogtimed',
              	name:'webreport_elogtimed',
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
					
                	store_reports_webreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('webreport_slogdated').value);
		            store_reports_webreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('webreport_elogdated').value);
		            store_reports_webreport.slogtime=Ext.util.Format.substr(Ext.getCmp('webreport_slogtimed').value, 16, 8);
		            store_reports_webreport.elogtime=Ext.util.Format.substr(Ext.getCmp('webreport_elogtimed').value, 16, 8);
		            store_reports_webreport.currentPage=1;         
		            reflash();  
                }
            },'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		store_reports_webreport.caExport='exp';
						document.getElementById("form1").FunName.value ="ncm_web_App_intStatisticsReport";
						document.getElementById("form1").caExport.value =store_reports_webreport.caExport;
						document.getElementById("form1").slogdate.value =store_reports_webreport.slogdate;
						document.getElementById("form1").elogdate.value =store_reports_webreport.elogdate;
						document.getElementById("form1").slogtime.value =store_reports_webreport.slogtime;
						document.getElementById("form1").elogtime.value =store_reports_webreport.elogtime;
                		document.getElementById("form1").submit();
               			store_reports_webreport.caExport="";
	              	// 	reflash();
                }
            }
          ]
         }
        }
        
        );
	store_reports_webreport.alreadyload = 1;
    this.callParent(arguments);
    }
})