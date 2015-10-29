

Ext.Loader.setConfig({enabled: true,paths:{'ssjk':'/v8/js/ssjk'}});
Ext.Loader.setConfig({enabled: true,paths:{'ncViewer':'/v8/js'}});
Ext.define('ncViewer.App', {
    extend: 'Ext.container.Viewport',    
    uses:['ncViewer.ncGridUser','ncViewer.lanProcy','ncViewer.lanSsjk','ncViewer.ncRzck','ncViewer.lanComp','ncViewer.lanBase','ncViewer.lanProcyXf','ncViewer.lanSystem','ncViewer.lanSoftUpdate','ncViewer.lanDeptment','ncViewer.lanGroupTree','ncViewer.lanGroupCheckTree','ssjk.lan_ipaddress_set'],
    initComponent: function(){

        Ext.define('State', {
            extend: 'Ext.data.Model',
            fields: ['groupid', 'groupname']
        });       
        Ext.apply(this, {
            layout: {
                type: 'border',
                padding: 2
            },

            items: [this.createNcCenter()]
        });

        this.callParent(arguments);
    },

    createNcCenter:function(){

//   var charPort=Ext.create('widget.ncCharPoral');
      groupid='';
      compid='';
      dxtitle='';
    	this.centerPanel=Ext.createWidget('panel', {
 //       resizeTabs: true,
 //       enableTabScroll: true,
         autoScroll:false,
          region:'center',
        	id:'layout_center',
        	margins: '2 2 1 0',
        defaults: {
            autoScroll: false,
            bodyPadding: 0
        },
        items: [
        {
        	  closable: false,
            autoScroll:false,
            id:'ssjk',
 //           iconCls: 'add16',          
  //          title: '…œÕ¯’π æ',
            items:[Ext.create(ncViewer.ncSwzs)]
        }
        
        
 //       charPort
        ]

                
       });
       
       return this.centerPanel;
    }

 
});
