var ictGlobalCtx = new f_IctGlobalContext(); 

var mapPortal = {
	'id_portal_content':['itm_portal_content','内容设置','ict_portal_content'], 
	'id_portal_ssid':['itm_portal_ssid','ssid管理','ict_portal_ssid'], 
	'id_portal_plate':['itm_portal_plate','模版管理','ict_portal_plate']
};

var mapSystem = {
	'id_system_user':['itm_system_user','系统用户管理','ict_system_user']
};

var listMenuMap = {	
	'id_portal':[true, mapPortal, "itm_infoquery", "Portal管理"], 
	'id_system':[false, mapSystem,"itm_system", "系统管理"]
}

var left_tabs_glob='';

//得到该一级菜单需要显示的个数
function getItemShowCount(id){
	var count = 0;
	var myMap = listMenuMap[id][1];
	for(var i in myMap){
		if(Ext.getCmp(i).menustatus == 1){
			count ++;
		}
	}
	return count;
}

//根据id获取到结构体
function getRecordById(id){
	for(var kId in listMenuMap){
		for(var j in listMenuMap[kId][1]){
			if(j == id){
				return listMenuMap[kId][1][j];
			}
		}
	}
}


//作用：根据id设置该项二级菜单的样式 
//参数：isNormal  true--普通
//                false--点击
function setItemStatById(id, isNormal){
	var hasFound = false;
	for(kId in listMenuMap){
		for(key in listMenuMap[kId][1]){
			if(key == id){
				hasFound = true;
				if(isNormal){
					Ext.getCmp(key).setText('<font style="font-size : 12px !important;color:#DDDDDD;margin-left: 24px !important;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;☆&nbsp;&nbsp;&nbsp;'+listMenuMap[kId][1][key][1]+'</font>');
				}else{
					Ext.getCmp(key).setText('<font style="font-size : 12px !important;color:#FEC56B;margin-left: 24px !important;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;★&nbsp;&nbsp;&nbsp;'+listMenuMap[kId][1][key][1]+'</font>');
				}
				break;
			}
		}
		if(hasFound == true)
			break;
	}
}

//根据groupid改变一级菜单下二级菜单的显示和隐藏
function toggleFirstByGroupId(id){
	for(var kId in listMenuMap){
		if(kId == id){
			toggleChildren(true, kId);
		}else{
			toggleChildren(false, kId);
		}
	}
}

//根据itemid获取在大map中的id，方便取该结构
function getIdByItemid(itemid){
	var tmpGroupId = ictGlobalCtx.globalSelectGroupId;
	var hasFound = false;
	var resultId = "id_frontPage";
	
	for(var kId in listMenuMap){
		for(var i in listMenuMap[kId][1]){
			if(listMenuMap[kId][1][i][0] == itemid){
				tmpGroupId = kId;
				resultId = i;
				hasFound = true;
				break;
			}
		}	
		if(hasFound == true){
			break;
		}
	}
	if(ictGlobalCtx.globalSelectGroupId!=tmpGroupId){
		//改变一级菜单的显隐
		toggleFirstByGroupId(tmpGroupId);
		ictGlobalCtx.globalSelectGroupId = tmpGroupId;
	}
	return resultId;
}

//改变点击项样式
function toggleClickItem(itemId){
	//把旧的改为普通样式
	if(ictGlobalCtx.globalSelectId != ""){
		setItemStatById(ictGlobalCtx.globalSelectId, true);
	}
	ictGlobalCtx.globalSelectId = getIdByItemid(itemId);//item.id;
	setItemStatById(ictGlobalCtx.globalSelectId, false);
}

//改变一级菜单样式
function setGroupHeader(isExpand, groupId){
	var groupHeader = Ext.getCmp(groupId);
	if(isExpand){
		if(groupHeader.hasCls("bkmenun_"+groupId)){
			groupHeader.removeCls("bkmenun_"+groupId);
			groupHeader.addCls("bkmenuf_"+groupId);
		}
	}else{
		if(groupHeader.hasCls("bkmenuf_"+groupId)){
			groupHeader.removeCls("bkmenuf_"+groupId);
			groupHeader.addCls("bkmenun_"+groupId);			
		}
	}
}

function toggleChildren(bVisible, gid){
	listMenuMap[gid][0] = bVisible;
	setGroupHeader(bVisible, gid);
	var itm;
	for (id in listMenuMap[gid][1]){
		itm=Ext.getCmp(id);
		if(itm.menustatus == 1){
			itm.setVisible(bVisible);
		}	
	}
}

//作用：跳转到指定页面
//参数：  mac--参数，可不传
function jumpToDestPage(resultId, mac){
	var recordClt = getRecordById(resultId);
	var itemid = recordClt[0];
	var title = recordClt[1];
	var srcurl = "ncViewer."+recordClt[2];
 	//展示获取到的页面
	var tabs_center=Ext.getCmp("layout_center");
	var tab_len=tabs_center.items.length;
	var m = 0;
	var i = 0;
	tabs_center.items.each(
		function(item){ 
			if(itemid==item.id){ 
				i++;
			}
			if(i==0){m++;}
		}
	);   
	if(i>0){
		tabs_center.setActiveTab(m);
		//调用该页面预留三个函数
		ictGlobalCtx.doJumpFunctionsByItemid(itemid, mac);
		return;
    } 
	if(tab_len>5){
		tabs_center.remove(0); 
	}
	var grid=Ext.create(srcurl);         	
	tabs_center.add({
		closable: true,
		autoScroll:false,
		id:itemid,          
		title: title,
		items:[grid]
	}).show();
}
Ext.Loader.setConfig({enabled: true,paths:{'ncViewer':'/js/ict'}});
Ext.define('ncViewer.App', {
    extend: 'Ext.container.Viewport',    
    uses:['ncViewer.frontPage','ncViewer.ict_system_user','ncViewer.ict_portal_content','ncViewer.ict_portal_plate','ncViewer.ict_portal_ssid'],
    initComponent: function(){
        Ext.define('State', {
            extend: 'Ext.data.Model',
            fields: ['groupid', 'groupname']
        });       
        Ext.apply(this, {
            layout: {
                type: 'border',
                padding: 2
            },
            items: [this.createNcTb(),this.createNcLeft(),this.createNcCenter(),this.createNcStatus()]
        });
        this.callParent(arguments);        
    },  
    /**
    创建工具条
     */
    createNcTb: function(){			
        //var right_fun_s=right_fun;
    	var tb = Ext.create('Ext.toolbar.Toolbar',{ style: 'background:#282c36 !important;',id: "maintab"});
        tb.add({
	    	    itemid:'logo',
				//title:'',
				xtype:'label',
        		html:'&nbsp;<img src="/images/mac/banner_logo.png" style="margin:0 0px 0 0px;" width="210" height="60">'
         	},'-',{
				text: '<font style="font-size : 12px !important; color:#FFF;">首页</font>',
				title: '首页',
				//id:'id_frontPage',
				itemid:'itm_frontPage',
				menustatus:1,
				scale:'large',
				iconAlign: 'top',
				srcurl:'ncViewer.frontPage',
				iconCls: 'icon_frontpage',
				handler:this.onItemClick        
			},'-','-','->','->',{
				text: '<font style="font-size : 12px !important;color:#FFF;">系统刷新</font>',
				itemid:'itm_xtsx',
				title:'系统刷新',
				scale:'large',
				iconAlign: 'top',
				iconCls: 'refresh',
				handler: this.onItemReflash
			},'-',{
				text: '<font style="font-size : 12px !important;color:#FFF;">重新登录</font>',
				title:'重新登录',
				scale:'large',
				itemid:'itm_cxdl',
				iconAlign: 'top',
				iconCls: 'unlock',
				handler: this.onItemClose
			}     
        ); 
/*  
		//console.info(right_tool);
        var isep=0;
		var right_tool_s = right_tool;
		var right_fun_s = right_fun;
		tb.items.each(function(item){
			if((right_tool_s.indexOf(item.title)!=-1)||(item.title=='系统刷新')||(item.title=='重新登录')||(item.title=='首页')||(item.itemid=='logo')||(item.itemid == 'itm_search')){
				if((right_fun_s.indexOf(item.title)!=-1)||(item.title=='系统刷新')||(item.title=='重新登录')||(item.title=='首页')||(item.itemid=='logo')||(item.itemid == 'itm_search')){
					item.hidden=false;
				}else{
					item.hidden=true;
				}				
				isep=1;      	 
			}else{
				if(item.text){
					item.hidden=true;
					isep=0;				
				}else{					
					if(isep==0){
						item.hidden=true;
						isep=1;
					}else{
						item.hidden=false;
						isep=0;
					}
				}			
			}
		});
*/
		this.toolPanel = Ext.create(Ext.panel.Panel, {
            id: 'header',
            margins: '0 2 0 2',
            border: true,
			autoScroll:false,
            region: 'north',   
			bodyStyle: 'background:#2A2D37 !important;',
            items:[tb]
        });
        return this.toolPanel;
       
    },
    onItemClose:function(){
    	self.location='/login.htm';
    },
    onItemClose2:function(){
    	self.location='/pronline/Msg?FunName@proauthExit';
    },
    onItemReflash:function(){
        var fp=Ext.create('Ext.FormPanel');
        fp.form.doAction('submit',{
				url:'/pronline/Msg',
				method:'POST',
				waitMsg:'正在重新加载配置, 请稍候...', 
				waitTitle:'提示', 
				params:{FunName:'ncsSysReflesh_v9'}, 
				success:function(form,action){  
					Ext.Msg.show({
						title: '操作',
						msg: '加载完成',
						//modal: true,
						width:50,
						height:50,
						buttons:Ext.MessageBox.OK,
						buttonText: {
							ok:"&nbsp;&nbsp;&nbsp;&nbsp;确定",
							align:'center'
						},
						icon: Ext.Msg.QUESTION					
					});
				},
				failure:function(form,action){        		  
					Ext.Msg.show({
							title: '操作',
							msg: '系统正忙,稍候重试!',
							//modal: true,
							width:50,
							height:50,
							buttons:Ext.MessageBox.OK,
							buttonText: {
								ok:"确定",
								align:'center'
							},
							icon: Ext.Msg.QUESTION
				
					});
				}
        });
    },
	createNcLeft:function(){  
		var leftMenu=Ext.create(Ext.panel.Panel, {
			margins: '0 0 0 0',  
            frame:false,			
			id:'leftMenu',
			height:900,
			autoScroll:false,
			animCollapse: true,
			layout: "column",  //设置为手风琴布局
			layoutConfig: {
				animate: true
			},  
			defaults:{                     
				layout: 'anchor', 
				defaults: {anchor: '100%'  } 
			},  
			bodyStyle: 'background:#232432 !important;',
			cls:'cls_menu_panel',
			width: 210,
			minSize: 100,
			maxSize: 500,							
			columnWidth: 10 / 10,        //设置列的宽度
			items:
			[
				{
					xtype : 'button',
					cls:"bkmenuf_id_portal", 
					text : '<font style="font-size : 14px !important;color:#42D5C9;margin:13px 0 0 60px !important;">Portal管理</font>',
					id : 'id_portal',
					itemid : 'itm_portal',
					menustatus:0,
					listeners:{
						click:this.onToggleFirst
					}
				},{
					xtype : 'button',
					hidden  : false,
					cls:"cls_menu_second_normal", 
					text : '<font style="font-size : 12px !important;color:#EEEEEE;margin-left: 24px !important;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;☆&nbsp;&nbsp;&nbsp;内容设置</font>',
					title : '内容设置',
					itemid:'itm_portal_content',
					menustatus:1,
					id:'id_portal_content',
					srcurl:'ncViewer.ict_portal_content',
					listeners:{
						mouseover:this.changeOnOver,
						mouseout:this.changeOnOut,
						click:this.onItemClick
					}
				}
			]
		});	
		return leftMenu;
/*
		var right_fun_s = right_fun;
		leftMenu.items.each(function(item){
			if(item.menustatus != 0){
				// 1-二级菜单显示 2-二级菜单不显示	
				if(right_fun_s.indexOf(item.title)!=-1){
					item.menustatus = 1;
					//item.hidden = false;
				}else{
					item.menustatus = 2;
					item.hidden = true;
				}
			}
		});
		//判断是否隐藏一级菜单
		leftMenu.items.each(function(item){
			if(item.menustatus == 0){
				if(getItemShowCount(item.id)>0){
					item.hidden = false;
				}else{
					item.hidden = true;
				}				
			}
		});

		this.left_tabs=Ext.createWidget('tabpanel', {
			width: 210,
			frame:false,
			border:false,
			autoScroll:false,
			margins: '0 2 0 2',
			id: 'left',
			layout: 'border',
			title:'当前位置：全部区域',
			collapsible: true,
			region: 'west',
			height:520,
			tabPosition: 'bottom',
			id:'treetab',    
			activeTab: 0,
			defaults :{
				bodyPadding: 0
			},
			listeners: {              
				tabchange:function(tp,p){
					servicecode='';
					corpdid='';
					ncsgroupid='';
					dxtitle='';
					fcode='';
				}
			}
		});

		this.left_tabs.add({closable: false, border:false, autoScroll:true, items:[leftMenu],title: '切换回菜单'});   
		this.left_tabs.add({closable: false, border:false, autoScroll:true, items:[Ext.create(ncViewer.lanGroupTree)],title: '切换到场所'});
		left_tabs_glob=this.left_tabs;
		return this.left_tabs;*/
    },
    createNcCenter:function(){
		//   var charPort=Ext.create('widget.ncCharPoral');
		servicecode='';
		corpdid='';
		ncsgroupid='';    
		compid='';
		dxtitle='';
		var sy_obj;
		var sy_title,sy_id;
		sy_obj=ncViewer.frontPage;
		sy_title='首页';
		sy_id='itm_frontPage';
      
    	this.centerPanel=Ext.createWidget('tabpanel', {
			resizeTabs: true,
			enableTabScroll: true,
			region:'center',
        	id:'layout_center',
        	margins: '2 2 1 0',
			defaults: {
				autoScroll: false,
				bodyPadding: 0
			},
			items: [
			{
				closable: true,
				autoScroll:false,
				id:sy_id,
				iconCls: 'add16',          
				title: sy_title,
				items:[Ext.create(sy_obj)]
			}],
			plugins: Ext.create('Ext.ux.TabCloseMenu', {
				extraItemsTail: [
					'-',
					{
						text: 'Closable',
						checked: true,
						hideOnClick: true,
						handler: function (item) {
							currentItem.tab.setClosable(item.checked);
						}
					},
					'-',
					{
						text: 'Enabled',
						checked: true,
						hideOnClick: true,
						handler: function(item) {
							currentItem.tab.setDisabled(!item.checked);
						}
					}
				],
            listeners: {
                aftermenu: function () {
                    currentItem = null;
                },
                beforemenu: function (menu, item) {
                    menu.child('[text="Closable"]').setChecked(item.closable);
                    menu.child('[text="Enabled"]').setChecked(!item.tab.isDisabled());
                    currentItem = item;
                }
            }
         }),
         listeners: {              
            tabchange:function(tp,p){
				console.log(p.id);
				toggleClickItem(p.id);
          	}
         }
                
       });       
       return this.centerPanel;
    },
    createNcStatus:function(){   	
    	this.ncstatus=Ext.create('Ext.ux.StatusBar', {
        id: 'statusbar',
        region: 'south',
        margins: '1 2 0 2',
        defaultText: 'Default status text',
        text: 'Ready',
		style: 'background:#00B1F1 !important;',
        iconCls: 'x-status-valid',
        items: [{
				xtype: 'button',
				text: '上海新网程信息技术股份有限公司&nbsp;&nbsp;',
				handler: function (){
					
				}
			}]
      });
	
      return this.ncstatus;
    },
	onToggleFirst:function(item){
		for(var kId in listMenuMap){
			if(kId == item.id){
				toggleChildren(!listMenuMap[kId][0], kId);
			}else{
				toggleChildren(false, kId);
			}
		}		
	},
	changeOnOut:function(item){
		if(item.id != ictGlobalCtx.globalSelectId){
			try{
				Ext.getCmp(item.id).setText('<font style="font-size : 12px !important;color:#EEEEEE;margin-left: 24px !important;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;☆&nbsp;&nbsp;&nbsp;'+item.title+'</font>');
			}catch(e){
				alert(e.message);
			}
		}		
	},
	changeOnOver:function(item){	
		if(item.id != ictGlobalCtx.globalSelectId){
			try{
				Ext.getCmp(item.id).setText('<font style="font-size : 12px !important;color:#A9FF96;margin-left: 24px !important;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;☆&nbsp;&nbsp;&nbsp;'+item.title+'</font>');
			}catch (e) {
				alert(e.message);
			}
		}
	},
    onItemClick:function(item){
		toggleClickItem(item.itemid);
    	var tab_title='';
    	var temp_text=item.text;    	 
    	if(temp_text.indexOf('font')== -1){
			tab_title=temp_text;
    	}
    	else{
    		tab_title=item.title;
    	}    
		//改变中间的视图      
		var i=0;
		var m=0;
		var tab_src=item.srcurl;
		var tab_id=item.itemid;
		var tabs_center=Ext.getCmp("layout_center");
		var tab_len=tabs_center.items.length;
		tabs_center.items.each(function(item){
	     	if(item.id==tab_id)
	     	{ 
				i++;
			}
			if(i==0){m++;}
		});   
		if(i>0){
			tabs_center.setActiveTab(m);
			return;
     	} 
		if(tab_len>5){
			tabs_center.remove(0); 
		}  
		var grid=Ext.create(tab_src);  	
		tabs_center.add({
			closable: true,
			autoScroll:false,
			id:tab_id,
			iconCls: 'add16',          
			title: tab_title,
			items:[grid]
		}).show();
    },
    showTabs:function(parentTab,childTab){
		alert("show tabs");  
    },
    refreshComTree: function(){
		Ext.getCmp('treeBtn').fireEvent('click');
    }
});


  

































