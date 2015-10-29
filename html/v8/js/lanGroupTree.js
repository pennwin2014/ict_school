/**
 定义部门树
 */
 
 Ext.define('ncViewer.lanGroupTree', {
    extend: 'Ext.tree.Panel',
    alias: 'widget.lanGroupTree',
   
    change: function(val) {
        if (val > 0) {
            return '<span style="color:green;">' + val + '</span>';
        } else if (val < 0) {
            return '<span style="color:red;">' + val + '</span>';
        }
        return val;
    },
    initComponent: function(){
        	var cstate='';
    	    Ext.define('treedata',{
        extend: 'Ext.data.Model',
        fields: [
            {name: 'text',     type: 'string'},
            {name: 'id',     type: 'string'},
            {name: 'userid', type: 'string'},
            {name:'ctext',   type:'string'}

        ]
       });

    
        var compstate = [
        {"groupid":"","groupname":"状态"},
        {"groupid":"1","groupname":"在线"},
        {"groupid":"2","groupname":"离线"}
     
    ];
    
    var store_compstate = Ext.create('Ext.data.Store', {
        model: 'State',
        data: compstate
    });
    var compstateCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '',
         labelWidth: 50,
         width: 50,
        valueField:'groupid',
        displayField: 'groupname',
        name : 'compstate',      
        value:'',
        id:'compstate',
        allowBlank: true,
        forceSelection: false,
        store: store_compstate,
        queryMode: 'local',
        typeAhead: true,
        listeners: {
			          'change':function(){			     
			        	  var vstate=Ext.getCmp('compstate').value;	
			        	  var vkey=Ext.getCmp('keyword_usertree').value;
			       			   store_usertree.new_params={keyword:vkey,state:vstate};
			          	  store_usertree.load();
			            }
			        }
    });
    	
   



    Ext.apply(this, {
 //       title: '部门电脑',
    	  margins: '0 0 0 0',
//  	  id:'tree_comp',
        store: store_usertree,
        autoScroll: true,
         border:false,
      forceFit: true,
   //     width: 110,
        useArrows: false,
            dockedItems: [{
            xtype: 'toolbar',
            items: [compstateCombo,
            {
               xtype:'textfield',
               id:'keyword_usertree',
               name:'keyword',
               style:'color:#7aa7d5',
               width:120,
               value:'',
               emptyText: '姓名/IP/MAC/证件号',
               enableKeyEvents:true,
               listeners:{                
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){
			        	    var vstate=Ext.getCmp('compstate').value;	
			        	    var vkey=Ext.getCmp('keyword_usertree').value;
			       			  store_usertree.new_params={keyword:vkey,state:vstate};
			          	  store_usertree.load();

		                
                }
               }
               }
             },
             
              {
                text:'',
                itemId: 'moveButton',
                iconCls:'accept',
 //               disabled: true,
                handler:function(){
        
			        	    var vstate=Ext.getCmp('compstate').value;	
			        	    var vkey=Ext.getCmp('keyword_usertree').value;
			       			  store_usertree.new_params={keyword:vkey,state:vstate};
			          	  store_usertree.load();
						         
   
                }
 
                
            }
             
             
             
             ]}]
              ,        
         listeners:{itemclick:function(view, rcd, item, idx, event, eOpts){
        	groupid=rcd.get('id');        
        	userid=rcd.get('userid');
        	if(groupid!=''){
        	dxtitle='部门：'+rcd.get('text');
        	
          }
          if(userid!=''){
          	dxtitle='用户：'+rcd.get('text');
          
          }
          if(groupid==''&&userid==''){
          	dxtitle='';
          
          }
          
        	var tabs_center=Ext.getCmp("layout_center");
   
        	var active = tabs_center.getActiveTab(); 
        
        	var active_id='lay_'+active.id;
        	
        	var tabs_if=Ext.getCmp(active_id).getActiveTab();
        
        	eval(tabs_if.classname).setPage(1);
        	eval(tabs_if.classname).reflsh();
          eval(tabs_if.classname).setTitle(dxtitle);
   
         }
        } 
   
   
   
   
   
        });

        this.callParent(arguments);
    }
});