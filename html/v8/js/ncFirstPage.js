/**定义实时监控tab*/
Ext.Loader.setConfig({enabled: true,paths:{'firstpage':'/v8/js/firstpage'}});


var store_display_firstpage=Ext.create('Ext.data.Store', 
{
        		pageSize: 40,  
        		id:'store_display_firstpage',      	
            fields: ['disp', 'title','index','classname','rindex'],
        		autoLoad:false,
        		proxy: 
        		{
            			type: 'ajax',
//            			url:'/data/nc_firstpage_display_list.htm',
         			url: '/pronline/Msg?FunName@nc_display_firstpage',
            			reader: {
                			type:'json',
                			root: 'eimdata',
                			totalProperty: 'totalCount'
            			},         
            			simpleSortMode: true
        		}
        		,
        listeners:{'load':function(){  
        	
       	var num,temp; 
        var pan1,pan2,pan3,pan4,pan5,pan6,pan7,pan8,pan9;
      	  store_display_firstpage.each(function(record){    
      	  num=record.get('index');
      	  if(num=='1'){
      	  	pan1=record.get('classname');
      	  }
      	  else if(num=='2'){
      	  	pan2=record.get('classname');
      	  }
      	  else if(num=='3'){
      	  	pan3=record.get('classname');
      	  }
      	  else if(num=='4'){
      	  	pan4=record.get('classname');
      	  }
      	  else if(num=='5'){
      	  	pan5=record.get('classname');
      	  }
      	  else if(num=='6'){
      	  	pan6=record.get('classname');
      	  }
      	  else if(num=='7'){
      	  	pan7=record.get('classname');
      	  }
      	  else if(num=='8'){
      	  	pan8=record.get('classname');
      	  }
      	  else if(num=='9'){
      	  	pan9=record.get('classname');
      	  }
      	  	
     	  	
        	  });
         
 
        	var tabs_center=Ext.getCmp("layout_center");

        	tabs_center.remove(0);
 	
        	 tabs_center.add({
            closable: false,
            autoScroll:true,            
            iconCls: 'add16',          
            title: '首页',
            items:[
            Ext.create('firstpage.nc_first_chart',{
            	pan1:pan1,
            	pan2:pan2,
            	pan3:pan3,
            	pan4:pan4,
            	pan5:pan5,
            	pan6:pan6,
            	pan7:pan7,
            	pan8:pan8,
            	pan9:pan9

            })
              ]
           }
 
          ).show(); 
       
        	}
        	}
 });





Ext.define('ncViewer.ncFirstPage',{
	  uses:['firstpage.nc_first_chart'],
    initComponent : function(){    
   	store_display_firstpage.load();
      this.callParent(arguments);
      
      
    }
})