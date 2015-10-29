/**
 定义在线信息tab
 */
 
Ext.define('ncViewer.ncmOnlineInfo',{
    extend: 'Ext.tab.Panel',

    initComponent : function(){
        Ext.apply(this,{
        layout : 'border',
        id:'lay_online',
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
        activeTab: 0,        
        defaults :{
 
            bodyPadding: 0
 
        },
        items: [{
            title: '在线信息', 
            id:'dwz',           
            html: '<iframe  id="lay_system_rightrole" name="lay_system_rightrole" src="/ncmonline/ncm_webshowuseronline.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '规则设置',
            id:'smsruleset',            
            html: '<iframe  id="lay_system_adminuser" name="lay_system_adminuser" src="/ncmonline/ncm_webSmsRule_lab.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
        ]
          
        });
        this.callParent(arguments);
    }
})