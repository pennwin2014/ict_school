/*
首页的有关图表
*/
Ext.Loader.setConfig({enabled: true,paths:{'firstpage':'/v8/js/firstpage'}});
Ext.define('firstpage.nc_first_chart', {
    extend: 'Ext.panel.Panel',
    uses: ['firstpage.nc_first_netflowline','firstpage.nc_first_servicepie','firstpage.nc_first_topflowbar','firstpage.ncFirstBlank','firstpage.nc_first_http','firstpage.nc_first_yxzt','firstpage.nc_first_httppie','firstpage.nc_first_forbit','firstpage.nc_first_warn'],
    initComponent: function(){
          taskManager=new Ext.util.TaskRunner();
    		 		   firsttask={
				   	   run:function(){			      
				       firstpage.nc_first_chart.startReflash();				     
				   	  },interval:10000
				  };
       
   function onCheckItem(){
   	  var temp_var='';
   	  var num=0;
    	menu_display.items.each(function(item){
    	if(item.checked){
    		if(num==0){
    			temp_var=item.tname;
    		}
    		else{
    			temp_var=temp_var+','+item.tname;
    		}
    		num++;
    		
     
       }
      }
      )

        store_display_firstpage.load({params: {update:'update',vartparams:temp_var}});
      
  }

    	
    	
 //创建显示模块菜单
   var menu_display = Ext.create('Ext.menu.Menu', {
        id: 'menu_display',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '网络服务分布图',
                id:'firstservicepie', 
                tname:'firstpage.nc_first_servicepie', 
                checked:false,      
               handler:onCheckItem    
       
           },
            {
                text: '网络接口速率图',
                id:'firstnetflowline', 
                tname:'firstpage.nc_first_netflowline', 
               checked:false,      
                handler:onCheckItem    
       
           },
            {
                text: '当前用户流量使用Top10',
                id:'firsttopflowbar', 
                tname:'firstpage.nc_first_topflowbar', 
               checked:false,      
               handler:onCheckItem           
           },
            {
                text: '用户上网行为实时审计',
                id:'firsthttp', 
                tname:'firstpage.nc_first_http', 
               checked:false,      
               handler:onCheckItem    
       
           }
             ,
            {
                text: '系统运行状况',
                id:'firstpage.nc_first_yxzt', 
                tname:'firstpage.nc_first_yxzt', 
               checked:false,      
               handler:onCheckItem           
           },
            {
                text: '网站类别访问分布图',
                id:'firstpage.nc_first_httppie', 
                tname:'firstpage.nc_first_httppie', 
               checked:false,      
               handler:onCheckItem    
       
           }
           ,
            {
                text: '用户上网实时阻断',
                id:'firstpage.nc_first_forbit', 
                tname:'firstpage.nc_first_forbit', 
               checked:false,      
               handler:onCheckItem           
           },
            {
                text: '用户上网实时告警',
                id:'firstpage.nc_first_warn', 
                tname:'firstpage.nc_first_warn', 
               checked:false,      
               handler:onCheckItem    
       
           }
        ]

        
        
        
    });
    var temp_class='';
    store_display_firstpage.each(function(record){ 
    	temp_class=temp_class+record.get('classname');
    }
    );   
  menu_display.items.each(function(item){
	if(temp_class.indexOf(item.tname)!=-1){
	    item.checked=true;
	  }
	  else{
	  	item.checked=false;
	  }
});
    
        Ext.apply(this, {
 //          	height: 750,
 //           autoScroll: true,
             margin: '0 0 0 0',
             tbar:[{
             	text:'选择显示模块',
 //           	 xtype:'splitbutton',
//             	 menu:menu_display,
             	 handler:function(){           	 	
             	 	
             	 	 var fp2 = Ext.create('Ext.form.Panel', {
       
       
        autoHeight: true,
        width   : 340,
        height:350,
        bodyPadding: 10,
        frame:true,
        defaults: {
            anchor: '100%',
            labelWidth: 100
        },
        items   : [
           
            {
                xtype: 'fieldset',
                title: '选择显示模块',
                collapsible: true,
                defaults: {
                    labelWidth: 150,
                    anchor: '100%',
                    layout: {
                        type: 'hbox',
                        defaultMargins: {top: 0, right: 5, bottom: 0, left: 0}
                    }
                },
                items: [
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '网络服务使用情况分布图',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_servicepie',inputValue: 'firstpage.nc_first_servicepie'},
                            {xtype: 'numberfield',name:'r1', fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    },
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '网络接口速率图',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_netflowline',inputValue: 'firstpage.nc_first_netflowline'},
                            {xtype: 'numberfield',name:'r2', fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    },
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '当前用户流量使用Top10',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_topflowbar',inputValue: 'firstpage.nc_first_topflowbar'},
                            {xtype: 'numberfield',name:'r3', fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }
                    
                    ,
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '用户上网行为实时审计',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_http',inputValue: 'firstpage.nc_first_http'},
                            {xtype: 'numberfield', name:'r4',fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }                    
                     ,
                     {
                        xtype: 'fieldcontainer',
                       fieldLabel: '系统运行状况',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_yxzt',inputValue: 'firstpage.nc_first_yxzt'},
                            {xtype: 'numberfield',name:'r5', fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }
                     ,                   ,
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '网址类别访问分布图',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_httppie',inputValue: 'firstpage.nc_first_httppie'},
                            {xtype: 'numberfield', name:'r6',fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }
                     ,                   ,                   ,
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '阻断日志',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_forbit',inputValue: 'firstpage.nc_first_forbit'},
                            {xtype: 'numberfield', name:'r7',fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }
                   ,                      ,                   ,                   ,
                    {
                        xtype: 'fieldcontainer',
                       fieldLabel: '告警日志',
                        defaults: {
                            labelWidth: 40
                        },
                        items: [
                            {xtype:'checkbox', name: 'nc_first_warn',inputValue: 'firstpage.nc_first_warn'},
                            {xtype: 'numberfield', name:'r8',fieldLabel: '&nbsp;&nbsp;序号', value: '1',width:90}
                           
                        ]
                    }
                ]
            },
          
        
            
        ],
        buttons: [
            {
                text   : '保存',
                handler: function() {
             
                 this.up('form').getForm().doAction('submit',{
                 url:'/pronline/Msg',
                 method:'post',
                 params:{FunName:'ncFistpagedisp_save_v8'},   
                  success:function(form,action){
             //   	var msgaa=action.result.reason;
                     Ext.getCmp('addwindow').close();
                    store_display_firstpage.load();
                 }   
                 })

                }
            },
            {
                text   : '关闭',
                handler: function() {
                   Ext.getCmp('addwindow').close();
                

                }
            }
        ]
    });
    store_display_firstpage.each(function(record){ 
    	temp_class=record.get('classname');

    	if(temp_class=='firstpage.nc_first_servicepie'){
    		fp2.getForm().findField('nc_first_servicepie').setValue(true); 	
    		fp2.getForm().findField('r1').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_netflowline'){
    		fp2.getForm().findField('nc_first_netflowline').setValue(true); 	
    		fp2.getForm().findField('r2').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_topflowbar'){
    		fp2.getForm().findField('nc_first_topflowbar').setValue(true); 	
    		fp2.getForm().findField('r3').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_http'){
    		fp2.getForm().findField('nc_first_http').setValue(true); 	
    		fp2.getForm().findField('r4').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_yxzt'){
    		fp2.getForm().findField('nc_first_yxzt').setValue(true); 	
    		fp2.getForm().findField('r5').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_httppie'){
    		fp2.getForm().findField('nc_first_httppie').setValue(true); 	
    		fp2.getForm().findField('r6').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_forbit'){
    		fp2.getForm().findField('nc_first_forbit').setValue(true); 	
    		fp2.getForm().findField('r7').setValue(record.get('rindex')); 	 	
       }
       else if(temp_class=='firstpage.nc_first_warn'){
    		fp2.getForm().findField('nc_first_warn').setValue(true); 	
    		fp2.getForm().findField('r8').setValue(record.get('rindex')); 	 	
       }
     }
    ); 
   	 	
                 var win2 = new Ext.Window ({
                             id:"addwindow",
                             title:"设置显示模块",
                             x:200,
                             y:100,
                             width:390,
                             height:450,
                             bodyPadding: 10,
                             modal:true,
                             resizable:true,
           //                  draggable:false,
                             items: fp2
                                               
         })
        win2.show();     	 	
             	 	
             	 	
             	 	
             	 	
             	 	
             	 	
             	 	
             	 	
             	},
             	 iconCls:'add'
            },'-',{
             	text:'恢复默认模块',
             	iconCls:'remove',
             	 handler:function(){
             	 	if(confirm('你确定要恢复显示模块吗!')){
             	 	store_display_firstpage.load({params: {update:'del'}});
             	}
             	} 
            },'-',
             {xtype: 'numberfield',  labelWidth: 60,  fieldLabel: '刷新间隔', id:'firststep',name: 'step' ,value: '10',width:110,
             	    listeners:
               		{
               			'change':function(){
               				taskManager.stop(firsttask);
               				var step=	Ext.getCmp("firststep").getValue();
               				if(step<2) step=2;
               				firsttask={
										   	 run:function(){			      
										       firstpage.nc_first_chart.startReflash();				     
										   	},interval:step*1000
										  };
               				taskManager.start(firsttask);
               				
               			}
               		}
             	},
             	'->',
             	 '-',
             	 {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;系统连续运行：<font color=red>1天50小时</font>&nbsp;&nbsp;&nbsp;</span>'},
             	'-',
             	 {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;在线用户：<font color=red>12</font>&nbsp;&nbsp;&nbsp;</span>'},
             	 '-',
             	{xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;注册用户：<font color=red>12</font>&nbsp;&nbsp;&nbsp;</span>'},
             	'-',
         
             	{xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;IP连接数：<font color=red>12</font>&nbsp;&nbsp;&nbsp;</span>'},
             	'-'
             	,        
             	{xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;系统当前时间：<font color=red>2014/02/13 10:10:10</font>&nbsp;&nbsp;&nbsp;</span>'},
             	'-'
            ],
             
            layout: {              
                padding: '0 0 0 0'
            },
            items: [           
            {
              layout: {type: 'hbox'},
              margin: '0 0 0 0',
               border: false,
           	 items:[
           
            Ext.create(this.pan1),
             Ext.create(this.pan2)
               ]
           }           
          ,
            {
             layout: {type: 'hbox', align: 'stretch'},
             margin: '0 0 0 0',
             border: false,
             items:[        	 
             	 Ext.create(this.pan3),
            	 Ext.create(this.pan4)     
              ]
           }           
          ,
            {
             layout: {type: 'hbox', align: 'stretch'},
             margin: '0 0 0 0',
             border: false,
             items:[        	 
            	 Ext.create(this.pan5)
           	 ,
            	 Ext.create(this.pan6)     
              ]
           } 
           ,
            {
             layout: {type: 'hbox', align: 'stretch'},
             margin: '0 0 0 0',
             border: false,
             items:[        	 
         	 Ext.create(this.pan7),Ext.create(this.pan8)     
              ]
           } 
           ]
                    
               
           
        });
       
        this.callParent(arguments);
 		   	 	
				  taskManager.start(firsttask);
 

  //       Ext.getCmp("firstservicepie").checked=false;
    }
    ,
    inheritableStatics : {  
        startReflash : function(){ 
//实时刷新
        	var store_first=Ext.StoreManager.lookup("store_display_firstpage");
        	var classname,store1;
        	store_first.each(function(record){
        		classname=record.get('classname');
        		if(classname=='firstpage.nc_first_netflowline'){
        			 store1=Ext.StoreManager.lookup("store_netflow");
		           store1.load({params: {wljk:Ext.getCmp('wljkcombo').getValue()}});
        		}
        		else if(classname=='firstpage.nc_first_servicepie'){
        			store1=Ext.StoreManager.lookup("store_first_service");
        			store1.load({params: {sclass:Ext.getCmp('serviceclass_first').getValue()}});
        		}
        		else if(classname=='firstpage.nc_first_topflowbar'){
        			 store1=Ext.StoreManager.lookup("store_first_topflowbar");
//       			 alert(Ext.getCmp('topflowgroupid_first').getValue());
		          	store1.load({params: {groupid:Ext.getCmp('topflowgroupid_first').getValue()}});
        		}
        		else if(classname=='firstpage.nc_first_http'){
        			 store1=Ext.StoreManager.lookup("store_first_xwzs");
		           store1.load();
        		}
        		
       // 		alert(record.get('classname'));
        	});
        	
//alert('aaa');

          
        } 
    }
    
    
    
});


            
       
