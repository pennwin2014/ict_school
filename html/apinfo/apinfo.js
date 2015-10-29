

var store_ap_info = Ext.create('Ext.data.Store', 
{
    id:'store_ap_info',
    fields: ['mac', 'pos'],
    proxy: 
    {
        type: 'ajax',
        url: '/pronline/Msg?FunName@query_ap_info',
        reader: 
        {
            type:'json',
            root: 'record'
        }
    },
    sorters: []
});


Ext.onReady(function()
{
    var grid_page = Ext.create('Ext.grid.Panel',
    {
        height: 400,
        width: '100%',
        autoScroll: true,
        store: store_ap_info,
        renderTo: 'divPos',
        columns:
        [
            {
                text: "AP MAC ",
                dataIndex: 'mac',
                width: '49%',
                align: 'center',
                hidden: false,
                sortable: false
            },
            {
                text: "AP POS",
                dataIndex: 'pos',
                width: '49%',
                align: 'center',
                hidden: false,
                sortable: false
            }
        ],
        columnLines: true,
        singleSelect:true
    });//Ext.grid.Panel
    grid_page.getSelectionModel().on('selectionchange', function(selModel, records)
    {
        if (records[0]) 
        {
            document.getElementById("txtAPMac").value = records[0].get('mac');
            document.getElementById("txtAPPos").value = records[0].get('pos');

            var btnPageAdd = document.getElementById("btnPageAdd");
            var btnPageObj1 = document.getElementById("btnPageObj1");
            var btnPageObj2 = document.getElementById("btnPageObj2");
            //document.getElementById("txtAPPos").disabled = true;
            btnPageObj1.style.display = "";
            btnPageObj2.style.display = "";
            btnPageAdd.style.display = "";
            btnPageObj1.value = "ÐÞ¸Ä";
            btnPageObj2.value = "É¾³ý";
        }
    });  
});//onReady 

function ResetInput()
{
    var btnPageAdd = document.getElementById("btnPageAdd");
    var btnPageObj1 = document.getElementById("btnPageObj1");
    var btnPageObj2 = document.getElementById("btnPageObj2");
    document.getElementById("txtAPMac").value = "";
    document.getElementById("txtAPPos").value = "";
		document.getElementById("txtAPMac").disabled = true;
    document.getElementById("txtAPPos").disabled = true;
    btnPageObj1.style.display = "none";
    btnPageObj2.style.display = "none";
    btnPageAdd.style.display = "";
	
}
function onLoad()
{
    ResetInput();
    store_ap_info.load(
    {
        params:
        {
           status: 'l',
           mac: '',
           pos:'',
        }
    });
}