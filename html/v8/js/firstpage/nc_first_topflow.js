/*
首页的有关图表
*/
Ext.Loader.setConfig({enabled: true,paths:{'swzs':'/v8/js/swzs'}});
Ext.define('swzs.nc_first_topflow', {
    extend: 'Ext.panel.Panel',
    uses: ['swzs.nc_first_topflowbar'],

    initComponent: function(){

        Ext.apply(this, {
 //          	height: 750,
 //           autoScroll: true,
             margin: '0 0 0 0',
            layout: {              
                padding: '0 0 0 0'
            },
            items: [           
            {
              layout: {type: 'hbox', align: 'stretch'},
              margin: '0 0 0 0',
               border: false,
            	 items:[Ext.create(swzs.nc_first_topflowbar,{
            	 	maximum:this.max1,
            	  title:"用户上网流量Top10(单位Kbytes)",
            	  store:this.store1}
            	 	)
            	
           ]
           }           
           ,
            {
             layout: {type: 'hbox', align: 'stretch'},
             margin: '0 0 0 0',
              border: false,
              items:[Ext.create(swzs.nc_sszs_topflowbar,{
            	 	maximum:this.max2,
            	  title:"网络应用流量Top10(单位Kbytes)",
            	  store:this.store2}
              
              )            
            ]
           }           
           
           
           
           ]
                    
               
           
        });
        this.callParent(arguments);
    }
    
});


            
       
