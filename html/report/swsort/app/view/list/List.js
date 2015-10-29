Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.authloglist',
    store : 'List', 
    height:parent.grid_height,
 //   width:parent.grid_width,
    autoScroll: true,
    title:'',
    columnLines: true,
    initComponent: function() {

        var myDate = new Date();
        var yy = ( myDate.getYear() < 1900 ) ? ( 1900 + myDate.getYear() ) : myDate.getYear();
        var mm=myDate.getMonth()+1;
        if(mm<10)
        {
            mm='0'+mm;
        }
        var dd=myDate.getDate();
        if(dd<10)
        {
            dd='0'+dd;
        }
        var today=yy+'/'+mm+'/'+dd;
        var startday=yy+'/'+mm+'/'+'01';

        this.tbar = Ext.create('Ext.PagingToolbar',{
            store:'List',
            displayInfo: true,
            displayMsg: '��ʾ{0} - {1}����¼ ��{2}����¼',
            emptyMsg: "û�м�¼����ʾ",
            items:[
                {                        
                    fieldLabel:'��ʼʱ��',
                    xtype: 'datefield',
                    format: "Y/m/d",
                    labelWidth: 60,
                    width:160,
                    value:startday,
                    name:'sdate',
                    id:'sdate'
                },
                {
                    fieldLabel:'',
                    width:60,
                    xtype: 'textfield',
                    value:'00:00:00',
                    name:'stime',
                    regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
                    regexText: "��ʽ��xx:xx:xx",
                    allowBlank  : false,
                    id:'stime'
                },
                 {
                    fieldLabel:'����ʱ��',
                    labelWidth: 60,
                    width:160,
                    xtype: 'datefield',
                    id:'edate',
                    format: "Y/m/d",
                    value:today,
                    name:'edate'
                },
                {
                    fieldLabel:'',
                    width:60,
                    xtype: 'textfield',
                    value:'23:59:59',
                    name:'etime',
                    regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
                    regexText: "��ʽ��xx:xx:xx",
                    allowBlank  : false,
                    id:'etime'
                },                   
                  
                {xtype:'combo',mode:'local', 
                	  fieldLabel:'״̬',  
                	  labelWidth: 50,              
                    triggerAction:  'all',
                    forceSelection: true,
                    width:140,
                    editable:       false,   
                    id:             'swstate',                           
                    name:           'swstate',
                    value:          '',
                    displayField:   'name',
                    valueField:     'value',
                    queryMode:     'local',
                    store: Ext.create('Ext.data.Store', 
                    {
                        fields : ['name', 'value'],
                        data   : 
                        [
                            {name : 'ȫ��',   value: ''},
                            {name : '����',  value: '2'},
                            {name : '����',  value: '0'}
                        ]
                    }),
                    emptyText:'',allowBlank:true
                },  
                {
                    text:'ͳ��',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
                {
                    text:'����',
                    anchor: 'right',
                    action:'advancecx',
                    iconCls:'add'
                },
	            '-',
                {
                    text:'����',
                    itemId: 'disableButton',
                    iconCls:'remove',
                    disabled: true,
                    action:'dsable'
                },
	            '-',
                {
                    text:'����',
                    itemId: 'enableButton',
                    iconCls:'accept',
                    disabled: true,
                    action:'enable'
                },
               
                '-',
         
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{

        var sm = Ext.create('Ext.selection.CheckboxModel',{
            listeners: {
                selectionchange: function(sm, selections) {  
                	var grid=Ext.ComponentQuery.query('authloglist')[0];                  
                    grid.down('#disableButton').setDisabled(selections.length == 0);
                    grid.down('#enableButton').setDisabled(selections.length == 0);  
                }
            }    
        });
        this.selModel=sm;
     
        this.columns = [
            {header: '���', dataIndex: 'num',  width:110},
            {header: '�û�����', dataIndex: 'cname',width:180,sortable: false,itemId: 'corpColumn'
            },
            {header: '��������', dataIndex: 'lcount',  width:110},
            {header: '״̬', dataIndex: 'status',  width:110, 
                renderer:function(value,metaData,record){
                    if(value=='0'){
                        return '<font color=green>����</font>';
                    }
                    else if(value=='2'){
                        return '<font color=red>����</font>';
                    }
                    else {
                        return value;
                    }
                }                
            },
            {flex:1,width:300}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
