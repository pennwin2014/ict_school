
Ext.define('MyDesktop.DwzWindow', {
    extend: 'Ext.ux.desktop.Module',

   

    id:'dwz-win',

    init : function(){
        this.launcher = {
            text: '单位组管理',
            iconCls:'icon-grid'
        };
    },

    createWindow : function(){
        var desktop = this.app.getDesktop();
        var win = desktop.getWindow('dwz-win');
         var htmlvar='<iframe src="/v4/windows/main_dwzgl.htm" style="width:1000px;height:950px;display:block;" FrameBorder=0 scrolling="no">';
        if(!win){
            win = desktop.createWindow({
                id: 'zxl-win',
                title:'单位组管理',
                width:1024,
                height:700,
                autoScroll: true,
                iconCls: 'icon-grid',
                animCollapse:false,
                constrainHeader:true,
    //            layout: 'fit',
                 html:htmlvar

            });
        }
        return win;
    }
});

