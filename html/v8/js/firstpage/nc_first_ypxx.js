 var ttitle='';

     
 var store_first_yxzt=Ext.create('Ext.data.Store', 
{
        	pageSize: 40,   
        	id:'store_first_yxzt',   		 	
          fields: ['fsystem','fdb','fdata','cpu','mem','swap'],
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
   
 
   		      
Ext.define('firstpage.nc_first_ypxx', {

    extend: 'Ext.panel.Panel',
   requires: ['Ext.chart.*'],
    initComponent: function(){

        Ext.apply(this, {
          flex:1,
           layout: {
            type: 'fit'
  //          align: 'stretch'
        },
        width:300,
        height: 140,

         broder:false,
      items:[
      {
              layout: {type: 'hbox', align: 'stretch'},
              margin: '0 0 0 0',
               border: false,
            
        items: [
        {
            xtype: 'chart',
            style: 'background:#fff',            
            animate: {
                easing: 'bounceOut',
                duration: 500
            },
            store: store_first_yxzt,
            insetPadding: 25,
            flex: 1,
            axes: [{
                type: 'gauge',
                position: 'gauge',
                minimum: 0,
                maximum: 100,
                 title: '系统使用存储空间',
                steps: 10,
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'fsystem',
                donut: 80,
                colorSet: ['#F49D10', '#ddd']
            }]
        }, {
            xtype: 'chart',
            style: 'background:#fff',
         
            animate: true,
            store: store_first_yxzt,
            insetPadding: 25,
            flex: 1,
            axes: [{
                type: 'gauge',
                position: 'gauge',
                minimum: 0,
                maximum: 100,
                steps: 10,
                title:'数据库存储空间',
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'fdb',
                donut: 80,
                colorSet: ['#82B525', '#ddd']
            }]
        }, {
            xtype: 'chart',
            style: 'background:#fff',
            
            animate: {
                easing: 'bounceOut',
                duration: 500
            },
            store: store_first_yxzt,
            insetPadding: 25,
            flex: 1,
            axes: [{
                type: 'gauge',
                position: 'gauge',
                minimum: 0,
                maximum: 100,
                 title: '日志文件存储空间',
                steps: 10,
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'fdata',
                donut: 80,
                colorSet: ['#3AA8CB', '#ddd']
            }]
        }]
       }
       
       ] 


        });
       
        this.callParent(arguments);
        store_first_yxzt.load();
    }
});
