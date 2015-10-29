
        Ext.define('EimData_swzshttp', 
    		{
        		extend: 'Ext.data.Model',
        		fields:['username','sip','stime','mesg','rate']
//        		,idProperty: 'sid'
        });
        
       var store_first_forbit = Ext.create('Ext.data.Store',{
        		pageSize: 40,
        		id:'store_first_forbit',
        		model: 'EimData_swzshttp',
        		autoLoad: false,
        		remoteSort: true,
        		proxy: 
        		{
           			type: 'ajax',
            			url:'/data/nc_first_forbit_list.htm',
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
      
    
Ext.define('firstpage.nc_first_forbit',{
    extend: 'Ext.grid.Panel',
    initComponent : function(){
 	
 //   	 var aaa=screen.height;
        Ext.apply(this,       
        {   
        	 flex:1,
          layout: 'fit',

           width:300,
           height: 310,        	       
         scrollOffset: 0,
         border:true,
 //        id:'grid_onlineuser',
         classname:'firstpage.nc_first_forbit',
         autoScroll: true,

 //        frame:true,
         store:store_first_forbit,
          viewConfig:{
	        	loadMask : false
	        },	
	        
	        tbar:[
             {                              
                                width:          170,
                                labelWidth: 80,
                                xtype:          'treecombox',
    //                            id: 'flowgroup',
                                mode:           'local',
                                value:          'mrs',
                                triggerAction:  'all',
                                forceSelection: true,
       //                         iconCls:'add',
                                editable:       false,
                                fieldLabel:     'ѡ����',
                                	iconCls:'remove',
                                name:           'StartBack',
                                displayField:   'text',
                                valueField:     'id',
                                queryMode: 'local',
                                value : '',
                                store:store_classtree
                   },           
            '-',
              {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;�û�����ʵʱ���</span>'},        
            
            '->'
             	   ,{
                text:'������Ϣ',
                iconCls:'add',
                handler: function(){
                }
              }
             	  
            ],
	        
	        
	        
	                
          columns:[
         {           
            text: "�û���",
            dataIndex: 'username',
            width:110,
            height:22,						
            sortable: false
         },
         {           
            text: "ԴIP��ַ",
            dataIndex: 'sip',
            width:110,						
            sortable: false
         }
         ,
         {           
            text: "����ʱ��",
            dataIndex: 'stime',
            width:100,						
            sortable: false
         }
         ,
         {           
            text: "�������",
            dataIndex: 'mesg',
           
             flex:1,
 						
            sortable: false
         }
          ],     
           columnLines: true
           
        }
        
        );
    
    
    
    
    this.callParent(arguments);
 //   store_first_forbit.load();
    }
    
})


   