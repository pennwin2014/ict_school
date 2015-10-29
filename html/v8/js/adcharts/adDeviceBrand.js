/*广告终端品牌饼图*/
	Ext.define('charts.deviceBrand',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_charts_devicebrand.alreadyload !=1){
		store_charts_devicebrand.sdate=startday;  
		store_charts_devicebrand.edate=today;
		store_charts_devicebrand.chart_name='';
		store_charts_devicebrand.chart_number='';
		store_charts_devicebrand.inum='';
		store_charts_devicebrand.id='';
		store_charts_devicebrand.rate='';
		store_charts_devicebrand.exppdf='';
		store_charts_devicebrand.expexcel='';
		store_charts_devicebrand.new_params ={sdate:store_charts_devicebrand.sdate,edate:store_charts_devicebrand.edate,chart_name:store_charts_devicebrand.chart_name,chart_number:store_charts_devicebrand.chart_number,inum:store_charts_devicebrand.inum,id:store_charts_devicebrand.id,rate:store_charts_devicebrand.rate,exppdf:store_charts_devicebrand.exppdf,expexcel:store_charts_devicebrand.expexcel};
		}
	function reflash(){
		store_charts_devicebrand.new_params ={sdate:store_charts_devicebrand.sdate,edate:store_charts_devicebrand.edate,chart_name:store_charts_devicebrand.chart_name,chart_number:store_charts_devicebrand.chart_number,inum:store_charts_devicebrand.inum,id:store_charts_devicebrand.id,rate:store_charts_devicebrand.rate,exppdf:store_charts_devicebrand.exppdf,expexcel:store_charts_devicebrand.expexcel};
		store_charts_devicebrand.currentPage=1;
		store_charts_devicebrand.load();
   
	}

	function openWin(value){
		parent.dhsrc("/pronline/Msg?FunName@ncm_web_MC_userlog_list&flag@"+value+"&sdate@"+document.getElementById("form1").sdate.value+"&edate@"+document.getElementById("form1").edate.value+"&dev@dev","UserLog","Userlogid");
		var	win2 = new Ext.Window ({
							id:"addwindow",
                             title:"Device Type Detail",
                             x:0,
                             y:0,
                             width:1000,
                             height:600,
					//		 layout:'fit',  
					//		 closeAction: 'close',
                             bodyPadding: 10,
                    //         modal:true,
                             resizable:true,
							 constrain:false,
                             html:'<iframe  id="gdrzIframe_xnsf" name="gdrzIframe_xnsf" src="/ncm_userman/ncm_web_MC_userlog_list.html" scrolling="auto" width=100% height=1000  FrameBorder=0 ></iframe>'
	});
	//win2.show();
	
	
	}
	//详细信息
	function showDetail(value, cellmeta, record, rowIndex, colIndex, store){
		return Ext.String.format('<div onclick="openWin(&quot;{0}&quot;)" style="cursor:pointer;">{1}</div>',value,value);	
	}
	
	var chart = Ext.create('Ext.chart.Chart', {
            xtype: 'chart',
			id:'devicebrandchart',
			width:Ext.get("layout_center").getWidth()-410,
            theme: 'Base:gradients',
			animate: true,
            store: store_charts_devicebrand,
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
                  height: 28,
                  renderer: function(storeItem, item) {
                    //calculate percentage.
                    var total = 0;
                    store_charts_devicebrand.each(function(rec) {
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
		store: store_charts_devicebrand,
		viewConfig:{
	        loadMask : false
	    },
		 columns:[{
            id: 'num',
            text: "序号",
            dataIndex: 'inum',
            hidden: false,
            width: 50,
            sortable: true
        },{
            text: "设备品牌",
            dataIndex: 'chart_name', 
            width: 100,
			renderer: showDetail
        },{
            text: "数量（个）",
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
                        value:store_charts_devicebrand.sdate,
                        id:'devicebrand_sdate1',
                        name:'devicebrand_sdate1'
					},'-',
			        	  {
                        fieldLabel:'到',labelWidth: 20,  width:130,        
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:store_charts_devicebrand.edate,
                        id:'devicebrand_edate1',
                        name:'devicebrand_edate1'
					},'-',
						{
						     text:"确认",
						     iconCls:'accept',
						     handler: function(){
			                 var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicebrand_sdate1').value);
		                	 var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicebrand_edate1').value);
			                 store_charts_devicebrand.sdate=aa;  
			                 store_charts_devicebrand.edate=bb;
			                 reflash();
		              
						}
						},'-',
						    {
						      	text:"导出",
						      	iconCls:'exp',
						      	handler: function(){
								var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicebrand_sdate1').value);
								var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('devicebrand_edate1').value);
								store_charts_devicebrand.sdate=aa;  
								store_charts_devicebrand.edate=bb;
								document.getElementById("form1").FunName.value ="ncm_DeviceBrandChart";
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
    store_charts_devicebrand.alreadyload = 1;
    this.callParent(arguments);
    }
    
})