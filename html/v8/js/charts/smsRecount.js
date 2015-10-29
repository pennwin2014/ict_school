/*短信数量线图*/
Ext.define('charts.smsRecount',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
		store_charts_smsrecount1.sdate=startday;  
		store_charts_smsrecount1.edate=today;
		store_charts_smsrecount1.chart_name='';
		store_charts_smsrecount1.chart_number='';
		store_charts_smsrecount1.inum='';
		store_charts_smsrecount1.id='';
		store_charts_smsrecount1.rate='';
		store_charts_smsrecount1.exppdf='';
		store_charts_smsrecount1.expexcel='';
		store_charts_smsrecount1.area='';
		store_charts_smsrecount1.country='';
		store_charts_smsrecount1.sptype='';
		var chart;
		var chart_max =0;
		var chart_min =0;	
	function reflash(){ 
		store_charts_smsrecount2.new_params ={update:"update_panel",sdate:store_charts_smsrecount2.sdate,edate:store_charts_smsrecount2.edate,chart_name:store_charts_smsrecount2.chart_name,chart_number:store_charts_smsrecount2.chart_number,inum:store_charts_smsrecount2.inum,id:store_charts_smsrecount2.id,rate:store_charts_smsrecount2.rate,exppdf:store_charts_smsrecount2.exppdf,expexcel:store_charts_smsrecount2.expexcel,country:store_charts_smsrecount2.country,area:store_charts_smsrecount2.area,sptype:store_charts_smsrecount2.sptype};
		store_charts_smsrecount2.load();
		
		store_charts_smsrecount1.new_params ={update:"update",sdate:store_charts_smsrecount1.sdate,edate:store_charts_smsrecount1.edate,chart_name:store_charts_smsrecount1.chart_name,chart_number:store_charts_smsrecount1.chart_number,inum:store_charts_smsrecount1.inum,id:store_charts_smsrecount1.id,rate:store_charts_smsrecount1.rate,exppdf:store_charts_smsrecount1.exppdf,expexcel:store_charts_smsrecount1.expexcel,country:store_charts_smsrecount1.country,area:store_charts_smsrecount1.area,sptype:store_charts_smsrecount1.sptype};
		store_charts_smsrecount1.currentPage=1;
		store_charts_smsrecount1.load();
	
	
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
	Ext.define('State', {
        extend: 'Ext.data.Model',
        fields: [
            {type: 'string', name: 'groupid'},
            {type: 'string', name: 'groupname'}
        ]
    });
	//加载运营商
	var sptypeStore = Ext.create('Ext.data.Store', {
		        model: 'State',
		        autoLoad:true,
						proxy:{
							type:'ajax',
							url:'/v8/js/charts/sptypeStore_ch.js',
							reader:{
								type:'json',
								root:'data'
							}
						}
		    });
    var sptypeCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'sptypeCombo',
				//fieldLabel: '运营商',
		        //labelWidth: 60,
		        width: 100,
		        valueField:'groupid',
				emptyText:'运营商',
		        value:'',
		        displayField: 'groupname',    
		        allowBlank: true,
		        store: sptypeStore,
		        triggerAction: 'all',
		         queryMode: 'local',
		         typeAhead: true,
		              listeners: {
					          'change':function(){
							//	alert(this.value);
								store_charts_smsrecount1.sptype = this.value;
								store_charts_smsrecount2.sptype = this.value;
					            }
					        },
		
							listConfig: 
		            			{
		                			loadingText: 'Searching...',
		                			emptyText: 'No matching found.'
		            			}
		    });
	
	  Ext.define('State2', {
        extend: 'Ext.data.Model',
        fields: [
            {type: 'string', name: 'id'},
            {type: 'string', name: 'name'}
        ]
    });
	//加载省市信息
	var areaStore = Ext.create('Ext.data.Store', {
		        model: 'State2',
		        autoLoad:true,
						proxy:{
							type:'ajax',
							url:'/pronline/Msg?FunName@ncm_areaInfo',
							reader:{
								type:'json',
								root:'data'
							}
						}
		    });
	var areaCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'areaCombo',
				//fieldLabel: '地区',
		        //labelWidth: 40,
		        width: 100,
		        valueField:'id',
				emptyText:'地区',
		        value:'',
		        displayField: 'name',    
		        allowBlank: true,
		        store: areaStore,
		        triggerAction: 'all',
		         queryMode: 'local',
		         typeAhead: true,
		              listeners: {
					          'change':function(){
							//alert(this.value);
								store_charts_smsrecount1.area = this.value;
								store_charts_smsrecount2.area = this.value;
					            }
					        },
		
							listConfig: 
		            			{
		                			loadingText: 'Searching...',
		                			emptyText: 'No matching found.'
		            			}
		    })
	//加载国内国际信息
	var countryStore = Ext.create('Ext.data.Store', {
		        model: 'State',
		        autoLoad:true,
						proxy:{
							type:'ajax',
							url:'/v8/js/charts/countryStore_ch.js',
							reader:{
								type:'json',
								root:'data'
							}
						}
		    });
	var countryCombo = Ext.create('Ext.form.field.ComboBox', {
				id:'countryCombo',
				//fieldLabel: '国家',
		        //labelWidth: 40,
		        width: 100,
		        valueField:'groupid',
				emptyText:'国家',
		        value:'',
		        displayField: 'groupname',    
		        allowBlank: true,
		        store: countryStore,
		        triggerAction: 'all',
		         queryMode: 'local',
		         typeAhead: true,
		              listeners: {
					          'change':function(){
							//alert(this.value);
								store_charts_smsrecount1.country = this.value;
								store_charts_smsrecount2.country = this.value;
					            }
					        },
		
							listConfig: 
		            			{
		                			loadingText: 'Searching...',
		                			emptyText: 'No matching found.'
		            			}
		    })
			
	chart= Ext.create(Ext.chart.Chart,{
		//renderTo:'hello',
		width:Ext.get("layout_center").getWidth()-350,
		store:store_charts_smsrecount1,
		legend:{position:'top'
		},
		axes:[//坐标轴定义
		{type:'Numeric',position:'left',fields:['chart_number'],
		title:'短信数量（条）',//坐标标题
		grid:{even:{fill:'#ccc'}},//设置背景网格线
		minimum:0,
		maximum:100,//最大坐标
		majorTickSteps:9//每格刻度细化到2
		},
		{type:'Category',position:'bottom',fields:['chart_name'],
		title:'时间段（小时）',
		label:{}//旋转90度
		}
		],
		series:[{ //图表类型定义
			type:'line',
			axis:'left',//说明数值在哪条坐标轴上
			xField:'chart_name',
			yField:'chart_number',
			showInLegend:false,
			highlight:true,
			
			tips:{//字段渲染，移到坐标点显示什么信息
			renderer:function(rec){
			this.update(rec.get('chart_number'));
			}
			},
			
			fill:true,//以fill配置项定义的颜色填充折线下的区域，不透明度采用opacity配置定义的值，默认为false,
		//	style:{//定义fill
		//	fill:'#0f0',//填充颜色
		//	opacity:0.2//透明度
		//	},
			showMarkers:true,
		//	markerConfig:{
		//		type:'circle',
		//		radius:5,
		//		fill:'#f00',
		//	},
			smooth:true//点之间的连线会很平滑的环绕数据点，否则会使用之间连接数据点
		
		}]
	});
    Ext.apply(this,       
    {   
		//height:Ext.get("layout_center").getHeight(),
		id:'panel1',
		height:550,
        border:false,
        frame:false,
		autoScroll: true,
		store: store_charts_smsrecount2,
		//layout: 'fit',
		 columns:[{
            id: 'num',
            text: "序号",
            dataIndex: 'inum',
            hidden: false,
            width: 40,
            sortable: true
        },{
            text: "时间段（小时）",
            dataIndex: 'chart_name', 
            width: 90,
			renderer: showDetail
        },{
            text: "短信数量(条)",
            dataIndex: 'chart_number',           
            width: 90,
			align: 'right',
            sortable: true
         
        },
        {
            text: "百分比1",
            dataIndex: 'rate',
            hidden: false,
            width: 80,
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
                        value:startday,
                        id:'smsrecount_sdate1',
                        name:'smsrecount_sdate1'
					},'-',
			        	  {
                        fieldLabel:'到',labelWidth: 20,  width:130,        
                        xtype: 'datefield',
						format: "Y/m/d",
                        value:today,
                        id:'smsrecount_edate1',
                        name:'smsrecount_edate1'
					},'-',countryCombo,'-',areaCombo,'-',sptypeCombo,'-',
						{
						     text:"查询",
						     iconCls:'accept',
						     handler: function(){
			                 var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsrecount_sdate1').value);
		                	 var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsrecount_edate1').value);
			                 store_charts_smsrecount1.sdate=aa;  
			                 store_charts_smsrecount1.edate=bb;
			                 reflash();
		              
						}
						},'-',
						    {
						      	text:"导出",
						      	iconCls:'exp',
						      	handler: function(){
								var aa=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsrecount_sdate1').value);
								var bb=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('smsrecount_edate1').value);
								document.getElementById("form1").FunName.value ="ncm_SMSRecountChart";
								document.getElementById('form1').sdate.value=aa;  
								document.getElementById('form1').edate.value=bb;
								document.getElementById('form1').expexcel.value='1';
								document.getElementById('form1').submit();
								document.getElementById('form1').expexcel.value="";
							}
						    }
						],
			lbar:chart
        });
		

	reflash();
    //store_charts_smsrecount1.load();
    this.callParent(arguments);
 
    }
    
})