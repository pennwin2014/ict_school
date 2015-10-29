 var ttitle='';

 Ext.Loader.setConfig({enabled: true,paths:{'swzs':'/v8/js/swzs'}}); 
  //服务饼图
var store_first_service=Ext.create('Ext.data.Store', 
 {
        		pageSize: 40, 
        		id:'store_first_service',       	
            fields: ['sname', 'data1','index','svname'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_first_service_list.htm',
//         			url: '/pronline/Msg?FunName@ncWebOnlieServiceUtop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}

   });       
  var store_serviceclass= Ext.create('Ext.data.Store', {
                                    fields : ['gname', 'gid'],
                                    autoLoad: true,
                                    proxy: 
												        		{
												           			type: 'ajax',
												            			url:'/data/nc_first_servicegroup_list.htm',
												//          			url: '/pronline/Msg?FunName@ncWebShowZsweb_v8',
												            			reader: {
												                			type:'json',
												                			root: 'eimdata',
												                			totalProperty: 'totalCount'
												            			},         
												            			simpleSortMode: true
												        		}
                                });     
    
 

//服务饼图
var store_first_moreservice=Ext.create('Ext.data.Store', 
       		{
        		pageSize: 40,        	
            fields: ['sname', 'data1','index','svname'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
            			url:'/data/nc_swzs_service_list.htm',
//         			url: '/pronline/Msg?FunName@ncWebOnlieServiceUtop_v8',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}
        		,
        listeners:{'load':function(){
        	var data1=[],data2=[],data3=[],data4=[],data5=[],data6=[],data7=[],data8=[],data9=[];	
        	var titlle1='',title2='',title3='',title4='',title5='',title6='',title7='',title8='',title9='';
        	var hidden1=true,hidden2=true,hidden3=true,hidden4=true,hidden5=true,hidden6=true,hidden7=true,hidden8=true,hidden9=true;
        	  store_first_moreservice.each(function(record){
               	
        	if(record.get('index')=='1'){title1=record.get('svname'); data1.push(record);hidden1=false;}
	        	else if(record.get('index')=='2'){data2.push(record);title2=record.get('svname');hidden2=false;}
	        	else if(record.get('index')=='3'){data3.push(record);title3=record.get('svname');hidden3=false;}
	        	else if(record.get('index')=='4'){data4.push(record);title4=record.get('svname');hidden4=false;}
	        	else if(record.get('index')=='5'){data5.push(record);title5=record.get('svname');hidden5=false;}
	        	else if(record.get('index')=='6'){data6.push(record);title6=record.get('svname');hidden6=false;}
	        	else if(record.get('index')=='7'){data7.push(record);title7=record.get('svname');hidden7=false;}
	        	else if(record.get('index')=='8'){data8.push(record);title8=record.get('svname');hidden8=false;}
	        	else if(record.get('index')=='9'){data9.push(record);title9=record.get('svname');hidden9=false;}
	        	
        	  });
        
  			
        var store1 = Ext.create('Ext.data.JsonStore', { fields: ['sname', 'data1','index','svname'],data: data1}); 
        var store2 = Ext.create('Ext.data.JsonStore', {fields: ['sname', 'data1','index','svname'],data: data2}); 	    
        var store3 = Ext.create('Ext.data.JsonStore', { fields: ['sname', 'data1','index','svname'],data: data3}); 
        var store4 = Ext.create('Ext.data.JsonStore', {fields: ['sname', 'data1','index','svname'],data: data4});     
        var store5 = Ext.create('Ext.data.JsonStore', { fields: ['sname', 'data1','index','svname'],data: data5}); 
        var store6 = Ext.create('Ext.data.JsonStore', {fields: ['sname', 'data1','index','svname'],data: data6}); 	    
        var store7 = Ext.create('Ext.data.JsonStore', { fields: ['sname', 'data1','index','svname'],data: data7}); 
        var store8 = Ext.create('Ext.data.JsonStore', {fields: ['sname', 'data1','index','svname'],data: data8});  
        var store9 = Ext.create('Ext.data.JsonStore', {fields: ['sname', 'data1','index','svname'],data: data9});
  

        new Ext.Window ({              
                             title:"网络服务Top9用户使用情况分布图",
                             x:100,
                             y:50,
                             width:1010,
                             height:640,
                             bodyPadding: 10,
                             modal:true,
  //                         headerPosition: 'bottom',
                             maximizable:true ,
                             resizable:true,
                             draggable:true,
                             items:[                             
                             Ext.create('swzs.nc_sszs_service',{
            	stitle1:title1,
              store1:store1,
              hidden1:hidden1,
              stitle2:title2,
              store2:store2,
              hidden2:hidden2,
              stitle3:title3,
              store3:store3,
              hidden3:hidden3,
              stitle4:title4,
              store4:store4,
              hidden4:hidden4,
              stitle5:title5,
              store5:store5,
              hidden5:hidden5,
              stitle6:title6,
              store6:store6,
              hidden6:hidden6,
              stitle7:title7,
              store7:store7,
              hidden7:hidden7,
              stitle8:title8,
              store8:store8,
              hidden8:hidden8,
              stitle9:title9,
              store9:store9,
              hidden9:hidden9
              })
                             
                             ]
                           });
win.show();


       	
        
        	}}
    		});   
    
    
      
        
Ext.define('firstpage.nc_first_servicepie', {

    extend: 'Ext.panel.Panel',
   requires: ['Ext.chart.*'],
    initComponent: function(){
     
 //   store_first_service.load();

    
    
        Ext.apply(this, {
          flex:1,
          layout: 'fit',
//  forceFit: true,
 //         hidden:this.hidden,
//          title:this.title,
            width:300,
            height: 310,            
             tbar:[
             {                              
                                width:          170,
                                labelWidth: 80,
                                xtype:          'combo',
                                id: 'serviceclass_first',
                                mode:           'remote',
                             
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                                fieldLabel:     '选择统计类别',
                                iconCls:'remove',
                                name:           'serviceclass_first',
                                displayField:   'gname',
                                valueField:     'gid',
                                queryMode: 'local',
                                value :'',
                                store:store_serviceclass
                            },
           
            '-',
             {xtype:'label', html:'<span class="x-panel-header-text x-panel-header-text-default">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;网络服务使用情况分布图</span>'},
            '->'
             	   ,{
                text:'更多信息',
                iconCls:'add',
                handler: function(){
                	store_first_moreservice.load();
                	     
        
                }
              }
             	  
            ],
            
            
            items: {
                xtype: 'chart',
                animate: true,
                shadow: true,
                store:store_first_service,
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

     //              ,
      //              style:{
      //              	font:'9pt'
     //             }
                }
               }]
            }
            


        });
       
        this.callParent(arguments);
 //       store_swzs_service.load();
    }
});
