/**
 ¶¨Òå²ßÂÔtab
 */
 
Ext.define('ncViewer.frontPage',{
    extend: 'Ext.panel.Panel',
    alias: 'widget.frontPage',
    height:900,
	width:parent.grid_width,
    autoscroll:false, 
    initComponent : function(){
        Ext.apply(this,{
			layout : 'border',
			id:'mac_frontpage',
			margins: '0 0 0 0',
			frame:false,        
			html: '<iframe  id="ict_frm_frontpage" name="ict_frm_frontpage" src="/ict_manage/school/frontPage.html" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 	  
        });
        this.callParent(arguments);
    }
});

