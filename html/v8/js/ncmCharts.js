/**
 定义图表tab
 */
 
Ext.define('ncViewer.ncmCharts',{
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
            title: '设备品牌', 
            id:'shopinfomenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncm_chart/ncm_deviceBrandChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '设备类型',
            id:'shopbrandmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncm_chart/ncm_deviceTypeChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '操作系统',
            id:'shopdetaimenu',            
            html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncm_chart/ncm_web_MC_os_chart.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '短信数量',
            id:'brandmenu',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_chart/ncm_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})