var queryWindow;
var queryFormPannel;
function showQueryWin(){
	
	/*
	//示例：创建一个combox，并绑定数据源
	//需要时使用
	var store_group = Ext.create('Ext.data.Store', {
		model: 'State',
		remoteSort: true,
		proxy: {
			type: 'ajax',
			url: '/pronline/Msg?FunName@ncsWebGroupComp',
			reader: {
				type:'json',
				root: 'corpclassdata'              
			},         
			simpleSortMode: true
		}
	});
	store_group.load();   
	  
	var groupCombo = Ext.create('Ext.form.field.ComboBox', {
		fieldLabel: '单位属组',
		valueField:'groupid',
		value:'',
		displayField: 'groupname',
		name : 'groupid1',      
		allowBlank: true,
		//        forceSelection: true,
		store: store_group,
		triggerAction: 'all',
		//      queryMode:'remote',
		queryMode: 'local',
		typeAhead: true
	});*/
	
	queryFormPannel=Ext.create('Ext.form.Panel', {         
		frame: true,
		fieldDefaults: {
			labelWidth: 65,                        
			xtype: 'textfield' 
		},
		bodyPadding: 10,
		items: [
			{                        
				fieldLabel:'MAC地址:',
				xtype: 'textfield',
				name:'macAddr'
			},{
				fieldLabel:'IP地址:',
				xtype: 'textfield',
				name:'ipAddr'
			},{
				xtype: 'button',
				anchor: 'right',
				style : 'margin-left:80px',
				text:'查&nbsp;&nbsp;询',
				handler: searchFun
			},{
			text: '重&nbsp;&nbsp;置',
			style : 'margin-left:10px',
			xtype: 'button',
			handler: function(){
					queryFormPannel.getForm().reset();
				}
			}
		]
	})


	
	

	queryWindow = Ext.widget('window', {
		title: '高级查询',
		closeAction: 'hide',
		x:400,
		y:32,
		width: 280,
		height: 330,
		minHeight: 330,
		//        layout: 'fit',
		resizable: true,
		modal: false,
		items: queryFormPannel
	});	
	queryWindow.hide();
}


function searchFun(){
	var ipAddr=queryFormPannel.getForm().getValues().ipAddr;
	var macAddr=queryFormPannel.getForm().getValues().macAddr;
	var reflash_param={type:'advance',ip:ipAddr,mac:macAddr};
	store.currentPage=1;
    reflash(reflash_param);
}