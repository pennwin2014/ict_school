/**
 定义日志tab
 */
 
Ext.define('ncViewer.ncmLogs',{
    extend: 'Ext.tab.Panel',

    initComponent : function(){
        
        Ext.apply(this,{
        layout : 'border',
        id:'lay_rzck',
        forceFit: true,
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
 //       activeTab: 0,        
         defaults :{ 
            bodyPadding: 0
        },
        items: [
		{
            title: '短信日志',
			id:'smslog', 
            html: '<iframe  id="ncm_logs_smslog" name="ncm_logs_smslog" src="/ncm_webplacelimit/ncm_web_Place_smslog_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
           
        }
        ,
         {
            title: '用户日志',
			id:'userlog', 
            html: '<iframe  id="ncm_logs_userlog" name="ncm_logs_userlog" src="/ncm_webplacelimit/ncm_web_Place_userlog_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
           
        },
        {
            title: '广告日志',
			id:'adlog', 
            html: '<iframe  id="ncm_logs_adlog" name="ncm_logs_adlog" src="/ncm_advertisement/ncm_web_Place_advertisement_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
           
        }]
        });
        this.callParent(arguments);
    }
})