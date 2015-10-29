        var ttitle='';
   
         var store_netflow=Ext.create('Ext.data.Store', 
       		{
        		pageSize: 40,
        		id:'store_netflow',
            fields: ['stime', {name:'总流量',mapping:'totbytes'},{name:'进流量',mapping:'inbytes'}, {name:'出流量',mapping:'outbytes'}],
        		autoLoad:true,
        		remoteSort: true,
        		proxy: 
        		{
            			type: 'ajax',
            			url: '/data/nc_devflowrate_list.htm',
//            			url: '/pronline/Msg?FunName@ncWebDevflow_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		},
        		sorters: 
        		[{
            			property: 'stime',
            			direction: 'DESC'
        		}]
    		}); 
   		 
   		 var store_devinteface= Ext.create('Ext.data.Store', {
                                    fields : ['gname', 'gid'],
                                    autoLoad: true,
                                    proxy: 
												        		{
												           			type: 'ajax',
												            			url:'/data/nc_devinterface_list.htm',
												//          			url: '/pronline/Msg?FunName@ncWebShowZsweb_v8',
												            			reader: {
												                			type:'json',
												                			root: 'eimdata',
												                			totalProperty: 'totalCount'
												            			},         
												            			simpleSortMode: true
												        		}
                                });     
   		 
    		      
Ext.define('firstpage.nc_first_more_netflowline', {

    extend: 'Ext.panel.Panel',
    requires: ['Ext.chart.*'],
    initComponent: function(){

        Ext.apply(this, {
          flex:1,
          layout: 'fit',
            height: 410,
            items: {
                xtype: 'chart',
                animate: true,
                shadow: false,
                store:store_netflow,
                legend: {
       
                   position: 'bottom'
                }, 
                margin: '30 10 10 10',

                
                 axes: [{
			            type: 'Numeric',
			            position: 'left',
			            minimum: 0,
	//	            maximum: this.maximum,
			            fields: ['总流量','进流量','出流量'],
			            renderer: Ext.util.Format.numberRenderer('0,0'),         
                  minorTickSteps: 1,
			            minimum: 0,
			            grid: {
                    odd: {
                        opacity: 1,
                        fill: '#fff',
                        stroke: '#bbb',
                        'stroke-width': 0.5
                    }  ,               
                     even: {
                        opacity: 1,
                        fill: '#fff',
                        stroke: '#bbb',
                        'stroke-width': 0.5
                    }
                }
			   //         hidden: true
			        }, {
			            type: 'Category',
			            position: 'bottom',
			            fields: ['stime'],
			            steps:3,
	                label: {
	                
	                font: '11px Arial',
	                rotate: {
	                    degrees: 0
	                }
	              }
			        }],
      //       theme: 'White',
                            
                 series: [{
                type: 'line',
                axis: 'left',
                xField: 'stime',
                yField: '总流量',
                style:{
                	'stroke-width':1.5
                },
 //               smooth:true,
                markerConfig: {
                    type: 'cross',
                    size: 0,
                    radius: 0,
                    'stroke-width': 0
                }
               },
               {
                type: 'line',
                axis: 'left',
                xField: 'stime',
                yField: '进流量',
                style:{
                	'stroke-width':1.5
                },
 //               smooth:true,
                markerConfig: {
                    type: 'cross',
                    size: 0,
                    radius: 0,
                    'stroke-width': 0
                }
               },
               {
                type: 'line',
                axis: 'left',
                xField: 'stime',
                yField: '出流量',
                style:{
                	'stroke-width':1.5
                },
 //               smooth:true,
                markerConfig: {
                    type: 'cross',
                    size: 0,
                    radius: 0,
                    'stroke-width': 0
                }
               }
               
               ]
            }
            


        });
       
        this.callParent(arguments);
        store_netflow.load();
    }
});
