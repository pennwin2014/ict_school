
  //服务饼图
var store_first_httppie=Ext.create('Ext.data.Store', 
 {
        		pageSize: 40, 
        		id:'store_first_httppie',       	
            fields: ['sname', 'data1','index','svname'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_first_httppie_list.htm',
//         			url: '/pronline/Msg?FunName@ncWebOnlieServiceUtop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}

   }); 

Ext.define('firstpage.nc_first_httppie', {

    extend: 'Ext.panel.Panel',
   requires: ['Ext.chart.*'],
    initComponent: function(){
     
    store_first_httppie.load();

    
    
        Ext.apply(this, {
          flex:1,
          layout: 'fit',
//  forceFit: true,
          hidden:this.hidden,
//          title:this.title,
            width:300,
            height: 310,            
             tbar:[
             {                              
                                width:          170,
                                labelWidth: 80,
                                xtype:          'treecombox',
                                id: 'httppiegroupid_first',
                                mode:           'remote',
                             
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                                fieldLabel:     '选择部门',
                                iconCls:'remove',
                                name:           'serviceclass_first',
                                displayField:   'text',
                                valueField:     'id',
                                queryMode: 'local',
                                value :'',
                                store:store_classtree
                            },
           
            '-',
             {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;网站类别访问分布图</span>'},
            '->'
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){
                	store_first_moreservice.load();
                	
/*
                	 var win = new Ext.Window ({
               
                             title:"增加或修改单位信息",
                             x:100,
                             y:100,
                             width:670,
                             height:600,
                             bodyPadding: 10,
                             modal:true,
  //                         headerPosition: 'bottom',
                             maximizable:true ,
                             resizable:true,
                             draggable:true,
                             html:'htmlvar'
                                               
         }
        )
        win.show();
   */      
        
                }
              }
             	  
            ],
            
            
            items: {
                xtype: 'chart',
                animate: true,
                shadow: true,
                store:store_first_httppie,
                legend: {
       
                   position: 'right'
                }, 
                insetPadding: 15,
                theme: 'Base:gradients',               
                 series: [{
                type: 'pie',
                field: 'data1',
                showInLegend: true,

               
                label: {
                    field: 'sname',
                    display: 'rotate',
                    contrast: true
                }
               }]
            }
            


        });
       
        this.callParent(arguments);
 //       store_swzs_service.load();
    }
});