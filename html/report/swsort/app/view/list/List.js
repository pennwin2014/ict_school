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
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[
                {                        
                    fieldLabel:'起始时间',
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
                    regexText: "格式：xx:xx:xx",
                    allowBlank  : false,
                    id:'stime'
                },
                 {
                    fieldLabel:'结束时间',
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
                    regexText: "格式：xx:xx:xx",
                    allowBlank  : false,
                    id:'etime'
                },                   
                  
                {xtype:'combo',mode:'local', 
                	  fieldLabel:'状态',  
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
                            {name : '全部',   value: ''},
                            {name : '禁用',  value: '2'},
                            {name : '正常',  value: '0'}
                        ]
                    }),
                    emptyText:'',allowBlank:true
                },  
                {
                    text:'统计',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
                {
                    text:'导出',
                    anchor: 'right',
                    action:'advancecx',
                    iconCls:'add'
                },
	            '-',
                {
                    text:'禁用',
                    itemId: 'disableButton',
                    iconCls:'remove',
                    disabled: true,
                    action:'dsable'
                },
	            '-',
                {
                    text:'启用',
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
            {header: '序号', dataIndex: 'num',  width:110},
            {header: '用户名称', dataIndex: 'cname',width:180,sortable: false,itemId: 'corpColumn'
            },
            {header: '上网天数', dataIndex: 'lcount',  width:110},
            {header: '状态', dataIndex: 'status',  width:110, 
                renderer:function(value,metaData,record){
                    if(value=='0'){
                        return '<font color=green>正常</font>';
                    }
                    else if(value=='2'){
                        return '<font color=red>禁用</font>';
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
