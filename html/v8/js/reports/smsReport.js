		
	Ext.define('reports.smsReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_reports_smsreport.alreadyload !=1){
		store_reports_smsreport.slogdate = today;
		store_reports_smsreport.elogdate = today;
		store_reports_smsreport.slogtime = '00:00:00';
		store_reports_smsreport.elogtime = '23:59:59';
		store_reports_smsreport.timeV='';
		store_reports_smsreport.localNum='';
		store_reports_smsreport.interNum='';
		store_reports_smsreport.keyword='';
		store_reports_smsreport.caExport='';
		store_reports_smsreport.new_params ={slogdate:store_reports_smsreport.slogdate,elogdate:store_reports_smsreport.elogdate,slogtime:store_reports_smsreport.slogtime,elogtime:store_reports_smsreport.elogtime,timeV:store_reports_smsreport.timeV,localNum:store_reports_smsreport.localNum,interNum:store_reports_smsreport.interNum,keyword:store_reports_smsreport.keyword,caExport:store_reports_smsreport.caExport};
		}

 function reflash(){
	store_reports_smsreport.new_params ={slogdate:store_reports_smsreport.slogdate,elogdate:store_reports_smsreport.elogdate,slogtime:store_reports_smsreport.slogtime,elogtime:store_reports_smsreport.elogtime,timeV:store_reports_smsreport.timeV,localNum:store_reports_smsreport.localNum,interNum:store_reports_smsreport.interNum,keyword:store_reports_smsreport.keyword,caExport:store_reports_smsreport.caExport};
	store_reports_smsreport.currentPage=1;
	store_reports_smsreport.load();
   
}

    Ext.apply(this,       
        {   
		//height:Ext.get("layout_center").getHeight(),
        border:false,
        frame:false,
		autoScroll: true,
        store: store_reports_smsreport,
		submit: function(){ 
		this.getEl().dom.action = '/pronline/Msg?FunName@ncm_web_App_smsStatisticsReport', 
		this.getEl().dom.method='POST',
		this.getEl().dom.submit(); 
	   }, 
        viewConfig:{
	        	loadMask : false
	        },
          columns:[
		  {
            text: "时间",
            dataIndex: 'timeV',
            width: 150,
            align: 'center',
            hidden: false,
            sortable: false
        },{
            text: "国内短信数量",
            dataIndex: 'localNum',
            width: 150,
            align: 'right',
            hidden: false,
            sortable: false
        },{
            text: "国际短信数量",
            dataIndex: 'interNum',
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
              	value:store_reports_smsreport.slogdate,
              	id:'smsreport_slogdated',
              	name:'smsreport_slogdated',
              	labelWidth:55, //标签宽度
				        width:170 //字段宽度 
            },{
            	  xtype:'timefield',
              	value:store_reports_smsreport.slogtime,
              	id:'smsreport_slogtimed',
              	name:'smsreport_slogtimed',
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
              	value:store_reports_smsreport.elogdate,
              	id:'smsreport_elogdated',
              	name:'smsreport_elogdated',
              	labelWidth:55, //标签宽度
				width:170 //字段宽度 
            },{
            	xtype:'timefield',
              	value:store_reports_smsreport.elogtime,
              	id:'smsreport_elogtimed',
              	name:'smsreport_elogtimed',
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
					
                	store_reports_smsreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsreport_slogdated').value);
		            store_reports_smsreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsreport_elogdated').value);
		            store_reports_smsreport.slogtime=Ext.util.Format.substr(Ext.getCmp('smsreport_slogtimed').value, 16, 8);
		            store_reports_smsreport.elogtime=Ext.util.Format.substr(Ext.getCmp('smsreport_elogtimed').value, 16, 8);
		            store_reports_smsreport.currentPage=1;         
		            reflash();  
                }
            },'-',{
                text:'导出',
                iconCls:'exp',
                handler:function(){
                		store_reports_smsreport.caExport='exp';
						document.getElementById("form1").FunName.value ="ncm_web_App_smsStatisticsReport";
						document.getElementById("form1").caExport.value =store_reports_smsreport.caExport;
						document.getElementById("form1").slogdate.value =store_reports_smsreport.slogdate;
						document.getElementById("form1").elogdate.value =store_reports_smsreport.elogdate;
						document.getElementById("form1").slogtime.value =store_reports_smsreport.slogtime;
						document.getElementById("form1").elogtime.value =store_reports_smsreport.elogtime;
                		document.getElementById("form1").submit();
               			store_reports_smsreport.caExport="";
	              	// 	reflash();
                }
            }
          ]
         }
        }
        
        );
	store_reports_smsreport.alreadyload = 1;
    this.callParent(arguments);
    }
})