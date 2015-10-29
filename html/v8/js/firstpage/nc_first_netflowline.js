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
   		 
    		      
Ext.define('firstpage.nc_first_netflowline', {

    extend: 'Ext.panel.Panel',
    uses: ['firstpage.nc_first_morenetflow'],
    requires: ['Ext.chart.*'],
    initComponent: function(){

        Ext.apply(this, {
          flex:1,
          layout: 'fit',
           tbar:[
             {                  labelWidth: 80,         
                                width:          170,
                                xtype:          'combo',
                                id: 'wljkcombo',
                                mode:           'local',
                                triggerAction:  'all',
                                forceSelection: true,
       //                         iconCls:'add',
                                editable:       false,
                                fieldLabel:     '选择网络接口',
                                	iconCls:'remove',
                                name:           'StartBack',
                                displayField:   'gname',
                                valueField:     'gid',
                                queryMode: 'local',
                                value : '',
                                store:store_devinteface
                            },
           
          '-',
          {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;网络接口速率图</span>'},
            '->',
         
          
             	
             	  {xtype:'label', html:'<span id="netflow_rate">当前速率:<font color=red>120kbps&nbsp;</font></span>'}
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){

                	 var win = new Ext.Window ({
               
                             title:"网络接口速率趋势图(Kbps)",
                             x:30,
                             y:50,
                             width:1130,
                             height:500,
                             bodyPadding: 10,
                             modal:true,
  //                         headerPosition: 'bottom',
                             maximizable:true ,
                             resizable:true,
                             draggable:true,
                              items:[                             
                             Ext.create('firstpage.nc_first_morenetflow')
                             ]
                                               
         }
        )
        win.show();

                }
              }
             	  
            ],
            height: 310,
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
