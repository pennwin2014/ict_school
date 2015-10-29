 var ttitle='';

     
 var store_first_topflowbar=Ext.create('Ext.data.Store', 
{
        	pageSize: 40,   
        	id:'store_first_topflowbar',   		 	
          fields: [{name:'username',mapping:'username'}, {name:'总流量',mapping:'totbytes'},{name:'进流量',mapping:'inbytes'},{name:'出流量',mapping:'outbytes'},{name:'index',mapping:'index'}],
        		autoLoad:true,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_first_topflow_list.htm',
//            			url: '/pronline/Msg?FunName@ncWebOnlieUStop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		},
        listeners:{'load':function(){
        	 maxfirstflow=0,temp_data=0;
        	var hidden1=true,hidden2=true;
        	  store_first_topflowbar.each(function(record){
               	
              temp_data=record.get('总流量'); if(temp_data>maxfirstflow){maxfirstflow=temp_data};
        	  });
     
        	}}  
 });
 
   		      
Ext.define('firstpage.nc_first_topflowbar', {

    extend: 'Ext.panel.Panel',
   requires: ['Ext.chart.*'],
    initComponent: function(){

        Ext.apply(this, {
          flex:1,
          layout: 'fit',
 //  forceFit: true,
 //         hidden:this.hidden,
 //         title:this.title,
        width:300,
            height: 310,
              tbar:[
             {                              
                                width:          170,
                                labelWidth: 80,
                                xtype:          'treecombox',
                                id: 'topflowgroupid_first',
                                value:          '0',
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
            {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;当前用户流量使用Top10(M)</span>'},
            '->'
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){
                	           var win = new Ext.Window ({
               
                             title:"上网用户实时流量监控",
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
                            Ext.create('ssjk.nc_online_use')
                             ]
                       }
                       
                  )
                   win.show();
                
                
              }
             } 
            ],
            
            items: {
                xtype: 'chart',
                animate: false,
                shadow: true,
                store:store_first_topflowbar,
                legend: {
       
                   position: 'bottom'
                }, 
                margin: '10 0 5 10',
               
                
                 axes: [{
			            type: 'Numeric',
			            position: 'left',
			            minimum: 0,
		              maximum: maxfirstflow,
			            fields: ['总流量','进流量','出流量'],
			            renderer: Ext.util.Format.numberRenderer('0,0'),
			            grid: true,			           
//		            title: 'SSSSSSfafdsaf',
			            minimum: 0
			   //         hidden: true
			        }, {
			            type: 'Category',
			            position: 'bottom',
			            fields: ['username'],
			            label: {
			                renderer: function(v) {
			           
			                    return Ext.String.ellipsis(v, 15, false);
			                },
			                font: '12px Arial',
			                rotate: {
			                    degrees: 0
			                }
			            }
			        }],
      //       theme: 'White',
                            
                 series: [{
                type: 'column',
                axis: 'left',
                 tips: {
                  trackMouse: true,
                  width: 140,
                  height: 65,
                  renderer: function(storeItem, item) {
                    this.setTitle(storeItem.get('username') + '<br>总流量:&nbsp;&nbsp;' + storeItem.get('总流量') + 'KB'+'<br>进流量:'+storeItem.get('进流量')+'KB'+'<br>进流量:'+storeItem.get('出流量')+'KB');
                  }
                  },
            xField: 'username',
            yField: ['总流量','进流量','出流量']
                

               }]
            }
            


        });
       
        this.callParent(arguments);
 //       store_swzs_service.load();
    }
});
