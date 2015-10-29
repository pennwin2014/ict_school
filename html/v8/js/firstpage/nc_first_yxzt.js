 var ttitle='';

     
 var store_first_yxzt=Ext.create('Ext.data.Store', 
{
        	pageSize: 40,   
        	id:'store_first_yxzt',   		 	
          fields: ['fsystem','fdb','fdata'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_first_system_list.htm',
//            			url: '/pronline/Msg?FunName@ncWebOnlieUStop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}
 });
   
 
   		      
Ext.define('firstpage.nc_first_yxzt', {

    extend: 'Ext.panel.Panel',
   uses: ['firstpage.nc_first_ypxx','firstpage.nc_first_cpuxx'],
    initComponent: function(){

               Ext.apply(this, {
        	 flex:1,
          layout: 'fit',
             margin: '0 0 0 0',
                tbar:[
          
            {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;系统运行状况</span>'},
            '->'
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){
                }
              }
             	  
            ],
           
            layout: {              
                padding: '0 0 0 0'
            },
            items: [           
            {
              layout: {type: 'hbox'},
              margin: '0 0 0 0',
               border: false,

        	   items:[Ext.create('firstpage.nc_first_ypxx')          
             ]
           } ,
                       {
              layout: {type: 'hbox'},
              margin: '0 0 0 0',
               border: false,

        	   items:[Ext.create('firstpage.nc_first_cpuxx')          
             ]
           }
                     
               
          
          
           ]
                    
               
           
        });
        this.callParent(arguments);
    }
});
