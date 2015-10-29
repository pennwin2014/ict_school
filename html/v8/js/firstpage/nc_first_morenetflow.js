 var ttitle='';

     
 var store_first_yxzt=Ext.create('Ext.data.Store', 
{
        	pageSize: 40,   
        	id:'store_first_yxzt',   		 	
          fields: ['fsystem','fdb','fdata'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_first_system_list.htm',
//            			url: '/pronline/Msg?FunName@ncWebOnlieUStop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}
 });
   
 
   		      
Ext.define('firstpage.nc_first_morenetflow', {

    extend: 'Ext.panel.Panel',
   uses: ['firstpage.nc_first_more_netflowline','firstpage.nc_first_morenetflow_grid'],
    initComponent: function(){

               Ext.apply(this, {
    //    	 flex:1,
   //       layout: 'fit',
             margin: '0 0 0 0',
                tbar:[
          {
             	text:'前一页',

             	 iconCls:'remove'
            },'-',{
             	text:'后一页',
             	iconCls:'add',
             	 handler:function(){
             	}
             },'-',
              {             
              	         labelWidth: 60,
                        	fieldLabel:'统计时间',
                        	xtype: 'datefield',
                        	format: "Y/m/d",
                        	width:160,
                        	value:'',
                        	name:'sdate'
              },      
             {xtype: 'numberfield',id:'firststep1',name: 'step' ,value:'10',width:40}
             ,
             
                          {     labelWidth: 80,         
                                width:          170,
                                xtype:          'combo',
                                id: 'wljkcombo1',
                                mode:           'local',
                                triggerAction:  'all',
                                forceSelection: true,
       //                         iconCls:'add',
                                editable:       false,
                                fieldLabel:     '选择网络接口',
                                	iconCls:'remove',
                                name:           'StartBack',
                                displayField:   'gname',
                                valueField:     'gid',
                                queryMode: 'local',
                                value : '',
                                store:store_devinteface
                            },
             
             
            {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;按</span>'},
                {                              
                                width:          50,
                                xtype:          'combo',
                                mode:           'local',
                                value:          '1',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                             
                                name:           'StartClean',
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store:          Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '分',   value: '1'},
                                        {name : '小时',  value: '2'},
                                        {name : '天',  value: '3'}
                                    ]
                                })
                            },
            {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;平均</span>'},
            
           
             	   ,{
                text:'确定',
                iconCls:'accept',
                handler: function(){
                }
              }
             	,'-' 
            ],
           
            layout: {              
                padding: '0 0 0 0'
            },
            items: [           
            {
              layout: {type: 'hbox'},
              margin: '0 0 0 0',
               border: false,

        	   items:[Ext.create('firstpage.nc_first_more_netflowline'),
        	      Ext.create('firstpage.nc_first_morenetflow_grid')
             ]
           } 
                     
               
          
          
           ]
                    
               
           
        });
        this.callParent(arguments);
    }
});
