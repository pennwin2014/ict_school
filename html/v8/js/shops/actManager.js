/*活动管理*/
Ext.define('shops.actManager', {
		extend:'Ext.tab.Panel',
		initComponent:function(){
				Ext.apply(this, {
						layout : 'border',
		        id:'actManager',
		        margins: '0 0 0 0',
		        forceFit: true,
		        border:false,
		        scrollOffset: 0,
				    autoScroll: false, 
				    activeTab:0, 
		        defaults :{
		             bodyPadding: 0
		        },
		        items:[{
		        		autoScroll:true,id:'shops_activemanager',border:false,frame:false,title: "促销广告主页", items:[Ext.create('shops.activeManager')]	
		        },{
		        		autoScroll:true,id:'shops_seriesmanager',border:false,frame:false,title: "促销广告明细", items:[Ext.create('shops.seriesManager')]	
		        }],
		        listeners:{
		        		tabchange:function(tp, p){
		        			var tabs_center=Ext.getCmp("userManager");
				            var active = tabs_center.getActiveTab();
										//隐藏所有模板
										// for(i=0; i<store_portplate.getCount();i++){
												// Ext.getCmp('plate'+i).setVisible(false); 
										// }
										
										if(p.title=='促销广告主页'){
                    		Ext.getCmp('page_main').bind(store_shops_activemanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_activemanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();
                  	}else if(p.title=='促销广告明细'){
                    		Ext.getCmp('page_main').bind(store_shops_seriesmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_seriesmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();
                  	}
		        		}	
		        }	
				});	
				this.callParent(arguments);
		}	
});