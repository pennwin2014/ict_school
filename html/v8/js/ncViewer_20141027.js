var plateid=4; //ģ��id
var intervalId;
var macId;

// ���������
var ua = navigator.userAgent.toLowerCase();  
function check(r){  
  return r.test(ua);  
}  
function getBrowserName()  
{  
  var browserName;  
  var isOpera = check(/opera/);  
  var isChrome = check(/chrome/);  
  var isFirefox = check(/firefox/);  
  var isWebKit = check(/webkit/);  
  var isSafari = !isChrome && check(/safari/);  
  var isIE = !isOpera && check(/msie/);  
  var isIE7 = isIE && check(/msie 7/);  
  var isIE8 = isIE && check(/msie 8/);  
  var isIE9 = isIE && check(/msie 9/);
  var isIE10 = isIE && check(/msie 10/);     
  
  if(isIE10)  
  {  
    browserName = "IE10";    
  }
  else if(isIE9)  
  {  
    browserName = "IE9";    
  }
  else if(isIE8)  
  {  
    browserName = "IE8";    
  }else if(isIE7)  
  {  
    browserName = "IE7";  
  }else if(isIE)  
  {  
    browserName = "IE";  
  }else if(isChrome)  
  {  
    browserName = "Chrome";  
  }else if(isFirefox)  
  {  
    browserName = "Firefox";  
  }else if(isOpera)  
  {  
    browserName = "Opera";  
  }else if(isWebKit)  
  {  
    browserName = "WebKit";  
  }else if(isSafari)  
  {  
    browserName = "Safari";  
  }else  
  {  
    browserName = "Others";  
  }  
  return browserName;  
}   
//alert(getBrowserName());

//����ѡ��ģ��  
store_portplate.load({params:{},callback:function(records, options, success){
				for(i=0; i<store_portplate.getCount();i++){
						if(store_portplate.getAt(i).get('status')==0){
								plateid=store_portplate.getAt(i).get('sid');
						}
				}	
		}
});

//����ģ���ǩ
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
						string2 = string2+"{style:{marginTop:'2px',marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate"+i+"', hidden:true,text:'ģ��1',handler:function(){plateid = store_portplate.getAt("+i+").get('sid');doreflashTab(store_portplate.getAt("+i+").get('sid'));"+string+"Ext.getCmp('plate"+i+"').addCls('stripes glow');}}"
				}
				else{
						string2 = string2+",{style:{marginBottom:'2px',marginLeft:'2px'},width:118,xtype:'button', id:'plate"+i+"', hidden:true,text:'ģ��1',handler:function(){plateid = store_portplate.getAt("+i+").get('sid');doreflashTab(store_portplate.getAt("+i+").get('sid'));"+string+"Ext.getCmp('plate"+i+"').addCls('stripes glow');}}"
				}
		}
		string2=string2+"]";
		
		return eval(string2);
}

function createPlateTab(){
		 Ext.getCmp('leftAll').setWidth(125);
		 Ext.getCmp('treetab').setWidth(123);
		 Ext.getCmp('treetab').add({
						id:'plateTab',
						closable: false,
					  border:false,
            items:createPlateItems(),
						/*tbar:[{width:118,xtype:'button', id:'plate_apply', hidden:false, text:'Ӧ��',handler:function(){
							store_portplate.load({params:{update:'update',sid:plateid}
							,callback:function(records, options, success){
							for(i=0; i<store_portplate.getCount();i++){
							Ext.getCmp('plate'+i).setText(store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).removeCls('stripes glow stripes2');
							}
							for(i=0; i<store_portplate.getCount();i++){
							if(store_portplate.getAt(i).get('status') == 0){
							//Ext.getCmp('plate_apply').setText('Ӧ��: '+store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).setText('Ӧ��: '+store_portplate.getAt(i).get('platename'));
							Ext.getCmp('plate'+i).addCls('stripes2');
							}
							}
							}
							});
						
						}}],*/
						title: 'ģ��'
		 });
		
		//��ȡ�޸�ģ������
		for(i=0;i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).setText(store_portplate.getAt(i).get('platename'));
		}
		//����Ĭ��ѡ��ģ��
		/*for(i=0; i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).removeCls('stripes glow');
			if(store_portplate.getAt(i).get('status')==0){
			Ext.getCmp('plate'+i).addCls('stripes glow');
			plateid=store_portplate.getAt(i).get('sid');
			}
		}*/
		//alert(plateid);
		//��ʾģ��
		for(i=0; i<store_portplate.getCount();i++){
			Ext.getCmp('plate'+i).setVisible(true); 
		}
		//��ǰӦ��ģ��
		/*for(i=0; i<store_portplate.getCount();i++){
			if(store_portplate.getAt(i).get('status') == 0){
				//Ext.getCmp('plate'+i).setText(store_portplate.getAt(i).get('platename'));
				Ext.getCmp('plate'+i).setText('Ӧ��: '+store_portplate.getAt(i).get('platename'));
				Ext.getCmp('plate'+i).addCls('stripes2');
			}
		}*/
		Ext.getCmp('treetab').setActiveTab(1);
}

function createAreaTab(){
		Ext.getCmp('leftAll').setWidth(302);
		Ext.getCmp('treetab').setWidth(300);
		//Ext.getCmp('leftAll').expand();
		if(autodef == "false"){//20140728
				var aaa = Ext.get("layout_center").getHeight()-55;
				Ext.getCmp('treetab').add({
						title:'����',	
						id:'areaTab',           
		        html: '<iframe id="treetab_areatree" name="treetab_areatree" src="/v8/ncm_group_tree_ear.htm" scrolling="no" width=100% height="'+aaa+'"  FrameBorder=0 ></iframe>' 
				});
		}
		if(autodef == "true"){
				var hhh=Ext.get("layout_center").getHeight()-55;
				Ext.getCmp('treetab').add({
						title:'��֯�ܹ�',	
						id:'areaTab',           
		        html: '<iframe  id="html_areaTab" name="html_areaTab" src="/v8/ncm_systems_tree_autopri.htm" scrolling="no" width=100% height="'+hhh+'" FrameBorder=0 ></iframe>' 
				});	
		}
		Ext.getCmp('treetab').setActiveTab(1);	
}


//ԥ԰����ͳ����˵�
function createAnalysisTab(){
	  Ext.getCmp('leftAll').setWidth(302);
		Ext.getCmp('treetab').setWidth(300);
		var bbb = Ext.get("layout_center").getHeight()-55;
		Ext.getCmp('treetab').add({
				/*title:'ԥ԰����ͳ��',	
				id:'analysisTab',
				closable: false,
				border:false,           
        html: '<iframe  id="treetab_analysisTab" name="treetab_analysisTab" src="/v8/ncm_analysis_tree.htm" scrolling="no" width=100% height="'+bbb+'"  FrameBorder=0 ></iframe>'
				layout:'accordion',
				layoutConfig:{
						animate:false,
						titleCollapse:true,
						activeOnTop:false
			  },
			  defaults:{
			  		bodyStyle:"padding:5px 5px 5px 5px; background-color: #FFFFFF"	
			  },
			  /*items:[{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'ʵʱ��Ϣ'+'</center>',
			  		itemId:'ssxx',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick	
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'���ݱ���'+'</center>',
			  		itemId:'sjbb',
			  		style:{margin:'20px'},	
			  		handler:this.onItemClick
			  		//html:'<ul><li>��������</li><li>�ο�ʵʱ�ֲ�</li><li>��Ŀ�����</li><li>��Ⱥ����</li></ul>'
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'Portal����'+'</center>',
			  		itemId:'ptgl',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick		
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'�زĹ���'+'</center>',
			  		itemId:'scgl',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick		
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'Ȩ�޹���'+'</center>',
			  		itemId:'qxgl',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'����'+'</center>',
			  		itemId:'shzh',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick		
			  },{
			  	  xtype:'button',
			  	  width:260,
			  		text:'<center>'+'�˻�����'+'</center>',
			  		itemId:'zhgl',
			  		style:{margin:'20px'},
			  		handler:this.onItemClick		
			  }]*/
			  /*items:[{
			  	  style:{margin:'20px'},
			  		title:'<center>'+'ʵʱ��Ϣ'+'</center>'	
			  },{
			  	  style:{margin:'20px'},
			  		title:'<center>'+'ʵʱ��Ϣ'+'</center>',	
			  },{ 
			  	style:{margin:'20px'},
			  		title:'<center>'+'ʵʱ��Ϣ'+'</center>',	
			  }]*/
		});	
}

var search_store='';//��������
function search(){//��������
		
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
		//����ģ��
		if(Ext.getCmp('plateTab'))
		Ext.getCmp('plateTab').destroy();
		//����������
	  if(Ext.getCmp('areaTab'))
	  Ext.getCmp('areaTab').destroy();
		//��������ģ��
		// for(i=1; i<=store_portplate.getCount();i++){
		// Ext.getCmp('plate'+i).setVisible(false); 
		// }
		var tabname = Ext.getCmp('tb').getComponent('search').getRawValue();
		Ext.getCmp('leftAll').setTitle('<center>'+'��֯�ܹ�����'+'</center>');
		if(tabname == '�û�����'){
			if(store_users_usermanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_usermanager',iconCls: 'user',border:false,frame:false,title: "�û�����", items:[Ext.create('users.userManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_users_usermanager"));  
			Ext.getCmp('page_main').setVisible(true);			
			Ext.getCmp('page_main').doRefresh();
			store_users_usermanager.alreadyadd = 1;
			search_store = 'store_users_usermanager';
			createAreaTab();
		}
		/*else if(tabname == '������'){
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "������", items:[]});
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
		else if(tabname == '���ҳ�����'){
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_advmanager',iconCls: 'shops_adv',border:false,frame:false,title: "���ҳ�����", items:[]});
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
		else if(tabname == '�̵����'){
			if(store_shops_shopmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "�̵����", items:[Ext.create('shops.shopManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopmanager.alreadyadd = 1;
			search_store = 'store_shops_shopmanager';
			createPlateTab();
		}
		else if(tabname == 'Ʒ�ƹ���'){
			if(store_shops_brandmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_brandmanager',iconCls: 'star',border:false,frame:false,title: "Ʒ�ƹ���", items:[Ext.create('shops.brandManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_brandmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_brandmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_brandmanager.alreadyadd = 1;
			search_store = 'store_shops_brandmanager';
			createPlateTab();
		}
		else if(tabname == '��Ʒ����'){
			if(store_shops_oditymanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "��Ʒ����", items:[Ext.create('shops.odityManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_oditymanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_oditymanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_oditymanager.alreadyadd = 1;
			search_store = 'store_shops_oditymanager';
			createPlateTab();
		}
		else if(tabname == '�������'){
			if(store_shops_groupmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_groupmanager',iconCls: 'shops_type',border:false,frame:false,title: "�������", items:[Ext.create('shops.groupManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_groupmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_groupmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_groupmanager.alreadyadd = 1;
			search_store = 'store_shops_groupmanager';
			createPlateTab();
		}
		else if(tabname == 'С�����'){
			if(store_shops_typemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_typemanager',iconCls: 'shops_type',border:false,frame:false,title: "С�����", items:[Ext.create('shops.typeManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_typemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_typemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_typemanager.alreadyadd = 1;
			search_store = 'store_shops_typemanager';
			createPlateTab();
		}
		else if(tabname == '��Ա����'){
			if(store_shops_shopusermanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopusermanager',iconCls: 'shops_vip',border:false,frame:false,title: "��Ա����", items:[Ext.create('shops.shopuserManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopusermanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopusermanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopusermanager.alreadyadd = 1;
			search_store = 'store_shops_shopusermanager';
			createPlateTab();
		}
		else if(tabname == '���ѹ���'){
			if(store_shops_consumpmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_consumpmanager',iconCls: 'shops_vip',border:false,frame:false,title: "���ѹ���", items:[Ext.create('shops.consumpManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_consumpmanager"));  
			Ext.getCmp('page_main').setVisible(true); 
			store_shops_shopusermanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_consumpmanager.alreadyadd = 1;
			search_store = 'store_shops_consumpmanager';
			createPlateTab();
		}
		else if(tabname == '�Ż݄�����'){
			if(store_shops_couponmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_couponmanager',iconCls: 'shops_vip',border:false,frame:false,title: "�Ż݄�����", items:[Ext.create('shops.couponManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_couponmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_couponmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_couponmanager.alreadyadd = 1;
			search_store = 'store_shops_couponmanager';
			createPlateTab();
		}
		else if(tabname == '�Ż݄�ʹ�ù���'){
			if(store_shops_couponusemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_couponusemanager',iconCls: 'shops_vip',border:false,frame:false,title: "�Ż݄�ʹ�ù���", items:[Ext.create('shops.couponuseManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_couponusemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_couponusemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_couponusemanager.alreadyadd = 1;
			search_store = 'store_shops_couponusemanager';
			createPlateTab();
		}
		else if(tabname == '�û���ע����'){
			if(store_shops_shopattentmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopattentmanager',iconCls: 'shops_vip',border:false,frame:false,title: "�û���ע����", items:[Ext.create('shops.shopattentManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopattentmanager"));  
			Ext.getCmp('page_main').setVisible(true); 
			store_shops_shopattentmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopattentmanager.alreadyadd = 1;
			search_store = 'store_shops_shopattentmanager';
			createPlateTab();
		}
		else if(tabname == '�û��ղع���'){
			if(store_shops_shopcollectmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_shopcollectmanager',iconCls: 'shops_vip',border:false,frame:false,title: "�û��ղع���", items:[Ext.create('shops.shopcollectManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_shopcollectmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_shopcollectmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_shopcollectmanager.alreadyadd = 1;
			search_store = 'store_shops_shopcollectmanager';
			createPlateTab();
		}
		else if(tabname == '���������ҳ'){
			if(store_shops_activemanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_activemanager',iconCls: 'shops_active',border:false,frame:false,title: "���������ҳ", items:[Ext.create('shops.activeManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_activemanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_activemanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_activemanager.alreadyadd = 1;
			search_store = 'store_shops_activemanager';
			createPlateTab();
		}
		else if(tabname == '���������ϸ'){
			if(store_shops_seriesmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'shops_seriesmanager',iconCls: 'shops_active',border:false,frame:false,title: "���������ϸ", items:[Ext.create('shops.seriesManager')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_shops_seriesmanager"));  
			Ext.getCmp('page_main').setVisible(true);
			store_shops_seriesmanager.new_params={plateid:plateid};
			Ext.getCmp('page_main').doRefresh();
			store_shops_seriesmanager.alreadyadd = 1;
			search_store = 'store_shops_seriesmanager';
			createPlateTab();
		}
		else if(tabname == '���ŷ��ͱ���'){
			if(store_reports_smsreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_smsreport',iconCls: 'report',border:false,frame:false,title: "���ŷ��ͱ���", items:[Ext.create('reports.smsReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_smsreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_smsreport.alreadyadd = 1;
			search_store = 'store_reports_smsreport';
		}
		else if(tabname == '���ŷ�������'){
			if(store_reports_smsdetailreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_smsdetailreport',iconCls: 'report',border:false,frame:false,title: "���ŷ�������", items:[Ext.create('reports.smsDetailReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_smsdetailreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_smsdetailreport.alreadyadd = 1;
			search_store = 'store_reports_smsdetailreport';
		}
		else if(tabname == '����������������'){
			if(store_reports_webreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_webreport',iconCls: 'report',border:false,frame:false,title: "����������������", items:[Ext.create('reports.webReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_webreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_webreport.alreadyadd = 1;
			search_store = 'store_reports_webreport';
		}
		else if(tabname == '��������'){
			if(store_reports_webdetailreport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_webdetailreport',iconCls: 'report',border:false,frame:false,title: "��������", items:[Ext.create('reports.webDetailReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_webdetailreport"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_reports_webdetailreport.alreadyadd = 1;
			search_store = 'store_reports_webdetailreport';
		}
		else if(tabname == '�豸����ͳ��'){
			if(store_reports_devicereport.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'reports_devicereport',iconCls: 'report',border:false,frame:false,title: "�豸����ͳ��", items:[Ext.create('reports.deviceReport')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_reports_devicereport"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();			
			store_reports_devicereport.alreadyadd = 1;
			search_store = 'store_reports_devicereport';
		}
		else if(tabname == '�û���־'){
			if(store_logs_userlog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_userlog',iconCls: 'log',border:false,frame:false,title: "�û���־", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_userlog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_userlog.alreadyadd = 1;
			search_store = 'store_logs_userlog';
		}
		else if(tabname == '������־'){
			if(store_logs_smslog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_smslog',iconCls: 'log',border:false,frame:false,title: "������־", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_smslog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_smslog.alreadyadd = 1;
			search_store = 'store_logs_smslog';
		}
		else if(tabname == '�����־'){
			if(store_logs_adlog.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'logs_adlog',iconCls: 'log',border:false,frame:false,title: "�����־", items:[Ext.create('logs.userlog')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_logs_adlog"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_logs_adlog.alreadyadd = 1;
			search_store = 'store_logs_adlog';
		}
		else if(tabname == '�ն�Ʒ��'){
			if(store_charts_devicebrand.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_devicebrand',iconCls: 'chart_pie',border:false,frame:false,title: "�ն�Ʒ��", items:[Ext.create('charts.deviceBrand')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_devicebrand"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_devicebrand.alreadyadd = 1;
			search_store = 'store_charts_devicebrand';
		}
		else if(tabname == '�ն�����'){
			if(store_charts_devicetype.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_deviceType',iconCls: 'chart_pie',border:false,frame:false,title: "�ն�����", items:[Ext.create('charts.deviceType')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_devicetype"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_devicetype.alreadyadd = 1;
			search_store = 'store_charts_devicetype';
		}
		else if(tabname == '�ն˲���ϵͳ'){
			if(store_charts_deviceos.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_deviceos',iconCls: 'chart_pie',border:false,frame:false,title: "�ն˲�������", items:[Ext.create('charts.deviceOS')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_deviceos"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_deviceos.alreadyadd = 1;
			search_store = 'store_charts_deviceos';
		}
		else if(tabname == '�����'){
			if(store_charts_advisit.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'charts_advisit',iconCls: 'chart_pie',border:false,frame:false,title: "�����", items:[Ext.create('charts.adVisit')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_charts_advisit"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_charts_advisit.alreadyadd = 1;
			search_store = 'store_charts_advisit';
		}
		else if(tabname == '��������'){
		tabs_center.add(
		{
            title: '��������',
			iconCls: 'chart_curve',
            id:'charts_smsrecount',            
            html: '<iframe  id="charts_smsrecount" name="charts_smsrecount" src="/ncm_chart/ncm_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == 'ʱ��η���'){
		tabs_center.add(
		{
            title: 'ʱ��η���',
			iconCls: 'chart_curve',
            id:'timegroupvisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_smsRecountChart_ch.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == 'ʵʱ�����������ͼ'){
		tabs_center.add(
		{
            title: 'ʵʱ�����������ͼ',
			iconCls: 'chart_curve',
            id:'realtimevisit',            
            html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_onlineUserRealtimeTrend.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '�������������'){
			if(store_rules_smsnumber.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsnumber',iconCls: 'rule',border:false,frame:false,title: "�������������", items:[Ext.create('rules.smsNumber')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsnumber"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsnumber.alreadyadd = 1;
			search_store = 'store_rules_smsnumber';
		}
		else if(tabname == '����ע������'){
			if(store_rules_smsregister.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsregister',iconCls: 'rule',border:false,frame:false,title: "����ע������", items:[Ext.create('rules.smsRegister')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsregister"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsregister.alreadyadd = 1;
			search_store = 'store_rules_smsregister';
		}
		else if(tabname == '����ʱ��'){
			if(store_rules_timelimit.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_timelimit',iconCls: 'rule',border:false,frame:false,title: "����ʱ��", items:[Ext.create('rules.timeLimit')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_timelimit"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_rules_timelimit.alreadyadd = 1;
			search_store = 'store_rules_timelimit';
		}
		else if(tabname == '���Ÿ�ʽ'){
			if(store_rules_smsformat.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_smsformat',iconCls: 'rule',border:false,frame:false,title: "���Ÿ�ʽ", items:[Ext.create('rules.smsFormat')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_smsformat"));  
			Ext.getCmp('page_main').setVisible(true); 
			Ext.getCmp('page_main').doRefresh();
			store_rules_smsformat.alreadyadd = 1;
			search_store = 'store_rules_smsformat';
		}
		else if(tabname == '������'){
			if(store_rules_blackuser.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'rules_blackuser',iconCls: 'rule',border:false,frame:false,title: "������", items:[Ext.create('rules.blackUser')]});
			Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_rules_blackuser"));  
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();		
			store_rules_blackuser.alreadyadd = 1;
			search_store = 'store_rules_blackuser';
		}
		else if(tabname == 'AP����'){
			if(store_users_apmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_apmanager',iconCls:'rule',border:false,frame:false,title: "AP����",items:[Ext.create('users.apManager')]});
            Ext.getCmp('page_main').bind(store_users_apmanager);
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_users_apmanager.alreadyadd = 1;
			search_store = 'store_users_apmanager';
		}
		else if(tabname == '��λ����'){
			if(store_users_groupmanager.alreadyadd != 1)
			Ext.getCmp("layout_center").add({autoScroll:true,id:'users_groupmanager',iconCls:'rule',border:false,frame:false,title: "��λ����",items:[Ext.create('users.groupManager')]});
            Ext.getCmp('page_main').bind(store_users_groupmanager);
			Ext.getCmp('page_main').setVisible(true);
			Ext.getCmp('page_main').doRefresh();
			store_users_groupmanager.alreadyadd = 1;
			search_store = 'store_users_groupmanager';
		}
		else if(tabname == 'ϵͳȨ�޹���'){
		tabs_center.add(
		{
            title: 'ϵͳȨ�޹���',
			      iconCls: 'system',
            id:'systems_rolemenu',           
            html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
		}
		);
		}
		else if(tabname == 'ϵͳ�û�����'){
		tabs_center.add(
		{
            title: 'ϵͳ�û�����',
			      iconCls: 'system',
            id:'systems_xtyhmenu',            
            html: '<iframe  id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '�û�������־'){
		tabs_center.add(
		{
            title: '�û�������־',
			      iconCls: 'system',
            id:'systems_usvisitmenu',            
            html: '<iframe  id="html_usvisit" name="html_usvisit" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
		);
		}
		else if(tabname == '�޸�����'){
		tabs_center.add(
		{
            title: '�޸�����', 
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
*����:��ʽ��ʱ��  
*ʾ��:DateUtil.Format("yyyy/MM/dd","Thu Nov 9 20:30:37 UTC+0800 2006 ");  
*����:2006/11/09  
*/  
DateUtil.Format=function(fmtCode,date){  
var result,d,arr_d;  

var patrn_now_1=/^y{4}-M{2}-d{2}\sh{2}:m{2}:s{2}$/;  
var patrn_now_11=/^y{4}-M{1,2}-d{1,2}\sh{1,2}:m{1,2}:s{1,2}$/;  

var patrn_now_2=/^y{4}\/M{2}\/d{2}\sh{2}:m{2}:s{2}$/;  
var patrn_now_22=/^y{4}\/M{1,2}\/d{1,2}\sh{1,2}:m{1,2}:s{1,2}$/;  

var patrn_now_3=/^y{4}��M{2}��d{2}��\sh{2}ʱm{2}��s{2}��$/;  
var patrn_now_33=/^y{4}��M{1,2}��d{1,2}��\sh{1,2}ʱm{1,2}��s{1,2}��$/;  

var patrn_date_1=/^y{4}-M{2}-d{2}$/;  
var patrn_date_11=/^y{4}-M{1,2}-d{1,2}$/;  

var patrn_date_2=/^y{4}\/M{2}\/d{2}$/;  
var patrn_date_22=/^y{4}\/M{1,2}\/d{1,2}$/;  

var patrn_date_3=/^y{4}��M{2}��d{2}��$/;  
var patrn_date_33=/^y{4}��M{1,2}��d{1,2}��$/;  

var patrn_time_1=/^h{2}:m{2}:s{2}$/;  
var patrn_time_11=/^h{1,2}:m{1,2}:s{1,2}$/;  
var patrn_time_2=/^h{2}ʱm{2}��s{2}��$/;  
var patrn_time_22=/^h{1,2}ʱm{1,2}��s{1,2}��$/;  

if(!fmtCode){fmtCode="yyyy/MM/dd hh:mm:ss";}  
if(date){  
d=new Date(date);  
if(isNaN(d)){  
msgBox("ʱ������Ƿ�\n��ȷ��ʱ��ʾ��:\nThu Nov 9 20:30:37 UTC+0800 2006\n��\n2006/      10/17");  
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
result=arr_d.yyyy+"��"+arr_d.MM+"��"+arr_d.dd+"��"+" "+arr_d.hh+"ʱ"+arr_d.mm+"��"+arr_d.ss+"��";  
}  
else if(patrn_now_33.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"��"+arr_d.MM+"��"+arr_d.dd+"��"+" "+arr_d.hh+"ʱ"+arr_d.mm+"��"+arr_d.ss+"��";  
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
result=arr_d.yyyy+"��"+arr_d.MM+"��"+arr_d.dd+"��";  
}  
else if(patrn_date_33.test(fmtCode))  
{  
arr_d=splitDate(d);  
result=arr_d.yyyy+"��"+arr_d.MM+"��"+arr_d.dd+"��";  
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
result=arr_d.hh+"ʱ"+arr_d.mm+"��"+arr_d.ss+"��";  
}  
else if(patrn_time_22.test(fmtCode)){  
arr_d=splitDate(d);  
result=arr_d.hh+"ʱ"+arr_d.mm+"��"+arr_d.ss+"��";  
}  
else{  
msgBox("û��ƥ���ʱ���ʽ!");  
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
yyyy=d.getFullYear();    //firefox �ر�
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
				yyyy=d.getFullYear();    //firefox �ر�
				MM=d.getMonth()+1;  
				dd=d.getDate();  
				hh=d.getHours();  
				mm=d.getMinutes();  
				ss=d.getSeconds();    
		}  
		return {"yyyy":yyyy,"MM":MM,"dd":dd,"hh":hh,"mm":mm,"ss":ss};    
}  
//��ʽ��ʱ��
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

  //���7��
now.endTime   = now.format( "yyyy/MM/dd", now.setDate( day ) );
now.beginTime = now.format( "yyyy/MM/dd", now.setDate( day-7 ) );

var	startday=now.beginTime;
var	today=now.endTime;

var grid_height; //��ʾ�����߶�
//����ģ��ID
function doreflashTab(plateid){
	//alert(111)
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
									eval(shops.advManager).init();}
					})
			}else{
				  if(plateid == 1949796946){  // ԥ԰demo
						  tabstore = 'store_'+tabframe+'3';
							Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager9'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager_plateid:plateid,mark:'A-1-1'};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager9).init();
									}
							})
					}else if(plateid == 1398571206){  // ��׼ģ��
			    	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager8'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager8).init();
									}
							})
			    }else if(plateid == 505170999){
			    	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager7'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager7).init();
									}
							})
			    }else if(plateid == 1089019303){
			    	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager6'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager6).init();
									}
							})
			    }else if(plateid == 1209111882){ // ̫��
				  	  tabstore = 'store_'+tabframe+'5';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager5'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager5_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager5).init();
									}
							})
				  }else if(plateid == 6){ // ��ԥ԰
						  tabstore = 'store_'+tabframe+'3';
						  Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager3'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager3_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager3).init();
									}
							})
					}else if(plateid == 5 || plateid == 1225573834 || plateid == 1088452031){//�����ģ��5��Ĭ�����̳�ģ��
						  tabstore = 'store_'+tabframe+'2';
							Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager2'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager2_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager2).init();
									}
							})
					}else{  // ԥ԰
						  tabstore = 'store_'+tabframe;
							Ext.getCmp('shops_advmanager').removeAll();
							Ext.getCmp('shops_advmanager').add(Ext.create('shops.advManager'));
							eval(tabstore).plateid = plateid;
							eval(tabstore).new_params={advmanager_plateid:plateid};
							eval(tabstore).load({
								  params:{},
									callback:function(records, options, success){ 
											eval(shops.advManager).init();
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

function doreflashTab1(areaid, groupid, groupcode){
		var tabs_obj=Ext.getCmp('layout_center');
		var index=tabs_obj.getActiveTab();
		var tabframe=index.id;
		if(tabframe == "systems_usvisitmenu"){
				document.getElementById("html_usvisit").contentWindow.document.getElementById('form1').groupcode.value = groupcode;
				document.getElementById("html_usvisit").contentWindow.reflash();
		}else if(tabframe == "systems_xtyhmenu"){
			  document.getElementById("lay_rzck_screenlog").contentWindow.document.getElementById('form1').groupcode.value = groupcode;
				document.getElementById("lay_rzck_screenlog").contentWindow.reflash();
		}else if(tabframe == "systems_rolemenu"){
			  document.getElementById("lay_rzck_imcontent").contentWindow.document.getElementById('form1').groupcode.value = groupcode;
				document.getElementById("lay_rzck_imcontent").contentWindow.reflash();
		}else{
				var tabstore = 'store_'+tabframe;
				eval(tabstore).areaid = areaid;
				eval(tabstore).groupid = groupid;
				eval(tabstore).groupcode = groupcode;
				eval(tabstore).new_params={areaid:areaid,groupid:groupid,groupcode:groupcode,mokeyword:""};
				eval(tabstore).load();
		}
}

//ͼ��鿴��ϸ��Ϣ
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
Ext.Loader.setConfig({enabled: true,paths:{'systems':'/v8/js/systems'}});

Ext.Loader.setConfig({enabled: true,paths:{'network':'/v8/js/network'}});
Ext.Loader.setConfig({enabled: true,paths:{'systemmanage':'/v8/js/systemmanage'}});
Ext.Loader.setConfig({enabled: true,paths:{'logs':'/v8/js/logs'}});
Ext.Loader.setConfig({enabled: true,paths:{'charts':'/v8/js/charts'}});
Ext.Loader.setConfig({enabled: true,paths:{'reports':'/v8/js/reports'}});
Ext.Loader.setConfig({enabled: true,paths:{'usermanger':'/v8/js/usermanger'}});
Ext.Loader.setConfig({enabled: true,paths:{'analysis':'/v8/js/analysis'}});
var new_params="";
//������չ���  
Ext.Loader.setConfig({enabled: true});  
 
Ext.require([  
    'Ext.ux.data.PagingMemoryProxy',  
    'Ext.ux.ProgressBarPager'  
]);  

var isFirstCreateNbTb = true;

/**
�״�ѡ�е�һ����ǩ
*/
function firstSelect()
{
    // �ڳ��γ�ʼ����ʱ��Ĭ��ѡ��"������Ϣ"
    if(isFirstCreateNbTb == true)
    {
        isFirstCreateNbTb = false;
        Ext.getCmp('tb').getComponent('online').addCls('stripes glow');
    		Ext.getCmp('leftAll').setVisible(false);
				Ext.getCmp('treetab').setVisible(false);
		    //tabs_center.setActiveTab(0);
    }
}

Ext.define('ncViewer.App', {
		extend: 'Ext.container.Viewport',    
		uses:['online.onlineInfo','online.onlinemac'],
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
		����������
		 */
		createNcTb: function(){
				//�������Բ˵�
				var menu_procy = Ext.create('Ext.menu.Menu', {
				    id: 'sfkglMenu',
				    style: {
				        overflow: 'visible'     
				    },
				    items: [{
				            text: '���Ʋ���',
				            itemId:'kzcl',
				            srcurl:'ncViewer.lanProcy',  
				            handler:this.onItemClick
				   
				    },{
				       	    text: '�����·�',
				            itemId:'clxf',
				            srcurl:'ncViewer.lanProcyXf',
				           handler:this.onItemClick
				    }]
				});

    	 	var tb = Ext.create('Ext.toolbar.Toolbar',{id:'tb',border:false,frame:false,style: 'background-image:url();'});
				tb.add({
						xtype:'label',
						html:'&nbsp;<img src="/images/large/background.jpg" width="2880" height="78" style="vertical-align:top;margin-top:-15px">',
						width:1
				},{
						xtype:'label',
						style:'margin-bottom:1px;margin-top:1px;margin-left:150px;'
				},{
						xtype:'button',
						style:'margin-bottom:1px;margin-top:1px;',
            itemId:'online',
            hidden:false,
            text:'������Ϣ',
            scale:'large',
            iconCls: 'zxxx',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmOnlineInfo',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'users',
            hidden:false,
            text:'�û�����',
						scale:'large',
            iconCls: 'users',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmUsers',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'shops',
            hidden:false,
            text:'�̳ǹ���',
						scale:'large',
            iconCls: 'shops',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmShops',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'shops_shop',
            hidden:false,
            text:'�̵����',
						scale:'large',
            iconCls: 'shop',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmShops',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'shops_ad',
            hidden:false,
            text:'������',
						scale:'large',
            iconCls: 'ad',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmShops',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'shops_product',
            hidden:false,
            text:'��Ʒ����',
						scale:'large',
            iconCls: 'product',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmShops',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'reports',
            hidden:false,
            text:'ͳ�Ʊ���',
            scale:'large',
            iconCls: 'reports',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmReports',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'charts',
            hidden:false,
            text:'ҵ���֪',
						scale:'large',
            iconCls: 'adcharts',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmCharts',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'logs',
            hidden:false,
            text:'��־��ѯ',
						scale:'large',
            iconCls: 'logs',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmLog',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'rules',
            hidden:false,
            text:'��������',
						scale:'large',
            iconCls: 'charts',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmSettings',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'settings',
            hidden:false,
            text:'ϵͳ����',
						scale:'large',
            iconCls: 'settings',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmSettings',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'systems',
            hidden:false,
            text:'ϵͳ����',
						scale:'large',
            iconCls: 'systems',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmSystems',
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'myaccount',
            hidden:false,
            text:'�ҵ��˺�',
						scale:'large',
            iconCls: 'accounts',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmAccounts'
            handler:this.onItemClick
        },{
						xtype:'button',
						style:'margin-bottom:1px',
            itemId:'yyanalysis',
            hidden:false,
            text:'ԥ԰����ͳ��',
						scale:'large',
            iconCls: 'reports',
            iconAlign: 'top',
            //srcurl:'ncViewer.ncmAnalysis'
            handler:this.onItemClick
        },

'->','->',{
		style:'margin-left:400px;',
		xtype:'combo',
		itemId:'search',
		store:store_search,
		//fieldLabel:'����',
            emptyText: '����������������',
		labelWidth: 30,
		width: 200,
		height:28,
		valueField:'groupid',
		displayField: 'groupname',    
		typeAhead: true,
		hideTrigger:true,
		hidden:true,
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
            tooltip: '����',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'search',
			hidden:true,
            handler: search
        },'-',{
		xtype:'combo',
		itemId:'skintheme',
		store:store_skintheme,
            emptyText: 'Ƥ������',
		labelWidth: 30,
		width: 100,
		valueField:'groupid',
		displayField: 'groupname',    
		typeAhead: true,
		hidden:true,
		listeners: {
		'change':function(){
		
		var fp=Ext.create('Ext.FormPanel');
		fp.form.doAction('submit',{
		url:'/pronline/Msg',
            method:'POST',
		params:{FunName:'ncm_skintheme',update:this.value,username:username}
		});
		
		Ext.Msg.confirm('�л�Ƥ������','�����µ�¼',
		function(btn){
		if(btn=='yes'){
		  self.location='/enter.html';							
		}else{
		  
		}
		},this);
		
		}
	}
        },'-','',{
		tooltip: '��������',
		width:50,
            itemId:'help',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'help',
            handler: this.onItemHelp
        },{
		tooltip: 'ȫ����ʾ',
		width:50,
            itemId:'max',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'hidden',
            handler: this.onItemMax
        },{
            tooltip: 'ϵͳˢ��',
		width:50,
            itemId:'xtsx',
            scale:'large',
            iconAlign: 'top',
            iconCls: 'refresh',
            handler: this.onItemReflash
        },{
            tooltip: '���µ�¼',
		width:50,
            scale:'large',
            itemId:'cxdl',
            iconAlign: 'top',
            iconCls: 'unlock',
		handler: this.onItemClose
        },{
	    	  itemId:'logo',
       		  xtype:'label'
		//html:'<center><div class="ain_3d"><span class="gallery">������</span><br><span class="gallery2">��¼�Ż�����ϵͳ</span></div></center>'
        }); 

        var right_fun_s=right_fun;
        var isep=0;
           	
        this.toolPanel = Ext.create(Ext.panel.Panel, {
            id: 'header',
            border: false,
						frame:false,
            region: 'north',
			      //bodyStyle: 'background:#e1e1e1;padding:1px 1px 1px 1px;',
            items:[tb]
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
		                       title:"������������",
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
		},
		onItemMax:function(){
		//Ext.getCmp('header').setTitle('<center>�����̵�¼�Ż�����ϵͳ</center>');
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
							waitMsg:'����ˢ������, ���Ժ�...', 
							waitTitle:'��ʾ', 
			
							params:{FunName:'ncm_system_reset'}, 
              success:function(form,action){
									Ext.Msg.alert('����',"ˢ�����!");
              },
             	failure:function(form,action){        		 
                  Ext.Msg.alert('����',"ϵͳ��æ,�Ժ�����!");
            	}
          })
    },
    createNcLeft:function(){
				var left_tabs=Ext.createWidget('tabpanel', {
				    width: 300,
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
										if(p.title=='ԥ԰����ͳ��'){
						            Ext.getCmp('leftAll').setWidth(302);
												Ext.getCmp('treetab').setWidth(300);	
						        }
										else if(p.title=='����'){
						            Ext.getCmp('leftAll').setWidth(302);
												Ext.getCmp('treetab').setWidth(300);	
						        }
										else if(p.title=='��֯�ܹ�'){
						            Ext.getCmp('leftAll').setWidth(302);
												Ext.getCmp('treetab').setWidth(300);	
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
            width: 302,
	          minSize: 100,
	          maxSize: 500,
            items:[]
        });
		 	  this.leftAll=Ext.create(Ext.panel.Panel, {
             id: 'leftAll',
             margins: '0 2 0 2',
             // layout: 'border',
             title:'<center>'+right_sy+'</center>',
             region: 'west',
             collapsible: true,
			 			 //collapsed: true,
             autoScroll: true,
             // bodyStyle: 'background:#fff;',
             split:true,
             width: 0,
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
						//deferredRender:false,
						bbar:[{
		           	xtype:'pagingtoolbar',
								border:false,
								frame:false,
		           	id:'page_main',
		           	hidden:true,
		            store:Ext.StoreManager.lookup('store_adminuser'),
		            beforePageText : "��",// update
                afterPageText : "ҳ  �� {0} ҳ",// update
                firstText : "��һҳ",
                prevText : "��һҳ",// update
                nextText : "��һҳ",
                lastText : "���ҳ",
                refreshText : "ˢ��",
		            displayInfo: true,
		            displayMsg: '��ʾ <font color=red>{0}</font> - <font color=red>{1}</font> ����  �� <font color=red>{2}</font> ��',
		            emptyMsg: "�� 0 ��",
								plugins: new Ext.ux.ProgressBarPager(),
								//plugins: Ext.create('Ext.ux.ProgressBarPager', {}),
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
		        	  xtype:'label', html:'<div style="color:white;padding-left:100px;padding-right:20px;padding-top:8px;padding-bottom:8px;background:url(/images/large/bottom.png);">�Ϻ���������Ϣ�����ɷ����޹�˾�н�</div>'
		        }],
		        
		        listeners: {        
		            tabchange:function(tp,p){ 
						    		grid_height=Ext.getCmp('layout_center').getHeight()-63;
										var tabs_center=Ext.getCmp("layout_center");
										var active = tabs_center.getActiveTab();
										Ext.getCmp('page_main').setVisible(false);
										Ext.getCmp('leftAll').setVisible(true);
										Ext.getCmp('treetab').setVisible(true);
										
										if(p.title=='Ȩ�޽�ɫ'){
												Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_system_role"));
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
										}
										else if(p.title=='������Ϣ'){
											  //if(store_online_onlineinfo.alreadyadd != 1)
											  	 // Ext.getCmp('online_onlineinfo').add(Ext.create('online.onlineInfo'));
											  //Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_online_onlineinfo"));
//												Ext.getCmp('page_main').bind(store_online_onlineinfo);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												Ext.getCmp('leftAll').setVisible(false);
						            Ext.getCmp('treetab').setVisible(false);
												//store_online_onlineinfo.alreadyadd = 1;						    
										}
										else if(p.title=='����Mac'){
												if(store_online_onlinemac.alreadyadd != 1)
													  Ext.getCmp('online_onlinemac').add(Ext.create('online.onlinemac'));
				            		Ext.getCmp('page_main').bind(store_online_onlinemac);
				                Ext.getCmp('page_main').setVisible(true);
				                Ext.getCmp('page_main').doRefresh();
				                Ext.getCmp('leftAll').setVisible(false);
						            Ext.getCmp('treetab').setVisible(false);
												store_online_onlinemac.alreadyadd = 1;
										}
										else if(p.title=='ϵͳ�û�'){
												Ext.getCmp('page_main').bind(store_adminuser);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
										}
										else if(p.title=='ϵͳ������־'){
												Ext.getCmp('page_main').bind(store_system_log);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
										}
										else if(p.title=='ϵͳ���˵�����'){
												if(store_systems_menu.alreadyadd != 1)
														Ext.getCmp('systems_menu').add(Ext.create('systems.menu'));
												Ext.getCmp('page_main').bind(store_systems_menu);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_systems_menu.alreadyadd = 1;
										}
										else if(p.title=='ϵͳ�Ӳ˵�����'){
												if(store_systems_submenu.alreadyadd != 1)
														Ext.getCmp('systems_submenu').add(Ext.create('systems.submenu'));
												Ext.getCmp('page_main').bind(store_systems_submenu);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_systems_submenu.alreadyadd = 1;
										}
										else if(p.title=='�����־'){
												if(store_logs_adlog.alreadyadd != 1)
															Ext.getCmp('logs_adlog').add(Ext.create('logs.adlog'));
												Ext.getCmp('page_main').bind(store_logs_adlog);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_logs_adlog.alreadyadd = 1;
										}
										else if(p.title=='�û���־'){
										if(store_logs_userlog.alreadyadd != 1)
											Ext.getCmp('logs_userlog').add(Ext.create('logs.userlog'));
										Ext.getCmp('page_main').bind(store_logs_userlog);
										Ext.getCmp('page_main').setVisible(true);
										Ext.getCmp('page_main').doRefresh();
										store_logs_userlog.alreadyadd = 1;
									  }
										else if(p.title=='������־'){
												if(store_logs_smslog.alreadyadd != 1)
												Ext.getCmp('logs_smslog').add(Ext.create('logs.smslog'));
				            		Ext.getCmp('page_main').bind(store_logs_smslog);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_logs_smslog.alreadyadd = 1;
									  }
									  else if(p.title=='�澯��־'){
												if(store_logs_alarmlog.alreadyadd != 1)
												Ext.getCmp('logs_alarmlog').add(Ext.create('logs.alarmlog'));
				            		Ext.getCmp('page_main').bind(store_logs_alarmlog);
				                Ext.getCmp('page_main').setVisible(true);
				                Ext.getCmp('page_main').doRefresh();
												store_logs_alarmlog.alreadyadd = 1;
									  }
									  else if(p.title=='MAC��־'){
												if(store_logs_maclog.alreadyadd != 1)
												Ext.getCmp('logs_maclog').add(Ext.create('logs.maclog'));
				            		Ext.getCmp('page_main').bind(store_logs_maclog);
				                Ext.getCmp('page_main').setVisible(true);
				                Ext.getCmp('page_main').doRefresh();
												store_logs_maclog.alreadyadd = 1;
									  }
										else if(p.title=='���ŷ��ͱ���'){
												if(store_reports_smsreport.alreadyadd != 1)
												Ext.getCmp('reports_smsreport').add(Ext.create('reports.smsReport'));
				            		Ext.getCmp('page_main').bind(store_reports_smsreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_smsreport.alreadyadd = 1;
									  }
										else if(p.title=='���ŷ�������'){
												if(store_reports_smsdetailreport.alreadyadd != 1)
												Ext.getCmp('reports_smsdetailreport').add(Ext.create('reports.smsDetailReport'));
				            		Ext.getCmp('page_main').bind(store_reports_smsdetailreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_smsdetailreport.alreadyadd = 1;
									  }
										else if(p.title=='����������������'){
												if(store_reports_webreport.alreadyadd != 1)
												Ext.getCmp('reports_webreport').add(Ext.create('reports.webReport'));
												Ext.getCmp('page_main').bind(store_reports_webreport);
					              Ext.getCmp('page_main').setVisible(true);
					              Ext.getCmp('page_main').doRefresh();
												store_reports_webreport.alreadyadd = 1;
										}
										else if(p.title=='��������'){
												if(store_reports_webdetailreport.alreadyadd != 1)
												Ext.getCmp('reports_webdetailreport').add(Ext.create('reports.webDetailReport'));
				            		Ext.getCmp('page_main').bind(store_reports_webdetailreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_webdetailreport.alreadyadd = 1;
									  }
										else if(p.title=='�豸����ͳ��'){
												if(store_reports_devicereport.alreadyadd != 1)
												Ext.getCmp('reports_devicereport').add(Ext.create('reports.deviceReport'));
				            		Ext.getCmp('page_main').bind(store_reports_devicereport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_devicereport.alreadyadd = 1;
									  }
									  else if(p.title=='SSIDͳ�Ʊ���'){
												if(store_reports_ssidreport.alreadyadd != 1)
												Ext.getCmp('reports_ssidreport').add(Ext.create('reports.ssidReport'));
				            		Ext.getCmp('page_main').bind(store_reports_ssidreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_ssidreport.alreadyadd = 1;
									  }
									  else if(p.title=='�û���������'){
												if(store_reports_trafficreport.alreadyadd != 1)
												Ext.getCmp('reports_trafficreport').add(Ext.create('reports.trafficReport'));
				            		Ext.getCmp('page_main').bind(store_reports_trafficreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_trafficreport.alreadyadd = 1;
									  }
									  else if(p.title=='AP�ն�����'){
												if(store_reports_macreport.alreadyadd != 1)
												Ext.getCmp('reports_macreport').add(Ext.create('reports.macReport'));
				            		Ext.getCmp('page_main').bind(store_reports_macreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_macreport.alreadyadd = 1;
									  }
									  else if(p.title=='�û����ʴ���'){
												if(store_reports_usercountreport.alreadyadd != 1)
												Ext.getCmp('reports_usercountreport').add(Ext.create('reports.usercountReport'));
				            		Ext.getCmp('page_main').bind(store_reports_usercountreport);
			                  Ext.getCmp('page_main').setVisible(true);
			                  Ext.getCmp('page_main').doRefresh();
												store_reports_usercountreport.alreadyadd = 1;
									  }
										else if(p.title=='�ն�Ʒ��'){
											if(store_charts_devicebrand.alreadyadd != 1)
											Ext.getCmp('charts_devicebrand').add(Ext.create('charts.deviceBrand'));
									            		Ext.getCmp('page_main').bind(store_charts_devicebrand);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_devicebrand.alreadyadd = 1;
									            	}
										else if(p.title=='�ն�����'){
											if(store_charts_devicetype.alreadyadd != 1)
											Ext.getCmp('charts_devicetype').add(Ext.create('charts.deviceType'));
									            		Ext.getCmp('page_main').bind(store_charts_devicetype);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_devicetype.alreadyadd = 1;
									            	}
										else if(p.title=='�̼Ҷ���'){
											if(store_charts_groupsms.alreadyadd != 1)
											Ext.getCmp('charts_groupsms').add(Ext.create('charts.groupSms'));
									            		Ext.getCmp('page_main').bind(store_charts_groupsms);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_groupsms.alreadyadd = 1;
									            	}
										else if(p.title=='�ն˲���ϵͳ'){
											if(store_charts_deviceos.alreadyadd != 1)
											Ext.getCmp('charts_deviceos').add(Ext.create('charts.deviceOS'));
									            		Ext.getCmp('page_main').bind(store_charts_deviceos);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_deviceos.alreadyadd = 1;
									            	}
										else if(p.title=='�����'){
											if(store_charts_advisit.alreadyadd != 1)
											Ext.getCmp('charts_advisit').add(Ext.create('charts.adVisit'));
									            		Ext.getCmp('page_main').bind(store_charts_advisit);
									                  Ext.getCmp('page_main').setVisible(true);
									                  Ext.getCmp('page_main').doRefresh();
											store_charts_advisit.alreadyadd = 1;
									            	}
										else if(p.title=='��������'){
									            		//Ext.getCmp('page_main').bind(store_rules_smsnumber);
											Ext.getCmp('page_main').setVisible(false);
											//Ext.getCmp('page_main').doRefresh();
									            	}
										else if(p.title=='���ҳ�����'){
										    Ext.getCmp('page_main').bind(store_shops_advmanager);
												if(store_shops_advmanager.alreadyadd != 1){
														doreflashTab(plateid);
												}
												store_shops_advmanager.alreadyadd = 1;
									  }
										else if(p.title=='�̵����'){
											if(store_shops_shopmanager.alreadyadd != 1)
											Ext.getCmp('shops_shopmanager').add(Ext.create('shops.shopManager'));
									        Ext.getCmp('page_main').bind(store_shops_shopmanager);
											store_shops_shopmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_shopmanager.alreadyadd = 1;
									            	}
										else if(p.title=='Ʒ�ƹ���'){
											if(store_shops_brandmanager.alreadyadd != 1)
											Ext.getCmp('shops_brandmanager').add(Ext.create('shops.brandManager'));
									            		Ext.getCmp('page_main').bind(store_shops_brandmanager);	
											store_shops_brandmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_brandmanager.alreadyadd = 1;
									            	}
										else if(p.title=='��Ʒ����'){
											if(store_shops_oditymanager.alreadyadd != 1)
											Ext.getCmp('shops_oditymanager').add(Ext.create('shops.odityManager'));
											Ext.getCmp('page_main').bind(store_shops_oditymanager);
											Ext.getCmp('page_main').setVisible(true);
											store_shops_oditymanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											store_shops_oditymanager.alreadyadd = 1;
									            	}
										else if(p.title=='�̱�������'){	
											if(store_shops_groupmanager.alreadyadd != 1)
											Ext.getCmp('shops_categorymanager').add(Ext.create('shops.categoryManager'));
											Ext.getCmp('page_main').bind(store_shops_groupmanager);
											store_shops_groupmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_groupmanager.alreadyadd = 1;
									            	}
										else if(p.title=='��Ա����'){	
											if(store_shops_shopusermanager.alreadyadd != 1)
											Ext.getCmp('shops_usermanager').add(Ext.create('shops.userManager'));
											Ext.getCmp('page_main').bind(store_shops_shopusermanager);
											store_shops_shopusermanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_shopusermanager.alreadyadd = 1;
									            	}
										else if(p.title=='�����'){
											if(store_shops_activemanager.alreadyadd != 1)
											Ext.getCmp('shops_actmanager').add(Ext.create('shops.actManager'));
											Ext.getCmp('page_main').bind(store_shops_activemanager);
											store_shops_activemanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_activemanager.alreadyadd = 1;
									            	}
										else if(p.title=='��������'){
											if(store_shops_compmanager.alreadyadd != 1)
											Ext.getCmp('shops_compmanager').add(Ext.create('shops.compManager'));
											Ext.getCmp('page_main').bind(store_shops_compmanager);
											store_shops_compmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_compmanager.alreadyadd = 1;
									  }   
										else if(p.title=='���Ź���'){
											if(store_shops_smsmanager.alreadyadd != 1)
											Ext.getCmp('shops_smsmanager').add(Ext.create('shops.smsManager'));
											Ext.getCmp('page_main').bind(store_shops_smsmanager);
											store_shops_smsmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_smsmanager.alreadyadd = 1;
									  }
									  else if(p.title=='��ͼ��ǩ����'){
											if(store_shops_markmanager.alreadyadd != 1)
											Ext.getCmp('shops_markmanager').add(Ext.create('shops.markManager'));
											Ext.getCmp('page_main').bind(store_shops_markmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_shops_markmanager.alreadyadd = 1;
									  } 
									  else if(p.title=='�̼����˹���'){
											if(store_shops_shoptypemanager.alreadyadd != 1)
											Ext.getCmp('shops_shoptypemanager').add(Ext.create('shops.shoptypeManager'));
											Ext.getCmp('page_main').bind(store_shops_shoptypemanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_shops_shoptypemanager.alreadyadd = 1;
									  }
										else if(p.title=='��λ�����'){
												if(store_systems_shopgtypemanager.alreadyadd != 1)
												Ext.getCmp('systems_shopgtypemanager').add(Ext.create('systems.shopgtypeManager'));
												Ext.getCmp('page_main').bind(store_systems_shopgtypemanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_systems_shopgtypemanager.alreadyadd = 1;
												if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
									  } 	
									  else if(p.title=='��֯�ܹ�Ȩ��'){
									  	  if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
									  } 									  
										else if(p.title=='�������'){
											if(store_shops_smsmanager.alreadyadd != 1)
											Ext.getCmp('myaccount_smsmanager').add(Ext.create('myaccount.buySms'));
									        Ext.getCmp('page_main').bind(store_shops_smsmanager);
											store_shops_smsmanager.new_params={plateid:plateid};
											Ext.getCmp('page_main').doRefresh();
											Ext.getCmp('page_main').setVisible(true);
											store_shops_smsmanager.alreadyadd = 1;
									            	} 
										else if(p.title=='��������'){
											if(store_rules_srvpar.alreadyadd != 1)
											Ext.getCmp('rules_srvpar').add(Ext.create('rules.srvpar'));
									        Ext.getCmp('page_main').bind(store_rules_srvpar);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_srvpar.alreadyadd = 1;
									            	}													
										else if(p.title=='�������������'){
											if(store_rules_smsnumber.alreadyadd != 1)
											Ext.getCmp('rules_smsnumber').add(Ext.create('rules.smsNumber'));
									            		Ext.getCmp('page_main').bind(store_rules_smsnumber);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsnumber.alreadyadd = 1;
									            	}
										else if(p.title=='����ע������'){
											if(store_rules_smsregister.alreadyadd != 1)
											Ext.getCmp('rules_smsregister').add(Ext.create('rules.smsRegister'));
									            		Ext.getCmp('page_main').bind(store_rules_smsregister);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsregister.alreadyadd = 1;
									            	}
										else if(p.title=='����ʱ��'){
											if(store_rules_timelimit.alreadyadd != 1)
											Ext.getCmp('rules_timelimit').add(Ext.create('rules.timeLimit'));
									            		Ext.getCmp('page_main').bind(store_rules_timelimit);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_timelimit.alreadyadd = 1;
									            	}
										else if(p.title=='���Ÿ�ʽ'){
											if(store_rules_smsformat.alreadyadd != 1)
											Ext.getCmp('rules_smsformat').add(Ext.create('rules.smsFormat'));
									            		Ext.getCmp('page_main').bind(store_rules_smsformat);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_rules_smsformat.alreadyadd = 1;
									            	}
										else if(p.title=='������'){
												if(store_rules_blackuser.alreadyadd != 1)
												Ext.getCmp('rules_blackuser').add(Ext.create('rules.blackUser'));
									      Ext.getCmp('page_main').bind(store_rules_blackuser);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_rules_blackuser.alreadyadd = 1;
												if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
									  }
										else if(p.title=='�ڰ�����'){
												if(store_rules_white.alreadyadd != 1)
												Ext.getCmp('rules_white').add(Ext.create('rules.white'));
												store_rules_white.alreadyadd = 1;
										}
										else if(p.title=='ģ�����'){
												if(store_rules_plate.alreadyadd != 1)
												Ext.getCmp('rules_plate').add(Ext.create('rules.plate'));
												Ext.getCmp('page_main').bind(store_rules_plate);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_rules_plate.alreadyadd = 1;
												if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
										}
										/*else if(p.title=='ģ��ҳ�����'){
												if(store_rules_page.alreadyadd != 1)
												Ext.getCmp('rules_page').add(Ext.create('rules.page'));
												Ext.getCmp('page_main').bind(store_rules_page);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_rules_page.alreadyadd = 1;
										}*/
										else if(p.title=='Ա������'){
												if(store_users_usermanager.alreadyadd != 1)
												Ext.getCmp('users_usermanager').add(Ext.create('users.userManager'));
												Ext.getCmp('page_main').bind(store_users_usermanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_usermanager.alreadyadd = 1;
												if(!Ext.getCmp('areaTab'))createAreaTab(); // ��������
									  }
									  else if(p.title=='��ʱ�û�'){
												if(store_users_staffmanager.alreadyadd != 1)
												Ext.getCmp('users_staffmanager').add(Ext.create('users.staffManager'));
										    Ext.getCmp('page_main').bind(store_users_staffmanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_staffmanager.alreadyadd = 1;
												if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
									  }
									  /*else if(p.title=='�û�MAC����'){
												if(store_users_usermacmanager.alreadyadd != 1)
												Ext.getCmp('users_usermacmanager').add(Ext.create('users.usermacManager'));
										    Ext.getCmp('page_main').bind(store_users_usermacmanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_usermacmanager.alreadyadd = 1;
									  }*/
										else if(p.title=='��ǩ����'){
									            		Ext.getCmp('page_main').bind(store_users_tagmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
									            	}
										else if(p.title=='��λ����'){
												if(store_users_groupmanager.alreadyadd != 1)
														Ext.getCmp('users_groupmanager').add(Ext.create('users.groupManager'));
												Ext.getCmp('page_main').bind(store_users_groupmanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_groupmanager.alreadyadd = 1;
												if(!Ext.getCmp('areaTab'))createAreaTab(); // ��������
									  }
										else if(p.title=='AP����'){
											if(store_users_apmanager.alreadyadd != 1)
											Ext.getCmp('users_apmanager').add(Ext.create('users.apManager'));
											Ext.getCmp('page_main').bind(store_users_apmanager);
											Ext.getCmp('page_main').setVisible(true);
											Ext.getCmp('page_main').doRefresh();
											store_users_apmanager.alreadyadd = 1;
									            	}
										else if(p.title=='SSID����'){
												if(store_users_ssidmanager.alreadyadd != 1)
												Ext.getCmp('users_ssidmanager').add(Ext.create('users.ssidManager'));
												Ext.getCmp('page_main').bind(store_users_ssidmanager);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_users_ssidmanager.alreadyadd = 1;
												if(Ext.getCmp('areaTab'))
												Ext.getCmp('areaTab').destroy();
    										Ext.getCmp('leftAll').setVisible(false);
    										Ext.getCmp('treetab').setVisible(false);
								    }
								    else if(p.title=='΢������'){
												if(store_settings_weixin.alreadyadd != 1)
												Ext.getCmp('settings_weixin').add(Ext.create('settings.weixin'));
												Ext.getCmp('page_main').bind(store_settings_weixin);
												Ext.getCmp('page_main').setVisible(true);
												Ext.getCmp('page_main').doRefresh();
												store_settings_weixin.alreadyadd = 1;
								    }
			            	var t_id=p.getId(); 
		          	}
		        }      
        });
        
        // ��ʼ������Ժ��״�ѡ��
        firstSelect();
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
                text: '�Ϻ���������Ϣ�����ɷ����޹�˾��Ȩ����&nbsp;&nbsp;',
               
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
				
				Ext.getCmp('leftAll').setWidth(0);
				Ext.getCmp('treetab').setWidth(0);
				
				//Ext.getCmp('leftAll').collapse();				
				bbbb=Ext.getCmp('header');
				//bbbb.setTitle('<center>�����̵�¼�Ż�����ϵͳ - '+item.text+'</center>');
				Ext.getCmp('leftAll').setTitle('<center>'+item.text+'</center>');
				/*
			    	 bbbb=document.all('ltitle');
			       bbbb.innerHTML='��ǰλ�ã�'+item.text;
			       
			       
			       
			        //�ı���ߵ���
			     var tabs_tree=Ext.getCmp('treetab');
			     var tab_len=tabs_tree.items.length;
			     var tree_panel;
			     var tree_flag=0;
			     tabs_tree.items.each(function(item) { 
						if(item.id=='tree_comp'){
									   		tree_flag=1;
									   	
									   	}
			      });	
			      if(tree_flag!=1){      
				       tabs_tree.remove(0);	 				
					   tabs_tree.add({closable: false,id:'tree_comp',border:false,items:[Ext.create(ncViewer.lanGroupTree)], title: '���������û�'}).show();
			       
			     }
			   */    
 				//�ı��м����ͼ      
       
				var i=0;
				var m=0;
				var tab_title=item.text;
				var tab_src=item.srcurl;
				var tab_id=item.itemId;
				var tabs_center=Ext.getCmp("layout_center");
				var tab_len=tabs_center.items.length;
				//�˵������ʽ
				if(Ext.getCmp('tb').getComponent('online'))Ext.getCmp('tb').getComponent('online').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('users'))Ext.getCmp('tb').getComponent('users').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('shops'))Ext.getCmp('tb').getComponent('shops').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('shops_shop'))Ext.getCmp('tb').getComponent('shops_shop').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('shops_ad'))Ext.getCmp('tb').getComponent('shops_ad').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('shops_product'))Ext.getCmp('tb').getComponent('shops_product').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('reports'))Ext.getCmp('tb').getComponent('reports').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('charts'))Ext.getCmp('tb').getComponent('charts').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('logs'))Ext.getCmp('tb').getComponent('logs').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('rules'))Ext.getCmp('tb').getComponent('rules').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('settings'))Ext.getCmp('tb').getComponent('settings').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('systems'))Ext.getCmp('tb').getComponent('systems').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('myaccount'))Ext.getCmp('tb').getComponent('myaccount').removeCls('stripes glow stripes2 shine');
				if(Ext.getCmp('tb').getComponent('yyanalysis'))Ext.getCmp('tb').getComponent('yyanalysis').removeCls('stripes glow stripes2 shine');
				Ext.getCmp('tb').getComponent(tab_id).addCls('stripes glow ');
				tabs_center.items.each(function(item) {
			     	if(item.id==tab_id){
					 			i++;
					 	}
			      if(i==0){m++;}
        });   
	      if(i>0){
	     	  tabs_center.setActiveTab(m);
	     	  return; 
	     	} 
	      if(tab_len>=1){
	      	for(i=0;i<tab_len;i++){     		
	      	 	tabs_center.remove(tab_len-i-1); 
	        }      	
	      } 
			
				if(search_store !='')
    				eval(search_store).alreadyadd = 0;
				//����ģ��
				if(Ext.getCmp('plateTab'))
				Ext.getCmp('plateTab').destroy();
				//����������
				if(Ext.getCmp('areaTab'))
				Ext.getCmp('areaTab').destroy();
				//����ԥ԰ͳ�Ʒ���
				if(Ext.getCmp('analysisTab'))
				Ext.getCmp('analysisTab').destroy();
				Ext.getCmp('page_main').setVisible(false);
				if(tab_id=='ssjk'){
						tabs_center.add({autoScroll:false,id:tab_id,iconCls: 'add16',title: tab_title,items:[Ext.create(tab_src)]});   
				    Ext.getCmp('page_main').store=store_adminuser;
						Ext.getCmp('page_main').doRefresh(); 
				}   
				else if(tab_id=='online'){  
						Ext.getCmp('leftAll').setVisible(false);
						Ext.getCmp('treetab').setVisible(false);
						
						//store_online_onlineinfo.alreadyadd = 0;
					  //store_online_onlinemac.alreadyadd=0; 
					  
						Ext.getCmp('page_main').bind(Ext.StoreManager.lookup("store_online_onlineinfo"));
						//if(right_sfun.indexOf("������Ϣ")>=0){
							  //tabs_center.remove('online_onlineinfo'); // �ر�ѡ�
							  //tabs_center.getComponent('online_onlineinfo').destroy();
								//tabs_center.add({autoScroll:true,id:tab_id+'_onlineinfo',iconCls: 'onlineinfo',border:false,frame:false,title: "������Ϣ"});  
						//}
//						store_online_onlineinfo.alreadyadd=0;
						if(right_sfun.indexOf("������Ϣ")>=0){
							
							tabs_center.add({autoScroll:true,id:tab_id+'_onlineinfo',iconCls: 'onlineinfo',border:false,frame:false,title: "������Ϣ", items:[Ext.create('online.onlineInfo')]});
				    }
				    store_online_onlinemac.alreadyadd=0;
				    if(right_sfun.indexOf("����Mac")>0)tabs_center.add({autoScroll:true,id:tab_id+'_onlinemac',iconCls: 'onlineinfo',border:false,frame:false,title: "����Mac"});
				}	
				else if(tab_id=='users'){ 
						store_users_usermanager.alreadyadd=0;
						store_users_staffmanager.alreadyadd=0;
						//store_users_usermacmanager.alreadyadd=0;
						store_systems_autopri.alreadyadd=0;
						store_users_groupmanager.alreadyadd=0;
						store_systems_shopgtypemanager.alreadyadd=0;
						
						tabs_center.add({autoScroll:true,id:tab_id+'_usermanager',iconCls: 'user',border:false,frame:false,title: "Ա������"});
						if(admincheck == "admin" || isexp == 'false')tabs_center.add({autoScroll:true,id:tab_id+'_staffmanager',iconCls: 'user',border:false,frame:false,title: "��ʱ�û�"});
					  //tabs_center.add({autoScroll:true,id:tab_id+'_usermacmanager',iconCls: 'user',border:false,frame:false,title: "�û�MAC����"});
					  if(right_sfun.indexOf("��λ����")>0)tabs_center.add({autoScroll:true,id:'users_groupmanager',iconCls: 'user',border:false,frame:false,title: "��λ����"});
					  if(admincheck == "admin")tabs_center.add({autoScroll:true,id:'systems_shopgtypemanager',iconCls: 'user',border:false,frame:false,title: "��λ�����"});
					  if(admincheck == "admin")tabs_center.add({
					        title: '��֯�ܹ�Ȩ��',
									iconCls: 'user',
					        id:'systems_areaTab2',            
					        html: '<iframe id="html_systems_areaTab2" name="html_systems_areaTab2" src="/v8/ncm_systems_tree_autopri.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						createAreaTab(); // ��������
				}
				else if(tab_id=='users_ap'){ 
						store_users_apmanager.alreadyadd=0;
						Ext.getCmp('page_main').setVisible(true); 
						if(right_fun.indexOf("AP����")>0)tabs_center.add({autoScroll:true,id:'users_apmanager',iconCls: 'user',border:false,frame:false,title: "AP����"});
				}
				else if(tab_id=='reports'){  
						store_reports_smsreport.alreadyadd=0;
						store_reports_smsdetailreport.alreadyadd=0;
						store_reports_webreport.alreadyadd=0;
						store_reports_webdetailreport.alreadyadd=0;
						store_reports_devicereport.alreadyadd=0;
						store_reports_ssidreport.alreadyadd=0;
						store_reports_trafficreport.alreadyadd=0;
						store_reports_macreport.alreadyadd=0;
						store_reports_usercountreport.alreadyadd=0;
					  
			 	    if(right_sfun.indexOf("���ŷ��ͱ���")>0)tabs_center.add({autoScroll:true,id:tab_id+'_smsreport',iconCls: 'report',border:false,frame:false,title: "���ŷ��ͱ���"});
					  if(right_sfun.indexOf("���ŷ�������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_smsdetailreport',iconCls: 'report',border:false,frame:false,title: "���ŷ�������"});
				    if(right_sfun.indexOf("����������������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_webreport',iconCls: 'report',border:false,frame:false,title: "����������������"});
						if(right_sfun.indexOf("��������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_webdetailreport',iconCls: 'report',border:false,frame:false,title: "��������"});
						if(right_sfun.indexOf("�豸����ͳ��")>0)tabs_center.add({autoScroll:true,id:tab_id+'_devicereport',iconCls: 'report',border:false,frame:false,title: "�豸����ͳ��"});   
						if(right_sfun.indexOf("SSIDͳ�Ʊ���")>0)tabs_center.add({autoScroll:true,id:tab_id+'_ssidreport',iconCls: 'report',border:false,frame:false,title: "SSIDͳ�Ʊ���"});
						if(right_sfun.indexOf("�û���������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_trafficreport',iconCls: 'report',border:false,frame:false,title: "�û���������"}); 
						if(right_sfun.indexOf("AP�ն�����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_macreport',iconCls: 'report',border:false,frame:false,title: "AP�ն�����"});   
						if(right_sfun.indexOf("�û����ʴ���")>0)tabs_center.add({autoScroll:true,id:tab_id+'_usercountreport',iconCls: 'report',border:false,frame:false,title: "�û����ʴ���"});   
						createAreaTab();				  
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
						
						//tabs_center.add({autoScroll:true,id:tab_id+'_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "������"});
						if(right_sfun.indexOf("���ҳ�����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_advmanager',iconCls: 'shops_ad',border:false,frame:false,title: "���ҳ�����"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_advmanager2',iconCls: 'shops_ad',border:false,frame:false,title: "���ҳ�����2"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_advmanager3',iconCls: 'shops_ad',border:false,frame:false,title: "���ҳ�����3"});
						if(right_sfun.indexOf("�̵����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "�̵����"});	
						if(right_sfun.indexOf("Ʒ�ƹ���")>0)tabs_center.add({autoScroll:true,id:tab_id+'_brandmanager',iconCls: 'star',border:false,frame:false,title: "Ʒ�ƹ���"});
						if(right_sfun.indexOf("��Ʒ����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "��Ʒ����"});
						if(right_sfun.indexOf("�̱�������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_categorymanager',iconCls: 'shops_type',border:false,frame:false,title: "�̱�������"});
						if(right_sfun.indexOf("��Ա����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_usermanager',iconCls: 'shops_vip',border:false,frame:false,title: "��Ա����"});
						if(right_sfun.indexOf("�����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_actmanager',iconCls: 'shops_active',border:false,frame:false,title: "�����"});
						if(right_sfun.indexOf("��������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_compmanager',iconCls: 'shops_comp',border:false,frame:false,title: "��������"});
						//if(right_fun.indexOf("���Ź���")>0)
						
						if(right_sfun.indexOf("���Ź���")>0)tabs_center.add({autoScroll:true,id:'shops_smsmanager',iconCls: 'phone',border:false,frame:false,title: "���Ź���"});
						if(right_sfun.indexOf("��ͼ��ǩ����")>0)tabs_center.add({autoScroll:true,id:'shops_markmanager',iconCls: 'phone',border:false,frame:false,title: "��ͼ��ǩ����"});
						if(right_sfun.indexOf("�̼����˹���")>0)tabs_center.add({autoScroll:true,id:'shops_shoptypemanager',iconCls: 'shoptype',border:false,frame:false,title: "�̼����˹���"});
						createPlateTab();//����ģ��
				} 
				else if(tab_id=='shops_ad'){//������
						store_shops_advmanager.alreadyadd=0;
						//tabs_center.add({autoScroll:true,id:'shops_admanager',iconCls: 'shops_ad',border:false,frame:false,title: "������"});
						tabs_center.add({autoScroll:true,id:'shops_advmanager',iconCls: 'shops_ad',border:false,frame:false,title: "���ҳ�����"});
						//����Ĭ��ѡ��ģ��
						for(i=0; i<store_portplate.getCount();i++){
								if(store_portplate.getAt(i).get('status')==0){
										plateid=store_portplate.getAt(i).get('sid');
								}
						}
				} 
				else if(tab_id=='shops_shop'){//�̵����
						store_shops_shopmanager.alreadyadd=0;
						tabs_center.add({autoScroll:true,id:'shops_shopmanager',iconCls: 'shops_shop',border:false,frame:false,title: "�̵����"});	
				} 
				else if(tab_id=='shops_product'){//��Ʒ����
						store_shops_oditymanager.alreadyadd=0;
						tabs_center.add({autoScroll:true,id:'shops_oditymanager',iconCls: 'shops_product',border:false,frame:false,title: "��Ʒ����"});	
				} 
				else if(tab_id=='charts'){
						store_charts_devicebrand.alreadyadd=0;
						store_charts_devicetype.alreadyadd=0;
						store_charts_deviceos.alreadyadd=0;
						store_charts_advisit.alreadyadd=0;
						store_charts_groupsms.alreadyadd=0;
					 	if(right_sfun.indexOf("�ն�Ʒ��")>0)tabs_center.add({autoScroll:true,id:'charts_devicebrand',iconCls:'chart_pie',border:false,frame:false,title: "�ն�Ʒ��"});
						if(right_sfun.indexOf("�ն�����")>0)tabs_center.add({autoScroll:true,id:'charts_devicetype',iconCls:'chart_pie',border:false,frame:false,title: "�ն�����"});	
						// if(right_sfun.indexOf("�̼Ҷ���")>0)tabs_center.add({autoScroll:true,id:'charts_groupsms',iconCls:'chart_pie',border:false,frame:false,title: "�̼Ҷ���"});	
						if(right_sfun.indexOf("�ն˲���ϵͳ")>0)tabs_center.add({autoScroll:true,id:'charts_deviceos',iconCls:'chart_pie',border:false,frame:false,title: "�ն˲���ϵͳ"});	
						//tabs_center.add({autoScroll:true,id:'charts_advisit',iconCls:'chart_pie',border:false,frame:false,title: "�����"});	
					
						// tabs_center.add({autoScroll:true,iconCls:'chart_pie',border:false,frame:false,title: "��������", items:[Ext.create('charts.smsRecount')]});			
						if(right_sfun.indexOf("��������")>0)tabs_center.add({
					          title: '��������',
							      iconCls: 'chart_curve',
					          id:tab_id+'_smsrecount',            
					          html: '<iframe  id="charts_smsrecount" name="charts_smsrecount" src="/ncm_chart/ncm_smsRecountChart_ch.html" scrolling="auto" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("ʵʱ�����������ͼ")>0)tabs_center.add({
					          title: 'ʵʱ�����������ͼ',
							      iconCls: 'chart_curve',
					          id:'realtimevisit',            
					          html: '<iframe  id="lay_rzck_proclog" name="lay_rzck_proclog" src="/ncm_adchart/ncm_ad_onlineUserRealtimeTrend.html" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						createAreaTab();
				}
				else if(tab_id=='logs'){  
						store_logs_smslog.alreadyadd=0;
						store_logs_userlog.alreadyadd=0;
						//store_logs_adlog.alreadyadd=0;
						store_logs_alarmlog.alreadyadd=0;
						store_logs_maclog.alreadyadd=0;
							
						if(right_sfun.indexOf("������־")>0)tabs_center.add({autoScroll:true,id:tab_id+'_smslog',iconCls: 'log',border:false,frame:false,title: "������־"});
						if(right_sfun.indexOf("�û���־")>0)tabs_center.add({autoScroll:true,id:tab_id+'_userlog',iconCls: 'log',border:false,frame:false,title: "�û���־"});
						// tabs_center.add({autoScroll:true,id:tab_id+'_adlog',iconCls: 'log',border:false,frame:false,title: "�����־"}); 
						if(right_sfun.indexOf("�澯��־")>0)tabs_center.add({autoScroll:true,id:tab_id+'_alarmlog',iconCls: 'log',border:false,frame:false,title: "�澯��־"}); 
						if(right_sfun.indexOf("MAC��־")>0)tabs_center.add({autoScroll:true,id:tab_id+'_maclog',iconCls: 'log',border:false,frame:false,title: "MAC��־"}); 
						createAreaTab();
				} 
				else if(tab_id=='rules'){ 
						//store_rules_smsnumber.alreadyadd=0;
						//store_rules_smsregister.alreadyadd=0;
						//store_rules_timelimit.alreadyadd=0;
						//store_rules_smsformat.alreadyadd=0;
						store_rules_blackuser.alreadyadd=0;
						
						store_users_ssidmanager.alreadyadd=0;
						store_rules_plate.alreadyadd=0;
						//store_rules_page.alreadyadd=0;
	
						//tabs_center.add({autoScroll:true,id:tab_id+'_smsnumber',iconCls: 'rule',border:false,frame:false,title: "�������������"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_smsregister',iconCls: 'rule',border:false,frame:false,title: "����ע������"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_timelimit',iconCls: 'rule',border:false,frame:false,title: "����ʱ��"});
						//tabs_center.add({autoScroll:true,id:tab_id+'_smsformat',iconCls: 'rule',border:false,frame:false,title: "���Ÿ�ʽ"});
						if(right_sfun.indexOf("������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_blackuser',iconCls: 'rule',border:false,frame:false,title: "������"});
						
						if(right_sfun.indexOf("SSID����")>0)tabs_center.add({autoScroll:true,id:'users_ssidmanager',iconCls: 'rule',border:false,frame:false,title: "SSID����"});
						if(right_sfun.indexOf("ģ�����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_plate',iconCls: 'rule',border:false,frame:false,title: "ģ�����"});
						//if(right_sfun.indexOf("ģ��ҳ�����")>0)tabs_center.add({autoScroll:true,id:tab_id+'_page',iconCls: 'rule',border:false,frame:false,title: "ģ��ҳ�����"});
						createAreaTab();
				}
				else if(tab_id=='settings'){
						store_settings_weixin.alreadyadd=0;
						store_rules_srvpar.alreadyadd=0;
						store_rules_white.alreadyadd=0;
						
						if(right_sfun.indexOf("��������")>0)tabs_center.add({autoScroll:true,id:'rules_srvpar',iconCls: 'set',border:false,frame:false,title: "��������"});
						if(right_sfun.indexOf("�ڰ�����")>0)tabs_center.add({autoScroll:true,id:'rules_white',iconCls: 'set',border:false,frame:false,title: "�ڰ�����"});
						if(right_sfun.indexOf("Portal����")>0)tabs_center.add({          
					          title: 'Portal����',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmPortal_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
					  });
						if(right_sfun.indexOf("���ŷ�����")>0)tabs_center.add({          
					          title: '���ŷ�����',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmSms_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'    
					  });
						if(right_sfun.indexOf("HTTP������")>0)tabs_center.add({          
					          title: 'HTTP������',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmHttp_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'
					  });
						if(right_sfun.indexOf("AC����")>0)tabs_center.add({          
					          title: 'AC����',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmAC_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("Radius����")>0)tabs_center.add({        
					          title: 'Radius����',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmRadius_Server_ParaSet" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("��������")>0)tabs_center.add({         
					          title: '��������',
							      iconCls: 'set',
					          html: '<iframe  id="dhIframe" name="gdrzIframe" src="/pronline/Msg?FunName@ncmWebSystemSet_base" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>'           
					  });
						if(right_sfun.indexOf("΢������")>0)tabs_center.add({autoScroll:true,id:tab_id+'_weixin',iconCls: 'set',border:false,frame:false,title: "΢������"});
				}
				else if(tab_id=='systems'){
					  store_systems_menu.alreadyadd=0;
					  store_systems_submenu.alreadyadd=0;
					  
					  if(right_sfun.indexOf("ϵͳ�û�����")>0)tabs_center.add({
					          title: 'ϵͳ�û�����',
										iconCls: 'system',
					          id:tab_id+'_xtyhmenu',            
					          html: '<iframe id="lay_rzck_screenlog" name="lay_rzck_screenlog" src="/ncmsysman/ncm_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						if(right_sfun.indexOf("ϵͳȨ�޹���")>0)tabs_center.add({
					          title: 'ϵͳȨ�޹���',
							      iconCls: 'system',
					          id:tab_id+'_rolemenu',           
					          html: '<iframe  id="lay_rzck_imcontent" name="lay_rzck_imcontent" src="/ncmsysman/ncm_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
						});
						if(admincheck == "admin")tabs_center.add({autoScroll:true,id:tab_id+'_menu',iconCls: 'system',border:false,frame:false,title: "ϵͳ���˵�����"});
						if(admincheck == "admin")tabs_center.add({autoScroll:true,id:tab_id+'_submenu',iconCls: 'system',border:false,frame:false,title: "ϵͳ�Ӳ˵�����"});
						if(right_sfun.indexOf("�û�������־")>0)tabs_center.add({
					          title: '�û�������־',
							      iconCls: 'system',
					          id:tab_id+'_usvisitmenu',            
					          html: '<iframe  id="html_usvisit" name="html_usvisit" src="/ncmsysman/ncm_uservisitlog.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
					  });
						createAreaTab();
				}
				else if(tab_id=='myaccount'){  
					  if(right_sfun.indexOf("�޸�����")>0)tabs_center.add({
				          title: '�޸�����', 
						      iconCls: 'key',
				          id:'myaccount_modipassword',           
				          html: '<iframe  id="lay_myaccount_modipassword" name="lay_myaccount_modipassword" src="/pronline/Msg?FunName@ncmWebDispModiPass" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
				    });
				}
				else if(tab_id=='yyanalysis'){
					  //tabs_center.add({autoScroll:true,id:tab_id+'_caidan',border:false,frame:false,items:[Ext.create('analysis.ncmAnalysis')]});
						//createAnalysisTab();	
				}
				else{
				    tabs_center.add({autoScroll:false,border:false,frame:false,id:tab_id,iconCls: 'add16',title: tab_title,items:[Ext.create(tab_src)]}); 
				}
				tabs_center.setActiveTab(0);
		}
});

