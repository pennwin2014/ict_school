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

        var wayCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '统计方式',
        labelWidth: 60,
        width:150,
        valueField:'value',
        displayField: 'name',
        id:'statway',
        name : 'statway',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                    
                                        {name : '按单位统计',   value: '0'},
                                        {name : '按时间段统计',   value: '1'}
                                                                      
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
			          'change':function(){
			          
			            }
			        }
    });


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
                        labelWidth: 65,
                        width:170,
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
                        labelWidth: 65,
                        width:170,
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
                    wayCombo,
                    
                    
                {
                    text:'统计',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
                {
                    text:'导出',
                    anchor: 'right',
                    action:'output',
                    name:'exp',
                    id:'exp',
                    iconCls:'add'
                    
                },
                '-',
         
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{

        this.columns = [
        {
          header: '详情',
            xtype: 'actioncolumn',
            hidden:true,
            dataIndex: 'sid',
             icon: '/newver/resources/themes/images/share/page_white_edit.png',
            tooltip: '点击该图标进入查看界面。',
            width: 60,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
            	
            	  var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
					        title: "促销优惠编辑",
					        field1: record
					
					      });
					      win.showAt(50,20); 	  	
            	
               
            }

        },
            {header: '单位名称', dataIndex: 'cname',width:180,sortable: false,itemId: 'corpColumn'
},
            {header: '时间段', hidden:true, dataIndex:'datarang', width:140,sortable: false,itemId: 'dataColumn'},
            {header: '上网人数', dataIndex: 'count',  width:110},
            {flex:1,width:300}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
