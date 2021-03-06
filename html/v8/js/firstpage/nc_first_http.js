
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
      
       
Ext.define('firstpage.nc_first_http',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
    	store_classtree.load();
 //   	 var aaa=screen.height;
   	 var bbb=screen.width;
        Ext.apply(this,       
        {   
        	 flex:1,
          layout: 'fit',

           width:300,
           height: 310,        	       
         scrollOffset: 0,
         border:true,
 //        id:'grid_onlineuser',
         classname:'firstpage.nc_first_http',
         autoScroll: true,

 //        frame:true,
         store:store_first_xwzs,
          viewConfig:{
	        	loadMask : false
	        },	
	        
	        tbar:[
             {                              
                                width:          170,
                                labelWidth: 80,
                                xtype:          'treecombox',
                                id: 'xwzsgroupid_first',
                                mode:           'local',
                                value:          '0',
                                triggerAction:  'all',
                                forceSelection: true,
       //                         iconCls:'add',
                                editable:       false,
                                fieldLabel:     '选择部门',
                                	iconCls:'remove',
                                name:           'StartBack',
                                displayField:   'text',
                                valueField:     'id',
                                queryMode: 'local',
                                value : '',
                                store:store_classtree
                   },           
            '-',
              {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;用户上网行为实时审计</span>'},        
            
            '->'
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){
                }
              }
             	  
            ],
	        
	        
	        
	                
          columns:[
         {           
            text: "用户名",
            dataIndex: 'username',
            width:110,
            height:22,						
            sortable: false
         },
         {           
            text: "源IP地址",
            dataIndex: 'sip',
            width:110,						
            sortable: false
         }
         ,
         {           
            text: "上网时间",
            dataIndex: 'stime',
            width:100,						
            sortable: false
         }
         ,
         {           
            text: "行为描述",
            dataIndex: 'mesg',
           
             flex:1,
 						
            sortable: false
         },
         {           
            text: "占用带宽(kbps)",
            dataIndex: 'rate',
            width:100,
            
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


   