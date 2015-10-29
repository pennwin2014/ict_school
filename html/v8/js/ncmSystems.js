/**
 定义系统管理tab
 */
 
Ext.define('ncViewer.ncmSystems',{
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
            title: '系统权限管理', 
            id:'rolemenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '系统用户管理',
            id:'xtyhmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '用户访问日志',
            id:'usvisitmenu',            
            html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})