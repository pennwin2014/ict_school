//�������ݴ洢
var store;
//ˢ�����ݴ洢ʱ��Ĭ�ϲ�����object����
var default_reflash_param={type:"init"};
function showPagingGrid(){
	//Ext.tip.QuickTipManager.init();
	//    Ext.tip.QuickTipManager.init();

	Ext.define('GridData', {
		extend: 'Ext.data.Model',
		//�������ֶ�
		fields: [
			'usermac','userip','ssid','manufacture','logcontent'
		],
		idProperty: ''
	});


	//��д�����������Զ��������ѯ����
	Ext.data.Store.prototype.doSort = function() {
		reflash(default_reflash_param);
	}
	
	//����Grid�����ݴ洢
	store = Ext.create('Ext.data.Store', {
		pageSize: 25,
		model: 'GridData',
		remoteSort: true,
		proxy: {
			type: 'ajax',
			url: '/pronline/Msg?FunName@ncwlan_monitor_log',
			reader: {
				type:'json',
				root: 'topics',
				totalProperty: 'totalCount'
			},         
			simpleSortMode: true
		},
		sorters: [{
			property: 'usermac',
			direction: 'DESC'
		}]
	});
	
	//��ʱд��
	var combo_data=[['1','��ѡ��'],['2','AP��־�澯'],['3','AC��־�澯'],['4','�ն���־�澯'],['5','������־�澯']];	
	
	var store_group=new Ext.data.SimpleStore({
		fields:['value','text'],
		data:combo_data
	});
	/*
	//ʾ��������һ��combox����������Դ
	//��Ҫʱʹ��
	var store_group = Ext.create('Ext.data.Store', {
		model: 'State',
		remoteSort: true,
		proxy: {
			type: 'ajax',
			url: '/pronline/Msg?FunName@ncwlan_monitor_combox',
			         
			simpleSortMode: true
		},
		reader: new Ext.data.ArrayReader({},[
			{name:'value'},
			{name:'text'}
		])
	});
	store_group.load();  */ 
	  
	var groupCombo = Ext.create('Ext.form.field.ComboBox', {
		fieldLabel: '',
		valueField:'value',
		//value:'',
		displayField: 'text',
		name : 'groupid1',      
		allowBlank: true,
		emptyText:'��ѡ��',
		store: store_group,
		triggerAction: 'all',
		queryMode: 'local',
		typeAhead: true,
		mode:'local'
	});
	
	groupCombo.on('select',function(combox){
		var param={type:"select",select:combox.getValue()};
		reflash(param);	
	});

	//render ���� �����Զ���Ҳ���Բ���
	function editcorp(value,p,record){
		return Ext.String.format('<a href="#xxm" onclick="gotoedit1({0})">�޸�</a>',record.getId());
	}
	function ipgj(value,p,record){
		var temp_disp=record.data.wlan_Bssid;
		return Ext.String.format( '<a href="#xxm" onclick=gotoIP("{0}","{1}")>'+temp_disp+'</a>',record.getId(),temp_disp); 
	}
  
	//������ƶ�����ǰ��Ԫ����ʱ ��ʾע�ͣ����ҵ�ǰ�������ɫΪ#FF3322
  function tooltips(value,cellmeta,record,rowIndex,columnIndex,store)
  {
  		  //cellmeta.attr = 'style=color:#FF0000'+' ext:qtip="'+value+'"';
  		  //return value;
  		  
  		var myAttr = '<span title="'+value+'"><font color="#EE4445">'+value+'</font></span>';
		  return myAttr;		
  }
	
	//var pluginExpanded = true;
	var sm = Ext.create('Ext.selection.CheckboxModel',{
		listeners: {
			selectionchange: function(sm, selections) {
				//��û������ʱdisable removeButton��moveButton
				grid.down('#removeButton').setDisabled(selections.length == 0);
				grid.down('#moveButton').setDisabled(selections.length == 0);
			}
		}
	});	

	//�Է�ҳ���������¹������滻��ҳ����    
	Ext.PagingToolbar_dir = Ext.extend(Ext.PagingToolbar, {
		doRefresh:function(){  
			reflash();
		},
		moveLast:function(){
			var b=this,a=b.getPageData().pageCount;
			if(b.fireEvent("beforechange",b,a)!==false){
				store.currentPage=a;
				reflash();
			}
		},
		moveFirst:function(){
			if(this.fireEvent("beforechange",this,1)!==false){
			store.currentPage=1;
			reflash();}
		}, 
		movePrevious:function(){
			var b=this,a=b.store.currentPage-1;if(a>0){if(b.fireEvent("beforechange",b,a)!==false){store.currentPage=a;reflash();}}
		},
		moveNext:function(){
			var c=this,b=c.getPageData().pageCount,a=c.store.currentPage+1;if(a<=b){if(c.fireEvent("beforechange",c,a)!==false){store.currentPage=a;reflash();}}
		},

		//��д�������̼�����Ӧ�¼�
		onPagingKeyDown:function(i,h){
			var d=this,b=h.getKey(),c=d.getPageData(),a=h.shiftKey?10:1,g;
			if(b==h.RETURN){
				h.stopEvent();g=d.readPageFromInput(c);
				if(g!==false){
					g=Math.min(Math.max(1,g),c.pageCount);
					if(d.fireEvent("beforechange",d,g)!==false){
						//d.store.loadPage(g)
						d.store.currentPage=g;
						reflash();
					}        
				}
			}
			else{
				if(b==h.HOME||b==h.END){
					h.stopEvent();g=b==h.HOME?1:c.pageCount;i.setValue(g)
				}
				else{
					if(b==h.UP||b==h.PAGEUP||b==h.DOWN||b==h.PAGEDOWN){
						h.stopEvent();g=d.readPageFromInput(c);
						if(g){
							if(b==h.DOWN||b==h.PAGEDOWN)
								{a*=-1}
							g+=a;
							if(g>=1&&g<=c.pages){
								i.setValue(g)
							}
						}
					}
				}
			}  
		}
	});

	//���ñ����
	var setWidth;
	var screenWidth=screen.width;
	if(screenWidth>1440) setWidth=1440;
	if(screenWidth<1024) setWidth=1024;
	setWidth=setWidth-241;

	//�������
	var grid = Ext.create('Ext.grid.Panel', {		
		scrollOffset: 0,
		height: 1000,
		store: store,
		selModel: sm,
		frame:true,
		//�������ÿһ�У������Լ���Ҫ�޸�
		columns:[
			{
				id: 'usermac',
				text: "�û�MAC",
				dataIndex: 'usermac',
				hidden: false,
				width: 120,
				align: 'center',
				sortable: false,
				//renderer: ipgj,                 
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='�������û�MAC'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "IP��ַ",	
				dataIndex: 'userip', 
				width: 110,
				align: 'center',
				//           flex: 1,
				sortable: true,
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='������IP��ַ'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "��������",
				dataIndex: 'onlinecou',           
				width: 60,
				align: 'center',
				//            flex: 1,
				sortable: true,
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='��������������'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "���ӳɹ���",
				dataIndex: 'succescon',
				hidden: false,
				width: 70,
				align: 'center',
				sortable: false,
				editor: {			    
					allowBlank:true
				}
			},{
				text: "ʱ��",
				dataIndex: 'late',
				hidden: false,
				width: 80,
				sortable: false,
				align: 'center',
				editor: {			    
					allowBlank:true
				}
			},{
				text: "������",
				dataIndex: 'lost',
				hidden: false,
				width: 100,
				align: 'center',
				sortable: false,
				editor: {			    
					allowBlank:true
				}
			},{
				text: "��֤�ɹ���",
				dataIndex: 'indenfy',
				width: 80,
				//          flex: 1,
				align: 'center',
				sortable: false
			},{
				id:'content',
				text: "�鿴",
				dataIndex: 'content',
				width: 310,
				align: 'center',
				sortable: false,
				renderer: tooltips
			},{
				flex: 1,
			}
		],
		autoExpandColumn: 'content',
    viewConfig: {forceFit:true},//�Զ���չÿ�еĳ��ȣ�ʹ���������������
		columnLines: true,

		// paging bar on the bottom
		tbar: Ext.create('Ext.PagingToolbar_dir', {
			store: store,
			displayInfo: true,
			displayMsg: '��ʾ{0} - {1}����¼ ��{2}����¼',
			emptyMsg: "û�м�¼����ʾ",
			//��������ҳ����Զ��幦�ܣ����������޸�
			items: [
			groupCombo,		
			{
				xtype:'textfield',
				id:'ipmac_search',
				name:'ipmac_search',
				style:'color:#7aa7d5',
				value:'����mac��ַ��ip��ַ',
				enableKeyEvents:true,
				listeners:{
					'focus':function(){
						if(this.value=='����mac��ַ��ip��ַ'){                
							this.setValue('');
						}
					},

					'keydown' : function(i,e){
						var aae=e.getKey() ; 
						if(aae==13){
							var ipmac_search=Ext.getCmp('ipmac_search').value;
							var para={type:'search',ipormac:ipmac_search}
							reflash(para);
						}
					}
				}
			},
			{
				text:'ȷ��',
				iconCls:'accept',
				handler: function(){
						var ipmac_search=Ext.getCmp('ipmac_search').value;
						var para={type:'search',ipormac:ipmac_search}
						reflash(para);
				}				
			},
			{
				text:'�߼���ѯ',
				iconCls:'accept',
				handler: function(){
						queryWindow.show();
				}
			}
			,'-',{
				text:'����',
				iconCls:'exp',
				handler:function(){
					
				}
			}]


		}),		
		plugins: [rowEditing],
		//html�н����󶨵��Ǹ�id�µı�ǩ��
		renderTo: 'grid-corp'
	});

	// ˢ�����ݴ洢��ʾ�������
	reflash();
}

Ext.define('Ext.ux.grid.plugin.RowEditing', {   
	extend: 'Ext.grid.plugin.RowEditing',   
	alias: 'plugin.ux.rowediting',    

	/**  
	* �Ƿ���Ӽ�¼�ڵ�ǰλ��<br/>  
	* whether add record at current rowIndex.<br/>  
	* see {@link #cfg-addPosition}  
	* @cfg {Boolean}  
	*/  
	addInPlace: false,   

	addPosition: 0,   

	/**  
	* �Ƿ��������¼�,0��������,1������,2����˫��,Ĭ��Ϊ˫��.<br/>  
	* The number of clicks on a grid required to display the editor (disable:0,click:1,dblclick:2)  
	* @cfg {Number}  
	*/  
	clicksToEdit:2,   

	/**  
	* �Ƿ���ȡ���༭��ʱ���Զ�ɾ����ӵļ�¼  
	* if true, auto remove phantom record on cancel,default is true.  
	* @cfg {Boolean}  
	*/  
	autoRecoverOnCancel: true,   

	/**  
	* adding flag  
	* @private  
	* @type Boolean  
	*/  
	adding: false,   

	autoCancel:true,   

	/**  
	* when add record, hide error tooltip for the first time  
	* @private  
	* @type Boolean  
	*/  
	hideTooltipOnAdd: true,   

	/**  
	* register canceledit event && relay canceledit event to grid  
	* @param {Ext.grid.Panel} grid  
	* @override  
	* @private  
	*/  
	init:function(grid){   
		var me = this;   
		/**  
		* ȡ���༭�¼�.<br/>  
		* Fires canceledit event.And will be relayEvents to Grid.<br/>  
		* @see {@link Ext.ux.grid.RowActions#event-beforeaction} <br/>  
		* @event canceledit  
		* @param {Object} context  
		*/  
		me.addEvents('canceledit');   
		me.callParent(arguments);   
		grid.addEvents('canceledit');   
		grid.relayEvents(me, ['canceledit']);   
	},   

	/**  
	* �ṩĬ�ϵ�Editor����  
	*      @example  
	*      {header:'�ֻ�',dataIndex:'phone',fieldType:'numberfield',field:{allowBlank:true}}  
	* provide default field config  
	* @param {String} fieldType ��ѡֵ:numberfield,checkboxfield,passwordField  
	* @return {Object}   
	* @protected  
	*/  
	getFieldCfg: function(fieldType){   
		switch(fieldType){   
			case 'passwordField':   
				return {   
					xtype: 'textfield',   
					inputType: 'password',   
					allowBlank:false  
				}   
			case 'numberfield':   
				return {   
					xtype: 'numberfield',   
					hideTrigger: true,   
					keyNavEnabled: false,   
					mouseWheelEnabled: false,   
					allowBlank:false  
				}   

			case 'checkboxfield':   
				return {   
					xtype: 'checkboxfield',   
					inputValue: 'true',   
					uncheckedValue: 'false'  
				}   
		}   
	},   

	/**  
	* Help to config field,just giving a fieldType and field as additional cfg.  
	* see {@link #getFieldCfg}  
	* @private  
	* @override  
	*/  
	getEditor: function() {   
		var me = this; 
		if (!me.editor) {   
			Ext.each(me.grid.headerCt.getGridColumns(),function(item,index,allItems){   
				if(item.fieldType){   
					item.field = Ext.applyIf(item.field||{},this.getFieldCfg(item.fieldType))   
				}   
			},this)   
			// keep a reference for custom editor..   
			me.editor = me.initEditor();   
		}   
		me.editor.editingPlugin = me   
		return me.editor;   
	},   

	/**  
	* if clicksToEdit===0 then mun the click/dblclick event  
	* @private  
	* @override  
	*/  
	initEditTriggers: function(){   
		var me = this    
		var clickEvent = me.clicksToEdit === 1 ? 'click' : 'dblclick'  
		me.callParent(arguments);    
		if(me.clicksToEdit === 0){   
			me.mun(me.view, 'cell' + clickEvent, me.startEditByClick, me);    
		}   
	},   

	/**  
	* ��Ӽ�¼  
	* add a record and start edit it (will not sync store)  
	* @param {Model/Object} data Data to initialize the Model's fields with <br/>  
	* @param {Object} config see {@link #cfg-addPosition}.   
	*/  
	startAdd: function(data,config){   
		var me = this;   
		var cfg = Ext.apply({   
			addInPlace: this.addInPlace,   
			addPosition: this.addPosition,   
			colIndex: 0   
		},config)   

		//find the position   
		var position;   
		if(cfg.addInPlace){   
			var selected = me.grid.getSelectionModel().getSelection()   
			if(selected && selected.length>0){   
				position = me.grid.store.indexOf(selected[0])    
				console.log('a',position)   
				position += (cfg.addPosition<=0) ? 0: 1   
			}
			else{   
				position = 0   
			}   
		}
		else{   
			position = (cfg.addPosition==-1 ? me.grid.store.getCount() : cfg.addPosition) || 0   
		}   

		var record = data.isModel ? data : me.grid.store.model.create(data);   
		var autoSync = me.grid.store.autoSync;   
		me.grid.store.autoSync = false;   
		me.grid.store.insert(position, record);   
		me.grid.store.autoSync = autoSync;   

		me.adding = true  
		me.startEdit(position,cfg.colIndex);   

		//since autoCancel:true dont work for me   
		if(me.hideTooltipOnAdd && me.getEditor().hideToolTip){   
			me.getEditor().hideToolTip()   
		}   
	},   

	/**  
	* �༭֮ǰ,�Զ�ȡ���༭  
	* Modify: if is editing, cancel first.  
	* @private  
	* @override  
	*/  
	startEdit: function(record, columnHeader) {   
		var me = this;   
		if(me.editing){   
			me.cancelEdit();    
		}  

		me.callParent(arguments);   
		//Ext.getCmp('groupid').setValue(record.get('groupname'));
		//temp_id=record.get('groupid');

	},   

	/**  
	* �޸�adding��״ֵ̬  
	* Modify: set adding=false  
	* @private  
	* @override  
	*/  
	completeEdit: function() {   
		var me = this;   
		if (me.editing && me.validateEdit()) {   
			me.editing = false;   
			me.fireEvent('edit', me.context);   
		}   
		me.adding = false 


		var scode=me.context.record.data.username;
		var sname=me.context.record.data.dispname;
		var scontact=me.context.record.data.contact;
		var stelphone=me.context.record.data.telphone;
		var saddress=me.context.record.data.address;
		var userid= me.context.record.data.userid;

		var gid='';

		//
		if(me.context.record.data.groupname!=''){
			gid=temp_id;
		}
		var gid2=document.getElementById("form1").groupid.value;
		var corpstate=document.getElementById('form1').lxstate.value;
		var keyword1=document.getElementById('form1').keyword.value;



		if(scode==''||scode=='�����뵥λ���'||sname==''||sname=='�����뵥λ����'||gid==''||gid=='0'){
			alert('��λ����͵�λ���Ʋ���Ϊ�պ͵�λ�鲻��Ϊ��');
			//        	reflash();
			return ;
		}

		//      alert(me.context.record.data.groupname);

		store.load({params: {update:"update",userid:userid,groupid1: gid,groupid:gid2,lxstate:corpstate,keyword:keyword1,username:scode,dispname:sname,address:saddress,telphone:stelphone,contact:scontact}});

		//      reflash();


	},   

	/**  
	* ȡ���༭  
	* 1.fireEvent 'canceledit'  
	* 2.when autoRecoverOnCancel is true, if record is phantom then remove it  
	* @private  
	* @override  
	*/  
	cancelEdit: function(){

		var me = this; 

		if (me.editing) {   
			me.getEditor().cancelEdit();   
			me.editing = false;   
			me.fireEvent('canceledit', me.context);    
			if (me.autoRecoverOnCancel){ 
				var temp=me.context.record.data.groupname;  
				if((me.adding)||(temp=='�����밸�����')||(temp=='')){   
					me.context.record.store.remove(me.context.record);   
					me.adding = false  
				}
				else{   
					//����Ҫreject,��ΪEditorû�и���record.   
					//me.context.record.reject()   
				}   
			}   
		}  
	}   
}) ; 

var rowEditing = Ext.create('Ext.ux.grid.plugin.RowEditing', {
	pluginId:'rowEditing', 
	saveBtnText: "����", 
	cancelBtnText: "ȡ��", 
	autoRecoverOnCancel:true,
	autoCancel: true
});
	

//ˢ�����ݴ洢������ҳ������޸�
function reflash(reflash_param){
	if(reflash_param==undefined){
		store.load({params: default_reflash_param});
	}
	else{	
		store.load({params: reflash_param});
	}
}