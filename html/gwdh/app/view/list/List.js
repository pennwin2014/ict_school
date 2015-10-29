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
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:180,
                    maxLength:128,
                    name:'keyword5',
                    style:'color:#7aa7d5',             
                    emptyText:'单位名称/楼层/经营描述',
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
                    text:'查询',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
                {
	                 text:'增加',
	                 iconCls:'add',
		             action:'addFloor'
	            },
	            '-',
                {
                    text:'删除',
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
                header: '编辑',
                xtype: 'actioncolumn',
                dataIndex: 'floorId',
                icon: '/images/v8/edit_task.png',
                tooltip: '点击该图标进入修改楼层信息界面。',
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
            {header: '单位名称', dataIndex: 'mallName',width:200,sortable: false},
            {header: '楼层', dataIndex: 'floorName', width:110,sortable: true},
            {header: '经营描述', dataIndex: 'floorDetail',  width:310},
            {header: '品牌数量', dataIndex: 'brandCount',   width:110},
            {
                header: '品牌编辑',
                xtype: 'actioncolumn',
                dataIndex: 'floorId',
                icon: '/images/v8/edit_task.png',
                tooltip: '点击该图标进入添加品牌界面。',
                width: 60,
                align: 'center',
                sortable: false,
                handler: function(grid, rowIndex, colIndex, actionItem, event,
                    record, row) {                      
            	    var win = Ext.create('dennisShoppingGuiding.view.common.AddWin',{
                        title: "品牌编辑", field1: record});
					win.showAt(50,20);
                }
            },
            {flex:1}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
});
