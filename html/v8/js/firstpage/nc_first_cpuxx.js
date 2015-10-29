 var ttitle='';


   		      
Ext.define('firstpage.nc_first_cpuxx', {

    extend: 'Ext.panel.Panel',
   requires: ['Ext.chart.*'],
    initComponent: function(){


        Ext.apply(this, {
        flex:1,
           layout: {
            type: 'fit'
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
                 title: 'CPU使用情况',
                steps: 10,
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'cpu',
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
                title:'内存使用情况',
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'mem',
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
                 title: '交换分区使用情况',
                steps: 10,
                margin: 7
            }],
            series: [{
                type: 'gauge',
                field: 'swap',
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
