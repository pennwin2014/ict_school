/*黑名单设置*/
var tempr = '';
var pidd = store_rules_white1.pid;
var ptyped = store_rules_white1.ptype;

Ext.define('rules.white1',{
    extend: 'Ext.form.Panel',
	  autoScroll: false,
    frame: false,
    
    initComponent : function(){
		if(store_rules_white1.alreadyload !=1){
		    store_rules_white1.pid = pidd;
		    store_rules_white1.ptype = ptyped;
		    store_rules_white1.wtype = '';
		    store_rules_white1.nametype = '';
		    store_rules_white1.allname = '';
		    store_rules_white1.keyword = '';
		    store_rules_white1.caExport = ''; //导出
		
				store_rules_white1.new_params={pid:store_rules_white1.pid,ptype:store_rules_white1.ptype,wtype:store_rules_white1.wtype,nametype:store_rules_white1.nametype,allname:store_rules_white1.allname,keyword:store_rules_white1.keyword,caExport:store_rules_white1.caExport};
   
		}
		
		function reflash(){
				store_rules_white1.new_params={pid:store_rules_white1.pid,ptype:store_rules_white1.ptype,wtype:store_rules_white1.wtype,nametype:store_rules_white1.nametype,allname:store_rules_white1.allname,keyword:store_rules_white1.keyword,caExport:store_rules_white1.caExport};
				//store_rules_white1.currentPage=1;
				store_rules_white1.load({
						callback:function(){
							  var user = store_rules_white1.first();
							  Ext.getCmp('white1_pid').setValue(user.get('pid'));
							  Ext.getCmp('white1_ptype').setValue(user.get('ptype'));
							  Ext.getCmp('white1_wtype').setValue(user.get('wtype'));
							  Ext.getCmp('white1_nametype').setValue(user.get('nametype'));
							  var reg = new RegExp(",", "g");
								tempr = user.get('allname').replace(reg, "\n");
								Ext.getCmp('white1_allname').setValue(tempr);	
						}
				});
		}
		
		var ptypeStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'全局', groupid:'0'},
        		{groupname:'Group', groupid:'1'},
        		{groupname:'SSID', groupid:'2'}
        ]
		});
		
		var wtypeStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'黑名单', groupid:'0'},
        		{groupname:'白名单', groupid:'1'}
        ]
		});
		
		var nametypeStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
        data:[
        		{groupname:'IP', groupid:'1'},
        		{groupname:'URL', groupid:'2'},
        		{groupname:'MAC', groupid:'3'}
        ]
		});

    Ext.apply(this,{
        bodyStyle:'padding:20 20 20 20',//表单边距  
				frame:true,
				fieldDefaults:{
					  labelSeparator :':',  //分隔符
					  labelWidth : 80,//标签宽度  
            width : 400,//字段宽度 
						labelAlign:'left',  //标签对齐方式
						msgTarget:'side'	// 在字段的右边显示一个提示信息
				},
				items:[{
						xtype:'container',
						anchor:'100%',
						layout:'column',
						items:[{
								xtype:'container',
								columnWidth:.5,
								layout:'anchor',
								items:[{
										xtype:'textfield',
										fieldLabel:'ID',  // 和ptype关联的ID,如:Groupid, 
										id:'white1_pid',
										name:'white1_pid',
										value:pidd,
										hidden:true
								},{
										xtype:'combo',
										id:'white1_wtype',
										name:'white1_wtype',
										fieldLabel:'黑白名单',
										emptyText:'请选择黑白名单',
										triggerAction:'all',
										store:wtypeStore,
										displayField:'groupname',
										valueField:'groupid',
										queryMode:'local',
										forceSelection:false,
										typeAhead:true,
										hidden:false,
										allowBlank:false, //是否允许为空
								    blankText:'黑白名单不能为空！',	
								    msgTarget:'qtip', //显示一个浮动的提示信息 
								    afterLabelTextTpl: required,
										listeners: {
							          'change':function(){
							          	  store_rules_white1.pid=Ext.getCmp('white1_pid').value;
							          	  store_rules_white1.ptype=Ext.getCmp('white1_ptype').value;
							              store_rules_white1.wtype=this.value;
														reflash();
						            }
						        },
										listConfig:{
						      			loadingText: 'Searching...',
						      			emptyText: 'No matching found.'
						        }
								}]	
						},{
								xtype:'container',
								columnWidth:.5,
								layout:'anchor',
								items:[{
										xtype:'combo',
										id:'white1_ptype',
										name:'white1_ptype',
										fieldLabel:'类别',
										emptyText:'请选择类别',
										triggerAction:'all',
										store:ptypeStore,
										displayField:'groupname',
										valueField:'groupid',
										queryMode:'local',
										forceSelection:false,
										typeAhead:true,
										allowBlank:true,
										value:ptyped,
										hidden:true
								},{
										xtype:'combo',
										id:'white1_nametype',
										name:'white1_nametype',
										fieldLabel:'名单类别',
										emptyText:'请选择名单类别',
										triggerAction:'all',
										store:nametypeStore,
										displayField:'groupname',
										valueField:'groupid',
										queryMode:'local',
										forceSelection:false,
										typeAhead:true,
										hidden:false,
										allowBlank:false, //是否允许为空
								    blankText:'名单类别不能为空！',	
								    msgTarget:'qtip', //显示一个浮动的提示信息
								    afterLabelTextTpl: required, 
										listeners: {
							          'change':function(){
							          	  store_rules_white1.pid=Ext.getCmp('white1_pid').value;
							          	  store_rules_white1.ptype=Ext.getCmp('white1_ptype').value;
							              store_rules_white1.nametype=this.value;
							              if(this.value == 1){
							              		Ext.getCmp('white1_display').setValue("以下文本框一行一条数据，不得多于500行！例如：192.168.20.201:8080");	
							              }else if(this.value == 2){
							              		Ext.getCmp('white1_display').setValue("以下文本框一行一条数据，不得多于500行！例如：short.weixin.qq.com:80");		
							              }else if(this.value == 3){
							              		Ext.getCmp('white1_display').setValue("以下文本框一行一条数据，不得多于500行！例如：00:19:21:C4:BA:DB");	
							              }else{
							              		Ext.getCmp('white1_display').setValue("以下文本框一行一条数据，不得多于500行！");	
							              }
														reflash();
						            }
						        },
										listConfig:{
						      			loadingText: 'Searching...',
						      			emptyText: 'No matching found.'
						        }
								}]	
						}]	
				},{
						xtype:'displayfield',
						id:'white1_display',
						name:'white1_display',
						width:800,
						fieldLabel:'注意', 
						value:'以下文本框一行一个数据，不得多于500行数据！'	
				},{
						xtype:'textarea',
						fieldLabel:'名称',
						id:'white1_allname', 	//字段组件id
						name:'white1_allname',
						height:document.body.clientHeight-263,
						width:document.body.clientWidth-335,
						grow:true,
						preventScrollbar:true   //设置多行文本框没有滚动条显示
				}],
				buttons:[{
						text:'保存',
						//style:'margin-right:550;',
					  handler:function(){
					  		if(Ext.getCmp('white1_wtype').value == ''){
					  				alert("黑白名单不能为空！");
					  		}else if(Ext.getCmp('white1_nametype').value == ''){
					  				alert("名单类别不能为空！");	
					  		}else{
							  		var pid1 = Ext.getCmp('white1_pid').value;
							  		var ptype1 = Ext.getCmp('white1_ptype').value;
							  		var wtype1 = Ext.getCmp('white1_wtype').value;
							  		var nametype1 = Ext.getCmp('white1_nametype').value;
							  		var allname1 = Ext.getCmp('white1_allname').value;
							  		if(nametype1 == 3){
							  				allname1 = allname1.toLocaleUpperCase();	
							  		}
							  		var arrname = allname1.split("\n");
							  		if(arrname.length > 500){
							  				alert("输入数据不能超过500行！");	
							  		}else{
									  		store_rules_white1.load({
									  				params: {update: "update",pid:pid1,ptype:ptype1,wtype:wtype1,nametype:nametype1,allname:allname1,tmp:tempr},
									  				
									  				callback:function(){
									  					  alert('数据保存成功！');
						    	  					  var user = store_rules_white1.first();
						    	  					  Ext.getCmp('white1_pid').setValue(user.get('pid'));
						    	  					  Ext.getCmp('white1_ptype').setValue(user.get('ptype'));
						    	  					  Ext.getCmp('white1_wtype').setValue(user.get('wtype'));
						    	  					  Ext.getCmp('white1_nametype').setValue(user.get('nametype'));
						    	  					  var reg = new RegExp(",", "g");
						    	  						var tempt = '';
						    	  						tempt = user.get('allname').replace(reg, "\n");
						    	  						Ext.getCmp('white1_allname').setValue(tempt);	
						    	  				}	
									  		});
							  	  }
					  	  }
					  }	
				},{
						text:'返回',
						style:'margin-right:500;',
						handler:function(){
								parent.Ext.getCmp('white1_windr2').close();	
						}
				}]
    });
    reflash();
		store_rules_white1.alreadyload = 1;
    this.callParent(arguments);
    }
})