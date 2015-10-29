﻿/*设备品牌饼图*/
	Ext.define('charts.deviceBrand',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		if(store_charts_devicebrand.alreadyload !=1){
		store_charts_devicebrand.type='1';
		store_charts_devicebrand.sdate=startday;  
		store_charts_devicebrand.edate=today;
		store_charts_devicebrand.chart_name='';
		store_charts_devicebrand.chart_number='';
		store_charts_devicebrand.inum='';
		store_charts_devicebrand.id='';
		store_charts_devicebrand.rate='';
		store_charts_devicebrand.exppdf='';
		store_charts_devicebrand.expexcel='';
		store_charts_devicebrand.new_params ={sdate:store_charts_devicebrand.sdate,edate:store_charts_devicebrand.edate,chart_name:store_charts_devicebrand.chart_name,chart_number:store_charts_devicebrand.chart_number,inum:store_charts_devicebrand.inum,id:store_charts_devicebrand.id,rate:store_charts_devicebrand.rate,exppdf:store_charts_devicebrand.exppdf,expexcel:store_charts_devicebrand.expexcel,type:store_charts_devicebrand.type};
		}
	function reflash(){
		store_charts_devicebrand.new_params ={sdate:store_charts_devicebrand.sdate,edate:store_charts_devicebrand.edate,chart_name:store_charts_devicebrand.chart_name,chart_number:store_charts_devicebrand.chart_number,inum:store_charts_devicebrand.inum,id:store_charts_devicebrand.id,rate:store_charts_devicebrand.rate,exppdf:store_charts_devicebrand.exppdf,expexcel:store_charts_devicebrand.expexcel,type:store_charts_devicebrand.type};
		store_charts_devicebrand.currentPage=1;
		store_charts_devicebrand.load();
	}


	//详细信息
	function showDetail(value, cellmeta, record, rowIndex, colIndex, store){
	//alert(store.get('nets_subytes2'));
		return Ext.String.format('<div onclick="eval(charts.deviceBrand).openWin(&quot;{0}&quot;,&quot;'+store_charts_devicebrand.type+'&quot;);" style="cursor:pointer;">{1}</div>',value,value);	
	}
	
	//加载终端连接类型
	  var timeFlagStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'PORTAL', groupid:'1'},
	  				{groupname:'广告', groupid:'2'}
	  		]
		});
    var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
		id:'deviceBrand_timeFlagCombo',
        width: 110,
        valueField:'groupid',
        emptyText:'请选择终端连接类型',
		value:store_charts_devicebrand.type,
        displayField: 'groupname',
        name : 'time_flag',      
        allowBlank: true,
        store: timeFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
	            store_charts_devicebrand.type=this.value;
				reflash();
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		}); 
	
	var chart = Ext.create('Ext.chart.Chart', {
            xtype: 'chart',
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
                showInLegend: true,             //标注
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
		 columns:[
		 {
            text: "序号",
            dataIndex: 'inum',
            hidden: false,
            width: 50,
            sortable: true
        },
		{
            text: "终端品牌",
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
					},'-',timeFlagCombo,'-',
						{
						     text:"查询",
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
								document.getElementById('form1').type.value=store_charts_devicebrand.type; 
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
    },
    inheritableStatics : {  
	openWin:function(value,type){
	
	if(type=='1'){
	var htmlvar = '<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_chart/ncm_web_Place_userlog_list.html&dev@'+value+'&sdate@'+store_charts_devicebrand.sdate+'&edate@'+store_charts_devicebrand.edate+'" scrolling="yes" name="drIframe1" frameborder="no" style="width:100%;height:100%;"></iframe>';

		var	windr = new Ext.Window ({
									title:"终端品牌PORTAL详情窗口 - "+value,
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
									//modal:true,
									resizable:true,
								  constrain:false,
								  draggable:true,
	                html:htmlvar
		});
	//windr.show();
	}
	if(type=='2'){
	//alert(value);
	var htmlvar = '<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncm_advertisement/ncm_web_Place_advertisment_chart.htm&devtype@'+value+'&sdate@'+store_charts_devicebrand.sdate+'&edate@'+store_charts_devicebrand.edate+'" scrolling="yes" name="drIframe2" frameborder="no" style="width:100%;height:100%;"></iframe>';
	var	win2 = new Ext.Window ({
                             title:"终端品牌广告详情窗口 - "+value,
                             x:0,
                             y:0,
                            width:window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth,
							height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
					//		 layout:'fit',  
					//		 closeAction: 'close',
                             bodyPadding: 10,
                    //         modal:true,
                             resizable:true,
							 constrain:false,
                             html:htmlvar
	});
	//win2.show();
	}
	
	
	//win2.show();
	}
	}
})
