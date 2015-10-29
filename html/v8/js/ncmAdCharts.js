/**
 定义图表tab
 */
 
Ext.define('ncViewer.ncmAdCharts',{
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
            title: '实时浏览次数趋势图',
            id:'realtimevisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_onlineUserRealtimeTrend.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '广告点击',
            id:'advisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_adVisit.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '时间段访问',
            id:'timegroupvisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '终端品牌', 
            id:'shopinfomenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncm_adchart/ncm_ad_deviceBrandChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },
        {
            title: '终端类型',
            id:'shopbrandmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncm_adchart/ncm_ad_deviceTypeChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})