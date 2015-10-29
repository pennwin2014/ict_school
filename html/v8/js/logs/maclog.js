/*短信日志*/
Ext.define('logs.maclog',{
		extend: 'Ext.grid.Panel',
		initComponent : function(){
				if(store_logs_maclog.alreadyload !=1){
						store_logs_maclog.time_flag='1';
						store_logs_maclog.slogdate = today;
						store_logs_maclog.elogdate = today;
						store_logs_maclog.slogtime = '00:00:00';
						store_logs_maclog.elogtime = '23:59:59';
						store_logs_maclog.sdate='';
						store_logs_maclog.edate='';
						store_logs_maclog.pname='';
						store_logs_maclog.mac='';
						store_logs_maclog.apmac='';
						store_logs_maclog.groupid='';
						store_logs_maclog.groupcode='';
						store_logs_maclog.mark='';
						store_logs_maclog.lrssi='';
						store_logs_maclog.urssi='';
						store_logs_maclog.starttime='';
						store_logs_maclog.endtime='';
						store_logs_maclog.flag='';
						store_logs_maclog.keyword='';
						store_logs_maclog.caExport='';
						store_logs_maclog.new_params ={time_flag:store_logs_maclog.time_flag,sdate:store_logs_maclog.sdate,edate:store_logs_maclog.edate,pname:store_logs_maclog.pname,mac:store_logs_maclog.mac,apmac:store_logs_maclog.apmac,groupid:store_logs_maclog.groupid,groupcode:store_logs_maclog.groupcode,mark:store_logs_maclog.mark,lrssi:store_logs_maclog.lrssi,urssi:store_logs_maclog.urssi,starttime:store_logs_maclog.starttime,endtime:store_logs_maclog.endtime,flag:store_logs_maclog.flag,keyword:store_logs_maclog.keyword,caExport:store_logs_maclog.caExport};
				}
				
				function reflash(){
						store_logs_maclog.new_params ={time_flag:store_logs_maclog.time_flag,sdate:store_logs_maclog.sdate,edate:store_logs_maclog.edate,pname:store_logs_maclog.pname,mac:store_logs_maclog.mac,apmac:store_logs_maclog.apmac,groupid:store_logs_maclog.groupid,groupcode:store_logs_maclog.groupcode,mark:store_logs_maclog.mark,lrssi:store_logs_maclog.lrssi,urssi:store_logs_maclog.urssi,starttime:store_logs_maclog.starttime,endtime:store_logs_maclog.endtime,flag:store_logs_maclog.flag,keyword:store_logs_maclog.keyword,caExport:store_logs_maclog.caExport};
						//store_logs_maclog.currentPage=1;
						store_logs_maclog.load();
				}
				
				//加载时间
			  var timeflagtore = Ext.create('Ext.data.Store', {
		        fields:['groupname', 'groupid'],
			  		data:[
			  				{groupname:'今天', groupid:'1'},
			  				{groupname:'最近三天', groupid:'2'},
			  				{groupname:'最近一周', groupid:'3'},
			  				{groupname:'最近一月', groupid:'4'},
			  				{groupname:'最近三月', groupid:'5'},
			  				{groupname:'最近半年', groupid:'6'},
			  				{groupname:'自定义', groupid:'0'}
			  		]
				});
		    var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
						id:'maclog_timeFlagCombo',
						fieldLabel: '时间',
		        labelWidth: 30,
		        width: 130,
		        valueField:'groupid',
						emptyText:'请选择',
		        value:store_logs_maclog.time_flag,
		        displayField: 'groupname',
		        name : 'time_flag',      
		        allowBlank: true,
		        store: timeflagtore,
		        triggerAction: 'all',
		        queryMode: 'local',
		        typeAhead: true,
		        listeners: {
			          'change':function(){
			              store_logs_maclog.time_flag=this.value;
										reflash();
		            }
		        },
						listConfig:{
		      			loadingText: 'Searching...',
		      			emptyText: 'No matching found.'
		        }
				});  
				
				//高级查询窗口
				var win3 = null;
			  function showCxForm() {
			    if (!win3) { //判断如果不存在就创建新的
			        win3 = new Ext.Window({
							    title:'MAC日志高级查询窗口',
							    closeAction: 'hide',
							    layout:'form',  
							    width:300,
							    draggable:true, //可拖动的
						      //maximizable:true, //可最大化的
						      //resizable: true, //可改变大小
			            modal: false,//后面的内容可以操作
						      //plain：true,//则主体背景透明
						      //items: fp2
							    constrain:true, //限制窗口只能在其容器内移动
						      //minimizable:true, //可最小化
						      
			            items:[new Ext.FormPanel({
			            	 id:'maclog_form2', 
			               layout:"form",
								     baseCls:"x-plain",
			               bodyPadding: 5,
			               items:[{   						
			                        	fieldLabel:'开始日期',
			                        	xtype: 'datefield',
			                        	format: "Y/m/d",
			                        	value:store_logs_maclog.slogdate,
			                        	name:'slogdate'
			                    	},{
			                    			fieldLabel:'开始时间',
			                    			xtype:'timefield',
								              	value:store_logs_maclog.slogtime,
								              	name:'slogtime',
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
			                    	},{   						
			                        	fieldLabel:'结束日期',
			                        	xtype: 'datefield',
			                        	format: "Y/m/d",
			                        	value:store_logs_maclog.elogdate,
			                        	name:'elogdate'
			                    	},{
			                    			fieldLabel:'结束时间',
			                    			xtype:'timefield',
								              	value:store_logs_maclog.elogtime,
								              	name:'elogtime',
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
			                      },{
			                        	fieldLabel:'Portal',
			                        	xtype: 'textfield',
																emptyText:'请输入Portal名称',
																value:store_logs_maclog.pname,
			                        	name:'pname'
			                    	},{
			                        	fieldLabel:'MAC地址',
			                        	xtype: 'textfield',
																emptyText:'请输入MAC地址',
																value:store_logs_maclog.mac,
			                        	name:'mac'
			                    	},{
			                        	fieldLabel:'AP MAC地址',
			                        	xtype: 'textfield',
																emptyText:'请输入AP MAC地址',
																value:store_logs_maclog.apmac,
			                        	name:'apmac'
			                    	}]  
			                })],  
							        buttons:[{
												  text:"确定",
												  handler:function(){
								        			var objectTmp = Ext.getCmp("maclog_form2").getForm().getValues();
															store_logs_maclog.slogdate = objectTmp.slogdate;
															store_logs_maclog.elogdate = objectTmp.elogdate;
															store_logs_maclog.slogtime = objectTmp.slogtime;
															store_logs_maclog.elogtime = objectTmp.elogtime;
								        			store_logs_maclog.sdate = objectTmp.slogdate+" "+objectTmp.slogtime;
								        			store_logs_maclog.edate = objectTmp.elogdate+" "+objectTmp.elogtime;
								        			store_logs_maclog.pname = objectTmp.pname;
								        			store_logs_maclog.mac = objectTmp.mac;
								        			store_logs_maclog.apmac = objectTmp.apmac;
								        			Ext.getCmp('maclog_timeFlagCombo').setValue('0');
															reflash();
															win3.close();
												  }
											},{
													text:"取消",
													handler: function(){
															win3.close();
													}
											}]
							});
					}
			    win3.show();
			  }
				
		    Ext.apply(this, {   
			    	height: Ext.get("layout_center").getHeight()-60,
		        scrollOffset: 0,
		        border:false,
		        frame:false,
		        store: store_logs_maclog,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "Portal",
		            dataIndex: 'pname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "Mac地址",
		            dataIndex: 'mac',
		            width: 120,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "AP Mac地址",
		            dataIndex: 'apmac',
		            width: 120,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "单位名称",
		            dataIndex: 'groupname',
		            width: 100,
		            //align: 'center',
		            hidden: false,
		            sortable: true
		        },{
		            text: "单位编码",
		            dataIndex: 'groupcode',
		            width: 200,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "标签",
		            dataIndex: 'mark',
		            width: 90,
		            //align: 'right',
		            hidden: true,
		            sortable: false
		        },{
		            text: "最低信号强度",
		            dataIndex: 'lrssi',
		            width: 90,
		            align: 'right',
		            hidden: false,
		            sortable: true
		        },{
		            text: "最大信号强度",
		            dataIndex: 'urssi',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "开始时间",
		            dataIndex: 'starttime',
		            width: 135,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "结束时间",
		            dataIndex: 'endtime',
		            width: 135,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "标记",
		            dataIndex: 'flag',
		            width: 60,
		            //align: 'center',
		            hidden: true,
		            sortable: true
		        },{
		        	 flex: 1	
		        }],
				
		        columnLines: true,
		        tbar:{items:[
		            '-', timeFlagCombo,//时间下拉菜单
		            '-', {
		            	  labelWidth: 45,
		            	  width:200,
		                fieldLabel:'关键字',
		                xtype: 'textfield',
		                id:'maclog_keyword',
		                name:'maclog_keyword',
										emptyText:'请输入Portal/MAC/AP Mac',
		                value:store_logs_maclog.keyword,
		                enableKeyEvents:true,
		                listeners:{
			                 'focus':function(){
					                 if(this.value=='请输入Portal/MAC/AP Mac'){                
					                    this.setValue('');
					                 }
		                   },
		         
			                'keydown' : function(i,e){
			                    var aae=e.getKey(); 
			                    if(aae==13){
			                    		var aa=Ext.getCmp('maclog_keyword').value;
					                    if(aa!='请输入Portal/MAC/AP Mac'){
							       	            store_logs_maclog.keyword=aa;
											            store_logs_maclog.currentPage=1;
											            reflash();
							                }
			                    }
			                }
		                }
		            },  '-',{
		                text:'查询',
		            		iconCls:'accept',
		                //disabled: true,
		                handler:function(){
			                 var bb=Ext.getCmp('maclog_keyword').value;
				               if(bb!='请输入Portal/MAC/AP Mac'){         
				                 	store_logs_maclog.keyword=bb;
				               }else{
				                 	store_logs_maclog.keyword='';
				               }
				               store_logs_maclog.currentPage=1;         
				               reflash();  
		                }
		            },'-',{
		                text:'高级',
		                anchor: 'right',
		                handler: showCxForm, //高级查询函数
		                iconCls:'option'
		            }
		        ]}
		    });
			  store_logs_maclog.alreadyload = 1;
		    this.callParent(arguments);
		}
})
