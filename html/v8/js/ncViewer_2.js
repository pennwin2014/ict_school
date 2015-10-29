Ext.Loader.setConfig({enabled: true,paths:{'ncViewer':'/v8/js'}});
Ext.Loader.setConfig({enabled: true,paths:{'users':'/v8/js/users'}});
Ext.Loader.setConfig({enabled: true,paths:{'online':'/v8/js/online'}});
Ext.Loader.setConfig({enabled: true,paths:{'shops':'/v8/js/shops'}});
Ext.Loader.setConfig({enabled: true,paths:{'rules':'/v8/js/rules'}});
Ext.Loader.setConfig({enabled: true,paths:{'settings':'/v8/js/settings'}});
Ext.Loader.setConfig({enabled: true,paths:{'myaccount':'/v8/js/myaccount'}});
Ext.Loader.setConfig({enabled: true,paths:{'systems':'/v8/js/systems'}});

Ext.Loader.setConfig({enabled: true,paths:{'network':'/v8/js/network'}});
Ext.Loader.setConfig({enabled: true,paths:{'systemmanage':'/v8/js/systemmanage'}});
Ext.Loader.setConfig({enabled: true,paths:{'logs':'/v8/js/logs'}});
Ext.Loader.setConfig({enabled: true,paths:{'charts':'/v8/js/charts'}});
Ext.Loader.setConfig({enabled: true,paths:{'reports':'/v8/js/reports'}});
Ext.Loader.setConfig({enabled: true,paths:{'usermanger':'/v8/js/usermanger'}});
Ext.Loader.setConfig({enabled: true,paths:{'analysis':'/v8/js/analysis'}});

var new_params = "";
var grid_height; //显示区表单高度

// 引入扩展组件
Ext.Loader.setConfig({enabled:true});

Ext.require([
		'Ext.ux.data.PagingMemoryProxy',
		'Ext.ux.ProgressBarPager'
]);

Ext.define('ncViewer.App', {
		extend:'Ext.container.Viewport',
		uses:[],
		initComponent: function(){
				Ext.define('State', {
						extend:'Ext.data.Model',
						fields:['groupid', 'groupname']
				});
				Ext.apply(this, {
						layout:{
								type:'border'
						},
						items:[this.createNcTb(),this.createNcLeft(),this.createNcCenter()]
				});
				this.callParent(arguments);	
		},
		
		createNcTb:function(){
			  var tb = Ext.create('Ext.toolbar.Toolbar', {id:'tb',border:false,frame:false,style: 'background-image:url();'});
			  tb.add({
						xtype:'label',
						html:'&nbsp;<img src="/images/large/background.jpg" width="2880" height="78" style="vertical-align:top;margin-top:-15px">',
						width:1
				},{
						xtype:'label',
						style:'margin-bottom:1px;margin-top:1px;margin-left:150px;'
				},
				'->','->',{
						style:'margin-left:400px;',
						xtype:'combo',
						itemId:'search',
						store:store_search,
						//fieldLabel:'搜索',
            emptyText: '请输入搜索功能名',
						labelWidth: 30,
						width: 200,
						height:28,
						valueField:'groupid',
						displayField: 'groupname',    
						typeAhead: true,
						hideTrigger:true,
						hidden:true,
						enableKeyEvents:true,
						listeners: {
								'change':function(){
										//search();
								}
								//'keydown':function(i,e){
				              // var aae=e.getKey(); 
				              // if(aae==13){
											// var tabname = Ext.getCmp('tb').getComponent('search').getRawValue();
											// if(tabname !=''){
											// search();
											// }
				              // }
				         // }
						}
        },{
						xtype:'button',
						itemId:'searchimg',
            tooltip: '搜索',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'search',
						hidden:true
            //handler: search
        },'-',{
						xtype:'combo',
						itemId:'skintheme',
						store:store_skintheme,
            emptyText: '皮肤主题',
						labelWidth: 30,
						width: 100,
						valueField:'groupid',
						displayField: 'groupname',    
						typeAhead: true,
						hidden:true,
						listeners: {
								'change':function(){
										var fp=Ext.create('Ext.FormPanel');
										fp.form.doAction('submit',{
										url:'/pronline/Msg',
								    method:'POST',
										params:{FunName:'ncm_skintheme',update:this.value,username:username}
										});
										Ext.Msg.confirm('切换皮肤主题','请重新登录',
										function(btn){
												if(btn=='yes'){
												  	self.location='/enter.html';							
												}else{
												}
										},this);
								}
						}
        },'-','',{
						tooltip: '操作帮助',
						width:50,
            itemId:'help',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'help',
            hidden:true
            //handler: this.onItemHelp
        },{
						tooltip: '全屏显示',
						width:50,
            itemId:'max',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'hidden',
            handler: this.onItemMax
        },{
            tooltip: '系统刷新',
						width:50,
            itemId:'xtsx',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'refresh',
            handler: this.onItemReflash
        },{
            tooltip: '重新登录',
						width:50,
            scale:'large',
            itemId:'cxdl',
            iconAlign: 'top',
            iconCls: 'unlock',
						handler: this.onItemClose
        },{
	    	  	itemId:'logo',
       		  xtype:'label'
						//html:'<center><div class="ain_3d"><span class="gallery">新网程</span><br><span class="gallery2">登录门户管理系统</span></div></center>'
        });
			  
				this.toolPanel = Ext.create(Ext.panel.Panel, {
						id:'header',
						border:false,
						frame:false,
						region:'north',
						items:[tb],
						listeners:{
            		"collapse":	function(p){
            				//p.setTitle('<center>新网程登录门户管理系统</center>');	
            		},
            		"expand": function(p){
            		}
            }
				});
				return this.toolPanel;
		},
		
		createNcLeft:function(){
			  var left_tabs = Ext.createWidget('tabpanel', {
			  		width:225,
				    border:false,
				    margins:'0 0 0 0',
						//height:970,
						//tabPosition:'left',
						region:'west',
				    id:'treetab',    
				    activeTab:0,
				    defaults :{
				        bodyPadding:0
				    },
						enableTabScroll:true,
						listeners: {        
			          /*tabchange:function(tp,p){ 
										if(p.title=='菜单'){
								        Ext.getCmp('leftAll').setWidth(125);
											  Ext.getCmp('treetab').setWidth(123);	
								    }
										else if(p.title=='区域'){
								         Ext.getCmp('leftAll').setWidth(180);
											   Ext.getCmp('treetab').setWidth(160);	
								    }
								}*/
						}	
			  });
			  this.leftPanel=Ext.create(Ext.panel.Panel, {
             //id: 'left',
             margins: '0 0 0 0',
             //layout: 'accordtion',
             title:'<center>'+'豫园分析统计'+'</center>',
             region: 'west',
             // collapsible: true,
             autoScroll: true,
			       border:false,
             bodyStyle: 'background:#DFF0F7;text-align:center;',
             //bodyStyle: 'background:#fff;text-align:center;',
             split:true,
			       style:{marginBottom:'2px',marginLeft:'0px'},
             width: 225,
             //height:723,
	           minSize: 100,
	           maxSize: 500,
             items:[{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px;margin-top:10px;',
			            itemId:'ssxx',
			            hidden:false,
			            text:'实时信息',
			            //scale:'large',
			            //iconCls: 'zxxx',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmOnlineInfo'
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'sjbb',
			            hidden:false,
			            text:'数据报表',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'ptgl',
			            hidden:false,
			            text:'Portal管理',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'scgl',
			            hidden:false,
			            text:'素材管理',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'qxgl',
			            hidden:false,
			            text:'权限管理',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'sezi',
			            hidden:false,
			            text:'设置',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 },{
									xtype:'button',
									height:30,
									width:200,
									style:'margin-bottom:10px',
			            itemId:'zhgl',
			            hidden:false,
			            text:'账户管理',
									//scale:'large',
			            //iconCls: 'users',
			            //iconAlign: 'left',
			            //srcurl:'ncViewer.ncmUsers',
			            handler:this.onItemClick
						 }]
        });
        left_tabs.add({
					  closable: false,
				    border:false,
            items:[this.leftPanel],
					  title: '豫园分析统计'
		 		});
				this.leftPanel = Ext.create(Ext.panel.Panel, {
						id: 'left',
            margins: '0 2 0 2',
            // layout: 'border',
            // title:'<center>'+right_sy+'</center>',
            region: 'west',
            // collapsible: true,
            autoScroll: true,
			  		border:false,
            bodyStyle: 'background:#DFF0F7;',
            split:true,
			 			style:{marginBottom:'2px',marginLeft:'2px'},
            width: 225,
	          minSize: 100,
	          maxSize: 500,
            items:[this.leftPanel]
				});
				return this.leftPanel;	
		},
		
		createNcCenter:function(){
				this.centerPanel = Ext.createWidget('tabpanel', {
						resizeTabs: true,
		        enableTabScroll: true,
		        autoScroll:false,
				    //tabPosition: 'top',
		        region:'center',
		        id:'layout_center',
						border:false,
						frame:false,
						//deferredRender:false,
						bbar:[{
		           	xtype:'pagingtoolbar',
								border:false,
								frame:false,
		           	id:'page_main',
		           	hidden:true,
		            store:Ext.StoreManager.lookup('store_adminuser'),
		            beforePageText : "第",// update
                afterPageText : "页  共 {0} 页",// update
                firstText : "第一页",
                prevText : "上一页",// update
                nextText : "下一页",
                lastText : "最后页",
                refreshText : "刷新",
		            displayInfo: true,
		            displayMsg: '显示 <font color=red>{0}</font> - <font color=red>{1}</font> 条，  共 <font color=red>{2}</font> 条',
		            emptyMsg: "共 0 条",
								plugins: new Ext.ux.ProgressBarPager(),
								//plugins: Ext.create('Ext.ux.ProgressBarPager', {}),
		            doRefresh:function(){  
		            		this.store.load();
		            },
		            moveLast:function(){
						    	var b=this,a=b.getPageData().pageCount;
						    	if(b.fireEvent("beforechange",b,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						      }
						    },
						    moveFirst:function(){
							    if(this.fireEvent("beforechange",this,1)!==false){
						    		this.store.currentPage=1;
						    		this.store.load();
							    }
							  }, 
						   	movePrevious:function(){
						   		var b=this,a=b.store.currentPage-1;
						   		if(a>0){
						   			if(b.fireEvent("beforechange",b,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						   			}
						   		}
						   	},
						   	moveNext:function(){
						   		var c=this,b=c.getPageData().pageCount,a=c.store.currentPage+1;
						   		if(a<=b){
						   			if(c.fireEvent("beforechange",c,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						   			}
						   		}
						   	},
						   	onPagingKeyDown:function(i,h){
						    	var d=this,b=h.getKey(),c=d.getPageData(),a=h.shiftKey?10:1,g;
						    	if(b==h.RETURN){
						    		h.stopEvent();
						    		g=d.readPageFromInput(c);
						        if(g!==false){
						        	g=Math.min(Math.max(1,g),c.pageCount);
						         	if(d.fireEvent("beforechange",d,g)!==false){
							    		this.store.currentPage=g;
							    		this.store.load();
							        }        
							      }
							    }else{
						      	if(b==h.HOME||b==h.END){
						    			h.stopEvent();g=b==h.HOME?1:c.pageCount;i.setValue(g)
						       	}else{
						        	if(b==h.UP||b==h.PAGEUP||b==h.DOWN||b==h.PAGEDOWN){
							          h.stopEvent();g=d.readPageFromInput(c);
							          if(g){
							            if(b==h.DOWN||b==h.PAGEDOWN){a*=-1}
							            g+=a;
							            if(g>=1&&g<=c.pages){
							            	i.setValue(g)
							            }
							          }
						         	}
						        }
						      }  
						    } 
		        },'->',{
		        	  xtype:'label', html:'<div style="color:white;padding-left:100px;padding-right:20px;padding-top:8px;padding-bottom:8px;background:url(/images/large/bottom.png);">上海新网程信息技术股份有限公司承建</div>'
		        }],
		        listeners:{
		        		tebchange:function(tp,p){
		        				grid_height=Ext.getCmp('layout_center').getHeight()-63;	
		        		}	
		        }	
				});
				return this.centerPanel;	
		},
		
		onItemMax:function(){
			  //Ext.getCmp('header').setTitle('<center>新网程登录门户管理系统</center>');
				Ext.getCmp('header').collapse();
				//Ext.getCmp('header').preventHeader=false;
		},
		
		onItemReflash:function(){
          var fp=Ext.create('Ext.FormPanel');
          fp.form.doAction('submit',{
              url:'/pronline/Msg',
              method:'POST',
							waitMsg:'正在刷新数据, 请稍候...', 
							waitTitle:'提示', 
			
							params:{FunName:'ncm_system_reset'}, 
              success:function(form,action){
									Ext.Msg.alert('操作',"刷新完成!");
              },
             	failure:function(form,action){        		 
                  Ext.Msg.alert('操作',"系统正忙,稍候重试!");
            	}
          })
    },
    
    onItemClose:function(){
    		self.location='/enter.html';
    },
    
    onItemClick:function(item){
				//Ext.getCmp('leftAll').setWidth(0);
				//Ext.getCmp('treetab').setWidth(0);
				
				//Ext.getCmp('leftAll').collapse();				
				bbbb=Ext.getCmp('header');
				//bbbb.setTitle('<center>新网程登录门户管理系统 - '+item.text+'</center>');
				//Ext.getCmp('leftAll').setTitle('<center>'+item.text+'</center>');
			   
 				//改变中间的视图      
				var i=0;
				var m=0;
				var tab_title=item.text;
				//var tab_src=item.srcurl;
				var tab_id=item.itemId;
				var tabs_center=Ext.getCmp("layout_center");
				var tab_len=tabs_center.items.length;
				tabs_center.items.each(function(item) {
			     	if(item.id==tab_id){
					 			i++;
					 	}
			      if(i==0){m++;}
        });   
	      if(i>0){
		     	  tabs_center.setActiveTab(m);
		     	  return; 
	     	} 
	      if(tab_len>=1){
		      	for(i=0;i<tab_len;i++){     		
		      	 		tabs_center.remove(tab_len-i-1); 
		        }      	
	      } 
			
				if(tab_id=='ssxx'){
						tabs_center.add({
				          title: '短信发送', 
						      iconCls: 'chart_curve',
				          id:tab_id+'_dxfs',           
				          html: '<iframe  id="lay_ssxx_dxfs" name="lay_ssxx_dxfs" src="/ncm_chart/ncm_sms_send_chart.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				    });
				}else if(tab_id=='sezi'){  // 设置
						//store_settings_weixin.alreadyadd=0;
						//store_rules_srvpar.alreadyadd=0;
						//store_rules_white.alreadyadd=0;
						
						//if(right_sfun.indexOf("参数设置")>0)tabs_center.add({autoScroll:true,id:'rules_srvpar',iconCls: 'set',border:false,frame:false,title: "参数设置"});
						//if(right_sfun.indexOf("黑白名单")>0)tabs_center.add({autoScroll:true,id:'rules_white',iconCls: 'set',border:false,frame:false,title: "黑白名单"});
						if(right_sfun.indexOf("Portal参数")>0)tabs_center.add({          
					          title: 'Portal参数',
							      iconCls: 'set',
							      id:tab_id+'_ptsz',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmPortal_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
					  });
						if(right_sfun.indexOf("短信服务器")>0)tabs_center.add({          
					          title: '短信服务器',
							      iconCls: 'set',
							      id:tab_id+'_dxfwq',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmSms_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'    
					  });
						if(right_sfun.indexOf("HTTP服务器")>0)tabs_center.add({          
					          title: 'HTTP服务器',
							      iconCls: 'set',
							      id:tab_id+'_httpfwq',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmHttp_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
					  });
						if(right_sfun.indexOf("AC设置")>0)tabs_center.add({          
					          title: 'AC设置',
							      iconCls: 'set',
							      id:tab_id+'_acsz',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmAC_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("Radius设置")>0)tabs_center.add({        
					          title: 'Radius设置',
							      iconCls: 'set',
							      id:tab_id+'_rdsz',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmRadius_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("网络设置")>0)tabs_center.add({         
					          title: '网络设置',
							      iconCls: 'set',
							      id:tab_id+'_wlsz',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmWebSystemSet_base" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
					  });
						//if(right_sfun.indexOf("微信设置")>0)tabs_center.add({autoScroll:true,id:tab_id+'_weixin',iconCls: 'set',border:false,frame:false,title: "微信设置"});
				}
				else if(tab_id=='qxgl'){ // 权限管理
					  //store_systems_menu.alreadyadd=0;
					  //store_systems_submenu.alreadyadd=0;
					  
					  if(right_sfun.indexOf("系统用户管理")>0)tabs_center.add({
					          title: '系统用户管理',
										iconCls: 'system',
					          id:tab_id+'_xtyhmenu',            
					          html: '<iframe id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("系统权限管理")>0)tabs_center.add({
					          title: '系统权限管理',
							      iconCls: 'system',
					          id:tab_id+'_rolemenu',           
					          html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
						});
						//if(admincheck == "admin")tabs_center.add({autoScroll:true,id:tab_id+'_menu',iconCls: 'system',border:false,frame:false,title: "系统主菜单管理"});
						//if(admincheck == "admin")tabs_center.add({autoScroll:true,id:tab_id+'_submenu',iconCls: 'system',border:false,frame:false,title: "系统子菜单管理"});
						if(right_sfun.indexOf("用户访问日志")>0)tabs_center.add({
					          title: '用户访问日志',
							      iconCls: 'system',
					          id:tab_id+'_usvisitmenu',            
					          html: '<iframe  id="html_usvisit" name="html_usvisit" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						//createAreaTab();
				}
				else if(tab_id=='zhgl'){ // 账户管理
					  if(right_sfun.indexOf("修改密码")>0)tabs_center.add({
				          title: '修改密码', 
						      iconCls: 'key',
				          id:tab_id+'_modipassword',           
				          html: '<iframe  id="lay_myaccount_modipassword" name="lay_myaccount_modipassword" src="/pronline/Msg?FunName@ncmWebDispModiPass" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				    });
				}
				else{
				    //tabs_center.add({autoScroll:false,border:false,frame:false,id:tab_id,iconCls: 'add16',title: tab_title,items:[Ext.create(tab_src)]}); 
				}
				tabs_center.setActiveTab(0);
		}
});

