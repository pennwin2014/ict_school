/**
 定义单位树
 */
 
 Ext.define('proauthRzAuthlog.view.list.facGroupTree', {
    extend: 'Ext.tree.Panel',
    alias: 'widget.facGroupTree',
   
    change: function(val) {
        if (val > 0) {
            return '<span style="color:green;">' + val + '</span>';
        } else if (val < 0) {
            return '<span style="color:red;">' + val + '</span>';
        }
        return val;
    },
    initComponent: function(){
       Ext.define('treedata', {
        extend: 'Ext.data.Model',
        fields: [
            {name: 'text',     type: 'string'},
            {name: 'id',     type: 'string'},
            {name:'fcode',   type:'string'},
            {name:'servicecode',type:'string'},
            {name: 'did', type: 'string'}
 
        ]
      });

	    store_fac = Ext.create('Ext.data.TreeStore', {
      model: 'treedata',
        proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsTreeUser_corp'
        },
        root: {
            text: '全部',
            id: '',
            expanded: true
        },
        folderSort: true,
        sorters: [{
            property: 'text',
           
            direction: 'ASC'
        }]
    });
  
  

    Ext.apply(this, {
    	  margins: '0 0 0 0',
    	  id:'flgroupTree',
        store: store_fac,
        autoScroll: true,
         height:parent.grid_height,
         border:true,
        forceFit: true,
        rootVisible: true,
   //     width: 110,
        useArrows: false,
        dockedItems: [{
            xtype: 'toolbar',
            items: [
            {
               xtype:'textfield',
               id:'keyword_factree',
               name:'keyword',
               style:'color:#7aa7d5',
               width:140,
               emptyText:'单位名称/代码',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
                 if(this.value==''){                
                    this.setValue('');
                  }
                 },
         
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){
                    var aa=Ext.getCmp('keyword_factree').value;
                    if(aa!=''){
                    	  var vkeyword=Ext.getCmp('keyword_factree').value;			         
						          	store_fac.load({params: {keyword:vkeyword}});

		                   }
                }
               }
               }
             },
             
              {
                text:'查询',
 //               itemId: 'moveButton',
                iconCls:'accept',
 //               disabled: true,
 //								id:"treeBtn",

                listeners: {
                	'click': function(){
                		var aa=Ext.getCmp('keyword_factree').value;
                    var vkeyword=Ext.getCmp('keyword_factree').value;		
         
				          	store_fac.load({params: {keyword:vkeyword}});
                	}
                }
 
                
            }
             
             
             
             ]}]
              ,        
        listeners:{
         	itemclick:function(view, rcd, item, idx, event, eOpts){
         	compidTmp = "";
         	compNameTmp = "";
         	
        	
          
         
          try{
	         document.getElementById('form1').did.value=rcd.get('did');
	         proauthRzAuthlog.controller.Procy.SetPage(1);  
           proauthRzAuthlog.controller.Procy.loadProcyListStore();
	          
				}catch(e){
					
				}
         }
   			}
   
   
        });

        this.callParent(arguments);
    }
});