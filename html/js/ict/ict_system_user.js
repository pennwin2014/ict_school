/**
 ¶¨Òå²ßÂÔtab
 */
 
 Ext.define('ncViewer.ict_system_user', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.ict_system_user',
    height:900,
    id:'system_user',
    autoscroll:false,  
    initComponent: function(){        
        var url;
        url = "/ict_manage/school/frontPage.html";      
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe id="ict_itm_system_user_s" name="ict_itm_system_user_s" src='+url+' scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>'     
        });
        this.callParent(arguments);
    }
});

