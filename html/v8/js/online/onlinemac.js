﻿/*在线信息*/
	Ext.define('online.onlinemac',{
    extend: 'Ext.grid.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
		
	if(store_online_onlinemac.alreadyload !=1){
		store_online_onlinemac.on_tMac='';
		store_online_onlinemac.on_uMac='';
		store_online_onlinemac.on_bSignal='';
		store_online_onlinemac.on_sSignal='';
		store_online_onlinemac.on_noise='';
		store_online_onlinemac.on_channel='';
		store_online_onlinemac.on_stime='';
		store_online_onlinemac.on_etime='';
		store_online_onlinemac.cond='';
		store_online_onlinemac.save='';
		store_online_onlinemac.del='';
		store_online_onlinemac.tr_gname='';
		store_online_onlinemac.flow_flag='';
		store_online_onlinemac.auth_flag='';
		store_online_onlinemac.new_params={on_tMac:store_online_onlinemac.on_tMac,on_uMac:store_online_onlinemac.on_uMac,on_bSignal:store_online_onlinemac.on_bSignal,on_sSignal:store_online_onlinemac.on_sSignal,on_noise:store_online_onlinemac.on_noise,on_channel:store_online_onlinemac.on_channel,on_stime:store_online_onlinemac.on_stime,on_etime:store_online_onlinemac.on_etime,cond:store_online_onlinemac.cond,save:store_online_onlinemac.save,del:store_online_onlinemac.del,flow_flag:store_online_onlinemac.flow_flag,auth_flag:store_online_onlinemac.auth_flag};  
	}
 function reflash(){
	store_online_onlinemac.new_params={on_tMac:store_online_onlinemac.on_tMac,on_uMac:store_online_onlinemac.on_uMac,on_bSignal:store_online_onlinemac.on_bSignal,on_sSignal:store_online_onlinemac.on_sSignal,on_noise:store_online_onlinemac.on_noise,on_channel:store_online_onlinemac.on_channel,on_stime:store_online_onlinemac.on_stime,on_etime:store_online_onlinemac.on_etime,cond:store_online_onlinemac.cond,save:store_online_onlinemac.save,del:store_online_onlinemac.del,flow_flag:store_online_onlinemac.flow_flag,auth_flag:store_online_onlinemac.auth_flag};  
	store_online_onlinemac.currentPage=1;
	store_online_onlinemac.load();
}

var sm = Ext.create('Ext.selection.CheckboxModel',{
    listeners: {
        selectionchange: function(sm, selections) {
            Ext.getCmp('onlineInfo_removeButton').setDisabled(selections.length == 0);
        }
    }
});


		//加载流量
	  var timeFlagStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'有流量', groupid:'1'},
	  				{groupname:'无流量', groupid:'2'}
	  		]
		});
    var timeFlagCombo = Ext.create('Ext.form.field.ComboBox', {
		id:'onlineinfo_flow_flag',
		fieldLabel: '流量',
        labelWidth: 30,
        width: 130,
        valueField:'groupid',
		emptyText:'请选择',
        value:store_online_onlineinfo.flow_flag,
        displayField: 'groupname',
        name : 'flow_flag',      
        allowBlank: true,
        store: timeFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
					store_online_onlineinfo.flow_flag=this.value;
					reflash();
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		});  	
    
    //是否认证
	  var authFlagStore = Ext.create('Ext.data.Store', {
        fields:['groupname', 'groupid'],
	  		data:[
	  				{groupname:'已认证', groupid:'1'},
	  				{groupname:'全部'  , groupid:'0'},
	  				{groupname:'未认证', groupid:'2'},
	  				{groupname:'退出认证', groupid:'9'}
	  		]
		});
    var authFlagCombo = Ext.create('Ext.form.field.ComboBox', {
		    id:'onlineinfo_auth_flag',
		    fieldLabel: '用户状态',
        labelWidth: 60,
        width: 140,
        valueField:'groupid',
		    emptyText:'已认证',
        value:store_online_onlineinfo.auth_flag,
        displayField: 'groupname',
        name : 'auth_flag',      
        allowBlank: true,
        store: authFlagStore,
        triggerAction: 'all',
        queryMode: 'local',
        typeAhead: true,
        listeners: {
	          'change':function(){
							store_online_onlineinfo.auth_flag=this.value;
							reflash();
							if(store_online_onlineinfo.auth_flag != 1)
								Ext.getCmp('onlineInfo_removeButton').setVisible(false);
							else
							  Ext.getCmp('onlineInfo_removeButton').setVisible(true);
            }
        },
				listConfig:{
      			loadingText: 'Searching...',
      			emptyText: 'No matching found.'
        }
		});  		
		
    Ext.apply(this,       
        {   
        //scrollOffset: 0,
        border:false,
		frame:false,
		//width: Ext.get("layout_center").getWidth()-5, 
		height: Ext.get("layout_center").getHeight()-63,
		//height:Ext.get("layout_center").getHeight(),
		autoScroll: true,
		selModel: sm,
		//height: grid_height,
        store: store_online_onlinemac,
        viewConfig:{
	        	loadMask : false
	      },
        columns:[{
            id: 'on_tMac',
            text: "终端Mac",
            dataIndex: 'on_tMac',
            width: 120,
            sortable: true
        },{
            id: 'on_uMac',
            text: "ApMac",
            dataIndex: 'on_uMac',
            width: 120,
            sortable: true
        },{
            text: "AP位置",
            dataIndex: 'on_apAddr',           
            width: 120,
 //           flex: 1,
            sortable: true
//            renderer: ipgj
        },{
            text: "最大信号强度",
            dataIndex: 'on_bSignal', 
           width: 100,
           align: 'right',
            sortable: false
        },{
            text: "最小信号强度",
            dataIndex: 'on_sSignal',           
            width: 100,
 //           flex: 1,
            align: 'right',
            sortable: false
//            renderer: ipgj
        },{
            text: "噪底",
            dataIndex: 'on_noise',
            hidden: false,
            width: 117,
            align: 'right',
            sortable: false
        },{
            text: "信道",
            dataIndex: 'on_channel',
            hidden: false,
            width: 70,
            align: 'center',
            sortable: false
        },{
            text: "开始时间",
            dataIndex: 'on_stime',
            hidden: false,
            width: 140,
            sortable: false
        },{
            text: "最后捕获时间",
            dataIndex: 'on_etime',
            hidden: false,
            width: 140,
            sortable: false
        },{
           flex:1,
           sortable: false
        }],
          
           columnLines: true,
           tbar:{items:[
  //          '-', 
            //timeFlagCombo,
//             authFlagCombo,
            '-',{
            	labelWidth: 45,
            	width:200,
                fieldLabel:'关键字',
                xtype: 'textfield',
                id:'onlinemac_cond',
                name:'onlinemac_cond',
				emptyText:'请输入终端Mac/APMac',
                value:store_online_onlinemac.cond,
                enableKeyEvents:true,
                listeners:{
	                'focus':function(){
			                 if(this.value=='请输入终端Mac/APMac'){                
			                    this.setValue('');
			                }
					},
	                'keydown' : function(i,e){
	                    var aae=e.getKey(); 
	                    if(aae==13){
	                    		var aa=Ext.getCmp('onlinemac_cond').value;
			                    if(aa!='请输入终端Mac/APMac'){
					       	            store_online_onlinemac.cond=aa;
							            store_online_onlinemac.currentPage=1;
							            reflash();
					                }
	                    }
	                }
                }
            }, '-', {
                text:'查询',
            	  iconCls:'search2',
                handler:function(){
	                 var bb=Ext.getCmp('onlinemac_cond').value;     
		               store_online_onlinemac.cond=bb;
		               store_online_onlinemac.currentPage=1;         
		               reflash();  
                }
            }
//            , '-', {
//                text:'强制下线',
//                id:'onlinemac_removeButton',
//                iconCls:'remove',
//                disabled: true,
//                handler:function(){
//							  if(confirm('您真的要执行删除操作吗？')){                  
//			                var selsid;  
//			                var rows=sm.getSelection();
//			                selsid='';
//			                for(var i=0;i<rows.length;i++){
//			                  	if(i==0){
//			                  	   selsid = rows[i].get('on_name') ;
//			                  	}
//			                  	else{
//			                       selsid = selsid + "," + rows[i].get('on_name') ;
//			                    }
//			                }
//											store_online_onlinemac.load({params: {del: "del",selsid:selsid}});
//                   }
//                }
//            }
          ]
         }
        }
        
        );
		store_online_onlinemac.alreadyload = 1;
		store_online_onlinemac.alreadyadd = 1;
		//store_online_onlinemac.load();
		//setInterval("store_online_onlinemac.load()",20000);
		this.callParent(arguments);
    }
})