/**
 豫园分析统计
 */
Ext.define('analysis.ncmAnalysis',{
    extend: 'Ext.container.Viewport',
    autoScroll: false,
    frame: false,
    initComponent : function(){
        Ext.apply(this,{
				    layout : 'border',
				    id:'analysis_menu',
				    forceFit: true,
				    //margins: '0 0 0 0',
						//frames:true,
				    //activeTab: 0,        
				    defaults :{ 
				        bodyPadding: 0
				    },
				    items: [{
		            region: 'west',
		            collapsible: true,
		            title: '左侧导航',
		            xtype: 'panel',
		            width: 200,
		            autoScroll: true
		        }, {
		            region: 'center',
		            xtype: 'tabpanel',
		            activeItem: 0,
		            items: {
		                title: '首页',
		                html: '这里是首页正文内容'
		            }
		        }]
        });
        this.callParent(arguments);
    }
})
