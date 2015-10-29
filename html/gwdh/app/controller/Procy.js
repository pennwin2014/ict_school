Ext.define('dennisShoppingGuiding.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List','common.AddWin'],    
    servicecode:'',
    servicename:'',
    mallId:0,
    floorName:'',
    floorDetail:'',
    actionName:'',
    searchtype:'',
    floorId:0,
  
    init: function() {
        this.control({
            'shoppingGuidList':{
            //         select: this.showTab,
            render: this.showRender
            },
            //�ؼ��ֲ�ѯ
            'shoppingGuidList button[action=keycx]':{
            click: this.dwglkeycx
            },
            'shoppingGuidList button[action=del]':{
            click: this.gwdhdel
            },            
            //���¥��
            'shoppingGuidList button[action=addFloor]':{
            click: this.addFloor
            }
        });        
    },

    showRender: function(){
        var store = Ext.ComponentQuery.query('shoppingGuidList')[0].getStore();
        store.on('beforeload', function (store, options) {
            //������ͼ�߶�
            var qgrid=Ext.ComponentQuery.query('shoppingGuidList')[0];
        	parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
            qgrid.setHeight(parseInt(parent.grid_height));    	
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
            var mallId = dennisShoppingGuiding.controller.Procy.mallId;
            var floorName = dennisShoppingGuiding.controller.Procy.floorName; 
            var floorDetail = dennisShoppingGuiding.controller.Procy.floorDetail;
            var actionName = dennisShoppingGuiding.controller.Procy.actionName;
            var floorId = dennisShoppingGuiding.controller.Procy.floorId;
                                  
            var new_params={keyword:keyword,mallId:mallId,actionName:actionName,floorName:floorName,floorDetail:floorDetail,floorId:floorId}; 
            Ext.apply(store.proxy.extraParams,new_params);
            dennisShoppingGuiding.controller.Procy.actionName = '';
        });//store.on('beforeload', function (store, options) {

        dennisShoppingGuiding.controller.Procy.loadProcyListStore();
    },// showRender: function(){

    //�ؼ��ֲ�ѯ
    dwglkeycx: function(thisBtn){
        dennisShoppingGuiding.controller.Procy.searchtype = ""; 
        dennisShoppingGuiding.controller.Procy.SetPage(1);  
        dennisShoppingGuiding.controller.Procy.loadProcyListStore();
    },

    gwdhdel:function(thisBtn){
        var grid=Ext.ComponentQuery.query('shoppingGuidList')[0];
        var tstore=grid.getStore();
        var rows = grid.getSelectionModel().getSelection(); 
        selsid='';
        for(var i=0;i<rows.length;i++){
            if(i==0){
                selsid="'" + rows[i].get('floorId') + "'";
            } else{
                selsid=selsid + "," + "'" + rows[i].get('floorId') + "'";
            }
        }
        tstore.load({params:{actionName:"delete",selsid:selsid}});
    },
           
    //���¥��
    addFloor: function(thisBtn){
        dennisShoppingGuiding.controller.Procy.showAddFloorWin(0, '', '', 0, null);
    },// addFloor: function(thisBtn){
  
    /**************************************
    * ���Ա�ǩ 
    ***************************************/  
    showTips: function( thisTV, eOpts ){
      thisTV.tip = Ext.create('Ext.tip.ToolTip', {
        target: thisTV.el,
        trackMouse: true,
        dismissDelay : 60000,
        html: '<p>���磺</p><p>&nbsp;&nbsp;&nbsp;�����˿ڣ�80,21</p><p>&nbsp;&nbsp;&nbsp; �˿ڶΣ�2000~3000</p><p>&nbsp;&nbsp;&nbsp;��ϣ�80,2000~3000,3005</p>'
      });
    },
    /**************************************
    * ȫ�ֺ��� 
    ***************************************/
    inheritableStatics:{
        loadProcyListStore:function(){
            var store = Ext.ComponentQuery.query('shoppingGuidList')[0].getStore();
            var keyword = Ext.getCmp('keyword_mobileaccount').value;         
            store.load();
        },
        SetPage:function(curpage){
            var store = Ext.ComponentQuery.query('shoppingGuidList')[0].getStore();
            store.currentPage = curpage;
        },
        setTitle: function(title){
             document.getElementById("titledx").innerHTML = 
                 '&nbsp'+ title +'&nbsp;&nbsp;';   
        },
        showAddFloorWin: function(floorId, floorName, floorDetail, mallId, record){
            var ensureButtonText = 
                0 == floorId ? '&nbsp;&nbsp;��&nbsp;&nbsp;�� &nbsp;&nbsp;' : '&nbsp;&nbsp;��&nbsp;&nbsp;�� &nbsp;&nbsp;';  
            var mallName = record ? record.get('mallName') : '';
            var brandCount = record ? record.get('brandCount') : '';
            mallId = 0 == mallId ? '' : mallId;
            var fp2 = Ext.create('Ext.FormPanel', {         
                frame: true,
                layout: 'form',
                //width: 580,//parent.width,
                //height: 330,//parent.height,
                fieldDefaults: {
                   labelWidth: 85,                        
                   width: 450
                },
                collapsible: false,
                bodyPadding: 10,
                items: [
                    {
                        xtype: 'hidden',
                        value:floorId,
                        name:'floorId',
                        id:'floorId'
                    },              
                    {
                        xtype: 'hidden',
                        value: mallName,
                        name:'mallName',
                        id:'mallName'
                    },
                    {
                        layout: 'hbox',
                        xtype: 'container',
                        items:[
                            {
                                xtype:'combo',
                                id: 'mallIdComb', 
                                fieldLabel:'�����̳�',
                                name: 'mallIdNa', 
                                hiddenName:'mallId',
                                valueField: 'id',  
                                displayField: 'name',  
                                hidden:false,
                                allowBlank:false,
                                blankText:'�̳����Ʋ���Ϊ�գ�',
                                //forceSelection:true,
                                editable:false,
                                msgTarget:'qtip', //��ʾһ����������ʾ��Ϣ 
                                store:store_comcorps,
                                triggerAction:'all',
                                queryMode:'local',
                                value: mallId,
                                typeAhead:true
                                ,
                                listeners : {
                                    afterRender : function(combo) {
                                        var cnt =  store_comcorps.getTotalCount();
//                                        msgTip = Ext.MessageBox.show({ 
//                                        title:'��ʾ', 
//                                        width : 250, 
//                                        msg:'totalcount' + cnt + '' 
//                                        });                                         
                                        //Ext.Msg.alert('info', );
                                        if (cnt) {
                                            var rec = store_comcorps.getAt(store_comcorps.find('id', mallId))
//                                            combo.setValue(rec.get('name'));//ͬʱ������Ὣ��nameΪfirstValueֵ��Ӧ�� text��ʾ
                                        }
                                    }
//                                    ,
//                                    onload : function() {
//                                    }
                                }                                
                            }
                        ]
                    },                         
                    {
                        layout: 'hbox',
                        xtype: 'container',       
                        items:[
                            {
                                fieldLabel:'¥������',
                                xtype: 'textfield',
                                maxLength:32,
                                name:'floorName',
                                id:'floorName',
                                value:floorName
                            } 
                        ]
                    },
                    {
                        layout: 'hbox',
                        xtype: 'container',       
                        items:[
                            {
                                fieldLabel:'��Ӫ����',
                                xtype: 'textarea',
                                maxLength:65535,
                                name:'floorDetail',
                                id:'floorDetail',
                                value:floorDetail,
                                height:250
                            }
                        ]
                    },
                    {
                        xtype: 'hidden',
                        value: brandCount,
                        name:'brandCount',
                        id:'brandCount'
                    },                       
                    {
                        layout: 'column',
                        xtype: 'container',
                         
                        items:[
                            {
                                xtype: 'container',
                                columnWidth: .20
                            },
                            {
                                //fieldLabel:'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;          ',
                                //labelWidth: 85,                        
                                xtype: 'button',
                                anchor: 'left',
                                //style : 'margin-left:80px',
                                text:ensureButtonText,
                                columnWidth: .25, 
                                handler: function(){
                                    var grid = Ext.ComponentQuery.query('shoppingGuidList')[0];             
                                    var store = grid.getStore();
//                                    var rec = store_comcorps.getAt(store_comcorps.find('name', 
//                                        fp2.getForm().getValues().mallIdNa));

                                    dennisShoppingGuiding.controller.Procy.mallId = 
//                                        rec ?  rec.get('id'): fp2.getForm().getValues().mallIdNa;
                                        fp2.getForm().getValues().mallIdNa;                                       
                                    dennisShoppingGuiding.controller.Procy.floorName = 
                                        fp2.getForm().getValues().floorName; 
                                    dennisShoppingGuiding.controller.Procy.floorDetail = 
                                        fp2.getForm().getValues().floorDetail;
                                    dennisShoppingGuiding.controller.Procy.floorId =
                                        fp2.getForm().getValues().floorId;
        							dennisShoppingGuiding.controller.Procy.actionName = 
        							    0 < dennisShoppingGuiding.controller.Procy.floorId
        							    ? "modifyFloor" : "addFloor";
                                    store.load(
                                        {callback: function(records, options, success){
                                            Ext.Msg.alert('info', '�������');
                                            if (dennisShoppingGuiding.controller.Procy.floorId <= 0) {
                                                Ext.getCmp("floorName").setValue('');
                                                Ext.getCmp("floorDetail").setValue('');                                            
                                            }
                                        }                              
                                        });
                                }
                            },
                            {
                                xtype: 'container',
                                columnWidth: .10
                            },
                            {
                             //   fieldLabel:'&nbsp;&nbsp;&nbsp;&nbsp;          ',
                                //labelWidth: 85,                        
                                text: '&nbsp;&nbsp;��&nbsp;&nbsp;�� &nbsp;&nbsp;',
                                //style : 'margin-left:50px',
                                xtype: 'button',
                                anchor: 'right',
                                columnWidth: .25, 
                                handler: function(){
                                    fp2.getForm().reset();
                                }
                            },
                            {
                                xtype: 'container',
                                columnWidth: .20
                            }
                        ]
                    }
                ]// items: [
            });// var fp2=Ext.create('Ext.FormPanel', {  

            var win3 = Ext.widget('window', {
                title: '����¥��',
    //            closeAction: 'hide',
                x:400,
                y:32,
                width: 580,
                //height: 330,
                //minHeight: 330,
    //            layout: 'fit',
                resizable: true,
                draggable:true,
                modal:false,
                items: fp2
            });

//            if(record && record != ''){
//               fp2.getForm().loadRecord(record);  
//            }              
            //Ext.getCmp("mgrouplist").setValue(mallId);
            //fp2.getForm().setValues({mgrouplist:mallId});
            win3.show();
        }//showAddFloorWin: function(floorId, floorName, floorDetail, mallId){ 
    }//inheritableStatics:{
   
});

