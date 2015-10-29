
Ext.define('MyDesktop.App', {
    extend: 'Ext.ux.desktop.App',

    requires: [
        'Ext.window.MessageBox',
        'Ext.ux.desktop.ShortcutModel',
        'MyDesktop.DwglWindow',
        'MyDesktop.GdrzWindow',
        'MyDesktop.ZxlWindow',
        'MyDesktop.DwzWindow',
        'MyDesktop.BkglWindow',
        'MyDesktop.BkgjWindow',
        'MyDesktop.ZxdwWindow',
        'MyDesktop.Settings'
    ],

    init: function() {
        // custom logic before getXYZ methods get called...

        this.callParent();

        // now ready...
    },

    getModules : function(){
        return [   
            new MyDesktop.DwglWindow(),
            new MyDesktop.GdrzWindow(),
            new MyDesktop.ZxlWindow(),
            new MyDesktop.ZxdwWindow(),
            new MyDesktop.BkgjWindow(),
            new MyDesktop.BkglWindow(),
            new MyDesktop.DwzWindow()
           
        ];
    },

    getDesktopConfig: function () {
        var me = this, ret = me.callParent();

        return Ext.apply(ret, {
            //cls: 'ux-desktop-black',

            contextMenuItems: [
                { text: 'Change Settings', handler: me.onSettings, scope: me }
            ],

            shortcuts: Ext.create('Ext.data.Store', {
                model: 'Ext.ux.desktop.ShortcutModel',
                data: [
                  
                    { name: '归档日志', iconCls: 'grid-shortcut', module: 'gdrz-win' },
                    { name: '单位管理', iconCls: 'accordion-shortcut', module: 'dwgl-win' },
                    { name: '在线率统计', iconCls: 'cpu-shortcut', module: 'zxl-win' },
                    { name: '布控管理', iconCls: 'notepad-shortcut', module: 'bkgl-win' },
                    { name: '布控告警查询', iconCls: 'grid-shortcut', module: 'bkgj-win' },
                    { name: '单位组管理', iconCls: 'grid-shortcut', module: 'dwz-win' },
                    { name: '在线单位', iconCls: 'grid-shortcut', module: 'zxdw-win' },
                    { name: '系统用户', iconCls: 'grid-shortcut', module: 'xtyh-win' },
                    { name: '角色管理', iconCls: 'grid-shortcut', module: 'jsgl-win' }
                ]
            }),

            wallpaper: '/newver/resources/windows/wallpapers/Blue-Sencha.jpg',
            wallpaperStretch: false
        });
    },

    // config for the start menu
    getStartConfig : function() {
        var me = this, ret = me.callParent();

        return Ext.apply(ret, {
            title: '系统管理员',
            iconCls: 'user',
            height: 300,
            toolConfig: {
                width: 100,
                items: [
                    {
                        text:'设置',
                        iconCls:'settings',
                        handler: me.onSettings,
                        scope: me
                    },
                    '-',
                    {
                        text:'退出',
                        iconCls:'logout',
                        handler: me.onLogout,
                        scope: me
                    }
                ]
            }
        });
    },

    getTaskbarConfig: function () {
        var ret = this.callParent();

        return Ext.apply(ret, {
            quickStart: [
               
                { name: '单位管理', iconCls: 'accordion', module: 'dwgl-win' }
            ],
            trayItems: [
                { xtype: 'trayclock', flex: 1 }
            ]
        });
    },

    onLogout: function () {
        Ext.Msg.confirm('退出', '您确认要退出系统吗?');
    },

    onSettings: function () {
        var dlg = new MyDesktop.Settings({
            desktop: this.desktop
        });
        dlg.show();
    }
});
