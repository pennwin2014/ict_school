/*设备操作类型饼图*/
	Ext.define('charts.deviceOS',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
	if(store_charts_deviceos.alreadyload !=1){
		store_charts_deviceos.sdate=startday;  
		store_charts_deviceos.edate=today;
		store_charts_deviceos.chart_name='';
		store_charts_deviceos.chart_number='';
		store_charts_deviceos.inum='';
		store_charts_deviceos.id='';
		store_charts_deviceos.rate='';
		store_charts_deviceos.exppdf='';
		store_charts_deviceos.expexcel='';
		store_charts_deviceos.new_params ={sdate:store_charts_deviceos.sdate,edate:store_charts_deviceos.edate,chart_name:store_charts_deviceos.chart_name,chart_number:store_charts_deviceos.chart_number,inum:store_charts_deviceos.inum,id:store_charts_deviceos.id,rate:store_charts_deviceos.rate,exppdf:store_charts_deviceos.exppdf,expexcel:store_charts_deviceos.expexcel};
	}	
	function reflash(){
		store_charts_deviceos.new_params ={sdate:store_charts_deviceos.sdate,edate:store_charts_deviceos.edate,chart_name:store_charts_deviceos.chart_name,chart_number:store_charts_deviceos.chart_number,inum:store_charts_deviceos.inum,id:store_charts_deviceos.id,rate:store_charts_deviceos.rate,exppdf:store_charts_deviceos.exppdf,expexcel:store_charts_deviceos.expexcel};
		store_charts_deviceos.currentPage=1;
		store_charts_deviceos.load();
   
	}



//详细信息
function showDetail(value, cellmeta, record, rowIndex, colIndex, store){
		//alert(store.name);
		return Ext.String.format('<div onclick="eval(charts.deviceOS).openWin(&quot;{0}&quot;,&quot;store_charts_deviceos&quot;);" style="cursor:pointer;">{1}</div>',value, value);	
}

	
	var chart = Ext.create('Ext.chart.Chart', {
            xtype: 'chart',
			width:Ext.get("layout_center").getWidth()-410,
            theme: 'Base:gradients',
			animate: true,
            store: store_charts_deviceos,
            shadow: true,
            legend: {
                position: 'left'
            },
			insetPadding: 20,
            series: [{
                type: 'pie',
                field: 'chart_number',
                showInLegend: true,             //右侧标注
				label:{
				field:'chart_name',
				display: 'rotate',
                renderer: Ext.util.Format.numberRenderer('0'), 
                contrast: true,
                font: '12px Arial'
				},
                tips: {
                  trackMouse: true,
                  width: 140,
                  height: 25,
                  renderer: function(storeItem, item) {
                    //calculate percentage.
                    var total = 0;
                    store_charts_deviceos.each(function(rec) {
                        total += rec.get('chart_number');
                    });
                    this.setTitle(storeItem.get('chart_name') + ': ' + (storeItem.get('chart_number') / total * 100).toFixed(2) + '%');
                  }
                },
                highlight: {
                  segment: {
                    margin: 20
                  }
                }
            }]
        });
    Ext.apply(this,       
    {   
		//height:Ext.get("layout_center").getHeight(),
		height:400,
        border:false,
        frame:false,
		autoScroll: true,
		store: store_charts_deviceos,
		viewConfig:{
	        loadMask : false
	    },
		 columns:[
		{
            text: "序号",
            dataIndex: 'inum',
            hidden: false,
            //align: 'center',
            width: 50,
            sortable: true
        },{
            text: "操作系统",
            dataIndex:'chart_name',
            hidden: false,
            //align: 'center', 
            width: 100,
            sortable: false,
			renderer: showDetail
        },{
            text: "数量（个）",
            dataIndex: 'chart_number',
            hidden: false,
            align: 'right',           
            width: 100,
            sortable: true
        },{
            text: "百分比",
            dataIndex: 'rate',
            hidden: false,
            align: 'right',
            width: 100,
            sortable:true
        }
		],
		 columnLines: true,
        tbar:[
				'-',{                        
                        fieldLabel:'从',labelWidth: 20,  width:130, 
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:store_charts_deviceos.sdate,
                        id:'deviceos_sdate1',
                        name:'deviceos_sdate1'
					},'-',
			        	  {
                        fieldLabel:'到',labelWidth: 20,  width:130,        
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:store_charts_deviceos.edate,
                        id:'deviceos_edate1',
                        name:'deviceos_edate1'
					},'-',
						{
						     text:"查询",
						     iconCls:'accept',
						     handler: function(){
			                 var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('deviceos_sdate1').value);
		                	 var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('deviceos_edate1').value);
			                 store_charts_deviceos.sdate=aa;  
			                 store_charts_deviceos.edate=bb;
			                 reflash();
		              
						}
						},'-',
						    {
						      	text:"导出",
						      	iconCls:'exp',
						      	handler: function(){
													var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('deviceos_sdate1').value);
													var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('deviceos_edate1').value);
													store_charts_deviceos.sdate=aa;  
													store_charts_deviceos.edate=bb;
													document.getElementById("form1").FunName.value ="ncm_web_Place_os_chart";
													document.getElementById('form1').sdate.value=aa;  
													document.getElementById('form1').edate.value=bb;
													document.getElementById('form1').start.value=0;
													document.getElementById('form1').limit.value=10;
													document.getElementById('form1').expexcel.value='1';
													document.getElementById('form1').submit();
													document.getElementById('form1').expexcel.value="";
										}
						    }
						],
			lbar:chart
        });
	store_charts_deviceos.alreadyload = 1;
    this.callParent(arguments);
    },
	inheritableStatics : {  
	openWin:function(value,tabframe){
	
		var htmlvar = '<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_chart/ncm_web_Place_userlog_list.html&os@'+value+'&sdate@'+store_charts_deviceos.sdate+'&edate@'+store_charts_deviceos.edate+' " scrolling="yes" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';
		var	windr = new Ext.Window ({
									id:"windr",
									title:"操作系统详情窗口",
									x:0,
									y:0,
									width:window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth,
									height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
									//maximizable: true,  
									//minimizable: true , 
									//width:1124,
									//height:600,
									bodyPadding: 0,
									//autoScroll:true,
									modal:true,
									resizable:true,
								  constrain:false,
								  draggable:true,
	                html:htmlvar
		});
		windr.on("beforeclose", function() {
	  	//doreflashTab();
	  })
		//windr.show();
	}
}    
})