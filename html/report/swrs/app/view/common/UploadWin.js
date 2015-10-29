Ext.define('proauthRzAuthlog.view.common.UploadWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.uploadwin',

	title: '上传图片',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
  initComponent: function() {
  	     var plateid=this.field1;
  	     var plateindex=this.field2;
  	 
              var fp3=    Ext.create('Ext.form.Panel', {
       
                            width: 460,
                            frame: true,
                             bodyPadding: '10 10 0',

                             defaults: {
                               anchor: '100%',
                               allowBlank: false,
                               msgTarget: 'side',
                               labelWidth: 90
                             },

                   items: [{
                      xtype: 'filefield',
             //         id: 'form-file',
                      emptyText: '选择文件上传',
                      fieldLabel: '文件名',
                      name: 'adfile',
                      value:'',
                      buttonText: '',
                      buttonConfig: {
                         iconCls: 'upload-icon'
                      }
                  }
                 ,
                 {xtype: 'hidden',name: 'sid', value: plateid}
                
                 ],

					        buttons: [{
					            text: '上传',
					            handler: function(thisbtn){
					            	var twin=this.up('window');
					            	var temp=this.up('form').getForm().findField('adfile').getValue();
					            	if(temp==''){
					            		alert('请先选取文件上传');
					            		return;
					            	}
           	    
					            	  this.up('form').getForm().doAction('submit',{
					                  url:'/pronline/upload',
					                 method:'post',
					                 enctype:'multipart/form-data',
					                 waitMsg: '正在上传文件...',
					                  params:{FunName:'ncmCxyh_upload',UploadPath:'/home/ncmysql/ncsrv/upload'},
					                  success:function(form,action){
					              			                  	
					                 Ext.getCmp('img_cxyh').setSrc(action.result.adfile);
 
					                 Ext.Msg.alert(action.result.msg);
					                 	var tstore=Ext.ComponentQuery.query('authloglist')[0].getStore();
					                 	tstore.load();
					             //      alert(tstore);
					                   twin.close();
                           

					                  }
					                  ,
					                 failure:function(form,action){
					                 	Ext.Msg.alert(action.result.msg);
					                   }    
					                 })
					
					            }
					        },{
					            text: '取消',
					            handler: function() {
					                this.up('form').getForm().reset();
					               this.up('window').close();
					               
					            }
					        }]
					    });
    this.items = [
       {
        xtype: 'container',       
        layout: 'hbox',
             
        items: [fp3
        
        ]
    }
    ];

    
    this.callParent(arguments);
   
  
  }
});