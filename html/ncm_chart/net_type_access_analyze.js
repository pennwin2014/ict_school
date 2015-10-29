var store_net_type_access = Ext.create('Ext.data.Store', 
{
    id:'store_ap_info',
    fields: ['id', 'type' , 'num' , 'rate'],
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


function Refresh()
{
		store_net_type_access.load
		(
		{
			
			
			
		
		}
		);
}