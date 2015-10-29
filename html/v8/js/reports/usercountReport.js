// 一天中一人的来访次数-0827
Ext.define('reports.usercountReport',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
				if(store_reports_usercountreport.alreadyload !=1){
						store_reports_usercountreport.slogdate = today;
						store_reports_usercountreport.elogdate = today;
						store_reports_usercountreport.slogtime = '00:00:00';
						store_reports_usercountreport.elogtime = '23:59:59';
						store_reports_usercountreport.username='';
						store_reports_usercountreport.usercount='';
						store_reports_usercountreport.groupid='';
						store_reports_usercountreport.keyword='';
						store_reports_usercountreport.caExport='';
						store_reports_usercountreport.new_params ={slogdate:store_reports_usercountreport.slogdate,elogdate:store_reports_usercountreport.elogdate,slogtime:store_reports_usercountreport.slogtime,elogtime:store_reports_usercountreport.elogtime,username:store_reports_usercountreport.username,usercount:store_reports_usercountreport.usercount,groupid:store_reports_usercountreport.groupid,keyword:store_reports_usercountreport.keyword,caExport:store_reports_usercountreport.caExport};
				}
		
				function reflash(){
						store_reports_usercountreport.new_params ={slogdate:store_reports_usercountreport.slogdate,elogdate:store_reports_usercountreport.elogdate,slogtime:store_reports_usercountreport.slogtime,elogtime:store_reports_usercountreport.elogtime,username:store_reports_usercountreport.username,usercount:store_reports_usercountreport.usercount,groupid:store_reports_usercountreport.groupid,keyword:store_reports_usercountreport.keyword,caExport:store_reports_usercountreport.caExport};
						//store_reports_usercountreport.currentPage=1;
						store_reports_usercountreport.load();
				}
				
		    Ext.apply(this, {   
		        border:false,
		        frame:false,
				    autoScroll: true,
		        store: store_reports_usercountreport,
		        viewConfig:{
			        	loadMask : false
			      },
		        columns:[{
		            text: "用户账号",
		            dataIndex: 'username',
		            width: 150,
		            //align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "单位名称",
		            dataIndex: 'groupname',
		            width: 150,
		            //align: 'center',
		            hidden: false,
		            sortable: false
		        },{
		            text: "访问次数",
		            dataIndex: 'usercount',
		            width: 100,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		            text: "停留时间",
		            dataIndex: 'conntime',
		            width: 150,
		            align: 'right',
		            hidden: false,
		            sortable: false
		        },{
		        	 flex: 1	
		        }],
		          
						columnLines: true,
						tbar:{items:[
								'-',{
										fieldLabel:'开始时间',
								  	xtype: 'datefield',
								  	format: "Y/m/d",
								  	value:store_reports_usercountreport.slogdate,
								  	id:'usercountreport_slogdated',
								  	name:'usercountreport_slogdated',
								  	labelWidth:55, //标签宽度
										width:170 //字段宽度 
								},{
										xtype:'timefield',
								  	value:store_reports_usercountreport.slogtime,
								  	id:'usercountreport_slogtimed',
								  	name:'usercountreport_slogtimed',
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
										fieldLabel:'结束时间',
								  	xtype: 'datefield',
								  	format: "Y/m/d",
								  	value:store_reports_usercountreport.elogdate,
								  	id:'usercountreport_elogdated',
								  	name:'usercountreport_elogdated',
								  	labelWidth:55, //标签宽度
										width:170 //字段宽度 
								},{
										xtype:'timefield',
								  	value:store_reports_usercountreport.elogtime,
								  	id:'usercountreport_elogtimed',
								  	name:'usercountreport_elogtimed',
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
		            		//labelWidth: 60,
		            		width:100,
		                //fieldLabel:'访问次数',
		                xtype: 'textfield',
		                id:'usercountreport_usercount',
		                name:'usercountreport_usercount',
						        emptyText:'访问次数',
		                value:store_reports_usercountreport.usercount,
		                enableKeyEvents:true,
		                listeners:{
				                'focus':function(){
						                 if(this.value=='访问次数'){                
						                    this.setValue('');
						                 }
												},
												
				                'keydown' : function(i,e){
				                    var aae=e.getKey(); 
				                    if(aae==13){
				                    		var aa=Ext.getCmp('usercountreport_usercount').value;
								       	    		store_reports_usercountreport.usercount=aa;
										    				store_reports_usercountreport.currentPage=1;
										    				reflash();
				                    }
				                }
		                }
		            },'-',{
								    text:'查询',
								    //itemId: 'moveButton',
										iconCls:'accept',
								    //disabled: true,
								    handler:function(){
										    store_reports_usercountreport.slogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('usercountreport_slogdated').value);
										    store_reports_usercountreport.elogdate=DateUtil.Format("yyyy/MM/dd",Ext.getCmp('usercountreport_elogdated').value);
										    store_reports_usercountreport.slogtime=Ext.util.Format.substr(Ext.getCmp('usercountreport_slogtimed').value, 16, 8);
										    store_reports_usercountreport.elogtime=Ext.util.Format.substr(Ext.getCmp('usercountreport_elogtimed').value, 16, 8);
										    store_reports_usercountreport.usercount=Ext.getCmp('usercountreport_usercount').value;
										    store_reports_usercountreport.currentPage=1;         
										    reflash();  
								    }
								},'-',{
								    text:'导出',
								    iconCls:'exp',
								    handler:function(){
								    		store_reports_usercountreport.caExport='exp';
												document.getElementById("form1").FunName.value ="ncm_web_App_userCount_list";
												document.getElementById("form1").caExport.value =store_reports_usercountreport.caExport;
												document.getElementById("form1").slogdate.value =store_reports_usercountreport.slogdate;
												document.getElementById("form1").elogdate.value =store_reports_usercountreport.elogdate;
												document.getElementById("form1").slogtime.value =store_reports_usercountreport.slogtime;
												document.getElementById("form1").elogtime.value =store_reports_usercountreport.elogtime;
								    		document.getElementById("form1").submit();
								    		
								   			store_reports_usercountreport.caExport="";
								    }
								}
						]}
		    });
				store_reports_usercountreport.alreadyload = 1;
		    this.callParent(arguments);
    }
})