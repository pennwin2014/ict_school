Ext.define('dennisShoppingGuiding.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.shoppingGuidList',
    store : 'List', 
    height:parent.grid_height - 50,
    width:parent.grid_width - 50,
    autoScroll: true,
    title:'',
    columnLines: true,
    initComponent: function() {

        this.tbar = Ext.create('Ext.PagingToolbar',{
            store:'List',
            displayInfo: true,
            displayMsg: '��ʾ{0} - {1}����¼ ��{2}����¼',
            emptyMsg: "û�м�¼����ʾ",
            items:[
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:180,
                    maxLength:128,
                    name:'keyword5',
                    style:'color:#7aa7d5',             
                    emptyText:'��λ����/¥��/��Ӫ����',
                    enableKeyEvents:true,
                    listeners:{
                        'focus':function(){
                        },
              
                        'keydown' : function(i,e){
                             var aae=e.getKey() ; 
                             if(aae==13){
                             }
                         }
                    }
                },
                {
                    text:'��ѯ',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
                {
	                 text:'����',
	                 iconCls:'add',
		             action:'addFloor'
	            },
	            '-',
                {
                    text:'ɾ��',
                    itemId: 'removeButton',
                    iconCls:'remove',
                    disabled: true,
                    action:'del'
                },
            
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{

        var sm = Ext.create('Ext.selection.CheckboxModel',{
            listeners: {
                selectionchange: function(sm, selections) {  
                	var grid=Ext.ComponentQuery.query('shoppingGuidList')[0];                  
                    grid.down('#removeButton').setDisabled(selections.length == 0);          
                }
            }    
        });
        this.selModel=sm;
     
        this.columns = [
            {
                header: '�༭',
                xtype: 'actioncolumn',
                dataIndex: 'floorId',
                icon: '/images/v8/edit_task.png',
                tooltip: '�����ͼ������޸�¥����Ϣ���档',
                width: 50,
                align: 'center',
                sortable: false,
                handler: function(grid, rowIndex, colIndex, actionItem, event, 
                    record, row) {
                    dennisShoppingGuiding.controller.Procy.showAddFloorWin(
                        record.get('floorId'), record.get('floorName'),
                        record.get('floorDetail'), record.get('mallId'), record);
                }
            },
            {header: '��λ����', dataIndex: 'mallName',width:200,sortable: false},
            {header: '¥��', dataIndex: 'floorName', width:110,sortable: true},
            {header: '��Ӫ����', dataIndex: 'floorDetail',  width:310},
            {header: 'Ʒ������', dataIndex: 'brandCount',   width:110},
            {
                header: 'Ʒ�Ʊ༭',
                xtype: 'actioncolumn',
                dataIndex: 'floorId',
                icon: '/images/v8/edit_task.png',
                tooltip: '�����ͼ��������Ʒ�ƽ��档',
                width: 60,
                align: 'center',
                sortable: false,
                handler: function(grid, rowIndex, colIndex, actionItem, event,
                    record, row) {                      
            	    var win = Ext.create('dennisShoppingGuiding.view.common.AddWin',{
                        title: "Ʒ�Ʊ༭", field1: record});
					win.showAt(50,20);
                }
            },
            {flex:1}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
});
