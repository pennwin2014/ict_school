Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List','common.AddWin','common.UploadWin'],
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
  
    init: function() {
        this.control({
          'authloglist':{
  //         select: this.showTab,
            render: this.showRender
          },
          //�ؼ��ֲ�ѯ
          'authloglist button[action=keycx]':{
                click: this.dwglkeycx
          },
          //���
          'authloglist button[action=advancecx]':{
                click: this.cxyhadd
          },
          			//��ʾ�ϴ�ͼƬ����
		     'addwin button[action=upfile]':{
				  click: this.showupload
			    },
			              //��ӱ���
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
//������ͼ�߶�
        var qgrid=Ext.ComponentQuery.query('authloglist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parseInt(parent.grid_height));
        	
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
           
           
         
       
            var new_params={keyword:keyword};
        
             Ext.apply(store.proxy.extraParams,new_params);

        });//store.on('beforeload', function (store, options) {

        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },// showRender: function(){
   showupload: function(thisBtn){
   var sid=thisBtn.up('form').getForm().findField('sid').getValue();
  
   if(sid==''){
		alert("���ȱ����¼,���ϴ�");
		return 0;	
		}
  
	   	var win = Ext.create('proauthRzAuthlog.view.common.UploadWin',{
	     title: "�ϴ�ͼƬ",
	    field1: sid,
	    field2:''
	    });
	    win.showAt(340,100); 
  
  },
    //�ؼ��ֲ�ѯ
    dwglkeycx: function(thisBtn){
    	 
        proauthRzAuthlog.controller.Procy.searchtype = ""; 
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },
  
    //���
    cxyhadd: function(thisBtn){
    var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
        title: "�����Żݱ༭",
        field1: ""

      });
      win.showAt(50,20); 	  	
    },// dwglavcx: function(thisBtn){
    cxyhaddsave:function(thisBtn){


  	var tstore=Ext.ComponentQuery.query('authloglist')[0].getStore();
  	
   	var tform=thisBtn.up('window').query('form')[0];   	
   
    var aname=tform.getForm().findField('name').value;
    if(aname==''){
    	alert('��������Ʒ����');
    	return;
    }
    var acid=tform.getForm().findField('cid').value;
    if(acid==''){
    	alert('�������̳�����');
    	return;
    }
 
    tform.getForm().doAction('submit',{
                url:'/pronline/Msg',
                 method:'post',
                 waitMsg: '���ڱ���...',
                  params:{FunName:'ncmCxyh_Save'},
                  success:function(form,action){

         tform.getForm().findField('sid').setValue(action.result.sid);  
    //      var store = Ext.ComponentQuery.query('resourcelist')[0].getStore();
          Ext.Msg.alert('����',"����ɹ�");  
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
    * ���Ա�ǩ 
    ***************************************/  
    showTips: function( thisTV, eOpts ){
      thisTV.tip = Ext.create('Ext.tip.ToolTip', {
        target: thisTV.el,
        trackMouse: true,
        dismissDelay : 60000,
        html: '<p>���磺</p><p>&nbsp;&nbsp;&nbsp;�����˿ڣ�80,21</p><p>&nbsp;&nbsp;&nbsp;�˿ڶΣ�2000~3000</p><p>&nbsp;&nbsp;&nbsp;��ϣ�80,2000~3000,3005</p>'
      });
    },
    /**************************************
    * ȫ�ֺ��� 
    ***************************************/
    inheritableStatics:{
        loadProcyListStore:function(){
            var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
         
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

