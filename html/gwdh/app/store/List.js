Ext.define('dennisShoppingGuiding.store.List', {
    extend: 'Ext.data.Store',
    pageSize: 40,
    model: 'dennisShoppingGuiding.model.List',
    remoteSort: true,
    proxy: {
        type: 'ajax',
        url : '/pronline/Msg?FunName@ncmShoppingGuiding',
        //getMethod: function(){ return 'POST'; },
        reader: {
          type: 'json',
          root: 'eimdata',
          totalProperty: 'totalCount'
        },
        simpleSortMode: true
    },
    sorters: {
        property: 'sid',
        direction: 'DESC'
    }
});

