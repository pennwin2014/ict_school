Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List','common.AddWin','common.UploadWin','list.facGroupTree'],
    servicecode:'',
    servicename:'',
    mac:'',
    apname:'',
    apmac:'',
    ssid:'',
    sdate:'',
    stime:'',
    edate:'',
    etime:'',
    searchtype:'',
    actionName:'',
    selsid:'',
    swstate:'',
  
    init: function() {
        this.control({
            'authloglist':{
            //         select: this.showTab,
            render: this.showRender
            },
            //关键字查询
            'authloglist button[action=keycx]':{
                click: this.dwglkeycx
            },
            //添加
            'authloglist button[action=advancecx]':{
                click: this.swtsexport
            },
            'authloglist button[action=dsable]':{
                click: this.dsableUsers
            },
            'authloglist button[action=enable]':{
                click: this.enableUsers
            },
          
          			//显示上传图片界面
		     'addwin button[action=upfile]':{
				  click: this.showupload
			    },
			              //添加保存
          'addwin button[action=add]':{
                click: this.cxyhaddsave
          },
          'authloglist button[action=del]':{
            click: this.cxyhdel
          },
          'authloglist button[action=hidShw]':{
            click: this.cxyhhidshw
          }
        });
    },
    
    showRender: function(){
        var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
        store.on('beforeload', function (store, options) {
//调整视图高度
        var qgrid=Ext.ComponentQuery.query('authloglist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parseInt(parent.grid_height));
        	
        var sdate = Ext.getCmp('sdate').value;
        var edate= Ext.getCmp('edate').value;
        var stime= Ext.getCmp('stime').value;
        var etime= Ext.getCmp('etime').value;
        var swstate=Ext.getCmp('swstate').value;
        var did=document.getElementById('form1').did.value;
        var actionName = proauthRzAuthlog.controller.Procy.actionName;
        var selsid = proauthRzAuthlog.controller.Procy.selsid;
        var new_params={sdate:sdate,edate:edate,stime:stime,etime:etime,did:did,actionName:actionName,selsid:selsid,swstate:swstate};
        
         Ext.apply(store.proxy.extraParams,new_params);
         proauthRzAuthlog.controller.Procy.searchtype = '';
         proauthRzAuthlog.controller.Procy.actionName = '';
        });//store.on('beforeload', function (store, options) {

        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },// showRender: function(){
   showupload: function(thisBtn){
   var sid=thisBtn.up('form').getForm().findField('sid').getValue();
  
   if(sid==''){
		alert("请先保存记录,再上传");
		return 0;	
		}
  
	   	var win = Ext.create('proauthRzAuthlog.view.common.UploadWin',{
	     title: "上传图片",
	    field1: sid,
	    field2:''
	    });
	    win.showAt(340,100); 
  
  },
    //关键字查询
    dwglkeycx: function(thisBtn){
    	var tgrid=Ext.ComponentQuery.query('authloglist')[0];
        proauthRzAuthlog.controller.Procy.searchtype = '';
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },
  
    //导出
    swtsexport: function(thisBtn){
        var form1 = document.getElementById('form1');
        
        var sdate = Ext.getCmp('sdate').value; 
        form1.sdate.value = sdate ?  Ext.Date.format(sdate, 'Y/m/d') : '';
        var stime = Ext.getCmp('stime').value; 
        form1.stime.value = stime ? stime : '';
        var edate = Ext.getCmp('edate').value;
        form1.edate.value = edate ?  Ext.Date.format(edate, 'Y/m/d') : '';
        var etime = Ext.getCmp('etime').value; 
        form1.etime.value = etime ? etime : '';

        form1.exp.value='exp';
        form1.submit();
        form1.exp.value="";        
    },// dwglavcx: function(thisBtn){
    dsableUsers: function(thisBtn){
    	//var tgrid=Ext.ComponentQuery.query('authloglist')[0];
        proauthRzAuthlog.controller.Procy.searchtype = '';
        proauthRzAuthlog.controller.Procy.actionName = 'dsableUsers';

        var grid=Ext.ComponentQuery.query('authloglist')[0];
        var tstore=grid.getStore();
        var rows = grid.getSelectionModel().getSelection(); 
        selsid='';
        for(var i=0;i<rows.length;i++){
            if(i==0){
                selsid= rows[i].get('cname');
            } else{
                selsid=selsid + "," + rows[i].get('cname');
            }
        }  
        proauthRzAuthlog.controller.Procy.selsid  = selsid;
        
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
        
      //  proauthRzAuthlog.controller.Procy.actionName = '';
      //  proauthRzAuthlog.controller.Procy.selsid  ='';
      //  proauthRzAuthlog.controller.Procy.loadProcyListStore();
        
    },
    enableUsers: function(thisBtn){
    	//var tgrid=Ext.ComponentQuery.query('authloglist')[0];
        proauthRzAuthlog.controller.Procy.searchtype = '';
        proauthRzAuthlog.controller.Procy.actionName = 'enableUsers';
        
        var grid=Ext.ComponentQuery.query('authloglist')[0];
        var tstore=grid.getStore();
        var rows = grid.getSelectionModel().getSelection(); 
        selsid='';
        for(var i=0;i<rows.length;i++){
            if(i==0){
                selsid= rows[i].get('cname');
            } else{
                selsid=selsid + "," + rows[i].get('cname');
            }
        }  
        proauthRzAuthlog.controller.Procy.selsid  = selsid;
        
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    //    proauthRzAuthlog.controller.Procy.actionName = '';
    //    proauthRzAuthlog.controller.Procy.selsid  ='';
    //    proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },        
    cxyhaddsave:function(thisBtn){

  	var tstore=Ext.ComponentQuery.query('authloglist')[0].getStore();
  	
   	var tform=thisBtn.up('window').query('form')[0];   	
   
    var aname=tform.getForm().findField('name').value;
    if(aname==''){
    	alert('请输入商品名称');
    	return;
    }
    var acid=tform.getForm().findField('cid').value;
    if(acid==''){
    	alert('请输入商城名称');
    	return;
    }
 
    tform.getForm().doAction('submit',{
                url:'/pronline/Msg',
                 method:'post',
                 waitMsg: '正在保存...',
                  params:{FunName:'ncmCxyh_Save'},
                  success:function(form,action){

         tform.getForm().findField('sid').setValue(action.result.sid);  
    //      var store = Ext.ComponentQuery.query('resourcelist')[0].getStore();
          Ext.Msg.alert('操作',"保存成功");  
          tstore.load();         
                  }
                  ,
                 failure:function(form,action){
                 	Ext.Msg.alert(action.result.msg);

                   }         
                 })  	
   },
     cxyhdel:function(thisBtn){
      var grid=Ext.ComponentQuery.query('authloglist')[0];
       var tstore=grid.getStore();
       var rows = grid.getSelectionModel().getSelection(); 
       selsid='';
         for(var i=0;i<rows.length;i++){
          if(i==0){
           selsid="'" + rows[i].get('sid') + "'";
           }
           else{
            selsid=selsid + "," + "'" + rows[i].get('sid') + "'";
             }
           }
          tstore.load({params:{del:"del",selsid:selsid}});
         },
    cxyhhidshw:function(thisBtn){
      var grid=Ext.ComponentQuery.query('authloglist')[0];
       var tstore=grid.getStore();
       var rows = grid.getSelectionModel().getSelection(); 
       selsid='';
         for(var i=0;i<rows.length;i++){
          if(i==0){
           selsid="'" + rows[i].get('sid') + "'";
           }
           else{
            selsid=selsid + "," + "'" + rows[i].get('sid') + "'";
             }
           }
          tstore.load({params:{hidshw:"hidShw",selsid:selsid}});
         },
    /**************************************
    * 策略标签 
    ***************************************/  
    showTips: function( thisTV, eOpts ){
      thisTV.tip = Ext.create('Ext.tip.ToolTip', {
        target: thisTV.el,
        trackMouse: true,
        dismissDelay : 60000,
        html: '<p>例如：</p><p>&nbsp;&nbsp;&nbsp;单个端口：80,21</p><p>&nbsp;&nbsp;&nbsp;端口段：2000~3000</p><p>&nbsp;&nbsp;&nbsp;组合：80,2000~3000,3005</p>'
      });
    },
    /**************************************
    * 全局函数 
    ***************************************/
    inheritableStatics:{
        loadProcyListStore:function(){
            var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
      //      var keyword = Ext.getCmp('keyword_mobileaccount').value;
         
            store.load();
        },
        SetPage:function(curpage){
            var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
            store.currentPage = curpage;
        },
        setTitle: function(title){
             document.getElementById("titledx").innerHTML = 
                 '&nbsp'+ title +'&nbsp;&nbsp;';   
        }
    }
   
});

