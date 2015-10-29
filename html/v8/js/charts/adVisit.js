/*设备操作类型饼图*/
	Ext.define('charts.adVisit',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
	if(store_charts_advisit.alreadyload !=1){
		store_charts_advisit.sdate=startday;  
		store_charts_advisit.edate=today;
		store_charts_advisit.chart_name='';
		store_charts_advisit.chart_number='';
		store_charts_advisit.inum='';
		store_charts_advisit.id='';
		store_charts_advisit.rate='';
		store_charts_advisit.exppdf='';
		store_charts_advisit.expexcel='';
		store_charts_advisit.username='';
		store_charts_advisit.new_params ={sdate:store_charts_advisit.sdate,edate:store_charts_advisit.edate,chart_name:store_charts_advisit.chart_name,chart_number:store_charts_advisit.chart_number,inum:store_charts_advisit.inum,id:store_charts_advisit.id,rate:store_charts_advisit.rate,exppdf:store_charts_advisit.exppdf,expexcel:store_charts_advisit.expexcel,username:store_charts_advisit.username};
	}	
	function reflash(){
		store_charts_advisit.new_params ={sdate:store_charts_advisit.sdate,edate:store_charts_advisit.edate,chart_name:store_charts_advisit.chart_name,chart_number:store_charts_advisit.chart_number,inum:store_charts_advisit.inum,id:store_charts_advisit.id,rate:store_charts_advisit.rate,exppdf:store_charts_advisit.exppdf,expexcel:store_charts_advisit.expexcel,username:store_charts_advisit.username};
		store_charts_advisit.currentPage=1;
		store_charts_advisit.load();
   
	}



//详细信息
function showDetail(value, cellmeta, record, rowIndex, colIndex, store){
		//alert(store.name);
		return Ext.String.format('<div onclick="eval(charts.adVisit).openWin(&quot;{0}&quot;,&quot;store_charts_advisit&quot;);" style="cursor:pointer;">{1}</div>',value, value);	
}

	
	var chart = Ext.create('Ext.chart.Chart', {
            xtype: 'chart',
			width:Ext.get("layout_center").getWidth()-410,
            theme: 'Base:gradients',
			animate: true,
            store: store_charts_advisit,
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
                  width: 110,
                  height: 50,
                  renderer: function(storeItem, item) {
                    //calculate percentage.
                    var total = 0;
                    store_charts_advisit.each(function(rec) {
                        total += rec.get('chart_number');
                    });
                   this.setTitle('广告 '+storeItem.get('chart_name') + '<br>点击 ' +storeItem.get('chart_number')+' 次<br>占 '+ (storeItem.get('chart_number') / total * 100).toFixed(2) + '%');
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
		store: store_charts_advisit,
		viewConfig:{
	        loadMask : false
	    },
		 columns:[
		{
            id: 'num',
            text: "序号",
            dataIndex: 'inum',
            hidden: false,
            width: 50,
            sortable: true
        },{
            text: "广告",
            dataIndex: 'chart_name', 
            width: 100,
			renderer: showDetail
        },{
            text: "点击次数（次）",
            dataIndex: 'chart_number',           
            width: 100,
			align: 'right',
            sortable: true
         
        },
        {
            text: "百分比",
            dataIndex: 'rate',
            hidden: false,
            width: 100,
			align: 'right',
            sortable: true
        }
		],
		 columnLines: true,
        tbar:[
				'-',{                        
                        fieldLabel:'从',labelWidth: 20,  width:130, 
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:store_charts_advisit.sdate,
                        id:'advisit_sdate1',
                        name:'advisit_sdate1'
					},'-',
			        	  {
                        fieldLabel:'到',labelWidth: 20,  width:130,        
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:store_charts_advisit.edate,
                        id:'advisit_edate1',
                        name:'advisit_edate1'
					},'-',{
                        //fieldLabel:'用户账号',
						emptyText:'用户账号',
						width:110,      
                        xtype: 'textfield',
                        id:'advisit_username',
						value:'',
                        name:'advisit_username'
					},'-',
						{
						     text:"查询",
						     iconCls:'accept',
						     handler: function(){
			                 var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('advisit_sdate1').value);
		                	 var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('advisit_edate1').value);
			                 store_charts_advisit.sdate=aa;  
			                 store_charts_advisit.edate=bb;
							 store_charts_advisit.username=Ext.getCmp('advisit_username').value;
			                 reflash();
		              
						}
						},'-',
						    {
						      	text:"导出",
						      	iconCls:'exp',
						      	handler: function(){
								var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('advisit_sdate1').value);
								var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('advisit_edate1').value);
								store_charts_advisit.sdate=aa;  
								store_charts_advisit.edate=bb;
								document.getElementById("form1").FunName.value ="ncm_Ad_AdVisit";
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
	store_charts_advisit.alreadyload = 1;
    this.callParent(arguments);
    },
	inheritableStatics : {  
	openWin:function(value,tabframe){
		var htmlvar = '<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_advertisement/ncm_web_Place_advertisment_chart.htm&adcod@'+value+'&sdate@'+store_charts_advisit.sdate+'&edate@'+store_charts_advisit.edate+' " scrolling="yes" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';
		var	windr = new Ext.Window ({
									title:"广告点击详情窗口 - "+value,
									x:0,
									y:0,
									width:window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth,
									height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
									//maximizable: true,  
									//minimizable: true , 
									//width:1124,
									//height:600,
									bodyPadding: 0,
									autoScroll:true,
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