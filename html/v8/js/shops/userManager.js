/*会员管理*/
Ext.define('shops.userManager',{
    extend: 'Ext.tab.Panel',
    initComponent : function(){
        Ext.apply(this,{
		        layout : 'border',
		        id:'userManager',
		        margins: '0 0 0 0',
		        forceFit: true,
		        border:false,
		        scrollOffset: 0,
				    autoScroll: false, 
				    activeTab:0, 
		        defaults :{
		             bodyPadding: 0
		        },
		        items: [{
		        		autoScroll:true,id:'shops_shopusermanager',border:false,frame:false,title: "会员管理", items:[Ext.create('shops.shopuserManager')]	
		        }
		      /*
		        ,{
		        		autoScroll:true,id:'shops_consumpmanager',border:false,frame:false,title: "消费管理", items:[Ext.create('shops.consumpManager')]	
		        },{
		        		autoScroll:true,id:'shops_couponmanager',border:false,frame:false,title: "优惠劵管理", items:[Ext.create('shops.couponManager')]	
		        },{
		        		autoScroll:true,id:'shops_couponusemanager',border:false,frame:false,title: "优惠劵使用管理", items:[Ext.create('shops.couponuseManager')]	
		        },{
		        		autoScroll:true,id:'shops_shopattentmanager',border:false,frame:false,title: "用户关注管理", items:[Ext.create('shops.shopattentManager')]	
		        },{
		        		autoScroll:true,id:'shops_shopcollectmanager',border:false,frame:false,title: "用户收藏管理", items:[Ext.create('shops.shopcollectManager')]	
		        }
		      */
		        ],
		        listeners: { 
                tabchange:function(tp,p){
                  	var tabs_center=Ext.getCmp("userManager");
				            var active = tabs_center.getActiveTab();
										//隐藏所有模板
										// for(i=0; i<store_portplate.getCount();i++){
												// Ext.getCmp('plate'+i).setVisible(false); 
										// }
                  	
                  	if(p.title=='会员管理'){
                    		Ext.getCmp('page_main').bind(store_shops_shopusermanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_shopusermanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();
                  	}else if(p.title=='消费管理'){
                  			Ext.getCmp('page_main').bind(store_shops_consumpmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_consumpmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}else if(p.title=='优惠劵管理'){
                  			Ext.getCmp('page_main').bind(store_shops_couponmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_couponmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}else if(p.title=='优惠劵使用管理'){
                  			Ext.getCmp('page_main').bind(store_shops_couponusemanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_couponusemanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}else if(p.title=='用户关注管理'){
                  			Ext.getCmp('page_main').bind(store_shops_shopattentmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_shopattentmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}else if(p.title=='用户收藏管理'){
                  			Ext.getCmp('page_main').bind(store_shops_shopcollectmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_shopcollectmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}
                }
            }
		  	});
        this.callParent(arguments);
    }
})