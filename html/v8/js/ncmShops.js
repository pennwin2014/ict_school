/**
 定义商城tab
 */
 var store_portplate = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['sid', 'platename'],idProperty: 'userid',autoLoad: false,id:'store_portplate',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncm_portplate',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
	store_portplate.on('beforeload', function (store, options) {
        Ext.apply(store.proxy.extraParams, store_portplate.new_params);
       }); 
	   
	store_portplate.load();
				
Ext.define('ncViewer.ncmShops',{
    extend: 'Ext.tab.Panel',

    initComponent : function(){
        
        Ext.apply(this,{
        layout : 'border',
        id:'ncm_shops',
        forceFit: true,
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
 //       activeTab: 0,  
    //    deferredRender:false,      
        defaults :{ 
            bodyPadding: 0
        },
        items: [{
            title: '商店管理', 
            id:'shopmanager',           
            html: '<iframe  id="viewIframe_shopmanager" name="viewIframe_shopmanager" src="/ncmshop/ncm_webcomshop_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '品牌管理',
            id:'brandmanager',            
            html: '<iframe  id="viewIframe_brandmanager" name="viewIframe_brandmanager" src="/ncmshop/ncm_webcombrand_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '商品管理',
            id:'productmanager',            
            html: '<iframe  id="viewIframe_productmanager" name="viewIframe_productmanager" src="/ncmshop/ncm_webcomodity_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '广告管理',
            id:'admanager',            
            html: '<iframe  id="viewIframe_admanager" name="viewIframe_admanager" src="/ncmshop/ncm_webcomindex_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '会员管理',
            id:'usermanager',            
            html: '<iframe  id="viewIframe_usermanager" name="viewIframe_usermanager" src="/ncmshop/ncm_webshopuser_lab.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '活动管理',
            id:'activemanager',            
            html: '<iframe  id="viewIframe_activemanager" name="viewIframe_activemanager" src="/ncmshop/ncm_webshopactive_lab.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '商标类别管理',
            id:'logotype',            
            html: '<iframe  id="viewIframe_logotype" name="viewIframe_logotype" src="/ncmshop/ncm_webshoptype_lab.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})