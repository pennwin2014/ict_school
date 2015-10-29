/**¶¨Òå¿Õ°×Àà*/
Ext.define('firstpage.ncFirstBlank',{
	extend: 'Ext.panel.Panel',
    initComponent : function(){ 
    	 Ext.apply(this, {
    	 	hidden:true
    	});
    	 	
    this.callParent(arguments);
    }
})