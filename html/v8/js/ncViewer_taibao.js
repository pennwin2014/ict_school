var plateid=1; //模板id
//设置选中模板  
store_portplate.load({params:{},callback:function(records, options, success){
		for(i=0; i<store_portplate.getCount();i++){
			if(store_portplate.getAt(i).get('status')==0){
				plateid=store_portplate.getAt(i).get('sid');
			}
		}	
	}
});

//创建模板标签
var required = '<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';
function createPlateItems(){
var string="";
for(i=0;i<store_portplate.getCount();i++){
string = string + "Ext.getCmp('plate"+i+"').removeCls('stripes glow');"
}
string = string + "";

var string2="[";
var j=0;
for(i=0;i<store_portplate.getCount();i++){
if(i==0){
string2 = string2+"{style:{marginTop:'2px',marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate"+i+"', hidden:true,text:'模板1',handler:function(){plateid = store_portplate.getAt("+i+").get('sid');doreflashTab(store_portplate.getAt("+i+").get('sid'));"+string+"Ext.getCmp('plate"+i+"').addCls('stripes glow');}}"
}
else{
string2 = string2+",{style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate"+i+"', hidden:true,text:'模板1',handler:function(){plateid = store_portplate.getAt("+i+").get('sid');doreflashTab(store_portplate.getAt("+i+").get('sid'));"+string+"Ext.getCmp('plate"+i+"').addCls('stripes glow');}}"
}
}
string2=string2+"]";

return eval(string2);
}

function createPlateTab(){

		 Ext.getCmp('treetab').add({
						id:'plateTab',
						closable: false,
					  border:false,
                        items:
						// [
						// {style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate0', hidden:true, text:'模板1',handler:function(){plateid = store_portplate.getAt(0).get('sid');doreflashTab(store_portplate.getAt(0).get('sid'));for(i=0;i<store_portplate.getCount();i++){Ext.getCmp('plate'+i).removeCls('stripes');}Ext.getCmp('plate0').addCls('stripes');}},
						// {style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate1', hidden:true, text:'模板2',handler:function(){plateid = store_portplate.getAt(1).get('sid');doreflashTab(store_portplate.getAt(1).get('sid'));for(i=0;i<store_portplate.getCount();i++){Ext.getCmp('plate'+i).removeCls('stripes');}Ext.getCmp('plate1').addCls('stripes');}},
						// {style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate2', hidden:true, text:'模板3',handler:function(){plateid = store_portplate.getAt(2).get('sid');doreflashTab(store_portplate.getAt(2).get('sid'));for(i=0;i<store_portplate.getCount();i++){Ext.getCmp('plate'+i).removeCls('stripes');}Ext.getCmp('plate2').addCls('stripes');}},
						// {style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate3', hidden:true, text:'模板4',handler:function(){plateid = store_portplate.getAt(3).get('sid');doreflashTab(store_portplate.getAt(3).get('sid'));for(i=0;i<store_portplate.getCount();i++){Ext.getCmp('plate'+i).removeCls('stripes');}Ext.getCmp('plate3').addCls('stripes');}}
						// ],
						createPlateItems(),
						tbar:[{width:118,xtype:'button', id:'plate_apply', hidden:false, text:'应用',handler:function(){
							store_portplate.load({params:{update:'update',sid:plateid}
							,callback:function(records, options, success){
							for(i=0; i<store_portplate.getCount();i++){
							Ext.getCmp('plate'+i).setText(store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).removeCls('stripes glow stripes2');
							}
							for(i=0; i<store_portplate.getCount();i++){
							if(store_portplate.getAt(i).get('status') == 0){
							//Ext.getCmp('plate_apply').setText('应用: '+store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).setText('应用: '+store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).addCls('stripes2');
							}
							}
							}
							});
						
						}}],
						title: '模板'
		 });
		
		//读取修改模板名字
		for(i=0;i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).setText(store_portplate.getAt(i).get('platename'));
		}
		//设置默认选中模板
		for(i=0; i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).removeCls('stripes glow');
			if(store_portplate.getAt(i).get('status')==0){
			Ext.getCmp('plate'+i).addCls('stripes glow');
			plateid=store_portplate.getAt(i).get('sid');
			}
		}
		//alert(plateid);
		//显示模板
		for(i=0; i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).setVisible(true); 
		}
		//当前应用模板
		for(i=0; i<store_portplate.getCount();i++){
			if(store_portplate.getAt(i).get('status') == 0){
				//Ext.getCmp('plate_apply').setText('应用: '+store_portplate.getAt(i).get('platename'));
				Ext.getCmp('plate'+i).setText('应用: '+store_portplate.getAt(i).get('platename'));
				Ext.getCmp('plate'+i).addCls('stripes2');
			}
		}
		Ext.getCmp('treetab').setActiveTab(1);
}

function createAreaTab(){
		Ext.getCmp('treetab').add({
				title:'区域',	
				id:'areaTab',           
        html: '<iframe id="treetab_areatree" name="treetab_areatree" src="/v8/ncm_group_tree_ear.htm" scrolling="no" width=100% height=660  FrameBorder=0 ></iframe>' 
		});	
		
		Ext.getCmp('treetab').setActiveTab(1);	
}

var search_store='';//搜索数据
function search(){//搜索函数
		
		var tabs_center=Ext.getCmp("layout_center");
		var tab_len=tabs_center.items.length;
		  if(tab_len>=1){
			for(i=0;i<tab_len;i++)
			{     		
			 tabs_center.remove(tab_len-i-1); 
			}      	
		  } 
		if(search_store !='')
		eval(search_store).alreadyadd = 0;
		Ext.getCmp('page_main').setVisible(false);
		//销毁模板
		if(Ext.getCmp('plateTab'))
		Ext.getCmp('plateTab').destroy();
		//销毁区域树
	  if(Ext.getCmp('areaTab'))
	  Ext.getCmp('areaTab').destroy();
		//隐藏所有模板
		// for(i=1; i<=store_portplate.getCount();i++){
		// Ext.getCmp('plate'+i).setVisible(false); 
		// }
		var tabname = Ext.getCmp('tb').getComponent('search').getRawValue();
		Ext.getCmp('leftAll').setTitle('<center>'+'自定义搜索'+'</center>');
		if(tabname == '用户管理'){
			if(store_users_usermanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_usermanager',iconCls: 'user',border:false,frame:false,title: "用户管理", items:[Ext.create('users.userManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_users_usermanager"));  
			Ext.getCmp('page_main').setVisible(true);			
			Ext.getCmp('page_main').doRefresh();
			store_users_usermanager.alreadyadd = 1;
			search_store = 'store_users_usermanager';
			createAreaTab();
		}
		/*else if(tabname == '广告管理'){
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "广告管理", items:[]});
			tabs_center.setActiveTab(0);
			if(store_shops_admanager.alreadyadd != 1)
			Ext.getCmp('shops_admanager').add(Ext.create('shops.adManager'));
      Ext.getCmp('page_main').bind(store_shops_admanager);
			Ext.getCmp('page_main').setVisible(true);
			store_shops_admanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_admanager.alreadyadd = 1;
			search_store = 'store_shops_admanager';
			createPlateTab();
		}*/
		else if(tabname == '广告页面管理'){
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_advmanager',iconCls: 'shops_adv',border:false,frame:false,title: "广告页面管理", items:[]});
			tabs_center.setActiveTab(0);
			if(store_shops_advmanager.alreadyadd != 1)
			Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager'));
      Ext.getCmp('page_main').bind(store_shops_advmanager);
			Ext.getCmp('page_main').setVisible(true);
			store_shops_advmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_advmanager.alreadyadd = 1;
			search_store = 'store_shops_advmanager';
			createPlateTab();
		}
		else if(tabname == '商店管理'){
			if(store_shops_shopmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "商店管理", items:[Ext.create('shops.shopManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopmanager.alreadyadd = 1;
			search_store = 'store_shops_shopmanager';
			createPlateTab();
		}
		else if(tabname == '品牌管理'){
			if(store_shops_brandmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_brandmanager',iconCls: 'star',border:false,frame:false,title: "品牌管理", items:[Ext.create('shops.brandManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_brandmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_brandmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_brandmanager.alreadyadd = 1;
			search_store = 'store_shops_brandmanager';
			createPlateTab();
		}
		else if(tabname == '商品管理'){
			if(store_shops_oditymanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "商品管理", items:[Ext.create('shops.odityManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_oditymanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_oditymanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_oditymanager.alreadyadd = 1;
			search_store = 'store_shops_oditymanager';
			createPlateTab();
		}
		else if(tabname == '大类管理'){
			if(store_shops_groupmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_groupmanager',iconCls: 'shops_type',border:false,frame:false,title: "大类管理", items:[Ext.create('shops.groupManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_groupmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_groupmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_groupmanager.alreadyadd = 1;
			search_store = 'store_shops_groupmanager';
			createPlateTab();
		}
		else if(tabname == '小类管理'){
			if(store_shops_typemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_typemanager',iconCls: 'shops_type',border:false,frame:false,title: "小类管理", items:[Ext.create('shops.typeManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_typemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_typemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_typemanager.alreadyadd = 1;
			search_store = 'store_shops_typemanager';
			createPlateTab();
		}
		else if(tabname == '会员管理'){
			if(store_shops_shopusermanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopusermanager',iconCls: 'shops_vip',border:false,frame:false,title: "会员管理", items:[Ext.create('shops.shopuserManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopusermanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopusermanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopusermanager.alreadyadd = 1;
			search_store = 'store_shops_shopusermanager';
			createPlateTab();
		}
		else if(tabname == '消费管理'){
			if(store_shops_consumpmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_consumpmanager',iconCls: 'shops_vip',border:false,frame:false,title: "消费管理", items:[Ext.create('shops.consumpManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_consumpmanager"));  
			Ext.getCmp('page_main').setVisible(true); 
			store_shops_shopusermanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_consumpmanager.alreadyadd = 1;
			search_store = 'store_shops_consumpmanager';
			createPlateTab();
		}
		else if(tabname == '优惠劵管理'){
			if(store_shops_couponmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_couponmanager',iconCls: 'shops_vip',border:false,frame:false,title: "优惠劵管理", items:[Ext.create('shops.couponManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_couponmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_couponmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_couponmanager.alreadyadd = 1;
			search_store = 'store_shops_couponmanager';
			createPlateTab();
		}
		else if(tabname == '优惠劵使用管理'){
			if(store_shops_couponusemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_couponusemanager',iconCls: 'shops_vip',border:false,frame:false,title: "优惠劵使用管理", items:[Ext.create('shops.couponuseManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_couponusemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_couponusemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_couponusemanager.alreadyadd = 1;
			search_store = 'store_shops_couponusemanager';
			createPlateTab();
		}
		else if(tabname == '用户关注管理'){
			if(store_shops_shopattentmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopattentmanager',iconCls: 'shops_vip',border:false,frame:false,title: "用户关注管理", items:[Ext.create('shops.shopattentManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopattentmanager"));  
			Ext.getCmp('page_main').setVisible(true); 
			store_shops_shopattentmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopattentmanager.alreadyadd = 1;
			search_store = 'store_shops_shopattentmanager';
			createPlateTab();
		}
		else if(tabname == '用户收藏管理'){
			if(store_shops_shopcollectmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopcollectmanager',iconCls: 'shops_vip',border:false,frame:false,title: "用户收藏管理", items:[Ext.create('shops.shopcollectManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopcollectmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopcollectmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopcollectmanager.alreadyadd = 1;
			search_store = 'store_shops_shopcollectmanager';
			createPlateTab();
		}
		else if(tabname == '促销广告主页'){
			if(store_shops_activemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_activemanager',iconCls: 'shops_active',border:false,frame:false,title: "促销广告主页", items:[Ext.create('shops.activeManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_activemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_activemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_activemanager.alreadyadd = 1;
			search_store = 'store_shops_activemanager';
			createPlateTab();
		}
		else if(tabname == '促销广告明细'){
			if(store_shops_seriesmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_seriesmanager',iconCls: 'shops_active',border:false,frame:false,title: "促销广告明细", items:[Ext.create('shops.seriesManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_seriesmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_seriesmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_seriesmanager.alreadyadd = 1;
			search_store = 'store_shops_seriesmanager';
			createPlateTab();
		}
		else if(tabname == '短信发送报表'){
			if(store_reports_smsreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_smsreport',iconCls: 'report',border:false,frame:false,title: "短信发送报表", items:[Ext.create('reports.smsReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_smsreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_smsreport.alreadyadd = 1;
			search_store = 'store_reports_smsreport';
		}
		else if(tabname == '短信发送详情'){
			if(store_reports_smsdetailreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_smsdetailreport',iconCls: 'report',border:false,frame:false,title: "短信发送详情", items:[Ext.create('reports.smsDetailReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_smsdetailreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_smsdetailreport.alreadyadd = 1;
			search_store = 'store_reports_smsdetailreport';
		}
		else if(tabname == '上网人数流量报表'){
			if(store_reports_webreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_webreport',iconCls: 'report',border:false,frame:false,title: "上网人数流量报表", items:[Ext.create('reports.webReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_webreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_webreport.alreadyadd = 1;
			search_store = 'store_reports_webreport';
		}
		else if(tabname == '上网详情'){
			if(store_reports_webdetailreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_webdetailreport',iconCls: 'report',border:false,frame:false,title: "上网详情", items:[Ext.create('reports.webDetailReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_webdetailreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_webdetailreport.alreadyadd = 1;
			search_store = 'store_reports_webdetailreport';
		}
		else if(tabname == '设备类型统计'){
			if(store_reports_devicereport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_devicereport',iconCls: 'report',border:false,frame:false,title: "设备类型统计", items:[Ext.create('reports.deviceReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_devicereport"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();			
			store_reports_devicereport.alreadyadd = 1;
			search_store = 'store_reports_devicereport';
		}
		else if(tabname == '用户日志'){
			if(store_logs_userlog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_userlog',iconCls: 'log',border:false,frame:false,title: "用户日志", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_userlog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_userlog.alreadyadd = 1;
			search_store = 'store_logs_userlog';
		}
		else if(tabname == '短信日志'){
			if(store_logs_smslog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_smslog',iconCls: 'log',border:false,frame:false,title: "短信日志", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_smslog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_smslog.alreadyadd = 1;
			search_store = 'store_logs_smslog';
		}
		else if(tabname == '广告日志'){
			if(store_logs_adlog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_adlog',iconCls: 'log',border:false,frame:false,title: "广告日志", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_adlog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_adlog.alreadyadd = 1;
			search_store = 'store_logs_adlog';
		}
		else if(tabname == '终端品牌'){
			if(store_charts_devicebrand.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_devicebrand',iconCls: 'chart_pie',border:false,frame:false,title: "终端品牌", items:[Ext.create('charts.deviceBrand_taibao')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_devicebrand"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_devicebrand.alreadyadd = 1;
			search_store = 'store_charts_devicebrand';
		}
		else if(tabname == '终端类型'){
			if(store_charts_devicetype.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_deviceType',iconCls: 'chart_pie',border:false,frame:false,title: "终端类型", items:[Ext.create('charts.deviceType_taibao')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_devicetype"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_devicetype.alreadyadd = 1;
			search_store = 'store_charts_devicetype';
		}
		else if(tabname == '终端操作系统'){
			if(store_charts_deviceos.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_deviceos',iconCls: 'chart_pie',border:false,frame:false,title: "终端操作类型", items:[Ext.create('charts.deviceOS')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_deviceos"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_deviceos.alreadyadd = 1;
			search_store = 'store_charts_deviceos';
		}
		else if(tabname == '广告点击'){
			if(store_charts_advisit.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_advisit',iconCls: 'chart_pie',border:false,frame:false,title: "广告点击", items:[Ext.create('charts.adVisit')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_advisit"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_advisit.alreadyadd = 1;
			search_store = 'store_charts_advisit';
		}
		else if(tabname == '短信数量'){
		tabs_center.add(
		{
            title: '短信数量',
			iconCls: 'chart_curve',
            id:'charts_smsrecount',            
            html: '<iframe  id="charts_smsrecount" name="charts_smsrecount" src="/ncm_chart/ncm_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '时间段访问'){
		tabs_center.add(
		{
            title: '时间段访问',
			iconCls: 'chart_curve',
            id:'timegroupvisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '实时浏览次数趋势图'){
		tabs_center.add(
		{
            title: '实时浏览次数趋势图',
			iconCls: 'chart_curve',
            id:'realtimevisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_onlineUserRealtimeTrend.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '国内外短信数量'){
			if(store_rules_smsnumber.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsnumber',iconCls: 'rule',border:false,frame:false,title: "国内外短信数量", items:[Ext.create('rules.smsNumber')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsnumber"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsnumber.alreadyadd = 1;
			search_store = 'store_rules_smsnumber';
		}
		else if(tabname == '短信注册数量'){
			if(store_rules_smsregister.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsregister',iconCls: 'rule',border:false,frame:false,title: "短信注册数量", items:[Ext.create('rules.smsRegister')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsregister"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsregister.alreadyadd = 1;
			search_store = 'store_rules_smsregister';
		}
		else if(tabname == '上网时长'){
			if(store_rules_timelimit.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_timelimit',iconCls: 'rule',border:false,frame:false,title: "上网时长", items:[Ext.create('rules.timeLimit')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_timelimit"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_timelimit.alreadyadd = 1;
			search_store = 'store_rules_timelimit';
		}
		else if(tabname == '短信格式'){
			if(store_rules_smsformat.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsformat',iconCls: 'rule',border:false,frame:false,title: "短信格式", items:[Ext.create('rules.smsFormat')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsformat"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsformat.alreadyadd = 1;
			search_store = 'store_rules_smsformat';
		}
		else if(tabname == '黑名单'){
			if(store_rules_blackuser.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_blackuser',iconCls: 'rule',border:false,frame:false,title: "黑名单", items:[Ext.create('rules.blackUser')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_blackuser"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();		
			store_rules_blackuser.alreadyadd = 1;
			search_store = 'store_rules_blackuser';
		}
		else if(tabname == 'AP管理'){
			if(store_users_apmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_apmanager',iconCls:'rule',border:false,frame:false,title: "AP管理",items:[Ext.create('users.apManager')]});
            Ext.getCmp('page_main').bind(store_users_apmanager);
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_users_apmanager.alreadyadd = 1;
			search_store = 'store_users_apmanager';
		}
		else if(tabname == '单位管理'){
			if(store_users_groupmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_groupmanager',iconCls:'rule',border:false,frame:false,title: "单位管理",items:[Ext.create('users.groupManager')]});
            Ext.getCmp('page_main').bind(store_users_groupmanager);
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_users_groupmanager.alreadyadd = 1;
			search_store = 'store_users_groupmanager';
		}
		else if(tabname == '系统权限管理'){
		tabs_center.add(
		{
            title: '系统权限管理',
			iconCls: 'system',
            id:'systems_rolemenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
		}
		);
		}
		else if(tabname == '系统用户管理'){
		tabs_center.add(
		{
            title: '系统用户管理',
			iconCls: 'system',
            id:'systems_xtyhmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '用户访问日志'){
		tabs_center.add(
		{
            title: '用户访问日志',
			iconCls: 'system',
            id:'systems_usvisitmenu',            
            html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '修改密码'){
		tabs_center.add(
		{
            title: '修改密码', 
			iconCls: 'key',
            id:'myaccount_modipassword',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/pronline/Msg?FunName@ncmWebDispModiPass" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		tabs_center.setActiveTab(0);
}
function DateUtil(){}  
/**  
*功能:格式化时间  
*示例:DateUtil.Format("yyyy/MM/dd","Thu Nov 9 20:30:37 UTC+0800 2006 ");  
*返回:2006/11/09  
*/  
DateUtil.Format=function(fmtCode,date){  
var result,d,arr_d;  

var patrn_now_1=/^y{4}-M{2}-d{2}\sh{2}:m{2}:s{2}$/;  
var patrn_now_11=/^y{4}-M{1,2}-d{1,2}\sh{1,2}:m{1,2}:s{1,2}$/;  

var patrn_now_2=/^y{4}\/M{2}\/d{2}\sh{2}:m{2}:s{2}$/;  
var patrn_now_22=/^y{4}\/M{1,2}\/d{1,2}\sh{1,2}:m{1,2}:s{1,2}$/;  

var patrn_now_3=/^y{4}年M{2}月d{2}日\sh{2}时m{2}分s{2}秒$/;  
var patrn_now_33=/^y{4}年M{1,2}月d{1,2}日\sh{1,2}时m{1,2}分s{1,2}秒$/;  

var patrn_date_1=/^y{4}-M{2}-d{2}$/;  
var patrn_date_11=/^y{4}-M{1,2}-d{1,2}$/;  

var patrn_date_2=/^y{4}\/M{2}\/d{2}$/;  
var patrn_date_22=/^y{4}\/M{1,2}\/d{1,2}$/;  

var patrn_date_3=/^y{4}年M{2}月d{2}日$/;  
var patrn_date_33=/^y{4}年M{1,2}月d{1,2}日$/;  

var patrn_time_1=/^h{2}:m{2}:s{2}$/;  
var patrn_time_11=/^h{1,2}:m{1,2}:s{1,2}$/;  
var patrn_time_2=/^h{2}时m{2}分s{2}秒$/;  
var patrn_time_22=/^h{1,2}时m{1,2}分s{1,2}秒$/;  

if(!fmtCode){fmtCode="yyyy/MM/dd hh:mm:ss";}  
if(date){  
d=new Date(date);  
if(isNaN(d)){  
msgBox("时间参数非法\n正确的时间示例:\nThu Nov 9 20:30:37 UTC+0800 2006\n或\n2006/      10/17");  
return;}  
}else{  
d=new Date();  
} 
if(patrn_now_1.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"-"+arr_d.MM+"-"+arr_d.dd+" "+arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_now_11.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"-"+arr_d.MM+"-"+arr_d.dd+" "+arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_now_2.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"/"+arr_d.MM+"/"+arr_d.dd+" "+arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_now_22.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"/"+arr_d.MM+"/"+arr_d.dd+" "+arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_now_3.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"年"+arr_d.MM+"月"+arr_d.dd+"日"+" "+arr_d.hh+"时"+arr_d.mm+"分"+arr_d.ss+"秒";  
}  
else if(patrn_now_33.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"年"+arr_d.MM+"月"+arr_d.dd+"日"+" "+arr_d.hh+"时"+arr_d.mm+"分"+arr_d.ss+"秒";  
}  

else if(patrn_date_1.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"-"+arr_d.MM+"-"+arr_d.dd;  
}  
else if(patrn_date_11.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"-"+arr_d.MM+"-"+arr_d.dd;  
}  
else if(patrn_date_2.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"/"+arr_d.MM+"/"+arr_d.dd;  
}  
else if(patrn_date_22.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"/"+arr_d.MM+"/"+arr_d.dd;  
}  
else if(patrn_date_3.test(fmtCode))  
{  
arr_d=splitDate(d,true);  
result=arr_d.yyyy+"年"+arr_d.MM+"月"+arr_d.dd+"日";  
}  
else if(patrn_date_33.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"年"+arr_d.MM+"月"+arr_d.dd+"日";  
}  
else if(patrn_time_1.test(fmtCode)){  
arr_d=splitDate(d,true);  
result=arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_time_11.test(fmtCode)){  
arr_d=splitDate(d);  
result=arr_d.hh+":"+arr_d.mm+":"+arr_d.ss;  
}  
else if(patrn_time_2.test(fmtCode)){  
arr_d=splitDate(d,true);  
result=arr_d.hh+"时"+arr_d.mm+"分"+arr_d.ss+"秒";  
}  
else if(patrn_time_22.test(fmtCode)){  
arr_d=splitDate(d);  
result=arr_d.hh+"时"+arr_d.mm+"分"+arr_d.ss+"秒";  
}  
else{  
msgBox("没有匹配的时间格式!");  
return;  
}  

return result;  
};  
function splitDate(d,isZero){  
var yyyy,MM,dd,hh,mm,ss;  
if(isZero){  
yyyy=d.getFullYear();  
MM=(d.getMonth()+1)<10?"0"+(d.getMonth()+1):d.getMonth()+1;  
dd=d.getDate()<10?"0"+d.getDate():d.getDate();  
hh=d.getHours()<10?"0"+d.getHours():d.getHours();  
mm=d.getMinutes()<10?"0"+d.getMinutes():d.getMinutes();  
ss=d.getSeconds()<10?"0"+d.getSeconds():d.getSeconds();  
}else{  
yyyy=d.getFullYear();    //firefox 特别
MM=d.getMonth()+1;  
dd=d.getDate();  
hh=d.getHours();  
mm=d.getMinutes();  
ss=d.getSeconds();    
}  
return {"yyyy":yyyy,"MM":MM,"dd":dd,"hh":hh,"mm":mm,"ss":ss};    
}  
function msgBox(msg){  
window.alert(msg);  
}

function splitDate(d,isZero){  
		var yyyy,MM,dd,hh,mm,ss;  
		if(isZero){  
				yyyy=d.getFullYear();  
				MM=(d.getMonth()+1)<10?"0"+(d.getMonth()+1):d.getMonth()+1;  
				dd=d.getDate()<10?"0"+d.getDate():d.getDate();  
				hh=d.getHours()<10?"0"+d.getHours():d.getHours();  
				mm=d.getMinutes()<10?"0"+d.getMinutes():d.getMinutes();  
				ss=d.getSeconds()<10?"0"+d.getSeconds():d.getSeconds();  
		}else{  
				yyyy=d.getFullYear();    //firefox 特别
				MM=d.getMonth()+1;  
				dd=d.getDate();  
				hh=d.getHours();  
				mm=d.getMinutes();  
				ss=d.getSeconds();    
		}  
		return {"yyyy":yyyy,"MM":MM,"dd":dd,"hh":hh,"mm":mm,"ss":ss};    
}  
//格式化时间
Date.prototype.format = function(format){
    var o = {
    "M+" : this.getMonth()+1, //month
    "d+" : this.getDate(),    //day
    "h+" : this.getHours(),   //hour
    "m+" : this.getMinutes(), //minute
    "s+" : this.getSeconds(), //second
    "q+" : Math.floor((this.getMonth()+3)/3),  //quarter
    "S" : this.getMilliseconds() //millisecond
    }
    if(/(y+)/.test(format)) format=format.replace(RegExp.$1,
    (this.getFullYear()+"").substr(4 - RegExp.$1.length));
    for(var k in o)if(new RegExp("("+ k +")").test(format))
    format = format.replace(RegExp.$1,
    RegExp.$1.length==1 ? o[k] :
    ("00"+ o[k]).substr((""+ o[k]).length));
    return format;
}
var now   = new Date();
var month = now.getMonth();
var year  = now.getFullYear();
var day   = now.getDate();
var wday  = now.getDay;
  //最近7天
now.endTime   = now.format( "yyyy/MM/dd", now.setDate( day ) );
now.beginTime = now.format( "yyyy/MM/dd", now.setDate( day-7 ) );

var	startday=now.beginTime;
var	today=now.endTime;

var grid_height; //显示区表单高度
//传递模板ID
function doreflashTab(plateid){
	var tabs_obj=Ext.getCmp('layout_center');
	var index=tabs_obj.getActiveTab();
	var tabframe=index.id;

	if(tabframe=='shops_categorymanager'){
		tabs_obj=Ext.getCmp('categoryManager');
		index=tabs_obj.getActiveTab();
		tabframe=index.id;

	}
	else if(tabframe=='shops_usermanager'){
		tabs_obj=Ext.getCmp('userManager');
		index=tabs_obj.getActiveTab();
		tabframe=index.id;
	}
	else if(tabframe=='shops_actmanager'){
		tabs_obj=Ext.getCmp('actManager');
		index=tabs_obj.getActiveTab();
		tabframe=index.id;
	}
	else if(tabframe=='shops_advmanager'){
			index=tabs_obj.getActiveTab();
			tabframe=index.id;
			var tabstore;
			//var tabstore = 'store_'+tabframe;
			if(plateid == ''){
				  tabstore = 'store_'+tabframe;
					eval(tabstore).load({params:{},
							callback:function(records, options, success){ 
									eval(shops.advManager).init()}
					})
			}else{
			    if(plateid == 8){
			    	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager6'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager6).init()
									}
							})
			    }else if(plateid == 7){
				  	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager5'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager5).init()
									}
							})
				  }else if(plateid == 6){
						  tabstore = 'store_'+tabframe+'3';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager3'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager3_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager3).init()
									}
							})
					}else if(plateid == 5){//如果是模板5，默认新商城模板
						  tabstore = 'store_'+tabframe+'2';
							Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager2'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager2_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager2).init()
									}
							})
					}else{
						  tabstore = 'store_'+tabframe;
							Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager).init()
									}
							})
					}
		  }
		  return;
	}
	var tabstore = 'store_'+tabframe;
	if(plateid == ''){
			eval(tabstore).load();
	}else{
			eval(tabstore).plateid = plateid;
			eval(tabstore).new_params={plateid:plateid};
			eval(tabstore).load();	
	}
}

function doreflashTab1(areaid, groupid){
		var tabs_obj=Ext.getCmp('layout_center');
		var index=tabs_obj.getActiveTab();
		var tabframe=index.id;
		
		var tabstore = 'store_'+tabframe;
		eval(tabstore).areaid = areaid;
		eval(tabstore).groupid = groupid;
		eval(tabstore).new_params={areaid:areaid,groupid:groupid};
		eval(tabstore).load();
}

//图表查看详细信息
function openWin(value,tabframe,tabstore){
alert(eval(tabstore).sdate);
eval(tabframe).openWin(value);
}
Ext.Loader.setConfig({enabled: true,paths:{'ncViewer':'/v8/js'}});
Ext.Loader.setConfig({enabled: true,paths:{'users':'/v8/js/users'}});
Ext.Loader.setConfig({enabled: true,paths:{'online':'/v8/js/online'}});
Ext.Loader.setConfig({enabled: true,paths:{'shops':'/v8/js/shops'}});
Ext.Loader.setConfig({enabled: true,paths:{'rules':'/v8/js/rules'}});
Ext.Loader.setConfig({enabled: true,paths:{'settings':'/v8/js/settings'}});
Ext.Loader.setConfig({enabled: true,paths:{'myaccount':'/v8/js/myaccount'}});

Ext.Loader.setConfig({enabled: true,paths:{'network':'/v8/js/network'}});
Ext.Loader.setConfig({enabled: true,paths:{'systemmanage':'/v8/js/systemmanage'}});
Ext.Loader.setConfig({enabled: true,paths:{'logs':'/v8/js/logs'}});
Ext.Loader.setConfig({enabled: true,paths:{'charts':'/v8/js/charts'}});
Ext.Loader.setConfig({enabled: true,paths:{'reports':'/v8/js/reports'}});
Ext.Loader.setConfig({enabled: true,paths:{'usermanger':'/v8/js/usermanger'}});
var new_params="";
//引入扩展组件  
Ext.Loader.setConfig({enabled: true});  
 
Ext.require([  
    'Ext.ux.data.PagingMemoryProxy',  
    'Ext.ux.ProgressBarPager'  
]);  

Ext.define('ncViewer_taibao.App', {
		extend: 'Ext.container.Viewport',    
		uses:['online.onlineInfo'],
		initComponent: function(){
		    Ext.define('State', {
		        extend: 'Ext.data.Model',
		        fields: ['groupid', 'groupname']
		    });       
				Ext.apply(this, {
		        layout: {
			
		            type: 'border'  
		        },
						items: [this.createNcTb(),this.createNcLeft(),this.createNcCenter()]
		    });
		    this.callParent(arguments);
		},
		
		/**
		创建工具条
		 */
		createNcTb: function(){
				//创建策略菜单
				var menu_procy = Ext.create('Ext.menu.Menu', {
				    id: 'sfkglMenu',
				    style: {
				        overflow: 'visible'     
				    }
				    ,
				    items: [
				      {
				            text: '控制策略',
				            itemId:'kzcl',
				            srcurl:'ncViewer.lanProcy',  
				            handler:this.onItemClick
				   
				       },
				       {
				       	    text: '策略下发',
				            itemId:'clxf',
				            srcurl:'ncViewer.lanProcyXf',
				           handler:this.onItemClick
				       }
				    ]
				    
				});

    	 	var tb = Ext.create('Ext.toolbar.Toolbar',{ id:'tb',border:false,frame:false,style: 'background-image:url();'});
				tb.add({
						xtype:'label',
						html:'&nbsp;<img src="/images/large/background.jpg" width="2880" height="62" style="vertical-align:top;margin-top:-15px">',
						//html:'&nbsp;<div width="1980" height="60" style="vertical-align:top;margin-top:-15px" class="gallery"></div>',
						width:1
				},'->','->',{
		style:'margin-left:400px;',
		xtype:'combo',
		itemId:'search',
		store:store_search,
		//fieldLabel:'搜索',
            emptyText: '请输入搜索功能名',
		labelWidth: 30,
		width: 200,
		height:28,
		valueField:'groupid',
		displayField: 'groupname',    
		typeAhead: true,
		hideTrigger:true,
		enableKeyEvents:true,
		listeners: {
		'change':function(){
		//search();
		}
		// ,
		// 'keydown':function(i,e){
                  // var aae=e.getKey(); 
                  // if(aae==13){
				// var tabname = Ext.getCmp('tb').getComponent('search').getRawValue();
				// if(tabname !=''){
				// search();
				// }
                  // }
           // }
	}
        },{
		xtype:'button',
		itemId:'searchimg',
            tooltip: '搜索',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'search',
            handler: search
        },'-',{
		xtype:'combo',
		itemId:'skintheme',
		store:store_skintheme,
            emptyText: '皮肤主题',
		labelWidth: 30,
		width: 100,
		valueField:'groupid',
		displayField: 'groupname',    
		typeAhead: true,
		listeners: {
		'change':function(){
		
		var fp=Ext.create('Ext.FormPanel');
		fp.form.doAction('submit',{
		url:'/pronline/Msg',
            method:'POST',
		params:{FunName:'ncm_skintheme',update:this.value,username:username}
		});
		
		Ext.Msg.confirm('切换皮肤主题','请重新登录',
		function(btn){
		if(btn=='yes'){
		  self.location='/enter.html';							
		}else{
		  
		}
		},this);
		
		}
	}
        },'-','',{
		tooltip: '操作帮助',
		width:50,
            itemId:'help',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'help',
            handler: this.onItemHelp
        },{
		tooltip: '全屏显示',
		width:50,
            itemId:'max',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'hidden',
            handler: this.onItemMax
        },{
            tooltip: '系统刷新',
		width:50,
            itemId:'xtsx',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'refresh',
            handler: this.onItemReflash
        },{
            tooltip: '重新登录',
		width:50,
            scale:'large',
            itemId:'cxdl',
            iconAlign: 'top',
            iconCls: 'unlock',
		handler: this.onItemClose
        },{
	    	  itemId:'logo',
       		  xtype:'label'
		//html:'<center><div class="ain_3d"><span class="gallery">新网程</span><br><span class="gallery2">登录门户管理系统</span></div></center>'
        }); 

        var right_fun_s=right_fun;
        var isep=0;
           	
        this.toolPanel = Ext.create(Ext.panel.Panel, {
            id: 'header',
			
			//collapsed: true,
			//collapsible: true,
			
			 // tools: [
                  // {
                // xtype:'button',
                // text:'界面',
                // glyph: 61,
                // menu:[{
                    // text:'Classic',
					// handler:function(){ Ext.MessageBox.alert('Classic');
					// self.location='/enter.html';
					
					// }
                // },{
                    // text:'Neptune',
					// handler:function(){ Ext.MessageBox.alert('Neptune');
					// self.location='/enter.html';
					
					// }
                // }]
				// }
                // ],
			//title:'<center>新网程登录门户管理系统</center>',
            border: false,
			frame:false,
            region: 'north',
			//bodyStyle: 'background:#e1e1e1;padding:1px 1px 1px 1px;',
			
            items:[
			tb
			]
          
        });
        return this.toolPanel;
    },
		onItemHelp:function(){
		
			var winhelp;
			var htmlvar;
			if(username == "admin"){
				htmlvar = '<iframe src="/specification/ncm_webspcification_admin.html" scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:2030px;"></iframe>';
			}
			else{
				htmlvar = '<iframe src="/specification/ncm_webspcification_shop.html" scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:2700px;"></iframe>';
			}
			winhelp =  new Ext.Window ({
		                       id:"winhelp",
		                       title:"操作帮助窗口",
		                       width:650,
		                       height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
		                       maximizable:true,
		                       bodyPadding: 0,
		                       autoScroll:true,
													 //headerPosition: 'bottom',
		                       resizable:true,
		                       closable :true,
		                       draggable:true,
		                       html:htmlvar
		  })
		  winhelp.show();
		
			// var winhelp2 =  new Ext.Window ({
		                       // id:"winhelp2",
		                       // width:0,
						   // x:0,y:0,
		                       // height:0,
		                       // maximizable:false,
		                       // bodyPadding: 0,
		                       // autoScroll:true,
		                       // resizable:false,
		                       // closable :false,
		                       // draggable:false,
		                      // html:'<div class="cover"><img src="/images/click.png" /></div>'
		  // })
		  // winhelp2.show();
		},
		onItemMax:function(){
		//Ext.getCmp('header').setTitle('<center>新网程登录门户管理系统</center>');
			Ext.getCmp('header').collapse();
			//Ext.getCmp('header').preventHeader=false;
		},
    onItemClose:function(){
    	self.location='/enter.html';
    },
    onItemReflash:function(){
          var fp=Ext.create('Ext.FormPanel');
               fp.form.doAction('submit',{
               url:'/pronline/Msg',
                method:'POST',
               params:{FunName:'ncsSysReflesh_v4'},
                 
                success:function(form,action){ 
           					Ext.getCmp('winstatus').close();
                 },
             		 failure:function(form,action){        		 
                    Ext.Msg.alert('操作',"系统正忙,稍候重试!");
                    Ext.getCmp('winstatus').close();
            		 }
                }
             )
					    var wint = Ext.create('Ext.Window', {
					 //       title: '系统刷新',
					        id:'winstatus',
					        header:false,
					        width: 150,
					        x:600,
					        y:210,
					        height: 40,
					        closable: false,
					        modal: true,
					  //      closeAction: 'hide',
					        bodyPadding: 10,
					        bbar: Ext.create('Ext.ux.StatusBar', {
					            id: 'win-statusbar',
					            defaultText: '系统就绪'
					        })
					    });					   
					    wint.show();
					    loadFn('win-button', 'win-statusbar');
    
    },
    createNcLeft:function(){   	
				var left_tabs=Ext.createWidget('tabpanel', {
				    width: 123,
				    border:false,
				    margins: '0 0 0 0',
						//height:970,
						//tabPosition:'left',
						region: 'west',
				    id:'treetab',    
				    activeTab: 0,
				    defaults :{
				        bodyPadding: 0
				    },
						enableTabScroll:true,
						listeners: {        
			            tabchange:function(tp,p){ 
						if(p.title=='菜单'){
				            Ext.getCmp('leftAll').setWidth(125);
							Ext.getCmp('treetab').setWidth(123);	
				        }
						else if(p.title=='区域'){
				            Ext.getCmp('leftAll').setWidth(180);
							Ext.getCmp('treetab').setWidth(160);	
				        }
						}
						}
				}); 
     		this.leftPanel=Ext.create(Ext.panel.Panel, {
             id: 'left',
             margins: '0 2 0 2',
             // layout: 'border',
             // title:'<center>'+right_sy+'</center>',
             region: 'west',
             // collapsible: true,
             autoScroll: true,
			       border:false,
             // bodyStyle: 'background:#fff;',
             split:true,
			       style:{marginBottom:'2px',marginLeft:'2px'},
             width: 123,
	           minSize: 100,
	           maxSize: 500,
             items:[{
								xtype:'button',
								style:'margin-bottom:1px;margin-top:1px;',
		            itemId:'onlineinfo',
		            hidden:false,
		            text:'在线信息',
		            scale:'large',
		            iconCls: 'zxxx',
		            iconAlign: 'left',
		            srcurl:'ncViewer.ncmOnlineInfo',
		            handler:this.onItemClick
						},{
								xtype:'button',
								style:'margin-bottom:1px',
		            itemId:'users',
		            hidden:false,
		            text:'用户管理',
								scale:'large',
		            iconCls: 'users',
		            iconAlign: 'left',
		            srcurl:'ncViewer.ncmUsers',
		            handler:this.onItemClick
					  },
							/*{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'users_ap',
					            hidden:false,
					            text:'A P 管理',
								scale:'large',
					            iconCls: 'users',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmUsers',
					            handler:this.onItemClick
					        },*/
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'shops',
					            hidden:false,
					            text:'商城管理',
								scale:'large',
					            iconCls: 'shops',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmShops',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'shops_shop',
					            hidden:false,
					            text:'商店管理',
								scale:'large',
					            iconCls: 'shop',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmShops',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'shops_ad',
					            hidden:false,
					            text:'广告管理',
								scale:'large',
					            iconCls: 'ad',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmShops',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'shops_product',
					            hidden:false,
					            text:'商品管理',
								scale:'large',
					            iconCls: 'product',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmShops',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'reports',
					            hidden:false,
					            text:'统计报表',
					            scale:'large',
					            iconCls: 'reports',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmReports',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'charts',
					            hidden:false,
					            text:'业务感知',
								scale:'large',
					            iconCls: 'adcharts',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmCharts',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'logs',
					            hidden:false,
					            text:'日志查询',
								scale:'large',
					            iconCls: 'logs',
					            iconAlign: 'left',
					            //srcurl:'ncViewer.ncmLog',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'rules',
					            hidden:false,
					            text:'规则设置',
								scale:'large',
					            iconCls: 'charts',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmSettings',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'settings',
					            hidden:false,
					            text:'系统配置',
								scale:'large',
					            iconCls: 'settings',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmSettings',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'systems',
					            hidden:false,
					            text:'系统管理',
								scale:'large',
					            iconCls: 'systems',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmSystems',
					            handler:this.onItemClick
					        }
					        ,
							{
								xtype:'button',
								style:'margin-bottom:1px',
					            itemId:'myaccount',
					            hidden:false,
					            text:'我的账号',
								scale:'large',
					            iconCls: 'accounts',
					            iconAlign: 'left',
					            srcurl:'ncViewer.ncmAccounts',
					            handler:this.onItemClick
					        }]
        });
				left_tabs.add({
					  closable: false,
				    border:false,
            items:[this.leftPanel],
					  title: '菜单'
		 		});
		 		this.leftAll=Ext.create(Ext.panel.Panel, {
             id: 'leftAll',
             margins: '0 2 0 2',
             // layout: 'border',
             title:'<center>'+right_sy+'</center>',
             region: 'west',
             // collapsible: true,
             autoScroll: true,
             // bodyStyle: 'background:#fff;',
             split:true,
             width: 125,
	           minSize: 100,
	           maxSize: 500,
             items:[left_tabs]
        });
        return this.leftAll;
    },
    createNcCenter:function(){
		    var swidth=screen.width;
		    groupid='';
		    compid='';
		    dxtitle='';
    		this.centerPanel=Ext.createWidget('tabpanel', {
		        resizeTabs: true,
		        enableTabScroll: true,
		        autoScroll:false,
				    //tabPosition: 'top',
		        region:'center',
		        id:'layout_center',
						border:false,
						frame:false,
						bbar:[{
		           	xtype:'pagingtoolbar',
								border:false,
								frame:false,
		           	id:'page_main',
		           	hidden:true,
		            store:Ext.StoreManager.lookup('store_adminuser'),
		            displayInfo: true,
		            displayMsg: '显示 <font color=red>{0}</font> - <font color=red>{1}</font> 条，  共 <font color=red>{2}</font> 条',
		            emptyMsg: "共 0 条",
								plugins: new Ext.ux.ProgressBarPager(),
		            doRefresh:function(){  
		             
		            this.store.load();
						
		            },
		            moveLast:function(){
						    	var b=this,a=b.getPageData().pageCount;
						    	if(b.fireEvent("beforechange",b,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						      }
						    },
						    moveFirst:function(){
							    if(this.fireEvent("beforechange",this,1)!==false){
						    		this.store.currentPage=1;
						    		this.store.load();
							    }
							  }, 
						   	movePrevious:function(){
						   		var b=this,a=b.store.currentPage-1;
						   		if(a>0){
						   			if(b.fireEvent("beforechange",b,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						   			}
						   		}
						   	},
						   	moveNext:function(){
						   		var c=this,b=c.getPageData().pageCount,a=c.store.currentPage+1;
						   		if(a<=b){
						   			if(c.fireEvent("beforechange",c,a)!==false){
						    		this.store.currentPage=a;
						    		this.store.load();
						   			}
						   		}
						   	},
						   	onPagingKeyDown:function(i,h){
						    	var d=this,b=h.getKey(),c=d.getPageData(),a=h.shiftKey?10:1,g;
						    	if(b==h.RETURN){
						    		h.stopEvent();
						    		g=d.readPageFromInput(c);
						        if(g!==false){
						        	g=Math.min(Math.max(1,g),c.pageCount);
						         	if(d.fireEvent("beforechange",d,g)!==false){
						    		this.store.currentPage=g;
						    		this.store.load();
							        }        
							      }
							    }else{
						      	if(b==h.HOME||b==h.END){
						    			h.stopEvent();g=b==h.HOME?1:c.pageCount;i.setValue(g)
						       	}else{
						        	if(b==h.UP||b==h.PAGEUP||b==h.DOWN||b==h.PAGEDOWN){
							          h.stopEvent();g=d.readPageFromInput(c);
							          if(g){
							            if(b==h.DOWN||b==h.PAGEDOWN){a*=-1}
							            g+=a;
							            if(g>=1&&g<=c.pages){
							            	i.setValue(g)
							            }
							          }
						         	}
						        }
						      }  
						    } 
		        },'->',{
		        	  xtype:'label', html:'<div style="color:white;padding-left:100px;padding-right:20px;padding-top:8px;padding-bottom:8px;background:url(/images/large/bottom.png);">中国电信集团公司承建</div>'
		        }],
		        
		        listeners: {        
		            tabchange:function(tp,p){ 
				    				grid_height=Ext.getCmp('layout_center').getHeight()-58;
			            	var tabs_center=Ext.getCmp("layout_center");
										var active = tabs_center.getActiveTab();
										Ext.getCmp('page_main').setVisible(false);
			            	if(p.title=='权限角色'){
			            			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_system_role"));
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
			            	}
										else if(p.title=='在线信息')Ext.getCmp('page_main').setVisible(true); 
		            		else if(p.title=='系统用户'){
			            			Ext.getCmp('page_main').bind(store_adminuser);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
		            		}
			            	else if(p.title=='系统操作日志'){
			            			Ext.getCmp('page_main').bind(store_system_log);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
			            	}
										else if(p.title=='广告日志'){
												if(store_logs_adlog.alreadyadd != 1)
												Ext.getCmp('logs_adlog').add(Ext.create('logs.adlog'));
				            		Ext.getCmp('page_main').bind(store_logs_adlog);
				                Ext.getCmp('page_main').setVisible(true);
				                Ext.getCmp('page_main').doRefresh();
												store_logs_adlog.alreadyadd = 1;
									  }
										else if(p.title=='用户日志'){
												if(store_logs_userlog.alreadyadd != 1)
												Ext.getCmp('logs_userlog').add(Ext.create('logs.userlog'));
				            		Ext.getCmp('page_main').bind(store_logs_userlog);
				                Ext.getCmp('page_main').setVisible(true);
				                Ext.getCmp('page_main').doRefresh();
												store_logs_userlog.alreadyadd = 1;
									  }
										else if(p.title=='短信日志'){
												if(store_logs_smslog.alreadyadd != 1)
												Ext.getCmp('logs_smslog').add(Ext.create('logs.smslog'));
				            		Ext.getCmp('page_main').bind(store_logs_smslog);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_logs_smslog.alreadyadd = 1;
									  }
										else if(p.title=='短信发送报表'){
												if(store_reports_smsreport.alreadyadd != 1)
												Ext.getCmp('reports_smsreport').add(Ext.create('reports.smsReport'));
				            		Ext.getCmp('page_main').bind(store_reports_smsreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_smsreport.alreadyadd = 1;
									  }
										else if(p.title=='短信发送详情'){
												if(store_reports_smsdetailreport.alreadyadd != 1)
												Ext.getCmp('reports_smsdetailreport').add(Ext.create('reports.smsDetailReport'));
				            		Ext.getCmp('page_main').bind(store_reports_smsdetailreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_smsdetailreport.alreadyadd = 1;
									  }
										else if(p.title=='上网人数流量报表'){
												if(store_reports_webreport.alreadyadd != 1)
												Ext.getCmp('reports_webreport').add(Ext.create('reports.webReport'));
												Ext.getCmp('page_main').bind(store_reports_webreport);
					              Ext.getCmp('page_main').setVisible(true);
					              Ext.getCmp('page_main').doRefresh();
												store_reports_webreport.alreadyadd = 1;
										}
										else if(p.title=='上网详情'){
												if(store_reports_webdetailreport.alreadyadd != 1)
												Ext.getCmp('reports_webdetailreport').add(Ext.create('reports.webDetailReport'));
				            		Ext.getCmp('page_main').bind(store_reports_webdetailreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_webdetailreport.alreadyadd = 1;
									  }
										else if(p.title=='设备类型统计'){
												if(store_reports_devicereport.alreadyadd != 1)
												Ext.getCmp('reports_devicereport').add(Ext.create('reports.deviceReport'));
				            		Ext.getCmp('page_main').bind(store_reports_devicereport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_devicereport.alreadyadd = 1;
									  }
									  else if(p.title=='SSID统计报表'){
												if(store_reports_ssidreport.alreadyadd != 1)
												Ext.getCmp('reports_ssidreport').add(Ext.create('reports.ssidReport'));
				            		Ext.getCmp('page_main').bind(store_reports_ssidreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_ssidreport.alreadyadd = 1;
									  }
										else if(p.title=='终端品牌'){
											if(store_charts_devicebrand.alreadyadd != 1)
											Ext.getCmp('charts_devicebrand').add(Ext.create('charts.deviceBrand_taibao'));
									            		Ext.getCmp('page_main').bind(store_charts_devicebrand);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_devicebrand.alreadyadd = 1;
									            	}
										else if(p.title=='终端类型'){
											if(store_charts_devicetype.alreadyadd != 1)
											Ext.getCmp('charts_devicetype').add(Ext.create('charts.deviceType_taibao'));
									            		Ext.getCmp('page_main').bind(store_charts_devicetype);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_devicetype.alreadyadd = 1;
									            	}
										else if(p.title=='商家短信'){
											if(store_charts_groupsms.alreadyadd != 1)
											Ext.getCmp('charts_groupsms').add(Ext.create('charts.groupSms'));
									            		Ext.getCmp('page_main').bind(store_charts_groupsms);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_groupsms.alreadyadd = 1;
									            	}
										else if(p.title=='终端操作系统'){
											if(store_charts_deviceos.alreadyadd != 1)
											Ext.getCmp('charts_deviceos').add(Ext.create('charts.deviceOS'));
									            		Ext.getCmp('page_main').bind(store_charts_deviceos);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_deviceos.alreadyadd = 1;
									            	}
										else if(p.title=='广告点击'){
											if(store_charts_advisit.alreadyadd != 1)
											Ext.getCmp('charts_advisit').add(Ext.create('charts.adVisit'));
									            		Ext.getCmp('page_main').bind(store_charts_advisit);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_advisit.alreadyadd = 1;
									            	}
										else if(p.title=='短信数量'){
									            		//Ext.getCmp('page_main').bind(store_rules_smsnumber);
											Ext.getCmp('page_main').setVisible(false);
											//Ext.getCmp('page_main').doRefresh();
									            	}
										else if(p.title=='广告管理'){
											
									            		Ext.getCmp('page_main').bind(store_shops_admanager);
											if(store_shops_admanager.alreadyadd != 1){
											Ext.getCmp('shops_admanager').add(Ext.create('shops.adManager'));
											doreflashTab(plateid);
											}
											store_shops_admanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_admanager.alreadyadd = 1;
									            	}
										else if(p.title=='广告页面管理'){
										    Ext.getCmp('page_main').bind(store_shops_advmanager);
												if(store_shops_advmanager.alreadyadd != 1){
														Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager'));
														doreflashTab(plateid);
												}
												store_shops_advmanager.new_params={advmanager_plateid:plateid};
												//Ext.getCmp('page_main').doRefresh();
												eval('store_shops_advmanager').load({
													  params:{},
														callback:function(records, options, success){ 
																eval(shops.advManager).init()
														}
												})
												eval(shops.advManager).init();
												//Ext.getCmp('page_main').setVisible(true);
												store_shops_advmanager.alreadyadd = 1;
									  }
										/*else if(p.title=='广告页面管理2'){
											
									            		Ext.getCmp('page_main').bind(store_shops_advmanager2);
											if(store_shops_advmanager2.alreadyadd != 1){
											Ext.getCmp('shops_advmanager2').add(Ext.create('shops.advManager2'));
											doreflashTab(plateid);
											}
											store_shops_advmanager2.new_params={advmanager2_plateid:plateid};
											//Ext.getCmp('page_main').doRefresh();
											eval('store_shops_advmanager2').load({params:{},
											callback:function(records, options, success){ 
											eval(shops.advManager2).init();}
											})
											eval(shops.advManager2).init();
											//Ext.getCmp('page_main').setVisible(true);
											store_shops_advmanager2.alreadyadd = 1;
									            	}
									  else if(p.title=='广告页面管理3'){
											
									            		Ext.getCmp('page_main').bind(store_shops_advmanager3);
											if(store_shops_advmanager3.alreadyadd != 1){
											Ext.getCmp('shops_advmanager3').add(Ext.create('shops.advManager3'));
											doreflashTab(plateid);
											}
											store_shops_advmanager3.new_params={advmanager3_plateid:plateid};
											//Ext.getCmp('page_main').doRefresh();
											eval('store_shops_advmanager3').load({params:{},
											callback:function(records, options, success){ 
											eval(shops.advManager3).init();}
											})
											eval(shops.advManager3).init();
											//Ext.getCmp('page_main').setVisible(true);
											store_shops_advmanager3.alreadyadd = 1;
									            	}*/
										else if(p.title=='商店管理'){
											if(store_shops_shopmanager.alreadyadd != 1)
											Ext.getCmp('shops_shopmanager').add(Ext.create('shops.shopManager'));
									            		Ext.getCmp('page_main').bind(store_shops_shopmanager);
											store_shops_shopmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											
											Ext.getCmp('page_main').setVisible(true);
											store_shops_shopmanager.alreadyadd = 1;
									            	}
										else if(p.title=='品牌管理'){
											if(store_shops_brandmanager.alreadyadd != 1)
											Ext.getCmp('shops_brandmanager').add(Ext.create('shops.brandManager'));
									            		Ext.getCmp('page_main').bind(store_shops_brandmanager);	
											store_shops_brandmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_brandmanager.alreadyadd = 1;
									            	}
										else if(p.title=='商品管理'){
											if(store_shops_oditymanager.alreadyadd != 1)
											Ext.getCmp('shops_oditymanager').add(Ext.create('shops.odityManager'));
											Ext.getCmp('page_main').bind(store_shops_oditymanager);
											Ext.getCmp('page_main').setVisible(true);
											store_shops_oditymanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											store_shops_oditymanager.alreadyadd = 1;
									            	}
										else if(p.title=='商标类别管理'){	
											if(store_shops_groupmanager.alreadyadd != 1)
											Ext.getCmp('shops_categorymanager').add(Ext.create('shops.categoryManager'));
											Ext.getCmp('page_main').bind(store_shops_groupmanager);
											store_shops_groupmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_groupmanager.alreadyadd = 1;
									            	}
										else if(p.title=='会员管理'){	
											if(store_shops_shopusermanager.alreadyadd != 1)
											Ext.getCmp('shops_usermanager').add(Ext.create('shops.userManager'));
											Ext.getCmp('page_main').bind(store_shops_shopusermanager);
											store_shops_shopusermanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_shopusermanager.alreadyadd = 1;
									            	}
										else if(p.title=='活动管理'){
											if(store_shops_activemanager.alreadyadd != 1)
											Ext.getCmp('shops_actmanager').add(Ext.create('shops.actManager'));
											Ext.getCmp('page_main').bind(store_shops_activemanager);
											store_shops_activemanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_activemanager.alreadyadd = 1;
									            	}
										else if(p.title=='晋级管理'){
											if(store_shops_compmanager.alreadyadd != 1)
											Ext.getCmp('shops_compmanager').add(Ext.create('shops.compManager'));
									        Ext.getCmp('page_main').bind(store_shops_compmanager);
											store_shops_compmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_compmanager.alreadyadd = 1;
									            	}   
										else if(p.title=='短信管理'){
											if(store_shops_smsmanager.alreadyadd != 1)
											Ext.getCmp('shops_smsmanager').add(Ext.create('shops.smsManager'));
									        Ext.getCmp('page_main').bind(store_shops_smsmanager);
											store_shops_smsmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_smsmanager.alreadyadd = 1;
									            	}
									  else if(p.title=='地图标签管理'){
											if(store_shops_markmanager.alreadyadd != 1)
											Ext.getCmp('shops_markmanager').add(Ext.create('shops.markManager'));
											Ext.getCmp('page_main').bind(store_shops_markmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_shops_markmanager.alreadyadd = 1;
									  } 
									   else if(p.title=='商家联盟管理'){
											if(store_shops_shoptypemanager.alreadyadd != 1)
											Ext.getCmp('shops_shoptypemanager').add(Ext.create('shops.shoptypeManager'));
											Ext.getCmp('page_main').bind(store_shops_shoptypemanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_shops_shoptypemanager.alreadyadd = 1;
									  } 
										else if(p.title=='购买短信'){
											if(store_shops_smsmanager.alreadyadd != 1)
											Ext.getCmp('myaccount_smsmanager').add(Ext.create('myaccount.buySms'));
									            		Ext.getCmp('page_main').bind(store_shops_smsmanager);
											store_shops_smsmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_smsmanager.alreadyadd = 1;
									            	}  	 						
										else if(p.title=='国内外短信数量'){
											if(store_rules_smsnumber.alreadyadd != 1)
											Ext.getCmp('rules_smsnumber').add(Ext.create('rules.smsNumber'));
									            		Ext.getCmp('page_main').bind(store_rules_smsnumber);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsnumber.alreadyadd = 1;
									            	}
										else if(p.title=='短信注册数量'){
											if(store_rules_smsregister.alreadyadd != 1)
											Ext.getCmp('rules_smsregister').add(Ext.create('rules.smsRegister'));
									            		Ext.getCmp('page_main').bind(store_rules_smsregister);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsregister.alreadyadd = 1;
									            	}
										else if(p.title=='上网时长'){
											if(store_rules_timelimit.alreadyadd != 1)
											Ext.getCmp('rules_timelimit').add(Ext.create('rules.timeLimit'));
									            		Ext.getCmp('page_main').bind(store_rules_timelimit);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_timelimit.alreadyadd = 1;
									            	}
										else if(p.title=='短信格式'){
											if(store_rules_smsformat.alreadyadd != 1)
											Ext.getCmp('rules_smsformat').add(Ext.create('rules.smsFormat'));
									            		Ext.getCmp('page_main').bind(store_rules_smsformat);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsformat.alreadyadd = 1;
									            	}
										else if(p.title=='黑名单'){
											if(store_rules_blackuser.alreadyadd != 1)
											Ext.getCmp('rules_blackuser').add(Ext.create('rules.blackUser'));
									            		Ext.getCmp('page_main').bind(store_rules_blackuser);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_blackuser.alreadyadd = 1;
									            	}
									  else if(p.title=='黑白名单'){
												if(store_rules_white.alreadyadd != 1)
												Ext.getCmp('rules_white').add(Ext.create('rules.white'));
					                      		
												store_rules_white.alreadyadd = 1;
                    }
                    else if(p.title=='模板管理'){
												if(store_rules_plate.alreadyadd != 1)
												Ext.getCmp('rules_plate').add(Ext.create('rules.plate'));
					              Ext.getCmp('page_main').bind(store_rules_plate);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_rules_plate.alreadyadd = 1;
                    }
										else if(p.title=='用户管理'){
											if(store_users_usermanager.alreadyadd != 1)
											Ext.getCmp('users_usermanager').add(Ext.create('users.userManager'));
									            		Ext.getCmp('page_main').bind(store_users_usermanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_users_usermanager.alreadyadd = 1;
									            	}
										else if(p.title=='标签管理'){
									            		Ext.getCmp('page_main').bind(store_users_tagmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
									            	}
										else if(p.title=='单位管理'){
											if(store_users_groupmanager.alreadyadd != 1)
											Ext.getCmp('users_groupmanager').add(Ext.create('users.groupManager'));
											Ext.getCmp('page_main').bind(store_users_groupmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_users_groupmanager.alreadyadd = 1;
									            	}
										else if(p.title=='系统用户管理'){
											
										}
										else if(p.title=='AP管理'){
											if(store_users_apmanager.alreadyadd != 1)
											Ext.getCmp('users_apmanager').add(Ext.create('users.apManager'));
											Ext.getCmp('page_main').bind(store_users_apmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_users_apmanager.alreadyadd = 1;
									            	}
										else if(p.title=='SSID管理'){
												if(store_users_ssidmanager.alreadyadd != 1)
												Ext.getCmp('users_ssidmanager').add(Ext.create('users.ssidManager'));
												Ext.getCmp('page_main').bind(store_users_ssidmanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_ssidmanager.alreadyadd = 1;
								    }
								    else if(p.title=='微信设置'){
												if(store_settings_weixin.alreadyadd != 1)
												Ext.getCmp('settings_weixin').add(Ext.create('settings.weixin'));
												Ext.getCmp('page_main').bind(store_settings_weixin);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_settings_weixin.alreadyadd = 1;
								    }
								    else if(p.title=='规则设置'){
												if(store_settings_par.alreadyadd != 1)
												Ext.getCmp('settings_par').add(Ext.create('settings.par'));
					                      		
												store_settings_par.alreadyadd = 1;
                    }
			            	var t_id=p.getId(); 
			            	 /*
			            	 if(t_id!='ssjk'){
			            	 	 bStartim=0;              
			                 bStartcomp=0;
			                 //如果左边的树不是check树，改变树
			                         //改变左边的树
											     var tabs_tree=Ext.getCmp('treetab');
											     var tab_len=tabs_tree.items.length;
											     var tree_panel;
											     var tree_flag=0;
											     tabs_tree.items.each(function(item) { 
														if(item.id=='tree_comp'){
																	   		tree_flag=1;;
																	   	
																	   	}
											      });	
			                       if(tree_flag!=1){      
															  tabs_tree.remove(0);																	 				
																   tabs_tree.add({ closable: false,id:'tree_comp',border:false,items:[Ext.create(ncViewer.lanGroupTree)],title: '部门用户'}).show();																       
														 }                          
			            	}
			            	else{
									var tabs_center=Ext.getCmp("layout_center");
					        	    var active = tabs_center.getActiveTab();
					
							        	var active_id='lay_'+active.id;
							        	var tabs_if=Ext.getCmp(active_id).getActiveTab();
							           if(tabs_if.id=='immon'){
							           	bStartcomp=0;
							           	bStartim=1;
							          }
							          else if(tabs_if.id=='sjcomputer'){
							          	bStartcomp=1;
							          	bStartim=0;
							          }                      	
			            		 else if(tabs_if.id=='sspm'){
			            		 	  bStartcomp=0;
							          	bStartim=0;
							          	            	  			 //改变左边的树
											     var tabs_tree=Ext.getCmp('treetab');
											     var tab_len=tabs_tree.items.length;
											     var tree_panel;
											     var tree_flag=0;
												   tabs_tree.items.each(function(item) { 
												     
												      });	
												  tabs_tree.remove(0);
												 				
													tabs_tree.add({
																	          closable: false,
																            id:'tree_comp_c',
																             border:false,
											                        items:[Ext.create(ncViewer.lanGroupCheckTree)],
																	            title: '部门上网人员'
													 }).show();
							          	
							          	
			            		 }
			            		
			            		
			            	}
			            	 */
		            	 	
		          	}
		        }      
        });
        return this.centerPanel;
    },
    createNcStatus:function(){   	
    		this.ncstatus=Ext.create('Ext.ux.StatusBar', {
            id: 'statusbar',
            region: 'south',
            margins: '',
						frame:false,
						border:false,
            defaultText: 'Default status text',
            text: 'Ready',
            iconCls: 'x-status-valid',
            items: [{
                xtype: 'button',
                text: '上海新网程信息技术股份有限公司版权所有&nbsp;&nbsp;',
               
                handler: function (){
                    var sb = Ext.getCmp('statusbar');
                    sb.setStatus({
                       text: 'Oops!',
                        iconCls: 'x-status-error',
                       clear: true // auto-clear after a set interval
                    });
                }
            }]
        });
        return this.ncstatus;
    },
    onItemClick:function(item){
				bbbb=Ext.getCmp('header');
				//bbbb.setTitle('<center>新网程登录门户管理系统 - '+item.text+'</center>');
				Ext.getCmp('leftAll').setTitle('<center>'+item.text+'</center>');
				/*
			    	 bbbb=document.all('ltitle');
			       bbbb.innerHTML='当前位置：'+item.text;
			       
			       
			       
			        //改变左边的树
			     var tabs_tree=Ext.getCmp('treetab');
			     var tab_len=tabs_tree.items.length;
			     var tree_panel;
			     var tree_flag=0;
			     tabs_tree.items.each(function(item) { 
						if(item.id=='tree_comp'){
									   		tree_flag=1;;
									   	
									   	}
			      });	
			      if(tree_flag!=1){      
				       tabs_tree.remove(0);	 				
					   tabs_tree.add({closable: false,id:'tree_comp',border:false,items:[Ext.create(ncViewer.lanGroupTree)], title: '部门上网用户'}).show();
			       
			     }
			   */    
 				//改变中间的视图      
       
				var i=0;
				var m=0;
				var tab_title=item.text;
				var tab_src=item.srcurl;
				var tab_id=item.itemId;
				var tabs_center=Ext.getCmp("layout_center");
				var tab_len=tabs_center.items.length;
        tabs_center.items.each(function(item) {
			
			// var itemStore = 'store_'+item.id;
			// if(eval(itemStore) != null){
			// eval(itemStore).alreadyload = 0;
			// }
	     	if(item.id==tab_id)
	     	{
			 i++;}
	       if(i==0){m++;}
        });   
	      if(i>0){
	     	  tabs_center.setActiveTab(m);
	     	  return; 
	     	} 
	      if(tab_len>=1){
	      	for(i=0;i<tab_len;i++)
	      	{     		
	      	 tabs_center.remove(tab_len-i-1); 
	        }      	
	      }  
				if(search_store !='')
    				eval(search_store).alreadyadd = 0;
				//销毁模板
				if(Ext.getCmp('plateTab'))
				Ext.getCmp('plateTab').destroy();
				//销毁区域树
				if(Ext.getCmp('areaTab'))
				Ext.getCmp('areaTab').destroy();
				Ext.getCmp('page_main').setVisible(false);
				if(tab_id=='ssjk'){
					tabs_center.add({autoScroll:false,id:tab_id,iconCls: 'add16',title: tab_title,items:[Ext.create(tab_src)]});   
				      Ext.getCmp('page_main').store=store_adminuser;
					    Ext.getCmp('page_main').doRefresh();
				 	        
				}   
				else if(tab_id=='onlineinfo'){  
					Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_online_onlineinfo"));  
					
					tabs_center.add({autoScroll:true,id:tab_id+'_onlineinfo',iconCls: 'onlineinfo',border:false,frame:false,title: "在线信息", items:[Ext.create('online.onlineInfo')]});
				}	
				else if(tab_id=='users'){ 
					store_users_usermanager.alreadyadd=0;
					
					tabs_center.add({autoScroll:true,id:tab_id+'_usermanager',iconCls: 'user',border:false,frame:false,title: "用户管理"});
				
					createAreaTab(); // 创建区域
				}
				else if(tab_id=='users_ap'){ 
					store_users_apmanager.alreadyadd=0;
					Ext.getCmp('page_main').setVisible(true); 
					if(right_fun.indexOf("AP管理")>0)tabs_center.add({autoScroll:true,id:'users_apmanager',iconCls: 'user',border:false,frame:false,title: "AP管理"});
				}
				else if(tab_id=='reports'){  
						store_reports_smsreport.alreadyadd=0;
						store_reports_smsdetailreport.alreadyadd=0;
						store_reports_webreport.alreadyadd=0;
						store_reports_webdetailreport.alreadyadd=0;
						store_reports_devicereport.alreadyadd=0;
						store_reports_ssidreport.alreadyadd=0;
					
			 	    tabs_center.add({autoScroll:true,id:tab_id+'_smsreport',iconCls: 'report',border:false,frame:false,title: "短信发送报表"});
			      tabs_center.add({autoScroll:true,id:tab_id+'_smsdetailreport',iconCls: 'report',border:false,frame:false,title: "短信发送详情"});
				    tabs_center.add({autoScroll:true,id:tab_id+'_webreport',iconCls: 'report',border:false,frame:false,title: "上网人数流量报表"});
			      tabs_center.add({autoScroll:true,id:tab_id+'_webdetailreport',iconCls: 'report',border:false,frame:false,title: "上网详情"});
			      tabs_center.add({autoScroll:true,id:tab_id+'_devicereport',iconCls: 'report',border:false,frame:false,title: "设备类型统计"});   
			      tabs_center.add({autoScroll:true,id:tab_id+'_ssidreport',iconCls: 'report',border:false,frame:false,title: "SSID统计报表"});   
				}
				else if(tab_id=='shops'){
						//store_shops_admanager.alreadyadd=0;
						store_shops_advmanager.alreadyadd=0;
						//store_shops_advmanager2.alreadyadd=0;
						//store_shops_advmanager3.alreadyadd=0;
						store_shops_shopmanager.alreadyadd=0;
						store_shops_brandmanager.alreadyadd=0;
						store_shops_oditymanager.alreadyadd=0;
						store_shops_groupmanager.alreadyadd=0;
						store_shops_shopusermanager.alreadyadd=0;
						store_shops_activemanager.alreadyadd=0;
						store_shops_compmanager.alreadyadd=0;
						store_shops_smsmanager.alreadyadd=0;
						store_shops_markmanager.alreadyadd=0;
						store_shops_shoptypemanager.alreadyadd=0;
						//Ext.getCmp('left').add(Ext.create('ncViewer.lanGroupTree'));
						
						//tabs_center.add({autoScroll:true,id:tab_id+'_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "广告管理"});
						tabs_center.add({autoScroll:true,id:tab_id+'_advmanager',iconCls: 'shops_ad',border:false,frame:false,title: "广告页面管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_advmanager2',iconCls: 'shops_ad',border:false,frame:false,title: "广告页面管理2"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_advmanager3',iconCls: 'shops_ad',border:false,frame:false,title: "广告页面管理3"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "商店管理"});	
						//tabs_center.add({autoScroll:true,id:tab_id+'_brandmanager',iconCls: 'star',border:false,frame:false,title: "品牌管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "商品管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_categorymanager',iconCls: 'shops_type',border:false,frame:false,title: "商标类别管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_usermanager',iconCls: 'shops_vip',border:false,frame:false,title: "会员管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_actmanager',iconCls: 'shops_active',border:false,frame:false,title: "活动管理"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_compmanager',iconCls: 'shops_comp',border:false,frame:false,title: "晋级管理"});
						//if(right_fun.indexOf("短信管理")>0)
						
						//tabs_center.add({autoScroll:true,id:'shops_smsmanager',iconCls: 'phone',border:false,frame:false,title: "短信管理"});
						//tabs_center.add({autoScroll:true,id:'shops_markmanager',iconCls: 'phone',border:false,frame:false,title: "地图标签管理"});
						//tabs_center.add({autoScroll:true,id:'shops_shoptypemanager',iconCls: 'shoptype',border:false,frame:false,title: "商家联盟管理"});
						createPlateTab();//创建模板
				} 
				else if(tab_id=='shops_ad'){//广告管理
					store_shops_advmanager.alreadyadd=0;
					//tabs_center.add({autoScroll:true,id:'shops_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "广告管理"});
					tabs_center.add({autoScroll:true,id:'shops_advmanager',iconCls: 'shops_ad',border:false,frame:false,title: "广告页面管理"});
					//设置默认选中模板
					for(i=0; i<store_portplate.getCount();i++){
						if(store_portplate.getAt(i).get('status')==0){
						plateid=store_portplate.getAt(i).get('sid');
						}
					}
				} 
				else if(tab_id=='shops_shop'){//商店管理
					store_shops_shopmanager.alreadyadd=0;
					tabs_center.add({autoScroll:true,id:'shops_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "商店管理"});	
				} 
				else if(tab_id=='shops_product'){//商品管理
					store_shops_oditymanager.alreadyadd=0;
					tabs_center.add({autoScroll:true,id:'shops_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "商品管理"});	
				} 
				else if(tab_id=='charts'){
					store_charts_devicebrand.alreadyadd=0;
					store_charts_devicetype.alreadyadd=0;
					store_charts_deviceos.alreadyadd=0;
					store_charts_advisit.alreadyadd=0;
					store_charts_groupsms.alreadyadd=0;
				 	tabs_center.add({autoScroll:true,id:'charts_devicebrand',iconCls:'chart_pie',border:false,frame:false,title: "终端品牌"});
					tabs_center.add({autoScroll:true,id:'charts_devicetype',iconCls:'chart_pie',border:false,frame:false,title: "终端类型"});	
					tabs_center.add({autoScroll:true,id:'charts_groupsms',iconCls:'chart_pie',border:false,frame:false,title: "商家短信"});	
					tabs_center.add({autoScroll:true,id:'charts_deviceos',iconCls:'chart_pie',border:false,frame:false,title: "终端操作系统"});	
				//	tabs_center.add({autoScroll:true,id:'charts_advisit',iconCls:'chart_pie',border:false,frame:false,title: "广告点击"});	
				
					// tabs_center.add({autoScroll:true,iconCls:'chart_pie',border:false,frame:false,title: "短信数量", items:[Ext.create('charts.smsRecount')]});			
					tabs_center.add(
					{
				          title: '短信数量',
						      iconCls: 'chart_curve',
				          id:tab_id+'_smsrecount',            
				          html: '<iframe  id="charts_smsrecount" name="charts_smsrecount" src="/ncm_chart/ncm_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      }
					);
				}
				else if(tab_id=='logs'){  
						store_logs_smslog.alreadyadd=0;
						store_logs_userlog.alreadyadd=0;
						store_logs_adlog.alreadyadd=0;
				 	        tabs_center.add({autoScroll:true,id:tab_id+'_smslog',iconCls: 'log',border:false,frame:false,title: "短信日志"});
				          tabs_center.add({autoScroll:true,id:tab_id+'_userlog',iconCls: 'log',border:false,frame:false,title: "用户日志"});
				      //    tabs_center.add({autoScroll:true,id:tab_id+'_adlog',iconCls: 'log',border:false,frame:false,title: "广告日志"}); 
				
				} 
				else if(tab_id=='rules'){ 
					store_rules_smsnumber.alreadyadd=0;
					store_rules_smsregister.alreadyadd=0;
					store_rules_timelimit.alreadyadd=0;
					store_rules_smsformat.alreadyadd=0;
					store_rules_blackuser.alreadyadd=0;
					store_rules_white.alreadyadd=0;
					store_users_ssidmanager.alreadyadd=0;
					store_rules_plate.alreadyadd=0;
					
				//	tabs_center.add({autoScroll:true,id:tab_id+'_smsnumber',iconCls: 'rule',border:false,frame:false,title: "国内外短信数量"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_smsregister',iconCls: 'rule',border:false,frame:false,title: "短信注册数量"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_timelimit',iconCls: 'rule',border:false,frame:false,title: "上网时长"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_smsformat',iconCls: 'rule',border:false,frame:false,title: "短信格式"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_blackuser',iconCls: 'rule',border:false,frame:false,title: "黑名单"});
					tabs_center.add({autoScroll:true,id:tab_id+'_white',iconCls: 'rule',border:false,frame:false,title: "黑白名单"});
					tabs_center.add({autoScroll:true,id:'users_ssidmanager',iconCls: 'rule',border:false,frame:false,title: "SSID管理"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_plate',iconCls: 'rule',border:false,frame:false,title: "模板管理"});
				}
				else if(tab_id=='settings'){
					store_settings_weixin.alreadyadd=0;
					store_settings_par.alreadyadd=0;
					
					tabs_center.add({          
				          title: 'Portal参数',
						      iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmPortal_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
				      },{          
				          title: '短信服务器',
						      iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmSms_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'    
				      },{          
				          title: 'HTTP服务器',
						     iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmHttp_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
				      },{          
				          title: 'AC设置',
						      iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmAC_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      },{          
				          title: 'Radius设置',
						      iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmRadius_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      },{          
				          title: '网络设置',
						      iconCls: 'set',
				          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmWebSystemSet_base" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
				      }
					);
				//	tabs_center.add({autoScroll:true,id:tab_id+'_par',iconCls: 'set',border:false,frame:false,title: "规则设置"});
				//	tabs_center.add({autoScroll:true,id:tab_id+'_weixin',iconCls: 'set',border:false,frame:false,title: "微信设置"});
				}
				else if(tab_id=='systems'){  
					store_users_groupmanager.alreadyadd=0;
					store_users_apmanager.alreadyadd=0;
					if(right_fun.indexOf("单位管理")>0)tabs_center.add({autoScroll:true,id:'users_groupmanager',iconCls: 'user',border:false,frame:false,title: "单位管理"});
				//		tabs_center.add({autoScroll:true,id:'users_apmanager',iconCls: 'user',border:false,frame:false,title: "AP管理"});
					tabs_center.add({
				          title: '系统用户管理',
									iconCls: 'system',
				          id:tab_id+'_xtyhmenu',            
				          html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      });
					if(username == "admin")tabs_center.add({
				          title: '系统权限管理',
						iconCls: 'system',
				          id:tab_id+'_rolemenu',           
				          html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					});
					tabs_center.add({
				          title: '用户访问日志',
						iconCls: 'system',
				          id:tab_id+'_usvisitmenu',            
				          html: '<iframe  id="lay_rzck_urllog" name="lay_rzck_urllog" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      });
				
				}
				else if(tab_id=='myaccount'){  
					
					tabs_center.add(
					{
				          title: '修改密码', 
						iconCls: 'key',
				          id:'myaccount_modipassword',           
				          html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/pronline/Msg?FunName@ncmWebDispModiPass" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				      }
				//        ,
				//		{
				//            title: '上传图片', 
				//			iconCls: 'key',
				//            id:'myaccount_uploadimg',           
				//            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/uploadimg/index.html" scrolling="yes" width=100% height=950  FrameBorder=0 ></iframe>' 
				//        }
					
					);
					
					//if(username != "admin")tabs_center.add({autoScroll:true,id:'myaccount_smsmanager',iconCls: 'phone',border:false,frame:false,title: "购买短信"});
				}
				else{
				      tabs_center.add({autoScroll:false,border:false,frame:false,id:tab_id,iconCls: 'add16',title: tab_title,items:[Ext.create(tab_src)]}); 
					
				}
				tabs_center.setActiveTab(0);
		}
});

