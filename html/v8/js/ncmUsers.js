/**
 �����û�����tab
 */
 
Ext.define('ncViewer.ncmUsers',{
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
        items: [{
            title: '�û�����', 
            id:'shopinfomenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmm/ncm_web_showuser_man.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '�����',
            id:'shopbrandmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmgroup_s/ncm_group_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '����ע�����',
            id:'shopdetaimenu',            
            html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncmgroup_s/ncm_smslog_list.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})