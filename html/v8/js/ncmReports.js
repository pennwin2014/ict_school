/**
 ����ͳ�Ʊ���tab
 */
 
Ext.define('ncViewer.ncmReports',{
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
            title: '���ŷ��ͱ���', 
            id:'smsrepmenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncm_appreport/ncm_web_App_smsStatisticsReport.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '���ŷ�������',
            id:'smsdetail',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncm_appreport/ncm_web_App_smsDetailReport.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '�豸����ͳ��',
            id:'devicestat',            
            html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncm_appreport/ncm_web_App_intDeviceReport.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '����������������',
            id:'peopletraffic',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_appreport/ncm_web_App_intStatisticsReport.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '������ϸ����',
            id:'netdetail',            
            html: '<iframe  id="lay_rzck_waishelog" name="lay_rzck_waishelog" src="/ncm_appreport/ncm_web_App_intDetailReport.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})