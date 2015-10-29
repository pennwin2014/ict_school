/**
 定义账号tab
 */
 
Ext.define('ncViewer.ncmAccounts',{
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
            title: '修改密码', 
            id:'shopinfomenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/pronline/Msg?FunName@ncmWebDispModiPass" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }]
        });
        this.callParent(arguments);
    }
})