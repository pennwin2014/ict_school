/**
 定义系统设置tab
 */
 
Ext.define('ncViewer.ncmSettings',{
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
            title: 'Portal参数',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmset/ncm_portalnext_paraset.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
        }
        
        ,{          
            title: '短信服务器',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmset/ncm_smsnext_set.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'    
         }
         ,{          
            title: 'HTTP服务器',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmset/ncm_httpnext_set.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
        }
       
        ,{          
            title: 'AC设置',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmset/ncm_acnext_set.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
        ,{          
            title: 'Radius设置',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmset/ncm_radiusnext_set.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
        ,
        {          
            title: '网络设置',
            html: '<iframe  id="dhIframe" name="gdrzIframe" src="/ncmsysman/ncm_net_interface.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
        }]
        });
        this.callParent(arguments);
    }
})