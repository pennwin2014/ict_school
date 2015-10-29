/**
 定义部门树
 */
 
 Ext.define('systems.autopri', {
    extend: 'Ext.tree.Panel',
  //  alias: 'widget.lanGroupCheckTree',
   
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
    	 
  //  	store_systems_autopri.on('beforeload', function (store, options) {
	
 //      var new_params = { state: Ext.getCmp('compstate').getValue() };
  //      Ext.apply(store.proxy.extraParams, new_params);
 //      });
	   	function reflash(){
			store_systems_autopri.currentPage=1;
			store_systems_autopri.load();
		
		}
			Ext.define('CheckData', {
			  extend: 'Ext.data.Model',
			  fields: [
			      'groupcode', 'groupname'
			  ],
			  idProperty: 'rolename'
		});
		//取权限check数据
		var store_type = Ext.create('Ext.data.Store', {
		    pageSize: 1000,
		    model: 'CheckData',
		    remoteSort: true,
		    proxy: {
		        type: 'ajax',
		        url: '/pronline/Msg?FunName@ncm_shopgtype&type@2',
		        reader: {
		            type:'json',
		            root: 'eimdata',
		            totalProperty: 'totalCount'
		        },         
		        simpleSortMode: true
		    }
		});
		store_right.load();
		
     //高级查询窗口
	function gotoedit1(value, id){
		var win3 = null;
		var groupname = '';
		var title='增加';
		var type='';
		var id1='';
		var autodef=false;
		var typeflag=false;
		if(value == 'update'){
		title='修改';
		groupname = store_systems_autopri.getAt(id).get('groupname');
		type = store_systems_autopri.getAt(id).get('type');
		id1 = store_systems_autopri.getAt(id).get('id');
		autodef = store_systems_autopri.getAt(id).get('autodef');
		if(autodef == 1){
		autodef=true;
		}else{
		autodef=false;
		}
		typeflag=true;
		}
		
	    if (!win3) //判断如果不存在就创建新的
	    {
	        win3 = new Ext.Window({
					    title:'自定义权限窗口 - '+ title,
					    closeAction: 'hide',
					    layout:'form',  
					    width:300,
					    draggable:true, //可拖动的
						  maximizable:true, //可最大化的
				      //resizable: true, //可改变大小
						  modal: false,//后面的内容可以操作
				      //plain：true,//则主体背景透明
				      //items: fp2
					    constrain:true, //限制窗口只能在其容器内移动
				      //minimizable:true, //可最小化
				      
	            items:[new Ext.FormPanel({
					id:'autopri_form2',
					layout:"form",
					baseCls:"x-plain",
					bodyPadding: 5,
					items:[
						{
	                        	fieldLabel:'权限名称',
	                        	xtype: 'textfield',
								emptyText:'请输入权限名称',
								value:groupname,
	                        	name:'groupname'
	                    }
					]
	                })],  
					        buttons:[{
										  text:"确定",
										  iconCls:'icon-save',		  
										  handler:function(){
						        			var objectTmp = Ext.getCmp("autopri_form2").getForm().getValues();
											var groupname1 = objectTmp.groupname;
											var parentcode1 = "";
											if(Ext.getCmp("tree_autopri").getSelectionModel().getLastSelected()){
											parentcode1 = Ext.getCmp("tree_autopri").getSelectionModel().getLastSelected().get("id");
											}else{
											alert("请选择一类")
											}
											var shopcode1=Ext.getCmp('autopri_typesearch').value;
											//alert(parentcode1);
											if(groupname1==""){
											alert("请输入自定义权限名称")
											}
											else{
											store_systems_autopri.load({params: {update:value,groupname:groupname1,id:id1,parentcode:parentcode1,shopcode:shopcode1}});
											store_systems_autopri.load({params: {shopcode:shopcode1}});
											//alert(111)
											//reflash();
											win3.close();
											}

											}
									},{
											text:"取消",
											handler: function(){
													win3.close();
											}
									}]
					});
			}
	    win3.show();
	
}
    Ext.apply(this, {
        title: '',
    	  margins: '0 0 0 0',
    	  id:'tree_autopri',
        store: store_systems_autopri,
        autoScroll: true,
        border:false,
		    forceFit: true,
        useArrows: false,
        dockedItems: [{
            xtype: 'toolbar',
            items: [
            '-',{
								xtype:'combo',
								valueField: 'groupcode',
								displayField: 'groupname',
								emptyText:'请选择类别',   
								store: store_type,
								forceSelection: true,
								triggerAction: 'all',
								afterLabelTextTpl: required,
								//allowBlank:false, //是否允许为空
								//blankText:'省份不能为空！',	
								msgTarget:'qtip', //显示一个浮动的提示信息 	
							  id:'autopri_typesearch',
								listeners: {
										'change':function(){
												var shopcode1 = this.value
												store_systems_autopri.load({params: {shopcode:shopcode1}});
				            }
		        		},
						},'-',{
                text:'查询',
                iconCls:'accept',
 //               disabled: true,
                handler:function(){
    
                	  var shopcode1=Ext.getCmp('autopri_typesearch').value;
                      //var vkeyword=Ext.getCmp('keyword').value;			         
						store_systems_autopri.load({params: {shopcode:shopcode1}});

   
                }
 
                
            },'-',{
                 text:'增加',
                 iconCls:'add',
	               handler : function() {
	               	  gotoedit1('add', '');
	               }
            },'-',{
                text:'删除',
								id:'srvpar_removeButton',
                iconCls:'remove',
                disabled: true,
                handler:function(){
							  if(confirm('您真的要执行删除操作吗？')){                  
			                var selsid;  
			                var seltype;  
			                var rows=sm.getSelection();
			                selsid='';
			                seltype='';
			                for(var i=0;i<rows.length;i++){
			                  	if(i==0){
			                  	   selsid="" + rows[i].get('pid') + "";
			                  	   seltype="" + rows[i].get('ptype') + "";
			                  	}
			                  	else{
			                       selsid=selsid + "," + "" + rows[i].get('pid') + "";
			                       seltype=seltype + "," + "" + rows[i].get('ptype') + "";
			                    }
			                }
                   }
                }
            }]
        }]
    });
	  store_systems_autopri.alreadyload = 1;
    this.callParent(arguments);
    }
});

