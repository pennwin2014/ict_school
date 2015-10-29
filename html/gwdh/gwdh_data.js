Ext.define('ShopData', {
extend: 'Ext.data.Model',
fields: ['iNum', 'id', 'name', 'service_code', 'sgroupid', 'imgpath',
    'floor', 'url', 'code', 'tel', 'address', 'level', 'plevel', 'type',
    'stype', 'descr', 'detail', 'systime', 'flags']
//idProperty: 'uid'
});

//�̵�
var store_shop = Ext.create('Ext.data.Store', {
 model:'ShopData',
 pageSize: 10000,
 autoLoad:true,
 proxy:{
 		type:'ajax',
 		url:'/pronline/Msg?FunName@ncm_webcomshop_list&plateid@',
 		reader:{
 				type:'json',
 				root:'eimdata',
 				totalProperty:'totalCount'	
 		},
 		simpleSortMode:true	
 }
});
store_shop.load();
//�̳�
var store_comcorps = Ext.create('Ext.data.Store', {
 model:'ShopData',
 pageSize: 10000,
 autoLoad:true,
 proxy:{
 		type:'ajax',
 		url:'/pronline/Msg?FunName@ncm_comshops_list&plateid@',
 		reader:{
 				type:'json',
 				root:'eimdata',
 				totalProperty:'totalCount'	
 		},
 		simpleSortMode:true	
 }
});
store_comcorps.load();
//{
//    callback: function(records, options, success){ 
//        Ext.Msg.alert('info', '�̳Ǽ������'); 
//    }}

Ext.define('TypeData', {
    extend: 'Ext.data.Model',
    fields: ['sid', 'name', 'groupname', 'descr', 'systime', 'imgpath', 'flags']
    //idProperty: 'uid'
});
var store_type = Ext.create('Ext.data.Store', {
    model:'TypeData',
    pageSize: 10000,
    autoLoad:true,
    proxy:{
    	type:'ajax',
    	url:'/pronline/Msg?FunName@ncm_webcomtype_list&plateid@'+'',
    	reader:{
			type:'json',
			root:'eimdata',
			totalProperty:'totalCount'	
    	},
    	simpleSortMode:true	
    }
});
store_type.load();

