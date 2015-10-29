/*商标类别管理*/
Ext.define('shops.categoryManager',{
    extend: 'Ext.tab.Panel',
    initComponent : function(){
        Ext.apply(this,{
		        layout : 'border',
		        id:'categoryManager',
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
		        		autoScroll:true,id:'shops_groupmanager',border:false,frame:false,title: "大类管理", items:[Ext.create('shops.groupManager')]	
		        },{
		        		autoScroll:true,id:'shops_typemanager',border:false,frame:false,title: "小类管理", items:[Ext.create('shops.typeManager')]	
		        }],
		        listeners: { 
                tabchange:function(tp,p){
                  	var tabs_center=Ext.getCmp("categoryManager");
				            var active = tabs_center.getActiveTab();
										//隐藏所有模板
										// for(i=0; i<store_portplate.getCount();i++){
												// Ext.getCmp('plate'+i).setVisible(false); 
										// }
                  	
                  	if(p.title=='大类管理'){
                    		Ext.getCmp('page_main').bind(store_shops_groupmanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_groupmanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();
                  	}else if(p.title=='小类管理'){
                  			Ext.getCmp('page_main').bind(store_shops_typemanager);
												Ext.getCmp('page_main').setVisible(true);
												store_shops_typemanager.new_params={plateid:plateid};
												Ext.getCmp('page_main').doRefresh();	
                  	}
                }
            }
		  	});
        this.callParent(arguments);
    }
})