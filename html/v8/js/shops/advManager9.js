/*广告管理*/
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

Ext.define('shops.advManager9',{
    extend: 'Ext.panel.Panel',
	  autoScroll: false,
    frame: false,
    initComponent : function(){
				if(store_shops_advmanager3.alreadyload !=1){
					  store_shops_advmanager3.flag = '';
					  store_shops_advmanager3.sid = '';
				    store_shops_advmanager3.name = '';
				    store_shops_advmanager3.sgroupid = '';
				    store_shops_advmanager3.descr = '';
				    store_shops_advmanager3.imgpath = '';
				    store_shops_advmanager3.serno = '';
				    store_shops_advmanager3.floor ='';
				    store_shops_advmanager3.level = '';
				    store_shops_advmanager3.plevel = '';
				    store_shops_advmanager3.type = '';
				    store_shops_advmanager3.systime = '';
				    store_shops_advmanager3.shopid = '';
				    store_shops_advmanager3.plateid = plateid;
				    store_shops_advmanager3.img_long = '';
				    store_shops_advmanager3.img_width = '';
				    store_shops_advmanager3.flags = '';
				    store_shops_advmanager3.label = '';
				    store_shops_advmanager3.keyword = '';
				    store_shops_advmanager3.caExport = ''; //导出
					  store_shops_advmanager3.new_params={advmanager3_flag:store_shops_advmanager3.flag,advmanager3_sid:store_shops_advmanager3.sid,advmanager3_name:store_shops_advmanager3.name,advmanager3_sgroupid:store_shops_advmanager3.sgroupid,advmanager3_descr:store_shops_advmanager3.descr,advmanager3_imgpath:store_shops_advmanager3.imgpath,advmanager3_serno:store_shops_advmanager3.serno,advmanager3_floor:store_shops_advmanager3.floor,advmanager3_level:store_shops_advmanager3.level,advmanager3_plevel:store_shops_advmanager3.plevel,advmanager3_type:store_shops_advmanager3.type,advmanager3_systime:store_shops_advmanager3.systime,advmanager3_shopid:store_shops_advmanager3.shopid,advmanager3_plateid:store_shops_advmanager3.plateid,advmanager3_flags:store_shops_advmanager3.flags,advmanager3_img_long:store_shops_advmanager3.img_long,advmanager3_img_width:store_shops_advmanager3.img_width,advmanager3_label:store_shops_advmanager3.label,keyword:store_shops_advmanager3.keyword,caExport:store_shops_advmanager3.caExport};
			  }
			  
				function reflash(){
						store_shops_advmanager3.new_params={advmanager3_flag:store_shops_advmanager3.flag,advmanager3_sid:store_shops_advmanager3.sid,advmanager3_name:store_shops_advmanager3.name,advmanager3_sgroupid:store_shops_advmanager3.sgroupid,advmanager3_descr:store_shops_advmanager3.descr,advmanager3_imgpath:store_shops_advmanager3.imgpath,advmanager3_serno:store_shops_advmanager3.serno,advmanager3_floor:store_shops_advmanager3.floor,advmanager3_level:store_shops_advmanager3.level,advmanager3_plevel:store_shops_advmanager3.plevel,advmanager3_type:store_shops_advmanager3.type,advmanager3_systime:store_shops_advmanager3.systime,advmanager3_shopid:store_shops_advmanager3.shopid,advmanager3_plateid:store_shops_advmanager3.plateid,advmanager3_flags:store_shops_advmanager3.flags,advmanager3_img_long:store_shops_advmanager3.img_long,advmanager3_img_width:store_shops_advmanager3.img_width,advmanager3_label:store_shops_advmanager3.label,keyword:store_shops_advmanager3.keyword,caExport:store_shops_advmanager3.caExport};
						//store_shops_advmanager3.currentPage=1;
						store_shops_advmanager3.load();
				}
				
				function gotoedit3(img_long, img_width){
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ncmshop/ncm_webcomindex4_dr_img.htm&img_long@'+img_long+'&img_width@'+img_width+'" scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
				    var windr2 = new Ext.Window ({
				                           id:"advmanager3_windr2",
				                           title:"上传图片",
				                           //x:0,
				                           y:100,
				                           width:420,
				                           height:120,
				                           bodyPadding: 0,
				                           modal:true,
																	 //headerPosition: 'bottom',
				                           resizable:true,
				                           closable :true,
				                           draggable:true,
				                           html:htmlvar
				    })
					  windr2.show();
				}
				
				function gotoedit2(img_long, img_width, view_long, view_width){
						var htmlvar='<iframe src="/pronline/Msg?FunName@ncmWebReplace&plate@ImgCropper/ImgCropper4.htm&img_long@'+img_long+'&img_width@'+img_width+'&view_long@'+view_long+'&view_width@'+view_width+'" scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
				    var windr2 = new Ext.Window ({
				                           id:"advmanager3_windr2",
				                           title:"上传图片",
				                           //x:0,
				                           y:0,
				                           width:window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth,
				                           height:window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight,
				                           autoScroll:true,
				                           bodyPadding: 0,
				                           modal:true,
																	 //headerPosition: 'bottom',
				                           resizable:true,
				                           closable :true,
				                           draggable:true,
				                           html:htmlvar
				    })
					  windr2.show();
				}
				
				function gotoedit1(pos, view_long, view_width){
					  Ext.getCmp('advmanager3_detail').setVisible(true);
					  Ext.getCmp('advmanager3_intro').setVisible(false);
					  for(i=1; i<store_shops_advmanager3.getCount()+1; i++){
							  Ext.getCmp('advmanager3_ads'+i).removeCls('stripes');  // 移除背景
					  		if(pos==store_shops_advmanager3.getAt(i-1).get('level')){
									  Ext.getCmp('advmanager3_ads'+pos).addCls('stripes');  // 添加背景
					  				Ext.getCmp('advmanager3_sid').setValue(store_shops_advmanager3.getAt(i-1).get('sid'));
							  		Ext.getCmp('advmanager3_update').setValue('update');
							  		Ext.getCmp('advmanager3_name').setValue(store_shops_advmanager3.getAt(i-1).get('name'));
							  		Ext.getCmp('advmanager3_serno').setValue(store_shops_advmanager3.getAt(i-1).get('serno'));
							  		Ext.getCmp('advmanager3_tyid').setValue(store_shops_advmanager3.getAt(i-1).get('sgroupid'));
							  		Ext.getCmp('advmanager3_shid').setValue(store_shops_advmanager3.getAt(i-1).get('shopid'));
							  		Ext.getCmp('advmanager3_floor').setValue(store_shops_advmanager3.getAt(i-1).get('floor'));
							  		Ext.getCmp('advmanager3_level').setValue(store_shops_advmanager3.getAt(i-1).get('level'));
							  		Ext.getCmp('advmanager3_plevel').setValue(store_shops_advmanager3.getAt(i-1).get('plevel'));
							  		Ext.getCmp('advmanager3_type').setValue(store_shops_advmanager3.getAt(i-1).get('type'));
							  		Ext.getCmp('advmanager3_plateid').setValue(store_shops_advmanager3.getAt(i-1).get('plateid'));
							  		Ext.getCmp('advmanager3_flags').setValue(store_shops_advmanager3.getAt(i-1).get('flags'));
							  		Ext.getCmp('advmanager3_descr').setValue(store_shops_advmanager3.getAt(i-1).get('descr'));
							  		Ext.getCmp('advmanager3_label').setValue(store_shops_advmanager3.getAt(i-1).get('label'));
							  		Ext.getCmp('advmanager3_imgpath').setValue(store_shops_advmanager3.getAt(i-1).get('imgpath'));
							  		Ext.getCmp('advmanager3_img').setSrc(store_shops_advmanager3.getAt(i-1).get('imgpath'));
							  		Ext.getCmp('advmanager3_img_long').setValue(store_shops_advmanager3.getAt(i-1).get('img_long'));
							  		Ext.getCmp('advmanager3_img_width').setValue(store_shops_advmanager3.getAt(i-1).get('img_width'));
							  		Ext.getCmp('advmanager3_view_long').setValue(view_long);
							  		Ext.getCmp('advmanager3_view_width').setValue(view_width);	
					  		}	
					  }
		    }
		    
				Ext.define('TypeData', {
				    extend: 'Ext.data.Model',
				    fields: ['sid', 'name', 'groupname', 'descr', 'systime', 'imgpath', 'flags']
				    //idProperty: 'uid'
				});
				var store_type = Ext.create('Ext.data.Store', {
					 model:'TypeData',
					 pageSize: 10000,
					 autoLoad:true,
					 proxy:{
					 		type:'ajax',
					 		url:'/pronline/Msg?FunName@ncm_webcomtype_list&plateid@'+plateid,
					 		reader:{
					 				type:'json',
					 				root:'eimdata',
					 				totalProperty:'totalCount'	
					 		},
					 		simpleSortMode:true	
					 }
				});
				var typeCombo = Ext.create('Ext.form.field.ComboBox', {
					 fieldLabel:'商品小类名称',
					 labelWidth:100,
					 width:320,
					 valueField:'sid',
					 displayField:'name',
					 id:'advmanager3_tyid',
					 name:'advmanager3_tyid',
					 allowBlank:true,
					 store:store_type,
					 triggerAction:'all',
					 queryMode:'local',
					 typeAhead:true,
					 hidden:true,
					 listeners:{
					 		'change':function(){
					 		}	
					 },
					 listConfig:{
					 		loadingText:'Searching...',
					 		emptyText:'No matching found.'	
					 }
				});
				
				Ext.define('ShopData', {
				    extend: 'Ext.data.Model',
				    fields: ['iNum', 'id', 'name', 'service_code', 'sgroupid', 'imgpath', 'floor', 'url', 'code', 'tel', 'address', 'level', 'plevel', 'type', 'stype', 'descr', 'detail', 'systime', 'flags']
				    //idProperty: 'uid'
				});
				var store_shop = Ext.create('Ext.data.Store', {
					 model:'ShopData',
					 pageSize: 10000,
					 autoLoad:true,
					 proxy:{
					 		type:'ajax',
					 		url:'/pronline/Msg?FunName@ncm_webcomshop_list&plateid@'+plateid,
					 		reader:{
					 				type:'json',
					 				root:'eimdata',
					 				totalProperty:'totalCount'	
					 		},
					 		simplleSortMode:true	
					 }
				});
				var shopCombo = Ext.create('Ext.form.field.ComboBox', {
					 fieldLabel:'商店名称',
					 afterLabelTextTpl: required,
					 labelWidth:100,
					 width:320,
					 valueField:'id',
					 displayField:'name',
					 id:'advmanager3_shid',
					 name:'advmanager3_shid',
					 hidden:false,
					 store:store_shop,
					 triggerAction:'all',
					 queryMode:'local',
					 typeAhead:true,
					 allowBlank:false, //是否允许为空
			     blankText:'商店名称不能为空！',	
			     msgTarget:'qtip', //显示一个浮动的提示信息 
					 listeners:	{
					 		'change':function(){
					 			 	//document.getElementById('form1').shopid.value = this.value;
					 		}	
					 },
					 listConfig:{
					 		loadingText:'Searching...',
					 		emptyText:'No matching found.'	
					 }
				});
				
				Ext.define('MarkData', {
				    extend: 'Ext.data.Model',
				    fields: ['sid', 'mark', 'mapid', 'sx','sy','ex', 'ey']
				    //idProperty: 'uid'
				});
				var store_mark = Ext.create('Ext.data.Store', {
					 model:'MarkData',
					 pageSize: 10000,
					 autoLoad:true,
					 proxy:{
					 		type:'ajax',
					 		url:'/pronline/Msg?FunName@ncm_webcommark_list',
					 		reader:{
					 				type:'json',
					 				root:'eimdata',
					 				totalProperty:'totalCount'	
					 		},
					 		simplleSortMode:true	
					 }
				});
				var markCombo = Ext.create('Ext.form.field.ComboBox', {
					 fieldLabel:'标签名称',
					 labelWidth:100,
					 width:320,
					 valueField:'mark',
					 displayField:'mark',
					 id:'advmanager3_label',
					 name:'advmanager3_label',
					 hidden:false,
					 store:store_mark,
					 triggerAction:'all',
					 queryMode:'local',
					 typeAhead:true,
					 allowBlank:true, //是否允许为空 
					 listeners:	{
					 		'change':function(){
					 		}	
					 },
					 listConfig:{
					 		loadingText:'Searching...',
					 		emptyText:'No matching found.'	
					 }
				});
				var markCombod = Ext.create('Ext.form.field.ComboBox', {
					 fieldLabel:'标签名称',
					 labelWidth:100,
					 width:320,
					 valueField:'mark',
					 displayField:'mark',
					 id:'advmanager3_mark',
					 name:'advmanager3_mark',
					 hidden:false,
					 value:'A-1-1',
					 store:store_mark,
					 triggerAction:'all',
					 queryMode:'local',
					 typeAhead:true,
					 allowBlank:true, //是否允许为空 
					 listeners:	{
					 		'change':function(){
					 			 	var mark = this.value;
					 			 	store_shops_advmanager3.load({
								  		params: {mark:mark},
								  		
								  		callback:function(records, options, success){
								  				eval(shops.advManager9).init();
								  		}
								  });
					 		}	
					 },
					 listConfig:{
					 		loadingText:'Searching...',
					 		emptyText:'No matching found.'	
					 }
				});
				
				var store_type1 = Ext.create('Ext.data.Store', {
					 fields:['groupid', 'groupname'],
					 data:[
					 		{groupid:"1", groupname:"广告"},
				      {groupid:"2", groupname:"品牌"},
				      {groupid:"3", groupname:"促销"},
				      {groupid:"4", groupname:"视频"}
					 ]
				});
					    
		    var fp = Ext.create('Ext.FormPanel', {
						id:'advmanager3_detail',
						hidden:true,
						frame:false,
						border:false,
						margins:'0 0 0 20',
						bodyPadding:0,
						defaultType:'textfield', //设置表单字段的默认类型  
						fieldDefaults:{ //统一设置表单字段默认属性
								labelSeparator:':', //分隔符
								labelWidth:100, //标签宽度
								width:320, //字段宽度
								msgTarget:'side'
						},
						items:[{
								xtype:'fieldset',
								width:350,
								title:'基本信息',
								collapsible: true,
								items:[{
										xtype:'textfield',
										fieldLabel:'增/改',
										id:'advmanager3_update',
										name:'advmanager3_update',
										hidden:true
								},{
										xtype:'textfield',
										fieldLabel:'序号',
										id:'advmanager3_sid',
										name:'advmanager3_sid',
										hidden:true
								},{
										xtype:'textfield',
										fieldLabel:'商标名称',
										id:'advmanager3_name',
										name:'advmanager3_name',
										afterLabelTextTpl: required,
										allowBlank:false, //是否允许为空
								    blankText:'商标名称不能为空！',	
								    msgTarget:'qtip' //显示一个浮动的提示信息 	
								},{
										xtype:'textfield',
										fieldLabel:'商标序列号',
										id:'advmanager3_serno',
										name:'advmanager3_serno',
										hidden:true		
								},
								typeCombo,
								shopCombo,
								markCombo,
								{
										xtype:'textfield',
										fieldLabel:'楼层',
										id:'advmanager3_floor',
										name:'advmanager3_floor',
										hidden:true	
								},{
										xtype:'numberfield',
										fieldLabel:'广告优先级',
										id:'advmanager3_level',
										name:'advmanager3_level',
										hidden:true,
										hideTrigger:false, //隐藏微调按钮
				        		allowDecimals:false, //不允许输入小数
				        		maxValue:100, //最大值
				        		minvalue:0, //最小值
				        		nanText:'请输入有效范围内的整数' //无效数字提示	
								},{
										xtype:'numberfield',
										fieldLabel:'促销优先级',
										id:'advmanager3_plevel',
										name:'advmanager3_plevel',
										hidden:true,
										hideTrigger:false, //隐藏微调按钮
				        		allowDecimals:false, //不允许输入小数
				        		maxValue:100, //最大值
				        		minvalue:0, //最小值
				        		nanText:'请输入有效范围内的整数' //无效数字提示	
								},{
										xtype:'combobox',
										fieldLabel:'活动类型',
				      		  id:'advmanager3_type',
				      		  name:'advmanager3_type',
				      		  hidden:true,
				      		  triggerAction:'all',
				      		  store:store_type1,
				      		  displayField:'groupname',
				      		  valueField:'groupid',
				      		  queryMode:'local',
				      		  forceSelection:true,
				      		  typeAhead:true,
				      		  allowBlank:true	
								},{
										xtype:'textfield',
										fieldLabel:'模板',
										id:'advmanager3_plateid',
										name:'advmanager3_plateid',
										hidden:true	
								},{
										xtype:'textfield',
										fieldLabel:'标记',
										id:'advmanager3_flags',
										name:'advmanager3_flags',
										hidden:true	
								},{
										xtype:'textfield',
										fieldLabel:'图片路径',
										id:'advmanager3_imgpath',
										name:'advmanager3_imgpath',
										hidden:true
								},{
										xtype:'textarea',
										fieldLabel:'描述',
										id:'advmanager3_descr',
										name:'advmanager3_descr'
								}]	
						},{
								xtype:'fieldset',
								title:'预览',
								collapsible: true,
								width:350,
								height:200,	
								items:{
										xtype:'image',
										id:'advmanager3_img',
										name:'advmanager3_img',
										autoWidth:true,
										autoHeight:true,
										width:350,
										height:200
								}	
						},{
								xtype:'button',
					  		text:'上传图片',
					  		handler:function(){
					  			  if(Ext.getCmp('advmanager3_sid').value==''){
					  			  		alert("请先点击左边页面模板图片位置");
					  			  		return 0;	
					  			  }
					  			  if(window.navigator.userAgent.indexOf("MSIE")>=1){
					  			  	  gotoedit3(Ext.getCmp('advmanager3_img_long').value, Ext.getCmp('advmanager3_img_width').value);
					  			  }else if(window.navigator.userAgent.indexOf("Firefox")>=1){
					  						gotoedit2(Ext.getCmp('advmanager3_img_long').value, Ext.getCmp('advmanager3_img_width').value, Ext.getCmp('advmanager3_view_long').value, Ext.getCmp('advmanager3_view_width').value);	
					  				}else{
					  						gotoedit2(Ext.getCmp('advmanager3_img_long').value, Ext.getCmp('advmanager3_img_width').value, Ext.getCmp('advmanager3_view_long').value, Ext.getCmp('advmanager3_view_width').value);		
					  				}
					  		}	
						},{
								xtype:'textfield',
								fieldLabel:'图片宽度',
								id:'advmanager3_img_width',
								name:'advmanager3_img_width',
								hidden:true
						},{
								xtype:'textfield',
								fieldLabel:'图片高度',
								id:'advmanager3_img_long',
								name:'advmanager3_img_long',
								hidden:true
						},{
								xtype:'textfield',
								fieldLabel:'框宽度',
								id:'advmanager3_view_width',
								name:'advmanager3_view_width',
								hidden:true
						},{
								xtype:'textfield',
								fieldLabel:'框高度',
								id:'advmanager3_view_long',
								name:'advmanager3_view_long',
								hidden:true
						}],
						buttons:[{
								text:'保存',
		            handler: function() {
		            	 if(Ext.getCmp('advmanager3_name').value==''){
		            	 		alert('商标名称不能为空！');	
		            	 }else if(Ext.getCmp('advmanager3_shid').value==''){
		            	 		alert('商店不能为空！');
		            	 }else{
					             fp.form.doAction('submit',{
							             url:'/pronline/Msg',
							             method:'post',
							             params:{FunName:'ncm_webcomindex4_list'},
							             success:function(form,action){
							             	  for(i=1; i<store_shops_advmanager3.getCount()+1; i++){
							             				if(action.result.level == store_shops_advmanager3.getAt(i-1).get('level')){
							             					  store_shops_advmanager3.getAt(i-1).set('sid', action.result.sid);
							             					  store_shops_advmanager3.getAt(i-1).set('name', action.result.name);
							             					  store_shops_advmanager3.getAt(i-1).set('sgroupid', action.result.sgroupid);
							             					  store_shops_advmanager3.getAt(i-1).set('descr', action.result.descr);
							             					  store_shops_advmanager3.getAt(i-1).set('imgpath', action.result.imgpath);
							             					  store_shops_advmanager3.getAt(i-1).set('serno', action.result.serno);
							             					  store_shops_advmanager3.getAt(i-1).set('floor', action.result.floor);
							             					  store_shops_advmanager3.getAt(i-1).set('level', action.result.level);
							             					  store_shops_advmanager3.getAt(i-1).set('plevel', action.result.plevel);
							             					  store_shops_advmanager3.getAt(i-1).set('type', action.result.type);
							             					  store_shops_advmanager3.getAt(i-1).set('shopid', action.result.shopid);
							             						store_shops_advmanager3.getAt(i-1).set('plateid', action.result.plateid);
							             						store_shops_advmanager3.getAt(i-1).set('flags', action.result.flags);
							             						store_shops_advmanager3.getAt(i-1).set('img_long', action.result.img_long);
							             						store_shops_advmanager3.getAt(i-1).set('img_width', action.result.img_width);
							             						store_shops_advmanager3.getAt(i-1).set('label', action.result.label);
							             				}	
							             		}
							             		Ext.getCmp('advmanager3_ads'+action.result.level).setSrc(action.result.imgpath);
							             		fp.getForm().reset();
															Ext.getCmp('advmanager3_img').setSrc('');	
							             } 
					             })
			             }
		            }	
						},{
								text:'删除',
								handler:function(){
									  if(confirm('您真的要执行删除操作吗？')){
									  	  var selsid;
									  	  selsid = Ext.getCmp('advmanager3_sid').getValue();
									  	  var alev = Ext.getCmp('advmanager3_level').value;
									  	  var mark = Ext.getCmp('advmanager3_label').value;
											  store_shops_advmanager3.load({
											  		params: {del: "del",selsid:selsid,mark:mark},
											  		
											  		callback:function(records, options, success){
											  				eval(shops.advManager3).init();
											  		}
											  });
				             		fp.getForm().reset();
												Ext.getCmp('advmanager3_img').setSrc('');
												Ext.getCmp('advmanager3_ads'+alev).removeCls('stripes');  // 移除背景	
									  }	
								}	
						},{
								text:'重置',
								handler:function(){
									  var alev = Ext.getCmp('advmanager3_level').value;
										fp.getForm().reset();
										Ext.getCmp('advmanager3_img').setSrc('');
										Ext.getCmp('advmanager3_ads'+alev).removeCls('stripes');  // 移除背景	
								}	
						}]
				});
		    var fp2 = Ext.create('Ext.FormPanel', {
						id:'advmanager3_intro',
						hidden:false,
						frame:false,
						border:false,
						margins:'0 0 0 20',
						bodyPadding:0,
						defaultType:'textfield', //设置表单字段的默认类型  
						fieldDefaults:{ //统一设置表单字段默认属性
								labelSeparator:':', //分隔符
								labelWidth:320, //标签宽度
								width:320, //字段宽度
								msgTarget:'side'
						},
						items:[{
								xtype:'fieldset',
								width:350,
								title:'操作说明',
								collapsible: true,
								items:[{
										xtype:'label',
										html:'<div style="height:80px"><br>1.&nbsp;&nbsp;&nbsp;&nbsp;请先点击左边页面模板图片位置<br><br>2.&nbsp;&nbsp;&nbsp;&nbsp;再对其信息进行操作<br></div><br>'
								}]
						}]
				});
				
		    Ext.apply(this, {
		        border:false,
						frame:false,
						autoScroll: true,
						bodyPadding:"5 10 5 10",
		        //store: store_shops_advmanager3,
		        layout: { pack: 'center', type: 'hbox' } ,
			      items:[{
		        	  layout: 'vbox',
			       		width:400,
			       		//height:642,
			       		items:[{
				       			layout:"hbox",
				       			width:400,
				       			height:170,
				       			autoScroll: true,
				       			items:[{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					src:'/images/yuyuandemo/yuyuan3-04_r7_c1.jpg'
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					src:'/images/yuyuandemo/oceans-clip.jpg'
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					src:'/images/yuyuandemo/DiorSnow.jpg'
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:135,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:100,
					       				height:135,
					       				src:'/images/yuyuandemo/yuyuan4-01_r3_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:100,
					       				height:135,
					       				src:'/images/yuyuandemo/yuyuan4-01_r3_c4.jpg'	
				       			},{
				       					xtype:'image',
					       				width:100,
					       				height:135,
					       				src:'/images/yuyuandemo/yuyuan4-01_r3_c12.jpg'	
				       			},{
				       					xtype:'image',
					       				width:100,
					       				height:135,
					       				src:'/images/yuyuandemo/yuyuan4-01_r3_c18.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:32,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:400,
					       				height:32,
					       				src:'/images/yuyuandemo/yuyuan4-01_r5_c1.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:72,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r7_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r7_c6.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r7_c16.jpg'	
				       			}]
			       		},{
				       			layout:"hbox",
				       			width:400,
				       			height:170,
				       			autoScroll: true,
				       			items:[{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads1",
					     					listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(1, 150, 400);
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads2",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		 	gotoedit1(2, 150, 400);		
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads3",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(3, 150, 400);		
							         						}
							         				}
						         		}
				       			}]
			       		},{
				       			layout:"hbox",
				       			width:400,
				       			height:170,
				       			autoScroll: true,
				       			items:[{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads4",
					     					listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(4, 150, 400);
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads5",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		 	gotoedit1(5, 150, 400);		
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads6",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(6, 150, 400);		
							         						}
							         				}
						         		}
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:32,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:400,
					       				height:32,
					       				src:'/images/yuyuandemo/yuyuan4-01_r13_c1.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:72,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r15_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r15_c6.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r15_c16.jpg'	
				       			}]
			       		},{
				       			layout:"hbox",
				       			width:400,
				       			height:170,
				       			autoScroll: true,
				       			items:[{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads7",
					     					listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(7, 150, 400);
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads8",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		 	gotoedit1(8, 150, 400);		
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:400,
					       				height:150,
					     					id: "advmanager3_ads9",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(9, 150, 400);		
							         						}
							         				}
						         		}
				       			}]
			       		},{
				       			layout:"hbox",
				       			width:400,
				       			height:120,
				       			//autoScroll: true,
				       			items:[{
					     					xtype: 'image',
					     					width:133,
					       				height:120,
					     					id: "advmanager3_ads10",
					     					listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(10, 120, 133);
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:133,
					       				height:120,
					     					id: "advmanager3_ads11",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		 	gotoedit1(11, 120, 133);		
							         						}
							         				}
						         		}
				       			},{
					     					xtype: 'image',
					     					width:133,
					       				height:120,
					     					id: "advmanager3_ads12",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(12, 120, 133);		
							         						}
							         				}
						         		}
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:32,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:400,
					       				height:32,
					       				src:'/images/yuyuandemo/yuyuan4-01_r21_c1.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:72,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r23_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r23_c6.jpg'	
				       			},{
				       					xtype:'image',
					       				width:133,
					       				height:72,
					       				src:'/images/yuyuandemo/yuyuan4-01_r23_c16.jpg'	
				       			}]
			       		},{
			       				layout: 'hbox',
				       			width:400,
				       			height:224,
				       			//autoScroll: true,
				       			items:[{
					       				xtype:'image',
					       				width:160,
					       				height:224,
					       				id: "advmanager3_ads13",
					     					listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(13, 224, 160);
							         						}
							         				}
						         		}
				       			},{
				       				  layout:'vbox',
				       				  width:240,
				       					height:224,
				       					items:[{
				       							xtype: 'image',
						       					width:240,
						       					height:112,
						       					id: "advmanager3_ads14",
							     					listeners:{       			
									         				el:{ 
																			click:function(){
																				  gotoedit1(14, 112, 240);
									         						}
									         				}
								         		}
				       					},{
				       							xtype: 'image',
						       					width:240,
						       					height:112,
						       					id: "advmanager3_ads15",
							     					listeners:{       			
									         				el:{ 
																			click:function(){
																				  gotoedit1(15, 112, 240);
									         						}
									         				}
								         		}	
				       					}]
			       				}]		
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:32,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:400,
					       				height:32,
					       				src:'/images/yuyuandemo/yuyuan4-01_r29_c1.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:70,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:200,
					       				height:70,
					       				src:'/images/yuyuandemo/yuyuan4-01_r31_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:200,
					       				height:70,
					       				src:'/images/yuyuandemo/yuyuan4-01_r31_c13.jpg'	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:75,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads16",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(16, 75, 200);		
							         						}
							         				}
						         		}	
				       			},{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads17",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(17, 75, 200);		
							         						}
							         				}
						         		}	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:75,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads18",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(18, 75, 200);		
							         						}
							         				}
						         		}	
				       			},{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads19",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(19, 75, 200);		
							         						}
							         				}
						         		}	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:75,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads20",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(20, 75, 200);		
							         						}
							         				}
						         		}	
				       			},{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				id: "advmanager3_ads21",
					       				listeners:{       			
							         				el:{ 
																	click:function(){
																		  gotoedit1(21, 75, 200);		
							         						}
							         				}
						         		}	
				       			}]
			       		},{
				       			layout: 'hbox',
				       			width:400,
				       			height:75,
				       			hidden:true,
				       			//autoScroll: true,
				       			items:[{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				src:'/images/yuyuandemo/yuyuan4-01_r39_c2.jpg'	
				       			},{
				       					xtype:'image',
					       				width:200,
					       				height:75,
					       				src:'/images/yuyuandemo/yuyuan4-01_r39_c13.jpg'	
				       			}]
			       		}]
		        },fp,fp2],
		        tbar:{items:['-',markCombod]}
		    });
			  store_shops_advmanager3.alreadyload = 1;
		    this.callParent(arguments);
    },
    inheritableStatics : {
    		init:function(){
						for(j=1; j<store_shops_advmanager3.getCount()+1; j++){
								switch(store_shops_advmanager3.getAt(j-1).get('level')){
			  	      		case "1":	Ext.getCmp('advmanager3_ads1').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "2":	Ext.getCmp('advmanager3_ads2').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "3":	Ext.getCmp('advmanager3_ads3').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "4":	Ext.getCmp('advmanager3_ads4').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "5":	Ext.getCmp('advmanager3_ads5').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "6":	Ext.getCmp('advmanager3_ads6').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "7":	Ext.getCmp('advmanager3_ads7').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "8":	Ext.getCmp('advmanager3_ads8').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "9":	Ext.getCmp('advmanager3_ads9').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "10":	Ext.getCmp('advmanager3_ads10').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "11":	Ext.getCmp('advmanager3_ads11').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "12":	Ext.getCmp('advmanager3_ads12').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "13":	Ext.getCmp('advmanager3_ads13').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "14":	Ext.getCmp('advmanager3_ads14').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "15":	Ext.getCmp('advmanager3_ads15').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "16":	Ext.getCmp('advmanager3_ads16').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "17":	Ext.getCmp('advmanager3_ads17').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "18":	Ext.getCmp('advmanager3_ads18').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "19":	Ext.getCmp('advmanager3_ads19').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "20":	Ext.getCmp('advmanager3_ads20').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		case "21":	Ext.getCmp('advmanager3_ads21').setSrc(store_shops_advmanager3.getAt(j-1).get('imgpath')); break;
			  	      		default:break;
			  	      }	
						}
    		}
    }
});

