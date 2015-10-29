/*用户流量统计报表*/	
Ext.define('reports.trafficReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
				if(store_reports_trafficreport.alreadyload !=1){
						store_reports_trafficreport.slogdate = today;
						store_reports_trafficreport.elogdate = today;
						store_reports_trafficreport.slogtime = '00:00:00';
						store_reports_trafficreport.elogtime = '23:59:59';
						store_reports_trafficreport.code='';
						store_reports_trafficreport.username='';
						store_reports_trafficreport.ssid='1';
						store_reports_trafficreport.keyword='';
						store_reports_trafficreport.caExport='';
						store_reports_trafficreport.new_params ={slogdate:store_reports_trafficreport.slogdate,elogdate:store_reports_trafficreport.elogdate,slogtime:store_reports_trafficreport.slogtime,elogtime:store_reports_trafficreport.elogtime,username:store_reports_trafficreport.username,ssid:store_reports_trafficreport.ssid,keyword:store_reports_trafficreport.keyword,caExport:store_reports_trafficreport.caExport};
				}
			
				function reflash(){
						store_reports_trafficreport.new_params ={slogdate:store_reports_trafficreport.slogdate,elogdate:store_reports_trafficreport.elogdate,slogtime:store_reports_trafficreport.slogtime,elogtime:store_reports_trafficreport.elogtime,code:store_reports_trafficreport.code,username:store_reports_trafficreport.username,ssid:store_reports_trafficreport.ssid,keyword:store_reports_trafficreport.keyword,caExport:store_reports_trafficreport.caExport};
						//store_reports_trafficreport.currentPage=1;
						store_reports_trafficreport.load();
				}
				
				//加载SSID
			  var ssidStore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  		    {groupname:'全部', groupid:'0'}
//			  		    ,
//			  				{groupname:'内部员工', groupid:'1'},
//			  				{groupname:'其他用户', groupid:'2'}
			  		]
				});
		 var store_classtree=Ext.create('Ext.data.TreeStore', 
				{   
						fields: ['text','code'],
						autoLoad : true,
						proxy: 
						{   
							type: 'ajax',   

						url: '/pronline/Msg?FunName@ncmTreeCorp_comp_v4'  
						}  
				}
			);
//		    var ssidCombo = Ext.create('Ext.form.field.ComboBox', {
//						id:'trafficreport_ssidCombo',
//						fieldLabel: '用户类型',
//		        labelWidth: 60,
//		        width: 160,
//		        valueField:'groupid',
//		        emptyText:'请选择',
//						value:store_reports_trafficreport.ssid,
//		        displayField: 'groupname',
//		        name : 'trafficreport_ssidCombo',      
//		        allowBlank: true,
//		        store: ssidStore,
//		        triggerAction: 'all',
//		        queryMode: 'local',
//		        typeAhead: true,
//		        listeners: {
//			          'change':function(){
//			            	store_reports_trafficreport.ssid=this.value;
//										reflash();
//		            }
//		        },
//						listConfig:{
//		      			loadingText: 'Searching...',
//		      			emptyText: 'No matching found.'
//		        }
//				}); 
		
		    Ext.apply(this, {   
						//height:Ext.get("layout_center").getHeight(),
		        border:false,
		        frame:false,
						autoScroll: true,
		        store: store_reports_trafficreport,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "用户账号",
		            dataIndex: 'username',
		            width: 120,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "SSID",
		            dataIndex: 'ssid',
		            width: 120,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "上网流量(Mb)",
		            dataIndex: 'traffic',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		          
		        columnLines: true,
		        tbar:{items:[
		        	  '-',
		            '-',{
		            		fieldLabel:'开始时间',
		              	xtype: 'datefield',
		              	format: "Y/m/d",
		              	value:store_reports_trafficreport.slogdate,
		              	id:'trafficreport_slogdated',
		              	name:'trafficreport_slogdated',
		              	labelWidth:55, //标签宽度
										width:170 //字段宽度 
		            },{
		            		xtype:'timefield',
		              	value:store_reports_trafficreport.slogtime,
		              	id:'trafficreport_slogtimed',
		              	name:'trafficreport_slogtimed',
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
		              	value:store_reports_trafficreport.elogdate,
		              	id:'trafficreport_elogdated',
		              	name:'trafficreport_elogdated',
		              	labelWidth:55, //标签宽度
										width:170 //字段宽度 
		            },{
		            		xtype:'timefield',
		              	value:store_reports_trafficreport.elogtime,
		              	id:'trafficreport_elogtimed',
		              	name:'trafficreport_elogtimed',
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
		            },'-',{
             xtype:'treecombox',
             id: 'mgroupid1', 
						labelWidth: 60,
						fieldLabel:'选择单位',
            	name: 'mgroupname1',  
            	valueField: 'code',  
						  displayField: 'text',  
            	store:store_classtree
 				}, {
		                text:'查询',
		                //itemId: 'moveButton',
		            		iconCls:'accept',
		                //disabled: true,
		                handler:function(){
				                store_reports_trafficreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('trafficreport_slogdated').value);
						            store_reports_trafficreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('trafficreport_elogdated').value);
						            store_reports_trafficreport.slogtime=Ext.util.Format.substr(Ext.getCmp('trafficreport_slogtimed').value, 16, 8);
						            store_reports_trafficreport.elogtime=Ext.util.Format.substr(Ext.getCmp('trafficreport_elogtimed').value, 16, 8);
						            store_reports_trafficreport.currentPage=1;  
						          
						            
						            store_reports_trafficreport.code = Ext.getCmp("mgroupid1").getValue();        
						            reflash();  
		                }
		            },'-',{
		                text:'导出',
		                iconCls:'exp',
		                handler:function(){
		               			document.getElementById('form1').caExport.value = 'exp';
		                		document.getElementById("form1").slogdate.value = DateUtil.Format("yyyy/MM/dd",Ext.getCmp('trafficreport_slogdated').value);
					            	document.getElementById("form1").elogdate.value = DateUtil.Format("yyyy/MM/dd",Ext.getCmp('trafficreport_elogdated').value);
					            	document.getElementById("form1").slogtime.value = Ext.util.Format.substr(Ext.getCmp('trafficreport_slogtimed').value, 16, 8);
					            	document.getElementById("form1").elogtime.value = Ext.util.Format.substr(Ext.getCmp('trafficreport_elogtimed').value, 16, 8);
//					            	document.getElementById("form1").ssid.value = Ext.getCmp('trafficreport_ssidCombo').value
					            	document.getElementById("form1").action = "/pronline/Msg?FunName@ncm_web_App_intTrafReport_list?r="+Math.random();
		  									document.getElementById("form1").method = "POST";
			                	document.getElementById("form1").submit();
			                	document.getElementById("form1").caExport.value="";
		                }
		            }
		        ]}
		    });
				store_reports_trafficreport.alreadyload = 1;
		    this.callParent(arguments);
    } 
})

