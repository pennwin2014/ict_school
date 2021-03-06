
        Ext.define('EimData_swzshttp', 
    		{
        		extend: 'Ext.data.Model',
        		fields:['username','sip','stime','mesg','rate']
//        		,idProperty: 'sid'
        });
        
       var store_first_xwzs = Ext.create('Ext.data.Store',{
        		pageSize: 40,
        		id:'store_first_xwzs',
        		model: 'EimData_swzshttp',
        		autoLoad: false,
        		remoteSort: true,
        		proxy: 
        		{
           			type: 'ajax',
            			url:'/data/nc_first_xwzs_list.htm',
//          			url: '/pronline/Msg?FunName@ncWebShowZsweb_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		},
        		sorters: 
        		[{
            			property: '',
            			direction: 'DESC'
        		}]
    		});
      
       
Ext.define('firstpage.nc_first_morenetflow_grid',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
    	store_classtree.load();
 //   	 var aaa=screen.height;
   	 var bbb=screen.width;
        Ext.apply(this,       
        {   
 //       	 flex:1,
 //         layout: 'fit',

           width:290,
           height: 430,        	       
         scrollOffset: 0,
         border:true,
 //        id:'grid_onlineuser',
         classname:'firstpage.nc_first_morenetflow_grid',
         autoScroll: true,

 //        frame:true,
         store:store_netflow,
          viewConfig:{
	        	loadMask : false
	        },	
       
	                
          columns:[
         {           
            text: "时间",
            dataIndex: 'stime',
            width:60,
            height:22,						
            sortable: false
         },
         {           
            text: "进流量(kbps)",
            dataIndex: '进流量',
            width:70,						
            sortable: false
         }
         ,
         {           
            text: "出流量(kbps)",
            dataIndex: '出流量',
            width:70,						
            sortable: false
         }
         ,
         {           
            text: "总流量(kbps)",
            dataIndex: '总流量',
           
            width:70,
 						
            sortable: false
         }
          ],     
           columnLines: true
           
        }
        
        );
    
    this.callParent(arguments);
    store_first_xwzs.load();
    }
    
})


   