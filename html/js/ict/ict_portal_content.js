/**
 ¶¨Òå²ßÂÔtab
 */
 
 Ext.define('ncViewer.ict_portal_content', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.ict_portal_content',
    height:900,
    id:'portal_content',
    autoscroll:false,  
    initComponent: function(){        
        var url;
        url = "/ict_manage/school/frontPage.html";      
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe id="ict_itm_portal_content_s" name="ict_itm_portal_content_s" src='+url+' scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>'     
        });
        this.callParent(arguments);
    }
});

