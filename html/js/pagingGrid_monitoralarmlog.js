//表格的数据存储
var store;
//刷新数据存储时的默认参数，object类型
var default_reflash_param={type:"init"};
function showPagingGrid(){
	//Ext.tip.QuickTipManager.init();
	//    Ext.tip.QuickTipManager.init();

	Ext.define('GridData', {
		extend: 'Ext.data.Model',
		//定义表格字段
		fields: [
			'usermac','userip','ssid','manufacture','logcontent'
		],
		idProperty: ''
	});


	//重写排序函数，解决远程条件查询问题
	Ext.data.Store.prototype.doSort = function() {
		reflash(default_reflash_param);
	}
	
	//创建Grid的数据存储
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
	
	//暂时写死
	var combo_data=[['1','请选择'],['2','AP日志告警'],['3','AC日志告警'],['4','终端日志告警'],['5','网络日志告警']];	
	
	var store_group=new Ext.data.SimpleStore({
		fields:['value','text'],
		data:combo_data
	});
	/*
	//示例：创建一个combox，并绑定数据源
	//需要时使用
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
		emptyText:'请选择',
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

	//render 函数 可以自定义也可以不用
	function editcorp(value,p,record){
		return Ext.String.format('<a href="#xxm" onclick="gotoedit1({0})">修改</a>',record.getId());
	}
	function ipgj(value,p,record){
		var temp_disp=record.data.wlan_Bssid;
		return Ext.String.format( '<a href="#xxm" onclick=gotoIP("{0}","{1}")>'+temp_disp+'</a>',record.getId(),temp_disp); 
	}
  
	//当鼠标移动到当前单元格上时 显示注释，并且当前字体的颜色为#FF3322
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
				//当没有数据时disable removeButton和moveButton
				grid.down('#removeButton').setDisabled(selections.length == 0);
				grid.down('#moveButton').setDisabled(selections.length == 0);
			}
		}
	});	

	//对翻页工具条重新构建，替换翻页函数    
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

		//重写几个键盘键的响应事件
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

	//设置表格宽度
	var setWidth;
	var screenWidth=screen.width;
	if(screenWidth>1440) setWidth=1440;
	if(screenWidth<1024) setWidth=1024;
	setWidth=setWidth-241;

	//创建表格
	var grid = Ext.create('Ext.grid.Panel', {		
		scrollOffset: 0,
		height: 1000,
		store: store,
		selModel: sm,
		frame:true,
		//定义表格的每一列，根据自己需要修改
		columns:[
			{
				id: 'usermac',
				text: "用户MAC",
				dataIndex: 'usermac',
				hidden: false,
				width: 120,
				align: 'center',
				sortable: false,
				//renderer: ipgj,                 
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='请输入用户MAC'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "IP地址",	
				dataIndex: 'userip', 
				width: 110,
				align: 'center',
				//           flex: 1,
				sortable: true,
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='请输入IP地址'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "在线人数",
				dataIndex: 'onlinecou',           
				width: 60,
				align: 'center',
				//            flex: 1,
				sortable: true,
				editor: {
					listeners: {
						'focus':function(){
							if(this.value=='请输入在线人数'){
								this.setValue('');
							}
						}
					},			    
					allowBlank:true
				}
			},{
				text: "连接成功率",
				dataIndex: 'succescon',
				hidden: false,
				width: 70,
				align: 'center',
				sortable: false,
				editor: {			    
					allowBlank:true
				}
			},{
				text: "时延",
				dataIndex: 'late',
				hidden: false,
				width: 80,
				sortable: false,
				align: 'center',
				editor: {			    
					allowBlank:true
				}
			},{
				text: "丢包率",
				dataIndex: 'lost',
				hidden: false,
				width: 100,
				align: 'center',
				sortable: false,
				editor: {			    
					allowBlank:true
				}
			},{
				text: "认证成功率",
				dataIndex: 'indenfy',
				width: 80,
				//          flex: 1,
				align: 'center',
				sortable: false
			},{
				id:'content',
				text: "查看",
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
    viewConfig: {forceFit:true},//自动延展每列的长度，使内容填满整个表格
		columnLines: true,

		// paging bar on the bottom
		tbar: Ext.create('Ext.PagingToolbar_dir', {
			store: store,
			displayInfo: true,
			displayMsg: '显示{0} - {1}条记录 共{2}条记录',
			emptyMsg: "没有记录可显示",
			//工具栏翻页后的自定义功能，根据需求修改
			items: [
			groupCombo,		
			{
				xtype:'textfield',
				id:'ipmac_search',
				name:'ipmac_search',
				style:'color:#7aa7d5',
				value:'输入mac地址或ip地址',
				enableKeyEvents:true,
				listeners:{
					'focus':function(){
						if(this.value=='输入mac地址或ip地址'){                
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
				text:'确认',
				iconCls:'accept',
				handler: function(){
						var ipmac_search=Ext.getCmp('ipmac_search').value;
						var para={type:'search',ipormac:ipmac_search}
						reflash(para);
				}				
			},
			{
				text:'高级查询',
				iconCls:'accept',
				handler: function(){
						queryWindow.show();
				}
			}
			,'-',{
				text:'导出',
				iconCls:'exp',
				handler:function(){
					
				}
			}]


		}),		
		plugins: [rowEditing],
		//html中将表格绑定到那个id下的标签中
		renderTo: 'grid-corp'
	});

	// 刷新数据存储显示表格数据
	reflash();
}

Ext.define('Ext.ux.grid.plugin.RowEditing', {   
	extend: 'Ext.grid.plugin.RowEditing',   
	alias: 'plugin.ux.rowediting',    

	/**  
	* 是否添加记录在当前位置<br/>  
	* whether add record at current rowIndex.<br/>  
	* see {@link #cfg-addPosition}  
	* @cfg {Boolean}  
	*/  
	addInPlace: false,   

	addPosition: 0,   

	/**  
	* 是否点击触发事件,0代表不触发,1代表单击,2代表双击,默认为双击.<br/>  
	* The number of clicks on a grid required to display the editor (disable:0,click:1,dblclick:2)  
	* @cfg {Number}  
	*/  
	clicksToEdit:2,   

	/**  
	* 是否在取消编辑的时候自动删除添加的记录  
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
		* 取消编辑事件.<br/>  
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
	* 提供默认的Editor配置  
	*      @example  
	*      {header:'手机',dataIndex:'phone',fieldType:'numberfield',field:{allowBlank:true}}  
	* provide default field config  
	* @param {String} fieldType 可选值:numberfield,checkboxfield,passwordField  
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
	* 添加记录  
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
	* 编辑之前,自动取消编辑  
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
	* 修改adding的状态值  
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



		if(scode==''||scode=='请输入单位编号'||sname==''||sname=='请输入单位名称'||gid==''||gid=='0'){
			alert('单位编码和单位名称不能为空和单位组不能为空');
			//        	reflash();
			return ;
		}

		//      alert(me.context.record.data.groupname);

		store.load({params: {update:"update",userid:userid,groupid1: gid,groupid:gid2,lxstate:corpstate,keyword:keyword1,username:scode,dispname:sname,address:saddress,telphone:stelphone,contact:scontact}});

		//      reflash();


	},   

	/**  
	* 取消编辑  
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
				if((me.adding)||(temp=='请输入案件编号')||(temp=='')){   
					me.context.record.store.remove(me.context.record);   
					me.adding = false  
				}
				else{   
					//不需要reject,因为Editor没有更改record.   
					//me.context.record.reject()   
				}   
			}   
		}  
	}   
}) ; 

var rowEditing = Ext.create('Ext.ux.grid.plugin.RowEditing', {
	pluginId:'rowEditing', 
	saveBtnText: "保存", 
	cancelBtnText: "取消", 
	autoRecoverOnCancel:true,
	autoCancel: true
});
	

//刷新数据存储，根据页面情况修改
function reflash(reflash_param){
	if(reflash_param==undefined){
		store.load({params: default_reflash_param});
	}
	else{	
		store.load({params: reflash_param});
	}
}